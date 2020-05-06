//###################################################################
//			���C�� ���[�`��
//###################################################################
#define _WINMAIN
#include "Header.h"
#include "Init.h"
void LoadMaterial(void* Data);
void RefreshCurrentPath();


//####################	���ׂĂ� �������� �͂��܂�  ####################
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//NOTE: ���������[�N�����o�������Ƃ��͈ȉ��̃R�����g�A�E�g����������B
	//::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);		//���������[�N�̌��o�p�t���O���Z�b�g����
	//::_CrtSetBreakAlloc(678);		//���������[�N���Ă���ӏ��Ŏ~�߂�

	// ���O�t�@�C�����o�͂��Ȃ�
	DxLib::SetOutApplicationLogValidFlag(false);

	// �J�����g�f�B���N�g���C��
	RefreshCurrentPath();

#ifndef _DEBUG
	//�����[�X��: DX�A�[�J�C�u�̐ݒ�
	DxLib::SetDXArchiveExtension(TS::System::DXArchiveExt.c_str());
	DxLib::SetDXArchiveKeyString(TS::System::DXArchivePass.c_str());
#endif

	TS_LOG("�y�v���O�������J�n����܂����z\n");
	
	// �V�X�e��DB���s���ēǂݍ���
	Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::System)]->Load(TS::FileNames::FDBs[static_cast<int>(TS::DB::FDBIndices::System)]);

	// �E�B���h�E��ԕ���
	Game.LoadWindowState();

	// �f�o�b�O�R���\�[���𗧂��グ��
	if (Init::SetupConsole()) {
		TS_MBW("�f�o�b�O�R���\�[���̗����グ�Ɏ��s���܂����B");
		return -1;
	}

	// �R�}���h���C����������́E�O����
	if(!Init::CheckStartOption()) {
		TS_LOG("�N�����[�h�̎w��ɂ��A�����������������߃v���O�������I�����܂��B");
		return Init::ExitCode;
	}

	// DX���C�u����������
	TS_LOG("DX���C�u�����̏��������s���܂�...");
	Init::BeforeDXLibInit();		//DX���C�u�����O����
	if(DxLib::DxLib_Init() == -1) {
		TS_MBW("���ɃQ�[���v���O�������N�����Ă��܂��B\n���d�N���͂ł��܂���B");
#ifdef _DEBUG
		::FreeConsole();
#endif
		return -1;
	}
	TS_LOG("DX���C�u�����̏��������������܂����B");
	DxLib::SetChangeScreenModeGraphicsSystemResetFlag(false);

	// GuruGuruSMF������������
	GGSINITIALIZE();
	GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, GetMainWindowHandle());
	TS_LOG("GuruGuruSMF�̏��������������܂����B");

	// �e��@�\���Z�b�g�A�b�v����
	Game.DB.LoadAll();
	Game.SQ.Setup();
	::LoadMaterial(nullptr);
	Init::ApplyStartOption();

	// ���C�����[�v
	TS_LOG("���C�����[�v���J�n���܂��B\n");
	while(true) {
		if(!Game.Pausing) {
			Game.DoSceneInput();
			Game.DoSceneDraw();
			TS_EVERYLOOP;
		} else {
			//�|�[�Y���̏���
			if(DxLib::ProcessMessage() == -1) {
				break;
			}
			Game.CheckInput();
			Game.RegularFunc.Do();

			//��ʂ��~������̂ōĕ`��̕K�v���Ȃ�
			//Game.FPSKeeper.Update();
		}
	}

	Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
	return 0;
}


//####################  �R�[���o�b�N�F�f�ޓǂݍ��ݏ����E�E�B���h�E�̏�Ԃ��ω������Ƃ��ɂ����s�����  ####################
void LoadMaterial(void *Data) {
	TS_LOG("���ׂĂ̔ėp�f�ނ�ǂݍ��݂܂�...");
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	Game.Material.Clear();

	//NowLoading�p�̃O���t�B�b�N���s�ǂݍ���
	int g = Game.Material.getGraphics()->Add(Game.DB.getFDBSystem()->GetString(TS::DB::System::NowLoadingFileName));

	//�A�C�R���O���t�B�b�N���s�ǂݍ���
	Game.Material.getGraphics()->LoadIconGraphic();

	//�f�[�^�x�[�X�̔ėp�f�ނ�ǂݍ��݊J�n
	TS_COUNTLOOP(type, static_cast<int>(TS::DB::DBMaterialIndices::Count)) {
		TS_COUNTLOOP(row, Game.DB.getFDBMaterial()->GetSubDB(type)->GetRowCount()) {
			if(type != static_cast<int>(TS::DB::DBMaterialIndices::Font)) {
				//�t�H���g�͔񓯊��œǂݍ��߂Ȃ�
				DxLib::SetUseASyncLoadFlag(true);	//�񓯊��ǂݍ��݁E��������
			}
			*Game.DB.getFDBMaterial()->GetSubDB(type)->GetCellFromIndex(row, Game.DB.getFDBMaterial()->GetSubDB(type)->GetColumnIndex(TS::DB::ColumnNameMaterialHandle))->GetIntegerPTR()
				= Game.DB.getFDBMaterial()->GetMaterialObject(type)->Add(
				type,
				*Game.DB.getFDBMaterial()->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
			DxLib::SetUseASyncLoadFlag(false);		//�񓯊��ǂݍ��݁E�����܂�
		}
	}

	//�X�N���v�g�ɓǂݍ��ݒ��̉�ʕ\�����ϑ�����
	while(DxLib::GetASyncLoadNum() > 0) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_NowLoading, g);
		TS_EVERYLOOP;
	}
	Game.Material.getGraphics()->Delete(g);

	TS_CLS;
	Game.FPSKeeper.Update();
	TS_LOG("���ׂĂ̔ėp�f�ނ̓ǂݍ��݂��������܂����B");
}


//####################  ���݂̃J�����g�f�B���N�g�������̃v���O�����̂���f�B���N�g���[�Ɏ����ŕ␳����  ####################
void RefreshCurrentPath() {
	char buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	char* PathSplit = ::strrchr(buf, '\\');
	*PathSplit = '\0';				//�����̋�؂�L�����폜����
	SetCurrentDirectory(buf);		//�J�����g�f�B���N�g����ݒ�
	Game.CurrentPath = string(buf) + "\\";		//�����ɋ�؂�L����t������
	//TS_LOG(buf);								//���݂̃J�����g�p�X��\������: ���ɂ���Ă͌l��񂪊܂܂��\�������邽�ߔp�~
}
