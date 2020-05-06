//###################################################################
//		�X�N���v�g���ʊ֐����`���܂��B
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\$MTFunctions.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBChar.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBClass.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBEnemy.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBItem.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBSkill.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBState.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBUnit.nut");


//####################  �O���[�o���ϐ�  ####################
WindowMTHandle <- -1;			//���b�Z�[�W�E�B���h�E��MT�n���h��
HPName <- Game.DB.VDBValue.GetSubDB(DBValueIndices.SelfActor).GetCell(0x7BAC0D68/*HP*/, "���O").String;
MPName <- Game.DB.VDBValue.GetSubDB(DBValueIndices.SelfActor).GetCell(0x46A20EE9/*MP*/, "���O").String;
EmotionSpriteIDList <- array(0);		//�G���[�V�����o���[���̃X�v���C�g���X�g


///[node]�f�o�b�O
///[summary]���O�o�͂��ĕԂ�
///[comment]�l�����O�ɏo������ł��̒l�����̂܂ܕԂ��܂��B
///[return]�����ɗ^�����l
function d(value) {
	print("" + value);
	return value;
}


///[node]�f�o�b�O
///[summary]���ׂẴA�C�e��������
///[par]���ꂼ��̃A�C�e���̏�����
///[comment]�A�C�e��DB�ɓo�^����Ă��邷�ׂẴA�C�e��������������Ԃɂ��܂��B
function TakeItemsAll(intCount) {
	print("���ׂẴA�C�e���� " + intCount + " �����������܂��B");
	local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	for(local i = 0; i < itemDB.GetRowCount(); i++) {
		Game.DB.VDBParty.AddItem(itemDB.GetCellFromIndex(i, itemDB.GetColumnIndex("FixedID")).Integer, intCount);
	}
}


///[node]�V�X�e��
///[summary]�v���C���Ԏ擾
///[comment]���݂̃v���C���Ԃ� [HH:MM:SS] �̏����ŕԂ��܂��B
///[return]���������ꂽ�o�ߎ���
function GetPlayTime() {
	local playTimeSecond = (Game.DXLib.GetNowCount() / 1000 - UI1874437940/*�N�����̎����b*/ + UI248120655/*�O��܂ł̃v���C����*/).tointeger();
	return format("%02d:%02d:%02d",
		playTimeSecond / 60 / 60, 	//����
		playTimeSecond / 60 % 60, 	//��
		playTimeSecond % 60);		//�b
}


///[node]�V�X�e��
///[summary]�Q�[�����Z�b�g
///[comment]�Q�[�����������Ƀ^�C�g����ʂɖ߂��܂��B�C�x���g���s���⃋�[�v�������ȂǁA�X�N���v�g�R�[�h�������\��������ӏ��ł͐�΂ɌĂяo���Ȃ��ŉ������B
function GameReset() {
	print("�Q�[�������Z�b�g���܂��B");
	Game.MT.Clear(false);
	Game.DrawBright.Set(255, 255, 255);
	Game.ForbidSceneDraw = false;
	Game.ForbidSceneInput = false;
	Game.Material.Sounds.StopAll(0, false);
	Game.ChangeToTitleScene();
}


///[node]�V�X�e��
///[summary]�}�b�vBGM��BGS�ꎞ��~
///[comment]�}�b�v��ʏ�ŗ���Ă���BGM��BGS���ꎞ��~���܂��B
function PauseMapBGMBGS() {
	if(Game.GetCurrentMainSceneID() != Scene.Map) {
		return;
	}

	if(Game.SceneMap.BGM.Handle != -1) {
		Game.Material.Sounds.Pause(Game.SceneMap.BGM.Handle, 1.0);
	}
	if(Game.SceneMap.BGS.Handle != -1) {
		Game.Material.Sounds.Pause(Game.SceneMap.BGS.Handle, 1.0);
	}
}


///[node]�V�X�e��
///[summary]�}�b�vBGM��BGS���t�ĊJ
///[comment]�}�b�v��ʏ�ŗ���Ă���BGM��BGS�̉��t���ĊJ���܂��B
function ResumeMapBGMBGS() {
	if(Game.GetCurrentMainSceneID() != Scene.Map) {
		return;
	}

	if(Game.SceneMap.BGM.Handle != -1) {
		Game.Material.Sounds.Resume(Game.SceneMap.BGM.Handle, true, 1.0);
	}
	if(Game.SceneMap.BGS.Handle != -1) {
		Game.Material.Sounds.Resume(Game.SceneMap.BGS.Handle, true, 1.0);
	}
}


///[node]�V�X�e��
///[summary]�}�b�v�ړ��㋤�ʏ���
///[comment]�}�b�v�ړ���̋��ʏ������s���܂��B
function DoAfterChangeMapProcedure() {
	Game.SceneMap.MapData.AfterLoadMap();

	if(UF24400435/*�}�b�v�ړ���Ƀt�F�[�h�C�����Ȃ�*/ == false) {
		Game.FadeIn(SquareSpeed.High);
	}

	UF1358433584/*�}�b�v�ړ����Ƀt�F�[�h�A�E�g���Ȃ�*/ = false;
	UF24400435/*�}�b�v�ړ���Ƀt�F�[�h�C�����Ȃ�*/ = false;
	StepCount = 0;

	//A*�@�̃A���S���Y��������������
	Astar.InitAstar();

	//�}�b�v���̃p�l������莞�ԕ\������
	Game.MT.AddThread("�}�b�v���p�l���\��", "MTMapNamePanel", Scene.Map, false)
		.AddValueInteger("starttime", Game.DXLib.GetNowCount());
}


///[node]�V�X�e��
///[summary]�ő�HP����эő�MP�𒴂��Ȃ��悤�Ɍʕϐ��𒲐�
///[par]�A�N�^�[�f�[�^
function FitToMaxParameters(actor) {
	if (actor == null) {
		return;
	}

	local maxHP = actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value;
	local currentHP = actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value;
	currentHP = Game.WithIn(currentHP, 0, maxHP);
	actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value = currentHP;

	local maxMP = actor.GetParameter(0x9A3FB31/*�ő�l�o*/).Value;
	local currentMP = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
	currentMP = Game.WithIn(currentMP, 0, maxMP);
	actor.SelfValues.Get(0x46A20EE9/*MP*/).Value = currentMP;
}


///[node]�`��
///[summary]�E�B���h�E�`��
///[comment]�w��̈�ɃE�B���h�E��`�悵�܂��B
///[par]�\���ʒuX���W
///[par]�\���ʒuY���W
///[par]�E�B���h�E��
///[par]�E�B���h�E����
///[return]�Ȃ�
function DrawWindow(intX, intY, intWidth, intHeight) {
	Game.DXLib.DrawWindow(
		Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*�V�X�e���O���t�B�b�N*/),
		intX, intY, intWidth, intHeight
	);
}


///[node]�`��
///[summary]�p�l���^�J�[�\���`��
///[comment]�w��ʒu�Ƀp�l���^�̃J�[�\����`�悵�܂��B
///[par]�\���ʒuX���W
///[par]�\���ʒuY���W
///[par]��
///[par]����
///[par]�A�N�e�B�u���ǂ���
///[return]�Ȃ�
function DrawCursorPanel(intX, intY, intWidth, intHeight, boolActive) {
	//�A�N�e�B�u�ȃJ�[�\���͕ω���t����
	if(boolActive == true) {
		Game.DXLib.SetDrawBlendMode(BlendMode.Add, 255 * fabs(sin(Game.DXLib.GetNowCount() / 250.0)));
	}

	Game.DXLib.DrawCursorPanel(
		Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*�V�X�e���O���t�B�b�N*/),
		intX, intY, intWidth, intHeight
	);

	if(boolActive == true) {
		Game.DXLib.RestoreDrawBlendMode();
	}
}


///[node]�`��
///[summary]���^�J�[�\���`��
///[comment]�w��ʒu�Ɏw������̖��J�[�\����`�悵�܂��B
///[par]�\���ʒuX���W
///[par]�\���ʒuY���W
///[par]�S����
///[return]�Ȃ�
function DrawCursorArrow(intX, intY, intDir4, boolActive) {
	if(boolActive == true) {
		//�A�N�e�B�u�ȃJ�[�\���͓_�ŕ\������
		if((Game.DXLib.GetNowCount() / 500 % 2) == 1) {
			return;
		}
	}

	Game.DXLib.DrawCursorArrow(
		Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*�V�X�e���O���t�B�b�N*/),
		intX, intY, intDir4
	);
}


///[node]�`��
///[summary]�c�X�N���[���o�[�`��
///[comment]�w��ʒu�ɏc�X�N���[���o�[��`�悵�܂��B
///[par]�\���ʒuX���W: �E�B���h�EX�{��
///[par]�\���ʒuY���W: �E�B���h�EY
///[par]�X�N���[���o�[����
///[par]�X�N���[���S��: �ő�v�f��
///[par]�X�N���[���̈�: �̈���ɂ���擪�̗v�f�ʒu
///[par]�X�N���[���̈�: �i�[�ł���v�f��
///[return]�Ȃ�
function DrawVScrollBar(intX, intY, intHeight, intMax, intTop, intAreaCount) {
	intX -= 35;		//�E�B���h�EX+�� ����̃I�t�Z�b�g
	if(intMax <= intAreaCount) {
		return;		//�X�N���[�����Ȃ��Ă��悢��
	}
	intTop = Game.WithIn(intTop, 0, intMax);		//�͈͓��֒���
	local nowY = intY + 32 + intTop * (intHeight - 64) / (intMax - intAreaCount);
	local hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*�V�X�e���O���t�B�b�N*/);

	//�㉺�����ɃX�N���[���ł��邱�Ƃ������A�C�R��
	Game.DXLib.DrawRectGraph(hdl, intX, intY + 16, 64 + 16, 16, 32, 8, true, false);
	Game.DXLib.DrawRectGraph(hdl, intX, intY + intHeight - 8 - 16, 64 + 16, 64 - 16 - 8, 32, 8, true, false);

	//�X�N���[���̌��݈ʒu�������A�C�R��
	Game.DXLib.DrawRectGraph(hdl, intX, nowY - 8, 64 + 16, 16, 32, 8, true, false);
	Game.DXLib.DrawRectGraph(hdl, intX, nowY, 64 + 16, 64 - 16 - 8, 32, 8, true, false);
}


///[node]�`��
///[summary]���[�^�[�P�F�`��
///[comment]�w��ʒu�ɒP�F�̃��[�^�[��`�悵�܂��B
///[par]�\���ʒuX���W
///[par]�\���ʒuY���W
///[par]��
///[par]����
///[par]�p�^�[���ԍ�
///[par]���[�^�[�̓h��Ԃ���
function DrawMeterSolid(intX, intY, intWidth, intHeight, intPattern, intEffectWidth) {
	DrawMeterPattern(intX, intY, intWidth, intHeight, intPattern, 0, intEffectWidth);
}


///[node]�`��
///[summary]���[�^�[�ϐF�`��
///[comment]�w��ʒu�Ɋ����ɉ����ĕϐF���郁�[�^�[��`�悵�܂��B
///[par]�\���ʒuX���W
///[par]�\���ʒuY���W
///[par]��
///[par]����
///[par]�p�^�[���J�n�ԍ�: ���̃p�^�[���ԍ������[�^�[�̊������ł��傫���Ƃ��̕\���Ɉ�v���܂��B
///[par]�p�^�[����
///[par]���[�^�[�̓h��Ԃ���
function DrawMeterPattern(intX, intY, intWidth, intHeight, intPatternStart, intPatternCount, intEffectWidth) {
	local hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x3C2F0B98/*���[�^�[�O���t�B�b�N*/);
	local MeterPatternOneWidth = Game.DXLib.GetGraphSize(hdl).Width;
	local MeterPatternOneHeight = 100;		//���[�^�[�p�^�[���O���t�B�b�N�ɂ����郁�[�^�[������̍���

	//���[�^�[�̏���𒴂��Ȃ��悤�ɂ���
	if(intEffectWidth > intWidth) {
		intEffectWidth = intWidth;
	}

	Game.DXLib.DrawRect(intX, intY, intWidth, intHeight, DARKGRAY, true, 1);		//�w�i
	Game.DXLib.DrawRect(intX, intY, intWidth, intHeight, WHITE, false, 1);			//�g
	Game.DXLib.DrawRectExtendGraph(
		hdl,
		intX + 1,
		intY + 1,
		intEffectWidth - 2,
		intHeight - 2,
		0,
		MeterPatternOneHeight * (intPatternStart + intPatternCount - intPatternCount * intEffectWidth / intWidth),
		MeterPatternOneWidth * intEffectWidth / intWidth,
		MeterPatternOneHeight,
		true
	);
}


///[node]����
///[summary]�����`�F�b�N/����L�[
///[comment]Z�L�[�AEnter�L�[�̂ǂ��炩�̉�����Ԓl���擾���܂��B
///[return]�ǂ��炩�̃L�[�̉�����Ԓl
function CheckDecideKeyInput() {
	return ((Game.GetKeyInputCounter(KeyID.Enter) > 0) ? Game.GetKeyInputCounter(KeyID.Enter) :
		((Game.GetKeyInputCounter(KeyID.Z) > 0) ? Game.GetKeyInputCounter(KeyID.Z) : 0));
}


///[node]����
///[summary]�����`�F�b�N/�L�����Z���L�[
///[comment]X�L�[�A�ECtrl�L�[�̂ǂ��炩�̉�����Ԓl���擾���܂��B
///[return]�ǂ��炩�̃L�[�̉�����Ԓl
function CheckCancelKeyInput() {
	return ((Game.GetKeyInputCounter(KeyID.RCtrl) > 0) ? Game.GetKeyInputCounter(KeyID.RCtrl) :
		((Game.GetKeyInputCounter(KeyID.X) > 0) ? Game.GetKeyInputCounter(KeyID.X) : 0)
	);
}


///[node]����
///[summary]�����`�F�b�N/��L�[
///[comment]��L�[��������Ă��邩�ǂ��������؂��܂��B
///[return]��������Ă��邩�ǂ���
function CheckUpKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Up) == 1
		|| (Game.GetKeyInputCounter(KeyID.Up) > 30 && Game.GetKeyInputCounter(KeyID.Up) % 2 == 0)
	);
}


///[node]����
///[summary]�����`�F�b�N/���L�[
///[comment]��L�[��������Ă��邩�ǂ��������؂��܂��B
///[return]��������Ă��邩�ǂ���
function CheckDownKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Down) == 1
		|| (Game.GetKeyInputCounter(KeyID.Down) > 30 && Game.GetKeyInputCounter(KeyID.Down) % 2 == 0)
	);
}


///[node]����
///[summary]�����`�F�b�N/�E�L�[
///[comment]��L�[��������Ă��邩�ǂ��������؂��܂��B
///[return]��������Ă��邩�ǂ���
function CheckRightKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Right) == 1
		|| (Game.GetKeyInputCounter(KeyID.Right) > 30 && Game.GetKeyInputCounter(KeyID.Right) % 2 == 0)
	);
}


///[node]����
///[summary]�����`�F�b�N/���L�[
///[comment]��L�[��������Ă��邩�ǂ��������؂��܂��B
///[return]��������Ă��邩�ǂ���
function CheckLeftKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Left) == 1
		|| (Game.GetKeyInputCounter(KeyID.Left) > 30 && Game.GetKeyInputCounter(KeyID.Left) % 2 == 0)
	);
}


///[node]����
///[summary]�Ԑڃt�F�[�h
///[par]�t�F�[�h�ϐ�������������֐�
///[par]�t�F�[�h�ϐ��𖈃t���[�����삷��֐��Btrue��Ԃ��܂ŏ������p�����܂��B
///[comment]�w�肵���֐���true��Ԃ��܂Ŗ��t���[�����s���āA�ԐړI�ȃt�F�[�h���s���܂��B���̊֐��͕K�����̓��\�b�h�̒�����Ăяo���ĉ������B
function DoFadeObject(funcFadeInit, funcFadeCount) {
	funcFadeInit();
	while(funcFadeCount() == false && Game.Wait(0, true, false)) {
		//�t�F�[�h�����I���҂�
	}
}


///[node]�C�x���g
///[summary]�I����
///[comment]�I������I�������܂��B
///[par]�I�����̃��X�g�B��؂�L���� | �ł��B
///[par]�L�����Z���ł���悤�ɂ��邩�ǂ���
///[return]�I�����ꂽ���ڂ̃C���f�b�N�X�B�L�����Z�����ꂽ�ꍇ�� -1 ���Ԃ���܂��B
function Choice(strChoiceList, boolCanCancel) {
	local list = split(strChoiceList, "|");
	if(strChoiceList == "") {
		return -1;		//�I���ł�����̂��Ȃ�
	}

	//�I��������郍�[�J���֐����`����
	local closeChoice = function(mt, list, boolCanceled) {
		local result = mt.GetValueInteger("Cursor");
		print(mt.GetValueInteger("Cursor") + ": " + (boolCanceled == true ? "�I�������L�����Z������܂����B" : "[" + list[mt.GetValueInteger("Cursor")] + "] ���I������܂����B"));
		mt.SetValueInteger("phase", 2);
		CloseMessage();		//���b�Z�[�W�E�B���h�E�������ɕ���
		return (boolCanceled == true) ? -1 : result;
	}

	//MT�o�^
	local mt = Game.MT.AddThread("�I����", "MTChoice", -1, false);
	mt.AddValueInteger("phase", 0);
	mt.AddValueInteger("trans", 0);
	mt.AddValueInteger("Cursor", 0);
	mt.AddValueString("msg", strChoiceList);

	//�t�F�[�h�C��
	while(mt.GetValueInteger("phase") != 1 && Game.Wait(0, true, false)) {
		//�t�F�[�h�����I���҂�
	}

	//���C�����[�`��
	while(Game.Wait(0, true, true)) {
		//���͏�Ԃ��`�F�b�N
		if(CheckUpKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.DownAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDownKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.UpAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDecideKeyInput() == 1) {
			//����
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);
			return closeChoice(mt, list, false);
		} else if(CheckCancelKeyInput() == 1 && boolCanCancel == true) {
			//�L�����Z��: �\�ȏꍇ�̂�
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, -1, 0, 0);
			mt.SetValueInteger("Cursor", -1);
			return closeChoice(mt, list, true);
		}
	}
}


///[node]�C�x���g
///[summary]�I����/�^�C�}�[�t��
///[comment]�I������I�������܂��B�w�莞�Ԃ��o�߂����ꍇ�A�I�����Ȃ��������ƂɂȂ�܂��B
///[par]�I�����̃��X�g�B��؂�L���� | �ł��B
///[par]��������/�~���b
///[return]�I�����ꂽ���ڂ̃C���f�b�N�X�B�I�����Ȃ������ꍇ�� -1 ���Ԃ���܂��B
function ChoiceWithTimer(strChoiceList, intWaitTimeMS) {
	local list = split(strChoiceList, "|");
	if(strChoiceList == "") {
		return -1;		//�I���ł�����̂��Ȃ�
	}

	//�I��������郍�[�J���֐����`����
	local closeChoice = function(mt, list, boolCanceled) {
		local result = mt.GetValueInteger("Cursor");
		print(mt.GetValueInteger("Cursor") + ": " + (boolCanceled == true ? "�I�������L�����Z������܂����B" : "[" + list[mt.GetValueInteger("Cursor")] + "] ���I������܂����B"));
		mt.SetValueInteger("phase", 2);
		CloseMessage();		//���b�Z�[�W�E�B���h�E�������ɕ���
		return (boolCanceled == true) ? -1 : result;
	}

	//MT�o�^
	local mt = Game.MT.AddThread("�I����", "MTChoice", -1, false);
	mt.AddValueInteger("phase", 0);
	mt.AddValueInteger("trans", 0);
	mt.AddValueInteger("Cursor", 0);
	mt.AddValueString("msg", strChoiceList);

	//�t�F�[�h�C��
	while(mt.GetValueInteger("phase") != 1 && Game.Wait(0, true, false)) {
		//�t�F�[�h�����I���҂�
	}

	//���C�����[�`��
	local startTime = Game.DXLib.GetNowCount();
	while(Game.Wait(0, true, true)) {
		//���͏�Ԃ��`�F�b�N
		if(Game.DXLib.GetNowCount() - startTime >= intWaitTimeMS) {
			//�^�C���A�b�v
			mt.SetValueInteger("Cursor", -1);
			return closeChoice(mt, list, true);
		} else if(CheckUpKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.DownAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDownKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.UpAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDecideKeyInput() == 1) {
			//����
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);
			return closeChoice(mt, list, false);
		}
	}
}


///[node]�C�x���g
///[summary]���b�Z�[�W
///[comment]�W���̃��b�Z�[�W�\���̖{�̃��\�b�h�ł��B�\�����Ƀ��Z�b�g�{�^����������Ă��Ή��ł��܂���B
///[par]�t�F�C�XID (�w�肵�Ȃ��ꍇ�� -1)
///[par]�b�Җ�
///[par]���b�Z�[�W���e
///[return]�Ȃ�
function ShowMessage(intFaceID, strSpeaker, strMsg) {
	//�\�����x���}�C�i�X�̂Ƃ��̓f�o�b�O�R���\�[���ւ̏o�͂ɂ���
	local msgSpeed = UI2084302691/*���b�Z�[�W�\�����x*/;
	if(msgSpeed < 0) {
		//���s�͏����āA���ł����s�ɂ���
		strMsg = Game.Replace(Game.Replace(strMsg, "\n", ""), "\f", "\n");
		print(strSpeeker + ":\n" + strMsg + "\n");
		return;
	}

	local withFace = (intFaceID <= -1) ? false : true;
	local rightFace = (intFaceID <= -1) ? false : Game.DB.FDBMaterial.GetSubDB(DBMaterialIndices.Face).GetCell(intFaceID, "���Α�").Boolean;

	//���ɃE�B���h�E��\�����Ă���ꍇ�͕���
	if(Game.MT.CountThread("MTMessage") > 0) {
		CloseMessage();
	}

	//MT�o�^
	local mt = Game.MT.AddThread("���b�Z�[�W", "MTMessage", -1, false);
	mt.AddValueInteger("isWaiting", FALSE);
	mt.AddValueInteger("phase", 0);
	mt.AddValueInteger("trans", 0);
	mt.AddValueInteger("faceID", intFaceID);
	mt.AddValueInteger("isRightFace", rightFace ? TRUE : FALSE);
	mt.AddValueInteger("offset", MsgTextOffset);
	mt.AddValueInteger("speakerOffset", 2);
	mt.AddValueString("speaker", strSpeaker);
	mt.AddValueString("msg", "");
	WindowMTHandle = mt.Handle;

	//�t�F�[�h�C��
	while(mt.GetValueInteger("phase") != 1 && Game.Wait(0, true, false)) {
		//�t�F�[�h�����I���҂�
	}

	//�p�����[�^�[������
	local showingLength = -1;		//�\���ʒu�݌v
	local point = 0;				//���ɂǂꂾ���i��ł��邩
	local lineIndex = 0;			//�c�ɂǂꂾ���i��ł��邩
	local maxLine = withFace ? 4 : 3;		//�P�œ�����̍ő�s��
	local maxPoint = withFace ? 17: 25;		//�P�s������̍ő啶����
	local newPageCnt = 0;					//���łŏグ��s��

	local showEnd = false;			//�Ō�܂ŕ\���������ǂ���
	local newPaging = false;		//���Œ��ł��邩�ǂ���
	local newPageStart = false;		//���Ŏ��̊����e�L�X�g�����グ�������s�t���O
	local newPageReserve = false;	//���ŗ\�� (���Œ��̉��ŗp�A���̃��[�v�ŕ�����i�߂Ȃ��悤�ɂ���t���O)

	local drawChar = "";			//���݂̍s�ŕ`�悵�Ă��镔���̕�����
	local oneChar = "";				//���ɕ`�悷�镶����

	local code = "";				//�X�N���v�g�R�[�h����
	local inCode = false;			//�X�N���v�g�R�[�h�����o���Ă��邩�ǂ���

	local lines = array(maxLine, "");		//�s���Ƃ̃��b�Z�[�W���e

	//�s���Ƃ̃��b�Z�[�W���e�����ۂ̕\���e�L�X�g�ɔ��f���郍�[�J���֐�
	local applyDrawText = function(mt, lines) {
		mt.SetValueString("msg", "");
		foreach(idx, line in lines) {
			mt.SetValueString("msg", mt.GetValueString("msg") + line);
			if(idx < line.len() - 1) {
				mt.SetValueString("msg", mt.GetValueString("msg") + "\n");
			}
		}
	}

	//�����̕������폜���郍�[�J���֐�: �����̐��䕶�����폜����̂Ɏg��
	local cutLastChar = function(text) {
		return Game.SubString(text, 0, Game.GetStringLength(text) - 1);
	}

	//���b�Z�[�W�\�����[�`��: �E�F�C�g�������������f���ꂽ�Ƃ��͏����S�̂𑦍��ɒ��f����
	while(true) {
		//���Œ�: �����e�L�X�g�����グ
		if(newPageStart == true) {
			mt.SetValueInteger("offset", mt.GetValueInteger("offset") - 5);
			if(mt.GetValueInteger("offset") > -27/*�`��萔*/ + MsgTextOffset) {
				if(Game.Wait(0, true, false) == false) {
					return;
				}
				continue;
			} else {
				//��s�������グ����ĊJ����
				newPageCnt--;
				mt.SetValueInteger("offset", MsgTextOffset);
				newPageStart = false;

				//��s��Ƀe�L�X�g���ڍs���A�Ō�̍s���󂯂�
				for(local line = 0; line < maxLine; line++) {
					if(line == maxLine - 1) {
						lines[line] = "";
					} else {
						lines[line] = lines[line + 1];
					}
				}

				if(newPageCnt < 0) {
					//���Ŋ���
					newPageCnt = 0;
					newPaging = false;
				}
				applyDrawText(mt, lines);		//�\���e�L�X�g�ɔ��f
				if(Game.Wait(0, true, false) == false) {
					return;
				}
			}
		}

		//�i�s��: ���̕����֐i��
		if(showEnd == false) {
			//���ł��\�񂳂�Ă��Ȃ���ΐi��
			if(newPageReserve == false) {
				showingLength++;
				point++;
				drawChar += Game.SubString(strMsg, showingLength, 1);
				if(drawChar != "") {
					//���ؗp�̕�����Ƃ��ăZ�b�g����
					oneChar = Game.SubString(drawChar, Game.GetStringLength(drawChar) - 1, 1);
				} else {
					oneChar = "";
				}
			}

			//���䕶�����ǂ��������؂���
			if(oneChar == "{") {
				//�R�[�h�J�n�_
				drawChar = cutLastChar(drawChar);		//�����̐��䕶��������
				code = "return (";
				inCode = true;
				continue;
			} else if(oneChar == "}" && inCode == true) {
				//�R�[�h�I�[
				code += ")";
				drawChar = cutLastChar(drawChar);		//�����̐��䕶��������
				inCode = false;

				//�R�[�h�����s���A����ꂽ����������݈ʒu�ɑ}������
				local temp = compilestring(code)().tostring();
				strMsg = Game.Insert(strMsg, showingLength + 1, temp);
			} else if(inCode == true) {
				//�R�[�h�ǂݎ��
				drawChar = cutLastChar(drawChar);		//�����̐��䕶��������
				code += oneChar;
				continue;
			} else if(oneChar == "\n" && lineIndex < maxLine - 1) {
				//�Ō�̍s���������s�L��
				drawChar = cutLastChar(drawChar);		//�����̐��䕶��������
				lineIndex++;
				point = 0;
				drawChar = "";
				newPageReserve = false;		//������x�������s�L����ǂ܂Ȃ�
			} else if(oneChar == "\f" || (oneChar == "\n" && lineIndex >= maxLine -1)) {
				//���Łi�Ō�̍s�ł̉��s���܂ށj
				drawChar = cutLastChar(drawChar);		//�����̐��䕶��������
				if(msgSpeed == 0) {
					//�u�ԕ\���̏ꍇ�͂����ŕ`�悷��
					if(Game.Wait(0, true, false) == false) {
						return;
					}
				}
				point = 0;
				drawChar = "";
				if(newPaging == false) {
					//���ł��s���Ă��Ȃ��ꍇ: ���[�U�[���͂�҂�
					mt.SetValueInteger("isWaiting", TRUE);
					Game.WaitKey(true);
					mt.SetValueInteger("isWaiting", FALSE);
					newPaging = true;
					newPageCnt = lineIndex;
					newPageReserve = false;

					if(lineIndex >= maxLine - 1) {
						//�Ō�̍s�ɂ���ꍇ: ��ɕő��肪�K�v
						newPageStart = true;
						mt.SetValueInteger("offset", MsgTextOffset);
					} else {
						//�Ō�̍s���܂��󂢂Ă���ꍇ: ��ɑ����̍s��i�߂�
						lineIndex++;
					}
				} else {
					//���Œ��ɉ��ł��悤�Ƃ����Ƃ�
					newPageReserve = true;		//��ł�����x�����ɋA���Ă���悤�ɂ���
					newPageStart = true;
					lineIndex--;
					continue;
				}
			} else if(oneChar == "\a") {
				//���f�L��: �E�B���h�E����Ȃ��ŏ����𒆒f����
				if(newPaging == true) {
					//���ł��I���܂ő҂�
					newPageReserve = true;		//��ł�����x�����ɋA���Ă���悤�ɂ���
					newPageStart = true;
					continue;
				}
				Game.Wait(0, true, false);
				return;
			} else {
				//��ʕ���: ���̂܂ܕ\��
				lines[lineIndex] = drawChar;
				applyDrawText(mt, lines);		//�\���e�L�X�g�ɔ��f
			}
		} else if(newPaging == true) {
			//�\�����I����Ă��āA���ł��܂��r���ł���ꍇ
			newPageStart = true;
		} else {
			//�\������: ���[�U�[���͂�҂t�F�[�Y
			mt.SetValueInteger("isWaiting", FALSE);		//�Ō�͉��ŗ\���L�����o�Ȃ��悤�ɂ���
			Game.WaitKey(true);
			CloseMessage();
			return;
		}

		if(showingLength + 1 >= Game.GetStringLength(strMsg)) {
			//�\�������𔻒�
			showEnd = true;
			if(msgSpeed == 0) {
				//�u�ԕ\���̏ꍇ�͂����ŕ`�悷��
				if(Game.Wait(0, true, false) == false) {
					return;
				}
			}
		}

		//���ۂ̕`�揈���A�\�����x�̒���
		if(msgSpeed > 0) {
			//�\�����x�� 1 �����邲�Ƃ� 1�t���[�������ҋ@���Ԃ�������
			if(Game.Wait(0.01 * msgSpeed, true, false) == false) {
				return;
			}
		}
	}
}


///[node]�C�x���g
///[summary]���b�Z�[�W/���t��
///[comment]�W���̃��b�Z�[�W�\���Ƀ{�C�X���悹�܂��B
///[par]�t�F�C�XID (�w�肵�Ȃ��ꍇ�� -1)
///[par]���[�U�[DB[�{�C�X]���ID
///[par]�b�Җ�
///[par]���b�Z�[�W���e
///[return]�Ȃ�
function ShowVoiceMessage(intFaceID, intVoiceID, strSpeaker, strMsg) {
	//�{�C�XID����T�E���h�t�@�C������T��
	local fileName = Game.DB.UDB.GetUserDB(0x49BEBA9A/*�{�C�X*/).GetCell(intVoiceID, "�t�@�C����").String;
	local hdl = Game.Material.Sounds.Add(fileName);
	Game.Material.Sounds.PlayOnce(hdl, PlayType.Back, -1, 0, 0);
	ShowMessage(intFaceID, strSpeaker, strMsg);
	Game.Material.Sounds.Delete(hdl);
}


///[node]�C�x���g
///[summary]���b�Z�[�W�����
///[comment]���ݕ\�����Ă��郁�b�Z�[�W�E�B���h�E����܂��B
///[return]�Ȃ�
function CloseMessage() {
	if(Game.MT.CountThread("���b�Z�[�W") == 0) {
		//�E�B���h�E���\������Ă��Ȃ��ꍇ�͏������Ȃ�
		WindowMTHandle = -1;
		return;
	} else if(WindowMTHandle == -1) {
		//�E�B���h�E���\������Ă��邪�n���h�����������Ă���ꍇ�͑�����
		Game.MT.DeleteThread("���b�Z�[�W");
		return;
	}

	local mt = Game.MT.GetThread(WindowMTHandle);

	//�t�F�[�h�A�E�g
	mt.SetValueInteger("phase", 2);
	while(Game.MT.CountThread("���b�Z�[�W") > 0 && Game.Wait(0, true, false)) {
		//�t�F�[�h�����I���҂�
	}
	WindowMTHandle = -1;
}


///[node]�C�x���g
///[summary]�Œ莮�e���b�v
///[comment]��ʂ̒����Ƀe���b�v��\�����܂��B
///[par]�\�����ԃ~���b
///[par]�L�[���͂Œ��f�ł��邩�ǂ���
///[par]�w�i��t���邩�ǂ��� (����ʂɕ\������ꍇ�� false ����)
///[par]�\���e���b�v
///[return]�Ȃ�
function ShowFixedSubtitles(intTimeMS, boolKeyBreak, boolEnabledBack, strMsg) {
	if(strMsg == "") {
		return;		//�\���ł���e�L�X�g���Ȃ�
	}

	//MT�o�^
	Game.MT.AddThread("�Œ莮�e���b�v", "MTFixedSubtitles", -1, false)
		.AddValueInteger("phase", 0)
		.AddValueInteger("trans", 0)
		.AddValueInteger("timeMS", intTimeMS)
		.AddValueInteger("canKeyBreak", boolKeyBreak.tointeger())
		.AddValueInteger("enabledBack", boolEnabledBack.tointeger())
		.AddValueInteger("starttime", -1)
		.AddValueString("msg", strMsg);

	//�e���b�v�\�����I���܂ő҂�
	while(Game.MT.CountThread("�Œ莮�e���b�v") > 0 && Game.Wait(0, true, true)) {
		//�����I���҂�
	}
}


///[node]�C�x���g
///[summary]�����e���b�v
///[comment]�������ɗ����e���b�v��\�����܂��B
///[par]�ړ����x
///[par]����L�[�ŃX�s�[�h�A�b�v�ł���悤�ɂ��邩�ǂ���
///[par]�e���b�v���e
///[return]�Ȃ�
function ShowFlowSubtitles(intFlowSpeed, boolCanSpeedUp, strMsg) {
	if(strMsg == "") {
		return;		//�\���ł���e�L�X�g���Ȃ�
	}

	//�p�����[�^�[������
	local LineHeight = 25;
	local allLineHeight = LineHeight;
	local list = list(strMsg, "\n");
	for(local line = 0; line < list.len(); line++) {
		allLineHeight += LineHeight;
	}

	//MT�o�^
	Game.MT.AddThread("�����e���b�v", "MTFlowSubtitles", -1, false)
		.AddValueInteger("canSpeedUp", boolCanSpeedUp.tointeger())
		.AddValueInteger("y", Game.WindowSize.Height + LineHeight)
		.AddValueInteger("allLineHeight", allLineHeight)
		.AddValueInteger("speed", intFlowSpeed)
		.AddValueString("msg", strMsg);

	//�e���b�v�\�����I���܂ő҂�
	while(Game.MT.CountThread("�����e���b�v") > 0 && Game.Wait(0, true, true)) {
		//�����I���҂�
	}
}


///[node]�C�x���g
///[summary]�G���[�V�����o���[��
///[comment]�t�L�_�V�A�C�R����\�����܂��B�����̐ݒ�ɒ��ӂ��ĉ������B
///[par]�ΏۃC�x���gID (�v���C���[�� -1)
///[par]�A�C�R���ԍ�X
///[par]�A�C�R���ԍ�Y
///[par]�\����҂��ǂ���
///[par]�\����҂ꍇ�̂�: �\���b��
///[par]�\����҂ꍇ�̂�: ���ׂẴt�L�_�V���������邩�ǂ���
function ShowEmotionBalloon(intEVID, intX, intY, boolWaitMode, intTime, boolDeleteAll) {
	local tileSize = Game.SceneMap.MapData.TileSize;

	//�ΏۃC�x���g�̉�ʏ�̈ʒu���擾����
	local pos = null;
	if(intEVID == -1) {
		pos = Game.SceneMap.MapData.Player.MapObject.PositionOnPixel;
	} else {
		pos = Game.SceneMap.MapData.EVGetData(intEVID).PositionOnPixel;
	}

	//�ΏۃC�x���g�̐^��ɃX�v���C�g���Z�b�g����
	local spid = Game.NextRand(INT_MIN, -2);
	Game.Sprite.AddLoadedGraphics(spid, Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x63E6D287/*�G���[�V�����A�C�R��*/), pos.X, pos.Y - tileSize);
	local sp = Game.Sprite.Get(spid);
	sp.GetGraphics(0).VisibleRange.Set(intX * tileSize, intY * tileSize, tileSize, tileSize);
	sp.WipeIn(SpriteWipeType.SlideDown, 20);
	EmotionSpriteIDList.append(spid);

	//�\����҂ꍇ
	if(boolWaitMode == true) {
		Game.Wait(0.5, true, false);
		Game.Wait(intTime, true, false);

		if(boolDeleteAll == true) {
			//�\������Ă��邷�ׂẴt�L�_�V����������ꍇ
			while(EmotionSpriteIDList.len() > 0) {
				local id = EmotionSpriteIDList.pop();
				Game.Sprite.Delete(id);
			}
		} else {
			//���o�����t�L�_�V����������
			EmotionSpriteIDList.pop();
			Game.Sprite.Delete(spid);
		}
	}
}


///[node]�C�x���g
///[summary]�h������
///[comment]�h���̃T�[�r�X���s���܂��B
///[par]�q�P��
///[par]���b�Z�[�W�̃t�F�C�XID (�w�肵�Ȃ��ꍇ�� -1)
///[par]���b�Z�[�W�̘b�Җ�
///[par]�I�[�v�j���O�g�[�N (������ $ �ƒu������)
///[par]���܂�O�̃��b�Z�[�W
///[par]���܂�����̃��b�Z�[�W
///[par]���܂�Ȃ������ꍇ�̃��b�Z�[�W
///[return]-1=���������肸���܂�Ȃ�����, 0=���܂�Ȃ�����, 1=���܂���
function Inn(intPricePerPerson, intFaceID, strSpeaker, strOpening, strBefore, strAfter, strNotStay) {
	local charge = intPricePerPerson * Game.DB.VDBParty.GetCharCount();
	strOpening = Game.Replace(strOpening, "$", charge.tostring());
	local result = 0;
	UF1222958890/*�������\��*/ = true;

	ShowMessage(intFaceID, strSpeaker, strOpening);

	switch(Choice("�͂�|������", true)) {
		case YesNoResult.Yes:
			//���z�`�F�b�N
			if(Game.DB.VDBParty.Money < charge) {
				//����������Ȃ�
				ShowMessage(-1, "", "����������܂���B");
				ShowMessage(intFaceID, strSpeaker, strNotStay);
				result = -1;
				break;
			}
			//���܂鏈��
			Game.DB.VDBParty.Money -= charge;
			ShowMessage(intFaceID, strSpeaker, strBefore);
			Game.FadeOut(SquareSpeed.High);
			Game.MT.SetSuspend("�}�b�v���p�l���\��", true);
			PauseMapBGMBGS();
			Game.Wait(1.0, true, false)
			Game.DB.FDBMaterial.PlayForWait(0x69C45725/*�h��*/, 255, 0, 0);
			HealPartyAll();
			ResumeMapBGMBGS();
			Game.FadeIn(SquareSpeed.High);
			Game.MT.SetSuspend("�}�b�v���p�l���\��", false);
			ShowMessage(intFaceID, strSpeaker, strAfter);
			result = 1;
			break;

		default:
			//���܂�Ȃ�
			ShowMessage(intFaceID, strSpeaker, strNotStay);
			result = 0;
			break;
	}

	UF1222958890/*�������\��*/ = false;

	return result;
}


///[node]���j���[
///[summary]���j���[��ʂ��J��
///[comment]���j���[��ʂ��Ăяo���܂��B�������I���܂őҋ@���܂��B
function StartMenu() {
	//�V�[�����甲����܂ő҂�
	Game.StartSubScene(Scene.Menu, true, false);
	SceneFadeIn();
	while(Game.CheckCurrentSceneID(Scene.Menu) == true
	&& Game.Wait(0, true, true)) {
		SceneSubInput();
	}
}


///[node]���j���[
///[summary]�Z�[�u���j���[���J��
///[comment]�Z�[�u�����ɕ��邱�Ƃ��ł��܂��B�������I���܂őҋ@���܂��B
function ShowSaveMenu() {
	Game.StartSubScene(Scene.Menu, true, false);
	OneMenuMode = true;
	Depth = 1;
	Cursor[0] = MenuItem.Save;
	SceneFadeIn();

	//�K�w���g�b�v�ɖ߂����܂ő҂�
	while(Depth >= 1
	&& Game.CheckCurrentSceneID(Scene.Menu) == true
	&& Game.Wait(0, true, true)) {
		SceneSubInput();
	}
}


///[node]���j���[
///[summary]�K�w��[�߂�
function MoveToNextDepth() {
	Depth++;
	Cursor[Depth] = 0;
	TopIndex[Depth] = 0;
}


///[node]�V���b�v
///[summary]�V���b�v��ʂ��J��
///[comment]�w���E���p���ł��邨�X���Ăяo���܂��B�������I���܂őҋ@���܂��B
function StartShop() {
	if(Game.SceneMap.ShopData.GetCount() == 0) {
		Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
		print("���i���������܂���B");
		return;
	}

	//�V�[�����甲����܂ő҂�
	Game.StartSubScene(Scene.Shop, true, false);
	while(Game.CheckCurrentSceneID(Scene.Shop) == true
	&& Game.Wait(0, true, true)) {
		SceneSubInput();
	}
}


///[node]�o�g��
///[summary]�퓬�J�n
///[comment]�o�g���V�[���Ɉڍs���܂��B
///[par]���j�b�gID
///[par]�퓬�w�i�t�@�C���� (����̂��̂��g�p����ꍇ��null)
///[par]�퓬BGM�t�@�C���� (����̂��̂��g�p����ꍇ��null)
///[par]�������Ȃ��퓬���ǂ���
///[par]�s�k���Ă����s���邩�ǂ���
///[return]�퓬����
function StartBattle(intUnitID, strBack, strBGM, boolDisabledEscape, boolDisabledGameOver) {
	Game.StartSubScene(Scene.Battle, false, false);
	InitBattle(intUnitID, strBack, strBGM, boolDisabledEscape, boolDisabledGameOver);

	//�V�[�����甲����܂ő҂�
	while(Game.CheckCurrentSceneID(Scene.Battle) == true
	&& Game.Wait(0, true, false)) {
		SceneSubInput();
	}

	if(IsBattleTestMode == true) {
		//�o�g���e�X�g���͑��I��
		return Result;
	}

	if(Result != BattleResult.Lose || boolDisabledGameOver == true) {
		//�Q�[���I�[�o�[���ȊO�̓}�b�v�֕��A
		SetHeadCharGraphics();		//�擪�҂̃O���t�B�b�N���Z�b�g
		ResumeMapBGMBGS();
		Game.Wait(1.0, true, false);
		Game.FadeIn(SquareSpeed.High);
	}

	return Result;
}


///[node]�o�g��
///[summary]�Q�[���I�[�o�[�ɂ���
///[comment]�Q�[���I�[�o�[��ʂ�\�����āA�^�C�g���V�[���Ɉڍs���܂��B
function GoGameOver() {
	local gmSHdl = Game.Material.Sounds.Add(Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.GameOverBGM, "�t�@�C����").String);
	Game.Material.Sounds.Play(gmSHdl, PlayType.Loop, 255, 0, 0, 0);

	//�S��ʂ̃X�v���C�g��\������
	Game.Sprite.AddWipeLoadedGraphics(
		0,
		Game.DB.FDBMaterial.GetSubDB(DBMaterialIndices.Graphics).GetCell(0x2EB7A344/*�Q�[���I�[�o�[�w�i*/, DB_ColumnNameMaterialHandle).Integer,
		0, 0, SpriteWipeType.Fade, Speed.High
	);
	Game.WaitKey(true);
	Game.FadeOut(SquareSpeed.High);

	//��Еt�����ă^�C�g���V�[����
	Game.EndSubScene();
	Game.Material.Sounds.Delete(gmSHdl);
	Game.Sprite.Delete(0);
	Game.ChangeToTitleScene();
}


///[node]�o�g��
///[summary]�A�N�^�[���擾
///[comment]�w�肵���C���f�b�N�X�̃A�N�^�[�����擾���܂��B�p�[�e�B�E���j�b�g�̗����ɑΉ����Ă��܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
///[return]�A�N�^�[��� (CDBActorData)
function GetActorData(idx) {
	if(idx < 0) {
		return null;
	} else if(idx < EnemyIndexOrigin) {
		//�p�[�e�B����擾
		return Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(idx));
	} else {
		//���j�b�g����擾
		return Enemies[idx - EnemyIndexOrigin];
	}
}


///[node]�o�g��
///[summary]���S����
///[comment]�w�肵���A�N�^�[������ł��邩�ǂ��������؂��܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
function IsDead(idx) {
	local actor = GetActorData(idx);
	return (
		actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value <= 0
		|| actor.SelfValues.Get(0x5486E688/*�����������ǂ���*/).Value == TRUE
		|| actor.CheckStateLimit(StateLimit.KO) == true
	);
}


///[node]�o�g��
///[summary]�s���s�\����
///[comment]�w�肵���A�N�^�[���s���ł��Ȃ���Ԃł��邩�ǂ��������؂��܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
function IsForbiddenAction(idx) {
	local actor = GetActorData(idx);
	return (
		IsDead(idx) == true
		|| actor.CheckStateLimit(StateLimit.ForbiddenAction) == true
	);
}


///[node]�o�g��
///[summary]��{�p�����[�^�[����
///[comment]�w�肵���p�����[�^�[�̒l���J���X�g�l���l�����Ȃ��瑝�����܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
///[par]�p�����[�^�[ID
///[par]�����l
function PlusActorBaseParameter(idx, parID, val) {
	local data = GetActorData(idx).GetParameter(parID);
	data.Value += val;
	data.Value = Game.WithIn(data.Value, 0, MaxBaseParameter);
}


///[node]�o�g��
///[summary]�g���p�����[�^�[����
///[comment]�w�肵���A�N�^�[�ʕϐ��̒l���J���X�g�l���l�����Ȃ��瑝�����܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
///[par]�g���p�����[�^�[�񋓑̃����o�[
///[par]�����l
///[return]���ۂɑ��������l
function PlusActorExParameter(idx, exPar, val) {
	local valID = -1;
	local maxVal = MaxExParameter;
	switch(exPar) {
		case ExParameter.HP:
			valID = 0x7BAC0D68/*HP*/;
			maxVal = GetActorData(idx).GetParameter(0x2019EC10/*�ő�g�o*/).Value;
			break;
		case ExParameter.MP:
			valID = 0x46A20EE9/*MP*/;
			maxVal = GetActorData(idx).GetParameter(0x9A3FB31/*�ő�l�o*/).Value;
			break;
	}
	local data = GetActorData(idx).SelfValues.Get(valID);
	local oldValue = data.Value;
	data.Value += val;
	data.Value = Game.WithIn(data.Value, 0, maxVal);
	return data.Value - oldValue;
}


///[node]�o�g��
///[summary]�S��/�p�[�e�B�S��
///[comment]�p�[�e�B�S����S�񕜂����܂��B
function HealPartyAll() {
	for(local i = 0 ; i < Game.DB.VDBParty.GetCharCount(); i++) {
		HealActor(i);
	}
}


///[node]�o�g��
///[summary]�S��/�A�N�^�[
///[comment]�w�肵���A�N�^�[��S�񕜂����܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
function HealActor(idx) {
	local actor = GetActorData(idx);
	actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value = actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value;
	actor.SelfValues.Get(0x46A20EE9/*MP*/).Value = actor.GetParameter(0x9A3FB31/*�ő�l�o*/).Value;
	actor.ClearStateOnlyBad();
}


///[node]�o�g��
///[summary]HP��/�A�N�^�[
///[comment]�w�肵���A�N�^�[��HP���񕜂����܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
///[par]�񕜗�
///[par]���b�Z�[�W�\�����s�����ǂ���
///[return]�񕜗�
function HPHeal(idx, intValue, boolEnabledMsg) {
	//���l�\��
	if(Game.GetCurrentSubSceneID() == Scene.Battle && intValue < 0) {
		//�_���[�W�̏ꍇ
		HPDamage(idx, -intValue, boolEnabledMsg);
		return;
	}

	//�p�����[�^�[����
	local delta = PlusActorExParameter(idx, ExParameter.HP, intValue);

	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		if(delta != 0) {
			DamageEffectTasks.append(SQDamageEffectData(idx, delta, SQDamageEffectData.DamageEffectType.HPHeal));

			if(boolEnabledMsg == true) {
				//���b�Z�[�W�\��
				local actor = GetActorData(idx);
				ActionMessage(actor.Name + " �� " + HPName + "�� " + delta + " �񕜂����I", 0.5);
			}
		} else {
			//�~�X
			DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));

			if(boolEnabledMsg == true) {
				//���b�Z�[�W�\��
				local actor = GetActorData(idx);
				ActionMessage("������ �Ȃɂ� �N����Ȃ������I", 0.5);
			}
		}
	}

	return delta;
}


///[node]�o�g��
///[summary]HP��/�T�C�h�S��
///[comment]�w�肵���T�C�h�̃A�N�^�[�S����HP���񕜂����܂��B
///[par]�s�����e
///[par]�ΏۃT�C�h
///[par]�񕜗ʂ̊�{�l
///[par]�񕜗ʂ̕��U�x
///[return]�񕜗ʂ̕��ϒl
function HPHealSideAll(actData, targetSide, intBase, intVariance) {
	local damage = 0;
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		local target = GetActorData(i);
		damage += HPHeal(i, CalcHeal(actor, target, intBase, intVariance), false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//�ꊇ���b�Z�[�W�\��
	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		local headName = GetHeadActorName(targetSide);
		if (damage != 0) {
			ActionMessage(headName + " �� " + HPName + "�� " + (actors.len() <= 1 ? "" : "���� ") + damage + " �񕜂����I", 0.5);
		} else {
			ActionMessage("������ �Ȃɂ� �N����Ȃ������I", 0.5);
		}
	}
	return damage;
}


///[node]�o�g��
///[summary]MP��/�A�N�^�[
///[comment]�w�肵���A�N�^�[��MP���񕜂����܂��B
///[par]�A�N�^�[�̃C���f�b�N�X�i���j�b�g�� EnemyIndexOrigin ����n�܂�j
///[par]�񕜗�
///[par]���b�Z�[�W�\�����s�����ǂ���
///[return]�񕜗�
function MPHeal(idx, intValue, boolEnabledMsg) {
	//���l�\��
	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		if(intValue > 0) {
			DamageEffectTasks.append(SQDamageEffectData(idx, intValue, SQDamageEffectData.DamageEffectType.MPHeal));
		} else if(intValue < 0) {
			//�_���[�W�̏ꍇ
			MPDamage(idx, -intValue, boolEnabledMsg);
			return;
		} else {
			//�~�X
			DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
		}
	}

	//�p�����[�^�[����
	PlusActorExParameter(idx, ExParameter.MP, intValue);

	//���b�Z�[�W�\��
	if(Game.GetCurrentSubSceneID() == Scene.Battle && boolEnabledMsg == true) {
		local actor = GetActorData(idx);
		ActionMessage(actor.Name + " �� " + MPName + "�� " + intValue + " �񕜂����I", 0.5);
	}
	return intValue;
}


///[node]�o�g��
///[summary]MP��/�T�C�h�S��
///[comment]�w�肵���T�C�h�̃A�N�^�[�S����MP���񕜂����܂��B
///[par]�s�����e
///[par]�ΏۃT�C�h
///[par]�񕜗ʂ̊�{�l
///[par]�񕜗ʂ̕��U�x
function MPHealSideAll(actData, targetSide, intBase, intVariance) {
	local damage = 0;
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		local target = GetActorData(i);
		damage += MPHeal(i, CalcHeal(actor, target, intBase, intVariance), false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//�ꊇ���b�Z�[�W�\��
	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		local headName = GetHeadActorName(targetSide);
		ActionMessage(headName + " �� " + MPName + "�� " + (actors.len() <= 1 ? "" : "���� ") + damage + " �񕜂����I", 0.5);
	}
	return damage;
}
