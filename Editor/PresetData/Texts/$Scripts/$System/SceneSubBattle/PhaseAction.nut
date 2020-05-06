//###################################################################
//		�o�g���V�[��: �T�u���[�`��: �s���t�F�[�Y
//###################################################################

///[summary]�s���t�F�[�Y�̏���
function Routine_PhaseAction() {
	//�s���𒀎����o���ď�������
	while(Actions.len() > 0 && Game.Wait(0, true, false)) {
		local actData = Actions.pop();
		local actor = GetActorData(actData.ActorIndex);
		local actorName = actor.Name;
		ActingIndex = actData.ActorIndex;		//�s�����̃A�N�^�[���n�C���C�g�����

		//�s���҂����ݍs���ł��Ȃ���Ԃ̎��̓X�L�b�v����
		if(IsForbiddenAction(actData.ActorIndex) == true) {
			continue;
		}

		//�����ȍs���ł���Ƃ��̓X�L�b�v����
		if(actData.Command == -1) {
			continue;
		}

		//�s����: �s���O�̊Ԑڌ��ʏ���
		StateBeforeAction(actor, actData);
		if(actData.Disabled == true) {
			continue;			//�Ԑڌ��ʂɂ���čs�������������ꂽ��X�L�b�v����
		}

		//���j�b�g�C�x���g: �s���O
		UnitEvent(UnitID, BattleEventType.BeforeAction, actData);

		//�Ώێ�: �U�����󂯂�O�̊Ԑڌ��ʏ���
		local target = (actData.TargetIndex == -1) ? null : GetActorData(actData.TargetIndex);
		local targetName = (target == null) ? "" : target.Name;
		if(target != null) {
			StateBeforeDamage(target, actData);
			if(actData.Disabled == true) {
				continue;		//�Ԑڌ��ʂɂ���čs�������������ꂽ��X�L�b�v����
			}
			target = GetActorData(actData.TargetIndex);
		}

		//�R�}���h�ɉ���������
		ActionCommands[actData.Command](actData, actor, actorName, target, targetName);

		//���j�b�g�C�x���g: �s����
		UnitEvent(UnitID, BattleEventType.AfterAction, actData);

		//�퓬�I������
		local result = JudgeBattleExit();
		if(result != -1) {
			MoveToEndingPhase(result);
			return;
		}
	}

	//�^�[���o�߂ŉ�������邩������Ȃ��Ԑڌ��ʂ̔���
	StatePlusTurnActorAll();

	//���j�b�g�C�x���g: �^�[���I����
	UnitEvent(UnitID, BattleEventType.EndingTurn, null);

	//�^�[���I��
	TurnCount++;
	DoTurnEndingTasks();

	//�퓬�I������
	local result = JudgeBattleExit();
	if(result != -1) {
		MoveToEndingPhase(result);
		return;
	}

	MoveToCommandPhase();
}


//�R�}���h�ɉ������s������
ActionCommands <- {

	///[summary]�U��
	///[par]�s�����e
	///[par]�s���� (CDBActorData)
	///[par]�s���Җ�
	///[par]�Ώێ� (CDBActorData)
	///[par]�ΏێҖ�
	[MenuItem.Attack] = function(actData, actor, actorName, target, targetName) {
		//�����U��
		ActionMessage(actorName + " �̍U���I", 0);
		if(actData.TargetIndex != -1 && IsDead(actData.TargetIndex) == true) {
			//�Ώۂ�����ł���ꍇ�̓����_���I������
			actData.TargetIndex = SelectRandom(GetAttackSide(actData.TargetIndex));
		}

		//�G�t�F�N�g�\��
		PlayActionEffect(actData.EffectID, GetAgainstAttackSide(actData.ActorIndex), null, actData);

		//���푮�����擾
		local attrList = CIntegerArray();
		local equipID = actor.GetEquipIDFromItemTypeID(0x12414D21/*����*/);
		if(equipID != -1) {
			attrList = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(equipID, "����").Integers;
		}

		//���ʓ��e
		if(actData.TargetIndex < 0) {
			//�S�̍U��
			HPDamageSideAll(
				actData,
				GetAgainstAttackSide(GetAttackSide(actData.ActorIndex)),
				UseType.Physic,
				attrList,
				0, 10, 100
			);
		} else {
			//�P�̍U��
			HPDamage(
				actData.TargetIndex,
				CalcPhysicalAttack(actor, target, attrList, 0, 10, 100, true),
				true
			);
		}
	},


	///[summary]�h��
	///[par]�s�����e
	///[par]�s���� (CDBActorData)
	///[par]�s���Җ�
	///[par]�Ώێ� (CDBActorData)
	///[par]�ΏێҖ�
	[MenuItem.Defense] = function(actData, actor, actorName, target, targetName) {
		ActionMessage(actorName + " �͐g���ł߂Ă���I", 0);
	},


	///[summary]�X�L��
	///[par]�s�����e
	///[par]�s���� (CDBActorData)
	///[par]�s���Җ�
	///[par]�Ώێ� (CDBActorData)
	///[par]�ΏێҖ�
	[MenuItem.Skill] = function(actData, actor, actorName, target, targetName) {
		local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
		local skillName = db.GetCell(actData.Option, db.GetColumnName(DBDefaultColumnIndices.Name)).String;
		local skillTypeID = db.GetCell(actData.Option, "���").Integer;
		local targetType = db.GetCell(actData.Option, "�Ώێ��").Integer;

		switch(skillTypeID) {
			case 0x297C0F3/*���@*/:
				ActionMessage(actorName + " �� " + skillName + " �� �������I", 0);
				break;

			case 0x5D2DCD84/*���Z*/:
				ActionMessage(actorName + " �� " + skillName + " �� �������I", 0);
				break;
		}

		//�Ώۂ�����ł���ꍇ�̏C������
		switch(db.GetCell(actData.Option, "�Ώێ��").Integer) {
			case TargetType.OneEnemy:
			case TargetType.OneParty:
				if(actData.TargetIndex != -1 && IsDead(actData.TargetIndex) == true) {
					actData.TargetIndex = SelectRandom(GetAttackSide(actData.TargetIndex));
				}
				break;
		}

		//�G�t�F�N�g�\��
		if(targetType != TargetType.None) {
			PlayActionEffect(db.GetCell(actData.Option, "�G�t�F�N�g").Integer, null, db, actData);
		}

		//�g�p����
		if(UseSkill(actData.Option, actData.ActorIndex, actData.TargetIndex) == true
		&& GetAttackSide(actData.ActorIndex) == AttackSide.Party) {
			//�p�[�e�B�̂�MP����
			local mp = actor.SelfValues.Get(0x46A20EE9/*MP*/);
			mp.Value = Game.WithIn(mp.Value - db.GetCell(actData.Option, "�R�X�g").Integers.Get(0), 0, MaxExParameter);
		}
	},


	///[summary]�A�C�e��
	///[par]�s�����e
	///[par]�s���� (CDBActorData)
	///[par]�s���Җ�
	///[par]�Ώێ� (CDBActorData)
	///[par]�ΏێҖ�
	[MenuItem.Item] = function(actData, actor, actorName, target, targetName) {
		local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
		local itemName = db.GetCell(actData.Option, db.GetColumnName(DBDefaultColumnIndices.Name)).String;
		local targetType = db.GetCell(actData.Option, "�Ώێ��").Integer;

		ActionMessage(actorName + " �� " + itemName + " �� �g�����I", 0);

		//�Ώۂ�����ł���ꍇ�̏C������
		switch(db.GetCell(actData.Option, "�Ώێ��").Integer) {
			case TargetType.OneEnemy:
			case TargetType.OneParty:
				if(actData.TargetIndex != -1 && IsDead(actData.TargetIndex) == true) {
					actData.TargetIndex = SelectRandom(GetAttackSide(actData.TargetIndex));
				}
				break;
		}

		//�G�t�F�N�g�\��
		if(targetType != TargetType.None) {
			PlayActionEffect(db.GetCell(actData.Option, "�G�t�F�N�g").Integer, null, db, actData);
		}

		//�g�p����
		if(UseItem(actData.Option, actData.ActorIndex, actData.TargetIndex, false) == true
		&& GetAttackSide(actData.ActorIndex) == AttackSide.Party) {
			//�p�[�e�B�̂ݏ����
			Game.DB.VDBParty.DeleteItem(actData.Option, 1);
		}
	},


	///[summary]������
	///[par]�s�����e
	///[par]�s���� (CDBActorData)
	///[par]�s���Җ�
	///[par]�Ώێ� (CDBActorData)
	///[par]�ΏێҖ�
	[MenuItem.Escape] = function(actData, actor, actorName, target, targetName) {
		Game.DB.FDBMaterial.Play(0x7C1FDDE0/*������*/, 255, 0, 0);
		if(GetAttackSide(actData.ActorIndex) == AttackSide.Party) {
			//�p�[�e�B�̏ꍇ�͐擪�Җ��ŕ\��
			ActionMessage(GetHeadActorName(AttackSide.Party) + " �� �����o�����I", 0);

			//��������
			if(JudgeEscape(actData.Option) == true) {
				//��������
				PTEscaped = true;
				return;
			} else {
				//�������s
				ActionMessage("������ �������˂Ă��܂����c�I", 0.5);
			}
		} else  {
			//�G�l�~�[�̏ꍇ�̓t���O�𗧂Ă�
			ActionMessage(actorName + " �� �����o�����I", 0.5);
			actor.SelfValues.Get(0x5486E688/*�����������ǂ���*/).Value = TRUE;
		}
	},
};


///[summary]�s�����X�g���A�N�^�[�̕q�����̏����ɕ��ёւ���
function SortActions() {
	//�e�A�N�^�[�̕q�����ɑ��x�␳�������āA���U�l��������
	foreach(act in Actions) {
		local actor = GetActorData(act.ActorIndex);

		switch(act.Command) {
			case MenuItem.Skill:
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value *=
					Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(act.Option, "���x�␳").Integer;
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value /= 100;
				break;

			case MenuItem.Item:
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value *=
					Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(act.Option, "���x�␳").Integer;
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value /= 100;
				break;

			case MenuItem.Escape:
				//������s���̓^�[���̍ŏ��ɍs��
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value = MaxExParameter + 1;
				break;
		}

		GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value +=
			Game.NextRand(0, actor.GetParameter(0x3884DE0B/*�q����*/).Value * SpeedVarianceRate / 100);
	}

	//���U�l���l��������ŕ��ёւ������s����
	Actions.sort(function(a, b) {
		local speedA = GetActorData(a.ActorIndex).GetParameter(0x3884DE0B/*�q����*/).Value +
						GetActorData(a.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value;
		local speedB = GetActorData(b.ActorIndex).GetParameter(0x3884DE0B/*�q����*/).Value +
						GetActorData(b.ActorIndex).SelfValues.Get(0x335C5037/*�q�����̕��U�l*/).Value;
		return ((speedA < speedB) ? -1 : ((speedA > speedB) ? 1 : 0));
	});

	//�f�o�b�O�o��
	if(DEBUG) {
		print(format("[%d �^�[����]", TurnCount));
		print("�s������:");
		foreach(idx, act in Actions) {
			local actor = GetActorData(act.ActorIndex);
			print("\t" + (Actions.len() - idx).tostring() + ": " + actor.Name);
		}
	}
}


///[summary]�^�[���J�n���̏���
function DoTurnStartingTasks() {
	//�h�䂷��A�N�^�[�̏���: �h��t���O�̑���
	foreach(act in Actions) {
		if(act.Command == MenuItem.Defense) {
			GetActorData(act.ActorIndex).SelfValues.Get(0x7BD2A434/*�h�䒆���ǂ���*/).Value = TRUE;

			//���������\�񂷂�
			AddTurnEndingTask(
				{ ["idx"] = act.ActorIndex, },
				function(args) {
					GetActorData(args["idx"]).SelfValues.Get(0x7BD2A434/*�h�䒆���ǂ���*/).Value = FALSE;
				}
			);
		}
	}

	//�p�[�e�B�L�����́u������v�͈�ɂ܂Ƃ߂�
	local counter = 0;
	local escapeActIndex = -1;
	for(local i = 0; i < Actions.len(); ) {
		if(GetAttackSide(Actions[i].ActorIndex) == AttackSide.Party
		&& Actions[i].Command == MenuItem.Escape) {
			counter++;
			if(counter == 1) {
				escapeActIndex = i;
				Actions[escapeActIndex].Option = 1;
			} else if(counter >= 2) {
				//2�l�ڈȍ~�̓�����s���͖����ɂ��āA������l�����X�V����
				Actions.remove(i);
				Actions[escapeActIndex].Option++;
				continue;
			}
		}
		i++;
	}

	//���j�b�g�C�x���g: �^�[���J�n��
	UnitEvent(UnitID, BattleEventType.StartTurn, null);

	//�Ԑڌ��ʏ���: �^�[���J�n��
	local actors = CreateAllActorDataList();
	local actorIdxs = CreateAllActorIndexList();
	foreach(idx, actor in actors) {
		StateBeforeTurn(actorIdxs[idx], actor);
	}
}


///[summary]�^�[���I�����̏�����ǉ�����
///[par]�����ɕK�v�Ȉ��� (�e�[�u��)
///[par]�������e���`�����֐�
function AddTurnEndingTask(args, func) {
	TurnEndingTaskArgs.append(args);
	TurnEndingTasks.append(func);
}


///[summary]�^�[���I�����̏���
function DoTurnEndingTasks() {
	//�Ԑڌ��ʏ���: �^�[���I����
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		StateAfterTurn(i, GetActorData(i));
	}
	for(local i = 0; i < Enemies.len(); i++) {
		StateAfterTurn(EnemyIndexOrigin + i, GetActorData(EnemyIndexOrigin + i));
	}

	while(TurnEndingTasks.len() > 0 && TurnEndingTaskArgs.len() > 0) {
		local task = TurnEndingTasks.pop();
		local args = TurnEndingTaskArgs.pop();
		task(args);
	}

	//�^�X�N��������
	TurnEndingTasks.resize(0);
	TurnEndingTaskArgs.resize(0);
}


///[summary]�A�N�^�[�̃C���f�b�N�X����G�t�F�N�g�̕\�����W��Ԃ�
///[par]�A�N�^�[�̃C���f�b�N�X (�}�C�i�X�̂Ƃ��̓T�C�h�S�̂�����)
///[par]�ΏۃT�C�h
///[return]�\�����W (CPoint)
function GetEffectLocation(idx, destSide) {
	local result = CPoint();
	if(idx <= -1) {
		if(destSide == AttackSide.Party) {
			//�p�[�e�B�S��
			result.Set(
				Game.WindowSize.Width / 2,
				GetActorData(0).Location.Y + PartyPanelHeight / 2
			);
		} else {
			//���j�b�g�S��
			result.Set(
				Game.WindowSize.Width / 2,
				Game.WindowSize.Height / 2
			);
		}
	} else if(destSide == AttackSide.Party) {
		//�p�[�e�B
		result.Set(
			GetActorData(idx).Location.X + (Game.WindowSize.Width / Game.DB.FDBSystem.GetInteger(DB_System_MaxPartyCount)) / 2,
			GetActorData(idx).Location.Y + PartyPanelHeight / 2
		);
	} else {
		//���j�b�g
		result.Set(
			GetActorData(idx).Location.X + GetActorData(idx).GraphSize.Width / 2,
			GetActorData(idx).Location.Y + GetActorData(idx).GraphSize.Height / 2
		);
	}
	return result;
}


///[summary]�G�t�F�N�g�Đ�
///[par]�G�t�F�N�gID
///[par]�ΏۃT�C�h (AttackSide)�A�P�̑Ώۂ̍s����null�ō\���܂���B
///[par]�A�C�e��/�X�L���̃f�[�^�x�[�X
///[par]�s�����e (SQActionData)
function PlayActionEffect(effectID, destSide, db, actData) {
	if(Game.GetKeyInputCounter(KeyID.A) > 0) {
		//���������Ă���Ƃ��͕\�����Ȃ�
		return;
	}

	if(0 <= actData.TargetIndex) {
		//�P��I�����͑Ώێ҂̃C���f�b�N�X����ΏۃT�C�h�������Ă���
		destSide = GetAttackSide(actData.TargetIndex);
	} else if(db != null) {
		switch(db.GetCell(actData.Option, "�Ώێ��").Integer) {
			case TargetType.AllParty:
			case TargetType.AllPartyWithDead:
				//�p�[�e�B�̓p�[�e�B�A���j�b�g�̓��j�b�g��ΏۂƂ���
				destSide = GetAttackSide(actData.ActorIndex);
				break;
			case TargetType.AllEnemy:
				//�p�[�e�B�̓��j�b�g�A���j�b�g�̓p�[�e�B��ΏۂƂ���
				destSide = GetAgainstAttackSide(GetAttackSide(actData.ActorIndex));
				break;
		}
	}

	local effectLocation = null;
	if(Game.DB.FDBEffect.GetSubDB(0).GetCell(effectID, "�`��Ώ�").Integer == DBEffectViewPosition.Each && actData.TargetIndex < 0) {
		//�S�̍U���ŃG�t�F�N�g���Ώیʂł���ꍇ�͈�l�ЂƂ�ɕ\������
		local effectHandle = -1;
		for(local i = GetFirstActorIndex(destSide); i != -1; i = GetNextActorIndex(i)) {
			effectLocation = GetEffectLocation(i, destSide);
			effectHandle = Game.DB.FDBEffect.Show(effectID, effectLocation.X, effectLocation.Y);
		}

		//�Ō�ɍĐ������G�t�F�N�g�̍Đ����I������܂ő҂�
		while(Game.DB.FDBEffect.GetEffectData(effectHandle).IsNull() == false
		&& Game.DB.FDBEffect.GetEffectData(effectHandle).WaitBreakFlag == false
		&& Game.Wait(0, true, true) == true) {
			//�����I���҂�
		}
	} else {
		//�P��G�t�F�N�g�\��
		effectLocation = GetEffectLocation(actData.TargetIndex, destSide);
		Game.DB.FDBEffect.ShowForWait(effectID, effectLocation.X, effectLocation.Y);
	}
}
