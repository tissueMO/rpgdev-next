//###################################################################
//		�^�C�g���V�[��
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\!SceneMainTitle_var.nut");

//####################  �V�[���N��  ####################
function SceneMainSetup() {
	//���̑��A����������
	Game.Material.Sounds.StandardVolume = Material_MaxVolume;
	HealPartyAll();

	//�f�ޓǂݍ���
	Game.SceneTitle.BGM.Load(Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.TitleBGM, "�t�@�C����").String, -1, 0, 0);
	Game.SceneTitle.BGM.Play(PlayType.Loop, false);
	Graphs[0] = Game.Material.Graphics.Add(Game.DB.UDB.GetUserDB(0x3F0A52DA/*����f�ރ��X�g*/).GetCell(DefaultMaterialID.TitleGraph, "�t�@�C����").String);

	Game.FadeIn(SquareSpeed.High);
}


//####################  �V�[���`��  ####################
function SceneMainDraw() {
	//���C���O���t�B�b�N�`��
	Game.DXLib.DrawExtendGraph(Graphs[0], 0, 0, 0.8, 0.8, false);

	//�g�b�v��ʂ̗D�揈��
	if(Depth == 0) {
		DrawWindow(Game.WindowSize.Width / 2 - 230, 330, 200, 110);
		foreach(idx in MenuItem) {
			local text = "";
			switch(idx) {
				case MenuItem.Start:
					text = "�͂��߂���";
					break;
				case MenuItem.Load:
					text = "�Â�����";
					break;
				case MenuItem.Exit:
					text = "�Q�[���I��";
					break;
			}
			Game.DXLib.DrawStringC(Game.WindowSize.Width / 2 - 130, 348 + 27 * idx, 0x1B6F049F/*�W���t�H���g*/, WHITE, text);
		}
		DrawCursorPanel(Game.WindowSize.Width / 2 - 230 + 15, 345 + Cursor[Depth] * 27, 200 - 15 * 2, 27, true);
		return;
	}

	//�Z�[�u���X�g
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, 255 - WindowTransparent);
	DrawWindow(Game.WindowSize.Width / 2 - 200, 250, 400, 195);
	for(local i = TopIndex[Depth], n = 0; n < AreaMax; i++, n++) {
		if(Game.SaveLoad.GetSaveHeader(i) == "") {
			Game.DXLib.DrawString(Game.WindowSize.Width / 2 - 200 + 30, 265 + n * 43, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
				format("%02d: No Data", i + 1)
			);
		} else {
			Game.DXLib.DrawString(Game.WindowSize.Width / 2 - 200 + 30, 265 + n * 43, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
				format("%02d: ", i + 1) + Game.SaveLoad.GetSaveDateTime(i)
			);
			Game.DXLib.DrawString(Game.WindowSize.Width / 2 - 200 + 50, 265 + n * 43 + 17, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
				Game.SaveLoad.GetSaveHeader(i)
			);
		}
	}

	//���ʌ��J�[�\��
	if(Depth == 2) {
		if(0 <= Cursor[1] + TopIndex[1] - TopIndex[Depth] && Cursor[1] + TopIndex[1] - TopIndex[Depth] < AreaMax) {
			DrawCursorArrow(Game.WindowSize.Width / 2 - 200 + 20, 256 + Cursor[1] * 43, Direction4.East, false);
		}
	}

	//�J�[�\���E�X�N���[���o�[
	DrawCursorPanel(Game.WindowSize.Width / 2 - 200 + 15, 250 + 12 + Cursor[Depth] * 43, 400 - 45, 37, true);
	DrawVScrollBar(Game.WindowSize.Width / 2 + 200, 250, 195, SaveCount, TopIndex[Depth], AreaMax);

	Game.DXLib.SetDrawBlendMode(-1, 0);
}


//####################  �V�[������  ####################
function SceneMainInput() {
	switch(Depth) {
		case 0:
			//�g�b�v���
			if(CheckUpKeyInput() == true) {
				Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDownKeyInput() == true) {
				Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDecideKeyInput() == 1) {
				//���ڌ���
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);
				switch(Cursor[Depth]) {
					case MenuItem.Start:
						//�͂��߂���
						Game.FadeOut(SquareSpeed.High);
						Game.Material.Sounds.Stop(Game.SceneTitle.BGM.Handle, 1.0, false);
						Game.Wait(1.0, true, false);

						//�������E�}�b�v�ֈڍs
						UI1874437940/*�N�����̎����b*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();
						UF1358433584/*�}�b�v�ړ����Ƀt�F�[�h�A�E�g���Ȃ�*/ = true;
						Game.ChangeToMapScene();
						Game.SceneMap.ChangeMap(
							TitleStart.MapFileName,
							TitleStart.PositionX,
							TitleStart.PositionY,
							TitleStart.Direction
						);
						break;

					case MenuItem.Load:
						//�Â�����
						Depth++;
						TopIndex[Depth] = 0;
						Cursor[Depth] = 0;

						//�Z�[�u�f�[�^�̃w�b�_�[�X�V
						Game.SaveLoad.GetAllSaveHeader();
						DoFadeObject(
							function() { WindowTransparent = 255; },
							function() { return ((WindowTransparent -= SquareSpeed.Moment) <= 0); }
						);
						break;

					case MenuItem.Exit:
						//�Q�[���I��
						Game.FadeOut(SquareSpeed.High);
						Game.ApplicationExit(ExitCode.Success);
						break;
				}
			}
			break;

		case 1:
			//�Â�����: �Z�[�u���X�g
			if(CheckUpKeyInput() == true) {
				if(0 <= TopIndex[Depth] + Cursor[Depth] - 1) {
					Cursor[Depth]--;
					if(Cursor[Depth] < 0) {
						Cursor[Depth] = 0;
						TopIndex[Depth]--;
						if(TopIndex[Depth] + Cursor[Depth] < 0) {
							TopIndex[Depth]++;		//����ȏ��ɍs���Ȃ�
						}
					}
				}
			} else if(CheckDownKeyInput() == true) {
				if(TopIndex[Depth] + Cursor[Depth] + 1 < SaveCount) {
					Cursor[Depth]++;
					if(Cursor[Depth] >= AreaMax) {
						Cursor[Depth] = AreaMax - 1;
						TopIndex[Depth]++;
						if(TopIndex[Depth] + Cursor[Depth] >= SaveCount) {
							TopIndex[Depth]--;		//����ȏ㉺�ɍs���Ȃ�
						}
					}
				}
			} else if(CheckDecideKeyInput() == 1) {
				//����: ���[�h���s
				if(Game.SaveLoad.IsEnabled(TopIndex[Depth] + Cursor[Depth]) == true) {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);
					Game.FadeOut(SquareSpeed.High);
					Game.Material.Sounds.Stop(Game.SceneTitle.BGM.Handle, 1.0, false);
					Game.Wait(1.0, true, false);
					Game.ChangeToMapScene();
					Game.SaveLoad.LoadData(TopIndex[Depth] + Cursor[Depth]);
					UI1874437940/*�N�����̎����b*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();

					//�}�b�v�ړ���̋��ʏ���
					DoAfterChangeMapProcedure();
				} else {
					//�����ȃZ�[�u�f�[�^
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, -1, 0, 0);
				}
			} else if(CheckCancelKeyInput() == 1) {
				//�L�����Z��
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, -1, 0, 0);
				DoFadeObject(
					function() { WindowTransparent = 0; },
					function() { return ((WindowTransparent += SquareSpeed.Moment) >= 255); }
				);
				Depth--;
			} else if(Game.GetKeyInputCounter(KeyID.D) == 1) {
				//D�L�[: �폜���s
				if(Game.SaveLoad.IsEnabled(TopIndex[Depth] + Cursor[Depth]) == false) {
					//�����ȃf�[�^�x�[�X
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, -1, 0, 0);
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);

					//�m�F�����
					CSceneMap.ShowMessage(-1, "�V�X�e��", "�Z�[�u�f�[�^ " + (TopIndex[Depth] + Cursor[Depth] + 1).tostring() + "�� ���폜���Ă���낵���ł����H\a");
					if(Choice("�͂�|������", true) == 0) {
						Game.SaveLoad.DeleteData(TopIndex[Depth] + Cursor[Depth]);
						Game.SaveLoad.GetAllSaveHeader();
					}
				}
			} else if(Game.GetKeyInputCounter(KeyID.C) == 1) {
				//C�L�[: ���ʂֈڍs
				if(Game.SaveLoad.IsEnabled(TopIndex[Depth] + Cursor[Depth]) == false) {
					//�����ȃf�[�^�x�[�X
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, -1, 0, 0);
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);
					Depth++;
					Cursor[Depth] = Cursor[Depth - 1];
					TopIndex[Depth] = TopIndex[Depth - 1];
				}
			}
			break;

		case 2:
			//�Â�����: ���ʐ�I��
			if(CheckUpKeyInput() == true) {
				if(0 <= TopIndex[Depth] + Cursor[Depth] - 1) {
					Cursor[Depth]--;
					if(Cursor[Depth] < 0) {
						Cursor[Depth] = 0;
						TopIndex[Depth]--;
						if(TopIndex[Depth] + Cursor[Depth] < 0) {
							TopIndex[Depth]++;		//����ȏ��ɍs���Ȃ�
						}
					}
				}
			} else if(CheckDownKeyInput() == true) {
				if(TopIndex[Depth] + Cursor[Depth] + 1 < SaveCount) {
					Cursor[Depth]++;
					if(Cursor[Depth] >= AreaMax) {
						Cursor[Depth] = AreaMax - 1;
						TopIndex[Depth]++;
						if(TopIndex[Depth] + Cursor[Depth] >= SaveCount) {
							TopIndex[Depth]--;		//����ȏ㉺�ɍs���Ȃ�
						}
					}
				}
			} else if(CheckDecideKeyInput() == 1) {
				//����: ���ʎ��s
				if(TopIndex[Depth] + Cursor[Depth] == TopIndex[Depth - 1] + Cursor[Depth - 1]) {
					//���ʌ��ƕ��ʐ悪����
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, -1, 0, 0);
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, -1, 0, 0);
					Game.SaveLoad.CopyData(TopIndex[Depth - 1] + Cursor[Depth - 1], TopIndex[Depth] + Cursor[Depth]);
					Game.SaveLoad.GetAllSaveHeader();
					Depth--;
					Cursor[Depth] = Cursor[Depth + 1];
					TopIndex[Depth] = TopIndex[Depth + 1];
				}
			} else if(CheckCancelKeyInput() == 1) {
				//�L�����Z��
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, -1, 0, 0);
				Depth--;
			}
			break;
	}
}


//####################  �V�[���j��  ####################
function SceneMainDispose() {
	foreach(hdl in Graphs) {
		Game.Material.Graphics.Delete(hdl);
	}
}
