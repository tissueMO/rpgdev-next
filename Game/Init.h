//###################################################################
//		初期化に必要な処理群
//###################################################################
#include "resource.h"

namespace Init {
	int SQHandle;				//スクリプトのハンドル
	string SQFileName;			//テストするスクリプトファイル名
	string SQTestCode;			//テストする関数名	

	int ExitCode;				//終了コード		


	//####################  DXライブラリの前処理  ####################
	void BeforeDXLibInit() {
		auto wndSize = Game.DB.getFDBSystem()->GetSize(TS::DB::System::WindowSize);
		CRGBColor transColor(Game.DB.getFDBSystem()->GetInteger(TS::DB::System::StandardTransparentColor));
		DxLib::SetDoubleStartValidFlag(false);			//多重起動禁止
		DxLib::SetAlwaysRunFlag(true);					//バックグラウンドでも処理を続行する
		DxLib::SetWindowUserCloseEnableFlag(false);		//閉じるボタンが押されたら自動で終了する
		DxLib::SetWindowIconID(IDI_ICON1);
		DxLib::SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_NATIVE);
		DxLib::SetGraphMode(wndSize.Width, wndSize.Height, 32);
		DxLib::SetWindowSize(wndSize.Width, wndSize.Height);
		DxLib::SetTransColor(transColor.Red, transColor.Green, transColor.Blue);
		DxLib::SetSysCommandOffFlag(true);
		DxLib::SetUseASyncChangeWindowModeFunction(Game.DB.getFDBSystem()->GetBoolean(TS::DB::System::AllowFullScreen), NULL, NULL);		//非同期的な最大化と通常ウィンドウの切り替えを許可する

		//補間方法
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


	//####################  コマンドライン引数を解析：戻り値がfalseの場合は即座に終了する必要があることを示す  ####################
	bool CheckStartOption() {
#ifdef _DEBUG
		TS_LOG("コマンドライン引数の解析を行います...");
		Init::ExitCode = EXIT_SUCCESS;

		// 引数を列挙する
		TS_COUNTLOOP(i, __argc) {
			TS_LOG("\targv[%d] = %s", i, __argv[i]);
		}
		Init::SQTestCode.clear();

		// コマンドライン文字列から起動モードを決定する
		if(__argc > 1) {	//プログラムファイルの他に追加オプションがあるかどうかを調べる
			string temp = __argv[1];
			const int DefaultArgsLength = 2;
			if(temp == "bt" && __argc == DefaultArgsLength + 2) {

				//バトルテスター: UnitID, パーティ情報 (ID:Lv|...)
				Game.RunMode = static_cast<int>(TS::System::RunMode::BattleTest);
				DxLib::SetMainWindowText("テストプレイ - バトル");
				TS_LOG("バトルテスターとして起動します。");

			} else if(temp == "mt" && DefaultArgsLength + 3 <= __argc && __argc <= DefaultArgsLength + 4) {

				//マップテスター: マップ名, X, Y, [初期化スクリプトコード]
				Game.RunMode = static_cast<int>(TS::System::RunMode::MapTest);
				DxLib::SetMainWindowText("テストプレイ - マップ");
				TS_LOG("マップテスターとして起動します。");

				//ファイル名を検証
				if(!PathFileExists(__argv[2])) {
					TS_MBE("与えられたマップファイルが見つかりませんでした。");
					Init::ExitCode = EXIT_FAILURE;
					return false;
				}
				if(__argc == DefaultArgsLength + 4) {
					Init::SQTestCode = __argv[5];

					//文字列リテラルの ".." が消えてしまうので、ここで付加する
					size_t pos = 0;
					while(true) {
						pos = Init::SQTestCode.find(((string)TS::SQ::UserStringName).substr(0, 2), pos);
						if(pos != string::npos) {
							//イコールを探す
							pos = Init::SQTestCode.find("=", pos);

							//開始のダブルクォート
							Init::SQTestCode.insert(pos + 1, "\"");

							//終端のダブルクォート
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

				//スクリプトコンパイラー: 出力先基準フルパス
				Game.RunMode = static_cast<int>(TS::System::RunMode::SQCompile);
				TS_LOG("スクリプトコンパイラーとして起動します。");

				//宛先フォルダーを設定
				string destDir = __argv[2];
				if(destDir.empty() == false && destDir.substr(destDir.size() - 1, 1) != "\\") {
					destDir += "\\";		//宛先フォルダーの末尾は必ず区切り記号にする
				}

				//ファイルリストを作成
				vector<string> files;
				CGameObject::GetFiles(files, "", ".nut", true);
				char buf[MAX_PATH];
				char sub[MAX_PATH];

				//逐次コンパイルしていく
				for(auto& fileName : files) {
					try {
						//生成先ファイルパスを設定
						::strcpy(buf, destDir.c_str());
						::strcat(buf, fileName.c_str());

						//生成先フォルダーを作成
						::strcpy(sub, buf);
						::PathRemoveFileSpec(sub);
						if(!::PathFileExists(sub) && !::MakeSureDirectoryPathExists(buf)) {
							throw "作成先のフォルダーの作成に失敗しました。";
						}

						Game.SQ.CompileToFile(fileName, buf);
						::printf("成功: %s\n", fileName.c_str());

					} catch(Sqrat::Exception e) {
						//失敗したファイルのリストを書き出す
						::printf("失敗: %s\n----> %s\n", fileName.c_str(), e.Message().c_str());
						Init::ExitCode = EXIT_FAILURE;
					} catch(char* e) {
						//失敗したファイルのリストを書き出す
						::printf("失敗: %s\n----> %s\n", fileName.c_str(), e);
						Init::ExitCode = EXIT_FAILURE;
					}
				}
				return false;		//コンパイルしたら終了する

			} else if(temp == "st" && DefaultArgsLength + 1 <= __argc && __argc <= DefaultArgsLength + 2) {

				//スクリプトテスター: スクリプト名, [テスト用パラメーター(テストする関数の呼出などの一文)]
				Game.RunMode = static_cast<int>(TS::System::RunMode::SQTest);
				DxLib::SetMainWindowText("テストプレイ - スクリプト");
				TS_LOG("スクリプトテスターとして起動します。");

				//ファイル名を検証
				if(!PathFileExists(__argv[2])) {
					TS_MBE("与えられたスクリプトが見つかりませんでした。");
					Init::ExitCode = EXIT_FAILURE;
					return false;
				}
				if(__argc == DefaultArgsLength + 2) {
					Init::SQTestCode = __argv[3];
				}
				Init::SQFileName = __argv[2];

			} else if(temp == "ac" && __argc == DefaultArgsLength + 2) {

				//DXアーカイブ: 対象フォルダーフルパス, 出力先フォルダーフルパス
				Game.RunMode = static_cast<int>(TS::System::RunMode::DXArchive);
				TS_LOG("DXアーカイバーとして起動します。");

				if(!Game.DXLib.EncodeArchive(__argv[2], __argv[3])) {
					Init::ExitCode = EXIT_FAILURE;
				}
				return false;		//処理が完了したら終了する

			} else if(temp == "sv" && __argc == DefaultArgsLength + 1) {

				//セーブデータ復号化: 対象データのインデックス
				TS_LOG("与えられたセーブデータを復号化します。");

				//セーブデータを復号化する
				string plain;
				string fileName = TS::DirectoryNames::SaveData + (string)__argv[2] + "_raw.txt";
				if(!Game.SaveLoad.GetPlainData(atoi(__argv[2]), plain)) {
					TS_MBE("セーブデータの復号化に失敗しました。");
					Init::ExitCode = EXIT_FAILURE;
					return false;
				}

				//復号化したデータを書き出す
				FILE *fp = ::fopen(fileName.c_str(), "w");
				::fputs(plain.c_str(), fp);
				::fclose(fp);
				return false;		//処理が完了したら終了する

			} else {
				TS_MBE("起動オプションが不正です。ゲームプログラムを起動できません。");
				return false;
			}
		} else {	//特に指定がない場合は、通常起動
			Game.RunMode = static_cast<int>(TS::System::RunMode::MainGame);
			DxLib::SetMainWindowText(Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
		}
#else
		//リリース時: コマンドライン文字列を受け付けない
		Game.RunMode = static_cast<int>(TS::System::RunMode::MainGame);
		DxLib::SetMainWindowText(Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
#endif
		return true;
		}


	//####################  起動モードに応じた初期設定  ####################
	void ApplyStartOption() {
		vector<string> spl;
		TS_LOG("起動モードに従ってゲームを開始します。");

		switch((TS::System::RunMode)Game.RunMode) {
			case TS::System::RunMode::BattleTest:
				//メインシーンはマップシーンから始める
				Game.ChangeToMapScene();

				//パーティ編成
				Game.DB.getVDBParty()->ClearMembers();
				Game.SplitString(spl, __argv[3], "|");
				for(auto& ch : spl) {
					vector<string> subspl;
					Game.SplitString(subspl, ch, ":");
					if(subspl.size() < 2) {
						continue;		//不正な形式
					}
					int id = atoi(subspl[0].c_str());
					int lv = atoi(subspl[1].c_str());

					//パーティに追加
					Game.DB.getVDBParty()->AddMember(id, -1);

					//レベル変更
					Game.DB.getVDBActor()->GetData(id)->LevelUp(lv - Game.DB.getVDBActor()->GetData(id)->Level);
				}

				//イベントハンドラー呼び出し
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_StartBattleTester, atoi(__argv[2]));
				break;

			case TS::System::RunMode::MapTest:
				//マップシーンから始める・イベントハンドラー呼び出し
				Game.ChangeToMapScene();
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_StartMapTester, __argv[2], atoi(__argv[3]), atoi(__argv[4]));
				break;

			case TS::System::RunMode::MainGame:
				//タイトルシーンから始める
				Game.ChangeToTitleScene();
				break;

			case TS::System::RunMode::SQTest:
				//仮にマップシーンで入り、スクリプトのテストを行う
				Game.ChangeToMapScene();
				Init::SQHandle = Game.SQ.Add(Init::SQFileName);		//テストするコードをインクルードした状態にする
		}

		//テスト起動コードが与えられている場合は、内部的にコードを生成して実行する
		if(!Init::SQTestCode.empty()) {
			TS_LOG(Game.Format("次のコードを実行します。\n>%s\n", Init::SQTestCode.c_str()));
			Sqrat::Script tester;
			tester.CompileString(Init::SQTestCode.c_str());
			tester.Run();
			
			if (Game.RunMode == static_cast<int>(TS::System::RunMode::SQTest)) {
				//スクリプトのテストが目的で実行されている場合はここで終了する
				CDebug::ActivateConsole();
				::printf("\n終了するにはEnterキーを押して下さい...(p:ログ出力)\n");
				if (CDebug::PauseConsole() == 'p') {
					//ログ出力して終了する
					Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Error));
				}
				else {
					Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
				}
			}
		}
	}


	//####################  コンソールを立ち上げる  ####################
	//返値はゲームプログラムを終了する必要があるかどうか
	bool SetupConsole() {
#ifdef _DEBUG
		if(!::AllocConsole()) {
			TS_MBE("デバッグコンソールの生成に失敗しました。");
			return true;		//ここでエラーが発生したらすぐに終了する
		}

		COORD size;
		size.X = 1100;
		size.Y = 800;
		::SetConsoleTitleA("デバッグ コンソール");
		::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), size);
		::MoveWindow(::GetConsoleWindow(), Game.ConsolePosition.X, Game.ConsolePosition.Y, size.X, size.Y, 0);
		if (::freopen("CONOUT$", "w", stdout) == NULL) {
			return true;
		}
		if (::freopen("CONIN$", "r", stdin) == NULL) {
			return true;
		}

		//----- フォントのサイズを変更するためにガンバル！
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
		//----- フォントサイズの変更はここまで

		ShowWindow(::GetConsoleWindow(), SW_SHOWNORMAL);
		SetForegroundWindow(::GetMainWindowHandle());
#endif
		return false;
	}
	}
