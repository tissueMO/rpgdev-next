//###################################################################
//		スプライト管理クラス
//###################################################################


//####################  スプライト統括クラス  ####################
class CSprite {
public:
	//####################  スプライト：単一データ  ####################
	class CSpriteData : public CIDNameData {
	public:
		//####################  スプライト：グラフィック部  ####################
		class CSpriteGraphicArea : public CIDNameData {
		public:
			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_NOBIND CRectangle VisibleRange;					//実視範囲（この範囲の左上が 0, 0 基点となる）
			SQ_PROPGET CRectangle* getVisibleRange() { return &this->VisibleRange; }

			SQ_NOBIND CPoint Location;							//スプライトの左上座標を基準(0, 0)とした、画像の左上座標
			SQ_PROPGET CPoint* getLocation() { return &this->Location; }

			SQ_NOBIND CSize Size;								//画像サイズ
			SQ_PROPGET CSize* getSize() { return &this->Size; }

			SQ_INSTANCEVALUE int ExtendRateV;					//垂直方向の拡大率 : 100
			SQ_INSTANCEVALUE int ExtendRateH;					//水平方向の拡大率 : 100

			SQ_NOBIND bool IsNewGraph;


			//メンバーメソッド
			SQ_INSTANCEFUNC void Draw(CPoint pos);
			SQ_INSTANCEFUNC void InitVisibleRange();
		};


		//####################  スプライト：テキスト部  ####################
		class CSpriteTextArea : public CIDNameData {
		public:
			//SQ_BEGINMEMBER
			//メンバー変数
			SQ_INSTANCEVALUE string Text;
			SQ_INSTANCEVALUE int Align;									//揃える方向
			SQ_INSTANCEVALUE int Color;
			SQ_INSTANCEVALUE int FontID;

			SQ_NOBIND CRectangle DrawArea;								//スプライトの左上座標を基準(0, 0)とした、文字列の描画可能範囲
			SQ_PROPGET CRectangle* getDrawArea() { return &this->DrawArea; }

			SQ_NOBIND CPoint Location;									//スプライトの左上座標を基準(0, 0)とした、文字列の左上座標
			SQ_PROPGET CPoint* getLocation() { return &this->Location; }


			//メンバーメソッド
			SQ_INSTANCEFUNC void Draw(CPoint pos);
		};


		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND Cstdvector<CSpriteGraphicArea> Graphics;
		SQ_NOBIND Cstdvector<CSpriteTextArea> Texts;

		SQ_NOBIND int WipeType;
		SQ_PROPGET int getWipeType() { return this->WipeType; }

		SQ_NOBIND int WipeSpeed;
		SQ_PROPGET int getWipeSpeed() { return this->WipeSpeed; }

		SQ_NOBIND int WipeCounter;
		SQ_PROPGET int getWipeCounter() { return this->WipeCounter; } SQ_PROPSET void setWipeCounter(int val) { this->WipeCounter = val; }

		SQ_NOBIND int WipeState;
		SQ_NOBIND bool DeleteAfterWipe;
		SQ_INSTANCEVALUE bool Hide = false;

		SQ_NOBIND CPoint Location;
		SQ_PROPGET CPoint* getLocation() { return &this->Location; }

		SQ_NOBIND int Trans;
		SQ_PROPGET int getTrans() { return this->Trans; } SQ_PROPSET void setTrans(int val) { this->Trans = TS_WITHIN(val, 0, 255); }


		//メンバーメソッド
		SQ_INSTANCEFUNC int GetGraphicsCount() { return this->Graphics.GetCount(); }
		SQ_INSTANCEFUNC int AddGraphics(string path, int x, int y, int baseHandle = -1);
		SQ_INSTANCEFUNC void ChangeGraphics(int index, string path, int x, int y, int baseHandle = -1);
		SQ_INSTANCEFUNC void DeleteGraphics(int index);
		SQ_INSTANCEFUNC CSpriteGraphicArea* GetGraphics(int index);

		SQ_INSTANCEFUNC int GetTextsCount() { return this->Texts.GetCount(); }
		SQ_INSTANCEFUNC int AddText(string text, int x, int y, int color, int fontID, int align = static_cast<int>(TS::Sprite::TextAlign::Left));
		SQ_INSTANCEFUNC void ChangeText(int index, string text);
		SQ_INSTANCEFUNC void DeleteText(int index);
		SQ_INSTANCEFUNC CSpriteTextArea* GetText(int index);

		SQ_INSTANCEFUNC void Draw();
		SQ_INSTANCEFUNC void WipeIn(int type, int wipeSpeed);
		SQ_INSTANCEFUNC void WipeOut(int type, int wipeSpeed, bool afterDelete);
	};


	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_NOBIND Cstdvector<CSpriteData> Data;
	SQ_INSTANCEVALUE bool ForbidLayerDraw = false;

	//メソッド
	SQ_NOBIND void DrawAll();
	SQ_INSTANCEFUNC void Add(int ID);
	SQ_INSTANCEFUNC void Delete(int ID);
	SQ_INSTANCEFUNC void Clear(int wipeType, int wipeSpeed = static_cast<int>(TS::Map::Speed::Normal));
	SQ_INSTANCEFUNC CSpriteData* Get(int ID);
	SQ_INSTANCEFUNC void AddNewGraphics(int ID, string path, int x, int y);
	SQ_INSTANCEFUNC void AddLoadedGraphics(int ID, int graphID, int x, int y);
	SQ_INSTANCEFUNC void AddWipeNewGraphics(int ID, string path, int x, int y, int wipeType, int wipeSpeed);
	SQ_INSTANCEFUNC void AddWipeLoadedGraphics(int ID, int graphID, int x, int y, int wipeType, int wipeSpeed);
	SQ_INSTANCEFUNC void Copy(int newID, int baseID, int x, int y);
	SQ_INSTANCEFUNC void CopyWipe(int newID, int baseID, int x, int y, int wipeType, int wipeSpeed);
	SQ_INSTANCEFUNC void WipeInRange(int startID, int endID, int wipeType, int wipeSpeed);
	SQ_INSTANCEFUNC void WipeOutRange(int startID, int endID, int wipeType, int wipeSpeed, bool afterDelete = true);

private:
	void releaseResource(CSpriteData* data);
};
