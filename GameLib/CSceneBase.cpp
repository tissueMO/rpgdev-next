//###################################################################
//			���V�[����`
//###################################################################
#include "Header.h"
extern void RestoreTilePallet();


//####################  �R���X�g���N�^�[  ####################
CSceneBase::CSceneBase() {
	this->SubSceneID = -1;

	//�O���t�B�b�N�����p�R�[���o�b�N�֐���o�^
	DxLib::SetRestoreGraphCallback(::RestoreTilePallet);
}


//####################  �V�[���`��  ####################
void CSceneBase::Draw() {
	if(this->SQHandle != -1) {
		if(this->SubSceneID == -1 || this->SQSubHandle != -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainDraw);
		} else {
			//���C���V�[����u�������ăT�u�V�[���Ɉڍs���Ă���
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDraw);
			return;
		}
	}
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDraw);
	}
}


//####################  �V�[������  ####################
void CSceneBase::Input() {
	//�T�u�V�[�����D��
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubInput);
	} else if(this->SQHandle != -1) {
		if(this->SubSceneID == -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainInput);
		} else {
			//���C���V�[����u�������ăT�u�V�[���Ɉڍs���Ă���
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubInput);
		}
	}
}


//####################  �V�[��������  ####################
void CSceneBase::Setup() {
	if(this->SQHandle != -1) {
		if(this->SubSceneID == -1 || this->SQSubHandle != -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainSetup);
		} else {
			//���C���V�[����u�������ăT�u�V�[���Ɉڍs���Ă���
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
			return;
		}
	}
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
	}
}


//####################  �V�[���j����  ####################
void CSceneBase::Dispose() {
	if(this->SQHandle != -1) {
		if(this->SubSceneID == -1 || this->SQSubHandle != -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainDispose);
		} else {
			//���C���V�[����u�������ăT�u�V�[���Ɉڍs���Ă���
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
			return;
		}
	}
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
	}
}
