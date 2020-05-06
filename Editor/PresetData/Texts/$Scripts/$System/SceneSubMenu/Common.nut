//###################################################################
//		メニューシーン: 汎用処理
//###################################################################

///[summary]説明欄のテキストを定義
///[return]説明欄に表示すべきテキスト
function GetMenuComment() {
	//優先処理
	if(TargetMemberSelecting == true) {
		if(Cursor[TargetMemberDepth] != -1) {
			return "対象となるパーティキャラを選択して下さい。";
		} else {
			return "パーティ全体が対象となります。";
		}
	} else if(PlaceSelecting == true) {
		return "場所を選択して下さい。";
	}

	//階層 1~2 は共通処理
	switch(Depth) {
		case 0:
			switch(Cursor[0]) {
				case MenuItem.Status:
					return "パーティキャラのステータスを確認します。";
				case MenuItem.Item:
					return "持っているアイテムを使用します。[D]破棄  [A]種別順に整理  [S]名前順に整理";
				case MenuItem.Equip:
					return "パーティキャラの装備を変更します。";
				case MenuItem.Skill:
					return "パーティキャラの移動中に有効なスキルを使用します。";
				case MenuItem.Member:
					return "パーティキャラの並び替えを行います。";
				case MenuItem.Save:
					return "現在のデータをセーブします。";
				case MenuItem.Option:
					return "ゲームの設定を行います。";
			}
			break;

		case 1:
			switch(Cursor[0]) {
				case MenuItem.Status:
				case MenuItem.Equip:
				case MenuItem.Skill:
				case MenuItem.Member:
					return "メンバーを選択して下さい。";
				case MenuItem.Item:
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					return db.GetCell(Game.DB.VDBParty.GetItemDataFromIndex(Cursor[1] + TopIndex[1]).ID, "注釈").String;
				case MenuItem.Save:
					return "セーブデータを選択して下さい。[C]複写  [D]削除";
				case MenuItem.Option:
					return "左右キーで設定値を変更します。変更結果は即座に反映されます。";
			}
			break;
	}

	//階層 2~ はメニューごとに処理
	switch(Cursor[0]) {
		case MenuItem.Status:	//対象メンバーのステータス表示: パラメーター・クラス・属性・装備・習得スキル
			local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Char).GetSubDB(0);
			return db.GetCell(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]), "肩書き").String;

		case MenuItem.Equip:
			switch(Depth) {
				case 2:			//対象メンバーの装備大種別選択
					return "装備品の種別を選択して下さい。";
				case 3:			//装備品選択
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					if(Cursor[3] + TopIndex[3] == 0) {
						return "装備しない状態にします。";
					} else {
						return db.GetCell(EquipList[Cursor[3] + TopIndex[3]].ID, "注釈").String;
					}
			}
			break;

		case MenuItem.Skill:	//対象メンバーのスキル選択
			local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));
			local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
			return db.GetCell(actor.LearnedSkills.GetID(Cursor[2] + TopIndex[2]), "注釈").String;

		case MenuItem.Member:	//対象メンバーの入替先選択
			return "対象メンバーの移動先を選択して下さい。";

		case MenuItem.Save:		//複写先選択
			return "複写先のセーブデータを選択して下さい。";
	}
	return "";
}


///[summary]メニューを閉じる
function CloseMenuScene() {
	//フェードアウト
	DoFadeObject(
		function() { WindowTransparent = 0; },
		function() { return ((WindowTransparent += SquareSpeed.Moment) >= 255); }
	);
	Game.EndSubScene();
}


///[summary]説明欄に任意のテキストを表示・選択肢処理
///[par]表示テキスト。選択肢にする場合は | で区切り、最初の要素がメッセージになります。
///[return]選択された項目のインデックス。キャンセルされた場合は -1 が返されます。
function ShowErrorMessage(strText) {
	ErrorMsg = strText;
	Cursor[ChoiceDepth] = 0;
	while(ErrorMsg != "" && Game.Wait(0, true, true)) {
		if(ErrorMsg.find("|") == null) {
			//単なるメッセージ表示
			if(CheckDecideKeyInput() == 1) {
				//決定キーでメッセージ解除
				ErrorMsg = "";
			}
		} else {
			//選択肢: 「|」の２つ目以降を項目とみなす
			local spl = split(ErrorMsg, "|");
			if(CheckLeftKeyInput() == true) {
				Cursor[ChoiceDepth] = Game.DownAround(Cursor[ChoiceDepth], 0, spl.len() - 1);
			} else if(CheckRightKeyInput() == true) {
				Cursor[ChoiceDepth] = Game.UpAround(Cursor[ChoiceDepth], 0, spl.len() - 1);
			} else if(CheckDecideKeyInput() == 1) {
				//決定キーで選択肢終了
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				ErrorMsg = "";
			} else if(CheckCancelKeyInput() == 1) {
				//キャンセル: カーソル位置をマイナスにする
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
				ErrorMsg = "";
				Cursor[ChoiceDepth] = -1;
			}
		}
	}

	//選択肢の結果を返す
	if(strText.find("|") != null) {
		print("選択された項目のインデックス [" + Cursor[ChoiceDepth].tostring() + "]");
		return Cursor[ChoiceDepth];
	} else {
		return -1;
	}
}


///[summary]アクターに指定したアイテムを装備したときの全パラメーター増減量を配列で返す
///[par]対象アクター (CDBActorData)
///[par]アイテム大種別ID
///[par]装備させようとしているアイテムのID
///[comment]並び順はデータベース上での順序になります。アドオンによる変動値は考慮しません。
///[return]増減量を示すint型の配列。装備できない場合は null が返されます。
function GetEquipParameterChanges(actorData, equipType, afterEquipID) {
	local parDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);
	local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local deltaList = array(parDB.GetRowCount(), 0);

	//エラーチェック
	if(afterEquipID != -1 && actorData.CheckEquip(afterEquipID) == false) {
		return null;	//装備できない場合は処理しない
	}

	//現在の装備品を取得する
	local beforeEquipID = actorData.GetEquipIDFromItemTypeID(equipType);

	//増減値計算: 以前の装備品と対象の装備品で同じパラメーターの変動値の差を取る
	for(local i = 0; i < deltaList.len(); i++) {
		local parID = parDB.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer;
		local before = ((beforeEquipID == -1) ? 0 :
			((itemDB.GetCell(beforeEquipID, "パラメーター").IDValues.Exists(parID) == false) ? 0 :
				itemDB.GetCell(beforeEquipID, "パラメーター").IDValues.GetValue(parID)));
		local after = ((afterEquipID == -1) ? 0 :
			((itemDB.GetCell(afterEquipID, "パラメーター").IDValues.Exists(parID) == false) ? 0 :
				itemDB.GetCell(afterEquipID, "パラメーター").IDValues.GetValue(parID)));
		deltaList[i] = after - before;
	}
	return deltaList;
}


///[summary]対象キャラを選択
///[par]対象種別 (SelectMemberType)
///[par]関数オブジェクトの引数となるテーブル
///[par]選択したときに行う処理内容を定義した関数オブジェクト
///[comment]第3引数に関数オブジェクトを代入したとき、選択後に実行される関数の戻り値がそのまま返されます。第3引数がnullのとき、返値は選択されたパーティメンバーのインデックスで、全体を選択した場合は 0 が返されます。
///[return]キャンセルされた場合は -1 が返されます。
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
			//決定
			//エラーチェック
			if(selectType == SelectMemberType.One) {
				//死者を選択できない単体選択のとき、選択メンバーが死んでいないか調べる
				if(IsDead(Cursor[TargetMemberDepth]) == true) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					ShowErrorMessage("死んでいるメンバーは選択できません。");
					continue;
				}
			}
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);

			//決定時の処理を指定されているときはここで実行する
			if(funcSelectedHandler != null) {
				result = funcSelectedHandler(par, Cursor[TargetMemberDepth]);
				Game.Wait(1.0, true, false);		//画面上の変化を見せるために一定時間待機
			}

			//全体を選択することを確定したとき、カーソル位置を 0 に戻す
			if(selectType == SelectMemberType.All) {
				Cursor[TargetMemberDepth] = 0;
			}
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//キャンセル: カーソル位置をマイナスにする
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
			Cursor[TargetMemberDepth] = -1;
			break;
		}
	}
	TargetMemberSelecting = false;

	//選択結果を返す
	return (result != null) ? result : Cursor[TargetMemberDepth];
}


///[summary]場所を選択
///[return]ユーザーDB上のインデックス。キャンセルされた場合は -1 が返されます。
function SelectPlace() {
	PlaceSelecting = true;
	Cursor[PlaceDepth] = 0;
	TopIndex[PlaceDepth] = 0;

	//場所リストを作成
	PlaceList = array(0);
	local db = Game.DB.UDB.GetUserDB(0x2C80399D/*場所リスト*/);
	for(local i = 0; i < db.GetRowCount(); i++) {
		//有効な場所のみリストに入れる
		if(db.GetCellFromIndex(i, db.GetColumnIndex("訪問済")).Boolean == true) {
			local newData = CIDNameData();
			newData.ID = i;
			newData.Name = db.GetCellFromIndex(i, db.GetColumnIndex("表示名")).String;
			PlaceList.append(newData);
		}
	}

	//有効な場所が一つもない場合は処理しない
	if(PlaceList.len() == 0) {
		ShowErrorMessage("有効な場所が一つもありません。");
		return -1;
	}

	//入力受付
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
			//決定
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//キャンセル: カーソル位置をマイナスにする
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
			Cursor[PlaceDepth] = -1;
			break;
		}
	}
	PlaceSelecting = false;

	//選択結果となるカーソル位置を場所リスト上のインデックスに変換
	local visitedCount = 0;
	local selectedIndex = -1;
	for(local i = 0; i < db.GetRowCount(); i++) {
		if(db.GetCellFromIndex(i, db.GetColumnIndex("訪問済")).Boolean == true) {
			visitedCount++;
			if (Cursor[PlaceDepth] + 1 == visitedCount) {
				//訪問済の項目数で数えて合致した項目のインデックスで返す
				selectedIndex = i;
				break;
			}
		}
	}

	//選択結果を返す
	return selectedIndex;
}


///[summary]説明欄の描画
///[par]テキスト
function DrawComment(text) {
	if(ErrorMsg == "") {
		//標準の説明を表示
		Game.DXLib.DrawString(16, Game.WindowSize.Height - 23, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, text);
		return;
	}

	local spl = split(ErrorMsg, "|");
	if(spl.len() == 1) {
		//単なるメッセージ表示
		Game.DXLib.DrawString(16, Game.WindowSize.Height - 23, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, ErrorMsg + "▼");
		return;
	}

	//選択肢: 「|」の２つ目以降を項目とみなす
	Game.DXLib.DrawString(16, Game.WindowSize.Height - 23, 0x2E19E6A3/*セーブデータリスト用*/, WHITE, spl[0]);
	for(local i = 1; i < spl.len(); i++) {
		Game.DXLib.DrawString(
			Game.WindowSize.Width * 4/7 + (Game.WindowSize.Width * 3/7 / (spl.len() - 1) * (i - 1)),
			Game.WindowSize.Height - 23,
			0x2E19E6A3/*セーブデータリスト用*/, WHITE, spl[i]
		);

		//選択位置にカーソルを描画
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


///[summary]対象キャラ選択の描画処理
function DrawTargetMemberSelect() {
	//アクターパネルを描画
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		DrawActorPanel(i, false);
		if(Cursor[TargetMemberDepth] == -1) {
			//カーソルが -1 のときは全体選択状態にする
			DrawCursorPanel(200 + 15, 12 + 110 * i, Game.WindowSize.Width - 200 - 15 * 2, 95, true);
		}
	}

	//カーソルを描画
	if(Cursor[TargetMemberDepth] != -1) {
		DrawCursorPanel(200 + 15, 12 + 110 * Cursor[TargetMemberDepth], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
	}
}


///[summary]場所選択の描画処理
function DrawPlaceSelect() {
	if(PlaceList == null) {
		return;		//場所リストが作成されていない場合は処理しない
	}
	for(local i = TopIndex[PlaceDepth], n = 0; n < AreaMax && i < PlaceList.len(); i++, n++) {
		Game.DXLib.DrawString(200 + 40, 20 + n * 25, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, PlaceList[i].Name);
	}
	DrawCursorPanel(200 + 15, 12 + 4 + 25 * Cursor[PlaceDepth], Game.WindowSize.Width - 200 - 15 * 2 - 20, 24, true);
	DrawVScrollBar(Game.WindowSize.Width, 0, Game.WindowSize.Height - 30, PlaceList.len(), TopIndex[PlaceDepth], AreaMax);
}


///[summary]アクターパネルを描画
///[par]パーティのインデックス
///[par]インデックスに合わせた位置ではなく、インデックス 0 の位置に描画するかどうか
function DrawActorPanel(ptidx, isAbsolutely) {
	if(ptidx < 0 || Game.DB.VDBParty.GetCharCount() <= ptidx) {
		return;		//無効なパーティインデックスは処理しない
	}

	local panelHeight = 110;
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(ptidx));
	if(isAbsolutely == true) {
		panelHeight = 0;		//絶対位置固定の場合はインデックスに関わらず同じ座標に描画する
	}

	//グラフィック
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

	//名前
	Game.DXLib.DrawString(
		LeftOffset - 110,
		20 + ptidx * panelHeight,
		0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
		actor.Name
	);

	//HP表示
	Game.DXLib.DrawString(
		LeftOffset,
		20 + ptidx * panelHeight,
		0x2E19E6A3/*セーブデータリスト用*/, WHITE,
		format(HPName + ": %4d /%4d", actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value, actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value)
	);
	DrawMeterPattern(
		LeftOffset + MeterOffset,
		22 + ptidx * panelHeight,
		MeterWidth,
		MeterHeight,
		HPMeterPatternStart,
		HPMeterPatternCount,
		(actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value == 0) ? 0 : (MeterWidth * actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value / actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value)
	);

	//MP表示
	Game.DXLib.DrawString(
		LeftOffset,
		20 + ptidx * panelHeight + 20,
		0x2E19E6A3/*セーブデータリスト用*/, WHITE,
		format(MPName + ": %4d /%4d", actor.SelfValues.Get(0x46A20EE9/*MP*/).Value, actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value)
	);
	DrawMeterSolid(
		LeftOffset + MeterOffset,
		40 + ptidx * panelHeight,
		MeterWidth,
		MeterHeight,
		MPMeterPattern,
		(actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value == 0) ? 0 : (MeterWidth * actor.SelfValues.Get(0x46A20EE9/*MP*/).Value / actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value)
	);

	//レベル
	Game.DXLib.DrawString(
		LeftOffset,
		20 + ptidx * panelHeight + 40,
		0x2E19E6A3/*セーブデータリスト用*/, WHITE,
		format("Lv: %d", actor.Level)
	);

	//間接効果アイコン
	for(local i = 0; i < actor.States.Count() && i < StateIconMaxCount; i++) {
		local pos = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0).GetCell(actor.States.GetID(i), "アイコン").ConvertToPoint();
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
