//###################################################################
//		������s���\�b�h�����@�\ �N���X��`
//###################################################################
#include "Header.h"

extern char buf[TS::System::BufferLengthMax];
void CheckApplicationExit();
void CheckSystemKeyInput();
void CheckSystemKeyInput_Debug();


//####################  �R���X�g���N�^�[  ####################
CRegularFunc::CRegularFunc() {
	Game.RegularFunc.AddFunction(::CheckApplicationExit);
	Game.RegularFunc.AddFunction(::CheckSystemKeyInput);
#ifdef _DEBUG
	Game.RegularFunc.AddFunction(::CheckSystemKeyInput_Debug);
#endif
}


//####################  ������s�F�v���O�������I�����ׂ������ׂ�  ####################
void CheckApplicationExit() {
	if(::GetWindowUserCloseFlag()) {
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
	} else if(::ProcessMessage() != 0) {
		Game.ApplicationExit();
	} else if((::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_START) != 0) {
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Question));
	}
}


//####################  ������s�F�V�X�e���L�[�ɂ�銄�荞�ݏ���  ####################
void CheckSystemKeyInput() {
	if(Game.SQ.IsDebugging()) {
		return;		//�X�N���v�g�̃f�o�b�O���͖�����
	}

	// [F11] �Ń|�[�Y
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F11) == 1) {
		Game.Pausing = !Game.Pausing;
		TS_LOG(Game.Pausing ? "�Q�[�����|�[�Y���ł��B" : "�|�[�Y����������܂����B");
		return;
	}

	// [F12] �Ń��Z�b�g
	// NOTE: �C�x���g�̎��s���⃋�[�v�r���ȂǂŃ��Z�b�g�����ƁA�X�N���v�g�̏����̑������^�C�g����ʂŐi��ł��܂��̂Ŏ����s�\�B
	//if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F12) == 1) {
	//	TS_LOG("�Q�[�������Z�b�g���܂��B");
	//	Game.MT.Clear(false);
	//	Game.Sprite.Clear(-1, 255);
	//	Game.ForbidDraw = false;
	//	Game.ForbidInput = false;
	//	Game.DrawBright.Set(255, 255, 255);
	//	Game.Pausing = false;
	//	Game.ChangeToTitleScene();
	//}

	// [PrintScreen] �ŃX�N���[���V���b�g�B�e
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_SYSRQ) == 1) {
		//�ۑ�����
		Game.CheckDirectory(TS::DirectoryNames::ScreenShot);	//�t�H���_�[�����݂��Ȃ��ꍇ�͍쐬����
		string path = TS::DirectoryNames::ScreenShot + CGameObject::NowTimeToString("%Y-%m-%d %Hh%Mm%Ss") + ".png";
		DxLib::SaveDrawScreenToPNG(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, path.c_str());
		TS_LOG("�X�N���[���V���b�g [" + path + "] ���B�e���܂����B");
		return;
	}
}


//####################  ������s�F�V�X�e���L�[�ɂ�銄�荞�ݏ����y�f�o�b�O�p�z  ####################
void CheckSystemKeyInput_Debug() {
	//�X�N���v�g�̃f�o�b�O���E�|�[�Y���͖�����
	if(Game.SQ.IsDebugging() || Game.Pausing) {
		return;
	}

	// [F3] �Ō��݂̃}�b�v�ɓ��蒼��
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F3) == 1 && Game.GetCurrentMainSceneID() == static_cast<int>(TS::System::Scene::Map)) {
		TS_LOG("���݂̃}�b�v�ɓ��蒼���܂�...");
		auto& map = Game.getSceneMap()->MapData;
		map.LoadMap(map.CurrentMapFileName, false);
		map.AfterLoadMap();
		return;
	}

	// [F4] �Ō��݂̃V�[���X�N���v�g���ăR���p�C��
	if(*Game.GetKeyInputCounterPTR(TS::System::SQDRecompileKey) == 1) {
		TS_LOG("���݂̃V�[���X�N���v�g���ăR���p�C�����܂�...");
		Game.SQCompileCurrentScene();
		return;
	}

	// [F5] �ŃX�N���v�g�f�o�b�K�[�N��
	if(*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) == 1) {
		TS_LOG("�X�N���v�g�f�o�b�K�[���N�����܂�...");
		*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) = 2;		//�A�����ăf�o�b�K�[���N������̂��������
		Game.SQ.StartDebugger();
		return;
	}

	// [F8] �ŃV�X�e�����\�[�X�\���ؑ�
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F8) == 1) {
		Game.EnabledSystemResource = !Game.EnabledSystemResource;
		return;
	}
}
