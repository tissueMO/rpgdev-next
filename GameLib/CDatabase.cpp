//###################################################################
//			�f�[�^�x�[�X�̓����N���X
//###################################################################
//dbType:FDB/VDB/UDB, dbIndex:�G�f�B�^�[��DB�^�u�ɑΉ�, subDB:�G�f�B�^�[��DB�^�u����DBs�R���N�V�����ɑΉ�/UDB��FixedID
#include "Header.h"


//####################  �R���X�g���N�^�[  ####################
CDatabase::CDatabase() {
	//FDB��VDB�̗v�f�𐶐�����F�����������ł͓ǂݍ��܂Ȃ�
	CDBBase* newDB;

	//�Œ�f�[�^�x�[�X
	this->FDB.clear();
	TS_COUNTLOOP(i, static_cast<int>(TS::DB::FDBIndices::Count)) {
		switch((TS::DB::FDBIndices)i) {
			case TS::DB::FDBIndices::System:
				newDB = new CDBSystem();
				break;
			case TS::DB::FDBIndices::Effect:
				newDB = new CDBEffect();
				break;
			case TS::DB::FDBIndices::Material:
				newDB = new CDBMaterial();
				break;
			case TS::DB::FDBIndices::Tileset:
				newDB = new CDBTileset();
				break;
			default:
				newDB = new CDBBase();
				break;
		}
		this->FDB.push_back(newDB);
	}

	//�σf�[�^�x�[�X
	this->VDB.clear();
	TS_COUNTLOOP(i, static_cast<int>(TS::DB::VDBIndices::Count)) {
		switch((TS::DB::VDBIndices)i) {
			case TS::DB::VDBIndices::Value:
				newDB = new CDBValue();
				break;
			case TS::DB::VDBIndices::Party:
				newDB = new CDBParty();
				break;
			case TS::DB::VDBIndices::Actor:
				newDB = new CDBActor();
				break;
			default:
				newDB = new CDBBase();
				break;
		}
		this->VDB.push_back(newDB);
	}
}


//####################  �f�X�g���N�^�[  ####################
CDatabase::~CDatabase() {
	for (auto fdb : this->FDB) {
		if (fdb != nullptr) {
			fdb->Dispose();
			delete fdb;
		}
	}
	for(auto vdb : this->VDB) {
		if (vdb != nullptr) {
			vdb->Dispose();
			delete vdb;
		}
	}
	this->UDB.Dispose();
}


//####################  �S�f�[�^�x�[�X��ǂݍ��� ####################
void CDatabase::LoadAll() {
	TS_LOG("���ׂẴf�[�^�x�[�X��ǂݍ��݂܂�...");

	TS_COUNTLOOP(i, static_cast<int>(TS::DB::DBType::Count)) {
		this->ReloadDB(i, false);
	}

	TS_LOG("���ׂẴf�[�^�x�[�X�̓ǂݍ��݂��������܂����B");
}


//####################  �w��f�[�^�x�[�X�̗񖼂���w��ID�s�̃Z�����擾  ####################
CDatabase::CDBCell* CDatabase::GetCell(int dbType, int dbIndex, int subDB, int rowID, string columnName) {
	return this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->GetCell(rowID, columnName);
}


//####################  �w��f�[�^�x�[�X�̗�C���f�b�N�X����w��ID�s�̃Z�����擾  ####################
CDatabase::CDBCell* CDatabase::GetCellFromIndex(int dbType, int dbIndex, int subDB, int rowIndex, int columnIndex) {
	return this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->GetCellFromIndex(rowIndex, columnIndex);
}


//####################  �w���ʂ̃f�[�^�x�[�X���ēǍ�  ####################
void CDatabase::ReloadDB(int dbType, bool includeFDBSystem) {
	switch(dbType) {
		case static_cast<int>(TS::DB::DBType::FDB):
			//�Œ�f�[�^�x�[�X
			TS_LOG("�e��ʕϐ��͉σf�[�^�x�[�X��ǂݍ��ނ܂œK�p����܂���B");
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::FDBIndices::Count)) {
				if(i == static_cast<int>(TS::DB::FDBIndices::System) && !includeFDBSystem) {
					//�V�X�e��DB�𖳎�����ݒ�̏ꍇ
					continue;
				}
				this->FDB[i]->Load(TS::FileNames::FDBs[i]);
			}

			//�p�ɂɎg���ݒ荀�ڂ��I�u�W�F�N�g�ϐ��ɃZ�b�g����
			Game.WindowSize = this->getFDBSystem()->GetSize(TS::DB::System::WindowSize);
			Game.EnabledSystemResource = this->getFDBSystem()->GetBoolean(TS::DB::System::EnabledSystemResource);
			break;

		case static_cast<int>(TS::DB::DBType::VDB):
			//�σf�[�^�x�[�X
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::VDBIndices::Count)) {
				this->VDB[i]->Load(TS::FileNames::VDBs[i]);
			}

			//�ʕϐ����X�g���Z�b�g�A�b�v
			int colIndex, valID, subDB;
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::FDBIndices::Count)) {
				colIndex = this->FDB[i]->GetSubDB(0)->GetColumnIndex(TS::DB::ColumnNameSelfValue);
				if(colIndex == -1) {
					continue;		//�ʕϐ��������Ȃ��f�[�^�x�[�X�͏������Ȃ�
				}

				//�Ώۃf�[�^�x�[�X�ԍ����擾����
				subDB = -1;
				switch((TS::DB::FDBIndices)i) {
					case TS::DB::FDBIndices::Char:
					case TS::DB::FDBIndices::Enemy:
						subDB = static_cast<int>(TS::DB::DBValueIndices::SelfActor);
						break;
					case TS::DB::FDBIndices::Class:
						subDB = static_cast<int>(TS::DB::DBValueIndices::SelfClass);
						break;
					case TS::DB::FDBIndices::Skill:
					case TS::DB::FDBIndices::Item:
						subDB = static_cast<int>(TS::DB::DBValueIndices::SelfUse);
						break;
					case TS::DB::FDBIndices::State:
						subDB = static_cast<int>(TS::DB::DBValueIndices::SelfState);
						break;
				}
				if(subDB == -1) {
					continue;		//�f�[�^�x�[�X�ԍ����擾�ł��Ȃ�������X�L�b�v����
				}

				//�܂��Z�b�g����Ă��Ȃ��ʕϐ�������΁A�����l�ŃZ�b�g����
				TS_COUNTLOOP(row, this->FDB[i]->GetSubDB(0)->GetRowCount()) {
					auto selfVals = this->FDB[i]->GetSubDB(0)->GetCellFromIndex(row, colIndex)->GetIDValues();
					if (selfVals == nullptr) {
						continue;
					}
					TS_COUNTLOOP(m, this->VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(subDB)->GetRowCount()) {
						valID = *(*this->VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(subDB)->GetRowFromIndex(m))[static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR();
						if(!selfVals->Exists(valID)) {
							//�Z�b�g����Ă��Ȃ��̂Œǉ��o�^����
							selfVals->SetValue(
								valID,
								*(*this->VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(subDB)->GetRowFromIndex(m))[static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)].GetIntegerPTR()
								);
						}
					}
				}
			}
			break;

		case static_cast<int>(TS::DB::DBType::UDB):
			//���[�U�[�f�[�^�x�[�X
			this->UDB.Load(TS::FileNames::UserDB);
			break;
	}
}


//####################  VDB/UDB�̂݁F�w���ʂ̃f�[�^�x�[�X�����ׂď�����  ####################
void CDatabase::InitDBType(int dbType) {
	switch(dbType) {
		case static_cast<int>(TS::DB::DBType::FDB):
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "�Œ�f�[�^�x�[�X�͏����������s�ł��܂���B");
			break;

		case static_cast<int>(TS::DB::DBType::VDB):
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::VDBIndices::Count)) {
				this->VDB[i]->Load(TS::FileNames::VDBs[i]);
			}
			break;

		case static_cast<int>(TS::DB::DBType::UDB):
			this->UDB.Load(TS::FileNames::UserDB);
			break;
	}
}


//####################  VDB/UDB�̂݁F�w��f�[�^�x�[�X�����ׂď�����  ####################
void CDatabase::InitDB(int dbType, int dbIndex, int subDB) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "�Œ�f�[�^�x�[�X [%d] �͏����������s�ł��܂���B", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitDB();
}


//####################  VDB/UDB�̂݁F�w��f�[�^�x�[�X�̎w��s��������  ####################
void CDatabase::InitRow(int dbType, int dbIndex, int subDB, int rowID) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "�Œ�f�[�^�x�[�X [%d] �͏����������s�ł��܂���B", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitRow(rowID);
}


//####################  VDB/UDB�̂݁F�w��f�[�^�x�[�X�̗񖼂���w��Z����������  ####################
void CDatabase::InitCell(int dbType, int dbIndex, int subDB, int rowID, string columnName) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "�Œ�f�[�^�x�[�X [%d] �͏����������s�ł��܂���B", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitCell(rowID, columnName);
}


//####################  VDB/UDB�̂݁F�w��f�[�^�x�[�X�̗�C���f�b�N�X����w��Z����������  ####################
void CDatabase::InitCellFromIndex(int dbType, int dbIndex, int subDB, int rowIndex, int columnIndex) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "�Œ�f�[�^�x�[�X [%d] �͏����������s�ł��܂���B", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitCellFromIndex(rowIndex, columnIndex);
}


//####################  �A�C�e��ID�����ʂ��擾����  ####################
//�Ԓl�͐���Ɏ擾�ł������ǂ���
bool CDatabase::GetItemTypes(int itemID, CIntegerArray* typeIDs, CIntegerArray* subTypeIDs) {
	auto db = this->FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0);
	if(db->GetRowFromID(itemID) == nullptr) {
		return false;
	}

	if(typeIDs != nullptr) {
		*typeIDs = *db->GetCell(itemID, TS::DB::ColumnNameItemType)->GetIntegers();
	}
	if(subTypeIDs != nullptr) {
		*subTypeIDs = *db->GetCell(itemID, TS::DB::ColumnNameItemSubType)->GetIntegers();
	}
	return true;
}


//####################  ��ID����FixedID�ɕϊ�����FFDB/VDB/UDB�Ή�  ####################
//UDB�͑�Q�����������ƂȂ�
int CDatabase::GetFixedID(int dbType, int dbIndex, int subDB, int visibleID) {
	return this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->ConvertToFixedID(visibleID);
}


//####################  �w�肵���f�[�^�x�[�X���擾�FFDB/VDB/UDB�Ή�  ####################
CDatabase::CDBBase* CDatabase::GetDB(int dbType, int dbIndex) {
	switch((TS::DB::DBType)dbType) {
		case TS::DB::DBType::FDB:
			TS_CHECKINDEX(dbIndex, 0, static_cast<int>(TS::DB::FDBIndices::Count)) {
				return this->FDB[dbIndex];
			}

		case TS::DB::DBType::VDB:
			TS_CHECKINDEX(dbIndex, 0, static_cast<int>(TS::DB::VDBIndices::Count)) {
				return this->VDB[dbIndex];
			}

		case TS::DB::DBType::UDB:
			return &this->UDB;
	}

	Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�����ȃf�[�^�x�[�X [���: %d / Idx: %d] �ł��B\n�����𑱍s�ł��Ȃ����߁A�Q�[���v���O�������I�����܂��B", dbType, dbIndex);
	return nullptr;
}


//####################  �g�������f�[�^�x�[�X�ւ̌ʃC���X�^���X���擾  ####################
CDatabase::CDBEffect* CDatabase::getFDBEffect() {
	return dynamic_cast<CDBEffect*>(this->FDB[static_cast<int>(TS::DB::FDBIndices::Effect)]);
}
CDatabase::CDBMaterial* CDatabase::getFDBMaterial() {
	return dynamic_cast<CDBMaterial*>(this->FDB[static_cast<int>(TS::DB::FDBIndices::Material)]);
}
CDatabase::CDBSystem* CDatabase::getFDBSystem() {
	return dynamic_cast<CDBSystem*>(this->FDB[static_cast<int>(TS::DB::FDBIndices::System)]);
}
CDatabase::CDBTileset* CDatabase::getFDBTileset() {
	return dynamic_cast<CDBTileset*>(this->FDB[static_cast<int>(TS::DB::FDBIndices::Tileset)]);
}
CDatabase::CDBValue* CDatabase::getVDBValue() {
	return dynamic_cast<CDBValue*>(this->VDB[static_cast<int>(TS::DB::VDBIndices::Value)]);
}
CDatabase::CDBParty* CDatabase::getVDBParty() {
	return dynamic_cast<CDBParty*>(this->VDB[static_cast<int>(TS::DB::VDBIndices::Party)]);
}
CDatabase::CDBActor* CDatabase::getVDBActor() {
	return dynamic_cast<CDBActor*>(this->VDB[static_cast<int>(TS::DB::VDBIndices::Actor)]);
}
