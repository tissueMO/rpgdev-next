//###################################################################
//			�}�b�v�f�[�^�̒��g�Ɋւ���N���X��`
//###################################################################
#include "Header.h"


//####################  ���s�O���t�B�b�N�̃L������ǂݍ���  ####################
void CSceneMap::CMapData::CMapCharObjectData::LoadGraphics(string fileName) {
	//���ɃO���t�B�b�N���ǂ܂�Ă���ꍇ�͔j������
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


//####################  ���s�O���t�B�b�N�̃L�����̃��\�[�X��j������  ####################
void CSceneMap::CMapData::CMapCharObjectData::DisposeGraphics() {
	Game.Material.getGraphics()->Delete(this->GraphHandle);
	this->GraphHandle = -1;
	this->Graphics.clear();
}


//####################  �R���X�g���N�^�[  ####################
CSceneMap::CMapData::CMapData() {
	this->Data = new TileArray;			//��̃^�C���z��𐶐�
}


//####################  �^�C����`�t�@�C����ǂݍ���  ####################
void CSceneMap::CMapData::loadTileFile(string fileName, CMapData& dest, vector<string>& plain) {
	string textBuffer;
	vector<string> spl;
	vector<string> subspl;

	//�^����ꂽ�t�@�C���̈��k�������A�e�L�X�g���s���Ƃɕ�������
	if(fileName.empty()) {
		return;		//��̃t�@�C�����͏������Ȃ�

#ifdef _DEBUG
	} else if(!PathFileExists(fileName.c_str())) {
		//DX�A�[�J�C�u��͎g�p�ł��Ȃ�
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��^�C����`�t�@�C���ł��B\n>%s", fileName.c_str());
		return;
	} else if(!CSceneMap::loadTextMap(fileName, textBuffer)) {
		//�񈳏k�`���̃}�b�v�̃��[�h�Ɏ��s����
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�}�b�v�̓ǂݍ��݂Ɏ��s���܂����B\n->%s", fileName.c_str());
		return;
#else
	} else if(!CSceneMap::deflate(fileName, textBuffer)) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�}�b�v�̓ǂݍ��݂Ɏ��s���܂����B\n->%s", fileName.c_str());
		return;		//�𓀂Ɏ��s����
#endif
	}
	Game.SplitString(plain, textBuffer, "\r\n");
	dest.CurrentMapFileName = fileName;

	//�w�b�_�[����ǂݍ���
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

	Game.PopHead(plain);		//��s�ǂݎ̂�

	//�s���ƂɊe�^�C������ǂݍ���
	for(int layer = static_cast<int>(TS::Map::Layer::Low); layer < TS::Map::MapLayerCountWithoutEV; layer++) {
		//�P�s����������
		TS_COUNTLOOP(y, dest.MapSize.Height) {
			textBuffer = Game.PopHead(plain);
			Game.SplitString(spl, textBuffer, "|");
			TS_COUNTLOOP(x, dest.MapSize.Width) {
				Game.SplitString(subspl, spl[x], ",");
				if(subspl.size() < static_cast<int>(TS::Map::QuarterTile::Count)) {
					continue;		//�s���Ȍ`��
				}

				// 1/4�^�C����񂪐�s����
				TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
					dest.Data->at(layer)[x][y].Quarter[q] = atoi(subspl[q].c_str());
				}

				//�^�C�����C���[�ɂ̓^�C���p���b�g�ʒu���܂܂�Ă���
				if(subspl.size() > static_cast<int>(TS::Map::QuarterTile::Count)) {
					dest.Data->at(layer)[x][y].PalletPosition.Set(subspl[static_cast<int>(TS::Map::QuarterTile::Count)].c_str());
				}

				//�}�b�v��̍��W���Z�b�g����
				dest.Data->at(layer)[x][y].MapPosition.Set(x, y);
			}
		}
		Game.PopHead(plain);		//��s�ǂݎ̂�
	}
	TS_LOG("�^�C����`�t�@�C���̓ǂݍ��݂��������܂����B");
}


//####################  �j������  ####################
//NOTE: MapData�̓f�X�g���N�^�[���Ă΂�Ȃ��d�l�̂��߁ASTL�n�̃I�u�W�F�N�g�����ׂĎ蓮�Ŕj�����Ȃ���΂Ȃ�Ȃ��B
void CSceneMap::CMapData::Dispose() {
	delete this->Data;

	//�v���C���[�O���t�B�b�N�j��
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

	//�C�x���g�y�[�W���ɍ폜
	for(auto& ev : this->EVs.Items) {
		ev.Dispose();
	}
	this->EVs.Clear();
}


//####################  �}�b�v��ǂݍ���  ####################
void CSceneMap::CMapData::LoadMap(string fileName, bool exceptEV) {
	if(fileName.empty()) {
		return;		//��̃t�@�C�����͏������Ȃ�
	}

	TS_LOG("���̃}�b�v��ǂݍ��݂܂�...\n>" + fileName);

	//�w�肳�ꂽ�}�b�v�̃��^���E�^�C������ǂݍ���
	auto scene = Game.getSceneMap();
	vector<string> plain;
	CSceneMap::CMapData::loadTileFile(fileName, *this, plain);

	//�J�[�{����������ꍇ�A���̃}�b�v�̃^�C�������ꎞ�I�ɓǂݍ���
	if(!this->BaseMapFileName.empty()) {
		TS_LOG("�J�[�{�����̃}�b�v�̃^�C������ǂݍ��݂܂�...\n>" + this->BaseMapFileName);

		CMapData baseMap;
		vector<string> subPlain;
		CSceneMap::CMapData::loadTileFile(this->BaseMapFileName, baseMap, subPlain);
		vector<string>().swap(subPlain);		//���߂̃��\�[�X���

		//���݂̃}�b�v�̒��ŋ󔒃^�C���ɂȂ��Ă��镔�����J�[�{�����̃^�C���ɒu��������E�e�w�͂ǂ��炩�Е��ɕt���Ă���Ƃ��낾����L���ɂ���
		for(int l = static_cast<int>(TS::Map::Layer::Low); l < static_cast<int>(TS::Map::MapLayerCountWithoutEV); l++) {
			TS_COUNTLOOP(x, min(this->MapSize.Width, baseMap.MapSize.Width)) {
				TS_COUNTLOOP(y, min(this->MapSize.Height, baseMap.MapSize.Height)) {
					if(l == static_cast<int>(TS::Map::Layer::Shadow)) {
						//�e�w�̂Ƃ��� 1/4 �^�C�����̔r���I�_���a�����
						TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
							this->Data->at(l)[x][y].Quarter[q] = this->Data->at(l)[x][y].Quarter[q] | baseMap.Data->at(l)[x][y].Quarter[q];
						}
					} else if(this->Data->at(l)[x][y].PalletPosition.IsEmpty()) {
						//�^�C�����C���[�̂Ƃ��̓p���b�g���W��1/4�^�C�������R�s�[
						this->Data->at(l)[x][y].PalletPosition = baseMap.Data->at(l)[x][y].PalletPosition;
						TS_COUNTLOOP(q, static_cast<int>(TS::Map::QuarterTile::Count)) {
							this->Data->at(l)[x][y].Quarter[q] = baseMap.Data->at(l)[x][y].Quarter[q];
						}
					}
				}
			}
		}
	}

	//�e�탊�\�[�X��ǂݍ��݁A�^�C���p���b�g�𐶐�����
	TS_LOG("�e�탊�\�[�X��ǂݍ��݁A�^�C���p���b�g�𐶐����܂��B");
	this->LoadBack(this->CurrentMapBackFileName);
	this->LoadFog(this->CurrentFogFileName);
	this->TilePallet = Game.DB.getFDBTileset()->LoadTileset(this->TilesetID);

	//�X�N���v�g��ǂݍ���
	TS_LOG("�}�b�v�̃X�N���v�g��ǂݍ��݂܂��B");
	this->EVClear();
	this->SQHandle = Game.SQ.Add(Game.Format(TS::FileNames::SQMapEvents, this->GUID.c_str()));

	//�C�x���g�z�u�������s���ꍇ�̓X�N���v�g���Ăяo��
	if(!exceptEV) {
		TS_LOG("�C�x���g�̔z�u�������s���܂��B");
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SetupMapEvents);

		//���̃}�b�v�ɊY������ʕϐ��������o��
		TS_LOG("�C�x���g�ʕϐ���K�p���܂��B");
		CSceneMap::applyMapSelfValue();
	}

	TS_LOG("�}�b�v�̓ǂݍ��݂��������܂����B");
}


//####################  �}�b�v��ǂݍ��񂾌�ɍs���ׂ����ʏ���  ####################
void CSceneMap::CMapData::AfterLoadMap() {
	//�v���C���[�I�u�W�F�N�g�Ɋւ�������Đݒ肷��
	this->setPlayerViewPixelPos();
	this->Player.MapObject.MoveCounter.Set(0, 0);

	//BGM�EBGS�Đ�
	auto scene = Game.getSceneMap();
	scene->BGM.Load(this->BGMRawString);
	scene->BGS.Load(this->BGSRawString);
	this->PlayBGMBGS();

	//�C�x���g�y�[�W���񔻒�
	//�}�b�v�ړ�����Ɏ��s���ׂ��C�x���g������΁A������Ăяo��
	for(auto& ev : this->EVs.Items) {
		//�����y�[�W���画�肵�Ă���
		for(int pageIndex = ev.GetPageCount() - 1; pageIndex >= 0; pageIndex--) {
			if(!Game.getSceneMap()->MapData.RunEV(ev.ID, pageIndex, static_cast<int>(TS::Map::EventScriptType::Page), false)) {
				continue;
			}

			//�L���ȃy�[�W������������A���͋N�������
			ev.SetPageIndex(pageIndex);			//�����œ���ړ��̃X�N���v�g���Ăяo�����
			if(ev.getCurrentPage()->TriggerType != static_cast<int>(TS::Map::EventTriggerType::AfterMoveMap)) {
				//�L���ȃy�[�W�����N������^�C�~���O���}�b�v�ړ�����ł͂Ȃ�
				break;
			} else if(!this->RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Trigger), false)) {
				//�L���ȃy�[�W�Ń}�b�v�ړ�����̋N�����������N������^�C�~���O�Ƃ��Ĕ��肳��Ȃ�����
				break;
			}

			//���s���A��\����Ԃɂ���
			this->RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Run), true);
			ev.SetPageIndex(-1);
		}
	}

	//�X�N���v�g��GC���s
	Game.SQ.CollectGarbage();
}


//####################  �}�b�v��BGM/BGS���Đ�����  ####################
void CSceneMap::CMapData::PlayBGMBGS() {
	//�ύX����K�v������ꍇ�̂݁ABGM��BGS���Đ�����
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


//####################  ���ݎ��s���̃C�x���g���擾����  ####################
//���[�U�[������~���Ȃ��C�x���g�̎��s�����ׂĊ܂�Ō�������
CSceneMap::CMapData::CMapEventData* CSceneMap::CMapData::getThisEVData() {
	if(this->runningEVNest_system.size() == 0) {
		//�C�x���g�����s���Ă��Ȃ�
		return nullptr;
	} else {
		return this->runningEVNest_system[0];
	}
}


//####################  �_�~�[�C�x���g�����ׂč폜����  ####################
void CSceneMap::CMapData::ClearDummyEVs() {
	this->EVs.Items.erase(std::remove_if(TS_CONTAINERALL(this->EVs.Items), [&](CMapEventData& ev) {
		if(ev.IsDummy) {
			ev.Dispose();
			return true;
		}
		return false;
	}), this->EVs.Items.end());

	//NOTE: �e�����o�[�̃|�C���^���Đݒ肷��ivector�v�f���폜�������_�ł��̗v�f�ȍ~�̃����o�[�Ɋւ���|�C���^��񂪂��ׂċ������߁j
	for(auto &ev : this->EVs.Items) {
		ev.TargetCharPTR = ev.getCurrentPage();
	}
}


//####################  ���s�\�񂳂ꂽ�ł��Â��C�x���g��ID��������o��  ####################
int CSceneMap::CMapData::PopReserveEVID() {
	if(this->reservedEVs.size() == 0) {
		return -1;
	} else {
		const int temp = this->reservedEVs[0];
		this->reservedEVs.erase(this->reservedEVs.begin());
		return temp;
	}
}


//####################  ���݂̃C�x���g�������O�o�͂���  ####################
void CSceneMap::CMapData::PrintEVData() {
	TS_LOG("\nEVs:");
	for(auto& ev : this->EVs.Items) {
		TS_LOG(ev.toString());
	}
}


//####################  �^�C�������擾����  ####################
CPoint CSceneMap::CMapData::GetTile(int layer, int x, int y) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Event)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				return this->Data->at(layer)[x][y].PalletPosition;
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ�}�b�v�^�C�� [Layer:%d, X:%d, Y:%d] �������ł��B", layer, x, y);
	return CPoint();
}


//####################  �^�C������ݒ肷��  ####################
//absolute��true�ɂ���ƁA�I�[�g�^�C�����X�V����Ȃ��Ȃ�A���݂̐ڑ���Ԃ��ێ�����
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
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ�}�b�v�^�C�� [Layer:%d, X:%d, Y:%d] �������ł��B", layer, x, y);
}


//####################  �w�肵���^�C�����W�� 1/4 ���W���擾����  ####################
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
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ 1/4�}�b�v�^�C�� [Layer:%d, X:%d, Y:%d, Quarter:%d] �������ł��B", layer, x, y, q);
	return -1;
}


//####################  �w�肵���^�C�����W�� 1/4 ���W��ݒ肷��  ####################
//�e�������̓I�[�g�^�C���̐ڑ���Ԃ�ύX����̂Ɏg��
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
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ 1/4�}�b�v�^�C�� [Layer:%d, X:%d, Y:%d, Quarter:%d] �������ł��B", layer, x, y, q);
}


//####################  �w�肵���^�C�����W�������邩�ǂ������擾����  ####################
bool CSceneMap::CMapData::IsWalkableTile(int x, int y) {
	for(int l = static_cast<int>(TS::Map::Layer::High); l >= static_cast<int>(TS::Map::Layer::Low); l--) {
		auto temp = this->Data->at(l)[x][y].PalletPosition;
		if(temp.IsEmpty()) {
			//�󔒃^�C���͎��̃��[�v��
			continue;
		}
		auto tile = Game.DB.getFDBTileset()->GetTilePallet(temp.X, temp.Y);

		//���ʑw�ˑ��̏ꍇ�͎��̃��[�v��
		switch(static_cast<TS::Map::HitType>(tile->Hit)) {
			case TS::Map::HitType::NG:
				return false;
			case TS::Map::HitType::OK:
				return true;
		}
	}

	//�S�w�ŋ󔒃^�C���̂Ƃ��͕����Ȃ�
	return false;
}


//####################  �w�肵�����W�Ɏw�肵���O���[�v�ԍ��̃^�C�������邩�ǂ����𒲂ׂ�  ####################
//���݂���ꍇ�͍ŏ��Ɍ����������C���[�̃C���f�b�N�X��Ԃ��A���݂��Ȃ��ꍇ�� -1
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
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ�}�b�v�^�C�� [X:%d, Y:%d] �������ł��B", x, y);
	return -1;
}


//####################  �w�肵�����W�̃^�C���̃O���[�v�ԍ����擾����  ####################
int CSceneMap::CMapData::GetTileGroupNum(int layer, int x, int y) {
	TS_CHECKINDEX(layer, static_cast<int>(TS::Map::Layer::Low), static_cast<int>(TS::Map::Layer::Shadow)) {
		TS_CHECKINDEX(x, 0, this->MapSize.Width) {
			TS_CHECKINDEX(y, 0, this->MapSize.Height) {
				const auto tilePos = this->Data->at(layer)[x][y].PalletPosition;
				return Game.DB.getFDBTileset()->GetTilePallet(tilePos.X, tilePos.Y)->Tag;
			}
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ�}�b�v�^�C�� [Layer:%d, X:%d, Y:%d] �������ł��B", layer, x, y);
	return -1;
}


//####################  ���ӂ̃I�[�g�^�C�����X�V����  ####################
void CSceneMap::CMapData::updateAutoTile(int layer, int x, int y, bool isParent) {
	//�w�肳�ꂽ�^�C�����I�[�g�^�C���łȂ��ꍇ�͏������Ȃ�
	if(layer >= static_cast<int>(TS::Map::Layer::Shadow) || this->Data->at(layer)[x][y].PalletPosition.Y >= TS::Map::AutoTileRowLength) {
		return;
	}

	int vx, vy, px, py, bx, by, cx, cy;
	CPoint ny, ax, ay, ac;
	CPoint palPos;
	TS::Map::QuarterTile part;
	const int tx = x;
	const int ty = y;

	//�w�肳�ꂽ�^�C�����W�ɂ��ăZ�b�g����
	for (int y = 1; y <= 2; y++) {
		for (int x = 1; x <= 2; x++) {
			//�T���p
			vx = (x % 2 == 1) ? -1 : 1;
			vy = (y % 2 == 1) ? -1 : 1;
			px = tx + (x == 0 ? -1 : (x == 3) ? 1 : 0);
			py = ty + (y == 0 ? -1 : (y == 3) ? 1 : 0);

			//���͂̃f�[�^�F��r�p�E�͈͊O�̋�Ԃɂ͔�r��Ɠ����^�C����������̂Ƃ݂Ȃ�
			palPos = this->Data->at(layer)[px][py].PalletPosition;
			ny = palPos;        //��r�
			ax = (px + vx < 0 || this->MapSize.Width <= px + vx) ? palPos : this->Data->at(layer)[px + vx][py].PalletPosition;      //���̔�r�p
			ay = (py + vy < 0 || this->MapSize.Height <= py + vy) ? palPos : this->Data->at(layer)[px][py + vy].PalletPosition;     //�c�̔�r�p
			ac = (px + vx < 0 || this->MapSize.Width <= px + vx || py + vy < 0 || this->MapSize.Height <= py + vy) ? palPos : this->Data->at(layer)[px + vx][py + vy].PalletPosition;   //�p(�΂�)�̔�r�p
																																													//�Ώۈʒu�̃p�����[�^�[
			bx = (x % 2 == 0) ? 1 : 0;
			by = (y % 2 == 0) ? 1 : 0;
			cx = ((x == 0) ? -1 : (x == 3) ? 1 : 0);
			cy = ((y == 0) ? -1 : (y == 3) ? 1 : 0);

			//�͈͊O�̕����͏������Ȃ�
			if (tx + cx < 0 || this->MapSize.Width <= tx + cx || ty + cy < 0 || this->MapSize.Height <= ty + cy) {
				continue;
			}
			palPos = this->Data->at(layer)[tx + cx][ty + cy].PalletPosition;
			part = static_cast<TS::Map::QuarterTile>((x - 1) + (y - 1) * 2);

			if(palPos.IsEmpty() == false && palPos.Y < TS::Map::AutoTileRowLength) {
				//�I�[�g�^�C���̏ꍇ�̂ݏ�������
				if(ny == ax && ny == ay && ny == ac) {
					//�c�E���E�p�����ׂē���^�C��
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::AllSide);
				} else if(ny == ax && ny == ay) {
					//�c�E��������^�C��
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::FourSide);
				} else if(ny == ay) {
					//�c������^�C��
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Vertical);
				} else if(ny == ax) {
					//��������^�C��
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Horizontal);
				} else {
					//�l���������قȂ�^�C���F�Ɨ��^
					this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Isolate);
				}
			} else {
				//�I�u�W�F�N�g�^�C���͏�ɓƗ��^�ɂ���
				this->Data->at(layer)[tx + cx][ty + cy].Quarter[static_cast<int>(part)] = static_cast<int>(TS::Map::AutoTilePattern::Isolate);
			}
		}
	}

	//��_�ł���ꍇ�A����ɂ��̔����ɂ���^�C���̐ڂ��Ă��镔�����X�V����
	if(isParent) {
		for(int x = -1; x <= +1; x++) {
			for(int y = -1; y <= +1; y++) {
				if(x == 0 && y == 0) {
					continue;       //��_�͏������Ȃ�
				} else if(tx + x < 0 || ty + y < 0 || this->MapSize.Width <= tx + x || this->MapSize.Height <= ty + y) {
					continue;       //�͈͊O�͏������Ȃ�
				}
				this->updateAutoTile(layer, tx + x, ty + y, false);
			}
		}
	}
}


//####################  �w����W����P���i�񂾍��W���擾  ####################
//�}�b�v�͈̔͊O�ɏo�Ă��܂����ꍇ�AX/Y�̒l���}�C�i�X�ɂȂ�
//�߂�l�̓��[�v�������ǂ���
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

	//�ړ���̍��W���}�b�v�̊O�ɂȂ�ꍇ�A���[�v�ݒ���m�F����
	//������
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
	//�c����
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
