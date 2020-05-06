//###################################################################
//		ショップシーン
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\!SceneSubShop_var.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\Common.nut");

//####################  シーン起動  ####################
function SceneSubSetup() {
	//フェードイン
	DoFadeObject(
		function() { WindowTransparent = 255; },
		function() { return ((WindowTransparent -= SquareSpeed.Moment) <= 0); }
	);
	UF1222958890/*所持金表示*/ = true;
}


//####################  シーン描画  ####################
function SceneSubDraw() {
	local offsetX = 30;
	local offsetY = 15;

	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, 255 - WindowTransparent);

	////店名
	// DrawWindow(0, 0, 250, 40);
	// Game.DXLib.DrawStringC(250 / 2, 9, 0x1B6F049F/*標準フォント*/, WHITE, ShopName);

	//メニュー
	DrawWindow(0, 40, 120, 115);
	foreach(idx in MenuItem) {
		local lineSpace = 30;
		local text = "";
		switch(idx) {
			case MenuItem.Buy:
				text = "購 入";
				break;
			case MenuItem.Sell:
				text = "売 却";
				break;
			case MenuItem.Exit:
				text = "終 了";
				break;
		}
		Game.DXLib.DrawString(offsetX, offsetY + 42 + lineSpace * idx, 0x1B6F049F/*標準フォント*/, WHITE, text);

		//カーソル
		if(idx == Cursor[0]) {
			DrawCursorPanel(15, 11 + 42 + lineSpace * idx, 120 - 15 * 2, 28, (Depth == 0));
		}
	}

	//説明欄
	DrawWindow(0, Game.WindowSize.Height - 32, Game.WindowSize.Width, 32);
	DrawComment(GetMenuComment());

	//購入・売却のみメインウィンドウを描画
	if(Cursor[0] == MenuItem.Exit) {
		return;
	}

	//メイン領域
	DrawWindow(120, 40, Game.WindowSize.Width - 120 - 180, Game.WindowSize.Height - 40 - 32);
	DrawWindow(Game.WindowSize.Width - 180, 40, 180, Game.WindowSize.Height - 40 - 32);

	//以降はメニューに応じた処理
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local id = -1;
	switch(Cursor[0]) {
		case MenuItem.Buy:
			//商品リスト
			id = Game.SceneMap.ShopData.Get(Cursor[1] + TopIndex[1]).ID;
			for(local i = (Depth == 0) ? 0 : TopIndex[1], n = 0; n < AreaMax && i < Game.SceneMap.ShopData.GetCount(); i++, n++) {
				local data = Game.SceneMap.ShopData.Get(i);
				local color = WHITE;		//デフォルトで選択可能な状態

				//購入できない場合は色を変える
				if(Game.DB.VDBParty.Money < db.GetCell(data.ID, "価格").Integer) {
					color = DARKGRAY;
				}

				//アイコン
				if(EnabledIcons == TRUE) {
					local pos = db.GetCell(data.ID, "アイコン").ConvertToPoint();
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

				Game.DXLib.DrawString(120 + 45, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color, data.Name);
				if(ShowAtSign == true) {
					//単価記号を表示する場合
					Game.DXLib.DrawString(Game.WindowSize.Width - 120 - 182, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color, "@");
				}
				Game.DXLib.DrawStringR(Game.WindowSize.Width - 120 - 100, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color,
					format("%d ", db.GetCell(data.ID, "価格").Integer) + US1246572451/*通貨名称*/
				);
			}

			//カーソル・所持数
			if(Depth >= 1 && Game.SceneMap.ShopData.GetCount() > 0) {
				DrawCursorPanel(120 + 15, 55 + 25 * Cursor[1], Game.WindowSize.Width - 120 - 200 - 15 * 2, 25, true);
				DrawVScrollBar(Game.WindowSize.Width - 180, 40, Game.WindowSize.Height - 70, Game.SceneMap.ShopData.GetCount(), TopIndex[1], AreaMax);
				Game.DXLib.DrawStringR(Game.WindowSize.Width - 20, Game.WindowSize.Height - 40 - 20, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					format("%d 個 持っている", Game.DB.VDBParty.GetItemCount(id))
				);
			}
			break;

		case MenuItem.Sell:
			//所持品リスト
			if(Game.DB.VDBParty.GetItemCountAll() == 0) {
				break;		//一つもアイテムを持っていない状態
			}
			id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;
			for(local i = (Depth == 0) ? 0 : TopIndex[1], n = 0; n < AreaMax && i < Game.DB.VDBParty.GetItemCountAll(); i++, n++) {
				local data = Game.DB.VDBParty.GetItemDataFromIndex(i);
				local color = WHITE;		//デフォルトで選択可能な状態

				//売却できない場合は色を変える
				if(db.GetCell(data.ID, "廃棄可").Boolean == false) {
					color = DARKGRAY;
				}

				//アイコン
				if(EnabledIcons == TRUE) {
					local pos = db.GetCell(data.ID, "アイコン").ConvertToPoint();
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

				Game.DXLib.DrawString(120 + 45, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color,
					data.Name
				);
				Game.DXLib.DrawString(120 + (Game.WindowSize.Width - 120 - 230) - 115, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color,
					format("×%2d", data.Value)
				);
				if(ShowAtSign == true) {
					//単価記号を表示する場合
					Game.DXLib.DrawString(Game.WindowSize.Width - 120 - 182, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color, "@");
				}
				Game.DXLib.DrawStringR(Game.WindowSize.Width - 120 - 100, 60 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, color,
					format("%d ", db.GetCell(data.ID, "価格").Integer * SellRate) + US1246572451/*通貨名称*/
				);
			}

			//カーソル描画
			if(Depth >= 1 && Game.DB.VDBParty.GetItemCountAll() > 0) {
				DrawCursorPanel(120 + 15, 55 + 25 * Cursor[1], Game.WindowSize.Width - 120 - 200 - 15 * 2, 25, true);
				DrawVScrollBar(Game.WindowSize.Width - 180, 40, Game.WindowSize.Height - 70, Game.DB.VDBParty.GetItemCountAll(), TopIndex[1], AreaMax);
			}
			break;
	}

	//選択中のアイテムが装備品のときはパーティメンバーのパラメーター変動を表示する
	if(Depth >= 1 && id != -1 && db.GetCell(id, "アイテム大種別").Integers.Count() > 0) {
		local parDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);
		local itemTypeFID = db.GetCell(id, "アイテム大種別").Integers.Get(0);
		local itemTypeVID = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetCell(itemTypeFID, "ID").Integer;
		if(100 <= itemTypeVID && itemTypeVID < 1000) {
			for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
				local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
				Game.DXLib.DrawString(Game.WindowSize.Width - 160, 55 + i * 90, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, actor.Name);

				//装備チェック
				if(actor.GetEquipIDFromItemTypeID(itemTypeFID) == id) {
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + 18, 0x2E19E6A3/*セーブデータリスト用*/, GRAY, "装備している");
					continue;
				}

				//変動値を取得
				local changes = GetEquipParameterChanges(actor, itemTypeFID, id);
				if(changes == null) {
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + 18, 0x2E19E6A3/*セーブデータリスト用*/, GRAY, "装備できない");
					continue;
				}

				local m = 0;
				for(local n = 0; n < changes.len(); n++) {
					if(changes[n] == 0) {
						continue;		//変動がない場合は処理しない
					}
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + (m + 1) * 18, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
						parDB.GetCellFromIndex(n, DBDefaultColumnIndices.Name).String
					);
					if(changes[n] > 0) {
						//増える場合
						Game.DXLib.DrawStringR(Game.WindowSize.Width - 30, 55 + i * 90 + (m + 1) * 18, 0x2E19E6A3/*セーブデータリスト用*/, GREENYELLOW,
							format("+%d", changes[n])
						);
					} else {
						//減る場合
						Game.DXLib.DrawStringR(Game.WindowSize.Width - 30, 55 + i * 90 + (m + 1) * 18, 0x2E19E6A3/*セーブデータリスト用*/, GRAY,
							changes[n].tostring()
						);
					}
					m++;
				}
				if(m == 0) {
					Game.DXLib.DrawString(Game.WindowSize.Width - 160 + 15, 55 + i * 90 + 18, 0x2E19E6A3/*セーブデータリスト用*/, GRAY, "変動なし");
				}
			}
		}
	}
}


//####################  シーン入力  ####################
function SceneSubInput() {
	//ショップはイベントから呼び出されるため、リセットされると不具合が起きる
	//if(Game.GetKeyInputCounter(KeyID.F12) == 1) {
	//	GameReset();
	//	return;
	//}

	//トップ画面の優先処理
	if(Depth == 0) {
		if(CheckUpKeyInput() == true) {
			Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, MenuItem.len());
		} else if(CheckDownKeyInput() == true) {
			Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, MenuItem.len());
		} else if(CheckDecideKeyInput() == 1) {
			//エラーチェック
			switch(Cursor[0]) {
				case MenuItem.Sell:
					if(Game.DB.VDBParty.GetItemCountAll() == 0) {
						//アイテムを持っていない場合はエラー出して戻る
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("アイテムを一つも持っていません。");
						return;
					}
					break;
				case MenuItem.Exit:
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					CloseMenuScene();
					return;
			}
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			MoveToNextDepth();
		} else if(CheckCancelKeyInput() == 1) {
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
			CloseMenuScene();
		}
		return;
	}

	//以降はメニューに応じた処理
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	switch(Cursor[0]) {
		case MenuItem.Buy:
			//商品リスト
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
				//決定: アイテムを購入
				local id = Game.SceneMap.ShopData.Get(Cursor[1] + TopIndex[1]).ID;

				//購入できないアイテムは処理しない
				if(Game.DB.VDBParty.Money < db.GetCell(id, "価格").Integer) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					ShowErrorMessage("お金が足りません。");
					return;
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					if(ShowErrorMessage("このアイテムを購入してよろしいですか？|はい|いいえ") == 0) {
						//購入処理
						Game.DB.FDBMaterial.Play(BuySellSEID, 255, 0, 0);
						Game.DB.VDBParty.Money -= db.GetCell(id, "価格").Integer;
						Game.DB.VDBParty.AddItem(id, 1);
					}
				}
			} else if(CheckCancelKeyInput() == 1) {
				//キャンセル
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
				Depth--;
			}
			break;

		case MenuItem.Sell:
			//所持品リスト
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
				//決定: アイテムを売却
				local id = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID;

				//売却できないアイテムは処理しない
				if(db.GetCell(id, "廃棄可").Boolean == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					ShowErrorMessage("このアイテムは売却できません。");
					return;
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					if(ShowErrorMessage("このアイテムを売却してよろしいですか？|はい|いいえ") == 0) {
						//売却処理
						Game.DB.FDBMaterial.Play(BuySellSEID, 255, 0, 0);
						Game.DB.VDBParty.Money += db.GetCell(id, "価格").Integer * SellRate;
						Game.DB.VDBParty.DeleteItem(id, 1);
						RefreshBuyItemCursor(1);		//選択位置修正
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
			}
			break;
	}
}


//####################  シーン破棄  ####################
function SceneSubDispose() {
	UF1222958890/*所持金表示*/ = false;

	//商品リストをクリアする
	Game.SceneMap.ShopData.Clear();
}


//####################  トップ階層の説明  ####################
function GetMenuComment() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	switch(Cursor[0]) {
		case MenuItem.Buy:
			if(Depth == 0) {
				return "商品を購入します。";
			}
			return db.GetCell(Game.SceneMap.ShopData.Get(Cursor[1] + TopIndex[1]).ID, "注釈").String;

		case MenuItem.Sell:
			if(Depth == 0) {
				return "持っているアイテムを売却します。[A]種別順に整理  [S]名前順に整理";
			}
			return db.GetCell(Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID, "注釈").String;

		case MenuItem.Exit:
			return "買い物を終了します。";
	}
	return "";
}


//####################  売却: アイテム選択位置の修正を行う  ####################
///[par]所持品から減らされた個数
function RefreshBuyItemCursor(intUsedLength) {
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
