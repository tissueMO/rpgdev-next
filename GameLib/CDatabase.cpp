//###################################################################
//			データベースの統括クラス
//###################################################################
//dbType:FDB/VDB/UDB, dbIndex:エディターのDBタブに対応, subDB:エディターのDBタブ内のDBsコレクションに対応/UDBはFixedID
#include "Header.h"


//####################  コンストラクター  ####################
CDatabase::CDatabase() {
	//FDBとVDBの要素を生成する：ただしここでは読み込まない
	CDBBase* newDB;

	//固定データベース
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

	//可変データベース
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


//####################  デストラクター  ####################
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


//####################  全データベースを読み込み ####################
void CDatabase::LoadAll() {
	TS_LOG("すべてのデータベースを読み込みます...");

	TS_COUNTLOOP(i, static_cast<int>(TS::DB::DBType::Count)) {
		this->ReloadDB(i, false);
	}

	TS_LOG("すべてのデータベースの読み込みが完了しました。");
}


//####################  指定データベースの列名から指定ID行のセルを取得  ####################
CDatabase::CDBCell* CDatabase::GetCell(int dbType, int dbIndex, int subDB, int rowID, string columnName) {
	return this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->GetCell(rowID, columnName);
}


//####################  指定データベースの列インデックスから指定ID行のセルを取得  ####################
CDatabase::CDBCell* CDatabase::GetCellFromIndex(int dbType, int dbIndex, int subDB, int rowIndex, int columnIndex) {
	return this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->GetCellFromIndex(rowIndex, columnIndex);
}


//####################  指定種別のデータベースを再読込  ####################
void CDatabase::ReloadDB(int dbType, bool includeFDBSystem) {
	switch(dbType) {
		case static_cast<int>(TS::DB::DBType::FDB):
			//固定データベース
			TS_LOG("各種個別変数は可変データベースを読み込むまで適用されません。");
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::FDBIndices::Count)) {
				if(i == static_cast<int>(TS::DB::FDBIndices::System) && !includeFDBSystem) {
					//システムDBを無視する設定の場合
					continue;
				}
				this->FDB[i]->Load(TS::FileNames::FDBs[i]);
			}

			//頻繁に使う設定項目をオブジェクト変数にセットする
			Game.WindowSize = this->getFDBSystem()->GetSize(TS::DB::System::WindowSize);
			Game.EnabledSystemResource = this->getFDBSystem()->GetBoolean(TS::DB::System::EnabledSystemResource);
			break;

		case static_cast<int>(TS::DB::DBType::VDB):
			//可変データベース
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::VDBIndices::Count)) {
				this->VDB[i]->Load(TS::FileNames::VDBs[i]);
			}

			//個別変数リストをセットアップ
			int colIndex, valID, subDB;
			TS_COUNTLOOP(i, static_cast<int>(TS::DB::FDBIndices::Count)) {
				colIndex = this->FDB[i]->GetSubDB(0)->GetColumnIndex(TS::DB::ColumnNameSelfValue);
				if(colIndex == -1) {
					continue;		//個別変数を持たないデータベースは処理しない
				}

				//対象データベース番号を取得する
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
					continue;		//データベース番号が取得できなかったらスキップする
				}

				//まだセットされていない個別変数があれば、初期値でセットする
				TS_COUNTLOOP(row, this->FDB[i]->GetSubDB(0)->GetRowCount()) {
					auto selfVals = this->FDB[i]->GetSubDB(0)->GetCellFromIndex(row, colIndex)->GetIDValues();
					if (selfVals == nullptr) {
						continue;
					}
					TS_COUNTLOOP(m, this->VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(subDB)->GetRowCount()) {
						valID = *(*this->VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(subDB)->GetRowFromIndex(m))[static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR();
						if(!selfVals->Exists(valID)) {
							//セットされていないので追加登録する
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
			//ユーザーデータベース
			this->UDB.Load(TS::FileNames::UserDB);
			break;
	}
}


//####################  VDB/UDBのみ：指定種別のデータベースをすべて初期化  ####################
void CDatabase::InitDBType(int dbType) {
	switch(dbType) {
		case static_cast<int>(TS::DB::DBType::FDB):
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "固定データベースは初期化を実行できません。");
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


//####################  VDB/UDBのみ：指定データベースをすべて初期化  ####################
void CDatabase::InitDB(int dbType, int dbIndex, int subDB) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "固定データベース [%d] は初期化を実行できません。", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitDB();
}


//####################  VDB/UDBのみ：指定データベースの指定行を初期化  ####################
void CDatabase::InitRow(int dbType, int dbIndex, int subDB, int rowID) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "固定データベース [%d] は初期化を実行できません。", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitRow(rowID);
}


//####################  VDB/UDBのみ：指定データベースの列名から指定セルを初期化  ####################
void CDatabase::InitCell(int dbType, int dbIndex, int subDB, int rowID, string columnName) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "固定データベース [%d] は初期化を実行できません。", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitCell(rowID, columnName);
}


//####################  VDB/UDBのみ：指定データベースの列インデックスから指定セルを初期化  ####################
void CDatabase::InitCellFromIndex(int dbType, int dbIndex, int subDB, int rowIndex, int columnIndex) {
	if(dbType == static_cast<int>(TS::DB::DBType::FDB)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "固定データベース [%d] は初期化を実行できません。", dbIndex);
		return;
	}
	this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->InitCellFromIndex(rowIndex, columnIndex);
}


//####################  アイテムIDから種別を取得する  ####################
//返値は正常に取得できたかどうか
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


//####################  可視IDからFixedIDに変換する：FDB/VDB/UDB対応  ####################
//UDBは第２引数が無効となる
int CDatabase::GetFixedID(int dbType, int dbIndex, int subDB, int visibleID) {
	return this->GetDB(dbType, dbIndex)->GetSubDB(subDB)->ConvertToFixedID(visibleID);
}


//####################  指定したデータベースを取得：FDB/VDB/UDB対応  ####################
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

	Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "無効なデータベース [種別: %d / Idx: %d] です。\n処理を続行できないため、ゲームプログラムを終了します。", dbType, dbIndex);
	return nullptr;
}


//####################  拡張したデータベースへの個別インスタンスを取得  ####################
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
