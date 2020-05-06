//###################################################################
//			どこでも使える汎用クラス群
//###################################################################
#include "CDebug.h"

//先行宣言
//SQ_NOBIND_BEGIN
class CSize;
class CPoint;
//SQ_NOBIND_END


//####################  自身が存在しているかどうかをチェックする機構  ####################
//スクリプトではnullptrを返してもインスタンスが生成されたとみなされてしまう。
//よって、使用前にこのチェックが必要となるため、スクリプトで扱うクラスは必ずこのクラスを継承する必要がある。
class CNullCheck {
public:
	//SQ_BEGINMEMBER
	SQ_INSTANCEFUNC bool IsNull() {
		return (this == nullptr);
	}
};


//####################  IDと名前を持つデータ  ####################
class CIDNameData : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int ID = -1;
	SQ_INSTANCEVALUE string Name;

	//コンストラクター
	SQ_NOBIND CIDNameData() = default;
	SQ_NOBIND CIDNameData(int id, string name) {
		this->ID = id;
		this->Name = name;
	}
};


//####################  IDと値(と名前)を持つデータ  ####################
class CIDValueData : public CIDNameData {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int Value = 0;

	//コンストラクター
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


//####################  サウンドデータ  ####################
class CSoundData {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
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

	//メンバーメソッド
	SQ_NOBIND void Load(string rawData);
	SQ_INSTANCEFUNC void Load(string fileName, int vol, int pit, int pan);
	SQ_INSTANCEFUNC void Play(int playType, bool oncePlay);
	SQ_INSTANCEFUNC void Stop();
	SQ_INSTANCEFUNC void Pause();
	SQ_INSTANCEFUNC void Resume(bool loopMode);
	SQ_INSTANCEFUNC void Dispose();
};


//####################  座標情報  ####################
class CPoint : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int X;
	SQ_INSTANCEVALUE int Y;


	//コンストラクター
	SQ_NOBIND CPoint() {
		this->Set(0, 0);
	}
	SQ_NOBIND CPoint(CSize size);
	SQ_NOBIND CPoint(int x, int y) {
		this->X = x;
		this->Y = y;
	}


	//演算子のオーバーロード
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


	//メンバーメソッド
	//文字列 [X:Y] からデータをセットする
	SQ_NOBIND void Set(string text);
	SQ_NOBIND string ToString();
	SQ_INSTANCEFUNC bool IsEmpty() {
		return (this->X == 0 && this->Y == 0);
	}
};


//####################  サイズ情報  ####################
class CSize : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int Width;
	SQ_INSTANCEVALUE int Height;


	//コンストラクター
	SQ_NOBIND CSize() {
		this->Set(0, 0);
	}
	SQ_NOBIND CSize(CPoint pos);
	SQ_NOBIND CSize(int width, int height) {
		this->Width = width;
		this->Height = height;
	}


	//演算子のオーバーロード
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


	//メンバーメソッド
	SQ_INSTANCEFUNC void Set(int width, int height) {
		this->Width = width;
		this->Height = height;
	}

	//文字列 [WxH] からデータをセットする
	SQ_NOBIND void Set(string text);
	SQ_NOBIND string ToString();
	SQ_INSTANCEFUNC bool IsEmpty() {
		return (this->Width == 0 && this->Height == 0);
	}
};


//####################  矩形情報  ####################
class CRectangle : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int Left;
	SQ_INSTANCEVALUE int Top;
	SQ_INSTANCEVALUE int Width;
	SQ_INSTANCEVALUE int Height;


	//コンストラクター
	SQ_NOBIND CRectangle() {
		this->Left = this->Top = this->Width = this->Height = 0;
	}
	SQ_NOBIND CRectangle(int left, int top, int width, int height) {
		this->Set(left, top, width, height);
	}
	SQ_NOBIND CRectangle(const CPoint& pos, const CSize& size) {
		this->Set(pos, size);
	}


	//メンバーメソッド
	//左上とサイズを指定してセット
	SQ_INSTANCEFUNC void Set(int left, int top, int width, int height) {
		this->Left = left;
		this->Top = top;
		this->Width = width;
		this->Height = height;
	}
	SQ_NOBIND void Set(const CPoint& pos, const CSize& size) {
		this->Set(pos.X, pos.Y, size.Width, size.Height);
	}

	//４隅の座標を指定してセット
	SQ_INSTANCEFUNC void SetByCorner(int left, int top, int right, int bottom) {
		this->Left = left;
		this->Top = top;
		this->Width = right - left;
		this->Height = bottom - top;
	}
	SQ_NOBIND void SetByCorner(CPoint leftTop, CPoint rightBottom) {
		this->SetByCorner(leftTop.X, leftTop.Y, rightBottom.X, rightBottom.Y);
	}

	//右端を返す
	SQ_INSTANCEFUNC int Right() {
		return this->Left + this->Width - 1;
	}

	//下端を返す
	SQ_INSTANCEFUNC int Bottom() {
		return this->Top + this->Height - 1;
	}

	//指定した点がこの矩形の中に含まれるかどうかを調べる
	SQ_INSTANCEFUNC virtual bool PointContains(int x, int y) {
		return (this->Left <= x && x <= this->Right() && this->Top <= y && y <= this->Bottom());
	}
	SQ_NOBIND bool PointContains(const CPoint& pos) {
		return this->PointContains(pos.X, pos.Y);
	}

	//指定した矩形がこの矩形の中に重なるかどうかを調べる
	SQ_NOBIND bool RectangleContains(CRectangle& rect) {
		return !(
			this->Left - rect.Right() <= 0
			|| rect.Left - this->Right() <= 0
			|| this->Top - rect.Top <= 0
			|| rect.Left - this->Bottom() <= 0
			);
	}
};


//####################  円情報  ####################
//NOTE: 矩形と円の衝突を判定するには、円を矩形にしてから矩形同士の衝突判定を行う。
class CCircle {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int Radius;
	SQ_INSTANCEVALUE int CenterX;
	SQ_INSTANCEVALUE int CenterY;


	//コンストラクター
	SQ_NOBIND CCircle() {
		this->Radius = this->CenterX = this->CenterY = 0;
	}
	SQ_NOBIND CCircle(int x, int y, int r) {
		this->Set(x, y, r);
	}
	SQ_NOBIND CCircle(const CRectangle& rect) {
		this->FromRectangle(rect);
	}


	//メンバーメソッド
	//中心と半径でセットする
	SQ_INSTANCEFUNC void Set(int x, int y, int r) {
		this->CenterX = x;
		this->CenterY = y;
		this->Radius = r;
	}

	//矩形を円に変換する
	SQ_INSTANCEFUNC void FromRectangle(const CRectangle& rect) {
		//短辺の半分の長さを半径に当てる
		this->CenterX = rect.Left + rect.Width / 2;
		this->CenterY = rect.Top + rect.Height / 2;
		if(rect.Width <= rect.Height) {
			this->Radius = rect.Width / 2;
		} else {
			this->Radius = rect.Height / 2;
		}
	}

	//この円を内接円とする矩形を返す
	SQ_INSTANCEFUNC CRectangle ToRectangle() {
		return CRectangle(this->CenterX - this->Radius, this->CenterY - this->Radius, this->Radius * 2, this->Radius * 2);
	}

	//指定した点がこの円の中に含まれるかどうかを調べる
	SQ_INSTANCEFUNC bool PointContains(int x, int y) {
		return (pow(x - this->CenterX, 2) + pow(y - this->CenterY, 2) <= pow(this->Radius, 2));
	}

	//指定した円がこの円に重なるかどうかを調べる
	SQ_INSTANCEFUNC bool CircleContains(const CCircle& c) {
		return (pow(c.CenterX - this->CenterX, 2) + pow(c.CenterY - this->CenterY, 2) <= pow(c.Radius + this->Radius, 2));
	}
};


//####################  RGBカラー情報  ####################
class CRGBColor : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_INSTANCEVALUE int Red;
	SQ_INSTANCEVALUE int Green;
	SQ_INSTANCEVALUE int Blue;


	//コンストラクター
	SQ_NOBIND CRGBColor() {
		this->Red = this->Green = this->Blue = 0;
	}
	SQ_NOBIND CRGBColor(int r, int g, int b) {
		this->Set(r, g, b);
	}
	SQ_NOBIND CRGBColor(int colorCode) {
		this->Set(colorCode);
	}


	//メンバーメソッド
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


//####################  スクリプト互換：整数型の配列  ####################
//NOTE: テンプレートにしないのは、スクリプトにバインドする都合のため。
class CIntegerArray : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//コンストラクター
	SQ_NOBIND CIntegerArray() = default;
	SQ_NOBIND CIntegerArray(int size, ...) {
		//可変長引数を解決する
		va_list args;
		va_start(args, size);

		//引数の数だけ値を登録する
		this->data.clear();
		TS_COUNTLOOP(i, size) {
			this->data.push_back(va_arg(args, int));
		}
		va_end(args);
	}
	SQ_NOBIND ~CIntegerArray() {
		vector<int>().swap(this->data);
	}


	//メンバーメソッド
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


//####################  スクリプト互換：文字列型の配列  ####################
//NOTE: テンプレートにしないのは、スクリプトにバインドする都合のため。
class CStringArray : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//コンストラクター
	SQ_NOBIND CStringArray() = default;
	SQ_NOBIND CStringArray(int size, ...) {
		//可変長引数を解決する
		va_list args;
		va_start(args, size);

		//引数の数だけ値を登録する
		this->data.clear();
		TS_COUNTLOOP(i, size) {
			this->data.push_back(va_arg(args, string));
		}
		va_end(args);
	}
	SQ_NOBIND ~CStringArray() {
		vector<string>().swap(this->data);
	}


	//メンバーメソッド
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
//####################  IDリストの基底クラス  ####################
//特徴：検索が高速・IDは一意になる・インデックスの概念がない。
//注意：リストを対象とした拡張for文はそのリストを所持しているクラスでのみ利用可能となる。
template<typename T = CIDNameData>
class Cstdmap {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	map<int, T> Items;


	//デストラクター
	~Cstdmap() {
		map<int, T>().swap(this->Items);
	}

	//メンバーメソッド
	//個数を調べる
	int GetCount() {
		return this->Items.size();
	}

	//指定したIDの要素が存在するか調べる
	bool Exists(int ID) {
		return this->Items.find(ID) != this->Items.end();
	}

	//要素を取得
	T* Get(int ID) {
		if(this->Items.find(ID) != this->Items.end()) {
			return &this->Items[ID];
		}
		CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないID [%d] でmap要素が参照されました。", ID);
		return nullptr;
	}

	//要素を追加
	void Add(T _val) {
		if(this->Exists(_val.ID)) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "既に存在するID [%d] でmap要素を追加しました。", _val.ID);
		}
		this->Items[_val.ID] = _val;
	}

	//要素を削除
	void Delete(int ID) {
		if(this->Exists(ID)) {
			this->Items.erase(ID);
		}
	}

	//リストを削除
	void Clear() {
		this->Items.clear();
	}

	//MaxID以下で最大の空きIDを取得
	int GetEmptyID(int maxID) {
		int ID = maxID;
		while(this->Items.find(ID) != this->Items.end()) {
			ID--;
		}
		return ID;
	}
};


//####################  可変長リストの基底クラス  ####################
//特徴：検索が高速だが追加と削除が低速・IDとインデックス両方の概念を持つ・重複IDが許される
//注意：リストを対象とした拡張for文はそのリストを所持しているクラスでのみ利用可能となる
//備考：IDソートと名前ソートも実装済み
template<typename T = CIDNameData*>
class Cstdvector {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	vector<T> Items;
	bool AllowOverlap = false;


	//コンストラクター
	Cstdvector() = default;
	Cstdvector(bool overlap) {
		this->AllowOverlap = overlap;
	}
	//デストラクター
	~Cstdvector() {
		vector<T>().swap(this->Items);
	}


	//メンバーメソッド
	//個数を数える
	int GetCount(int ID = -1) {
		if(ID == -1) {
			return this->Items.size();
		} else {
			return std::count_if(TS_CONTAINERALL(this->Items), [&](T& item) {
				return item.ID == ID;
			});
		}
	}

	//指定したIDの要素が存在するか調べる
	bool Exists(int ID) {
		return std::find_if(TS_CONTAINERALL(this->Items), [&](T& item) {
			return item.ID == ID;
		}) != this->Items.end();
	}

	//指定したIDの要素に対応するインデックスを取得する
	int GetIndexFromID(int ID) {
		TS_COUNTLOOP(i, this->Items.size()) {
			if(this->Items[i].ID == ID) {
				return i;
			}
		}
		return -1;		//存在しない場合は -1 を返す
	}

	//IDから要素を取得
	T* GetFromID(int ID) {
		auto it = std::find_if(TS_CONTAINERALL(this->Items), [&](T& item) {
			return item.ID == ID;
		});
		if(it == this->Items.end()) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないID [%d] でvector要素が参照されました。", ID);
			return nullptr;
		}
		return &*it;
	}

	//インデックスから要素を取得
	T* GetFromIndex(int index) {
		try {
			return &this->Items.at(index);
		} catch(...) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のインデックス [%d] でvector要素が参照されました。", index);
			return nullptr;
		}
	}

	//要素を追加
	void Add(T _val) {
		if(this->Exists(_val.ID) && !this->AllowOverlap) {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "既に存在するID [%d] でvector要素を追加しました。", _val.ID);
		}
		this->Items.push_back(_val);
	}

	//要素を指定位置に挿入
	//-1を指定すると末尾に追加する
	void Insert(T _val, int index) {
		if(index == -1) {
			this->Add(_val);
		} else {
			index = CGameObject::WithIn(index, 0, this->Items.size() - 1);
			this->Items.insert(this->Items.begin() + index, _val);
		}
	}

	//IDから要素を削除
	void DeleteFromID(int ID) {
		this->Items.erase(std::remove_if(TS_CONTAINERALL(this->Items), [&](T& item) {
			return item.ID == ID;
		}), this->Items.end());
	}

	//インデックスから要素を削除
	void DeleteFromIndex(int index) {
		TS_CHECKINDEX(index, 0, static_cast<int>(this->Items.size())) {
			this->Items.erase(this->Items.begin() + index);
		} else {
			CGameObject::RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外のインデックス [%d] でvector要素を削除しようとしました。", index);
		}
	}

	//リストを削除
	void Clear() {
		this->Items.clear();
		vector<T>().swap(this->Items);		//いわゆるswap技法でサイズをゼロにする
	}

	//ソート
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


//####################  個別変数データ  ####################
class CSelfValueData : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_NOBIND Cstdmap<CIDValueData> data;


	//コンストラクター
	SQ_NOBIND CSelfValueData() = default;
	SQ_NOBIND CSelfValueData(Cstdmap<CIDValueData> *baseData) {
		this->data = *baseData;
	}
	//デストラクター
	SQ_NOBIND ~CSelfValueData() {
		this->data.Clear();
	}


	//メンバーメソッド
	SQ_INSTANCEFUNC CIDValueData* Get(int ID) {
		//存在しない場合は作成する
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


//####################  arrayクラスの多次元配列を簡易化する  ####################
template<typename T, size_t Size, size_t ...Sizes> struct array_multi_type {
	using type = std::array<typename array_multi_type<T, Sizes...>::type, Size>;
};
template<typename T, size_t Size> struct array_multi_type<T, Size> {
	using type = std::array<T, Size>;
};
template<typename T, size_t Size, size_t ...Sizes> using array_multi = typename array_multi_type<T, Size, Sizes...>::type;
