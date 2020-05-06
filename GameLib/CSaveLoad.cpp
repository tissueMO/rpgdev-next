//###################################################################
//			�Í������{�����Z�[�u�f�[�^��ǂݏ�������N���X
//###################################################################
//#define DISENABLEDMEMCHECK
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  ���ׂẴw�b�_�[���擾  ####################
void CSaveLoad::GetAllSaveHeader() {
	string fileName, version, date, HeaderText, data;
	string gameVersion = Game.DB.getFDBSystem()->GetString(TS::DB::System::GameVersion);
	vector<string> dataLines;

	this->saveHeaderList.clear();
	Game.CheckDirectory(TS::DirectoryNames::Log);		//�t�H���_�[�����݂��Ȃ��ꍇ�͍쐬����
	TS_COUNTLOOP(i, Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxSaveDataCount)) {
		bool isEnabled = true;
		fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), i);
		if(PathFileExists(fileName.c_str())) {
			//���݂���ꍇ�̂݃f�[�^�𕜍������A�e�L�X�g���s���Ƃɕ���
			if(!this->decodeFileToString(data, fileName)) {
				HeaderText = "(I/O �G���[)";
				isEnabled = false;
			}
			Game.SplitString(dataLines, data, "\n");

			version = Game.PopHead(dataLines);			//�P�s�ڂ̓o�[�W����
			date = Game.PopHead(dataLines);				//�Q�s�ڂ̓Z�[�u����
			HeaderText = Game.PopHead(dataLines);		//�R�s�ڂ̓��[�U�[��`�̃w�b�_�[���
			if(version != gameVersion) {
				HeaderText = "(�݊����Ȃ�)";
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


//####################  �w�b�_�[�����擾  ####################
string CSaveLoad::GetSaveHeader(int index) {
	TS_CHECKINDEX(index, 0, this->saveHeaderList.size()) {
		return this->saveHeaderList[index].Header;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] ���w�肳��܂����B", index);
		return "";
	}
}


//####################  �Z�[�u���������擾  ####################
string CSaveLoad::GetSaveDateTime(int index) {
	TS_CHECKINDEX(index, 0, this->saveHeaderList.size()) {
		return this->saveHeaderList[index].DateTime;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] ���w�肳��܂����B", index);
		return "";
	}
}


//####################  �w�肵���Z�[�u�f�[�^�̕����������f�[�^���擾  ####################
//�Ԓl�͐����������ǂ���
bool CSaveLoad::GetPlainData(int index, string& plain) {
	string fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), index);
	TS_LOG("�Z�[�u�f�[�^�𕜍������܂��B\n>" + fileName);
	Game.CheckDirectory(TS::DirectoryNames::SaveData);		//�t�H���_�[�����݂��Ȃ��ꍇ�͍쐬����

	//���������Ă���n�߂�
	if(!this->decodeFileToString(plain, fileName)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�Z�[�u�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}
	return true;
}


//####################  �w�肵���Z�[�u�f�[�^���L���ł��邩�ǂ����𒲂ׂ�  ####################
bool CSaveLoad::IsEnabled(int index) {
	TS_CHECKINDEX(index, 0, this->saveHeaderList.size()) {
		return this->saveHeaderList[index].Enabled;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] ���w�肳��܂����B", index);
		return false;
	}
}


//####################  �Z�[�u�f�[�^�𕡎�  ####################
bool CSaveLoad::CopyData(int fromIndex, int toIndex) {
	if(!this->IsEnabled(fromIndex)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�����ȃZ�[�u�f�[�^�̃C���f�b�N�X [%d] �����ʌ��Ɏw�肳��܂����B", fromIndex);
		return false;
	} else TS_CHECKINDEX(toIndex, 0, this->saveHeaderList.size()) {
		bool Result = CopyFile(Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), fromIndex).c_str(), Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), toIndex).c_str(), false);
		if(!Result) {
			Game.PrintAPIError();
		}
		this->GetAllSaveHeader();		//���X�g���X�V����
		return Result;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] �����ʐ�Ɏw�肳��܂����B", toIndex);
		return false;
	}
}


//####################  �Z�[�u�f�[�^���폜  ####################
bool CSaveLoad::DeleteData(int targetIndex) {
	TS_CHECKINDEX(targetIndex, 0, this->saveHeaderList.size()) {
		string fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), targetIndex);
		TS_LOG("�Z�[�u�f�[�^���폜���܂��B\n>" + fileName);
		bool Result = DeleteFile(fileName.c_str());
		if(!Result) {
			Game.PrintAPIError();
		}
		this->GetAllSaveHeader();		//���X�g���X�V����
		return Result;
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] ���폜���悤�Ƃ��܂����B", targetIndex);
		return false;
	}
}


//####################  �Z�[�u�f�[�^����������  ####################
//�Ԓl�� �����������ǂ���
bool CSaveLoad::SaveData(int targetIndex) {
	TS_CHECKINDEX(targetIndex, 0, this->saveHeaderList.size()) {
		string TextBuffer;
		string fileName = Game.Format("%s%d", TS::DirectoryNames::SaveData.c_str(), targetIndex);
		TS_LOG("�Z�[�u�f�[�^��ۑ����܂��B\n>" + fileName);
		Game.CheckDirectory(TS::DirectoryNames::SaveData);				//�t�H���_�[�����݂��Ȃ��ꍇ�͍쐬����

		TextBuffer = this->generateRawData();							//�Z�[�u�f�[�^�̐��f�[�^�𐶐�
		return this->encodeStringToFile(TextBuffer, fileName);			//�Z�[�u�f�[�^���Í������ď����o��
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] �ŃZ�[�u���悤�Ƃ��܂����B", targetIndex);
		return false;
	}
}


//####################  �ꎞ�I�ɃZ�[�u�f�[�^��ۑ�����  ####################
//�Ԓl�� �����������ǂ���
bool CSaveLoad::QuickSave() {
	TS_LOG("�ꎞ�I�ɃZ�[�u�f�[�^��ۑ����܂��B");
	this->temporarySaveData = this->generateRawData();
	return true;
}


//####################  �ꎞ�I�ɕۑ������Z�[�u�f�[�^��ǂݍ���  ####################
//�Ԓl�� �����������ǂ���
bool CSaveLoad::QuickLoad() {
	TS_LOG("�ꎞ�I�ɕۑ������Z�[�u�f�[�^��ǂݍ��݂܂��B");
	return this->loadRawData(this->temporarySaveData);
}


//####################  �Z�[�u�f�[�^��ǂݍ���  ####################
//�Ԓl�� �����������ǂ���
bool CSaveLoad::LoadData(int targetIndex) {
	if(!this->IsEnabled(targetIndex)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�����ȃZ�[�u�f�[�^�̃C���f�b�N�X [%d] �����[�h���悤�Ƃ��܂����B", targetIndex);
		return false;
	} else TS_CHECKINDEX(targetIndex, 0, this->saveHeaderList.size()) {
		//���f�[�^�ɕ��������Ă���Q�[�����֔��f����
		string plain;
		if(!this->GetPlainData(targetIndex, plain)) {
			return false;		//�ǂݍ��ݎ��s
		}
		return this->loadRawData(plain);
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃Z�[�u�f�[�^�̃C���f�b�N�X [%d] �����[�h���悤�Ƃ��܂����B", targetIndex);
		return false;
	}
}


//####################  �Í����O�̃Z�[�u�f�[�^�̌��^�𐶐�  ####################
string CSaveLoad::generateRawData() {
	string TextBuffer;

	//1. �w�b�_�[���
	TextBuffer = Game.DB.getFDBSystem()->GetString(TS::DB::System::GameVersion) + "\n";
	TextBuffer += CGameObject::NowTimeToString("%Y/%m/%d %H:%M:%S") + "\n";
	TextBuffer += Game.SQ.DoFunc<string>(TS::SQ::SQEvent_RequiredSaveHeader) + "\n";

	//2. �σf�[�^�x�[�X
	TextBuffer += TS::SaveLoad::VDB + "\n";
	for(auto DB : Game.DB.VDB) {
		DB->Save(TextBuffer);
		TextBuffer += TS::SaveLoad::VDBSplit + "\n";
	}

	//3. ���[�U�[�f�[�^�x�[�X
	TextBuffer += TS::SaveLoad::UDB + "\n";
	Game.DB.UDB.Save(TextBuffer);

	//4. �}�b�v�E�C�x���g���
	TextBuffer += TS::SaveLoad::EV + "\n";
	if(Game.getSceneMap() != nullptr) {
		TextBuffer += TS::SaveLoad::MapData + "\n";
		CSceneMap::Save(TextBuffer);
	} else {
		TextBuffer += TS::SaveLoad::NoMap + "\n";
	}

	//5. �X�N���v�g�ˑ�����
	TextBuffer += TS::SaveLoad::User + "\n";
	TextBuffer += Game.SQ.DoFunc<string>(TS::SQ::SQEvent_SaveUserData);
	return TextBuffer;
}


//####################  ���������ꂽ�Z�[�u�f�[�^�̌��^������ۂɃQ�[�����𔽉f����  ####################
bool CSaveLoad::loadRawData(const string &textBuffer) {
	vector<string> plain;
	vector<string> TSVData;
	string strbuf;

	//�e�L�X�g���s���Ƃɕ���
	Game.SplitString(plain, textBuffer, "\n");

	//1. �w�b�_�[��� �͓ǂݎ̂Ă�
	do {
		strbuf = Game.PopHead(plain);
	} while(strbuf != TS::SaveLoad::VDB);

	//2. �σf�[�^�x�[�X
	for(auto db : Game.DB.VDB) {
		strbuf = Game.PopHead(plain);

		//���o�����o��܂ň�̃f�[�^�x�[�X�Ƃ݂Ȃ�
		TSVData.clear();
		while(strbuf != TS::SaveLoad::VDBSplit) {
			TSVData.push_back(strbuf);
			strbuf = Game.PopHead(plain);
		}
		db->Load(TSVData);
	}

	//3. ���[�U�[�f�[�^�x�[�X
	Game.PopHead(plain);		//�擪�̌��o���͓ǂݎ̂Ă�
	Game.DB.UDB.Load(plain);

	//4. �}�b�v�E�C�x���g���
	Game.PopHead(plain);		//�擪�̌��o���͓ǂݎ̂Ă�
	if(Game.PopHead(plain) != TS::SaveLoad::NoMap) {
		CSceneMap::Load(plain);		//�}�b�v�f�[�^�����݂���ꍇ�̂ݓǂݍ���
	}

	//5. �X�N���v�g�ˑ�����
	Game.PopHead(plain);		//�擪�̌��o���͓ǂݎ̂Ă�
	string toSQdata;
	while(plain.size() > 0) {
		toSQdata += Game.PopHead(plain);
		toSQdata += "\n";
	}
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_LoadUserData, toSQdata);

	//6. �ŏI����
	//BGM�EBGS�Đ�
	Game.getSceneMap()->MapData.PlayBGMBGS();

	return true;
}


//####################  �^����ꂽ��������Í������ăt�@�C���ɏ����o��  ####################
//�Ԓl�� �����������ǂ���
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

	//�Í����t�@�C�������o��
	if((fp = ::fopen(fileName.c_str(), "w")) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�Z�[�u�t�@�C���̏������݃G���[���������܂����B\n>%s", fileName.c_str());
		return false;
	}
	::fputs(encoded.c_str(), fp);
	::fclose(fp);

	/* ���؂������ʁA�����͎����ŉ������Ă���炵���B
	delete sinkCipher;
	delete filter;
	delete sinkEncoded;
	delete encoder;
	*/

	return true;
}


//####################  �t�@�C���𕜍������ĕ�����ɂ���  ####################
//�Ԓl�� �����������ǂ���
bool CSaveLoad::decodeFileToString(string &plain, string fileName) {
	plain = "";

	//�Í����t�@�C���ǂݍ���
	FILE *fp;
	string encoded, decoded;
	CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption encctx;
	byte key[CryptoPP::DES::DEFAULT_KEYLENGTH];
	::memcpy(key, TS::System::CryptoPPKey.c_str(), CryptoPP::DES::DEFAULT_KEYLENGTH);
	encctx.SetKey(key, CryptoPP::DES::DEFAULT_KEYLENGTH);

	if((fp = ::fopen(fileName.c_str(), "r")) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�Z�[�u�t�@�C���̓ǂݍ��݃G���[\n>%s", fileName.c_str());
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

	/* ���؂������ʁA�����͎����ŉ������Ă���炵���B
	delete sinkDecoded;
	delete decoder;
	delete sinkRecovered;
	delete filter;
	*/

	return true;
}
