//###################################################################
//			�ėp�N���X��`�̕⑫
//###################################################################
//NOTE: �O���錾��Q�[���I�u�W�F�N�g���ւ�镔���̓w�b�_�[�ɏ������L�q�ł��Ȃ����߁A�ʓr�\�[�X�t�@�C���Œ�`����B
#include "Header.h"
char buf[TS::System::BufferLengthMax];


//####################  ���W���F�R���X�g���N�^�[  ####################
CPoint::CPoint(CSize size) {
	this->X = size.Width;
	this->Y = size.Height;
}


//####################  ���W���F�����񂩂�Z�b�g  ####################
void CPoint::Set(string text) {
	vector<string> buf;
	Game.SplitString(buf, text, ":");
	if(buf.size() == 2) {
		return this->Set(atoi(buf[0].c_str()), atoi(buf[1].c_str()));
	}
}


//####################  ���W���F������ɂ���  ####################
SQ_NOBIND string CPoint::ToString() {
	return Game.Format("%d:%d", this->X, this->Y);
}


//####################  �T�C�Y���F�R���X�g���N�^�[  ####################
CSize::CSize(CPoint pos) {
	this->Width = pos.X;
	this->Height = pos.Y;
}


//####################  �T�C�Y���F�����񂩂�Z�b�g  ####################
void CSize::Set(string text) {
	vector<string> buf;
	Game.SplitString(buf, text, "x");
	if(buf.size() == 2) {
		return this->Set(atoi(buf[0].c_str()), atoi(buf[1].c_str()));
	}
}


//####################  �T�C�Y���F������ɂ���  ####################
string CSize::ToString() {
	return Game.Format("%dx%d", this->Width, this->Height);
}


//####################  RGB�F���F�Z�b�g  ####################
void CRGBColor::Set(int r, int g, int b) {
	r = Game.WithIn(r, 0, 255);
	g = Game.WithIn(g, 0, 255);
	b = Game.WithIn(b, 0, 255);
	this->Red = r;
	this->Green = g;
	this->Blue = b;
}


//####################  ���f�[�^��������Z�b�g  ####################
void CSoundData::Load(string rawData) {
	vector<string> spl;
	Game.SplitString(spl, rawData, "|");
	if(spl.size() == 4) {
		this->Load(spl[0], atoi(spl[1].c_str()), atoi(spl[2].c_str()), atoi(spl[3].c_str()));
	} else {
		TS_LOG("�^����ꂽ�T�E���h�f�[�^�����񂪕s���ł��B\n>%s", rawData.c_str());
	}
}


//####################  �T�E���h���F�����Z�b�g  ####################
void CSoundData::Load(string fileName, int vol, int pit, int pan) {
	//�ȑO�̃f�[�^��j������
	if(this->Handle != -1) {
		if (this->FileName == fileName && this->Volume == vol && this->Pitch == pit && this->Panpot == pan) {
			// ���݂̃f�[�^�Ɉ�v���Ă���ꍇ�̓��[�h���X�L�b�v
			return;
		}

		this->Dispose();
	}

	this->FileName = fileName;
	this->Volume = vol;			//���ۂ̉��ʂ͍Đ����Ɏ������������
	this->Pitch = Game.WithIn(pit, TS::Material::Common_MinPitch, TS::Material::Common_MaxPitch);
	this->Panpot = Game.WithIn(pan, TS::Material::MinPan, TS::Material::MaxPan);

	//NOTE: �s�b�`���[�g��p�ɂɕύX����Ɨ�����s�����
	if(strcmp(PathFindExtension(fileName.c_str()), ".mid") != 0) {
		//MIDI�ȊO�̓s�b�`���[�g��ǂݍ��ݑO�ɃZ�b�g����
		DxLib::SetCreateSoundPitchRate(Game.WithIn(5 * this->Pitch, TS::Material::MinPitch, TS::Material::MaxPitch));
	}
	this->Handle = Game.Material.getSounds()->Add(fileName);
	DxLib::SetCreateSoundPitchRate(0);
}


//####################  �T�E���h���F�Đ�  ####################
void CSoundData::Play(int playType, bool oncePlay) {
	if(playType == static_cast<int>(TS::Material::PlayType::Loop) || !oncePlay) {
		Game.Material.getSounds()->Play(this->Handle, playType, this->Volume, this->Pitch, this->Panpot);
	} else {
		Game.Material.getSounds()->PlayOnce(this->Handle, playType, this->Volume, this->Pitch, this->Panpot);
	}
}


//####################  �T�E���h���F��~  ####################
void CSoundData::Stop() {
	Game.Material.getSounds()->Stop(this->Handle);
}


//####################  �T�E���h���F�ꎞ��~  ####################
void CSoundData::Pause() {
	Game.Material.getSounds()->Pause(this->Handle);
}


//####################  �T�E���h���F�ĊJ  ####################
void CSoundData::Resume(bool loopMode) {
	Game.Material.getSounds()->Resume(this->Handle, loopMode);
}


//####################  �T�E���h���F�j��  ####################
void CSoundData::Dispose() {
	Game.Material.getSounds()->Delete(this->Handle);
	this->Handle = -1;
}


//####################  �X�N���v�g�݊��z��i�����^�j�F�擾  ####################
int CIntegerArray::Get(int index) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		return this->data[index];
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��C���f�b�N�X [%d] �ŃX�N���v�g�݊��z��i�����^�j�̗v�f���Q�Ƃ���܂����B", index);
		return 0;
	}
}


//####################  �X�N���v�g�݊��z��i�����^�j�F�ݒ�  ####################
void CIntegerArray::Set(int index, int value) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		this->data[index] = value;
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��C���f�b�N�X [%d] �ŃX�N���v�g�݊��z��i�����^�j�̗v�f���Q�Ƃ���܂����B", index);
		return;
	}
}


//####################  �X�N���v�g�݊��z��i�����^�j�F�}��  ####################
void CIntegerArray::Insert(int index, int value) {
	if(index == -1) {
		this->data.push_back(value);
	} else {
		index = Game.WithIn(index, 0, this->data.size() - 1);
		this->data.insert(this->data.begin() + index, value);
	}
}


//####################  �X�N���v�g�݊��z��i�����^�j�F�폜  ####################
void CIntegerArray::Delete(int index) {
	TS_CHECKINDEX(index, 0, static_cast<int>(this->data.size())) {
		this->data.erase(this->data.begin() + index);
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃C���f�b�N�X [%d] �ŃX�N���v�g�݊��z��i�����^�j�̗v�f���폜���悤�Ƃ��܂����B", index);
	}
}


//####################  �X�N���v�g�݊��z��i�����^�j�F�l����  ####################
bool CIntegerArray::Exists(int value) {
	for(auto& val : this->data) {
		if(value == val) {
			return true;
		}
	}
	return false;
}


//####################  �X�N���v�g�݊��z��i������^�j�F�擾  ####################
string CStringArray::Get(int index) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		return this->data[index];
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��C���f�b�N�X [%d] �ŃX�N���v�g�݊��z��i������^�j�̗v�f���Q�Ƃ���܂����B", index);
		return "";
	}
}


//####################  �X�N���v�g�݊��z��i������^�j�F�ݒ�  ####################
void CStringArray::Set(int index, string value) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		this->data[index] = value;
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��C���f�b�N�X [%d] �ŃX�N���v�g�݊��z��i������^�j�̗v�f���Q�Ƃ���܂����B", index);
		return;
	}
}


//####################  �X�N���v�g�݊��z��i������^�j�F�}��  ####################
void CStringArray::Insert(int index, string value) {
	if(index == -1) {
		this->data.push_back(value);
	} else {
		index = Game.WithIn(index, 0, this->data.size() - 1);
		this->data.insert(this->data.begin() + index, value);
	}
}


//####################  �X�N���v�g�݊��z��i������^�j�F�폜  ####################
void CStringArray::Delete(int index) {
	TS_CHECKINDEX(index, 0, static_cast<int>(this->data.size())) {
		this->data.erase(this->data.begin() + index);
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃C���f�b�N�X [%d] �ŃX�N���v�g�݊��z��i������^�j�̗v�f���폜���悤�Ƃ��܂����B", index);
	}
}


//####################  �X�N���v�g�݊��z��i������^�j�F�l����  ####################
bool CStringArray::Exists(string value) {
	for(auto& val : this->data) {
		if(value == val) {
			return true;
		}
	}
	return false;
}
