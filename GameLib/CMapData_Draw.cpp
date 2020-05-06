//###################################################################
//			マップデータの中身に関するクラス定義
//###################################################################
#include "Header.h"


//####################  イベントとタイルを一括描画  ####################
void CSceneMap::CMapData::DrawMap(int tileAnimLoopFrame) {
	//前準備
	bool skip = false;
	bool playerTopHidden = false;
	vector<tuple<CTileData*, CPoint>> SemiTopTiles;
	vector<tuple<CTileData*, CPoint>> TopTiles;
	vector<tuple<int, CPoint>> MiddleEVs;
	vector<tuple<int, CPoint>> HighEVs;
	vector<tuple<int, CPoint>> PrimaryEVs;			//上半身も隠されるグループ
	vector<tuple<int, CPoint>> SecondaryEVs;		//上半身は後で描画するグループ

	this->setPlayerViewPixelPos();
	if(tileAnimLoopFrame <= 0) {
		tileAnimLoopFrame = 1;		//不正な引数は修正する
	}

	int left = -this->MapPixelOffset.X / this->TileSize;
	int top = -this->MapPixelOffset.Y / this->TileSize;
	CRectangle drawRange(left - 1, top - 1, Game.WindowSize.Width / this->TileSize + 2, Game.WindowSize.Height / this->TileSize + 2 + 1);
	CMapEventData* ev;
	CMapEventData::CMapEventPageData* pg;
	CPoint tempPos;

	// 1. 遠景
	this->DrawBack();

	// 2. 描画優先度：通常 のタイル
#pragma region NormalTiles
	TS_NOBLEND;
	for(int layer = static_cast<int>(TS::Map::Layer::Low); layer <= static_cast<int>(TS::Map::Layer::High); layer++) {
		for(int x = drawRange.Left, m = -offset; m < drawRange.Width; x++, m++) {
			if(!this->checkTileRangeX(x)) {
				continue;		//マップ外のためスキップ
			}

			for(int y = drawRange.Top, n = -offset; n < drawRange.Height; y++, n++) {
				if(!this->checkTileRangeY(y)) {
					continue;		//マップ外のためスキップ
				}

				//パレット位置を取得
				CPoint tilePos = this->Data->at(layer)[x][y].PalletPosition;
				auto& palTile = *Game.DB.getFDBTileset()->GetTilePallet(this->Data->at(layer)[x][y].PalletPosition.X, this->Data->at(layer)[x][y].PalletPosition.Y);
				if(&palTile == nullptr || tilePos.IsEmpty()) {
					//不正なパレット位置 / 空白タイルは処理しない
					continue;
				}

				//後回しにするタイルをここで仕分けする
				skip = false;
				switch((TS::Map::TileDrawPriority)palTile.Priority) {
					case TS::Map::TileDrawPriority::SemiTopMost:
						skip = true;
						SemiTopTiles.push_back(make_tuple(&this->Data->at(layer)[x][y], CPoint(m, n)));
						break;

					case TS::Map::TileDrawPriority::TopMost:
						skip = true;
						TopTiles.push_back(make_tuple(&this->Data->at(layer)[x][y], CPoint(m, n)));
						break;
				}
				if(skip) {
					continue;		//後回しにする
				}

				//実際に描画する
				this->drawTileOne(m, n, 0, 0, this->Data->at(layer)[x][y], tileAnimLoopFrame);
			}
		}
	}
#pragma endregion

	// 3. 主人公より下のイベントの下半身
#pragma region LowEV_Bottom
	for(int i = 0; i < this->EVs.GetCount(); i++) {
		//表示されており、描画領域内のイベントのみ描画する
		ev = this->EVs.GetFromIndex(i);
		tempPos = ev->Position;

		//座標を一時的にループ対応にする
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			if(drawRange.Left < 0 && this->MapSize.Width + drawRange.Left < tempPos.X) {
				tempPos.X -= this->MapSize.Width;
			} else if(this->MapSize.Width <= drawRange.Right() && tempPos.X <= drawRange.Right() - this->MapSize.Width - 1) {
				tempPos.X += this->MapSize.Width;
			}
		}
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			if(drawRange.Top < 0 && this->MapSize.Height + drawRange.Top < tempPos.Y) {
				tempPos.Y -= this->MapSize.Height;
			} else if(this->MapSize.Height <= drawRange.Bottom() && tempPos.Y <= drawRange.Bottom() - this->MapSize.Height + 1) {
				tempPos.Y += this->MapSize.Height;
			}
		}

		if(ev->GetPageIndex() != -1 && ev->Hide == false && drawRange.PointContains(tempPos)) {
			//後回しにするイベントをここで仕分けする
			pg = ev->getCurrentPage();
			skip = false;
			switch((TS::Map::EventDrawPriority)pg->DrawPriority) {
				case TS::Map::EventDrawPriority::Middle:
					skip = true;
					MiddleEVs.push_back(make_tuple(i, tempPos));
					break;
				case TS::Map::EventDrawPriority::High:
					skip = true;
					HighEVs.push_back(make_tuple(i, tempPos));
					break;
			}
			if(skip) {
				continue;		//後回しにする
			}

			//SemiTransparentタイルがある場合は下半身を透明で描画する
			if(!this->checkTilePriority(ev, static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent))) {
				TS_NOBLEND;
			} else {
				TS_ALPHABLEND(TS::Map::SemiTransparent);
			}

			//タイルグラフィックでない場合は上半身を次のループで描画する
			if(!pg->IsTilesetGraphics) {
				PrimaryEVs.push_back(make_tuple(i, tempPos));
			}

			//実際に描画する
			this->drawEVOne(tileAnimLoopFrame, drawRange, ev, tempPos, pg, true);
		}
	}
#pragma endregion

	// 4. 主人公より下のイベントの上半身
#pragma region LowEV_Top
	for(auto& evTuple : PrimaryEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 5. 主人公と同じのイベントの下半身
#pragma region NormalEV_Bottom
	PrimaryEVs.clear();
	for(auto& evTuple : MiddleEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		pg = ev->getCurrentPage();

		//配置座標にSemiタイルがあるか否かで振り分ける: 先に描く / 後に描く
		if(this->checkIsGraphTopPrimary(ev->Position.X, ev->Position.Y)) {
			PrimaryEVs.push_back(evTuple);
		} else {
			SecondaryEVs.push_back(evTuple);
		}

		//SemiTransparentタイルがある場合は下半身を透明で描画する
		if(!this->checkTilePriority(ev, static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent))) {
			TS_NOBLEND;
		} else {
			TS_ALPHABLEND(TS::Map::SemiTransparent);
		}

		//いずれにせよ下半身はここで描画する
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), pg, true);
	}
#pragma endregion

	// 6. 主人公の下半身
#pragma region PlayerBottom
	playerTopHidden = false;
	if(this->Player.MapObject.Hide == false
	   && 0 <= this->Player.MapObject.Position.X
	   && 0 <= this->Player.MapObject.Position.Y
	   && this->Player.MapObject.Position.X < this->MapSize.Width
	   && this->Player.MapObject.Position.Y < this->MapSize.Height) {

		//上半身が隠されているかどうかを検査する
		if(this->checkIsGraphTopPrimary(this->Player.MapObject.Position.X, this->Player.MapObject.Position.Y)) {
			playerTopHidden = true;
		}

		//SemiTransparentタイルがある場合は下半身を透明で描画する
		if(!this->checkTilePriority(&this->Player.MapObject, static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent))) {
			TS_NOBLEND;
		} else {
			TS_ALPHABLEND(TS::Map::SemiTransparent);
		}

		//実際に描画する
		this->drawPlayer(tileAnimLoopFrame, true);
	}
#pragma endregion

	// 7. 影層: 常に下半身だけを隠す
#pragma region Shadow
	TS_ALPHABLEND(this->ShadowTransparent);
	for(int x = drawRange.Left, m = -offset; m < drawRange.Width; x++, m++) {
		if(!this->checkTileRangeX(x)) {
			continue;		//マップ外のためスキップ
		}

		for(int y = drawRange.Top, n = -offset; n < drawRange.Height; y++, n++) {
			if(!this->checkTileRangeY(y)) {
				continue;		//マップ外のためスキップ
			}

			//実際に描画する
			TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
				if(this->Data->at(static_cast<int>(TS::Map::Layer::Shadow))[x][y].Quarter[q] == TRUE) {
					Game.DXLib.DrawBox(
						m * this->TileSize + (q % 2) * (this->TileSize / 2) + this->MapPixelOffset.X % this->TileSize,
						n * this->TileSize + (q / 2) * (this->TileSize / 2) + this->MapPixelOffset.Y % this->TileSize,
						m * this->TileSize + (q % 2) * (this->TileSize / 2) + (this->TileSize / 2) + this->MapPixelOffset.X % this->TileSize,
						n * this->TileSize + (q / 2) * (this->TileSize / 2) + (this->TileSize / 2) + this->MapPixelOffset.Y % this->TileSize,
						BLACK
						);
				}
			}
		}
	}
	TS_NOBLEND;
#pragma endregion

	// 8. 主人公と同じのイベントの上半身: Ａパターン [SemiTopタイルで隠される場合は先に描画する]
#pragma region NormalEV_Top1
	for(auto& evTuple : PrimaryEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 9. 主人公の上半身: Ａパターン [SemiTopタイルで隠される場合は先に描画する]
#pragma region PlayerTop1
	if(playerTopHidden && this->Player.MapObject.Hide == false
	   && 0 <= this->Player.MapObject.Position.X
	   && 0 <= this->Player.MapObject.Position.Y
	   && this->Player.MapObject.Position.X < this->MapSize.Width
	   && this->Player.MapObject.Position.Y < this->MapSize.Height) {
		this->drawPlayer(tileAnimLoopFrame, false);
	}
#pragma endregion

	// 10. SemiTopMostタイル
#pragma region SemiTopTile
	TS_COUNTLOOP(i, SemiTopTiles.size()) {
		this->drawTileOne(std::get<1>(SemiTopTiles[i]).X, std::get<1>(SemiTopTiles[i]).Y, 0, 0, *std::get<0>(SemiTopTiles[i]), tileAnimLoopFrame);
	}
#pragma endregion

	// 11. 主人公と同じのイベント上半身: Ｂパターン [SemiTopタイルで隠されないものはここで描画する]
#pragma region NormalEV_Top2
	for(auto& evTuple : SecondaryEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 12. 主人公の上半身: Ｂパターン [SemiTopタイルで隠されない場合はここで描画する]
#pragma region PlayerTop2
	if(playerTopHidden == false && this->Player.MapObject.Hide == false
	   && 0 <= this->Player.MapObject.Position.X
	   && 0 <= this->Player.MapObject.Position.Y
	   && this->Player.MapObject.Position.X < this->MapSize.Width
	   && this->Player.MapObject.Position.Y < this->MapSize.Height) {
		this->drawPlayer(tileAnimLoopFrame, false);
	}
#pragma endregion

	// 13. 主人公より上のイベント全身
#pragma region HighEV
	//下半身
	for(auto& evTuple : HighEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), true);
	}

	//上半身
	for(auto& evTuple : HighEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 14. 描画優先度：最前面 のタイル
#pragma region TopTile
	TS_COUNTLOOP(i, TopTiles.size()) {
		this->drawTileOne(std::get<1>(TopTiles[i]).X, std::get<1>(TopTiles[i]).Y, 0, 0, *std::get<0>(TopTiles[i]), tileAnimLoopFrame);
	}
#pragma endregion

	// 15. フォグ
	this->DrawFog();

	// デバッグ出力
	//Game.DXLib.DrawString(300, 30, -1, WHITE, Game.Format("drawRange (%d, %d, %d, %d)", drawRange.Left, drawRange.Top, drawRange.Right(), drawRange.Bottom()).c_str());
}


//####################  遠景を読み込む  ####################
void CSceneMap::CMapData::LoadBack(string fileName) {
	Game.Material.getGraphics()->Delete(this->BackHandle);
	this->BackHandle = Game.Material.getGraphics()->Add(fileName);
	this->CurrentMapBackFileName = fileName;
}


//####################  遠景を描画する  ####################
void CSceneMap::CMapData::DrawBack() {
	this->drawMapPicture(this->BackHandle, this->BackScrollType, this->PictureScrollCounter, -1, -1);
}


//####################  フォグを読み込む  ####################
void CSceneMap::CMapData::LoadFog(string fileName) {
	Game.Material.getGraphics()->Delete(this->FogHandle);
	this->FogHandle = Game.Material.getGraphics()->Add(fileName);
	this->CurrentFogFileName = fileName;
}


//####################  フォグを描画する  ####################
//システムDBの設定項目は、フォグ"透明度"であるため、255から差し引く操作を行っている
void CSceneMap::CMapData::DrawFog() {
	this->drawMapPicture(this->FogHandle, this->FogScrollType, this->PictureScrollCounter, this->FogBlend, 255 - this->FogTransparent);
}


//####################  遠景/フォグを実際に描画する  ####################
void CSceneMap::CMapData::drawMapPicture(int handle, int scrollType, CPoint& scrollCounter, int blendMode, int blendParam) {
	CPoint increment;
	CSize picSize;
	DxLib::GetGraphSize(handle, &picSize.Width, &picSize.Height);
	if(picSize.Width == 0 || picSize.Height == 0) {
		return;		//描画できない
	}

	//スクロール方法に応じて移動カウンターを加算セットして描画基準点を算出する
	switch((TS::Map::ScrollType)scrollType) {
		case TS::Map::ScrollType::None:
			//スクロールしない
			increment.Set(0, 0);
			break;

		case TS::Map::ScrollType::FollowChar:
			//移動に合わせてスクロール
			increment.Set(scrollCounter.X % picSize.Width, scrollCounter.Y % picSize.Height);
			break;

		case TS::Map::ScrollType::Down:
			increment.Set(0, this->animCnt % picSize.Height);
			break;

		case TS::Map::ScrollType::Left:
			increment.Set(-this->animCnt % picSize.Width, 0);
			break;

		case TS::Map::ScrollType::LeftDown:
			increment.Set(-this->animCnt % picSize.Width, this->animCnt % picSize.Height);
			break;

		case TS::Map::ScrollType::LeftUp:
			increment.Set(-this->animCnt % picSize.Width, -this->animCnt % picSize.Height);
			break;

		case TS::Map::ScrollType::Right:
			increment.Set(this->animCnt % picSize.Width, 0);
			break;

		case TS::Map::ScrollType::RightDown:
			increment.Set(this->animCnt % picSize.Width, this->animCnt % picSize.Height);
			break;

		case TS::Map::ScrollType::RightUp:
			increment.Set(this->animCnt % picSize.Width, -this->animCnt % picSize.Height);
			break;

		case TS::Map::ScrollType::Up:
			increment.Set(0, -this->animCnt % picSize.Height);
			break;
	}

	//3x3=合計９枚描画する
	if(blendMode == -1) {
		TS_NOBLEND;
	} else {
		DxLib::SetDrawBlendMode(Game.DXLib.ConvertToDXBlend(blendMode), blendParam);
	}
	for(int x = -1; x <= +1; x++) {
		for(int y = -1; y <= +1; y++) {
			Game.DXLib.DrawGraph(handle, increment.X + picSize.Width * x, increment.Y + picSize.Height * y);
			//デバッグ用: Game.DXLib.DrawRect(increment.X + picSize.Width * x, increment.Y + picSize.Height * y, picSize.Width, picSize.Height, RED, false);
		}
	}
	TS_NOBLEND;
}


//####################  主人公のマップ上におけるピクセル座標をセットする  ####################
void CSceneMap::CMapData::setPlayerViewPixelPos() {
	//主人公位置を確認: 画面端にいる場合は主人公自体を動かし、それ以外はマップのオフセットで動かす、ただしマップのループが有効な軸は常にマップのオフセットで動かす
	bool loopX = (this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both));
	bool loopY = (this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both));

	if(loopX == false && this->Player.MapObject.Position.X * this->TileSize + this->Player.MapObject.MoveCounter.X < Game.WindowSize.Width / 2) {
		//左端に寄っている
		this->MapPixelOffset.X = 0;
		this->playerViewPixelPos.X = this->Player.MapObject.Position.X * this->TileSize + this->Player.MapObject.MoveCounter.X;
	} else if(loopX == false && this->MapSize.Width * this->TileSize - (Game.WindowSize.Width / 2) < this->Player.MapObject.Position.X * this->TileSize + this->Player.MapObject.MoveCounter.X) {
		//右端に寄っている
		this->MapPixelOffset.X = (Game.WindowSize.Width - this->MapSize.Width * this->TileSize);
		this->playerViewPixelPos.X = (this->Player.MapObject.Position.X - (this->MapSize.Width - Game.WindowSize.Width / this->TileSize)) * this->TileSize + this->Player.MapObject.MoveCounter.X;
	} else {
		//主人公中央表示
		this->MapPixelOffset.X = -(this->Player.MapObject.Position.X * this->TileSize) + Game.WindowSize.Width / 2 - this->Player.MapObject.MoveCounter.X;
		this->playerViewPixelPos.X = Game.WindowSize.Width / 2;
	}

	if(loopY == false && this->Player.MapObject.Position.Y * this->TileSize + this->Player.MapObject.MoveCounter.Y <= Game.WindowSize.Height / 2) {
		//上端に寄っている
		this->MapPixelOffset.Y = 0;
		this->playerViewPixelPos.Y = this->Player.MapObject.Position.Y * this->TileSize + this->Player.MapObject.MoveCounter.Y - this->TileSize;
	} else if(loopY == false && this->MapSize.Height * this->TileSize - (Game.WindowSize.Height / 2) <= this->Player.MapObject.Position.Y * this->TileSize + this->Player.MapObject.MoveCounter.Y) {
		//下端に寄っている
		this->MapPixelOffset.Y = (Game.WindowSize.Height - this->MapSize.Height * this->TileSize);
		this->playerViewPixelPos.Y = (this->Player.MapObject.Position.Y - (this->MapSize.Height - Game.WindowSize.Height / this->TileSize)) * this->TileSize + this->Player.MapObject.MoveCounter.Y - this->TileSize;
	} else {
		//主人公中央表示
		this->MapPixelOffset.Y = -(this->Player.MapObject.Position.Y * this->TileSize) + Game.WindowSize.Height / 2 - this->Player.MapObject.MoveCounter.Y;
		this->playerViewPixelPos.Y = Game.WindowSize.Height / 2 - this->TileSize;
	}
}


//####################  単一レイヤータイルを描画する  ####################
void CSceneMap::CMapData::drawTileOne(int x, int y, int offsetX, int offsetY, const CTileData& palTile, int tileAnimLoopFrame) {
	//アニメーションIDを算出する
	auto& palData = *Game.DB.getFDBTileset()->GetTilePallet(palTile.PalletPosition.X, palTile.PalletPosition.Y);
	if(&palData == nullptr || (palTile.PalletPosition.X == 0 && palTile.PalletPosition.Y == 0)) {
		//不正なパレット位置 / 空白タイルは処理しない
		return;
	}

	int animIndex = (palData.AnimPatternCount <= 0) ? 0 : (this->animCnt % tileAnimLoopFrame) / (tileAnimLoopFrame / palData.AnimPatternCount);

	if(palTile.PalletPosition.Y >= TS::Map::AutoTileRowLength) {
		//単一タイルを描画
		Game.DXLib.DrawRectGraph(
			this->TilePallet->TilesetHandle,
			x * this->TileSize + this->MapPixelOffset.X % this->TileSize + offsetX,
			y * this->TileSize + this->MapPixelOffset.Y % this->TileSize + offsetY,
			palTile.PalletPosition.X * this->TileSize,
			palTile.PalletPosition.Y * this->TileSize,
			this->TileSize, this->TileSize
			);
	} else {
		//オートタイル用の1/4描画
		TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
			Game.DXLib.DrawRectGraph(
				this->TilePallet->AutoTiles[palTile.PalletPosition.X + palTile.PalletPosition.Y * TS::Map::TileColumnCount],
				x * this->TileSize + (q % 2) * (this->TileSize / 2) + this->MapPixelOffset.X % this->TileSize + offsetX,
				y * this->TileSize + (q / 2) * (this->TileSize / 2) + this->MapPixelOffset.Y % this->TileSize + offsetY,
				(q % 2) * (this->TileSize / 2) + animIndex * this->TileSize,
				(q / 2) * (this->TileSize / 2) + palTile.Quarter[q] * this->TileSize,
				this->TileSize / 2, this->TileSize / 2
				);
		}
	}
}


//####################  単一イベントを描画  ####################
void CSceneMap::CMapData::drawEVOne(int tileAnimLoopFrame, const CRectangle& drawRange, CMapEventData* ev, CPoint& pos, CMapEventData::CMapEventPageData* pg, bool isBottom) {
	if(!pg->IsTilesetGraphics) {
		if(isBottom) {
			//下半身 (下からタイルサイズ分) を描画
			Game.DXLib.DrawRectGraph(
				pg->GraphHandle,
				(pos.X - drawRange.Left - offset) * this->TileSize + this->MapPixelOffset.X % this->TileSize + ev->MoveCounter.X,
				(pos.Y - drawRange.Top - offset) * this->TileSize + this->MapPixelOffset.Y % this->TileSize + ev->MoveCounter.Y,
				pg->OneCharSize.Width * pg->AnimID,
				pg->OneCharSize.Height * (pg->getDirection() + 1) - this->TileSize,
				pg->OneCharSize.Width, this->TileSize
				);
		} else {
			//上半身を描画
			ev->PositionOnPixel.Set(
				(pos.X - drawRange.Left - offset) * this->TileSize + this->MapPixelOffset.X % this->TileSize + ev->MoveCounter.X,
				(pos.Y - drawRange.Top - offset) * this->TileSize + this->MapPixelOffset.Y % this->TileSize + ev->MoveCounter.Y
				);
			Game.DXLib.DrawRectGraph(
				pg->GraphHandle,
				ev->PositionOnPixel.X,
				ev->PositionOnPixel.Y - (pg->OneCharSize.Height - this->TileSize),
				pg->OneCharSize.Width * pg->AnimID,
				pg->OneCharSize.Height * pg->getDirection(),
				pg->OneCharSize.Width, pg->OneCharSize.Height - this->TileSize
				);
		}
	} else if(isBottom) {
		//そのままグラフィックを描画
		const auto& palTile = *Game.DB.getFDBTileset()->GetTilePallet(pg->TilePalletPosition.X, pg->TilePalletPosition.Y);
		if(&palTile == nullptr || pg->TilePalletPosition.IsEmpty()) {
			//不正なパレット位置 / 空白タイルは処理しない
			return;
		}

		//実際に描画
		CTileData tileData;
		tileData.PalletPosition = pg->TilePalletPosition;
		ev->PositionOnPixel.Set(
			(pos.X - drawRange.Left - offset) * this->TileSize + this->MapPixelOffset.X % this->TileSize + ev->MoveCounter.X,
			(pos.Y - drawRange.Top - offset) * this->TileSize + this->MapPixelOffset.Y % this->TileSize + ev->MoveCounter.Y
			);
		this->drawTileOne(pos.X - drawRange.Left - offset, pos.Y - drawRange.Top - offset, ev->MoveCounter.X, ev->MoveCounter.Y, tileData, tileAnimLoopFrame);
	}
	TS_NOBLEND;
}


//####################  プレイヤーを描画  ####################
void CSceneMap::CMapData::drawPlayer(int tileAnimLoopFrame, bool isBottom) {
	if(isBottom) {
		//下半身を描画
		Game.DXLib.DrawRectGraph(
			this->Player.CharData.GraphHandle,
			this->playerViewPixelPos.X,
			this->playerViewPixelPos.Y + this->TileSize,
			this->Player.CharData.OneCharSize.Width * this->Player.CharData.AnimID,
			this->Player.CharData.OneCharSize.Height * (this->Player.CharData.getDirection() + 1) - this->TileSize,
			this->Player.CharData.OneCharSize.Width, this->TileSize
			);
	} else {
		//上半身を描画
		this->Player.MapObject.PositionOnPixel.Set(
			this->playerViewPixelPos.X,
			this->playerViewPixelPos.Y + this->TileSize - (this->Player.CharData.OneCharSize.Height - this->TileSize)
			);
		Game.DXLib.DrawRectGraph(
			this->Player.CharData.GraphHandle,
			this->Player.MapObject.PositionOnPixel.X, this->Player.MapObject.PositionOnPixel.Y,
			this->Player.CharData.OneCharSize.Width * this->Player.CharData.AnimID,
			this->Player.CharData.OneCharSize.Height * this->Player.CharData.getDirection(),
			this->Player.CharData.OneCharSize.Width, this->Player.CharData.OneCharSize.Height - this->TileSize
			);
	}
	TS_NOBLEND;
}


//####################  指定した座標に指定した描画優先度のタイルが存在するか調べる  ####################
bool CSceneMap::CMapData::checkTilePriority(CMapObjectData* ev, int priority) {
	for(int layer = static_cast<int>(TS::Map::Layer::Low); layer <= static_cast<int>(TS::Map::Layer::High); layer++) {
		CPoint tilePos = this->Data->at(layer)[ev->Position.X][ev->Position.Y].PalletPosition;
		auto& palTile = *Game.DB.getFDBTileset()->GetTilePallet(tilePos.X, tilePos.Y);
		if(&palTile != nullptr && !tilePos.IsEmpty()) {
			if(palTile.Priority == static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent)) {
				return true;
			}
		}
	}
	return false;
}


//####################  指定したマップ座標がマップの中にあるかどうかを検査する  ####################
//ループ設定が有効である場合は座標が調整される
bool CSceneMap::CMapData::checkTileRangeX(int& x) {
	if(x < 0) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			x = this->MapSize.Width + x;
		} else {
			return false;		//範囲外
		}
	} else if(this->MapSize.Width <= x) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			x -= this->MapSize.Width;
		} else {
			return false;		//範囲外
		}
	}
	return true;
}
bool CSceneMap::CMapData::checkTileRangeY(int& y) {
	if(y < 0) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			y = this->MapSize.Height + y;
		} else {
			return false;		//範囲外
		}
	} else if(this->MapSize.Height <= y) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			y -= this->MapSize.Height;
		} else {
			return false;		//範囲外
		}
	}
	return true;
}


//####################  通常描画のイベント/主人公の上半身が影やSemiTopタイルに隠れるかどうかを検査する  ####################
bool CSceneMap::CMapData::checkIsGraphTopPrimary(int tx, int ty) {
	for(int l = static_cast<int>(TS::Map::Layer::Low); l <= static_cast<int>(TS::Map::Layer::High); l++) {
		auto tilePal = *Game.DB.getFDBTileset()->GetTilePallet(
			this->Data->at(l)[tx][ty].PalletPosition.X,
			this->Data->at(l)[tx][ty].PalletPosition.Y
			);

		//タイルの描画優先度を検査する
		switch((TS::Map::TileDrawPriority)tilePal.Priority) {
			case TS::Map::TileDrawPriority::SemiTopMost:
			case TS::Map::TileDrawPriority::SemiTransparent:
				return true;		//上半身を（影よりも）先に描く必要がある
		}
	}
	return false;
}
