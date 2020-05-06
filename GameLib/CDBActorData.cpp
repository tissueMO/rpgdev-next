//###################################################################
//			アクター（パーティキャラ/エネミーを統合）情報
//###################################################################
#include "Header.h"


//####################  グラフィックを読み込む  ####################
void CDatabase::CDBActorData::LoadGraphics(string fileName) {
	this->GraphHandle = Game.Material.getGraphics()->Add(fileName);
	this->GraphSize = Game.DXLib.GetGraphSize(this->GraphHandle);
}


//####################  グラフィックを破棄する  ####################
void CDatabase::CDBActorData::DisposeGraphics() {
	Game.Material.getGraphics()->Delete(this->GraphHandle);
}


//####################  指定した属性を追加する  ####################
void CDatabase::CDBActorData::SetAttribute(int attrID) {
	auto tgt = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Attribute))->GetRowFromID(attrID);
	if(tgt == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しない属性ID [%d] で追加しようとしました。", attrID);
	} else {
		string attrName = *tgt->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		if(this->Attributes.Exists(attrID)) {
			//存在する場合は重複カウンターを加算
			this->Attributes.Get(this->Attributes.GetIndexFromID(attrID))->Value++;
		} else {
			//存在しない場合は重複カウンターを初期化して追加する
			this->Attributes.Add(CIDValueData(attrID, attrName, 1));
		}
	}
}


//####################  指定したクラスを追加する  ####################
//NOTE: クラスは唯一パラメーター変動が倍率指定で行われるため、既に適用されている変動を一旦解除してから適用する必要がある。
void CDatabase::CDBActorData::SetClass(int ID) {
	if(this->Classes.GetIndexFromID(ID) != -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "クラス [%d: %s] は既に対象アクター [%d: %s] に設定されています。", ID, Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(), this->ID, this->Name.c_str());
		return;
	}

	this->releaseAddonAll();		//一旦アドオンをすべて解除する
	this->baseSetDataWithAddon(ID, this->Classes, TS::DB::FDBIndices::Class, TS::DB::DBValueIndices::SelfClass, TS::DB::AddonFrom::Count);		//AddonFrom::Count にすることでアドオン適用を回避させる
	this->applyAddonAll();			//アドオン・パラメーター変動をすべて再適用する

	//属性の適用
	for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
		this->SetAttribute(attr);
	}

	//スキルの適用
	for(auto& skill : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameSkill.c_str())->GetIDValues()->getData()) {
		if(this->Level >= skill.Value) {
			//習得レベルに達している場合は覚えさせる
			this->SetSkill(skill.ID);
		} else {
			//習得レベルに達していない場合は予定リストに送る
			this->PotentialSkills.Add(skill);
		}
	}
}


//####################  指定した装備を追加する  ####################
bool CDatabase::CDBActorData::SetEquip(int ID, bool absolute) {
	//関連アドオン: 装備固定・大種別指定・小種別指定
	if(absolute == false && !this->CheckEquip(ID)) {
		auto row = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ID);
		if(row == nullptr) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "無効なアイテム [%d] を対象アクター [%d: %s] に装備しようとしました。", ID, this->ID, this->Name.c_str());
		} else {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "アイテム [%d: %s] を対象アクター [%d: %s] に装備できません。", ID, row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(), this->ID, this->Name.c_str());
		}
		return false;
	}

	//既に同一の大種別を装備している状態の場合は先に解除する
	CIntegerArray itemTypeIDs;
	Game.DB.GetItemTypes(ID, &itemTypeIDs, nullptr);
	for(auto& id : itemTypeIDs.getData()) {
		this->ReleaseEquipType(id, absolute);
	}

	//装備処理
	this->baseSetDataWithAddon(ID, this->Equips, TS::DB::FDBIndices::Item, TS::DB::DBValueIndices::SelfUse, TS::DB::AddonFrom::Item);

	//属性の適用
	for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
		this->SetAttribute(attr);
	}

	//パラメーター変動値の適用
	for(auto& par : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameParameter.c_str())->GetIDValues()->getData()) {
		this->Parameters.GetFromID(par.ID)->Value += par.Value;
	}
	return true;
}


//####################  指定したスキルを追加する  ####################
void CDatabase::CDBActorData::SetSkill(int ID) {
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0);
	auto row = db->GetRowFromID(ID);
	if(row == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないID [%d] で習得スキルに追加しようとしました。", ID);
	} else {
		const int index = this->LearnedSkills.GetIndexFromID(ID);
		if(index != -1) {
			//既に習得している場合は重複カウンターを加算
			this->LearnedSkills.Get(index)->Value++;
		} else {
			//新規追加
			CActorSubData newData;
			newData.ID = ID;
			newData.Name = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
			newData.Value = 1;

			//個別変数の初期値をセット
			Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfUse), &newData.SelfValues);

			this->LearnedSkills.Add(newData);
		}
	}
}


//####################  指定した間接効果を追加する  ####################
//返値は正常に追加できたかどうか
bool CDatabase::CDBActorData::SetState(int ID, bool absolute) {
	if (absolute == true || TS_RAND(100) < 100 - this->GetStateResistance(ID)) {
		//罹患させる
		if (this->States.GetIndexFromID(ID) != -1) {
			//既に罹患している場合は、重ね掛けをするか上位置換のどちらかになる
			const int upperStateID = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameStateUpper.c_str())->GetIntegerPTR();
			if (upperStateID != -1) {
				//上位置換を行う
				TS_LOG("重ね掛けのため、間接効果 [%d: %s] を上位互換の [%d: %s] に置き換えます。",
					ID,
					Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
					upperStateID,
					Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(upperStateID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
				);

				//下位効果を解除
				this->baseDeleteDataWithAddon(ID, this->States, TS::DB::FDBIndices::State, TS::DB::AddonFrom::State);
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DBStateEnd, ID, this, true);		//解除したときの処理をスクリプト委託する

				//上位効果を罹患
				return this->SetState(upperStateID, true);
			}
		}

		//間接効果耐性を考慮した上で追加する
		this->baseSetDataWithAddon(ID, this->States, TS::DB::FDBIndices::State, TS::DB::DBValueIndices::SelfState, TS::DB::AddonFrom::State);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DBStateStart, ID, this);		//罹患したときの処理をスクリプト委託する
		return true;
	}

	return false;
}


//####################  指定した属性を削除する  ####################
void CDatabase::CDBActorData::DeleteAttribute(int attrID) {
	const int index = this->Attributes.GetIndexFromID(attrID);
	if(index == -1) {
		return;
	}
	//重複カウンターがゼロになったら削除できる
	this->Attributes.Get(index)->Value--;
	if(this->Attributes.Get(index)->Value <= 0) {
		this->Attributes.DeleteFromIndex(index);
	}
}


//####################  指定したクラスを削除する  ####################
bool CDatabase::CDBActorData::ReleaseClass(int ID) {
	if(this->Classes.GetIndexFromID(ID) == -1) {
		return false;
	}

	this->releaseAddonAll();		//一旦アドオンをすべて解除する
	this->baseDeleteDataWithAddon(ID, this->Classes, TS::DB::FDBIndices::Class, TS::DB::AddonFrom::Count);		//AddonFrom::Count にすることでアドオン適用を回避させる
	this->applyAddonAll();			//アドオン・パラメーター変動をすべて再適用する

	//属性の解除
	for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
		this->DeleteAttribute(attr);
	}

	//スキルの解除
	for(auto& skill : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameSkill.c_str())->GetIDValues()->getData()) {
		if(this->Level >= skill.Value) {
			//習得済リストから削除
			this->ReleaseSkill(skill.ID);
		}
		//習得予定リストからは必ず削除する
		this->PotentialSkills.DeleteFromID(skill.ID);
	}
	return true;
}


//####################  指定した装備を削除する  ####################
//返値は正常に削除できたかどうか
bool CDatabase::CDBActorData::ReleaseEquip(int ID, bool absolute) {
	if(this->Equips.GetIndexFromID(ID) == -1) {
		return false;
	}

	//関連アドオン: 装備固定
	bool OK = true;
	CIntegerArray itemTypeIDs;
	Game.DB.GetItemTypes(ID, &itemTypeIDs, nullptr);
	for(auto& id : itemTypeIDs.getData()) {
		if(absolute == false && this->CheckEquipLock(id)) {
			OK &= false;
			continue;
		}

		this->baseDeleteDataWithAddon(ID, this->Equips, TS::DB::FDBIndices::Item, TS::DB::AddonFrom::Item);

		//属性の解除
		for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
			this->DeleteAttribute(attr);
		}

		//パラメーター変動値の解除
		for(auto& par : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameParameter.c_str())->GetIDValues()->getData()) {
			this->Parameters.GetFromID(par.ID)->Value -= par.Value;
		}
		break;
	}
	return OK;
}


//####################  指定したスキルを削除する  ####################
bool CDatabase::CDBActorData::ReleaseSkill(int ID) {
	const int index = this->LearnedSkills.GetIndexFromID(ID);
	if(index == -1) {
		return false;
	}
	//重複カウンターがゼロになったら削除できる
	this->LearnedSkills.Get(index)->Value--;
	if(this->LearnedSkills.Get(index)->Value <= 0) {
		this->LearnedSkills.DeleteFromIndex(index);
	}
	return true;
}


//####################  指定した間接効果を削除する  ####################
bool CDatabase::CDBActorData::ReleaseState(int ID) {
	if(this->States.GetIndexFromID(ID) == -1) {
		return false;
	}
	this->baseDeleteDataWithAddon(ID, this->States, TS::DB::FDBIndices::State, TS::DB::AddonFrom::State);
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DBStateEnd, ID, this, false);		//解除したときの処理をスクリプト委託する
	return true;
}


//####################  クラスをすべて削除する  ####################
void CDatabase::CDBActorData::ClearClass() {
	// 解除できないクラスは存在しないため問答無用で一つずつ解除していく
	while(this->Classes.Count() > 0) {
		this->ReleaseClass(this->Classes.GetID(0));
	}
}


//####################  スキルをすべて削除する  ####################
//注意：習得予定のスキルは維持される
void CDatabase::CDBActorData::ClearSkill() {
	// 忘却できないクラスは存在しないため問答無用で一つずつ忘却していく
	while(this->LearnedSkills.Count() > 0) {
		this->ReleaseSkill(this->LearnedSkills.GetID(0));
	}
}


//####################  装備品をすべて削除する  ####################
void CDatabase::CDBActorData::ClearEquip(bool absolute) {
	int index = 0;
	while(index < this->Equips.Count()) {
		if (!this->ReleaseEquip(this->Equips.GetID(index), absolute)) {
			// 解除できないものはスキップする
			index++;
		}
	}
}


//####################  間接効果をすべて削除する  ####################
void CDatabase::CDBActorData::ClearState() {
	// 解除できない間接効果は存在しないため問答無用で一つずつ解除していく
	while(this->States.Count() > 0) {
		this->ReleaseState(this->States.GetID(0));
	}
}


//####################  間接効果のバッドステータスのみを削除  ####################
void CDatabase::CDBActorData::ClearStateOnlyBad() {
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0);
	while(true) {
		//バッドステータスを検索
		int id = -1;
		for(auto& st : this->States.getData()) {
			if(Game.IntToBool(*db->GetCell(st.ID, TS::DB::ColumnNameStateBad.c_str())->GetIntegerPTR())) {
				id = st.ID;
				break;
			}
		}
		if(id == -1) {
			return;		//これ以上バッドステータスが見つからなければ終了
		}

		//該当した間接効果を削除
		this->ReleaseState(id);
	}
}


//####################  指定した属性の耐性を取得する  ####################
int CDatabase::CDBActorData::GetAttributeResistance(int ID) {
	int temp = 100;		//基本値
	for(auto& data : this->rstAttribute) {
		if(get<1>(data).ID == ID) {
			temp += (get<1>(data).Value - 100);
		}
	}
	return temp;
}


//####################  指定した間接効果の耐性を取得する  ####################
int CDatabase::CDBActorData::GetStateResistance(int ID) {
	int temp = 0;		//基本値
	for(auto& data : this->rstState) {
		if(get<1>(data).ID == ID) {
			temp += get<1>(data).Value;
		}
	}
	return temp;
}


//####################  装備大種別から装備品IDを取得する  ####################
int CDatabase::CDBActorData::GetEquipIDFromItemTypeID(int itemTypeID) {
	for(auto& eq : this->Equips.getData()) {
		CIntegerArray types;
		Game.DB.GetItemTypes(eq.ID, &types, nullptr);
		for(auto& id : types.getData()) {
			if(id == itemTypeID) {
				return eq.ID;
			}
		}
	}
	return -1;
}


//####################  指定した装備大種別が固定されているか調べる  ####################
bool CDatabase::CDBActorData::CheckEquipLock(int itemTypeID) {
	int pri = INT_MIN;		//優先度は最低
	int temp = FALSE;		//デフォルトで使える状態
	for(auto& data : this->lckEquipType) {
		if(get<0>(data) > pri && get<1>(data).ID == itemTypeID) {
			//暫定値よりも優先度が高い場合はそれを代表値にする
			pri = get<0>(data);
			temp = get<1>(data).Value;
		}
	}
	return Game.IntToBool(temp);
}


//####################  指定した装備品を装備できるか調べる  ####################
bool CDatabase::CDBActorData::CheckEquip(int itemID) {
	CIntegerArray itemTypeIDs, itemSubTypeIDs;

	//例外チェック
	if(!Game.DB.GetItemTypes(itemID, &itemTypeIDs, &itemSubTypeIDs)) {
		return false;		//無効なアイテムID
	}
	for(auto& id : itemTypeIDs.getData()) {
		if(this->CheckEquipLock(id)) {
			return false;		//固定された大種別
		}
	}

	//装備大種別の装備可否をチェック
	int pri = INT_MIN;			//優先度は最低
	for(auto& typeID : itemTypeIDs.getData()) {
		int temp = TRUE;			//デフォルトで装備可能
		for(auto& data : this->pmsEquipType) {
			if(get<0>(data) > pri && get<1>(data).ID == typeID) {
				//暫定値よりも優先度が高い場合はそれを代表値にする
				pri = get<0>(data);
				temp = get<1>(data).Value;
			}
		}
		if(!Game.IntToBool(temp)) {
			return false;		//代表値が使えない状態を示していたらここで終わる
		}
	}

	//装備小種別の装備可否をチェック: 対象アイテムに小種別が設定されていない場合は誰でも装備できるアイテムとみなす
	pri = INT_MIN;			//優先度は最低
	if(itemSubTypeIDs.Count() > 0) {
		for(auto& subTypeID : itemSubTypeIDs.getData()) {
			int temp = FALSE;		//デフォルトで装備不可
			for(auto& data : this->pmsEquipSubType) {
				if(get<0>(data) > pri && get<1>(data).ID == subTypeID) {
					//暫定値よりも優先度が高い場合はそれを代表値にする
					pri = get<0>(data);
					temp = get<1>(data).Value;
				}
			}
			if(!Game.IntToBool(temp)) {
				return false;		//代表値が使えない状態を示していたらここで終わる
			}
		}
	}

	return true;
}


//####################  指定した装備大種別の装備品を解除する  ####################
//返値は正常に解除できたかどうか
bool CDatabase::CDBActorData::ReleaseEquipType(int itemTypeID, bool absolute) {
	if(absolute == false && this->CheckEquipLock(itemTypeID)) {
		return false;
	}

	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0);
	for(auto& eq : this->Equips.getData()) {
		if(db->GetCell(eq.ID, TS::DB::ColumnNameItemType.c_str())->GetIntegers()->Exists(itemTypeID)) {
			return this->ReleaseEquip(eq.ID, absolute);
		}
	}
	return true;		//解除するものがなくても正常とみなす
}


//####################  指定した制約に相当する間接効果を罹患しているかどうかを調べる  ####################
bool CDatabase::CDBActorData::CheckStateLimit(int stateLimit) {
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0);

	for(auto state : this->States.getData()) {
		if(*db->GetCell(state.ID, TS::DB::ColumnNameStateLimit.c_str())->GetIntegerPTR() == stateLimit) {
			return true;
		}
	}
	return false;
}


//####################  指定したスキルが使える状態かどうかを調べる  ####################
bool CDatabase::CDBActorData::CheckUseSkill(int skillID) {
	//有効なIDかどうかのチェック
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0);
	const auto row = db->GetRowFromID(skillID);
	if(row == nullptr) {
		return false;
	}

	//習得チェック
	if(this->LearnedSkills.GetIndexFromID(skillID) == -1) {
		return false;
	}

	//装備小種別の使用許可チェック
	auto itemSubTypeIDs = db->GetCell(skillID, TS::DB::ColumnNameItemSubType.c_str())->GetIntegers();
	if(itemSubTypeIDs->Count() > 0) {
		//使用許可設定がある場合はいずれか一つを装備していればOK
		auto itemDB = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0);
		bool OK = false;

		//装備品をすべてチェック
		for(auto& equip : this->Equips.getData()) {
			//対象スキルの小種別と部分一致するか調べる
			auto itemSubTypeIDs_equip = itemDB->GetCell(equip.ID, TS::DB::ColumnNameItemSubType.c_str())->GetIntegers();
			for(auto& subTypeIDs : itemSubTypeIDs_equip->getData()) {
				if(!itemSubTypeIDs->Exists(subTypeIDs)) {
					continue;
				}
				OK |= true;
				break;
			}
			if(OK) {
				break;
			}
		}
		if (!OK) {
			return false;
		}
	}

	//スキル種別の使用制限チェック
	int skillTypeID = *db->GetCell(skillID, TS::DB::ColumnNameSkillType.c_str())->GetIntegerPTR();
	if(!this->CheckUseSkillType(skillTypeID)) {
		return false;
	}

	//スキル個別の使用制限チェック
	int pri = INT_MIN;		//優先度は最低
	int temp = TRUE;		//デフォルトで使える状態
	for(auto& data : this->pmsSkillUse) {
		if(get<1>(data).ID == skillID && get<0>(data) > pri) {
			//暫定値よりも優先度が高い場合はそれを代表値にする
			pri = get<0>(data);
			temp = get<1>(data).Value;
		}
	}
	return Game.IntToBool(temp);
}


//####################  指定した種別のスキルを使用できるかどうかを検証する  ####################
bool CDatabase::CDBActorData::CheckUseSkillType(int skillTypeID) {
	int pri = INT_MIN;		//優先度は最低
	int temp = TRUE;		//デフォルトで使える状態
	for(auto& data : this->pmsSkillType) {
		if(get<1>(data).ID == skillTypeID && get<0>(data) > pri) {
			//暫定値よりも優先度が高い場合はそれを代表値にする
			pri = get<0>(data);
			temp = get<1>(data).Value;
		}
	}
	return Game.IntToBool(temp);
}


//####################  経験値を相対的に操作する  ####################
//経験値に合わせてレベルが変動するが、減らしてもレベルが下がることはない
//返値は変動したレベル数
int CDatabase::CDBActorData::PlusEXP(int exp) {
	if(exp > 0 && this->EXP == INT_MAX) {
		//既に経験値がカンストしている場合は何もしない
	} else if(exp > 0 && exp > INT_MAX - this->EXP) {
		//経験値をカンストさせる
		this->EXP = INT_MAX;
	} else {
		//通常通り加算
		this->EXP += exp;
	}

	//レベルアップ判定・レベル変動操作・パラメーター更新
	const int beforeLv = this->Level;
	int lv = this->Level;		//現在の経験値に対応するレベル暫定値
	int beforeEXP = CDBActorData::getCurveValue(lv, this->EXPCurve);
	while(beforeEXP < CDBActorData::getCurveValue(lv + 1, this->EXPCurve)
			&& CDBActorData::getCurveValue(lv + 1, this->EXPCurve) <= this->EXP) {
		beforeEXP = CDBActorData::getCurveValue(lv + 1, this->EXPCurve);
		lv++;					//次のレベルに必要な経験値を上回っている場合は次に進める
	}
	this->LevelUp(lv - this->Level);

	return this->Level - beforeLv;
}


//####################  レベルを相対的に操作する  ####################
//レベルを下げることもできるが、この場合は経験値が減らない
//返値は次のレベルアップに必要な経験値
int CDatabase::CDBActorData::LevelUp(int val) {
	this->LevelUpParameters.Clear();
	this->LevelUpSkills.Clear();

	if(val == 0) {
		return this->GetNextEXP();		//変動がない場合は処理しない
	}

	const int maxLevel = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxLevel.c_str());
	const int beforeLevel = this->Level;
	if(beforeLevel >= maxLevel) {
		//既に最大レベルに達している場合は処理できない
		return INT_MAX;
	}
	this->Level += val;
	if(this->Level > maxLevel) {
		//レベルアップ後に最大レベルを超えてしまう場合は調整する
		this->Level = maxLevel;
	}

	//操作後のレベルに必要な経験値を満たしていない場合は足す
	const int afterLvFirstEXP = CDBActorData::getCurveValue(this->Level, this->EXPCurve);
	if(this->EXP < afterLvFirstEXP) {
		this->EXP = afterLvFirstEXP;
	}

	//変動前のパラメーターを一時的に記憶する
	TS_COUNTLOOP(i, Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowCount()) {
		this->LevelUpParameters.Add(*this->Parameters.GetFromIndex(i));
	}

	//クラスはパラメーターを倍率で動かしているので、変動前にすべて解除する必要がある
	vector<int> classIDs;
	while(this->Classes.Count() > 0) {
		const int id = this->Classes.GetID(0);
		classIDs.push_back(id);
		this->ReleaseClass(id);
	}

	//パラメーターを更新する: 曲線から生成される基本値での差分で計算する
	TS_COUNTLOOP(i, Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowCount()) {
		const int beforePar = CDBActorData::getCurveValue(beforeLevel, *this->ParameterCurve.GetFromIndex(i));
		const int afterPar = CDBActorData::getCurveValue(this->Level, *this->ParameterCurve.GetFromIndex(i));
		const int delta = afterPar - beforePar;
		this->Parameters.GetFromIndex(i)->Value += delta;
	}

	//解除したクラスをすべて復元する
	for(auto id : classIDs) {
		this->SetClass(id);
	}

	//ここで変動後のパラメーターとの差分を出す
	for(auto& par : this->LevelUpParameters.getData()) {
		par.Value = this->Parameters.GetFromID(par.ID)->Value - par.Value;
	}

	//新しいスキルを習得させる
	for(auto& pskl : this->PotentialSkills.getData()) {
		if(beforeLevel < pskl.Value && pskl.Value <= this->Level) {
			//このレベルアップによって習得レベルに達した場合は習得させる
			this->SetSkill(pskl.ID);

			//習得したスキルをレベルアップ習得スキルリストに登録する: スキル名が格納されていないデータを使うので、ここで取得する
			pskl.Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(pskl.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
			this->LevelUpSkills.Add(pskl);
		}
	}
	return this->GetNextEXP();
}


//####################  次のレベルアップに必要な経験値を取得する  ####################
int CDatabase::CDBActorData::GetNextEXP() {
	if(this->Level >= Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxLevel.c_str())) {
		//最大レベルの時は常に最大値で返す
		return INT_MAX;
	}
	const int afterLvFirstEXP = CDBActorData::getCurveValue(this->Level + 1, this->EXPCurve);
	return afterLvFirstEXP - this->EXP;
}


//####################  アドオン適用をすべて一括解除する  ####################
//優先度が高い順に取り外す
void CDatabase::CDBActorData::releaseAddonAll() {
	//間接効果
	for(auto& state : this->States.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetCell(state.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->releaseAddonOne(&addons, TS::DB::AddonFrom::State);
	}

	//装備
	for(auto& equip : this->Equips.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(equip.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->releaseAddonOne(&addons, TS::DB::AddonFrom::Item);
	}

	//クラス
	for(auto& cls : this->Classes.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->releaseAddonOne(&addons, TS::DB::AddonFrom::Class);

		//パラメーター変動率を解除する
		auto parRates = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameParameterRate.c_str())->GetIDValues();
		for(auto& par : parRates->getData()) {
			this->Parameters.GetFromID(par.ID)->Value = this->Parameters.GetFromID(par.ID)->Value * 100.0 / par.Value + 0.5;		//四捨五入をしないと正確に復元できなくなる
		}
	}

	//NOTE: アクター情報は操作しない。
}


//####################  アドオンをすべて一括適用させる  ####################
//優先度が低い順に適用する
void CDatabase::CDBActorData::applyAddonAll() {
	//NOTE: アクター情報は操作しない。

	//クラス
	for(auto& cls : this->Classes.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->applyAddonOne(&addons, TS::DB::AddonFrom::Class);

		//パラメーター変動率を適用させる
		auto parRates = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameParameterRate.c_str())->GetIDValues();
		for(auto& par : parRates->getData()) {
			this->Parameters.GetFromID(par.ID)->Value = this->Parameters.GetFromID(par.ID)->Value * par.Value / 100.0 + 0.5;		//四捨五入をしないと正確に復元できなくなる
		}
	}

	//装備
	for(auto& equip : this->Equips.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(equip.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->applyAddonOne(&addons, TS::DB::AddonFrom::Item);
	}

	//間接効果
	for(auto& state : this->States.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetCell(state.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->applyAddonOne(&addons, TS::DB::AddonFrom::State);
	}
}


//####################  アドオン付きのデータリストに追加する  ####################
void CDatabase::CDBActorData::baseSetDataWithAddon(int ID, CActorSubDataArray& tgt, TS::DB::FDBIndices FDBType, TS::DB::DBValueIndices selfValueType, TS::DB::AddonFrom priority) {
	auto db = Game.DB.FDB[static_cast<int>(FDBType)]->GetSubDB(0);
	auto row = db->GetRowFromID(ID);
	if(row == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないID [%d] でアドオン付きのデータリストに追加しようとしました。", ID);
	} else {
		CActorSubData newData;
		newData.ID = ID;
		newData.Name = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();

		//個別変数の初期値をセット
		Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(selfValueType), &newData.SelfValues);

		//アドオンを適用
		if(priority != TS::DB::AddonFrom::Count) {
			auto addons = row->at(db->GetColumnIndex(TS::DB::ColumnNameAddon.c_str())).ConvertToAddons();
			this->applyAddonOne(&addons, priority);
		}
		tgt.Add(newData);
	}
}


//####################  アドオン付きのデータリストから削除する  ####################
void CDatabase::CDBActorData::baseDeleteDataWithAddon(int ID, CActorSubDataArray& tgt, TS::DB::FDBIndices FDBType, TS::DB::AddonFrom priority) {
	auto db = Game.DB.FDB[static_cast<int>(FDBType)]->GetSubDB(0);
	if(priority != TS::DB::AddonFrom::Count) {
		auto addons = db->GetRowFromID(ID)->at(db->GetColumnIndex(TS::DB::ColumnNameAddon.c_str())).ConvertToAddons();
		this->releaseAddonOne(&addons, priority);
	}
	tgt.DeleteFromID(ID);
}


//####################  このアクターにアドオンを適用する  ####################
//情報を上書きするタイプのアドオンは、自分より優先度の高いアドオンがあれば操作せず、追加するときは解除時のために優先度を記憶する
void CDatabase::CDBActorData::applyAddonOne(CDatabase::CDBCell::CDBAddons* addons, TS::DB::AddonFrom priority) {
	for(auto& addon : addons->getData()) {
		string name;
		auto newSet = make_tuple(static_cast<int>(priority), CIDValueData(addon.ID, "Unknown", addon.Value));
		switch((TS::DBLiteral::Addon)addon.Type) {
			case TS::DBLiteral::Addon::SelfValuePlus:
				this->SelfValues.Get(addon.ID)->Value += addon.Value;
				break;
			case TS::DBLiteral::Addon::ParameterPlus:
				this->Parameters.GetFromID(addon.ID)->Value += addon.Value;
				break;

			case TS::DBLiteral::Addon::AttributeResistanceSet:
				get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Attribute))->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
				this->rstAttribute.push_back(newSet);
				break;
			case TS::DBLiteral::Addon::StateResistanceSet:
				get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
				this->rstState.push_back(newSet);
				break;

			case TS::DBLiteral::Addon::LockEquipType:
				if(!this->checkHigherAddon(this->lckEquipType, newSet)) {
					get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType))->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->lckEquipType.push_back(newSet);
				}
				break;
			case TS::DBLiteral::Addon::SetEquipType:
				if(!this->checkHigherAddon(this->pmsEquipType, newSet)) {
					get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType))->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->pmsEquipType.push_back(newSet);
				}
				break;
			case TS::DBLiteral::Addon::SetEquipSubType:
				if(!this->checkHigherAddon(this->pmsEquipSubType, newSet)) {
					get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemSubType))->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->pmsEquipSubType.push_back(newSet);
				}
				break;
			case TS::DBLiteral::Addon::SetSkillUse:
				if(!this->checkHigherAddon(this->pmsSkillUse, newSet)) {
					get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->pmsSkillUse.push_back(newSet);
				}
				break;
			case TS::DBLiteral::Addon::SetSkillType:
				if(!this->checkHigherAddon(this->pmsSkillType, newSet)) {
					get<1>(newSet).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::SkillType))->GetRowFromID(get<1>(newSet).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->pmsSkillType.push_back(newSet);
				}
				break;
		}
	}
}


//####################  このアクターに付加されたアドオンを解除する  ####################
//情報を上書きするタイプのアドオンは、自分より優先度の高いアドオンによって上書きされていたら解除しないようにする
//情報を上乗せするタイプのアドオンは、自分の分を差し引く
void CDatabase::CDBActorData::releaseAddonOne(CDatabase::CDBCell::CDBAddons* addons, TS::DB::AddonFrom priority) {
	for(auto& addon : addons->getData()) {
		string name;
		auto set = make_tuple(static_cast<int>(priority), CIDValueData(addon.ID, "Unknown", addon.Value));
		switch((TS::DBLiteral::Addon)addon.Type) {
			case TS::DBLiteral::Addon::SelfValuePlus:
				this->SelfValues.Get(addon.ID)->Value -= addon.Value;
				break;
			case TS::DBLiteral::Addon::ParameterPlus:
				//NOTE: クラスはパラメーター変動率の設定があるため [アドオン:パラメーター増減] が無効になる
				if(priority != TS::DB::AddonFrom::Class) {
					this->Parameters.GetFromID(addon.ID)->Value -= addon.Value;
				}
				break;

			case TS::DBLiteral::Addon::AttributeResistanceSet:
				get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Attribute))->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
				this->deleteAddonBuffer(this->rstAttribute, set);
				break;
			case TS::DBLiteral::Addon::StateResistanceSet:
				get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
				this->deleteAddonBuffer(this->rstState, set);
				break;

			case TS::DBLiteral::Addon::LockEquipType:
				if(!this->checkHigherAddon(this->lckEquipType, set)) {
					get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType))->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->deleteAddonBuffer(this->lckEquipType, set);
				}
				break;
			case TS::DBLiteral::Addon::SetEquipType:
				if(!this->checkHigherAddon(this->pmsEquipType, set)) {
					get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType))->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->deleteAddonBuffer(this->pmsEquipType, set);
				}
				break;
			case TS::DBLiteral::Addon::SetEquipSubType:
				if(!this->checkHigherAddon(this->pmsEquipSubType, set)) {
					get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemSubType))->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->deleteAddonBuffer(this->pmsEquipSubType, set);
				}
				break;
			case TS::DBLiteral::Addon::SetSkillUse:
				if(!this->checkHigherAddon(this->pmsSkillUse, set)) {
					get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->deleteAddonBuffer(this->pmsSkillUse, set);
				}
				break;
			case TS::DBLiteral::Addon::SetSkillType:
				if(!this->checkHigherAddon(this->pmsSkillType, set)) {
					get<1>(set).Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::SkillType))->GetRowFromID(get<1>(set).ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
					this->deleteAddonBuffer(this->pmsSkillType, set);
				}
				break;
		}
	}
}


//####################  指定したアドオン履歴から、指定した優先度よりも高く、IDが重複するものがあるかどうかを調べる  ####################
bool CDatabase::CDBActorData::checkHigherAddon(vector<tuple<int, CIDValueData>>& list, tuple<int, CIDValueData>& base) {
	for(auto& addonbuf : list) {
		if(get<0>(addonbuf) > get<0>(base) && get<1>(addonbuf).ID == get<1>(base).ID) {
			return true;		//baseの効果を反映/解除すべきでない状態
		}
	}
	return false;
}


//####################  指定したスキルがユーザーによって後から追加されたものであるかどうかを調べる  ####################
//習得重複回数から習得予定スキルの個数を差し引いてもゼロにならない場合はユーザーによって追加されたとみなせる
bool CDatabase::CDBActorData::checkUserAddedSkill(int ID) {
	const int index = this->LearnedSkills.GetIndexFromID(ID);
	if(index == -1) {
		return false;		//無効なスキル
	}
	int counter = this->LearnedSkills.Get(index)->Value;
	for(auto& pskl : this->PotentialSkills.getData()) {
		if(pskl.ID == ID) {
			counter--;
		}
	}
	return (counter > 0);
}


//####################  指定したX値をもとに曲線値を生成する  ####################
int CDatabase::CDBActorData::getCurveValue(int x, CCurveData& cv) {
	if(x <= 1) {
		return cv.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Base)];
	} else {
		auto temp = (cv.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Base)]
			+ (cv.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Coefficient)] / 1000.0)
			* ::pow((double)x, cv.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Index)] / 1000.0));
		if(temp < INT_MAX) {
			return static_cast<int>(temp);
		} else {
			//int型のオーバーフローを防ぐためにカンストさせる
			return INT_MAX;
		}
	}
}


//####################  指定したアドオン履歴から、指定したアドオンを１つ取り除く  ####################
void CDatabase::CDBActorData::deleteAddonBuffer(vector<tuple<int, CIDValueData>>& list, tuple<int, CIDValueData>& base) {
	TS_COUNTLOOP(i, list.size()) {
		if(get<0>(list[i]) == get<0>(base) && get<1>(list[i]).ID == get<1>(base).ID) {
			list.erase(list.begin() + i);
			return;
		}
	}
}
