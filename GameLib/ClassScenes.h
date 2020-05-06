//###################################################################
//		各種シーン
//###################################################################

//####################  シーンの基底クラス  ####################
SQ_ABSTRACT class CSceneBase : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_NOBIND int StartTime;
	SQ_NOBIND int SQHandle;
	SQ_NOBIND int SQSubHandle;
	SQ_NOBIND int SubSceneID;
	SQ_PROPGET int getSubSceneID() { return this->SubSceneID; }

	SQ_NOBIND CSoundData BGM;
	SQ_PROPGET CSoundData* getBGM() { return &this->BGM; }

	SQ_INSTANCEVALUE string BeforeBGMFileName;
	SQ_INSTANCEFUNC void PlayBGM(string fileName, int volume, int pitch, int pan) {
		this->BGM.Load(fileName, volume, pitch, pan);
		this->BGM.Play(static_cast<int>(TS::Material::PlayType::Loop), false);
	}

	SQ_NOBIND CSoundData BGS;
	SQ_PROPGET CSoundData* getBGS() { return &this->BGS; }

	SQ_INSTANCEVALUE string BeforeBGSFileName;
	SQ_INSTANCEFUNC void PlayBGS(string fileName, int volume, int pitch, int pan) {
		this->BGS.Load(fileName, volume, pitch, pan);
		this->BGS.Play(static_cast<int>(TS::Material::PlayType::Loop), false);
	}

	//コンストラクター
	SQ_NOBIND CSceneBase();

	//メンバーメソッド
	SQ_NOBIND virtual void Draw();
	SQ_NOBIND virtual void Input();
	SQ_NOBIND virtual void Setup();
	SQ_NOBIND virtual void Dispose();

	//純粋仮想関数
	SQ_INSTANCEFUNC virtual string GetName() = 0;
};


//####################  シーン：タイトル  ####################
class CSceneTitle : public CSceneBase {
public:
	//SQ_BEGINMEMBER
	//メンバーメソッド
	SQ_NOBIND void Setup() override;
	SQ_NOBIND string GetName() override { return "Title"; }
};


//####################  シーン：マップ  ####################
class CSceneMap : public CSceneBase {
public:
	//内部蓄積データベース用：マップ情報も含むイベント個別変数の単体情報
	//SQ_NOBIND_BEGIN
	class CEVSelfValueOneData {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		string MapGUID;
		int EVID;
		CIDValueData Value;

		//コンストラクター
		CEVSelfValueOneData(string mapGUID, int EVID, CIDValueData *baseData) {
			this->MapGUID = mapGUID;
			this->EVID = EVID;
			this->Value = *baseData;
		}
	};
	//SQ_NOBIND_END

	//マップ情報
	class CMapData {
	public:
		//１タイルあたりのタイル情報
		//SQ_NOBIND_BEGIN
		class CTileData {
		public:
			using QuarterArray = array<int, static_cast<int>(TS::Map::QuarterTile::Count)>;

			//コンストラクター
			CTileData() { TS_COUNTLOOP(i, this->Quarter.size()) this->Quarter[i] = static_cast<int>(TS::Map::AutoTilePattern::Isolate); }

			//メンバー変数
			CPoint MapPosition;
			CPoint PalletPosition;
			QuarterArray Quarter;
		};
		//SQ_NOBIND_END

		//移動の指示内容
		class CEventMoveTask {
		public:
			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_INSTANCEVALUE int TaskType;
			SQ_INSTANCEVALUE int OptionInteger;
			SQ_INSTANCEVALUE string OptionString;

			//コンストラクター
			SQ_NOBIND CEventMoveTask() = default;
			SQ_NOBIND CEventMoveTask(int type, int par1, string par2) {
				this->TaskType = type;
				this->OptionInteger = par1;
				this->OptionString = par2;
			}
		};

		//マップオブジェクト情報
		SQ_NOBIND class CMapCharObjectData;		//前方宣言
		class CMapObjectData : public CIDNameData {
		public:
			//SQ_BEGINMEMBER
			//デストラクター
			SQ_NOBIND ~CMapObjectData() {
				vector<CEventMoveTask>().swap(this->moveExtraTask);
			}


			//メンバー変数
			SQ_NOBIND CMapCharObjectData* TargetCharPTR = nullptr;

			SQ_NOBIND CPoint Position;
			SQ_PROPGET CPoint* getPosition() { return &this->Position; }

			SQ_NOBIND CPoint PositionOnPixel;
			SQ_PROPGET CPoint* getPositionOnPixel() { return &this->PositionOnPixel; }

			SQ_NOBIND CPoint MoveCounter;
			SQ_PROPGET CPoint* getMoveCounter() { return &this->MoveCounter; }

			SQ_INSTANCEVALUE bool Hide = false;
			SQ_INSTANCEVALUE bool ForbiddenAutoMove = false;		//移動カウンターを自動で処理するかどうか
			SQ_INSTANCEVALUE int MoveWaitTimer = 0;					//移動待ちカウンター


			//メンバーメソッド
			SQ_NOBIND int moveScript(CEventMoveTask task, int tasknowIndex, bool EVTrigger);
			SQ_NOBIND int getExtraMoveTaskCount() { return this->moveExtraTask.size(); }
			SQ_NOBIND CEventMoveTask getExtraMoveTask(int index);
			SQ_NOBIND void clearExtraMoveTask() { this->moveExtraTask.clear(); }
			SQ_INSTANCEFUNC void DoExtraMove(bool waitMode);
			SQ_INSTANCEFUNC void AutoSetMoveCounter(int direction4);
			SQ_PROPGET bool getIsMoving() { return (!this->ForbiddenAutoMove && !this->MoveCounter.IsEmpty()); }
			SQ_INSTANCEFUNC CMapObjectData* SetExtraMove(int type, int par1, string par2);
			SQ_INSTANCEFUNC void ApplyMapLoopPosition();

		protected:
			vector<CEventMoveTask> moveExtraTask;	//方向移動コマンドの指示リスト
			bool routeLock = false;					//方向移動が遮られたとき、スキップせずに待つかどうか
		};

		//人イベント情報
		class CMapCharObjectData : public CNullCheck {
		public:
			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_INSTANCEVALUE int MoveSpeed = 0;
			SQ_INSTANCEVALUE int AnimID = 0;
			SQ_INSTANCEVALUE bool EnabledStopAnim = false;
			SQ_INSTANCEVALUE bool EnabledWalkAnim = false;
			SQ_INSTANCEVALUE bool NoHit = false;
			SQ_INSTANCEVALUE bool FixedDirection = false;
			SQ_INSTANCEVALUE CSize OneCharSize;

			SQ_NOBIND string Graphics;
			SQ_PROPGET string getGraphics() { return this->Graphics; }

			SQ_NOBIND int Direction = -1;
			SQ_PROPGET int getDirection() { return this->Direction; } SQ_PROPSET void setDirection(int val) { if(!this->FixedDirection) this->Direction = val; }

			SQ_NOBIND int GraphHandle = -1;
			SQ_PROPGET int getGraphHandle() { return this->GraphHandle; }


			//メンバーメソッド
			SQ_INSTANCEFUNC virtual void LoadGraphics(string fileName);
			SQ_NOBIND virtual void DisposeGraphics();
		};

		//プレイヤーキャラ情報
		class CMapPlayer {
		public:
			//SQ_BEGINMEMBER
			//コンストラクター
			SQ_NOBIND CMapPlayer() { this->MapObject.TargetCharPTR = &this->CharData; }

			//メンバー変数
			SQ_NOBIND CMapObjectData MapObject;
			SQ_PROPGET CMapObjectData* getMapObject() { return &this->MapObject; }

			SQ_NOBIND CMapCharObjectData CharData;
			SQ_PROPGET CMapCharObjectData* getCharData() { return &this->CharData; }
		};

		//タイルデータ
		typedef array_multi<CTileData, TS::Map::MapLayerCountWithoutEV, TS::Map::MapSizeMaxLength, TS::Map::MapSizeMaxLength> TileArray;

		//イベント情報
		class CMapEventData : public CSceneMap::CMapData::CMapObjectData {
		public:
			//イベントページ情報：物イベントの要素も兼ねる
			class CMapEventPageData : public CSceneMap::CMapData::CMapCharObjectData {
			public:
				//SQ_BEGINMEMBER
				//メンバー変数
				SQ_INSTANCEVALUE int TriggerType;
				SQ_INSTANCEVALUE int MoveType;
				SQ_INSTANCEVALUE int MoveFrequency;
				SQ_INSTANCEVALUE int DrawPriority;
				SQ_INSTANCEVALUE CPoint TilePalletPosition;

				SQ_NOBIND bool IsTilesetGraphics;
				SQ_PROPGET bool getIsTilesetGraphics() { return this->IsTilesetGraphics; }

				SQ_NOBIND CSize HitRect;
				SQ_PROPGET CSize* getHitRect() { return &this->HitRect; }

				//メンバーメソッド
				SQ_INSTANCEFUNC void LoadGraphics(string fileName) override;
				SQ_NOBIND void DisposeGraphics() override;
			};

			//SQ_BEGINMEMBER
			//コンストラクター・デストラクター
			SQ_NOBIND CMapEventData() = default;
			SQ_NOBIND CMapEventData(vector<string>& buf);
			SQ_NOBIND ~CMapEventData() {
				vector<CEventMoveTask>().swap(this->moveTask);
				vector<CMapEventPageData>().swap(this->pages);
			}

			//メンバー変数
			SQ_NOBIND bool IsDummy = false;							//ダミーイベント（表示だけして、実行内容・起動条件・日常移動を定義しないイベント）であるかどうか
			SQ_PROPGET bool getIsDummy() { return this->IsDummy; }

			SQ_INSTANCEVALUE int VisibleID;
			SQ_NOBIND int MoveTaskNowIndex = -1;					//日常移動の指示リストにおける現在のインデックス

			SQ_NOBIND CSelfValueData SelfValues;					//イベント個別変数
			SQ_PROPGET CSelfValueData* getSelfValues() { return &this->SelfValues; }

			//メンバーメソッド
			SQ_INSTANCEFUNC void SetPageIndex(int val);
			SQ_INSTANCEFUNC int GetPageIndex();
			SQ_INSTANCEFUNC int GetPageCount();
			SQ_INSTANCEFUNC void AddPage(CMapEventPageData pg);
			SQ_INSTANCEFUNC CMapEventData* AddMoveTask(int type, int par1, string par2);
			SQ_INSTANCEFUNC void SetDirection4(int dir4);
			SQ_INSTANCEFUNC void SetDirection4ToPlayer();
			SQ_INSTANCEFUNC void SetDirection4AgainstPlayer();
			SQ_INSTANCEFUNC int GetDirection4ToPlayer();
			SQ_PROPGET bool getRouteLock() { return this->routeLock; }
			SQ_PROPGET CMapEventPageData* getCurrentPage();

			//セミプライベートメソッド
			SQ_NOBIND void Dispose();
			SQ_NOBIND void moveRandom();
			SQ_NOBIND void moveNear();
			SQ_NOBIND void moveAway();
			SQ_NOBIND void toString(string& buf);
			SQ_NOBIND string toString();
			SQ_NOBIND int getMoveTaskCount() { return this->moveTask.size(); }
			SQ_NOBIND CEventMoveTask* getNextMoveTask();

		private:
			//メンバー変数
			int pageIndex = -1;						// -1 のとき非出現状態
			vector<CEventMoveTask> moveTask;		//日常移動の指示リスト
			vector<CMapEventPageData> pages;		//イベントページ
			CPoint relativePosition;				//初期位置からの相対的な距離
		};

		//SQ_BEGINMEMBER
		SQ_NOBIND CMapData();

		//メンバー変数
		SQ_NOBIND int TileSize;
		SQ_PROPGET int getTileSize() { return this->TileSize; }

		SQ_NOBIND int ShadowTransparent;
		SQ_PROPGET int getShadowTransparent() { return this->ShadowTransparent; }

		SQ_NOBIND int FogTransparent;
		SQ_PROPGET int getFogTransparent() { return this->FogTransparent; }

		SQ_NOBIND CPoint MapPixelOffset;						//現在画面上に見えている左上のマップピクセル座標
		SQ_PROPGET CPoint* getMapPixelOffset() { return &this->MapPixelOffset; }

		SQ_NOBIND CDatabase::CDBTileset::CTilePallet* TilePallet;
		SQ_PROPGET CDatabase::CDBTileset::CTilePallet* getTilePallet() { return this->TilePallet; }

		SQ_NOBIND CPoint PictureScrollCounter;
		SQ_INSTANCEVALUE string GUID;

		SQ_NOBIND CSize MapSize;
		SQ_PROPGET CSize* getMapSize() { return &this->MapSize; }

		SQ_INSTANCEVALUE int MapLoopOption;

		SQ_NOBIND int TilesetID;
		SQ_PROPGET int getTilesetID() { return this->TilesetID; }

		SQ_NOBIND int MoveSpeed;
		SQ_PROPGET int getMoveSpeed() { return this->MoveSpeed; }

		SQ_NOBIND string BaseMapFileName;
		SQ_PROPGET string getBaseMapFileName() { return this->BaseMapFileName; }

		SQ_INSTANCEVALUE string CurrentMapName;

		SQ_NOBIND string CurrentMapFileName;
		SQ_PROPGET string getCurrentMapFileName() { return this->CurrentMapFileName; }

		SQ_NOBIND string BGMRawString;
		SQ_PROPGET string getFirstMapBGMRawString() { return this->BGMRawString; }

		SQ_NOBIND string BGSRawString;
		SQ_PROPGET string getFirstMapBGSRawString() { return this->BGSRawString; }

		SQ_NOBIND int BackHandle;
		SQ_PROPGET int getBackHandle() { return this->BackHandle; }

		SQ_NOBIND string CurrentMapBackFileName;
		SQ_PROPGET string getCurrentMapBackFileName() { return this->CurrentMapBackFileName; }

		SQ_INSTANCEVALUE int BackScrollType;
		SQ_INSTANCEVALUE int BackScrollSpeed;

		SQ_NOBIND int FogHandle;
		SQ_PROPGET int getFogHandle() { return this->FogHandle; }

		SQ_NOBIND string CurrentFogFileName;
		SQ_PROPGET string getCurrentFogFileName() { return this->CurrentFogFileName; }

		SQ_INSTANCEVALUE int FogScrollType;
		SQ_INSTANCEVALUE int FogScrollSpeed;
		SQ_INSTANCEVALUE int FogBlend;

		SQ_NOBIND CIntegerArray Units;
		SQ_PROPGET CIntegerArray* getUnits() { return &this->Units; }

		SQ_INSTANCEVALUE int EncounterRate;

		SQ_NOBIND string BattleBackFileName;
		SQ_PROPGET string getBattleBackFileName() { return this->BattleBackFileName; }

		SQ_NOBIND CMapPlayer Player;
		SQ_PROPGET CMapPlayer* getPlayer() { return &this->Player; }

		SQ_NOBIND Cstdvector<CMapEventData> EVs;


		//メンバーメソッド
		SQ_NOBIND void Dispose();
		SQ_PROPGET int getAnimCounter() { return this->animCnt; } SQ_PROPSET void setAnimCounter(int value) { this->animCnt = value; }
		SQ_INSTANCEFUNC void LoadMap(string fileName, bool exceptEV);
		SQ_INSTANCEFUNC void AfterLoadMap();
		SQ_NOBIND void PlayBGMBGS();

		//タイル系
		SQ_INSTANCEFUNC CPoint GetTile(int layer, int x, int y);
		SQ_INSTANCEFUNC void SetTile(int layer, int x, int y, int tilex, int tiley, bool absolute);
		SQ_INSTANCEFUNC int GetQuarterTile(int layer, int x, int y, int q);
		SQ_INSTANCEFUNC void SetQuarterTile(int layer, int x, int y, int q, int val);
		SQ_INSTANCEFUNC bool IsWalkableTile(int x, int y);
		SQ_INSTANCEFUNC int CheckTileGroupNum(int x, int y, int groupNum);
		SQ_INSTANCEFUNC int GetTileGroupNum(int layer, int x, int y);

		//描画系
		SQ_INSTANCEFUNC void DrawMap(int tileAnimLoopFrame);
		SQ_INSTANCEFUNC void LoadBack(string fileName);
		SQ_INSTANCEFUNC void DrawBack();
		SQ_INSTANCEFUNC void LoadFog(string fileName);
		SQ_INSTANCEFUNC void DrawFog();

		//イベントリスト系
		SQ_INSTANCEFUNC int EVCount() { return this->EVs.GetCount(); }
		SQ_INSTANCEFUNC int EVReservedCount() { return this->reservedEVs.size(); }
		SQ_INSTANCEFUNC int GetEVFixedID(int visibleID);
		SQ_INSTANCEFUNC void EVClear();
		SQ_INSTANCEFUNC void EVDeleteFromID(int id);
		SQ_INSTANCEFUNC bool EVExists(int id) { return this->EVs.Exists(id); }
		SQ_INSTANCEFUNC CMapEventData* EVGetData(int id) { return this->EVs.GetFromID(id); }
		SQ_INSTANCEFUNC CMapEventData* EVGetDataFromIndex(int idx) { return this->EVs.GetFromIndex(idx); }
		SQ_INSTANCEFUNC void ReserveEV(int EVID);
		SQ_NOBIND void ClearEVReserve() { this->reservedEVs.clear(); }
		SQ_INSTANCEFUNC bool RunEV(int EVID, int type, bool pushRunningStack);
		SQ_NOBIND bool RunEV(int EVID, int pageIndex, int type, bool pushRunningStack);
		SQ_INSTANCEFUNC void SetEV(int EVID, int visibleID, int x, int y, string name);
		SQ_INSTANCEFUNC void AddEVPage(int EVID, string param, string graph);
		SQ_INSTANCEFUNC int SetDummyEV(int x, int y, string graph, int direction4, int animID, int priority, bool enabledStopAnim, bool enabledWalkAnim, bool fixedDirection);
		SQ_PROPGET bool getIsEVRunning() { return (this->runningEVNest.size() > 0); }		//ユーザー操作を停止して実行するイベントのみ検索対象とする

		SQ_INSTANCEFUNC void ClearDummyEVs();
		SQ_PROPGET CMapEventData* getThisEVData();
		SQ_NOBIND int PopReserveEVID();
		SQ_NOBIND void PrintEVData();

		//イベント動作系
		SQ_INSTANCEFUNC bool CheckBeforeMove(int EVID, int x, int y, int direction4, bool EVTrigger);
		SQ_INSTANCEFUNC bool CheckOverlapTriggerEV(int EVID, int x, int y);
		SQ_INSTANCEFUNC bool CheckPressEnterTriggerEV();

	private:
		//メンバー変数
		int animCnt = 0;
		int SQHandle = -1;
		TileArray* Data;
		vector<int> reservedEVs;
		vector<int> runningEVNest;									//操作を停止して実行するイベントの呼び出し階層
		vector<CMapEventData*> runningEVNest_system;				//操作を停止しないイベントも含むすべての呼び出し階層

		//プライベートメソッド
		static void loadTileFile(string fileName, CMapData& dest, vector<string>& plain);
		bool GetOneMovePosition(CPoint& beforePos, CPoint& afterPos, int direction4);
		void updateAutoTile(int layer, int x, int y, bool isParent);
		void drawMapPicture(int handle, int scrollType, CPoint& scrollCounter, int blendMode, int blendParam);
		void refreshAllEVTargetChar();

		//マップ描画の効率化に使う
		const int offset = 1;
		CPoint playerViewPixelPos;
		inline void setPlayerViewPixelPos();
		inline void drawTileOne(int x, int y, int offsetX, int offsetY, const CTileData& palTile, int tileAnimLoopFrame);
		inline void drawEVOne(int tileAnimLoopFrame, const CRectangle& drawRange, CMapEventData* ev, CPoint& pos, CMapEventData::CMapEventPageData* pg, bool isBottom);
		inline void drawPlayer(int tileAnimLoopFrame, bool isBottom);
		inline bool checkTilePriority(CMapObjectData* ev, int priority);
		inline bool checkTileRangeX(int& x);
		inline bool checkTileRangeY(int& y);
		inline bool checkIsGraphTopPrimary(int tx, int ty);
	};

	//ショップ情報
	class CShopData {
	public:
		//SQ_BEGINMEMBER
		//コンストラクター
		SQ_NOBIND CShopData() {
			vector<CIDValueData>().swap(this->ItemList);
		}

		//メンバーメソッド
		SQ_INSTANCEFUNC void Add(int id);
		SQ_INSTANCEFUNC void Clear();
		SQ_INSTANCEFUNC int GetCount();
		SQ_INSTANCEFUNC CIDValueData Get(int index);
		SQ_INSTANCEFUNC void SortItemsByVisibleID(bool reverse);
		SQ_INSTANCEFUNC void SortItemsByName(bool reverse);
		SQ_INSTANCEFUNC void SortItemsByPrice(bool reverse);

	private:
		vector<CIDValueData> ItemList;
	};


	//SQ_BEGINMEMBER
	SQ_NOBIND CSceneMap();

	//メンバー変数
	SQ_NOBIND static vector<CSceneMap::CEVSelfValueOneData> SelfValueData;

	SQ_NOBIND CMapData MapData;
	SQ_PROPGET CMapData* getMapData() { return &this->MapData; }

	SQ_NOBIND CShopData ShopData;
	SQ_PROPGET CShopData* getShopData() { return &this->ShopData; }


	//メンバーメソッド
	SQ_NOBIND void Dispose() override;
	SQ_NOBIND string GetName() override { return "Map"; }
	SQ_INSTANCEFUNC void ShowMessage(int faceID, string speakerName, string msg);
	SQ_INSTANCEFUNC void ChangeMap(string fileName, int x, int y, int direction4);
	SQ_CLASSFUNC static int TurnLeft4(int direction4);
	SQ_CLASSFUNC static int TurnLeft8(int direction8);
	SQ_CLASSFUNC static int TurnRight4(int direction4);
	SQ_CLASSFUNC static int TurnRight8(int direction8);
	SQ_CLASSFUNC static int TurnHalf4(int direction4);
	SQ_CLASSFUNC static int TurnHalf8(int direction8);
	SQ_CLASSFUNC static int PointToDirection4(const CPoint& pos);
	SQ_CLASSFUNC static int PointToDirection8(const CPoint& pos);
	SQ_CLASSFUNC static CPoint Direction4ToPoint(int direction4);
	SQ_CLASSFUNC static CPoint Direction8ToPoint(int direction8);
	SQ_NOBIND static void Load(vector<string>& vectorbuf);
	SQ_NOBIND static void Save(string& strbuf);
	SQ_NOBIND static void PrintMap();
	SQ_NOBIND static void PrintEVs();
	SQ_NOBIND static void PrintSelfValueData();

private:
	//プライベートメソッド
	static bool deflate(string fileName, string& text);
	static bool loadTextMap(string fileName, string& text);
	static void commitMapSelfValue();
	static void applyMapSelfValue();
	static void loadSelfValueData(vector<string>& vectorbuf);
	static void saveSelfValueData(string& strbuf);
};
