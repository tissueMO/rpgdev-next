//###################################################################
//		定期実行メソッド処理機構 クラス定義
//###################################################################
#include "Header.h"

extern char buf[TS::System::BufferLengthMax];
void CheckApplicationExit();
void CheckSystemKeyInput();
void CheckSystemKeyInput_Debug();


//####################  コンストラクター  ####################
CRegularFunc::CRegularFunc() {
	Game.RegularFunc.AddFunction(::CheckApplicationExit);
	Game.RegularFunc.AddFunction(::CheckSystemKeyInput);
#ifdef _DEBUG
	Game.RegularFunc.AddFunction(::CheckSystemKeyInput_Debug);
#endif
}


//####################  定期実行：プログラムを終了すべきか調べる  ####################
void CheckApplicationExit() {
	if(::GetWindowUserCloseFlag()) {
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
	} else if(::ProcessMessage() != 0) {
		Game.ApplicationExit();
	} else if((::GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_START) != 0) {
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Question));
	}
}


//####################  定期実行：システムキーによる割り込み処理  ####################
void CheckSystemKeyInput() {
	if(Game.SQ.IsDebugging()) {
		return;		//スクリプトのデバッグ中は無効化
	}

	// [F11] でポーズ
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F11) == 1) {
		Game.Pausing = !Game.Pausing;
		TS_LOG(Game.Pausing ? "ゲームがポーズ中です。" : "ポーズが解除されました。");
		return;
	}

	// [F12] でリセット
	// NOTE: イベントの実行中やループ途中などでリセットされると、スクリプトの処理の続きがタイトル画面で進んでしまうので実現不可能。
	//if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F12) == 1) {
	//	TS_LOG("ゲームをリセットします。");
	//	Game.MT.Clear(false);
	//	Game.Sprite.Clear(-1, 255);
	//	Game.ForbidDraw = false;
	//	Game.ForbidInput = false;
	//	Game.DrawBright.Set(255, 255, 255);
	//	Game.Pausing = false;
	//	Game.ChangeToTitleScene();
	//}

	// [PrintScreen] でスクリーンショット撮影
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_SYSRQ) == 1) {
		//保存処理
		Game.CheckDirectory(TS::DirectoryNames::ScreenShot);	//フォルダーが存在しない場合は作成する
		string path = TS::DirectoryNames::ScreenShot + CGameObject::NowTimeToString("%Y-%m-%d %Hh%Mm%Ss") + ".png";
		DxLib::SaveDrawScreenToPNG(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, path.c_str());
		TS_LOG("スクリーンショット [" + path + "] を撮影しました。");
		return;
	}
}


//####################  定期実行：システムキーによる割り込み処理【デバッグ用】  ####################
void CheckSystemKeyInput_Debug() {
	//スクリプトのデバッグ中・ポーズ中は無効化
	if(Game.SQ.IsDebugging() || Game.Pausing) {
		return;
	}

	// [F3] で現在のマップに入り直す
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F3) == 1 && Game.GetCurrentMainSceneID() == static_cast<int>(TS::System::Scene::Map)) {
		TS_LOG("現在のマップに入り直します...");
		auto& map = Game.getSceneMap()->MapData;
		map.LoadMap(map.CurrentMapFileName, false);
		map.AfterLoadMap();
		return;
	}

	// [F4] で現在のシーンスクリプトを再コンパイル
	if(*Game.GetKeyInputCounterPTR(TS::System::SQDRecompileKey) == 1) {
		TS_LOG("現在のシーンスクリプトを再コンパイルします...");
		Game.SQCompileCurrentScene();
		return;
	}

	// [F5] でスクリプトデバッガー起動
	if(*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) == 1) {
		TS_LOG("スクリプトデバッガーを起動します...");
		*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) = 2;		//連続してデバッガーが起動するのを回避する
		Game.SQ.StartDebugger();
		return;
	}

	// [F8] でシステムリソース表示切替
	if(*Game.GetKeyInputCounterPTR(KEY_INPUT_F8) == 1) {
		Game.EnabledSystemResource = !Game.EnabledSystemResource;
		return;
	}
}
