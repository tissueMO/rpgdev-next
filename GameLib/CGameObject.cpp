//###################################################################
//			汎用処理群
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];
extern void printSQCallStackAll(HSQUIRRELVM& v);
void SQDebuggerKiller();


//####################  コンストラクター  ####################
CGameObject::CGameObject() {
	//各種オブジェクトを初期化する
	this->initKeyInputData();
	this->DrawBright.Set(255, 255, 255);

	//素材管理クラスのインスタンスをセットする
	TS_COUNTLOOP(i, static_cast<int>(TS::Material::MaterialType::Count)) {
		switch(static_cast<TS::Material::MaterialType>(i)) {
			case TS::Material::MaterialType::Graphics:
				this->Material.Types[i] = new CMaterial::CMaterialGraphics();
				break;
			case TS::Material::MaterialType::Sound:
				this->Material.Types[i] = new CMaterial::CMaterialSound();
				break;
			case TS::Material::MaterialType::Font:
				this->Material.Types[i] = new CMaterial::CMaterialFont();
				break;
		}
	}
}


//####################  現在の値を範囲内に調整する  ####################
int CGameObject::WithIn(int val, int _min, int _max) {
	if(_min > _max) {
		std::swap(_min, _max);		//大小関係が逆の場合は入れ替える
	}
	return TS_WITHIN(val, _min, _max);
}


//####################  数値のループカウンティング・増分  ####################
int CGameObject::UpAround(int val, int _min, int _max) {
	if(_min > _max) {
		std::swap(_min, _max);		//大小関係が逆の場合は入れ替える
	}
	const int temp = val + 1;
	return (temp < _max) ? temp : _min;
}


//####################  数値のループカウンティング・差分  ####################
int CGameObject::DownAround(int val, int _min, int _max) {
	if(_min > _max) {
		std::swap(_min, _max);		//大小関係が逆の場合は入れ替える
	}
	const int temp = val - 1;
	return (_min <= temp) ? temp : _max - 1;
}


//####################  整数を論理値に変換  ####################
bool CGameObject::IntToBool(int value) {
	return (value == FALSE) ? false : true;
}


//####################  論理値を整数に変換  ####################
int CGameObject::BoolToInt(bool value) {
	return value ? TRUE : FALSE;
}


//####################  最小値と最大値を指定して乱数を得る  ####################
int CGameObject::NextRand(int min, int max) {
	return min + GetRand(max - min);
}


//####################  シーンを遷移する  ####################
void CGameObject::ChangeScene(CSceneBase* nextScene, TS::System::Scene sceneID) {
	//前のシーンを破棄する
	this->DisposeScene();

	//MT関数はすべて破棄する
	Game.MT.Clear(false);

	//シーンを遷移させて素材を読み込む
	this->currentSceneID = static_cast<int>(sceneID);
	if(nextScene != nullptr) {
		TS_LOG("シーン [%s] へ移行します。", nextScene->GetName().c_str());
		this->initKeyInputData();
		this->currentScene = nextScene;
		this->currentScene->StartTime = DxLib::GetNowCount();
		this->currentScene->SQHandle = Game.SQ.Add(TS::FileNames::SQScenes[this->currentSceneID]);
		this->currentScene->SQSubHandle = -1;
		this->currentScene->Setup();
	}
	this->SQ.CollectGarbage();
}


//####################  現在のシーンを破棄する  ####################
void CGameObject::DisposeScene() {
	if(this->currentScene != nullptr) {
		TS_LOG("現在のシーン [%s] を破棄します。", this->currentScene->GetName().c_str());
		this->currentScene->BGM.Dispose();
		this->currentScene->BGS.Dispose();
		this->currentScene->Dispose();
		delete this->currentScene;
	}
}


//####################  サブシーンを遷移する  ####################
void CGameObject::StartSubScene(int sceneID, bool keepMainScene, bool waitMode) {
	sceneID = Game.WithIn(sceneID, 0, static_cast<int>(TS::System::Scene::Count) - 1);

	//前のサブシーンを破棄する
	if(this->currentScene->SQSubHandle != -1) {
		TS_LOG("現在のサブシーン [ID: %d] を破棄します。", this->currentSceneID);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
		//Game.SQ.Delete(this->currentScene->SQSubHandle);
	}

	//サブシーン移行に際して初期化を行う
	TS_LOG("サブシーン [ID: %d] へ移行します。", sceneID);
	this->currentScene->SubSceneID = sceneID;

	//キー押下状態は継続したままにしないと、シーン移行した直後に入力が誤作動してしまう
	//this->initKeyInputData();

	if(keepMainScene) {
		//メインシーンを維持しながらサブシーンへ移行する
		this->currentScene->SQSubHandle = Game.SQ.Add(TS::FileNames::SQScenes[sceneID]);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
	} else {
		//現在のシーンスクリプトを破棄して置き換える: メインシーン自体がなくなるわけではないので、シーン自体の破棄処理は行わない
		//Game.SQ.Delete(this->currentScene->SQHandle);
		this->currentScene->SQHandle = Game.SQ.Add(TS::FileNames::SQScenes[sceneID]);
		this->currentScene->SQSubHandle = -1;
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubSetup);
	}
	this->SQ.CollectGarbage();

	//待機するモードの場合はこの中でメインループを構える: シーンIDが変更されるまで抜けない
	if(waitMode) {
		while(this->GetCurrentSubSceneID() == sceneID) {
			Game.DoSceneInput();
			Game.DoSceneDraw();
			TS_EVERYLOOP;
		}
	}
}


//####################  サブシーンを終了してメインシーンに戻る  ####################
void CGameObject::EndSubScene() {
	if(this->currentScene->SubSceneID == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在サブシーンには移行していません。");
		return;
	}
	TS_LOG("サブシーン [ID: %d] を終了します。", this->currentScene->SubSceneID);
	if(this->currentScene->SQSubHandle == -1) {
		//メインシーンを置き換える形でサブシーンに移行した場合: メインシーンのスクリプトを再度読み直す
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
		//Game.SQ.Delete(this->currentScene->SQHandle);
		this->currentScene->SQHandle = Game.SQ.Add(TS::FileNames::SQScenes[this->currentSceneID]);
	} else {
		//メインシーンを維持しながらサブシーンに移行した場合
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_SceneSubDispose);
		//Game.SQ.Delete(this->currentScene->SQSubHandle);
		this->currentScene->SQSubHandle = -1;
	}
	this->SQ.CollectGarbage();
	this->currentScene->SubSceneID = -1;
}


//####################  現在サブシーンが有効であるかどうかを取得する  ####################
bool CGameObject::IsSubScene() {
	return (this->currentScene != nullptr && this->currentScene->SubSceneID != -1);
}


//####################  現在のメインシーンIDを取得する  ####################
int CGameObject::GetCurrentMainSceneID() {
	if(this->currentScene != nullptr) {
		return this->currentSceneID;
	}
	return -1;
}


//####################  現在のサブシーンIDを取得する  ####################
int CGameObject::GetCurrentSubSceneID() {
	if(this->currentScene != nullptr) {
		return this->currentScene->SubSceneID;
	}
	return -1;
}


//####################  指定したシーンが現在のシーンと一致するかどうかを検証する  ####################
//メインシーンを維持しながらサブシーンに移行した場合、サブシーンでのみ一致する
bool CGameObject::CheckCurrentSceneID(int sceneID) {
	if(this->GetCurrentSubSceneID() == -1) {
		//現在メインシーンのとき、以下のシーンのみ認める
		switch(static_cast<TS::System::Scene>(this->GetCurrentMainSceneID())) {
			case TS::System::Scene::Title:
			case TS::System::Scene::Map:
				return (this->GetCurrentMainSceneID() == sceneID);
		}
		return false;
	} else {
		//現在サブシーンのとき、以下のシーンを認めない
		switch(static_cast<TS::System::Scene>(sceneID)) {
			case TS::System::Scene::Title:
			case TS::System::Scene::Map:
				return false;
		}
		return (this->GetCurrentSubSceneID() == sceneID);
	}
}


//####################  タイトルシーンへ移動する  ####################
void CGameObject::ChangeToTitleScene() {
	this->ChangeScene(new CSceneTitle(), TS::System::Scene::Title);
}


//####################  マップシーンへ移動する  ####################
void CGameObject::ChangeToMapScene() {
	this->ChangeScene(new CSceneMap(), TS::System::Scene::Map);
}


//####################  現在のシーンの描画処理を行う  ####################
void CGameObject::DoSceneDraw() {
	//画面をクリア
	TS_NOBLEND;
	TS_CLS;

	//現在のシーンを呼び出す
	if(Game.ForbidSceneDraw == false && this->currentScene != nullptr) {
		//シーン描画にのみ、フィルターを適用する
		DxLib::SetDrawBright(this->DrawBright.Red, this->DrawBright.Green, this->DrawBright.Blue);
		this->currentScene->Draw();
		DxLib::SetDrawBright(255, 255, 255);
	}
	TS_NOBLEND;

	//既定の描画処理
	Game.Sprite.DrawAll();					//スプライトをすべて描画
	TS_NOBLEND;
}


//####################  現在のシーンの入力処理を行う  ####################
void CGameObject::DoSceneInput() {
	//キー入力情報を更新
	this->CheckInput();

	//現在のシーンを呼び出す
	if(Game.ForbidSceneInput == false && this->currentScene != nullptr) {
		this->currentScene->Input();
	}
}


//####################  現在のシーンスクリプトを読み直す  ####################
void CGameObject::SQCompileCurrentScene() {
	if(this->currentScene->SQSubHandle != -1) {
		Game.SQ.Recompile(this->currentScene->SQSubHandle);
	} else {
		Game.SQ.Recompile(this->currentScene->SQHandle);
	}
}


//####################  マップシーンであるならそのシーンオブジェクトを返す  ####################
CSceneMap* CGameObject::getSceneMap() {
	auto sceneObj = dynamic_cast<CSceneMap*>(this->currentScene);
	if(sceneObj == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在マップシーンではないため、シーンオブジェクトを取得できません。");
	}
	return sceneObj;
}


//####################  タイトルシーンであるならそのシーンオブジェクトを返す  ####################
CSceneTitle* CGameObject::getSceneTitle() {
	auto sceneObj = dynamic_cast<CSceneTitle*>(this->currentScene);
	if(sceneObj == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "現在タイトルシーンではないため、シーンオブジェクトを取得できません。");
	}
	return sceneObj;
}


//####################  シーン名を返す  ####################
string CGameObject::GetCurrentSceneName() {
	return (this->currentScene == nullptr) ? "(無効)" : this->currentScene->GetName();
}


//####################  別のスクリプトを読み込む  ####################
void CGameObject::SQInclude(string fileName) {
	this->SQ.Do(fileName, false);
}


//####################  スクリプトを中断し、ステップ実行へ移行する  ####################
void CGameObject::SQBreakPoint() {
	this->SQ.StartStepRun();
}


//####################  スクリプトのステップ実行を終了してコード実行を再開する  ####################
void CGameObject::SQBreakResume() {
	this->SQ.EscapeDebugger();
}


//####################  時間指定でウェイト  ####################
//ScreenFlipを含むため、正確なウェイト秒にならないことがある
//キーが押されて時間いっぱいのウェイトにならなかった場合は返値がfalseになる
bool CGameObject::Wait(double timeSecond, bool enabledMT, bool keyUpBreak) {
	const int startTime = DxLib::GetNowCount();
	const int sceneTime = this->currentScene->StartTime;
	bool beforePressed = this->IsAnyKeyDown();		//ウェイト前からキーが押されているかどうかのフラグ
	bool pressing = false;
	bool keyDownFlag;

	//指定時間が経過するまでサブメインループを構成する、ポーズ中は停止したままにする
	while(Game.Pausing || timeSecond < 0.0 || ::GetNowCount() - startTime <= timeSecond * 1000) {
		//継続的なキー押下状況の更新
		this->CheckInput();

		if(keyUpBreak) {
			//キー入力が関わるウェイトのときのみ、入力情報をチェック
			keyDownFlag = this->IsAnyKeyDown();

			if(!beforePressed) {
				//ウェイト前にキーが押されていない状態
				if(keyDownFlag) {
					pressing = true;			//キーが押された状態になった
				} else if(pressing) {
					return false;				//キーが離されてウェイト解除
				}
			} else if(!keyDownFlag) {
				//ウェイト前から押されていたキーが解除されたことを検知
				beforePressed = false;
			}
		}

		//毎ループで行う処理
		if(!enabledMT || Game.Pausing) {
			TS_EVERYLOOP_NOMT;		//他のスレッドもすべて止めてウェイトする
		} else {
			Game.DoSceneDraw();
			TS_EVERYLOOP;			//他のスレッドは動作させながらウェイトする
		}
		if(sceneTime != this->currentScene->StartTime) {
			//シーンが遷移したら強制的に抜ける
			return false;
		}
	}
	return true;
}


//####################  キーが押されて離されるまでウェイト  ####################
void CGameObject::WaitKey(bool enabledMT) {
	this->Wait(-1.0, enabledMT, true);
}


//####################  何かキーが押された状態であるかを調べる  ####################
bool CGameObject::IsAnyKeyDown() {
	return std::find_if(TS_CONTAINERALL(this->keyInputData), [](int val) {
		return val > 0;
	}) != this->keyInputData.end();
}


//####################  累積的なキー入力情報を取得する  ####################
int* CGameObject::GetKeyInputCounterPTR(int keyID) {
	TS_CHECKINDEX(keyID, 0, this->keyInputData.size()) {
		return &this->keyInputData[keyID];
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "無効なキー番号 [%d] です。", keyID);
		return nullptr;
	}
}
int CGameObject::GetKeyInputCounter(int keyID) {
	return *this->GetKeyInputCounterPTR(keyID);
}
void CGameObject::SetKeyInputCounter(int keyID, int val) {
	*this->GetKeyInputCounterPTR(keyID) = val;
}


//####################  トランジションの前準備を行う  ####################
void CGameObject::PrepareTransition() {
	//描画を停止する
	Game.ForbidSceneDraw = true;
	Game.Sprite.ForbidLayerDraw = true;

	//最後のスクリーンを保存する
	this->screenHandleForTransition = DxLib::MakeGraph(Game.WindowSize.Width, Game.WindowSize.Height);
	DxLib::GetDrawScreenGraph(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, this->screenHandleForTransition);
}


//####################  トランジションを実行する  ####################
void CGameObject::DoTransition(string blendFileName, int speed) {
	//描画を再開してブレンドを開始する
	int blendHandle = DxLib::LoadBlendGraph((TS::DirectoryNames::PrePath + blendFileName).c_str());
	Game.ForbidSceneDraw = false;
	Game.Sprite.ForbidLayerDraw = false;

	for(int i = 0; i < 255; i += speed) {
		Game.DoSceneDraw();
		DxLib::DrawBlendGraph(0, 0, this->screenHandleForTransition, false, blendHandle, i, TS::System::DXBlendRange);
		TS_EVERYLOOP_NOMT;
	}

	//完全状態で再度描画する
	this->DoSceneDraw();
	Game.FPSKeeper.Update();

	//トランジションに使用したリソースを破棄する
	DxLib::DeleteGraph(blendHandle);
	DxLib::DeleteGraph(this->screenHandleForTransition);
}


//####################  画面をフェードアウトさせる  ####################
void CGameObject::FadeOut(int speed) {
	for(int cnt = 0; cnt < 255; cnt += speed) {
		this->DoSceneDraw();
		TS_ALPHABLEND(cnt);
		TS_CLS;
		TS_NOBLEND;
		TS_EVERYLOOP_NOMT;
	}

	//完全状態で再度描画する
	TS_CLS;
	Game.FPSKeeper.Update();

	//フェードアウト中はシーン描画を停止する
	Game.ForbidSceneDraw = true;
}


//####################  画面をフェードインさせる  ####################
void CGameObject::FadeIn(int speed) {
	Game.ForbidSceneDraw = false;
	for(int cnt = 0; cnt < 255; cnt += speed) {
		this->DoSceneDraw();
		TS_ALPHABLEND(255 - cnt);
		TS_CLS;
		TS_NOBLEND;
		TS_EVERYLOOP_NOMT;
	}

	//完全状態で再度描画する
	this->DoSceneDraw();
	Game.FPSKeeper.Update();
}


//####################  フォルダーが存在しない場合は作成する  ####################
void CGameObject::CheckDirectory(string path) {
	if(!::PathIsDirectory(path.c_str())) {
		::CreateDirectory(path.c_str(), nullptr);
	}
}


//####################  指定フォルダーにあるファイルをすべて列挙する  ####################
//フォルダーはプログラムの位置が基準となり、ファイル名もここからのパスでセットされる
//フィルター拡張子は [.xxx] の形式で指定すること
void CGameObject::GetFiles(vector<string> &dest, string folder, string filter, bool includeSubFolder) {
	//再帰的に追加するため宛先リストは初期化しない
	//dest.clear();

	HANDLE hFind;
	WIN32_FIND_DATA fd;

	//検索用パス文字列を作る
	if(folder.empty() == false && folder.substr(folder.size() - 1, 1) != "\\") {
		folder += "\\";		//対象フォルダーの末尾は必ず区切り記号にする
	}
	string pattern = Game.CurrentPath + folder;
	pattern += "*";

	//ファイル探索
	hFind = ::FindFirstFile(pattern.c_str(), &fd);
	if(hFind == INVALID_HANDLE_VALUE) {
		return;		//検索失敗
	}

	//順次ファイル名を格納していく
	do {
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		   && strcmp(fd.cFileName, "..") != 0 && strcmp(fd.cFileName, ".") != 0
		   && includeSubFolder) {
			//フォルダーが検出されたらその中身を検査する
			string subDir = folder + string(fd.cFileName);
			CGameObject::GetFiles(dest, subDir, filter, true);
		} else if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
			//隠しファイルは除く
			string str = fd.cFileName;
			string ext = ::PathFindExtension(str.c_str());
			if(str != "desktop.ini" && ext == filter) {
				str = folder + str;
				dest.push_back(str);	//ファイル名をリストに格納
			}
		}
	} while(::FindNextFile(hFind, &fd));		//次のファイルを探索

	::FindClose(hFind);
}


//####################  文字列を区切り文字に従って分割する  ####################
//使用例：SplitString(out, "hoge,piyo,fug", ",");
void CGameObject::SplitString(vector<string> &dest, const string &src, const string &delim) {
	string::size_type start = 0;	//デリミタを検索するインデックス
	dest.clear();
	if(src.empty()) {
		return;			//ソース文字列が空白の場合は追加しない
	}
	while(true) {
		const string::size_type end = src.find(delim, start);
		if(end != string::npos) {
			dest.push_back(src.substr(start, end - start).c_str());
		} else {		//デリミタが見つからなかった場合
			dest.push_back(src.substr(start, src.length() - start).c_str());
			break;
		}
		start = end + delim.length();
	}
}


//####################  文字列リストの先頭行を抜き出す  ####################
//注意：複数個の引数の中で使わないこと。引数の評価順序は不定であるため、順番通りに取り出されない可能性がある。
string CGameObject::PopHead(vector<string>& src) {
	if(src.size() == 0) {
		return "";
		//NOTE: 中身がない場合でも警告は出さない。
	} else {
		string strtemp = src[0];
		src.erase(src.begin());
		return strtemp;
	}
}


//####################  メッセージボックスを表示する  ####################
int CGameObject::MsgBox(string msg, string caption, int iconID, int buttonID) {
	return ::MessageBox(DxLib::GetMainWindowHandle(), msg.c_str(), caption.c_str(), iconID + buttonID);
}


//####################  デバッグウィンドウにメッセージを表示する  ####################
void CGameObject::PrintLog(LPCTSTR fileName, LPCTSTR funcName, int lineNum, string header, string fmt, ...) {
	//可変長引数を解決する
	va_list args;
	va_start(args, fmt);
	::_vsntprintf(::buf, sizeof(::buf) - 1, fmt.c_str(), args);
	va_end(args);

	fmt = ::buf;
	fmt = header + fmt;		//ヘッダーを付加

	//ソース情報を付加する
	if(fileName != nullptr && funcName != nullptr) {
		::sprintf_s(::buf, " -----> %s -> %s 関数 -> %d 行目", PathFindFileName(fileName), funcName, lineNum);
		fmt += ::buf;
	}

	//時刻を付加する場合
	if(!Game.NoTimeStamp) {
		struct tm *ti;			//時間情報
		time_t now;				//現在までの経過時間
		string NowTime;			//最終的な現在日時の文字列
		::time(&now);
		ti = ::localtime(&now);
		::strftime(::buf, 255, "%Y/%m/%d ", ti);
		NowTime += ::buf;
		::strftime(::buf, 255, "%H:%M:%S   ", ti);
		NowTime += ::buf;
		fmt = ::buf + fmt;		//ヘッダーよりも前に付加
	}

	//改行記号を自動で付加する
	fmt += "\n";

	Game.Log.push_front(fmt);				//ログに追加する
	if(Game.Log.size() > TS::System::DebugLogCountMax) {
		Game.Log.pop_back();				//最も古いログを消去する
	}

#ifdef _DEBUG
	::printf(fmt.c_str());
	::OutputDebugString(fmt.c_str());		//イミディエイトウィンドウに出力
#endif
}


//####################  デバッグ出力の簡易版  ####################
void CGameObject::PrintLog(string text) {
	TS_LOG(text);
}


//####################  アプリケーション内警告を発生させる  ####################
void CGameObject::RaiseWarning(LPCTSTR fileName, LPCTSTR funcName, int lineNum, TS::System::ErrorID id, string fmt, ...) {
	//可変長引数を解決する
	va_list args;
	va_start(args, fmt);
	::_vsntprintf(::buf, sizeof(::buf) - 1, fmt.c_str(), args);
	va_end(args);

	TS_WARN(fileName, funcName, lineNum, std::to_string(static_cast<int>(id)) + " : " + ::buf);
}


//####################  アプリケーション内エラーを発生させる  ####################
void CGameObject::RaiseError(LPCTSTR fileName, LPCTSTR funcName, int lineNum, TS::System::ErrorID id, string fmt, ...) {
	//可変長引数を解決する
	va_list args;
	va_start(args, fmt);
	::_vsntprintf(::buf, sizeof(::buf) - 1, fmt.c_str(), args);
	va_end(args);

	TS_ERROR(fileName, funcName, lineNum, std::to_string(static_cast<int>(id)) + " : " + ::buf);
}


//####################  API関数で発生したエラーをイミディエイトウィンドウに出力する  ####################
void CGameObject::PrintAPIError() {
	string ErrorText;
	LPVOID lpMsgBuf;
	::FormatMessage(				//エラー表示文字列作成
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, nullptr
	);
	ErrorText = "ERROR From WinAPI : ";
	ErrorText += static_cast<const char*>(lpMsgBuf);
	if(ErrorText.find("正しく終了") == string::npos) {		//正常終了しているエラーメッセージは出力しない
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::WinAPI, ErrorText);			//メッセージ出力
	}
	::LocalFree(lpMsgBuf);
}


//####################  ログ情報をすべてファイルに書き出す  ####################
void CGameObject::WriteDownLog() {
	TS_LOG("ログファイルに出力します。");
	Game.NoTimeStamp = true;		//時刻情報を含めないログ出力に切り替える

	//ゲームの状態を内部ログへ出力する
	TS_LOG(Game.Format("\n現在のシーン: %s", (Game.currentScene == nullptr) ? "null" : Game.currentScene->GetName().c_str()).c_str());

	//可変DB、ユーザーDBの情報を内部ログへ出力する
	for(auto vdb : Game.DB.VDB) {
		vdb->PrintData();
	}
	Game.DB.UDB.PrintData();

	//イベント個別変数の情報を内部ログへ出力する
	CSceneMap::PrintMap();
	CSceneMap::PrintEVs();
	CSceneMap::PrintSelfValueData();

	//時刻情報を取得してファイル名にする
	struct tm *ti;			//時間情報
	time_t now;				//現在までの経過時間
	string fileName = TS::DirectoryNames::Log;
	::time(&now);
	ti = ::localtime(&now);
	::strftime(::buf, 255, "%Y-%m-%d ", ti);
	fileName += ::buf;
	::strftime(::buf, 255, "%Hh%Mm%Ss.log", ti);
	fileName += ::buf;

	//書き込み用にファイルを開く
	FILE* fp;
	Game.CheckDirectory(TS::DirectoryNames::Log);		//フォルダーが存在しない場合は作成する
	if((fp = ::fopen(fileName.c_str(), "w")) == nullptr) {
		TS_MBE("ログファイルの出力に失敗しました。");
		return;
	}

	//ログ書き込み処理: 時系列順にするために逆から書き出す
	while(!Log.empty()) {
		::fputs(Log.back().c_str(), fp);
		Log.pop_back();
	}
	::fclose(fp);
}


//####################  デバッグコンソールからの入力を受け付ける  ####################
string CGameObject::ScanConsole() {
#ifdef _DEBUG
	CDebug::ActivateConsole();
	::printf(">");
	::fflush(stdin);
	::fgets(::buf, sizeof(::buf) - 1, stdin);
	::buf[strlen(::buf) - 1] = '\0';
	CDebug::ActivateGameWindow();
	return ::buf;
#else
	return "";
#endif
}


//####################  システムリソース情報を描画する  ####################
void CGameObject::DrawSystemResourceInfo() {
	string strbuf;

	//フレームレート
	this->frameCounter++;
	const int nowTime = DxLib::GetNowCount();
	const int delta = nowTime - this->lastTimeCount;
	if(delta >= 1000) {
		this->nowFPS = static_cast<double>(this->frameCounter) / delta * 1000.0;
		this->lastTimeCount = nowTime;
		this->frameCounter = 0;
	}
	strbuf += Game.Format("FPS: %6.2lf\n", this->nowFPS);
	//strbuf += Game.Format("FPS: %6.2lf (Skip %d)\n", this->nowFPS, this->FPSKeeper.getTempSkipFrameCount());

	//消費メモリ
#ifdef _DEBUG
	PROCESS_MEMORY_COUNTERS pmc = PROCESS_MEMORY_COUNTERS();
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, true, GetCurrentProcessId());
	double mem = 0, swap = 0;
	if(hProcess != nullptr) {
		if(::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {		// プロセスの使用メモリを調べる
			mem = pmc.WorkingSetSize / 1024.0 / 1024.0;			// MB単位
			swap = pmc.PagefileUsage / 1024.0 / 1024.0;			// MB単位
			::CloseHandle(hProcess);

			//記録更新
			if(this->maxRAM < mem) {
				this->maxRAM = mem;
			}
			if(this->maxSwap < swap) {
				this->maxSwap = swap;
			}
		}
	}
	strbuf += Game.Format("RAM:  %6.2lf MB (Max %6.2lf)\n", mem, this->maxRAM);
	strbuf += Game.Format("Swap: %6.2lf MB (Max %6.2lf)\n\n", swap, this->maxSwap);
#endif

	//ゲームシステム
	strbuf += Game.Format("Scene: %s [%d / %d]\n", Game.GetCurrentSceneName().c_str(), Game.GetCurrentMainSceneID(), Game.GetCurrentSubSceneID());
	strbuf += Game.Format("Forbidden-Draw:  %s\n", Game.ForbidSceneDraw ? "ON" : "OFF");
	strbuf += Game.Format("Forbidden-Input: %s\n", Game.ForbidSceneInput ? "ON" : "OFF");
	strbuf += "\n";

	//その他のリソース
	strbuf += Game.Format("SQ-Scripts: %2d\n", Game.SQ.GetCount());
	strbuf += Game.Format("MT-Threads: %2d\n", Game.MT.GetThreadCount());
	strbuf += Game.Format("Sprites:    %2d\n", Game.Sprite.Data.GetCount());
	strbuf += Game.Format("Material-Graphics: %3d\n", Game.Material.getGraphics()->Data.GetCount());
	strbuf += Game.Format("Material-Sounds:   %3d\n", Game.Material.getSounds()->Data.GetCount());
	strbuf += Game.Format("Material-Fonts:    %3d\n", Game.Material.getFonts()->Data.GetCount());

	TS_ALPHABLEND(TS::Map::SemiTransparent);
	DxLib::DrawBox(0, 0, 180, 185, BLACK, true);
	TS_NOBLEND;
	DxLib::DrawStringToHandle(0, 0, strbuf.c_str(), WHITE, Game.Material.getFonts()->DefaultFontHandle);
}


//####################  ウィンドウ状態を書き出す  ####################
void CGameObject::SaveWindowState() {
	CPoint gWndPos;
	RECT cWndRect;
	FILE *fp;
	char path[MAX_PATH];

	//コンソールの表示位置を調べる
	cWndRect.left = 0;
	cWndRect.top = 0;
#ifdef _DEBUG
	::GetWindowRect(::GetConsoleWindow(), &cWndRect);
#endif

	// 書き出し先のパスを生成 (AppData/.../ゲームタイトル 以下)
	::SHGetSpecialFolderPath(nullptr, path, CSIDL_APPDATA, 0);
	::strcat_s(path, "\\");
	::strcat_s(path, Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
	if (!::PathFileExists(path)) {
		// ディレクトリーが存在しない場合は作成
		::CreateDirectory(path, nullptr);
	}
	::strcat_s(path, "\\windowsettings.dat");

	fp = ::fopen(path, "w");
	if(fp != nullptr) {
		DxLib::GetWindowPosition(&gWndPos.X, &gWndPos.Y);
		::fprintf(fp, "%d\n%d\n%d\n%d\n%d\n%d\n", cWndRect.left, cWndRect.top, gWndPos.X, gWndPos.Y, ::GetWindowModeFlag(), Game.FPSKeeper.getSkipFrameCount());
		::fclose(fp);
	}
}


//####################  ウィンドウ状態を復元する  ####################
void CGameObject::LoadWindowState() {
	CPoint gWndPos;
	FILE* fp;
	char path[MAX_PATH];
	const DWORD dwNameSize = sizeof(::buf);

	// 設定されなかった場合の初期値をセットしておく
	gWndPos.Set(100, 100);
	DxLib::ChangeWindowMode(true);
	this->FPSKeeper.setSkipFrameCount(0);

	// 書き出し先のパスを生成 (AppData/.../ゲームタイトル 以下)
	::SHGetSpecialFolderPath(nullptr, path, CSIDL_APPDATA, 0);
	::strcat_s(path, "\\");
	::strcat_s(path, Game.DB.getFDBSystem()->GetString(TS::DB::System::GameTitle).c_str());
	if (!::PathFileExists(path)) {
		// ディレクトリーが存在しない場合は作成
		::CreateDirectory(path, nullptr);
	}
	::strcat_s(path, "\\windowsettings.dat");

	TS_LOG("ウィンドウ状態を復元します...");
	fp = ::fopen(path, "r");
	if(fp != nullptr) {
		//オープンに成功したときだけ
		TS_COUNTLOOP(i, 4) {
			::fgets(::buf, sizeof(::buf) - 1, fp);
			switch(i) {
				case 0:
					Game.ConsolePosition.X = atoi(::buf);
					break;
				case 1:
					Game.ConsolePosition.Y = atoi(::buf);
					break;

				case 2:
					gWndPos.X = atoi(::buf);
					break;
				case 3:
					gWndPos.Y = atoi(::buf);
					break;

				case 4:
					DxLib::ChangeWindowMode(atoi(::buf));
					break;

				case 5:
					Game.FPSKeeper.setSkipFrameCount(atoi(::buf));
					break;
			}
		}

		// NOTE: コンソールウィンドウ位置の設定は初期化処理の中で行われる

		DxLib::SetWindowPosition(gWndPos.X, gWndPos.Y);
		::fclose(fp);
		TS_LOG("ウィンドウ状態の復元が完了しました。");
	} else {
		//デフォルト設定で起動する
		TS_LOG("ウィンドウ状態の復元に失敗したため、デフォルト設定で起動します。");
		DxLib::ChangeWindowMode(true);
	}
}


//####################  プログラムを終了させる  ####################
void CGameObject::ApplicationExit(int code) {
	if(code == static_cast<int>(TS::System::ExitCode::Question)) {
		if(::MessageBox(::GetMainWindowHandle(), "ゲームを終了しますか？", "Game.exe", MB_YESNO + MB_ICONQUESTION) != IDYES) {
			return;
		}
		code = static_cast<int>(TS::System::ExitCode::Success);
	}

	TS_LOG_NOTIME("\n--------------------------------------------------------------------\n");
	TS_LOG("プログラムを終了する準備に入ります...\n");
	if(Game.SQ.IsDebugging()) {
		//デバッガーが起動中の場合はただちに終了の合図を送る
		Game.SQ.KillDebuggerFlag = true;
		TS_LOG("スクリプトデバッガーを終了しています...\n");
		Game.RegularFunc.AddFunction(::SQDebuggerKiller);
		return;
	}
	if(code != static_cast<int>(TS::System::ExitCode::Success) && code != static_cast<int>(TS::System::ExitCode::InvalidSystemDB)) {
		TS_LOG_NOTIME("\n--------------------------------------------------------------------\n");
		TS_LOG_NOTIME("\t予期せぬエラーにつき異常終了");
		TS_LOG_NOTIME("\n--------------------------------------------------------------------\n");

		//異常終了のときはログをファイルに出力する
		::printSQCallStackAll(Game.SQ.GetVM());
		Game.WriteDownLog();		
	}

	//破棄処理
	Game.DisposeScene();
	vector<CSceneMap::CEVSelfValueOneData>().swap(CSceneMap::SelfValueData);
	Game.Material.Dispose();
	if (code != static_cast<int>(TS::System::ExitCode::InvalidSystemDB)) {
		Game.SaveWindowState();
	}
	Game.SQ.Dispose();
	::GGSFREE();
	DxLib::DxLib_End();
#ifdef _DEBUG
	::FreeConsole();
#endif
	std::exit(code);			//すべてのオブジェクトを破棄した後、プログラムを終了する
}


//####################  処理時間の計測を開始  ####################
void CGameObject::StartMeasureProcTime() {
	CGameObject::MeasureProcTime = DxLib::GetNowHiPerformanceCount();
}


//####################  処理時間の計測結果を表示し、続けて計測を開始する  ####################
void CGameObject::ContinueMeasureProcTime(string tag) {
	CGameObject::EndMeasureProcTime(tag);
	CGameObject::StartMeasureProcTime();
}


//####################  処理時間の計測結果を表示  ####################
void CGameObject::EndMeasureProcTime(string tag) {
	CGameObject::MeasureProcTime = DxLib::GetNowHiPerformanceCount() - CGameObject::MeasureProcTime;
	TS_LOG("処理時間: %lld マイクロ秒 [%s]", CGameObject::MeasureProcTime, tag.c_str());
}


//####################  トリガー付の定期実行：スクリプトデバッガーを終了するのを監視してプログラムを終了させる  ####################
void SQDebuggerKiller() {
	if(!Game.SQ.IsDebugging()) {
		Game.ApplicationExit(static_cast<int>(TS::System::ExitCode::Success));
	}
}


//####################  すべてのキーの入力状態を継続的にカウントする  ####################
void CGameObject::CheckInput() {
	char charbuf[MAXBYTE + 1];
	DxLib::GetHitKeyStateAll(charbuf);
	const int input = DxLib::GetJoypadInputState(DX_INPUT_PAD1);
	for(int i = 0; i <= MAXBYTE; i++) {
		//カウントを無視するコードをチェック
		auto ignore = false;
		for(const auto& ignoreCode : this->ignoreKeyCodes) {
			if(i == ignoreCode) {
				ignore = true;
			}
		}
		if(ignore == true) {
			(*Game.GetKeyInputCounterPTR(i)) = 0;
			continue;
		}

		//カウント処理実行
		if(charbuf[i] == TRUE) {
			(*Game.GetKeyInputCounterPTR(i))++;
		} else {
			//ゲームパッドの入力情報をキー情報に変換する
			if(i == KEY_INPUT_DOWN && (input & PAD_INPUT_DOWN) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_UP && (input & PAD_INPUT_UP) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_LEFT && (input & PAD_INPUT_LEFT) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_RIGHT && (input & PAD_INPUT_RIGHT) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_Z && (input & PAD_INPUT_1) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_X && (input & PAD_INPUT_2) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_C && (input & PAD_INPUT_3) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_A && (input & PAD_INPUT_4) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_S && (input & PAD_INPUT_5) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_D && (input & PAD_INPUT_6) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_Q && (input & PAD_INPUT_7) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_W && (input & PAD_INPUT_8) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_ESCAPE && (input & PAD_INPUT_9) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			} else if(i == KEY_INPUT_SPACE && (input & PAD_INPUT_10) != FALSE) {
				(*Game.GetKeyInputCounterPTR(i))++;
				continue;
			}
			*Game.GetKeyInputCounterPTR(i) = 0;
		}
	}
}


//####################  UTF-8 から Shift-JIS に変換する  ####################
string CGameObject::UTF8ToSJIS(string srcUTF8) {
	//Unicodeへ変換後の文字列長を得る
	const int lenghtUnicode = ::MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, nullptr, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	::MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	const int lengthSJis = ::WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, nullptr, 0, nullptr, nullptr);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	::WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, nullptr, nullptr);

	string strSJis(bufShiftJis);

	delete[] bufUnicode;
	delete[] bufShiftJis;
	return strSJis;
}


//####################  両端のダブルクォートを削除する  ####################
string CGameObject::CutDQ(string baseString) {
	if(baseString.size() > 0) {
		if(baseString.substr(0, 1) == "\"") {
			baseString = baseString.substr(1);
		}
		if(baseString.substr(baseString.size() - 1, 1) == "\"") {
			baseString = baseString.substr(0, baseString.size() - 1);
		}
	}
	return baseString;
}


//####################  文字列を置換して返す  ####################
string CGameObject::Replace(string baseString, string before, string after) {
	string::size_type Pos(baseString.find(before));

	//findとreplaceを繰り返して、文字列に含まれる全ての文字列を置換する
	while(Pos != string::npos) {
		baseString.replace(Pos, before.length(), after);
		Pos = baseString.find(before, Pos + after.length());
	}

	return baseString;
}


//####################  全角文字に対応した文字数を返す  ####################
int CGameObject::GetStringLength(string src) {
	auto wstr = Game.CWString(src);
	return wstr.size();
}


//####################  指定位置に文字列を挿入  ####################
string CGameObject::Insert(string dest, int index, string text) {
	auto wstr = Game.CWString(dest);
	index = Game.WithIn(index, 0, wstr.size());
	wstr.insert(index, Game.CWString(text));
	return Game.CString(wstr);
}


//####################  string を wstring に変換する  ####################
wstring CGameObject::CWString(string const &str, int codeMulti) {
	const int sizeWide = MultiByteToWideChar(codeMulti, 0, &str[0], -1, nullptr, 0);
	if(sizeWide == 0) {
		return L"";
	}
	wstring wstr;
	wstr.resize(sizeWide);
	MultiByteToWideChar(codeMulti, 0, &str[0], -1, &wstr[0], sizeWide);
	if(wstr[wstr.size() - 1] == L'\0') {
		//末尾の終端記号を消す
		wstr = wstr.substr(0, wstr.size() - 1);
	}
	return wstr;
}


//####################  wstring を string に変換する  ####################
string CGameObject::CString(wstring const &wstr, int codeMulti) {
	const int sizeMulti = WideCharToMultiByte(codeMulti, 0, &wstr[0], -1, nullptr, 0, nullptr, nullptr);
	if(sizeMulti == 0) {
		return "";
	}
	string str;
	str.resize(sizeMulti);
	WideCharToMultiByte(codeMulti, 0, &wstr[0], -1, &str[0], sizeMulti, nullptr, nullptr);
	if(str[str.size() - 1] == '\0') {
		//末尾の終端記号を消す
		str = str.substr(0, str.size() - 1);
	}
	return str;
}


//####################  0から25までの整数をアルファベットに置き換える  ####################
string CGameObject::IntToAlphabet(int value) {
	string RChar;
	switch(value % 26) {
		case  0: RChar += "Ａ"; break;
		case  1: RChar += "Ｂ"; break;
		case  2: RChar += "Ｃ"; break;
		case  3: RChar += "Ｄ"; break;
		case  4: RChar += "Ｅ"; break;
		case  5: RChar += "Ｆ"; break;
		case  6: RChar += "Ｇ"; break;
		case  7: RChar += "Ｈ"; break;
		case  8: RChar += "Ｉ"; break;
		case  9: RChar += "Ｊ"; break;
		case 10: RChar += "Ｋ"; break;
		case 11: RChar += "Ｌ"; break;
		case 12: RChar += "Ｍ"; break;
		case 13: RChar += "Ｎ"; break;
		case 14: RChar += "Ｏ"; break;
		case 15: RChar += "Ｐ"; break;
		case 16: RChar += "Ｑ"; break;
		case 17: RChar += "Ｒ"; break;
		case 18: RChar += "Ｓ"; break;
		case 19: RChar += "Ｔ"; break;
		case 20: RChar += "Ｕ"; break;
		case 21: RChar += "Ｖ"; break;
		case 22: RChar += "Ｗ"; break;
		case 23: RChar += "Ｘ"; break;
		case 24: RChar += "Ｙ"; break;
		case 25: RChar += "Ｚ"; break;
	}
	return RChar;
}


//####################  部分文字列を取得する  ####################
//length=-1のとき、末尾までを対象とする
string CGameObject::SubString(string src, int start, int length) {
	auto wstr = Game.CWString(src);
	start = Game.WithIn(start, 0, wstr.size());
	if(length == -1 || wstr.size() <= start + length) {
		return Game.CString(wstr.substr(start));
	} else {
		length = Game.WithIn(length, 0, src.size() - start);
		return Game.CString(wstr.substr(start, length));
	}
}


//####################  現在時刻を文字列にする  ####################
string CGameObject::NowTimeToString(string format) {
	//時間取得
	struct tm *ti;			//時間情報
	time_t now;				//現在までの経過時間
	::time(&now);
	ti = ::localtime(&now);

	//日時を書式化
	::strftime(::buf, sizeof(::buf) - 1, format.c_str(), ti);
	return ::buf;
}


//####################  書式文字列をstring型に変換する  ####################
string CGameObject::Format(string text, ...) {
	va_list ap;
	va_start(ap, text);
	::_vsntprintf(::buf, sizeof(::buf) - 1, text.c_str(), ap);
	va_end(ap);
	return ::buf;
}


//####################  キー入力情報を初期化する  ####################
void CGameObject::initKeyInputData() {
	for(int i = 0; i <= MAXBYTE; i++) {
		this->keyInputData[i] = 0;
	}
}
