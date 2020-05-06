//###################################################################
//		�V���b�v�V�[��
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\!SceneSubShop_var.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\Common.nut");

//####################  �V�[���N��  ####################
function SceneSubSetup() {
	//�t�F�[�h�C��
	DoFadeObject(
		function() { WindowTransparent = 255; },
		function() { return ((WindowTransparent -= SquareSpeed.Moment) <= 0); }
	);
	UF1222958890/*�������\��*/ = true;
}


//####################  �V�[���`��  ####################
function SceneSubDraw() {
	local offsetX = 30;
	local offsetY = 15;

	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, 255 - WindowTransparent);

	////�X��
	// DrawWindow(0, 0, 250, 40);
	// Game.DXLib.DrawStringC(250 / 2, 9, 0x1B6F049F/*�W���t�H���g*/, WHITE, ShopName);

	//���j���[
	DrawWindow(0, 40, 120, 115);
	foreach(idx in MenuItem) {
		local lineSpace = 30;
		local text = "";
		switch(idx) {
			case MenuItem.Buy:
				text = "�w ��";
				break;
			case MenuItem.Sell:
				text = "�� �p";
				break;
			case MenuItem.Exit:
				text = "�I ��";
				break;
		}
		Game.DXLib.DrawString(offsetX, offsetY + 42 + lineSpace * idx, 0x1B6F049F/*�W���t�H���g*/, WHITE, text);

		//�J�[�\��
		if(idx == Cursor[0]) {
			DrawCursorPanel(15, 11 + 42 + lineSpace * idx, 120 - 15 * 2, 28, (Depth == 0));
		}
	}

	//������
	DrawWindow(0, Game.WindowSize.Height - 32, Game.WindowSize.Width, 32);
	DrawComment(GetMenuComment());

	//�w���E���p�̂݃��C���E�B���h�E��`��
	if(Cursor[0] == MenuItem.Exit) {
		return;
	}

	//���C���̈�
	DrawWindow(120, 40, Game.WindowSize.Width - 120 - 180, Game.WindowSize.Height - 40 - 32);
	DrawWindow(Game.WindowSize.Width - 180, 40, 180, Game.WindowSize.Height - 40 - 32);

	//�ȍ~�̓��j���[�ɉ���������
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local id = -1;
	switch(Cursor[0]) {
		case MenuItem.Buy:
			//���i���X�g
			id = Game.SceneMap.ShopData.Get(Cursor[1] + TopIndex[1]).ID;
			for(local i = (Depth == 0) ? 0 : TopIndex[1], n = 0; n < AreaMax && i < Game.SceneMap.ShopData.GetCount(); i++, n++) {
				local data = Game.SceneMap.ShopData.Get(i);
				local color = WHITE;		//�f�t�H���g�őI���\�ȏ��

				//�w���ł��Ȃ��ꍇ�͐F��ς���
				if(Game.DB.VDBParty.Money < db.GetCell(data.ID, "���i").Integer) {
					color = DARKGRAY;
				}

				//�A�C�R��
				if(EnabledIcons == TRUE) {
					local pos = db.GetCell(data.ID, "�A�C�R��").ConvertToPoint();
					Game.DXLib.DrawRectExtendGraph(
						Game.Material.Graphics.IconGraphicHandle,
						120 + 25,
						60 + n * 25,
						UseIconSize, UseIconSize,
						pos.X * Game.Material.Graphics.IconSize,
						pos.Y * Game.Material.Graphics.IconSize,
						Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
						true
					);
				}

				Game.DXLib.DrawString(120 + 45, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color, data.Name);
				if(ShowAtSign == true) {
					//�P���L����\������ꍇ
					Game.DXLib.DrawString(Game.WindowSize.Width - 120 - 182, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color, "@");
				}
				Game.DXLib.DrawStringR(Game.WindowSize.Width - 120 - 100, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color,
					format("%d ", db.GetCell(data.ID, "���i").Integer) + US1246572451/*�ʉݖ���*/
				);
			}

			//�J�[�\���E������
			if(Depth >= 1 && Game.SceneMap.ShopData.GetCount() > 0) {
				DrawCursorPanel(120 + 15, 55 + 25 * Cursor[1], Game.WindowSize.Width - 120 - 200 - 15 * 2, 25, true);
				DrawVScrollBar(Game.WindowSize.Width - 180, 40, Game.WindowSize.Height - 70, Game.SceneMap.ShopData.GetCount(), TopIndex[1], AreaMax);
				Game.DXLib.DrawStringR(Game.WindowSize.Width - 20, Game.WindowSize.Height - 40 - 20, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
					format("%d �� �����Ă���", Game.DB.VDBParty.GetItemCount(id))
				);
			}
			break;

		case MenuItem.Sell:
			//�����i���X�g
			if(Game.DB.VDBParty.GetItemCountAll() == 0) {
				break;		//����A�C�e���������Ă��Ȃ����
			}
			id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;
			for(local i = (Depth == 0) ? 0 : TopIndex[1], n = 0; n < AreaMax && i < Game.DB.VDBParty.GetItemCountAll(); i++, n++) {
				local data = Game.DB.VDBParty.GetItemDataFromIndex(i);
				local color = WHITE;		//�f�t�H���g�őI���\�ȏ��

				//���p�ł��Ȃ��ꍇ�͐F��ς���
				if(db.GetCell(data.ID, "�p����").Boolean == false) {
					color = DARKGRAY;
				}

				//�A�C�R��
				if(EnabledIcons == TRUE) {
					local pos = db.GetCell(data.ID, "�A�C�R��").ConvertToPoint();
					Game.DXLib.DrawRectExtendGraph(
						Game.Material.Graphics.IconGraphicHandle,
						120 + 25,
						60 + n * 25,
						UseIconSize, UseIconSize,
						pos.X * Game.Material.Graphics.IconSize,
						pos.Y * Game.Material.Graphics.IconSize,
						Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
						true
					);
				}

				Game.DXLib.DrawString(120 + 45, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color,
					data.Name
				);
				Game.DXLib.DrawString(120 + (Game.WindowSize.Width - 120 - 230) - 115, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color,
					format("�~%2d", data.Value)
				);
				if(ShowAtSign == true) {
					//�P���L����\������ꍇ
					Game.DXLib.DrawString(Game.WindowSize.Width - 120 - 182, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color, "@");
				}
				Game.DXLib.DrawStringR(Game.WindowSize.Width - 120 - 100, 60 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color,
					format("%d ", db.GetCell(data.ID, "���i").Integer * SellRate) + US1246572451/*�ʉݖ���*/
				);
			}

			//�J�[�\���`��
			if(Depth >= 1 && Game.DB.VDBParty.GetItemCountAll() > 0) {
				DrawCursorPanel(120 + 15, 55 + 25 * Cursor[1], Game.WindowSize.Width - 120 - 200 - 15 * 2, 25, true);
				DrawVScrollBar(Game.WindowSize.Width - 180, 40, Game.WindowSize.Height - 70, Game.DB.VDBParty.GetItemCountAll(), TopIndex[1], AreaMax);
			}
			break;
	}

	//�I�𒆂̃A�C�e���������i�̂Ƃ��̓p�[�e�B�����o�[�̃p�����[�^�[�ϓ���\������
	if(Depth >= 1 && id != -1 && db.GetCell(id, "�A�C�e������").Integers.Count() > 0) {
		local parDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);
		local itemTypeFID = db.GetCell(id, "�A�C�e������").Integers.Get(0);
		local itemTypeVID = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetCell(itemTypeFID, "ID").Integer;
		if(100 <= itemTypeVID && itemTypeVID < 1000) {
			for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
				local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
				Game.DXLib.DrawString(Game.WindowSize.Width - 160, 55 + i * 90, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, actor.Name);

				//�����`�F�b�N
				if(actor.GetEquipIDFromItemTypeID(itemTypeFID) == id) {
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + 18, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GRAY, "�������Ă���");
					continue;
				}

				//�ϓ��l���擾
				local changes = GetEquipParameterChanges(actor, itemTypeFID, id);
				if(changes == null) {
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + 18, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GRAY, "�����ł��Ȃ�");
					continue;
				}

				local m = 0;
				for(local n = 0; n < changes.len(); n++) {
					if(changes[n] == 0) {
						continue;		//�ϓ����Ȃ��ꍇ�͏������Ȃ�
					}
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + (m + 1) * 18, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
						parDB.GetCellFromIndex(n, DBDefaultColumnIndices.Name).String
					);
					if(changes[n] > 0) {
						//������ꍇ
						Game.DXLib.DrawStringR(Game.WindowSize.Width - 30, 55 + i * 90 + (m + 1) * 18, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GREENYELLOW,
							format("+%d", changes[n])
						);
					} else {
						//����ꍇ
						Game.DXLib.DrawStringR(Game.WindowSize.Width - 30, 55 + i * 90 + (m + 1) * 18, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GRAY,
							changes[n].tostring()
						);
					}
					m++;
				}
				if(m == 0) {
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + 18, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GRAY, "�ϓ��Ȃ�");
				}
			}
		}
	}
}


//####################  �V�[������  ####################
function SceneSubInput() {
	//�V���b�v�̓C�x���g����Ăяo����邽�߁A���Z�b�g�����ƕs����N����
	//if(Game.GetKeyInputCounter(KeyID.F12) == 1) {
	//	GameReset();
	//	return;
	//}

	//�g�b�v��ʂ̗D�揈��
	if(Depth == 0) {
		if(CheckUpKeyInput() == true) {
			Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, MenuItem.len());
		} else if(CheckDownKeyInput() == true) {
			Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, MenuItem.len());
		} else if(CheckDecideKeyInput() == 1) {
			//�G���[�`�F�b�N
			switch(Cursor[0]) {
				case MenuItem.Sell:
					if(Game.DB.VDBParty.GetItemCountAll() == 0) {
						//�A�C�e���������Ă��Ȃ��ꍇ�̓G���[�o���Ė߂�
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("�A�C�e������������Ă��܂���B");
						return;
					}
					break;
				case MenuItem.Exit:
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					CloseMenuScene();
					return;
			}
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			MoveToNextDepth();
		} else if(CheckCancelKeyInput() == 1) {
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
			CloseMenuScene();
		}
		return;
	}

	//�ȍ~�̓��j���[�ɉ���������
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	switch(Cursor[0]) {
		case MenuItem.Buy:
			//���i���X�g
			if(CheckUpKeyInput() == true) {
				if(0 < Cursor[Depth] + TopIndex[Depth]) {
					if(0 < Cursor[Depth]) {
						Cursor[Depth]--;
					} else {
						TopIndex[Depth]--;
					}
				}
			} else if(CheckDownKeyInput() == true) {
				if(Cursor[Depth] + TopIndex[Depth] + 1 < Game.SceneMap.ShopData.GetCount()) {
					if(Cursor[Depth] + 1 < AreaMax) {
						Cursor[Depth]++;
					} else {
						TopIndex[Depth]++;
					}
				}
			} else if(CheckDecideKeyInput() == 1) {
				//����: �A�C�e�����w��
				local id = Game.SceneMap.ShopData.Get(Cursor[1] + TopIndex[1]).ID;

				//�w���ł��Ȃ��A�C�e���͏������Ȃ�
				if(Game.DB.VDBParty.Money < db.GetCell(id, "���i").Integer) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					ShowErrorMessage("����������܂���B");
					return;
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					if(ShowErrorMessage("���̃A�C�e�����w�����Ă�낵���ł����H|�͂�|������") == 0) {
						//�w������
						Game.DB.FDBMaterial.Play(BuySellSEID, 255, 0, 0);
						Game.DB.VDBParty.Money -= db.GetCell(id, "���i").Integer;
						Game.DB.VDBParty.AddItem(id, 1);
					}
				}
			} else if(CheckCancelKeyInput() == 1) {
				//�L�����Z��
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
				Depth--;
			}
			break;

		case MenuItem.Sell:
			//�����i���X�g
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
			} else if(CheckDecideKeyInput() == 1) {
				//����: �A�C�e���𔄋p
				local id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;

				//���p�ł��Ȃ��A�C�e���͏������Ȃ�
				if(db.GetCell(id, "�p����").Boolean == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					ShowErrorMessage("���̃A�C�e���͔��p�ł��܂���B");
					return;
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					if(ShowErrorMessage("���̃A�C�e���𔄋p���Ă�낵���ł����H|�͂�|������") == 0) {
						//���p����
						Game.DB.FDBMaterial.Play(BuySellSEID, 255, 0, 0);
						Game.DB.VDBParty.Money += db.GetCell(id, "���i").Integer * SellRate;
						Game.DB.VDBParty.DeleteItem(id, 1);
						RefreshBuyItemCursor(1);		//�I���ʒu�C��
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
			}
			break;
	}
}


//####################  �V�[���j��  ####################
function SceneSubDispose() {
	UF1222958890/*�������\��*/ = false;

	//���i���X�g���N���A����
	Game.SceneMap.ShopData.Clear();
}


//####################  �g�b�v�K�w�̐���  ####################
function GetMenuComment() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	switch(Cursor[0]) {
		case MenuItem.Buy:
			if(Depth == 0) {
				return "���i���w�����܂��B";
			}
			return db.GetCell(Game.SceneMap.ShopData.Get(Cursor[1] + TopIndex[1]).ID, "����").String;

		case MenuItem.Sell:
			if(Depth == 0) {
				return "�����Ă���A�C�e���𔄋p���܂��B[A]��ʏ��ɐ���  [S]���O���ɐ���";
			}
			return db.GetCell(Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID, "����").String;

		case MenuItem.Exit:
			return "���������I�����܂��B";
	}
	return "";
}


//####################  ���p: �A�C�e���I���ʒu�̏C�����s��  ####################
///[par]�����i���猸�炳�ꂽ��
function RefreshBuyItemCursor(intUsedLength) {
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
