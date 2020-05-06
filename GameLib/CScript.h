//###################################################################
//		スクリプト管理クラス
//###################################################################
//SQ_NOBIND_BEGIN
extern void SQErrorMessage(string funcName, string msg);
extern void printSQCallStackAll(HSQUIRRELVM& v);

class CScript {
public:
	//メンバー変数
	bool KillDebuggerFlag = false;
	int StepDestinationLevel = -1;		//ステップ実行の対象とする呼び出し階層の深さ

	//メンバーメソッド
	CScript();
	HSQUIRRELVM &GetVM();
	int Add(string fileName);
	int GetCount() { return this->Scripts.GetCount(); }
	void Dispose();
	void Do(string fileName, bool onceMode, bool forceRun = false);
	bool Recompile(int ID);
	bool Recompile(string fileName);
	void RecompileAll();
	void CompileToFile(string srcFileName, string destFileName);
	void StartStepRun();
	void StartDebugger();
	void EscapeDebugger();
	void Setup();
	void CollectGarbage() { ::sq_collectgarbage(this->sqvm); }
	void PrintLog();
	bool IsDebugging();

	bool GetSQBool(string valueName, string slotName = "");
	int GetSQInteger(string valueName, string slotName = "");
	string GetSQString(string valueName, string slotName = "");
	void SetSQValueByVal(bool* value, string valueName, string slotName = "");
	void SetSQValueByVal(int* value, string valueName, string slotName = "");
	void SetSQValueByVal(string* value, string valueName, string slotName = "");


	//複数の型を混ぜた引数でスクリプト内にある関数を実行（存在しない場合は実行されない）
	template<typename Tr, typename ... Tn>		//戻り値の型、引数の型リスト
	Tr DoFunc(string funcName, Tn ... argN) {
		try {
			auto func = Sqrat::Function(Sqrat::RootTable(), _SC(funcName.c_str()));
			if(func.IsNull()) {
				return FALSE;		//関数が存在しない場合は常に FALSE で返す
			}
			return func.Evaluate<Tr, Tn...>(argN...);
		} catch(Sqrat::Exception e) {
			::SQErrorMessage(funcName, e.Message());
		} catch(...) {
			//その他の例外はすべて想定外エラーとしてログを吐いた上でプログラムを落とす
			//スクリプトのコールスタックを吐き出す
			::printSQCallStackAll(this->sqvm);
#ifdef _DEBUG
			Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "スクリプトの実行時エラーが発生しました。\r\nデバッグコンソール上に出力されている内容もしくはLogsフォルダーに出力されるログファイルをご参照の上、スクリプトを修正して下さい。");
#else
			Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "スクリプトの実行時エラーが発生しました。");
#endif
		}
		return FALSE;
	}


private:
	//メンバー変数
	Cstdvector<CIDNameData> Scripts;
	HSQUIRRELVM sqvm;
	bool debugging = false;
	bool reloading = false;

	//プライベートメソッド
	void BindObjects();
	void SQGlue();
	void compileSQ(CIDNameData& sq);
	int generateHandle();
	int findID(string fileName);
	bool isOnceFile(string fileName);
};
//SQ_NOBIND_END
