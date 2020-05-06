//###################################################################
//			暗号化を施したセーブデータを読み書きするクラス
//###################################################################
//#define DISENABLEDMEMCHECK
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  すべてのヘッダーを取得  ####################
void CSaveLoad::GetAllSaveHeader() {
	string fileName, version, date, HeaderText, data;
	string gameVersion = Game.DB.getFDBSystem()->GetString(TS::DB::System::GameVersion);
	vector<string> dataLines;

	this->saveHeaderList.clear();
	Game.CheckDirectory(TS::DirectoryNames::Log);		//フォルダーが存在しない場合は作成する
	TS_COUNTLOOP(i, Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxSaveDataCount)) {
		bool isEnabled = true;
		fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), i);
		if(PathFileExists(fileName.c_str())) {
			//存在する場合のみデータを復号化し、テキストを行ごとに分解
			if(!this->decodeFileToString(data, fileName)) {
				HeaderText = "(I/O エラー)";
				isEnabled = false;
			}
			Game.SplitString(dataLines, data, "\n");

			version = Game.PopHead(dataLines);			//１行目はバージョン
			date = Game.PopHead(dataLines);				//２行目はセーブ日時
			HeaderText = Game.PopHead(dataLines);		//３行目はユーザー定義のヘッダー情報
			if(version != gameVersion) {
				HeaderText = "(互換性なし)";
				isEnabled = false;
			}
		} else {
			HeaderText = "";
			date = "";
			isEnabled = false;
		}
		this->saveHeaderList.push_back(CSaveHeaderData(isEnabled, date, HeaderText));
	}
}


//####################  ヘッダー情報を取得  ####################
string CSaveLoad::GetSaveHeader(int index) {
	TS_CHECKINDEX(index, 0, this->saveHeaderList.size()) {
		return this->saveHeaderList[index].Header;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] が指定されました。", index);
		return "";
	}
}


//####################  セーブ日時情報を取得  ####################
string CSaveLoad::GetSaveDateTime(int index) {
	TS_CHECKINDEX(index, 0, this->saveHeaderList.size()) {
		return this->saveHeaderList[index].DateTime;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] が指定されました。", index);
		return "";
	}
}


//####################  指定したセーブデータの復号化したデータを取得  ####################
//返値は成功したかどうか
bool CSaveLoad::GetPlainData(int index, string& plain) {
	string fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), index);
	TS_LOG("セーブデータを復号化します。\n>" + fileName);
	Game.CheckDirectory(TS::DirectoryNames::SaveData);		//フォルダーが存在しない場合は作成する

	//復号化してから始める
	if(!this->decodeFileToString(plain, fileName)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "セーブファイルの読み込みに失敗しました。\n>%s", fileName.c_str());
		return false;
	}
	return true;
}


//####################  指定したセーブデータが有効であるかどうかを調べる  ####################
bool CSaveLoad::IsEnabled(int index) {
	TS_CHECKINDEX(index, 0, this->saveHeaderList.size()) {
		return this->saveHeaderList[index].Enabled;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] が指定されました。", index);
		return false;
	}
}


//####################  セーブデータを複写  ####################
bool CSaveLoad::CopyData(int fromIndex, int toIndex) {
	if(!this->IsEnabled(fromIndex)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "無効なセーブデータのインデックス [%d] が複写元に指定されました。", fromIndex);
		return false;
	} else TS_CHECKINDEX(toIndex, 0, this->saveHeaderList.size()) {
		bool Result = CopyFile(Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), fromIndex).c_str(), Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), toIndex).c_str(), false);
		if(!Result) {
			Game.PrintAPIError();
		}
		this->GetAllSaveHeader();		//リストを更新する
		return Result;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] が複写先に指定されました。", toIndex);
		return false;
	}
}


//####################  セーブデータを削除  ####################
bool CSaveLoad::DeleteData(int targetIndex) {
	TS_CHECKINDEX(targetIndex, 0, this->saveHeaderList.size()) {
		string fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), targetIndex);
		TS_LOG("セーブデータを削除します。\n>" + fileName);
		bool Result = DeleteFile(fileName.c_str());
		if(!Result) {
			Game.PrintAPIError();
		}
		this->GetAllSaveHeader();		//リストを更新する
		return Result;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] を削除しようとしました。", targetIndex);
		return false;
	}
}


//####################  セーブデータを書き込む  ####################
//返値は 成功したかどうか
bool CSaveLoad::SaveData(int targetIndex) {
	TS_CHECKINDEX(targetIndex, 0, this->saveHeaderList.size()) {
		string TextBuffer;
		string fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), targetIndex);
		TS_LOG("セーブデータを保存します。\n>" + fileName);
		Game.CheckDirectory(TS::DirectoryNames::SaveData);				//フォルダーが存在しない場合は作成する

		TextBuffer = this->generateRawData();							//セーブデータの生データを生成
		return this->encodeStringToFile(TextBuffer, fileName);			//セーブデータを暗号化して書き出す
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] でセーブしようとしました。", targetIndex);
		return false;
	}
}


//####################  一時的にセーブデータを保存する  ####################
//返値は 成功したかどうか
bool CSaveLoad::QuickSave() {
	TS_LOG("一時的にセーブデータを保存します。");
	this->temporarySaveData = this->generateRawData();
	return true;
}


//####################  一時的に保存したセーブデータを読み込む  ####################
//返値は 成功したかどうか
bool CSaveLoad::QuickLoad() {
	TS_LOG("一時的に保存したセーブデータを読み込みます。");
	return this->loadRawData(this->temporarySaveData);
}


//####################  セーブデータを読み込む  ####################
//返値は 成功したかどうか
bool CSaveLoad::LoadData(int targetIndex) {
	if(!this->IsEnabled(targetIndex)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "無効なセーブデータのインデックス [%d] をロードしようとしました。", targetIndex);
		return false;
	} else TS_CHECKINDEX(targetIndex, 0, this->saveHeaderList.size()) {
		//生データに復号化してからゲーム情報へ反映する
		string plain;
		if(!this->GetPlainData(targetIndex, plain)) {
			return false;		//読み込み失敗
		}
		return this->loadRawData(plain);
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のセーブデータのインデックス [%d] をロードしようとしました。", targetIndex);
		return false;
	}
}


//####################  暗号化前のセーブデータの原型を生成  ####################
string CSaveLoad::generateRawData() {
	string TextBuffer;

	//1. ヘッダー情報
	TextBuffer = Game.DB.getFDBSystem()->GetString(TS::DB::System::GameVersion) + "\n";
	TextBuffer += CGameObject::NowTimeToString("%Y/%m/%d %H:%M:%S") + "\n";
	TextBuffer += Game.SQ.DoFunc<string>(TS::SQ::SQEvent_RequiredSaveHeader) + "\n";

	//2. 可変データベース
	TextBuffer += TS::SaveLoad::VDB + "\n";
	for(auto DB : Game.DB.VDB) {
		DB->Save(TextBuffer);
		TextBuffer += TS::SaveLoad::VDBSplit + "\n";
	}

	//3. ユーザーデータベース
	TextBuffer += TS::SaveLoad::UDB + "\n";
	Game.DB.UDB.Save(TextBuffer);

	//4. マップ・イベント情報
	TextBuffer += TS::SaveLoad::EV + "\n";
	if(Game.getSceneMap() != nullptr) {
		TextBuffer += TS::SaveLoad::MapData + "\n";
		CSceneMap::Save(TextBuffer);
	} else {
		TextBuffer += TS::SaveLoad::NoMap + "\n";
	}

	//5. スクリプト依存部分
	TextBuffer += TS::SaveLoad::User + "\n";
	TextBuffer += Game.SQ.DoFunc<string>(TS::SQ::SQEvent_SaveUserData);
	return TextBuffer;
}


//####################  復号化されたセーブデータの原型から実際にゲーム情報を反映する  ####################
bool CSaveLoad::loadRawData(const string &textBuffer) {
	vector<string> plain;
	vector<string> TSVData;
	string strbuf;

	//テキストを行ごとに分解
	Game.SplitString(plain, textBuffer, "\n");

	//1. ヘッダー情報 は読み捨てる
	do {
		strbuf = Game.PopHead(plain);
	} while(strbuf != TS::SaveLoad::VDB);

	//2. 可変データベース
	for(auto db : Game.DB.VDB) {
		strbuf = Game.PopHead(plain);

		//見出しが出るまで一つのデータベースとみなす
		TSVData.clear();
		while(strbuf != TS::SaveLoad::VDBSplit) {
			TSVData.push_back(strbuf);
			strbuf = Game.PopHead(plain);
		}
		db->Load(TSVData);
	}

	//3. ユーザーデータベース
	Game.PopHead(plain);		//先頭の見出しは読み捨てる
	Game.DB.UDB.Load(plain);

	//4. マップ・イベント情報
	Game.PopHead(plain);		//先頭の見出しは読み捨てる
	if(Game.PopHead(plain) != TS::SaveLoad::NoMap) {
		CSceneMap::Load(plain);		//マップデータが存在する場合のみ読み込む
	}

	//5. スクリプト依存部分
	Game.PopHead(plain);		//先頭の見出しは読み捨てる
	string toSQdata;
	while(plain.size() > 0) {
		toSQdata += Game.PopHead(plain);
		toSQdata += "\n";
	}
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_LoadUserData, toSQdata);

	//6. 最終処理
	//BGM・BGS再生
	Game.getSceneMap()->MapData.PlayBGMBGS();

	return true;
}


//####################  与えられた文字列を暗号化してファイルに書き出す  ####################
//返値は 成功したかどうか
bool CSaveLoad::encodeStringToFile(const string &plain, string fileName) {
	FILE *fp;
	string encoded, cipher;
	CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption encctx;
	byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
	::memcpy(key, TS::System::CryptoPPKey.c_str(), CryptoPP::DES::DEFAULT_KEYLENGTH);
	encctx.SetKey(key, CryptoPP::DES::DEFAULT_KEYLENGTH);

	// ciper
	auto sinkCipher = new CryptoPP::StringSink(cipher);
	auto filter = new CryptoPP::StreamTransformationFilter(encctx, sinkCipher);
	CryptoPP::StringSource(plain, true, filter);

	// hex encode
	auto sinkEncoded = new CryptoPP::StringSink(encoded);
	auto encoder = new CryptoPP::HexEncoder(sinkEncoded);
	CryptoPP::StringSource(cipher, true, encoder);

	//暗号化ファイル書き出し
	if((fp = ::fopen(fileName.c_str(), "w")) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "セーブファイルの書き込みエラーが発生しました。\n>%s", fileName.c_str());
		return false;
	}
	::fputs(encoded.c_str(), fp);
	::fclose(fp);

	/* 検証した結果、これらは自動で解放されているらしい。
	delete sinkCipher;
	delete filter;
	delete sinkEncoded;
	delete encoder;
	*/

	return true;
}


//####################  ファイルを復号化して文字列にする  ####################
//返値は 成功したかどうか
bool CSaveLoad::decodeFileToString(string &plain, string fileName) {
	plain = "";

	//暗号化ファイル読み込み
	FILE *fp;
	string encoded, decoded;
	CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption encctx;
	byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
	::memcpy(key, TS::System::CryptoPPKey.c_str(), CryptoPP::DES::DEFAULT_KEYLENGTH);
	encctx.SetKey(key, CryptoPP::DES::DEFAULT_KEYLENGTH);

	if((fp = ::fopen(fileName.c_str(), "r")) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "セーブファイルの読み込みエラー\n>%s", fileName.c_str());
		return false;
	}
	encoded = "";
	while(::fgets(::buf, sizeof(::buf) - 1, fp) != nullptr) {
		encoded += ::buf;
	}
	::fclose(fp);

	// hex decode
	auto sinkDecoded = new CryptoPP::StringSink(decoded);
	auto decoder = new CryptoPP::HexDecoder(sinkDecoded);
	CryptoPP::StringSource(encoded, true, decoder);

	// ciper decode
	CryptoPP::ECB_Mode<CryptoPP::DES>::Decryption decctx;
	decctx.SetKey(key, CryptoPP::DES::DEFAULT_KEYLENGTH);
	auto sinkRecovered = new CryptoPP::StringSink(plain);
	auto filter = new CryptoPP::StreamTransformationFilter(decctx, sinkRecovered);
	CryptoPP::StringSource(decoded, true, filter);

	/* 検証した結果、これらは自動で解放されているらしい。
	delete sinkDecoded;
	delete decoder;
	delete sinkRecovered;
	delete filter;
	*/

	return true;
}
