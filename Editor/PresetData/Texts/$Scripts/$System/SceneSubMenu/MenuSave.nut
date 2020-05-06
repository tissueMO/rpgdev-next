//###################################################################
//		���j���[�V�[��: �Z�[�u���j���[
//###################################################################

///[summary]�Z�[�u���X�g��`��
function DrawSaveList(startIndex) {
	for(local i = startIndex, n = 0; n < SaveAreaMax && i < Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount); i++, n++) {
		Game.DXLib.DrawString(200 + 40, 20 + n * 50, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, format("%02d:", i + 1));
		if(Game.SaveLoad.GetSaveHeader(i) == "") {
			Game.DXLib.DrawString(200 + 70, 20 + n * 50, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, "No Data");
		} else {
			Game.DXLib.DrawString(200 + 70, 20 + n * 50 +  0, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, Game.SaveLoad.GetSaveDateTime(i));
			Game.DXLib.DrawString(200 + 70, 20 + n * 50 + 20, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, Game.SaveLoad.GetSaveHeader(i));
		}
	}

	//�J�[�\���E�X�N���[���o�[
	switch(Depth) {
		case 0:
			return;
		case 1:
			DrawCursorPanel(200 + 15, 12 + 2 + 50 * Cursor[Depth], Game.WindowSize.Width - 200 - 15 * 2 - 20, 48, true);
			break;
		case 2:
			//���ʌ�
			if(0 <= Cursor[1] + TopIndex[1] - TopIndex[Depth] && Cursor[1] + TopIndex[1] - TopIndex[Depth] < SaveAreaMax) {
				DrawCursorArrow(225, 12 + 50 * (Cursor[1] + TopIndex[1] - TopIndex[Depth]), Direction4.East, false);
			}
			//���ʐ�
			DrawCursorPanel(200 + 15, 12 + 2 + 50 * Cursor[Depth], Game.WindowSize.Width - 200 - 15 * 2 - 20, 48, true);
			break;
	}
	DrawVScrollBar(
		Game.WindowSize.Width,
		0,
		Game.WindowSize.Height - 30,
		Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount),
		TopIndex[Depth],
		SaveAreaMax
	);
}


///[summary]�Z�[�u���X�g��̓��͏���
function InputSaveMenu() {
	//�㉺����͋���
	if(CheckUpKeyInput() == true) {
		if(0 < Cursor[Depth] + TopIndex[Depth]) {
			if(0 < Cursor[Depth]) {
				Cursor[Depth]--;
			} else {
				TopIndex[Depth]--;
			}
		}
		return;
	} else if(CheckDownKeyInput() == true) {
		if(Cursor[Depth] + TopIndex[Depth] + 1 < Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount)) {
			if(Cursor[Depth] + 1 < SaveAreaMax) {
				Cursor[Depth]++;
			} else {
				TopIndex[Depth]++;
			}
		}
		return;
	} else if(CheckCancelKeyInput() == 1) {
		//�L�����Z��
		Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
		Depth--;
		return;
	}

	//���̑��̏���
	switch(Depth) {
		case 1:		//�I��
			if(CheckDecideKeyInput() == 1) {
				//����: �Z�[�u���s (�㏑���m�F�Ȃ�)
				UI248120655/*�O��܂ł̃v���C����*/ += Game.DXLib.GetNowCount() / 1000 - UI1874437940/*�N�����̎����b*/;
				UI1874437940/*�N�����̎����b*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();
				if(Game.SaveLoad.SaveData(Cursor[1] + TopIndex[1]) == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					ShowErrorMessage("�Z�[�u�Ɏ��s���܂����B");
					return;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				Game.SaveLoad.GetAllSaveHeader();		//�Z�[�u���X�g�X�V
			} else if(Game.GetKeyInputCounter(KeyID.D) == 1) {
				//D: �폜
				if(Game.SaveLoad.IsEnabled(Cursor[1] + TopIndex[1]) == false) {
					//�����ȃZ�[�u�f�[�^�͍폜�ł��Ȃ�
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					return;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				if(ShowErrorMessage("�Z�[�u�f�[�^ " + (Cursor[1] + TopIndex[1] + 1).tostring() + "�� ���폜���Ă���낵���ł����H|�͂�|������") != 0) {
					return;
				} else if(Game.SaveLoad.DeleteData(Cursor[1] + TopIndex[1]) == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					ShowErrorMessage("�Z�[�u�f�[�^�̍폜�Ɏ��s���܂����B");
					return;
				}
				Game.SaveLoad.GetAllSaveHeader();		//�Z�[�u���X�g�X�V
			} else if(Game.GetKeyInputCounter(KeyID.C) == 1) {
				//C: ����
				if(Game.SaveLoad.IsEnabled(Cursor[1] + TopIndex[1]) == false) {
					//�����ȃZ�[�u�f�[�^�͕��ʂł��Ȃ�
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					return;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				MoveToNextDepth();
				Cursor[Depth] = Cursor[Depth - 1];
				TopIndex[Depth] = TopIndex[Depth - 1];
			}
			break;

		case 2:		//����
			if(CheckDecideKeyInput() == 1) {
				//����: ���ʎ��s (�㏑���m�F�Ȃ�)
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				Game.SaveLoad.CopyData(Cursor[1] + TopIndex[1], Cursor[2] + TopIndex[2]);
				Game.SaveLoad.GetAllSaveHeader();		//�Z�[�u���X�g�X�V
				Depth = 1;
				Cursor[Depth] = Cursor[2];
				TopIndex[Depth] = TopIndex[2];
			}
			break;
	}
}
