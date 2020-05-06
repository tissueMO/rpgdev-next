//###################################################################
//		�������ɕK�v�ȏ����Q
//###################################################################
#include "resource.h"

namespace Init {
	int SQHandle;				//�X�N���v�g�̃n���h��
	string SQFileName;			//�e�X�g����X�N���v�g�t�@�C����
	string SQTestCode;			//�e�X�g����֐���	

	int ExitCode;				//�I���R�[�h		


	//####################  DX���C�u�����̑O����  ####################
	void BeforeDXLibInit() {
		auto wndSize = Game.DB.getFDBSystem()->GetSize(TS::DB::System::WindowSize);
		CRGBColor transColor(Game.DB.getFDBSystem()->GetInteger(TS::DB::System::StandardTransparentColor));
		DxLib::SetDoubleStartValidFlag(false);			//���d�N���֎~
		DxLib::SetAlwaysRunFlag(true);					//�o�b�N�O���E���h�ł������𑱍s����
		DxLib::SetWindowUserCloseEnableFlag(false);		//����{�^���������ꂽ�玩���ŏI������
		DxLib::SetWindowIconID(IDI_ICON1);
		DxLib::SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
		DxLib::SetGraphMode(wndSize.Width, wndSize.Height, 32);
		DxLib::SetWindowSize(wndSize.Width, wndSize.Height);
		DxLib::SetTransColor(transColor.Red, transColor.Green, transColor.Blue);
		DxLib::SetSysCommandOffFlag(true);
		DxLib::SetUseASyncChangeWindowModeFunction(Game.DB.getFDBSystem()->GetBoolean(TS::DB::System::AllowFullScreen), NULL, NULL);		//�񓯊��I�ȍő剻�ƒʏ�E�B���h�E�̐؂�ւ���������

		//��ԕ��@
		switch(Game.DB.getFDBSystem()->GetInteger(TS::DB::System::DrawInterpolation)) {
			case static_cast<int>(TS::System::DXInterpolation::Bilinear) :
				DxLib::SetDrawMode(DX_DRAWMODE_BILINEAR);
				DxLib::SetFullScreenScalingMode(DX_FSSCALINGMODE_BILINEAR);
				break;
			case static_cast<int>(TS::System::DXInterpolation::Nearest) :
				DxLib::SetDrawMode(DX_DRAWMODE_NEAREST);
				DxLib::SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
				break;
		}
	}


	//####################  �R�}���h���C����������́F�߂�l��false�̏ꍇ�͑����ɏI������K�v�����邱�Ƃ�����  ####################
	bool CheckStartOption() {
#ifdef _DEBUG
		TS_LOG("�R�}���h���C�������̉�͂��s���܂�...");
		Init::ExitCode = EXIT_SUCCESS;

		// ������񋓂���
		TS_COUNTLOOP(i, __argc) {
			TS_LOG("\targv[%d] = %s", i, __argv[i]);
		}
		Init::SQTestCode.clear();

		// �R�}���h���C�������񂩂�N�����[�h�����肷��
		if(__argc > 1) {	//�v���O�����t�@�C���̑��ɒǉ��I�v�V���������邩�ǂ����𒲂ׂ�
			string temp = __argv[1];
			const int DefaultArgsLength = 2;
			if(temp == "bt" && __argc == DefaultArgsLength + 2) {

				//�o�g���e�X�^�[: UnitID, �p�[�e�B��� (ID:Lv|...)
				Game.RunMode = static_cast<int>(TS::System::RunMode::BattleTest);
				DxLib::SetMainWindowText("�e�X�g�v���C - �o�g��");
				TS_LOG("�o�g���e�X�^�[�Ƃ��ċN�����܂��B");

			} else if(temp == "mt" && DefaultArgsLength + 3 <= __argc && __argc <= DefaultArgsLength + 4) {

				//�}�b�v�e�X�^�[: �}�b�v��, X, Y, [�������X�N���v�g�R�[�h]
				Game.RunMode = static_cast<int>(TS::System::RunMode::MapTest);
				DxLib::SetMainWindowText("�e�X�g�v���C - �}�b�v");
				TS_LOG("�}�b�v�e�X�^�[�Ƃ��ċN�����܂��B");

				//�t�@�C����������
				if(!PathFileExists(__argv[2])) {
					TS_MBE("�^����ꂽ�}�b�v�t�@�C����������܂���ł����B");
					Init::ExitCode = EXIT_FAILURE;
					return false;
				}
				if(__argc == DefaultArgsLength + 4) {
					Init::SQTestCode = __argv[5];

					//�����񃊃e������ ".." �������Ă��܂��̂ŁA�����ŕt������
					size_t pos = 0;
					while(true) {
						pos = Init::SQTestCode.find(((string)TS::SQ::UserStringName).substr(0, 2), pos);
						if(pos != string::npos) {
							//�C�R�[����T��
							pos = Init::SQTestCode.find("=", pos);

							//�J�n�̃_�u���N�H�[�g
							Init::SQTestCode.insert(pos + 1, "\"");

							//�I�[�̃_�u���N�H�[�g
							pos = Init::SQTestCode.find(";", pos + 1 + 1);
							if(pos != string::npos) {
								Init::SQTestCode.insert(pos, "\"");
							}
							continue;
						}
						break;
					}
				}

			} else if(temp == "sc" && __argc == DefaultArgsLength + 1) {

				//�X�N���v�g�R���p�C���[: �o�͐��t���p�X
				Game.RunMode = static_cast<int>(TS::System::RunMode::SQCompile);
				TS_LOG("�X�N���v�g�R���p�C���[�Ƃ��ċN�����܂��B");

				//����t�H���_�[��ݒ�
				string destDir = __argv[2];
				if(destDir.empty() == false && destDir.substr(destDir.size() - 1, 1) != "\\") {
					destDir += "\\";		//����t�H���_�[�̖����͕K����؂�L���ɂ���
				}

				//�t�@�C�����X�g���쐬
				vector<string> files;
				CGameObject::GetFiles(files, "", ".nut", true);
				char buf[MAX_PATH];
				char sub[MAX_PATH];

				//�����R���p�C�����Ă���
				for(auto& fileName : files) {
					try {
						//������t�@�C���p�X��ݒ�
						::strcpy(buf, destDir.c_str());
						::strcat(buf, fileName.c_str());

						//������t�H���_�[���쐬
						::strcpy(sub, buf);
						::PathRemoveFileSpec(sub);
						if(!::PathFileExists(sub) && !::MakeSureDirectoryPathExists(buf)) {
							throw "�쐬��̃t�H���_�[�̍쐬�Ɏ��s���܂����B";
						}

						Game.SQ.CompileToFile(fileName, buf);
						::printf("����: %s\n", fileName.c_str());

					} catch(Sqrat::Exception e) {
						//���s�����t�@�C���̃��X�g�������o��
						::printf("���s: %s\n----> %s\n", fileName.c_str(), e.Message().c_str());
						Init::ExitCode = EXIT_FAILURE;
					} catch(char* e) {
						//���s�����t�@�C���̃��X�g�������o��
						::printf("���s: %s\n----> %s\n", fileName.c_str(), e);
						Init::ExitCode = EXIT_FAILURE;
					}
				}
				return false;		//�R���p�C��������I������

			} else if(temp == "st" && DefaultArgsLength + 1 <= __argc && __argc <= DefaultArgsLength + 2) {

				//�X�N���v�g�e�X�^�[: �X�N���v�g��, [�e�X�g�p�p�����[�^�[(�e�X�g����֐��̌ďo�Ȃǂ̈ꕶ)]
				Game.RunMode = static_cast<int>(TS::System::RunMode::SQTest);
				DxLib::SetMainWindowText("�e�X�g�v���C - �X�N���v�g");
				TS_LOG("�X�N���v�g�e�X�^�[�Ƃ��ċN�����܂��B");

				//�t�@�C����������
				if(!PathFileExists(__argv[2])) {
					TS_MBE("�^����ꂽ�X�N���v�g��������܂���ł����B");
					Init::ExitCode = EXIT_FAILURE;
					return false;
				}
				if(__argc == DefaultArgsLength + 2) {
					Init::SQTestCode = __argv[3];
				}
				Init::SQFileName = __argv[2];

			} else if(temp == "ac" && __argc == DefaultArgsLength + 2) {

				//DX�A�[�J�C�u: �Ώۃt�H���_�[�t���p�X, �o�͐�t�H���_�[�t���p�X
				Game.RunMode = static_cast<int>(TS::System::RunMode::DXArchive);
				TS_LOG("DX�A�[�J�C�o�[�Ƃ��ċN�����܂��B");

				if(!Game.DXLib.EncodeArchive(__argv[2], __argv[3])) {
					Init::ExitCode = EXIT_FAILURE;
				}
				return false;		//����������������I������

			} else if(temp == "sv" && __argc == DefaultArgsLength + 1) {

				//�Z�[�u�f�[�^������: �Ώۃf�[�^�̃C���f�b�N�X
				TS_LOG("�^����ꂽ�Z�[�u�f�[�^�𕜍������܂��B");

				//�Z�[�u�f�[�^�𕜍�������
				string plain;
				string fileName = TS::DirectoryNames::SaveData + (string)__argv[2] + "_raw.txt";
				if(!Game.SaveLoad.GetPlainData(atoi(__argv[2]), plain)) {
					TS_MBE("�Z�[�u�f�[�^�̕������Ɏ��s���܂����B");
					Init::ExitCode = EXIT_FAILURE;
					return false;
				}

				//�����������f�[�^�������o��
				FILE *fp = ::fopen(fileName.c_str(), "w");
				::fputs(plain.c_str(), fp);
				::fclose(fp);
				return false;		//����������������I������

			} else {
				TS_MBE("�N���I�v�V�������s���ł��B�Q�[���v���O�������N���ł��܂���B");
				return false;
			}
		} else {	//���Ɏw�肪�Ȃ��ꍇ�́A�ʏ�N��
			Game.RunMode = static_cast<int>(TS::System::RunMode::MainGame);
			DxLib::SetMainWindowText(Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
		}
#else
		//�����[�X��: �R�}���h���C����������󂯕t���Ȃ�
		Game.RunMode = static_cast<int>(TS::System::RunMode::MainGame);
		DxLib::SetMainWindowText(Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
#endif
		return true;
		}


	//####################  �N�����[�h�ɉ����������ݒ�  ####################
	void ApplyStartOption() {
		vector<string> spl;
		TS_LOG("�N�����[�h�ɏ]���ăQ�[�����J�n���܂��B");

		switch((TS::System::RunMode)Game.RunMode) {
			case TS::System::RunMode::BattleTest:
				//���C���V�[���̓}�b�v�V�[������n�߂�
				Game.ChangeToMapScene();

				//�p�[�e�B�Ґ�
				Game.DB.getVDBParty()->ClearMembers();
				Game.SplitString(spl, __argv[3], "|");
				for(auto& ch : spl) {
					vector<string> subspl;
					Game.SplitString(subspl, ch, ":");
					if(subspl.size() < 2) {
						continue;		//�s���Ȍ`��
					}
					int id = atoi(subspl[0].c_str());
					int lv = atoi(subspl[1].c_str());

					//�p�[�e�B�ɒǉ�
					Game.DB.getVDBParty()->AddMember(id, -1);

					//���x���ύX
					Game.DB.getVDBActor()->GetData(id)->LevelUp(lv - Game.DB.getVDBActor()->GetData(id)->Level);
				}

				//�C�x���g�n���h���[�Ăяo��
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_StartBattleTester, atoi(__argv[2]));
				break;

			case TS::System::RunMode::MapTest:
				//�}�b�v�V�[������n�߂�E�C�x���g�n���h���[�Ăяo��
				Game.ChangeToMapScene();
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_StartMapTester, __argv[2], atoi(__argv[3]), atoi(__argv[4]));
				break;

			case TS::System::RunMode::MainGame:
				//�^�C�g���V�[������n�߂�
				Game.ChangeToTitleScene();
				break;

			case TS::System::RunMode::SQTest:
				//���Ƀ}�b�v�V�[���œ���A�X�N���v�g�̃e�X�g���s��
				Game.ChangeToMapScene();
				Init::SQHandle = Game.SQ.Add(Init::SQFileName);		//�e�X�g����R�[�h���C���N���[�h������Ԃɂ���
		}

		//�e�X�g�N���R�[�h���^�����Ă���ꍇ�́A�����I�ɃR�[�h�𐶐����Ď��s����
		if(!Init::SQTestCode.empty()) {
			TS_LOG(Game.Format("���̃R�[�h�����s���܂��B\n>%s\n", Init::SQTestCode.c_str()));
			Sqrat::Script tester;
			tester.CompileString(Init::SQTestCode.c_str());
			tester.Run();
			
			if (Game.RunMode == static_cast<int>(TS::System::RunMode::SQTest)) {
				//�X�N���v�g�̃e�X�g���ړI�Ŏ��s����Ă���ꍇ�͂����ŏI������
				CDebug::ActivateConsole();
				::printf("\n�I������ɂ�Enter�L�[�������ĉ�����...(p:���O�o��)\n");
				if (CDebug::PauseConsole() == 'p') {
					//���O�o�͂��ďI������
					Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Error));
				}
				else {
					Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
				}
			}
		}
	}


	//####################  �R���\�[���𗧂��グ��  ####################
	//�Ԓl�̓Q�[���v���O�������I������K�v�����邩�ǂ���
	bool SetupConsole() {
#ifdef _DEBUG
		if(!::AllocConsole()) {
			TS_MBE("�f�o�b�O�R���\�[���̐����Ɏ��s���܂����B");
			return true;		//�����ŃG���[�����������炷���ɏI������
		}

		COORD size;
		size.X = 1100;
		size.Y = 800;
		::SetConsoleTitleA("�f�o�b�O �R���\�[��");
		::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), size);
		::MoveWindow(::GetConsoleWindow(), Game.ConsolePosition.X, Game.ConsolePosition.Y, size.X, size.Y, 0);
		if (::freopen("CONOUT$", "w", stdout) == NULL) {
			return true;
		}
		if (::freopen("CONIN$", "r", stdin) == NULL) {
			return true;
		}

		//----- �t�H���g�̃T�C�Y��ύX���邽�߂ɃK���o���I
		HINSTANCE hLib = LoadLibrary(_T("KERNEL32.DLL"));
		if (hLib == NULL) {
			return true;
		}
		BOOL(WINAPI* GetConsoleFontInfo)(HANDLE, BOOL, DWORD, PCONSOLE_FONT_INFO) = (BOOL(WINAPI*)(HANDLE, BOOL, DWORD, PCONSOLE_FONT_INFO))GetProcAddress(hLib, "GetConsoleFontInfo");
		DWORD(WINAPI* GetNumberOfConsoleFonts)(VOID) = (DWORD(WINAPI*)(VOID))GetProcAddress(hLib, "GetNumberOfConsoleFonts");
		BOOL(WINAPI* SetConsoleFont)(HANDLE, DWORD) = (BOOL(WINAPI*)(HANDLE, DWORD))GetProcAddress(hLib, "SetConsoleFont");
		::FreeLibrary(hLib);

		DWORD fontNum = GetNumberOfConsoleFonts();
		CONSOLE_FONT_INFO* fonts = (CONSOLE_FONT_INFO*)malloc(sizeof(CONSOLE_FONT_INFO)* fontNum);
		GetConsoleFontInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, fontNum, fonts);
		for(int index = 0; index < fontNum; index++) {
			fonts[index].dwFontSize = GetConsoleFontSize(GetStdHandle(STD_OUTPUT_HANDLE), fonts[index].nFont);
			if(fonts[index].dwFontSize.X == 6 && fonts[index].dwFontSize.Y == 13) {
				SetConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), fonts[index].nFont);
				break;
			}
		}
		::free(fonts);
		//----- �t�H���g�T�C�Y�̕ύX�͂����܂�

		ShowWindow(::GetConsoleWindow(), SW_SHOWNORMAL);
		SetForegroundWindow(::GetMainWindowHandle());
#endif
		return false;
	}
	}
