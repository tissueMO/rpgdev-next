//###################################################################
//			�V���b�v�f�[�^�̒��g�Ɋւ���N���X��`
//###################################################################
#include "Header.h"


//####################  ���i��ǉ�  ####################
void CSceneMap::CShopData::Add(int id) {
	auto db = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0);
	if(db->GetRowFromID(id) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�ID�̃A�C�e�� [%d] ��񂵂悤�Ƃ��܂����B", id);
		return;
	}

	//�Œ���K�v�ȏ����ꏏ�ɃZ�b�g����
	string name = *db->GetCell(id, db->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).c_str())->GetStringPTR();
	int price = *db->GetCell(id, TS::DB::ColumnNameItemPrice)->GetIntegerPTR();

	this->ItemList.push_back(CIDValueData(id, name, price));
}


//####################  ���i���X�g��������  ####################
void CSceneMap::CShopData::Clear() {
	vector<CIDValueData>().swap(this->ItemList);
}


//####################  ���i�����擾  ####################
int CSceneMap::CShopData::GetCount() {
	return this->ItemList.size();
}


//####################  ���i�����擾  ####################
CIDValueData CSceneMap::CShopData::Get(int index) {
	TS_CHECKINDEX(index, 0, this->ItemList.size()) {
		return this->ItemList[index];
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��C���f�b�N�X�̏��i��� [%d] ���Q�Ƃ���܂����B", index);
		return CIDValueData();
	}
}


//####################  ��ID���ŕ��ёւ�  ####################
void CSceneMap::CShopData::SortItemsByVisibleID(bool reverse) {
	//�A�C�e��DB�֐ڑ����ĉ�ID�Ŕ�r����
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


//####################  ���O���ŕ��ёւ�  ####################
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


//####################  ���i���ŕ��ёւ�  ####################
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

