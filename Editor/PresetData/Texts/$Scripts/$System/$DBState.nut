//###################################################################
//		�Ԑڌ��� �X�N���v�g
//###################################################################

///[summary]�늳�����Ƃ��̏���
///[comment]�Q�[���v���O��������C�x���g�n���h���[�Ƃ��ČĂяo����܂��B
///[par]�Ԑڌ���ID
///[par]�ΏۃA�N�^�[ (CDBActorData)
function StateStart(id, actor) {
	if(Game.GetCurrentSubSceneID() != Scene.Battle) {
		return;
	}

	//�e�탁�b�Z�[�W�\��
	switch(id) {
		case 0xEC71D8F/*����*/:
			ActionMessage(actor.Name + " �� �����Ă��܂����I", 0.5);
			break;
		case 0x3C6F7958/*��*/:
			ActionMessage(actor.Name + " �� �łɖ`���ꂽ�I", 0.5);
			break;
		case 0x424CADA2/*�ғ�*/:
			ActionMessage(actor.Name + " �� �ғłɖ`���ꂽ�I", 0.5);
			break;
		case 0x600D76BA/*����*/:
			ActionMessage(actor.Name + " �� ���������I", 0.5);
			break;
		case 0x16AB4BBE/*�Ӗ�*/:
			ActionMessage(actor.Name + " �� ���͂�D��ꂽ�I", 0.5);
			break;
		case 0x4CFF4C48/*����*/:
			ActionMessage(actor.Name + " �� ���@�𕕂���ꂽ�I", 0.5);
			break;
		case 0x4C3A1FDA/*�p���[�A�b�v*/:
			ActionMessage(actor.Name + " �� �U���͂��A�b�v�����I", 0.5);
			break;
		case 0x69CE82E8/*�f�B�t�F���X�A�b�v*/:
			ActionMessage(actor.Name + " �� ����͂��A�b�v�����I", 0.5);
			break;
		case 0x5D4CF03C/*�X�s�[�h�A�b�v*/:
			ActionMessage(actor.Name + " �� �q�������A�b�v�����I", 0.5);
			break;
	}
}


///[summary]�������ꂽ�Ƃ��̏���
///[comment]�Q�[���v���O��������C�x���g�n���h���[�Ƃ��ČĂяo����܂��B
///[par]�Ԑڌ���ID
///[par]�ΏۃA�N�^�[ (CDBActorData)
///[par]��ʌ݊��̊Ԑڌ��ʂɜ늳�����邽�߂ɉ������ꂽ���ǂ���
function StateEnd(id, actor, hasUpper) {
	if(hasUpper
	|| Game.GetCurrentSubSceneID() != Scene.Battle
	|| actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value == 0) {
		return;
	}

	//�e�탁�b�Z�[�W�\��
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	switch(id) {
		case 0xEC71D8F/*����*/:
			ActionMessage(actor.Name + " �� �ڂ��o�܂����I", 0.5);
			break;
		case 0x3C6F7958/*��*/:
			ActionMessage(actor.Name + " �� �ł��������I", 0.5);
			break;
		case 0x424CADA2/*�ғ�*/:
			ActionMessage(actor.Name + " �� �ғł��������I", 0.5);
			break;
		case 0x600D76BA/*����*/:
			ActionMessage(actor.Name + " �� ��ɕԂ����I", 0.5);
			break;
		case 0x16AB4BBE/*�Ӗ�*/:
			ActionMessage(actor.Name + " �� ���͂��񕜂����I", 0.5);
			break;

		default:
			ActionMessage(actor.Name + " �� " + db.GetCell(id, "���O").String + " ���������I", 0.5);
			break;
	}
}


///[summary]�^�[���J�n���̏���
///[par]�A�N�^�[�̃C���f�b�N�X
///[par]�ΏۃA�N�^�[ (CDBActorData)
function StateBeforeTurn(actorIdx, actor) {
	//�ΏۃA�N�^�[���늳���Ă��邷�ׂĂ̊Ԑڌ��ʂ�����
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {
			case 0xEC71D8F/*����*/:
				ActionMessage(actor.Name + " �� �����Ă���I", 0);
				break;
		}
	}
}


///[summary]�^�[���I�����̏���
///[par]�A�N�^�[�̃C���f�b�N�X
///[par]�ΏۃA�N�^�[ (CDBActorData)
function StateAfterTurn(actorIdx, actor) {
	//�ΏۃA�N�^�[���늳���Ă��邷�ׂĂ̊Ԑڌ��ʂ�����
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {
			case 0x3C6F7958/*��*/:
				ActionMessage(actor.Name + " �� �łɖ`����Ă���I", 0);
				HPDamage(actorIdx, actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value * 0.1, true);
				break;

			case 0x424CADA2/*�ғ�*/:
				ActionMessage(actor.Name + " �� �ғłɖ`����Ă���I", 0);
				HPDamage(actorIdx, actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value * 0.2, true);
				break;
		}
	}
}


///[summary]�s���O�̏���
///[par]�ΏۃA�N�^�[ (CDBActorData)
///[par]�s�����e
///[comment]�s����ύX����ɂ� action �𑀍삵�܂��B
function StateBeforeAction(actor, action) {
	//�ΏۃA�N�^�[���늳���Ă��邷�ׂĂ̊Ԑڌ��ʂ�����
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {
			case 0x600D76BA/*����*/:
				ActionMessage(actor.Name + " �� �������Ă���I", 0.5);

				//�R�}���h�������I�ɒʏ�U���ɕύX����
				action.Command = MenuItem.Attack;
				action.EffectID = EmptyEquipEffectID;

				//�Ώۂ�G��������Ȃ������_���I������
				do {
					if(Game.NextRand(1, 100) <= 50) {
						//�p�[�e�B��I��
						action.TargetIndex = Game.NextRand(0, Game.DB.VDBParty.GetCharCount() - 1);
					} else {
						//���j�b�g��I��
						action.TargetIndex = EnemyIndexOrigin + Game.NextRand(0, Enemies.len() - 1);
					}
				} while(IsDead(action.TargetIndex) == true);
				break;

			case 0x4CFF4C48/*����*/:
				if(action.Command == MenuItem.Skill) {
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
					local skillName = db.GetCell(action.Option, "���O").String;
					if(db.GetCell(action.Option, "���").Integer == 0x297C0F3/*���@*/) {
						ActionMessage(actor.Name + " �� " + skillName + " �������悤�Ƃ����I", 0.5);
						ActionMessage("������ " + actor.Name + " �� ���t�𔭂��邱�Ƃ��ł��Ȃ��I", 0.5);
						action.Disabled = true;		//�s���𖳌���
					}
				}
				break;
		}
	}
}


///[summary]�U�����󂯂�O�̏���
///[par]�ΏۃA�N�^�[ (CDBActorData)
///[par]�s�����e
///[comment]�s����ύX����ɂ� action �𑀍삵�܂��B
function StateBeforeDamage(actor, action) {
	//�ΏۃA�N�^�[���늳���Ă��邷�ׂĂ̊Ԑڌ��ʂ�����
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {

		}
	}
}


///[summary]�}�b�v�ړ����̏���
///[par]�p�[�e�B�����o�[�̃C���f�b�N�X
function StateMap(idx) {
	local actor = GetActorData(idx);
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);

	//�ΏۃA�N�^�[���늳���Ă��邷�ׂĂ̊Ԑڌ��ʂ�����
	for(local i = 0; i < actor.States.Count(); ) {
		local id = actor.States.GetID(i);
		actor.States.GetSelfValues(i).Get(0x2C0353DF/*�o�ߕ���*/).Value++;

		// 5�����Ƃɏ���
		if(actor.States.GetSelfValues(i).Get(0x2C0353DF/*�o�ߕ���*/).Value % 5 == 0) {
			switch(id) {
				case 0x3C6F7958/*��*/:
				case 0x424CADA2/*�ғ�*/:
					Game.DB.FDBMaterial.Play(0x4C6DF6BD/*�_���[�W��*/, 100, 0, 0);
					Game.DXLib.Flash(RED, 180, 16, 1);
					if(actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value > 10) {
						PlusActorExParameter(idx, ExParameter.HP, -10);
					}
					break;
			}
		}

		//��������
		if(db.GetCell(id, "��������").Integer == StateSolve.Walk
		&& db.GetCell(id, "���������l").Integer <= actor.States.GetSelfValues(i).Get(0x2C0353DF/*�o�ߕ���*/).Value
		&& Game.NextRand(1, 100) <= db.GetCell(id, "�����m��").Integer) {
			actor.ReleaseState(id);
			continue;
		}
		i++;
	}
}


///[summary]�w�肵�����������ɂ��ĉ���������s���A�������ꂽ�ꍇ�͉������s��
///[par]�ΏۃA�N�^�[ (CDBActorData)
///[par]�������� (StateSolve)
function StateJudgeRelease(actor, stateSolve) {
	//�ΏۃA�N�^�[���늳���Ă��邷�ׂĂ̊Ԑڌ��ʂ�����
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	for(local i = 0; i < actor.States.Count(); ) {
		local id = actor.States.GetID(i);
		local solve = db.GetCell(id, "��������").Integer;

		//������������v����ꍇ�̂݉���������s��
		if(solve == stateSolve) {
			if(solve == StateSolve.Turn
			&& actor.States.GetSelfValues(i).Get(0x45B7FFE4/*�o�߃^�[����*/).Value < db.GetCell(id, "���������l").Integer) {
				//�K�v�^�[�����ɒB���Ă��Ȃ�
				i++;
				continue;
			} else if(Game.NextRand(1, 100) <= db.GetCell(id, "�����m��").Integer) {
				//��������
				actor.ReleaseState(id);
				continue;
			}
		}
		i++;
	}
}


///[summary]���݂̃A�N�^�[�S���ɂ��ĊԐڌ��ʂ̌o�߃^�[���������Z���ĉ�������
function StatePlusTurnActorAll() {
	//�p�[�e�B�ƃ��j�b�g���������������X�g�𐶐����Ĉꊇ��������
	local allActorList = CreateAllActorDataList();
	foreach(actor in allActorList) {
		for(local n = 0; n < actor.States.Count(); n++) {
			actor.States.GetSelfValues(n).Get(0x45B7FFE4/*�o�߃^�[����*/).Value++;
		}
		StateJudgeRelease(actor, StateSolve.Turn);
	}
}


///[summary]�p�[�e�B�S���̐퓬���̂ݗL���ȊԐڌ��ʂ����ׂĉ���
function StateReleaseOnlyBattle() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == true) {
			continue;		//����ł���L�����͏������Ȃ�
		}

		local actor = GetActorData(i);
		for(local n = 0; n < actor.States.Count(); ) {
			local id = actor.States.GetID(n);
			local onlyBattle = !db.GetCell(id, "�ړ������L��").Boolean;
			if(onlyBattle == true) {
				actor.ReleaseState(id);
				continue;
			}
			n++;
		}
	}
}


///[summary]�p�[�e�B�S���̃}�b�v�ړ����̏��������s
function StateMapPartyAll() {
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == true) {
			continue;		//����ł���L�����͏������Ȃ�
		}
		StateMap(i);
	}
}
