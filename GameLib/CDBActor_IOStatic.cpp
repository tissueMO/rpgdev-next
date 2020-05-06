//###################################################################
//			�A�N�^�[�i�p�[�e�B�L����/�G�l�~�[�𓝍��j���
//###################################################################
#include "Header.h"


//####################  �Z�[�u�f�[�^����A�N�^�[���𐶐�����  ####################
CDatabase::CDBActorData CDatabase::CDBActorData::Load(vector<string>& buf) {
	CDBActorData newData;
	newData.PotentialSkills.SetAllowOverlap(true);
	int id, val, count, cntsub;
	string temp;

	//��{���
	sscanf(Game.PopHead(buf).c_str(), "%d", &newData.ID);

	//�Œ�f�[�^�x�[�X�����{���������Ă���
	auto row = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(newData.ID);
	if(row == nullptr) {
		//�����ȃA�N�^�[�̏ꍇ�͏I�[�܂œǂݐ؂��Ē��f����
		while(Game.PopHead(buf) != TS::SaveLoad::ActorSplit) {}
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��p�[�e�B�L���� [ID: %d] �̕��������悤�Ƃ��܂����B", newData.ID);
		return newData;
	} else {
		newData = CDBActorData::CreateActorFromChar(newData.ID);

		//�Z�[�u�f�[�^�ŕ������铮�I���X�g�͂��ׂď���������
		newData.ClearClass();
		newData.ClearEquip(true);
		newData.ClearSkill();
		newData.ClearState();
	}

	sscanf(Game.PopHead(buf).c_str(), "%d,%d", &newData.Level, &newData.EXP);
	newData.Name = Game.PopHead(buf);
	newData.Title = Game.PopHead(buf);

	//���݂̃��x���ɑΉ������{�p�����[�^�[���Z�o
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter));
	TS_COUNTLOOP(i, db->GetRowCount()) {
		newData.Parameters.GetFromIndex(i)->Value = newData.getCurveValue(newData.Level, *newData.ParameterCurve.GetFromIndex(i));
	}

	//�N���X���
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d", &subData.ID, &cntsub);

		//�N���X�ʕϐ�
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfClass))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		newData.SetClass(subData.ID);
		*newData.Classes.GetSelfValues(newData.Classes.GetIndexFromID(subData.ID)) = subData.SelfValues;
	}

	//�����i���
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d", &subData.ID, &cntsub);

		//�A�C�e���ʕϐ�
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfUse))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		newData.SetEquip(subData.ID, true);
		*newData.Equips.GetSelfValues(newData.Equips.GetIndexFromID(subData.ID)) = subData.SelfValues;
	}

	//�Ԑڌ��ʏ��
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d", &subData.ID, &cntsub);

		//�N���X�ʕϐ�
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfState))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		newData.SetState(subData.ID, true);
		*newData.States.GetSelfValues(newData.States.GetIndexFromID(subData.ID)) = subData.SelfValues;
	}

	//�K���σX�L�����
	newData.LearnedSkills.Clear();
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		CActorSubData subData;
		sscanf(Game.PopHead(buf).c_str(), "%d,%d,%d", &subData.ID, &subData.Value, &cntsub);

		//�N���X�ʕϐ�
		TS_COUNTLOOP(n, cntsub) {
			sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
			subData.SelfValues.Get(id)->Value = val;
			subData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfUse))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		}
		subData.Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(subData.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		newData.LearnedSkills.Add(subData);
	}

	//�A�N�^�[�ʕϐ�
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
		newData.SelfValues.Get(id)->Value = val;
		newData.SelfValues.Get(id)->Name = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfActor))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	}

	//�p�����[�^�[
	count = atoi(Game.PopHead(buf).c_str());
	TS_COUNTLOOP(i, count) {
		sscanf(Game.PopHead(buf).c_str(), "%d=%d", &id, &val);
		newData.Parameters.GetFromID(id)->Value = val;
		newData.Parameters.GetFromID(id)->Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowFromID(id)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	}

	return newData;
}


//####################  �Z�[�u�p�̏��𐶐�����  ####################
void CDatabase::CDBActorData::Save(string& buf) {
	//��{���
	buf += Game.Format("%d\n", this->ID);
	buf += Game.Format("%d,%d\n", this->Level, this->EXP);
	buf += this->Name + "\n";
	buf += this->Title + "\n";

	//�N���X���
	buf += to_string(this->Classes.Count()) + "\n";
	for(auto& item : this->Classes.getData()) {
		buf += Game.Format("%d,%d\n", item.ID, item.SelfValues.data.GetCount());

		//�N���X�ʕϐ�
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//�����i���
	buf += to_string(this->Equips.Count()) + "\n";
	for(auto& item : this->Equips.getData()) {
		buf += Game.Format("%d,%d\n", item.ID, item.SelfValues.data.GetCount());

		//�A�C�e���ʕϐ�
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//�Ԑڌ��ʏ��
	buf += to_string(this->States.Count()) + "\n";
	for(auto& item : this->States.getData()) {
		buf += Game.Format("%d,%d\n", item.ID, item.SelfValues.data.GetCount());

		//�Ԑڌ��ʌʕϐ�
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//�K���σX�L�����
	buf += to_string(this->LearnedSkills.Count()) + "\n";
	for(auto& item : this->LearnedSkills.getData()) {
		buf += Game.Format("%d,%d,%d\n", item.ID, item.Value, item.SelfValues.data.GetCount());

		//�X�L���ʕϐ�
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%d=%d\n", val.second.ID, val.second.Value);
		}
	}

	//�A�N�^�[�ʕϐ�
	buf += to_string(this->SelfValues.data.GetCount()) + "\n";
	for(auto& item : this->SelfValues.data.Items) {
		buf += Game.Format("%d=%d\n", item.second.ID, item.second.Value);
	}

	//�p�����[�^�[
	buf += to_string(this->Parameters.GetCount()) + "\n";
	for(auto& item : this->Parameters.Items) {
		buf += Game.Format("%d=%d\n", item.ID, item.Value);
	}

	buf += TS::SaveLoad::ActorSplit + "\n";
}


//####################  ���O�o�͂��s��  ####################
void CDatabase::CDBActorData::PrintData() {
	string buf;
	TS_LOG("%d (%s)", this->ID, this->Name.c_str());
	TS_LOG("\t�O���t�B�b�N: %s", this->Graphics.c_str());
	TS_LOG("\tNPC: %s", TS_BOOLSTR(this->IsNPC));
	TS_LOG("\tLV: %d", this->Level);
	TS_LOG("\tEXP: %d (NextEXP: %d)", this->EXP, this->GetNextEXP());

	//�A�N�^�[�ʕϐ�
	TS_LOG("\t�A�N�^�[�ʕϐ�:");
	buf = "\t\t";
	for(auto& item : this->SelfValues.data.Items) {
		buf += Game.Format("%s=%d  ", item.second.Name.c_str(), item.second.Value);
	}
	TS_LOG(buf);

	//����
	TS_LOG("\t����:");
	buf = "\t\t";
	for(auto& item : this->Attributes.getData()) {
		buf += Game.Format("%s  ", item.Name.c_str());
	}
	TS_LOG(buf);

	//�p�����[�^�[
	TS_LOG("\t�p�����[�^�[:");
	buf = "\t\t";
	for(auto& item : this->Parameters.Items) {
		buf += Game.Format("%s=%d  ", item.Name.c_str(), item.Value);
	}
	TS_LOG(buf);

	//�N���X
	TS_LOG("\t�N���X:");
	for(auto& item : this->Classes.getData()) {
		buf = Game.Format("\t\t%s (", item.Name.c_str());

		//�ʕϐ����
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//�����i
	TS_LOG("\t�����i:");
	for(auto& item : this->Equips.getData()) {
		CIntegerArray itemTypeIDs;
		auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType));
		Game.DB.GetItemTypes(item.ID, &itemTypeIDs, nullptr);
		buf = Game.Format("\t\t%s%s = %s (", db->GetRowFromID(itemTypeIDs.Get(0))->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(), (itemTypeIDs.Count() > 1) ? "..." : "", item.Name.c_str());

		//�ʕϐ����
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//�K���X�L��
	TS_LOG("\t�K���σX�L��:");
	for(auto& item : this->LearnedSkills.getData()) {
		buf = Game.Format("\t\t%s (", item.Name.c_str());

		//�ʕϐ����
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//�Ԑڌ���
	TS_LOG("\t�늳���Ă���Ԑڌ���:");
	for(auto& item : this->States.getData()) {
		buf = Game.Format("\t\t%s (", item.Name.c_str());

		//�ʕϐ����
		for(auto& val : item.SelfValues.data.Items) {
			buf += Game.Format("%s=%d  ", val.second.Name.c_str(), val.second.Value);
		}
		buf += ")";
		TS_LOG(buf);
	}

	//�����ϐ�
	TS_LOG("\t�����ϐ�:");
	for(auto& item : this->rstAttribute) {
		TS_LOG("\t\t%s -> %3d %% (�D��x %d)", get<1>(item).Name.c_str(), get<1>(item).Value, get<0>(item));
	}

	//�Ԑڌ��ʑϐ�
	TS_LOG("\t�Ԑڌ��ʑϐ�:");
	for(auto& item : this->rstState) {
		TS_LOG("\t\t%s -> %3d %% (�D��x %d)", get<1>(item).Name.c_str(), get<1>(item).Value, get<0>(item));
	}

	//�Œ葕������
	TS_LOG("\t�Œ葕������:");
	for(auto& item : this->lckEquipType) {
		TS_LOG("\t\t%s -> %s (�D��x %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//�������ʉ�
	TS_LOG("\t�������ʉ�:");
	for(auto& item : this->pmsEquipType) {
		TS_LOG("\t\t%s -> %s (�D��x %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//��������ʉ�
	TS_LOG("\t��������ʉ�:");
	for(auto& item : this->pmsEquipSubType) {
		TS_LOG("\t\t%s -> %s (�D��x %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//�X�L���g�p��
	TS_LOG("\t�X�L���g�p��:");
	for(auto& item : this->pmsSkillUse) {
		TS_LOG("\t\t%s -> %s (�D��x %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}

	//�X�L����ʎg�p��
	TS_LOG("\t�X�L����ʎg�p��:");
	for(auto& item : this->pmsSkillType) {
		TS_LOG("\t\t%s -> %s (�D��x %d)", get<1>(item).Name.c_str(), Game.IntToBool(get<1>(item).Value) ? "OK" : "NG", get<0>(item));
	}
}


//####################  �p�[�e�B�L����DB���x�[�X�ɂ��ăA�N�^�[�𐶐�����  ####################
CDatabase::CDBActorData CDatabase::CDBActorData::CreateActorFromChar(int ID) {
	CDBActorData newData;
	newData.PotentialSkills.SetAllowOverlap(true);
	CDBCell::CDBIDValues* idVals;
	CIntegerArray* ids;

	//�p�[�e�B�L�������x�[�X�ɂ���
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0);
	if(db->GetRowFromID(ID) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�����ȃp�[�e�B�L����ID [%d] �ŃA�N�^�[�𐶐����悤�Ƃ��܂����B", ID);
		return newData;
	}

	//�e��ݒ�
	newData.baseFDBIndex = static_cast<int>(TS::DB::FDBIndices::Char);
	newData.ID = ID;
	newData.Name = *db->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	newData.Graphics = *db->GetCell(ID, TS::DB::ColumnNameGraphics)->GetStringPTR();
	newData.Title = *db->GetCell(ID, TS::DB::ColumnNameCharTitle)->GetStringPTR();
	newData.Level = *db->GetCell(ID, TS::DB::ColumnNameCharLevel)->GetIntegerPTR();
	newData.IsNPC = *db->GetCell(ID, TS::DB::ColumnNameCharIsNPC)->GetBooleanPTR();

	//����
	ids = db->GetCell(ID, TS::DB::ColumnNameAttribute)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetAttribute(val);
	}

	//�������F���ʂ͌����Ƃ��Ă��ׂđ����ł����ԁA����ʂ͌����Ƃ��Ă��ׂđ����ł��Ȃ���Ԃɂ���
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

	//�p�����[�^�[�E�����Ȑ�
	auto cvs = db->GetCell(ID, TS::DB::ColumnNameParameter)->ConvertToCurves();
	for(auto& cv : cvs) {
		newData.ParameterCurve.Add(cv);
		newData.Parameters.Add(CIDValueData(
			cv.ID,
			*Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowFromID(cv.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR(),
			newData.getCurveValue(newData.Level, cv)
			));
	}

	//�o���l�E�o���l�Ȑ�
	cvs = db->GetCell(ID, TS::DB::ColumnNameCharEXPCurve)->ConvertToCurves();
	if(cvs.size() == 1) {
		newData.EXPCurve = cvs[0];
		if(newData.Level <= 1) {
			newData.EXP = newData.EXPCurve.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Base)];
		} else {
			newData.EXP = CDBActorData::getCurveValue(newData.Level, newData.EXPCurve);
		}
	}

	//�N���X
	ids = db->GetCell(ID, TS::DB::ColumnNameCharClass)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetClass(val);
	}

	//�����i
	ids = db->GetCell(ID, TS::DB::ColumnNameCharEquip)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetEquip(val);
	}

	//�K���X�L��
	idVals = db->GetCell(ID, TS::DB::ColumnNameSkill)->GetIDValues();
	for(auto& val : idVals->getData()) {
		if(newData.Level >= val.Value) {
			//�K����Ԃɂ���
			newData.SetSkill(val.ID);
		}
		//�K���\�胊�X�g�ɂ͕K�������
		newData.PotentialSkills.Add(CActorSubData(val));
	}

	//�ʕϐ�: �����l���Z�b�g���āA���ɕύX����l��K�p����
	Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfActor), &newData.SelfValues);
	auto selfChange = db->GetCell(ID, TS::DB::ColumnNameSelfValue)->GetIDValues();
	for(auto& val : selfChange->getData()) {
		newData.SelfValues.Set(val.ID, val.Value);
	}

	//�A�h�I��
	auto addons = db->GetCell(ID, TS::DB::ColumnNameAddon)->ConvertToAddons();
	newData.applyAddonOne(&addons, TS::DB::AddonFrom::Actor);

	TS_LOG("�p�[�e�B�L���������x�[�X�ɃA�N�^�[ [%s] �𐶐����܂����B", newData.Name.c_str());
	return newData;
}


//####################  �G�l�~�[DB���x�[�X�ɂ��ăA�N�^�[���𐶐�����  ####################
CDatabase::CDBActorData CDatabase::CDBActorData::CreateActorFromEnemy(int ID) {
	CDBActorData newData;
	newData.PotentialSkills.SetAllowOverlap(true);
	CDBCell::CDBIDValues* idVals;
	CIntegerArray* ids;

	//�G�l�~�[���x�[�X�ɂ���
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Enemy)]->GetSubDB(0);
	if(db->GetRowFromID(ID) == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�����ȃG�l�~�[ID [%d] �ŃA�N�^�[�𐶐����悤�Ƃ��܂����B", ID);
		return newData;
	}

	//�e��ݒ�
	newData.baseFDBIndex = static_cast<int>(TS::DB::FDBIndices::Enemy);
	newData.ID = ID;
	newData.Name = *db->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
	newData.Graphics = *db->GetCell(ID, TS::DB::ColumnNameGraphics)->GetStringPTR();
	newData.IsNPC = true;

	//����
	ids = db->GetCell(ID, TS::DB::ColumnNameAttribute)->GetIntegers();
	for(auto& val : ids->getData()) {
		newData.SetAttribute(val);
	}

	//�p�����[�^�[: ����l�[���œo�^���Ă���A�ύX����l��K�p����
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

	//�ʕϐ�: ����l�œo�^���Ă���A�ύX����l��K�p����
	Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfActor), &newData.SelfValues);
	auto selfChange = db->GetCell(ID, TS::DB::ColumnNameSelfValue)->GetIDValues();
	for(auto& val : selfChange->getData()) {
		newData.SelfValues.Set(val.ID, val.Value);
	}

	//�A�h�I��
	auto addons = db->GetCell(ID, TS::DB::ColumnNameAddon)->ConvertToAddons();
	newData.applyAddonOne(&addons, TS::DB::AddonFrom::Actor);

	TS_LOG("�G�l�~�[�����x�[�X�ɃA�N�^�[ [%s] �𐶐����܂����B", newData.Name.c_str());
	return newData;
}
