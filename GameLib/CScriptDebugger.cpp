//###################################################################
//			Squirrel組み込みスクリプト管理/デバッガー
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];

void printCallStack();
void dprintf_squirrel(HSQUIRRELVM vm, const SQChar* str, ...);
void SQErrorMessage(string funcName, string msg);
void compileErrorHandler(HSQUIRRELVM v, const SQChar* msg, const SQChar* source, SQInteger line, SQInteger column);
void printSQCallStackAll(HSQUIRRELVM& v);
int errorHandler(HSQUIRRELVM v);
int debugHookFunc(HSQUIRRELVM v);


//####################  例外発生時に手動でこの関数へ飛ばすとスタックダンプを出してくれる  ####################
void printCallStack() {
	::sqstd_printcallstack(Game.SQ.GetVM());			//F10キーでステップ実行すること
	Game.SQ.RecompileAll();								//修正したらこの行を実行すること
}


//####################  printコマンドによって呼び出される関数  ####################
void dprintf_squirrel(HSQUIRRELVM vm, const SQChar* str, ...) {
	va_list arglist;
	va_start(arglist, str);
	::vsnprintf(::buf, sizeof(::buf) - 1, str, arglist);
	va_end(arglist);
	const bool temp = Game.NoTimeStamp;
	Game.NoTimeStamp = true;
	string strbuf = Game.Replace(::buf, "\n", "");		//改行は一旦すべてカットして自分で行う
	TS_LOG(strbuf);
	Game.NoTimeStamp = temp;
}


//####################  関数実行時のエラーメッセージ  ####################
void SQErrorMessage(string funcName, string msg) {
	TS_LOG("次の関数呼び出しにおいてランタイムエラーが発生しています。\n> %s関数\n", funcName.c_str());
	Game.NoTimeStamp = true;
	TS_LOG("エラー内容: \n" + msg);
	Game.NoTimeStamp = false;
}


//####################  コンパイルエラーハンドラー用のコールバック関数  ####################
void compileErrorHandler(HSQUIRRELVM v, const SQChar* msg, const SQChar* source, SQInteger line, SQInteger column) {
	TS_LOG("スクリプト内でコンパイルエラーが発生しています。\n>ファイル: %s -> %d 行 %d 文字目\n\nエラー内容:", string(source).empty() ? "(即席コード)" : source, line, column);
	Game.NoTimeStamp = true;
	TS_LOG(msg);
	Game.NoTimeStamp = false;
}


//####################  コールスタックを出力する  ####################
void printSQCallStackAll(HSQUIRRELVM& v) {
	int verIndex, srcLevel;
	SQObject val;
	SQStackInfos info;

	//呼び出し履歴: 逆順に取り出される（現在の階層が一番最初に来る）ので一旦コンテナーに入れて逆順にする
	deque<string> srcLevels;
	Game.NoTimeStamp = true;
	TS_LOG("\n----- スクリプト コールスタック -----------------------------------\n");
	for(srcLevel = 1; SQ_SUCCEEDED(::sq_stackinfos(v, srcLevel, &info)); srcLevel++) {
		string str;
		str = Game.Format("%s\nファイル: %s\n関数: [%s] %d 行目\n", (srcLevel == 1) ? "[現在の階層]" : "", string(info.source).empty() ? "(即席コード)" : info.source, info.funcname, info.line);

		//ローカル変数
		verIndex = 0;
		auto verName = ::sq_getlocal(v, srcLevel, verIndex);
		while(verName != nullptr) {
			::sq_getstackobj(v, -1, &val);
			switch(::sq_gettype(v, -1)) {
				case OT_NULL:
					str += Game.Format("\t%s = null\n", verName);
					break;
				case OT_INTEGER:
					str += Game.Format("\t%s = %d\n", verName, val._unVal.nInteger);
					break;
				case OT_FLOAT:
					str += Game.Format("\t%s = %f\n", verName, val._unVal.fFloat);
					break;
				case OT_STRING:
					str += Game.Format("\t%s = %s\n", verName, ::sq_objtostring(&val));
					break;
				case OT_BOOL:
					str += Game.Format("\t%s = %s\n", verName, (::sq_objtobool(&val)) ? "true" : "false");
					break;
				case OT_INSTANCE:
					str += Game.Format("\t%s = (インスタンス)\n", verName);
					break;
				case OT_ARRAY:
					str += Game.Format("\t%s = (配列)\n", verName);
					break;
				case OT_CLASS:
					str += Game.Format("\t%s = (クラス)\n", verName);
					break;
				case OT_CLOSURE:
					str += Game.Format("\t%s = (クロージャ)\n", verName);
					break;
				case OT_GENERATOR:
					str += Game.Format("\t%s = (ジェネレーター)\n", verName);
					break;
				case OT_TABLE:
					str += Game.Format("\t%s = (テーブル)\n", verName);
					break;
				case OT_THREAD:
					str += Game.Format("\t%s = (スレッド)\n", verName);
					break;
				case OT_USERDATA:
					str += Game.Format("\t%s = (ユーザーデータ型)\n", verName);
					break;
				case OT_USERPOINTER:
					str += Game.Format("\t%s = (ユーザーポインタ)\n", verName);
					break;
				default:
					str += Game.Format("\t%s = (その他)\n", verName);
					break;
			}
			verIndex++;
			verName = ::sq_getlocal(v, srcLevel, verIndex);
		}
		srcLevels.push_front(str);
	}
	for(int i = 1; srcLevels.empty() == false; i++) {
		TS_LOG("呼出階層: %d %s", i, srcLevels.front().c_str());
		srcLevels.pop_front();
	}
	TS_LOG("----------------------------------------\n");
}


//####################  ランタイムエラーハンドラー用のコールバック関数  ####################
int errorHandler(HSQUIRRELVM v) {
	string errText = Sqrat::LastErrorString(v);
	SQStackInfos info;
	string fileName;

	CDebug::ActivateConsole();
	TS_LOG("スクリプト内でランタイムエラーが発生しています。\n");
	Game.NoTimeStamp = true;

	//呼び出し履歴を出力
	::printSQCallStackAll(v);

	//呼び出し階層をチェック
	if(SQ_SUCCEEDED(::sq_stackinfos(v, 1, &info))) {
		fileName = info.source;
	}

	//エラー内容を示す
	TS_LOG("エラー内容:");
	::dprintf_squirrel(v, errText.c_str());
	Game.NoTimeStamp = false;

	//操作を受け付ける
	bool handled = false;
#ifdef _DEBUG
	const bool limitedDebug = Game.DB.getFDBSystem()->GetBoolean(TS::DB::System::EnabledLimitedDebug);
#else
	const bool limitedDebug = true;
#endif
	while(!handled) {
		if(limitedDebug) {
			//異常終了をなるべく避けるためにここでゲームを終了する
			::printf("\nスクリプトデバッガーは処理を続行できません。操作を入力して下さい...\n\t[E]プログラム中断 (ログ出力なし)\n\t[L]プログラム中断 (ログ出力あり)\n");
		} else {
			//異常終了をいとわない場合に行える操作を含める
			::printf("\nデバッグを行います。操作を入力して下さい...\n\t[E]プログラム中断 (ログ出力なし)\n\t[L]プログラム中断 (ログ出力あり)\n\t[I]エラーを無視して処理を続行 (危険: リスクを承知した上で実行して下さい)\n\t[C]任意のコードを実行 (危険: リスクを承知した上で実行して下さい)\n");
		}

		// NOTE: ランタイムエラー時にコンパイルをするとスタック順序が狂うため、メモリの不正アクセスが発生するため廃止した。
		//::printf("\nエラー箇所を修正して下さい。\n\t[E]プログラム中断\n\t[O]エラーが発生したスクリプトを再コンパイル (推奨)\n\t[A]すべてのスクリプトを再コンパイル (警告: 読込順序は順不同のため更なる不具合を引き起こす可能性があります)\n");
		//::printf("\nエラー箇所を修正して下さい。\n\t[E]プログラム中断\n\t[O]エラーが発生したスクリプトを再コンパイル (推奨)\n\t[S]シーンスクリプトを再コンパイル (注意: エラースクリプトがシーンとは無関係である場合は効果がありません)\n\t[A]すべてのスクリプトを再コンパイル (警告: 読み込み順序は順不同のため更なる不具合を引き起こす可能性があります)\n\t[I]エラーを無視して続行 (警告: 問題が処理されないまま続行するため非常に危険です)\n");

		switch(CDebug::PauseConsole()) {
			case 'e':	//プログラム中断: ログなし
				handled = true;
				Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
				break;

			case 'l':	//プログラム中断: ログあり
				handled = true;
				Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Error));
				break;

				//case 'o':	//単一コンパイル
				//	if(!fileName.empty()) {
				//		handled = true;
				//		Game.SQ.Add(fileName);
				//	} else {
				//		::puts("コンパイルできるスクリプトではありません。\n");
				//	}
				//	break;

				//case 's':	//シーンスクリプトコンパイル
				//	handled = true;
				//	Game.SQCompileCurrentScene();
				//	break;

				//case 'a':	//全スクリプトコンパイル
				//	handled = true;
				//	Game.SQ.RecompileAll();
				//	break;

			case 'i':	//無視して続行
				if(limitedDebug) {
					break;		//制限付きデバッグモードでは実行できない
				}
				handled = true;
				break;

			case 'c':	//即席コード実行
				if(limitedDebug) {
					break;		//制限付きデバッグモードでは実行できない
				}
				string code = Game.ScanConsole();
				if(code.empty()) {
					break;
				}
				if(code.substr(code.size() - 1, 1) == ";") {
					Sqrat::Script tester;
					try {
						::puts("");		//空行を作る
						tester.CompileString(code);
						tester.Run();
						::puts("即席スクリプトコードの実行が完了しました。");
					} catch(...) {
						::puts("\ncmdError: 与えられたコードを実行できません。\n");
					}
				} else {
					::puts("cmdError: コードの末尾にはセミコロンが必要です。");
				}
				break;
		}
	}

	CDebug::ActivateGameWindow();
	return 0;
}


//####################  デバッグ用のコールバック関数（１行実行するたびに呼び出される）  ####################
int debugHookFunc(HSQUIRRELVM v) {
	if(!Game.SQ.IsDebugging()) {
		return 0;						//デバッガーが起動していないときは処理しない
	} else if(Game.SQ.KillDebuggerFlag) {
		Game.SQ.EscapeDebugger();		//デバッガーの終了が要求されているときはただちに終了する
		return 0;
	}

	int srcLevel;
	SQStackInfos info;

	//呼び出し階層をチェック
	for(srcLevel = 1; SQ_SUCCEEDED(::sq_stackinfos(v, srcLevel, &info)); srcLevel++) {}

	//ステップオーバー/アウト中のとき、指定階層より深くなっているときは処理を停止しない
	if(Game.SQ.StepDestinationLevel < srcLevel) {
		return 0;
	}

	//呼び出し履歴を出力
	::printSQCallStackAll(v);
	Game.NoTimeStamp = false;

	//処理を停止する
	while(!Game.SQ.KillDebuggerFlag) {
		TS_EVERYLOOP_NOMT;
		//Game.CheckInput();
		::printf("\n[F5]ステップ実行終了  [F10]ステップオーバー  [F11]ステップイン  [F12]ステップアウト\n");
		const int key = CDebug::PauseConsole();

		switch(key) {
			case TS::System::SQDebugKey:
				//デバッガーを終了する
				Game.SQ.EscapeDebugger();
				TS_LOG("スクリプトのステップ実行が終了しました。");
				*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) = 2;		//この直後にデバッガーが起動しないようにする
				return 0;

			case TS::System::SQDebugStepOverKey:
				//ステップオーバー: 次の行の実行で呼び出し階層が深まった場合は戻るまで処理を進める
				TS_LOG("ステップオーバー: 次の行を実行します...");
				Game.SQ.StepDestinationLevel = srcLevel;
				return 0;

			case TS::System::SQDebugStepInKey:
				//ステップイン: 次の行の実行で呼び出し階層が深まってもすぐ止める
				TS_LOG("ステップイン: 次の行を実行します...");
				Game.SQ.StepDestinationLevel = INT_MAX;
				return 0;

			case TS::System::SQDebugStepOutKey:
				//ステップアウト: デバッガーは動いたままにする
				if(srcLevel - 1 <= 0) {
					//これ以上上の呼び出し階層がない場合はステップ実行を終了する
					TS_LOG("ステップアウト: これ以上一つ上のレベルがないため、ステップ実行が終了しました。");
					Game.SQ.EscapeDebugger();
				} else {
					//一つ上の呼び出し階層になるまで処理を進める
					TS_LOG("ステップアウト: 一つ上のレベルに進むまで実行します...");
					Game.SQ.StepDestinationLevel = srcLevel - 1;
				}
				return 0;

				//スクリプト実行中にエラーが起きた場合、復帰できなくなるため廃止
				//case 'c':
				//	//即席コード実行
				//	string code = Game.ScanConsole();
				//	CDebug::ActivateConsole();
				//	if(code.empty()) {
				//		break;
				//	}
				//	if(code.substr(code.size() - 1, 1) == ";") {
				//		Sqrat::Script tester;
				//		try {
				//			::puts("");		//空行を作る
				//			tester.CompileString(code);
				//			tester.Run();
				//			::puts("即席スクリプトコードの実行が完了しました。");
				//		} catch(...) {
				//			::puts("\ncmdError: 与えられたコードを実行できません。\n");
				//		}
				//	} else {
				//		::puts("cmdError: コードの末尾にはセミコロンが必要です。");
				//	}
				//	break;
		}
	}
	Game.SQ.EscapeDebugger();
	return 0;
}
