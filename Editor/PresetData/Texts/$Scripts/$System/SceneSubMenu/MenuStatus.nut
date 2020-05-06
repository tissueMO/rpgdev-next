//###################################################################
//		メニューシーン: ステータスメニュー
//###################################################################

///[summary]現在のキャラのステータスを描画
///[par]右下の領域をスキルリストとして描画するかどうか
function DrawCharStatus(boolEnabledRightArea) {
	//基本情報
	local temp = "";
	local actorID = Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]);
	DrawActorPanel(Cursor[1], true);

	//クラス
	temp = "";
	for(local i = 0; i < Game.DB.VDBActor.GetData(actorID).Classes.Count(); i++) {
		temp += Game.DB.VDBActor.GetData(actorID).Classes.GetName(i) + "  ";
	}
	Game.DXLib.DrawString(420, 60, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "クラス: " + temp);

	//属性
	temp = "";
	for(local i = 0; i < Game.DB.VDBActor.GetData(actorID).Attributes.Count(); i++) {
		temp += Game.DB.VDBActor.GetData(actorID).Attributes.GetName(i) + "  ";
	}
	Game.DXLib.DrawString(420, 80, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "属　性: " + temp);

	//間接効果
	temp = "";
	for(local i = 0; i < Game.DB.VDBActor.GetData(actorID).States.Count(); i++) {
		temp += Game.DB.VDBActor.GetData(actorID).States.GetName(i) + "  ";
	}
	Game.DXLib.DrawString(420, 100, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "状　態: " + temp);

	//装備品
	Game.DXLib.DrawString(220, 140, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "装 備:");
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

			//アイコン
			if(EnabledIcons == TRUE) {
				local pos = itemDB.GetCell(equipID, "アイコン").ConvertToPoint();
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

		Game.DXLib.DrawString(240, 160 + idx * 20, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, typeName + ":");
		Game.DXLib.DrawString(320, 160 + idx * 20, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, equipName);
	}

	//パラメーター
	local parDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);
	Game.DXLib.DrawString(220, 270, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "パラメーター:");
	for(local i = 0; i < parDB.GetRowCount(); i++) {
		local id = parDB.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer;
		local name = parDB.GetCellFromIndex(i, DBDefaultColumnIndices.Name).String;
		Game.DXLib.DrawString(240, 290 + 20 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, name + ":");
		Game.DXLib.DrawString(340, 290 + 20 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, format("%4d", Game.DB.VDBActor.GetData(actorID).GetParameter(id).Value));
	}

	//右側は自由領域として違うメニューでも使い回せるようにする
	if(boolEnabledRightArea == true) {
		//経験値
		Game.DXLib.DrawString(420, 135 + 20 * 0, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "累積経験値:");
		Game.DXLib.DrawStringR(Game.WindowSize.Width - 50, 135 + 20 * 0, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, Game.DB.VDBActor.GetData(actorID).EXP.tostring());
		Game.DXLib.DrawString(420, 135 + 20 * 1, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "次のLvまで:");
		Game.DXLib.DrawStringR(Game.WindowSize.Width - 50, 135 + 20 * 1, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, Game.DB.VDBActor.GetData(actorID).GetNextEXP().tostring());

		//習得スキル
		local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
		Game.DXLib.DrawString(420, 190, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "習得スキル:");
		for(local i = TopIndex[Depth], n = 0; i < Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count() && n < MaxStatusSubListCount; i++, n++) {
			local name = Game.DB.VDBActor.GetData(actorID).LearnedSkills.GetName(i);
			local id = Game.DB.VDBActor.GetData(actorID).LearnedSkills.GetID(i);

			//アイコン
			if(EnabledIcons == TRUE) {
				local pos = db.GetCell(id, "アイコン").ConvertToPoint();
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

			Game.DXLib.DrawString(440 + 25, 160 + 50 + 20 * n, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, name);
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


///[summary]現在のキャラのステータスの入力処理
function InputCharStatus() {
	//対象メンバーのステータス表示: 上下キー操作でスキルリストをスクロールする
	local actorID = Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]);
	if(CheckUpKeyInput() == true) {
		//スクロールできる場合のみ処理する
		if(Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count() > MaxStatusSubListCount) {
			TopIndex[Depth]--;
			if(TopIndex[Depth] < 0) {
				TopIndex[Depth]++;		//これ以上上に行けない
			}
		}
	} else if(CheckDownKeyInput() == true) {
		//スクロールできる場合のみ処理する
		if(Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count() > MaxStatusSubListCount) {
			TopIndex[Depth]++;
			if(TopIndex[Depth] + MaxStatusSubListCount > Game.DB.VDBActor.GetData(actorID).LearnedSkills.Count()) {
				TopIndex[Depth]--;		//これ以上下に行けない
			}
		}
	} else if(CheckDecideKeyInput() == 1) {
		//決定: トップに戻る
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		Depth = 0;
	} else if(CheckCancelKeyInput() == 1) {
		//キャンセル: 一つ前の階層に戻る
		Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
		Depth--;
	}
}


///[summary]装備大種別のリストを更新
///[comment]装備リストを表示する前に更新する必要があります。
function RefreshEquipTypeList() {
	EquipTypeList.resize(0);
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType);
	for(local i = 0; i < Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetRowCount(); i++) {
		if(db.GetCellFromIndex(i, db.GetColumnIndex("装備品")).Boolean == false) {
			continue;		//装備品ではないアイテム種別
		}
		EquipTypeList.append(db.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer);
	}
}
