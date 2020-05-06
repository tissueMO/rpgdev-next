//###################################################################
//		���j���[�V�[��: �ėp����
//###################################################################

///[summary]�������̃e�L�X�g���`
///[return]�������ɕ\�����ׂ��e�L�X�g
function GetMenuComment() {
	//�D�揈��
	if(TargetMemberSelecting == true) {
		if(Cursor[TargetMemberDepth] != -1) {
			return "�ΏۂƂȂ�p�[�e�B�L������I�����ĉ������B";
		} else {
			return "�p�[�e�B�S�̂��ΏۂƂȂ�܂��B";
		}
	} else if(PlaceSelecting == true) {
		return "�ꏊ��I�����ĉ������B";
	}

	//�K�w 1~2 �͋��ʏ���
	switch(Depth) {
		case 0:
			switch(Cursor[0]) {
				case MenuItem.Status:
					return "�p�[�e�B�L�����̃X�e�[�^�X���m�F���܂��B";
				case MenuItem.Item:
					return "�����Ă���A�C�e�����g�p���܂��B[D]�j��  [A]��ʏ��ɐ���  [S]���O���ɐ���";
				case MenuItem.Equip:
					return "�p�[�e�B�L�����̑�����ύX���܂��B";
				case MenuItem.Skill:
					return "�p�[�e�B�L�����̈ړ����ɗL���ȃX�L�����g�p���܂��B";
				case MenuItem.Member:
					return "�p�[�e�B�L�����̕��ёւ����s���܂��B";
				case MenuItem.Save:
					return "���݂̃f�[�^���Z�[�u���܂��B";
				case MenuItem.Option:
					return "�Q�[���̐ݒ���s���܂��B";
			}
			break;

		case 1:
			switch(Cursor[0]) {
				case MenuItem.Status:
				case MenuItem.Equip:
				case MenuItem.Skill:
				case MenuItem.Member:
					return "�����o�[��I�����ĉ������B";
				case MenuItem.Item:
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					return db.GetCell(Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID, "����").String;
				case MenuItem.Save:
					return "�Z�[�u�f�[�^��I�����ĉ������B[C]����  [D]�폜";
				case MenuItem.Option:
					return "���E�L�[�Őݒ�l��ύX���܂��B�ύX���ʂ͑����ɔ��f����܂��B";
			}
			break;
	}

	//�K�w 2~ �̓��j���[���Ƃɏ���
	switch(Cursor[0]) {
		case MenuItem.Status:	//�Ώۃ����o�[�̃X�e�[�^�X�\��: �p�����[�^�[�E�N���X�E�����E�����E�K���X�L��
			local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Char).GetSubDB(0);
			return db.GetCell(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]), "������").String;

		case MenuItem.Equip:
			switch(Depth) {
				case 2:			//�Ώۃ����o�[�̑������ʑI��
					return "�����i�̎�ʂ�I�����ĉ������B";
				case 3:			//�����i�I��
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					if(Cursor[3] + TopIndex[3] == 0) {
						return "�������Ȃ���Ԃɂ��܂��B";
					} else {
						return db.GetCell(EquipList[Cursor[3] + TopIndex[3]].ID, "����").String;
					}
			}
			break;

		case MenuItem.Skill:	//�Ώۃ����o�[�̃X�L���I��
			local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
			local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
			return db.GetCell(actor.LearnedSkills.GetID(Cursor[2] + TopIndex[2]), "����").String;

		case MenuItem.Member:	//�Ώۃ����o�[�̓��֐�I��
			return "�Ώۃ����o�[�̈ړ����I�����ĉ������B";

		case MenuItem.Save:		//���ʐ�I��
			return "���ʐ�̃Z�[�u�f�[�^��I�����ĉ������B";
	}
	return "";
}


///[summary]���j���[�����
function CloseMenuScene() {
	//�t�F�[�h�A�E�g
	DoFadeObject(
		function() { WindowTransparent = 0; },
		function() { return ((WindowTransparent += SquareSpeed.Moment) >= 255); }
	);
	Game.EndSubScene();
}


///[summary]�������ɔC�ӂ̃e�L�X�g��\���E�I��������
///[par]�\���e�L�X�g�B�I�����ɂ���ꍇ�� | �ŋ�؂�A�ŏ��̗v�f�����b�Z�[�W�ɂȂ�܂��B
///[return]�I�����ꂽ���ڂ̃C���f�b�N�X�B�L�����Z�����ꂽ�ꍇ�� -1 ���Ԃ���܂��B
function ShowErrorMessage(strText) {
	ErrorMsg = strText;
	Cursor[ChoiceDepth] = 0;
	while(ErrorMsg != "" && Game.Wait(0, true, true)) {
		if(ErrorMsg.find("|") == null) {
			//�P�Ȃ郁�b�Z�[�W�\��
			if(CheckDecideKeyInput() == 1) {
				//����L�[�Ń��b�Z�[�W����
				ErrorMsg = "";
			}
		} else {
			//�I����: �u|�v�̂Q�ڈȍ~�����ڂƂ݂Ȃ�
			local spl = split(ErrorMsg, "|");
			if(CheckLeftKeyInput() == true) {
				Cursor[ChoiceDepth] = Game.DownAround(Cursor[ChoiceDepth], 0, spl.len() - 1);
			} else if(CheckRightKeyInput() == true) {
				Cursor[ChoiceDepth] = Game.UpAround(Cursor[ChoiceDepth], 0, spl.len() - 1);
			} else if(CheckDecideKeyInput() == 1) {
				//����L�[�őI�����I��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				ErrorMsg = "";
			} else if(CheckCancelKeyInput() == 1) {
				//�L�����Z��: �J�[�\���ʒu���}�C�i�X�ɂ���
				Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
				ErrorMsg = "";
				Cursor[ChoiceDepth] = -1;
			}
		}
	}

	//�I�����̌��ʂ�Ԃ�
	if(strText.find("|") != null) {
		print("�I�����ꂽ���ڂ̃C���f�b�N�X [" + Cursor[ChoiceDepth].tostring() + "]");
		return Cursor[ChoiceDepth];
	} else {
		return -1;
	}
}


///[summary]�A�N�^�[�Ɏw�肵���A�C�e���𑕔������Ƃ��̑S�p�����[�^�[�����ʂ�z��ŕԂ�
///[par]�ΏۃA�N�^�[ (CDBActorData)
///[par]�A�C�e������ID
///[par]���������悤�Ƃ��Ă���A�C�e����ID
///[comment]���я��̓f�[�^�x�[�X��ł̏����ɂȂ�܂��B�A�h�I���ɂ��ϓ��l�͍l�����܂���B
///[return]�����ʂ�����int�^�̔z��B�����ł��Ȃ��ꍇ�� null ���Ԃ���܂��B
function GetEquipParameterChanges(actorData, equipType, afterEquipID) {
	local parDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);
	local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local deltaList = array(parDB.GetRowCount(), 0);

	//�G���[�`�F�b�N
	if(afterEquipID != -1 && actorData.CheckEquip(afterEquipID) == false) {
		return null;	//�����ł��Ȃ��ꍇ�͏������Ȃ�
	}

	//���݂̑����i���擾����
	local beforeEquipID = actorData.GetEquipIDFromItemTypeID(equipType);

	//�����l�v�Z: �ȑO�̑����i�ƑΏۂ̑����i�œ����p�����[�^�[�̕ϓ��l�̍������
	for(local i = 0; i < deltaList.len(); i++) {
		local parID = parDB.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer;
		local before = ((beforeEquipID == -1) ? 0 :
			((itemDB.GetCell(beforeEquipID, "�p�����[�^�[").IDValues.Exists(parID) == false) ? 0 :
				itemDB.GetCell(beforeEquipID, "�p�����[�^�[").IDValues.GetValue(parID)));
		local after = ((afterEquipID == -1) ? 0 :
			((itemDB.GetCell(afterEquipID, "�p�����[�^�[").IDValues.Exists(parID) == false) ? 0 :
				itemDB.GetCell(afterEquipID, "�p�����[�^�[").IDValues.GetValue(parID)));
		deltaList[i] = after - before;
	}
	return deltaList;
}


///[summary]�ΏۃL������I��
///[par]�Ώێ�� (SelectMemberType)
///[par]�֐��I�u�W�F�N�g�̈����ƂȂ�e�[�u��
///[par]�I�������Ƃ��ɍs���������e���`�����֐��I�u�W�F�N�g
///[comment]��3�����Ɋ֐��I�u�W�F�N�g���������Ƃ��A�I����Ɏ��s�����֐��̖߂�l�����̂܂ܕԂ���܂��B��3������null�̂Ƃ��A�Ԓl�͑I�����ꂽ�p�[�e�B�����o�[�̃C���f�b�N�X�ŁA�S�̂�I�������ꍇ�� 0 ���Ԃ���܂��B
///[return]�L�����Z�����ꂽ�ꍇ�� -1 ���Ԃ���܂��B
function SelectTargetMember(selectType, par, funcSelectedHandler) {
	local result = null;
	TargetMemberSelecting = true;
	Cursor[TargetMemberDepth] = (selectType == SelectMemberType.All) ? -1 : 0;
	TopIndex[TargetMemberDepth] = 0;

	while(Game.Wait(0, true, true)) {
		if(CheckUpKeyInput() == true) {
			if(selectType != SelectMemberType.All) {
				Cursor[TargetMemberDepth] = Game.DownAround(Cursor[TargetMemberDepth], 0, Game.DB.VDBParty.GetCharCount());
			}
		} else if(CheckDownKeyInput() == true) {
			if(selectType != SelectMemberType.All) {
				Cursor[TargetMemberDepth] = Game.UpAround(Cursor[TargetMemberDepth], 0, Game.DB.VDBParty.GetCharCount());
			}
		} else if(CheckDecideKeyInput() == 1) {
			//����
			//�G���[�`�F�b�N
			if(selectType == SelectMemberType.One) {
				//���҂�I���ł��Ȃ��P�̑I���̂Ƃ��A�I�������o�[������ł��Ȃ������ׂ�
				if(IsDead(Cursor[TargetMemberDepth]) == true) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					ShowErrorMessage("����ł��郁���o�[�͑I���ł��܂���B");
					continue;
				}
			}
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);

			//���莞�̏������w�肳��Ă���Ƃ��͂����Ŏ��s����
			if(funcSelectedHandler != null) {
				result = funcSelectedHandler(par, Cursor[TargetMemberDepth]);
				Game.Wait(1.0, true, false);		//��ʏ�̕ω��������邽�߂Ɉ�莞�ԑҋ@
			}

			//�S�̂�I�����邱�Ƃ��m�肵���Ƃ��A�J�[�\���ʒu�� 0 �ɖ߂�
			if(selectType == SelectMemberType.All) {
				Cursor[TargetMemberDepth] = 0;
			}
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//�L�����Z��: �J�[�\���ʒu���}�C�i�X�ɂ���
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
			Cursor[TargetMemberDepth] = -1;
			break;
		}
	}
	TargetMemberSelecting = false;

	//�I�����ʂ�Ԃ�
	return (result != null) ? result : Cursor[TargetMemberDepth];
}


///[summary]�ꏊ��I��
///[return]���[�U�[DB��̃C���f�b�N�X�B�L�����Z�����ꂽ�ꍇ�� -1 ���Ԃ���܂��B
function SelectPlace() {
	PlaceSelecting = true;
	Cursor[PlaceDepth] = 0;
	TopIndex[PlaceDepth] = 0;

	//�ꏊ���X�g���쐬
	PlaceList = array(0);
	local db = Game.DB.UDB.GetUserDB(0x2C80399D/*�ꏊ���X�g*/);
	for(local i = 0; i < db.GetRowCount(); i++) {
		//�L���ȏꏊ�̂݃��X�g�ɓ����
		if(db.GetCellFromIndex(i, db.GetColumnIndex("�K���")).Boolean == true) {
			local newData = CIDNameData();
			newData.ID = i;
			newData.Name = db.GetCellFromIndex(i, db.GetColumnIndex("�\����")).String;
			PlaceList.append(newData);
		}
	}

	//�L���ȏꏊ������Ȃ��ꍇ�͏������Ȃ�
	if(PlaceList.len() == 0) {
		ShowErrorMessage("�L���ȏꏊ���������܂���B");
		return -1;
	}

	//���͎�t
	while(Game.Wait(0, true, true)) {
		if(CheckUpKeyInput() == true) {
			if(0 < Cursor[PlaceDepth] + TopIndex[PlaceDepth]) {
				if(0 < Cursor[PlaceDepth]) {
					Cursor[PlaceDepth]--;
				} else {
					TopIndex[PlaceDepth]--;
				}
			}
		} else if(CheckDownKeyInput() == true) {
			if(Cursor[PlaceDepth] + TopIndex[PlaceDepth] + 1 < PlaceList.len()) {
				if(Cursor[PlaceDepth] + 1 < AreaMax) {
					Cursor[PlaceDepth]++;
				} else {
					TopIndex[PlaceDepth]++;
				}
			}
		} else if(CheckDecideKeyInput() == 1) {
			//����
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//�L�����Z��: �J�[�\���ʒu���}�C�i�X�ɂ���
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
			Cursor[PlaceDepth] = -1;
			break;
		}
	}
	PlaceSelecting = false;

	//�I�����ʂƂȂ�J�[�\���ʒu���ꏊ���X�g��̃C���f�b�N�X�ɕϊ�
	local visitedCount = 0;
	local selectedIndex = -1;
	for(local i = 0; i < db.GetRowCount(); i++) {
		if(db.GetCellFromIndex(i, db.GetColumnIndex("�K���")).Boolean == true) {
			visitedCount++;
			if (Cursor[PlaceDepth] + 1 == visitedCount) {
				//�K��ς̍��ڐ��Ő����č��v�������ڂ̃C���f�b�N�X�ŕԂ�
				selectedIndex = i;
				break;
			}
		}
	}

	//�I�����ʂ�Ԃ�
	return selectedIndex;
}


///[summary]�������̕`��
///[par]�e�L�X�g
function DrawComment(text) {
	if(ErrorMsg == "") {
		//�W���̐�����\��
		Game.DXLib.DrawString(16, Game.WindowSize.Height - 23, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, text);
		return;
	}

	local spl = split(ErrorMsg, "|");
	if(spl.len() == 1) {
		//�P�Ȃ郁�b�Z�[�W�\��
		Game.DXLib.DrawString(16, Game.WindowSize.Height - 23, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, ErrorMsg + "��");
		return;
	}

	//�I����: �u|�v�̂Q�ڈȍ~�����ڂƂ݂Ȃ�
	Game.DXLib.DrawString(16, Game.WindowSize.Height - 23, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, spl[0]);
	for(local i = 1; i < spl.len(); i++) {
		Game.DXLib.DrawString(
			Game.WindowSize.Width * 4/7 + (Game.WindowSize.Width * 3/7 / (spl.len() - 1) * (i - 1)),
			Game.WindowSize.Height - 23,
			0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, spl[i]
		);

		//�I���ʒu�ɃJ�[�\����`��
		if((i - 1) != Cursor[ChoiceDepth]) {
			continue;
		}
		DrawCursorPanel(
			Game.WindowSize.Width * 4/7 + (Game.WindowSize.Width * 3/7 / (spl.len() - 1) * (i - 1)) - 5,
			Game.WindowSize.Height - 23,
			(Game.WindowSize.Width * 3/7 / (spl.len() - 1)) - 8,
			15, true
		);
	}
}


///[summary]�ΏۃL�����I���̕`�揈��
function DrawTargetMemberSelect() {
	//�A�N�^�[�p�l����`��
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		DrawActorPanel(i, false);
		if(Cursor[TargetMemberDepth] == -1) {
			//�J�[�\���� -1 �̂Ƃ��͑S�̑I����Ԃɂ���
			DrawCursorPanel(200 + 15, 12 + 110 * i, Game.WindowSize.Width - 200 - 15 * 2, 95, true);
		}
	}

	//�J�[�\����`��
	if(Cursor[TargetMemberDepth] != -1) {
		DrawCursorPanel(200 + 15, 12 + 110 * Cursor[TargetMemberDepth], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
	}
}


///[summary]�ꏊ�I���̕`�揈��
function DrawPlaceSelect() {
	if(PlaceList == null) {
		return;		//�ꏊ���X�g���쐬����Ă��Ȃ��ꍇ�͏������Ȃ�
	}
	for(local i = TopIndex[PlaceDepth], n = 0; n < AreaMax && i < PlaceList.len(); i++, n++) {
		Game.DXLib.DrawString(200 + 40, 20 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, PlaceList[i].Name);
	}
	DrawCursorPanel(200 + 15, 12 + 4 + 25 * Cursor[PlaceDepth], Game.WindowSize.Width - 200 - 15 * 2 - 20, 24, true);
	DrawVScrollBar(Game.WindowSize.Width, 0, Game.WindowSize.Height - 30, PlaceList.len(), TopIndex[PlaceDepth], AreaMax);
}


///[summary]�A�N�^�[�p�l����`��
///[par]�p�[�e�B�̃C���f�b�N�X
///[par]�C���f�b�N�X�ɍ��킹���ʒu�ł͂Ȃ��A�C���f�b�N�X 0 �̈ʒu�ɕ`�悷�邩�ǂ���
function DrawActorPanel(ptidx, isAbsolutely) {
	if(ptidx < 0 || Game.DB.VDBParty.GetCharCount() <= ptidx) {
		return;		//�����ȃp�[�e�B�C���f�b�N�X�͏������Ȃ�
	}

	local panelHeight = 110;
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(ptidx));
	if(isAbsolutely == true) {
		panelHeight = 0;		//��Έʒu�Œ�̏ꍇ�̓C���f�b�N�X�Ɋւ�炸�������W�ɕ`�悷��
	}

	//�O���t�B�b�N
	local charSize = Game.DXLib.GetGraphSize(actor.GraphHandle);
	charSize.Set(charSize.Width / EventAnimation.Count, charSize.Height / Direction4.None);
	Game.DXLib.DrawRectGraph(
		actor.GraphHandle,
		LeftOffset - 75,
		20 + ptidx * panelHeight + 10 + (64 - charSize.Height),
		charSize.Width * EventAnimation.Stop,
		charSize.Height * Direction4.South,
		charSize.Width, charSize.Height,
		true, false
	);

	//���O
	Game.DXLib.DrawString(
		LeftOffset - 110,
		20 + ptidx * panelHeight,
		0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE,
		actor.Name
	);

	//HP�\��
	Game.DXLib.DrawString(
		LeftOffset,
		20 + ptidx * panelHeight,
		0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
		format(HPName + ": %4d /%4d", actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value, actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value)
	);
	DrawMeterPattern(
		LeftOffset + MeterOffset,
		22 + ptidx * panelHeight,
		MeterWidth,
		MeterHeight,
		HPMeterPatternStart,
		HPMeterPatternCount,
		(actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value == 0) ? 0 : (MeterWidth * actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value / actor.GetParameter(0x2019EC10/*�ő�g�o*/).Value)
	);

	//MP�\��
	Game.DXLib.DrawString(
		LeftOffset,
		20 + ptidx * panelHeight + 20,
		0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
		format(MPName + ": %4d /%4d", actor.SelfValues.Get(0x46A20EE9/*MP*/).Value, actor.GetParameter(0x9A3FB31/*�ő�l�o*/).Value)
	);
	DrawMeterSolid(
		LeftOffset + MeterOffset,
		40 + ptidx * panelHeight,
		MeterWidth,
		MeterHeight,
		MPMeterPattern,
		(actor.GetParameter(0x9A3FB31/*�ő�l�o*/).Value == 0) ? 0 : (MeterWidth * actor.SelfValues.Get(0x46A20EE9/*MP*/).Value / actor.GetParameter(0x9A3FB31/*�ő�l�o*/).Value)
	);

	//���x��
	Game.DXLib.DrawString(
		LeftOffset,
		20 + ptidx * panelHeight + 40,
		0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
		format("Lv: %d", actor.Level)
	);

	//�Ԑڌ��ʃA�C�R��
	for(local i = 0; i < actor.States.Count() && i < StateIconMaxCount; i++) {
		local pos = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0).GetCell(actor.States.GetID(i), "�A�C�R��").ConvertToPoint();
		Game.DXLib.DrawRectExtendGraph(
			Game.Material.Graphics.IconGraphicHandle,
			LeftOffset + i * (StateIconSize + 2),
			20 + ptidx * panelHeight + 60,
			StateIconSize, StateIconSize,
			pos.X * Game.Material.Graphics.IconSize,
			pos.Y * Game.Material.Graphics.IconSize,
			Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
			true
		);
	}
}
