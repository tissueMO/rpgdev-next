//###################################################################
//		���j���[�V�[��: �������j���[
//###################################################################

///[summary]���݂̃L�����̑�����ʂ̕`��
function DrawEquipMenu() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	DrawCharStatus(false);
	DrawCursorPanel(230, 156 + 20 * Cursor[2], 190, 21, (Depth == 2));		//�����i��ʂ̃J�[�\��

	//�����i���X�g
	local cur = (Depth >= 3) ? Cursor[3] : 0;
	local top = (Depth >= 3) ? TopIndex[3] : 0;
	Game.DXLib.DrawString(420, 140, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "�����i���X�g:");
	for(local i = top, n = 0; i < EquipList.len() && n < MaxStatusSubListCount; i++, n++) {
		local color = DARKGRAY;
		if(EquipList[i].ID == -1
		|| Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1])).CheckEquip(EquipList[i].ID) == true) {
			//�u�������O���v or �����\�ȃA�C�e���̂ݗL���F�ɂ���
			color = WHITE;
		}

		//�A�C�R��
		if(EnabledIcons == TRUE && EquipList[i].ID != -1) {
			local pos = db.GetCell(EquipList[i].ID, "�A�C�R��").ConvertToPoint();
			Game.DXLib.DrawRectExtendGraph(
				Game.Material.Graphics.IconGraphicHandle,
				440,
				158 + n * 20,
				UseIconSize, UseIconSize,
				pos.X * Game.Material.Graphics.IconSize,
				pos.Y * Game.Material.Graphics.IconSize,
				Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
				true
			);
		}

		Game.DXLib.DrawString(460, 160 + 20 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, color, EquipList[i].Name);
		if(EquipList[i].Value > 0) {
			Game.DXLib.DrawString(580, 160 + 20 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, color, format("�~ %d", EquipList[i].Value));
		}
	}

	if(Depth >= 3) {
		//�����i�I��
		//�J�[�\���ƃX�N���[���o�[
		DrawCursorPanel(425, 156 + 20 * cur, 190, 21, (Depth == 3));
		DrawVScrollBar(Game.WindowSize.Width - 20, 145, 300, EquipList.len(), TopIndex[3], MaxStatusSubListCount);

		//�p�����[�^�[�����l�v���r���[
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
		local changes = GetEquipParameterChanges(actor, EquipTypeList[Cursor[2]], EquipList[Cursor[3] + TopIndex[3]].ID);
		if(changes != null) {
			for(local i = 0; i < changes.len(); i++) {
				if(changes[i] < 0) {
					Game.DXLib.DrawString(375, 290 + 20 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GRAY, changes[i].tostring());
				} else if(changes[i] > 0) {
					Game.DXLib.DrawString(375, 290 + 20 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, GREENYELLOW, format("+%d", changes[i]));
				}
			}
		}
	}
}


///[summary]���݂̃L�����̑�����ʂ̓���
function InputEquipMenu() {
	switch(Depth) {
		case 2:
			//�������ʑI��
			if(CheckUpKeyInput() == true) {
				Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, EquipTypeList.len());
				RefreshEquipList(EquipTypeList[Cursor[Depth]]);
			} else if(CheckDownKeyInput() == true) {
				Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, EquipTypeList.len());
				RefreshEquipList(EquipTypeList[Cursor[Depth]]);
			} else if(CheckDecideKeyInput() == 1) {
				//����
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				MoveToNextDepth();
			} else if(CheckCancelKeyInput() == 1) {
				//�L�����Z��
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
				Depth--;
			}
			break;

		case 3:
			//�����i�I��
			if(CheckUpKeyInput() == true) {
				if(0 < Cursor[Depth] + TopIndex[Depth]) {
					if(0 < Cursor[Depth]) {
						Cursor[Depth]--;
					} else {
						TopIndex[Depth]--;
					}
				}
			} else if(CheckDownKeyInput() == true) {
				if(Cursor[Depth] + TopIndex[Depth] + 1 < EquipList.len()) {
					if(Cursor[Depth] < MaxStatusSubListCount) {
						Cursor[Depth]++;
					} else {
						TopIndex[Depth]++;
					}
				}
			} else if(CheckDecideKeyInput() == 1) {
				//����: ��������
				local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
				if(EquipList[Cursor[3] + TopIndex[3]].ID != -1
				&& actor.CheckEquip(EquipList[Cursor[3] + TopIndex[3]].ID) == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					ShowErrorMessage("���̃L�����ɑ��������邱�Ƃ͂ł��܂���B");
					break;
				}

				//�X�V�O�ɑ������Ă����A�C�e�������ׂċL������
				local beforeEquipIDs = {};
				foreach(idx, typeID in EquipTypeList) {
					beforeEquipIDs[typeID] <- actor.GetEquipIDFromItemTypeID(typeID);
				}

				if(EquipList[Cursor[3] + TopIndex[3]].ID == -1) {
					//�������O��
					if(actor.ReleaseEquipType(EquipTypeList[Cursor[2]], false) == false) {
						//�������O���Ȃ�����
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("���݂��̃L�����͑�����ύX�ł��Ȃ���ԂɂȂ��Ă��܂��B");
						break;
					}
				} else {
					//��������
					if(actor.SetEquip(EquipList[Cursor[3] + TopIndex[3]].ID, false) == false) {
						//�����Ɏ��s
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("���݂��̃L�����͑�����ύX�ł��Ȃ���ԂɂȂ��Ă��܂��B");
						break;
					}
				}
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);

				//HP�EMP���ő�l�𒴂��Ȃ��悤�Ɏ����␳
				FitToMaxParameters(actor);

				//�������Ă����A�C�e���������A�C�e���ɖ߂�
				local isSelected = false;
				local addItems = array(0);
				foreach(equipTypeID, beforeEquipID in beforeEquipIDs) {
					local appended = false;
					if(beforeEquipID != -1) {
						foreach(i, itemID in addItems) {
							if(beforeEquipID == itemID) {
								appended = true;
								break;
							}
						}
					}
					if(appended || beforeEquipID == actor.GetEquipIDFromItemTypeID(equipTypeID)) {
						//���ɊO���������i or �������ω����Ȃ��������ʂ̓X�L�b�v
						continue;
					}

					//���Ȃ��Ƃ��������ɕω����������Ƃ��ă}�[�N���Ă���
					isSelected = true;
					if(beforeEquipID == -1) {
						//�O�������i���Ȃ��̂ŃX�L�b�v
						continue;
					}

					//�O���������i���A�C�e���ꗗ�ɖ߂�
					Game.DB.VDBParty.AddItem(beforeEquipID, 1);

					//�����̑��������̂���P��̃A�C�e�����d�����Ė߂��Ȃ��悤�ɂ���
					addItems.append(beforeEquipID);
				}

				//���ۂɑ����i��I�񂾏ꍇ
				if(isSelected == true && EquipList[Cursor[3] + TopIndex[3]].ID != -1) {
					//�����A�C�e�����猸�炷
					Game.DB.VDBParty.DeleteItem(EquipList[Cursor[3] + TopIndex[3]].ID, 1);
				}

				//�����i���X�g���X�V����
				RefreshEquipList(EquipTypeList[Cursor[2]]);
				if(EquipList.len() <= Cursor[3] + TopIndex[3]) {
					//�J�[�\���ʒu�␳
					if(0 < TopIndex[3]) {
						TopIndex[3]--;
					} else {
						Cursor[3]--;
					}
				}
			} else if(CheckCancelKeyInput() == 1) {
				//�L�����Z��
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
				Depth--;
			}
			break;
	}
}


///[summary]�����A�C�e�����X�g����w�肵���������ʂɍi�荞��
///[par]�A�C�e������ID
function RefreshEquipList(intEquipTypeID) {
	EquipList.resize(0);

	//�������O�����߂̍��ڂ�p�ӂ���
	local emptyData = CIDValueData();
	emptyData.ID = -1;
	emptyData.Name = "(�������O��)";
	emptyData.Value = -1;
	EquipList.append(emptyData);

	//�w�肳�ꂽ�������ʂɊY������A�C�e���̂݃��X�g�ɓo�^����
	for(local i = 0; i < Game.DB.VDBParty.GetItemCountAll(); i++) {
		local data = Game.DB.VDBParty.GetItemDataFromIndex(i);
		if(Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(data.ID, "�A�C�e������").Integers.Exists(intEquipTypeID) == true) {
			EquipList.append(data);
		}
	}
}
