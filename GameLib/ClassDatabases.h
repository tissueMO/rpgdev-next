//###################################################################
//		各種データベース
//###################################################################

//データベースの統括クラス
class CDatabase {
public:
	//####################  曲線情報  ####################
	//SQ_NOBIND_BEGIN
	typedef array<int, static_cast<int>(TS::DB::DBCurveDataIndices::Count)> Curve;
	class CCurveData : public CIDNameData {
	public:
		Curve Curve;
	};
	//SQ_NOBIND_END


	//####################  アドオンデータ  ####################
	class CAddonData : public CIDValueData {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_INSTANCEVALUE int Type;

		//コンストラクター
		SQ_NOBIND CAddonData() = default;
		SQ_NOBIND CAddonData(int type, int id, int param) : CIDValueData(id, param) {
			this->Type = type;
		}
	};


	//####################  データベース基盤  ####################
	//データベースのセル情報
	class CDBCell : public CNullCheck {
	public:
		//IDと値のペアのセル
		class CDBIDValues : public CNullCheck {
		public:
			//SQ_BEGINMEMBER
			//メンバーメソッド
			SQ_NOBIND vector<CIDValueData>& getData() { return this->data.Items; }
			SQ_INSTANCEFUNC bool Exists(int id) { return this->data.Exists(id); }
			SQ_INSTANCEFUNC int Count() { return this->data.GetCount(); }
			SQ_INSTANCEFUNC void Add(int id, int val) { this->data.Add(CIDValueData(id, val)); }
			SQ_INSTANCEFUNC void Insert(int id, int val, int index) { this->data.Insert(CIDValueData(id, val), index); }
			SQ_INSTANCEFUNC void Delete(int index) { this->data.DeleteFromIndex(index); }
			SQ_INSTANCEFUNC void Clear() { this->data.Clear(); }
			SQ_INSTANCEFUNC CIDValueData* Get(int index) { return this->data.GetFromIndex(index); }
			SQ_INSTANCEFUNC int GetValue(int id) { return this->data.GetFromID(id)->Value; }
			SQ_INSTANCEFUNC void SetValue(int id, int val) {
				if(!this->data.Exists(id)) {
					this->data.Add(CIDValueData(id, val));
				} else {
					this->data.GetFromID(id)->Value = val;
				}
			}

		private:
			Cstdvector<CIDValueData> data;
		};

		//アドオンセル: スクリプトは読み取り専用
		class CDBAddons : public CNullCheck {
		public:
			//SQ_BEGINMEMBER
			//メンバーメソッド
			SQ_NOBIND void Clear() { this->data.Clear(); }
			SQ_NOBIND void Add(int type, int id, int param) { this->data.Add(CAddonData(type, id, param)); }
			SQ_INSTANCEFUNC int Count() { return this->data.GetCount(); }
			SQ_INSTANCEFUNC CAddonData Get(int index) { return *this->data.GetFromIndex(index); }
			SQ_NOBIND vector<CAddonData>& getData() { return this->data.Items; }

		private:
			Cstdvector<CAddonData> data;
		};


		//SQ_BEGINMEMBER
		//コンストラクター
		SQ_NOBIND CDBCell() = default;
		SQ_NOBIND CDBCell(int cellType, string rawData);
		SQ_NOBIND CDBCell(string columnName, string rawData);

		//メンバーメソッド
		SQ_INSTANCEFUNC void Init();
		SQ_NOBIND void SetData(string rawData, int cellType = -1);
		SQ_INSTANCEFUNC string ToString();
		SQ_INSTANCEFUNC int GetCellType();
		SQ_INSTANCEFUNC int ConvertToColorCode();
		SQ_INSTANCEFUNC CPoint ConvertToPoint();
		SQ_NOBIND vector<CCurveData> ConvertToCurves();
		SQ_NOBIND CDBAddons ConvertToAddons();

		//セル種別に対応する値を返す：該当種別以外はダミー値となる
		SQ_NOBIND int* GetIntegerPTR();
		SQ_PROPGET int getInteger() { return this->IsNull() ? 0 : this->intValue; } SQ_PROPSET void setInteger(int val) { if (!this->IsNull()) this->intValue = val; }

		SQ_NOBIND bool* GetBooleanPTR();
		SQ_PROPGET bool getBoolean() { return this->IsNull() ? false : this->boolValue; } SQ_PROPSET void setBoolean(bool val) { if(!this->IsNull()) this->boolValue = val; }

		SQ_NOBIND string* GetStringPTR();
		SQ_PROPGET string getString() { return this->IsNull() ? "" : this->strValue; } SQ_PROPSET void setString(string val) { if(!this->IsNull()) this->strValue = val; }

		SQ_NOBIND CIntegerArray* GetIntegers();
		SQ_PROPGET CIntegerArray* getIntegers() { return this->IsNull() ? nullptr : &this->intValues; }

		SQ_NOBIND CDBIDValues* GetIDValues();
		SQ_PROPGET CDBIDValues* getIDValues() { return this->IsNull() ? nullptr : &this->IDValues; }

	private:
		//メンバー変数
		int cellType;
		string rawData;
		string columnType;

		int intValue;
		bool boolValue;
		string strValue;
		CIntegerArray intValues;
		CDBIDValues IDValues;

		//プライベートメソッド
		void setCellType(string columnName);
	};

	//データベースの行情報
	typedef vector<CDBCell> DBRow;

	//単一データベース情報
	class CDBBaseOne : public CIDNameData {
	public:
		//SQ_BEGINMEMBER
		//コンストラクター
		SQ_NOBIND CDBBaseOne() = default;

		//メンバーメソッド
		SQ_NOBIND void LoadRow(const vector<string>* split);
		SQ_NOBIND void SetColumnTypes(const vector<string>* split);
		SQ_NOBIND void SetColumnNames(const vector<string>* split);
		SQ_NOBIND void SaveRow(string& strbuf);
		SQ_NOBIND void AddRow(DBRow row);
		SQ_NOBIND void DeleteRowFromIndex(int rowIndex);
		SQ_NOBIND void DeleteRowFromID(int rowID);
		SQ_NOBIND void ClearRow();
		SQ_NOBIND void AddCell(int rowIndex, CDatabase::CDBCell* cell);
		SQ_NOBIND void AddColumn(string columnName);
		SQ_NOBIND void ClearColumn();
		SQ_INSTANCEFUNC void InitDB();
		SQ_INSTANCEFUNC void InitRow(int rowID);
		SQ_INSTANCEFUNC void InitRowFromIndex(int rowIndex);
		SQ_INSTANCEFUNC void InitCell(int rowID, string columnName);
		SQ_INSTANCEFUNC void InitCellFromIndex(int rowIndex, int columnIndex);
		SQ_INSTANCEFUNC int GetRowCount();
		SQ_INSTANCEFUNC int GetColumnCount();
		SQ_INSTANCEFUNC int GetColumnIndex(string columnName);
		SQ_INSTANCEFUNC int ConvertToFixedID(int visibleID);
		SQ_NOBIND int ConvertToVisibleID(int fixedID);
		SQ_INSTANCEFUNC string GetColumnName(int columnIndex);
		SQ_INSTANCEFUNC string GetColumnType(int columnIndex);
		SQ_NOBIND DBRow* GetRowFromID(int rowID);
		SQ_NOBIND DBRow* GetRowFromIndex(int rowIndex);
		SQ_INSTANCEFUNC CDatabase::CDBCell* GetCell(int rowID, string columnName);
		SQ_INSTANCEFUNC CDatabase::CDBCell* GetCellFromIndex(int rowIndex, int columnIndex);

	protected:
		//メンバー変数
		vector<string> columnTypes;
		vector<string> columnNames;
		vector<DBRow> rows;
	};


	//全データベースの基底クラス
	class CDBBase : public CNullCheck {
	public:
		//SQ_BEGINMEMBER
		//メンバーメソッド
		SQ_NOBIND virtual void Dispose();
		SQ_NOBIND virtual bool Load(string fileName);
		SQ_NOBIND virtual void Load(vector<string>& vectorbuf);
		SQ_NOBIND virtual void Save(string& strbuf);
		SQ_NOBIND virtual void PrintData();
		SQ_INSTANCEFUNC int GetSubDBCount();
		SQ_INSTANCEFUNC string GetDBFileName();
		SQ_INSTANCEFUNC CDBBaseOne* GetSubDB(int index);

	protected:
		//メンバー変数
		string fileName;
		FILE* fp;
		int fpDx;
		string stringBuffer;
		vector<string> splitBuffer;
		Cstdvector<CDBBaseOne> DBs;

		//プライベートメソッド
		bool openFile(string fileName);
		bool isEOF();
		void loadOneDB(CDBBaseOne* db);
		bool gets(char* charbuf, int size);
		void closeFile();
		int readNextRow();
	};


	//####################  拡張：固定データベース  ####################
	//システムDB: XMLを読み込み、設定項目名を列にした１行のDBBaseOneを生成する
	class CDBSystem : public CDatabase::CDBBase {
	public:
		//SQ_BEGINMEMBER
		//メンバーメソッド
		SQ_NOBIND bool Load(string fileName) override;
		SQ_INSTANCEFUNC int GetInteger(string settingName);
		SQ_INSTANCEFUNC bool GetBoolean(string settingName);
		SQ_INSTANCEFUNC string GetString(string settingName);
		SQ_INSTANCEFUNC CSize GetSize(string settingName);

	private:
		//プライベートメソッド
		string perseSettingName();
		string perseValue();
	};

	//汎用素材DB: 素材管理クラスと連携している
	class CDBMaterial : public CDatabase::CDBBase {
	public:
		//SQ_BEGINMEMBER
		//メンバーメソッド
		SQ_NOBIND bool Load(string fileName) override;
		SQ_NOBIND static CMaterialBase* GetMaterialObject(int subDBIndex);
		SQ_INSTANCEFUNC int GetHandle(int type, int id);
		SQ_INSTANCEFUNC void Play(int soundID, int volume, int pitch, int pan);
		SQ_INSTANCEFUNC void PlayForWait(int soundID, int volume, int pitch, int pan);
		SQ_INSTANCEFUNC void Stop(int soundID);

	private:
		bool loaded = false;
	};

	//エフェクトDB: エフェクト再生機能を持つ
	class CDBEffect : public CDatabase::CDBBase {
	public:
		//SQ_NOBIND_BEGIN
		class CDBEffectLayerData : public CIDNameData {
		public:
			//メンバー変数
			CSize SectionSize;
		};
		//SQ_NOBIND_END

		//単一エフェクト再生情報
		class CDBEffectData : public CIDNameData {
		public:
			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_NOBIND int Handle;
			SQ_NOBIND int SQHandle;

			SQ_NOBIND int Frame;
			SQ_PROPGET int getFrame() { return this->Frame; }

			SQ_NOBIND int TargetType;
			SQ_PROPGET int getTargetType() { return this->TargetType; }

			SQ_NOBIND bool WaitBreakFlag = false;		//このフラグが立つと再生待機を中断する
			SQ_PROPGET bool getWaitBreakFlag() { return this->WaitBreakFlag; } SQ_PROPSET void setWaitBreakFlag(bool value) { this->WaitBreakFlag = value; }

			SQ_NOBIND CPoint TargetPosition;
			SQ_NOBIND Cstdvector<CDBEffectLayerData> Graphics;

			//メンバーメソッド
			SQ_INSTANCEFUNC void LoadGraph(int layerIndex, string fileName, int secSizeWidth, int secSizeHeight);
			SQ_INSTANCEFUNC void Draw(int layerIndex, int destX, int destY, int secX, int secY, int blendMode, int trans, int ext, int angle);
			SQ_INSTANCEFUNC void PlaySE(int id, int volume, int pitch, int pan, int sameCounter);
			SQ_INSTANCEFUNC void Flash(int color, int strength, int count, int sameCounter);
			SQ_INSTANCEFUNC void Shake(int strength, int sameCounter);
			SQ_INSTANCEFUNC void WaitBreak();
		};

		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND vector<CDBEffectData> PlayingList;

		//メンバーメソッド
		SQ_NOBIND void Dispose() override;
		SQ_INSTANCEFUNC int Show(int id, int destX, int destY);
		SQ_INSTANCEFUNC void ShowForWait(int id, int destX, int destY);
		SQ_INSTANCEFUNC CDBEffectData* GetEffectData(int handle);
	};

	//タイルセットDB
	class CDBTileset : public CDatabase::CDBBase {
	public:
		//タイルセットの１タイルあたりのデータ
		class CTileData {
		public:
			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_INSTANCEVALUE int Hit;
			SQ_INSTANCEVALUE int InOut;
			SQ_INSTANCEVALUE int Priority;
			SQ_INSTANCEVALUE int Tag;
			SQ_INSTANCEVALUE int AnimPatternCount;
		};

		//タイルパレットグラフィックの情報
		class CTilePallet {
		public:
			using PalletArray = array<int, TS::Map::TileColumnCount * TS::Map::AutoTileRowLength>;

			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_NOBIND int TilesetHandle;
			SQ_PROPGET int getTilesetHandle() { return this->TilesetHandle; }

			SQ_NOBIND PalletArray AutoTiles;
			SQ_PROPGET CIntegerArray getAutoTiles();
		};

		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND int ActiveTilesetID;
		SQ_PROPGET int getActiveTilesetID() { return this->ActiveTilesetID; }

		//メンバーメソッド
		SQ_NOBIND void Dispose() override;
		SQ_NOBIND void DisposeTilePallet();
		SQ_INSTANCEFUNC CTilePallet* LoadTileset(int id);
		SQ_INSTANCEFUNC CTileData* GetTilePallet(int tileX, int tileY);
		SQ_INSTANCEFUNC CSize GetTilePalletSize();

	private:
		//メンバー変数
		using TilesetRow = vector<CTileData>;
		vector<TilesetRow> activeTilesetData;
		CTilePallet* loadedPallet;

		//メンバーメソッド
		CTileData setData(string raw);
		void loadActiveTilesetFile();
		CTilePallet* loadActiveTilesetGraph();
	};


	//####################  拡張：可変データベース  ####################
	//変数フラグDB
	class CDBValue : public CDatabase::CDBBase {
	public:
		//SQ_BEGINMEMBER
		//メンバーメソッド
		SQ_NOBIND bool Load(string fileName) override;
		SQ_NOBIND void Load(vector<string>& vectorbuf) override;
		SQ_NOBIND void Save(string& strbuf) override;
		SQ_NOBIND void PrintData() override;
		SQ_NOBIND void GetSelfValueList(int type, CSelfValueData* dest);
		SQ_INSTANCEFUNC CSelfValueData GetSelfValues(int type);

	private:
		//プライベートメソッド
		void bindValues();
	};

	//アクター情報
	class CDBActorData : public CIDNameData {
	public:
		//####################  IDと値がペアになったデータに個別変数を加えたデータ  ####################
		class CActorSubData : public CIDValueData {
		public:
			//SQ_BEGINMEMBER
			//コンストラクター
			CActorSubData() = default;
			CActorSubData(CIDValueData data) {
				this->ID = data.ID;
				this->Name = data.Name;
				this->Value = data.Value;
			}

			//メンバー変数
			SQ_INSTANCEVALUE CSelfValueData SelfValues;
		};


		//####################  スクリプト互換：「IDと値がペアになったデータに個別変数を加えたデータ」の配列  ####################
		//NOTE: テンプレートにしないのは、スクリプトにバインドする都合のため。
		//NOTE: このクラスに要素を追加/削除/変更する操作をC++側に限定しているのは、追加/削除/変更時に特定の処理を挟めるようにするため。
		class CActorSubDataArray : public CNullCheck {
		public:
			//SQ_BEGINMEMBER
			//デストラクター
			SQ_NOBIND ~CActorSubDataArray() {
				this->data.Clear();
			}

			//メンバーメソッド
			SQ_INSTANCEFUNC int GetIndexFromID(int ID) { return this->data.GetIndexFromID(ID); }
			SQ_NOBIND CActorSubData* Get(int index) { return this->data.GetFromIndex(index); }
			SQ_INSTANCEFUNC int GetID(int index) { return this->data.GetFromIndex(index)->ID; }
			SQ_INSTANCEFUNC string GetName(int index) { return this->data.GetFromIndex(index)->Name; }
			SQ_INSTANCEFUNC int GetValue(int index) { return this->data.GetFromIndex(index)->Value; }
			SQ_INSTANCEFUNC CSelfValueData* GetSelfValues(int index) { return &this->data.GetFromIndex(index)->SelfValues; }
			SQ_INSTANCEFUNC bool Exists(int ID) { return this->data.Exists(ID); }
			SQ_NOBIND void Add(CActorSubData value) { this->data.Add(value); }
			SQ_NOBIND void DeleteFromID(int ID) { this->data.DeleteFromID(ID); }
			SQ_NOBIND void DeleteFromIndex(int index) { this->data.DeleteFromIndex(index); }
			SQ_INSTANCEFUNC int Count() { return this->data.GetCount(); }
			SQ_NOBIND void Clear() { this->data.Clear(); }
			SQ_NOBIND void SetAllowOverlap(bool val) { this->data.AllowOverlap = val; }
			SQ_NOBIND vector<CActorSubData>& getData() { return this->data.Items; }

		private:
			Cstdvector<CActorSubData> data;
		};


		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_INSTANCEVALUE string Title;
		SQ_INSTANCEVALUE bool IsNPC;
		SQ_INSTANCEVALUE int EXP;

		SQ_NOBIND string Graphics;
		SQ_PROPGET string getGraphics() { return this->Graphics; }

		SQ_NOBIND int GraphHandle;
		SQ_PROPGET int getGraphHandle() { return this->GraphHandle; }

		SQ_NOBIND CSize GraphSize;
		SQ_PROPGET CSize* getGraphSize() { return &this->GraphSize; }

		SQ_NOBIND CPoint Location;
		SQ_PROPGET CPoint* getLocation() { return &this->Location; }

		SQ_NOBIND int Level;
		SQ_PROPGET int getLevel() { return this->Level; }

		SQ_NOBIND CSelfValueData SelfValues;
		SQ_PROPGET CSelfValueData* getSelfValues() { return &this->SelfValues; }

		SQ_NOBIND Cstdvector<CIDValueData> Parameters;
		SQ_NOBIND Cstdvector<CCurveData> ParameterCurve;
		SQ_NOBIND CCurveData EXPCurve;

		SQ_NOBIND CActorSubDataArray PotentialSkills;			//習得予定のスキル・レベル
		SQ_PROPGET CActorSubDataArray* getPotentialSkills() { return &this->PotentialSkills; }

		SQ_NOBIND CActorSubDataArray Attributes;
		SQ_PROPGET CActorSubDataArray* getAttributes() { return &this->Attributes; }

		SQ_NOBIND CActorSubDataArray Classes;
		SQ_PROPGET CActorSubDataArray* getClasses() { return &this->Classes; }

		SQ_NOBIND CActorSubDataArray Equips;
		SQ_PROPGET CActorSubDataArray* getEquips() { return &this->Equips; }

		SQ_NOBIND CActorSubDataArray States;
		SQ_PROPGET CActorSubDataArray* getStates() { return &this->States; }

		SQ_NOBIND CActorSubDataArray LearnedSkills;				//実際に習得しているスキル・習得元
		SQ_PROPGET CActorSubDataArray* getLearnedSkills() { return &this->LearnedSkills; }

		SQ_NOBIND CActorSubDataArray LevelUpParameters;			//レベルアップでのパラメーター変動情報
		SQ_PROPGET CActorSubDataArray* getLevelUpParameters() { return &this->LevelUpParameters; }

		SQ_NOBIND CActorSubDataArray LevelUpSkills;			//レベルアップでの習得スキル情報
		SQ_PROPGET CActorSubDataArray* getLevelUpSkills() { return &this->LevelUpSkills; }

		//メンバーメソッド
		SQ_INSTANCEFUNC void PrintData();
		SQ_NOBIND void Save(string& buf);
		SQ_NOBIND static CDBActorData Load(vector<string>& buf);
		SQ_CLASSFUNC static CDBActorData CreateActorFromChar(int ID);
		SQ_CLASSFUNC static CDBActorData CreateActorFromEnemy(int ID);
		SQ_INSTANCEFUNC void LoadGraphics(string fileName);
		SQ_INSTANCEFUNC void DisposeGraphics();
		SQ_INSTANCEFUNC int GetBaseFDBIndex() { return this->baseFDBIndex; }

		SQ_INSTANCEFUNC void SetClass(int ID);
		SQ_INSTANCEFUNC bool SetEquip(int ID, bool absolute = false);
		SQ_INSTANCEFUNC void SetSkill(int ID);
		SQ_INSTANCEFUNC bool SetState(int ID, bool absolute);

		SQ_INSTANCEFUNC bool ReleaseClass(int ID);
		SQ_INSTANCEFUNC bool ReleaseEquip(int ID, bool absolute = false);
		SQ_INSTANCEFUNC bool ReleaseEquipType(int itemTypeID, bool absolute = false);
		SQ_INSTANCEFUNC bool ReleaseSkill(int ID);
		SQ_INSTANCEFUNC bool ReleaseState(int ID);

		SQ_INSTANCEFUNC void ClearClass();
		SQ_INSTANCEFUNC void ClearEquip(bool absolute = false);
		SQ_INSTANCEFUNC void ClearSkill();
		SQ_INSTANCEFUNC void ClearState();
		SQ_INSTANCEFUNC void ClearStateOnlyBad();

		SQ_INSTANCEFUNC CIDValueData* GetParameter(int ID) { return this->Parameters.GetFromID(ID); }
		SQ_INSTANCEFUNC int GetAttributeResistance(int ID);
		SQ_INSTANCEFUNC int GetStateResistance(int ID);
		SQ_INSTANCEFUNC int GetEquipIDFromItemTypeID(int itemTypeID);

		SQ_INSTANCEFUNC bool CheckAttribute(int attrID) { return this->Attributes.Exists(attrID); }
		SQ_INSTANCEFUNC bool CheckStateLimit(int stateLimit);
		SQ_INSTANCEFUNC bool CheckUseSkill(int skillID);
		SQ_INSTANCEFUNC bool CheckUseSkillType(int skillTypeID);
		SQ_INSTANCEFUNC bool CheckEquipLock(int itemTypeID);
		SQ_INSTANCEFUNC bool CheckEquip(int itemID);

		SQ_INSTANCEFUNC int PlusEXP(int exp);
		SQ_INSTANCEFUNC int LevelUp(int val);
		SQ_INSTANCEFUNC int GetNextEXP();

	private:
		//メンバー変数
		int baseFDBIndex = -1;

		//１番目は優先度、２番目はアドオン情報
		vector<tuple<int, CIDValueData>> rstAttribute;
		vector<tuple<int, CIDValueData>> rstState;
		vector<tuple<int, CIDValueData>> lckEquipType;
		vector<tuple<int, CIDValueData>> pmsEquipType;
		vector<tuple<int, CIDValueData>> pmsEquipSubType;
		vector<tuple<int, CIDValueData>> pmsSkillUse;
		vector<tuple<int, CIDValueData>> pmsSkillType;


		//プライベートメソッド
		void SetAttribute(int attrID);
		void DeleteAttribute(int attrID);
		void ClearAttribute() { this->Attributes.Clear(); }

		void releaseAddonOne(CDatabase::CDBCell::CDBAddons* addons, TS::DB::AddonFrom priority);
		void releaseAddonAll();
		void applyAddonOne(CDatabase::CDBCell::CDBAddons* addons, TS::DB::AddonFrom priority);
		void applyAddonAll();

		void baseSetDataWithAddon(int ID, CActorSubDataArray& tgt, TS::DB::FDBIndices FDBType, TS::DB::DBValueIndices selfValueType, TS::DB::AddonFrom priority);
		void baseDeleteDataWithAddon(int ID, CActorSubDataArray& tgt, TS::DB::FDBIndices FDBType, TS::DB::AddonFrom priority);

		void deleteAddonBuffer(vector<tuple<int, CIDValueData>>& list, tuple<int, CIDValueData>& base);
		bool checkHigherAddon(vector<tuple<int, CIDValueData>>& list, tuple<int, CIDValueData>& base);
		bool checkUserAddedSkill(int ID);

		static int getCurveValue(int x, CCurveData& cv);
	};

	//パーティキャラDB
	class CDBActor : public CDatabase::CDBBase {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND Cstdvector<CDBActorData> DB;

		//メンバーメソッド
		SQ_NOBIND bool Load(string fileName) override;
		SQ_NOBIND void Load(vector<string>& buf) override;
		SQ_NOBIND void Save(string& buf) override;
		SQ_NOBIND void PrintData() override;
		SQ_NOBIND void Dispose() override;

		SQ_INSTANCEFUNC int GetCount() { return this->DB.GetCount(); }
		SQ_INSTANCEFUNC void InitData(int ID);
		SQ_INSTANCEFUNC void InitDataFromIndex(int index);
		SQ_INSTANCEFUNC CDBActorData* GetData(int ID) { return this->DB.GetFromID(ID); }
		SQ_INSTANCEFUNC CDBActorData* GetDataFromIndex(int index) { return this->DB.GetFromIndex(index); }
	};

	//パーティDB
	class CDBParty : public CDatabase::CDBBase {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND int Money = 0;

		//メンバーメソッド
		SQ_NOBIND bool Load(string fileName) override;
		SQ_NOBIND void Load(vector<string>& vectorbuf) override;
		SQ_NOBIND void Save(string& strbuf) override;
		SQ_NOBIND void PrintData() override;
		SQ_NOBIND void Dispose() override;
		SQ_INSTANCEFUNC void ClearMembers() { this->members.Clear(); }
		SQ_INSTANCEFUNC void ClearItems() { this->items.Clear(); }
		SQ_INSTANCEFUNC void AddMember(int id, int index);
		SQ_INSTANCEFUNC void DeleteMemberFromID(int id);
		SQ_INSTANCEFUNC void DeleteMemberFromIndex(int index);
		SQ_INSTANCEFUNC int CheckCharInParty(int id);
		SQ_INSTANCEFUNC int GetCharIDFromPartyIndex(int index);
		SQ_INSTANCEFUNC int GetCharCount();
		SQ_INSTANCEFUNC void AddItem(int id, int count);
		SQ_INSTANCEFUNC void DeleteItem(int id, int count);
		SQ_INSTANCEFUNC CIDValueData* GetItemDataFromIndex(int index);
		SQ_INSTANCEFUNC int GetItemCountAll();
		SQ_INSTANCEFUNC int GetItemTypeCount(int itemTypeID);
		SQ_INSTANCEFUNC int GetItemCount(int itemID);
		SQ_INSTANCEFUNC void SortItemsByVisibleID(bool reverse);
		SQ_INSTANCEFUNC void SortItemsByName(bool reverse);
		SQ_PROPGET int getMoney() { return Money; } SQ_PROPSET void setMoney(int val);

	private:
		//メンバー変数
		Cstdvector<CIDNameData> members;
		Cstdvector<CIDValueData> items;

		//メンバーメソッド
		int getHavingItemIndex(int id);
	};


	//####################  拡張：ユーザーデータベース  ####################
	//ユーザーDBのTSVファイルをすべて読み込んでUserDBsに格納する
	class CDBUser : public CDatabase::CDBBase {
	public:
		//SQ_BEGINMEMBER
		//メンバーメソッド
		SQ_NOBIND bool Load(string fileName) override;
		SQ_NOBIND void Load(vector<string>& vectorbuf) override;
		SQ_NOBIND void Save(string& strbuf) override;
		SQ_NOBIND void PrintData() override;
		SQ_NOBIND void Dispose() override;
		SQ_INSTANCEFUNC CDBBaseOne* GetUserDB(int id);

	private:
		Cstdvector<CDBBaseOne> UserDBs;
	};

	//SQ_BEGINMEMBER
	SQ_NOBIND CDatabase();
	SQ_NOBIND ~CDatabase();

	//メンバー変数
	SQ_NOBIND vector<CDBBase*> FDB;
	SQ_NOBIND vector<CDBBase*> VDB;
	SQ_NOBIND CDBUser UDB;
	SQ_PROPGET CDBUser* getUDB() { return &this->UDB; }

	//メンバーメソッド
	SQ_NOBIND void LoadAll();
	SQ_NOBIND void ReloadDB(int dbType, bool includeFDBSystem);
	SQ_INSTANCEFUNC CDBBase* GetDB(int dbType, int dbIndex);
	SQ_PROPGET CDBEffect* getFDBEffect();
	SQ_PROPGET CDBMaterial* getFDBMaterial();
	SQ_PROPGET CDBSystem* getFDBSystem();
	SQ_PROPGET CDBTileset* getFDBTileset();
	SQ_PROPGET CDBValue* getVDBValue();
	SQ_PROPGET CDBParty* getVDBParty();
	SQ_PROPGET CDBActor* getVDBActor();
	SQ_INSTANCEFUNC CDatabase::CDBCell* GetCell(int dbType, int dbIndex, int subDB, int rowID, string columnName);
	SQ_INSTANCEFUNC CDatabase::CDBCell* GetCellFromIndex(int dbType, int dbIndex, int subDB, int rowIndex, int columnIndex);
	SQ_INSTANCEFUNC void InitDBType(int dbType);
	SQ_INSTANCEFUNC void InitDB(int dbType, int dbIndex, int subDB);
	SQ_INSTANCEFUNC void InitRow(int dbType, int dbIndex, int subDB, int rowID);
	SQ_INSTANCEFUNC void InitCell(int dbType, int dbIndex, int subDB, int rowID, string columnName);
	SQ_INSTANCEFUNC void InitCellFromIndex(int dbType, int dbIndex, int subDB, int rowIndex, int columnIndex);

	SQ_NOBIND bool GetItemTypes(int itemID, CIntegerArray* typeIDs, CIntegerArray* subTypeIDs);
	SQ_NOBIND int GetFixedID(int dbType, int dbIndex, int subDB, int visibleID);
};
