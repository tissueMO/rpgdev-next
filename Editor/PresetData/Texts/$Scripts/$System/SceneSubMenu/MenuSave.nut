//###################################################################
//		メニューシーン: セーブメニュー
//###################################################################

///[summary]セーブリストを描画
function DrawSaveList(startIndex) {
	for(local i = startIndex, n = 0; n < SaveAreaMax && i < Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount); i++, n++) {
		Game.DXLib.DrawString(200 + 40, 20 + n * 50, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, format("%02d:", i + 1));
		if(Game.SaveLoad.GetSaveHeader(i) == "") {
			Game.DXLib.DrawString(200 + 70, 20 + n * 50, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, "No Data");
		} else {
			Game.DXLib.DrawString(200 + 70, 20 + n * 50 +  0, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, Game.SaveLoad.GetSaveDateTime(i));
			Game.DXLib.DrawString(200 + 70, 20 + n * 50 + 20, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, Game.SaveLoad.GetSaveHeader(i));
		}
	}

	//カーソル・スクロールバー
	switch(Depth) {
		case 0:
			return;
		case 1:
			DrawCursorPanel(200 + 15, 12 + 2 + 50 * Cursor[Depth], Game.WindowSize.Width - 200 - 15 * 2 - 20, 48, true);
			break;
		case 2:
			//複写元
			if(0 <= Cursor[1] + TopIndex[1] - TopIndex[Depth] && Cursor[1] + TopIndex[1] - TopIndex[Depth] < SaveAreaMax) {
				DrawCursorArrow(225, 12 + 50 * (Cursor[1] + TopIndex[1] - TopIndex[Depth]), Direction4.East, false);
			}
			//複写先
			DrawCursorPanel(200 + 15, 12 + 2 + 50 * Cursor[Depth], Game.WindowSize.Width - 200 - 15 * 2 - 20, 48, true);
			break;
	}
	DrawVScrollBar(
		Game.WindowSize.Width,
		0,
		Game.WindowSize.Height - 30,
		Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount),
		TopIndex[Depth],
		SaveAreaMax
	);
}


///[summary]セーブリスト上の入力処理
function InputSaveMenu() {
	//上下操作は共通
	if(CheckUpKeyInput() == true) {
		if(0 < Cursor[Depth] + TopIndex[Depth]) {
			if(0 < Cursor[Depth]) {
				Cursor[Depth]--;
			} else {
				TopIndex[Depth]--;
			}
		}
		return;
	} else if(CheckDownKeyInput() == true) {
		if(Cursor[Depth] + TopIndex[Depth] + 1 < Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount)) {
			if(Cursor[Depth] + 1 < SaveAreaMax) {
				Cursor[Depth]++;
			} else {
				TopIndex[Depth]++;
			}
		}
		return;
	} else if(CheckCancelKeyInput() == 1) {
		//キャンセル
		Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
		Depth--;
		return;
	}

	//その他の処理
	switch(Depth) {
		case 1:		//選択
			if(CheckDecideKeyInput() == 1) {
				//決定: セーブ実行 (上書き確認なし)
				UI248120655/*前回までのプレイ時間*/ += Game.DXLib.GetNowCount() / 1000 - UI1874437940/*起動時の時刻秒*/;
				UI1874437940/*起動時の時刻秒*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();
				if(Game.SaveLoad.SaveData(Cursor[1] + TopIndex[1]) == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					ShowErrorMessage("セーブに失敗しました。");
					return;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				Game.SaveLoad.GetAllSaveHeader();		//セーブリスト更新
			} else if(Game.GetKeyInputCounter(KeyID.D) == 1) {
				//D: 削除
				if(Game.SaveLoad.IsEnabled(Cursor[1] + TopIndex[1]) == false) {
					//無効なセーブデータは削除できない
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					return;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				if(ShowErrorMessage("セーブデータ " + (Cursor[1] + TopIndex[1] + 1).tostring() + "番 を削除してもよろしいですか？|はい|いいえ") != 0) {
					return;
				} else if(Game.SaveLoad.DeleteData(Cursor[1] + TopIndex[1]) == false) {
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					ShowErrorMessage("セーブデータの削除に失敗しました。");
					return;
				}
				Game.SaveLoad.GetAllSaveHeader();		//セーブリスト更新
			} else if(Game.GetKeyInputCounter(KeyID.C) == 1) {
				//C: 複写
				if(Game.SaveLoad.IsEnabled(Cursor[1] + TopIndex[1]) == false) {
					//無効なセーブデータは複写できない
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
					return;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				MoveToNextDepth();
				Cursor[Depth] = Cursor[Depth - 1];
				TopIndex[Depth] = TopIndex[Depth - 1];
			}
			break;

		case 2:		//複写
			if(CheckDecideKeyInput() == 1) {
				//決定: 複写実行 (上書き確認なし)
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				Game.SaveLoad.CopyData(Cursor[1] + TopIndex[1], Cursor[2] + TopIndex[2]);
				Game.SaveLoad.GetAllSaveHeader();		//セーブリスト更新
				Depth = 1;
				Cursor[Depth] = Cursor[2];
				TopIndex[Depth] = TopIndex[2];
			}
			break;
	}
}
