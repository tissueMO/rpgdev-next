//###################################################################
//		�Q�[���I�u�W�F�N�g�����̉�����G���ȃN���X�Q
//###################################################################

//SQ_NOBIND_BEGIN
//####################  list: ���[�v���ɕK�����s���ׂ��֐����܂Ƃ߂�N���X  ####################
class CRegularFunc {
public:
	//�R���X�g���N�^�[
	SQ_NOBIND CRegularFunc();

	//�����o�[���\�b�h
	//���ׂĎ��s
	SQ_NOBIND void Do() {
		for(void(*Func)(void) : this->List) {
			Func();
		}
	}

	//�֐���ǉ�
	void AddFunction(void(*func)(void)) {
		this->List.push_back(func);
	}

private:
	list<void(*)(void)> List;
};
//SQ_NOBIND_END


//####################  list: �^���}���`�X���b�h�V�X�e���iFunc�Ƃ͈���ĕK���I������������j  ####################
class CMultiThread {
public:
	//####################  �X���b�h�P��f�[�^  ####################
	class CThreadInfo : public CNullCheck {
	public:
		//SQ_BEGINMEMBER
		//�����o�[�ϐ�
		SQ_NOBIND string Name;								//���̃X���b�h�̖���
		SQ_PROPGET string getName() { return this->Name; }

		SQ_NOBIND string SQFuncName;						//�X�N���v�g�֐��̖���
		SQ_PROPGET string getSQFuncName() { return this->SQFuncName; }

		SQ_NOBIND bool Suspended;							//�ꎞ��~���ł��邩
		SQ_PROPGET bool getSuspended() { return this->Suspended; } SQ_PROPSET void setSuspended(bool value) { this->Suspended = value; }

		SQ_NOBIND bool Protected;							//�����Ȃ�󋵂ł��폜�ł��Ȃ��悤�ɂ���i�܂�RegularFunc�Ɠ��������ɂȂ�j
		SQ_PROPGET bool getProtected() { return this->Protected; } SQ_PROPSET void setProtected(bool value) { this->Protected = value; }

		SQ_NOBIND int Handle;								//��ӂ̎��ʔԍ�
		SQ_PROPGET int getHandle() { return this->Handle; }

		SQ_NOBIND bool DoneAhead;							//��s�I�Ɏ��s���ꂽ��
		SQ_NOBIND bool Deleting;							//�폜�\�񂳂ꂽ��
		SQ_NOBIND bool IsSQFunc;							//�X�N���v�g�֐��ł��邩�ǂ���
		SQ_NOBIND bool(*Function)(CThreadInfo& Data);		//�֘A�t����֐��i���ꂪ���ۂ̏������e�j
		SQ_NOBIND int SceneLimit;							//����V�[���ł̂ݗL���ɂ���


		//�����o�[���\�b�h
		SQ_NOBIND string GetValueData();

		//����������ɑ��݂���ϐ������w�肷��ƒl���ύX�����
		SQ_NOBIND CThreadInfo& AddInteger(string name, int Value) {
			Vint[name] = Value;
			return *this;
		}
		SQ_INSTANCEFUNC CThreadInfo* AddValueInteger(string name, int Value) {
			Vint[name] = Value;
			return this;
		}
		SQ_NOBIND CThreadInfo& AddDecimal(string name, double Value) {
			Vdec[name] = Value;
			return *this;
		}
		SQ_INSTANCEFUNC CThreadInfo* AddValueDecimal(string name, double Value) {
			Vdec[name] = Value;
			return this;
		}
		SQ_NOBIND CThreadInfo& AddString(string name, string Value) {
			Vstr[name] = Value;
			return *this;
		}
		SQ_INSTANCEFUNC CThreadInfo* AddValueString(string name, string Value) {
			Vstr[name] = Value;
			return this;
		}
		
		SQ_INSTANCEFUNC void SetValueInteger(string name, int Value);
		SQ_INSTANCEFUNC void SetValueDecimal(string name, double Value);
		SQ_INSTANCEFUNC void SetValueString(string name, string Value);
	
		SQ_INSTANCEFUNC int GetValueInteger(string name);
		SQ_INSTANCEFUNC double GetValueDecimal(string name);
		SQ_INSTANCEFUNC string GetValueString(string name);

		SQ_NOBIND int* GetInteger(string name);
		SQ_NOBIND double* GetDecimal(string name);
		SQ_NOBIND string* GetString(string name);

	private:
		//�����o�[�ϐ�
		map<string, int> Vint;					//��ƕϐ��Q�i�ǉ��͂ł��邪�O�����瑀��E�擾�͂ł��Ȃ��j
		map<string, double> Vdec;
		map<string, string> Vstr;
	};

	//SQ_BEGINMEMBER
	//�����o�[���\�b�h
	SQ_NOBIND ~CMultiThread();
	SQ_NOBIND CThreadInfo& AddThread(string name, bool(*func)(CThreadInfo& Data), TS::System::Scene enableSceneID, bool isProtected = false);
	SQ_INSTANCEFUNC CMultiThread::CThreadInfo* AddThread(string name, string funcName, int enableSceneID, bool isProtected);
	SQ_NOBIND void Do();
	SQ_NOBIND void DoAhead(string name);
	SQ_NOBIND void PrintLog();
	SQ_INSTANCEFUNC int GetThreadCount();
	SQ_INSTANCEFUNC int CountThread(string name);
	SQ_INSTANCEFUNC CThreadInfo* GetThread(int handle);
	SQ_INSTANCEFUNC void Clear(bool LeaveCurrentScene);
	SQ_INSTANCEFUNC void DeleteThread(string name);
	SQ_INSTANCEFUNC void SetSuspend(string name, bool IsSuspended);
	SQ_INSTANCEFUNC void SetProtect(string name, bool IsProtected);
	SQ_INSTANCEFUNC bool GetSuspend(string name);
	SQ_INSTANCEFUNC bool GetProtect(string name);
	SQ_INSTANCEFUNC void WaitForThread(string name, double MaxSec);

private:
	//�����o�[�ϐ�
	list<CThreadInfo> List;
	vector<CThreadInfo*> runningStack;

	//�v���C�x�[�g���\�b�h
	bool doThread(CThreadInfo& trd);
	void clearDeletingThread();
	void clearInvalidThreadInStack();
};


//####################  �t���[�����[�g���Ǘ�����N���X  ####################
class CFPS {
public:
	//SQ_BEGINMEMBER
	//�R���X�g���N�^�[
	SQ_NOBIND CFPS();

	//�����o�[�ϐ�
	SQ_PROPGET int getSkipFrameCount() { return this->fixedSkipFrameCount; } SQ_PROPSET void setSkipFrameCount(int val);
	SQ_NOBIND int getTempSkipFrameCount() { return this->tempSkipFrameCount; }

	//�����o�[���\�b�h
	SQ_INSTANCEFUNC void Update();

private:
	//�����o�[�ϐ�
	int frame_time;					//�P�t���[���̎��ԁi�~���b�j
	int wait_time;					//�҂�����
	int last_time;					//�Ō�Ɏ擾��������
	int fixedSkipFrameCount = 0;	//�t���[������������
	int tempSkipFrameCount = 0;		//�t���[���x���̂��߂Ƀt���[����������ׂ���
	int nowSkipFrameCount = 0;		//���݉���t���[���𑗂�����

	//�v���C�x�[�g���\�b�h
	void Wait();
};
