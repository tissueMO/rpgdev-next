//###################################################################
//			すべてのファイルはこれをインクルードする
//###################################################################
#pragma once

//GameLibが代表してリンクを行う
#ifdef GAMELIB
//デバッグ・リリースを問わないライブラリ
#pragma comment(lib, "shlwapi.lib")					//スクリーンショットで使用 (ファイル処理のAPI)
#pragma comment(lib, "GuruGuruSMF4.lib")			//MIDI再生用 -> 静的リンクしているが、実際にはDLLを読みに行ってしまう
#pragma comment(lib, "imagehlp.lib")				//ディレクトリ作成で使用
#endif

//デバッグ時のみ
#if defined(_DEBUG) && defined(GAMELIB)
#pragma comment(lib, "Debug\\squirrel.lib")
#pragma comment(lib, "Debug\\sqstdlib.lib")
#pragma comment(lib, "Debug\\zlib.lib")
#pragma comment(lib, "Debug\\cryptlib.lib")
#pragma comment(lib, "Psapi.lib")					//消費メモリの取得で使用
#endif

//リリース時のみ
#if !defined(_DEBUG) && defined(GAMELIB)
#pragma comment(lib, "Release\\squirrel.lib")
#pragma comment(lib, "Release\\sqstdlib.lib")
#pragma comment(lib, "Release\\zlib.lib")
#pragma comment(lib, "Release\\cryptlib.lib")
#endif

//#define _CRT_SECURE_NO_WARNINGS					//古くて安全でない関数を使用することがある
#pragma warning(disable:4297)						//例外をスローしないはずなのにしている
#pragma warning(disable:4996)						//vs::printfの安全性について
#pragma warning(disable:4482)						//列挙体の :: 指定
#pragma warning(disable:4244)						//double から float への変換はデータが失われる可能性がある
#pragma warning(disable:4804)						//演算中の bool の使用方法が安全ではない
#pragma warning(disable:4805)						//演算中の char と bool の混在は安全ではない
#pragma warning(disable:4715)						//値を返さないコントロールパス
#pragma warning(disable:4138)						//始まりのないコメントの閉じ
#pragma warning(disable:4091)						//typedef に関する問題
#pragma warning(disable:4018)						//sign と unsigned の比較

//暗号化関連: crypto++
#include "cryptopp\\aes.h"
#include "cryptopp\\filters.h"
#include "cryptopp\\modes.h"
#include "cryptopp\\des.h"
#include "cryptopp\\hex.h"

//圧縮解凍関連: zlib
#include "zlib\\zlib.h"
#define GZ_MODE_READ "rb"				//読み込み・バイナリ

#include <string>
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <algorithm>
#include <iostream>
#include <sstream>

#include <stdarg.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <time.h>
#include <math.h>

#include <imagehlp.h>					//ディレクトリ作成で使用

#include "sqrat.h"
#include "DxLib.h"
#include "GuruGuruSMF4_Cpp.h"
#include "ColorDefines.h"

#ifdef _DEBUG
#include <Psapi.h>						//消費メモリの取得で使用
#endif

using namespace std;


//マクロ定義はここで行う
#define PI								(3.1415926535897932384626433832795)		//円周率の定義
#define TS_MT							CMultiThread::CThreadInfo& Data
#define TS_EVERYLOOP					while(DxLib::GetWindowMinSizeFlag() && DxLib::ProcessMessage() != -1) { /*最小化時は処理を止める*/ } Game.MT.Do(); Game.RegularFunc.Do(); Game.FPSKeeper.Update()
#define TS_EVERYLOOP_NOMT				while(DxLib::GetWindowMinSizeFlag() && DxLib::ProcessMessage() != -1) { /*最小化時は処理を止める*/ } Game.RegularFunc.Do(); Game.FPSKeeper.Update()						//MT関数を実行しないバージョン
#define TS_CLS							DxLib::DrawBox(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, BLACK, true);
#define TS_COUNTLOOP(name, count)		for(int name = 0; name < (count); name++)
#define TS_MBI(Message)					if(DxLib::GetWindowModeFlag()) MessageBox(DxLib::GetMainWindowHandle(), TEXT(Message), TEXT("Debug"), MB_OK + MB_ICONINFORMATION)
#define TS_MBW(Message)					if(DxLib::GetWindowModeFlag()) MessageBox(DxLib::GetMainWindowHandle(), TEXT(Message), TEXT("Debug"), MB_OK + MB_ICONWARNING)
#define TS_MBE(Message)					MessageBox(::GetMainWindowHandle(), TEXT(Message), TEXT("システム エラー"), MB_OK + MB_ICONERROR)	//この直後に普通は終了処理が入るので、ウィンドウモードの確認はわざわざ行わない
#define TS_WITHIN(val, _min, _max)		max(_min, min(val, _max))						//範囲内に調整した値を返す
#define TS_CHECKINDEX(val, _min, _max)	if(_min <= val && val < _max)					//指定値がインデックスの有効範囲内であるかを返す
#define TS_CONTAINERALL(c)				(c).begin(), (c).end()

#define TS_BOOLSTR(v)					((v) ? "TRUE" : "FALSE")
#define TS_NOBLEND						DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1)
#define TS_ALPHABLEND(i)				DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, i)
#define TS_RAND(n)						Game.NextRand(0, n - 1)							//n未満の乱数を発生させる

#define TS_LOG_NOTIME(...)				Game.NoTimeStamp = true; Game.PrintLog(nullptr, nullptr, 0, "", __VA_ARGS__); Game.NoTimeStamp = false;	//イミディエイトウィンドウに表示するが時刻は表示しない
#define TS_LOG(...)						Game.PrintLog(nullptr, nullptr, 0, "", __VA_ARGS__);			//イミディエイトウィンドウに表示するだけ
#define TS_WARN(file, func, line, ...)	Game.PrintLog(_T(file), _T(func), line, "Warning ", __VA_ARGS__);		//警告としてイミディエイトウィンドウに表示するだけで、プレイヤーには伝わらない
#define TS_ERROR(file, func, line, ...)	Game.PrintLog(_T(file), _T(func), line, "Error ", __VA_ARGS__); MessageBox(DxLib::GetMainWindowHandle(), TEXT("内部エラーが発生したため終了します。\n詳細は出力されたログをご参照下さい。"), TEXT("システム エラー"), MB_OK + MB_ICONERROR); Game.ApplicationExit((int)TS::System::ExitCode::Error);		//イミディエイトウィンドウに詳細を表示しつつ、プレイヤーにはエラーが起きたことだけを伝える

//スクリプトバインド用マクロ
#define SQ_NOBIND					//バインドしない
#define SQ_NOBIND_BEGIN				//バインド対象外の区間始まり
#define SQ_NOBIND_END				//バインド対象外の区間終わり
#define SQ_BEGINMEMBER				//メンバー変数・関数の定義始まり
#define SQ_ABSTRACT					//抽象クラス
#define SQ_CLASSVALUE				//静的メンバー変数
#define SQ_CLASSFUNC				//静的メソッド
#define SQ_INSTANCEVALUE			//インスタンス変数
#define SQ_INSTANCEFUNC				//インスタンスメソッド
#define SQ_CONST					//定数
#define SQ_ENUM						//列挙体
#define SQ_PROPGET					//プロパティのGetter
#define SQ_PROPSET					//プロパティのSetter
#define SQ_EVENTHANDLER(objectname, eventname, returntype/*戻り値の型*/, .../*引数リスト*/)	const string objectname = eventname
#define SQ_EVENTHANDLER_NOBIND(objectname, eventname, returntype/*戻り値の型*/, .../*引数リスト*/)	const string objectname = eventname

//最後に各種ヘッダーを読み込む
#include "CommonConstants.h"
#include "ConstantIntegers.h"
#include "ConstantStrings.h"
#include "General.h"
#include "CGameObject.h"


//親ソースが各クラスのインスタンスを生成する
#ifdef _WINMAIN
SQ_INSTANCEVALUE CGameObject Game;

//staticメンバー変数の実体化
LONGLONG CGameObject::MeasureProcTime;
bool CGameObject::NoTimeStamp = false;
deque<string> CGameObject::Log;
vector<CSceneMap::CEVSelfValueOneData> CSceneMap::SelfValueData;
#else
extern CGameObject Game;
#endif


//メモリリークの検出機能に必要な準備
#if _DEBUG
#ifndef DISENABLEDMEMCHECK
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new					new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s)			::_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #define free(s)				::_free_dbg(s, _NORMAL_BLOCK)
#define calloc(c, s)		::_calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)		::_realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _recalloc(p, c, s)	::_recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s)		::_expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif

