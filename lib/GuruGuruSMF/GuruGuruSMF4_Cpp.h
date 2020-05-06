// GuruGuruSMF ver.4.0.0 ヘッダ(C++スタイル)

#ifndef _GURUGURUSMF4H_
#define _GURUGURUSMF4H_

#include <stdio.h>
#include <windows.h>

namespace GuruGuruSmf {

	namespace PlayOption
	{
		const int Loop = 1;			// ループ再生する
		const int SkipBeginning = 2;	// 冒頭の空白をスキップ
		const int SkipEnding = 4;		// 末尾の空白をスキップ
		const int MasterPitch = 8;		// マスターピッチを有効にする
		const int Normalize = 16;		// 最大音量を127に引き上げる
		const int Exclusive = 32;		// エクスクルーシブを送信する
	}
	
	namespace LoadOption
	{
		const int Buffered = 1;		// データを内部に保持するか
	}
	
	namespace Device
	{
		const int DirectMusic = -1100;				// DirectMusic Portモード
	}
	
	namespace GgsError {
		enum Code {
			NoError, 			// エラーなし
			Failed,				// なんだかよくわからないけど失敗
			FileNotFound,		// ファイルが存在しない
			CannotOpenFile,		// ファイルが開けない
			FileLoadFailed,		// ファイル読み込みに失敗
			BadFileSize,		// ファイルサイズが不正
			NoSmf,				// SMFではありません
			BadSmf,				// 不正なSMF
			UnmatchedSmf,		// 未対応のSMF
			CannotOpenDevice,	// デバイスが開けない
			CannotStartTimer,	// タイマーが開始できない
			IllegalPointer,		// 範囲外を参照しようとした
			TrackLimit,			// メッセージの途中でトラックが終端に達した
			NotReady,			// 演奏可能ではない
			UndefinedId,		// 未定義のID
			ApiNull				// APIが有効でない
		};
	}
	typedef GgsError::Code GGSERROR;
	
	namespace PlayerState {
		enum Code {
			Stop,		// 停止中
			Play,		// 演奏中
			Pause		// 一時停止中
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
		int TimeBase;		// ベースカウント
		int TotalTick;		// 総Tick
		int TotalTime;		// 総演奏時間
		int FirstNoteTick;	// 最初の発音tick
		int FirstNoteTime;	// 最初の発音の時間
		int LastNoteTick;	// 最後の発音tick
		int LastNoteTime;	// 最後の発音の時間
		int LoopTick;		// CC#111の位置
		int LoopTime;		// CC#111の位置の時間
	};

	struct SmfListKeys
	{
		int Count;		// キーの数
		int* Keys;		// キーのリスト
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

