//###################################################################
//			�f�[�^�x�[�X�̊��N���X
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �j������  ####################
void CDatabase::CDBBase::Dispose() {
	//�s�E����폜
	for(auto& db : this->DBs.Items) {
		db.ClearRow();
		db.ClearColumn();
	}
	this->DBs.Clear();
}


//####################  �t�@�C������ǂݍ���  ####################
//�f�[�^�x�[�X�t�@�C����ǂݍ��ލۂɌĂяo����郁�\�b�h
bool CDatabase::CDBBase::Load(string fileName) {
	TS_LOG("�f�[�^�x�[�X�̓ǂݍ��݂��J�n���܂�...%s", fileName.c_str());
	if(!this->openFile(fileName)) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�f�[�^�x�[�X�̓ǂݍ��݂Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}

	//�S�T�uDB�ǂݍ���
	this->DBs.Clear();
	this->fileName = fileName;
	for(int i = 0; this->isEOF() == false; i++) {
		auto newDB = CDatabase::CDBBaseOne();
		newDB.ID = i;				//�T�uDB��ID�̓C���f�b�N�X�Ɉ�v������
		newDB.Name = fileName;		//�T�uDB�̖��O�̓t�@�C�����ɂ���
		this->loadOneDB(&newDB);
		this->DBs.Add(newDB);
	}

	//NOTE: �Ō�ɗ]���ȋ�s�����邽�߂ɗ]���ȃT�uDB���ǉ�����Ă��܂��̂��������Ȃ����߁A�����Ńf�[�^�𗎂Ƃ�
	//this->DBs.DeleteFromIndex(this->DBs.GetCount() - 1);

	this->closeFile();
	return true;
}


//####################  �t�@�C������ǂݍ���  ####################
//�Z�[�u�f�[�^����ǂݍ��ޏꍇ�ɌĂяo����郁�\�b�h
void CDatabase::CDBBase::Load(vector<string>& vectorbuf) {
	//����ł͏������s��Ȃ�
}


//####################  �f�[�^�x�[�X�̒��g�����O�Ƃ��ďo�͂���  ####################
void CDatabase::CDBBase::PrintData() {
	TS_LOG("\nDB: " + this->fileName);
	TS_COUNTLOOP(i, this->DBs.GetCount()) {
		auto db = this->DBs.GetFromIndex(i);
		//�����̃T�uDB�������Ă���ꍇ�̓C���f�b�N�X������
		if(this->DBs.GetCount() > 1) {
			TS_LOG("\nSubDB[" + std::to_string(i) + "]:");
		}

		//�v�f���o�͂���
		string strbuf;
		for(int row = -1; row < db->GetRowCount(); row++) {
			strbuf = "";
			TS_COUNTLOOP(col, db->GetColumnCount()) {
				if(row == -1) {
					//��
					strbuf += db->GetColumnName(col);
				} else {
					//�v�f�̒l
					strbuf += db->GetCellFromIndex(row, col)->ToString();
				}
				strbuf += "\t";
			}
			TS_LOG(strbuf);
		}
	}
}


//####################  �t�@�C���ɏ�������  ####################
void CDatabase::CDBBase::Save(string& strbuf) {
	//�S�T�uDB��������
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


//####################  ���̃f�[�^�x�[�X�����L����P��f�[�^�x�[�X�̐���Ԃ�  ####################
int CDatabase::CDBBase::GetSubDBCount() {
	return this->DBs.GetCount();
}


//####################  �f�[�^�x�[�X�̃\�[�X�t�@�C������Ԃ�  ####################
string CDatabase::CDBBase::GetDBFileName() {
	return this->fileName;
}


//####################  �P��f�[�^�x�[�X���擾  ####################
CDatabase::CDBBaseOne* CDatabase::CDBBase::GetSubDB(int index) {
	//NOTE: �P��f�[�^�x�[�X��nullptr���擾����ړI�͂Ȃ��A�擾�ł��Ȃ������ꍇ�̓G���[�������m�肷��̂ŕK���͈̓`�F�b�N���s���B
	TS_CHECKINDEX(index, 0, this->DBs.GetCount()) {
		return this->DBs.GetFromIndex(index);
	} else {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃C���f�b�N�X [%d] �ŒP��f�[�^�x�[�X���擾���悤�Ƃ��܂����B\n�����𑱍s�ł��Ȃ����߁A�Q�[���v���O�������I�����܂��B", index);
		return nullptr;
	}
}


//####################  �t�@�C����ǂݎ���p�ŊJ��  ####################
bool CDatabase::CDBBase::openFile(string fileName) {
	//DX�A�[�J�C�u��͎g�p�ł��Ȃ��̂Ŗ���
	//if(!PathFileExists(fileName.c_str())) {
	//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��f�[�^�x�[�X�ł��B\n>%s", fileName.c_str());
	//	return false;
	//}

#ifdef _DEBUG		//�f�o�b�O���[�h�̂Ƃ��͓ǂݍ��ݐ�p�Ń��[�h�������̂ŕW��IO���g��
	this->fp = ::fopen(fileName.c_str(), "r");
	if(this->fp == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�f�[�^�x�[�X�t�@�C���̃I�[�v���Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}
#else				//�����[�X���[�h�̂Ƃ���DX���C�u�����A�[�J�C�u���J�������̂�DX���C�u�����̃t�@�C�������֐����g��
	this->fpDx = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
	if(this->fpDx == 0) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�f�[�^�x�[�X�t�@�C���̃I�[�v���Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}
#endif

	return true;
}


//####################  �����܂œǂݍ��񂾂��ǂ����𒲂ׂ�  ####################
bool CDatabase::CDBBase::isEOF() {
#ifdef _DEBUG
	return (::feof(this->fp) != NULL);
#else
	return (DxLib::FileRead_eof(this->fpDx) != 0);
#endif
}


//####################  ���݂̃X�g���[������ǂݍ���ŒP�̃f�[�^�x�[�X�Ɋi�[����  ####################
void CDatabase::CDBBase::loadOneDB(CDBBaseOne* db) {
	db->ClearColumn();
	db->ClearRow();

	//�^��������
	this->readNextRow();
	db->SetColumnTypes(&this->splitBuffer);

	//�񖼂�����
	this->readNextRow();
	db->SetColumnNames(&this->splitBuffer);

	//�S�s�ǂݍ���
	while(this->readNextRow() > 1) {
		db->LoadRow(&this->splitBuffer);
	}

	vector<string>().swap(this->splitBuffer);
}


//####################  �P�s�ǂݐi�߂�  ####################
//�Ԓl�́A���S�Ɉ�s����ǂݍ��񂾂��ǂ���
bool CDatabase::CDBBase::gets(char* charbuf, int size) {
#ifdef _DEBUG
	::fgets(charbuf, size, this->fp);

	//fgets�ł͉��s�R�[�h���܂܂�Ă��܂��̂ł����ŏ�������
	if(charbuf[::strlen(charbuf) - 1] == '\n') {
		charbuf[::strlen(charbuf) - 1] = '\0';
		return true;
	}
#else
	DxLib::FileRead_gets(buf, size, this->fpDx);

	//��s�����S�ɓǂ߂����ǂ����́A������̒����� (size-1) �����ł��邩�ǂ����Ŕ��f����
	if(::strlen(buf) < size - 1) {
		return true;
	}
#endif
	return false;
}


//####################  �J���Ă���X�g���[�������  ####################
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


//####################  ���̍s��ǂݍ���Ńo�b�t�@�Ɋi�[����  ####################
int CDatabase::CDBBase::readNextRow() {
	//��s�ǂݍ���
	bool lineEnd = false;
	this->stringBuffer = "";
	while(!this->isEOF() && !lineEnd) {
		//�o�b�t�@�̒P�ʂŕ����ēǂݍ���
		lineEnd = this->gets(::buf, sizeof(::buf) - 1);
		this->stringBuffer += ::buf;
	}

	//�^�u��؂�ɂ���ăZ����������
	Game.SplitString(this->splitBuffer, this->stringBuffer, "\t");
	return this->splitBuffer.size();
}
