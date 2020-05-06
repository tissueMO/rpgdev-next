//###################################################################
//		メニューシーン: スキルメニュー
//###################################################################

///[summary]現在のキャラのスキルリストを描画
function DrawSkillList(startIndex) {
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
	local MP = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
	local MMP = actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value;

	//キャラ名
	Game.DXLib.DrawString(LeftOffset - 110, 20, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, actor.Name);

	//MP表示
	Game.DXLib.DrawString(LeftOffset, 20, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
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

	//スキルリスト
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	for(local i = startIndex, n = 0; n < AreaMax && i < actor.LearnedSkills.Count(); i++, n++) {
		local id = actor.LearnedSkills.GetID(i);
		local color = DARKGRAY;		//デフォルトで使用できない表示色
		local requireMP = db.GetCell(id, "コスト").Integers.Get(0);

		//場面判定
		switch(db.GetCell(id, "場面制限").Integer) {
			case SceneLimit.OnlyMap:
			case SceneLimit.AlwaysOK:
				//使用可否判定
				if(MP >= requireMP && actor.CheckUseSkill(id) == true) {
					color = WHITE;		//使用可能
				}
				break;
		}

		//アイコン
		if(EnabledIcons == TRUE) {
			local pos = db.GetCell(id, "アイコン").ConvertToPoint();
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

		Game.DXLib.DrawString(200 + 55, 80 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color,
			actor.LearnedSkills.GetName(i)
		);
		Game.DXLib.DrawString(Game.WindowSize.Width - 150, 80 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color,
			format("消費" + MPName + ":%4d", requireMP)
		);
	}

	DrawCursorPanel(200 + 15, 80 - 4 + 25 * Cursor[2], Game.WindowSize.Width - 200 - 15 * 2 - 20, 24, true);
	DrawVScrollBar(Game.WindowSize.Width, 80, 370, actor.LearnedSkills.Count(), TopIndex[2], AreaMax);
}


///[summary]現在のキャラのスキルリスト上の入力処理
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
		//キャンセル
		Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
		Depth--;
	} else if(CheckDecideKeyInput() == 1) {
		//決定: スキルを「使う」
		local id = actor.LearnedSkills.GetID(Cursor[2] + TopIndex[2]);
		local idx = -1;
		local used = false;
		local MP = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
		local requireMP = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(id, "コスト").Integers.Get(0);

		//場面判定
		switch(Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(id, "場面制限").Integer) {
			case SceneLimit.OnlyBattle:
			case SceneLimit.AlwaysNG:
				Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
				ShowErrorMessage("このスキルは移動中に使えません。");
				return;
		}
		//MP充足判定
		if(MP < requireMP) {
			Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
			ShowErrorMessage(MPName + "が足りません。");
			return;
		}
		//可否判定
		if(actor.CheckUseSkill(id) == false) {
			Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
			ShowErrorMessage("このスキルは現在使えません。");
			return;
		}

		//対象を選択して、スキル使用処理を実行する
		switch(Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(id, "対象種別").Integer) {
			case TargetType.None:
				//即時使用
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = UseSkill(id, Cursor[1], Cursor[1]);
				actor.SelfValues.Get(0x46A20EE9/*MP*/).Value -= requireMP;
				break;

			case TargetType.OneParty:
				//味方選択
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.One, {["id"]=id, ["actor"]=actor, ["requireMP"]=requireMP, ["Cursor"]=Cursor[1]}, function(par, idx) {
					par["actor"].SelfValues.Get(0x46A20EE9/*MP*/).Value -= par["requireMP"];
					return UseSkill(par["id"], par["Cursor"], idx);
				});
				break;

			case TargetType.OnePartyWithDead:
				//味方選択・死者選択可
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.OneDead, {["id"]=id, ["actor"]=actor, ["requireMP"]=requireMP, ["Cursor"]=Cursor[1]}, function(par, idx) {
					par["actor"].SelfValues.Get(0x46A20EE9/*MP*/).Value -= par["requireMP"];
					return UseSkill(par["id"], par["Cursor"], idx);
				});
				break;

			case TargetType.AllParty:
			case TargetType.AllPartyWithDead:
				//パーティ全体
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.All, {["id"]=id, ["actor"]=actor, ["requireMP"]=requireMP, ["Cursor"]=Cursor[1]}, function(par, idx) {
					par["actor"].SelfValues.Get(0x46A20EE9/*MP*/).Value -= par["requireMP"];
					return UseSkill(par["id"], par["Cursor"], -1);
				});
				break;

			case TargetType.Place:
				//場所選択
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				idx = SelectPlace();
				if(idx != -1) {
					actor.SelfValues.Get(0x46A20EE9/*MP*/).Value -= requireMP;
					used = UseSkill(id, Cursor[1], idx);
				}
				break;

			default:
				//移動中に敵を選択することはできない
				Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
				ShowErrorMessage("このスキルを移動中に使うことはできません。");
				break;
		}
	}
}
