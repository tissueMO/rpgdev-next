//###################################################################
//			�A�N�^�[�i�p�[�e�B�L����/�G�l�~�[�𓝍��j���
//###################################################################
#include "Header.h"


//####################  �O���t�B�b�N��ǂݍ���  ####################
void CDatabase::CDBActorData::LoadGraphics(string fileName) {
	this->GraphHandle = Game.Material.getGraphics()->Add(fileName);
	this->GraphSize = Game.DXLib.GetGraphSize(this->GraphHandle);
}


//####################  �O���t�B�b�N��j������  ####################
void CDatabase::CDBActorData::DisposeGraphics() {
	Game.Material.getGraphics()->Delete(this->GraphHandle);
}


//####################  �w�肵��������ǉ�����  ####################
void CDatabase::CDBActorData::SetAttribute(int attrID) {
	auto tgt = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Attribute))->GetRowFromID(attrID);
	if(tgt == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�����ID [%d] �Œǉ����悤�Ƃ��܂����B", attrID);
	} else {
		string attrName = *tgt->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
		if(this->Attributes.Exists(attrID)) {
			//���݂���ꍇ�͏d���J�E���^�[�����Z
			this->Attributes.Get(this->Attributes.GetIndexFromID(attrID))->Value++;
		} else {
			//���݂��Ȃ��ꍇ�͏d���J�E���^�[�����������Ēǉ�����
			this->Attributes.Add(CIDValueData(attrID, attrName, 1));
		}
	}
}


//####################  �w�肵���N���X��ǉ�����  ####################
//NOTE: �N���X�͗B��p�����[�^�[�ϓ����{���w��ōs���邽�߁A���ɓK�p����Ă���ϓ�����U�������Ă���K�p����K�v������B
void CDatabase::CDBActorData::SetClass(int ID) {
	if(this->Classes.GetIndexFromID(ID) != -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "�N���X [%d: %s] �͊��ɑΏۃA�N�^�[ [%d: %s] �ɐݒ肳��Ă��܂��B", ID, Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(), this->ID, this->Name.c_str());
		return;
	}

	this->releaseAddonAll();		//��U�A�h�I�������ׂĉ�������
	this->baseSetDataWithAddon(ID, this->Classes, TS::DB::FDBIndices::Class, TS::DB::DBValueIndices::SelfClass, TS::DB::AddonFrom::Count);		//AddonFrom::Count �ɂ��邱�ƂŃA�h�I���K�p�����������
	this->applyAddonAll();			//�A�h�I���E�p�����[�^�[�ϓ������ׂčēK�p����

	//�����̓K�p
	for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
		this->SetAttribute(attr);
	}

	//�X�L���̓K�p
	for(auto& skill : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameSkill.c_str())->GetIDValues()->getData()) {
		if(this->Level >= skill.Value) {
			//�K�����x���ɒB���Ă���ꍇ�͊o��������
			this->SetSkill(skill.ID);
		} else {
			//�K�����x���ɒB���Ă��Ȃ��ꍇ�͗\�胊�X�g�ɑ���
			this->PotentialSkills.Add(skill);
		}
	}
}


//####################  �w�肵��������ǉ�����  ####################
bool CDatabase::CDBActorData::SetEquip(int ID, bool absolute) {
	//�֘A�A�h�I��: �����Œ�E���ʎw��E����ʎw��
	if(absolute == false && !this->CheckEquip(ID)) {
		auto row = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ID);
		if(row == nullptr) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�����ȃA�C�e�� [%d] ��ΏۃA�N�^�[ [%d: %s] �ɑ������悤�Ƃ��܂����B", ID, this->ID, this->Name.c_str());
		} else {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�A�C�e�� [%d: %s] ��ΏۃA�N�^�[ [%d: %s] �ɑ����ł��܂���B", ID, row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(), this->ID, this->Name.c_str());
		}
		return false;
	}

	//���ɓ���̑��ʂ𑕔����Ă����Ԃ̏ꍇ�͐�ɉ�������
	CIntegerArray itemTypeIDs;
	Game.DB.GetItemTypes(ID, &itemTypeIDs, nullptr);
	for(auto& id : itemTypeIDs.getData()) {
		this->ReleaseEquipType(id, absolute);
	}

	//��������
	this->baseSetDataWithAddon(ID, this->Equips, TS::DB::FDBIndices::Item, TS::DB::DBValueIndices::SelfUse, TS::DB::AddonFrom::Item);

	//�����̓K�p
	for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
		this->SetAttribute(attr);
	}

	//�p�����[�^�[�ϓ��l�̓K�p
	for(auto& par : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameParameter.c_str())->GetIDValues()->getData()) {
		this->Parameters.GetFromID(par.ID)->Value += par.Value;
	}
	return true;
}


//####################  �w�肵���X�L����ǉ�����  ####################
void CDatabase::CDBActorData::SetSkill(int ID) {
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0);
	auto row = db->GetRowFromID(ID);
	if(row == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�ID [%d] �ŏK���X�L���ɒǉ����悤�Ƃ��܂����B", ID);
	} else {
		const int index = this->LearnedSkills.GetIndexFromID(ID);
		if(index != -1) {
			//���ɏK�����Ă���ꍇ�͏d���J�E���^�[�����Z
			this->LearnedSkills.Get(index)->Value++;
		} else {
			//�V�K�ǉ�
			CActorSubData newData;
			newData.ID = ID;
			newData.Name = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
			newData.Value = 1;

			//�ʕϐ��̏����l���Z�b�g
			Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfUse), &newData.SelfValues);

			this->LearnedSkills.Add(newData);
		}
	}
}


//####################  �w�肵���Ԑڌ��ʂ�ǉ�����  ####################
//�Ԓl�͐���ɒǉ��ł������ǂ���
bool CDatabase::CDBActorData::SetState(int ID, bool absolute) {
	if (absolute == true || TS_RAND(100) < 100 - this->GetStateResistance(ID)) {
		//�늳������
		if (this->States.GetIndexFromID(ID) != -1) {
			//���ɜ늳���Ă���ꍇ�́A�d�ˊ|�������邩��ʒu���̂ǂ��炩�ɂȂ�
			const int upperStateID = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameStateUpper.c_str())->GetIntegerPTR();
			if (upperStateID != -1) {
				//��ʒu�����s��
				TS_LOG("�d�ˊ|���̂��߁A�Ԑڌ��� [%d: %s] ����ʌ݊��� [%d: %s] �ɒu�������܂��B",
					ID,
					Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
					upperStateID,
					Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(upperStateID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
				);

				//���ʌ��ʂ�����
				this->baseDeleteDataWithAddon(ID, this->States, TS::DB::FDBIndices::State, TS::DB::AddonFrom::State);
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DBStateEnd, ID, this, true);		//���������Ƃ��̏������X�N���v�g�ϑ�����

				//��ʌ��ʂ�늳
				return this->SetState(upperStateID, true);
			}
		}

		//�Ԑڌ��ʑϐ����l��������Œǉ�����
		this->baseSetDataWithAddon(ID, this->States, TS::DB::FDBIndices::State, TS::DB::DBValueIndices::SelfState, TS::DB::AddonFrom::State);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DBStateStart, ID, this);		//�늳�����Ƃ��̏������X�N���v�g�ϑ�����
		return true;
	}

	return false;
}


//####################  �w�肵���������폜����  ####################
void CDatabase::CDBActorData::DeleteAttribute(int attrID) {
	const int index = this->Attributes.GetIndexFromID(attrID);
	if(index == -1) {
		return;
	}
	//�d���J�E���^�[���[���ɂȂ�����폜�ł���
	this->Attributes.Get(index)->Value--;
	if(this->Attributes.Get(index)->Value <= 0) {
		this->Attributes.DeleteFromIndex(index);
	}
}


//####################  �w�肵���N���X���폜����  ####################
bool CDatabase::CDBActorData::ReleaseClass(int ID) {
	if(this->Classes.GetIndexFromID(ID) == -1) {
		return false;
	}

	this->releaseAddonAll();		//��U�A�h�I�������ׂĉ�������
	this->baseDeleteDataWithAddon(ID, this->Classes, TS::DB::FDBIndices::Class, TS::DB::AddonFrom::Count);		//AddonFrom::Count �ɂ��邱�ƂŃA�h�I���K�p�����������
	this->applyAddonAll();			//�A�h�I���E�p�����[�^�[�ϓ������ׂčēK�p����

	//�����̉���
	for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
		this->DeleteAttribute(attr);
	}

	//�X�L���̉���
	for(auto& skill : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameSkill.c_str())->GetIDValues()->getData()) {
		if(this->Level >= skill.Value) {
			//�K���σ��X�g����폜
			this->ReleaseSkill(skill.ID);
		}
		//�K���\�胊�X�g����͕K���폜����
		this->PotentialSkills.DeleteFromID(skill.ID);
	}
	return true;
}


//####################  �w�肵���������폜����  ####################
//�Ԓl�͐���ɍ폜�ł������ǂ���
bool CDatabase::CDBActorData::ReleaseEquip(int ID, bool absolute) {
	if(this->Equips.GetIndexFromID(ID) == -1) {
		return false;
	}

	//�֘A�A�h�I��: �����Œ�
	bool OK = true;
	CIntegerArray itemTypeIDs;
	Game.DB.GetItemTypes(ID, &itemTypeIDs, nullptr);
	for(auto& id : itemTypeIDs.getData()) {
		if(absolute == false && this->CheckEquipLock(id)) {
			OK &= false;
			continue;
		}

		this->baseDeleteDataWithAddon(ID, this->Equips, TS::DB::FDBIndices::Item, TS::DB::AddonFrom::Item);

		//�����̉���
		for(auto& attr : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameAttribute.c_str())->GetIntegers()->getData()) {
			this->DeleteAttribute(attr);
		}

		//�p�����[�^�[�ϓ��l�̉���
		for(auto& par : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(ID, TS::DB::ColumnNameParameter.c_str())->GetIDValues()->getData()) {
			this->Parameters.GetFromID(par.ID)->Value -= par.Value;
		}
		break;
	}
	return OK;
}


//####################  �w�肵���X�L�����폜����  ####################
bool CDatabase::CDBActorData::ReleaseSkill(int ID) {
	const int index = this->LearnedSkills.GetIndexFromID(ID);
	if(index == -1) {
		return false;
	}
	//�d���J�E���^�[���[���ɂȂ�����폜�ł���
	this->LearnedSkills.Get(index)->Value--;
	if(this->LearnedSkills.Get(index)->Value <= 0) {
		this->LearnedSkills.DeleteFromIndex(index);
	}
	return true;
}


//####################  �w�肵���Ԑڌ��ʂ��폜����  ####################
bool CDatabase::CDBActorData::ReleaseState(int ID) {
	if(this->States.GetIndexFromID(ID) == -1) {
		return false;
	}
	this->baseDeleteDataWithAddon(ID, this->States, TS::DB::FDBIndices::State, TS::DB::AddonFrom::State);
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DBStateEnd, ID, this, false);		//���������Ƃ��̏������X�N���v�g�ϑ�����
	return true;
}


//####################  �N���X�����ׂč폜����  ####################
void CDatabase::CDBActorData::ClearClass() {
	// �����ł��Ȃ��N���X�͑��݂��Ȃ����ߖⓚ���p�ň���������Ă���
	while(this->Classes.Count() > 0) {
		this->ReleaseClass(this->Classes.GetID(0));
	}
}


//####################  �X�L�������ׂč폜����  ####################
//���ӁF�K���\��̃X�L���͈ێ������
void CDatabase::CDBActorData::ClearSkill() {
	// �Y�p�ł��Ȃ��N���X�͑��݂��Ȃ����ߖⓚ���p�ň���Y�p���Ă���
	while(this->LearnedSkills.Count() > 0) {
		this->ReleaseSkill(this->LearnedSkills.GetID(0));
	}
}


//####################  �����i�����ׂč폜����  ####################
void CDatabase::CDBActorData::ClearEquip(bool absolute) {
	int index = 0;
	while(index < this->Equips.Count()) {
		if (!this->ReleaseEquip(this->Equips.GetID(index), absolute)) {
			// �����ł��Ȃ����̂̓X�L�b�v����
			index++;
		}
	}
}


//####################  �Ԑڌ��ʂ����ׂč폜����  ####################
void CDatabase::CDBActorData::ClearState() {
	// �����ł��Ȃ��Ԑڌ��ʂ͑��݂��Ȃ����ߖⓚ���p�ň���������Ă���
	while(this->States.Count() > 0) {
		this->ReleaseState(this->States.GetID(0));
	}
}


//####################  �Ԑڌ��ʂ̃o�b�h�X�e�[�^�X�݂̂��폜  ####################
void CDatabase::CDBActorData::ClearStateOnlyBad() {
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0);
	while(true) {
		//�o�b�h�X�e�[�^�X������
		int id = -1;
		for(auto& st : this->States.getData()) {
			if(Game.IntToBool(*db->GetCell(st.ID, TS::DB::ColumnNameStateBad.c_str())->GetIntegerPTR())) {
				id = st.ID;
				break;
			}
		}
		if(id == -1) {
			return;		//����ȏ�o�b�h�X�e�[�^�X��������Ȃ���ΏI��
		}

		//�Y�������Ԑڌ��ʂ��폜
		this->ReleaseState(id);
	}
}


//####################  �w�肵�������̑ϐ����擾����  ####################
int CDatabase::CDBActorData::GetAttributeResistance(int ID) {
	int temp = 100;		//��{�l
	for(auto& data : this->rstAttribute) {
		if(get<1>(data).ID == ID) {
			temp += (get<1>(data).Value - 100);
		}
	}
	return temp;
}


//####################  �w�肵���Ԑڌ��ʂ̑ϐ����擾����  ####################
int CDatabase::CDBActorData::GetStateResistance(int ID) {
	int temp = 0;		//��{�l
	for(auto& data : this->rstState) {
		if(get<1>(data).ID == ID) {
			temp += get<1>(data).Value;
		}
	}
	return temp;
}


//####################  �������ʂ��瑕���iID���擾����  ####################
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


//####################  �w�肵���������ʂ��Œ肳��Ă��邩���ׂ�  ####################
bool CDatabase::CDBActorData::CheckEquipLock(int itemTypeID) {
	int pri = INT_MIN;		//�D��x�͍Œ�
	int temp = FALSE;		//�f�t�H���g�Ŏg������
	for(auto& data : this->lckEquipType) {
		if(get<0>(data) > pri && get<1>(data).ID == itemTypeID) {
			//�b��l�����D��x�������ꍇ�͂�����\�l�ɂ���
			pri = get<0>(data);
			temp = get<1>(data).Value;
		}
	}
	return Game.IntToBool(temp);
}


//####################  �w�肵�������i�𑕔��ł��邩���ׂ�  ####################
bool CDatabase::CDBActorData::CheckEquip(int itemID) {
	CIntegerArray itemTypeIDs, itemSubTypeIDs;

	//��O�`�F�b�N
	if(!Game.DB.GetItemTypes(itemID, &itemTypeIDs, &itemSubTypeIDs)) {
		return false;		//�����ȃA�C�e��ID
	}
	for(auto& id : itemTypeIDs.getData()) {
		if(this->CheckEquipLock(id)) {
			return false;		//�Œ肳�ꂽ����
		}
	}

	//�������ʂ̑����ۂ��`�F�b�N
	int pri = INT_MIN;			//�D��x�͍Œ�
	for(auto& typeID : itemTypeIDs.getData()) {
		int temp = TRUE;			//�f�t�H���g�ő����\
		for(auto& data : this->pmsEquipType) {
			if(get<0>(data) > pri && get<1>(data).ID == typeID) {
				//�b��l�����D��x�������ꍇ�͂�����\�l�ɂ���
				pri = get<0>(data);
				temp = get<1>(data).Value;
			}
		}
		if(!Game.IntToBool(temp)) {
			return false;		//��\�l���g���Ȃ���Ԃ������Ă����炱���ŏI���
		}
	}

	//��������ʂ̑����ۂ��`�F�b�N: �ΏۃA�C�e���ɏ���ʂ��ݒ肳��Ă��Ȃ��ꍇ�͒N�ł������ł���A�C�e���Ƃ݂Ȃ�
	pri = INT_MIN;			//�D��x�͍Œ�
	if(itemSubTypeIDs.Count() > 0) {
		for(auto& subTypeID : itemSubTypeIDs.getData()) {
			int temp = FALSE;		//�f�t�H���g�ő����s��
			for(auto& data : this->pmsEquipSubType) {
				if(get<0>(data) > pri && get<1>(data).ID == subTypeID) {
					//�b��l�����D��x�������ꍇ�͂�����\�l�ɂ���
					pri = get<0>(data);
					temp = get<1>(data).Value;
				}
			}
			if(!Game.IntToBool(temp)) {
				return false;		//��\�l���g���Ȃ���Ԃ������Ă����炱���ŏI���
			}
		}
	}

	return true;
}


//####################  �w�肵���������ʂ̑����i����������  ####################
//�Ԓl�͐���ɉ����ł������ǂ���
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
	return true;		//����������̂��Ȃ��Ă�����Ƃ݂Ȃ�
}


//####################  �w�肵������ɑ�������Ԑڌ��ʂ�늳���Ă��邩�ǂ����𒲂ׂ�  ####################
bool CDatabase::CDBActorData::CheckStateLimit(int stateLimit) {
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0);

	for(auto state : this->States.getData()) {
		if(*db->GetCell(state.ID, TS::DB::ColumnNameStateLimit.c_str())->GetIntegerPTR() == stateLimit) {
			return true;
		}
	}
	return false;
}


//####################  �w�肵���X�L�����g�����Ԃ��ǂ����𒲂ׂ�  ####################
bool CDatabase::CDBActorData::CheckUseSkill(int skillID) {
	//�L����ID���ǂ����̃`�F�b�N
	auto db = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0);
	const auto row = db->GetRowFromID(skillID);
	if(row == nullptr) {
		return false;
	}

	//�K���`�F�b�N
	if(this->LearnedSkills.GetIndexFromID(skillID) == -1) {
		return false;
	}

	//��������ʂ̎g�p���`�F�b�N
	auto itemSubTypeIDs = db->GetCell(skillID, TS::DB::ColumnNameItemSubType.c_str())->GetIntegers();
	if(itemSubTypeIDs->Count() > 0) {
		//�g�p���ݒ肪����ꍇ�͂����ꂩ��𑕔����Ă����OK
		auto itemDB = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0);
		bool OK = false;

		//�����i�����ׂă`�F�b�N
		for(auto& equip : this->Equips.getData()) {
			//�ΏۃX�L���̏���ʂƕ�����v���邩���ׂ�
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

	//�X�L����ʂ̎g�p�����`�F�b�N
	int skillTypeID = *db->GetCell(skillID, TS::DB::ColumnNameSkillType.c_str())->GetIntegerPTR();
	if(!this->CheckUseSkillType(skillTypeID)) {
		return false;
	}

	//�X�L���ʂ̎g�p�����`�F�b�N
	int pri = INT_MIN;		//�D��x�͍Œ�
	int temp = TRUE;		//�f�t�H���g�Ŏg������
	for(auto& data : this->pmsSkillUse) {
		if(get<1>(data).ID == skillID && get<0>(data) > pri) {
			//�b��l�����D��x�������ꍇ�͂�����\�l�ɂ���
			pri = get<0>(data);
			temp = get<1>(data).Value;
		}
	}
	return Game.IntToBool(temp);
}


//####################  �w�肵����ʂ̃X�L�����g�p�ł��邩�ǂ��������؂���  ####################
bool CDatabase::CDBActorData::CheckUseSkillType(int skillTypeID) {
	int pri = INT_MIN;		//�D��x�͍Œ�
	int temp = TRUE;		//�f�t�H���g�Ŏg������
	for(auto& data : this->pmsSkillType) {
		if(get<1>(data).ID == skillTypeID && get<0>(data) > pri) {
			//�b��l�����D��x�������ꍇ�͂�����\�l�ɂ���
			pri = get<0>(data);
			temp = get<1>(data).Value;
		}
	}
	return Game.IntToBool(temp);
}


//####################  �o���l�𑊑ΓI�ɑ��삷��  ####################
//�o���l�ɍ��킹�ă��x�����ϓ����邪�A���炵�Ă����x���������邱�Ƃ͂Ȃ�
//�Ԓl�͕ϓ��������x����
int CDatabase::CDBActorData::PlusEXP(int exp) {
	if(exp > 0 && this->EXP == INT_MAX) {
		//���Ɍo���l���J���X�g���Ă���ꍇ�͉������Ȃ�
	} else if(exp > 0 && exp > INT_MAX - this->EXP) {
		//�o���l���J���X�g������
		this->EXP = INT_MAX;
	} else {
		//�ʏ�ʂ���Z
		this->EXP += exp;
	}

	//���x���A�b�v����E���x���ϓ�����E�p�����[�^�[�X�V
	const int beforeLv = this->Level;
	int lv = this->Level;		//���݂̌o���l�ɑΉ����郌�x���b��l
	int beforeEXP = CDBActorData::getCurveValue(lv, this->EXPCurve);
	while(beforeEXP < CDBActorData::getCurveValue(lv + 1, this->EXPCurve)
			&& CDBActorData::getCurveValue(lv + 1, this->EXPCurve) <= this->EXP) {
		beforeEXP = CDBActorData::getCurveValue(lv + 1, this->EXPCurve);
		lv++;					//���̃��x���ɕK�v�Ȍo���l�������Ă���ꍇ�͎��ɐi�߂�
	}
	this->LevelUp(lv - this->Level);

	return this->Level - beforeLv;
}


//####################  ���x���𑊑ΓI�ɑ��삷��  ####################
//���x���������邱�Ƃ��ł��邪�A���̏ꍇ�͌o���l������Ȃ�
//�Ԓl�͎��̃��x���A�b�v�ɕK�v�Ȍo���l
int CDatabase::CDBActorData::LevelUp(int val) {
	this->LevelUpParameters.Clear();
	this->LevelUpSkills.Clear();

	if(val == 0) {
		return this->GetNextEXP();		//�ϓ����Ȃ��ꍇ�͏������Ȃ�
	}

	const int maxLevel = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxLevel.c_str());
	const int beforeLevel = this->Level;
	if(beforeLevel >= maxLevel) {
		//���ɍő僌�x���ɒB���Ă���ꍇ�͏����ł��Ȃ�
		return INT_MAX;
	}
	this->Level += val;
	if(this->Level > maxLevel) {
		//���x���A�b�v��ɍő僌�x���𒴂��Ă��܂��ꍇ�͒�������
		this->Level = maxLevel;
	}

	//�����̃��x���ɕK�v�Ȍo���l�𖞂����Ă��Ȃ��ꍇ�͑���
	const int afterLvFirstEXP = CDBActorData::getCurveValue(this->Level, this->EXPCurve);
	if(this->EXP < afterLvFirstEXP) {
		this->EXP = afterLvFirstEXP;
	}

	//�ϓ��O�̃p�����[�^�[���ꎞ�I�ɋL������
	TS_COUNTLOOP(i, Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowCount()) {
		this->LevelUpParameters.Add(*this->Parameters.GetFromIndex(i));
	}

	//�N���X�̓p�����[�^�[��{���œ������Ă���̂ŁA�ϓ��O�ɂ��ׂĉ�������K�v������
	vector<int> classIDs;
	while(this->Classes.Count() > 0) {
		const int id = this->Classes.GetID(0);
		classIDs.push_back(id);
		this->ReleaseClass(id);
	}

	//�p�����[�^�[���X�V����: �Ȑ����琶��������{�l�ł̍����Ōv�Z����
	TS_COUNTLOOP(i, Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowCount()) {
		const int beforePar = CDBActorData::getCurveValue(beforeLevel, *this->ParameterCurve.GetFromIndex(i));
		const int afterPar = CDBActorData::getCurveValue(this->Level, *this->ParameterCurve.GetFromIndex(i));
		const int delta = afterPar - beforePar;
		this->Parameters.GetFromIndex(i)->Value += delta;
	}

	//���������N���X�����ׂĕ�������
	for(auto id : classIDs) {
		this->SetClass(id);
	}

	//�����ŕϓ���̃p�����[�^�[�Ƃ̍������o��
	for(auto& par : this->LevelUpParameters.getData()) {
		par.Value = this->Parameters.GetFromID(par.ID)->Value - par.Value;
	}

	//�V�����X�L�����K��������
	for(auto& pskl : this->PotentialSkills.getData()) {
		if(beforeLevel < pskl.Value && pskl.Value <= this->Level) {
			//���̃��x���A�b�v�ɂ���ďK�����x���ɒB�����ꍇ�͏K��������
			this->SetSkill(pskl.ID);

			//�K�������X�L�������x���A�b�v�K���X�L�����X�g�ɓo�^����: �X�L�������i�[����Ă��Ȃ��f�[�^���g���̂ŁA�����Ŏ擾����
			pskl.Name = *Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(pskl.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();
			this->LevelUpSkills.Add(pskl);
		}
	}
	return this->GetNextEXP();
}


//####################  ���̃��x���A�b�v�ɕK�v�Ȍo���l���擾����  ####################
int CDatabase::CDBActorData::GetNextEXP() {
	if(this->Level >= Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxLevel.c_str())) {
		//�ő僌�x���̎��͏�ɍő�l�ŕԂ�
		return INT_MAX;
	}
	const int afterLvFirstEXP = CDBActorData::getCurveValue(this->Level + 1, this->EXPCurve);
	return afterLvFirstEXP - this->EXP;
}


//####################  �A�h�I���K�p�����ׂĈꊇ��������  ####################
//�D��x���������Ɏ��O��
void CDatabase::CDBActorData::releaseAddonAll() {
	//�Ԑڌ���
	for(auto& state : this->States.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetCell(state.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->releaseAddonOne(&addons, TS::DB::AddonFrom::State);
	}

	//����
	for(auto& equip : this->Equips.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(equip.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->releaseAddonOne(&addons, TS::DB::AddonFrom::Item);
	}

	//�N���X
	for(auto& cls : this->Classes.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->releaseAddonOne(&addons, TS::DB::AddonFrom::Class);

		//�p�����[�^�[�ϓ�������������
		auto parRates = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameParameterRate.c_str())->GetIDValues();
		for(auto& par : parRates->getData()) {
			this->Parameters.GetFromID(par.ID)->Value = this->Parameters.GetFromID(par.ID)->Value * 100.0 / par.Value + 0.5;		//�l�̌ܓ������Ȃ��Ɛ��m�ɕ����ł��Ȃ��Ȃ�
		}
	}

	//NOTE: �A�N�^�[���͑��삵�Ȃ��B
}


//####################  �A�h�I�������ׂĈꊇ�K�p������  ####################
//�D��x���Ⴂ���ɓK�p����
void CDatabase::CDBActorData::applyAddonAll() {
	//NOTE: �A�N�^�[���͑��삵�Ȃ��B

	//�N���X
	for(auto& cls : this->Classes.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->applyAddonOne(&addons, TS::DB::AddonFrom::Class);

		//�p�����[�^�[�ϓ�����K�p������
		auto parRates = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetCell(cls.ID, TS::DB::ColumnNameParameterRate.c_str())->GetIDValues();
		for(auto& par : parRates->getData()) {
			this->Parameters.GetFromID(par.ID)->Value = this->Parameters.GetFromID(par.ID)->Value * par.Value / 100.0 + 0.5;		//�l�̌ܓ������Ȃ��Ɛ��m�ɕ����ł��Ȃ��Ȃ�
		}
	}

	//����
	for(auto& equip : this->Equips.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetCell(equip.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->applyAddonOne(&addons, TS::DB::AddonFrom::Item);
	}

	//�Ԑڌ���
	for(auto& state : this->States.getData()) {
		auto addons = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetCell(state.ID, TS::DB::ColumnNameAddon.c_str())->ConvertToAddons();
		this->applyAddonOne(&addons, TS::DB::AddonFrom::State);
	}
}


//####################  �A�h�I���t���̃f�[�^���X�g�ɒǉ�����  ####################
void CDatabase::CDBActorData::baseSetDataWithAddon(int ID, CActorSubDataArray& tgt, TS::DB::FDBIndices FDBType, TS::DB::DBValueIndices selfValueType, TS::DB::AddonFrom priority) {
	auto db = Game.DB.FDB[static_cast<int>(FDBType)]->GetSubDB(0);
	auto row = db->GetRowFromID(ID);
	if(row == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ�ID [%d] �ŃA�h�I���t���̃f�[�^���X�g�ɒǉ����悤�Ƃ��܂����B", ID);
	} else {
		CActorSubData newData;
		newData.ID = ID;
		newData.Name = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR();

		//�ʕϐ��̏����l���Z�b�g
		Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(selfValueType), &newData.SelfValues);

		//�A�h�I����K�p
		if(priority != TS::DB::AddonFrom::Count) {
			auto addons = row->at(db->GetColumnIndex(TS::DB::ColumnNameAddon.c_str())).ConvertToAddons();
			this->applyAddonOne(&addons, priority);
		}
		tgt.Add(newData);
	}
}


//####################  �A�h�I���t���̃f�[�^���X�g����폜����  ####################
void CDatabase::CDBActorData::baseDeleteDataWithAddon(int ID, CActorSubDataArray& tgt, TS::DB::FDBIndices FDBType, TS::DB::AddonFrom priority) {
	auto db = Game.DB.FDB[static_cast<int>(FDBType)]->GetSubDB(0);
	if(priority != TS::DB::AddonFrom::Count) {
		auto addons = db->GetRowFromID(ID)->at(db->GetColumnIndex(TS::DB::ColumnNameAddon.c_str())).ConvertToAddons();
		this->releaseAddonOne(&addons, priority);
	}
	tgt.DeleteFromID(ID);
}


//####################  ���̃A�N�^�[�ɃA�h�I����K�p����  ####################
//�����㏑������^�C�v�̃A�h�I���́A�������D��x�̍����A�h�I��������Α��삹���A�ǉ�����Ƃ��͉������̂��߂ɗD��x���L������
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


//####################  ���̃A�N�^�[�ɕt�����ꂽ�A�h�I������������  ####################
//�����㏑������^�C�v�̃A�h�I���́A�������D��x�̍����A�h�I���ɂ���ď㏑������Ă�����������Ȃ��悤�ɂ���
//������悹����^�C�v�̃A�h�I���́A�����̕�����������
void CDatabase::CDBActorData::releaseAddonOne(CDatabase::CDBCell::CDBAddons* addons, TS::DB::AddonFrom priority) {
	for(auto& addon : addons->getData()) {
		string name;
		auto set = make_tuple(static_cast<int>(priority), CIDValueData(addon.ID, "Unknown", addon.Value));
		switch((TS::DBLiteral::Addon)addon.Type) {
			case TS::DBLiteral::Addon::SelfValuePlus:
				this->SelfValues.Get(addon.ID)->Value -= addon.Value;
				break;
			case TS::DBLiteral::Addon::ParameterPlus:
				//NOTE: �N���X�̓p�����[�^�[�ϓ����̐ݒ肪���邽�� [�A�h�I��:�p�����[�^�[����] �������ɂȂ�
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


//####################  �w�肵���A�h�I����������A�w�肵���D��x���������AID���d��������̂����邩�ǂ����𒲂ׂ�  ####################
bool CDatabase::CDBActorData::checkHigherAddon(vector<tuple<int, CIDValueData>>& list, tuple<int, CIDValueData>& base) {
	for(auto& addonbuf : list) {
		if(get<0>(addonbuf) > get<0>(base) && get<1>(addonbuf).ID == get<1>(base).ID) {
			return true;		//base�̌��ʂ𔽉f/�������ׂ��łȂ����
		}
	}
	return false;
}


//####################  �w�肵���X�L�������[�U�[�ɂ���Čォ��ǉ����ꂽ���̂ł��邩�ǂ����𒲂ׂ�  ####################
//�K���d���񐔂���K���\��X�L���̌������������Ă��[���ɂȂ�Ȃ��ꍇ�̓��[�U�[�ɂ���Ēǉ����ꂽ�Ƃ݂Ȃ���
bool CDatabase::CDBActorData::checkUserAddedSkill(int ID) {
	const int index = this->LearnedSkills.GetIndexFromID(ID);
	if(index == -1) {
		return false;		//�����ȃX�L��
	}
	int counter = this->LearnedSkills.Get(index)->Value;
	for(auto& pskl : this->PotentialSkills.getData()) {
		if(pskl.ID == ID) {
			counter--;
		}
	}
	return (counter > 0);
}


//####################  �w�肵��X�l�����ƂɋȐ��l�𐶐�����  ####################
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
			//int�^�̃I�[�o�[�t���[��h�����߂ɃJ���X�g������
			return INT_MAX;
		}
	}
}


//####################  �w�肵���A�h�I����������A�w�肵���A�h�I�����P��菜��  ####################
void CDatabase::CDBActorData::deleteAddonBuffer(vector<tuple<int, CIDValueData>>& list, tuple<int, CIDValueData>& base) {
	TS_COUNTLOOP(i, list.size()) {
		if(get<0>(list[i]) == get<0>(base) && get<1>(list[i]).ID == get<1>(base).ID) {
			list.erase(list.begin() + i);
			return;
		}
	}
}
