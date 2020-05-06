//###################################################################
//			�C�x���g�Ɋւ���N���X��`
//###################################################################
#include "Header.h"
bool EVExtraMove(TS_MT);


//####################  �C�x���g�y�[�W�̃��\�[�X��ǂݍ���  ####################
void CSceneMap::CMapData::CMapEventData::CMapEventPageData::LoadGraphics(string fileName) {
	//���ɃO���t�B�b�N���ǂ܂�Ă���ꍇ�͔j������
	if(!this->Graphics.empty() && !this->IsTilesetGraphics) {
		this->DisposeGraphics();
	}

	this->Graphics = fileName;
	if(fileName.find(":") == string::npos) {
		//�^�C���O���t�B�b�N�łȂ��ꍇ�̂ݓǂݍ���
		this->GraphHandle = Game.Material.getGraphics()->Add(fileName);
		this->IsTilesetGraphics = false;
		CSize graphSize;
		DxLib::GetGraphSize(this->GraphHandle, &graphSize.Width, &graphSize.Height);
		this->OneCharSize.Set(
			graphSize.Width / static_cast<int>(TS::Map::EventAnimation::Count),
			graphSize.Height / static_cast<int>(TS::Map::Direction4::None)
			);
	} else {
		this->GraphHandle = -1;
		this->IsTilesetGraphics = true;
		this->TilePalletPosition.Set(this->Graphics);
	}
}


//####################  �C�x���g�y�[�W�̃��\�[�X��j������  ####################
void CSceneMap::CMapData::CMapEventData::CMapEventPageData::DisposeGraphics() {
	Game.Material.getGraphics()->Delete(this->GraphHandle);
	this->GraphHandle = -1;
	this->Graphics.clear();
}


//####################  �L���ȃy�[�W�̃C���f�b�N�X��ݒ肷��  ####################
// -1 ��ݒ肷��Ɣ�\����ԂƂȂ�
void CSceneMap::CMapData::CMapEventData::SetPageIndex(int val) {
	TS_CHECKINDEX(val, -1, this->GetPageCount()) {
		//�ύX�����Ƃ��̂ݏ�������
		if(this->pageIndex != val) {
			this->pageIndex = val;
			this->moveTask.clear();
			this->MoveTaskNowIndex = -1;
			this->routeLock = false;
			this->TargetCharPTR = this->getCurrentPage();

			//����ړ��̃X�N���v�g��ǂݍ��ށi�_�~�[�C�x���g�������j
			if(val != -1 && !this->IsDummy) {
				Game.getSceneMap()->MapData.RunEV(this->ID, static_cast<int>(TS::Map::EventScriptType::Move), false);
			}
		}
	}
}


//####################  �L���ȃy�[�W�̃C���f�b�N�X���擾����  ####################
int CSceneMap::CMapData::CMapEventData::GetPageIndex() {
	return this->pageIndex;
}


//####################  �y�[�W�����擾����  ####################
int CSceneMap::CMapData::CMapEventData::GetPageCount() {
	return this->pages.size();
}


//####################  �y�[�W��ǉ�����  ####################
void CSceneMap::CMapData::CMapEventData::AddPage(CMapEventPageData pg) {
	this->pages.push_back(pg);
}


//####################  ����ړ��̃^�X�N��ǉ�����  ####################
CSceneMap::CMapData::CMapEventData* CSceneMap::CMapData::CMapEventData::AddMoveTask(int type, int par1, string par2) {
	this->moveTask.push_back(CEventMoveTask(type, par1, par2));
	return this;
}


//####################  �C�x���g�̌�����ݒ肷��  ####################
void CSceneMap::CMapData::CMapEventData::SetDirection4(int dir4) {
	if(this->getCurrentPage() != nullptr && !this->getCurrentPage()->FixedDirection) {
		this->getCurrentPage()->setDirection(dir4);
	}
}


//####################  ��l���̕�����������  ####################
void CSceneMap::CMapData::CMapEventData::SetDirection4ToPlayer() {
	this->getCurrentPage()->setDirection(this->GetDirection4ToPlayer());
}


//####################  ��l���ɔw������������  ####################
void CSceneMap::CMapData::CMapEventData::SetDirection4AgainstPlayer() {
	this->getCurrentPage()->setDirection(CSceneMap::TurnHalf4(this->GetDirection4ToPlayer()));
}


//####################  ��l���ւ̌������擾����  ####################
int CSceneMap::CMapData::CMapEventData::GetDirection4ToPlayer() {
	int dir = static_cast<int>(TS::Map::Direction4::None);
	if(this->getCurrentPage() != nullptr && !this->getCurrentPage()->FixedDirection) {
		//��l���Ƃ̋������Z�o
		CPoint distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
		if(distance.X < 0 && abs(distance.X) > abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::West);
		} else if(distance.X > 0 && abs(distance.X) > abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::East);
		} else if(distance.Y < 0 && abs(distance.X) < abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::North);
		} else if(distance.Y > 0 && abs(distance.X) < abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::South);
		}
	}
	return dir;
}


//####################  �w�肵�������ɐi�ނ��߂ɕK�v��MoveCounter�̒l���Z�b�g����  ####################
void CSceneMap::CMapData::CMapObjectData::AutoSetMoveCounter(int direction4) {
	CPoint preMove = CSceneMap::Direction4ToPoint(direction4);
	this->MoveCounter.X = (preMove.X < 0) ? +Game.getSceneMap()->MapData.TileSize : (preMove.X > 0) ? -Game.getSceneMap()->MapData.TileSize : 0;
	this->MoveCounter.Y = (preMove.Y < 0) ? +Game.getSceneMap()->MapData.TileSize : (preMove.Y > 0) ? -Game.getSceneMap()->MapData.TileSize : 0;
}


//####################  ���ݗL���ȃy�[�W���擾����  ####################
CSceneMap::CMapData::CMapEventData::CMapEventPageData* CSceneMap::CMapData::CMapEventData::getCurrentPage() {
	if(this->pageIndex == -1) {
		return nullptr;
	} else {
		return &this->pages[this->pageIndex];
	}
}


//####################  �C�x���g�����ړ��̎w�����s��  ####################
//�Ԓl�Ŏ����̃C�x���g�ւ̎Q�Ƃ�Ԃ����߁A�A���I�ɌĂяo���ăZ�b�g�ł���悤�ɂ���
CSceneMap::CMapData::CMapObjectData* CSceneMap::CMapData::CMapObjectData::SetExtraMove(int type, int par1, string par2) {
	this->moveExtraTask.push_back(CEventMoveTask(type, par1, par2));
	return this;
}


//####################  �}�b�v�͈̔͊O�ɂ�����}�b�v��̃��[�v�ݒ�ɏ]���ĕ␳����  ####################
void CSceneMap::CMapData::CMapObjectData::ApplyMapLoopPosition() {
	if(Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal)
	   || Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
		//X�������̃��[�v
		if(this->Position.X < 0) {
			this->Position.X = Game.getSceneMap()->MapData.MapSize.Width - 1;
		} else if(Game.getSceneMap()->MapData.MapSize.Width <= this->Position.X) {
			this->Position.X = 0;
		}
	}
	if(Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical)
	   || Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
		//Y�������̃��[�v
		if(this->Position.Y < 0) {
			this->Position.Y = Game.getSceneMap()->MapData.MapSize.Height - 1;
		} else if(Game.getSceneMap()->MapData.MapSize.Height <= this->Position.Y) {
			this->Position.Y = 0;
		}
	}
}


//####################  �C�x���g�����ړ������s����  ####################
void CSceneMap::CMapData::CMapObjectData::DoExtraMove(bool waitMode) {
	if(this->moveExtraTask.size() == 0) {
		return;		//�^�X�N���Ȃ��ꍇ�͏������Ȃ�
	}

	this->MoveWaitTimer = 0;		//�ړ��ҋ@�^�C�}�[������������
	if(waitMode) {
		//�ړ����I���܂ő҂ꍇ
		for(int i = 0; i < this->moveExtraTask.size();) {
			//���݂̃C���f�b�N�X�̃^�X�N�����s����
			i = this->moveScript(this->moveExtraTask[i], i, false);

			//��x��ʂ��X�V���A�P��ړ����I���܂ő҂�
			do {
				Game.DoSceneDraw();
				TS_EVERYLOOP;
			} while(this->getIsMoving());
		}
		this->moveExtraTask.clear();
	} else {
		Game.MT.AddThread("EVExtraMove", ::EVExtraMove, TS::System::Scene::Map, false)
			.AddInteger("ID", this->ID)
			.AddInteger("idx", 0);
	}
}


//####################  �C�x���g�����ړ������I�Ɏ��s����  ####################
//Vint["ID"] = �C�x���gID
//Vint["idx"] = ���ݎ��s���̃^�X�N�C���f�b�N�X
bool EVExtraMove(TS_MT) {
	CSceneMap::CMapData::CMapObjectData* ev = nullptr;
	if(*Data.GetInteger("ID") == -1) {
		ev = &Game.getSceneMap()->MapData.Player.MapObject;
	} else {
		ev = Game.getSceneMap()->MapData.EVs.GetFromID(*Data.GetInteger("ID"));
	}
	if(ev == nullptr) {
		return true;		//�C�x���g�����݂��Ȃ��̂ŏI��
	}

	if(!ev->getIsMoving()) {
		//���݂̈ړ��^�X�N�����s����
		*Data.GetInteger("idx") = ev->moveScript(ev->getExtraMoveTask(*Data.GetInteger("idx")), *Data.GetInteger("idx"), false);
	}
	if(*Data.GetInteger("idx") <= ev->getExtraMoveTaskCount()) {
		return false;		//�܂��I����Ă��Ȃ�
	}

	//����
	ev->clearExtraMoveTask();
	return true;
}


//####################  ���ׂẴC�x���g�y�[�W�̃��\�[�X��j������  ####################
void CSceneMap::CMapData::CMapEventData::Dispose() {
	for(auto& pg : this->pages) {
		pg.DisposeGraphics();
	}
	vector<CMapEventPageData>().swap(this->pages);
	this->TargetCharPTR = nullptr;
}


//####################  �Z�[�u�f�[�^����C�x���g����ǂݍ���ŃC���X�^���X�𐶐�����  ####################
CSceneMap::CMapData::CMapEventData::CMapEventData(vector<string>& buf) {
	string temp;
	vector<string> spl;
	int pgCount = 0;

	//�C�x���g���F���l��
	temp = Game.PopHead(buf);
	Game.SplitString(spl, temp, ":");
	this->ID = atoi(Game.PopHead(spl).c_str());
	this->VisibleID = atoi(Game.PopHead(spl).c_str());
	pgCount = atoi(Game.PopHead(spl).c_str());
	this->pageIndex = atoi(Game.PopHead(spl).c_str());
	this->Position.X = atoi(Game.PopHead(spl).c_str());
	this->Position.Y = atoi(Game.PopHead(spl).c_str());
	this->Hide = atoi(Game.PopHead(spl).c_str());
	this->MoveTaskNowIndex = atoi(Game.PopHead(spl).c_str());

	//�C�x���g���F������
	temp = Game.PopHead(buf);
	this->Name = temp;

	//�y�[�W���
	TS_COUNTLOOP(i, pgCount) {
		CMapEventPageData page;

		//�y�[�W���F���l��
		temp = Game.PopHead(buf);
		Game.SplitString(spl, temp, ":");
		page.TriggerType = atoi(Game.PopHead(spl).c_str());
		page.MoveType = atoi(Game.PopHead(spl).c_str());
		page.Direction = atoi(Game.PopHead(spl).c_str());
		page.MoveSpeed = atoi(Game.PopHead(spl).c_str());
		page.MoveFrequency = atoi(Game.PopHead(spl).c_str());
		page.AnimID = atoi(Game.PopHead(spl).c_str());
		page.DrawPriority = atoi(Game.PopHead(spl).c_str());
		page.HitRect.Width = atoi(Game.PopHead(spl).c_str());
		page.HitRect.Height = atoi(Game.PopHead(spl).c_str());
		page.EnabledStopAnim = atoi(Game.PopHead(spl).c_str());
		page.EnabledWalkAnim = atoi(Game.PopHead(spl).c_str());
		page.NoHit = atoi(Game.PopHead(spl).c_str());
		page.FixedDirection = atoi(Game.PopHead(spl).c_str());

		//�y�[�W���F������
		page.LoadGraphics(Game.PopHead(buf));

		this->pages.push_back(page);
	}
}


//####################  �C�x���g�����Z�[�u�f�[�^�ɏ�������  ####################
void CSceneMap::CMapData::CMapEventData::toString(string& buf) {
	//�C�x���g���
	buf += Game.Format("%d:%d:%d:%d:%d:%d:%d:%d\n",
					   this->ID,
					   this->VisibleID,
					   this->GetPageCount(),
					   this->pageIndex,
					   this->Position.X,
					   this->Position.Y,
					   this->Hide,
					   this->MoveTaskNowIndex
					   );
	buf += Game.Format("%s\n", this->Name.c_str());

	//�y�[�W���
	for(auto& page : this->pages) {
		buf += Game.Format("%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\n",
						   page.TriggerType,
						   page.MoveType,
						   page.Direction,
						   page.MoveSpeed,
						   page.MoveFrequency,
						   page.AnimID,
						   page.DrawPriority,
						   page.HitRect.Width,
						   page.HitRect.Height,
						   page.EnabledStopAnim,
						   page.EnabledWalkAnim,
						   page.NoHit,
						   page.FixedDirection
						   );
		buf += Game.Format("%s\n", page.Graphics.c_str());
	}
}


//####################  �C�x���g�������O�o�͗p�̃f�[�^�ɂ��ĕԂ�  ####################
string CSceneMap::CMapData::CMapEventData::toString() {
	string buf;

	buf = Game.Format("%d (%s)\n\tPageIdx:%d\tPos(%d, %d)\t����:%d\n",
					  this->ID,
					  this->Name.c_str(),
					  this->pageIndex,
					  this->Position.X,
					  this->Position.Y,
					  this->Hide
					  );

	//�A�N�e�B�u�ȃy�[�W�̏�񂾂����g��
	if(this->pageIndex != -1) {
		auto page = this->getCurrentPage();
		buf += Game.Format("\t�N������:%d\t����ړ�:%d\t���x:%d\t�p�x:%d\n\t�A�j��ID:%d\t�`��D��x:%d\t�ڐG�͈�:%dx%d\t�Î~�A�j��:%d\t���s�A�j��:%d\t���蔲��:%d\t�����Œ�:%d\n\t�O���t�B�b�N:\"%s\"\n",
						   page->TriggerType,
						   page->MoveType,
						   page->MoveSpeed,
						   page->MoveFrequency,
						   page->AnimID,
						   page->DrawPriority,
						   page->HitRect.Width,
						   page->HitRect.Height,
						   page->EnabledStopAnim,
						   page->EnabledWalkAnim,
						   page->NoHit,
						   page->FixedDirection,
						   page->Graphics.c_str()
						   );
	}

	//�ʕϐ����
	buf += "\t�ʕϐ�: ";
	for(auto value : this->SelfValues.data.Items) {
		buf += Game.Format("ID:%d(%s)=%d  ",
						   value.second.ID,
						   Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfEvent))->GetRowFromID(value.second.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
						   value.second.Value
						   );
	}
	buf += "\n";
	return buf;
}


//####################  �C�x���g����ړ��̎��̃^�X�N�����o��  ####################
CSceneMap::CMapData::CEventMoveTask* CSceneMap::CMapData::CMapEventData::getNextMoveTask() {
	TS_CHECKINDEX(this->MoveTaskNowIndex + 1, 0, this->moveTask.size()) {
		return &this->moveTask[this->MoveTaskNowIndex + 1];
	} else {
		return nullptr;
	}
}


//####################  �C�x���g�����ړ��̎w�肵���C���f�b�N�X�̃^�X�N���擾����  ####################
CSceneMap::CMapData::CEventMoveTask CSceneMap::CMapData::CMapObjectData::getExtraMoveTask(int index) {
	TS_CHECKINDEX(index, 0, this->moveExtraTask.size()) {
		return this->moveExtraTask[index];
	}
	return CEventMoveTask();
}


//####################  �����_���ړ����J�n����  ####################
void CSceneMap::CMapData::CMapEventData::moveRandom() {
	CPoint preMove;

	//�����ʒu����10x10�͈͓̔��Ń����_���ړ�����
	do {
		preMove.Set(0, 0);
		preMove = CSceneMap::Direction4ToPoint(Game.NextRand(0, static_cast<int>(TS::Map::Direction4::None) - 1));
	} while(
		this->relativePosition.X + preMove.X < -TS::Map::EVRandomMoveRange
		|| this->relativePosition.Y + preMove.Y < -TS::Map::EVRandomMoveRange
		|| this->relativePosition.X + preMove.X > TS::Map::EVRandomMoveRange
		|| this->relativePosition.Y + preMove.Y > TS::Map::EVRandomMoveRange
		);

	//���ۂɈړ�������: �����œ����ɐڐG������s��
	if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), true)) {
		int dir4 = CSceneMap::PointToDirection4(preMove);
		this->Position = this->Position + preMove;
		this->relativePosition = this->relativePosition + preMove;
		this->AutoSetMoveCounter(dir4);
		this->SetDirection4(dir4);
	}
}


//####################  ��l���ɋ߂Â��ړ����J�n����  ####################
void CSceneMap::CMapData::CMapEventData::moveNear() {
	CPoint preMove, distance;

	//��l���Ƃ̋������Z�o
	distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;

	if(distance.X != 0 && distance.Y != 0) {
		//�c�E���ǂ���ł��ړ��ł���: �㉺/���E�̓����_���Ɍ��߂�
		switch(Game.NextRand(0, 1)) {
			case 0:		//���ړ�
				preMove.X = (distance.X < 0) ? -1 : +1;
				break;
			case 1:		//�c�ړ�
				preMove.Y = (distance.Y < 0) ? -1 : +1;
				break;
		}
	} else if(distance.X != 0) {
		//�������ł݈̂ړ�
		preMove.X = (distance.X < 0) ? -1 : +1;
	} else if(distance.Y != 0) {
		//�c�����ł݈̂ړ�
		preMove.Y = (distance.Y < 0) ? -1 : +1;
	}

	//���ۂɈړ�������: �����œ����ɐڐG������s��
	if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), true)) {
		int dir4 = CSceneMap::PointToDirection4(preMove);
		this->Position = this->Position + preMove;
		this->AutoSetMoveCounter(dir4);
		this->SetDirection4(dir4);
	}
}


//####################  ��l�����牓������ړ����J�n����  ####################
void CSceneMap::CMapData::CMapEventData::moveAway() {
	CPoint preMove, distance;
	int Xmove = (Game.NextRand(1, 100) <= 50);

	//��l���Ƃ̋������Z�o
	distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;

	//��������_�Ƃ��Ď�l��������ی��ɍs���Ȃ��悤�ɂ���
	if(distance.X == 0 && abs(distance.Y) != 0) {
		//�c�����ŕ���ł�����: Y�œ�����X�̓����_���ړ�
		if(Xmove) {
			preMove.X = (Game.NextRand(1, 100) <= 50) ? -1 : +1;
		} else {
			preMove.Y = (distance.Y > 0) ? -1 : +1;
		}
	} else if(abs(distance.X) != 0 && distance.Y == 0) {
		//�������ŕ���ł�����: X�œ�����Y�̓����_���ړ�
		if(Xmove) {
			preMove.X = (distance.X > 0) ? -1 : +1;
		} else {
			preMove.Y = (Game.NextRand(1, 100) <= 50) ? -1 : +1;
		}
	} else if(distance.X > 0 && distance.Y > 0) {
		//��P�ی�
		if(Xmove) {
			preMove.X = -1;
		} else {
			preMove.Y = -1;
		}
	} else if(distance.X < 0 && distance.Y > 0) {
		//��Q�ی�
		if(Xmove) {
			preMove.X = +1;
		} else {
			preMove.Y = -1;
		}
	} else if(distance.X < 0 && distance.Y < 0) {
		//��R�ی�
		if(Xmove) {
			preMove.X = +1;
		} else {
			preMove.Y = +1;
		}
	} else if(distance.X > 0 && distance.Y < 0) {
		//��S�ی�
		if(Xmove) {
			preMove.X = -1;
		} else {
			preMove.Y = +1;
		}
	}

	//���ۂɈړ�������: �����œ����ɐڐG������s��
	if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), true)) {
		int dir4 = CSceneMap::PointToDirection4(preMove);
		this->Position = this->Position + preMove;
		this->AutoSetMoveCounter(dir4);
		this->SetDirection4(dir4);
	}
}


//####################  �X�N���v�g�w��̈ړ����J�n����  ####################
//�Ԓl�͎��Ɏ��s���ׂ��^�X�N�C���f�b�N�X
int CSceneMap::CMapData::CMapObjectData::moveScript(CEventMoveTask task, int tasknowIndex, bool EVTrigger) {
	int temp;
	CPoint distance, preMove;
	auto charObj = this->TargetCharPTR;
	if(charObj == nullptr) {
		return -1;
	}
	auto EVchar = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(charObj);

	switch((TS::Map::EventMoveTask)task.TaskType) {
		case TS::Map::EventMoveTask::Wait:
			//�I�v�V�����̒P�ʂ̓~���b
			if(this->MoveWaitTimer <= 0) {
				this->MoveWaitTimer = DxLib::GetNowCount();		//�E�F�C�g�̃X�^�[�g�������Z�b�g
			}
			if(::GetNowCount() - this->MoveWaitTimer < task.OptionInteger) {
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			this->MoveWaitTimer = 0;			//�^�C�}�[���������Ď��֐i��
			break;

		case TS::Map::EventMoveTask::Repeat:
			return -1;		//�ŏ��̃C���f�b�N�X�ֈړ�����

		case TS::Map::EventMoveTask::Forward:		//����O�i
			preMove = CSceneMap::Direction4ToPoint(charObj->getDirection());
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(charObj->getDirection());
			} else if(this->routeLock) {
				//��Q���ɓ��������Ƃ��A�o�H�Œ�̏ꍇ�͎��̃t���[���ōēx�ړ������݂�
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			break;
		case TS::Map::EventMoveTask::Backward:		//������
			temp = CSceneMap::TurnHalf4(charObj->getDirection());
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);		//�����Œ肾�ƕύX����Ȃ����Ƃ�����
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(charObj->getDirection());
			} else if(this->routeLock) {
				//��Q���ɓ��������Ƃ��A�o�H�Œ�̏ꍇ�͎��̃t���[���ōēx�ړ������݂�
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			break;
		case TS::Map::EventMoveTask::Move:			//�w������ֈ���i��
			preMove = CSceneMap::Direction4ToPoint(task.OptionInteger);
			charObj->setDirection(task.OptionInteger);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(task.OptionInteger);
			} else if(this->routeLock) {
				//��Q���ɓ��������Ƃ��A�o�H�Œ�̏ꍇ�͎��̃t���[���ōēx�ړ������݂�
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			break;
		case TS::Map::EventMoveTask::MoveRandom:	//�����_�������ֈ���i��
			temp = Game.NextRand(0, static_cast<int>(TS::Map::Direction4::None) - 1);
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(temp);
			} else if(this->routeLock) {
				//��Q���ɓ��������Ƃ��A�o�H�Œ�̏ꍇ�͎��̃t���[���ōēx�ړ������݂�
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			break;
		case TS::Map::EventMoveTask::MoveToPlayer:			//��l�����ֈ���i��
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			temp = CSceneMap::PointToDirection4(distance);
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(temp);
			} else if(this->routeLock) {
				//��Q���ɓ��������Ƃ��A�o�H�Œ�̏ꍇ�͎��̃t���[���ōēx�ړ������݂�
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			break;
		case TS::Map::EventMoveTask::MoveAgainstPlayer:	//���E��l�����ֈ���i��
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			distance.X = -distance.X;
			distance.Y = -distance.Y;		//��l���Ƃ͔��Ε����ɂ���
			temp = CSceneMap::PointToDirection4(distance);
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(temp);
			} else if(this->routeLock) {
				//��Q���ɓ��������Ƃ��A�o�H�Œ�̏ꍇ�͎��̃t���[���ōēx�ړ������݂�
				return tasknowIndex;			//���̃^�X�N�Ɉڂ炸�Ɏ��̃t���[���܂ő҂�
			}
			break;

		case TS::Map::EventMoveTask::Direction:
			charObj->setDirection(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::Turn180:
			charObj->setDirection(CSceneMap::TurnHalf4(charObj->getDirection()));
			break;
		case TS::Map::EventMoveTask::TurnLeft:
			charObj->setDirection(CSceneMap::TurnLeft4(charObj->getDirection()));
			break;
		case TS::Map::EventMoveTask::TurnRight:
			charObj->setDirection(CSceneMap::TurnRight4(charObj->getDirection()));
			break;
		case TS::Map::EventMoveTask::TurnRandom:
			charObj->setDirection(Game.NextRand(0, static_cast<int>(TS::Map::Direction4::None) - 1));
			break;

		case TS::Map::EventMoveTask::TurnToPlayer:
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			charObj->setDirection(CSceneMap::PointToDirection4(distance));
			break;

		case TS::Map::EventMoveTask::TurnAgainstPlayer:
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			charObj->setDirection(CSceneMap::TurnHalf4(CSceneMap::PointToDirection4(distance)));
			break;

		case TS::Map::EventMoveTask::EnabledStopAnim:
			charObj->EnabledStopAnim = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::EnabledWalkAnim:
			charObj->EnabledWalkAnim = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::FixedDirection:
			charObj->FixedDirection = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::NoHit:
			charObj->NoHit = Game.IntToBool(task.OptionInteger);
			break;

		case TS::Map::EventMoveTask::Hide:
			this->Hide = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::LockRoute:
			this->routeLock = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::SetMoveFrequency:
			if(EVchar != nullptr) {
				//�C�x���g�y�[�W������Ώۂł���Ƃ��̂ݎ��s
				EVchar->MoveFrequency = task.OptionInteger;
			}
			break;
		case TS::Map::EventMoveTask::SetMoveSpeed:
			if(EVchar != nullptr) {
				//�C�x���g�y�[�W������Ώۂł���Ƃ��̂ݎ��s
				EVchar->MoveSpeed = task.OptionInteger;
			}
			break;

		case TS::Map::EventMoveTask::ChangeGraphics:
			if(EVchar != nullptr) {
				//�C�x���g�y�[�W������Ώۂł���Ƃ��̂ݎ��s
				EVchar->LoadGraphics(task.OptionString);
			}
			break;

		case TS::Map::EventMoveTask::SQ:
			//�C�x���g�n���h���[�Ƃ��ăX�N���v�g���Ăяo��
			bool result = Game.SQ.DoFunc<bool>(TS::SQ::SQEvent_EVMove, this->ID, task.OptionInteger);
			if(!result) {
				return tasknowIndex;		//false���Ԃ����ꍇ�͎��̃t���[���ōĎ��s����
			}
			break;
	}
	this->ApplyMapLoopPosition();		//�}�b�v�͈̔͊O�ɏo����}�b�v�̃��[�v�ݒ�ɏ]���ĕ␳����
	return tasknowIndex + 1;
}
