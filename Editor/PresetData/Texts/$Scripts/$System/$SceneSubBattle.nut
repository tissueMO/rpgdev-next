//###################################################################
//		�o�g���V�[��
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\!var.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Common.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Draw.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\MainRoutine.nut");

//####################  �V�[���N��  ####################
function SceneSubSetup() {
	Phase = BattlePhase.Setup;
	Game.PrepareTransition();
	Game.DoTransition(
		Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.BattleStartWipeGraph, "�t�@�C����").String,
		SquareSpeed.High
	);
	TurnCount = 1;
}


//####################  �V�[���`��  ####################
function SceneSubDraw() {
	if(Phase == BattlePhase.Setup) {
		return;		//�������͕`�悵�Ȃ�
	}
	BattleDraw();
}


//####################  �V�[������  ####################
function SceneSubInput() {
	BattleInput();
}


//####################  �V�[���j��  ####################
function SceneSubDispose() {
	//�G�l�~�[�O���t�B�b�N�j��
	foreach(enemy in Enemies) {
		enemy.DisposeGraphics();
	}
	print("�퓬���I�����܂��B");
}


//####################  �V�[�������������ĊJ�n  ####################
function InitBattle(intUnitID, strBack, strBGM, boolDisabledEscape, boolDisabledGameOver) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Unit).GetSubDB(0);

	if (db.GetCell(intUnitID, "ID").IsNull() == true) {
		//���j�b�gID�����݂��Ȃ��ꍇ�̓G���[�ŗ��Ƃ�
		print("�w�肳�ꂽ���j�b�gID�����݂��Ȃ����߁A�퓬���J�n�ł��܂���ł����BUnitID=" + intUnitID);
		Game.FadeOut(SquareSpeed.High);
		Game.EndSubScene();
		return;
	}

	local src = db.GetCell(intUnitID, "�z�u���").String;
	local enemyList = split(src, "|");

	//�������O���[�o���ϐ��ɑ������
	UnitID = intUnitID;
	FileBack = strBack;
	FileBGM = strBGM;
	DisabledEscape = boolDisabledEscape;
	DisabledGameOver = boolDisabledGameOver;

	//���j�b�g�̌ʕϐ��f�[�^�������l�ŃZ�b�g����
	UnitSelfValues = Game.DB.VDBValue.GetSelfValues(DBValueIndices.SelfUnit);

	//�G�l�~�[���A�N�^�[�Ƃ��Đ���
	Enemies.resize(0);
	foreach(enemy in enemyList) {
		local spl = "";
		spl = split(enemy, ";");		// [ID], [X:Y] �ɕ�������
		local enemyData = CDBActorData.CreateActorFromEnemy(spl[0].tointeger());
		spl = split(spl[1], ":");
		enemyData.Location.Set(spl[0].tointeger(), spl[1].tointeger());
		enemyData.LoadGraphics(enemyData.Graphics);

		//�p�����[�^�[�ݒ�
		enemyData.SelfValues.Get(0x7BAC0D68/*HP*/).Value = enemyData.GetParameter(0x2019EC10/*�ő�g�o*/).Value;
		enemyData.SelfValues.Get(0x7BD2A434/*�h�䒆���ǂ���*/).Value = FALSE;

		Enemies.append(enemyData);
		DamageMotionUT.append(SQShakeData());
	}

	//����ID�̃G�l�~�[�Ƀi���o�����O����
	foreach(idx, enemy in Enemies) {
		if(enemy.SelfValues.Get(0x725E9664/*�i���o�����O����*/).Value == TRUE) {
			continue;		//�i���o�����O�ς݂̃G�l�~�[�̓X�L�b�v
		}

		//����ID�̃G�l�~�[���𐔂���
		local sames = array(0);
		foreach(subEnemy in Enemies) {
			if(subEnemy.Name == enemy.Name) {
				sames.append(subEnemy);
			}
		}
		if(sames.len() <= 1) {
			//�B��̃G�l�~�[�ɂ̓i���o�����O���Ȃ�
			enemy.SelfValues.Get(0x725E9664/*�i���o�����O����*/).Value = TRUE;
			continue;
		}

		//�Y�������G�l�~�[�ɑ΂��ăi���o�����O����
		foreach(sub, subEnemy in sames) {
			subEnemy.Name = subEnemy.Name + Game.IntToAlphabet(sub);
			subEnemy.SelfValues.Get(0x725E9664/*�i���o�����O����*/).Value = TRUE;
		}
	}

	//�p�[�e�B�ɑ΂��鏈��
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = GetActorData(i);
		actor.Location.Set(
			Game.WindowSize.Width / Game.DB.FDBSystem.GetInteger(DB_System_MaxPartyCount) * i,
			Game.WindowSize.Height - CommentPanelHeight - PartyPanelHeight
		);
		actor.SelfValues.Get(0x7BD2A434/*�h�䒆���ǂ���*/).Value = FALSE;
		PTPanelShakes.append(SQShakeData());
	}

	//�퓬�w�i�O���t�B�b�N�̓ǂݍ���
	if(FileBack == null || FileBack.len() == 0) {
		//�W���̐퓬�w�i���g��
		HdlBack = Game.Material.Graphics.Add(Game.SceneMap.MapData.BattleBackFileName);
	} else {
		HdlBack = Game.Material.Graphics.Add(FileBack);
	}

	//�퓬BGM�̍Đ�
	if(Game.BGMKeep == false) {
		PauseMapBGMBGS();
		Game.Wait(0.75, true, false);

		if(FileBGM == null || FileBGM.len() == 0) {
			//�W���̐퓬BGM���g��
			HdlBGM = Game.Material.Sounds.Add(Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.BattleBGM, "�t�@�C����").String);
		} else {
			HdlBGM = Game.Material.Sounds.Add(FileBGM)
		}
		Game.Material.Sounds.Play(HdlBGM, PlayType.Loop, 255, 0, 0, 0);
	}

	Phase = BattlePhase.Start;
	Game.FadeIn(SquareSpeed.High);

	//�f�o�b�O�p�o��
	print("���j�b�g�ꗗ:");
	foreach(idx, enemy in Enemies) {
		print("\t" + idx.tostring() + ": " + enemy.ID.tostring() + " [" + enemy.Name + "] (" + enemy.Location.X.tostring() + "," + enemy.Location.Y.tostring() + ")");
	}
	print("�퓬���J�n����܂����B");

	//���j�b�g�C�x���g: �퓬�J�n��
	UnitEvent(UnitID, BattleEventType.StartBattle, null);
}
