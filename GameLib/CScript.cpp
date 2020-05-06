//###################################################################
//			Squirrel組み込みスクリプト管理/クラス定義
//###################################################################
#include "Header.h"
#include <sqstdmath.h>
#include <sqstdstring.h>
extern char buf[TS::System::BufferLengthMax];

extern void printCallStack();
extern void dprintf_squirrel(HSQUIRRELVM vm, const SQChar* str, ...);
extern void SQErrorMessage(string funcName, string msg);
extern void compileErrorHandler(HSQUIRRELVM v, const SQChar* msg, const SQChar* source, SQInteger line, SQInteger column);
extern void printSQCallStackAll(HSQUIRRELVM& v);
extern int errorHandler(HSQUIRRELVM v);
extern int debugHookFunc(HSQUIRRELVM v);


//####################  コンストラクター  ####################
CScript::CScript() {
	this->sqvm = sq_open(50 * 1024);
	::sq_notifyallexceptions(this->sqvm, true);			//すべての例外を捕捉する
	::sq_setprintfunc(this->sqvm, ::dprintf_squirrel);	//自作の出力ハンドラーをセット
	::sq_enabledebuginfo(this->sqvm, true);				//常にデバッグ情報付きでコンパイルする
	::sq_setcompilererrorhandler(this->sqvm, ::compileErrorHandler);		//コンパイルエラーハンドラーをセット
	//::sqstd_seterrorhandlers(this->sqvm);				//標準のエラーハンドラーをセット
	::sq_newclosure(this->sqvm, ::errorHandler, 0);		//ランタイムエラーハンドラー用関数をスタックへ
	::sq_seterrorhandler(this->sqvm);					//この関数をランタイムエラーハンドラーとしてセット
	Sqrat::DefaultVM::Set(this->sqvm);

	int top;

	//Squirrel標準ライブラリ: 数学API
	top = ::sq_gettop(sqvm);			//現在のスタックトップ値を取得
	::sq_pushroottable(sqvm);			//ルートテーブルをスタックに積む
	::sqstd_register_mathlib(sqvm);
	::sq_settop(sqvm, top);				//スタックトップ値を戻す

	//Squirrel標準ライブラリ: 文字列API
	top = ::sq_gettop(sqvm);			//現在のスタックトップ値を取得
	::sq_pushroottable(sqvm);			//ルートテーブルをスタックに積む
	::sqstd_register_stringlib(this->sqvm);
	::sq_settop(sqvm, top);				//スタックトップ値を戻す

	//定数：デバッグモードであるかどうか
#ifdef _DEBUG
	//デバッガーをセットする
	::sq_newclosure(this->sqvm, ::debugHookFunc, 0);	//デバッガー用関数をスタックへ
	::sq_setdebughook(this->sqvm);						//この関数をデバッガーとしてセット
	Sqrat::ConstTable().Const("DEBUG", TRUE);
#else
	Sqrat::ConstTable().Const("DEBUG", FALSE);
#endif
}


//####################  外部からVMを参照する  ####################
HSQUIRRELVM &CScript::GetVM() {
	return this->sqvm;
}


//####################  スクリプトを追加  ####################
int CScript::Add(string fileName) {
	//DXアーカイブ後は使用できないので無効
	//if(!PathFileExists(fileName.c_str())) {
	//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないスクリプトです。\n>%s", fileName.c_str());
	//	return -1;
	//}

	//既に登録されていないか調べる
	bool newAdd = false;
	CIDNameData NewItem;
	NewItem.ID = this->findID(fileName);
	NewItem.Name = fileName;
	if(NewItem.ID == -1) {
		NewItem.ID = this->generateHandle();
		newAdd = true;
	}

	//再コンパイル時、既に読み込まれている ! ファイルはIDを返すだけにする
	if(this->reloading && newAdd == false && this->isOnceFile(fileName)) {
		TS_LOG("スクリプト [%d: %s] は再コンパイルされません。", NewItem.ID, NewItem.Name.c_str());
		return NewItem.ID;
	}

	//自動でコンパイルして実行する
	TS_LOG("スクリプト [%d: %s] を読み込みます...", NewItem.ID, NewItem.Name.c_str());
	this->compileSQ(NewItem);
	if(newAdd) {
		this->Scripts.Add(NewItem);
	}
	return NewItem.ID;
}


//####################  VMを破棄  ####################
void CScript::Dispose() {
	::sq_close(this->sqvm);
}


//####################  スクリプトをコンパイルしてファイルに出力  ####################
// NOTE: コンパイル済みのファイル拡張子は .cnut が通例とされているが、定数定義してしまっているので、拡張子は変えないことにする。
void CScript::CompileToFile(string srcFileName, string destFileName) {
	Sqrat::Script script;
	script.CompileFile(srcFileName.c_str());

	//拡張子を .cnut に変更する場合は以下をコメントアウトすること
	//destFileName = Game.Replace(destFileName, ".nut", ".nut");

	script.WriteCompiledFile(destFileName.c_str());
}


//####################  次のコードからステップ実行を有効にします。  ####################
void CScript::StartStepRun() {
	this->debugging = true;
	this->StepDestinationLevel = INT_MAX;
	::printf("スクリプトのステップ実行を開始します...");
}


//####################  管理オブジェクトを使える状態にする  ####################
void CScript::Setup() {
	TS_LOG("スクリプトの準備中です...");

	//組み込み関数・定数のバインド
	this->BindObjects();
	Game.SQ.Do(TS::FileNames::SQConstants, true);		//定数定義を行うスクリプトは必ず最初に呼び出す必要がある

	//オブジェクトのバインド
	Game.SQ.SQGlue();

	//C++側から発せられるイベントを処理するためのイベントハンドラーを定義したスクリプトを既定で追加する
	this->Add(TS::FileNames::SQEventHandler);
	Game.SQ.Do(TS::FileNames::SQFunctions, true);		//汎用的な関数定義を行うスクリプトを読み込んでおく

	TS_LOG("スクリプトの準備が完了しました。");
}


//####################  C++側の関数やオブジェクトをスクリプトに公開する  ####################
void CScript::BindObjects() {
	//定数：int型に合わせた論理値
	Sqrat::ConstTable().Const("FALSE", (int)FALSE);
	Sqrat::ConstTable().Const("TRUE", (int)TRUE);

	//定数：基本色
	Sqrat::ConstTable().Const("RED", (int)RED);
	Sqrat::ConstTable().Const("GREEN", (int)GREEN);
	Sqrat::ConstTable().Const("BLUE", (int)BLUE);
	Sqrat::ConstTable().Const("BLACK", (int)BLACK);
	Sqrat::ConstTable().Const("WHITE", (int)WHITE);
	Sqrat::ConstTable().Const("YELLOW", (int)YELLOW);
	Sqrat::ConstTable().Const("GREENYELLOW", (int)GREENYELLOW);
	Sqrat::ConstTable().Const("GREENBLUE", (int)GREENBLUE);
	Sqrat::ConstTable().Const("BLUEGREEN", (int)BLUEGREEN);
	Sqrat::ConstTable().Const("GRAY", (int)GRAY);
	Sqrat::ConstTable().Const("DARKGRAY", (int)DARKGRAY);
	Sqrat::ConstTable().Const("ORANGE", (int)ORANGE);
	Sqrat::ConstTable().Const("PURPLE", (int)PURPLE);
	Sqrat::ConstTable().Const("PINK", (int)PINK);
	Sqrat::ConstTable().Const("CYAN", (int)CYAN);
	Sqrat::ConstTable().Const("BROWN", (int)BROWN);
	Sqrat::ConstTable().Const("DARKBLUE", (int)DARKBLUE);

	//数学関数等
	Sqrat::ConstTable().Const("INT_MAX", (int)INT_MAX);
	Sqrat::ConstTable().Const("INT_MIN", (int)INT_MIN);
}


//####################  デバッガーを開始する  ####################
void CScript::StartDebugger() {
	if(CDebug::StartDebugConsole()) {
		this->StartStepRun();		//呼び出し先の指示によってステップ実行へ移行する
	}
}


//####################  デバッガーを終了する  ####################
void CScript::EscapeDebugger() {
	this->debugging = false;
}


//####################  ファイルから直接スクリプトを実行  ####################
void CScript::Do(string fileName, bool onceMode, bool forceRun) {
	//DXアーカイブ後は使用できないので無効
	//if(!PathFileExists(fileName.c_str())) {
	//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないスクリプトです。\n>%s", fileName.c_str());
	//	return;
	//}
	this->Add(fileName);	//追加してコンパイル＋実行
}


//####################  スクリプトを再コンパイル  ####################
//返値は実際にコンパイルが行われたかどうか
bool CScript::Recompile(int ID) {
	if(this->Scripts.Exists(ID)) {
		string fileName = this->Scripts.GetFromID(ID)->Name;
		this->reloading = true;
		this->Add(fileName);		//コンパイル＋実行
		this->reloading = false;
		return true;
	}
	return false;
}
bool CScript::Recompile(string fileName) {
	return this->Recompile(this->findID(fileName));
}


//####################  登録されたすべてのスクリプトを再コンパイル  ####################
void CScript::RecompileAll() {
	//すべてコンパイルして再実行
	this->reloading = true;
	for(auto& sq : this->Scripts.Items) {
		this->Add(sq.Name);
	}
	this->reloading = false;
}


//####################  デバッガーが起動しているかどうかを取得  ####################
bool CScript::IsDebugging() {
	return this->debugging;
}


//####################  スクリプトで定義された変数・テーブル・クラスのメンバーの値を取得  ####################
Sqrat::Object GetValue(string valueName, string slotName) {
	//テーブルを取得
	Sqrat::Object Table;
	if(slotName == "") {
		Table = Sqrat::RootTable();
	} else {
		Table = Sqrat::RootTable().GetSlot(_SC(slotName.c_str()));
	}
	if(Table.IsNull()) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "存在しないテーブル [%s] が参照されました。", slotName.c_str());
	}

	//値を取得
	Sqrat::Object value = Table.GetSlot(_SC(valueName.c_str()));
	if(value.IsNull()) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "存在しない変数 [%s] が参照されました。", slotName.c_str());
	}
	return value;
}
bool CScript::GetSQBool(string valueName, string slotName) {
	return GetValue(valueName, slotName).Cast<bool>();
}
int CScript::GetSQInteger(string valueName, string slotName) {
	return GetValue(valueName, slotName).Cast<int>();
}
string CScript::GetSQString(string valueName, string slotName) {
	return GetValue(valueName, slotName).Cast<Sqrat::string>();
}


//####################  スクリプトで定義された変数・テーブル・クラスのメンバーの値を書き換えもしくは追加  ####################
void CScript::SetSQValueByVal(bool *value, string valueName, string slotName) {
	if(slotName.empty()) {
		//グローバル変数を書き換えもしくは追加
		Sqrat::RootTable().SetValue(_SC(valueName.c_str()), *value);
	} else {
		//テーブル・クラスのメンバー変数を書き換えもしくは追加
		Sqrat::Table Table;
		Table.SetValue(_SC(valueName.c_str()), *value);
	}
}
void CScript::SetSQValueByVal(int *value, string valueName, string slotName) {
	if(slotName.empty()) {
		//グローバル変数を書き換えもしくは追加
		Sqrat::RootTable().SetValue(_SC(valueName.c_str()), *value);
	} else {
		//テーブル・クラスのメンバー変数を書き換えもしくは追加
		Sqrat::Table Table;
		Table.SetValue(_SC(valueName.c_str()), *value);
	}
}
void CScript::SetSQValueByVal(string *value, string valueName, string slotName) {
	if(slotName.empty()) {
		//グローバル変数を書き換えもしくは追加
		Sqrat::RootTable().SetValue(_SC(valueName.c_str()), *value);
	} else {
		//テーブル・クラスのメンバー変数を書き換えもしくは追加
		Sqrat::Table Table;
		Table.SetValue(_SC(valueName.c_str()), *value);
	}
}


//####################  使われていないハンドル値を取得  ####################
int CScript::generateHandle() {
	int result = DxLib::GetRand(INT_MAX);
	while(this->Scripts.Exists(result)) {
		result = DxLib::GetRand(INT_MAX);
	}
	return result;
}


//####################  ファイル名からIDを検索する  ####################
int CScript::findID(string fileName) {
	for(auto& sq : this->Scripts.Items) {
		if(sq.Name == fileName) {
			return sq.ID;
		}
	}
	return -1;
}


//####################  ファイルが一度しか実行されないスクリプトであるかどうかを調べる  ####################
bool CScript::isOnceFile(string fileName) {
	string fileNameWithoutDir = PathFindFileName(fileName.c_str());
	return (fileNameWithoutDir.size() >= 1 && fileNameWithoutDir.substr(0, 1) == "!");
}


//####################  現在のスクリプト情報をログ出力する  ####################
void CScript::PrintLog() {
	TS_LOG("スクリプト一覧:");
	for(auto& sq : this->Scripts.Items) {
		TS_LOG("\t" + to_string(sq.ID) + ": " + sq.Name);
	}
}


//####################  スクリプトをコンパイルして実行：エラーが起きた場合は修正を待つ  ####################
void CScript::compileSQ(CIDNameData& sq) {
	bool compileError = false;

#ifdef _DEBUG
	//デバッグ時: ファイルから直接コンパイルし、エラーが起きたら修正を促す
	do {
		compileError = false;
		try {
			Sqrat::Script script;
			script.CompileFile(sq.Name.c_str());
			script.Run();
		} catch(Sqrat::Exception e) {
			// NOTE: コンパイルエラーハンドラーによってエラー内容が出力されている。
			CDebug::ActivateConsole();

			bool handled = false;
			while(!handled) {
				::printf("\nエラー箇所を修正して下さい。\n\t[C]再コンパイル  [E]プログラム中断 (ログ出力なし)  [L]プログラム中断 (ログ出力あり)\n");
				switch(CDebug::PauseConsole()) {
					case 'c':
						handled = true;
						break;

					case 'e':	//プログラム中断: ログなし
						handled = true;
						Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
						break;

					case 'l':	//プログラム中断: ログあり
						handled = true;
						Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Error));
						break;
				}
			}

			CDebug::ActivateGameWindow();
			compileError = true;
		}
	} while(compileError);
#else
	//リリース時: DXアーカイブからコードを取り出してコンパイルする
	void* code;
	int hdl = DxLib::FileRead_open((TS::DirectoryNames::PrePath + sq.Name).c_str(), false);
	if (hdl == 0) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "スクリプトの取り出しに失敗しました。\n>%s", sq.Name.c_str());
		return;
	}
	int size = DxLib::FileRead_size((TS::DirectoryNames::PrePath + sq.Name).c_str());
	code = malloc(size + 1);
	DxLib::FileRead_read(code, size, hdl);
	((char*)code)[size] = '\0';			//バイナリとして読み込んでいるので、終端記号を自前で付加する

	try {
		Sqrat::Script script;
		script.CompileString((char*)code);
		script.Run();
	} catch(Sqrat::Exception e) {
		compileError = true;
	}

	DxLib::FileRead_close(hdl);
	free(code);

	//エラーが起きたらログを出力して即座に終了する
	if(compileError) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "スクリプトのコンパイルエラーが発生しました。\n>(%d Bytes) %s", size, sq.Name.c_str());
		return;
	}
#endif
}
