//###################################################################
//			�ǂ��ł��g����ėp�N���X�Q
//###################################################################
#include "CDebug.h"

//��s�錾
//SQ_NOBIND_BEGIN
class CSize;
class CPoint;
//SQ_NOBIND_END


//####################  ���g�����݂��Ă��邩�ǂ������`�F�b�N����@�\  ####################
//�X�N���v�g�ł�nullptr��Ԃ��Ă��C���X�^���X���������ꂽ�Ƃ݂Ȃ���Ă��܂��B
//����āA�g�p�O�ɂ��̃`�F�b�N���K�v�ƂȂ邽�߁A�X�N���v�g�ň����N���X�͕K�����̃N���X���p������K�v������B
class CNullCheck {
public:
	//SQ_BEGINMEMBER
	SQ_INSTANCEFUNC bool IsNull() {
		return (this == nullptr);
	}
};


//####################  ID�Ɩ��O�����f�[�^  ####################
class CIDNameData : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int ID = -1;
	SQ_INSTANCEVALUE string Name;

	//�R���X�g���N�^�[
	SQ_NOBIND CIDNameData() = default;
	SQ_NOBIND CIDNameData(int id, string name) {
		this->ID = id;
		this->Name = name;
	}
};


//####################  ID�ƒl(�Ɩ��O)�����f�[�^  ####################
class CIDValueData : public CIDNameData {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int Value = 0;

	//�R���X�g���N�^�[
	SQ_NOBIND CIDValueData() = default;
	SQ_NOBIND CIDValueData(int id, int param) {
		this->ID = id;
		this->Value = param;
	}
	SQ_NOBIND CIDValueData(int id, string name, int param) {
		this->ID = id;
		this->Name = name;
		this->Value = param;
	}
};


//####################  �T�E���h�f�[�^  ####################
class CSoundData {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_NOBIND string FileName;
	SQ_PROPGET string getFileName() { return this->FileName; }

	SQ_NOBIND int Volume;
	SQ_PROPGET int getVolume() { return this->Volume; }

	SQ_NOBIND int Pitch;
	SQ_PROPGET int getPitch() { return this->Pitch; }

	SQ_NOBIND int Panpot;
	SQ_PROPGET int getPanpot() { return this->Panpot; }

	SQ_NOBIND int Handle = -1;
	SQ_PROPGET int getHandle() { return this->Handle; }

	//�����o�[���\�b�h
	SQ_NOBIND void Load(string rawData);
	SQ_INSTANCEFUNC void Load(string fileName, int vol, int pit, int pan);
	SQ_INSTANCEFUNC void Play(int playType, bool oncePlay);
	SQ_INSTANCEFUNC void Stop();
	SQ_INSTANCEFUNC void Pause();
	SQ_INSTANCEFUNC void Resume(bool loopMode);
	SQ_INSTANCEFUNC void Dispose();
};


//####################  ���W���  ####################
class CPoint : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int X;
	SQ_INSTANCEVALUE int Y;


	//�R���X�g���N�^�[
	SQ_NOBIND CPoint() {
		this->Set(0, 0);
	}
	SQ_NOBIND CPoint(CSize size);
	SQ_NOBIND CPoint(int x, int y) {
		this->X = x;
		this->Y = y;
	}


	//���Z�q�̃I�[�o�[���[�h
	SQ_NOBIND bool operator==(const CPoint &b) {
		if(this->X == b.X && this->Y == b.Y) {
			return true;
		} else {
			return false;
		}
	}
	SQ_NOBIND CPoint operator+(const CPoint &b) {
		CPoint temp(this->X + b.X, this->Y + b.Y);
		return temp;
	}
	SQ_NOBIND CPoint operator-(const CPoint &b) {
		CPoint temp(this->X - b.X, this->Y - b.Y);
		return temp;
	}

	SQ_INSTANCEFUNC void Set(int x, int y) {
		this->X = x;
		this->Y = y;
	}


	//�����o�[���\�b�h
	//������ [X:Y] ����f�[�^���Z�b�g����
	SQ_NOBIND void Set(string text);
	SQ_NOBIND string ToString();
	SQ_INSTANCEFUNC bool IsEmpty() {
		return (this->X == 0 && this->Y == 0);
	}
};


//####################  �T�C�Y���  ####################
class CSize : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int Width;
	SQ_INSTANCEVALUE int Height;


	//�R���X�g���N�^�[
	SQ_NOBIND CSize() {
		this->Set(0, 0);
	}
	SQ_NOBIND CSize(CPoint pos);
	SQ_NOBIND CSize(int width, int height) {
		this->Width = width;
		this->Height = height;
	}


	//���Z�q�̃I�[�o�[���[�h
	SQ_NOBIND bool operator==(const CSize &b) {
		if(this->Width == b.Width && this->Height == b.Height) {
			return true;
		} else {
			return false;
		}
	}
	SQ_NOBIND CSize operator+(const CSize &b) {
		CSize temp(this->Width + b.Width, this->Height + b.Height);
		return temp;
	}
	SQ_NOBIND CSize operator-(const CSize &b) {
		CSize temp(this->Width - b.Width, this->Height - b.Height);
		return temp;
	}


	//�����o�[���\�b�h
	SQ_INSTANCEFUNC void Set(int width, int height) {
		this->Width = width;
		this->Height = height;
	}

	//������ [WxH] ����f�[�^���Z�b�g����
	SQ_NOBIND void Set(string text);
	SQ_NOBIND string ToString();
	SQ_INSTANCEFUNC bool IsEmpty() {
		return (this->Width == 0 && this->Height == 0);
	}
};


//####################  ��`���  ####################
class CRectangle : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int Left;
	SQ_INSTANCEVALUE int Top;
	SQ_INSTANCEVALUE int Width;
	SQ_INSTANCEVALUE int Height;


	//�R���X�g���N�^�[
	SQ_NOBIND CRectangle() {
		this->Left = this->Top = this->Width = this->Height = 0;
	}
	SQ_NOBIND CRectangle(int left, int top, int width, int height) {
		this->Set(left, top, width, height);
	}
	SQ_NOBIND CRectangle(const CPoint& pos, const CSize& size) {
		this->Set(pos, size);
	}


	//�����o�[���\�b�h
	//����ƃT�C�Y���w�肵�ăZ�b�g
	SQ_INSTANCEFUNC void Set(int left, int top, int width, int height) {
		this->Left = left;
		this->Top = top;
		this->Width = width;
		this->Height = height;
	}
	SQ_NOBIND void Set(const CPoint& pos, const CSize& size) {
		this->Set(pos.X, pos.Y, size.Width, size.Height);
	}

	//�S���̍��W���w�肵�ăZ�b�g
	SQ_INSTANCEFUNC void SetByCorner(int left, int top, int right, int bottom) {
		this->Left = left;
		this->Top = top;
		this->Width = right - left;
		this->Height = bottom - top;
	}
	SQ_NOBIND void SetByCorner(CPoint leftTop, CPoint rightBottom) {
		this->SetByCorner(leftTop.X, leftTop.Y, rightBottom.X, rightBottom.Y);
	}

	//�E�[��Ԃ�
	SQ_INSTANCEFUNC int Right() {
		return this->Left + this->Width - 1;
	}

	//���[��Ԃ�
	SQ_INSTANCEFUNC int Bottom() {
		return this->Top + this->Height - 1;
	}

	//�w�肵���_�����̋�`�̒��Ɋ܂܂�邩�ǂ����𒲂ׂ�
	SQ_INSTANCEFUNC virtual bool PointContains(int x, int y) {
		return (this->Left <= x && x <= this->Right() && this->Top <= y && y <= this->Bottom());
	}
	SQ_NOBIND bool PointContains(const CPoint& pos) {
		return this->PointContains(pos.X, pos.Y);
	}

	//�w�肵����`�����̋�`�̒��ɏd�Ȃ邩�ǂ����𒲂ׂ�
	SQ_NOBIND bool RectangleContains(CRectangle& rect) {
		return !(
			this->Left - rect.Right() <= 0
			|| rect.Left - this->Right() <= 0
			|| this->Top - rect.Top <= 0
			|| rect.Left - this->Bottom() <= 0
			);
	}
};


//####################  �~���  ####################
//NOTE: ��`�Ɖ~�̏Փ˂𔻒肷��ɂ́A�~����`�ɂ��Ă����`���m�̏Փ˔�����s���B
class CCircle {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int Radius;
	SQ_INSTANCEVALUE int CenterX;
	SQ_INSTANCEVALUE int CenterY;


	//�R���X�g���N�^�[
	SQ_NOBIND CCircle() {
		this->Radius = this->CenterX = this->CenterY = 0;
	}
	SQ_NOBIND CCircle(int x, int y, int r) {
		this->Set(x, y, r);
	}
	SQ_NOBIND CCircle(const CRectangle& rect) {
		this->FromRectangle(rect);
	}


	//�����o�[���\�b�h
	//���S�Ɣ��a�ŃZ�b�g����
	SQ_INSTANCEFUNC void Set(int x, int y, int r) {
		this->CenterX = x;
		this->CenterY = y;
		this->Radius = r;
	}

	//��`���~�ɕϊ�����
	SQ_INSTANCEFUNC void FromRectangle(const CRectangle& rect) {
		//�Z�ӂ̔����̒����𔼌a�ɓ��Ă�
		this->CenterX = rect.Left + rect.Width / 2;
		this->CenterY = rect.Top + rect.Height / 2;
		if(rect.Width <= rect.Height) {
			this->Radius = rect.Width / 2;
		} else {
			this->Radius = rect.Height / 2;
		}
	}

	//���̉~����ډ~�Ƃ����`��Ԃ�
	SQ_INSTANCEFUNC CRectangle ToRectangle() {
		return CRectangle(this->CenterX - this->Radius, this->CenterY - this->Radius, this->Radius * 2, this->Radius * 2);
	}

	//�w�肵���_�����̉~�̒��Ɋ܂܂�邩�ǂ����𒲂ׂ�
	SQ_INSTANCEFUNC bool PointContains(int x, int y) {
		return (pow(x - this->CenterX, 2) + pow(y - this->CenterY, 2) <= pow(this->Radius, 2));
	}

	//�w�肵���~�����̉~�ɏd�Ȃ邩�ǂ����𒲂ׂ�
	SQ_INSTANCEFUNC bool CircleContains(const CCircle& c) {
		return (pow(c.CenterX - this->CenterX, 2) + pow(c.CenterY - this->CenterY, 2) <= pow(c.Radius + this->Radius, 2));
	}
};


//####################  RGB�J���[���  ####################
class CRGBColor : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_INSTANCEVALUE int Red;
	SQ_INSTANCEVALUE int Green;
	SQ_INSTANCEVALUE int Blue;


	//�R���X�g���N�^�[
	SQ_NOBIND CRGBColor() {
		this->Red = this->Green = this->Blue = 0;
	}
	SQ_NOBIND CRGBColor(int r, int g, int b) {
		this->Set(r, g, b);
	}
	SQ_NOBIND CRGBColor(int colorCode) {
		this->Set(colorCode);
	}


	//�����o�[���\�b�h
	SQ_INSTANCEFUNC void Set(int r, int g, int b);
	SQ_NOBIND void Set(int colorCode) {
		this->Set(
			colorCode >> (8 * 2) & 0xff,
			colorCode >> (8 * 1) & 0xff,
			colorCode >> (8 * 0) & 0xff
			);
	}
	SQ_INSTANCEFUNC int ToCode() {
		return ::GetColor(this->Red, this->Green, this->Blue);
	}
};


//####################  �X�N���v�g�݊��F�����^�̔z��  ####################
//NOTE: �e���v���[�g�ɂ��Ȃ��̂́A�X�N���v�g�Ƀo�C���h����s���̂��߁B
class CIntegerArray : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�R���X�g���N�^�[
	SQ_NOBIND CIntegerArray() = default;
	SQ_NOBIND CIntegerArray(int size, ...) {
		//�ϒ���������������
		va_list args;
		va_start(args, size);

		//�����̐������l��o�^����
		this->data.clear();
		TS_COUNTLOOP(i, size) {
			this->data.push_back(va_arg(args, int));
		}
		va_end(args);
	}
	SQ_NOBIND ~CIntegerArray() {
		vector<int>().swap(this->data);
	}


	//�����o�[���\�b�h
	SQ_NOBIND vector<int>& getData() { return this->data; }
	SQ_INSTANCEFUNC void Add(int value) { this->data.push_back(value); }
	SQ_INSTANCEFUNC bool Exists(int value);
	SQ_INSTANCEFUNC int Count() { return this->data.size(); }
	SQ_INSTANCEFUNC int Get(int index);
	SQ_INSTANCEFUNC void Set(int index, int value);
	SQ_INSTANCEFUNC void Insert(int index, int value);
	SQ_INSTANCEFUNC void Delete(int index);
	SQ_INSTANCEFUNC void Clear() { vector<int>().swap(this->data); }

private:
	vector<int> data;
};


//####################  �X�N���v�g�݊��F������^�̔z��  ####################
//NOTE: �e���v���[�g�ɂ��Ȃ��̂́A�X�N���v�g�Ƀo�C���h����s���̂��߁B
class CStringArray : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�R���X�g���N�^�[
	SQ_NOBIND CStringArray() = default;
	SQ_NOBIND CStringArray(int size, ...) {
		//�ϒ���������������
		va_list args;
		va_start(args, size);

		//�����̐������l��o�^����
		this->data.clear();
		TS_COUNTLOOP(i, size) {
			this->data.push_back(va_arg(args, string));
		}
		va_end(args);
	}
	SQ_NOBIND ~CStringArray() {
		vector<string>().swap(this->data);
	}


	//�����o�[���\�b�h
	SQ_NOBIND vector<string>& getData() { return this->data; }
	SQ_INSTANCEFUNC void Add(string value) { this->data.push_back(value); }
	SQ_INSTANCEFUNC bool Exists(string value);
	SQ_INSTANCEFUNC int Count() { return this->data.size(); }
	SQ_INSTANCEFUNC string Get(int index);
	SQ_INSTANCEFUNC void Set(int index, string value);
	SQ_INSTANCEFUNC void Insert(int index, string value);
	SQ_INSTANCEFUNC void Delete(int index);
	SQ_INSTANCEFUNC void Clear() { vector<string>().swap(this->data); }

private:
	vector<string> data;
};


//SQ_NOBIND_BEGIN
//####################  ID���X�g�̊��N���X  ####################
//�����F�����������EID�͈�ӂɂȂ�E�C���f�b�N�X�̊T�O���Ȃ��B
//���ӁF���X�g��ΏۂƂ����g��for���͂��̃��X�g���������Ă���N���X�ł̂ݗ��p�\�ƂȂ�B
template<typename T = CIDNameData>
class Cstdmap {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	map<int, T> Items;


	//�f�X�g���N�^�[
	~Cstdmap() {
		map<int, T>().swap(this->Items);
	}

	//�����o�[���\�b�h
	//���𒲂ׂ�
	int GetCount() {
		return this->Items.size();
	}

	//�w�肵��ID�̗v�f�����݂��邩���ׂ�
	bool Exists(int ID) {
		return this->Items.find(ID) != this->Items.end();
	}

	//�v�f���擾
	T* Get(int ID) {
		if(this->Items.find(ID) != this->Items.end()) {
			return &this->Items[ID];
		}
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�ID [%d] ��map�v�f���Q�Ƃ���܂����B", ID);
		return nullptr;
	}

	//�v�f��ǉ�
	void Add(T _val) {
		if(this->Exists(_val.ID)) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "���ɑ��݂���ID [%d] ��map�v�f��ǉ����܂����B", _val.ID);
		}
		this->Items[_val.ID] = _val;
	}

	//�v�f���폜
	void Delete(int ID) {
		if(this->Exists(ID)) {
			this->Items.erase(ID);
		}
	}

	//���X�g���폜
	void Clear() {
		this->Items.clear();
	}

	//MaxID�ȉ��ōő�̋�ID���擾
	int GetEmptyID(int maxID) {
		int ID = maxID;
		while(this->Items.find(ID) != this->Items.end()) {
			ID--;
		}
		return ID;
	}
};


//####################  �ϒ����X�g�̊��N���X  ####################
//�����F���������������ǉ��ƍ폜���ᑬ�EID�ƃC���f�b�N�X�����̊T�O�����E�d��ID���������
//���ӁF���X�g��ΏۂƂ����g��for���͂��̃��X�g���������Ă���N���X�ł̂ݗ��p�\�ƂȂ�
//���l�FID�\�[�g�Ɩ��O�\�[�g�������ς�
template<typename T = CIDNameData*>
class Cstdvector {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	vector<T> Items;
	bool AllowOverlap = false;


	//�R���X�g���N�^�[
	Cstdvector() = default;
	Cstdvector(bool overlap) {
		this->AllowOverlap = overlap;
	}
	//�f�X�g���N�^�[
	~Cstdvector() {
		vector<T>().swap(this->Items);
	}


	//�����o�[���\�b�h
	//���𐔂���
	int GetCount(int ID = -1) {
		if(ID == -1) {
			return this->Items.size();
		} else {
			return std::count_if(TS_CONTAINERALL(this->Items), [&](T& item) {
				return item.ID == ID;
			});
		}
	}

	//�w�肵��ID�̗v�f�����݂��邩���ׂ�
	bool Exists(int ID) {
		return std::find_if(TS_CONTAINERALL(this->Items), [&](T& item) {
			return item.ID == ID;
		}) != this->Items.end();
	}

	//�w�肵��ID�̗v�f�ɑΉ�����C���f�b�N�X���擾����
	int GetIndexFromID(int ID) {
		TS_COUNTLOOP(i, this->Items.size()) {
			if(this->Items[i].ID == ID) {
				return i;
			}
		}
		return -1;		//���݂��Ȃ��ꍇ�� -1 ��Ԃ�
	}

	//ID����v�f���擾
	T* GetFromID(int ID) {
		auto it = std::find_if(TS_CONTAINERALL(this->Items), [&](T& item) {
			return item.ID == ID;
		});
		if(it == this->Items.end()) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�ID [%d] ��vector�v�f���Q�Ƃ���܂����B", ID);
			return nullptr;
		}
		return &*it;
	}

	//�C���f�b�N�X����v�f���擾
	T* GetFromIndex(int index) {
		try {
			return &this->Items.at(index);
		} catch(...) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃C���f�b�N�X [%d] ��vector�v�f���Q�Ƃ���܂����B", index);
			return nullptr;
		}
	}

	//�v�f��ǉ�
	void Add(T _val) {
		if(this->Exists(_val.ID) && !this->AllowOverlap) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "���ɑ��݂���ID [%d] ��vector�v�f��ǉ����܂����B", _val.ID);
		}
		this->Items.push_back(_val);
	}

	//�v�f���w��ʒu�ɑ}��
	//-1���w�肷��Ɩ����ɒǉ�����
	void Insert(T _val, int index) {
		if(index == -1) {
			this->Add(_val);
		} else {
			index = CGameObject::WithIn(index, 0, this->Items.size() - 1);
			this->Items.insert(this->Items.begin() + index, _val);
		}
	}

	//ID����v�f���폜
	void DeleteFromID(int ID) {
		this->Items.erase(std::remove_if(TS_CONTAINERALL(this->Items), [&](T& item) {
			return item.ID == ID;
		}), this->Items.end());
	}

	//�C���f�b�N�X����v�f���폜
	void DeleteFromIndex(int index) {
		TS_CHECKINDEX(index, 0, static_cast<int>(this->Items.size())) {
			this->Items.erase(this->Items.begin() + index);
		} else {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̃C���f�b�N�X [%d] ��vector�v�f���폜���悤�Ƃ��܂����B", index);
		}
	}

	//���X�g���폜
	void Clear() {
		this->Items.clear();
		vector<T>().swap(this->Items);		//������swap�Z�@�ŃT�C�Y���[���ɂ���
	}

	//�\�[�g
	void Sort(int sortType, bool reverse) {
		switch(static_cast<TS::System::SortType>(sortType)) {
			case TS::System::SortType::ID:
				if(!reverse) {
					std::sort(TS_CONTAINERALL(this->Items),
							  [](const T& a, const T& b)->int { return a.ID < b.ID; }
					);
				} else {
					std::sort(TS_CONTAINERALL(this->Items),
							  [](const T& a, const T& b)->int { return a.ID > b.ID; }
					);
				}
				break;
			case TS::System::SortType::Name:
				if(!reverse) {
					std::sort(TS_CONTAINERALL(this->Items),
							  [](const T& a, const T& b)->int { return a.Name < b.Name; }
					);
				} else {
					std::sort(TS_CONTAINERALL(this->Items),
							  [](const T& a, const T& b)->int { return a.Name > b.Name; }
					);
				}
				break;
		}
	}

};
//SQ_NOBIND_END


//####################  �ʕϐ��f�[�^  ####################
class CSelfValueData : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�����o�[�ϐ�
	SQ_NOBIND Cstdmap<CIDValueData> data;


	//�R���X�g���N�^�[
	SQ_NOBIND CSelfValueData() = default;
	SQ_NOBIND CSelfValueData(Cstdmap<CIDValueData> *baseData) {
		this->data = *baseData;
	}
	//�f�X�g���N�^�[
	SQ_NOBIND ~CSelfValueData() {
		this->data.Clear();
	}


	//�����o�[���\�b�h
	SQ_INSTANCEFUNC CIDValueData* Get(int ID) {
		//���݂��Ȃ��ꍇ�͍쐬����
		if(!this->data.Exists(ID)) {
			this->data.Add(CIDValueData(ID, 0));
		}
		return this->data.Get(ID);
	}
	SQ_INSTANCEFUNC void Set(int ID, int val) {
		this->Get(ID)->Value = val;
	}
	SQ_NOBIND void Add(const CIDValueData* valueData) {
		this->data.Add(*valueData);
	}
};


//####################  array�N���X�̑������z����ȈՉ�����  ####################
template<typename T, size_t Size, size_t ...Sizes> struct array_multi_type {
	using type = std::array<typename array_multi_type<T, Sizes...>::type, Size>;
};
template<typename T, size_t Size> struct array_multi_type<T, Size> {
	using type = std::array<T, Size>;
};
template<typename T, size_t Size, size_t ...Sizes> using array_multi = typename array_multi_type<T, Size, Sizes...>::type;
