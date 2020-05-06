//###################################################################
//			マップデータの中身に関するクラス定義
//###################################################################
#include "Header.h"


//####################  歩行グラフィックのキャラを読み込む  ####################
void CSceneMap::CMapData::CMapCharObjectData::LoadGraphics(string fileName) {
	//既にグラフィックが読まれている場合は破棄する
	if(!this->Graphics.empty()) {
		this->DisposeGraphics();
	}

	this->Graphics = fileName;
	this->GraphHandle = Game.Material.getGraphics()->Add(fileName);
	CSize graphSize;
	DxLib::GetGraphSize(this->GraphHandle, &graphSize.Width, &graphSize.Height);
	this->OneCharSize.Set(
		graphSize.Width / static_cast<int>(TS::Map::EventAnimation::Count),
		graphSize.Height / static_cast<int>(TS::Map::Direction4::None)
	);
}


//####################  歩行グラフィックのキャラのリソースを破棄する  ####################
void CSceneMap::CMapData::CMapCharObjectData::DisposeGraphics() {
	Game.Material.getGraphics()->Delete(this->GraphHandle);
	this->GraphHandle = -1;
	this->Graphics.clear();
}


//####################  コンストラクター  ####################
CSceneMap::CMapData::CMapData() {
	this->Data = new TileArray;			//空のタイル配列を生成
}


//####################  タイル定義ファイルを読み込む  ####################
void CSceneMap::CMapData::loadTileFile(string fileName, CMapData& dest, vector<string>& plain) {
	string textBuffer;
	vector<string> spl;
	vector<string> subspl;

	//与えられたファイルの圧縮を解き、テキストを行ごとに分解する
	if(fileName.empty()) {
		return;		//空のファイル名は処理しない

#ifdef _DEBUG
	} else if(!PathFileExists(fileName.c_str())) {
		//DXアーカイブ後は使用できない
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないタイル定義ファイルです。\n>%s", fileName.c_str());
		return;
	} else if(!CSceneMap::loadTextMap(fileName, textBuffer)) {
		//非圧縮形式のマップのロードに失敗した
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "マップの読み込みに失敗しました。\n->%s", fileName.c_str());
		return;
#else
	} else if(!CSceneMap::deflate(fileName, textBuffer)) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "マップの読み込みに失敗しました。\n->%s", fileName.c_str());
		return;		//解凍に失敗した
#endif
	}
	Game.SplitString(plain, textBuffer, "\r\n");
	dest.CurrentMapFileName = fileName;

	//ヘッダー情報を読み込む
	dest.CurrentMapName = Game.PopHead(plain);
	dest.GUID = Game.PopHead(plain);
	dest.MapSize.Set(Game.PopHead(plain));
	dest.MapLoopOption = atoi(Game.PopHead(plain).c_str());
	dest.TilesetID = atoi(Game.PopHead(plain).c_str());
	dest.MoveSpeed = atoi(Game.PopHead(plain).c_str());
	dest.Player.CharData.MoveSpeed = dest.MoveSpeed;
	dest.BaseMapFileName = Game.PopHead(plain);

	dest.BGMRawString = Game.PopHead(plain);
	dest.BGSRawString = Game.PopHead(plain);

	dest.CurrentMapBackFileName = Game.PopHead(plain);
	dest.BackScrollType = atoi(Game.PopHead(plain).c_str());
	dest.BackScrollSpeed = atoi(Game.PopHead(plain).c_str());
	dest.CurrentFogFileName = Game.PopHead(plain);
	dest.FogScrollType = atoi(Game.PopHead(plain).c_str());
	dest.FogScrollSpeed = atoi(Game.PopHead(plain).c_str());
	dest.FogBlend = atoi(Game.PopHead(plain).c_str());

	Game.SplitString(spl, Game.PopHead(plain), "|");
	dest.Units = CIntegerArray();
	for(auto& unitIDstr : spl) {
		dest.Units.Add(atoi(unitIDstr.c_str()));
	}

	dest.EncounterRate = atoi(Game.PopHead(plain).c_str());
	dest.BattleBackFileName = Game.PopHead(plain);

	Game.PopHead(plain);		//空行読み捨て

	//行ごとに各タイル情報を読み込む
	for(int layer = static_cast<int>(TS::Map::Layer::Low); layer < TS::Map::MapLayerCountWithoutEV; layer++) {
		//１行ずつ処理する
		TS_COUNTLOOP(y, dest.MapSize.Height) {
			textBuffer = Game.PopHead(plain);
			Game.SplitString(spl, textBuffer, "|");
			TS_COUNTLOOP(x, dest.MapSize.Width) {
				Game.SplitString(subspl, spl[x], ",");
				if(subspl.size() < static_cast<int>(TS::Map::QuarterTile::Count)) {
					continue;		//不正な形式
				}

				// 1/4タイル情報が先行する
				TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
					dest.Data->at(layer)[x][y].Quarter[q] = atoi(subspl[q].c_str());
				}

				//タイルレイヤーにはタイルパレット位置が含まれている
				if(subspl.size() > static_cast<int>(TS::Map::QuarterTile::Count)) {
					dest.Data->at(layer)[x][y].PalletPosition.Set(subspl[static_cast<int>(TS::Map::QuarterTile::Count)].c_str());
				}

				//マップ上の座標をセットする
				dest.Data->at(layer)[x][y].MapPosition.Set(x, y);
			}
		}
		Game.PopHead(plain);		//空行読み捨て
	}
	TS_LOG("タイル定義ファイルの読み込みが完了しました。");
}


//####################  破棄処理  ####################
//NOTE: MapDataはデストラクターが呼ばれない仕様のため、STL系のオブジェクトもすべて手動で破棄しなければならない。
void CSceneMap::CMapData::Dispose() {
	delete this->Data;

	//プレイヤーグラフィック破棄
	this->Player.CharData.DisposeGraphics();

	string().swap(this->GUID);
	string().swap(this->BaseMapFileName);
	string().swap(this->BattleBackFileName);
	string().swap(this->BGMRawString);
	string().swap(this->BGSRawString);
	string().swap(this->CurrentFogFileName);
	string().swap(this->CurrentMapBackFileName);
	string().swap(this->CurrentMapName);
	string().swap(this->CurrentMapFileName);
	string().swap(this->Player.MapObject.Name);
	string().swap(this->Player.CharData.Graphics);
	this->Units.Clear();

	vector<int>().swap(this->runningEVNest);
	vector<int>().swap(this->reservedEVs);
	vector<CMapEventData*>().swap(this->runningEVNest_system);

	//イベントページを先に削除
	for(auto& ev : this->EVs.Items) {
		ev.Dispose();
	}
	this->EVs.Clear();
}


//####################  マップを読み込む  ####################
void CSceneMap::CMapData::LoadMap(string fileName, bool exceptEV) {
	if(fileName.empty()) {
		return;		//空のファイル名は処理しない
	}

	TS_LOG("次のマップを読み込みます...\n>" + fileName);

	//指定されたマップのメタ情報・タイル情報を読み込む
	auto scene = Game.getSceneMap();
	vector<string> plain;
	CSceneMap::CMapData::loadTileFile(fileName, *this, plain);

	//カーボン元がある場合、そのマップのタイル情報も一時的に読み込む
	if(!this->BaseMapFileName.empty()) {
		TS_LOG("カーボン元のマップのタイル情報を読み込みます...\n>" + this->BaseMapFileName);

		CMapData baseMap;
		vector<string> subPlain;
		CSceneMap::CMapData::loadTileFile(this->BaseMapFileName, baseMap, subPlain);
		vector<string>().swap(subPlain);		//早めのリソース解放

		//現在のマップの中で空白タイルになっている部分をカーボン元のタイルに置き換える・影層はどちらか片方に付いているところだけを有効にする
		for(int l = static_cast<int>(TS::Map::Layer::Low); l < static_cast<int>(TS::Map::MapLayerCountWithoutEV); l++) {
			TS_COUNTLOOP(x, min(this->MapSize.Width, baseMap.MapSize.Width)) {
				TS_COUNTLOOP(y, min(this->MapSize.Height, baseMap.MapSize.Height)) {
					if(l == static_cast<int>(TS::Map::Layer::Shadow)) {
						//影層のときは 1/4 タイル情報の排他的論理和を取る
						TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
							this->Data->at(l)[x][y].Quarter[q] = this->Data->at(l)[x][y].Quarter[q] | baseMap.Data->at(l)[x][y].Quarter[q];
						}
					} else if(this->Data->at(l)[x][y].PalletPosition.IsEmpty()) {
						//タイルレイヤーのときはパレット座標と1/4タイル情報をコピー
						this->Data->at(l)[x][y].PalletPosition = baseMap.Data->at(l)[x][y].PalletPosition;
						TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
							this->Data->at(l)[x][y].Quarter[q] = baseMap.Data->at(l)[x][y].Quarter[q];
						}
					}
				}
			}
		}
	}

	//各種リソースを読み込み、タイルパレットを生成する
	TS_LOG("各種リソースを読み込み、タイルパレットを生成します。");
	this->LoadBack(this->CurrentMapBackFileName);
	this->LoadFog(this->CurrentFogFileName);
	this->TilePallet = Game.DB.getFDBTileset()->LoadTileset(this->TilesetID);

	//スクリプトを読み込む
	TS_LOG("マップのスクリプトを読み込みます。");
	this->EVClear();
	this->SQHandle = Game.SQ.Add(Game.Format(TS::FileNames::SQMapEvents, this->GUID.c_str()));

	//イベント配置処理を行う場合はスクリプトを呼び出す
	if(!exceptEV) {
		TS_LOG("イベントの配置処理を行います。");
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SetupMapEvents);

		//次のマップに該当する個別変数情報を取り出す
		TS_LOG("イベント個別変数を適用します。");
		CSceneMap::applyMapSelfValue();
	}

	TS_LOG("マップの読み込みが完了しました。");
}


//####################  マップを読み込んだ後に行うべき共通処理  ####################
void CSceneMap::CMapData::AfterLoadMap() {
	//プレイヤーオブジェクトに関する情報を再設定する
	this->setPlayerViewPixelPos();
	this->Player.MapObject.MoveCounter.Set(0, 0);

	//BGM・BGS再生
	auto scene = Game.getSceneMap();
	scene->BGM.Load(this->BGMRawString);
	scene->BGS.Load(this->BGSRawString);
	this->PlayBGMBGS();

	//イベントページ初回判定
	//マップ移動直後に実行すべきイベントがあれば、それを呼び出す
	for(auto& ev : this->EVs.Items) {
		//末尾ページから判定していく
		for(int pageIndex = ev.GetPageCount() - 1; pageIndex >= 0; pageIndex--) {
			if(!Game.getSceneMap()->MapData.RunEV(ev.ID, pageIndex, static_cast<int>(TS::Map::EventScriptType::Page), false)) {
				continue;
			}

			//有効なページが見つかったら、次は起動判定へ
			ev.SetPageIndex(pageIndex);			//ここで日常移動のスクリプトが呼び出される
			if(ev.getCurrentPage()->TriggerType != static_cast<int>(TS::Map::EventTriggerType::AfterMoveMap)) {
				//有効なページだが起動するタイミングがマップ移動直後ではない
				break;
			} else if(!this->RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Trigger), false)) {
				//有効なページでマップ移動直後の起動条件だが起動するタイミングとして判定されなかった
				break;
			}

			//実行し、非表示状態にする
			this->RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Run), true);
			ev.SetPageIndex(-1);
		}
	}

	//スクリプトのGC実行
	Game.SQ.CollectGarbage();
}


//####################  マップのBGM/BGSを再生する  ####################
void CSceneMap::CMapData::PlayBGMBGS() {
	//変更する必要がある場合のみ、BGMとBGSを再生する
	auto scene = Game.getSceneMap();
	if(!Game.BGMKeep) {
		if(scene->BGM.FileName.empty() == false && scene->BeforeBGMFileName != scene->BGM.FileName) {
			scene->BGM.Play(static_cast<int>(TS::Material::PlayType::Loop), false);
			scene->BeforeBGMFileName = scene->BGM.FileName;
		}
	}
	if(!Game.BGSKeep) {
		if(scene->BGS.FileName.empty() == false && scene->BeforeBGSFileName != scene->BGS.FileName) {
			scene->BGS.Play(static_cast<int>(TS::Material::PlayType::Loop), false);
			scene->BeforeBGSFileName = scene->BGS.FileName;
		}
	}
}


//####################  現在実行中のイベントを取得する  ####################
//ユーザー操作を停止しないイベントの実行もすべて含んで検索する
CSceneMap::CMapData::CMapEventData* CSceneMap::CMapData::getThisEVData() {
	if(this->runningEVNest_system.size() == 0) {
		//イベントを実行していない
		return nullptr;
	} else {
		return this->runningEVNest_system[0];
	}
}


//####################  ダミーイベントをすべて削除する  ####################
void CSceneMap::CMapData::ClearDummyEVs() {
	this->EVs.Items.erase(std::remove_if(TS_CONTAINERALL(this->EVs.Items), [&](CMapEventData& ev) {
		if(ev.IsDummy) {
			ev.Dispose();
			return true;
		}
		return false;
	}), this->EVs.Items.end());

	//NOTE: 各メンバーのポインタを再設定する（vector要素を削除した時点でその要素以降のメンバーに関するポインタ情報がすべて狂うため）
	for(auto &ev : this->EVs.Items) {
		ev.TargetCharPTR = ev.getCurrentPage();
	}
}


//####################  実行予約された最も古いイベントのIDを一つ抜き出す  ####################
int CSceneMap::CMapData::PopReserveEVID() {
	if(this->reservedEVs.size() == 0) {
		return -1;
	} else {
		const int temp = this->reservedEVs[0];
		this->reservedEVs.erase(this->reservedEVs.begin());
		return temp;
	}
}


//####################  現在のイベント情報をログ出力する  ####################
void CSceneMap::CMapData::PrintEVData() {
	TS_LOG("\nEVs:");
	for(auto& ev : this->EVs.Items) {
		TS_LOG(ev.toString());
	}
}


//####################  タイル情報を取得する  ####################
CPoint CSceneMap::CMapData::GetTile(int layer, int x, int y) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Event)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				return this->Data->at(layer)[x][y].PalletPosition;
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定されたマップタイル [Layer:%d, X:%d, Y:%d] が無効です。", layer, x, y);
	return CPoint();
}


//####################  タイル情報を設定する  ####################
//absoluteをtrueにすると、オートタイルが更新されなくなり、現在の接続状態を維持する
void CSceneMap::CMapData::SetTile(int layer, int x, int y, int tilex, int tiley, bool absolute) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Shadow)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				this->Data->at(layer)[x][y].PalletPosition.Set(tilex, tiley);
				if(!absolute) {
					this->updateAutoTile(layer, x, y, true);
				}
				return;
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定されたマップタイル [Layer:%d, X:%d, Y:%d] が無効です。", layer, x, y);
}


//####################  指定したタイル座標の 1/4 座標を取得する  ####################
int CSceneMap::CMapData::GetQuarterTile(int layer, int x, int y, int q) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Event)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				TS_CHECKINDEX(q, 0, static_cast<int>(TS::Map::QuarterTile::Count)) {
					return this->Data->at(layer)[x][y].Quarter[q];
				}
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定された 1/4マップタイル [Layer:%d, X:%d, Y:%d, Quarter:%d] が無効です。", layer, x, y, q);
	return -1;
}


//####################  指定したタイル座標の 1/4 座標を設定する  ####################
//影もしくはオートタイルの接続状態を変更するのに使う
void CSceneMap::CMapData::SetQuarterTile(int layer, int x, int y, int q, int val) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Event)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				TS_CHECKINDEX(q, 0, static_cast<int>(TS::Map::QuarterTile::Count)) {
					this->Data->at(layer)[x][y].Quarter[q] = val;
					return;
				}
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定された 1/4マップタイル [Layer:%d, X:%d, Y:%d, Quarter:%d] が無効です。", layer, x, y, q);
}


//####################  指定したタイル座標が歩けるかどうかを取得する  ####################
bool CSceneMap::CMapData::IsWalkableTile(int x, int y) {
	for(int l = static_cast<int>(TS::Map::Layer::High); l >= static_cast<int>(TS::Map::Layer::Low); l--) {
		auto temp = this->Data->at(l)[x][y].PalletPosition;
		if(temp.IsEmpty()) {
			//空白タイルは次のループへ
			continue;
		}
		auto tile = Game.DB.getFDBTileset()->GetTilePallet(temp.X, temp.Y);

		//下位層依存の場合は次のループへ
		switch(static_cast<TS::Map::HitType>(tile->Hit)) {
			case TS::Map::HitType::NG:
				return false;
			case TS::Map::HitType::OK:
				return true;
		}
	}

	//全層で空白タイルのときは歩けない
	return false;
}


//####################  指定した座標に指定したグループ番号のタイルがあるかどうかを調べる  ####################
//存在する場合は最初に見つかったレイヤーのインデックスを返し、存在しない場合は -1
int CSceneMap::CMapData::CheckTileGroupNum(int x, int y, int groupNum) {
	TS_CHECKINDEX(x, 0, this->MapSize.Width) {
		TS_CHECKINDEX(y, 0, this->MapSize.Height) {
			TS_COUNTLOOP(layer, static_cast<int>(TS::Map::Layer::Shadow)) {
				const auto tilePos = this->Data->at(layer)[x][y].PalletPosition;
				if(Game.DB.getFDBTileset()->GetTilePallet(tilePos.X, tilePos.Y)->Tag == groupNum) {
					return layer;
				}
			}
			return -1;
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定されたマップタイル [X:%d, Y:%d] が無効です。", x, y);
	return -1;
}


//####################  指定した座標のタイルのグループ番号を取得する  ####################
int CSceneMap::CMapData::GetTileGroupNum(int layer, int x, int y) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Shadow)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				const auto tilePos = this->Data->at(layer)[x][y].PalletPosition;
				return Game.DB.getFDBTileset()->GetTilePallet(tilePos.X, tilePos.Y)->Tag;
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定されたマップタイル [Layer:%d, X:%d, Y:%d] が無効です。", layer, x, y);
	return -1;
}


//####################  周辺のオートタイルを更新する  ####################
void CSceneMap::CMapData::updateAutoTile(int layer, int x, int y, bool isParent) {
	//指定されたタイルがオートタイルでない場合は処理しない
	if(layer >= static_cast<int>(TS::Map::Layer::Shadow) || this->Data->at(layer)[x][y].PalletPosition.Y >= TS::Map::AutoTileRowLength) {
		return;
	}

	int vx, vy, px, py, bx, by, cx, cy;
	CPoint ny, ax, ay, ac;
	CPoint palPos;
	TS::Map::QuarterTile part;
	const int tx = x;
	const int ty = y;

	//指定されたタイル座標についてセットする
	for (int y = 1; y <= 2; y++) {
		for (int x = 1; x <= 2; x++) {
			//探索用
			vx = (x % 2 == 1) ? -1 : 1;
			vy = (y % 2 == 1) ? -1 : 1;
			px = tx + (x == 0 ? -1 : (x == 3) ? 1 : 0);
			py = ty + (y == 0 ? -1 : (y == 3) ? 1 : 0);

			//周囲のデータ：比較用・範囲外の空間には比較基準と同じタイルがあるものとみなす
			palPos = this->Data->at(layer)[px][py].PalletPosition;
			ny = palPos;        //比較基準
			ax = (px + vx < 0 || this->MapSize.Width <= px + vx) ? palPos : this->Data->at(layer)[px + vx][py].PalletPosition;      //横の比較用
			ay = (py + vy < 0 || this->MapSize.Height <= py + vy) ? palPos : this->Data->at(layer)[px][py + vy].PalletPosition;     //縦の比較用
			ac = (px + vx < 0 || this->MapSize.Width <= px + vx || py + vy < 0 || this->MapSize.Height <= py + vy) ? palPos : this->Data->at(layer)[px + vx][py + vy].PalletPosition;   //角(斜め)の比較用
																																													//対象位置のパラメーター
			bx = (x % 2 == 0) ? 1 : 0;
			by = (y % 2 == 0) ? 1 : 0;
			cx = ((x == 0) ? -1 : (x == 3) ? 1 : 0);
			cy = ((y == 0) ? -1 : (y == 3) ? 1 : 0);

			//範囲外の部分は処理しない
			if (tx + cx < 0 || this->MapSize.Width <= tx + cx || ty + cy < 0 || this->MapSize.Height <= ty + cy) {
				continue;
			}
			palPos = this->Data->at(layer)[tx + cx][ty + cy].PalletPosition;
			part = static_cast<TS::Map::QuarterTile>((x - 1) + (y - 1) * 2);

			if(palPos.IsEmpty() == false && palPos.Y < TS::Map::AutoTileRowLength) {
				//オートタイルの場合のみ処理する
				if(ny == ax && ny == ay && ny == ac) {
					//縦・横・角がすべて同一タイル
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::AllSide);
				} else if(ny == ax && ny == ay) {
					//縦・横が同一タイル
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::FourSide);
				} else if(ny == ay) {
					//縦が同一タイル
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Vertical);
				} else if(ny == ax) {
					//横が同一タイル
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Horizontal);
				} else {
					//四方八方が異なるタイル：独立型
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Isolate);
				}
			} else {
				//オブジェクトタイルは常に独立型にする
				this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Isolate);
			}
		}
	}

	//基点である場合、さらにその八方にあるタイルの接している部分を更新する
	if(isParent) {
		for(int x = -1; x <= +1; x++) {
			for(int y = -1; y <= +1; y++) {
				if(x == 0 && y == 0) {
					continue;       //基点は処理しない
				} else if(tx + x < 0 || ty + y < 0 || this->MapSize.Width <= tx + x || this->MapSize.Height <= ty + y) {
					continue;       //範囲外は処理しない
				}
				this->updateAutoTile(layer, tx + x, ty + y, false);
			}
		}
	}
}


//####################  指定座標から１歩進んだ座標を取得  ####################
//マップの範囲外に出てしまった場合、X/Yの値がマイナスになる
//戻り値はループしたかどうか
bool CSceneMap::CMapData::GetOneMovePosition(CPoint& beforePos, CPoint& afterPos, int direction4) {
	bool looped = false;
	afterPos = beforePos;

	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::South:
			afterPos.Y++;
			break;
		case TS::Map::Direction4::West:
			afterPos.X--;
			break;
		case TS::Map::Direction4::East:
			afterPos.X++;
			break;
		case TS::Map::Direction4::North:
			afterPos.Y--;
			break;
	}

	//移動先の座標がマップの外になる場合、ループ設定を確認する
	//横方向
	if(afterPos.X < 0) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			//beforePos.X = this->MapSize.Width;
			afterPos.X = this->MapSize.Width - 1;
			looped = true;
		}
	} else if(this->MapSize.Width <= afterPos.X) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			//beforePos.X = -1;
			afterPos.X = 0;
			looped = true;
		} else {
			afterPos.X = -1;
		}
	}
	//縦方向
	if(afterPos.Y < 0) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			//beforePos.Y = this->MapSize.Height;
			afterPos.Y = this->MapSize.Height - 1;
			looped = true;
		}
	} else if(this->MapSize.Height <= afterPos.Y) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			//beforePos.Y = -1;
			afterPos.Y = 0;
			looped = true;
		} else {
			afterPos.Y = -1;
		}
	}
	return looped;
}
