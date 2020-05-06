//###################################################################
//			Squirrel�g�ݍ��݃X�N���v�g�Ǘ�/�N���X��`
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


//####################  �R���X�g���N�^�[  ####################
CScript::CScript() {
	this->sqvm = sq_open(50 * 1024);
	::sq_notifyallexceptions(this->sqvm, true);			//���ׂĂ̗�O��ߑ�����
	::sq_setprintfunc(this->sqvm, ::dprintf_squirrel);	//����̏o�̓n���h���[���Z�b�g
	::sq_enabledebuginfo(this->sqvm, true);				//��Ƀf�o�b�O���t���ŃR���p�C������
	::sq_setcompilererrorhandler(this->sqvm, ::compileErrorHandler);		//�R���p�C���G���[�n���h���[���Z�b�g
	//::sqstd_seterrorhandlers(this->sqvm);				//�W���̃G���[�n���h���[���Z�b�g
	::sq_newclosure(this->sqvm, ::errorHandler, 0);		//�����^�C���G���[�n���h���[�p�֐����X�^�b�N��
	::sq_seterrorhandler(this->sqvm);					//���̊֐��������^�C���G���[�n���h���[�Ƃ��ăZ�b�g
	Sqrat::DefaultVM::Set(this->sqvm);

	int top;

	//Squirrel�W�����C�u����: ���wAPI
	top = ::sq_gettop(sqvm);			//���݂̃X�^�b�N�g�b�v�l���擾
	::sq_pushroottable(sqvm);			//���[�g�e�[�u�����X�^�b�N�ɐς�
	::sqstd_register_mathlib(sqvm);
	::sq_settop(sqvm, top);				//�X�^�b�N�g�b�v�l��߂�

	//Squirrel�W�����C�u����: ������API
	top = ::sq_gettop(sqvm);			//���݂̃X�^�b�N�g�b�v�l���擾
	::sq_pushroottable(sqvm);			//���[�g�e�[�u�����X�^�b�N�ɐς�
	::sqstd_register_stringlib(this->sqvm);
	::sq_settop(sqvm, top);				//�X�^�b�N�g�b�v�l��߂�

	//�萔�F�f�o�b�O���[�h�ł��邩�ǂ���
#ifdef _DEBUG
	//�f�o�b�K�[���Z�b�g����
	::sq_newclosure(this->sqvm, ::debugHookFunc, 0);	//�f�o�b�K�[�p�֐����X�^�b�N��
	::sq_setdebughook(this->sqvm);						//���̊֐����f�o�b�K�[�Ƃ��ăZ�b�g
	Sqrat::ConstTable().Const("DEBUG", TRUE);
#else
	Sqrat::ConstTable().Const("DEBUG", FALSE);
#endif
}


//####################  �O������VM���Q�Ƃ���  ####################
HSQUIRRELVM &CScript::GetVM() {
	return this->sqvm;
}


//####################  �X�N���v�g��ǉ�  ####################
int CScript::Add(string fileName) {
	//DX�A�[�J�C�u��͎g�p�ł��Ȃ��̂Ŗ���
	//if(!PathFileExists(fileName.c_str())) {
	//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��X�N���v�g�ł��B\n>%s", fileName.c_str());
	//	return -1;
	//}

	//���ɓo�^����Ă��Ȃ������ׂ�
	bool newAdd = false;
	CIDNameData NewItem;
	NewItem.ID = this->findID(fileName);
	NewItem.Name = fileName;
	if(NewItem.ID == -1) {
		NewItem.ID = this->generateHandle();
		newAdd = true;
	}

	//�ăR���p�C�����A���ɓǂݍ��܂�Ă��� ! �t�@�C����ID��Ԃ������ɂ���
	if(this->reloading && newAdd == false && this->isOnceFile(fileName)) {
		TS_LOG("�X�N���v�g [%d: %s] �͍ăR���p�C������܂���B", NewItem.ID, NewItem.Name.c_str());
		return NewItem.ID;
	}

	//�����ŃR���p�C�����Ď��s����
	TS_LOG("�X�N���v�g [%d: %s] ��ǂݍ��݂܂�...", NewItem.ID, NewItem.Name.c_str());
	this->compileSQ(NewItem);
	if(newAdd) {
		this->Scripts.Add(NewItem);
	}
	return NewItem.ID;
}


//####################  VM��j��  ####################
void CScript::Dispose() {
	::sq_close(this->sqvm);
}


//####################  �X�N���v�g���R���p�C�����ăt�@�C���ɏo��  ####################
// NOTE: �R���p�C���ς݂̃t�@�C���g���q�� .cnut ���ʗ�Ƃ���Ă��邪�A�萔��`���Ă��܂��Ă���̂ŁA�g���q�͕ς��Ȃ����Ƃɂ���B
void CScript::CompileToFile(string srcFileName, string destFileName) {
	Sqrat::Script script;
	script.CompileFile(srcFileName.c_str());

	//�g���q�� .cnut �ɕύX����ꍇ�͈ȉ����R�����g�A�E�g���邱��
	//destFileName = Game.Replace(destFileName, ".nut", ".nut");

	script.WriteCompiledFile(destFileName.c_str());
}


//####################  ���̃R�[�h����X�e�b�v���s��L���ɂ��܂��B  ####################
void CScript::StartStepRun() {
	this->debugging = true;
	this->StepDestinationLevel = INT_MAX;
	::printf("�X�N���v�g�̃X�e�b�v���s���J�n���܂�...");
}


//####################  �Ǘ��I�u�W�F�N�g���g�����Ԃɂ���  ####################
void CScript::Setup() {
	TS_LOG("�X�N���v�g�̏������ł�...");

	//�g�ݍ��݊֐��E�萔�̃o�C���h
	this->BindObjects();
	Game.SQ.Do(TS::FileNames::SQConstants, true);		//�萔��`���s���X�N���v�g�͕K���ŏ��ɌĂяo���K�v������

	//�I�u�W�F�N�g�̃o�C���h
	Game.SQ.SQGlue();

	//C++�����甭������C�x���g���������邽�߂̃C�x���g�n���h���[���`�����X�N���v�g������Œǉ�����
	this->Add(TS::FileNames::SQEventHandler);
	Game.SQ.Do(TS::FileNames::SQFunctions, true);		//�ėp�I�Ȋ֐���`���s���X�N���v�g��ǂݍ���ł���

	TS_LOG("�X�N���v�g�̏������������܂����B");
}


//####################  C++���̊֐���I�u�W�F�N�g���X�N���v�g�Ɍ��J����  ####################
void CScript::BindObjects() {
	//�萔�Fint�^�ɍ��킹���_���l
	Sqrat::ConstTable().Const("FALSE", (int)FALSE);
	Sqrat::ConstTable().Const("TRUE", (int)TRUE);

	//�萔�F��{�F
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

	//���w�֐���
	Sqrat::ConstTable().Const("INT_MAX", (int)INT_MAX);
	Sqrat::ConstTable().Const("INT_MIN", (int)INT_MIN);
}


//####################  �f�o�b�K�[���J�n����  ####################
void CScript::StartDebugger() {
	if(CDebug::StartDebugConsole()) {
		this->StartStepRun();		//�Ăяo����̎w���ɂ���ăX�e�b�v���s�ֈڍs����
	}
}


//####################  �f�o�b�K�[���I������  ####################
void CScript::EscapeDebugger() {
	this->debugging = false;
}


//####################  �t�@�C�����璼�ڃX�N���v�g�����s  ####################
void CScript::Do(string fileName, bool onceMode, bool forceRun) {
	//DX�A�[�J�C�u��͎g�p�ł��Ȃ��̂Ŗ���
	//if(!PathFileExists(fileName.c_str())) {
	//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��X�N���v�g�ł��B\n>%s", fileName.c_str());
	//	return;
	//}
	this->Add(fileName);	//�ǉ����ăR���p�C���{���s
}


//####################  �X�N���v�g���ăR���p�C��  ####################
//�Ԓl�͎��ۂɃR���p�C�����s��ꂽ���ǂ���
bool CScript::Recompile(int ID) {
	if(this->Scripts.Exists(ID)) {
		string fileName = this->Scripts.GetFromID(ID)->Name;
		this->reloading = true;
		this->Add(fileName);		//�R���p�C���{���s
		this->reloading = false;
		return true;
	}
	return false;
}
bool CScript::Recompile(string fileName) {
	return this->Recompile(this->findID(fileName));
}


//####################  �o�^���ꂽ���ׂẴX�N���v�g���ăR���p�C��  ####################
void CScript::RecompileAll() {
	//���ׂăR���p�C�����čĎ��s
	this->reloading = true;
	for(auto& sq : this->Scripts.Items) {
		this->Add(sq.Name);
	}
	this->reloading = false;
}


//####################  �f�o�b�K�[���N�����Ă��邩�ǂ������擾  ####################
bool CScript::IsDebugging() {
	return this->debugging;
}


//####################  �X�N���v�g�Œ�`���ꂽ�ϐ��E�e�[�u���E�N���X�̃����o�[�̒l���擾  ####################
Sqrat::Object GetValue(string valueName, string slotName) {
	//�e�[�u�����擾
	Sqrat::Object Table;
	if(slotName == "") {
		Table = Sqrat::RootTable();
	} else {
		Table = Sqrat::RootTable().GetSlot(_SC(slotName.c_str()));
	}
	if(Table.IsNull()) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "���݂��Ȃ��e�[�u�� [%s] ���Q�Ƃ���܂����B", slotName.c_str());
	}

	//�l���擾
	Sqrat::Object value = Table.GetSlot(_SC(valueName.c_str()));
	if(value.IsNull()) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "���݂��Ȃ��ϐ� [%s] ���Q�Ƃ���܂����B", slotName.c_str());
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


//####################  �X�N���v�g�Œ�`���ꂽ�ϐ��E�e�[�u���E�N���X�̃����o�[�̒l�����������������͒ǉ�  ####################
void CScript::SetSQValueByVal(bool *value, string valueName, string slotName) {
	if(slotName.empty()) {
		//�O���[�o���ϐ������������������͒ǉ�
		Sqrat::RootTable().SetValue(_SC(valueName.c_str()), *value);
	} else {
		//�e�[�u���E�N���X�̃����o�[�ϐ������������������͒ǉ�
		Sqrat::Table Table;
		Table.SetValue(_SC(valueName.c_str()), *value);
	}
}
void CScript::SetSQValueByVal(int *value, string valueName, string slotName) {
	if(slotName.empty()) {
		//�O���[�o���ϐ������������������͒ǉ�
		Sqrat::RootTable().SetValue(_SC(valueName.c_str()), *value);
	} else {
		//�e�[�u���E�N���X�̃����o�[�ϐ������������������͒ǉ�
		Sqrat::Table Table;
		Table.SetValue(_SC(valueName.c_str()), *value);
	}
}
void CScript::SetSQValueByVal(string *value, string valueName, string slotName) {
	if(slotName.empty()) {
		//�O���[�o���ϐ������������������͒ǉ�
		Sqrat::RootTable().SetValue(_SC(valueName.c_str()), *value);
	} else {
		//�e�[�u���E�N���X�̃����o�[�ϐ������������������͒ǉ�
		Sqrat::Table Table;
		Table.SetValue(_SC(valueName.c_str()), *value);
	}
}


//####################  �g���Ă��Ȃ��n���h���l���擾  ####################
int CScript::generateHandle() {
	int result = DxLib::GetRand(INT_MAX);
	while(this->Scripts.Exists(result)) {
		result = DxLib::GetRand(INT_MAX);
	}
	return result;
}


//####################  �t�@�C��������ID����������  ####################
int CScript::findID(string fileName) {
	for(auto& sq : this->Scripts.Items) {
		if(sq.Name == fileName) {
			return sq.ID;
		}
	}
	return -1;
}


//####################  �t�@�C������x�������s����Ȃ��X�N���v�g�ł��邩�ǂ����𒲂ׂ�  ####################
bool CScript::isOnceFile(string fileName) {
	string fileNameWithoutDir = PathFindFileName(fileName.c_str());
	return (fileNameWithoutDir.size() >= 1 && fileNameWithoutDir.substr(0, 1) == "!");
}


//####################  ���݂̃X�N���v�g�������O�o�͂���  ####################
void CScript::PrintLog() {
	TS_LOG("�X�N���v�g�ꗗ:");
	for(auto& sq : this->Scripts.Items) {
		TS_LOG("\t" + to_string(sq.ID) + ": " + sq.Name);
	}
}


//####################  �X�N���v�g���R���p�C�����Ď��s�F�G���[���N�����ꍇ�͏C����҂�  ####################
void CScript::compileSQ(CIDNameData& sq) {
	bool compileError = false;

#ifdef _DEBUG
	//�f�o�b�O��: �t�@�C�����璼�ڃR���p�C�����A�G���[���N������C���𑣂�
	do {
		compileError = false;
		try {
			Sqrat::Script script;
			script.CompileFile(sq.Name.c_str());
			script.Run();
		} catch(Sqrat::Exception e) {
			// NOTE: �R���p�C���G���[�n���h���[�ɂ���ăG���[���e���o�͂���Ă���B
			CDebug::ActivateConsole();

			bool handled = false;
			while(!handled) {
				::printf("\n�G���[�ӏ����C�����ĉ������B\n\t[C]�ăR���p�C��  [E]�v���O�������f (���O�o�͂Ȃ�)  [L]�v���O�������f (���O�o�͂���)\n");
				switch(CDebug::PauseConsole()) {
					case 'c':
						handled = true;
						break;

					case 'e':	//�v���O�������f: ���O�Ȃ�
						handled = true;
						Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
						break;

					case 'l':	//�v���O�������f: ���O����
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
	//�����[�X��: DX�A�[�J�C�u����R�[�h�����o���ăR���p�C������
	void* code;
	int hdl = DxLib::FileRead_open((TS::DirectoryNames::PrePath + sq.Name).c_str(), false);
	if (hdl == 0) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�X�N���v�g�̎��o���Ɏ��s���܂����B\n>%s", sq.Name.c_str());
		return;
	}
	int size = DxLib::FileRead_size((TS::DirectoryNames::PrePath + sq.Name).c_str());
	code = malloc(size + 1);
	DxLib::FileRead_read(code, size, hdl);
	((char*)code)[size] = '\0';			//�o�C�i���Ƃ��ēǂݍ���ł���̂ŁA�I�[�L�������O�ŕt������

	try {
		Sqrat::Script script;
		script.CompileString((char*)code);
		script.Run();
	} catch(Sqrat::Exception e) {
		compileError = true;
	}

	DxLib::FileRead_close(hdl);
	free(code);

	//�G���[���N�����烍�O���o�͂��đ����ɏI������
	if(compileError) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::SQ, "�X�N���v�g�̃R���p�C���G���[���������܂����B\n>(%d Bytes) %s", size, sq.Name.c_str());
		return;
	}
#endif
}
