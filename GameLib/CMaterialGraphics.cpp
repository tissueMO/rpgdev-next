//###################################################################
//			�O���t�B�b�N�f�ފǗ��N���X��`
//###################################################################
#include "Header.h"


//####################  �O���t�B�b�N��ǉ�  ####################
int CMaterial::CMaterialGraphics::Add(int dbType, int dbID) {
	auto row = Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(dbID);
	return this->Add(*row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR());
}
int CMaterial::CMaterialGraphics::Add(string fileName) {
	CIDValueData NewItem;
	NewItem.Name = fileName;

	//�t�@�C���`�F�b�N
	if(fileName.empty()) {
		//�t�@�C��������̏ꍇ�͓ǂݍ��܂Ȃ�
		return -1;

		//DX�A�[�J�C�u��͎g�p�ł��Ȃ��̂Ŗ���
		//} else if(!PathFileExists(fileName.c_str())) {
		//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��O���t�B�b�N��ǉ����悤�Ƃ��܂����B\n>%s", fileName.c_str());
	}

	//���ɓ����O���t�B�b�N���ǂݍ��܂�Ă��Ȃ������ׂ�
	for(auto& it : this->Data.Items) {
		if(it.second.Name == fileName) {
			it.second.Value++;		//�Q�ƃJ�E���^�[���Z
			return it.second.ID;
		}
	}

	//���݂��Ȃ���ΐV���ɓǂݍ���
	NewItem.ID = DxLib::LoadGraph((TS::DirectoryNames::PrePath + fileName).c_str());
	NewItem.Value = 1;			//�Q�ƃJ�E���^�[������
	this->Data.Add(NewItem);
	return NewItem.ID;
}


//####################  �O���t�B�b�N�n���h���𕜌�  ####################
int CMaterial::CMaterialGraphics::Restore(int dbType, int dbID) {
	string fileName = *Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(dbID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR();

	//���ɓǂݍ��܂�Ă���O���t�B�b�N�f�[�^����t�@�C�����̈�v������̂�T��
	for(auto& item : this->Data.Items) {
		if(item.second.Name == fileName) {
			return item.second.ID;
		}
	}
	return -1;
}


//####################  �O���t�B�b�N���폜  ####################
void CMaterial::CMaterialGraphics::Delete(int hdl) {
	if(!this->Data.Exists(hdl)) {
		return;		//���݂��Ȃ��ꍇ�͏������Ȃ�
	}

	//���ɓ���̃O���t�B�b�N���c���Ă���ꍇ�͂܂����\�[�X���폜���Ȃ�
	for(auto& item : this->Data.Items) {
		if(item.second.ID == hdl) {
			item.second.Value--;
			if(item.second.Value > 0) {
				return;		//�܂��c���Ă���
			} else {
				break;		//�����c���Ă��Ȃ�
			}
		}
	}

	//�c���Ă��Ȃ���Ύ��ۂɍ폜����
	DxLib::DeleteGraph(hdl);
	this->Data.Delete(hdl);
}


//####################  �O���t�B�b�N�����ׂč폜  ####################
void CMaterial::CMaterialGraphics::Clear() {
	for(auto& it : this->Data.Items) {
		DxLib::DeleteGraph(it.second.ID);		//�n���h�������ׂč폜
	}
	this->Data.Clear();
}


//####################  �A�C�R���O���t�B�b�N��ǂݍ���  ####################
void CMaterial::CMaterialGraphics::LoadIconGraphic() {
	this->iconGraphicHandle = this->Add(Game.DB.getFDBSystem()->GetString(TS::DB::System::IconFileName));
	this->IconSize = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::IconSize);
}
