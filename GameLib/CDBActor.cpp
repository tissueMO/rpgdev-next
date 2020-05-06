//###################################################################
//			�p�[�e�B�A�N�^�[ �f�[�^�x�[�X
//###################################################################
#include "Header.h"


//####################  �ǂݍ��ݏ���  ####################
bool CDatabase::CDBActor::Load(string fileName) {
	//�t�@�C������͓ǂݍ��܂��A�p�[�e�B�L����FDB�����荞��
	auto db = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char));
	auto chars = db->GetSubDB(0);
	this->fileName = db->GetDBFileName();

	this->DB.Clear();
	TS_COUNTLOOP(i, chars->GetRowCount()) {
		this->DB.Add(CDBActorData::CreateActorFromChar(*chars->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR()));
	}
	return true;
}


//####################  �Z�[�u�f�[�^����ǂݍ���  ####################
void CDatabase::CDBActor::Load(vector<string>& buf) {
	//�����o�[�ϐ��A�N���X�A�����A�X�L���A�Ԑڌ��ʂ𕜌����ēK�p����
	int length = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, length) {
		CDBActorData newData = CDBActorData::Load(buf);
		if(this->DB.Exists(newData.ID)) {
			//FDB�ɑ��݂���L�����̂ݕ�������
			*this->DB.GetFromID(newData.ID) = newData;
		}
		Game.PopHead(buf);		//��؂�L���̓ǂݎ̂�
	}
}


//####################  �Z�[�u�f�[�^�ɏ�������  ####################
void CDatabase::CDBActor::Save(string& buf) {
	//�����o�[�ϐ��A�N���X�A�����A�X�L���A�Ԑڌ��ʂ�ۑ�����
	buf += to_string(this->DB.GetCount()) + "\n";
	for(auto& ch : this->DB.Items) {
		ch.Save(buf);
	}
}


//####################  ���O�ɏo��  ####################
void CDatabase::CDBActor::PrintData() {
	TS_LOG("\nDB: �p�[�e�B�A�N�^�[���");
	for(auto& ch : this->DB.Items) {
		ch.PrintData();
	}
}


//####################  �j������  ####################
void CDatabase::CDBActor::Dispose() {
	this->DB.Clear();
}


//####################  �w�肵��ID�̃L������������  ####################
void CDatabase::CDBActor::InitData(int ID) {
	if(this->DB.Exists(ID)) {
		*this->DB.GetFromID(ID) = CDBActorData::CreateActorFromChar(ID);
	}
}


//####################  �w�肵���C���f�b�N�X�̃L������������  ####################
void CDatabase::CDBActor::InitDataFromIndex(int index) {
	TS_CHECKINDEX(index, 0, this->DB.GetCount()) {
		int ID = this->DB.GetFromIndex(index)->ID;
		*this->DB.GetFromIndex(index) = CDBActorData::CreateActorFromChar(ID);
	}
}
