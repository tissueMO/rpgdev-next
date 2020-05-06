//###################################################################
//		�t���[�����[�g�ێ��Ǘ��N���X
//###################################################################
#include "Header.h"


//####################  �R���X�g���N�^�[  ####################
CFPS::CFPS() {
	this->frame_time = (int)(1000.0f / TS::System::FixedFPS);		//�P�t���[���̎��Ԃ̌v�Z
	this->wait_time = this->last_time = 0;
	this->tempSkipFrameCount = 0;
	this->nowSkipFrameCount = 0;

	//���t���b�V�����[�g�̒�������
	if(::GetRefreshRate() > TS::System::FixedFPS) {
		DxLib::SetWaitVSyncFlag(false);
	}
}


//####################  �Œ�t���[�����[�g��ύX  ####################
void CFPS::setSkipFrameCount(int val) {
	switch(val) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			this->fixedSkipFrameCount = val;
			break;

		default:
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ�Œ�t���[���X�L�b�v�� [%d] �͑Ή����Ă��܂���B", val);
	}
}


//####################  �P�t���[�����X�V  ####################
//60FPS���ێ�����E�F�C�g�����݁A�X�V���ׂ��t���[���ł���Ή�ʍX�V���s��
void CFPS::Update() {
	this->Wait();
	this->nowSkipFrameCount++;

#ifdef _DEBUG
	//���\�[�X����`��
	if(Game.EnabledSystemResource) {
		TS_NOBLEND;
		Game.DrawSystemResourceInfo();
	}
#endif

	//�t���[���X�L�b�v�͌Œ蕪�����l�����Ȃ�
	if(this->nowSkipFrameCount > this->fixedSkipFrameCount) {
	//if(this->nowSkipFrameCount > this->fixedSkipFrameCount && this->nowSkipFrameCount > this->tempSkipFrameCount) {
		//��ʍX�V
		this->nowSkipFrameCount = 0;
		this->tempSkipFrameCount = 0;
		DxLib::ScreenFlip();
	}
}


//####################  ��������������ꍇ�Ɏw��FPS�ɂȂ�悤�ɃE�F�C�g����  ####################
void CFPS::Wait() {
	this->wait_time = this->frame_time - (::GetNowCount() - this->last_time);

	if(this->wait_time < 0 && this->tempSkipFrameCount == 0) {
		//�ᑬ��: �t���[���x�����Ă���̂Ńt���[���X�L�b�v���邱�Ƃ���������
		this->tempSkipFrameCount = ceil(abs((double)this->wait_time) / (double)this->frame_time);

		//�v�Z���邽�тɃt���[���x�����N���Ă����ꍇ�A�ȉ������������Ă��܂��Ɖi���Ƀt���[���X�V�ł��Ȃ��Ȃ�
		//this->nowSkipFrameCount = 0;
	} else if(this->wait_time > 0) {
		//������: �҂����Ԃ�����΁A�w�莞�ԑ҂�
		this->tempSkipFrameCount = 0;
		DxLib::WaitTimer(this->wait_time);
	}
	this->last_time = DxLib::GetNowCount();
}

