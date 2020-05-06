//###################################################################
//			データベースの基底クラス
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  破棄処理  ####################
void CDatabase::CDBBase::Dispose() {
	//行・列を削除
	for(auto& db : this->DBs.Items) {
		db.ClearRow();
		db.ClearColumn();
	}
	this->DBs.Clear();
}


//####################  ファイルから読み込む  ####################
//データベースファイルを読み込む際に呼び出されるメソッド
bool CDatabase::CDBBase::Load(string fileName) {
	TS_LOG("データベースの読み込みを開始します...%s", fileName.c_str());
	if(!this->openFile(fileName)) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "データベースの読み込みに失敗しました。\n>%s", fileName.c_str());
		return false;
	}

	//全サブDB読み込み
	this->DBs.Clear();
	this->fileName = fileName;
	for(int i = 0; this->isEOF() == false; i++) {
		auto newDB = CDatabase::CDBBaseOne();
		newDB.ID = i;				//サブDBのIDはインデックスに一致させる
		newDB.Name = fileName;		//サブDBの名前はファイル名にする
		this->loadOneDB(&newDB);
		this->DBs.Add(newDB);
	}

	//NOTE: 最後に余分な空行があるために余分なサブDBが追加されてしまうのが避けられないため、ここでデータを落とす
	//this->DBs.DeleteFromIndex(this->DBs.GetCount() - 1);

	this->closeFile();
	return true;
}


//####################  ファイルから読み込む  ####################
//セーブデータから読み込む場合に呼び出されるメソッド
void CDatabase::CDBBase::Load(vector<string>& vectorbuf) {
	//既定では処理を行わない
}


//####################  データベースの中身をログとして出力する  ####################
void CDatabase::CDBBase::PrintData() {
	TS_LOG("\nDB: " + this->fileName);
	TS_COUNTLOOP(i, this->DBs.GetCount()) {
		auto db = this->DBs.GetFromIndex(i);
		//複数のサブDBを持っている場合はインデックスを示す
		if(this->DBs.GetCount() > 1) {
			TS_LOG("\nSubDB[" + std::to_string(i) + "]:");
		}

		//要素を出力する
		string strbuf;
		for(int row = -1; row < db->GetRowCount(); row++) {
			strbuf = "";
			TS_COUNTLOOP(col, db->GetColumnCount()) {
				if(row == -1) {
					//列名
					strbuf += db->GetColumnName(col);
				} else {
					//要素の値
					strbuf += db->GetCellFromIndex(row, col)->ToString();
				}
				strbuf += "\t";
			}
			TS_LOG(strbuf);
		}
	}
}


//####################  ファイルに書き込む  ####################
void CDatabase::CDBBase::Save(string& strbuf) {
	//全サブDB書き込み
	for(auto& db : this->DBs.Items) {
		TS_COUNTLOOP(row, db.GetRowCount()) {
			this->stringBuffer = "";
			TS_COUNTLOOP(col, db.GetColumnCount()) {
				this->stringBuffer += db.GetCellFromIndex(row, col)->ToString();
				if(col < db.GetColumnCount() - 1) {
					this->stringBuffer += "\t";
				}
			}
			strbuf += this->stringBuffer + "\n";
		}
		strbuf += "\n";
	}
}


//####################  このデータベースが所有する単一データベースの数を返す  ####################
int CDatabase::CDBBase::GetSubDBCount() {
	return this->DBs.GetCount();
}


//####################  データベースのソースファイル名を返す  ####################
string CDatabase::CDBBase::GetDBFileName() {
	return this->fileName;
}


//####################  単一データベースを取得  ####################
CDatabase::CDBBaseOne* CDatabase::CDBBase::GetSubDB(int index) {
	//NOTE: 単一データベースはnullptrを取得する目的はなく、取得できなかった場合はエラー落ちが確定するので必ず範囲チェックを行う。
	TS_CHECKINDEX(index, 0, this->DBs.GetCount()) {
		return this->DBs.GetFromIndex(index);
	} else {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のインデックス [%d] で単一データベースを取得しようとしました。\n処理を続行できないため、ゲームプログラムを終了します。", index);
		return nullptr;
	}
}


//####################  ファイルを読み取り専用で開く  ####################
bool CDatabase::CDBBase::openFile(string fileName) {
	//DXアーカイブ後は使用できないので無効
	//if(!PathFileExists(fileName.c_str())) {
	//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないデータベースです。\n>%s", fileName.c_str());
	//	return false;
	//}

#ifdef _DEBUG		//デバッグモードのときは読み込み専用でロードしたいので標準IOを使う
	this->fp = ::fopen(fileName.c_str(), "r");
	if(this->fp == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "データベースファイルのオープンに失敗しました。\n>%s", fileName.c_str());
		return false;
	}
#else				//リリースモードのときはDXライブラリアーカイブを開きたいのでDXライブラリのファイル処理関数を使う
	this->fpDx = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
	if(this->fpDx == 0) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "データベースファイルのオープンに失敗しました。\n>%s", fileName.c_str());
		return false;
	}
#endif

	return true;
}


//####################  末尾まで読み込んだかどうかを調べる  ####################
bool CDatabase::CDBBase::isEOF() {
#ifdef _DEBUG
	return (::feof(this->fp) != NULL);
#else
	return (DxLib::FileRead_eof(this->fpDx) != 0);
#endif
}


//####################  現在のストリームから読み込んで単体データベースに格納する  ####################
void CDatabase::CDBBase::loadOneDB(CDBBaseOne* db) {
	db->ClearColumn();
	db->ClearRow();

	//型名を処理
	this->readNextRow();
	db->SetColumnTypes(&this->splitBuffer);

	//列名を処理
	this->readNextRow();
	db->SetColumnNames(&this->splitBuffer);

	//全行読み込み
	while(this->readNextRow() > 1) {
		db->LoadRow(&this->splitBuffer);
	}

	vector<string>().swap(this->splitBuffer);
}


//####################  １行読み進める  ####################
//返値は、完全に一行分を読み込んだかどうか
bool CDatabase::CDBBase::gets(char* charbuf, int size) {
#ifdef _DEBUG
	::fgets(charbuf, size, this->fp);

	//fgetsでは改行コードも含まれてしまうのでここで除去する
	if(charbuf[::strlen(charbuf) - 1] == '\n') {
		charbuf[::strlen(charbuf) - 1] = '\0';
		return true;
	}
#else
	DxLib::FileRead_gets(buf, size, this->fpDx);

	//一行分完全に読めたかどうかは、文字列の長さが (size-1) 未満であるかどうかで判断する
	if(::strlen(buf) < size - 1) {
		return true;
	}
#endif
	return false;
}


//####################  開いているストリームを閉じる  ####################
void CDatabase::CDBBase::closeFile() {
#ifdef _DEBUG
	if(this->fp != nullptr) {
		::fclose(this->fp);
		this->fp = nullptr;
	}
#else
	if(this->fpDx != 0) {
		DxLib::FileRead_close(this->fpDx);
		this->fpDx = 0;
	}
#endif
}


//####################  次の行を読み込んでバッファに格納する  ####################
int CDatabase::CDBBase::readNextRow() {
	//一行読み込む
	bool lineEnd = false;
	this->stringBuffer = "";
	while(!this->isEOF() && !lineEnd) {
		//バッファの単位で分けて読み込む
		lineEnd = this->gets(::buf, sizeof(::buf) - 1);
		this->stringBuffer += ::buf;
	}

	//タブ区切りによってセル分割する
	Game.SplitString(this->splitBuffer, this->stringBuffer, "\t");
	return this->splitBuffer.size();
}
