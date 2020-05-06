//###################################################################
//		�X�N���v�g�Ǘ��N���X
//###################################################################
//SQ_NOBIND_BEGIN
extern void SQErrorMessage(string funcName, string msg);
extern void printSQCallStackAll(HSQUIRRELVM& v);

class CScript {
public:
	//�����o�[�ϐ�
	bool KillDebuggerFlag = false;
	int StepDestinationLevel = -1;		//�X�e�b�v���s�̑ΏۂƂ���Ăяo���K�w�̐[��

	//�����o�[���\�b�h
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


	//�����̌^�������������ŃX�N���v�g���ɂ���֐������s�i���݂��Ȃ��ꍇ�͎��s����Ȃ��j
	template<typename Tr, typename ... Tn>		//�߂�l�̌^�A�����̌^���X�g
	Tr DoFunc(string funcName, Tn ... argN) {
		try {
			auto func = Sqrat::Function(Sqrat::RootTable(), _SC(funcName.c_str()));
			if(func.IsNull()) {
				return FALSE;		//�֐������݂��Ȃ��ꍇ�͏�� FALSE �ŕԂ�
			}
			return func.Evaluate<Tr, Tn...>(argN...);
		} catch(Sqrat::Exception e) {
			::SQErrorMessage(funcName, e.Message());
		} catch(...) {
			//���̑��̗�O�͂��ׂđz��O�G���[�Ƃ��ă��O��f������Ńv���O�����𗎂Ƃ�
			//�X�N���v�g�̃R�[���X�^�b�N��f���o��
			::printSQCallStackAll(this->sqvm);
#ifdef _DEBUG
			Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "�X�N���v�g�̎��s���G���[���������܂����B\r\n�f�o�b�O�R���\�[����ɏo�͂���Ă�����e��������Logs�t�H���_�[�ɏo�͂���郍�O�t�@�C�������Q�Ƃ̏�A�X�N���v�g���C�����ĉ������B");
#else
			Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "�X�N���v�g�̎��s���G���[���������܂����B");
#endif
		}
		return FALSE;
	}


private:
	//�����o�[�ϐ�
	Cstdvector<CIDNameData> Scripts;
	HSQUIRRELVM sqvm;
	bool debugging = false;
	bool reloading = false;

	//�v���C�x�[�g���\�b�h
	void BindObjects();
	void SQGlue();
	void compileSQ(CIDNameData& sq);
	int generateHandle();
	int findID(string fileName);
	bool isOnceFile(string fileName);
};
//SQ_NOBIND_END
