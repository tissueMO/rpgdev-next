//###################################################################
//			パーティ データベース
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  読み込み処理  ####################
bool CDatabase::CDBParty::Load(string fileName) {
	//ファイルからは読み込まず、初期設定DBから取り込む
	auto db = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Init));
	this->fileName = db->GetDBFileName();
	auto party = db->GetSubDB(0)->GetCellFromIndex(0, static_cast<int>(TS::DB::DBInitIndices::Party))->GetIntegers();
	auto items = db->GetSubDB(0)->GetCellFromIndex(0, static_cast<int>(TS::DB::DBInitIndices::Item))->GetIDValues();
	this->Money = *db->GetSubDB(0)->GetCellFromIndex(0, static_cast<int>(TS::DB::DBInitIndices::Money))->GetIntegerPTR();

	//パーティメンバーを登録
	this->members.Clear();
	TS_COUNTLOOP(i, party->Count()) {
		this->AddMember(party->Get(i), -1);
	}

	//所持アイテムを登録
	this->items.Clear();
	TS_COUNTLOOP(i, items->Count()) {
		this->AddItem(items->Get(i)->ID, items->Get(i)->Value);
	}
	return true;
}


//####################  セーブデータからのロード  ####################
void CDatabase::CDBParty::Load(vector<string>& vectorbuf) {
	string temp;
	vector<string> split;

	//パーティメンバーリストを読み込み
	temp = Game.PopHead(vectorbuf);
	Game.SplitString(split, temp, ";");
	this->members.Clear();
	for(auto& member : split) {
		this->AddMember(atoi(member.c_str()), -1);
	}

	//所持アイテムリストを読み込み
	temp = Game.PopHead(vectorbuf);
	Game.SplitString(split, temp, ";");
	this->items.Clear();
	for(auto& item : split) {
		vector<string> sub;
		Game.SplitString(sub, item, ":");
		if(sub.size() == 2) {
			this->AddItem(atoi(sub[0].c_str()), atoi(sub[1].c_str()));
		}
	}

	//所持金を読み込み
	this->Money = atoi(Game.PopHead(vectorbuf).c_str());
}


//####################  セーブデータへのセーブ  ####################
void CDatabase::CDBParty::Save(string& strbuf) {
	//パーティメンバーリストを書き込み
	TS_COUNTLOOP(i, this->members.GetCount()) {
		strbuf += Game.Format("%d", this->members.GetFromIndex(i)->ID);
		if(i < this->members.GetCount() - 1) {
			strbuf += ";";
		}
	}
	strbuf += "\n";

	//所持アイテムリストを書き込み
	TS_COUNTLOOP(i, this->items.GetCount()) {
		//個数も保存する
		strbuf += Game.Format("%d:%d", this->items.GetFromIndex(i)->ID, this->items.GetFromIndex(i)->Value);
		if(i < this->items.GetCount() - 1) {
			strbuf += ";";
		}
	}
	strbuf += "\n";

	//所持金を書き込み
	strbuf += Game.Format("%d\n", this->Money);
}


//####################  データベースの中身をログとして出力する  ####################
void CDatabase::CDBParty::PrintData() {
	TS_LOG("\nDB: パーティ情報");

	//パーティメンバーリストを書き込み
	TS_LOG("\nParty:");
	for(auto& member : this->members.Items) {
		TS_LOG("%d (%s)", member.ID, member.Name.c_str());
	}

	//所持アイテムリストを書き込み
	TS_LOG("\nHavingItem:");
	for(auto& item : this->items.Items) {
		TS_LOG("%d (%s) * %d", item.ID, item.Name.c_str(), item.Value);
	}

	//所持金を書き込み
	TS_LOG("\nHavingMoney: " + to_string(this->Money));

	TS_LOG("");		//空行を出力
}


//####################  破棄処理  ####################
void CDatabase::CDBParty::Dispose() {
	this->members.Clear();
	this->items.Clear();
}


//####################  メンバーを加入  ####################
//末尾に追加する場合は -1 を指定する
void CDatabase::CDBParty::AddMember(int id, int index) {
	if(this->CheckCharInParty(id) != -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "[ID: %d] は既にパーティに加入しています。", id);
		return;
	} else if(this->members.GetCount() >= Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxPartyCount)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "パーティ最大人数に達しているため、[ID: %d] をパーティに追加できません。", id);
		return;
	}
	auto cell = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(id);
	if(cell == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "無効なパーティキャラID [%d] でパーティに追加しようとしました。", id);
		return;
	}

	if(index > this->members.GetCount()) {
		index = -1;		//範囲外の場合は末尾に追加へ
	}

	//追加処理
	CIDNameData newData(id, *cell->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR());
	this->members.Insert(newData, index);
}


//####################  指定IDのメンバーを脱退  ####################
void CDatabase::CDBParty::DeleteMemberFromID(int id) {
	const int index = this->CheckCharInParty(id);
	if(index == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "パーティにいないキャラ [ID: %d] を脱退しようとしました。", id);
		return;
	} else if(this->members.GetCount() <= 1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "パーティを０人にすることはできません。");
		return;
	}
	this->DeleteMemberFromIndex(index);
}


//####################  指定位置のメンバーを脱退  ####################
void CDatabase::CDBParty::DeleteMemberFromIndex(int index) {
	if(this->members.GetCount() <= 1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "パーティを０人にすることはできません。");
		return;
	}
	this->members.DeleteFromIndex(index);
}


//####################  指定キャラがメンバーにいるか調べ、いる場合はその位置を返す  ####################
int CDatabase::CDBParty::CheckCharInParty(int id) {
	TS_COUNTLOOP(i, this->members.GetCount()) {
		if(id == this->members.GetFromIndex(i)->ID) {
			return i;
		}
	}
	return -1;
}


//####################  指定位置のキャラIDを調べる  ####################
int CDatabase::CDBParty::GetCharIDFromPartyIndex(int index) {
	if(index < 0 || this->GetCharCount() <= index) {
		return -1;
	} else {
		return this->members.GetFromIndex(index)->ID;
	}
}


//####################  メンバー数を返す  ####################
int CDatabase::CDBParty::GetCharCount() {
	return this->members.GetCount();
}


//####################  アイテムを所持  ####################
void CDatabase::CDBParty::AddItem(int id, int count) {
	auto cell = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(id);
	if(cell == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "無効なアイテムID [%d] で所持しようとしました。", id);
		return;
	} else if(count == 0) {
		return;
	} else if(count < 0) {
		//個数をマイナスにすると破棄処理を行う
		this->DeleteItem(id, -count);
		return;
	}

	//既に所持しているかどうかを調べる
	const int index = this->getHavingItemIndex(id);
	if(index == -1) {
		//新規追加
		CIDValueData newData(id, *cell->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR(), count);
		this->items.Add(newData);
	} else {
		//個数追加
		this->items.GetFromIndex(index)->Value += count;
	}
}


//####################  アイテムを破棄  ####################
void CDatabase::CDBParty::DeleteItem(int id, int count) {
	//所持していない場合は処理しない
	if(!this->items.Exists(id)) {
		return;
	} else if(count == 0) {
		return;
	} else if(count < 0) {
		//個数をマイナスにすると追加処理を行う
		this->AddItem(id, -count);
		return;
	}

	//所持数をチェック
	if(this->items.GetFromID(id)->Value <= count) {
		//削除処理
		this->items.DeleteFromID(id);
	} else {
		//個数減算
		this->items.GetFromID(id)->Value -= count;
	}
}


//####################  指定位置のアイテム情報を取得する  ####################
CIDValueData* CDatabase::CDBParty::GetItemDataFromIndex(int index) {
	return this->items.GetFromIndex(index);
}


//####################  所持アイテムの総数を返す  ####################
int CDatabase::CDBParty::GetItemCountAll() {
	return this->items.GetCount();
}


//####################  指定した大種別の所持アイテム数を返す  ####################
int CDatabase::CDBParty::GetItemTypeCount(int itemTypeID) {
	int count = 0;
	TS_COUNTLOOP(i, this->items.GetCount()) {
		//アイテムDBへ接続して種別を確認する
		const int currentID = this->items.GetFromIndex(i)->ID;
		CIntegerArray types;
		Game.DB.GetItemTypes(currentID, &types, nullptr);
		TS_COUNTLOOP(m, types.Count()) {
			if(itemTypeID == types.Get(m)) {
				count++;		//指定した大種別を含んでいる場合はカウントする
				break;
			}
		}
	}
	return count;
}


//####################  指定したIDのアイテムの所持アイテム数を返す  ####################
int CDatabase::CDBParty::GetItemCount(int itemID) {
	if(!this->items.Exists(itemID)) {
		return 0;
	}
	return this->items.GetFromID(itemID)->Value;
}


//####################  所持アイテムを、可視IDをもとに昇順・降順に整理する  ####################
void CDatabase::CDBParty::SortItemsByVisibleID(bool reverse) {
	//アイテムDBへ接続して可視IDで比較する
	if(!reverse) {
		std::sort(TS_CONTAINERALL(this->items.Items), [&](auto& x, auto& y)->int {
			const int idx = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->ConvertToVisibleID(x.ID);
			const int idy = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->ConvertToVisibleID(y.ID);
			return idx < idy;
		});
	} else {
		std::sort(TS_CONTAINERALL(this->items.Items), [&](auto& x, auto& y)->int {
			const int idx = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->ConvertToVisibleID(x.ID);
			const int idy = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item))->GetSubDB(0)->ConvertToVisibleID(y.ID);
			return idx > idy;
		});
	}
}


//####################  所持アイテムを、名前をもとに昇順・降順に整理する  ####################
void CDatabase::CDBParty::SortItemsByName(bool reverse) {
	this->items.Sort(static_cast<int>(TS::System::SortType::Name), reverse);
}


//####################  所持金をセットする  ####################
void CDatabase::CDBParty::setMoney(int val) {
	this->Money = Game.WithIn(val, 0, INT_MAX);
}


//####################  所持しているアイテムのリスト上のインデックスを取得する  ####################
int CDatabase::CDBParty::getHavingItemIndex(int id) {
	TS_COUNTLOOP(i, this->items.GetCount()) {
		if(this->items.GetFromIndex(i)->ID == id) {
			return i;
		}
	}
	return -1;
}
