//###################################################################
//			汎用クラス定義の補足
//###################################################################
//NOTE: 前方宣言やゲームオブジェクトが関わる部分はヘッダーに処理を記述できないため、別途ソースファイルで定義する。
#include "Header.h"
char buf[TS::System::BufferLengthMax];


//####################  座標情報：コンストラクター  ####################
CPoint::CPoint(CSize size) {
	this->X = size.Width;
	this->Y = size.Height;
}


//####################  座標情報：文字列からセット  ####################
void CPoint::Set(string text) {
	vector<string> buf;
	Game.SplitString(buf, text, ":");
	if(buf.size() == 2) {
		return this->Set(atoi(buf[0].c_str()), atoi(buf[1].c_str()));
	}
}


//####################  座標情報：文字列にする  ####################
SQ_NOBIND string CPoint::ToString() {
	return Game.Format("%d:%d", this->X, this->Y);
}


//####################  サイズ情報：コンストラクター  ####################
CSize::CSize(CPoint pos) {
	this->Width = pos.X;
	this->Height = pos.Y;
}


//####################  サイズ情報：文字列からセット  ####################
void CSize::Set(string text) {
	vector<string> buf;
	Game.SplitString(buf, text, "x");
	if(buf.size() == 2) {
		return this->Set(atoi(buf[0].c_str()), atoi(buf[1].c_str()));
	}
}


//####################  サイズ情報：文字列にする  ####################
string CSize::ToString() {
	return Game.Format("%dx%d", this->Width, this->Height);
}


//####################  RGB色情報：セット  ####################
void CRGBColor::Set(int r, int g, int b) {
	r = Game.WithIn(r, 0, 255);
	g = Game.WithIn(g, 0, 255);
	b = Game.WithIn(b, 0, 255);
	this->Red = r;
	this->Green = g;
	this->Blue = b;
}


//####################  生データから情報をセット  ####################
void CSoundData::Load(string rawData) {
	vector<string> spl;
	Game.SplitString(spl, rawData, "|");
	if(spl.size() == 4) {
		this->Load(spl[0], atoi(spl[1].c_str()), atoi(spl[2].c_str()), atoi(spl[3].c_str()));
	} else {
		TS_LOG("与えられたサウンドデータ文字列が不正です。\n>%s", rawData.c_str());
	}
}


//####################  サウンド情報：情報をセット  ####################
void CSoundData::Load(string fileName, int vol, int pit, int pan) {
	//以前のデータを破棄する
	if(this->Handle != -1) {
		if (this->FileName == fileName && this->Volume == vol && this->Pitch == pit && this->Panpot == pan) {
			// 現在のデータに一致している場合はロードをスキップ
			return;
		}

		this->Dispose();
	}

	this->FileName = fileName;
	this->Volume = vol;			//実際の音量は再生時に自動調整される
	this->Pitch = Game.WithIn(pit, TS::Material::Common_MinPitch, TS::Material::Common_MaxPitch);
	this->Panpot = Game.WithIn(pan, TS::Material::MinPan, TS::Material::MaxPan);

	//NOTE: ピッチレートを頻繁に変更すると落ちる不具合あり
	if(strcmp(PathFindExtension(fileName.c_str()), ".mid") != 0) {
		//MIDI以外はピッチレートを読み込み前にセットする
		DxLib::SetCreateSoundPitchRate(Game.WithIn(5 * this->Pitch, TS::Material::MinPitch, TS::Material::MaxPitch));
	}
	this->Handle = Game.Material.getSounds()->Add(fileName);
	DxLib::SetCreateSoundPitchRate(0);
}


//####################  サウンド情報：再生  ####################
void CSoundData::Play(int playType, bool oncePlay) {
	if(playType == static_cast<int>(TS::Material::PlayType::Loop) || !oncePlay) {
		Game.Material.getSounds()->Play(this->Handle, playType, this->Volume, this->Pitch, this->Panpot);
	} else {
		Game.Material.getSounds()->PlayOnce(this->Handle, playType, this->Volume, this->Pitch, this->Panpot);
	}
}


//####################  サウンド情報：停止  ####################
void CSoundData::Stop() {
	Game.Material.getSounds()->Stop(this->Handle);
}


//####################  サウンド情報：一時停止  ####################
void CSoundData::Pause() {
	Game.Material.getSounds()->Pause(this->Handle);
}


//####################  サウンド情報：再開  ####################
void CSoundData::Resume(bool loopMode) {
	Game.Material.getSounds()->Resume(this->Handle, loopMode);
}


//####################  サウンド情報：破棄  ####################
void CSoundData::Dispose() {
	Game.Material.getSounds()->Delete(this->Handle);
	this->Handle = -1;
}


//####################  スクリプト互換配列（整数型）：取得  ####################
int CIntegerArray::Get(int index) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		return this->data[index];
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないインデックス [%d] でスクリプト互換配列（整数型）の要素が参照されました。", index);
		return 0;
	}
}


//####################  スクリプト互換配列（整数型）：設定  ####################
void CIntegerArray::Set(int index, int value) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		this->data[index] = value;
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないインデックス [%d] でスクリプト互換配列（整数型）の要素が参照されました。", index);
		return;
	}
}


//####################  スクリプト互換配列（整数型）：挿入  ####################
void CIntegerArray::Insert(int index, int value) {
	if(index == -1) {
		this->data.push_back(value);
	} else {
		index = Game.WithIn(index, 0, this->data.size() - 1);
		this->data.insert(this->data.begin() + index, value);
	}
}


//####################  スクリプト互換配列（整数型）：削除  ####################
void CIntegerArray::Delete(int index) {
	TS_CHECKINDEX(index, 0, static_cast<int>(this->data.size())) {
		this->data.erase(this->data.begin() + index);
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のインデックス [%d] でスクリプト互換配列（整数型）の要素を削除しようとしました。", index);
	}
}


//####################  スクリプト互換配列（整数型）：値検索  ####################
bool CIntegerArray::Exists(int value) {
	for(auto& val : this->data) {
		if(value == val) {
			return true;
		}
	}
	return false;
}


//####################  スクリプト互換配列（文字列型）：取得  ####################
string CStringArray::Get(int index) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		return this->data[index];
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないインデックス [%d] でスクリプト互換配列（文字列型）の要素が参照されました。", index);
		return "";
	}
}


//####################  スクリプト互換配列（文字列型）：設定  ####################
void CStringArray::Set(int index, string value) {
	TS_CHECKINDEX(index, 0, this->data.size()) {
		this->data[index] = value;
	} else {
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないインデックス [%d] でスクリプト互換配列（文字列型）の要素が参照されました。", index);
		return;
	}
}


//####################  スクリプト互換配列（文字列型）：挿入  ####################
void CStringArray::Insert(int index, string value) {
	if(index == -1) {
		this->data.push_back(value);
	} else {
		index = Game.WithIn(index, 0, this->data.size() - 1);
		this->data.insert(this->data.begin() + index, value);
	}
}


//####################  スクリプト互換配列（文字列型）：削除  ####################
void CStringArray::Delete(int index) {
	TS_CHECKINDEX(index, 0, static_cast<int>(this->data.size())) {
		this->data.erase(this->data.begin() + index);
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のインデックス [%d] でスクリプト互換配列（文字列型）の要素を削除しようとしました。", index);
	}
}


//####################  スクリプト互換配列（文字列型）：値検索  ####################
bool CStringArray::Exists(string value) {
	for(auto& val : this->data) {
		if(value == val) {
			return true;
		}
	}
	return false;
}
