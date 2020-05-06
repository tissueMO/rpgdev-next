//###################################################################
//			メイン ルーチン
//###################################################################
#define _WINMAIN
#include "Header.h"
#include "Init.h"
void LoadMaterial(void* Data);
void RefreshCurrentPath();


//####################	すべては ここから はじまる  ####################
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//NOTE: メモリリークを検出したいときは以下のコメントアウトを解除する。
	//::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);		//メモリリークの検出用フラグをセットする
	//::_CrtSetBreakAlloc(678);		//メモリリークしている箇所で止める

	// ログファイルを出力しない
	DxLib::SetOutApplicationLogValidFlag(false);

	// カレントディレクトリ修正
	RefreshCurrentPath();

#ifndef _DEBUG
	//リリース時: DXアーカイブの設定
	DxLib::SetDXArchiveExtension(TS::System::DXArchiveExt.c_str());
	DxLib::SetDXArchiveKeyString(TS::System::DXArchivePass.c_str());
#endif

	TS_LOG("【プログラムが開始されました】\n");
	
	// システムDBを先行して読み込む
	Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::System)]->Load(TS::FileNames::FDBs[static_cast<int>(TS::DB::FDBIndices::System)]);

	// ウィンドウ状態復元
	Game.LoadWindowState();

	// デバッグコンソールを立ち上げる
	if (Init::SetupConsole()) {
		TS_MBW("デバッグコンソールの立ち上げに失敗しました。");
		return -1;
	}

	// コマンドライン引数を解析・前準備
	if(!Init::CheckStartOption()) {
		TS_LOG("起動モードの指定により、処理が完了したためプログラムを終了します。");
		return Init::ExitCode;
	}

	// DXライブラリ初期化
	TS_LOG("DXライブラリの初期化を行います...");
	Init::BeforeDXLibInit();		//DXライブラリ前処理
	if(DxLib::DxLib_Init() == -1) {
		TS_MBW("既にゲームプログラムが起動しています。\n多重起動はできません。");
#ifdef _DEBUG
		::FreeConsole();
#endif
		return -1;
	}
	TS_LOG("DXライブラリの初期化が完了しました。");
	DxLib::SetChangeScreenModeGraphicsSystemResetFlag(false);

	// GuruGuruSMFを初期化する
	GGSINITIALIZE();
	GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, GetMainWindowHandle());
	TS_LOG("GuruGuruSMFの初期化が完了しました。");

	// 各種機構をセットアップする
	Game.DB.LoadAll();
	Game.SQ.Setup();
	::LoadMaterial(nullptr);
	Init::ApplyStartOption();

	// メインループ
	TS_LOG("メインループが開始します。\n");
	while(true) {
		if(!Game.Pausing) {
			Game.DoSceneInput();
			Game.DoSceneDraw();
			TS_EVERYLOOP;
		} else {
			//ポーズ中の処理
			if(DxLib::ProcessMessage() == -1) {
				break;
			}
			Game.CheckInput();
			Game.RegularFunc.Do();

			//画面を停止させるので再描画の必要がない
			//Game.FPSKeeper.Update();
		}
	}

	Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
	return 0;
}


//####################  コールバック：素材読み込み処理・ウィンドウの状態が変化したときにも実行される  ####################
void LoadMaterial(void *Data) {
	TS_LOG("すべての汎用素材を読み込みます...");
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	Game.Material.Clear();

	//NowLoading用のグラフィックを先行読み込み
	int g = Game.Material.getGraphics()->Add(Game.DB.getFDBSystem()->GetString(TS::DB::System::NowLoadingFileName));

	//アイコングラフィックを先行読み込み
	Game.Material.getGraphics()->LoadIconGraphic();

	//データベースの汎用素材を読み込み開始
	TS_COUNTLOOP(type, static_cast<int>(TS::DB::DBMaterialIndices::Count)) {
		TS_COUNTLOOP(row, Game.DB.getFDBMaterial()->GetSubDB(type)->GetRowCount()) {
			if(type != static_cast<int>(TS::DB::DBMaterialIndices::Font)) {
				//フォントは非同期で読み込めない
				DxLib::SetUseASyncLoadFlag(true);	//非同期読み込み・ここから
			}
			*Game.DB.getFDBMaterial()->GetSubDB(type)->GetCellFromIndex(row, Game.DB.getFDBMaterial()->GetSubDB(type)->GetColumnIndex(TS::DB::ColumnNameMaterialHandle))->GetIntegerPTR()
				= Game.DB.getFDBMaterial()->GetMaterialObject(type)->Add(
				type,
				*Game.DB.getFDBMaterial()->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
			DxLib::SetUseASyncLoadFlag(false);		//非同期読み込み・ここまで
		}
	}

	//スクリプトに読み込み中の画面表示を委託する
	while(DxLib::GetASyncLoadNum() > 0) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_NowLoading, g);
		TS_EVERYLOOP;
	}
	Game.Material.getGraphics()->Delete(g);

	TS_CLS;
	Game.FPSKeeper.Update();
	TS_LOG("すべての汎用素材の読み込みが完了しました。");
}


//####################  現在のカレントディレクトリをこのプログラムのあるディレクトリーに自動で補正する  ####################
void RefreshCurrentPath() {
	char buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	char* PathSplit = ::strrchr(buf, '\\');
	*PathSplit = '\0';				//末尾の区切り記号を削除する
	SetCurrentDirectory(buf);		//カレントディレクトリを設定
	Game.CurrentPath = string(buf) + "\\";		//末尾に区切り記号を付加する
	//TS_LOG(buf);								//現在のカレントパスを表示する: 環境によっては個人情報が含まれる可能性があるため廃止
}
