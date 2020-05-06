
//#include "stdafx.h"
#include "GuruGuruSMF4_Cpp.h"


namespace GuruGuruSmf {
	
	// 関数ポインタの型定義
	typedef GGSERROR (WINAPI *FunctionOpenDevice)(int deviceId, HWND handle);
	typedef void (WINAPI *FunctionCloseDevice)();
	typedef GGSERROR (WINAPI *FunctionAddDlsW)(LPCWSTR fileName);
	typedef GGSERROR (WINAPI *FunctionAddDlsA)(LPCSTR fileName);
	typedef GGSERROR (WINAPI *FunctionPlay)(int option, int musicId, int seek, int fadeInTime, int keyShift);
	typedef void (WINAPI *FunctionStop)(int fadeOutTime);
	typedef void (WINAPI *FunctionPause)();
	typedef void (WINAPI *FunctionRestart)();
	typedef void (WINAPI *FunctionSetMasterTempo)(float tempo);
	typedef void (WINAPI *FunctionSetMasterVolume)(int volume);
	typedef void (WINAPI *FunctionSetMasterPitch)(int pitch);
	typedef void (WINAPI *FunctionGetPlayerStatus)(PlayerStatus* status, int size);
	typedef GGSERROR (WINAPI *FunctionAddListFromFileW)(LPCWSTR fileName, int option, int musicId);
	typedef GGSERROR (WINAPI *FunctionAddListFromFileA)(LPCSTR fileName, int option, int musicId);
	typedef GGSERROR (WINAPI *FunctionAddListFromMemory)(LPCBYTE smfBinary, int length, int option, int musicId);
	typedef void (WINAPI *FunctionDeleteListItem)(int musicId);
	typedef void (WINAPI *FunctionClearList)();
	typedef const SmfListKeys* (WINAPI *FunctionEnumList)();
	typedef GGSERROR (WINAPI *FunctionGetSmfInformation)(SmfInformation* info, int size, int musicId);
	
	// 関数ポインタ置き場
	static FunctionOpenDevice DllOpenDevice = 0;
	static FunctionCloseDevice DllCloseDevice = 0;
	static FunctionAddDlsW DllAddDlsW = 0;
	static FunctionAddDlsA DllAddDlsA = 0;
	static FunctionPlay DllPlay = 0;
	static FunctionStop DllStop = 0;
	static FunctionPause DllPause = 0;
	static FunctionRestart DllRestart = 0;
	static FunctionSetMasterTempo DllSetMasterTempo = 0;
	static FunctionSetMasterVolume DllSetMasterVolume = 0;
	static FunctionSetMasterPitch DllSetMasterPitch = 0;
	static FunctionGetPlayerStatus DllGetPlayerStatus = 0;
	static FunctionAddListFromFileW DllAddListFromFileW = 0;
	static FunctionAddListFromFileA DllAddListFromFileA = 0;
	static FunctionAddListFromMemory DllAddListFromMemory = 0;
	static FunctionDeleteListItem DllDeleteListItem = 0;
	static FunctionClearList DllClearList = 0;
	static FunctionEnumList DllEnumList = 0;
	static FunctionGetSmfInformation DllGetSmfInformation = 0;
	
	Ggs4* Ggs4::instance = 0;	// インスタンス置き場
	static HMODULE DllHandle = 0;	// DLLハンドル置き場
	
	// インスタンス取得(Singleton)
	Ggs4* Ggs4::GetInstance()
	{
		if(instance == 0){
			instance = new Ggs4();
		}
		return instance;
	}
	
	// インスタンス破棄
	void Ggs4::FreeInstance()
	{
		if(instance){
			delete instance;
			instance = 0;
		}
		if(DllHandle){
			FreeLibrary(DllHandle);
			DllHandle = 0;
		}
		
	}

	// 初期化
	GGSERROR Ggs4::Initialize()
	{
		if(DllHandle) return GgsError::NoError;	// 念のため
		
		// アプリケーションのフォルダを取得しフルパス合成(脆弱性対策)
		wchar_t Path[_MAX_PATH];
		wchar_t Drive[_MAX_DRIVE];
		wchar_t Dir[_MAX_DIR];
		GetModuleFileNameW(NULL, Path, sizeof(Path));
		_wsplitpath_s(Path, Drive, _MAX_DRIVE, Dir, _MAX_DIR, NULL, 0, NULL, 0);
		swprintf_s(Path, _MAX_PATH, L"%s%s%s", Drive, Dir, L"GuruGuruSMF4.dll");

		DllHandle = LoadLibraryW(Path);
		if(DllHandle == 0) return GgsError::Failed;
		
		/*	Ansi版・Windows9xをターゲットにする場合はこちらにしてみてください
		char Path[_MAX_PATH];
		char Drive[_MAX_DRIVE];
		char Dir[_MAX_DIR];
		GetModuleFileNameA(NULL, Path, sizeof(Path));
		_splitpath_s(Path, Drive, _MAX_DRIVE, Dir, _MAX_DIR, NULL, 0, NULL, 0);
		sprintf_s(Path, _MAX_PATH, "%s%s%s", Drive, Dir, "GuruGuruSMF4.dll");
		
		DllHandle = LoadLibraryA(Path);
		if(DllHandle == 0) return GgsError::Failed;
		*/
		
		DllOpenDevice = (FunctionOpenDevice)GetProcAddress(DllHandle, "GGS4OpenDevice");
		DllCloseDevice = (FunctionCloseDevice)GetProcAddress(DllHandle, "GGS4CloseDevice");
		DllAddDlsW = (FunctionAddDlsW)GetProcAddress(DllHandle, "GGS4AddDlsW");
		DllAddDlsA = (FunctionAddDlsA)GetProcAddress(DllHandle, "GGS4AddDlsA");
		DllPlay = (FunctionPlay)GetProcAddress(DllHandle, "GGS4Play");
		DllStop = (FunctionStop)GetProcAddress(DllHandle, "GGS4Stop");
		DllPause = (FunctionPause)GetProcAddress(DllHandle, "GGS4Pause");
		DllRestart = (FunctionRestart)GetProcAddress(DllHandle, "GGS4Restart");
		DllSetMasterTempo = (FunctionSetMasterTempo)GetProcAddress(DllHandle, "GGS4SetMasterTempo");
		DllSetMasterVolume = (FunctionSetMasterVolume)GetProcAddress(DllHandle, "GGS4SetMasterVolume");
		DllSetMasterPitch = (FunctionSetMasterPitch)GetProcAddress(DllHandle, "GGS4SetMasterPitch");
		DllGetPlayerStatus = (FunctionGetPlayerStatus)GetProcAddress(DllHandle, "GGS4GetPlayerStatus");
		DllAddListFromFileW = (FunctionAddListFromFileW)GetProcAddress(DllHandle, "GGS4AddListFromFileW");
		DllAddListFromFileA = (FunctionAddListFromFileA)GetProcAddress(DllHandle, "GGS4AddListFromFileA");
		DllAddListFromMemory = (FunctionAddListFromMemory)GetProcAddress(DllHandle, "GGS4AddListFromMemory");
		DllDeleteListItem = (FunctionDeleteListItem)GetProcAddress(DllHandle, "GGS4DeleteListItem");
		DllClearList = (FunctionClearList)GetProcAddress(DllHandle, "GGS4ClearList");
		DllEnumList = (FunctionEnumList)GetProcAddress(DllHandle, "GGS4EnumList");
		DllGetSmfInformation = (FunctionGetSmfInformation)GetProcAddress(DllHandle, "GGS4GetSmfInformation");
		
		return GgsError::NoError;
	}
	
	// デストラクタ
	Ggs4::~Ggs4()
	{
		CloseDevice();
	}
	
	// デバイスを開く
	GGSERROR Ggs4::OpenDevice(int deviceId, HWND handle)
	{
		if(!DllOpenDevice) return GgsError::ApiNull;
		return DllOpenDevice(deviceId, handle);
	}
	
	// デバイスを閉じる
	void Ggs4::CloseDevice()
	{
		if(!DllCloseDevice) return;
		DllCloseDevice();
	}
	
	// DLSを追加(UNICODE)
	GGSERROR Ggs4::AddDlsW(LPCWSTR fileName)
	{
		if(!DllAddDlsW) return GgsError::ApiNull;
		return DllAddDlsW(fileName);
	}
	
	// DLSを追加(Ansi)
	GGSERROR Ggs4::AddDlsA(LPCSTR fileName)
	{
		if(!DllAddDlsA) return GgsError::ApiNull;
		return DllAddDlsA(fileName);
	}
	
	// 演奏！
	GGSERROR Ggs4::Play(int option, int musicId, int seek, int fadeInTime, int keyShift)
	{
		if(!DllPlay) return GgsError::ApiNull;
		return DllPlay(option, musicId, seek, fadeInTime, keyShift);
	}
	
	// 停止
	void Ggs4::Stop(int fadeOutTime)
	{
		if(!DllStop) return;
		DllStop(fadeOutTime);
	}
	
	// 一時停止
	void Ggs4::Pause()
	{
		if(!DllPause) return;
		DllPause();
	}
	
	// 一時停止解除
	void Ggs4::Restart()
	{
		if(!DllRestart) return;
		DllRestart();
	}
	
	// マスターテンポ
	void Ggs4::SetMasterTempo(float tempo)
	{
		if(!DllSetMasterTempo) return;
		DllSetMasterTempo(tempo);
	}
	
	// マスターボリューム
	void Ggs4::SetMasterVolume(int volume)
	{
		if(!DllSetMasterVolume) return;
		DllSetMasterVolume(volume);
	}
	
	// マスターピッチ
	void Ggs4::SetMasterPitch(int pitch)
	{
		if(!DllSetMasterPitch) return;
		DllSetMasterPitch(pitch);
	}
	
	// 演奏情報取得
	void Ggs4::GetPlayerStatus(PlayerStatus* status)
	{
		if(!DllGetPlayerStatus) return;
		DllGetPlayerStatus(status, sizeof(PlayerStatus));
	}
	
	// ファイルからリストへ追加(UNICODE)
	GGSERROR Ggs4::AddListFromFileW(LPCWSTR fileName, int option, int musicId)
	{
		if(!DllAddListFromFileW) return GgsError::ApiNull;
		return DllAddListFromFileW(fileName, option, musicId);
	}
	
	// ファイルからリストへ追加(Ansi)
	GGSERROR Ggs4::AddListFromFileA(LPCSTR fileName, int option, int musicId)
	{
		if(!DllAddListFromFileA) return GgsError::ApiNull;
		return DllAddListFromFileA(fileName, option, musicId);
	}
	
	// メモリからリストへ追加
	GGSERROR Ggs4::AddListFromMemory(LPCBYTE smfBinary, int length, int option, int musicId)
	{
		if(!DllAddListFromMemory) return GgsError::ApiNull;
		return DllAddListFromMemory(smfBinary, length, option, musicId);
	}
	
	// リストから1つ削除
	void Ggs4::DeleteListItem(int musicId)
	{
		if(!DllDeleteListItem) return;
		DllDeleteListItem(musicId);
	}
	
	// リストをクリア
	void Ggs4::ClearList()
	{
		if(!DllClearList) return;
		DllClearList();
	}
	
	// リストを列挙
	const SmfListKeys* Ggs4::EnumList()
	{
		if(!DllEnumList) return 0;
		return DllEnumList();
	}
	
	// SMFの情報を取得
	GGSERROR Ggs4::GetSmfInformation(SmfInformation* info, int musicId)
	{
		if(!DllGetSmfInformation) return GgsError::ApiNull;
		return DllGetSmfInformation(info, sizeof(SmfInformation), musicId);
	}
	
}


