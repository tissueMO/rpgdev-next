//###################################################################
//		�f�o�b�O �R���\�[��
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
	CDebug();						//�C���X�^���X���ł��Ȃ��悤�ɂ���

	//�v���C�x�[�g���\�b�h
	static bool cmdHelp(CMDARGS);
	static bool cmdSystemList(CMDARGS);
	static bool cmdDB(CMDARGS);
	static bool cmdMap(CMDARGS);
	static bool cmdSQ(CMDARGS);
};
//SQ_NOBIND_END

