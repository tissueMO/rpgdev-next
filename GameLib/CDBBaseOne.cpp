//###################################################################
//			�P��f�[�^�x�[�X
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �����o�b�t�@����s�𐶐�  ####################
void CDatabase::CDBBaseOne::LoadRow(const vector<string>* split) {
	auto newRow = DBRow();

	//��̐������Z���𐶐����đ������
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		newRow.push_back(CDBCell(this->columnTypes.at(i), split->at(i)));
	}

	this->AddRow(newRow);
}


//####################  ��̌^�����X�g���Z�b�g  ####################
void CDatabase::CDBBaseOne::SetColumnTypes(const vector<string>* split) {
	this->columnTypes.clear();
	for(auto& type : *split) {
		this->columnTypes.push_back(type);
	}
}


//####################  �񖼃��X�g���Z�b�g  ####################
void CDatabase::CDBBaseOne::SetColumnNames(const vector<string>* split) {
	this->columnNames.clear();
	for(auto& name : *split) {
		this->columnNames.push_back(name);
	}
}


//####################  �ۑ��p�e�L�X�g�𐶐�  ####################
void CDatabase::CDBBaseOne::SaveRow(string &strbuf) {
	//�S�s���s
	strbuf = "";
	for(auto& row : this->rows) {
		//�S����s
		TS_COUNTLOOP(i, this->GetColumnCount()) {
			if(i > 0) {
				strbuf += "\t";
			}
			strbuf += row[i].ToString();
		}
		strbuf += "\n";
	}
}


//####################  �s��ǉ�  ####################
void CDatabase::CDBBaseOne::AddRow(DBRow row) {
	this->rows.push_back(row);
}


//####################  �w��C���f�b�N�X�̍s���폜  ####################
void CDatabase::CDBBaseOne::DeleteRowFromIndex(int rowIndex) {
	if(rowIndex < 0 || this->rows.size() < rowIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�f�[�^�x�[�X [%s / Idx: %d] �ɂ����āA�͈͊O�̃C���f�b�N�X�̃f�[�^�x�[�X�s [%d] ���폜���悤�Ƃ��܂���", this->Name.c_str(), this->ID, rowIndex);
	} else {
		this->rows.erase(this->rows.begin() + rowIndex);
	}
}


//####################  �w��ID�̍s���폜  ####################
void CDatabase::CDBBaseOne::DeleteRowFromID(int rowID) {
	TS_COUNTLOOP(rowIndex, this->rows.size()) {
		if(*this->rows[rowIndex][static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR() == rowID) {
			this->DeleteRowFromIndex(rowIndex);
			return;
		}
	}
}


//####################  �s���N���A  ####################
void CDatabase::CDBBaseOne::ClearRow() {
	vector<DBRow>().swap(this->rows);
}


//####################  �w��s�ɗ��ǉ�  ####################
//�񖼂�cell�̒��Ɋi�[����Ă���
void CDatabase::CDBBaseOne::AddCell(int rowIndex, CDBCell* cell) {
	this->rows[rowIndex].push_back(*cell);
}


//####################  ���ǉ�  ####################
void CDatabase::CDBBaseOne::AddColumn(string columnName) {
	this->columnNames.push_back(columnName);
}


//####################  ����N���A  ####################
void CDatabase::CDBBaseOne::ClearColumn() {
	vector<string>().swap(this->columnNames);
	vector<string>().swap(this->columnTypes);
}


//####################  �񖼂���Z�������擾  ####################
CDatabase::CDBCell* CDatabase::CDBBaseOne::GetCell(int rowID, string columnName) {
	TS_COUNTLOOP(rowIndex, this->rows.size()) {
		if(*this->rows[rowIndex][static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR() == rowID) {
			return &this->rows[rowIndex][this->GetColumnIndex(columnName)];
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "�f�[�^�x�[�X [%s / Idx: %d] �ɂ����āA���݂��Ȃ��Z�� [ID: %d / ��: \"%s\"] ���Q�Ƃ���܂����B", this->Name.c_str(), this->ID, rowID, columnName.c_str());
	return nullptr;
}


//####################  ��C���f�b�N�X����Z�������擾  ####################
CDatabase::CDBCell* CDatabase::CDBBaseOne::GetCellFromIndex(int rowIndex, int columnIndex) {
	if(rowIndex < 0 || this->GetRowCount() <= rowIndex || columnIndex < 0 || this->GetColumnCount() <= columnIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�f�[�^�x�[�X [%s / Idx: %d] �ɂ����āA�͈͊O�̍s�Ɨ� [%d, %d] ���Q�Ƃ���܂����B", this->Name.c_str(), this->ID, rowIndex, columnIndex);
		return nullptr;
	} else {
		return &this->rows[rowIndex][columnIndex];
	}
}


//####################  ��C���f�b�N�X����񖼂��擾  ####################
string CDatabase::CDBBaseOne::GetColumnName(int columnIndex) {
	if(columnIndex < 0 || this->GetColumnCount() <= columnIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�f�[�^�x�[�X [%s / Idx: %d] �ɂ����āA�͈͊O�̗�C���f�b�N�X [%d] ���Q�Ƃ���܂����B", this->Name.c_str(), this->ID, columnIndex);
		return "";
	} else {
		return this->columnNames.at(columnIndex);
	}
}


//####################  ��C���f�b�N�X�����̌^�����擾  ####################
string CDatabase::CDBBaseOne::GetColumnType(int columnIndex) {
	if(columnIndex < 0 || this->GetColumnCount() <= columnIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�f�[�^�x�[�X [%s / Idx: %d] �ɂ����āA�͈͊O�̗�C���f�b�N�X [%d] ���Q�Ƃ���܂����B", this->Name.c_str(), this->ID, columnIndex);
		return "";
	} else {
		return this->columnTypes[columnIndex];
	}
}


//####################  �w��ID�̍s���擾  ####################
CDatabase::DBRow* CDatabase::CDBBaseOne::GetRowFromID(int rowID) {
	TS_COUNTLOOP(i, this->GetRowCount()) {
		if((*this->rows[i][static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR()) == rowID) {
			return &this->rows[i];
		}
	}
	//NOTE: �s�̎擾��C++�ł����s���Ȃ����߁AID�ɂ��擾�ł͌x�����o���Ȃ��B
	return nullptr;
}


//####################  �w��C���f�b�N�X�̍s���擾  ####################
CDatabase::DBRow* CDatabase::CDBBaseOne::GetRowFromIndex(int rowIndex) {
	if(rowIndex < 0 || this->GetRowCount() <= rowIndex) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�f�[�^�x�[�X [%s / Idx: %d] �ɂ����āA�͈͊O�̍s�C���f�b�N�X [%d] ���Q�Ƃ���܂����B", this->Name.c_str(), this->ID, rowIndex);
		return nullptr;
	} else {
		return &this->rows.at(rowIndex);
	}
}


//####################  �s�����擾  ####################
int CDatabase::CDBBaseOne::GetRowCount() {
	try {
		return this->rows.size();
	} catch(...) {
		return 0;
	}
}


//####################  �񐔂��擾  ####################
int CDatabase::CDBBaseOne::GetColumnCount() {
	return this->columnNames.size();
}


//####################  �񖼂����C���f�b�N�X���擾  ####################
int CDatabase::CDBBaseOne::GetColumnIndex(string columnName) {
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		if(columnName == this->columnNames.at(i)) {
			return i;
		}
	}
	return -1;
}


//####################  ��ID����FixedID�ɕϊ�  ####################
int CDatabase::CDBBaseOne::ConvertToFixedID(int visibleID) {
	TS_COUNTLOOP(i, this->GetRowCount()) {
		if(*this->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR() == visibleID) {
			return *this->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR();
		}
	}
	return -1;
}


//####################  FixedID�����ID�ɕϊ�  ####################
int CDatabase::CDBBaseOne::ConvertToVisibleID(int fixedID) {
	DBRow* row = this->GetRowFromID(fixedID);
	if(row == nullptr) {
		return -1;
	} else {
		return *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID)).GetIntegerPTR();
	}
}


//####################  �S�̂�������  ####################
void CDatabase::CDBBaseOne::InitDB() {
	//�S�s���s
	for(auto& row : this->rows) {
		this->InitRow(*row[static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)].GetIntegerPTR());
	}
}


//####################  �sID���w�肵�čs��������  ####################
void CDatabase::CDBBaseOne::InitRow(int rowID) {
	//�S����s
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		this->InitCell(rowID, this->columnNames[i].c_str());
	}
}


//####################  �s�C���f�b�N�X���w�肵�Ďw��s��������  ####################
void CDatabase::CDBBaseOne::InitRowFromIndex(int rowIndex) {
	//�S����s
	TS_COUNTLOOP(i, this->GetColumnCount()) {
		this->InitCellFromIndex(rowIndex, i);
	}
}


//####################  �񖼂���w��ID�s�̎w��Z����������  ####################
void CDatabase::CDBBaseOne::InitCell(int rowID, string columnName) {
	this->GetCell(rowID, columnName)->Init();
}


//####################  ��C���f�b�N�X����w��C���f�b�N�X�s�̎w��Z����������  ####################
void CDatabase::CDBBaseOne::InitCellFromIndex(int rowIndex, int columnIndex) {
	this->GetCellFromIndex(rowIndex, columnIndex)->Init();
}

