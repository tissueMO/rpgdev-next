//###################################################################
//			単一データベース
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  分割バッファから行を生成  ####################
void CDatabase::CDBBaseOne::LoadRow(const vector<string>* split) {
	auto newRow = DBRow();

	//列の数だけセルを生成して代入する
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		newRow.push_back(CDBCell(this->columnTypes.at(i), split->at(i)));
	}

	this->AddRow(newRow);
}


//####################  列の型名リストをセット  ####################
void CDatabase::CDBBaseOne::SetColumnTypes(const vector<string>* split) {
	this->columnTypes.clear();
	for(auto& type : *split) {
		this->columnTypes.push_back(type);
	}
}


//####################  列名リストをセット  ####################
void CDatabase::CDBBaseOne::SetColumnNames(const vector<string>* split) {
	this->columnNames.clear();
	for(auto& name : *split) {
		this->columnNames.push_back(name);
	}
}


//####################  保存用テキストを生成  ####################
void CDatabase::CDBBaseOne::SaveRow(string &strbuf) {
	//全行実行
	strbuf = "";
	for(auto& row : this->rows) {
		//全列実行
		TS_COUNTLOOP(i, this->GetColumnCount()) {
			if(i > 0) {
				strbuf += "\t";
			}
			strbuf += row[i].ToString();
		}
		strbuf += "\n";
	}
}


//####################  行を追加  ####################
void CDatabase::CDBBaseOne::AddRow(DBRow row) {
	this->rows.push_back(row);
}


//####################  指定インデックスの行を削除  ####################
void CDatabase::CDBBaseOne::DeleteRowFromIndex(int rowIndex) {
	if(rowIndex < 0 || this->rows.size() < rowIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "データベース [%s / Idx: %d] において、範囲外のインデックスのデータベース行 [%d] を削除しようとしました", this->Name.c_str(), this->ID, rowIndex);
	} else {
		this->rows.erase(this->rows.begin() + rowIndex);
	}
}


//####################  指定IDの行を削除  ####################
void CDatabase::CDBBaseOne::DeleteRowFromID(int rowID) {
	TS_COUNTLOOP(rowIndex, this->rows.size()) {
		if(*this->rows[rowIndex][static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR() == rowID) {
			this->DeleteRowFromIndex(rowIndex);
			return;
		}
	}
}


//####################  行をクリア  ####################
void CDatabase::CDBBaseOne::ClearRow() {
	vector<DBRow>().swap(this->rows);
}


//####################  指定行に列を追加  ####################
//列名はcellの中に格納されている
void CDatabase::CDBBaseOne::AddCell(int rowIndex, CDBCell* cell) {
	this->rows[rowIndex].push_back(*cell);
}


//####################  列を追加  ####################
void CDatabase::CDBBaseOne::AddColumn(string columnName) {
	this->columnNames.push_back(columnName);
}


//####################  列をクリア  ####################
void CDatabase::CDBBaseOne::ClearColumn() {
	vector<string>().swap(this->columnNames);
	vector<string>().swap(this->columnTypes);
}


//####################  列名からセル情報を取得  ####################
CDatabase::CDBCell* CDatabase::CDBBaseOne::GetCell(int rowID, string columnName) {
	TS_COUNTLOOP(rowIndex, this->rows.size()) {
		if(*this->rows[rowIndex][static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR() == rowID) {
			return &this->rows[rowIndex][this->GetColumnIndex(columnName)];
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "データベース [%s / Idx: %d] において、存在しないセル [ID: %d / 列名: \"%s\"] が参照されました。", this->Name.c_str(), this->ID, rowID, columnName.c_str());
	return nullptr;
}


//####################  列インデックスからセル情報を取得  ####################
CDatabase::CDBCell* CDatabase::CDBBaseOne::GetCellFromIndex(int rowIndex, int columnIndex) {
	if(rowIndex < 0 || this->GetRowCount() <= rowIndex || columnIndex < 0 || this->GetColumnCount() <= columnIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "データベース [%s / Idx: %d] において、範囲外の行と列 [%d, %d] が参照されました。", this->Name.c_str(), this->ID, rowIndex, columnIndex);
		return nullptr;
	} else {
		return &this->rows[rowIndex][columnIndex];
	}
}


//####################  列インデックスから列名を取得  ####################
string CDatabase::CDBBaseOne::GetColumnName(int columnIndex) {
	if(columnIndex < 0 || this->GetColumnCount() <= columnIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "データベース [%s / Idx: %d] において、範囲外の列インデックス [%d] が参照されました。", this->Name.c_str(), this->ID, columnIndex);
		return "";
	} else {
		return this->columnNames.at(columnIndex);
	}
}


//####################  列インデックスから列の型名を取得  ####################
string CDatabase::CDBBaseOne::GetColumnType(int columnIndex) {
	if(columnIndex < 0 || this->GetColumnCount() <= columnIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "データベース [%s / Idx: %d] において、範囲外の列インデックス [%d] が参照されました。", this->Name.c_str(), this->ID, columnIndex);
		return "";
	} else {
		return this->columnTypes[columnIndex];
	}
}


//####################  指定IDの行を取得  ####################
CDatabase::DBRow* CDatabase::CDBBaseOne::GetRowFromID(int rowID) {
	TS_COUNTLOOP(i, this->GetRowCount()) {
		if((*this->rows[i][static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR()) == rowID) {
			return &this->rows[i];
		}
	}
	//NOTE: 行の取得はC++でしか行われないため、IDによる取得では警告を出さない。
	return nullptr;
}


//####################  指定インデックスの行を取得  ####################
CDatabase::DBRow* CDatabase::CDBBaseOne::GetRowFromIndex(int rowIndex) {
	if(rowIndex < 0 || this->GetRowCount() <= rowIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "データベース [%s / Idx: %d] において、範囲外の行インデックス [%d] が参照されました。", this->Name.c_str(), this->ID, rowIndex);
		return nullptr;
	} else {
		return &this->rows.at(rowIndex);
	}
}


//####################  行数を取得  ####################
int CDatabase::CDBBaseOne::GetRowCount() {
	try {
		return this->rows.size();
	} catch(...) {
		return 0;
	}
}


//####################  列数を取得  ####################
int CDatabase::CDBBaseOne::GetColumnCount() {
	return this->columnNames.size();
}


//####################  列名から列インデックスを取得  ####################
int CDatabase::CDBBaseOne::GetColumnIndex(string columnName) {
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		if(columnName == this->columnNames.at(i)) {
			return i;
		}
	}
	return -1;
}


//####################  可視IDからFixedIDに変換  ####################
int CDatabase::CDBBaseOne::ConvertToFixedID(int visibleID) {
	TS_COUNTLOOP(i, this->GetRowCount()) {
		if(*this->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR() == visibleID) {
			return *this->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR();
		}
	}
	return -1;
}


//####################  FixedIDから可視IDに変換  ####################
int CDatabase::CDBBaseOne::ConvertToVisibleID(int fixedID) {
	DBRow* row = this->GetRowFromID(fixedID);
	if(row == nullptr) {
		return -1;
	} else {
		return *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID)).GetIntegerPTR();
	}
}


//####################  全体を初期化  ####################
void CDatabase::CDBBaseOne::InitDB() {
	//全行実行
	for(auto& row : this->rows) {
		this->InitRow(*row[static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR());
	}
}


//####################  行IDを指定して行を初期化  ####################
void CDatabase::CDBBaseOne::InitRow(int rowID) {
	//全列実行
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		this->InitCell(rowID, this->columnNames[i].c_str());
	}
}


//####################  行インデックスを指定して指定行を初期化  ####################
void CDatabase::CDBBaseOne::InitRowFromIndex(int rowIndex) {
	//全列実行
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		this->InitCellFromIndex(rowIndex, i);
	}
}


//####################  列名から指定ID行の指定セルを初期化  ####################
void CDatabase::CDBBaseOne::InitCell(int rowID, string columnName) {
	this->GetCell(rowID, columnName)->Init();
}


//####################  列インデックスから指定インデックス行の指定セルを初期化  ####################
void CDatabase::CDBBaseOne::InitCellFromIndex(int rowIndex, int columnIndex) {
	this->GetCellFromIndex(rowIndex, columnIndex)->Init();
}

