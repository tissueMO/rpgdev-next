//###################################################################
//			Squirrel�g�ݍ��݃X�N���v�g�Ǘ�/�f�o�b�K�[
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];

void printCallStack();
void dprintf_squirrel(HSQUIRRELVM vm, const SQChar* str, ...);
void SQErrorMessage(string funcName, string msg);
void compileErrorHandler(HSQUIRRELVM v, const SQChar* msg, const SQChar* source, SQInteger line, SQInteger column);
void printSQCallStackAll(HSQUIRRELVM& v);
int errorHandler(HSQUIRRELVM v);
int debugHookFunc(HSQUIRRELVM v);


//####################  ��O�������Ɏ蓮�ł��̊֐��֔�΂��ƃX�^�b�N�_���v���o���Ă����  ####################
void printCallStack() {
	::sqstd_printcallstack(Game.SQ.GetVM());			//F10�L�[�ŃX�e�b�v���s���邱��
	Game.SQ.RecompileAll();								//�C�������炱�̍s�����s���邱��
}


//####################  print�R�}���h�ɂ���ČĂяo�����֐�  ####################
void dprintf_squirrel(HSQUIRRELVM vm, const SQChar* str, ...) {
	va_list arglist;
	va_start(arglist, str);
	::vsnprintf(::buf, sizeof(::buf) - 1, str, arglist);
	va_end(arglist);
	const bool temp = Game.NoTimeStamp;
	Game.NoTimeStamp = true;
	string strbuf = Game.Replace(::buf, "\n", "");		//���s�͈�U���ׂăJ�b�g���Ď����ōs��
	TS_LOG(strbuf);
	Game.NoTimeStamp = temp;
}


//####################  �֐����s���̃G���[���b�Z�[�W  ####################
void SQErrorMessage(string funcName, string msg) {
	TS_LOG("���̊֐��Ăяo���ɂ����ă����^�C���G���[���������Ă��܂��B\n> %s�֐�\n", funcName.c_str());
	Game.NoTimeStamp = true;
	TS_LOG("�G���[���e: \n" + msg);
	Game.NoTimeStamp = false;
}


//####################  �R���p�C���G���[�n���h���[�p�̃R�[���o�b�N�֐�  ####################
void compileErrorHandler(HSQUIRRELVM v, const SQChar* msg, const SQChar* source, SQInteger line, SQInteger column) {
	TS_LOG("�X�N���v�g���ŃR���p�C���G���[���������Ă��܂��B\n>�t�@�C��: %s -> %d �s %d ������\n\n�G���[���e:", string(source).empty() ? "(���ȃR�[�h)" : source, line, column);
	Game.NoTimeStamp = true;
	TS_LOG(msg);
	Game.NoTimeStamp = false;
}


//####################  �R�[���X�^�b�N���o�͂���  ####################
void printSQCallStackAll(HSQUIRRELVM& v) {
	int verIndex, srcLevel;
	SQObject val;
	SQStackInfos info;

	//�Ăяo������: �t���Ɏ��o�����i���݂̊K�w����ԍŏ��ɗ���j�̂ň�U�R���e�i�[�ɓ���ċt���ɂ���
	deque<string> srcLevels;
	Game.NoTimeStamp = true;
	TS_LOG("\n----- �X�N���v�g �R�[���X�^�b�N -----------------------------------\n");
	for(srcLevel = 1; SQ_SUCCEEDED(::sq_stackinfos(v, srcLevel, &info)); srcLevel++) {
		string str;
		str = Game.Format("%s\n�t�@�C��: %s\n�֐�: [%s] %d �s��\n", (srcLevel == 1) ? "[���݂̊K�w]" : "", string(info.source).empty() ? "(���ȃR�[�h)" : info.source, info.funcname, info.line);

		//���[�J���ϐ�
		verIndex = 0;
		auto verName = ::sq_getlocal(v, srcLevel, verIndex);
		while(verName != nullptr) {
			::sq_getstackobj(v, -1, &val);
			switch(::sq_gettype(v, -1)) {
				case OT_NULL:
					str += Game.Format("\t%s = null\n", verName);
					break;
				case OT_INTEGER:
					str += Game.Format("\t%s = %d\n", verName, val._unVal.nInteger);
					break;
				case OT_FLOAT:
					str += Game.Format("\t%s = %f\n", verName, val._unVal.fFloat);
					break;
				case OT_STRING:
					str += Game.Format("\t%s = %s\n", verName, ::sq_objtostring(&val));
					break;
				case OT_BOOL:
					str += Game.Format("\t%s = %s\n", verName, (::sq_objtobool(&val)) ? "true" : "false");
					break;
				case OT_INSTANCE:
					str += Game.Format("\t%s = (�C���X�^���X)\n", verName);
					break;
				case OT_ARRAY:
					str += Game.Format("\t%s = (�z��)\n", verName);
					break;
				case OT_CLASS:
					str += Game.Format("\t%s = (�N���X)\n", verName);
					break;
				case OT_CLOSURE:
					str += Game.Format("\t%s = (�N���[�W��)\n", verName);
					break;
				case OT_GENERATOR:
					str += Game.Format("\t%s = (�W�F�l���[�^�[)\n", verName);
					break;
				case OT_TABLE:
					str += Game.Format("\t%s = (�e�[�u��)\n", verName);
					break;
				case OT_THREAD:
					str += Game.Format("\t%s = (�X���b�h)\n", verName);
					break;
				case OT_USERDATA:
					str += Game.Format("\t%s = (���[�U�[�f�[�^�^)\n", verName);
					break;
				case OT_USERPOINTER:
					str += Game.Format("\t%s = (���[�U�[�|�C���^)\n", verName);
					break;
				default:
					str += Game.Format("\t%s = (���̑�)\n", verName);
					break;
			}
			verIndex++;
			verName = ::sq_getlocal(v, srcLevel, verIndex);
		}
		srcLevels.push_front(str);
	}
	for(int i = 1; srcLevels.empty() == false; i++) {
		TS_LOG("�ďo�K�w: %d %s", i, srcLevels.front().c_str());
		srcLevels.pop_front();
	}
	TS_LOG("----------------------------------------\n");
}


//####################  �����^�C���G���[�n���h���[�p�̃R�[���o�b�N�֐�  ####################
int errorHandler(HSQUIRRELVM v) {
	string errText = Sqrat::LastErrorString(v);
	SQStackInfos info;
	string fileName;

	CDebug::ActivateConsole();
	TS_LOG("�X�N���v�g���Ń����^�C���G���[���������Ă��܂��B\n");
	Game.NoTimeStamp = true;

	//�Ăяo���������o��
	::printSQCallStackAll(v);

	//�Ăяo���K�w���`�F�b�N
	if(SQ_SUCCEEDED(::sq_stackinfos(v, 1, &info))) {
		fileName = info.source;
	}

	//�G���[���e������
	TS_LOG("�G���[���e:");
	::dprintf_squirrel(v, errText.c_str());
	Game.NoTimeStamp = false;

	//������󂯕t����
	bool handled = false;
#ifdef _DEBUG
	const bool limitedDebug = Game.DB.getFDBSystem()->GetBoolean(TS::DB::System::EnabledLimitedDebug);
#else
	const bool limitedDebug = true;
#endif
	while(!handled) {
		if(limitedDebug) {
			//�ُ�I�����Ȃ�ׂ������邽�߂ɂ����ŃQ�[�����I������
			::printf("\n�X�N���v�g�f�o�b�K�[�͏����𑱍s�ł��܂���B�������͂��ĉ�����...\n\t[E]�v���O�������f (���O�o�͂Ȃ�)\n\t[L]�v���O�������f (���O�o�͂���)\n");
		} else {
			//�ُ�I�������Ƃ�Ȃ��ꍇ�ɍs���鑀����܂߂�
			::printf("\n�f�o�b�O���s���܂��B�������͂��ĉ�����...\n\t[E]�v���O�������f (���O�o�͂Ȃ�)\n\t[L]�v���O�������f (���O�o�͂���)\n\t[I]�G���[�𖳎����ď����𑱍s (�댯: ���X�N�����m������Ŏ��s���ĉ�����)\n\t[C]�C�ӂ̃R�[�h�����s (�댯: ���X�N�����m������Ŏ��s���ĉ�����)\n");
		}

		// NOTE: �����^�C���G���[���ɃR���p�C��������ƃX�^�b�N�������������߁A�������̕s���A�N�Z�X���������邽�ߔp�~�����B
		//::printf("\n�G���[�ӏ����C�����ĉ������B\n\t[E]�v���O�������f\n\t[O]�G���[�����������X�N���v�g���ăR���p�C�� (����)\n\t[A]���ׂẴX�N���v�g���ăR���p�C�� (�x��: �Ǎ������͏��s���̂��ߍX�Ȃ�s��������N�����\��������܂�)\n");
		//::printf("\n�G���[�ӏ����C�����ĉ������B\n\t[E]�v���O�������f\n\t[O]�G���[�����������X�N���v�g���ăR���p�C�� (����)\n\t[S]�V�[���X�N���v�g���ăR���p�C�� (����: �G���[�X�N���v�g���V�[���Ƃ͖��֌W�ł���ꍇ�͌��ʂ�����܂���)\n\t[A]���ׂẴX�N���v�g���ăR���p�C�� (�x��: �ǂݍ��ݏ����͏��s���̂��ߍX�Ȃ�s��������N�����\��������܂�)\n\t[I]�G���[�𖳎����đ��s (�x��: ��肪��������Ȃ��܂ܑ��s���邽�ߔ��Ɋ댯�ł�)\n");

		switch(CDebug::PauseConsole()) {
			case 'e':	//�v���O�������f: ���O�Ȃ�
				handled = true;
				Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
				break;

			case 'l':	//�v���O�������f: ���O����
				handled = true;
				Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Error));
				break;

				//case 'o':	//�P��R���p�C��
				//	if(!fileName.empty()) {
				//		handled = true;
				//		Game.SQ.Add(fileName);
				//	} else {
				//		::puts("�R���p�C���ł���X�N���v�g�ł͂���܂���B\n");
				//	}
				//	break;

				//case 's':	//�V�[���X�N���v�g�R���p�C��
				//	handled = true;
				//	Game.SQCompileCurrentScene();
				//	break;

				//case 'a':	//�S�X�N���v�g�R���p�C��
				//	handled = true;
				//	Game.SQ.RecompileAll();
				//	break;

			case 'i':	//�������đ��s
				if(limitedDebug) {
					break;		//�����t���f�o�b�O���[�h�ł͎��s�ł��Ȃ�
				}
				handled = true;
				break;

			case 'c':	//���ȃR�[�h���s
				if(limitedDebug) {
					break;		//�����t���f�o�b�O���[�h�ł͎��s�ł��Ȃ�
				}
				string code = Game.ScanConsole();
				if(code.empty()) {
					break;
				}
				if(code.substr(code.size() - 1, 1) == ";") {
					Sqrat::Script tester;
					try {
						::puts("");		//��s�����
						tester.CompileString(code);
						tester.Run();
						::puts("���ȃX�N���v�g�R�[�h�̎��s���������܂����B");
					} catch(...) {
						::puts("\ncmdError: �^����ꂽ�R�[�h�����s�ł��܂���B\n");
					}
				} else {
					::puts("cmdError: �R�[�h�̖����ɂ̓Z�~�R�������K�v�ł��B");
				}
				break;
		}
	}

	CDebug::ActivateGameWindow();
	return 0;
}


//####################  �f�o�b�O�p�̃R�[���o�b�N�֐��i�P�s���s���邽�тɌĂяo�����j  ####################
int debugHookFunc(HSQUIRRELVM v) {
	if(!Game.SQ.IsDebugging()) {
		return 0;						//�f�o�b�K�[���N�����Ă��Ȃ��Ƃ��͏������Ȃ�
	} else if(Game.SQ.KillDebuggerFlag) {
		Game.SQ.EscapeDebugger();		//�f�o�b�K�[�̏I�����v������Ă���Ƃ��͂������ɏI������
		return 0;
	}

	int srcLevel;
	SQStackInfos info;

	//�Ăяo���K�w���`�F�b�N
	for(srcLevel = 1; SQ_SUCCEEDED(::sq_stackinfos(v, srcLevel, &info)); srcLevel++) {}

	//�X�e�b�v�I�[�o�[/�A�E�g���̂Ƃ��A�w��K�w���[���Ȃ��Ă���Ƃ��͏������~���Ȃ�
	if(Game.SQ.StepDestinationLevel < srcLevel) {
		return 0;
	}

	//�Ăяo���������o��
	::printSQCallStackAll(v);
	Game.NoTimeStamp = false;

	//�������~����
	while(!Game.SQ.KillDebuggerFlag) {
		TS_EVERYLOOP_NOMT;
		//Game.CheckInput();
		::printf("\n[F5]�X�e�b�v���s�I��  [F10]�X�e�b�v�I�[�o�[  [F11]�X�e�b�v�C��  [F12]�X�e�b�v�A�E�g\n");
		const int key = CDebug::PauseConsole();

		switch(key) {
			case TS::System::SQDebugKey:
				//�f�o�b�K�[���I������
				Game.SQ.EscapeDebugger();
				TS_LOG("�X�N���v�g�̃X�e�b�v���s���I�����܂����B");
				*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) = 2;		//���̒���Ƀf�o�b�K�[���N�����Ȃ��悤�ɂ���
				return 0;

			case TS::System::SQDebugStepOverKey:
				//�X�e�b�v�I�[�o�[: ���̍s�̎��s�ŌĂяo���K�w���[�܂����ꍇ�͖߂�܂ŏ�����i�߂�
				TS_LOG("�X�e�b�v�I�[�o�[: ���̍s�����s���܂�...");
				Game.SQ.StepDestinationLevel = srcLevel;
				return 0;

			case TS::System::SQDebugStepInKey:
				//�X�e�b�v�C��: ���̍s�̎��s�ŌĂяo���K�w���[�܂��Ă������~�߂�
				TS_LOG("�X�e�b�v�C��: ���̍s�����s���܂�...");
				Game.SQ.StepDestinationLevel = INT_MAX;
				return 0;

			case TS::System::SQDebugStepOutKey:
				//�X�e�b�v�A�E�g: �f�o�b�K�[�͓������܂܂ɂ���
				if(srcLevel - 1 <= 0) {
					//����ȏ��̌Ăяo���K�w���Ȃ��ꍇ�̓X�e�b�v���s���I������
					TS_LOG("�X�e�b�v�A�E�g: ����ȏ���̃��x�����Ȃ����߁A�X�e�b�v���s���I�����܂����B");
					Game.SQ.EscapeDebugger();
				} else {
					//���̌Ăяo���K�w�ɂȂ�܂ŏ�����i�߂�
					TS_LOG("�X�e�b�v�A�E�g: ���̃��x���ɐi�ނ܂Ŏ��s���܂�...");
					Game.SQ.StepDestinationLevel = srcLevel - 1;
				}
				return 0;

				//�X�N���v�g���s���ɃG���[���N�����ꍇ�A���A�ł��Ȃ��Ȃ邽�ߔp�~
				//case 'c':
				//	//���ȃR�[�h���s
				//	string code = Game.ScanConsole();
				//	CDebug::ActivateConsole();
				//	if(code.empty()) {
				//		break;
				//	}
				//	if(code.substr(code.size() - 1, 1) == ";") {
				//		Sqrat::Script tester;
				//		try {
				//			::puts("");		//��s�����
				//			tester.CompileString(code);
				//			tester.Run();
				//			::puts("���ȃX�N���v�g�R�[�h�̎��s���������܂����B");
				//		} catch(...) {
				//			::puts("\ncmdError: �^����ꂽ�R�[�h�����s�ł��܂���B\n");
				//		}
				//	} else {
				//		::puts("cmdError: �R�[�h�̖����ɂ̓Z�~�R�������K�v�ł��B");
				//	}
				//	break;
		}
	}
	Game.SQ.EscapeDebugger();
	return 0;
}
