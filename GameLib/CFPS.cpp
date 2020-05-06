//###################################################################
//		フレームレート維持管理クラス
//###################################################################
#include "Header.h"


//####################  コンストラクター  ####################
CFPS::CFPS() {
	this->frame_time = (int)(1000.0f / TS::System::FixedFPS);		//１フレームの時間の計算
	this->wait_time = this->last_time = 0;
	this->tempSkipFrameCount = 0;
	this->nowSkipFrameCount = 0;

	//リフレッシュレートの調整準備
	if(::GetRefreshRate() > TS::System::FixedFPS) {
		DxLib::SetWaitVSyncFlag(false);
	}
}


//####################  固定フレームレートを変更  ####################
void CFPS::setSkipFrameCount(int val) {
	switch(val) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			this->fixedSkipFrameCount = val;
			break;

		default:
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定された固定フレームスキップ数 [%d] は対応していません。", val);
	}
}


//####################  １フレームを更新  ####################
//60FPSを維持するウェイトを挟み、更新すべきフレームであれば画面更新を行う
void CFPS::Update() {
	this->Wait();
	this->nowSkipFrameCount++;

#ifdef _DEBUG
	//リソース情報を描画
	if(Game.EnabledSystemResource) {
		TS_NOBLEND;
		Game.DrawSystemResourceInfo();
	}
#endif

	//フレームスキップは固定分しか考慮しない
	if(this->nowSkipFrameCount > this->fixedSkipFrameCount) {
	//if(this->nowSkipFrameCount > this->fixedSkipFrameCount && this->nowSkipFrameCount > this->tempSkipFrameCount) {
		//画面更新
		this->nowSkipFrameCount = 0;
		this->tempSkipFrameCount = 0;
		DxLib::ScreenFlip();
	}
}


//####################  処理が早すぎる場合に指定FPSになるようにウェイトする  ####################
void CFPS::Wait() {
	this->wait_time = this->frame_time - (::GetNowCount() - this->last_time);

	if(this->wait_time < 0 && this->tempSkipFrameCount == 0) {
		//低速環境: フレーム遅延しているのでフレームスキップすることを検討する
		this->tempSkipFrameCount = ceil(abs((double)this->wait_time) / (double)this->frame_time);

		//計算するたびにフレーム遅延が起きていた場合、以下を初期化してしまうと永遠にフレーム更新できなくなる
		//this->nowSkipFrameCount = 0;
	} else if(this->wait_time > 0) {
		//高速環境: 待ち時間があれば、指定時間待つ
		this->tempSkipFrameCount = 0;
		DxLib::WaitTimer(this->wait_time);
	}
	this->last_time = DxLib::GetNowCount();
}

