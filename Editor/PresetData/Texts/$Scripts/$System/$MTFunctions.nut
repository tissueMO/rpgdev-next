//###################################################################
//		�^���I�ȕ���X���b�h�Ƃ��ē��삳����֐����`���܂��B
//###################################################################

//####################  ���b�Z�[�W�E�B���h�E�`��  ####################
//int["phase"] = 0:�t�F�[�h�C��, 1:����, 2:�t�F�[�h�A�E�g
//int["isWaiting"] = ���b�Z�[�W����ҋ@�����ǂ���
//int["trans"] = �E�B���h�E�s�����x
//int["faceID"] = �t�F�C�XID (�w�肵�Ȃ��ꍇ�� -1)
//int["isRightFace"] = �t�F�C�X�����Α����ǂ���
//int["offset"] = ���b�Z�[�W���e�I�t�Z�b�g
//int["speakerOffset"] = �b�Җ��I�t�Z�b�g
//str["speaker"] = �b�Җ�
//str["msg"] = ���b�Z�[�W���e
function MTMessage(intHandle) {
	//�E�B���h�E�J����
	switch(Game.MT.GetThread(intHandle).GetValueInteger("phase")) {
		case 0:		//�t�F�[�h�C��
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") + SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") >= 255) {
				//�����t�F�[�Y�ֈڍs
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 1);
				Game.MT.GetThread(intHandle).SetValueInteger("trans", 255);
			}
			break;

		case 2:		//�t�F�[�h�A�E�g
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") - SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") < 0) {
				return true;		//�X���b�h�I��
			}
			break;
	}
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans"));

	local hdl = -1;
	if(Game.MT.GetThread(intHandle).GetValueInteger("faceID") != -1) {
		hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Face, Game.MT.GetThread(intHandle).GetValueInteger("faceID"));
	}
	local size = (hdl <= -1) ? null : Game.DXLib.GetGraphSize(hdl);
	local x = (hdl <= -1 || Game.MT.GetThread(intHandle).GetValueInteger("isRightFace") == FALSE) ? 0 : Game.WindowSize.Width - size.Width;

	//�E�B���h�E
	DrawWindow(0, Game.WindowSize.Height - MsgWndHeight, Game.WindowSize.Width, MsgWndHeight);

	//���b�Z�[�W
	Game.DXLib.SetDrawArea(16, Game.WindowSize.Height - MsgWndHeight + 16, Game.WindowSize.Width - 16, Game.WindowSize.Height);
	if(hdl > -1 && Game.MT.GetThread(intHandle).GetValueInteger("isRightFace") == FALSE) {
		//�t�F�C�X�����̂Ƃ��������b�Z�[�W���e�̈ʒu���E�Ɉړ�����
		Game.DXLib.DrawString(
			16 + MsgFaceStandardWidth,
			16 + Game.WindowSize.Height - MsgWndHeight + Game.MT.GetThread(intHandle).GetValueInteger("offset"),
			MsgFontID,
			WHITE,
			Game.MT.GetThread(intHandle).GetValueString("msg")
		);
	} else {
		Game.DXLib.DrawString(
			16,
			16 + Game.WindowSize.Height - MsgWndHeight + Game.MT.GetThread(intHandle).GetValueInteger("offset"),
			MsgFontID,
			WHITE,
			Game.MT.GetThread(intHandle).GetValueString("msg")
		);
	}
	Game.DXLib.ResetDrawArea();

	//�t�F�C�X
	if(hdl != -1) {
		Game.DXLib.DrawGraph(hdl, x, Game.WindowSize.Height - size.Height, true, false);
	}

	//���O�p�l��: �󔒂̏ꍇ�͕\�����Ȃ�
	if(Game.MT.GetThread(intHandle).GetValueString("speaker") != "") {
		if(hdl <= -1 || IsFixedSizedFace == TRUE) {
			//���b�Z�[�W�E�B���h�E�̍���ɕ\��
			DrawWindow(
				0,
				Game.WindowSize.Height - MsgWndHeight - MsgNameHeight + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				MsgNameWidth,
				MsgNameHeight
			);
			Game.DXLib.DrawStringC(
				MsgNameWidth / 2,
				Game.WindowSize.Height - MsgWndHeight - MsgNameHeight + 10 + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				0x6DD57515/*�E�B���h�E���e�L�X�g*/,
				WHITE,
				Game.MT.GetThread(intHandle).GetValueString("speaker")
			);
		} else if(Game.MT.GetThread(intHandle).GetValueInteger("isRightFace") == FALSE) {
			//���b�Z�[�W�E�B���h�E�̍����ɕ\��
			DrawWindow(
				0,
				Game.WindowSize.Height - MsgNameHeight + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				MsgNameWidth,
				MsgNameHeight
			);
			Game.DXLib.DrawStringC(
				MsgNameWidth / 2,
				Game.WindowSize.Height - MsgNameHeight + 10 + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				0x6DD57515/*�E�B���h�E���e�L�X�g*/,
				WHITE,
				Game.MT.GetThread(intHandle).GetValueString("speaker")
			);
		} else {
			//���b�Z�[�W�E�B���h�E�̉E���ɕ\��
			DrawWindow(
				Game.WindowSize.Width - MsgNameWidth,
				Game.WindowSize.Height - MsgNameHeight + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				MsgNameWidth,
				MsgNameHeight
			);
			Game.DXLib.DrawStringC(
				Game.WindowSize.Width - MsgNameWidth + MsgNameWidth / 2,
				Game.WindowSize.Height - MsgNameHeight + 10 + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				0x6DD57515/*�E�B���h�E���e�L�X�g*/,
				WHITE,
				Game.MT.GetThread(intHandle).GetValueString("speaker")
			);
		}
	}

	//���ŃC���W�P�[�^�[�̃A�j���[�V����
	if(Game.MT.GetThread(intHandle).GetValueInteger("isWaiting") == TRUE) {
		local wndhdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*�V�X�e���O���t�B�b�N*/);
		local delta = Game.DXLib.GetNowCount();
		local animIdx = Game.DXLib.GetNowCount() % 800 / 200;
		Game.DXLib.DrawRectGraph(wndhdl, Game.WindowSize.Width - 24, Game.WindowSize.Height - 24, 96 + 16 * (animIdx % 2), 64 + 16 * (animIdx / 2), 16, 16, true, false);
	}

	Game.DXLib.RestoreDrawBlendMode();
	return false;		//�X���b�h�p��
}


//####################  �I����  ####################
//int["phase"] = 0:�t�F�[�h�C��, 1:����, 2:�t�F�[�h�A�E�g
//int["trans"] = �E�B���h�E�s�����x
//int["Cursor"] = �J�[�\���ʒu
//str["msg"] = �I�������e
function MTChoice(intHandle) {
	local list = split(Game.MT.GetThread(intHandle).GetValueString("msg"), "|");
	local fontSize = Game.DB.FDBMaterial.GetSubDB(DBMaterialIndices.Font).GetCell(0x2ABC7058/*���b�Z�[�W�t�H���g*/, "�s��").Integer;

	//�E�B���h�E�J����
	switch(Game.MT.GetThread(intHandle).GetValueInteger("phase")) {
		case 0:		//�t�F�[�h�C��
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") + SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") >= 255) {
				//�����t�F�[�Y�ֈڍs
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 1);
				Game.MT.GetThread(intHandle).SetValueInteger("trans", 255);
			}
			break;

		case 2:		//�t�F�[�h�A�E�g
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") - SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") < 0) {
				return true;		//�X���b�h�I��
			}
			break;
	}
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans"));

	//�E�B���h�E
	DrawWindow(
		ChoiceWndMargin,
		Game.WindowSize.Height * 1 / 3 - (fontSize * list.len()) / 2 - 14,
		Game.WindowSize.Width - ChoiceWndMargin * 2,
		fontSize * list.len() + 22
	);

	//�I�������X�g
	for(local i = 0; i < list.len(); i++) {
		Game.DXLib.DrawString(
			ChoiceWndMargin + 45,
			Game.WindowSize.Height * 1 / 3 - (fontSize * list.len()) / 2 + i * fontSize,
			MsgFontID,
			WHITE,
			list[i]
		);
	}

	//�J�[�\��
	DrawCursorPanel(
		ChoiceWndMargin + 15,
		Game.WindowSize.Height * 1 / 3 - (fontSize * list.len()) / 2 + Game.MT.GetThread(intHandle).GetValueInteger("Cursor") * fontSize - 5 + 2,
		Game.WindowSize.Width - ChoiceWndMargin * 2 - 15 * 2
		fontSize - 2, true
	);

	Game.DXLib.RestoreDrawBlendMode();
	return false;		//�X���b�h�p��
}


//####################  �Œ莮�e���b�v  ####################
//int["phase"] = 0:�t�F�[�h�C��, 1:�ҋ@, 2:�t�F�[�h�A�E�g
//int["trans"] = �s�����x
//int["timeMS"] = �ҋ@�b��
//int["canKeyBreak"] = ����L�[�����Œ��f�ł���悤�ɂ��邩�ǂ���
//int["enabledBack"] = �w�i��`�悷�邩�ǂ���
//int["starttime"] = �ҋ@���J�n�������� (�����p)
//str["msg"] = �e���b�v���e
function MTFixedSubtitles(intHandle) {
	local hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*�V�X�e���O���t�B�b�N*/);
	local lines = split(Game.MT.GetThread(intHandle).GetValueString("msg"), "\n");

	switch(Game.MT.GetThread(intHandle).GetValueInteger("phase")) {
		case 0:		//�t�F�[�h�C��
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") + SquareSpeed.Normal
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") >= 255) {
				//�ҋ@�t�F�[�Y�ֈڍs
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 1);
				Game.MT.GetThread(intHandle).SetValueInteger("trans", 255);
				Game.MT.GetThread(intHandle).SetValueInteger("starttime", Game.DXLib.GetNowCount());
			}
			break;

		case 1:		//�ҋ@
			local now = Game.DXLib.GetNowCount();
			if(CheckDecideKeyInput() > 0 && Game.MT.GetThread(intHandle).GetValueInteger("canKeyBreak") == TRUE) {
				//�L�[���͂Œ��f
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 2);
			} else if(now - Game.MT.GetThread(intHandle).GetValueInteger("starttime") >= Game.MT.GetThread(intHandle).GetValueInteger("timeMS")) {
				//���Ԍo�߂ŏI��
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 2);
			}
			break;

		case 2:		//�t�F�[�h�A�E�g
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") - SquareSpeed.Normal
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") < 0) {
				return true;		//�X���b�h�I��
			}
			break;
	}

	//�w�i��Y���ĕ`�悷��
	if(Game.MT.GetThread(intHandle).GetValueInteger("enabledBack") == TRUE) {
		Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans") / 3);
		Game.DXLib.DrawRectExtendGraph(hdl, 0, Game.WindowSize.Height * 1 / 4, Game.WindowSize.Width, lines.len() * SubtitleLineHeight, 0, 0, 64, 64, true);
	}

	//�������`�悷��
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans"));
	Game.DXLib.DrawStringC(
		Game.WindowSize.Width / 2,
		Game.WindowSize.Height * 1 / 4,
		SubtitleFontID,
		WHITE,
		Game.MT.GetThread(intHandle).GetValueString("msg")
	);

	Game.DXLib.SetDrawBlendMode(-1, -1);
	return false;		//�X���b�h�p��
}


//####################  �����e���b�v  ####################
//int["canSpeedUp"] = ����L�[������Ԃō��������邩�ǂ���
//int["y"] = �\���ʒuY���W
//int["allLineHeight"] = �S�s����
//int["speed"] = �ړ����x
//str["msg"] = �e���b�v���e
function MTFlowSubtitles(intHandle) {
	Game.DXLib.DrawString(
		Game.WindowSize.Width * 1 / 10,
		Game.MT.GetThread(intHandle).GetValueInteger("y"),
		SubtitleFontID,
		WHITE,
		Game.MT.GetThread(intHandle).GetValueString("msg")
	);

	//����
	if(CheckDecideKeyInput() > 0 && Game.MT.GetThread(intHandle).GetValueInteger("canSpeedUp") == TRUE) {
		//�X�s�[�h�A�b�v
		Game.MT.GetThread(intHandle).SetValueInteger("y",
			Game.MT.GetThread(intHandle).GetValueInteger("y") - SquareSpeed.High
		);
	} else {
		Game.MT.GetThread(intHandle).SetValueInteger("y",
			Game.MT.GetThread(intHandle).GetValueInteger("y") - Game.MT.GetThread(intHandle).GetValueInteger("speed")
		);
	}

	//�I������
	if(-Game.MT.GetThread(intHandle).GetValueInteger("allLineHeight") >= Game.MT.GetThread(intHandle).GetValueInteger("y")) {
		return true;		//�X���b�h�I��
	} else {
		return false;		//�X���b�h�p��
	}
}
