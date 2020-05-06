//###################################################################
//		�Q�[���v���O�����̈ꕔ�ɂ��Ĉϑ����󂯂ď������܂��B
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\EVMoveExtend.nut");


//�o�g���e�X�g���J�n����
IsBattleTestMode <- false;
function/*void*/ EventHandler_StartBattleTest(intUnitID) {
	IsBattleTestMode = true;
	HealPartyAll();
	TakeItemsAll(ItemCountForTester);
	StartBattle(
		intUnitID,
		Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.BattleTestGraph, "�t�@�C����").String,
		null,		//�W����BGM
		false,		//��������
		false		//��������Q�[���I�[�o�[
	);
	Game.ApplicationExit(ExitCode.Success);
}


//�}�b�v�e�X�g���J�n����
function/*void*/ EventHandler_StartMapTest(strMapFileName, intX, intY) {
	HealPartyAll();
	TakeItemsAll(ItemCountForTester);
	UF1358433584/*�}�b�v�ړ����Ƀt�F�[�h�A�E�g���Ȃ�*/ = true;
	UI1874437940/*�N�����̎����b*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();
	EventHandler_ChangeMap(strMapFileName, intX, intY, Direction4.South);
}


//�f�[�^�ǂݍ��ݒ��ɍs���`�揈��
MarqueeValue <- 400;		//�}�[�L�[�ʒu
function/*void*/ EventHandler_NowLoading(intGraphHandle) {
	Game.DXLib.DrawRectGraph(intGraphHandle, 0, 0, 0, 0, Game.WindowSize.Width, Game.WindowSize.Height, false, false);

	//�}�[�L�[��`��
	MarqueeValue = Game.UpAround(MarqueeValue + SquareSpeed.Normal, MarqueeMinimum, MarqueeMaximum);
	Game.DXLib.SetDrawArea(120, 203, 120 + 400, 203 + 16);
	Game.DXLib.DrawRectGraph(intGraphHandle, 120 + MarqueeValue, 203, 0, 480, 640, 16, true, false);
	Game.DXLib.ResetDrawArea();
}


//�X�v���C�g���C�v����������
function/*void*/ EventHandler_WipeSprite(spriteData) {
	local SlideOffset = 50;		//�X���C�h��

	switch(spriteData.WipeType) {
		case SpriteWipeType.Fade:		//�t�F�[�h
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				spriteData.GetGraphics(i).Draw(CPoint());
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideRight:		//�X���C�h: �E
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.X = (SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideLeft:		//�X���C�h: ��
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.X = -(SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideDown:		//�X���C�h: ��
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.Y = (SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideUp:		//�X���C�h: ��
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.Y = -(SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.Transition:		//�g�����W�V�����摜���g�p
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local g = spriteData.GetGraphics(i);
				local hdl = g.ID;
				Game.DXLib.DrawBlendGraph(
					spriteData.Location.X + g.Location.X,
					spriteData.Location.Y + g.Location.Y,
					hdl,
					true,
					Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.WipeBlendGraph, "�t�@�C����").String,
					255 - spriteData.WipeCounter
				);
			}
			break;

		default:
			//���̂܂ܕ`��
			spriteData.Draw();
			break;
	}
}


//�C�x���g�����ړ��̊g���I�ȃR�}���h����������
function/*bool*/ EventHandler_EVMove(intEVID, intOption) {
	local ev = null;
	local getSV = null;
	local setSV = null;

	if(intEVID != -1) {
		//�C�x���g�͒ʏ�ʂ�̎擾
		ev = Game.SceneMap.MapData.EVGetData(intEVID);
		getSV = function(ev) {
			return ev.SelfValues.Get(0x5CA27058/*�W�����v�J�E���^�[*/).Value;
		}
		setSV = function(ev, val) {
			ev.SelfValues.Get(0x5CA27058/*�W�����v�J�E���^�[*/).Value = val;
		}
	} else {
		//��l���̓C�x���g�݊��̃I�u�W�F�N�g
		ev = Game.SceneMap.MapData.Player.MapObject;
		getSV = function(ev) {
			return UI1734005777/*��l���W�����v�J�E���^�[*/;
		}
		setSV = function(ev, val) {
			UI1734005777/*��l���W�����v�J�E���^�[*/ = val;
		}
	}

	switch(intOption) {
		case EVMoveExType.Jump:		//�W�����v
			//�ړ��J�E���^�[��`��ʒu�̔������Ɏg��
			ev.ForbiddenAutoMove = true;
			if(getSV(ev) == 1) {
				//���n���[�V����
				ev.MoveCounter.Y += SquareSpeed.Normal;
				if(ev.MoveCounter.Y >= 0) {
					//���[�V�����I��
					ev.MoveCounter.Y = 0;
					ev.ForbiddenAutoMove = false;
					setSV(ev, 0);
					return true;
				}
			} else {
				//���􃂁[�V����
				ev.MoveCounter.Y -= SquareSpeed.Normal;
				if(ev.MoveCounter.Y <= -Game.SceneMap.MapData.TileSize) {
					//���n���[�V�����ֈڍs
					setSV(ev, 1);
				}
			}
			return false;

		case EVMoveExType.Oni:		//�S���[�h
			//A*�@�ŒǐՂ���
			if(Astar.MoveTasks.len() == 0 || Astar.MoveTaskIndex >= Astar.MoveTasks.len()) {
				//�o�H���c���Ă��Ȃ��ꍇ�͍ŐV�̌o�H���쐬����
				Astar.ResetRouteByAstar(intEVID, Game.SceneMap.MapData.Player.MapObject.Position, Astar.GetDistance(-1, intEVID));
			}
			if(Astar.MoveTasks.len() > 0 && Astar.MoveTaskIndex < Astar.MoveTasks.len()) {
				//�o�H���܂��c���Ă���ꍇ�͂������������
				local dir = Astar.MoveTasks[Astar.MoveTaskIndex];
				ev.SetDirection4(dir);
				if(Game.SceneMap.MapData.CheckBeforeMove(intEVID, ev.Position.X, ev.Position.Y, dir, true) == true) {
					//�ړ��ł���ꍇ�̂ݏ���
					local rpos = Game.SceneMap.Direction4ToPoint(dir);
					ev.AutoSetMoveCounter(dir);
					ev.Position.Set(ev.Position.X + rpos.X, ev.Position.Y + rpos.Y);
				}
				Astar.MoveTaskIndex++;
			}
			return true;
	}
	return true;
}


//�Z�[�u�f�[�^�̃w�b�_�[���𐶐�����
function/*string*/ EventHandler_RequiredSaveHeader() {
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(0));
	return (GetPlayTime() + "  " + actor.Name + format("  Lv. %2d", actor.Level));
}


//�Z�[�u�f�[�^�̃��[�U�[��`�̗̈���Z�b�g����
function/*string*/ EventHandler_SaveUserData() {
	local buf = "";

	//�ݒ���V���A���C�Y
	buf += format("%d;", Game.Material.Sounds.StandardVolume);

	return buf;
}


//�Z�[�u�f�[�^�̃��[�U�[��`�̗̈�����[�h����
function/*void*/ EventHandler_LoadUserData(strSaveData) {
	local spl = split(strSaveData, ";");
	if(spl.len() <= 1) {
		return;		//�f�[�^�Ȃ�
	}

	//�ݒ���f�V���A���C�Y
	Game.Material.Sounds.StandardVolume = spl[0].tointeger();
}


//���b�Z�[�W�E�B���h�E��\������
function/*void*/ EventHandler_ShowMessage(intFaceID, strSpeaker, strMsg) {
	//���ۂ̕`��͕�����s�X�N���v�g�ōs����
	ShowMessage(intFaceID, strSpeaker, strMsg);
}


//�}�b�v���ړ�����
function/*void*/ EventHandler_ChangeMap(strMapFileName, intX, intY, intDirection) {
	local oldBGM = Game.SceneMap.MapData.FirstMapBGMRawString;
	local oldBGS = Game.SceneMap.MapData.FirstMapBGSRawString;

	if(UF1358433584/*�}�b�v�ړ����Ƀt�F�[�h�A�E�g���Ȃ�*/ == false) {
		Game.FadeOut(SquareSpeed.High);
	}

	//�}�b�v���\�����폜
	Game.MT.DeleteThread("�}�b�v���p�l���\��");

	Game.SceneMap.MapData.LoadMap(strMapFileName, false);
	Game.SceneMap.MapData.Player.MapObject.Position.Set(intX, intY);
	if(intDirection != Direction4.None) {
		Game.SceneMap.MapData.Player.CharData.Direction = intDirection;
	}

	local soundStopping = false;
	if(Game.BGMKeep == false
	&& oldBGM.len() > 0
	&& Game.SceneMap.MapData.FirstMapBGMRawString.len() > 0
	&& oldBGM != Game.SceneMap.MapData.FirstMapBGMRawString) {
		soundStopping = true;
		Game.Material.Sounds.Stop(Game.SceneMap.BGM.Handle, 0.5, false);
	}
	if(Game.BGSKeep == false
	&& oldBGS.len() > 0
	&& Game.SceneMap.MapData.FirstMapBGSRawString.len() > 0
	&& oldBGS != Game.SceneMap.MapData.FirstMapBGSRawString) {
		soundStopping = true;
		Game.Material.Sounds.Stop(Game.SceneMap.BGS.Handle, 0.5, false);
	}
	if(soundStopping == true) {
		Game.Wait(0.5, true, false);
	}

	//�}�b�v�ړ���̋��ʏ���
	DoAfterChangeMapProcedure();
}


//�f�o�b�O�R���\�[���̊g���I�ȃR�}���h����������
function/*bool*/ EventHandler_DebugCommand(strCommandArg) {
	local stateDB = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	local selfValueDB = Game.DB.GetDB(DBType.VDB, VDBIndices.Value).GetSubDB(DBValueIndices.SelfActor);
	local parameterDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);

	local spl = split(strCommandArg, " ");
	switch(spl[0]) {
		case "h":
			//�w���v
			print("	�O���[�o���ϐ��o��:		gbl [(�i�荞�ތ^��)]");
			print("	�퓬�����ɂ���:			bwin");
			print("	�퓬�s�k�ɂ���:			blose");
			print("	�퓬�����ɂ���:			besc");
			print("	�퓬���f�ɂ���:			bbrk");
			print("	���j�b�g����:		lunit");
			print("	PT/UT�A�N�^�[�ʕϐ��ݒ�:		esval [�C���f�b�N�X] [�ʕϐ�ID] [�l]");
			print("	PT/UT�A�N�^�[�p�����[�^�[�ݒ�:		espar [�C���f�b�N�X] [�p�����[�^�[ID] [�l]");
			print("	PT/UT�A�N�^�[�Ԑڌ��ʐݒ�:		esstt [�C���f�b�N�X] [�Ԑڌ���ID]");
			print("	PT/UT�A�N�^�[�Ԑڌ��ʑS����:		ersttall [�C���f�b�N�X]");
			print("	PT/UT�A�N�^�[�o�X�e�S����:		ersttbad [�C���f�b�N�X]");
			return true;

		case "gbl":
			//�O���[�o���ϐ��o��
			local typeName = "";
			if(spl.len() == 1) {
				//���ׂĕ\��
				typeName = null;
			} else if(spl.len() == 2) {
				//�^�𐧌����ĕ\��
				typeName = spl[1];
			}

			//���[�g�e�[�u���𑖍�
			foreach(key, var in getroottable()) {
				local varType = type(var);
				if(typeName == null || varType == typeName) {
					if(varType == "null") {
						continue;
					} else if(varType == "instance") {
						print("\t[" + key + "] = instance");
					} else {
						print("\t[" + key + "] = " + var.tostring());
					}
				}
			}
			return true;

		case "lunit":
			//���j�b�g�A�N�^�[�� [List Unit]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			}
			print("���j�b�g�G�l�~�[�ꗗ:");
			foreach(enemy in Enemies) {
				enemy.PrintData();
			}
			return true;

		case "esval":
			//���j�b�g�ʕϐ��ݒ� [Enemy SetSelfValue]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			} else if(spl.len() != 4) {
				print("cmdError: �����̌����s���ł��B");
				return true;
			}
			GetActorData(spl[1].tointeger()).SelfValues.Get(selfValueDB.ConvertToFixedID(spl[2].tointeger())).Value = spl[3].tointeger();
			return true;

		case "espar":
			//���j�b�g�p�����[�^�[�ݒ� [Enemy SetParameter]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			} else if(spl.len() != 4) {
				print("cmdError: �����̌����s���ł��B");
				return true;
			}
			GetActorData(spl[1].tointeger()).GetParameter(parameterDB.ConvertToFixedID(spl[2].tointeger())).Value = spl[3].tointeger();
			return true;

		case "esstt":
			//���j�b�g�Ԑڌ��ʐݒ� [Enemy SetState]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			} else if(spl.len() != 3) {
				print("cmdError: �����̌����s���ł��B");
				return true;
			}
			GetActorData(spl[1].tointeger()).SetState(stateDB.ConvertToFixedID(spl[2].tointeger()), true);
			return true;

		case "ersttall":
			//���j�b�g�Ԑڌ��ʑS���� [Enemy ResetState All]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			} else if(spl.len() != 2) {
				print("cmdError: �����̌����s���ł��B");
				return true;
			}
			GetActorData(spl[1].tointeger()).ClearState();
			return true;

		case "ersttbad":
			//���j�b�g�o�X�e�S���� [Enemy ResetState Bad]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			} else if(spl.len() != 2) {
				print("cmdError: �����̌����s���ł��B");
				return true;
			}
			GetActorData(spl[1].tointeger()).ClearStateOnlyBad();
			return true;

		case "bwin":
			//�퓬�����ɂ��� [Battle Win]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			}
			MoveToEndingPhase(BattleResult.Win);
			return true;

		case "blose":
			//�퓬�s�k�ɂ��� [Battle Lose]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			}
			MoveToEndingPhase(BattleResult.Lose);
			return true;

		case "besc":
			//�퓬�����ɂ��� [Battle Escape]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			}
			MoveToEndingPhase(BattleResult.Escape);
			return true;

		case "bbrk":
			//�퓬���f�ɂ��� [Battle Break]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("�o�g���V�[���ł͂Ȃ����߁A�����ł��܂���B");
				return true;
			}
			MoveToEndingPhase(BattleResult.Break);
			return true;
	}
	return false;
}
