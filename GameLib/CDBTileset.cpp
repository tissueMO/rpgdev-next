//###################################################################
//			タイルセット データベース
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  タイルセットを破棄する  ####################
void CDatabase::CDBTileset::Dispose() {
	if(this != nullptr) {
		if(this->DBs.GetCount() > 0) {
			this->DBs.GetFromIndex(0)->ClearRow();
		}
		for(auto& tile : this->activeTilesetData) {
			vector<CTileData>().swap(tile);
		}
		vector<TilesetRow>().swap(this->activeTilesetData);
		this->DisposeTilePallet();
	}
}


//####################  指定したIDのタイルセットを読み込む  ####################
CDatabase::CDBTileset::CTilePallet* CDatabase::CDBTileset::LoadTileset(int id) {
	this->ActiveTilesetID = id;
	this->loadActiveTilesetFile();
	return this->loadActiveTilesetGraph();
}


//####################  現在のタイルパレットの指定位置の情報を取得  ####################
CDatabase::CDBTileset::CTileData* CDatabase::CDBTileset::GetTilePallet(int tileX, int tileY) {
	if(tileX < 0 || TS::Map::TileColumnCount <= tileX || tileY < 0 || this->activeTilesetData.size() <= tileY) {
		return nullptr;
	} else {
		return &this->activeTilesetData[tileY][tileX];
	}
}


//####################  タイルパレットのタイル単位のサイズを取得  ####################
CSize CDatabase::CDBTileset::GetTilePalletSize() {
	return CSize(TS::Map::TileColumnCount, this->activeTilesetData.size());
}


//####################  オートタイルのハンドル群をスクリプト用に生成した返す  ####################
CIntegerArray CDatabase::CDBTileset::CTilePallet::getAutoTiles() {
	CIntegerArray ret;
	for(auto at : this->AutoTiles) {
		ret.Add(at);
	}
	return ret;
}


//####################  タイル情報の生データから正規のデータへ  ####################
CDatabase::CDBTileset::CTileData CDatabase::CDBTileset::setData(string raw) {
	CTileData result = CTileData();
	vector<string> spl;

	//生データを分割
	Game.SplitString(spl, raw, ";");
	if(spl.size() >= 4) {
		result.Hit = atoi(spl[0].c_str());
		result.InOut = atoi(spl[1].c_str());
		result.Priority = atoi(spl[2].c_str());
		result.Tag = atoi(spl[3].c_str());
		result.AnimPatternCount = 1;
	}
	return result;
}


//####################  現在のタイルパレットを破棄する  ####################
void CDatabase::CDBTileset::DisposeTilePallet() {
	if(this->loadedPallet != nullptr) {
		if(Game.Material.getGraphics() != nullptr) {
			for(auto& hdl : this->loadedPallet->AutoTiles) {
				Game.Material.getGraphics()->Delete(hdl);
			}
		}
		DxLib::DeleteGraph(this->loadedPallet->TilesetHandle);
		delete this->loadedPallet;
		this->loadedPallet = nullptr;
	}
}


//####################  現在アクティブになっているタイルセット定義ファイルを読み込む  ####################
void CDatabase::CDBTileset::loadActiveTilesetFile() {
	//前回のタイルセットは破棄する
	this->activeTilesetData.clear();
	if(!this->openFile(Game.Format(TS::FileNames::TilesetData, this->ActiveTilesetID))) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "タイルセット定義ファイルのオープンに失敗しました。\n>%s", Game.Format(TS::FileNames::TilesetData, this->ActiveTilesetID).c_str());
		return;
	}

	//1行目から127回、行単位取得、FixedID|WxH|タイル情報
	vector<string> spl;
	vector<string> subspl;
	TS_COUNTLOOP(y, TS::Map::AutoTileRowLength) {
		TilesetRow row;
		TS_COUNTLOOP(x, TS::Map::TileColumnCount) {
			row.push_back(CTileData());
			if(x == 0 && y == 0) {
				//空白タイルの定義
				row[x].Hit = static_cast<int>(TS::Map::HitType::OK);
				row[x].InOut = static_cast<int>(TS::Map::InOutType::OKBottom) | static_cast<int>(TS::Map::InOutType::OKLeft) | static_cast<int>(TS::Map::InOutType::OKRight) | static_cast<int>(TS::Map::InOutType::OKTop);
				row[x].Priority = static_cast<int>(TS::Map::TileDrawPriority::Normal);
				row[x].Tag = 0;
				row[x].AnimPatternCount = 0;
			} else {
				this->gets(::buf, sizeof(::buf) - 1);	//１行読み込み

				//区切り記号で分割
				Game.SplitString(spl, ::buf, "|");
				if(spl.size() >= 3) {
					//タイル情報の生データから正規のデータを作る
					row[x] = this->setData(spl[2]);

					//横方向の長さからアニメーションパターン数を得る
					row[x].AnimPatternCount = atoi(spl[1].substr(0, spl[1].find("x")).c_str());
				}
			}
		}
		this->activeTilesetData.push_back(row);
	}

	//以降、オブジェクトタイル、FixedID|WxH
	CSize setSize;
	while(true) {
		this->gets(::buf, sizeof(::buf) - 1);		//１行読み込み
		if(this->isEOF()) {
			break;				//終端まで読み込んだ
		}

		//オブジェクトタイルの縦横幅を確認
		Game.SplitString(spl, ::buf, "|");
		Game.SplitString(subspl, spl[1], "x");
		setSize.Set(
			atoi(subspl[0].c_str()),
			atoi(subspl[1].c_str())
			);

		TS_COUNTLOOP(y, setSize.Height) {
			TilesetRow row;
			this->gets(::buf, sizeof(::buf) - 1);	//１行読み込み

			//区切り記号で分割
			Game.SplitString(spl, ::buf, ",");
			TS_COUNTLOOP(x, spl.size()) {
				//タイル情報の生データから正規のデータを作る
				row.push_back(this->setData(spl[x]));
			}
			this->activeTilesetData.push_back(row);
		}
	}
	this->closeFile();
}


//####################  現在アクティブなタイルセットのパレットを生成する  ####################
CDatabase::CDBTileset::CTilePallet* CDatabase::CDBTileset::loadActiveTilesetGraph() {
	//前回のタイルパレットグラフィックを破棄する
	this->DisposeTilePallet();

	this->loadedPallet = new CTilePallet();
	auto activeTileRow = this->DBs.GetFromIndex(0)->GetRowFromID(this->ActiveTilesetID);

	//サイズ情報を取得
	auto palletSize = this->GetTilePalletSize();
	auto tileSize = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::TileSize);

	//読み込み
	string strbuf;
	vector<string> at;
	strbuf = *activeTileRow->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBTilesetColumnIndices::AutoTiles)).GetStringPTR();
	Game.SplitString(at, strbuf, "|");
	vector<string> obj;
	strbuf = *activeTileRow->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBTilesetColumnIndices::ObjTiles)).GetStringPTR();
	Game.SplitString(obj, strbuf, "|");

	//オブジェクトタイルの生成準備
	vector<CSize> sizeList;
	vector<int> objHandles;
	int tilesetHeight = tileSize * TS::Map::AutoTileRowLength;

	//オブジェクトタイルのグラフィックとそのサイズを読み込む
	TS_COUNTLOOP(i, obj.size()) {
		int x, y;
		objHandles.push_back(DxLib::LoadGraph((TS::DirectoryNames::PrePath + obj[i]).c_str()));
		DxLib::GetGraphSize(objHandles[i], &x, &y);
		sizeList.push_back(CSize(x, y));
		tilesetHeight += y;
	}

	//タイルパレット用のスクリーンを生成し、一時的に描画先を変更する
	this->loadedPallet->TilesetHandle = DxLib::MakeScreen(tileSize * palletSize.Width, tilesetHeight, true);
	DxLib::SetDrawScreen(this->loadedPallet->TilesetHandle);
	DxLib::SetUsePremulAlphaConvertLoad(true);

	//オートタイル読み込み：統一パレットに描画しつつ、別個にもハンドルを控えておく
	TS_COUNTLOOP(x, TS::Map::TileColumnCount) {
		TS_COUNTLOOP(y, TS::Map::AutoTileRowLength) {
			this->loadedPallet->AutoTiles[x + y * TS::Map::TileColumnCount] = -1;
			TS_CHECKINDEX(x + y * TS::Map::TileColumnCount, 1, at.size()) {
				//splの要素数は、左上の透明タイルを抜くので一つ少ないことに注意
				this->loadedPallet->AutoTiles[x + y * TS::Map::TileColumnCount] = Game.Material.getGraphics()->Add(at[x + y * TS::Map::TileColumnCount - 1]);
				DxLib::DrawGraph(x * tileSize, y * tileSize, this->loadedPallet->AutoTiles[x + y * TS::Map::TileColumnCount], true);
			}
		}
	}

	//オブジェクトタイルは一連のグラフィックとして描画
	int amount = tileSize * TS::Map::AutoTileRowLength;
	TS_COUNTLOOP(i, obj.size()) {
		//描画したらすぐに破棄する
		DxLib::DrawGraph(0, amount, objHandles[i], true);
		DxLib::DeleteGraph(objHandles[i]);
		amount += sizeList[i].Height;
	}

	//描画先を元に戻して完了する
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	TS_NOBLEND;
	DxLib::SetUsePremulAlphaConvertLoad(false);

	return this->loadedPallet;
}
