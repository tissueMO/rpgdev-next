//###################################################################
//		�}�b�v�V�[��
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\!SceneMainMap_var.nut");

//####################  �V�[���N��  ####################
function SceneMainSetup() {
	SetHeadCharGraphics();		//�擪�҂̃O���t�B�b�N���Z�b�g
	IsWaitingForFirstFrame = true;
}


//####################  �V�[���`��  ####################
function SceneMainDraw() {
	//�}�b�v�̕W���`��
	Game.SceneMap.MapData.DrawMap(60);

	//�������\��
	if(UF1222958890/*�������\��*/ == true) {
		DrawWindow(Game.WindowSize.Width - 180, 0, 180, 35);
		Game.DXLib.DrawString(Game.WindowSize.Width - 140, 10, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE,
			format("%10d ", Game.DB.VDBParty.Money) + US1246572451/*�ʉݖ���*/
		);
	}

	//�f�o�b�O�p�\��
	if(DEBUG && UF358059746/*$�f�o�b�O�p�K�C�h�\��*/ == true) {
		Game.DXLib.DrawStringR(Game.WindowSize.Width, 0, -1, WHITE,
			"[A]�_�b�V�� [S]���蔲�� [X/Ctrl]���j���[�J��\n[F3]�}�b�v�ēǍ� [F4]�r�p�ēǍ� [F5]�f�o�b�O\n[F8]���\�[�X�g�p�� [F11]�|�[�Y [F12]���Z�b�g"
		);
	}
}


//####################  �V�[������  ####################
function SceneMainInput() {
	if(IsWaitingForFirstFrame) {
		// �}�b�v�ړ����� 1�t���[�� �̊Ԃ́A�����N���C�x���g����҂��̂��߁A�V�[�����͂��󂯕t���Ȃ�
		IsWaitingForFirstFrame = false;
		return;
	}

	local mapData = Game.SceneMap.MapData;
	local noEncounterFlag = false;

	if(mapData.IsEVRunning == true || mapData.EVReservedCount() > 0) {
		return;		//�C�x���g���s��or�\�񒆂͑���ł��Ȃ�
	} else if(mapData.Player.MapObject.IsMoving == true) {
		return;		//�ړ����͑���ł��Ȃ�
	}

	//�f�o�b�O���̂ݗL���ȑ���
	if(DEBUG == TRUE) {
		//�����蔻�薳��
		mapData.Player.CharData.NoHit = (Game.GetKeyInputCounter(KeyID.S) > 0);

		//�G���J�E���g���薳����
		noEncounterFlag = mapData.Player.CharData.NoHit;

		//�f�o�b�O�p�R�}���h�\���ؑ�
		if(Game.GetKeyInputCounter(KeyID.F8) == 1) {
			UF358059746/*$�f�o�b�O�p�K�C�h�\��*/ = !UF358059746/*$�f�o�b�O�p�K�C�h�\��*/;
		}
	}

	//��������I���������̏���
	if(MovingFlag == true) {
		MovingFlag = false;

		//�}�b�v��ŗL���ȊԐڌ��ʂ̏���
		StateMapPartyAll();

		//�����_���G���J�E���g����
		if(noEncounterFlag == false) {
			local minStepCount = (NormalEncounterStepCount / mapData.EncounterRate.tofloat() * 100).tointeger();
			if(StepCount >= minStepCount
			&& mapData.Units.Count() > 0
			&& Game.NextRand(1, 100) <= (StepCount - minStepCount)) {
				//�G���J�E���g����: �ǂ̃��j�b�g�Ƒ����������������_���Ɍ��肷��
				local unitID = mapData.Units.Get(Game.NextRand(0, mapData.Units.Count() - 1));
				print(StepCount + " ���� �G���J�E���g [ID: " + unitID + "] ���������܂����B");
				StepCount = 0;
				StartBattle(unitID, null, null, false, false);
				return;
			}
		}
	}

	if(CheckDecideKeyInput() == 1) {
		//����L�[�ŋN������C�x���g�͂����Ŕ��肷��
		mapData.CheckPressEnterTriggerEV();
		return;
	} else if(CheckCancelKeyInput() == 1) {
		//���j���[��ʂ��J��
		Game.DB.FDBMaterial.Play(IntoMenuSEID, 255, 0, 0);
		StartMenu();
		return;
	} else if(Game.GetKeyInputCounter(KeyID.F12) == 1) {
		//���Z�b�g
		GameReset();
		return;
	} else {
		//�V�K�ړ�: ��ԍŌ�ɉ����ꂽ�����L�[������
		local dir = Direction4.None;
		local cnt = INT_MAX;
		if(Game.GetKeyInputCounter(KeyID.Up) > 0 && Game.GetKeyInputCounter(KeyID.Up) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Up);
			dir = Direction4.North;
		}
		if(Game.GetKeyInputCounter(KeyID.Down) > 0 && Game.GetKeyInputCounter(KeyID.Down) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Down);
			dir = Direction4.South;
		}
		if(Game.GetKeyInputCounter(KeyID.Left) > 0 && Game.GetKeyInputCounter(KeyID.Left) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Left);
			dir = Direction4.West;
		}
		if(Game.GetKeyInputCounter(KeyID.Right) > 0 && Game.GetKeyInputCounter(KeyID.Right) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Right);
			dir = Direction4.East;
		}

		if(dir == Direction4.None) {
			return;		//�������͂��Ȃ�����
		}

		//�_�b�V������
		local dash = (Game.GetKeyInputCounter(KeyID.A) > 0);
		if(dash == false) {
			mapData.Player.CharData.MoveSpeed = mapData.MoveSpeed;
		} else {
			mapData.Player.CharData.MoveSpeed = mapData.MoveSpeed + 1;
		}

		//�ړ�����������: �ړ��ł��邩�ǂ����̔�����s���A�����œ����ɐڐG�C�x���g�̔�����s����
		mapData.Player.CharData.Direction = dir;
		if(mapData.CheckBeforeMove(-1, mapData.Player.MapObject.Position.X, mapData.Player.MapObject.Position.Y, dir, true) == false) {
			return;		//�ړ��ł��Ȃ�
		}

		//�ړ�����
		StepCount++;
		MovingFlag = true;
		mapData.Player.MapObject.AutoSetMoveCounter(dir);
		switch(dir) {
			case Direction4.North:
				mapData.Player.MapObject.Position.Y -= 1;
				break;
			case Direction4.South:
				mapData.Player.MapObject.Position.Y += 1;
				break;
			case Direction4.West:
				mapData.Player.MapObject.Position.X -= 1;
				break;
			case Direction4.East:
				mapData.Player.MapObject.Position.X += 1;
				break;
		}

		//�}�b�v�͈̔͊O�ɏo����}�b�v�̃��[�v�ݒ�ɏ]���ĕ␳����
		mapData.Player.MapObject.ApplyMapLoopPosition();
	}

	//��q�̏���
	if(mapData.CheckTileGroupNum(mapData.Player.MapObject.Position.X, mapData.Player.MapObject.Position.Y, LadderTileGroupNum) != -1) {
		//���݂̃^�C������q�ł���ꍇ�A������k�����ɌŒ肷��
		mapData.Player.CharData.FixedDirection = false;
		mapData.Player.CharData.Direction = Direction4.North;		//�����̌Œ����U�������Ȃ��ƓK�p����Ȃ�
		mapData.Player.CharData.FixedDirection = true;
	} else {
		mapData.Player.CharData.FixedDirection = false;
	}
}


//####################  �V�[���j��  ####################
function SceneDispose() {
}


//####################  �擪�҂̕��s�O���t�B�b�N��ݒ�  ####################
function SetHeadCharGraphics() {
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == true) {
			continue;		//����ł���L�����͕\�����Ȃ�
		}
		Game.SceneMap.MapData.Player.CharData.LoadGraphics(
			Game.DB.GetDB(DBType.FDB, FDBIndices.Char).GetSubDB(0).GetCell(Game.DB.VDBParty.GetCharIDFromPartyIndex(i), "�O���t�B�b�N").String
		);
		return;
	}

	//�f�o�b�O��: �S������ł���ꍇ�A�֋X�I�Ɉ�ԑO�̃L�����ɂ���
	if(DEBUG) {
		Game.SceneMap.MapData.Player.CharData.LoadGraphics(
			Game.DB.GetDB(DBType.FDB, FDBIndices.Char).GetSubDB(0).GetCell(Game.DB.VDBParty.GetCharIDFromPartyIndex(0), "�O���t�B�b�N").String
		);
	} else {
		Game.SceneMap.MapData.Player.CharData.LoadGraphics("");
	}
}


//####################  MT���s: �}�b�v���\���p�l��  ####################
//["starttime"] = �J�n���̎���
function MTMapNamePanel(intHandle) {
	local delta = Game.DXLib.GetNowCount() - Game.MT.GetThread(intHandle).GetValueInteger("starttime");
	local offset = 0;

	if(delta < MapNameFadeTime) {
		//�t�F�[�h�C���t�F�[�Y
		Game.DXLib.SetDrawBlendMode(
			BlendMode.Alpha,
			delta * 255 / MapNameFadeTime
		);
		offset = -(200 / 2) + (delta * (200 / 2) / MapNameFadeTime);
	} else if(delta > MapNamePanelTime + MapNameFadeTime) {
		//�t�F�[�h�A�E�g�t�F�[�Y
		Game.DXLib.SetDrawBlendMode(
			BlendMode.Alpha,
			255 - ((delta - (MapNamePanelTime + MapNameFadeTime)) * 255 / MapNameFadeTime)
		);
		if(delta > MapNamePanelTime + MapNameFadeTime * 2) {
			return true;		//�X���b�h�I��
		}
		offset = -(delta - (MapNamePanelTime + MapNameFadeTime)) * (200 / 2) / MapNameFadeTime;
	}

	DrawWindow(offset, 0, 230, 35);
	Game.DXLib.DrawStringC(offset + 230 / 2, 10, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, Game.SceneMap.MapData.CurrentMapName);
	Game.DXLib.RestoreDrawBlendMode();

	return false;		//�X���b�h�p��
}
