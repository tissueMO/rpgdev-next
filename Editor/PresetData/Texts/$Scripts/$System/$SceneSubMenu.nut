//###################################################################
//		���j���[�V�[��
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\!var.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\Common.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuStatus.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuItem.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuEquip.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuSkill.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuMember.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuSave.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuOption.nut");

//####################  �V�[���N��  ####################
function SceneSubSetup() {
	//�Z�[�u�f�[�^�̃w�b�_�[�X�V
	Game.SaveLoad.GetAllSaveHeader();

	//�p�[�e�B�L�����̃O���t�B�b�N�ǂݍ���
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
		actor.LoadGraphics(actor.Graphics);
	}
}


//####################  �V�[���`��  ####################
function SceneSubDraw() {
	local offsetX = 30;
	local offsetY = 15;

	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, 255 - WindowTransparent);
	if(OneMenuMode == false) {
		//�T�C�h���j���[�\��
		DrawWindow(0, 0, 200, 235);
		foreach(idx in MenuItem) {
			local lineSpace = 30;
			local text = "";
			switch(idx) {
				case MenuItem.Status:
					text = "�X�e�[�^�X";
					break;
				case MenuItem.Item:
					text = "�A�C�e��";
					break;
				case MenuItem.Equip:
					text = "����";
					break;
				case MenuItem.Skill:
					text = "�X�L��";
					break;
				case MenuItem.Member:
					text = "���ёւ�";
					break;
				case MenuItem.Save:
					text = "�Z�[�u";
					break;
				case MenuItem.Option:
					text = "���ݒ�";
					break;
			}
			Game.DXLib.DrawString(offsetX, offsetY + lineSpace * idx, 0x1B6F049F/*�W���t�H���g*/, WHITE, text);

			//�J�[�\��
			if(idx == Cursor[0]) {
				DrawCursorPanel(15, 12 + lineSpace * idx, 200 - 15 * 2, 28, (Depth == 0));
			}
		}

		//���ݏ��
		offsetY = 235;
		DrawWindow(0, offsetY, 200, 155);

		Game.DXLib.DrawString(offsetX - 15, offsetY + 15, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, "���ݒn:");
		Game.DXLib.DrawString(offsetX, offsetY + 35, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE,
			Game.SceneMap.MapData.CurrentMapName
		);
		Game.DXLib.DrawString(offsetX - 15, offsetY + 60, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, "�v���C����:");
		Game.DXLib.DrawString(offsetX, offsetY + 80, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE,
			GetPlayTime()
		);
		Game.DXLib.DrawString(offsetX - 15, offsetY + 105, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, "������:");
		Game.DXLib.DrawString(offsetX, offsetY + 125, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE,
			format("%7d ", Game.DB.VDBParty.Money) + US1246572451/*�ʉݖ���*/
		);
	}

	DrawWindow(200, 0, Game.WindowSize.Width - 200, Game.WindowSize.Height - 30);	//��g
	DrawWindow(0, Game.WindowSize.Height - 32, Game.WindowSize.Width, 32);			//������

	//�D�揈��: ������ or �G���[���b�Z�[�W/�I����
	DrawComment(GetMenuComment());

	if(TargetMemberSelecting == true) {
		//�D�揈��: �Ώۃ����o�[�I��
		DrawTargetMemberSelect();
		return;
	} else if(PlaceSelecting == true) {
		//�D�揈��: �ꏊ�I��
		DrawPlaceSelect();
		return;
	}

	//�K�w 0~1 �͋��ʏ���
	switch(Depth) {
		case 0:
		case 1:
			switch(Cursor[0]) {
				case MenuItem.Status:
				case MenuItem.Equip:
				case MenuItem.Skill:
				case MenuItem.Member:
					//�A�N�^�[�p�l����`��
					for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
						DrawActorPanel(i, false);
					}
					if(Depth == 1) {
						DrawCursorPanel(200 + 15, 12 + 110 * Cursor[Depth], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
					}
					return;
			}
			break;
	}

	//�K�w 2~ �̓��j���[���Ƃɏ���
	switch(Cursor[0]) {
		case MenuItem.Status:
			DrawCharStatus(true);
			break;
		case MenuItem.Item:
			DrawItemList((Depth == 0) ? 0 : TopIndex[1]);
			break;
		case MenuItem.Equip:
			DrawEquipMenu();
			break;
		case MenuItem.Skill:
			DrawSkillList(TopIndex[2]);
			break;
		case MenuItem.Member:
			DrawMemberMenu();
			break;
		case MenuItem.Save:
			DrawSaveList((Depth == 0) ? 0 : TopIndex[Depth]);
			break;
		case MenuItem.Option:
			DrawOptionMenu();
			break;
	}
}


//####################  �V�[������  ####################
function SceneSubInput() {
	//�C�x���g����Ăяo���ꂽ���j���[�ł̓��Z�b�g�ł��Ȃ��悤�ɂ���
	if(Game.GetKeyInputCounter(KeyID.F12) == 1 && OneMenuMode == false) {
		GameReset();
		return;
	}

	//�K�w 0~1 �͋��ʏ���
	switch(Depth) {
		case 0:
			//�g�b�v���
			if(CheckUpKeyInput() == true) {
				Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDownKeyInput() == true) {
				Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDecideKeyInput() == 1) {
				//�G���[�`�F�b�N
				switch(Cursor[0]) {
					case MenuItem.Item:
						if(Game.DB.VDBParty.GetItemCountAll() == 0) {
							//�A�C�e���������Ă��Ȃ��ꍇ�̓G���[�o���Ė߂�
							Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
							ShowErrorMessage("�A�C�e������������Ă��܂���B");
							return;
						}
						break;

					case MenuItem.Save:
						if(UF1110905879/*�Z�[�u�֎~*/ == true) {
							Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
							ShowErrorMessage("���݃Z�[�u���֎~����Ă��܂��B");
							return;
						}
						break;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				MoveToNextDepth();
			} else if(CheckCancelKeyInput() == 1) {
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
				CloseMenuScene();
			}
			return;

		case 1:
			if(CheckCancelKeyInput() == 1) {
				//�L�����Z���̗D�揈��
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
				if(OneMenuMode == false) {
					//�g�b�v�K�w�ɖ߂�
					Depth--;
				} else {
					//�P�ꃁ�j���[�̂Ƃ��̓g�b�v�K�w�ɖ߂炸���j���[�����
					CloseMenuScene();
				}
				return;
			}

			switch(Cursor[0]) {
				case MenuItem.Status:
				case MenuItem.Equip:
				case MenuItem.Skill:
				case MenuItem.Member:
					//�p�[�e�B�����o�[�I��
					if(CheckUpKeyInput() == true) {
						Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
					} else if(CheckDownKeyInput() == true) {
						Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
					} else if(CheckDecideKeyInput() == 1) {
						//����
						local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));

						//�G���[�`�F�b�N
						switch(Cursor[0]) {
							case MenuItem.Skill:
								if(actor.LearnedSkills.Count() == 0) {
									Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
									ShowErrorMessage("�I�����ꂽ�L�����̓X�L��������K�����Ă��܂���B");
									return;
								} else if(IsDead(Cursor[1]) == true) {
									Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
									ShowErrorMessage("�I�����ꂽ�L�����͎���ł��邽�߁A�X�L�����g�p�ł��܂���B");
									return;
								}
								break;
						}

						Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
						MoveToNextDepth();
						RefreshEquipTypeList();					//�X�e�[�^�X�p: �������ʂ̃��X�g���X�V
						RefreshEquipList(EquipTypeList[0]);		//�����p: �����i���X�g���i�荞��

						//�ʏ���
						switch(Cursor[0]) {
							case MenuItem.Member:
								Cursor[Depth] = Cursor[Depth - 1];
								break;
						}
					}
					return;
			}
			break;
	}

	//�K�w 2~ �̓��j���[���Ƃɏ���
	switch(Cursor[0]) {
		case MenuItem.Status:
			InputCharStatus();
			break;
		case MenuItem.Item:
			InputItemMenu();
			break;
		case MenuItem.Equip:
			InputEquipMenu();
			break;
		case MenuItem.Skill:
			InputSkillMenu();
			break;
		case MenuItem.Member:
			InputMemberMenu();
			break;
		case MenuItem.Save:
			InputSaveMenu();
			break;
		case MenuItem.Option:
			InputOptionMenu();
			break;
	}
}


//####################  �V�[���j��  ####################
function SceneSubDispose() {
	//�p�[�e�B�L�����̃O���t�B�b�N�j��
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
		actor.DisposeGraphics();
	}
}


//####################  �V�[�����t�F�[�h�C��  ####################
function SceneFadeIn() {
	//�t�F�[�h�C��
	DoFadeObject(
		function() { WindowTransparent = 255; },
		function() { return ((WindowTransparent -= SquareSpeed.Moment) <= 0); }
	);
}
