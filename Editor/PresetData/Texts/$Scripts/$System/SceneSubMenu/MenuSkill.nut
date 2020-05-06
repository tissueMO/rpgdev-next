//###################################################################
//		���j���[�V�[��: �X�L�����j���[
//###################################################################

///[summary]���݂̃L�����̃X�L�����X�g��`��
function DrawSkillList(startIndex) {
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
	local MP = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
	local MMP = actor.GetParameter(0x9A3FB31/*�ő�l�o*/).Value;

	//�L������
	Game.DXLib.DrawString(LeftOffset - 110, 20, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, WHITE, actor.Name);

	//MP�\��
	Game.DXLib.DrawString(LeftOffset, 20, 0x2E19E6A3/*�Z�[�u�f�[�^���X�g�p*/, WHITE,
		format(MPName + ": %4d /%4d", MP, MMP)
	);
	DrawMeterSolid(
		LeftOffset + MeterOffset,
		22,
		MeterWidth,
		MeterHeight,
		MPMeterPattern,
		(MMP == 0) ? 0 : (MeterWidth * MP / MMP)
	);

	//�X�L�����X�g
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	for(local i = startIndex, n = 0; n < AreaMax && i < actor.LearnedSkills.Count(); i++, n++) {
		local id = actor.LearnedSkills.GetID(i);
		local color = DARKGRAY;		//�f�t�H���g�Ŏg�p�ł��Ȃ��\���F
		local requireMP = db.GetCell(id, "�R�X�g").Integers.Get(0);

		//��ʔ���
		switch(db.GetCell(id, "��ʐ���").Integer) {
			case SceneLimit.OnlyMap:
			case SceneLimit.AlwaysOK:
				//�g�p�۔���
				if(MP >= requireMP && actor.CheckUseSkill(id) == true) {
					color = WHITE;		//�g�p�\
				}
				break;
		}

		//�A�C�R��
		if(EnabledIcons == TRUE) {
			local pos = db.GetCell(id, "�A�C�R��").ConvertToPoint();
			Game.DXLib.DrawRectExtendGraph(
				Game.Material.Graphics.IconGraphicHandle,
				235,
				80 + n * 25,
				UseIconSize, UseIconSize,
				pos.X * Game.Material.Graphics.IconSize,
				pos.Y * Game.Material.Graphics.IconSize,
				Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
				true
			);
		}

		Game.DXLib.DrawString(200 + 55, 80 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color,
			actor.LearnedSkills.GetName(i)
		);
		Game.DXLib.DrawString(Game.WindowSize.Width - 150, 80 + n * 25, 0x6DD57515/*�E�B���h�E���e�L�X�g*/, color,
			format("����" + MPName + ":%4d", requireMP)
		);
	}

	DrawCursorPanel(200 + 15, 80 - 4 + 25 * Cursor[2], Game.WindowSize.Width - 200 - 15 * 2 - 20, 24, true);
	DrawVScrollBar(Game.WindowSize.Width, 80, 370, actor.LearnedSkills.Count(), TopIndex[2], AreaMax);
}


///[summary]���݂̃L�����̃X�L�����X�g��̓��͏���
function InputSkillMenu() {
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));

	if(CheckUpKeyInput() == true) {
		if(0 < Cursor[Depth] + TopIndex[Depth]) {
			if(0 < Cursor[Depth]) {
				Cursor[Depth]--;
			} else {
				TopIndex[Depth]--;
			}
		}
	} else if(CheckDownKeyInput() == true) {
		if(Cursor[Depth] + TopIndex[Depth] + 1 < actor.LearnedSkills.Count()) {
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
	} else if(CheckDecideKeyInput() == 1) {
		//����: �X�L�����u�g���v
		local id = actor.LearnedSkills.GetID(Cursor[2] + TopIndex[2]);
		local idx = -1;
		local used = false;
		local MP = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
		local requireMP = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(id, "�R�X�g").Integers.Get(0);

		//��ʔ���
		switch(Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(id, "��ʐ���").Integer) {
			case SceneLimit.OnlyBattle:
			case SceneLimit.AlwaysNG:
				Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
				ShowErrorMessage("���̃X�L���͈ړ����Ɏg���܂���B");
				return;
		}
		//MP�[������
		if(MP < requireMP) {
			Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
			ShowErrorMessage(MPName + "������܂���B");
			return;
		}
		//�۔���
		if(actor.CheckUseSkill(id) == false) {
			Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
			ShowErrorMessage("���̃X�L���͌��ݎg���܂���B");
			return;
		}

		//�Ώۂ�I�����āA�X�L���g�p���������s����
		switch(Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(id, "�Ώێ��").Integer) {
			case TargetType.None:
				//�����g�p
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = UseSkill(id, Cursor[1], Cursor[1]);
				actor.SelfValues.Get(0x46A20EE9/*MP*/).Value -= requireMP;
				break;

			case TargetType.OneParty:
				//�����I��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.One, {["id"]=id, ["actor"]=actor, ["requireMP"]=requireMP, ["Cursor"]=Cursor[1]}, function(par, idx) {
					par["actor"].SelfValues.Get(0x46A20EE9/*MP*/).Value -= par["requireMP"];
					return UseSkill(par["id"], par["Cursor"], idx);
				});
				break;

			case TargetType.OnePartyWithDead:
				//�����I���E���ґI����
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.OneDead, {["id"]=id, ["actor"]=actor, ["requireMP"]=requireMP, ["Cursor"]=Cursor[1]}, function(par, idx) {
					par["actor"].SelfValues.Get(0x46A20EE9/*MP*/).Value -= par["requireMP"];
					return UseSkill(par["id"], par["Cursor"], idx);
				});
				break;

			case TargetType.AllParty:
			case TargetType.AllPartyWithDead:
				//�p�[�e�B�S��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.All, {["id"]=id, ["actor"]=actor, ["requireMP"]=requireMP, ["Cursor"]=Cursor[1]}, function(par, idx) {
					par["actor"].SelfValues.Get(0x46A20EE9/*MP*/).Value -= par["requireMP"];
					return UseSkill(par["id"], par["Cursor"], -1);
				});
				break;

			case TargetType.Place:
				//�ꏊ�I��
				Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
				idx = SelectPlace();
				if(idx != -1) {
					actor.SelfValues.Get(0x46A20EE9/*MP*/).Value -= requireMP;
					used = UseSkill(id, Cursor[1], idx);
				}
				break;

			default:
				//�ړ����ɓG��I�����邱�Ƃ͂ł��Ȃ�
				Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
				ShowErrorMessage("���̃X�L�����ړ����Ɏg�����Ƃ͂ł��܂���B");
				break;
		}
	}
}
