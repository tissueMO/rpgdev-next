//###################################################################
//			ユーザー定義のデータベース
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  読み込み処理  ####################
bool CDatabase::CDBUser::Load(string fileName) {
	//デフォルトの読み込み処理が先行する
	CDBBase::Load(fileName);

	//ユーザーDBの中身をすべてロードする
	this->UserDBs.Clear();
	TS_COUNTLOOP(row, this->GetSubDB(0)->GetRowCount()) {
		if(!this->openFile(Game.Format(TS::FileNames::UserDBData, *this->GetSubDB(0)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR()))) {
			continue;		//読み込みに失敗した場合はスキップ
		}

		//データベース情報をセット
		auto db = CDatabase::CDBBaseOne();
		db.ID = *this->GetSubDB(0)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR();
		db.Name = *this->GetSubDB(0)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR();
		this->UserDBs.Add(db);
		auto addedDB = this->UserDBs.GetFromIndex(this->UserDBs.GetCount() - 1);

		//１行目：列数
		this->readNextRow();
		int colCount = atoi(this->stringBuffer.c_str());

		//２行目～：１行に１列の列情報が格納されているが、エディター用なので読み捨てる
		vector<string> names;
		TS_COUNTLOOP(i, colCount) {
			this->readNextRow();
		}

		//列型名がタブ区切り
		this->readNextRow();
		addedDB->SetColumnTypes(&this->splitBuffer);

		//列表示名がタブ区切り
		this->readNextRow();
		addedDB->SetColumnNames(&this->splitBuffer);

		//以降、行情報
		while(this->readNextRow() >= 1) {
			addedDB->LoadRow(&this->splitBuffer);
		}

		this->closeFile();
	}
	return true;
}


//####################  読み込み処理  ####################
//ユーザーDBの中身、行データのみを読み出す
void CDatabase::CDBUser::Load(vector<string>& vectorbuf) {
	int id, count;
	string temp;
	vector<string> split;

	//先頭のデータベース数を確認する
	count = atoi(Game.PopHead(vectorbuf).c_str());
	TS_COUNTLOOP(i, count) {
		id = atoi(Game.PopHead(vectorbuf).c_str());
		this->UserDBs.GetFromID(id)->ClearRow();
		while(true) {
			temp = Game.PopHead(vectorbuf);
			if(temp.empty()) {
				break;		//空行までを一つのデータベースと認識する
			}
			Game.SplitString(split, temp, "\t");
			this->UserDBs.GetFromID(id)->LoadRow(&split);
		}
	}
}


//####################  書き込み処理  ####################
//ユーザーDBの中身、行データのみを書き出す
void CDatabase::CDBUser::Save(string& strbuf) {
	//先頭にデータベース数を入れる
	strbuf += Game.Format("%d\n", this->UserDBs.GetCount());
	TS_COUNTLOOP(i, this->UserDBs.GetCount()) {
		//各DBの先頭にデータベースIDを入れる
		strbuf += Game.Format("%d\n", *this->GetSubDB(0)->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
		TS_COUNTLOOP(row, this->UserDBs.GetFromIndex(i)->GetRowCount()) {
			this->stringBuffer = "";
			TS_COUNTLOOP(col, this->UserDBs.GetFromIndex(i)->GetColumnCount()) {
				this->stringBuffer += this->UserDBs.GetFromIndex(i)->GetCellFromIndex(row, col)->ToString();
				if(col < this->UserDBs.GetFromIndex(i)->GetColumnCount() - 1) {
					this->stringBuffer += "\t";
				}
			}
			strbuf += this->stringBuffer + "\n";
		}
		strbuf += "\n";
	}
}


//####################  ユーザーDBへのアクセサー  ####################
CDatabase::CDBBaseOne* CDatabase::CDBUser::GetUserDB(int id) {
	return this->UserDBs.GetFromID(id);
}


//####################  データベースの中身をログとして出力する  ####################
void CDatabase::CDBUser::PrintData() {
	TS_LOG("\nDB: " + this->fileName);
	for(auto& db : this->UserDBs.Items) {
		TS_LOG("\n[" + db.Name + "]");

		//要素を出力する
		string strbuf;
		for(int row = -1; row < db.GetRowCount(); row++) {
			strbuf = "";
			TS_COUNTLOOP(col, db.GetColumnCount()) {
				if(row == -1) {
					//列名
					strbuf += db.GetColumnName(col) + "(" + db.GetColumnType(col) + ")";
				} else {
					//要素の値
					strbuf += db.GetCellFromIndex(row, col)->ToString();
				}
				strbuf += "\t";
			}
			TS_LOG(strbuf);
		}
	}
}


//####################  破棄処理  ####################
void CDatabase::CDBUser::Dispose() {
	this->UserDBs.Clear();
}
