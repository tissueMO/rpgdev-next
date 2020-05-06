//###################################################################
//		メニューシーン: 装備メニュー
//###################################################################

///[summary]現在のキャラの装備画面の描画
function DrawEquipMenu() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	DrawCharStatus(false);
	DrawCursorPanel(230, 156 + 20 * Cursor[2], 190, 21, (Depth == 2));		//装備品種別のカーソル

	//装備品リスト
	local cur = (Depth >= 3) ? Cursor[3] : 0;
	local top = (Depth >= 3) ? TopIndex[3] : 0;
	Game.DXLib.DrawString(420, 140, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, "装備品リスト:");
	for(local i = top, n = 0; i < EquipList.len() && n < MaxStatusSubListCount; i++, n++) {
		local color = DARKGRAY;
		if(EquipList[i].ID == -1
		|| Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1])).CheckEquip(EquipList[i].ID) == true) {
			//「装備を外す」 or 装備可能なアイテムのみ有効色にする
			color = WHITE;
		}

		//アイコン
		if(EnabledIcons == TRUE && EquipList[i].ID != -1) {
			local pos = db.GetCell(EquipList[i].ID, "アイコン").ConvertToPoint();
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

		Game.DXLib.DrawString(460, 160 + 20 * i, 0x2E19E6A3/*セーブデータリスト用*/, color, EquipList[i].Name);
		if(EquipList[i].Value > 0) {
			Game.DXLib.DrawString(580, 160 + 20 * i, 0x2E19E6A3/*セーブデータリスト用*/, color, format("× %d", EquipList[i].Value));
		}
	}

	if(Depth >= 3) {
		//装備品選択
		//カーソルとスクロールバー
		DrawCursorPanel(425, 156 + 20 * cur, 190, 21, (Depth == 3));
		DrawVScrollBar(Game.WindowSize.Width - 20, 145, 300, EquipList.len(), TopIndex[3], MaxStatusSubListCount);

		//パラメーター増減値プレビュー
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
		local changes = GetEquipParameterChanges(actor, EquipTypeList[Cursor[2]], EquipList[Cursor[3] + TopIndex[3]].ID);
		if(changes != null) {
			for(local i = 0; i < changes.len(); i++) {
				if(changes[i] < 0) {
					Game.DXLib.DrawString(375, 290 + 20 * i, 0x2E19E6A3/*セーブデータリスト用*/, GRAY, changes[i].tostring());
				} else if(changes[i] > 0) {
					Game.DXLib.DrawString(375, 290 + 20 * i, 0x2E19E6A3/*セーブデータリスト用*/, GREENYELLOW, format("+%d", changes[i]));
				}
			}
		}
	}
}


///[summary]現在のキャラの装備画面の入力
function InputEquipMenu() {
	switch(Depth) {
		case 2:
			//装備大種別選択
			if(CheckUpKeyInput() == true) {
				Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, EquipTypeList.len());
				RefreshEquipList(EquipTypeList[Cursor[Depth]]);
			} else if(CheckDownKeyInput() == true) {
				Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, EquipTypeList.len());
				RefreshEquipList(EquipTypeList[Cursor[Depth]]);
			} else if(CheckDecideKeyInput() == 1) {
				//決定
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				MoveToNextDepth();
			} else if(CheckCancelKeyInput() == 1) {
				//キャンセル
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
				Depth--;
			}
			break;

		case 3:
			//装備品選択
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
				//決定: 装備する
				local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
				if(EquipList[Cursor[3] + TopIndex[3]].ID != -1
				&& actor.CheckEquip(EquipList[Cursor[3] + TopIndex[3]].ID) == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					ShowErrorMessage("このキャラに装備させることはできません。");
					break;
				}

				//更新前に装備していたアイテムをすべて記憶する
				local beforeEquipIDs = {};
				foreach(idx, typeID in EquipTypeList) {
					beforeEquipIDs[typeID] <- actor.GetEquipIDFromItemTypeID(typeID);
				}

				if(EquipList[Cursor[3] + TopIndex[3]].ID == -1) {
					//装備を外す
					if(actor.ReleaseEquipType(EquipTypeList[Cursor[2]], false) == false) {
						//装備を外せなかった
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("現在このキャラは装備を変更できない状態になっています。");
						break;
					}
				} else {
					//装備する
					if(actor.SetEquip(EquipList[Cursor[3] + TopIndex[3]].ID, false) == false) {
						//装備に失敗
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("現在このキャラは装備を変更できない状態になっています。");
						break;
					}
				}
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);

				//HP・MPが最大値を超えないように自動補正
				FitToMaxParameters(actor);

				//装備していたアイテムを所持アイテムに戻す
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
						//既に外した装備品 or 装備が変化しなかった部位はスキップ
						continue;
					}

					//少なくとも装備欄に変化があったとしてマークしておく
					isSelected = true;
					if(beforeEquipID == -1) {
						//外す装備品がないのでスキップ
						continue;
					}

					//外した装備品をアイテム一覧に戻す
					Game.DB.VDBParty.AddItem(beforeEquipID, 1);

					//複数の装備欄を占領する単一のアイテムを重複して戻さないようにする
					addItems.append(beforeEquipID);
				}

				//実際に装備品を選んだ場合
				if(isSelected == true && EquipList[Cursor[3] + TopIndex[3]].ID != -1) {
					//所持アイテムから減らす
					Game.DB.VDBParty.DeleteItem(EquipList[Cursor[3] + TopIndex[3]].ID, 1);
				}

				//装備品リストを更新する
				RefreshEquipList(EquipTypeList[Cursor[2]]);
				if(EquipList.len() <= Cursor[3] + TopIndex[3]) {
					//カーソル位置補正
					if(0 < TopIndex[3]) {
						TopIndex[3]--;
					} else {
						Cursor[3]--;
					}
				}
			} else if(CheckCancelKeyInput() == 1) {
				//キャンセル
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
				Depth--;
			}
			break;
	}
}


///[summary]所持アイテムリストから指定した装備大種別に絞り込む
///[par]アイテム大種別ID
function RefreshEquipList(intEquipTypeID) {
	EquipList.resize(0);

	//装備を外すための項目を用意する
	local emptyData = CIDValueData();
	emptyData.ID = -1;
	emptyData.Name = "(装備を外す)";
	emptyData.Value = -1;
	EquipList.append(emptyData);

	//指定された装備大種別に該当するアイテムのみリストに登録する
	for(local i = 0; i < Game.DB.VDBParty.GetItemCountAll(); i++) {
		local data = Game.DB.VDBParty.GetItemDataFromIndex(i);
		if(Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(data.ID, "アイテム大種別").Integers.Exists(intEquipTypeID) == true) {
			EquipList.append(data);
		}
	}
}
