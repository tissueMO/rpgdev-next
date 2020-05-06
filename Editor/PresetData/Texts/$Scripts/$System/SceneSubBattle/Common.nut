//###################################################################
//		�o�g���V�[��: �ėp����
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\Common.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Classes.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Expression.nut");


///[summary]���S���Ă���p�[�e�B�����o�[��S���h��
///[comment]�퓬�I����Ȃǂɑh�������i���Ȃ��ꍇ�̂��߂ɁA���S���Ă��郁���o�[��S�� HP=1 �őh�������܂��B
function RecoverPartyAll() {
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == false) {
			continue;
		}

		local actor = GetActorData(i);
		actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value = 1;
	}
}


///[summary]�������̃e�L�X�g���`
///[return]�������ɕ\�����ׂ��e�L�X�g
function GetBattleComment() {
	switch(Phase) {
		case BattlePhase.Action:
			return ActionMsg;

		case BattlePhase.Command:
			if(TargetUTSelecting == true) {
				return "�ΏۃG�l�~�[��I�����ĉ������B";
			} else if(TargetPTSelecting == true) {
				return "�Ώۃ����o�[��I�����ĉ������B";
			} else if(Depth == 0) {
				return "�R�}���h��I�����ĉ������B";
			}
			switch(Cursor[0]) {
				case MenuItem.Skill:
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
					switch(Depth) {
						case 1:		return "��ʂ�I�����ĉ������B";
						case 2:		return db.GetCell(SkillList[Cursor[2] + TopIndex[2]].ID, "����").String;
					}
					break;

				case MenuItem.Item:
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
					switch(Depth) {
						case 1:		return "��ʂ�I�����ĉ������B";
						case 2:
							switch(Cursor[1]) {
								case CommandItemType.Having:
									return db.GetCell(Game.DB.VDBParty.GetItemDataFromIndex(Cursor[2] + TopIndex[2]).ID, "����").String;
								case CommandItemType.Equip:
									local id = actor.GetEquipIDFromItemTypeID(EquipTypeList[Cursor[2]]);
									if(id == -1) {
										return "�������Ă��Ȃ���Ԃł��B";
									}
									return db.GetCell(id, "����").String;
							}
					}
					break;
			}
			break;
	}
	return "";
}


///[summary]�s�����b�Z�[�W��\��
///[par]���b�Z�[�W���e
///[par]�W���̑҂����Ԃɉ����đ҂b��
function ActionMessage(strText, plusTime) {
	ActionMsg = strText;
	ActionWait(plusTime);
}


///[summary]�s����̃E�F�C�g����
///[par]�W���̑҂����Ԃɉ����đ҂b��
function ActionWait(plusTime) {
	if(Phase == BattlePhase.Ending) {
		//�퓬�I����͈�؂̃E�F�C�g�����܂Ȃ�
		return;
	} else if(Game.GetKeyInputCounter(KeyID.A) > 0) {
		//������
		Game.Wait(0.1, true, true);
	} else {
		Game.Wait((0.5 + plusTime) * UI315411244/*�퓬���b�Z�[�W�\�����x*/, true, true);
	}
}


///[summary]�p�[�e�B�����o�[�I��
///[par]���҂�I���ł���悤�ɂ��邩�ǂ���
///[return]�I���C���f�b�N�X�B�L�����Z�����ꂽ�ꍇ�� -1 ���Ԃ���܂��B
function SelectPTMember(boolDeadMode) {
	TargetPTAllowDead = boolDeadMode;
	TargetPTSelecting = true;
	TopIndex[TargetPTDepth] = 0;

	//�ŏ��̃C���f�b�N�X�����߂�
	Cursor[TargetPTDepth] = 0;
	while(boolDeadMode == false && IsDead(Cursor[TargetPTDepth]) == true) {
		Cursor[TargetPTDepth]++;
	}

	while(Game.Wait(0, true, true)) {
		if(CheckLeftKeyInput() == true) {
			//�O�̃L������
			for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
				Cursor[TargetPTDepth] = Game.DownAround(Cursor[TargetPTDepth], 0, Game.DB.VDBParty.GetCharCount());

				//����ł���ꍇ�͔�΂�
				if(boolDeadMode == false && IsDead(Cursor[TargetPTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckRightKeyInput() == true) {
			//���̃L������
			for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
				Cursor[TargetPTDepth] = Game.UpAround(Cursor[TargetPTDepth], 0, Game.DB.VDBParty.GetCharCount());

				//����ł���ꍇ�͔�΂�
				if(boolDeadMode == false && IsDead(Cursor[TargetPTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckDecideKeyInput() == 1) {
			//����
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//�L�����Z��: �J�[�\���ʒu���}�C�i�X�ɂ���
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
			Cursor[TargetPTDepth] = -1;
			break;
		}
	}
	TargetPTSelecting = false;

	//�I�����ʂ�Ԃ�
	return Cursor[TargetPTDepth];
}


///[summary]�G�l�~�[�I��
///[return]�I���C���f�b�N�X�B�L�����Z�����ꂽ�ꍇ�� -1 ���Ԃ���܂��B
function SelectUTEnemy() {
	TargetUTSelecting = true;
	Cursor[TargetUTDepth] = 0;
	TopIndex[TargetUTDepth] = 0;

	//�ŏ��̃C���f�b�N�X�����߂�
	Cursor[TargetUTDepth] = 0;
	while(IsDead(EnemyIndexOrigin + Cursor[TargetUTDepth]) == true) {
		Cursor[TargetUTDepth]++;
	}

	while(Game.Wait(0, true, true)) {
		if(CheckLeftKeyInput() == true) {
			//�O�̃G�l�~�[��
			for(local i = 0; i < Enemies.len(); i++) {
				Cursor[TargetUTDepth] = Game.DownAround(Cursor[TargetUTDepth], 0, Enemies.len());

				//����ł���ꍇ�͔�΂�
				if(IsDead(EnemyIndexOrigin + Cursor[TargetUTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckRightKeyInput() == true) {
			//���̃G�l�~�[��
			for(local i = 0; i < Enemies.len(); i++) {
				Cursor[TargetUTDepth] = Game.UpAround(Cursor[TargetUTDepth], 0, Enemies.len());

				//����ł���ꍇ�͔�΂�
				if(IsDead(EnemyIndexOrigin + Cursor[TargetUTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckDecideKeyInput() == 1) {
			//����
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//�L�����Z��: �J�[�\���ʒu���}�C�i�X�ɂ���
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
			Cursor[TargetUTDepth] = -1;
			break;
		}
	}
	TargetUTSelecting = false;

	//�I�����ʂ�Ԃ�
	return (Cursor[TargetUTDepth] == -1) ? -1 : Cursor[TargetUTDepth] + EnemyIndexOrigin;
}


///[summary]�w�肵���T�C�h�̑Ώۂ������_���ɑI������
///[par]�ΏۃT�C�h (AttackSide)
///[return]�A�N�^�[�̃C���f�b�N�X
function SelectRandom(side) {
	local idx = -1;
	if(side == AttackSide.Party) {
		do {
			idx = Game.NextRand(0, Game.DB.VDBParty.GetCharCount() - 1);
		} while(IsDead(idx) == true);
	} else {
		do {
			idx = EnemyIndexOrigin + Game.NextRand(0, Enemies.len() - 1);
		} while(IsDead(idx) == true);
	}
	return idx;
}


///[summary]�ΏۑI�𒆂ł��邩�ǂ����𒲂ׂ�
///[return]�ΏۑI�𒆂��ǂ���
function IsTargetSelecting() {
	return (TargetPTSelecting || TargetUTSelecting);
}


///[summary]�p�[�e�B�L����/�G�l�~�[�̃A�N�^�[��񃊃X�g�������������z���Ԃ�
///[return]CDBActorData�z��
function CreateAllActorDataList() {
	local allActorList = array(0);

	//�p�[�e�B�L����
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		allActorList.append(Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i)));
	}

	//�G�l�~�[
	foreach(enemy in Enemies) {
		allActorList.append(enemy);
	}
	return allActorList;
}


///[summary]�p�[�e�B�L����/�G�l�~�[�̃A�N�^�[�C���f�b�N�X�������������z���Ԃ�
///[return]Integer�z��
function CreateAllActorIndexList() {
	local allActorList = array(0);

	//�p�[�e�B�L����
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		allActorList.append(i);
	}

	//�G�l�~�[
	foreach(idx, enemy in Enemies) {
		allActorList.append(EnemyIndexOrigin + idx);
	}
	return allActorList;
}


///[summary]�w�肵��ID�̃A�C�e�����s���\�񂵂Ă�����𒲂ׂ�
///[par]�A�C�e��ID
///[return]�s���\��
function GetReservedItemCount(itemID) {
	//���Օi�łȂ��ꍇ�͖���
	if(Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(itemID, "���Օi").Boolean == false) {
		return 0;
	}

	//�s���\��̒�����p�[�e�B�L�����̃A�C�e���R�}���h�œ���̃A�C�e��ID��I�����Ă���f�[�^�𐔂���
	local count = 0;
	foreach(data in Actions) {
		if(GetAttackSide(data.ActorIndex) == AttackSide.Party
		&& data.Command == MenuItem.Item
		&& data.Option == itemID) {
			count++;
		}
	}
	return count;
}


///[summary]�X�L����ʃ��X�g���X�V
function RefreshSkillTypeList() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.SkillType);
	SkillTypeList.resize(0);
	for(local i = 0; i < db.GetRowCount(); i++) {
		local data = CIDNameData();
		data.ID = db.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer;
		data.Name = db.GetCellFromIndex(i, DBDefaultColumnIndices.Name).String;
		SkillTypeList.append(data);
	}
}


///[summary]���ݑI������Ă����ʂɍi�����X�L�����X�g�ōX�V
function RefreshSkillList() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
	SkillList.resize(0);

	//�K���X�L�����璲�ׂ�
	for(local i = 0; i < actor.LearnedSkills.Count(); i++) {
		//���݂̎�ʂɈ�v���Ȃ����̂͏������Ȃ�
		if(db.GetCell(actor.LearnedSkills.GetID(i), "���").Integer != SkillTypeList[Cursor[1]].ID) {
			continue;
		}
		local data = CIDValueData();
		data.ID = db.GetCell(actor.LearnedSkills.GetID(i), db.GetColumnName(DBDefaultColumnIndices.FixedID)).Integer;
		data.Name = db.GetCell(actor.LearnedSkills.GetID(i), db.GetColumnName(DBDefaultColumnIndices.Name)).String;
		data.Value = db.GetCell(actor.LearnedSkills.GetID(i), "�R�X�g").Integers.Get(0);
		SkillList.append(data);
	}
}


///[summary]HP�_���[�W��^����
///[par]�A�N�^�[�̃C���f�b�N�X
///[par]�_���[�W��
///[par]���b�Z�[�W��\�����邩�ǂ���
///[return]�_���[�W��
function HPDamage(idx, value, enabledMsg) {
	//�_���[�W�ʂ𐮐��ɒ���
	value = value.tointeger();

	//���l�\��
	if(value > 0) {
		Game.DB.FDBMaterial.Stop(0x4C6DF6BD/*�_���[�W��*/);
		Game.DB.FDBMaterial.Play(0x4C6DF6BD/*�_���[�W��*/, 255, 0, 0);
		DamageEffectTasks.append(SQDamageEffectData(idx, value, SQDamageEffectData.DamageEffectType.HPDamage));

		//�_���[�W���[�V����
		if(GetAttackSide(idx) == AttackSide.Party) {
			PTPanelShakes[idx].StartShake(7);
		} else {
			DamageMotionUT[idx - EnemyIndexOrigin].StartShake(-1);
		}
	} else if(value < 0) {
		//�񕜂̏ꍇ
		HPHeal(idx, -value, enabledMsg);
		return;
	} else {
		//�~�X
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
	}

	//�N���e�B�J���\��: �~�X�������ꍇ�͕\�����Ȃ�
	if(CriticalHitFlag == true && value > 0) {
		CriticalHitFlag = false;
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Critical));
	}

	//�p�����[�^�[����
	PlusActorExParameter(idx, ExParameter.HP, -value);

	//���b�Z�[�W�\��
	local actor = GetActorData(idx);
	if(enabledMsg == true) {
		if(GetAttackSide(idx) == AttackSide.Party) {
			if(value == 0) {
				ActionMessage("�~�X�I " + actor.Name + " �� �_���[�W���󂯂Ȃ��I", 0.5);
			} else {
				ActionMessage(actor.Name + " �� " + value + " �̃_���[�W���󂯂��I", 0.5);
			}
			if(IsDead(idx) == true) {
				ActionMessage(actor.Name + " �� ����ł��܂����I", 0);
			}
		} else {
			if(value == 0) {
				ActionMessage("�~�X�I " + actor.Name + " �� �_���[�W��^�����Ȃ��I", 0.5);
			} else {
				ActionMessage(actor.Name + " �� " + value + " �̃_���[�W��^�����I", 0.5);
			}
			if(IsDead(idx) == true) {
				ActionMessage(actor.Name + " �� �|�����I", 0);
			}
		}
	}

	//�Ώۂ����񂾏ꍇ�͂��ׂĂ̊Ԑڌ��ʂ���������
	if(IsDead(idx) == true) {
		actor.ClearState();
	} else {
		//�_���[�W���󂯂ĉ�������邩������Ȃ��Ԑڌ��ʂ̔���
		StateJudgeRelease(actor, StateSolve.Damamge);
	}
	return value;
}


///[summary]�w��T�C�h�S�̂�HP�_���[�W��^����
///[par]�s�����e (SQActionData)
///[par]�ΏۃT�C�h (AttackSide)
///[par]�_���[�W�v�Z�̕��@ (UseType)
///[par]�U������ID���X�g (CIntegerArray)
///[par]��{�l
///[par]���U�x
///[par]������
function HPDamageSideAll(actData, targetSide, calcType, attrList, base, variance, successRate) {
	local damage = 0;
	local headName = GetHeadActorName(targetSide);
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		local target = GetActorData(i);

		//�Ώێ�: �U�����󂯂�O�̊Ԑڌ��ʏ���
		local action = clone actData;
		local target = GetActorData(i);
		action.TargetIndex = i;
		StateBeforeDamage(target, action);
		if(action.Disabled == true) {
			continue;		//�U�������������ꂽ��X�L�b�v
		}
		target = GetActorData(action.TargetIndex);

		local value = 0;
		switch(calcType) {
			case UseType.Physic:
				value = CalcPhysicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
			case UseType.Magic:
				value = CalcMagicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
		}
		damage += HPDamage(action.TargetIndex, value, false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//�ꊇ���b�Z�[�W�\��
	if(targetSide == AttackSide.Party) {
		ActionMessage(headName + " �� " + (actors.len() <= 1 ? "" : "���� ") + damage + " �̃_���[�W���󂯂��I", 0.5);
		foreach(idx in actors) {
			if(IsDead(idx) == true) {
				ActionMessage(GetActorData(idx).Name + " �� ����ł��܂����I", 0);
			}
		}
	} else {
		ActionMessage(headName + " �� " + (actors.len() <= 1 ? "" : "���� ") + damage + " �̃_���[�W�I", 0.5);
		foreach(idx in actors) {
			if(IsDead(idx) == true) {
				ActionMessage(GetActorData(idx).Name + " �� �|�����I", 0);
			}
		}
	}
}


///[comment]MP�_���[�W��^����
///[par]�A�N�^�[�̃C���f�b�N�X
///[par]�_���[�W��
///[par]���b�Z�[�W��\�����邩�ǂ���
///[return]�_���[�W��
function MPDamage(idx, value, enabledMsg) {
	//�_���[�W�ʂ𐮐��ɒ���
	value = value.tointeger();

	//���l�\��
	if(value > 0) {
		DamageEffectTasks.append(SQDamageEffectData(idx, value, SQDamageEffectData.DamageEffectType.MPDamage));
	} else if(value < 0) {
		//�񕜂̏ꍇ
		MPHeal(idx, -value, enabledMsg);
		return;
	} else {
		//�~�X
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
	}

	//�N���e�B�J���\��: �~�X�������ꍇ�ł��\������
	if(CriticalHitFlag == true) {
		CriticalHitFlag = false;
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Critical));
	}

	//�p�����[�^�[����
	local actor = GetActorData(idx);
	local delta = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value - value;
	if(delta < 0) {
		//�������������ʂ��}�C�i�X�ɂȂ�ꍇ�̓[���ɂȂ�悤�ɒ�������
		value = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
	}
	PlusActorExParameter(idx, ExParameter.MP, -value);

	//���b�Z�[�W�\��
	if(enabledMsg == true) {
		if(GetAttackSide(idx) == AttackSide.Party) {
			if(value == 0) {
				ActionMessage("�~�X�I " + actor.Name + " �� " + MPName + "�� ������Ȃ������I", 0.5);
			} else {
				ActionMessage(actor.Name + " �� " + value + " �|�C���g�� " + MPName + "�� �����ꂽ�I", 0.5);
			}
		} else {
			if(value == 0) {
				ActionMessage("�~�X�I " + actor.Name + " �� " + MPName + "�� ������Ȃ������I", 0.5);
			} else {
				ActionMessage(actor.Name + " �� " + MPName + "�� " + value + " �|�C���g �������I", 0.5);
			}
		}
	}
	return value;
}


///[summary]�w��T�C�h�S�̂�MP�_���[�W��^����
///[par]�s�����e (SQActionData)
///[par]�ΏۃT�C�h (AttackSide)
///[par]�_���[�W�v�Z�̕��@ (UseType)
///[par]�U������ID���X�g (CIntegerArray)
///[par]��{�l
///[par]���U�x
///[par]������
function MPDamageSideAll(actData, targetSide, calcType, attrList, base, variance, successRate) {
	local damage = 0;
	local headName = GetHeadActorName(targetSide);
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		//�Ώێ�: �U�����󂯂�O�̊Ԑڌ��ʏ���
		local action = clone actData;
		local target = GetActorData(i);
		action.TargetIndex = i;
		StateBeforeDamage(target, action);
		if(action.Disabled == true) {
			continue;		//�U�������������ꂽ��X�L�b�v
		}
		target = GetActorData(action.TargetIndex);

		local value = 0;
		switch(calcType) {
			case UseType.Physic:
				value = CalcPhysicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
			case UseType.Magic:
				value = CalcMagicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
		}
		damage += MPDamage(action.TargetIndex, value, false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//�ꊇ���b�Z�[�W�\��
	if(targetSide == AttackSide.Party) {
		ActionMessage(headName + " �� " + MPName + "�� " + (actors.len() <= 1 ? "" : "���� ") + damage + " �|�C���g �����ꂽ�I", 0.5);
	} else {
		ActionMessage(headName + " �� " + MPName + "�� " + (actors.len() <= 1 ? "" : "���� ") + damage + " �|�C���g �������I", 0.5);
	}
}


///[comment]�Ԑڌ��ʂ�t�^����
///[par]�A�N�^�[�̃C���f�b�N�X
///[par]�Ԑڌ���ID
///[par]������
function SetState(idx, stateID, successRate) {
	local actor = GetActorData(idx);
	local stateDB = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	local isFailed = false;

	if(actor.States.Exists(stateID) && stateDB.GetCell(stateID, "��ʌ݊�").Integer == -1) {
		//��ʌ݊�����������̊Ԑڌ��ʂ͏d�ˊ|�����Ȃ��悤�ɂ���
		print("�Ԑڌ��ʂ̏d�ˊ|�������(��ʌ݊��Ȃ����ɜ늳���Ă��邽��): actorID=" + actor.ID + ", stateID=" + stateID);
		isFailed = true;
	}
	if(CheckUpperState(idx, stateID) == true) {
		//��ʌ݊��̊Ԑڌ��ʂɊ��ɜ늳���Ă���ꍇ�͏d�ˊ|�����Ȃ��悤�ɂ���
		print("�Ԑڌ��ʂ̏d�ˊ|�������(��ʌ݊��ɜ늳���Ă��邽��): actorID=" + actor.ID + ", stateID=" + stateID);
		isFailed = true;
	}
	if(JudgeHit(actor, successRate) == false) {
		//��������
		print("�Ԑڌ��ʖ������s: actorID=" + actor.ID + ", stateID=" + stateID);
		isFailed = true;
	}
	if(isFailed == false && actor.SetState(stateID, false) == false) {
		//�늳���s
		isFailed = true;
	}

	if(isFailed == true) {
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
		ActionMessage("������ " + actor.Name + " �ɂ� �����Ȃ������I", 0.5);
	}
}


///[comment]��ʌ݊��̊Ԑڌ��ʂ��t�^����Ă��邩�ǂ�������
///[par]�A�N�^�[�̃C���f�b�N�X
///[par]�Ԑڌ���ID
///[return]��ʌ݊��̊Ԑڌ��ʂ��t�^����Ă��邩�ǂ���
function CheckUpperState(idx, stateID) {
	local actor = GetActorData(idx);
	local stateDB = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);

	if(stateDB.GetCell(stateID, "��ʌ݊�").Integer == -1) {
		// ���̊Ԑڌ��ʂɏ�ʌ݊��͑��݂��Ȃ�
		return false;
	}

	local upperStateID = stateDB.GetCell(stateID, "��ʌ݊�").Integer;
	local hasUpperState = actor.States.Exists(upperStateID);
	return hasUpperState;
}


///[summary]�X�L��/�A�C�e���̓T�^�I�Ȓ��ڌ���
///[par]�g�p����X�L��/�A�C�e����ID
///[par]�s���҂̃C���f�b�N�X
///[par]�Ώێ҂̃C���f�b�N�X
///[par]�Ώێ��
///[par]���ڌ��ʂ̎��
///[par]�Ԑڌ���ID
///[par]���ʗ�
///[par]���U�x
///[par]������
///[par]�������X�g (CIntegerArray)
///[return]���ۂɌ��ʂ����������ǂ���
function TypicalUse(intID, intActorIdx, intTargetIdx, targetType, useType, stateID, value, variance, successRate, attrList) {
	local used = true;
	local side = -1;

	//�S�̑Ώۂ̏ꍇ�͂ǂ��瑤�̃T�C�h��ΏۂƂ���̂����m�肷��
	switch(targetType) {
		case TargetType.AllParty:
		case TargetType.AllPartyWithDead:
			side = GetAttackSide(intActorIdx);
			break;
		case TargetType.AllEnemy:
			side = GetAgainstAttackSide(GetAttackSide(intActorIdx));
			break;
	}

	switch(useType) {
		case UseType.Physic:
			if(intTargetIdx < 0) {
				//�S�̍U��
				HPDamageSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					UseType.Physic,
					attrList,
					value, variance, successRate
				);
			} else {
				//�P�̍U��
				HPDamage(
					intTargetIdx,
					CalcPhysicalAttack(GetActorData(intActorIdx), GetActorData(intTargetIdx), attrList, value, variance, successRate, false),
					true
				);
			}
			break;

		case UseType.Magic:
			if(intTargetIdx < 0) {
				//�S�̍U��
				HPDamageSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					UseType.Magic,
					attrList,
					value, variance, successRate
				);
			} else {
				//�P�̍U��
				HPDamage(
					intTargetIdx,
					CalcMagicalAttack(GetActorData(intActorIdx), GetActorData(intTargetIdx), attrList, value, variance, successRate, false),
					true
				);
			}
			break;

		case UseType.Heal:
			local delta = 0;

			if(intTargetIdx < 0) {
				//�S��
				delta = HPHealSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					value, variance
				);
			} else {
				//�P��
				delta = HPHeal(
					intTargetIdx,
					CalcHeal(GetActorData(intActorIdx), GetActorData(intTargetIdx), value, variance),
					true
				);
			}

			if(delta == 0) {
				used = false;
			}
			break;

		case UseType.Utility:
			if(intTargetIdx < 0) {
				//�S��
				SetStateSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					stateID, successRate
				);
			} else {
				//�P��
				SetState(intTargetIdx, stateID, successRate);
			}
			break;
	}
	return used;
}


///[summary]�w��T�C�h�S�̂ɊԐڌ��ʂ�t�^����
///[par]�s�����e (SQActionData)
///[par]�ΏۃT�C�h (AttackSide)
///[par]�Ԑڌ���ID
///[par]������
function SetStateSideAll(actData, targetSide, stateID, successRate) {
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		//�Ώێ�: �U�����󂯂�O�̊Ԑڌ��ʏ���
		local action = clone actData;
		local target = GetActorData(i);
		action.TargetIndex = i;
		StateBeforeDamage(target, action);
		if(action.Disabled == true) {
			continue;		//�U�������������ꂽ��X�L�b�v
		}
		target = GetActorData(action.TargetIndex);

		//�Ԑڌ��ʂ�t�^����
		SetState(action.TargetIndex, stateID, successRate);
	}
}


///[summary]�T�C�h�擾
///[par]�A�N�^�[�̃C���f�b�N�X
///[return]�T�C�h (AttackSide)
function GetAttackSide(idx) {
	if(idx < 0) {
		return null;
	} else if(idx < EnemyIndexOrigin) {
		return AttackSide.Party;
	} else {
		return AttackSide.Unit;
	}
}


///[summary]�w��T�C�h�̔��΂�Ԃ�
///[par]�T�C�h (AttackSide)
///[return]�^����ꂽ���΂̃T�C�h (AttackSide)
function GetAgainstAttackSide(side) {
	if(side == AttackSide.Party) {
		return AttackSide.Unit;
	} else {
		return AttackSide.Party;
	}
	return null;
}


///[summary]�w��T�C�h�̍ŏ��̃C���f�b�N�X��Ԃ�
///[par]�T�C�h (AttackSide)
///[return]�A�N�^�[�̃C���f�b�N�X�B���݂��Ȃ��ꍇ�� -1 ���Ԃ���܂��B
function GetFirstActorIndex(side) {
	local result = 0;
	if(side == AttackSide.Party) {
		result = 0;
		while(result < Game.DB.VDBParty.GetCharCount() && IsDead(result) == true) {
			result++;
		}
		if(Game.DB.VDBParty.GetCharCount() <= result) {
			return -1;		//�S������ł���
		}
	} else {
		result = EnemyIndexOrigin;
		while(result - EnemyIndexOrigin < Enemies.len() && IsDead(result) == true) {
			result++;
		}
		if(Enemies.len() <= result - EnemyIndexOrigin) {
			return -1;		//�S������ł���
		}
	}
	return result;
}


///[summary]�w��T�C�h�̎��̃C���f�b�N�X��Ԃ�
///[par]�T�C�h (AttackSide)
///[return]�A�N�^�[�̃C���f�b�N�X�B���݂��Ȃ��ꍇ�� -1 ���Ԃ���܂��B
function GetNextActorIndex(idx) {
	local result = idx + 1;
	if(GetAttackSide(idx) == AttackSide.Party) {
		while(result < Game.DB.VDBParty.GetCharCount() && IsDead(result) == true) {
			result++;
		}
		if(Game.DB.VDBParty.GetCharCount() <= result) {
			return -1;		//���̃A�N�^�[�����Ȃ�
		}
	} else {
		while(result - EnemyIndexOrigin < Enemies.len() && IsDead(result) == true) {
			result++;
		}
		if(Enemies.len() <= result - EnemyIndexOrigin) {
			return -1;		//�S������ł���
		}
	}
	return result;
}


///[summary]�w��T�C�h�̐擪�Җ���Ԃ�
///[par]�T�C�h (AttackSide)
///[return]�擪�Җ��B�p�[�e�B�������l����ꍇ�͕����`�ƂȂ�
function GetHeadActorName(side) {
	local idx = GetFirstActorIndex(side);
	if(idx != -1) {
		return GetActorData(idx).Name + " " + GetPluralString();
	}
	return "";
}


///[summary]�擪�Җ��ɑ��������`��Ԃ�
///[return]�p�[�e�B�������l����ꍇ�Ɍ���A�����`�̕�����B��l�p�[�e�B�̏ꍇ�͋󕶎�
function GetPluralString() {
	if(Game.DB.VDBParty.GetCharCount() > 1) {
		return PluralString;
	}
	return "";
}


///[summary]�퓬�I������
///[return]�퓬���� (BattleResult)�A�I�����Ă��Ȃ��ꍇ�� -1 ���Ԃ���܂��B
function JudgeBattleExit() {
	//��������
	if(GetFirstActorIndex(AttackSide.Unit) == -1) {
		return BattleResult.Win;
	}

	//�s�k����
	if(GetFirstActorIndex(AttackSide.Party) == -1) {
		return BattleResult.Lose;
	}

	//��������
	if(PTEscaped == true) {
		return BattleResult.Escape;
	}

	//���̑�: �t�F�[�Y���I���t�F�[�Y�ɂȂ��Ă���ꍇ�͒��f���ꂽ�Ƃ݂Ȃ�
	if(Phase == BattlePhase.Ending) {
		return BattleResult.Break;
	}

	return -1;
}
