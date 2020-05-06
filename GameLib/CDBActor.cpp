//###################################################################
//			パーティアクター データベース
//###################################################################
#include "Header.h"


//####################  読み込み処理  ####################
bool CDatabase::CDBActor::Load(string fileName) {
	//ファイルからは読み込まず、パーティキャラFDBから取り込む
	auto db = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char));
	auto chars = db->GetSubDB(0);
	this->fileName = db->GetDBFileName();

	this->DB.Clear();
	TS_COUNTLOOP(i, chars->GetRowCount()) {
		this->DB.Add(CDBActorData::CreateActorFromChar(*chars->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR()));
	}
	return true;
}


//####################  セーブデータから読み込み  ####################
void CDatabase::CDBActor::Load(vector<string>& buf) {
	//メンバー変数、クラス、装備、スキル、間接効果を復元して適用する
	int length = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, length) {
		CDBActorData newData = CDBActorData::Load(buf);
		if(this->DB.Exists(newData.ID)) {
			//FDBに存在するキャラのみ復元する
			*this->DB.GetFromID(newData.ID) = newData;
		}
		Game.PopHead(buf);		//区切り記号の読み捨て
	}
}


//####################  セーブデータに書き込み  ####################
void CDatabase::CDBActor::Save(string& buf) {
	//メンバー変数、クラス、装備、スキル、間接効果を保存する
	buf += to_string(this->DB.GetCount()) + "\n";
	for(auto& ch : this->DB.Items) {
		ch.Save(buf);
	}
}


//####################  ログに出力  ####################
void CDatabase::CDBActor::PrintData() {
	TS_LOG("\nDB: パーティアクター情報");
	for(auto& ch : this->DB.Items) {
		ch.PrintData();
	}
}


//####################  破棄処理  ####################
void CDatabase::CDBActor::Dispose() {
	this->DB.Clear();
}


//####################  指定したIDのキャラを初期化  ####################
void CDatabase::CDBActor::InitData(int ID) {
	if(this->DB.Exists(ID)) {
		*this->DB.GetFromID(ID) = CDBActorData::CreateActorFromChar(ID);
	}
}


//####################  指定したインデックスのキャラを初期化  ####################
void CDatabase::CDBActor::InitDataFromIndex(int index) {
	TS_CHECKINDEX(index, 0, this->DB.GetCount()) {
		int ID = this->DB.GetFromIndex(index)->ID;
		*this->DB.GetFromIndex(index) = CDBActorData::CreateActorFromChar(ID);
	}
}
