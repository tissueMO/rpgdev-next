//###################################################################
//		���j���[�V�[��: �X�e�[�^�X���j���[
//###################################################################

///[summary]���݂̃L�����̃X�e�[�^�X��`��
///[par]�E���̗̈���X�L�����X�g�Ƃ��ĕ`�悷�邩�ǂ���
function DrawCharStatus(boolEnabledRightArea) {
	//��{���
	local temp = "";
	local actorID = Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]);
	DrawActorPanel(Cursor[1], true);

	//�N���X
	temp = "";
	for(local i = 0; i < Game.DB.VDBActor.GetData(actorID).Classes.Count(); i++) {
		temp += Game.DB.VDBActor.GetData(actorID).Classes.GetName(i) + "  ";
	}
	Game.DXLib.DrawString(420, 60, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "�N���X: " + temp);

	//����
	temp = "";
	for(local i = 0; i < Game.DB.VDBActor.GetData(actorID).Attributes.Count(); i++) {
		temp += Game.DB.VDBActor.GetData(actorID).Attributes.GetName(i) + "  ";
	}
	Game.DXLib.DrawString(420, 80, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "���@��: " + temp);

	//�Ԑڌ���
	temp = "";
	for(local i = 0; i < Game.DB.VDBActor.GetData(actorID).States.Count(); i++) {
		temp += Game.DB.VDBActor.GetData(actorID).States.GetName(i) + "  ";
	}
	Game.DXLib.DrawString(420, 100, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "��@��: " + temp);

	//�����i
	Game.DXLib.DrawString(220, 140, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "�� ��:");
	foreach(idx, equipTypeID in EquipTypeList) {
		local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
		local equipID = Game.DB.VDBActor.GetData(actorID).GetEquipIDFromItemTypeID(equipTypeID);
		local equipName = "---------";
		local typeName = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetCell(
			equipTypeID,
			Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetColumnName(DBDefaultColumnIndices.Name)
			).String;

		if(equipID != -1) {
			equipName = itemDB.GetCell(equipID, Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetColumnName(DBDefaultColumnIndices.Name)).String;

			//�A�C�R��
			if(EnabledIcons == TRUE) {
				local pos = itemDB.GetCell(equipID, "�A�C�R��").ConvertToPoint();
				Game.DXLib.DrawRectExtendGraph(
					Game.Material.Graphics.IconGraphicHandle,
					300,
					158 + idx * 20,
					UseIconSize, UseIconSize,
					pos.X * Game.Material.Graphics.IconSize,
					pos.Y * Game.Material.Graphics.IconSize,
					Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
					true
				);
			}
		}

		Game.DXLib.DrawString(240, 160 + idx * 20, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, typeName + ":");
		Game.DXLib.DrawString(320, 160 + idx * 20, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, equipName);
	}

	//�p�����[�^�[
	local parDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);
	Game.DXLib.DrawString(220, 270, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "�p�����[�^�[:");
	for(local i = 0; i < parDB.GetRowCount(); i++) {
		local id = parDB.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer;
		local name = parDB.GetCellFromIndex(i, DBDefaultColumnIndices.Name).String;
		Game.DXLib.DrawString(240, 290 + 20 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, name + ":");
		Game.DXLib.DrawString(340, 290 + 20 * i, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, format("%4d", Game.DB.VDBActor.GetData(actorID).GetParameter(id).Value));
	}

	//�E���͎��R�̈�Ƃ��ĈႤ���j���[�ł��g���񂹂�悤�ɂ���
	if(boolEnabledRightArea == true) {
		//�o���l
		Game.DXLib.DrawString(420, 135 + 20 * 0, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "�ݐόo���l:");
		Game.DXLib.DrawStringR(Game.WindowSize.Width - 50, 135 + 20 * 0, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, Game.DB.VDBActor.GetData(actorID).EXP.tostring());
		Game.DXLib.DrawString(420, 135 + 20 * 1, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "����Lv�܂�:");
		Game.DXLib.DrawStringR(Game.WindowSize.Width - 50, 135 + 20 * 1, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, Game.DB.VDBActor.GetData(actorID).GetNextEXP().tostring());

		//�K���X�L��
		local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
		Game.DXLib.DrawString(420, 190, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, "�K���X�L��:");
		for(local i = TopIndex[Depth], n = 0; i < Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count() && n < MaxStatusSubListCount; i++, n++) {
			local name = Game.DB.VDBActor.GetData(actorID).LearnedSkills.GetName(i);
			local id = Game.DB.VDBActor.GetData(actorID).LearnedSkills.GetID(i);

			//�A�C�R��
			if(EnabledIcons == TRUE) {
				local pos = db.GetCell(id, "�A�C�R��").ConvertToPoint();
				Game.DXLib.DrawRectExtendGraph(
					Game.Material.Graphics.IconGraphicHandle,
					445,
					158 + 50 + n * 20,
					UseIconSize, UseIconSize,
					pos.X * Game.Material.Graphics.IconSize,
					pos.Y * Game.Material.Graphics.IconSize,
					Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
					true
				);
			}

			Game.DXLib.DrawString(440 + 25, 160 + 50 + 20 * n, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE, name);
		}
		DrawVScrollBar(
			Game.WindowSize.Width - 20,
			190,
			260,
			Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count(),
			TopIndex[Depth],
			MaxStatusSubListCount
		);
	}
}


///[summary]���݂̃L�����̃X�e�[�^�X�̓��͏���
function InputCharStatus() {
	//�Ώۃ����o�[�̃X�e�[�^�X�\��: �㉺�L�[����ŃX�L�����X�g���X�N���[������
	local actorID = Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]);
	if(CheckUpKeyInput() == true) {
		//�X�N���[���ł���ꍇ�̂ݏ�������
		if(Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count() > MaxStatusSubListCount) {
			TopIndex[Depth]--;
			if(TopIndex[Depth] < 0) {
				TopIndex[Depth]++;		//����ȏ��ɍs���Ȃ�
			}
		}
	} else if(CheckDownKeyInput() == true) {
		//�X�N���[���ł���ꍇ�̂ݏ�������
		if(Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count() > MaxStatusSubListCount) {
			TopIndex[Depth]++;
			if(TopIndex[Depth] + MaxStatusSubListCount > Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count()) {
				TopIndex[Depth]--;		//����ȏ㉺�ɍs���Ȃ�
			}
		}
	} else if(CheckDecideKeyInput() == 1) {
		//����: �g�b�v�ɖ߂�
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		Depth = 0;
	} else if(CheckCancelKeyInput() == 1) {
		//�L�����Z��: ��O�̊K�w�ɖ߂�
		Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
		Depth--;
	}
}


///[summary]�������ʂ̃��X�g���X�V
///[comment]�������X�g��\������O�ɍX�V����K�v������܂��B
function RefreshEquipTypeList() {
	EquipTypeList.resize(0);
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType);
	for(local i = 0; i < Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetRowCount(); i++) {
		if(db.GetCellFromIndex(i, db.GetColumnIndex("�����i")).Boolean == false) {
			continue;		//�����i�ł͂Ȃ��A�C�e�����
		}
		EquipTypeList.append(db.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer);
	}
}
