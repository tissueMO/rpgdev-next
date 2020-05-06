//###################################################################
//		疑似マルチスレッドクラス定義
//###################################################################
#include "Header.h"

//注意：スレッドの保護された状態とは、削除されない状態であり、停止できないわけではない。ただし自律終了は行われる。
//備考：C++スレッド関数の引数にはスレッドのオブジェクトが、SQスレッド関数の引数には一意の識別番号が渡される。


//####################  スレッドの個別変数のポインタを取得  ####################
int* CMultiThread::CThreadInfo::GetInteger(string name) {
	if(Vint.find(name) != Vint.end()) {
		return &Vint[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-Integer要素が参照されました。", name.c_str());
	return nullptr;
}
double* CMultiThread::CThreadInfo::GetDecimal(string name) {
	if(Vdec.find(name) != Vdec.end()) {
		return &Vdec[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-Decimal要素が参照されました。", name.c_str());
	return nullptr;
}
string* CMultiThread::CThreadInfo::GetString(string name) {
	if(Vstr.find(name) != Vstr.end()) {
		return &Vstr[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-String要素が参照されました。", name.c_str());
	return nullptr;
}


//####################  スレッドの個別変数の値を取得  ####################
int CMultiThread::CThreadInfo::GetValueInteger(string name) {
	if(Vint.find(name) != Vint.end()) {
		return Vint[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-Integer要素が取得されました。", name.c_str());
	return 0;
}
double CMultiThread::CThreadInfo::GetValueDecimal(string name) {
	if(Vdec.find(name) != Vdec.end()) {
		return Vdec[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-Decimal要素が取得されました。", name.c_str());
	return 0.0;
}
string CMultiThread::CThreadInfo::GetValueString(string name) {
	if(Vstr.find(name) != Vstr.end()) {
		return Vstr[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-String要素が取得されました。", name.c_str());
	return "";
}


//####################  スレッドの個別変数の値を設定  ####################
void CMultiThread::CThreadInfo::SetValueInteger(string name, int Value) {
	if(Vint.find(name) != Vint.end()) {
		Vint[name] = Value;
		return;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-Integer要素が設定されました。", name.c_str());
}
void CMultiThread::CThreadInfo::SetValueDecimal(string name, double Value) {
	if(Vdec.find(name) != Vdec.end()) {
		Vdec[name] = Value;
		return;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-Decimal要素が設定されました。", name.c_str());
}
void CMultiThread::CThreadInfo::SetValueString(string name, string Value) {
	if(Vstr.find(name) != Vstr.end()) {
		Vstr[name] = Value;
		return;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない名前 [%s] でMT-String要素が設定されました。", name.c_str());
}


//####################  個別変数をログ出力用のテキストにして返す  ####################
string CMultiThread::CThreadInfo::GetValueData() {
	string buf;

	//整数
	if(this->Vint.size() > 0) {
		buf += "\n\t\t[Vint] ";
		for(auto& val : this->Vint) {
			buf += Game.Format("%s=%d  ", val.first.c_str(), val.second);
		}
	}

	//実数
	if(this->Vdec.size() > 0) {
		buf += "\n\t\t[Vdec] ";
		for(auto& val : this->Vdec) {
			buf += Game.Format("%s=%f  ", val.first.c_str(), val.second);
		}
	}

	//文字列
	if(this->Vstr.size() > 0) {
		buf += "\n\t\t[Vstr] ";
		for(auto& val : this->Vstr) {
			buf += Game.Format("%s=\"%s\"  ", val.first.c_str(), val.second.c_str());
		}
	}

	return buf;
}


//####################  破棄処理  ####################
CMultiThread::~CMultiThread() {
	list<CThreadInfo>().swap(this->List);
	vector<CThreadInfo*>().swap(this->runningStack);
}


//####################  スレッドを追加：C++関数  ####################
//名前を空白にした場合、今後参照できない唯一のスレッドとしてセットされる
//EnableSceneID を -1 にすると、常に有効なスレッドとなる
CMultiThread::CThreadInfo& CMultiThread::AddThread(string name, bool(*func)(CMultiThread::CThreadInfo& Data), TS::System::Scene enableSceneID, bool isProtected) {
	if(isProtected) {
		//isProtected を true にすると、既に同一の関数が登録されている場合は重複して登録しない
		auto it = std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
			return (item.Function == func && item.Name == name);
		});

		//存在している場合は登録せずに既にあるスレッドで返す
		if(it != this->List.end()) {
			return *it;
		}
	}

	CThreadInfo NewItem;
	NewItem.Handle = Game.NextRand(0, INT_MAX);
	NewItem.SceneLimit = static_cast<int>(enableSceneID);
	NewItem.Protected = isProtected;
	NewItem.Suspended = false;
	NewItem.DoneAhead = false;
	NewItem.Deleting = false;

	//C++関数の設定
	NewItem.Function = func;
	NewItem.IsSQFunc = false;

	if(name.empty()) {
		//無名の場合は識別番号がそのままスレッド名になる
		name = Game.Format("%d", NewItem.Handle);
	}
	NewItem.Name = name;

	this->List.push_back(NewItem);
	return *--List.end();		//追加したスレッドへの参照で返す
}


//####################  スレッドを追加：スクリプト関数  ####################
//EnableSceneID を -1 にすると、常に有効なスレッドとなる
CMultiThread::CThreadInfo* CMultiThread::AddThread(string name, string funcName, int enableSceneID, bool isProtected) {
	if(isProtected) {
		//isProtected を true にすると、既に同一の関数が登録されている場合は重複して登録しない
		auto it = std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
			return (item.IsSQFunc && item.SQFuncName == funcName);
		});

		//存在している場合は登録せずに既にあるスレッドで返す
		if(it != this->List.end()) {
			return &*it;
		}
	}

	CThreadInfo NewItem;
	NewItem.Handle = Game.NextRand(0, INT_MAX);
	NewItem.SceneLimit = enableSceneID;
	NewItem.Protected = isProtected;
	NewItem.Suspended = false;
	NewItem.DoneAhead = false;
	NewItem.Deleting = false;

	//スクリプト関数の設定
	NewItem.Function = nullptr;
	NewItem.IsSQFunc = true;
	NewItem.SQFuncName = funcName;

	if(name.empty()) {
		//無名の場合は識別番号がそのままスレッド名になる
		name = Game.Format("%d", NewItem.Handle);
	}
	NewItem.Name = name;

	this->List.push_back(NewItem);
	return &*--List.end();		//追加したスレッドへのポインタで返す
}


//####################  スレッドの総数を取得  ####################
int CMultiThread::GetThreadCount() {
	return std::count_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (!item.Deleting);
	});
}


//####################  指定したスレッドの個数を取得  ####################
int CMultiThread::CountThread(string name) {
	return std::count_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (item.Name == name && !item.Deleting);
	});
}


//####################  ハンドルに対応するスレッドを取得する  ####################
CMultiThread::CThreadInfo* CMultiThread::GetThread(int handle) {
	auto it = std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (item.Handle == handle);
	});
	return (it != this->List.end()) ? &*it : nullptr;
}


//####################  スレッドをすべて実行  ####################
void CMultiThread::Do() {
	bool result;
	for(auto& it : this->List) {
		if(it.Deleting) {
			continue;						//削除予約されている場合はスキップ
		} else if(std::find(TS_CONTAINERALL(this->runningStack), &it) != this->runningStack.end()) {
			continue;						//現在実行中のスレッドである場合はスキップ
		}

		if(it.DoneAhead == false && it.Suspended == false && (it.SceneLimit == static_cast<int>(TS::System::Scene::All) || Game.CheckCurrentSceneID(it.SceneLimit))) {
			this->runningStack.push_back(&it);		//実行中のスレッドとしてスタックに積む
			result = this->doThread(it);			//スレッドを実行
			this->runningStack.pop_back();			//スタックに積んだものを下ろす
			if(result) {
				it.Deleting = true;			//正常終了した場合はスレッドの削除予約を行う
			}
		}
		it.DoneAhead = false;
	}
	this->clearInvalidThreadInStack();
	this->clearDeletingThread();
}


//####################  指定したスレッドを先行的に実行  ####################
//名前を空白にした場合、すべてのスレッドを対象とする
void CMultiThread::DoAhead(string name) {
	bool result;
	for(auto& it : this->List) {
		if(name.empty() || it.Name == name) {
			if(std::find(TS_CONTAINERALL(this->runningStack), &it) != this->runningStack.end()) {
				continue;					//現在実行中のスレッドである場合はスキップ
			}

			if(it.DoneAhead == false && it.Suspended == false && (it.SceneLimit == static_cast<int>(TS::System::Scene::All) || Game.CheckCurrentSceneID(it.SceneLimit))) {
				this->runningStack.push_back(&it);		//実行中のスレッドとしてスタックに積む
				result = this->doThread(it);			//スレッドを実行
				this->runningStack.pop_back();			//スタックに積んだものを下ろす
				if(result) {
					it.Deleting = true;			//正常終了した場合はスレッドの削除予約を行う
				}
			}
			it.DoneAhead = true;
		}
	}
	this->clearInvalidThreadInStack();
	this->clearDeletingThread();
}


//####################  スレッドをすべて削除  ####################
//注意：保護されたスレッド、LeaveCurrentSceneで指定したシーンでのみ有効な関数を除く
void CMultiThread::Clear(bool LeaveCurrentScene) {
	for(auto& it : this->List) {
		if(!it.Protected) {
			if(!LeaveCurrentScene || !Game.CheckCurrentSceneID(it.SceneLimit)) {
				it.Deleting = true;		//次回の全実行のときに削除されるように予約する
				it.Suspended = true;
			}
		}
	}
}


//####################  指定したスレッドを削除する  ####################
//注意：保護されたスレッドを除く
void CMultiThread::DeleteThread(string name) {
	this->List.erase(std::remove_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (item.Name == name && !item.Protected);
	}), this->List.end());
}


//####################  指定したスレッドの停止状態を設定する  ####################
//名前を空白にした場合、すべてのスレッドを対象とする
//検索されたスレッドが存在しない場合は何も起こらない
void CMultiThread::SetSuspend(string name, bool IsSuspended) {
	for(auto& it : this->List) {
		if(it.Name == name || name.empty()) {
			it.Suspended = IsSuspended;
		}
	}
}


//####################  指定したスレッドの保護状態を設定する  ####################
//名前を空白にした場合、すべてのスレッドを対象とする
//検索されたスレッドが存在しない場合は何も起こらない
void CMultiThread::SetProtect(string name, bool IsProtected) {
	for(auto& it : this->List) {
		if(it.Name == name || name.empty()) {
			it.Protected = IsProtected;
		}
	}
}


//####################  指定したスレッドの停止状態を取得する  ####################
//新たに重複したスレッドが追加されて状態が統一されていない場合が考えられるが、少なくとも最初に見つかるものは設定済みとみなせる
bool CMultiThread::GetSuspend(string name) {
	for(auto& it : this->List) {
		if(it.Name == name) {
			return it.Suspended;
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないMT関数 [%s] が参照されました。", name.c_str());
	return false;
}


//####################  指定したスレッドの保護状態を取得する  ####################
//新たに重複したスレッドが追加されて状態が統一されていない場合が考えられるが、少なくとも最初に見つかるものは設定済みとみなせる
bool CMultiThread::GetProtect(string name) {
	for(auto& it : this->List) {
		if(it.Name == name) {
			return it.Protected;
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないMT関数 [%s] が参照されました。", name.c_str());
	return false;
}


//####################  指定したスレッドの終了を待つ  ####################
void CMultiThread::WaitForThread(string name, double MaxSec) {
	int StartTime = DxLib::GetNowCount();
	while(CountThread(name) > 0 || ::GetNowCount() - StartTime > MaxSec * 1000) {
		Game.DoSceneDraw();
		TS_EVERYLOOP;
	}
}


//####################  現在のスレッド情報をログ出力する  ####################
void CMultiThread::PrintLog() {
	string buf;
	TS_LOG("ＭＴ関数一覧:");
	for(auto& thread : this->List) {
		buf = "\t" + thread.Name + ":\n\t\t";
		buf += "  ";
		buf += (thread.IsSQFunc ? "SQ" : "C++");
		buf += "  ";
		buf += (thread.Suspended ? "[停止] " : "");
		buf += "  ";
		buf += (thread.Protected ? "[保護]" : "");
		buf += "  ";
		buf += (thread.Deleting ? "[削除中]" : "");
		buf += thread.GetValueData();
		TS_LOG(buf);
	}
}


//####################  指定されたスレッド関数を実際に呼び出す  ####################
bool CMultiThread::doThread(CThreadInfo& trd) {
	if(!trd.IsSQFunc) {
		//C++関数を呼び出す: スレッドのオブジェクトを引数に渡す
		return trd.Function(trd);
	} else {
		//SQ関数を呼び出す: スレッドのハンドルを引数に渡す
		Sqrat::Script tempSQ;
		auto func = Sqrat::Function(Sqrat::RootTable(), _SC(trd.SQFuncName.c_str()));
		if(!func.IsNull()) {
			//スクリプト関数が存在する場合のみ
			return func.Evaluate<bool>(trd.Handle);
		} else {
			//存在しない場合は直ちにスレッドから削除するように true で返す
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "スクリプト関数を持つスレッドの実行に失敗しました。\n>%s", trd.SQFuncName.c_str());
			return true;
		}
	}
}


//####################  削除予約されているスレッドを一括削除する  ####################
void CMultiThread::clearDeletingThread() {
	//実行中のスレッドが存在しない場合のみ処理する
	if(this->runningStack.empty()) {
		this->List.erase(std::remove_if(TS_CONTAINERALL(this->List), [](CThreadInfo& item) {
			return item.Deleting;
		}), this->List.end());
	}
}


//####################  実行中のスレッドを格納しているスタックに存在しないスレッドがある場合は削除する  ####################
void CMultiThread::clearInvalidThreadInStack() {
	this->runningStack.erase(std::remove_if(TS_CONTAINERALL(this->runningStack), [&](CThreadInfo* item) {
		//同一のスレッドがスレッドリストに登録されているかどうかを調べて、存在しない場合は削除
		return std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& thread) {
			return item == &thread;
		}) == this->List.end();
	}), this->runningStack.end());
}
