//###################################################################
//			ショップデータの中身に関するクラス定義
//###################################################################
#include "Header.h"


//####################  商品を追加  ####################
void CSceneMap::CShopData::Add(int id) {
	auto db = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0);
	if(db->GetRowFromID(id) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないIDのアイテム [%d] を陳列しようとしました。", id);
		return;
	}

	//最低限必要な情報も一緒にセットする
	string name = *db->GetCell(id, db->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).c_str())->GetStringPTR();
	int price = *db->GetCell(id, TS::DB::ColumnNameItemPrice)->GetIntegerPTR();

	this->ItemList.push_back(CIDValueData(id, name, price));
}


//####################  商品リストを初期化  ####################
void CSceneMap::CShopData::Clear() {
	vector<CIDValueData>().swap(this->ItemList);
}


//####################  商品数を取得  ####################
int CSceneMap::CShopData::GetCount() {
	return this->ItemList.size();
}


//####################  商品情報を取得  ####################
CIDValueData CSceneMap::CShopData::Get(int index) {
	TS_CHECKINDEX(index, 0, this->ItemList.size()) {
		return this->ItemList[index];
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないインデックスの商品情報 [%d] が参照されました。", index);
		return CIDValueData();
	}
}


//####################  可視ID順で並び替え  ####################
void CSceneMap::CShopData::SortItemsByVisibleID(bool reverse) {
	//アイテムDBへ接続して可視IDで比較する
	if(!reverse) {
		std::sort(TS_CONTAINERALL(this->ItemList), [](CIDValueData& x, CIDValueData& y)->int {
			int idx = *Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->GetCellFromIndex(x.ID, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR();
			int idy = *Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->GetCellFromIndex(y.ID, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR();
			return idx - idy;
		});
	} else {
		std::sort(TS_CONTAINERALL(this->ItemList), [](CIDValueData& x, CIDValueData& y)->int {
			int idx = *Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->GetCellFromIndex(x.ID, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR();
			int idy = *Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->GetCellFromIndex(y.ID, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR();
			return idy - idx;
		});
	}
}


//####################  名前順で並び替え  ####################
void CSceneMap::CShopData::SortItemsByName(bool reverse) {
	if(!reverse) {
		std::sort(TS_CONTAINERALL(this->ItemList), [](CIDValueData& x, CIDValueData& y)->int {
			return ::strcmp(x.Name.c_str(), y.Name.c_str());
		});
	} else {
		std::sort(TS_CONTAINERALL(this->ItemList), [](CIDValueData& x, CIDValueData& y)->int {
			return -::strcmp(x.Name.c_str(), y.Name.c_str());
		});
	}
}


//####################  価格順で並び替え  ####################
void CSceneMap::CShopData::SortItemsByPrice(bool reverse) {
	if(!reverse) {
		std::sort(TS_CONTAINERALL(this->ItemList), [](CIDValueData& x, CIDValueData& y)->int {
			return x.Value - y.Value;
		});
	} else {
		std::sort(TS_CONTAINERALL(this->ItemList), [](CIDValueData& x, CIDValueData& y)->int {
			return y.Value - x.Value;
		});
	}
}

