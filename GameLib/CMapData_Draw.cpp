//###################################################################
//			�}�b�v�f�[�^�̒��g�Ɋւ���N���X��`
//###################################################################
#include "Header.h"


//####################  �C�x���g�ƃ^�C�����ꊇ�`��  ####################
void CSceneMap::CMapData::DrawMap(int tileAnimLoopFrame) {
	//�O����
	bool skip = false;
	bool playerTopHidden = false;
	vector<tuple<CTileData*, CPoint>> SemiTopTiles;
	vector<tuple<CTileData*, CPoint>> TopTiles;
	vector<tuple<int, CPoint>> MiddleEVs;
	vector<tuple<int, CPoint>> HighEVs;
	vector<tuple<int, CPoint>> PrimaryEVs;			//�㔼�g���B�����O���[�v
	vector<tuple<int, CPoint>> SecondaryEVs;		//�㔼�g�͌�ŕ`�悷��O���[�v

	this->setPlayerViewPixelPos();
	if(tileAnimLoopFrame <= 0) {
		tileAnimLoopFrame = 1;		//�s���Ȉ����͏C������
	}

	int left = -this->MapPixelOffset.X / this->TileSize;
	int top = -this->MapPixelOffset.Y / this->TileSize;
	CRectangle drawRange(left - 1, top - 1, Game.WindowSize.Width / this->TileSize + 2, Game.WindowSize.Height / this->TileSize + 2 + 1);
	CMapEventData* ev;
	CMapEventData::CMapEventPageData* pg;
	CPoint tempPos;

	// 1. ���i
	this->DrawBack();

	// 2. �`��D��x�F�ʏ� �̃^�C��
#pragma region NormalTiles
	TS_NOBLEND;
	for(int layer = static_cast<int>(TS::Map::Layer::Low); layer <= static_cast<int>(TS::Map::Layer::High); layer++) {
		for(int x = drawRange.Left, m = -offset; m < drawRange.Width; x++, m++) {
			if(!this->checkTileRangeX(x)) {
				continue;		//�}�b�v�O�̂��߃X�L�b�v
			}

			for(int y = drawRange.Top, n = -offset; n < drawRange.Height; y++, n++) {
				if(!this->checkTileRangeY(y)) {
					continue;		//�}�b�v�O�̂��߃X�L�b�v
				}

				//�p���b�g�ʒu���擾
				CPoint tilePos = this->Data->at(layer)[x][y].PalletPosition;
				auto& palTile = *Game.DB.getFDBTileset()->GetTilePallet(this->Data->at(layer)[x][y].PalletPosition.X, this->Data->at(layer)[x][y].PalletPosition.Y);
				if(&palTile == nullptr || tilePos.IsEmpty()) {
					//�s���ȃp���b�g�ʒu / �󔒃^�C���͏������Ȃ�
					continue;
				}

				//��񂵂ɂ���^�C���������Ŏd��������
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
					continue;		//��񂵂ɂ���
				}

				//���ۂɕ`�悷��
				this->drawTileOne(m, n, 0, 0, this->Data->at(layer)[x][y], tileAnimLoopFrame);
			}
		}
	}
#pragma endregion

	// 3. ��l����艺�̃C�x���g�̉����g
#pragma region LowEV_Bottom
	for(int i = 0; i < this->EVs.GetCount(); i++) {
		//�\������Ă���A�`��̈���̃C�x���g�̂ݕ`�悷��
		ev = this->EVs.GetFromIndex(i);
		tempPos = ev->Position;

		//���W���ꎞ�I�Ƀ��[�v�Ή��ɂ���
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
			//��񂵂ɂ���C�x���g�������Ŏd��������
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
				continue;		//��񂵂ɂ���
			}

			//SemiTransparent�^�C��������ꍇ�͉����g�𓧖��ŕ`�悷��
			if(!this->checkTilePriority(ev, static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent))) {
				TS_NOBLEND;
			} else {
				TS_ALPHABLEND(TS::Map::SemiTransparent);
			}

			//�^�C���O���t�B�b�N�łȂ��ꍇ�͏㔼�g�����̃��[�v�ŕ`�悷��
			if(!pg->IsTilesetGraphics) {
				PrimaryEVs.push_back(make_tuple(i, tempPos));
			}

			//���ۂɕ`�悷��
			this->drawEVOne(tileAnimLoopFrame, drawRange, ev, tempPos, pg, true);
		}
	}
#pragma endregion

	// 4. ��l����艺�̃C�x���g�̏㔼�g
#pragma region LowEV_Top
	for(auto& evTuple : PrimaryEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 5. ��l���Ɠ����̃C�x���g�̉����g
#pragma region NormalEV_Bottom
	PrimaryEVs.clear();
	for(auto& evTuple : MiddleEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		pg = ev->getCurrentPage();

		//�z�u���W��Semi�^�C�������邩�ۂ��ŐU�蕪����: ��ɕ`�� / ��ɕ`��
		if(this->checkIsGraphTopPrimary(ev->Position.X, ev->Position.Y)) {
			PrimaryEVs.push_back(evTuple);
		} else {
			SecondaryEVs.push_back(evTuple);
		}

		//SemiTransparent�^�C��������ꍇ�͉����g�𓧖��ŕ`�悷��
		if(!this->checkTilePriority(ev, static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent))) {
			TS_NOBLEND;
		} else {
			TS_ALPHABLEND(TS::Map::SemiTransparent);
		}

		//������ɂ��扺���g�͂����ŕ`�悷��
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), pg, true);
	}
#pragma endregion

	// 6. ��l���̉����g
#pragma region PlayerBottom
	playerTopHidden = false;
	if(this->Player.MapObject.Hide == false
	   && 0 <= this->Player.MapObject.Position.X
	   && 0 <= this->Player.MapObject.Position.Y
	   && this->Player.MapObject.Position.X < this->MapSize.Width
	   && this->Player.MapObject.Position.Y < this->MapSize.Height) {

		//�㔼�g���B����Ă��邩�ǂ�������������
		if(this->checkIsGraphTopPrimary(this->Player.MapObject.Position.X, this->Player.MapObject.Position.Y)) {
			playerTopHidden = true;
		}

		//SemiTransparent�^�C��������ꍇ�͉����g�𓧖��ŕ`�悷��
		if(!this->checkTilePriority(&this->Player.MapObject, static_cast<int>(TS::Map::TileDrawPriority::SemiTransparent))) {
			TS_NOBLEND;
		} else {
			TS_ALPHABLEND(TS::Map::SemiTransparent);
		}

		//���ۂɕ`�悷��
		this->drawPlayer(tileAnimLoopFrame, true);
	}
#pragma endregion

	// 7. �e�w: ��ɉ����g�������B��
#pragma region Shadow
	TS_ALPHABLEND(this->ShadowTransparent);
	for(int x = drawRange.Left, m = -offset; m < drawRange.Width; x++, m++) {
		if(!this->checkTileRangeX(x)) {
			continue;		//�}�b�v�O�̂��߃X�L�b�v
		}

		for(int y = drawRange.Top, n = -offset; n < drawRange.Height; y++, n++) {
			if(!this->checkTileRangeY(y)) {
				continue;		//�}�b�v�O�̂��߃X�L�b�v
			}

			//���ۂɕ`�悷��
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

	// 8. ��l���Ɠ����̃C�x���g�̏㔼�g: �`�p�^�[�� [SemiTop�^�C���ŉB�����ꍇ�͐�ɕ`�悷��]
#pragma region NormalEV_Top1
	for(auto& evTuple : PrimaryEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 9. ��l���̏㔼�g: �`�p�^�[�� [SemiTop�^�C���ŉB�����ꍇ�͐�ɕ`�悷��]
#pragma region PlayerTop1
	if(playerTopHidden && this->Player.MapObject.Hide == false
	   && 0 <= this->Player.MapObject.Position.X
	   && 0 <= this->Player.MapObject.Position.Y
	   && this->Player.MapObject.Position.X < this->MapSize.Width
	   && this->Player.MapObject.Position.Y < this->MapSize.Height) {
		this->drawPlayer(tileAnimLoopFrame, false);
	}
#pragma endregion

	// 10. SemiTopMost�^�C��
#pragma region SemiTopTile
	TS_COUNTLOOP(i, SemiTopTiles.size()) {
		this->drawTileOne(std::get<1>(SemiTopTiles[i]).X, std::get<1>(SemiTopTiles[i]).Y, 0, 0, *std::get<0>(SemiTopTiles[i]), tileAnimLoopFrame);
	}
#pragma endregion

	// 11. ��l���Ɠ����̃C�x���g�㔼�g: �a�p�^�[�� [SemiTop�^�C���ŉB����Ȃ����̂͂����ŕ`�悷��]
#pragma region NormalEV_Top2
	for(auto& evTuple : SecondaryEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 12. ��l���̏㔼�g: �a�p�^�[�� [SemiTop�^�C���ŉB����Ȃ��ꍇ�͂����ŕ`�悷��]
#pragma region PlayerTop2
	if(playerTopHidden == false && this->Player.MapObject.Hide == false
	   && 0 <= this->Player.MapObject.Position.X
	   && 0 <= this->Player.MapObject.Position.Y
	   && this->Player.MapObject.Position.X < this->MapSize.Width
	   && this->Player.MapObject.Position.Y < this->MapSize.Height) {
		this->drawPlayer(tileAnimLoopFrame, false);
	}
#pragma endregion

	// 13. ��l������̃C�x���g�S�g
#pragma region HighEV
	//�����g
	for(auto& evTuple : HighEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), true);
	}

	//�㔼�g
	for(auto& evTuple : HighEVs) {
		ev = this->EVs.GetFromIndex(std::get<0>(evTuple));
		this->drawEVOne(tileAnimLoopFrame, drawRange, ev, std::get<1>(evTuple), ev->getCurrentPage(), false);
	}
#pragma endregion

	// 14. �`��D��x�F�őO�� �̃^�C��
#pragma region TopTile
	TS_COUNTLOOP(i, TopTiles.size()) {
		this->drawTileOne(std::get<1>(TopTiles[i]).X, std::get<1>(TopTiles[i]).Y, 0, 0, *std::get<0>(TopTiles[i]), tileAnimLoopFrame);
	}
#pragma endregion

	// 15. �t�H�O
	this->DrawFog();

	// �f�o�b�O�o��
	//Game.DXLib.DrawString(300, 30, -1, WHITE, Game.Format("drawRange (%d, %d, %d, %d)", drawRange.Left, drawRange.Top, drawRange.Right(), drawRange.Bottom()).c_str());
}


//####################  ���i��ǂݍ���  ####################
void CSceneMap::CMapData::LoadBack(string fileName) {
	Game.Material.getGraphics()->Delete(this->BackHandle);
	this->BackHandle = Game.Material.getGraphics()->Add(fileName);
	this->CurrentMapBackFileName = fileName;
}


//####################  ���i��`�悷��  ####################
void CSceneMap::CMapData::DrawBack() {
	this->drawMapPicture(this->BackHandle, this->BackScrollType, this->PictureScrollCounter, -1, -1);
}


//####################  �t�H�O��ǂݍ���  ####################
void CSceneMap::CMapData::LoadFog(string fileName) {
	Game.Material.getGraphics()->Delete(this->FogHandle);
	this->FogHandle = Game.Material.getGraphics()->Add(fileName);
	this->CurrentFogFileName = fileName;
}


//####################  �t�H�O��`�悷��  ####################
//�V�X�e��DB�̐ݒ荀�ڂ́A�t�H�O"�����x"�ł��邽�߁A255���獷������������s���Ă���
void CSceneMap::CMapData::DrawFog() {
	this->drawMapPicture(this->FogHandle, this->FogScrollType, this->PictureScrollCounter, this->FogBlend, 255 - this->FogTransparent);
}


//####################  ���i/�t�H�O�����ۂɕ`�悷��  ####################
void CSceneMap::CMapData::drawMapPicture(int handle, int scrollType, CPoint& scrollCounter, int blendMode, int blendParam) {
	CPoint increment;
	CSize picSize;
	DxLib::GetGraphSize(handle, &picSize.Width, &picSize.Height);
	if(picSize.Width == 0 || picSize.Height == 0) {
		return;		//�`��ł��Ȃ�
	}

	//�X�N���[�����@�ɉ����Ĉړ��J�E���^�[�����Z�Z�b�g���ĕ`���_���Z�o����
	switch((TS::Map::ScrollType)scrollType) {
		case TS::Map::ScrollType::None:
			//�X�N���[�����Ȃ�
			increment.Set(0, 0);
			break;

		case TS::Map::ScrollType::FollowChar:
			//�ړ��ɍ��킹�ăX�N���[��
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

	//3x3=���v�X���`�悷��
	if(blendMode == -1) {
		TS_NOBLEND;
	} else {
		DxLib::SetDrawBlendMode(Game.DXLib.ConvertToDXBlend(blendMode), blendParam);
	}
	for(int x = -1; x <= +1; x++) {
		for(int y = -1; y <= +1; y++) {
			Game.DXLib.DrawGraph(handle, increment.X + picSize.Width * x, increment.Y + picSize.Height * y);
			//�f�o�b�O�p: Game.DXLib.DrawRect(increment.X + picSize.Width * x, increment.Y + picSize.Height * y, picSize.Width, picSize.Height, RED, false);
		}
	}
	TS_NOBLEND;
}


//####################  ��l���̃}�b�v��ɂ�����s�N�Z�����W���Z�b�g����  ####################
void CSceneMap::CMapData::setPlayerViewPixelPos() {
	//��l���ʒu���m�F: ��ʒ[�ɂ���ꍇ�͎�l�����̂𓮂����A����ȊO�̓}�b�v�̃I�t�Z�b�g�œ������A�������}�b�v�̃��[�v���L���Ȏ��͏�Ƀ}�b�v�̃I�t�Z�b�g�œ�����
	bool loopX = (this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both));
	bool loopY = (this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both));

	if(loopX == false && this->Player.MapObject.Position.X * this->TileSize + this->Player.MapObject.MoveCounter.X < Game.WindowSize.Width / 2) {
		//���[�Ɋ���Ă���
		this->MapPixelOffset.X = 0;
		this->playerViewPixelPos.X = this->Player.MapObject.Position.X * this->TileSize + this->Player.MapObject.MoveCounter.X;
	} else if(loopX == false && this->MapSize.Width * this->TileSize - (Game.WindowSize.Width / 2) < this->Player.MapObject.Position.X * this->TileSize + this->Player.MapObject.MoveCounter.X) {
		//�E�[�Ɋ���Ă���
		this->MapPixelOffset.X = (Game.WindowSize.Width - this->MapSize.Width * this->TileSize);
		this->playerViewPixelPos.X = (this->Player.MapObject.Position.X - (this->MapSize.Width - Game.WindowSize.Width / this->TileSize)) * this->TileSize + this->Player.MapObject.MoveCounter.X;
	} else {
		//��l�������\��
		this->MapPixelOffset.X = -(this->Player.MapObject.Position.X * this->TileSize) + Game.WindowSize.Width / 2 - this->Player.MapObject.MoveCounter.X;
		this->playerViewPixelPos.X = Game.WindowSize.Width / 2;
	}

	if(loopY == false && this->Player.MapObject.Position.Y * this->TileSize + this->Player.MapObject.MoveCounter.Y <= Game.WindowSize.Height / 2) {
		//��[�Ɋ���Ă���
		this->MapPixelOffset.Y = 0;
		this->playerViewPixelPos.Y = this->Player.MapObject.Position.Y * this->TileSize + this->Player.MapObject.MoveCounter.Y - this->TileSize;
	} else if(loopY == false && this->MapSize.Height * this->TileSize - (Game.WindowSize.Height / 2) <= this->Player.MapObject.Position.Y * this->TileSize + this->Player.MapObject.MoveCounter.Y) {
		//���[�Ɋ���Ă���
		this->MapPixelOffset.Y = (Game.WindowSize.Height - this->MapSize.Height * this->TileSize);
		this->playerViewPixelPos.Y = (this->Player.MapObject.Position.Y - (this->MapSize.Height - Game.WindowSize.Height / this->TileSize)) * this->TileSize + this->Player.MapObject.MoveCounter.Y - this->TileSize;
	} else {
		//��l�������\��
		this->MapPixelOffset.Y = -(this->Player.MapObject.Position.Y * this->TileSize) + Game.WindowSize.Height / 2 - this->Player.MapObject.MoveCounter.Y;
		this->playerViewPixelPos.Y = Game.WindowSize.Height / 2 - this->TileSize;
	}
}


//####################  �P�ꃌ�C���[�^�C����`�悷��  ####################
void CSceneMap::CMapData::drawTileOne(int x, int y, int offsetX, int offsetY, const CTileData& palTile, int tileAnimLoopFrame) {
	//�A�j���[�V����ID���Z�o����
	auto& palData = *Game.DB.getFDBTileset()->GetTilePallet(palTile.PalletPosition.X, palTile.PalletPosition.Y);
	if(&palData == nullptr || (palTile.PalletPosition.X == 0 && palTile.PalletPosition.Y == 0)) {
		//�s���ȃp���b�g�ʒu / �󔒃^�C���͏������Ȃ�
		return;
	}

	int animIndex = (palData.AnimPatternCount <= 0) ? 0 : (this->animCnt % tileAnimLoopFrame) / (tileAnimLoopFrame / palData.AnimPatternCount);

	if(palTile.PalletPosition.Y >= TS::Map::AutoTileRowLength) {
		//�P��^�C����`��
		Game.DXLib.DrawRectGraph(
			this->TilePallet->TilesetHandle,
			x * this->TileSize + this->MapPixelOffset.X % this->TileSize + offsetX,
			y * this->TileSize + this->MapPixelOffset.Y % this->TileSize + offsetY,
			palTile.PalletPosition.X * this->TileSize,
			palTile.PalletPosition.Y * this->TileSize,
			this->TileSize, this->TileSize
			);
	} else {
		//�I�[�g�^�C���p��1/4�`��
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


//####################  �P��C�x���g��`��  ####################
void CSceneMap::CMapData::drawEVOne(int tileAnimLoopFrame, const CRectangle& drawRange, CMapEventData* ev, CPoint& pos, CMapEventData::CMapEventPageData* pg, bool isBottom) {
	if(!pg->IsTilesetGraphics) {
		if(isBottom) {
			//�����g (������^�C���T�C�Y��) ��`��
			Game.DXLib.DrawRectGraph(
				pg->GraphHandle,
				(pos.X - drawRange.Left - offset) * this->TileSize + this->MapPixelOffset.X % this->TileSize + ev->MoveCounter.X,
				(pos.Y - drawRange.Top - offset) * this->TileSize + this->MapPixelOffset.Y % this->TileSize + ev->MoveCounter.Y,
				pg->OneCharSize.Width * pg->AnimID,
				pg->OneCharSize.Height * (pg->getDirection() + 1) - this->TileSize,
				pg->OneCharSize.Width, this->TileSize
				);
		} else {
			//�㔼�g��`��
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
		//���̂܂܃O���t�B�b�N��`��
		const auto& palTile = *Game.DB.getFDBTileset()->GetTilePallet(pg->TilePalletPosition.X, pg->TilePalletPosition.Y);
		if(&palTile == nullptr || pg->TilePalletPosition.IsEmpty()) {
			//�s���ȃp���b�g�ʒu / �󔒃^�C���͏������Ȃ�
			return;
		}

		//���ۂɕ`��
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


//####################  �v���C���[��`��  ####################
void CSceneMap::CMapData::drawPlayer(int tileAnimLoopFrame, bool isBottom) {
	if(isBottom) {
		//�����g��`��
		Game.DXLib.DrawRectGraph(
			this->Player.CharData.GraphHandle,
			this->playerViewPixelPos.X,
			this->playerViewPixelPos.Y + this->TileSize,
			this->Player.CharData.OneCharSize.Width * this->Player.CharData.AnimID,
			this->Player.CharData.OneCharSize.Height * (this->Player.CharData.getDirection() + 1) - this->TileSize,
			this->Player.CharData.OneCharSize.Width, this->TileSize
			);
	} else {
		//�㔼�g��`��
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


//####################  �w�肵�����W�Ɏw�肵���`��D��x�̃^�C�������݂��邩���ׂ�  ####################
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


//####################  �w�肵���}�b�v���W���}�b�v�̒��ɂ��邩�ǂ�������������  ####################
//���[�v�ݒ肪�L���ł���ꍇ�͍��W�����������
bool CSceneMap::CMapData::checkTileRangeX(int& x) {
	if(x < 0) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			x = this->MapSize.Width + x;
		} else {
			return false;		//�͈͊O
		}
	} else if(this->MapSize.Width <= x) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			x -= this->MapSize.Width;
		} else {
			return false;		//�͈͊O
		}
	}
	return true;
}
bool CSceneMap::CMapData::checkTileRangeY(int& y) {
	if(y < 0) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			y = this->MapSize.Height + y;
		} else {
			return false;		//�͈͊O
		}
	} else if(this->MapSize.Height <= y) {
		if(this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical) || this->MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
			y -= this->MapSize.Height;
		} else {
			return false;		//�͈͊O
		}
	}
	return true;
}


//####################  �ʏ�`��̃C�x���g/��l���̏㔼�g���e��SemiTop�^�C���ɉB��邩�ǂ�������������  ####################
bool CSceneMap::CMapData::checkIsGraphTopPrimary(int tx, int ty) {
	for(int l = static_cast<int>(TS::Map::Layer::Low); l <= static_cast<int>(TS::Map::Layer::High); l++) {
		auto tilePal = *Game.DB.getFDBTileset()->GetTilePallet(
			this->Data->at(l)[tx][ty].PalletPosition.X,
			this->Data->at(l)[tx][ty].PalletPosition.Y
			);

		//�^�C���̕`��D��x����������
		switch((TS::Map::TileDrawPriority)tilePal.Priority) {
			case TS::Map::TileDrawPriority::SemiTopMost:
			case TS::Map::TileDrawPriority::SemiTransparent:
				return true;		//�㔼�g���i�e�����j��ɕ`���K�v������
		}
	}
	return false;
}
