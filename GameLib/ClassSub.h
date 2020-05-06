//###################################################################
//		ゲームオブジェクトが実体化する雑多なクラス群
//###################################################################

//SQ_NOBIND_BEGIN
//####################  list: ループ中に必ず実行すべき関数をまとめるクラス  ####################
class CRegularFunc {
public:
	//コンストラクター
	SQ_NOBIND CRegularFunc();

	//メンバーメソッド
	//すべて実行
	SQ_NOBIND void Do() {
		for(void(*Func)(void) : this->List) {
			Func();
		}
	}

	//関数を追加
	void AddFunction(void(*func)(void)) {
		this->List.push_back(func);
	}

private:
	list<void(*)(void)> List;
};
//SQ_NOBIND_END


//####################  list: 疑似マルチスレッドシステム（Funcとは違って必ず終了条件がある）  ####################
class CMultiThread {
public:
	//####################  スレッド単一データ  ####################
	class CThreadInfo : public CNullCheck {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND string Name;								//このスレッドの名称
		SQ_PROPGET string getName() { return this->Name; }

		SQ_NOBIND string SQFuncName;						//スクリプト関数の名称
		SQ_PROPGET string getSQFuncName() { return this->SQFuncName; }

		SQ_NOBIND bool Suspended;							//一時停止中であるか
		SQ_PROPGET bool getSuspended() { return this->Suspended; } SQ_PROPSET void setSuspended(bool value) { this->Suspended = value; }

		SQ_NOBIND bool Protected;							//いかなる状況でも削除できないようにする（つまりRegularFuncと同じ性質になる）
		SQ_PROPGET bool getProtected() { return this->Protected; } SQ_PROPSET void setProtected(bool value) { this->Protected = value; }

		SQ_NOBIND int Handle;								//一意の識別番号
		SQ_PROPGET int getHandle() { return this->Handle; }

		SQ_NOBIND bool DoneAhead;							//先行的に実行されたか
		SQ_NOBIND bool Deleting;							//削除予約されたか
		SQ_NOBIND bool IsSQFunc;							//スクリプト関数であるかどうか
		SQ_NOBIND bool(*Function)(CThreadInfo& Data);		//関連付ける関数（これが実際の処理内容）
		SQ_NOBIND int SceneLimit;							//特定シーンでのみ有効にする


		//メンバーメソッド
		SQ_NOBIND string GetValueData();

		//いずれも既に存在する変数名を指定すると値が変更される
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
		//メンバー変数
		map<string, int> Vint;					//作業変数群（追加はできるが外部から操作・取得はできない）
		map<string, double> Vdec;
		map<string, string> Vstr;
	};

	//SQ_BEGINMEMBER
	//メンバーメソッド
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
	//メンバー変数
	list<CThreadInfo> List;
	vector<CThreadInfo*> runningStack;

	//プライベートメソッド
	bool doThread(CThreadInfo& trd);
	void clearDeletingThread();
	void clearInvalidThreadInStack();
};


//####################  フレームレートを管理するクラス  ####################
class CFPS {
public:
	//SQ_BEGINMEMBER
	//コンストラクター
	SQ_NOBIND CFPS();

	//メンバー変数
	SQ_PROPGET int getSkipFrameCount() { return this->fixedSkipFrameCount; } SQ_PROPSET void setSkipFrameCount(int val);
	SQ_NOBIND int getTempSkipFrameCount() { return this->tempSkipFrameCount; }

	//メンバーメソッド
	SQ_INSTANCEFUNC void Update();

private:
	//メンバー変数
	int frame_time;					//１フレームの時間（ミリ秒）
	int wait_time;					//待ち時間
	int last_time;					//最後に取得した時間
	int fixedSkipFrameCount = 0;	//フレーム送りをする回数
	int tempSkipFrameCount = 0;		//フレーム遅延のためにフレーム送りをすべき回数
	int nowSkipFrameCount = 0;		//現在何回フレームを送ったか

	//プライベートメソッド
	void Wait();
};
