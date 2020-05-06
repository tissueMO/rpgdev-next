//###################################################################
//			システム データベース
//###################################################################
#include "Header.h"


//####################  読み込み処理  ####################
//システムDBはXML形式で書き出され、内部DBでは１行に設定項目を並べていく形を取る。
bool CDatabase::CDBSystem::Load(string fileName) {
	if(!this->openFile(fileName)) {
		TS_MBE("システムデータベースの読み込みに失敗しました。\nゲームプログラムがプロジェクトフォルダーの直下に存在しているか確認して下さい。");
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "システムデータベースの読み込みに失敗しました。\n>%s", fileName.c_str());
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::InvalidSystemDB));		//NOTE: ここでErrorとして終了してログを出力すると、他のデータベースが未初期化のためさらにエラーを起こしてしまう。
		return false;		//オープン失敗
	}

	//XML読み込み準備
	this->DBs.Clear();
	this->DBs.Add(CDatabase::CDBBaseOne());
	this->fileName = fileName;

	//設定項目が定義される箇所まで読み進める
	do {
		this->readNextRow();
		this->stringBuffer += "\n";
	} while(!this->isEOF() && this->stringBuffer.find("<DBProperty") == string::npos);

	//全行読み込み
	auto newRow = DBRow();
	this->GetSubDB(0)->ClearColumn();
	do {
		this->readNextRow();
		if(this->isEOF() || this->stringBuffer.find("</DBProperty>") != string::npos) {
			break;		//終端まで達したら抜ける
		}

		//サフィックスに応じて型分けを行う
		if(this->stringBuffer.find("<str") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());
			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::String), this->perseValue()));
		} else if(this->stringBuffer.find("<int") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());
			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::OneNumber), this->perseValue()));
		} else if(this->stringBuffer.find("<bool") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());

			//true/falseを数値に変換
			string value;
			value = (this->perseValue() == "true") ? "1" : "0";

			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::Boolean), value));
		} else if(this->stringBuffer.find("<size") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());

			//幅と高さを追加で読み込む
			this->readNextRow();
			auto width = this->perseValue();
			this->readNextRow();
			auto height = this->perseValue();

			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::Numbers), width + ";" + height));
		}
	} while(true);
	this->GetSubDB(0)->AddRow(newRow);

	this->closeFile();
	return true;
}


//####################  設定値を取得  ####################
int CDatabase::CDBSystem::GetInteger(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		return *item->GetIntegerPTR();
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在のシステムDBに含まれない設定項目名です。既定値が返されます。\n>%s", settingName.c_str());
	return 0;
}
bool CDatabase::CDBSystem::GetBoolean(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		return *this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName))->GetBooleanPTR();
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在のシステムDBに含まれない設定項目名です。既定値が返されます。\n>%s", settingName.c_str());
	return false;
}
string CDatabase::CDBSystem::GetString(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		return *this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName))->GetStringPTR();
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在のシステムDBに含まれない設定項目名です。既定値が返されます。\n>%s", settingName.c_str());
	return "";
}
CSize CDatabase::CDBSystem::GetSize(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		auto ints = item->GetIntegers();
		string buf;
		CSize result;
		if(ints->Count() == 2) {
			result.Set(ints->Get(0), ints->Get(1));
		}
		return result;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在のシステムDBに含まれない設定項目名です。既定値が返されます。\n>%s", settingName.c_str());
	return CSize();
}


//####################  現在のバッファから設定項目名を取り出して返す  ####################
string CDatabase::CDBSystem::perseSettingName() {
	const auto start = this->stringBuffer.find("<");
	const auto end1 = this->stringBuffer.find(">");
	const auto end2 = this->stringBuffer.find(" />");		//タグ囲いしないタイプの設定
	if(start == string::npos || (end1 == string::npos && end2 == string::npos)) {
		return "";		//無効なバッファ文字列
	} else if(end2 != string::npos) {
		return this->stringBuffer.substr(start + 1, end2 - start - 1);
	} else {
		return this->stringBuffer.substr(start + 1, end1 - start - 1);
	}
}


//####################  現在のバッファから値部分を取り出して返す  ####################
string CDatabase::CDBSystem::perseValue() {
	const auto start = this->stringBuffer.find(">");
	const auto end = this->stringBuffer.find("</");
	if(start == string::npos || end == string::npos) {
		return "";		//無効なバッファ文字列
	} else {
		return this->stringBuffer.substr(start + 1, end - start - 1);
	}
}
