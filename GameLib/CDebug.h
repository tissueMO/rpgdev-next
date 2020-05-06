//###################################################################
//		デバッグ コンソール
//###################################################################
#define CMDARGS		string& buf, vector<string>& spl

//SQ_NOBIND_BEGIN
static class CDebug {
public:
	static bool StartDebugConsole();
	static void ActivateConsole();
	static void ActivateGameWindow();
	static void ClsConsole();
	static int PauseConsole();

private:
	CDebug();						//インスタンス化できないようにする

	//プライベートメソッド
	static bool cmdHelp(CMDARGS);
	static bool cmdSystemList(CMDARGS);
	static bool cmdDB(CMDARGS);
	static bool cmdMap(CMDARGS);
	static bool cmdSQ(CMDARGS);
};
//SQ_NOBIND_END

