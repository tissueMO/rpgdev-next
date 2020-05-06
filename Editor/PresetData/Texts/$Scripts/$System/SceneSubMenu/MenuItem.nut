//###################################################################
//		���j���[�V�[��: �A�C�e�����j���[
//###################################################################

///[summary]�����A�C�e�����X�g��`��
///[par]�J�n�C���f�b�N�X
function DrawItemList(startIndex) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local color = DARKGRAY;		//�f�t�H���g�Ŏg�p�ł��Ȃ��\���F

	for(local i = startIndex, n = 0; n < AreaMax && i < Game.DB.VDBParty.GetItemCountAll(); i++, n++) {
		local data = Game.DB.VDBParty.GetItemDataFromIndex(i);
		color = DARKGRAY;

		//�g�p�ł���ꍇ�͐F��ς���
		switch(db.GetCell(data.ID, "��ʐ���").Integer) {
			case SceneLimit.OnlyMap:
			case SceneLimit.AlwaysOK:
				color = WHITE;
				break;
		}

		//�A�C�R��
		if(EnabledIcons == TRUE) {
			local pos = db.GetCell(data.ID, "�A�C�R��").ConvertToPoint();
			Game.DXLib.DrawRectExtendGraph(
				Game.Material.Graphics.IconGraphicHandle,
				200 + 35,
				20 + n * 25,
				UseIconSize, UseIconSize,
				pos.X * Game.Material.Graphics.IconSize,
				pos.Y * Game.Material.Graphics.IconSize,
				Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
				true
			);
		}

		Game.DXLib.DrawString(200 + 55, 20 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color, data.Name);
		Game.DXLib.DrawString(Game.WindowSize.Width - 100, 20 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color, format("�~ %d", data.Value));
	}

	//�A�C�e���������Ă���ꍇ�̂݃J�[�\���`��
	if(Depth == 1 && Game.DB.VDBParty.GetItemCountAll() > 0) {
		DrawCursorPanel(200 + 15, 12 + 4 + 25 * Cursor[1], Game.WindowSize.Width - 200 - 15 * 2 - 20, 24, true);
		DrawVScrollBar(Game.WindowSize.Width, 0, Game.WindowSize.Height - 30, Game.DB.VDBParty.GetItemCountAll(), TopIndex[1], AreaMax);
	}
}


///[summary]�A�C�e�����X�g��̓��͏���
function InputItemMenu() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	if(CheckUpKeyInput() == true) {
		if(0 < Cursor[Depth] + TopIndex[Depth]) {
			if(0 < Cursor[Depth]) {
				Cursor[Depth]--;
			} else {
				TopIndex[Depth]--;
			}
		}
	} else if(CheckDownKeyInput() == true) {
		if(Cursor[Depth] + TopIndex[Depth] + 1 < Game.DB.VDBParty.GetItemCountAll()) {
			if(Cursor[Depth] + 1 < AreaMax) {
				Cursor[Depth]++;
			} else {
				TopIndex[Depth]++;
			}
		}
	} else if(CheckCancelKeyInput() == 1) {
		//�L�����Z��
		Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
		Depth--;
	} else if(Game.GetKeyInputCounter(KeyID.A) == 1) {
		//��ʏ�����
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		switch(ShowErrorMessage("��ʏ��ɐ������܂��B������I�����Ă��������B|����|�~��|�L�����Z��")) {
			case 0:
				Game.DB.VDBParty.SortItemsByVisibleID(false);
				break;
			case 1:
				Game.DB.VDBParty.SortItemsByVisibleID(true);
				break;
		}
	} else if(Game.GetKeyInputCounter(KeyID.S) == 1) {
		//���O������
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		switch(ShowErrorMessage("���O���ɐ������܂��B������I�����Ă��������B|����|�~��|�L�����Z��")) {
			case 0:
				Game.DB.VDBParty.SortItemsByName(false);
				break;
			case 1:
				Game.DB.VDBParty.SortItemsByName(true);
				break;
		}
	} else if(Game.GetKeyInputCounter(KeyID.D) == 1) {
		//�j��
		local id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;
		if(db.GetCell(id, "�p����").Boolean == false) {
			//�̂Ă��Ȃ��A�C�e��
			Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
			ShowErrorMessage("���̃A�C�e���͔j�����邱�Ƃ��ł��܂���B");
		} else {
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			if(ShowErrorMessage("�{���ɔj�����Ă���낵���ł����H|�͂�|������") == 0) {
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				Game.DB.VDBParty.DeleteItem(id, 1);
				RefreshItemCursor(1);		//�I���ʒu�C��
			}
		}
	} else if(CheckDecideKeyInput() == 1) {
		//����: �A�C�e�����u�g���v
		local id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;
		local idx = -1;
		local used = false;

		//�ړ����Ɏg���Ȃ��A�C�e���͏������Ȃ�
		local limit = db.GetCell(id, "��ʐ���").Integer;
		if(limit == SceneLimit.OnlyBattle || limit == SceneLimit.AlwaysNG) {
			Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
			ShowErrorMessage("���̃A�C�e���͈ړ����Ɏg���܂���B");
			return;
		}

		//�Ώۂ�I�����āA�A�C�e���g�p���������s����
		switch(db.GetCell(id, "�Ώێ��").Integer) {
			case TargetType.None:
				//�����g�p
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = UseItem(id, 0, -1, false);
				break;

			case TargetType.OneParty:
				//�����I��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.One, {["id"]=id}, function(par, idx) {
					return UseItem(par["id"], 0, idx, false);
				});
				break;

			case TargetType.OnePartyWithDead:
				//�����I���E���ґI����
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.OneDead, {["id"]=id}, function(par, idx) {
					return UseItem(par["id"], 0, idx, false);
				});
				break;

			case TargetType.AllParty:
			case TargetType.AllPartyWithDead:
				//�p�[�e�B�S��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.All, {["id"]=id}, function(par, idx) {
					return UseItem(par["id"], 0, -1, false);
				});
				break;

			case TargetType.Place:
				//�ꏊ�I��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				idx = SelectPlace();
				if(idx != -1) {
					used = UseItem(id, 0, idx, false);
				}
				break;

			default:
				//�ړ����ɓG��I�����邱�Ƃ͂ł��Ȃ�
				Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
				ShowErrorMessage("���̃A�C�e�����ړ����Ɏg�����Ƃ͂ł��܂���B");
				break;
		}

		//�����
		if(used == true) {
			Game.DB.VDBParty.DeleteItem(id, 1);
			RefreshItemCursor(1);		//�I���ʒu�C��
		}
	}
}


///[summary]�A�C�e���I���ʒu�̏C�����s��
///[par]�����i���猸�炳�ꂽ��
function RefreshItemCursor(intUsedLength) {
	if(Game.DB.VDBParty.GetItemCountAll() == 0) {
		//�����A�C�e�����[���ɂȂ���
		Depth = 0;
	} else if(Game.DB.VDBParty.GetItemCountAll() <= Cursor[1] + TopIndex[1]) {
		//�J�[�\���ʒu�C��
		if(TopIndex[1] > 0) {
			TopIndex[1] -= intUsedLength;
		} else {
			Cursor[1] -= intUsedLength;
		}
	}
}

