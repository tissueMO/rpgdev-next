//###################################################################
//			�p�[�e�B �f�[�^�x�[�X
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �ǂݍ��ݏ���  ####################
bool CDatabase::CDBParty::Load(string fileName) {
	//�t�@�C������͓ǂݍ��܂��A�����ݒ�DB�����荞��
	auto db = Game.DB.GetDB(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Init));
	this->fileName = db->GetDBFileName();
	auto party = db->GetSubDB(0)->GetCellFromIndex(0, static_cast<int>(TS::DB::DBInitIndices::Party))->GetIntegers();
	auto items = db->GetSubDB(0)->GetCellFromIndex(0, static_cast<int>(TS::DB::DBInitIndices::Item))->GetIDValues();
	this->Money = *db->GetSubDB(0)->GetCellFromIndex(0, static_cast<int>(TS::DB::DBInitIndices::Money))->GetIntegerPTR();

	//�p�[�e�B�����o�[��o�^
	this->members.Clear();
	TS_COUNTLOOP(i, party->Count()) {
		this->AddMember(party->Get(i), -1);
	}

	//�����A�C�e����o�^
	this->items.Clear();
	TS_COUNTLOOP(i, items->Count()) {
		this->AddItem(items->Get(i)->ID, items->Get(i)->Value);
	}
	return true;
}


//####################  �Z�[�u�f�[�^����̃��[�h  ####################
void CDatabase::CDBParty::Load(vector<string>& vectorbuf) {
	string temp;
	vector<string> split;

	//�p�[�e�B�����o�[���X�g��ǂݍ���
	temp = Game.PopHead(vectorbuf);
	Game.SplitString(split, temp, ";");
	this->members.Clear();
	for(auto& member : split) {
		this->AddMember(atoi(member.c_str()), -1);
	}

	//�����A�C�e�����X�g��ǂݍ���
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

	//��������ǂݍ���
	this->Money = atoi(Game.PopHead(vectorbuf).c_str());
}


//####################  �Z�[�u�f�[�^�ւ̃Z�[�u  ####################
void CDatabase::CDBParty::Save(string& strbuf) {
	//�p�[�e�B�����o�[���X�g����������
	TS_COUNTLOOP(i, this->members.GetCount()) {
		strbuf += Game.Format("%d", this->members.GetFromIndex(i)->ID);
		if(i < this->members.GetCount() - 1) {
			strbuf += ";";
		}
	}
	strbuf += "\n";

	//�����A�C�e�����X�g����������
	TS_COUNTLOOP(i, this->items.GetCount()) {
		//�����ۑ�����
		strbuf += Game.Format("%d:%d", this->items.GetFromIndex(i)->ID, this->items.GetFromIndex(i)->Value);
		if(i < this->items.GetCount() - 1) {
			strbuf += ";";
		}
	}
	strbuf += "\n";

	//����������������
	strbuf += Game.Format("%d\n", this->Money);
}


//####################  �f�[�^�x�[�X�̒��g�����O�Ƃ��ďo�͂���  ####################
void CDatabase::CDBParty::PrintData() {
	TS_LOG("\nDB: �p�[�e�B���");

	//�p�[�e�B�����o�[���X�g����������
	TS_LOG("\nParty:");
	for(auto& member : this->members.Items) {
		TS_LOG("%d (%s)", member.ID, member.Name.c_str());
	}

	//�����A�C�e�����X�g����������
	TS_LOG("\nHavingItem:");
	for(auto& item : this->items.Items) {
		TS_LOG("%d (%s) * %d", item.ID, item.Name.c_str(), item.Value);
	}

	//����������������
	TS_LOG("\nHavingMoney: " + to_string(this->Money));

	TS_LOG("");		//��s���o��
}


//####################  �j������  ####################
void CDatabase::CDBParty::Dispose() {
	this->members.Clear();
	this->items.Clear();
}


//####################  �����o�[������  ####################
//�����ɒǉ�����ꍇ�� -1 ���w�肷��
void CDatabase::CDBParty::AddMember(int id, int index) {
	if(this->CheckCharInParty(id) != -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "[ID: %d] �͊��Ƀp�[�e�B�ɉ������Ă��܂��B", id);
		return;
	} else if(this->members.GetCount() >= Game.DB.getFDBSystem()->GetInteger(TS::DB::System::MaxPartyCount)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�p�[�e�B�ő�l���ɒB���Ă��邽�߁A[ID: %d] ���p�[�e�B�ɒǉ��ł��܂���B", id);
		return;
	}
	auto cell = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(id);
	if(cell == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "�����ȃp�[�e�B�L����ID [%d] �Ńp�[�e�B�ɒǉ����悤�Ƃ��܂����B", id);
		return;
	}

	if(index > this->members.GetCount()) {
		index = -1;		//�͈͊O�̏ꍇ�͖����ɒǉ���
	}

	//�ǉ�����
	CIDNameData newData(id, *cell->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR());
	this->members.Insert(newData, index);
}


//####################  �w��ID�̃����o�[��E��  ####################
void CDatabase::CDBParty::DeleteMemberFromID(int id) {
	const int index = this->CheckCharInParty(id);
	if(index == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "�p�[�e�B�ɂ��Ȃ��L���� [ID: %d] ��E�ނ��悤�Ƃ��܂����B", id);
		return;
	} else if(this->members.GetCount() <= 1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�p�[�e�B���O�l�ɂ��邱�Ƃ͂ł��܂���B");
		return;
	}
	this->DeleteMemberFromIndex(index);
}


//####################  �w��ʒu�̃����o�[��E��  ####################
void CDatabase::CDBParty::DeleteMemberFromIndex(int index) {
	if(this->members.GetCount() <= 1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�p�[�e�B���O�l�ɂ��邱�Ƃ͂ł��܂���B");
		return;
	}
	this->members.DeleteFromIndex(index);
}


//####################  �w��L�����������o�[�ɂ��邩���ׁA����ꍇ�͂��̈ʒu��Ԃ�  ####################
int CDatabase::CDBParty::CheckCharInParty(int id) {
	TS_COUNTLOOP(i, this->members.GetCount()) {
		if(id == this->members.GetFromIndex(i)->ID) {
			return i;
		}
	}
	return -1;
}


//####################  �w��ʒu�̃L����ID�𒲂ׂ�  ####################
int CDatabase::CDBParty::GetCharIDFromPartyIndex(int index) {
	if(index < 0 || this->GetCharCount() <= index) {
		return -1;
	} else {
		return this->members.GetFromIndex(index)->ID;
	}
}


//####################  �����o�[����Ԃ�  ####################
int CDatabase::CDBParty::GetCharCount() {
	return this->members.GetCount();
}


//####################  �A�C�e��������  ####################
void CDatabase::CDBParty::AddItem(int id, int count) {
	auto cell = Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(id);
	if(cell == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "�����ȃA�C�e��ID [%d] �ŏ������悤�Ƃ��܂����B", id);
		return;
	} else if(count == 0) {
		return;
	} else if(count < 0) {
		//�����}�C�i�X�ɂ���Ɣj���������s��
		this->DeleteItem(id, -count);
		return;
	}

	//���ɏ������Ă��邩�ǂ����𒲂ׂ�
	const int index = this->getHavingItemIndex(id);
	if(index == -1) {
		//�V�K�ǉ�
		CIDValueData newData(id, *cell->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR(), count);
		this->items.Add(newData);
	} else {
		//���ǉ�
		this->items.GetFromIndex(index)->Value += count;
	}
}


//####################  �A�C�e����j��  ####################
void CDatabase::CDBParty::DeleteItem(int id, int count) {
	//�������Ă��Ȃ��ꍇ�͏������Ȃ�
	if(!this->items.Exists(id)) {
		return;
	} else if(count == 0) {
		return;
	} else if(count < 0) {
		//�����}�C�i�X�ɂ���ƒǉ��������s��
		this->AddItem(id, -count);
		return;
	}

	//���������`�F�b�N
	if(this->items.GetFromID(id)->Value <= count) {
		//�폜����
		this->items.DeleteFromID(id);
	} else {
		//�����Z
		this->items.GetFromID(id)->Value -= count;
	}
}


//####################  �w��ʒu�̃A�C�e�������擾����  ####################
CIDValueData* CDatabase::CDBParty::GetItemDataFromIndex(int index) {
	return this->items.GetFromIndex(index);
}


//####################  �����A�C�e���̑�����Ԃ�  ####################
int CDatabase::CDBParty::GetItemCountAll() {
	return this->items.GetCount();
}


//####################  �w�肵�����ʂ̏����A�C�e������Ԃ�  ####################
int CDatabase::CDBParty::GetItemTypeCount(int itemTypeID) {
	int count = 0;
	TS_COUNTLOOP(i, this->items.GetCount()) {
		//�A�C�e��DB�֐ڑ����Ď�ʂ��m�F����
		const int currentID = this->items.GetFromIndex(i)->ID;
		CIntegerArray types;
		Game.DB.GetItemTypes(currentID, &types, nullptr);
		TS_COUNTLOOP(m, types.Count()) {
			if(itemTypeID == types.Get(m)) {
				count++;		//�w�肵�����ʂ��܂�ł���ꍇ�̓J�E���g����
				break;
			}
		}
	}
	return count;
}


//####################  �w�肵��ID�̃A�C�e���̏����A�C�e������Ԃ�  ####################
int CDatabase::CDBParty::GetItemCount(int itemID) {
	if(!this->items.Exists(itemID)) {
		return 0;
	}
	return this->items.GetFromID(itemID)->Value;
}


//####################  �����A�C�e�����A��ID�����Ƃɏ����E�~���ɐ�������  ####################
void CDatabase::CDBParty::SortItemsByVisibleID(bool reverse) {
	//�A�C�e��DB�֐ڑ����ĉ�ID�Ŕ�r����
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


//####################  �����A�C�e�����A���O�����Ƃɏ����E�~���ɐ�������  ####################
void CDatabase::CDBParty::SortItemsByName(bool reverse) {
	this->items.Sort(static_cast<int>(TS::System::SortType::Name), reverse);
}


//####################  ���������Z�b�g����  ####################
void CDatabase::CDBParty::setMoney(int val) {
	this->Money = Game.WithIn(val, 0, INT_MAX);
}


//####################  �������Ă���A�C�e���̃��X�g��̃C���f�b�N�X���擾����  ####################
int CDatabase::CDBParty::getHavingItemIndex(int id) {
	TS_COUNTLOOP(i, this->items.GetCount()) {
		if(this->items.GetFromIndex(i)->ID == id) {
			return i;
		}
	}
	return -1;
}
