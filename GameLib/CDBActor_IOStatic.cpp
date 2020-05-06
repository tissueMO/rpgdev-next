//###################################################################
//			アクター（パーティキャラ/エネミーを統合）情報
//###################################################################
#include "Header.h"


//####################  セーブデータからアクター情報を生成する  ####################
CDatabase::CDBActorData CDatabase::CDBActorData::Load(vector<string>& buf) {
	CDBActorData newData;
	newData.PotentialSkills.SetAllowOverlap(true);
	int id, val, count, cntsub;
	string temp;

	//基本情報
	sscanf(Game.PopHead(buf).c_str(), "%d", &newData.ID);

	//固定データベースから基本情報を持ってくる
	auto row = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(newData.ID);
	if(row == nullptr) {
		//無効なアクターの場合は終端まで読み切って中断する
		while(Game.PopHead(buf) != TS::SaveLoad::ActorSplit) {}
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないパーティキャラ [ID: %d] の復元をしようとしました。", newData.ID);
		return newData;
	} else {
		newData = CDBActorData::CreateActorFromChar(newData.ID);

		//セーブデータで復元する動的リストはすべて初期化する
		newData.ClearClass();
		newData.ClearEquip(true);
		newData.ClearSkill();
		newData.ClearState();
	}

	sscanf(Game.PopHead(buf).c_str(), "%d,%d", &newData.Level, &newData.EXP);
	newData.Name = Game.PopHead(buf);
	newData.Title = Game.PopHead(buf);

	//現在のレベルに対応する基本パラメーターを算出
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter));
	TS_COUNTLOOP(i, db->GetRowCount()) {
		newData.Parameters.GetFromIndex(i)->Value = newData.getCurveValue(newData.Level, *newData.ParameterCurve.GetFromIndex(i));
	}

	//クラス情報
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d", &subData.ID, &cntsub);

		//クラス個別変数
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfClass))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		newData.SetClass(subData.ID);
		*newData.Classes.GetSelfValues(newData.Classes.GetIndexFromID(subData.ID)) = subData.SelfValues;
	}

	//装備品情報
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d", &subData.ID, &cntsub);

		//アイテム個別変数
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfUse))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		newData.SetEquip(subData.ID, true);
		*newData.Equips.GetSelfValues(newData.Equips.GetIndexFromID(subData.ID)) = subData.SelfValues;
	}

	//間接効果情報
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d", &subData.ID, &cntsub);

		//クラス個別変数
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfState))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		newData.SetState(subData.ID, true);
		*newData.States.GetSelfValues(newData.States.GetIndexFromID(subData.ID)) = subData.SelfValues;
	}

	//習得済スキル情報
	newData.LearnedSkills.Clear();
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d,%d", &subData.ID, &subData.Value, &cntsub);

		//クラス個別変数
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfUse))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		subData.Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(subData.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		newData.LearnedSkills.Add(subData);
	}

	//アクター個別変数
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
		newData.SelfValues.Get(id)->Value = val;
		newData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfActor))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	}

	//パラメーター
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
		newData.Parameters.GetFromID(id)->Value = val;
		newData.Parameters.GetFromID(id)->Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	}

	return newData;
}


//####################  セーブ用の情報を生成する  ####################
void CDatabase::CDBActorData::Save(string& buf) {
	//基本情報
	buf += Game.Format("%d\n", this->ID);
	buf += Game.Format("%d,%d\n", this->Level, this->EXP);
	buf += this->Name + "\n";
	buf += this->Title + "\n";

	//クラス情報
	buf += to_string(this->Classes.Count()) + "\n";
	for(auto& item : this->Classes.getData()) {
		buf += Game.Format("%d,%d\n", item.ID, item.SelfValues.data.GetCount());

		//クラス個別変数
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//装備品情報
	buf += to_string(this->Equips.Count()) + "\n";
	for(auto& item : this->Equips.getData()) {
		buf += Game.Format("%d,%d\n", item.ID, item.SelfValues.data.GetCount());

		//アイテム個別変数
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//間接効果情報
	buf += to_string(this->States.Count()) + "\n";
	for(auto& item : this->States.getData()) {
		buf += Game.Format("%d,%d\n", item.ID, item.SelfValues.data.GetCount());

		//間接効果個別変数
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//習得済スキル情報
	buf += to_string(this->LearnedSkills.Count()) + "\n";
	for(auto& item : this->LearnedSkills.getData()) {
		buf += Game.Format("%d,%d,%d\n", item.ID, item.Value, item.SelfValues.data.GetCount());

		//スキル個別変数
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//アクター個別変数
	buf += to_string(this->SelfValues.data.GetCount()) + "\n";
	for(auto& item : this->SelfValues.data.Items) {
		buf += Game.Format("%d=%d\n", item.second.ID, item.second.Value);
	}

	//パラメーター
	buf += to_string(this->Parameters.GetCount()) + "\n";
	for(auto& item : this->Parameters.Items) {
		buf += Game.Format("%d=%d\n", item.ID, item.Value);
	}

	buf += TS::SaveLoad::ActorSplit + "\n";
}


//####################  ログ出力を行う  ####################
void CDatabase::CDBActorData::PrintData() {
	string buf;
	TS_LOG("%d (%s)", this->ID, this->Name.c_str());
	TS_LOG("\tグラフィック: %s", this->Graphics.c_str());
	TS_LOG("\tNPC: %s", TS_BOOLSTR(this->IsNPC));
	TS_LOG("\tLV: %d", this->Level);
	TS_LOG("\tEXP: %d (NextEXP: %d)", this->EXP, this->GetNextEXP());

	//アクター個別変数
	TS_LOG("\tアクター個別変数:");
	buf = "\t\t";
	for(auto& item : this->SelfValues.data.Items) {
		buf += Game.Format("%s=%d  ", item.second.Name.c_str(), item.second.Value);
	}
	TS_LOG(buf);

	//属性
	TS_LOG("\t属性:");
	buf = "\t\t";
	for(auto& item : this->Attributes.getData()) {
		buf += Game.Format("%s  ", item.Name.c_str());
	}
	TS_LOG(buf);

	//パラメーター
	TS_LOG("\tパラメーター:");
	buf = "\t\t";
	for(auto& item : this->Parameters.Items) {
		buf += Game.Format("%s=%d  ", item.Name.c_str(), item.Value);
	}
	TS_LOG(buf);

	//クラス
	TS_LOG("\tクラス:");
	for(auto& item : this->Classes.getData()) {
		buf = Game.Format("\t\t%s (", item.Name.c_str());

		//個別変数情報
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//装備品
	TS_LOG("\t装備品:");
	for(auto& item : this->Equips.getData()) {
		CIntegerArray itemTypeIDs;
		auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType));
		Game.DB.GetItemTypes(item.ID, &itemTypeIDs, nullptr);
		buf = Game.Format("\t\t%s%s = %s (", db->GetRowFromID(itemTypeIDs.Get(0))->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(), (itemTypeIDs.Count() > 1) ? "..." : "", item.Name.c_str());

		//個別変数情報
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//習得スキル
	TS_LOG("\t習得済スキル:");
	for(auto& item : this->LearnedSkills.getData()) {
		buf = Game.Format("\t\t%s (", item.Name.c_str());

		//個別変数情報
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//間接効果
	TS_LOG("\t罹患している間接効果:");
	for(auto& item : this->States.getData()) {
		buf = Game.Format("\t\t%s (", item.Name.c_str());

		//個別変数情報
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//属性耐性
	TS_LOG("\t属性耐性:");
	for(auto& item : this->rstAttribute) {
		TS_LOG("\t\t%s -> %3d %% (優先度 %d)", get<1>(item).Name.c_str(), get<1>(item).Value, get<0>(item));
	}

	//間接効果耐性
	TS_LOG("\t間接効果耐性:");
	for(auto& item : this->rstState) {
		TS_LOG("\t\t%s -> %3d %% (優先度 %d)", get<1>(item).Name.c_str(), get<1>(item).Value, get<0>(item));
	}

	//固定装備大種別
	TS_LOG("\t固定装備大種別:");
	for(auto& item : this->lckEquipType) {
		TS_LOG("\t\t%s -> %s (優先度 %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//装備大種別可否
	TS_LOG("\t装備大種別可否:");
	for(auto& item : this->pmsEquipType) {
		TS_LOG("\t\t%s -> %s (優先度 %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//装備小種別可否
	TS_LOG("\t装備小種別可否:");
	for(auto& item : this->pmsEquipSubType) {
		TS_LOG("\t\t%s -> %s (優先度 %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//スキル使用可否
	TS_LOG("\tスキル使用可否:");
	for(auto& item : this->pmsSkillUse) {
		TS_LOG("\t\t%s -> %s (優先度 %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//スキル種別使用可否
	TS_LOG("\tスキル種別使用可否:");
	for(auto& item : this->pmsSkillType) {
		TS_LOG("\t\t%s -> %s (優先度 %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}
}


//####################  パーティキャラDBをベースにしてアクターを生成する  ####################
CDatabase::CDBActorData CDatabase::CDBActorData::CreateActorFromChar(int ID) {
	CDBActorData newData;
	newData.PotentialSkills.SetAllowOverlap(true);
	CDBCell::CDBIDValues* idVals;
	CIntegerArray* ids;

	//パーティキャラをベースにする
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0);
	if(db->GetRowFromID(ID) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "無効なパーティキャラID [%d] でアクターを生成しようとしました。", ID);
		return newData;
	}

	//各種設定
	newData.baseFDBIndex = static_cast<int>(TS::DB::FDBIndices::Char);
	newData.ID = ID;
	newData.Name = *db->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	newData.Graphics = *db->GetCell(ID, TS::DB::ColumnNameGraphics)->GetStringPTR();
	newData.Title = *db->GetCell(ID, TS::DB::ColumnNameCharTitle)->GetStringPTR();
	newData.Level = *db->GetCell(ID, TS::DB::ColumnNameCharLevel)->GetIntegerPTR();
	newData.IsNPC = *db->GetCell(ID, TS::DB::ColumnNameCharIsNPC)->GetBooleanPTR();

	//属性
	ids = db->GetCell(ID, TS::DB::ColumnNameAttribute)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetAttribute(val);
	}

	//装備許可：大種別は原則としてすべて装備できる状態、小種別は原則としてすべて装備できない状態にする
	auto itemTypes = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType));
	TS_COUNTLOOP(i, itemTypes->GetRowCount()) {
		newData.pmsEquipType.push_back(make_tuple(-1, CIDValueData(
			*itemTypes->GetRowFromIndex(i)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID)).GetIntegerPTR(),
			itemTypes->GetRowFromIndex(i)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
			TRUE)));
	}
	ids = db->GetCell(ID, TS::DB::ColumnNameItemSubType)->GetIntegers();
	for(auto& val : ids->getData()) {
		string name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemSubType))->GetRowFromID(val)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		newData.pmsEquipSubType.push_back(make_tuple(-1, CIDValueData(val, name, TRUE)));
	}

	//パラメーター・成長曲線
	auto cvs = db->GetCell(ID, TS::DB::ColumnNameParameter)->ConvertToCurves();
	for(auto& cv : cvs) {
		newData.ParameterCurve.Add(cv);
		newData.Parameters.Add(CIDValueData(
			cv.ID,
			*Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowFromID(cv.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR(),
			newData.getCurveValue(newData.Level, cv)
			));
	}

	//経験値・経験値曲線
	cvs = db->GetCell(ID, TS::DB::ColumnNameCharEXPCurve)->ConvertToCurves();
	if(cvs.size() == 1) {
		newData.EXPCurve = cvs[0];
		if(newData.Level <= 1) {
			newData.EXP = newData.EXPCurve.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Base)];
		} else {
			newData.EXP = CDBActorData::getCurveValue(newData.Level, newData.EXPCurve);
		}
	}

	//クラス
	ids = db->GetCell(ID, TS::DB::ColumnNameCharClass)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetClass(val);
	}

	//装備品
	ids = db->GetCell(ID, TS::DB::ColumnNameCharEquip)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetEquip(val);
	}

	//習得スキル
	idVals = db->GetCell(ID, TS::DB::ColumnNameSkill)->GetIDValues();
	for(auto& val : idVals->getData()) {
		if(newData.Level >= val.Value) {
			//習得状態にする
			newData.SetSkill(val.ID);
		}
		//習得予定リストには必ず入れる
		newData.PotentialSkills.Add(CActorSubData(val));
	}

	//個別変数: 初期値をセットして、次に変更する値を適用する
	Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfActor), &newData.SelfValues);
	auto selfChange = db->GetCell(ID, TS::DB::ColumnNameSelfValue)->GetIDValues();
	for(auto& val : selfChange->getData()) {
		newData.SelfValues.Set(val.ID, val.Value);
	}

	//アドオン
	auto addons = db->GetCell(ID, TS::DB::ColumnNameAddon)->ConvertToAddons();
	newData.applyAddonOne(&addons, TS::DB::AddonFrom::Actor);

	TS_LOG("パーティキャラ情報をベースにアクター [%s] を生成しました。", newData.Name.c_str());
	return newData;
}


//####################  エネミーDBをベースにしてアクター情報を生成する  ####################
CDatabase::CDBActorData CDatabase::CDBActorData::CreateActorFromEnemy(int ID) {
	CDBActorData newData;
	newData.PotentialSkills.SetAllowOverlap(true);
	CDBCell::CDBIDValues* idVals;
	CIntegerArray* ids;

	//エネミーをベースにする
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Enemy)]->GetSubDB(0);
	if(db->GetRowFromID(ID) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "無効なエネミーID [%d] でアクターを生成しようとしました。", ID);
		return newData;
	}

	//各種設定
	newData.baseFDBIndex = static_cast<int>(TS::DB::FDBIndices::Enemy);
	newData.ID = ID;
	newData.Name = *db->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	newData.Graphics = *db->GetCell(ID, TS::DB::ColumnNameGraphics)->GetStringPTR();
	newData.IsNPC = true;

	//属性
	ids = db->GetCell(ID, TS::DB::ColumnNameAttribute)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetAttribute(val);
	}

	//パラメーター: 既定値ゼロで登録してから、変更する値を適用する
	auto parDB = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter));
	idVals = db->GetCell(ID, TS::DB::ColumnNameParameter)->GetIDValues();
	TS_COUNTLOOP(i, parDB->GetRowCount()) {
		newData.Parameters.Add(CIDValueData(
			*parDB->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR(),
			*Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR(),
			0
			));
	}
	for(auto& val : idVals->getData()) {
		newData.Parameters.GetFromID(val.ID)->Value = val.Value;
	}

	//個別変数: 既定値で登録してから、変更する値を適用する
	Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfActor), &newData.SelfValues);
	auto selfChange = db->GetCell(ID, TS::DB::ColumnNameSelfValue)->GetIDValues();
	for(auto& val : selfChange->getData()) {
		newData.SelfValues.Set(val.ID, val.Value);
	}

	//アドオン
	auto addons = db->GetCell(ID, TS::DB::ColumnNameAddon)->ConvertToAddons();
	newData.applyAddonOne(&addons, TS::DB::AddonFrom::Actor);

	TS_LOG("エネミー情報をベースにアクター [%s] を生成しました。", newData.Name.c_str());
	return newData;
}
