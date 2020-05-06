// GuruGuruSMF ver.4.0.0 �w�b�_(C++�X�^�C��)

#ifndef _GURUGURUSMF4H_
#define _GURUGURUSMF4H_

#include <stdio.h>
#include <windows.h>

namespace GuruGuruSmf {

	namespace PlayOption
	{
		const int Loop = 1;			// ���[�v�Đ�����
		const int SkipBeginning = 2;	// �`���̋󔒂��X�L�b�v
		const int SkipEnding = 4;		// �����̋󔒂��X�L�b�v
		const int MasterPitch = 8;		// �}�X�^�[�s�b�`��L���ɂ���
		const int Normalize = 16;		// �ő剹�ʂ�127�Ɉ����グ��
		const int Exclusive = 32;		// �G�N�X�N���[�V�u�𑗐M����
	}
	
	namespace LoadOption
	{
		const int Buffered = 1;		// �f�[�^������ɕێ����邩
	}
	
	namespace Device
	{
		const int DirectMusic = -1100;				// DirectMusic Port���[�h
	}
	
	namespace GgsError {
		enum Code {
			NoError, 			// �G���[�Ȃ�
			Failed,				// �Ȃ񂾂��悭�킩��Ȃ����ǎ��s
			FileNotFound,		// �t�@�C�������݂��Ȃ�
			CannotOpenFile,		// �t�@�C�����J���Ȃ�
			FileLoadFailed,		// �t�@�C���ǂݍ��݂Ɏ��s
			BadFileSize,		// �t�@�C���T�C�Y���s��
			NoSmf,				// SMF�ł͂���܂���
			BadSmf,				// �s����SMF
			UnmatchedSmf,		// ���Ή���SMF
			CannotOpenDevice,	// �f�o�C�X���J���Ȃ�
			CannotStartTimer,	// �^�C�}�[���J�n�ł��Ȃ�
			IllegalPointer,		// �͈͊O���Q�Ƃ��悤�Ƃ���
			TrackLimit,			// ���b�Z�[�W�̓r���Ńg���b�N���I�[�ɒB����
			NotReady,			// ���t�\�ł͂Ȃ�
			UndefinedId,		// ����`��ID
			ApiNull				// API���L���łȂ�
		};
	}
	typedef GgsError::Code GGSERROR;
	
	namespace PlayerState {
		enum Code {
			Stop,		// ��~��
			Play,		// ���t��
			Pause		// �ꎞ��~��
		};
	}
	typedef PlayerState::Code PLAYERSTATE;

	struct PlayerStatus
	{
		PLAYERSTATE State;
		int Tempo;
		int Tick;
		int Time;
		int LoopCount;
	};

	struct SmfInformation
	{
		int TimeBase;		// �x�[�X�J�E���g
		int TotalTick;		// ��Tick
		int TotalTime;		// �����t����
		int FirstNoteTick;	// �ŏ��̔���tick
		int FirstNoteTime;	// �ŏ��̔����̎���
		int LastNoteTick;	// �Ō�̔���tick
		int LastNoteTime;	// �Ō�̔����̎���
		int LoopTick;		// CC#111�̈ʒu
		int LoopTime;		// CC#111�̈ʒu�̎���
	};

	struct SmfListKeys
	{
		int Count;		// �L�[�̐�
		int* Keys;		// �L�[�̃��X�g
	};
	
	class Ggs4
	{
	private:
		static Ggs4* instance;
		
		Ggs4(){}
		Ggs4(const Ggs4&);
		Ggs4& operator =(const Ggs4&);
		~Ggs4();
		
	public:
		GGSERROR OpenDevice(int deviceId, HWND handle);
		void CloseDevice();
		GGSERROR AddDlsW(LPCWSTR fileName);
		GGSERROR AddDlsA(LPCSTR fileName);
		GGSERROR Play(int option, int musicId, int seek, int fadeInTime, int keyShift);
		void Stop(int fadeOutTime);
		void Pause();
		void Restart();
		void SetMasterTempo(float tempo);
		void SetMasterVolume(int volume);
		void SetMasterPitch(int pitch);
		void GetPlayerStatus(PlayerStatus* status);
		GGSERROR AddListFromFileW(LPCWSTR fileName, int option, int musicId);
		GGSERROR AddListFromFileA(LPCSTR fileName, int option, int musicId);
		GGSERROR AddListFromMemory(LPCBYTE smfBinary, int length, int option, int musicId);
		void DeleteListItem(int musicId);
		void ClearList();
		const SmfListKeys* EnumList();
		GGSERROR GetSmfInformation(SmfInformation* info, int musicId);

		static GGSERROR Initialize();
		static Ggs4* GetInstance();
		static void FreeInstance();
	};

}

#define GGS GuruGuruSmf::Ggs4::GetInstance()
#define GGSINITIALIZE GuruGuruSmf::Ggs4::Initialize
#define GGSFREE GuruGuruSmf::Ggs4::FreeInstance

#endif

