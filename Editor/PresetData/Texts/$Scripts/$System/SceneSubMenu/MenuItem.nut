//###################################################################
//		メニューシーン: アイテムメニュー
//###################################################################

///[summary]所持アイテムリストを描画
///[par]開始インデックス
function DrawItemList(startIndex) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local color = DARKGRAY;		//デフォルトで使用できない表示色

	for(local i = startIndex, n = 0; n < AreaMax && i < Game.DB.VDBParty.GetItemCountAll(); i++, n++) {
		local data = Game.DB.VDBParty.GetItemDataFromIndex(i);
		color = DARKGRAY;

		//使用できる場合は色を変える
		switch(db.GetCell(data.ID, "場面制限").Integer) {
			case SceneLimit.OnlyMap:
			case SceneLimit.AlwaysOK:
				color = WHITE;
				break;
		}

		//アイコン
		if(EnabledIcons == TRUE) {
			local pos = db.GetCell(data.ID, "アイコン").ConvertToPoint();
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

		Game.DXLib.DrawString(200 + 55, 20 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color, data.Name);
		Game.DXLib.DrawString(Game.WindowSize.Width - 100, 20 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color, format("× %d", data.Value));
	}

	//アイテムを持っている場合のみカーソル描画
	if(Depth == 1 && Game.DB.VDBParty.GetItemCountAll() > 0) {
		DrawCursorPanel(200 + 15, 12 + 4 + 25 * Cursor[1], Game.WindowSize.Width - 200 - 15 * 2 - 20, 24, true);
		DrawVScrollBar(Game.WindowSize.Width, 0, Game.WindowSize.Height - 30, Game.DB.VDBParty.GetItemCountAll(), TopIndex[1], AreaMax);
	}
}


///[summary]アイテムリスト上の入力処理
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
		//キャンセル
		Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
		Depth--;
	} else if(Game.GetKeyInputCounter(KeyID.A) == 1) {
		//種別順整理
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		switch(ShowErrorMessage("種別順に整理します。順序を選択してください。|昇順|降順|キャンセル")) {
			case 0:
				Game.DB.VDBParty.SortItemsByVisibleID(false);
				break;
			case 1:
				Game.DB.VDBParty.SortItemsByVisibleID(true);
				break;
		}
	} else if(Game.GetKeyInputCounter(KeyID.S) == 1) {
		//名前順整理
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		switch(ShowErrorMessage("名前順に整理します。順序を選択してください。|昇順|降順|キャンセル")) {
			case 0:
				Game.DB.VDBParty.SortItemsByName(false);
				break;
			case 1:
				Game.DB.VDBParty.SortItemsByName(true);
				break;
		}
	} else if(Game.GetKeyInputCounter(KeyID.D) == 1) {
		//破棄
		local id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;
		if(db.GetCell(id, "廃棄可").Boolean == false) {
			//捨てられないアイテム
			Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
			ShowErrorMessage("このアイテムは破棄することができません。");
		} else {
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			if(ShowErrorMessage("本当に破棄してもよろしいですか？|はい|いいえ") == 0) {
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				Game.DB.VDBParty.DeleteItem(id, 1);
				RefreshItemCursor(1);		//選択位置修正
			}
		}
	} else if(CheckDecideKeyInput() == 1) {
		//決定: アイテムを「使う」
		local id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;
		local idx = -1;
		local used = false;

		//移動中に使えないアイテムは処理しない
		local limit = db.GetCell(id, "場面制限").Integer;
		if(limit == SceneLimit.OnlyBattle || limit == SceneLimit.AlwaysNG) {
			Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
			ShowErrorMessage("このアイテムは移動中に使えません。");
			return;
		}

		//対象を選択して、アイテム使用処理を実行する
		switch(db.GetCell(id, "対象種別").Integer) {
			case TargetType.None:
				//即時使用
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = UseItem(id, 0, -1, false);
				break;

			case TargetType.OneParty:
				//味方選択
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.One, {["id"]=id}, function(par, idx) {
					return UseItem(par["id"], 0, idx, false);
				});
				break;

			case TargetType.OnePartyWithDead:
				//味方選択・死者選択可
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.OneDead, {["id"]=id}, function(par, idx) {
					return UseItem(par["id"], 0, idx, false);
				});
				break;

			case TargetType.AllParty:
			case TargetType.AllPartyWithDead:
				//パーティ全体
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				used = SelectTargetMember(SelectMemberType.All, {["id"]=id}, function(par, idx) {
					return UseItem(par["id"], 0, -1, false);
				});
				break;

			case TargetType.Place:
				//場所選択
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				idx = SelectPlace();
				if(idx != -1) {
					used = UseItem(id, 0, idx, false);
				}
				break;

			default:
				//移動中に敵を選択することはできない
				Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
				ShowErrorMessage("このアイテムを移動中に使うことはできません。");
				break;
		}

		//消費処理
		if(used == true) {
			Game.DB.VDBParty.DeleteItem(id, 1);
			RefreshItemCursor(1);		//選択位置修正
		}
	}
}


///[summary]アイテム選択位置の修正を行う
///[par]所持品から減らされた個数
function RefreshItemCursor(intUsedLength) {
	if(Game.DB.VDBParty.GetItemCountAll() == 0) {
		//所持アイテムがゼロになった
		Depth = 0;
	} else if(Game.DB.VDBParty.GetItemCountAll() <= Cursor[1] + TopIndex[1]) {
		//カーソル位置修正
		if(TopIndex[1] > 0) {
			TopIndex[1] -= intUsedLength;
		} else {
			Cursor[1] -= intUsedLength;
		}
	}
}

