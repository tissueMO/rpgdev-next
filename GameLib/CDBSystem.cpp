//###################################################################
//			�V�X�e�� �f�[�^�x�[�X
//###################################################################
#include "Header.h"


//####################  �ǂݍ��ݏ���  ####################
//�V�X�e��DB��XML�`���ŏ����o����A����DB�ł͂P�s�ɐݒ荀�ڂ���ׂĂ����`�����B
bool CDatabase::CDBSystem::Load(string fileName) {
	if(!this->openFile(fileName)) {
		TS_MBE("�V�X�e���f�[�^�x�[�X�̓ǂݍ��݂Ɏ��s���܂����B\n�Q�[���v���O�������v���W�F�N�g�t�H���_�[�̒����ɑ��݂��Ă��邩�m�F���ĉ������B");
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�V�X�e���f�[�^�x�[�X�̓ǂݍ��݂Ɏ��s���܂����B\n>%s", fileName.c_str());
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::InvalidSystemDB));		//NOTE: ������Error�Ƃ��ďI�����ă��O���o�͂���ƁA���̃f�[�^�x�[�X�����������̂��߂���ɃG���[���N�����Ă��܂��B
		return false;		//�I�[�v�����s
	}

	//XML�ǂݍ��ݏ���
	this->DBs.Clear();
	this->DBs.Add(CDatabase::CDBBaseOne());
	this->fileName = fileName;

	//�ݒ荀�ڂ���`�����ӏ��܂œǂݐi�߂�
	do {
		this->readNextRow();
		this->stringBuffer += "\n";
	} while(!this->isEOF() && this->stringBuffer.find("<DBProperty") == string::npos);

	//�S�s�ǂݍ���
	auto newRow = DBRow();
	this->GetSubDB(0)->ClearColumn();
	do {
		this->readNextRow();
		if(this->isEOF() || this->stringBuffer.find("</DBProperty>") != string::npos) {
			break;		//�I�[�܂ŒB�����甲����
		}

		//�T�t�B�b�N�X�ɉ����Č^�������s��
		if(this->stringBuffer.find("<str") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());
			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::String), this->perseValue()));
		} else if(this->stringBuffer.find("<int") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());
			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::OneNumber), this->perseValue()));
		} else if(this->stringBuffer.find("<bool") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());

			//true/false�𐔒l�ɕϊ�
			string value;
			value = (this->perseValue() == "true") ? "1" : "0";

			newRow.push_back(CDBCell(static_cast<int>(TS::DB::ColumnType::Boolean), value));
		} else if(this->stringBuffer.find("<size") != string::npos) {
			this->GetSubDB(0)->AddColumn(this->perseSettingName().c_str());

			//���ƍ�����ǉ��œǂݍ���
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


//####################  �ݒ�l���擾  ####################
int CDatabase::CDBSystem::GetInteger(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		return *item->GetIntegerPTR();
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݂̃V�X�e��DB�Ɋ܂܂�Ȃ��ݒ荀�ږ��ł��B����l���Ԃ���܂��B\n>%s", settingName.c_str());
	return 0;
}
bool CDatabase::CDBSystem::GetBoolean(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		return *this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName))->GetBooleanPTR();
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݂̃V�X�e��DB�Ɋ܂܂�Ȃ��ݒ荀�ږ��ł��B����l���Ԃ���܂��B\n>%s", settingName.c_str());
	return false;
}
string CDatabase::CDBSystem::GetString(string settingName) {
	auto item = this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName));
	if(item != nullptr) {
		return *this->GetSubDB(0)->GetCellFromIndex(0, this->GetSubDB(0)->GetColumnIndex(settingName))->GetStringPTR();
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݂̃V�X�e��DB�Ɋ܂܂�Ȃ��ݒ荀�ږ��ł��B����l���Ԃ���܂��B\n>%s", settingName.c_str());
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
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݂̃V�X�e��DB�Ɋ܂܂�Ȃ��ݒ荀�ږ��ł��B����l���Ԃ���܂��B\n>%s", settingName.c_str());
	return CSize();
}


//####################  ���݂̃o�b�t�@����ݒ荀�ږ������o���ĕԂ�  ####################
string CDatabase::CDBSystem::perseSettingName() {
	const auto start = this->stringBuffer.find("<");
	const auto end1 = this->stringBuffer.find(">");
	const auto end2 = this->stringBuffer.find(" />");		//�^�O�͂����Ȃ��^�C�v�̐ݒ�
	if(start == string::npos || (end1 == string::npos && end2 == string::npos)) {
		return "";		//�����ȃo�b�t�@������
	} else if(end2 != string::npos) {
		return this->stringBuffer.substr(start + 1, end2 - start - 1);
	} else {
		return this->stringBuffer.substr(start + 1, end1 - start - 1);
	}
}


//####################  ���݂̃o�b�t�@����l���������o���ĕԂ�  ####################
string CDatabase::CDBSystem::perseValue() {
	const auto start = this->stringBuffer.find(">");
	const auto end = this->stringBuffer.find("</");
	if(start == string::npos || end == string::npos) {
		return "";		//�����ȃo�b�t�@������
	} else {
		return this->stringBuffer.substr(start + 1, end - start - 1);
	}
}
