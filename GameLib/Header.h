//###################################################################
//			���ׂẴt�@�C���͂�����C���N���[�h����
//###################################################################
#pragma once

//GameLib����\���ă����N���s��
#ifdef GAMELIB
//�f�o�b�O�E�����[�X����Ȃ����C�u����
#pragma comment(lib, "shlwapi.lib")					//�X�N���[���V���b�g�Ŏg�p (�t�@�C��������API)
#pragma comment(lib, "GuruGuruSMF4.lib")			//MIDI�Đ��p -> �ÓI�����N���Ă��邪�A���ۂɂ�DLL��ǂ݂ɍs���Ă��܂�
#pragma comment(lib, "imagehlp.lib")				//�f�B���N�g���쐬�Ŏg�p
#endif

//�f�o�b�O���̂�
#if defined(_DEBUG) && defined(GAMELIB)
#pragma comment(lib, "Debug\\squirrel.lib")
#pragma comment(lib, "Debug\\sqstdlib.lib")
#pragma comment(lib, "Debug\\zlib.lib")
#pragma comment(lib, "Debug\\cryptlib.lib")
#pragma comment(lib, "Psapi.lib")					//��������̎擾�Ŏg�p
#endif

//�����[�X���̂�
#if !defined(_DEBUG) && defined(GAMELIB)
#pragma comment(lib, "Release\\squirrel.lib")
#pragma comment(lib, "Release\\sqstdlib.lib")
#pragma comment(lib, "Release\\zlib.lib")
#pragma comment(lib, "Release\\cryptlib.lib")
#endif

//#define _CRT_SECURE_NO_WARNINGS					//�Â��Ĉ��S�łȂ��֐����g�p���邱�Ƃ�����
#pragma warning(disable:4297)						//��O���X���[���Ȃ��͂��Ȃ̂ɂ��Ă���
#pragma warning(disable:4996)						//vs::printf�̈��S���ɂ���
#pragma warning(disable:4482)						//�񋓑̂� :: �w��
#pragma warning(disable:4244)						//double ���� float �ւ̕ϊ��̓f�[�^��������\��������
#pragma warning(disable:4804)						//���Z���� bool �̎g�p���@�����S�ł͂Ȃ�
#pragma warning(disable:4805)						//���Z���� char �� bool �̍��݂͈��S�ł͂Ȃ�
#pragma warning(disable:4715)						//�l��Ԃ��Ȃ��R���g���[���p�X
#pragma warning(disable:4138)						//�n�܂�̂Ȃ��R�����g�̕�
#pragma warning(disable:4091)						//typedef �Ɋւ�����
#pragma warning(disable:4018)						//sign �� unsigned �̔�r

//�Í����֘A: crypto++
#include "cryptopp\\aes.h"
#include "cryptopp\\filters.h"
#include "cryptopp\\modes.h"
#include "cryptopp\\des.h"
#include "cryptopp\\hex.h"

//���k�𓀊֘A: zlib
#include "zlib\\zlib.h"
#define GZ_MODE_READ "rb"				//�ǂݍ��݁E�o�C�i��

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

#include <imagehlp.h>					//�f�B���N�g���쐬�Ŏg�p

#include "sqrat.h"
#include "DxLib.h"
#include "GuruGuruSMF4_Cpp.h"
#include "ColorDefines.h"

#ifdef _DEBUG
#include <Psapi.h>						//��������̎擾�Ŏg�p
#endif

using namespace std;


//�}�N����`�͂����ōs��
#define PI								(3.1415926535897932384626433832795)		//�~�����̒�`
#define TS_MT							CMultiThread::CThreadInfo& Data
#define TS_EVERYLOOP					while(DxLib::GetWindowMinSizeFlag() && DxLib::ProcessMessage() != -1) { /*�ŏ������͏������~�߂�*/ } Game.MT.Do(); Game.RegularFunc.Do(); Game.FPSKeeper.Update()
#define TS_EVERYLOOP_NOMT				while(DxLib::GetWindowMinSizeFlag() && DxLib::ProcessMessage() != -1) { /*�ŏ������͏������~�߂�*/ } Game.RegularFunc.Do(); Game.FPSKeeper.Update()						//MT�֐������s���Ȃ��o�[�W����
#define TS_CLS							DxLib::DrawBox(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, BLACK, true);
#define TS_COUNTLOOP(name, count)		for(int name = 0; name < (count); name++)
#define TS_MBI(Message)					if(DxLib::GetWindowModeFlag()) MessageBox(DxLib::GetMainWindowHandle(), TEXT(Message), TEXT("Debug"), MB_OK + MB_ICONINFORMATION)
#define TS_MBW(Message)					if(DxLib::GetWindowModeFlag()) MessageBox(DxLib::GetMainWindowHandle(), TEXT(Message), TEXT("Debug"), MB_OK + MB_ICONWARNING)
#define TS_MBE(Message)					MessageBox(::GetMainWindowHandle(), TEXT(Message), TEXT("�V�X�e�� �G���["), MB_OK + MB_ICONERROR)	//���̒���ɕ��ʂ͏I������������̂ŁA�E�B���h�E���[�h�̊m�F�͂킴�킴�s��Ȃ�
#define TS_WITHIN(val, _min, _max)		max(_min, min(val, _max))						//�͈͓��ɒ��������l��Ԃ�
#define TS_CHECKINDEX(val, _min, _max)	if(_min <= val && val < _max)					//�w��l���C���f�b�N�X�̗L���͈͓��ł��邩��Ԃ�
#define TS_CONTAINERALL(c)				(c).begin(), (c).end()

#define TS_BOOLSTR(v)					((v) ? "TRUE" : "FALSE")
#define TS_NOBLEND						DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1)
#define TS_ALPHABLEND(i)				DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, i)
#define TS_RAND(n)						Game.NextRand(0, n - 1)							//n�����̗����𔭐�������

#define TS_LOG_NOTIME(...)				Game.NoTimeStamp = true; Game.PrintLog(nullptr, nullptr, 0, "", __VA_ARGS__); Game.NoTimeStamp = false;	//�C�~�f�B�G�C�g�E�B���h�E�ɕ\�����邪�����͕\�����Ȃ�
#define TS_LOG(...)						Game.PrintLog(nullptr, nullptr, 0, "", __VA_ARGS__);			//�C�~�f�B�G�C�g�E�B���h�E�ɕ\�����邾��
#define TS_WARN(file, func, line, ...)	Game.PrintLog(_T(file), _T(func), line, "Warning ", __VA_ARGS__);		//�x���Ƃ��ăC�~�f�B�G�C�g�E�B���h�E�ɕ\�����邾���ŁA�v���C���[�ɂ͓`���Ȃ�
#define TS_ERROR(file, func, line, ...)	Game.PrintLog(_T(file), _T(func), line, "Error ", __VA_ARGS__); MessageBox(DxLib::GetMainWindowHandle(), TEXT("�����G���[�������������ߏI�����܂��B\n�ڍׂ͏o�͂��ꂽ���O�����Q�Ɖ������B"), TEXT("�V�X�e�� �G���["), MB_OK + MB_ICONERROR); Game.ApplicationExit((int)TS::System::ExitCode::Error);		//�C�~�f�B�G�C�g�E�B���h�E�ɏڍׂ�\�����A�v���C���[�ɂ̓G���[���N�������Ƃ�����`����

//�X�N���v�g�o�C���h�p�}�N��
#define SQ_NOBIND					//�o�C���h���Ȃ�
#define SQ_NOBIND_BEGIN				//�o�C���h�ΏۊO�̋�Ԏn�܂�
#define SQ_NOBIND_END				//�o�C���h�ΏۊO�̋�ԏI���
#define SQ_BEGINMEMBER				//�����o�[�ϐ��E�֐��̒�`�n�܂�
#define SQ_ABSTRACT					//���ۃN���X
#define SQ_CLASSVALUE				//�ÓI�����o�[�ϐ�
#define SQ_CLASSFUNC				//�ÓI���\�b�h
#define SQ_INSTANCEVALUE			//�C���X�^���X�ϐ�
#define SQ_INSTANCEFUNC				//�C���X�^���X���\�b�h
#define SQ_CONST					//�萔
#define SQ_ENUM						//�񋓑�
#define SQ_PROPGET					//�v���p�e�B��Getter
#define SQ_PROPSET					//�v���p�e�B��Setter
#define SQ_EVENTHANDLER(objectname, eventname, returntype/*�߂�l�̌^*/, .../*�������X�g*/)	const string objectname = eventname
#define SQ_EVENTHANDLER_NOBIND(objectname, eventname, returntype/*�߂�l�̌^*/, .../*�������X�g*/)	const string objectname = eventname

//�Ō�Ɋe��w�b�_�[��ǂݍ���
#include "CommonConstants.h"
#include "ConstantIntegers.h"
#include "ConstantStrings.h"
#include "General.h"
#include "CGameObject.h"


//�e�\�[�X���e�N���X�̃C���X�^���X�𐶐�����
#ifdef _WINMAIN
SQ_INSTANCEVALUE CGameObject Game;

//static�����o�[�ϐ��̎��̉�
LONGLONG CGameObject::MeasureProcTime;
bool CGameObject::NoTimeStamp = false;
deque<string> CGameObject::Log;
vector<CSceneMap::CEVSelfValueOneData> CSceneMap::SelfValueData;
#else
extern CGameObject Game;
#endif


//���������[�N�̌��o�@�\�ɕK�v�ȏ���
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

