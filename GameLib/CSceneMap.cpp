//###################################################################
//			マップシーン定義
//###################################################################
#include "Header.h"
#define Z_INFLATE_WINDOWBITS (15/*zlibフォーマット値*/ + 16/*gzipフォーマット値*/ + 16/*zlib/gzip互換用フォーマット値*/)

const uInt buflimit = 1024 * 1024 * 1;
const uInt tempsize = 16384;
extern char buf[TS::System::BufferLengthMax];
bool EVPage(TS_MT);
bool EVTrigger(TS_MT);
bool EVMove(TS_MT);
bool Anim(TS_MT);
void RestoreTilePallet();


//####################  初期設定  ####################
CSceneMap::CSceneMap() {
	//頻繁に使う設定項目をコピー
	this->MapData.TileSize = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::TileSize);
	this->MapData.ShadowTransparent = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::ShadowTransparent);
	this->MapData.FogTransparent = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::FogTransparent);

	//プレイヤー設定
	this->MapData.Player.MapObject.ID = -1;
	this->MapData.Player.MapObject.Hide = false;
	this->MapData.Player.CharData.EnabledStopAnim = true;
	this->MapData.Player.CharData.EnabledWalkAnim = true;
	this->MapData.Player.CharData.NoHit = false;
	this->MapData.Player.CharData.FixedDirection = false;
	this->MapData.Player.CharData.Direction = static_cast<int>(TS::Map::Direction4::South);

	//標準のMT関数を登録
	Game.MT.AddThread("EVPage", ::EVPage, TS::System::Scene::Map, true);
	Game.MT.AddThread("EVTrigger", ::EVTrigger, TS::System::Scene::Map, true);
	Game.MT.AddThread("EVMove", ::EVMove, TS::System::Scene::Map, true);
	Game.MT.AddThread("Anim", ::Anim, TS::System::Scene::Map, true)
		.AddInteger("counter", 0);
}


//####################  シーン破棄時  ####################
void CSceneMap::Dispose() {
	CSceneBase::Dispose();
	Game.DB.getFDBTileset()->DisposeTilePallet();
	this->MapData.Dispose();
}


//####################  現在のマップにあるイベントの個別変数情報を内部蓄積データにコミットする  ####################
void CSceneMap::commitMapSelfValue() {
	//全イベントを走査
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		//全イベント個別変数を走査
		for(auto& selfValue : ev.SelfValues.data.Items) {
			auto exists = false;

			//既にコミット済みの個別変数テーブルを走査
			for(auto& item : SelfValueData) {
				if(item.MapGUID == Game.getSceneMap()->MapData.GUID 
				&& item.EVID == ev.ID
				&& selfValue.first == item.Value.ID) {
					//既にデータが登録されている場合は値を更新する
					item.Value = selfValue.second;
					exists = true;
					break;
				}
			}

			if(!exists) {
				//新規登録
				SelfValueData.push_back(CEVSelfValueOneData(
					Game.getSceneMap()->MapData.GUID,
					ev.ID,
					&selfValue.second
				));
			}
		}
	}
}


//####################  現在のマップのイベントに対し、コミットされている個別変数情報を反映する  ####################
//注意：マップイベントを配置し終えた後に呼び出すこと。
void CSceneMap::applyMapSelfValue() {
	//全データを走査し、このマップであるが存在しないイベントの情報がある場合は削除する
	SelfValueData.erase(std::remove_if(TS_CONTAINERALL(SelfValueData), [&](CEVSelfValueOneData& item) {
		//このマップではない場合はスキップ
		if(item.MapGUID != Game.getSceneMap()->MapData.GUID) {
			return false;
		}

		//全イベントを走査し、該当イベントが存在しないかどうかを調べる
		return std::find_if(TS_CONTAINERALL(Game.getSceneMap()->MapData.EVs.Items), [&](CSceneMap::CMapData::CMapEventData& data) {
			return item.EVID == data.ID;
		}) == Game.getSceneMap()->MapData.EVs.Items.end();
	}), SelfValueData.end());

	//全イベントを走査
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		//先に初期値を与える
		Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfEvent), &ev.SelfValues);

		//このマップのこのイベントの個別変数情報が既に登録されているかどうかを調べる
		for(auto& sv : SelfValueData) {
			if(sv.MapGUID == Game.getSceneMap()->MapData.GUID && sv.EVID == ev.ID) {
				//既にデータが登録されている場合は最新値をセットする
				ev.SelfValues.Get(sv.Value.ID)->Value = sv.Value.Value;
			}
		}
	}
}


//####################  セーブデータから内部蓄積データベースに反映する  ####################
void CSceneMap::loadSelfValueData(vector<string>& vectorbuf) {
	string temp;
	vector<string> spl;
	vector<string> subspl;
	auto& selfEV = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfEvent));

	//空行が出てくるまで連続的に読み込む
	SelfValueData.clear();
	while(!(temp = Game.PopHead(vectorbuf)).empty()) {
		Game.SplitString(spl, temp, "\t");
		if(spl.size() < 4) {
			continue;		//不正なデータは無視
		}

		//有効な変数のIDであるか検証し、無効なデータは落とす
		const int valueID = atoi(spl[2].c_str());
		auto row = selfEV.GetRowFromID(valueID);
		if(row == nullptr) {
			continue;
		}

		//実際に登録する
		auto data = CIDValueData(valueID, *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR(), atoi(spl[3].c_str()));
		SelfValueData.push_back(CEVSelfValueOneData(spl[0].c_str(), atoi(spl[1].c_str()), &data));
	}
}


//####################  内部蓄積データベースをセーブデータに出力する  ####################
void CSceneMap::saveSelfValueData(string& strbuf) {
	for(auto& data : SelfValueData) {
		strbuf += Game.Format("%s\t%d\t%d\t%d\n", data.MapGUID.c_str(), data.EVID, data.Value.ID, data.Value.Value);
	}
	strbuf += "\n";		//空行を作る
}


//####################  セーブデータから現在のマップをロード  ####################
void CSceneMap::Load(vector<string>& vectorbuf) {
	string temp;
	auto& map = Game.getSceneMap()->MapData;

	//マップの基本情報
	temp = Game.PopHead(vectorbuf);
	Game.getSceneMap()->BeforeBGMFileName = "";
	Game.getSceneMap()->BeforeBGSFileName = "";
	map.LoadMap(temp, true);
	map.Player.MapObject.Hide = atoi(Game.PopHead(vectorbuf).c_str());
	map.Player.MapObject.Position.Set(Game.PopHead(vectorbuf));
	map.Player.CharData.Direction = atoi(Game.PopHead(vectorbuf).c_str());
	map.Player.CharData.LoadGraphics(Game.PopHead(vectorbuf));

	//イベントの配置情報
	temp = Game.PopHead(vectorbuf);
	const int EVCount = atoi(temp.c_str());
	TS_COUNTLOOP(i, EVCount) {
		//データをロードしてインスタンスを生成する
		map.EVs.Add(CMapData::CMapEventData(vectorbuf));
	}
	TS_COUNTLOOP(i, EVCount) {
		//日常移動はスクリプトから復元する
		auto ev = map.EVs.GetFromIndex(i);
		ev->TargetCharPTR = ev->getCurrentPage();
		Game.getSceneMap()->MapData.RunEV(ev->ID, ev->GetPageIndex(), static_cast<int>(TS::Map::EventScriptType::Move), false);
	}

	//イベント個別変数の内部蓄積データベースを読み込み、現在地マップに該当する個別変数情報を適用する
	CSceneMap::loadSelfValueData(vectorbuf);
	CSceneMap::applyMapSelfValue();

	//スクリプトのGC実行
	Game.SQ.CollectGarbage();
}


//####################  現在のマップをセーブデータに出力  ####################
void CSceneMap::Save(string& strbuf) {
	//セーブ前に現在の個別変数情報をコミットしておく
	CSceneMap::commitMapSelfValue();

	//マップの基本情報
	auto& map = Game.getSceneMap()->MapData;
	strbuf += map.CurrentMapFileName + "\n";
	strbuf += to_string(map.Player.MapObject.Hide) + "\n";
	strbuf += map.Player.MapObject.Position.ToString() + "\n";
	strbuf += to_string(map.Player.CharData.getDirection()) + "\n";
	strbuf += map.Player.CharData.Graphics + "\n";

	//イベントの配置情報
	strbuf += to_string(map.EVs.GetCount()) + "\n";
	for(auto& ev : map.EVs.Items) {
		ev.toString(strbuf);		//イベント基本情報、ページ情報を出力
	}

	//イベント個別変数の内部蓄積データベース
	CSceneMap::saveSelfValueData(strbuf);
}


//####################  マップ情報をログ出力する  ####################
void CSceneMap::PrintMap() {
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return;
	}

	TS_LOG("\nマップ情報:");
	TS_LOG(Game.Format("\tマップ名: %s", Game.getSceneMap()->MapData.CurrentMapName.c_str()).c_str());
	TS_LOG(Game.Format("\tマップGUID: %s", Game.getSceneMap()->MapData.GUID.c_str()).c_str());
	TS_LOG(Game.Format("\tプレイヤー情報:").c_str());
	TS_LOG(Game.Format("\t\t座標: (x, y) = (%d, %d)",
		Game.getSceneMap()->MapData.Player.MapObject.Position.X,
		Game.getSceneMap()->MapData.Player.MapObject.Position.Y).c_str());
	TS_LOG(Game.Format("\t\t向き: %d", Game.getSceneMap()->MapData.Player.CharData.Direction).c_str());
}


//####################  イベント情報をログ出力する  ####################
void CSceneMap::PrintEVs() {
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return;
	}

	TS_LOG("\nマップイベント一覧:");
	for(auto& data : Game.getSceneMap()->MapData.EVs.Items) {
		TS_LOG(data.toString());
	}
}


//####################  内部蓄積データベースをログ出力する  ####################
void CSceneMap::PrintSelfValueData() {
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return;
	}

	TS_LOG("\nマップイベント個別変数の内部蓄積データベース:");
	for(auto& data : SelfValueData) {
		TS_LOG("\tMap [%s] -> EVID: %d -> 個別変数ID: %d (%s) = %d",
			   data.MapGUID.c_str(),
			   data.EVID,
			   data.Value.ID,
			   Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfEvent))->GetRowFromID(data.Value.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
			   data.Value.Value
			   );
	}
}


//####################  gzip解凍処理  ####################
bool CSceneMap::deflate(string fileName, string& text) {
	text = "";
	int ret;

#ifdef _DEBUG
	//デバッグ時: ファイルから直接解凍を行う
	gzFile inFile = ::gzopen(fileName.c_str(), GZ_MODE_READ);

	//解凍処理
	while((ret = ::gzread(inFile, ::buf, sizeof(::buf) - 1)) > 0) {
		::buf[sizeof(::buf) - 1] = '\0';		//途中で途切れたとき、gzread関数は末尾に終端記号を付けてくれないことがあるらしい
		text += ::buf;
	}

	//gzerrorでエラーメッセージを取得する
	if(ret == -1) {
		const char *msg = ::gzerror(inFile, &ret);
		if(ret == Z_ERRNO) {
			msg = ::strerror(ret);
		}
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "解凍に失敗しました。\n>%s\n\n\n>%s", fileName.c_str(), msg);
		return false;
	}

	//gzcloseに失敗した場合、ファイルは閉じられるのでgzerrorできない
	if((ret = ::gzclose(inFile)) != Z_OK) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "解凍後のファイルクローズに失敗しました。\n>%s", fileName.c_str());
		return false;
	}

#else
	//リリース時: DXアーカイブからバイナリを読み出してストリーム解凍を行う
	z_stream z;

	//解凍ストリームの初期設定
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	//解凍準備
	ret = ::inflateInit2(&z, Z_INFLATE_WINDOWBITS);
	if(ret != Z_OK) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "解凍に失敗しました。\n>%s\n\n\n>%s", fileName.c_str(), z.msg);
		return false;
	}

	//対象ファイルをDXアーカイブから取り出す
	int hdl = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str(), false);
	if (hdl == 0) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "マップファイルの取り出しに失敗しました。\n>%s\n\n\n>%s", fileName.c_str(), z.msg);
		return false;
	}
	int size = DxLib::FileRead_size((TS::DirectoryNames::PrePath + fileName).c_str());
	void* bin = malloc(size);
	DxLib::FileRead_read(bin, size, hdl);

	//入力元、出力先を用意する
	z.next_in = (Bytef*)bin;
	z.avail_in = size;					//入力（圧縮データ）が分割されることはない
	z.next_out = (Bytef*)::buf;
	const auto availOutSize = sizeof(::buf) - 2;
	z.avail_out = availOutSize;			//出力（解凍データ）は分割しながら書き出す

	//逐次解凍していく
	while(ret != Z_STREAM_END) {
		ret = ::inflate(&z, Z_NO_FLUSH);
		if(ret != Z_OK && ret != Z_STREAM_END) {
			::inflateEnd(&z);			//これをしないと大量のリーク発生をもたらす
			::free(bin);
			Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "解凍に失敗しました。\n>%s\n\n\n>%s", fileName.c_str(), z.msg);
			return false;
		}
		text += (char*)::buf;			//解凍すると改行記号を含む文字列が得られる

		//次の展開準備
		z.next_out = (Bytef*)::buf;
		z.avail_out = availOutSize;		//出力（解凍データ）は分割しながら書き出す
	}

	DxLib::FileRead_close(hdl);
	::inflateEnd(&z);		//これをしないと大量のリーク発生をもたらす
	::free(bin);
#endif

	return true;
}


//####################  非圧縮マップデータ読み込み処理  ####################
bool CSceneMap::loadTextMap(string fileName, string& text) {
	text = "";
	const int size = sizeof(::buf) - 1;
	
#ifdef _DEBUG		//デバッグモードのときは読み込み専用でロードしたいので標準IOを使う

	FILE* fp;
	fp = ::fopen(fileName.c_str(), "r");
	if (fp == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "テキストファイルのオープンに失敗しました。\n>%s", fileName.c_str());
		return false;
	}

	while (::feof(fp) == 0) {
		while (::feof(fp) == 0) {
			::fgets(::buf, size, fp);

			//fgetsでは改行コードも含まれてしまうのでここで除去する
			if (::buf[::strlen(::buf) - 1] == '\n') {
				::buf[::strlen(::buf) - 1] = '\0';
				break;
			}
		}

		text += ::buf;
		text += "\r\n";
	}

	if (fp != nullptr) {
		::fclose(fp);
		fp = nullptr;
	}

#else				//リリースモードのときはDXライブラリアーカイブを開きたいのでDXライブラリのファイル処理関数を使う

	int fp;
	fp = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
	if (fp == 0) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "テキストファイルのオープンに失敗しました。\n>%s", fileName.c_str());
		return false;
	}

	while (DxLib::FileRead_eof(fp) == 0) {
		while (DxLib::FileRead_eof(fp) == 0) {
			DxLib::FileRead_gets(::buf, size, fp);

			//一行分完全に読めたかどうかは、文字列の長さが (size-1) 未満であるかどうかで判断する
			if (::strlen(::buf) < size - 1) {
				break;
			}
		}

		text += ::buf;
		text += "\r\n";
	}

	DxLib::FileRead_close(fp);

#endif

	return true;
}


//####################  標準コマンド：メッセージ表示  ####################
//一旦C++側で受けるが、拡張性を持たせるためにイベントハンドラーを呼び出す
void CSceneMap::ShowMessage(int faceID, string speakerName, string msg) {
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_ShowMessage, faceID, speakerName, msg);
}


//####################  標準コマンド：場所移動  ####################
//一旦C++側で受けるが、拡張性を持たせるためにイベントハンドラーを呼び出す
void CSceneMap::ChangeMap(string fileName, int x, int y, int direction4) {
	this->MapData.ClearEVReserve();			//イベント予約をすべて破棄
	//Game.MT.Clear(false);
	CSceneMap::commitMapSelfValue();		//個別変数情報をコミット
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_ChangeMap, fileName, x, y, direction4);
}


//####################  指定した方向から左を向いた４方向を返す  ####################
int CSceneMap::TurnLeft4(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:		return static_cast<int>(TS::Map::Direction4::North);
		case TS::Map::Direction4::West:		return static_cast<int>(TS::Map::Direction4::South);
		case TS::Map::Direction4::South:	return static_cast<int>(TS::Map::Direction4::East);
		case TS::Map::Direction4::North:	return static_cast<int>(TS::Map::Direction4::West);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  指定した方向から左を向いた８方向を返す  ####################
int CSceneMap::TurnLeft8(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:	return static_cast<int>(TS::Map::Direction8::NorthEast);
		case TS::Map::Direction8::NorthEast:	return static_cast<int>(TS::Map::Direction8::North);
		case TS::Map::Direction8::North:	return static_cast<int>(TS::Map::Direction8::NorthWest);
		case TS::Map::Direction8::NorthWest:	return static_cast<int>(TS::Map::Direction8::West);
		case TS::Map::Direction8::West:	return static_cast<int>(TS::Map::Direction8::SouthWest);
		case TS::Map::Direction8::SouthWest:	return static_cast<int>(TS::Map::Direction8::South);
		case TS::Map::Direction8::South:	return static_cast<int>(TS::Map::Direction8::SouthEast);
		case TS::Map::Direction8::SouthEast:	return static_cast<int>(TS::Map::Direction8::East);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  指定した方向から右を向いた４方向を返す  ####################
int CSceneMap::TurnRight4(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:	return static_cast<int>(TS::Map::Direction4::South);
		case TS::Map::Direction4::West:	return static_cast<int>(TS::Map::Direction4::North);
		case TS::Map::Direction4::South:	return static_cast<int>(TS::Map::Direction4::West);
		case TS::Map::Direction4::North:	return static_cast<int>(TS::Map::Direction4::East);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  指定した方向から右を向いた８方向を返す  ####################
int CSceneMap::TurnRight8(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:	return static_cast<int>(TS::Map::Direction8::SouthEast);
		case TS::Map::Direction8::SouthEast:	return static_cast<int>(TS::Map::Direction8::South);
		case TS::Map::Direction8::South:	return static_cast<int>(TS::Map::Direction8::SouthWest);
		case TS::Map::Direction8::SouthWest:	return static_cast<int>(TS::Map::Direction8::West);
		case TS::Map::Direction8::West:	return static_cast<int>(TS::Map::Direction8::NorthWest);
		case TS::Map::Direction8::NorthWest:	return static_cast<int>(TS::Map::Direction8::North);
		case TS::Map::Direction8::North:	return static_cast<int>(TS::Map::Direction8::NorthEast);
		case TS::Map::Direction8::NorthEast:	return static_cast<int>(TS::Map::Direction8::East);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  指定した方向から反対を向いた４方向を返す  ####################
int CSceneMap::TurnHalf4(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:	return static_cast<int>(TS::Map::Direction4::West);
		case TS::Map::Direction4::West:	return static_cast<int>(TS::Map::Direction4::East);
		case TS::Map::Direction4::South:	return static_cast<int>(TS::Map::Direction4::North);
		case TS::Map::Direction4::North:	return static_cast<int>(TS::Map::Direction4::South);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  指定した方向から反対を向いた８方向を返す  ####################
int CSceneMap::TurnHalf8(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:	return static_cast<int>(TS::Map::Direction8::West);
		case TS::Map::Direction8::SouthEast:	return static_cast<int>(TS::Map::Direction8::NorthWest);
		case TS::Map::Direction8::West:	return static_cast<int>(TS::Map::Direction8::East);
		case TS::Map::Direction8::SouthWest:	return static_cast<int>(TS::Map::Direction8::NorthEast);
		case TS::Map::Direction8::South:	return static_cast<int>(TS::Map::Direction8::North);
		case TS::Map::Direction8::NorthEast:	return static_cast<int>(TS::Map::Direction8::SouthWest);
		case TS::Map::Direction8::North:	return static_cast<int>(TS::Map::Direction8::South);
		case TS::Map::Direction8::NorthWest:	return static_cast<int>(TS::Map::Direction8::SouthEast);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  相対座標から４方向を返す  ####################
int CSceneMap::PointToDirection4(const CPoint& pos) {
	if(pos.X < 0 && (pos.Y == 0 || abs(pos.X) >= abs(pos.Y))) {
		return static_cast<int>(TS::Map::Direction4::West);
	} else if(pos.X > 0 && (pos.Y == 0 || abs(pos.X) > abs(pos.Y))) {
		return static_cast<int>(TS::Map::Direction4::East);
	} else if((pos.X == 0 || abs(pos.X) <= abs(pos.Y)) && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction4::North);
	} else if((pos.X == 0 || abs(pos.X) < abs(pos.Y)) && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction4::South);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  相対座標から８方向を返す  ####################
int CSceneMap::PointToDirection8(const CPoint& pos) {
	if(pos.X < 0 && pos.Y == 0) {
		return static_cast<int>(TS::Map::Direction8::West);
	} else if(pos.X > 0 && pos.Y == 0) {
		return static_cast<int>(TS::Map::Direction8::East);
	} else if(pos.X == 0 && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction8::North);
	} else if(pos.X == 0 && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction8::South);
	} else if(pos.X < 0 && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction8::NorthWest);
	} else if(pos.X > 0 && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction8::NorthEast);
	} else if(pos.X < 0 && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction8::SouthWest);
	} else if(pos.X > 0 && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction8::SouthEast);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  指定した方向への１タイル分の相対座標を返す  ####################
CPoint CSceneMap::Direction4ToPoint(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:		return CPoint(+1, 0);
		case TS::Map::Direction4::West:		return CPoint(-1, 0);
		case TS::Map::Direction4::South:	return CPoint(0, +1);
		case TS::Map::Direction4::North:	return CPoint(0, -1);
	}
	return CPoint();
}


//####################  指定した方向への１タイル分の相対座標を返す  ####################
CPoint CSceneMap::Direction8ToPoint(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:			return CPoint(+1, 0);
		case TS::Map::Direction8::SouthEast:	return CPoint(+1, +1);
		case TS::Map::Direction8::SouthWest:	return CPoint(-1, +1);
		case TS::Map::Direction8::West:			return CPoint(-1, 0);
		case TS::Map::Direction8::South:		return CPoint(0, +1);
		case TS::Map::Direction8::North:		return CPoint(0, -1);
		case TS::Map::Direction8::NorthEast:	return CPoint(+1, -1);
		case TS::Map::Direction8::NorthWest:	return CPoint(-1, -1);
	}
	return CPoint();
}


//####################  ＭＴ実行：イベントページ判定  ####################
bool EVPage(TS_MT) {
	//マップシーンでないときは処理しない
	if(Game.IsSubScene() || Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return false;
	}

	//NOTE: イベント中にフラグが変わったときにページも変えられるように、以下の判定は廃止した。
	//イベント実行中は判定しない
	//if(Game.getSceneMap()->MapData.getIsEVRunning()) {
	//	return false;
	//}

	//全イベントを走査
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		if(ev.IsDummy) {
			continue;		//ダミーイベントは対象外
		}

		//末尾ページから判定
		bool found = false;
		for(int pageIndex = ev.GetPageCount() - 1; pageIndex >= 0; pageIndex--) {
			//判定実行
			if(Game.getSceneMap()->MapData.RunEV(ev.ID, pageIndex, static_cast<int>(TS::Map::EventScriptType::Page), false)) {
				//trueが返されたら有効なページで、ページインデックスが変更になる場合はここで日常移動の内容を更新する
				found = true;
				if(pageIndex != ev.GetPageIndex()) {
					ev.SetPageIndex(pageIndex);
				}
				break;
			}
		}
		if(!found) {
			//非出現状態にすると様々な弊害が起きるため、最初のページでセットする
			ev.SetPageIndex(0);
			
			//いずれも満たさなければ非出現状態にする
			//ev.SetPageIndex(-1);
		}
	}
	return false;
}


//####################  ＭＴ実行：イベント起動判定  ####################
bool EVTrigger(TS_MT) {
	//マップシーンでないときは処理しない
	if(Game.IsSubScene() || Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return false;
	}
	//イベント実行中は判定しない
	if(Game.getSceneMap()->MapData.getIsEVRunning()) {
		return false;
	}

	//予約されているイベントがあればそれを先に実行する
	int EVID;
	while((EVID = Game.getSceneMap()->MapData.PopReserveEVID()) != -1) {
		Game.getSceneMap()->MapData.RunEV(EVID, static_cast<int>(TS::Map::EventScriptType::Run), true);
	}

	//全イベントを走査：自動・並列実行のみを判定
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		auto pg = ev.getCurrentPage();
		if(pg == nullptr) {
			//未出現イベントは除外
			continue;
		} else if(pg->TriggerType != static_cast<int>(TS::Map::EventTriggerType::Auto) && pg->TriggerType != static_cast<int>(TS::Map::EventTriggerType::Parallel)) {
			//対象外の起動条件は除外
			continue;
		}

		//判定実行
		if(Game.getSceneMap()->MapData.RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Trigger), false)) {
			//trueが返されたら起動する
			if(ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Parallel)) {
				//並列実行イベントは実行中のイベントIDスタックに登録しない（＝操作停止しない）
				Game.getSceneMap()->MapData.RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Run), false);
			} else {
				//自動実行イベントは実行中のイベントIDスタックに登録する（＝操作停止する）
				Game.getSceneMap()->MapData.RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Run), true);
			}
		}
	}
	return false;
}


//####################  ＭＴ実行：イベント日常移動  ####################
bool EVMove(TS_MT) {
	int temp;

	//マップシーンでないときは処理しない
	if(Game.IsSubScene() || Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return false;
	}

	//プレイヤーが移動中のときはその移動を継続する
	auto& player = Game.getSceneMap()->MapData.Player;
	if(player.MapObject.getIsMoving()) {
		//移動中の場合はその移動を継続する・主人公の移動に合わせたスクロールのカウンターもここで動かす
		int delta;

		delta = 0;
		if(player.MapObject.MoveCounter.X < 0) {
			delta = static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.X = min(0, player.MapObject.MoveCounter.X + delta);
		} else if(player.MapObject.MoveCounter.X > 0) {
			delta = -static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.X = max(0, player.MapObject.MoveCounter.X + delta);
		}
		Game.getSceneMap()->MapData.PictureScrollCounter.X += delta;

		delta = 0;
		if(player.MapObject.MoveCounter.Y < 0) {
			delta = static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.Y = min(0, player.MapObject.MoveCounter.Y + delta);
		} else if(player.MapObject.MoveCounter.Y > 0) {
			delta = -static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.Y = max(0, player.MapObject.MoveCounter.Y + delta);
		}
		Game.getSceneMap()->MapData.PictureScrollCounter.Y += delta;

		//移動が終了したら一致起動イベントの判定を行う
		if(!player.MapObject.getIsMoving() && !Game.getSceneMap()->MapData.getIsEVRunning()) {
			Game.getSceneMap()->MapData.CheckOverlapTriggerEV(-1, player.MapObject.Position.X, player.MapObject.Position.Y);
		}
	}

	//全イベントを走査
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		auto pg = ev.getCurrentPage();
		if(pg == nullptr) {
			//未出現イベントは除外
			continue;
		}

		if(ev.getIsMoving() && !ev.ForbiddenAutoMove) {
			//移動カウンターの自動処理が有効で、かつ移動中のときはその移動を継続する
			if(ev.MoveCounter.X < 0) {
				ev.MoveCounter.X = min(0, ev.MoveCounter.X + (int)pow(2.0, pg->MoveSpeed));
			} else if(ev.MoveCounter.X > 0) {
				ev.MoveCounter.X = max(0, ev.MoveCounter.X - (int)pow(2.0, pg->MoveSpeed));
			}
			if(ev.MoveCounter.Y < 0) {
				ev.MoveCounter.Y = min(0, ev.MoveCounter.Y + (int)pow(2.0, pg->MoveSpeed));
			} else if(ev.MoveCounter.Y > 0) {
				ev.MoveCounter.Y = max(0, ev.MoveCounter.Y - (int)pow(2.0, pg->MoveSpeed));
			}

			//移動が終了したら一致起動イベントの判定を行う
			if(!ev.getIsMoving() && !Game.getSceneMap()->MapData.getIsEVRunning()) {
				Game.getSceneMap()->MapData.CheckOverlapTriggerEV(ev.ID, ev.Position.X, ev.Position.Y);
			}
		}

		//イベント実行中でないときのみ、新規移動を行う
		if(!ev.getIsMoving() && !Game.getSceneMap()->MapData.getIsEVRunning()) {
			if(ev.getCurrentPage()->MoveType == static_cast<int>(TS::Map::EventMoveType::Script)) {
				//スクリプト指定の移動を行う
				auto task = ev.getNextMoveTask();
				if(task != nullptr) {
					ev.MoveTaskNowIndex = ev.moveScript(*task, ev.MoveTaskNowIndex, true);
				}
			} else {
				//規定ウェイト時間まで達したら典型移動を開始する
				ev.MoveWaitTimer++;

				//移動頻度に応じた判定: 待ち時間フレーム数を算出
				temp = TS::Map::EVMoveFrequencyFrameLatest - (ev.getCurrentPage()->MoveFrequency * TS::Map::EVMoveFrequencyFrameLatest / (static_cast<int>(TS::Map::Speed::Moment) + 1));

				if(temp <= ev.MoveWaitTimer) {
					switch(static_cast<TS::Map::EventMoveType>(ev.getCurrentPage()->MoveType)) {
						case TS::Map::EventMoveType::Random:
							ev.moveRandom();
							break;
						case TS::Map::EventMoveType::Near:
							ev.moveNear();
							break;
						case TS::Map::EventMoveType::Away:
							ev.moveAway();
							break;
					}
					ev.MoveWaitTimer = 0;

					if(ev.getIsMoving()) {
						//マップの範囲外に出たらマップのループ設定に従って補正する
						ev.ApplyMapLoopPosition();
					}
				}
			}
		}
	}
	return false;
}


//####################  ＭＴ実行：アニメーション  ####################
//Vint["counter"]
bool Anim(TS_MT) {
	int animSpeed;
	const int FrequencyFrameLatest = TS::Map::EVMoveFrequencyFrameLatest / 2;

	//マップタイルのアニメーションパターン数はタイルごとに異なるため、描画時に計算する
	*Data.GetInteger("counter") = Game.UpAround(*Data.GetInteger("counter"), 0, INT_MAX);
	Game.getSceneMap()->MapData.setAnimCounter(*Data.GetInteger("counter"));

	//主人公・イベントのアニメーションを行う
	for(int i = -1; i < Game.getSceneMap()->MapData.EVs.GetCount(); i++) {
		CSceneMap::CMapData::CMapObjectData* ev;
		CSceneMap::CMapData::CMapCharObjectData* pg;
		int frequency;
		if(i == -1) {
			//主人公の情報をセット
			ev = &Game.getSceneMap()->MapData.Player.MapObject;
			pg = &Game.getSceneMap()->MapData.Player.CharData;
			frequency = static_cast<int>(TS::Map::Speed::Normal);
		} else {
			//イベントの情報をセット
			ev = Game.getSceneMap()->MapData.EVs.GetFromIndex(i);
			pg = Game.getSceneMap()->MapData.EVs.GetFromIndex(i)->getCurrentPage();
			if(pg == nullptr || dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(pg) == nullptr) {
				continue;
			}
			frequency = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(pg)->MoveFrequency;
		}

		if(!ev->getIsMoving()) {
			//静止アニメーション
			if(!pg->EnabledStopAnim) {
				//アニメーションを行わない場合は初期状態を保つが、それ以外は静止状態にする
				if(pg->EnabledWalkAnim) {
					pg->AnimID = static_cast<int>(TS::Map::EventAnimation::Stop);
				}
				continue;
			}
			animSpeed = (FrequencyFrameLatest - (FrequencyFrameLatest * frequency / (static_cast<int>(TS::Map::Speed::Moment) + 1)));
			pg->AnimID = ((*Data.GetInteger("counter") % animSpeed) / (animSpeed / 2) == 0) ? static_cast<int>(TS::Map::EventAnimation::Move1) : static_cast<int>(TS::Map::EventAnimation::Move2);
		} else {
			//歩行アニメーション
			if(!pg->EnabledWalkAnim) {
				//アニメーションを行わない場合は初期状態を保つが、それ以外は静止状態にする
				if(pg->EnabledStopAnim) {
					pg->AnimID = static_cast<int>(TS::Map::EventAnimation::Stop);
				}
				continue;
			}
			animSpeed = (FrequencyFrameLatest - (FrequencyFrameLatest * static_cast<int>(TS::Map::Speed::Moment) / (static_cast<int>(TS::Map::Speed::Moment) + 1)));
			pg->AnimID = ((*Data.GetInteger("counter") % animSpeed) / (animSpeed / 2) == 0) ? static_cast<int>(TS::Map::EventAnimation::Move1) : static_cast<int>(TS::Map::EventAnimation::Move2);
		}
	}

	return false;
}


//####################  コールバック関数：フルスクリーンモード切替でMakeScreenしたグラフィックが失われるので復元する  ####################
void RestoreTilePallet() {
	DxLib::ReloadFileGraphAll();			//失った画像データを取り戻す
	if(Game.getSceneMap() != nullptr) {
		Game.getSceneMap()->MapData.TilePallet = Game.DB.getFDBTileset()->LoadTileset(Game.getSceneMap()->MapData.TilesetID);
	}
}
