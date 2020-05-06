//###################################################################
//		ゲームオブジェクト
//###################################################################
#include "CDXLibWrapper.h"
#include "ClassMaterials.h"
#include "ClassDatabases.h"
#include "ClassScenes.h"
#include "ClassSub.h"
#include "CSaveLoad.h"
#include "CScript.h"
#include "CSprite.h"


class CGameObject {
public:
	//SQ_BEGINMEMBER
	//コンストラクター
	SQ_NOBIND CGameObject();

	//####################  メンバーインスタンス  ####################
	SQ_NOBIND CRegularFunc RegularFunc;
	SQ_NOBIND CScript SQ;

	SQ_NOBIND CMultiThread MT;
	SQ_PROPGET CMultiThread* getMT() { return &this->MT; }

	SQ_NOBIND CFPS FPSKeeper;
	SQ_PROPGET CFPS* getFPSKeeper() { return &this->FPSKeeper; }

	SQ_NOBIND CDXLibWrapper DXLib;
	SQ_PROPGET CDXLibWrapper* getDXLib() { return &this->DXLib; }

	SQ_NOBIND CDatabase DB;
	SQ_PROPGET CDatabase* getDB() { return &this->DB; }

	SQ_NOBIND CSprite Sprite;
	SQ_PROPGET CSprite* getSprite() { return &this->Sprite; }

	SQ_NOBIND CSaveLoad SaveLoad;
	SQ_PROPGET CSaveLoad* getSaveLoad() { return &this->SaveLoad; }

	SQ_NOBIND CMaterial Material;
	SQ_PROPGET CMaterial* getMaterial() { return &this->Material; }


	//####################  メンバー変数  ####################
	SQ_NOBIND CPoint ConsolePosition;
	SQ_NOBIND bool EnabledSystemResource;
	SQ_NOBIND bool Pausing = false;
	SQ_INSTANCEVALUE bool ForbidSceneDraw = false;
	SQ_INSTANCEVALUE bool ForbidSceneInput = false;
	SQ_INSTANCEVALUE bool BGMKeep = false;
	SQ_INSTANCEVALUE bool BGSKeep = false;

	SQ_NOBIND string CurrentPath;									//現在のプログラムが含まれるフォルダーに至るフルパス
	SQ_PROPGET string getCurrentPath() { return this->CurrentPath; }

	SQ_NOBIND int RunMode;
	SQ_PROPGET int getRunMode() { return this->RunMode; }

	SQ_NOBIND CSize WindowSize;
	SQ_PROPGET CSize* getWindowSize() { return &this->WindowSize; }

	SQ_NOBIND CRGBColor DrawBright;
	SQ_PROPGET CRGBColor* getDrawBright() { return &this->DrawBright; }


	//####################  クラス変数  ####################
	SQ_NOBIND static LONGLONG MeasureProcTime;						//処理時間の計測用内部カウンター
	SQ_NOBIND static bool NoTimeStamp;								//デバッグログに時刻情報を付加するかどうかのフラグ
	SQ_NOBIND static deque<string> Log;								//デバッグ用：イミディエイトウィンドウに出力されたエラーテキストのログ


	//####################  メソッド  ####################
	//インスタンスメソッド
	SQ_INSTANCEFUNC bool Wait(double timeSecond, bool enabledMT, bool keyUpBreak);
	SQ_INSTANCEFUNC void WaitKey(bool enabledMT);
	SQ_INSTANCEFUNC bool IsAnyKeyDown();
	SQ_NOBIND int* GetKeyInputCounterPTR(int keyID);
	SQ_INSTANCEFUNC int GetKeyInputCounter(int keyID);
	SQ_INSTANCEFUNC void SetKeyInputCounter(int keyID, int val);
	SQ_INSTANCEFUNC void PrepareTransition();
	SQ_INSTANCEFUNC void DoTransition(string blendFileName, int speed);
	SQ_INSTANCEFUNC void FadeOut(int speed);
	SQ_INSTANCEFUNC void FadeIn(int speed);
	SQ_NOBIND void SaveWindowState();
	SQ_NOBIND void LoadWindowState();
	SQ_NOBIND void ChangeScene(CSceneBase* nextScene, TS::System::Scene sceneID);
	SQ_NOBIND void DisposeScene();
	SQ_NOBIND void CheckInput();
	SQ_INSTANCEFUNC void StartSubScene(int sceneID, bool keepMainScene, bool waitMode);
	SQ_INSTANCEFUNC void EndSubScene();
	SQ_INSTANCEFUNC bool IsSubScene();
	SQ_INSTANCEFUNC void ChangeToTitleScene();
	SQ_INSTANCEFUNC void ChangeToMapScene();
	SQ_INSTANCEFUNC int GetCurrentMainSceneID();
	SQ_INSTANCEFUNC int GetCurrentSubSceneID();
	SQ_INSTANCEFUNC bool CheckCurrentSceneID(int sceneID);
	SQ_INSTANCEFUNC void DoSceneDraw();
	SQ_INSTANCEFUNC void DoSceneInput();
	SQ_NOBIND void SQCompileCurrentScene();
	SQ_PROPGET CSceneMap* getSceneMap();
	SQ_PROPGET CSceneTitle* getSceneTitle();
	SQ_INSTANCEFUNC string GetCurrentSceneName();
	SQ_INSTANCEFUNC void SQInclude(string fileName);
	SQ_INSTANCEFUNC void SQBreakPoint();
	SQ_INSTANCEFUNC void SQBreakResume();
	SQ_NOBIND void DrawSystemResourceInfo();

	//クラスメソッド
	SQ_CLASSFUNC static int MsgBox(string msg, string caption, int iconID, int buttonID);
	SQ_NOBIND static void PrintLog(LPCTSTR fileName, LPCTSTR funcName, int lineNum, string header, string fmt, ...);
	SQ_NOBIND static void RaiseWarning(LPCTSTR fileName, LPCTSTR funcName, int lineNum, TS::System::ErrorID id, string fmt, ...);
	SQ_NOBIND static void RaiseError(LPCTSTR fileName, LPCTSTR funcName, int lineNum, TS::System::ErrorID id, string fmt, ...);
	SQ_NOBIND static void PrintAPIError();
	SQ_NOBIND static void WriteDownLog();
	SQ_CLASSFUNC static string ScanConsole();
	SQ_CLASSFUNC static void PrintLog(string text);
	SQ_CLASSFUNC static void ApplicationExit(int code = static_cast<int>(TS::System::ExitCode::Error));

	SQ_CLASSFUNC static void StartMeasureProcTime();
	SQ_CLASSFUNC static void ContinueMeasureProcTime(string tag);
	SQ_CLASSFUNC static void EndMeasureProcTime(string tag);

	SQ_CLASSFUNC static int WithIn(int val, int _min, int _max);
	SQ_CLASSFUNC static int UpAround(int val, int _min, int _max);
	SQ_CLASSFUNC static int DownAround(int val, int _min, int _max);
	SQ_CLASSFUNC static bool IntToBool(int value);
	SQ_CLASSFUNC static int BoolToInt(bool value);
	SQ_CLASSFUNC static int NextRand(int min, int max);

	SQ_NOBIND static void CheckDirectory(string path);
	SQ_NOBIND static void GetFiles(vector<string>& dest, string folder, string filter = "*.*", bool includeSubFolder = false);
	SQ_NOBIND static void SplitString(vector<string>& dest, const string& src, const string& delim);
	SQ_NOBIND static string UTF8ToSJIS(string srcUTF8);
	SQ_NOBIND static string PopHead(vector<string>& src);

	SQ_NOBIND static string NowTimeToString(string format);
	SQ_NOBIND static string Format(string text, ...);
	SQ_CLASSFUNC static string CutDQ(string baseString);
	SQ_CLASSFUNC static string IntToAlphabet(int value);
	SQ_CLASSFUNC static string SubString(string src, int start, int length);
	SQ_CLASSFUNC static string Replace(string baseString, string before, string after);
	SQ_CLASSFUNC static string Insert(string dest, int index, string text);
	SQ_CLASSFUNC static int GetStringLength(string src);

private:
	//メンバー定数
	std::array<int, 2> ignoreKeyCodes = {
		0x70/*ひらかなカタカナキー*/,
		0x94/*半角全角キー*/
	};    //キー入力の一括チェックで無視するキーコードを表すリスト

	//メンバー変数
	std::array<int, MAXBYTE + 1> keyInputData;					//各キーが継続的にどれだけ押されているかを示す
	CSceneBase* currentScene = nullptr;
	int currentSceneID;
	int screenHandleForTransition;
	int frameCounter;
	int lastTimeCount;
	double nowFPS;
	double maxRAM;
	double maxSwap;

	//プライベートメソッド
	void initKeyInputData();
	wstring CWString(string const &str, int codeMulti = CP_ACP);
	string CString(wstring const &wstr, int codeMulti = CP_ACP);
};
