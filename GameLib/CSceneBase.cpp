//###################################################################
//			基底シーン定義
//###################################################################
#include "Header.h"
extern void RestoreTilePallet();


//####################  コンストラクター  ####################
CSceneBase::CSceneBase() {
	this->SubSceneID = -1;

	//グラフィック復元用コールバック関数を登録
	DxLib::SetRestoreGraphCallback(::RestoreTilePallet);
}


//####################  シーン描画  ####################
void CSceneBase::Draw() {
	if(this->SQHandle != -1) {
		if(this->SubSceneID == -1 || this->SQSubHandle != -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainDraw);
		} else {
			//メインシーンを置き換えてサブシーンに移行している
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDraw);
			return;
		}
	}
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDraw);
	}
}


//####################  シーン入力  ####################
void CSceneBase::Input() {
	//サブシーンが優先
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubInput);
	} else if(this->SQHandle != -1) {
		if(this->SubSceneID == -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainInput);
		} else {
			//メインシーンを置き換えてサブシーンに移行している
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubInput);
		}
	}
}


//####################  シーン生成時  ####################
void CSceneBase::Setup() {
	if(this->SQHandle != -1) {
		if(this->SubSceneID == -1 || this->SQSubHandle != -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainSetup);
		} else {
			//メインシーンを置き換えてサブシーンに移行している
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
			return;
		}
	}
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
	}
}


//####################  シーン破棄時  ####################
void CSceneBase::Dispose() {
	if(this->SQHandle != -1) {
		if(this->SubSceneID == -1 || this->SQSubHandle != -1) {
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneMainDispose);
		} else {
			//メインシーンを置き換えてサブシーンに移行している
			Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
			return;
		}
	}
	if(this->SQSubHandle != -1) {
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
	}
}
