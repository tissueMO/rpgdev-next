//###################################################################
//		�^���}���`�X���b�h�N���X��`
//###################################################################
#include "Header.h"

//���ӁF�X���b�h�̕ی삳�ꂽ��ԂƂ́A�폜����Ȃ���Ԃł���A��~�ł��Ȃ��킯�ł͂Ȃ��B�����������I���͍s����B
//���l�FC++�X���b�h�֐��̈����ɂ̓X���b�h�̃I�u�W�F�N�g���ASQ�X���b�h�֐��̈����ɂ͈�ӂ̎��ʔԍ����n�����B


//####################  �X���b�h�̌ʕϐ��̃|�C���^���擾  ####################
int* CMultiThread::CThreadInfo::GetInteger(string name) {
	if(Vint.find(name) != Vint.end()) {
		return &Vint[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-Integer�v�f���Q�Ƃ���܂����B", name.c_str());
	return nullptr;
}
double* CMultiThread::CThreadInfo::GetDecimal(string name) {
	if(Vdec.find(name) != Vdec.end()) {
		return &Vdec[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-Decimal�v�f���Q�Ƃ���܂����B", name.c_str());
	return nullptr;
}
string* CMultiThread::CThreadInfo::GetString(string name) {
	if(Vstr.find(name) != Vstr.end()) {
		return &Vstr[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-String�v�f���Q�Ƃ���܂����B", name.c_str());
	return nullptr;
}


//####################  �X���b�h�̌ʕϐ��̒l���擾  ####################
int CMultiThread::CThreadInfo::GetValueInteger(string name) {
	if(Vint.find(name) != Vint.end()) {
		return Vint[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-Integer�v�f���擾����܂����B", name.c_str());
	return 0;
}
double CMultiThread::CThreadInfo::GetValueDecimal(string name) {
	if(Vdec.find(name) != Vdec.end()) {
		return Vdec[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-Decimal�v�f���擾����܂����B", name.c_str());
	return 0.0;
}
string CMultiThread::CThreadInfo::GetValueString(string name) {
	if(Vstr.find(name) != Vstr.end()) {
		return Vstr[name];
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-String�v�f���擾����܂����B", name.c_str());
	return "";
}


//####################  �X���b�h�̌ʕϐ��̒l��ݒ�  ####################
void CMultiThread::CThreadInfo::SetValueInteger(string name, int Value) {
	if(Vint.find(name) != Vint.end()) {
		Vint[name] = Value;
		return;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-Integer�v�f���ݒ肳��܂����B", name.c_str());
}
void CMultiThread::CThreadInfo::SetValueDecimal(string name, double Value) {
	if(Vdec.find(name) != Vdec.end()) {
		Vdec[name] = Value;
		return;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-Decimal�v�f���ݒ肳��܂����B", name.c_str());
}
void CMultiThread::CThreadInfo::SetValueString(string name, string Value) {
	if(Vstr.find(name) != Vstr.end()) {
		Vstr[name] = Value;
		return;
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ����O [%s] ��MT-String�v�f���ݒ肳��܂����B", name.c_str());
}


//####################  �ʕϐ������O�o�͗p�̃e�L�X�g�ɂ��ĕԂ�  ####################
string CMultiThread::CThreadInfo::GetValueData() {
	string buf;

	//����
	if(this->Vint.size() > 0) {
		buf += "\n\t\t[Vint] ";
		for(auto& val : this->Vint) {
			buf += Game.Format("%s=%d  ", val.first.c_str(), val.second);
		}
	}

	//����
	if(this->Vdec.size() > 0) {
		buf += "\n\t\t[Vdec] ";
		for(auto& val : this->Vdec) {
			buf += Game.Format("%s=%f  ", val.first.c_str(), val.second);
		}
	}

	//������
	if(this->Vstr.size() > 0) {
		buf += "\n\t\t[Vstr] ";
		for(auto& val : this->Vstr) {
			buf += Game.Format("%s=\"%s\"  ", val.first.c_str(), val.second.c_str());
		}
	}

	return buf;
}


//####################  �j������  ####################
CMultiThread::~CMultiThread() {
	list<CThreadInfo>().swap(this->List);
	vector<CThreadInfo*>().swap(this->runningStack);
}


//####################  �X���b�h��ǉ��FC++�֐�  ####################
//���O���󔒂ɂ����ꍇ�A����Q�Ƃł��Ȃ��B��̃X���b�h�Ƃ��ăZ�b�g�����
//EnableSceneID �� -1 �ɂ���ƁA��ɗL���ȃX���b�h�ƂȂ�
CMultiThread::CThreadInfo& CMultiThread::AddThread(string name, bool(*func)(CMultiThread::CThreadInfo& Data), TS::System::Scene enableSceneID, bool isProtected) {
	if(isProtected) {
		//isProtected �� true �ɂ���ƁA���ɓ���̊֐����o�^����Ă���ꍇ�͏d�����ēo�^���Ȃ�
		auto it = std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
			return (item.Function == func && item.Name == name);
		});

		//���݂��Ă���ꍇ�͓o�^�����Ɋ��ɂ���X���b�h�ŕԂ�
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

	//C++�֐��̐ݒ�
	NewItem.Function = func;
	NewItem.IsSQFunc = false;

	if(name.empty()) {
		//�����̏ꍇ�͎��ʔԍ������̂܂܃X���b�h���ɂȂ�
		name = Game.Format("%d", NewItem.Handle);
	}
	NewItem.Name = name;

	this->List.push_back(NewItem);
	return *--List.end();		//�ǉ������X���b�h�ւ̎Q�ƂŕԂ�
}


//####################  �X���b�h��ǉ��F�X�N���v�g�֐�  ####################
//EnableSceneID �� -1 �ɂ���ƁA��ɗL���ȃX���b�h�ƂȂ�
CMultiThread::CThreadInfo* CMultiThread::AddThread(string name, string funcName, int enableSceneID, bool isProtected) {
	if(isProtected) {
		//isProtected �� true �ɂ���ƁA���ɓ���̊֐����o�^����Ă���ꍇ�͏d�����ēo�^���Ȃ�
		auto it = std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
			return (item.IsSQFunc && item.SQFuncName == funcName);
		});

		//���݂��Ă���ꍇ�͓o�^�����Ɋ��ɂ���X���b�h�ŕԂ�
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

	//�X�N���v�g�֐��̐ݒ�
	NewItem.Function = nullptr;
	NewItem.IsSQFunc = true;
	NewItem.SQFuncName = funcName;

	if(name.empty()) {
		//�����̏ꍇ�͎��ʔԍ������̂܂܃X���b�h���ɂȂ�
		name = Game.Format("%d", NewItem.Handle);
	}
	NewItem.Name = name;

	this->List.push_back(NewItem);
	return &*--List.end();		//�ǉ������X���b�h�ւ̃|�C���^�ŕԂ�
}


//####################  �X���b�h�̑������擾  ####################
int CMultiThread::GetThreadCount() {
	return std::count_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (!item.Deleting);
	});
}


//####################  �w�肵���X���b�h�̌����擾  ####################
int CMultiThread::CountThread(string name) {
	return std::count_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (item.Name == name && !item.Deleting);
	});
}


//####################  �n���h���ɑΉ�����X���b�h���擾����  ####################
CMultiThread::CThreadInfo* CMultiThread::GetThread(int handle) {
	auto it = std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (item.Handle == handle);
	});
	return (it != this->List.end()) ? &*it : nullptr;
}


//####################  �X���b�h�����ׂĎ��s  ####################
void CMultiThread::Do() {
	bool result;
	for(auto& it : this->List) {
		if(it.Deleting) {
			continue;						//�폜�\�񂳂�Ă���ꍇ�̓X�L�b�v
		} else if(std::find(TS_CONTAINERALL(this->runningStack), &it) != this->runningStack.end()) {
			continue;						//���ݎ��s���̃X���b�h�ł���ꍇ�̓X�L�b�v
		}

		if(it.DoneAhead == false && it.Suspended == false && (it.SceneLimit == static_cast<int>(TS::System::Scene::All) || Game.CheckCurrentSceneID(it.SceneLimit))) {
			this->runningStack.push_back(&it);		//���s���̃X���b�h�Ƃ��ăX�^�b�N�ɐς�
			result = this->doThread(it);			//�X���b�h�����s
			this->runningStack.pop_back();			//�X�^�b�N�ɐς񂾂��̂����낷
			if(result) {
				it.Deleting = true;			//����I�������ꍇ�̓X���b�h�̍폜�\����s��
			}
		}
		it.DoneAhead = false;
	}
	this->clearInvalidThreadInStack();
	this->clearDeletingThread();
}


//####################  �w�肵���X���b�h���s�I�Ɏ��s  ####################
//���O���󔒂ɂ����ꍇ�A���ׂẴX���b�h��ΏۂƂ���
void CMultiThread::DoAhead(string name) {
	bool result;
	for(auto& it : this->List) {
		if(name.empty() || it.Name == name) {
			if(std::find(TS_CONTAINERALL(this->runningStack), &it) != this->runningStack.end()) {
				continue;					//���ݎ��s���̃X���b�h�ł���ꍇ�̓X�L�b�v
			}

			if(it.DoneAhead == false && it.Suspended == false && (it.SceneLimit == static_cast<int>(TS::System::Scene::All) || Game.CheckCurrentSceneID(it.SceneLimit))) {
				this->runningStack.push_back(&it);		//���s���̃X���b�h�Ƃ��ăX�^�b�N�ɐς�
				result = this->doThread(it);			//�X���b�h�����s
				this->runningStack.pop_back();			//�X�^�b�N�ɐς񂾂��̂����낷
				if(result) {
					it.Deleting = true;			//����I�������ꍇ�̓X���b�h�̍폜�\����s��
				}
			}
			it.DoneAhead = true;
		}
	}
	this->clearInvalidThreadInStack();
	this->clearDeletingThread();
}


//####################  �X���b�h�����ׂč폜  ####################
//���ӁF�ی삳�ꂽ�X���b�h�ALeaveCurrentScene�Ŏw�肵���V�[���ł̂ݗL���Ȋ֐�������
void CMultiThread::Clear(bool LeaveCurrentScene) {
	for(auto& it : this->List) {
		if(!it.Protected) {
			if(!LeaveCurrentScene || !Game.CheckCurrentSceneID(it.SceneLimit)) {
				it.Deleting = true;		//����̑S���s�̂Ƃ��ɍ폜�����悤�ɗ\�񂷂�
				it.Suspended = true;
			}
		}
	}
}


//####################  �w�肵���X���b�h���폜����  ####################
//���ӁF�ی삳�ꂽ�X���b�h������
void CMultiThread::DeleteThread(string name) {
	this->List.erase(std::remove_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& item) {
		return (item.Name == name && !item.Protected);
	}), this->List.end());
}


//####################  �w�肵���X���b�h�̒�~��Ԃ�ݒ肷��  ####################
//���O���󔒂ɂ����ꍇ�A���ׂẴX���b�h��ΏۂƂ���
//�������ꂽ�X���b�h�����݂��Ȃ��ꍇ�͉����N����Ȃ�
void CMultiThread::SetSuspend(string name, bool IsSuspended) {
	for(auto& it : this->List) {
		if(it.Name == name || name.empty()) {
			it.Suspended = IsSuspended;
		}
	}
}


//####################  �w�肵���X���b�h�̕ی��Ԃ�ݒ肷��  ####################
//���O���󔒂ɂ����ꍇ�A���ׂẴX���b�h��ΏۂƂ���
//�������ꂽ�X���b�h�����݂��Ȃ��ꍇ�͉����N����Ȃ�
void CMultiThread::SetProtect(string name, bool IsProtected) {
	for(auto& it : this->List) {
		if(it.Name == name || name.empty()) {
			it.Protected = IsProtected;
		}
	}
}


//####################  �w�肵���X���b�h�̒�~��Ԃ��擾����  ####################
//�V���ɏd�������X���b�h���ǉ�����ď�Ԃ����ꂳ��Ă��Ȃ��ꍇ���l�����邪�A���Ȃ��Ƃ��ŏ��Ɍ�������̂͐ݒ�ς݂Ƃ݂Ȃ���
bool CMultiThread::GetSuspend(string name) {
	for(auto& it : this->List) {
		if(it.Name == name) {
			return it.Suspended;
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�MT�֐� [%s] ���Q�Ƃ���܂����B", name.c_str());
	return false;
}


//####################  �w�肵���X���b�h�̕ی��Ԃ��擾����  ####################
//�V���ɏd�������X���b�h���ǉ�����ď�Ԃ����ꂳ��Ă��Ȃ��ꍇ���l�����邪�A���Ȃ��Ƃ��ŏ��Ɍ�������̂͐ݒ�ς݂Ƃ݂Ȃ���
bool CMultiThread::GetProtect(string name) {
	for(auto& it : this->List) {
		if(it.Name == name) {
			return it.Protected;
		}
	}
	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�MT�֐� [%s] ���Q�Ƃ���܂����B", name.c_str());
	return false;
}


//####################  �w�肵���X���b�h�̏I����҂�  ####################
void CMultiThread::WaitForThread(string name, double MaxSec) {
	int StartTime = DxLib::GetNowCount();
	while(CountThread(name) > 0 || ::GetNowCount() - StartTime > MaxSec * 1000) {
		Game.DoSceneDraw();
		TS_EVERYLOOP;
	}
}


//####################  ���݂̃X���b�h�������O�o�͂���  ####################
void CMultiThread::PrintLog() {
	string buf;
	TS_LOG("�l�s�֐��ꗗ:");
	for(auto& thread : this->List) {
		buf = "\t" + thread.Name + ":\n\t\t";
		buf += "  ";
		buf += (thread.IsSQFunc ? "SQ" : "C++");
		buf += "  ";
		buf += (thread.Suspended ? "[��~] " : "");
		buf += "  ";
		buf += (thread.Protected ? "[�ی�]" : "");
		buf += "  ";
		buf += (thread.Deleting ? "[�폜��]" : "");
		buf += thread.GetValueData();
		TS_LOG(buf);
	}
}


//####################  �w�肳�ꂽ�X���b�h�֐������ۂɌĂяo��  ####################
bool CMultiThread::doThread(CThreadInfo& trd) {
	if(!trd.IsSQFunc) {
		//C++�֐����Ăяo��: �X���b�h�̃I�u�W�F�N�g�������ɓn��
		return trd.Function(trd);
	} else {
		//SQ�֐����Ăяo��: �X���b�h�̃n���h���������ɓn��
		Sqrat::Script tempSQ;
		auto func = Sqrat::Function(Sqrat::RootTable(), _SC(trd.SQFuncName.c_str()));
		if(!func.IsNull()) {
			//�X�N���v�g�֐������݂���ꍇ�̂�
			return func.Evaluate<bool>(trd.Handle);
		} else {
			//���݂��Ȃ��ꍇ�͒����ɃX���b�h����폜����悤�� true �ŕԂ�
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "�X�N���v�g�֐������X���b�h�̎��s�Ɏ��s���܂����B\n>%s", trd.SQFuncName.c_str());
			return true;
		}
	}
}


//####################  �폜�\�񂳂�Ă���X���b�h���ꊇ�폜����  ####################
void CMultiThread::clearDeletingThread() {
	//���s���̃X���b�h�����݂��Ȃ��ꍇ�̂ݏ�������
	if(this->runningStack.empty()) {
		this->List.erase(std::remove_if(TS_CONTAINERALL(this->List), [](CThreadInfo& item) {
			return item.Deleting;
		}), this->List.end());
	}
}


//####################  ���s���̃X���b�h���i�[���Ă���X�^�b�N�ɑ��݂��Ȃ��X���b�h������ꍇ�͍폜����  ####################
void CMultiThread::clearInvalidThreadInStack() {
	this->runningStack.erase(std::remove_if(TS_CONTAINERALL(this->runningStack), [&](CThreadInfo* item) {
		//����̃X���b�h���X���b�h���X�g�ɓo�^����Ă��邩�ǂ����𒲂ׂāA���݂��Ȃ��ꍇ�͍폜
		return std::find_if(TS_CONTAINERALL(this->List), [&](CThreadInfo& thread) {
			return item == &thread;
		}) == this->List.end();
	}), this->runningStack.end());
}
