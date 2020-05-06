//###################################################################
//			�}�b�v�f�[�^�̒��g�Ɋւ���N���X��`
//###################################################################
#include "Header.h"


//####################  �C�x���g�̋N���\����s��  ####################
void CSceneMap::CMapData::ReserveEV(int EVID) {
	this->reservedEVs.push_back(EVID);
}


//####################  �w�肵���C�x���g�̃A�N�e�B�u�ȃy�[�W�̎w���ʂ̃X�N���v�g���Ăяo��  ####################
bool CSceneMap::CMapData::RunEV(int EVID, int type, bool pushRunningStack) {
	return this->RunEV(EVID, this->EVs.GetFromID(EVID)->GetPageIndex(), type, pushRunningStack);
}


//####################  �w�肵���C�x���g�̎w�肵���y�[�W�̎w���ʂ̃X�N���v�g���Ăяo��  ####################
bool CSceneMap::CMapData::RunEV(int EVID, int pageIndex, int type, bool pushRunningStack) {
	bool result = false;

	//���ɓ���C�x���g�̎��s���e�����s���ł���ꍇ�͏������Ȃ�
	if(type == static_cast<int>(TS::Map::EventScriptType::Run)) {
		for(auto& ev : this->runningEVNest_system) {
			if(EVID == ev->ID) {
				return false;
			}
		}
	}

	if(pageIndex != -1) {
		//���s���̃C�x���gID���Ăяo�������ɓo�^����
		this->runningEVNest_system.insert(this->runningEVNest_system.begin(), this->EVs.GetFromID(EVID));
		if(pushRunningStack) {
			this->runningEVNest.insert(this->runningEVNest.begin(), EVID);
		}

		result = Game.SQ.DoFunc<bool>(
			Game.Format(TS::SQ::SQEvent_DoMapEvent, EVID),
			type,
			pageIndex
			);

		//�V�[�����ύX����Č��݂̃C���X�^���X�������ɂȂ����ꍇ�͒��f����
		if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
			return false;
		}

		//���s����������Ăяo����������폜����
		this->runningEVNest_system.erase(this->runningEVNest_system.begin());
		if(pushRunningStack) {
			this->runningEVNest.erase(this->runningEVNest.begin());
		}
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽID [%d] �̃C�x���g�͌��݃}�b�v��ŏo����Ԃɂ���܂���B", EVID);
	}
	return result;
}


//####################  �C�x���g��ǉ�����  ####################
void CSceneMap::CMapData::SetEV(int EVID, int visibleID, int x, int y, string name) {
	CMapEventData ev;
	ev.ID = EVID;
	ev.VisibleID = visibleID;
	ev.Position.Set(x, y);
	ev.Name = name;
	ev.IsDummy = false;
	ev.SetPageIndex(-1);
	this->EVs.Add(ev);
	this->refreshAllEVTargetChar();
}


//####################  �w�肵���C�x���g�̃y�[�W��ǉ�����  ####################
void CSceneMap::CMapData::AddEVPage(int EVID, string param, string graph) {
	CMapEventData::CMapEventPageData pg;
	vector<string> buf;
	Game.SplitString(buf, param, ":");

	//������Ŏ����ꂽ���l�p�����[�^�[�𕪉����đ������
	pg.TriggerType = atoi(Game.PopHead(buf).c_str());
	pg.MoveType = atoi(Game.PopHead(buf).c_str());
	pg.Direction = atoi(Game.PopHead(buf).c_str());
	pg.MoveSpeed = atoi(Game.PopHead(buf).c_str());
	pg.MoveFrequency = atoi(Game.PopHead(buf).c_str());
	pg.AnimID = atoi(Game.PopHead(buf).c_str());
	pg.DrawPriority = atoi(Game.PopHead(buf).c_str());
	pg.HitRect.Width = atoi(Game.PopHead(buf).c_str());
	pg.HitRect.Height = atoi(Game.PopHead(buf).c_str());
	pg.EnabledStopAnim = atoi(Game.PopHead(buf).c_str());
	pg.EnabledWalkAnim = atoi(Game.PopHead(buf).c_str());
	pg.NoHit = atoi(Game.PopHead(buf).c_str());
	pg.FixedDirection = atoi(Game.PopHead(buf).c_str());
	pg.LoadGraphics(graph);

	this->EVs.GetFromID(EVID)->AddPage(pg);
}


//####################  �_�~�[�C�x���g�𐶐�����  ####################
//�Ԓl�̓C�x���g��FixedID
int CSceneMap::CMapData::SetDummyEV(int x, int y, string graph, int direction4, int animID, int priority, bool enabledStopAnim, bool enabledWalkAnim, bool fixedDirection) {
	//�C�x���g��FixedID�������_����������
	int id;
	do {
		id = Game.NextRand(1, INT_MAX);
	} while(this->EVs.Exists(id));
	this->SetEV(id, -1, x, y, "�_�~�[�C�x���g");
	auto ev = this->EVGetData(id);

	//����̃y�[�W��ǉ�
	CMapEventData::CMapEventPageData pg;
	pg.TriggerType = static_cast<int>(TS::Map::EventTriggerType::None);
	pg.MoveType = static_cast<int>(TS::Map::EventMoveType::None);
	pg.Direction = direction4;
	pg.MoveSpeed = static_cast<int>(TS::Map::Speed::Normal);
	pg.MoveFrequency = static_cast<int>(TS::Map::Speed::Normal);
	pg.AnimID = animID;
	pg.DrawPriority = priority;
	pg.HitRect.Set(0, 0);
	pg.EnabledStopAnim = enabledStopAnim;
	pg.EnabledWalkAnim = enabledWalkAnim;
	pg.NoHit = true;
	pg.FixedDirection = fixedDirection;
	pg.LoadGraphics(graph);
	ev->AddPage(pg);
	ev->SetPageIndex(0);
	ev->IsDummy = true;

	this->refreshAllEVTargetChar();
	return id;
}


//####################  �C�x���g�̉�ID����FixedID���擾����  ####################
int CSceneMap::CMapData::GetEVFixedID(int visibleID) {
	for(auto& ev : this->EVs.Items) {
		if(ev.VisibleID == visibleID) {
			return ev.ID;
		}
	}
	return -1;
}


//####################  �C�x���g�����ׂč폜����  ####################
void CSceneMap::CMapData::EVClear() {
	while(this->EVs.GetCount() > 0) {
		this->EVs.GetFromIndex(0)->Dispose();
		this->EVs.DeleteFromIndex(0);
	}
	this->EVs.Clear();
}


//####################  �C�x���g���폜����  ####################
void CSceneMap::CMapData::EVDeleteFromID(int id) {
	this->EVs.GetFromID(id)->Dispose();
	this->EVs.DeleteFromID(id);
	this->refreshAllEVTargetChar();
}


//####################  �w��L�������w�肵�������Ɉ�i�߂邩�ǂ��������؂���  ####################
//EVID=-1 �͎�l��������
//EVTrigger��true�ɂ���ƁA��l���ƃC�x���g�Ƃ̊ԂɂԂ��蔻�肪�N�������Ƃ��Ƀg���K�[�����������ꍇ�ɃC�x���g�N����\�񂷂�
bool CSceneMap::CMapData::CheckBeforeMove(int EVID, int x, int y, int direction4, bool EVTrigger) {
	CPoint temp;
	CPoint beforePos(x, y);
	CPoint afterPos;
	this->GetOneMovePosition(beforePos, afterPos, direction4);
	CRectangle range;

	//�ړ���̍��W���}�b�v�̊O�ɂȂ�ꍇ�i���[�v�ݒ肪�s���Ă��Ȃ��ꍇ�j�͈ړ��ł��Ȃ�
	if(afterPos.X < 0 || afterPos.Y < 0) {
		return false;
	}

	//���̑��E���O�ɒf��ł������
	if(EVID != -1 && !this->EVs.GetFromID(EVID)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��C�x���gID [%d] ���Q�Ƃ���܂����B", EVID);
		return false;
	} else if(EVID != -1 && this->EVs.GetFromID(EVID)->GetPageIndex() == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�w�肳�ꂽ�C�x���g [ID: %d] �͌��ݔ�\����Ԃ̃C�x���g�ł��B", EVID);
		return false;
	} else if(EVID == -1 && this->Player.CharData.NoHit) {
		return true;		//��l���������蔻��������Ȃ��Ƃ��̎�l���ړ��́A��ɂǂ��ł��ړ��\
	} else if(EVID != -1 && this->EVs.GetFromID(EVID)->getCurrentPage() != nullptr && this->EVs.GetFromID(EVID)->getCurrentPage()->NoHit) {
		return true;		//�C�x���g�������蔻��������Ȃ��Ƃ��͏�ɂǂ��ł��ړ��\
	}

	//�ړ���ƈړ����̃^�C���p���b�g�����擾���Ă���
	CDatabase::CDBTileset::CTileData* tile;

	//1. �ړ����̒ʍs�ݒ�
	for(int l = static_cast<int>(TS::Map::Layer::High); l >= static_cast<int>(TS::Map::Layer::Low); l--) {
		temp = this->Data->at(l)[beforePos.X][beforePos.Y].PalletPosition;
		if(temp.IsEmpty()) {
			//�󔒃^�C���͎��̃��[�v��
			continue;
		}
		tile = Game.DB.getFDBTileset()->GetTilePallet(temp.X, temp.Y);

		//�I�[�g�^�C���̂�: �������C���[�ɂ��āA�ړ��悪�����^�C���̏ꍇ�͈ړ��ł���Ƃ݂Ȃ�
		if(temp.Y < TS::Map::AutoTileRowLength) {
			if(temp == this->Data->at(l)[afterPos.X][afterPos.Y].PalletPosition) {
				break;
			}
		}

		//����ȊO�͒ʍs�ݒ�𔻒肷��
		if((tile->InOut & (int)pow(2.0, direction4 + 1)) == FALSE) {
			return false;
		} else {
			break;
		}
	}

	//2. �ړ���̒ʍs�ݒ�
	for(int l = static_cast<int>(TS::Map::Layer::High); l >= static_cast<int>(TS::Map::Layer::Low); l--) {
		temp = this->Data->at(l)[afterPos.X][afterPos.Y].PalletPosition;
		if(temp.IsEmpty()) {
			//�󔒃^�C���͎��̃��[�v��
			continue;
		}
		tile = Game.DB.getFDBTileset()->GetTilePallet(temp.X, temp.Y);

		//�I�[�g�^�C���̂�: �������C���[�ɂ��āA�ړ����Ɠ����^�C���̏ꍇ�͈ړ��ł���Ƃ݂Ȃ�
		if(temp.Y < TS::Map::AutoTileRowLength) {
			if(temp == this->Data->at(l)[beforePos.X][beforePos.Y].PalletPosition) {
				break;
			}
		}

		//����ȊO�͒ʍs�ݒ�𔻒肷��
		if((tile->InOut & (int)pow(2.0, Game.getSceneMap()->TurnHalf4(direction4) + 1)) == FALSE) {
			return false;
		} else {
			break;
		}
	}

	//3. �ړ�����W�ɂ���C�x���g�Ƃ̐ڐG����: ��l���̂�
	if(EVID == -1) {
		for(auto& ev : this->EVs.Items) {
			if(ev.GetPageIndex() == -1 || ev.getCurrentPage()->NoHit) {
				continue;		//�����蔻��̂Ȃ��C�x���g�͖���
			} else if(ev.Position == afterPos) {
				if(ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Push) || ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed)) {
					Game.getSceneMap()->MapData.ReserveEV(ev.ID);
				}
				return false;
			}
		}
	}

	//4. �ړ�����W�ɂ����l���Ƃ̐ڐG����: �C�x���g�̂�
	if(EVID != -1 && this->Player.CharData.NoHit == false && this->EVs.GetFromID(EVID)->getCurrentPage() != nullptr) {
		if(this->Player.MapObject.Position == afterPos) {
			if(this->EVs.GetFromID(EVID)->getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed)) {
				Game.getSceneMap()->MapData.ReserveEV(EVID);
			}
			return false;
		}
	}

	//5. �}�b�v�^�C���̓����蔻��
	return this->IsWalkableTile(afterPos.X, afterPos.Y);
}


//####################  �w��L�����̌��݈ʒu�ŃC�x���g���N�����邩�ǂ��������؂���  ####################
//EVID=-1 �͎�l��������
//�����蔻��̗L�����킸�A�ڐG�N������C�x���g����l���ƈ�v���Ă�����C�x���g�N����\�񂷂�
bool CSceneMap::CMapData::CheckOverlapTriggerEV(int EVID, int x, int y) {
	CPoint afterPos(x, y);
	CRectangle range;
	bool reserved = false;

	//1. ���ݒn���W�ɂ���C�x���g�Ƃ̈�v����: ��l���̂�
	if(EVID == -1) {
		for(auto& ev : this->EVs.Items) {
			if(ev.GetPageIndex() != -1) {
				auto pg = ev.getCurrentPage();
				range.Set(ev.Position.X - pg->HitRect.Width, ev.Position.Y - pg->HitRect.Height, pg->HitRect.Width * 2 + 1, pg->HitRect.Height * 2 + 1);
				if(range.PointContains(afterPos) && (ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Push) || ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed))) {
					//��l�����C�x���g�ɐڐG�����Ƃ��ɃC�x���g�N���\����s��
					this->ReserveEV(ev.ID);
					reserved |= true;
				}
			}
		}
		return reserved;
	}

	//2. ��̂��C�x���g�̂Ƃ��A��l���Ƃ̈�v����
	if(EVID != -1 && this->EVs.GetFromID(EVID)->getCurrentPage() != nullptr) {
		auto& ev = *this->EVs.GetFromID(EVID);
		auto pg = ev.getCurrentPage();
		range.Set(ev.Position.X - pg->HitRect.Width, ev.Position.Y - pg->HitRect.Height, pg->HitRect.Width * 2 + 1, pg->HitRect.Height * 2 + 1);
		if(range.PointContains(this->Player.MapObject.Position)) {
			if(this->EVs.GetFromID(EVID)->getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed)) {
				//�C�x���g����l���ɐڐG�����Ƃ��ɃC�x���g�N���\����s��
				this->ReserveEV(EVID);
				reserved |= true;
			}
		}
		return reserved;
	}

	return reserved;
}


//####################  ���݂̎�l���̈ʒu�Ō���L�[����������N������C�x���g������ꍇ�͂��̋N���\����s��  ####################
bool CSceneMap::CMapData::CheckPressEnterTriggerEV() {
	//��l���̈����̍��W���}�b�v�̊O�ɂȂ�ꍇ�i���[�v�ݒ肪�s���Ă��Ȃ��ꍇ�j�͖���
	CPoint beforePos = this->Player.MapObject.Position;
	CPoint afterPos = CPoint();
	this->GetOneMovePosition(beforePos, afterPos, this->Player.CharData.getDirection());
	if(afterPos.X < 0 || afterPos.Y < 0) {
		return false;
	}

	//�����蔻������C�x���g�ׂ͗荇���`�ŁA�����Ȃ��C�x���g�͈�v������Ԃ̂Ƃ��ɋN������
	CRectangle range;
	for(auto& ev : this->EVs.Items) {
		auto pg = ev.getCurrentPage();
		if(pg == nullptr || pg->TriggerType != static_cast<int>(TS::Map::EventTriggerType::PressEnterKey)) {
			continue;		//�ΏۊO�̃C�x���g
		}

		//�����蔻��̗L���ɂ���ĕ�����
		range.Set(ev.Position.X - pg->HitRect.Width, ev.Position.Y - pg->HitRect.Height, pg->HitRect.Width * 2 + 1, pg->HitRect.Height * 2 + 1);
		if(range.PointContains(this->Player.MapObject.Position)) {
			//�ڐG�͈͍l��: ��v�����Ԃ�����
			this->ReserveEV(ev.ID);
			return true;
		} else if(pg->NoHit == false && ev.Position == afterPos) {
			//�ׂ荇����Ԃ�����
			this->ReserveEV(ev.ID);
			return true;
		}
	}
	return false;
}


//####################  �C�x���g���X�g���ύX���ꂽ�Ƃ��Ɋe�C�x���g�̃}�b�v�I�u�W�F�N�g�Ɋ֘A�t����L�����N�^�[�I�u�W�F�N�g���Đݒ肷��  ####################
//���̏������K�v�Ȃ̂́A�C�x���g���X�g�̗v�f�𑝌������Ƃ�
//vector�v�f�̌����ϓ��������_�ł��̗v�f�ȍ~�̃����o�[�Ɋւ���|�C���^��񂪂��ׂċ������߂ł���
void CSceneMap::CMapData::refreshAllEVTargetChar() {
	for(auto &ev : this->EVs.Items) {
		ev.TargetCharPTR = ev.getCurrentPage();
	}
}
