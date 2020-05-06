//###################################################################
//		�o�g���V�[��: �T�u���[�`��: �퓬�I���t�F�[�Y
//###################################################################

//�퓬�I���t�F�[�Y�̃T�u�֐�
PhaseEndingSubRoutine <- {

	///[summary]�������̃T�u���[�`��
	[BattleResult.Win] = function() {
		//���j�b�g�C�x���g: ������
		UnitEvent(UnitID, BattleEventType.EndingBattleWin, null);

		local enemyDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Enemy).GetSubDB(0);
		local wipeGraphFileName = Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.BattleEndingWipeGraph, "�t�@�C����").String;

		//�l���o���l�A���z�̎Z�o
		Results.EXP = 0;
		Results.Money = 0;
		foreach(enemy in Enemies) {
			if(enemy.SelfValues.Get(0x5486E688/*�����������ǂ���*/).Value == TRUE) {
				continue;		//���������G�l�~�[�͑ΏۊO�ƂȂ�
			}
			Results.EXP += enemyDB.GetCell(enemy.ID, "�o���l").Integer;
			Results.Money += enemyDB.GetCell(enemy.ID, "������").Integer;
		}

		//�������Z
		Game.DB.VDBParty.Money += Results.Money;

		//��ʂɔ��f
		ActionMessage("�퓬�����I", 0);
		Game.DB.FDBMaterial.Play(0x5F5E6646/*�퓬�I��*/, 255, 0, 0);
		Game.PrepareTransition();
		Depth = EndingDepth.EXPMoney;
		Phase = BattlePhase.Ending;
		Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
		Game.WaitKey(true);

		//�o���l���Z�A���x���A�b�v����
		for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
			local actor = GetActorData(i);
			if(IsDead(i) == true) {
				continue;		//����ł���L�����͑ΏۊO�ƂȂ�
			}

			local beforeLv = actor.Level;
			local lv = actor.PlusEXP(Results.EXP);		//���̒��Ń��x���A�b�v�������s����
			if(lv <= 0) {
				continue;		//���x���͏オ��Ȃ�����
			}

			//�ȍ~�A���x���A�b�v���̏���
			Results.LevelUpName = actor.Name;
			Results.BeforeLevel = beforeLv;
			Results.AfterLevel = actor.Level;

			//�p�����[�^�[�㏸���X�g
			Results.LevelUpParameters = array(actor.LevelUpParameters.Count());
			foreach(idx, par in Results.LevelUpParameters) {
				Results.LevelUpParameters[idx] = CIDValueData();
				Results.LevelUpParameters[idx].ID = actor.LevelUpParameters.GetID(idx);
				Results.LevelUpParameters[idx].Name = actor.LevelUpParameters.GetName(idx);
				Results.LevelUpParameters[idx].Value = actor.LevelUpParameters.GetValue(idx);
			}

			Game.DB.FDBMaterial.Stop(0x5F5E6646/*�퓬�I��*/);
			Game.DB.FDBMaterial.Stop(0x7706A0ED/*���x���A�b�v*/);
			Game.DB.FDBMaterial.Play(0x7706A0ED/*���x���A�b�v*/, 255, 0, 0);
			Game.PrepareTransition();
			Depth = EndingDepth.LevelUp;
			Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
			Game.WaitKey(true);

			//�K���X�L�����X�g
			Results.LevelUpSkills = array(actor.LevelUpSkills.Count());
			foreach(idx, par in Results.LevelUpSkills) {
				Results.LevelUpSkills[idx] = CIDNameData();
				Results.LevelUpSkills[idx].ID = actor.LevelUpSkills.GetID(idx);
				Results.LevelUpSkills[idx].Name = actor.LevelUpSkills.GetName(idx);
			}
			if(Results.LevelUpSkills.len() > 0) {
				//�K���X�L��������ꍇ�̂�
				Game.PrepareTransition();
				Depth = EndingDepth.LearnSkill;
				Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
				Game.WaitKey(true);
			}
		}

		//�E���A�C�e������
		local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
		Results.DropItems = array(0);
		foreach(enemy in Enemies) {
			local items = enemyDB.GetCell(enemy.ID, "�h���b�v�A�C�e��").IDValues;
			if(items.Count() == 0) {
				continue;		//�h���b�v�A�C�e�����Ȃ��ꍇ�̓X�L�b�v
			}

			//�f�[�^�̈�ԏォ�画�肷��
			local id = -1;
			for(local i = 0; i < items.Count(); i++) {
				if(Game.NextRand(1, 100) <= items.Get(i).Value) {
					//�m�肵���炻��ȍ~�̔�����s��Ȃ�
					id = items.Get(i).ID;
					break;
				}
			}
			if(id == -1) {
				continue;
			}

			//�E������
			local item = CIDNameData()
			item.ID = id;
			item.Name = itemDB.GetCell(id, itemDB.GetColumnName(DBDefaultColumnIndices.Name)).String;
			Results.DropItems.append(item);
			Game.DB.VDBParty.AddItem(id, 1);
		}
		if(Results.DropItems.len() > 0) {
			//�E�������ꍇ�̂�
			Game.PrepareTransition();
			Depth = EndingDepth.DropItem;
			Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
			Game.WaitKey(true);
		}

		Game.DB.FDBMaterial.Stop(0x5F5E6646/*�퓬�I��*/);
		Game.DB.FDBMaterial.Stop(0x7706A0ED/*���x���A�b�v*/);
	},


	///[summary]�s�k���̃T�u���[�`��
	[BattleResult.Lose] = function() {
		//���j�b�g�C�x���g: �s�k��
		UnitEvent(UnitID, BattleEventType.EndingBattleLose, null);

		ActionMessage(GetActorData(0).Name + "�� �S�ł��Ă��܂���...", 1.0);
		Game.Wait(5.0, false, true);
		Phase = BattlePhase.Ending;
		Game.FadeOut(SquareSpeed.High);
		if(DisabledGameOver == false) {
			GoGameOver();
		}
	},


	///[summary]�������̃T�u���[�`��
	[BattleResult.Escape] = function() {
		//���j�b�g�C�x���g: ������
		Phase = BattlePhase.Ending;
		UnitEvent(UnitID, BattleEventType.EndingBattleEscape, null);
	},


	///[summary]���f���̃T�u���[�`��
	[BattleResult.Break] = function() {
		//���ɏ������s��Ȃ�
		Phase = BattlePhase.Ending;
	},
};
