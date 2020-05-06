//###################################################################
//		���j���[�V�[��: ���ёւ����j���[
//###################################################################

///[summary]�����o�[���X�g�̕`�揈��
function DrawMemberMenu() {
	//�A�N�^�[�p�l��
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		DrawActorPanel(i, false);
	}

	//�J�[�\��
	switch(Depth) {
		case 1:
			DrawCursorPanel(200 + 15, 12 + 110 * Cursor[1], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
			break;
		case 2:
			//�ړ���
			DrawCursorArrow(225, 12 + 110 * Cursor[1], Direction4.East, false);
			//�ړ���
			DrawCursorPanel(200 + 15, 12 + 110 * Cursor[2], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
			break;
	}
}


///[summary]�����o�[���X�g��̓��͏���
function InputMemberMenu() {
	//�ړ���I��
	if(CheckUpKeyInput() == true) {
		Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
	} else if(CheckDownKeyInput() == true) {
		Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
	} else if(CheckDecideKeyInput() == 1) {
		//����: �������s
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		local id = Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]);

		//�L�����ړ�
		Game.DB.VDBParty.DeleteMemberFromIndex(Cursor[1]);
		if(Cursor[2] + 1 <= Game.DB.VDBParty.GetCharCount()) {
			Game.DB.VDBParty.AddMember(id, Cursor[2]);
		} else {
			Game.DB.VDBParty.AddMember(id, -1);
		}
		SetHeadCharGraphics();		//�擪�L�����̃O���t�B�b�N��ύX

		//�ړ��Ώۂ̑I���ɖ߂�
		Depth = 1;
		Cursor[Depth] = Cursor[Depth + 1];
	} else if(CheckCancelKeyInput() == 1) {
		//�L�����Z��
		Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
		Depth--;
	}
}
