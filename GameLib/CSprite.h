//###################################################################
//		�X�v���C�g�Ǘ��N���X
//###################################################################


//####################  �X�v���C�g�����N���X  ####################
class CSprite {
public:
	//####################  �X�v���C�g�F�P��f�[�^  ####################
	class CSpriteData : public CIDNameData {
	public:
		//####################  �X�v���C�g�F�O���t�B�b�N��  ####################
		class CSpriteGraphicArea : public CIDNameData {
		public:
			//SQ_BEGINMEMBER
			//�����o�[�ϐ�
			SQ_NOBIND CRectangle VisibleRange;					//�����͈́i���͈̔͂̍��オ 0, 0 ��_�ƂȂ�j
			SQ_PROPGET CRectangle* getVisibleRange() { return &this->VisibleRange; }

			SQ_NOBIND CPoint Location;							//�X�v���C�g�̍�����W���(0, 0)�Ƃ����A�摜�̍�����W
			SQ_PROPGET CPoint* getLocation() { return &this->Location; }

			SQ_NOBIND CSize Size;								//�摜�T�C�Y
			SQ_PROPGET CSize* getSize() { return &this->Size; }

			SQ_INSTANCEVALUE int ExtendRateV;					//���������̊g�嗦 : 100
			SQ_INSTANCEVALUE int ExtendRateH;					//���������̊g�嗦 : 100

			SQ_NOBIND bool IsNewGraph;


			//�����o�[���\�b�h
			SQ_INSTANCEFUNC void Draw(CPoint pos);
			SQ_INSTANCEFUNC void InitVisibleRange();
		};


		//####################  �X�v���C�g�F�e�L�X�g��  ####################
		class CSpriteTextArea : public CIDNameData {
		public:
			//SQ_BEGINMEMBER
			//�����o�[�ϐ�
			SQ_INSTANCEVALUE string Text;
			SQ_INSTANCEVALUE int Align;									//���������
			SQ_INSTANCEVALUE int Color;
			SQ_INSTANCEVALUE int FontID;

			SQ_NOBIND CRectangle DrawArea;								//�X�v���C�g�̍�����W���(0, 0)�Ƃ����A������̕`��\�͈�
			SQ_PROPGET CRectangle* getDrawArea() { return &this->DrawArea; }

			SQ_NOBIND CPoint Location;									//�X�v���C�g�̍�����W���(0, 0)�Ƃ����A������̍�����W
			SQ_PROPGET CPoint* getLocation() { return &this->Location; }


			//�����o�[���\�b�h
			SQ_INSTANCEFUNC void Draw(CPoint pos);
		};


		//SQ_BEGINMEMBER
		//�����o�[�ϐ�
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


		//�����o�[���\�b�h
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
	//�����o�[�ϐ�
	SQ_NOBIND Cstdvector<CSpriteData> Data;
	SQ_INSTANCEVALUE bool ForbidLayerDraw = false;

	//���\�b�h
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
