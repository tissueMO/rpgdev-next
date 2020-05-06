//###################################################################
//			�^�C���Z�b�g �f�[�^�x�[�X
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �^�C���Z�b�g��j������  ####################
void CDatabase::CDBTileset::Dispose() {
	if(this != nullptr) {
		if(this->DBs.GetCount() > 0) {
			this->DBs.GetFromIndex(0)->ClearRow();
		}
		for(auto& tile : this->activeTilesetData) {
			vector<CTileData>().swap(tile);
		}
		vector<TilesetRow>().swap(this->activeTilesetData);
		this->DisposeTilePallet();
	}
}


//####################  �w�肵��ID�̃^�C���Z�b�g��ǂݍ���  ####################
CDatabase::CDBTileset::CTilePallet* CDatabase::CDBTileset::LoadTileset(int id) {
	this->ActiveTilesetID = id;
	this->loadActiveTilesetFile();
	return this->loadActiveTilesetGraph();
}


//####################  ���݂̃^�C���p���b�g�̎w��ʒu�̏����擾  ####################
CDatabase::CDBTileset::CTileData* CDatabase::CDBTileset::GetTilePallet(int tileX, int tileY) {
	if(tileX < 0 || TS::Map::TileColumnCount <= tileX || tileY < 0 || this->activeTilesetData.size() <= tileY) {
		return nullptr;
	} else {
		return &this->activeTilesetData[tileY][tileX];
	}
}


//####################  �^�C���p���b�g�̃^�C���P�ʂ̃T�C�Y���擾  ####################
CSize CDatabase::CDBTileset::GetTilePalletSize() {
	return CSize(TS::Map::TileColumnCount, this->activeTilesetData.size());
}


//####################  �I�[�g�^�C���̃n���h���Q���X�N���v�g�p�ɐ��������Ԃ�  ####################
CIntegerArray CDatabase::CDBTileset::CTilePallet::getAutoTiles() {
	CIntegerArray ret;
	for(auto at : this->AutoTiles) {
		ret.Add(at);
	}
	return ret;
}


//####################  �^�C�����̐��f�[�^���琳�K�̃f�[�^��  ####################
CDatabase::CDBTileset::CTileData CDatabase::CDBTileset::setData(string raw) {
	CTileData result = CTileData();
	vector<string> spl;

	//���f�[�^�𕪊�
	Game.SplitString(spl, raw, ";");
	if(spl.size() >= 4) {
		result.Hit = atoi(spl[0].c_str());
		result.InOut = atoi(spl[1].c_str());
		result.Priority = atoi(spl[2].c_str());
		result.Tag = atoi(spl[3].c_str());
		result.AnimPatternCount = 1;
	}
	return result;
}


//####################  ���݂̃^�C���p���b�g��j������  ####################
void CDatabase::CDBTileset::DisposeTilePallet() {
	if(this->loadedPallet != nullptr) {
		if(Game.Material.getGraphics() != nullptr) {
			for(auto& hdl : this->loadedPallet->AutoTiles) {
				Game.Material.getGraphics()->Delete(hdl);
			}
		}
		DxLib::DeleteGraph(this->loadedPallet->TilesetHandle);
		delete this->loadedPallet;
		this->loadedPallet = nullptr;
	}
}


//####################  ���݃A�N�e�B�u�ɂȂ��Ă���^�C���Z�b�g��`�t�@�C����ǂݍ���  ####################
void CDatabase::CDBTileset::loadActiveTilesetFile() {
	//�O��̃^�C���Z�b�g�͔j������
	this->activeTilesetData.clear();
	if(!this->openFile(Game.Format(TS::FileNames::TilesetData, this->ActiveTilesetID))) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�^�C���Z�b�g��`�t�@�C���̃I�[�v���Ɏ��s���܂����B\n>%s", Game.Format(TS::FileNames::TilesetData, this->ActiveTilesetID).c_str());
		return;
	}

	//1�s�ڂ���127��A�s�P�ʎ擾�AFixedID|WxH|�^�C�����
	vector<string> spl;
	vector<string> subspl;
	TS_COUNTLOOP(y, TS::Map::AutoTileRowLength) {
		TilesetRow row;
		TS_COUNTLOOP(x, TS::Map::TileColumnCount) {
			row.push_back(CTileData());
			if(x == 0 && y == 0) {
				//�󔒃^�C���̒�`
				row[x].Hit = static_cast<int>(TS::Map::HitType::OK);
				row[x].InOut = static_cast<int>(TS::Map::InOutType::OKBottom) | static_cast<int>(TS::Map::InOutType::OKLeft) | static_cast<int>(TS::Map::InOutType::OKRight) | static_cast<int>(TS::Map::InOutType::OKTop);
				row[x].Priority = static_cast<int>(TS::Map::TileDrawPriority::Normal);
				row[x].Tag = 0;
				row[x].AnimPatternCount = 0;
			} else {
				this->gets(::buf, sizeof(::buf) - 1);	//�P�s�ǂݍ���

				//��؂�L���ŕ���
				Game.SplitString(spl, ::buf, "|");
				if(spl.size() >= 3) {
					//�^�C�����̐��f�[�^���琳�K�̃f�[�^�����
					row[x] = this->setData(spl[2]);

					//�������̒�������A�j���[�V�����p�^�[�����𓾂�
					row[x].AnimPatternCount = atoi(spl[1].substr(0, spl[1].find("x")).c_str());
				}
			}
		}
		this->activeTilesetData.push_back(row);
	}

	//�ȍ~�A�I�u�W�F�N�g�^�C���AFixedID|WxH
	CSize setSize;
	while(true) {
		this->gets(::buf, sizeof(::buf) - 1);		//�P�s�ǂݍ���
		if(this->isEOF()) {
			break;				//�I�[�܂œǂݍ���
		}

		//�I�u�W�F�N�g�^�C���̏c�������m�F
		Game.SplitString(spl, ::buf, "|");
		Game.SplitString(subspl, spl[1], "x");
		setSize.Set(
			atoi(subspl[0].c_str()),
			atoi(subspl[1].c_str())
			);

		TS_COUNTLOOP(y, setSize.Height) {
			TilesetRow row;
			this->gets(::buf, sizeof(::buf) - 1);	//�P�s�ǂݍ���

			//��؂�L���ŕ���
			Game.SplitString(spl, ::buf, ",");
			TS_COUNTLOOP(x, spl.size()) {
				//�^�C�����̐��f�[�^���琳�K�̃f�[�^�����
				row.push_back(this->setData(spl[x]));
			}
			this->activeTilesetData.push_back(row);
		}
	}
	this->closeFile();
}


//####################  ���݃A�N�e�B�u�ȃ^�C���Z�b�g�̃p���b�g�𐶐�����  ####################
CDatabase::CDBTileset::CTilePallet* CDatabase::CDBTileset::loadActiveTilesetGraph() {
	//�O��̃^�C���p���b�g�O���t�B�b�N��j������
	this->DisposeTilePallet();

	this->loadedPallet = new CTilePallet();
	auto activeTileRow = this->DBs.GetFromIndex(0)->GetRowFromID(this->ActiveTilesetID);

	//�T�C�Y�����擾
	auto palletSize = this->GetTilePalletSize();
	auto tileSize = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::TileSize);

	//�ǂݍ���
	string strbuf;
	vector<string> at;
	strbuf = *activeTileRow->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBTilesetColumnIndices::AutoTiles)).GetStringPTR();
	Game.SplitString(at, strbuf, "|");
	vector<string> obj;
	strbuf = *activeTileRow->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBTilesetColumnIndices::ObjTiles)).GetStringPTR();
	Game.SplitString(obj, strbuf, "|");

	//�I�u�W�F�N�g�^�C���̐�������
	vector<CSize> sizeList;
	vector<int> objHandles;
	int tilesetHeight = tileSize * TS::Map::AutoTileRowLength;

	//�I�u�W�F�N�g�^�C���̃O���t�B�b�N�Ƃ��̃T�C�Y��ǂݍ���
	TS_COUNTLOOP(i, obj.size()) {
		int x, y;
		objHandles.push_back(DxLib::LoadGraph((TS::DirectoryNames::PrePath + obj[i]).c_str()));
		DxLib::GetGraphSize(objHandles[i], &x, &y);
		sizeList.push_back(CSize(x, y));
		tilesetHeight += y;
	}

	//�^�C���p���b�g�p�̃X�N���[���𐶐����A�ꎞ�I�ɕ`����ύX����
	this->loadedPallet->TilesetHandle = DxLib::MakeScreen(tileSize * palletSize.Width, tilesetHeight, true);
	DxLib::SetDrawScreen(this->loadedPallet->TilesetHandle);
	DxLib::SetUsePremulAlphaConvertLoad(true);

	//�I�[�g�^�C���ǂݍ��݁F����p���b�g�ɕ`�悵�A�ʌɂ��n���h�����T���Ă���
	TS_COUNTLOOP(x, TS::Map::TileColumnCount) {
		TS_COUNTLOOP(y, TS::Map::AutoTileRowLength) {
			this->loadedPallet->AutoTiles[x + y * TS::Map::TileColumnCount] = -1;
			TS_CHECKINDEX(x + y * TS::Map::TileColumnCount, 1, at.size()) {
				//spl�̗v�f���́A����̓����^�C���𔲂��̂ň���Ȃ����Ƃɒ���
				this->loadedPallet->AutoTiles[x + y * TS::Map::TileColumnCount] = Game.Material.getGraphics()->Add(at[x + y * TS::Map::TileColumnCount - 1]);
				DxLib::DrawGraph(x * tileSize, y * tileSize, this->loadedPallet->AutoTiles[x + y * TS::Map::TileColumnCount], true);
			}
		}
	}

	//�I�u�W�F�N�g�^�C���͈�A�̃O���t�B�b�N�Ƃ��ĕ`��
	int amount = tileSize * TS::Map::AutoTileRowLength;
	TS_COUNTLOOP(i, obj.size()) {
		//�`�悵���炷���ɔj������
		DxLib::DrawGraph(0, amount, objHandles[i], true);
		DxLib::DeleteGraph(objHandles[i]);
		amount += sizeList[i].Height;
	}

	//�`�������ɖ߂��Ċ�������
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	TS_NOBLEND;
	DxLib::SetUsePremulAlphaConvertLoad(false);

	return this->loadedPallet;
}
