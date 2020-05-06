//###################################################################
//			�t���O�E�ϐ� �f�[�^�x�[�X
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �ǂݍ��ݏ���  ####################
bool CDatabase::CDBValue::Load(string fileName) {
	//�f�t�H���g�̓ǂݍ��ݏ�������s����
	if(!CDBBase::Load(fileName)) {
		return false;		//�ǂݍ��݂Ɏ��s�����ꍇ�́A���̐�̏����𑱍s�ł��Ȃ�
	}

	//�X�N���v�g�ɋ��ʕϐ������J����
	this->bindValues();
	return true;
}


//####################  �Z�[�u�f�[�^����̃��[�h����  ####################
void CDatabase::CDBValue::Load(vector<string>& vectorbuf) {
	//���ʕϐ��Q�̓ǂݍ���
	int id, intVal;
	string str;
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		while(true) {
			str = Game.PopHead(vectorbuf);
			if(str == TS::SaveLoad::ValueSplit) {
				break;		//��؂�s���o����؂�ւ���
			}
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					::sscanf(str.c_str(), (TS::SQ::UserFlagName + "=%d\n").c_str(), &id, &intVal);
					*this->GetSubDB(type)->GetCell(id, this->GetSubDB(type)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).c_str())->GetBooleanPTR() = Game.IntToBool(intVal);
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					::sscanf(str.c_str(), (TS::SQ::UserIntegerName + "=%d\n").c_str(), &id, &intVal);
					*this->GetSubDB(type)->GetCell(id, this->GetSubDB(type)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).c_str())->GetIntegerPTR() = intVal;
					break;

				case TS::DB::DBValueIndices::CommonString:
					::sscanf(str.c_str(), (TS::SQ::UserStringName + "=").c_str(), &id);
					*this->GetSubDB(type)->GetCell(id, this->GetSubDB(type)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).c_str())->GetStringPTR() = Game.PopHead(vectorbuf);
					break;
			}
		}
	}

	//�X�N���v�g�ɒl�𔽉f������
	this->bindValues();
}


//####################  �Z�[�u�f�[�^�ւ̃Z�[�u����  ####################
void CDatabase::CDBValue::Save(string& strbuf) {
	//���ʕϐ��Q�̏����o��
	string valName;
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					valName = Game.Format(TS::SQ::UserFlagName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					strbuf += Game.Format(valName + "=%d\n", Game.BoolToInt(Game.SQ.GetSQBool(valName)));
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					valName = Game.Format(TS::SQ::UserIntegerName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					strbuf += Game.Format(valName + "=%d\n", Game.SQ.GetSQInteger(valName));
					break;

				case TS::DB::DBValueIndices::CommonString:
					valName = Game.Format(TS::SQ::UserStringName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					strbuf += Game.Format(valName + "=\n%s\n", Game.SQ.GetSQString(valName).c_str());
					break;
			}
		}
		strbuf += TS::SaveLoad::ValueSplit + "\n";		//��ʂ��Ƃ̋�؂�
	}
}


//####################  �f�[�^�x�[�X�̒��g�����O�Ƃ��ďo�͂���  ####################
void CDatabase::CDBValue::PrintData() {
	TS_LOG("\nDB: " + this->fileName);
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		//�����̃T�uDB�������Ă���ꍇ�̓C���f�b�N�X������
		if(this->DBs.GetCount() > 1) {
			TS_LOG("\nSubDB[" + std::to_string(type) + "]:");
		}

		//�ŐV�l�ŏo�͂���
		string valID, valName;
		TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
			valName = *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR();
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					valID = Game.Format(TS::SQ::UserFlagName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					TS_LOG(valID + " (%s) = %s", valName.c_str(), TS_BOOLSTR(Game.SQ.GetSQBool(valID)));
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					valID = Game.Format(TS::SQ::UserIntegerName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					TS_LOG(valID + " (%s) = %d", valName.c_str(), Game.SQ.GetSQInteger(valID));
					break;

				case TS::DB::DBValueIndices::CommonString:
					valID = Game.Format(TS::SQ::UserStringName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					TS_LOG(valID + " (%s) = \"%s\"", valName.c_str(), Game.SQ.GetSQString(valID).c_str());
					break;
			}
		}
	}
}


//####################  �w�肵����ʂ̕ϐ����X�g�𐶐����ĕԂ�  ####################
void CDatabase::CDBValue::GetSelfValueList(int type, CSelfValueData* dest) {
	dest->data.Clear();

	if(type <= static_cast<int>(TS::DB::DBValueIndices::CommonString) || static_cast<int>(TS::DB::DBValueIndices::Count) <= type) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̌ʕϐ���� [%d] �ł��B", type);
		return;
	}

	//�S�s���s
	TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
		dest->data.Add(CIDValueData(
			*this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR(),
			*this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR(),
			*this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetIntegerPTR()			//����l
			));
	}
}


//####################  �w�肵����ʂ̌ʕϐ����X�g���X�N���v�g�p�ɐ������ĕԂ�  ####################
CSelfValueData CDatabase::CDBValue::GetSelfValues(int type) {
	CSelfValueData selfValues;
	this->GetSelfValueList(type, &selfValues);
	return selfValues;
}


//####################  �X�N���v�g�ɋ��ʕϐ������J�E�l���Z�b�g����  ####################
void CDatabase::CDBValue::bindValues() {
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					Game.SQ.SetSQValueByVal(
						this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetBooleanPTR(),
						Game.Format(TS::SQ::UserFlagName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR())
						);
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					Game.SQ.SetSQValueByVal(
						this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetIntegerPTR(),
						Game.Format(TS::SQ::UserIntegerName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR())
						);
					break;

				case TS::DB::DBValueIndices::CommonString:
					Game.SQ.SetSQValueByVal(
						this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetStringPTR(),
						Game.Format(TS::SQ::UserStringName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR())
						);
					break;
			}
		}
	}
}
