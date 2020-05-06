//###################################################################
//			�ėp�����Q
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];
extern void printSQCallStackAll(HSQUIRRELVM& v);
void SQDebuggerKiller();


//####################  �R���X�g���N�^�[  ####################
CGameObject::CGameObject() {
	//�e��I�u�W�F�N�g������������
	this->initKeyInputData();
	this->DrawBright.Set(255, 255, 255);

	//�f�ފǗ��N���X�̃C���X�^���X���Z�b�g����
	TS_COUNTLOOP(i, static_cast<int>(TS::Material::MaterialType::Count)) {
		switch(static_cast<TS::Material::MaterialType>(i)) {
			case TS::Material::MaterialType::Graphics:
				this->Material.Types[i] = new CMaterial::CMaterialGraphics();
				break;
			case TS::Material::MaterialType::Sound:
				this->Material.Types[i] = new CMaterial::CMaterialSound();
				break;
			case TS::Material::MaterialType::Font:
				this->Material.Types[i] = new CMaterial::CMaterialFont();
				break;
		}
	}
}


//####################  ���݂̒l��͈͓��ɒ�������  ####################
int CGameObject::WithIn(int val, int _min, int _max) {
	if(_min > _max) {
		std::swap(_min, _max);		//�召�֌W���t�̏ꍇ�͓���ւ���
	}
	return TS_WITHIN(val, _min, _max);
}


//####################  ���l�̃��[�v�J�E���e�B���O�E����  ####################
int CGameObject::UpAround(int val, int _min, int _max) {
	if(_min > _max) {
		std::swap(_min, _max);		//�召�֌W���t�̏ꍇ�͓���ւ���
	}
	const int temp = val + 1;
	return (temp < _max) ? temp : _min;
}


//####################  ���l�̃��[�v�J�E���e�B���O�E����  ####################
int CGameObject::DownAround(int val, int _min, int _max) {
	if(_min > _max) {
		std::swap(_min, _max);		//�召�֌W���t�̏ꍇ�͓���ւ���
	}
	const int temp = val - 1;
	return (_min <= temp) ? temp : _max - 1;
}


//####################  ������_���l�ɕϊ�  ####################
bool CGameObject::IntToBool(int value) {
	return (value == FALSE) ? false : true;
}


//####################  �_���l�𐮐��ɕϊ�  ####################
int CGameObject::BoolToInt(bool value) {
	return value ? TRUE : FALSE;
}


//####################  �ŏ��l�ƍő�l���w�肵�ė����𓾂�  ####################
int CGameObject::NextRand(int min, int max) {
	return min + GetRand(max - min);
}


//####################  �V�[����J�ڂ���  ####################
void CGameObject::ChangeScene(CSceneBase* nextScene, TS::System::Scene sceneID) {
	//�O�̃V�[����j������
	this->DisposeScene();

	//MT�֐��͂��ׂĔj������
	Game.MT.Clear(false);

	//�V�[����J�ڂ����đf�ނ�ǂݍ���
	this->currentSceneID = static_cast<int>(sceneID);
	if(nextScene != nullptr) {
		TS_LOG("�V�[�� [%s] �ֈڍs���܂��B", nextScene->GetName().c_str());
		this->initKeyInputData();
		this->currentScene = nextScene;
		this->currentScene->StartTime = DxLib::GetNowCount();
		this->currentScene->SQHandle = Game.SQ.Add(TS::FileNames::SQScenes[this->currentSceneID]);
		this->currentScene->SQSubHandle = -1;
		this->currentScene->Setup();
	}
	this->SQ.CollectGarbage();
}


//####################  ���݂̃V�[����j������  ####################
void CGameObject::DisposeScene() {
	if(this->currentScene != nullptr) {
		TS_LOG("���݂̃V�[�� [%s] ��j�����܂��B", this->currentScene->GetName().c_str());
		this->currentScene->BGM.Dispose();
		this->currentScene->BGS.Dispose();
		this->currentScene->Dispose();
		delete this->currentScene;
	}
}


//####################  �T�u�V�[����J�ڂ���  ####################
void CGameObject::StartSubScene(int sceneID, bool keepMainScene, bool waitMode) {
	sceneID = Game.WithIn(sceneID, 0, static_cast<int>(TS::System::Scene::Count) - 1);

	//�O�̃T�u�V�[����j������
	if(this->currentScene->SQSubHandle != -1) {
		TS_LOG("���݂̃T�u�V�[�� [ID: %d] ��j�����܂��B", this->currentSceneID);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
		//Game.SQ.Delete(this->currentScene->SQSubHandle);
	}

	//�T�u�V�[���ڍs�ɍۂ��ď��������s��
	TS_LOG("�T�u�V�[�� [ID: %d] �ֈڍs���܂��B", sceneID);
	this->currentScene->SubSceneID = sceneID;

	//�L�[������Ԃ͌p�������܂܂ɂ��Ȃ��ƁA�V�[���ڍs��������ɓ��͂���쓮���Ă��܂�
	//this->initKeyInputData();

	if(keepMainScene) {
		//���C���V�[�����ێ����Ȃ���T�u�V�[���ֈڍs����
		this->currentScene->SQSubHandle = Game.SQ.Add(TS::FileNames::SQScenes[sceneID]);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
	} else {
		//���݂̃V�[���X�N���v�g��j�����Ēu��������: ���C���V�[�����̂��Ȃ��Ȃ�킯�ł͂Ȃ��̂ŁA�V�[�����̂̔j�������͍s��Ȃ�
		//Game.SQ.Delete(this->currentScene->SQHandle);
		this->currentScene->SQHandle = Game.SQ.Add(TS::FileNames::SQScenes[sceneID]);
		this->currentScene->SQSubHandle = -1;
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
	}
	this->SQ.CollectGarbage();

	//�ҋ@���郂�[�h�̏ꍇ�͂��̒��Ń��C�����[�v���\����: �V�[��ID���ύX�����܂Ŕ����Ȃ�
	if(waitMode) {
		while(this->GetCurrentSubSceneID() == sceneID) {
			Game.DoSceneInput();
			Game.DoSceneDraw();
			TS_EVERYLOOP;
		}
	}
}


//####################  �T�u�V�[�����I�����ă��C���V�[���ɖ߂�  ####################
void CGameObject::EndSubScene() {
	if(this->currentScene->SubSceneID == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݃T�u�V�[���ɂ͈ڍs���Ă��܂���B");
		return;
	}
	TS_LOG("�T�u�V�[�� [ID: %d] ���I�����܂��B", this->currentScene->SubSceneID);
	if(this->currentScene->SQSubHandle == -1) {
		//���C���V�[����u��������`�ŃT�u�V�[���Ɉڍs�����ꍇ: ���C���V�[���̃X�N���v�g���ēx�ǂݒ���
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
		//Game.SQ.Delete(this->currentScene->SQHandle);
		this->currentScene->SQHandle = Game.SQ.Add(TS::FileNames::SQScenes[this->currentSceneID]);
	} else {
		//���C���V�[�����ێ����Ȃ���T�u�V�[���Ɉڍs�����ꍇ
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
		//Game.SQ.Delete(this->currentScene->SQSubHandle);
		this->currentScene->SQSubHandle = -1;
	}
	this->SQ.CollectGarbage();
	this->currentScene->SubSceneID = -1;
}


//####################  ���݃T�u�V�[�����L���ł��邩�ǂ������擾����  ####################
bool CGameObject::IsSubScene() {
	return (this->currentScene != nullptr && this->currentScene->SubSceneID != -1);
}


//####################  ���݂̃��C���V�[��ID���擾����  ####################
int CGameObject::GetCurrentMainSceneID() {
	if(this->currentScene != nullptr) {
		return this->currentSceneID;
	}
	return -1;
}


//####################  ���݂̃T�u�V�[��ID���擾����  ####################
int CGameObject::GetCurrentSubSceneID() {
	if(this->currentScene != nullptr) {
		return this->currentScene->SubSceneID;
	}
	return -1;
}


//####################  �w�肵���V�[�������݂̃V�[���ƈ�v���邩�ǂ��������؂���  ####################
//���C���V�[�����ێ����Ȃ���T�u�V�[���Ɉڍs�����ꍇ�A�T�u�V�[���ł݈̂�v����
bool CGameObject::CheckCurrentSceneID(int sceneID) {
	if(this->GetCurrentSubSceneID() == -1) {
		//���݃��C���V�[���̂Ƃ��A�ȉ��̃V�[���̂ݔF�߂�
		switch(static_cast<TS::System::Scene>(this->GetCurrentMainSceneID())) {
			case TS::System::Scene::Title:
			case TS::System::Scene::Map:
				return (this->GetCurrentMainSceneID() == sceneID);
		}
		return false;
	} else {
		//���݃T�u�V�[���̂Ƃ��A�ȉ��̃V�[����F�߂Ȃ�
		switch(static_cast<TS::System::Scene>(sceneID)) {
			case TS::System::Scene::Title:
			case TS::System::Scene::Map:
				return false;
		}
		return (this->GetCurrentSubSceneID() == sceneID);
	}
}


//####################  �^�C�g���V�[���ֈړ�����  ####################
void CGameObject::ChangeToTitleScene() {
	this->ChangeScene(new CSceneTitle(), TS::System::Scene::Title);
}


//####################  �}�b�v�V�[���ֈړ�����  ####################
void CGameObject::ChangeToMapScene() {
	this->ChangeScene(new CSceneMap(), TS::System::Scene::Map);
}


//####################  ���݂̃V�[���̕`�揈�����s��  ####################
void CGameObject::DoSceneDraw() {
	//��ʂ��N���A
	TS_NOBLEND;
	TS_CLS;

	//���݂̃V�[�����Ăяo��
	if(Game.ForbidSceneDraw == false && this->currentScene != nullptr) {
		//�V�[���`��ɂ̂݁A�t�B���^�[��K�p����
		DxLib::SetDrawBright(this->DrawBright.Red, this->DrawBright.Green, this->DrawBright.Blue);
		this->currentScene->Draw();
		DxLib::SetDrawBright(255, 255, 255);
	}
	TS_NOBLEND;

	//����̕`�揈��
	Game.Sprite.DrawAll();					//�X�v���C�g�����ׂĕ`��
	TS_NOBLEND;
}


//####################  ���݂̃V�[���̓��͏������s��  ####################
void CGameObject::DoSceneInput() {
	//�L�[���͏����X�V
	this->CheckInput();

	//���݂̃V�[�����Ăяo��
	if(Game.ForbidSceneInput == false && this->currentScene != nullptr) {
		this->currentScene->Input();
	}
}


//####################  ���݂̃V�[���X�N���v�g��ǂݒ���  ####################
void CGameObject::SQCompileCurrentScene() {
	if(this->currentScene->SQSubHandle != -1) {
		Game.SQ.Recompile(this->currentScene->SQSubHandle);
	} else {
		Game.SQ.Recompile(this->currentScene->SQHandle);
	}
}


//####################  �}�b�v�V�[���ł���Ȃ炻�̃V�[���I�u�W�F�N�g��Ԃ�  ####################
CSceneMap* CGameObject::getSceneMap() {
	auto sceneObj = dynamic_cast<CSceneMap*>(this->currentScene);
	if(sceneObj == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݃}�b�v�V�[���ł͂Ȃ����߁A�V�[���I�u�W�F�N�g���擾�ł��܂���B");
	}
	return sceneObj;
}


//####################  �^�C�g���V�[���ł���Ȃ炻�̃V�[���I�u�W�F�N�g��Ԃ�  ####################
CSceneTitle* CGameObject::getSceneTitle() {
	auto sceneObj = dynamic_cast<CSceneTitle*>(this->currentScene);
	if(sceneObj == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "���݃^�C�g���V�[���ł͂Ȃ����߁A�V�[���I�u�W�F�N�g���擾�ł��܂���B");
	}
	return sceneObj;
}


//####################  �V�[������Ԃ�  ####################
string CGameObject::GetCurrentSceneName() {
	return (this->currentScene == nullptr) ? "(����)" : this->currentScene->GetName();
}


//####################  �ʂ̃X�N���v�g��ǂݍ���  ####################
void CGameObject::SQInclude(string fileName) {
	this->SQ.Do(fileName, false);
}


//####################  �X�N���v�g�𒆒f���A�X�e�b�v���s�ֈڍs����  ####################
void CGameObject::SQBreakPoint() {
	this->SQ.StartStepRun();
}


//####################  �X�N���v�g�̃X�e�b�v���s���I�����ăR�[�h���s���ĊJ����  ####################
void CGameObject::SQBreakResume() {
	this->SQ.EscapeDebugger();
}


//####################  ���Ԏw��ŃE�F�C�g  ####################
//ScreenFlip���܂ނ��߁A���m�ȃE�F�C�g�b�ɂȂ�Ȃ����Ƃ�����
//�L�[��������Ď��Ԃ����ς��̃E�F�C�g�ɂȂ�Ȃ������ꍇ�͕Ԓl��false�ɂȂ�
bool CGameObject::Wait(double timeSecond, bool enabledMT, bool keyUpBreak) {
	const int startTime = DxLib::GetNowCount();
	const int sceneTime = this->currentScene->StartTime;
	bool beforePressed = this->IsAnyKeyDown();		//�E�F�C�g�O����L�[��������Ă��邩�ǂ����̃t���O
	bool pressing = false;
	bool keyDownFlag;

	//�w�莞�Ԃ��o�߂���܂ŃT�u���C�����[�v���\������A�|�[�Y���͒�~�����܂܂ɂ���
	while(Game.Pausing || timeSecond < 0.0 || ::GetNowCount() - startTime <= timeSecond * 1000) {
		//�p���I�ȃL�[�����󋵂̍X�V
		this->CheckInput();

		if(keyUpBreak) {
			//�L�[���͂��ւ��E�F�C�g�̂Ƃ��̂݁A���͏����`�F�b�N
			keyDownFlag = this->IsAnyKeyDown();

			if(!beforePressed) {
				//�E�F�C�g�O�ɃL�[��������Ă��Ȃ����
				if(keyDownFlag) {
					pressing = true;			//�L�[�������ꂽ��ԂɂȂ���
				} else if(pressing) {
					return false;				//�L�[��������ăE�F�C�g����
				}
			} else if(!keyDownFlag) {
				//�E�F�C�g�O���牟����Ă����L�[���������ꂽ���Ƃ����m
				beforePressed = false;
			}
		}

		//�����[�v�ōs������
		if(!enabledMT || Game.Pausing) {
			TS_EVERYLOOP_NOMT;		//���̃X���b�h�����ׂĎ~�߂ăE�F�C�g����
		} else {
			Game.DoSceneDraw();
			TS_EVERYLOOP;			//���̃X���b�h�͓��삳���Ȃ���E�F�C�g����
		}
		if(sceneTime != this->currentScene->StartTime) {
			//�V�[�����J�ڂ����狭���I�ɔ�����
			return false;
		}
	}
	return true;
}


//####################  �L�[��������ė������܂ŃE�F�C�g  ####################
void CGameObject::WaitKey(bool enabledMT) {
	this->Wait(-1.0, enabledMT, true);
}


//####################  �����L�[�������ꂽ��Ԃł��邩�𒲂ׂ�  ####################
bool CGameObject::IsAnyKeyDown() {
	return std::find_if(TS_CONTAINERALL(this->keyInputData), [](int val) {
		return val > 0;
	}) != this->keyInputData.end();
}


//####################  �ݐϓI�ȃL�[���͏����擾����  ####################
int* CGameObject::GetKeyInputCounterPTR(int keyID) {
	TS_CHECKINDEX(keyID, 0, this->keyInputData.size()) {
		return &this->keyInputData[keyID];
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�����ȃL�[�ԍ� [%d] �ł��B", keyID);
		return nullptr;
	}
}
int CGameObject::GetKeyInputCounter(int keyID) {
	return *this->GetKeyInputCounterPTR(keyID);
}
void CGameObject::SetKeyInputCounter(int keyID, int val) {
	*this->GetKeyInputCounterPTR(keyID) = val;
}


//####################  �g�����W�V�����̑O�������s��  ####################
void CGameObject::PrepareTransition() {
	//�`����~����
	Game.ForbidSceneDraw = true;
	Game.Sprite.ForbidLayerDraw = true;

	//�Ō�̃X�N���[����ۑ�����
	this->screenHandleForTransition = DxLib::MakeGraph(Game.WindowSize.Width, Game.WindowSize.Height);
	DxLib::GetDrawScreenGraph(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, this->screenHandleForTransition);
}


//####################  �g�����W�V���������s����  ####################
void CGameObject::DoTransition(string blendFileName, int speed) {
	//�`����ĊJ���ău�����h���J�n����
	int blendHandle = DxLib::LoadBlendGraph((TS::DirectoryNames::PrePath + blendFileName).c_str());
	Game.ForbidSceneDraw = false;
	Game.Sprite.ForbidLayerDraw = false;

	for(int i = 0; i < 255; i += speed) {
		Game.DoSceneDraw();
		DxLib::DrawBlendGraph(0, 0, this->screenHandleForTransition, false, blendHandle, i, TS::System::DXBlendRange);
		TS_EVERYLOOP_NOMT;
	}

	//���S��Ԃōēx�`�悷��
	this->DoSceneDraw();
	Game.FPSKeeper.Update();

	//�g�����W�V�����Ɏg�p�������\�[�X��j������
	DxLib::DeleteGraph(blendHandle);
	DxLib::DeleteGraph(this->screenHandleForTransition);
}


//####################  ��ʂ��t�F�[�h�A�E�g������  ####################
void CGameObject::FadeOut(int speed) {
	for(int cnt = 0; cnt < 255; cnt += speed) {
		this->DoSceneDraw();
		TS_ALPHABLEND(cnt);
		TS_CLS;
		TS_NOBLEND;
		TS_EVERYLOOP_NOMT;
	}

	//���S��Ԃōēx�`�悷��
	TS_CLS;
	Game.FPSKeeper.Update();

	//�t�F�[�h�A�E�g���̓V�[���`����~����
	Game.ForbidSceneDraw = true;
}


//####################  ��ʂ��t�F�[�h�C��������  ####################
void CGameObject::FadeIn(int speed) {
	Game.ForbidSceneDraw = false;
	for(int cnt = 0; cnt < 255; cnt += speed) {
		this->DoSceneDraw();
		TS_ALPHABLEND(255 - cnt);
		TS_CLS;
		TS_NOBLEND;
		TS_EVERYLOOP_NOMT;
	}

	//���S��Ԃōēx�`�悷��
	this->DoSceneDraw();
	Game.FPSKeeper.Update();
}


//####################  �t�H���_�[�����݂��Ȃ��ꍇ�͍쐬����  ####################
void CGameObject::CheckDirectory(string path) {
	if(!::PathIsDirectory(path.c_str())) {
		::CreateDirectory(path.c_str(), nullptr);
	}
}


//####################  �w��t�H���_�[�ɂ���t�@�C�������ׂė񋓂���  ####################
//�t�H���_�[�̓v���O�����̈ʒu����ƂȂ�A�t�@�C��������������̃p�X�ŃZ�b�g�����
//�t�B���^�[�g���q�� [.xxx] �̌`���Ŏw�肷�邱��
void CGameObject::GetFiles(vector<string> &dest, string folder, string filter, bool includeSubFolder) {
	//�ċA�I�ɒǉ����邽�߈��惊�X�g�͏��������Ȃ�
	//dest.clear();

	HANDLE hFind;
	WIN32_FIND_DATA fd;

	//�����p�p�X����������
	if(folder.empty() == false && folder.substr(folder.size() - 1, 1) != "\\") {
		folder += "\\";		//�Ώۃt�H���_�[�̖����͕K����؂�L���ɂ���
	}
	string pattern = Game.CurrentPath + folder;
	pattern += "*";

	//�t�@�C���T��
	hFind = ::FindFirstFile(pattern.c_str(), &fd);
	if(hFind == INVALID_HANDLE_VALUE) {
		return;		//�������s
	}

	//�����t�@�C�������i�[���Ă���
	do {
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		   && strcmp(fd.cFileName, "..") != 0 && strcmp(fd.cFileName, ".") != 0
		   && includeSubFolder) {
			//�t�H���_�[�����o���ꂽ�炻�̒��g����������
			string subDir = folder + string(fd.cFileName);
			CGameObject::GetFiles(dest, subDir, filter, true);
		} else if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
			//�B���t�@�C���͏���
			string str = fd.cFileName;
			string ext = ::PathFindExtension(str.c_str());
			if(str != "desktop.ini" && ext == filter) {
				str = folder + str;
				dest.push_back(str);	//�t�@�C���������X�g�Ɋi�[
			}
		}
	} while(::FindNextFile(hFind, &fd));		//���̃t�@�C����T��

	::FindClose(hFind);
}


//####################  ���������؂蕶���ɏ]���ĕ�������  ####################
//�g�p��FSplitString(out, "hoge,piyo,fug", ",");
void CGameObject::SplitString(vector<string> &dest, const string &src, const string &delim) {
	string::size_type start = 0;	//�f���~�^����������C���f�b�N�X
	dest.clear();
	if(src.empty()) {
		return;			//�\�[�X�����񂪋󔒂̏ꍇ�͒ǉ����Ȃ�
	}
	while(true) {
		const string::size_type end = src.find(delim, start);
		if(end != string::npos) {
			dest.push_back(src.substr(start, end - start).c_str());
		} else {		//�f���~�^��������Ȃ������ꍇ
			dest.push_back(src.substr(start, src.length() - start).c_str());
			break;
		}
		start = end + delim.length();
	}
}


//####################  �����񃊃X�g�̐擪�s�𔲂��o��  ####################
//���ӁF�����̈����̒��Ŏg��Ȃ����ƁB�����̕]�������͕s��ł��邽�߁A���Ԓʂ�Ɏ��o����Ȃ��\��������B
string CGameObject::PopHead(vector<string>& src) {
	if(src.size() == 0) {
		return "";
		//NOTE: ���g���Ȃ��ꍇ�ł��x���͏o���Ȃ��B
	} else {
		string strtemp = src[0];
		src.erase(src.begin());
		return strtemp;
	}
}


//####################  ���b�Z�[�W�{�b�N�X��\������  ####################
int CGameObject::MsgBox(string msg, string caption, int iconID, int buttonID) {
	return ::MessageBox(DxLib::GetMainWindowHandle(), msg.c_str(), caption.c_str(), iconID + buttonID);
}


//####################  �f�o�b�O�E�B���h�E�Ƀ��b�Z�[�W��\������  ####################
void CGameObject::PrintLog(LPCTSTR fileName, LPCTSTR funcName, int lineNum, string header, string fmt, ...) {
	//�ϒ���������������
	va_list args;
	va_start(args, fmt);
	::_vsntprintf(::buf, sizeof(::buf) - 1, fmt.c_str(), args);
	va_end(args);

	fmt = ::buf;
	fmt = header + fmt;		//�w�b�_�[��t��

	//�\�[�X����t������
	if(fileName != nullptr && funcName != nullptr) {
		::sprintf_s(::buf, " -----> %s -> %s �֐� -> %d �s��", PathFindFileName(fileName), funcName, lineNum);
		fmt += ::buf;
	}

	//������t������ꍇ
	if(!Game.NoTimeStamp) {
		struct tm *ti;			//���ԏ��
		time_t now;				//���݂܂ł̌o�ߎ���
		string NowTime;			//�ŏI�I�Ȍ��ݓ����̕�����
		::time(&now);
		ti = ::localtime(&now);
		::strftime(::buf, 255, "%Y/%m/%d ", ti);
		NowTime += ::buf;
		::strftime(::buf, 255, "%H:%M:%S   ", ti);
		NowTime += ::buf;
		fmt = ::buf + fmt;		//�w�b�_�[�����O�ɕt��
	}

	//���s�L���������ŕt������
	fmt += "\n";

	Game.Log.push_front(fmt);				//���O�ɒǉ�����
	if(Game.Log.size() > TS::System::DebugLogCountMax) {
		Game.Log.pop_back();				//�ł��Â����O����������
	}

#ifdef _DEBUG
	::printf(fmt.c_str());
	::OutputDebugString(fmt.c_str());		//�C�~�f�B�G�C�g�E�B���h�E�ɏo��
#endif
}


//####################  �f�o�b�O�o�͂̊ȈՔ�  ####################
void CGameObject::PrintLog(string text) {
	TS_LOG(text);
}


//####################  �A�v���P�[�V�������x���𔭐�������  ####################
void CGameObject::RaiseWarning(LPCTSTR fileName, LPCTSTR funcName, int lineNum, TS::System::ErrorID id, string fmt, ...) {
	//�ϒ���������������
	va_list args;
	va_start(args, fmt);
	::_vsntprintf(::buf, sizeof(::buf) - 1, fmt.c_str(), args);
	va_end(args);

	TS_WARN(fileName, funcName, lineNum, std::to_string(static_cast<int>(id)) + " : " + ::buf);
}


//####################  �A�v���P�[�V�������G���[�𔭐�������  ####################
void CGameObject::RaiseError(LPCTSTR fileName, LPCTSTR funcName, int lineNum, TS::System::ErrorID id, string fmt, ...) {
	//�ϒ���������������
	va_list args;
	va_start(args, fmt);
	::_vsntprintf(::buf, sizeof(::buf) - 1, fmt.c_str(), args);
	va_end(args);

	TS_ERROR(fileName, funcName, lineNum, std::to_string(static_cast<int>(id)) + " : " + ::buf);
}


//####################  API�֐��Ŕ��������G���[���C�~�f�B�G�C�g�E�B���h�E�ɏo�͂���  ####################
void CGameObject::PrintAPIError() {
	string ErrorText;
	LPVOID lpMsgBuf;
	::FormatMessage(				//�G���[�\��������쐬
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, nullptr
	);
	ErrorText = "ERROR From WinAPI : ";
	ErrorText += static_cast<const char*>(lpMsgBuf);
	if(ErrorText.find("�������I��") == string::npos) {		//����I�����Ă���G���[���b�Z�[�W�͏o�͂��Ȃ�
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::WinAPI, ErrorText);			//���b�Z�[�W�o��
	}
	::LocalFree(lpMsgBuf);
}


//####################  ���O�������ׂăt�@�C���ɏ����o��  ####################
void CGameObject::WriteDownLog() {
	TS_LOG("���O�t�@�C���ɏo�͂��܂��B");
	Game.NoTimeStamp = true;		//���������܂߂Ȃ����O�o�͂ɐ؂�ւ���

	//�Q�[���̏�Ԃ�������O�֏o�͂���
	TS_LOG(Game.Format("\n���݂̃V�[��: %s", (Game.currentScene == nullptr) ? "null" : Game.currentScene->GetName().c_str()).c_str());

	//��DB�A���[�U�[DB�̏���������O�֏o�͂���
	for(auto vdb : Game.DB.VDB) {
		vdb->PrintData();
	}
	Game.DB.UDB.PrintData();

	//�C�x���g�ʕϐ��̏���������O�֏o�͂���
	CSceneMap::PrintMap();
	CSceneMap::PrintEVs();
	CSceneMap::PrintSelfValueData();

	//���������擾���ăt�@�C�����ɂ���
	struct tm *ti;			//���ԏ��
	time_t now;				//���݂܂ł̌o�ߎ���
	string fileName = TS::DirectoryNames::Log;
	::time(&now);
	ti = ::localtime(&now);
	::strftime(::buf, 255, "%Y-%m-%d ", ti);
	fileName += ::buf;
	::strftime(::buf, 255, "%Hh%Mm%Ss.log", ti);
	fileName += ::buf;

	//�������ݗp�Ƀt�@�C�����J��
	FILE* fp;
	Game.CheckDirectory(TS::DirectoryNames::Log);		//�t�H���_�[�����݂��Ȃ��ꍇ�͍쐬����
	if((fp = ::fopen(fileName.c_str(), "w")) == nullptr) {
		TS_MBE("���O�t�@�C���̏o�͂Ɏ��s���܂����B");
		return;
	}

	//���O�������ݏ���: ���n�񏇂ɂ��邽�߂ɋt���珑���o��
	while(!Log.empty()) {
		::fputs(Log.back().c_str(), fp);
		Log.pop_back();
	}
	::fclose(fp);
}


//####################  �f�o�b�O�R���\�[������̓��͂��󂯕t����  ####################
string CGameObject::ScanConsole() {
#ifdef _DEBUG
	CDebug::ActivateConsole();
	::printf(">");
	::fflush(stdin);
	::fgets(::buf, sizeof(::buf) - 1, stdin);
	::buf[strlen(::buf) - 1] = '\0';
	CDebug::ActivateGameWindow();
	return ::buf;
#else
	return "";
#endif
}


//####################  �V�X�e�����\�[�X����`�悷��  ####################
void CGameObject::DrawSystemResourceInfo() {
	string strbuf;

	//�t���[�����[�g
	this->frameCounter++;
	const int nowTime = DxLib::GetNowCount();
	const int delta = nowTime - this->lastTimeCount;
	if(delta >= 1000) {
		this->nowFPS = static_cast<double>(this->frameCounter) / delta * 1000.0;
		this->lastTimeCount = nowTime;
		this->frameCounter = 0;
	}
	strbuf += Game.Format("FPS: %6.2lf\n", this->nowFPS);
	//strbuf += Game.Format("FPS: %6.2lf (Skip %d)\n", this->nowFPS, this->FPSKeeper.getTempSkipFrameCount());

	//�������
#ifdef _DEBUG
	PROCESS_MEMORY_COUNTERS pmc = PROCESS_MEMORY_COUNTERS();
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, true, GetCurrentProcessId());
	double mem = 0, swap = 0;
	if(hProcess != nullptr) {
		if(::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {		// �v���Z�X�̎g�p�������𒲂ׂ�
			mem = pmc.WorkingSetSize / 1024.0 / 1024.0;			// MB�P��
			swap = pmc.PagefileUsage / 1024.0 / 1024.0;			// MB�P��
			::CloseHandle(hProcess);

			//�L�^�X�V
			if(this->maxRAM < mem) {
				this->maxRAM = mem;
			}
			if(this->maxSwap < swap) {
				this->maxSwap = swap;
			}
		}
	}
	strbuf += Game.Format("RAM:  %6.2lf MB (Max %6.2lf)\n", mem, this->maxRAM);
	strbuf += Game.Format("Swap: %6.2lf MB (Max %6.2lf)\n\n", swap, this->maxSwap);
#endif

	//�Q�[���V�X�e��
	strbuf += Game.Format("Scene: %s [%d / %d]\n", Game.GetCurrentSceneName().c_str(), Game.GetCurrentMainSceneID(), Game.GetCurrentSubSceneID());
	strbuf += Game.Format("Forbidden-Draw:  %s\n", Game.ForbidSceneDraw ? "ON" : "OFF");
	strbuf += Game.Format("Forbidden-Input: %s\n", Game.ForbidSceneInput ? "ON" : "OFF");
	strbuf += "\n";

	//���̑��̃��\�[�X
	strbuf += Game.Format("SQ-Scripts: %2d\n", Game.SQ.GetCount());
	strbuf += Game.Format("MT-Threads: %2d\n", Game.MT.GetThreadCount());
	strbuf += Game.Format("Sprites:    %2d\n", Game.Sprite.Data.GetCount());
	strbuf += Game.Format("Material-Graphics: %3d\n", Game.Material.getGraphics()->Data.GetCount());
	strbuf += Game.Format("Material-Sounds:   %3d\n", Game.Material.getSounds()->Data.GetCount());
	strbuf += Game.Format("Material-Fonts:    %3d\n", Game.Material.getFonts()->Data.GetCount());

	TS_ALPHABLEND(TS::Map::SemiTransparent);
	DxLib::DrawBox(0, 0, 180, 185, BLACK, true);
	TS_NOBLEND;
	DxLib::DrawStringToHandle(0, 0, strbuf.c_str(), WHITE, Game.Material.getFonts()->DefaultFontHandle);
}


//####################  �E�B���h�E��Ԃ������o��  ####################
void CGameObject::SaveWindowState() {
	CPoint gWndPos;
	RECT cWndRect;
	FILE *fp;
	char path[MAX_PATH];

	//�R���\�[���̕\���ʒu�𒲂ׂ�
	cWndRect.left = 0;
	cWndRect.top = 0;
#ifdef _DEBUG
	::GetWindowRect(::GetConsoleWindow(), &cWndRect);
#endif

	// �����o����̃p�X�𐶐� (AppData/.../�Q�[���^�C�g�� �ȉ�)
	::SHGetSpecialFolderPath(nullptr, path, CSIDL_APPDATA, 0);
	::strcat_s(path, "\\");
	::strcat_s(path, Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
	if (!::PathFileExists(path)) {
		// �f�B���N�g���[�����݂��Ȃ��ꍇ�͍쐬
		::CreateDirectory(path, nullptr);
	}
	::strcat_s(path, "\\windowsettings.dat");

	fp = ::fopen(path, "w");
	if(fp != nullptr) {
		DxLib::GetWindowPosition(&gWndPos.X, &gWndPos.Y);
		::fprintf(fp, "%d\n%d\n%d\n%d\n%d\n%d\n", cWndRect.left, cWndRect.top, gWndPos.X, gWndPos.Y, ::GetWindowModeFlag(), Game.FPSKeeper.getSkipFrameCount());
		::fclose(fp);
	}
}


//####################  �E�B���h�E��Ԃ𕜌�����  ####################
void CGameObject::LoadWindowState() {
	CPoint gWndPos;
	FILE* fp;
	char path[MAX_PATH];
	const DWORD dwNameSize = sizeof(::buf);

	// �ݒ肳��Ȃ������ꍇ�̏����l���Z�b�g���Ă���
	gWndPos.Set(100, 100);
	DxLib::ChangeWindowMode(true);
	this->FPSKeeper.setSkipFrameCount(0);

	// �����o����̃p�X�𐶐� (AppData/.../�Q�[���^�C�g�� �ȉ�)
	::SHGetSpecialFolderPath(nullptr, path, CSIDL_APPDATA, 0);
	::strcat_s(path, "\\");
	::strcat_s(path, Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
	if (!::PathFileExists(path)) {
		// �f�B���N�g���[�����݂��Ȃ��ꍇ�͍쐬
		::CreateDirectory(path, nullptr);
	}
	::strcat_s(path, "\\windowsettings.dat");

	TS_LOG("�E�B���h�E��Ԃ𕜌����܂�...");
	fp = ::fopen(path, "r");
	if(fp != nullptr) {
		//�I�[�v���ɐ��������Ƃ�����
		TS_COUNTLOOP(i, 4) {
			::fgets(::buf, sizeof(::buf) - 1, fp);
			switch(i) {
				case 0:
					Game.ConsolePosition.X = atoi(::buf);
					break;
				case 1:
					Game.ConsolePosition.Y = atoi(::buf);
					break;

				case 2:
					gWndPos.X = atoi(::buf);
					break;
				case 3:
					gWndPos.Y = atoi(::buf);
					break;

				case 4:
					DxLib::ChangeWindowMode(atoi(::buf));
					break;

				case 5:
					Game.FPSKeeper.setSkipFrameCount(atoi(::buf));
					break;
			}
		}

		// NOTE: �R���\�[���E�B���h�E�ʒu�̐ݒ�͏����������̒��ōs����

		DxLib::SetWindowPosition(gWndPos.X, gWndPos.Y);
		::fclose(fp);
		TS_LOG("�E�B���h�E��Ԃ̕������������܂����B");
	} else {
		//�f�t�H���g�ݒ�ŋN������
		TS_LOG("�E�B���h�E��Ԃ̕����Ɏ��s�������߁A�f�t�H���g�ݒ�ŋN�����܂��B");
		DxLib::ChangeWindowMode(true);
	}
}


//####################  �v���O�������I��������  ####################
void CGameObject::ApplicationExit(int code) {
	if(code == static_cast<int>(TS::System::ExitCode::Question)) {
		if(::MessageBox(::GetMainWindowHandle(), "�Q�[�����I�����܂����H", "Game.exe", MB_YESNO + MB_ICONQUESTION) != IDYES) {
			return;
		}
		code = static_cast<int>(TS::System::ExitCode::Success);
	}

	TS_LOG_NOTIME("\n--------------------------------------------------------------------\n");
	TS_LOG("�v���O�������I�����鏀���ɓ���܂�...\n");
	if(Game.SQ.IsDebugging()) {
		//�f�o�b�K�[���N�����̏ꍇ�͂������ɏI���̍��}�𑗂�
		Game.SQ.KillDebuggerFlag = true;
		TS_LOG("�X�N���v�g�f�o�b�K�[���I�����Ă��܂�...\n");
		Game.RegularFunc.AddFunction(::SQDebuggerKiller);
		return;
	}
	if(code != static_cast<int>(TS::System::ExitCode::Success) && code != static_cast<int>(TS::System::ExitCode::InvalidSystemDB)) {
		TS_LOG_NOTIME("\n--------------------------------------------------------------------\n");
		TS_LOG_NOTIME("\t�\�����ʃG���[�ɂ��ُ�I��");
		TS_LOG_NOTIME("\n--------------------------------------------------------------------\n");

		//�ُ�I���̂Ƃ��̓��O���t�@�C���ɏo�͂���
		::printSQCallStackAll(Game.SQ.GetVM());
		Game.WriteDownLog();		
	}

	//�j������
	Game.DisposeScene();
	vector<CSceneMap::CEVSelfValueOneData>().swap(CSceneMap::SelfValueData);
	Game.Material.Dispose();
	if (code != static_cast<int>(TS::System::ExitCode::InvalidSystemDB)) {
		Game.SaveWindowState();
	}
	Game.SQ.Dispose();
	::GGSFREE();
	DxLib::DxLib_End();
#ifdef _DEBUG
	::FreeConsole();
#endif
	std::exit(code);			//���ׂẴI�u�W�F�N�g��j��������A�v���O�������I������
}


//####################  �������Ԃ̌v�����J�n  ####################
void CGameObject::StartMeasureProcTime() {
	CGameObject::MeasureProcTime = DxLib::GetNowHiPerformanceCount();
}


//####################  �������Ԃ̌v�����ʂ�\�����A�����Čv�����J�n����  ####################
void CGameObject::ContinueMeasureProcTime(string tag) {
	CGameObject::EndMeasureProcTime(tag);
	CGameObject::StartMeasureProcTime();
}


//####################  �������Ԃ̌v�����ʂ�\��  ####################
void CGameObject::EndMeasureProcTime(string tag) {
	CGameObject::MeasureProcTime = DxLib::GetNowHiPerformanceCount() - CGameObject::MeasureProcTime;
	TS_LOG("��������: %lld �}�C�N���b [%s]", CGameObject::MeasureProcTime, tag.c_str());
}


//####################  �g���K�[�t�̒�����s�F�X�N���v�g�f�o�b�K�[���I������̂��Ď����ăv���O�������I��������  ####################
void SQDebuggerKiller() {
	if(!Game.SQ.IsDebugging()) {
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
	}
}


//####################  ���ׂẴL�[�̓��͏�Ԃ��p���I�ɃJ�E���g����  ####################
void CGameObject::CheckInput() {
	char charbuf[MAXBYTE + 1];
	DxLib::GetHitKeyStateAll(charbuf);
	const int input = DxLib::GetJoypadInputState(DX_INPUT_PAD1);
	for(int i = 0; i <= MAXBYTE; i++) {
		//�J�E���g�𖳎�����R�[�h���`�F�b�N
		auto ignore = false;
		for(const auto& ignoreCode : this->ignoreKeyCodes) {
			if(i == ignoreCode) {
				ignore = true;
			}
		}
		if(ignore == true) {
			(*Game.GetKeyInputCounterPTR(i)) = 0;
			continue;
		}

		//�J�E���g�������s
		if(charbuf[i] == TRUE) {
			(*Game.GetKeyInputCounterPTR(i))++;
		} else {
			//�Q�[���p�b�h�̓��͏����L�[���ɕϊ�����
			if(i == KEY_INPUT_DOWN && (input & PAD_INPUT_DOWN) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_UP && (input & PAD_INPUT_UP) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_LEFT && (input & PAD_INPUT_LEFT) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_RIGHT && (input & PAD_INPUT_RIGHT) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_Z && (input & PAD_INPUT_1) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_X && (input & PAD_INPUT_2) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_C && (input & PAD_INPUT_3) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_A && (input & PAD_INPUT_4) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_S && (input & PAD_INPUT_5) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_D && (input & PAD_INPUT_6) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_Q && (input & PAD_INPUT_7) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_W && (input & PAD_INPUT_8) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_ESCAPE && (input & PAD_INPUT_9) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_SPACE && (input & PAD_INPUT_10) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			}
			*Game.GetKeyInputCounterPTR(i) = 0;
		}
	}
}


//####################  UTF-8 ���� Shift-JIS �ɕϊ�����  ####################
string CGameObject::UTF8ToSJIS(string srcUTF8) {
	//Unicode�֕ϊ���̕����񒷂𓾂�
	const int lenghtUnicode = ::MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, nullptr, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8����Unicode�֕ϊ�
	::MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJIS�֕ϊ���̕����񒷂𓾂�
	const int lengthSJis = ::WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, nullptr, 0, nullptr, nullptr);

	//�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
	char* bufShiftJis = new char[lengthSJis];

	//Unicode����ShiftJIS�֕ϊ�
	::WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, nullptr, nullptr);

	string strSJis(bufShiftJis);

	delete[] bufUnicode;
	delete[] bufShiftJis;
	return strSJis;
}


//####################  ���[�̃_�u���N�H�[�g���폜����  ####################
string CGameObject::CutDQ(string baseString) {
	if(baseString.size() > 0) {
		if(baseString.substr(0, 1) == "\"") {
			baseString = baseString.substr(1);
		}
		if(baseString.substr(baseString.size() - 1, 1) == "\"") {
			baseString = baseString.substr(0, baseString.size() - 1);
		}
	}
	return baseString;
}


//####################  �������u�����ĕԂ�  ####################
string CGameObject::Replace(string baseString, string before, string after) {
	string::size_type Pos(baseString.find(before));

	//find��replace���J��Ԃ��āA������Ɋ܂܂��S�Ă̕������u������
	while(Pos != string::npos) {
		baseString.replace(Pos, before.length(), after);
		Pos = baseString.find(before, Pos + after.length());
	}

	return baseString;
}


//####################  �S�p�����ɑΉ�������������Ԃ�  ####################
int CGameObject::GetStringLength(string src) {
	auto wstr = Game.CWString(src);
	return wstr.size();
}


//####################  �w��ʒu�ɕ������}��  ####################
string CGameObject::Insert(string dest, int index, string text) {
	auto wstr = Game.CWString(dest);
	index = Game.WithIn(index, 0, wstr.size());
	wstr.insert(index, Game.CWString(text));
	return Game.CString(wstr);
}


//####################  string �� wstring �ɕϊ�����  ####################
wstring CGameObject::CWString(string const &str, int codeMulti) {
	const int sizeWide = MultiByteToWideChar(codeMulti, 0, &str[0], -1, nullptr, 0);
	if(sizeWide == 0) {
		return L"";
	}
	wstring wstr;
	wstr.resize(sizeWide);
	MultiByteToWideChar(codeMulti, 0, &str[0], -1, &wstr[0], sizeWide);
	if(wstr[wstr.size() - 1] == L'\0') {
		//�����̏I�[�L��������
		wstr = wstr.substr(0, wstr.size() - 1);
	}
	return wstr;
}


//####################  wstring �� string �ɕϊ�����  ####################
string CGameObject::CString(wstring const &wstr, int codeMulti) {
	const int sizeMulti = WideCharToMultiByte(codeMulti, 0, &wstr[0], -1, nullptr, 0, nullptr, nullptr);
	if(sizeMulti == 0) {
		return "";
	}
	string str;
	str.resize(sizeMulti);
	WideCharToMultiByte(codeMulti, 0, &wstr[0], -1, &str[0], sizeMulti, nullptr, nullptr);
	if(str[str.size() - 1] == '\0') {
		//�����̏I�[�L��������
		str = str.substr(0, str.size() - 1);
	}
	return str;
}


//####################  0����25�܂ł̐������A���t�@�x�b�g�ɒu��������  ####################
string CGameObject::IntToAlphabet(int value) {
	string RChar;
	switch(value % 26) {
		case  0: RChar += "�`"; break;
		case  1: RChar += "�a"; break;
		case  2: RChar += "�b"; break;
		case  3: RChar += "�c"; break;
		case  4: RChar += "�d"; break;
		case  5: RChar += "�e"; break;
		case  6: RChar += "�f"; break;
		case  7: RChar += "�g"; break;
		case  8: RChar += "�h"; break;
		case  9: RChar += "�i"; break;
		case 10: RChar += "�j"; break;
		case 11: RChar += "�k"; break;
		case 12: RChar += "�l"; break;
		case 13: RChar += "�m"; break;
		case 14: RChar += "�n"; break;
		case 15: RChar += "�o"; break;
		case 16: RChar += "�p"; break;
		case 17: RChar += "�q"; break;
		case 18: RChar += "�r"; break;
		case 19: RChar += "�s"; break;
		case 20: RChar += "�t"; break;
		case 21: RChar += "�u"; break;
		case 22: RChar += "�v"; break;
		case 23: RChar += "�w"; break;
		case 24: RChar += "�x"; break;
		case 25: RChar += "�y"; break;
	}
	return RChar;
}


//####################  ������������擾����  ####################
//length=-1�̂Ƃ��A�����܂ł�ΏۂƂ���
string CGameObject::SubString(string src, int start, int length) {
	auto wstr = Game.CWString(src);
	start = Game.WithIn(start, 0, wstr.size());
	if(length == -1 || wstr.size() <= start + length) {
		return Game.CString(wstr.substr(start));
	} else {
		length = Game.WithIn(length, 0, src.size() - start);
		return Game.CString(wstr.substr(start, length));
	}
}


//####################  ���ݎ����𕶎���ɂ���  ####################
string CGameObject::NowTimeToString(string format) {
	//���Ԏ擾
	struct tm *ti;			//���ԏ��
	time_t now;				//���݂܂ł̌o�ߎ���
	::time(&now);
	ti = ::localtime(&now);

	//������������
	::strftime(::buf, sizeof(::buf) - 1, format.c_str(), ti);
	return ::buf;
}


//####################  �����������string�^�ɕϊ�����  ####################
string CGameObject::Format(string text, ...) {
	va_list ap;
	va_start(ap, text);
	::_vsntprintf(::buf, sizeof(::buf) - 1, text.c_str(), ap);
	va_end(ap);
	return ::buf;
}


//####################  �L�[���͏�������������  ####################
void CGameObject::initKeyInputData() {
	for(int i = 0; i <= MAXBYTE; i++) {
		this->keyInputData[i] = 0;
	}
}
