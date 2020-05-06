//###################################################################
//		疑似的な並列スレッドとして動作させる関数を定義します。
//###################################################################

//####################  メッセージウィンドウ描画  ####################
//int["phase"] = 0:フェードイン, 1:処理, 2:フェードアウト
//int["isWaiting"] = メッセージ送り待機中かどうか
//int["trans"] = ウィンドウ不透明度
//int["faceID"] = フェイスID (指定しない場合は -1)
//int["isRightFace"] = フェイスが反対側かどうか
//int["offset"] = メッセージ内容オフセット
//int["speakerOffset"] = 話者名オフセット
//str["speaker"] = 話者名
//str["msg"] = メッセージ内容
function MTMessage(intHandle) {
	//ウィンドウ開閉処理
	switch(Game.MT.GetThread(intHandle).GetValueInteger("phase")) {
		case 0:		//フェードイン
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") + SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") >= 255) {
				//処理フェーズへ移行
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 1);
				Game.MT.GetThread(intHandle).SetValueInteger("trans", 255);
			}
			break;

		case 2:		//フェードアウト
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") - SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") < 0) {
				return true;		//スレッド終了
			}
			break;
	}
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans"));

	local hdl = -1;
	if(Game.MT.GetThread(intHandle).GetValueInteger("faceID") != -1) {
		hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Face, Game.MT.GetThread(intHandle).GetValueInteger("faceID"));
	}
	local size = (hdl <= -1) ? null : Game.DXLib.GetGraphSize(hdl);
	local x = (hdl <= -1 || Game.MT.GetThread(intHandle).GetValueInteger("isRightFace") == FALSE) ? 0 : Game.WindowSize.Width - size.Width;

	//ウィンドウ
	DrawWindow(0, Game.WindowSize.Height - MsgWndHeight, Game.WindowSize.Width, MsgWndHeight);

	//メッセージ
	Game.DXLib.SetDrawArea(16, Game.WindowSize.Height - MsgWndHeight + 16, Game.WindowSize.Width - 16, Game.WindowSize.Height);
	if(hdl > -1 && Game.MT.GetThread(intHandle).GetValueInteger("isRightFace") == FALSE) {
		//フェイスが左のときだけメッセージ内容の位置が右に移動する
		Game.DXLib.DrawString(
			16 + MsgFaceStandardWidth,
			16 + Game.WindowSize.Height - MsgWndHeight + Game.MT.GetThread(intHandle).GetValueInteger("offset"),
			MsgFontID,
			WHITE,
			Game.MT.GetThread(intHandle).GetValueString("msg")
		);
	} else {
		Game.DXLib.DrawString(
			16,
			16 + Game.WindowSize.Height - MsgWndHeight + Game.MT.GetThread(intHandle).GetValueInteger("offset"),
			MsgFontID,
			WHITE,
			Game.MT.GetThread(intHandle).GetValueString("msg")
		);
	}
	Game.DXLib.ResetDrawArea();

	//フェイス
	if(hdl != -1) {
		Game.DXLib.DrawGraph(hdl, x, Game.WindowSize.Height - size.Height, true, false);
	}

	//名前パネル: 空白の場合は表示しない
	if(Game.MT.GetThread(intHandle).GetValueString("speaker") != "") {
		if(hdl <= -1 || IsFixedSizedFace == TRUE) {
			//メッセージウィンドウの左上に表示
			DrawWindow(
				0,
				Game.WindowSize.Height - MsgWndHeight - MsgNameHeight + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				MsgNameWidth,
				MsgNameHeight
			);
			Game.DXLib.DrawStringC(
				MsgNameWidth / 2,
				Game.WindowSize.Height - MsgWndHeight - MsgNameHeight + 10 + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				0x6DD57515/*ウィンドウ内テキスト*/,
				WHITE,
				Game.MT.GetThread(intHandle).GetValueString("speaker")
			);
		} else if(Game.MT.GetThread(intHandle).GetValueInteger("isRightFace") == FALSE) {
			//メッセージウィンドウの左下に表示
			DrawWindow(
				0,
				Game.WindowSize.Height - MsgNameHeight + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				MsgNameWidth,
				MsgNameHeight
			);
			Game.DXLib.DrawStringC(
				MsgNameWidth / 2,
				Game.WindowSize.Height - MsgNameHeight + 10 + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				0x6DD57515/*ウィンドウ内テキスト*/,
				WHITE,
				Game.MT.GetThread(intHandle).GetValueString("speaker")
			);
		} else {
			//メッセージウィンドウの右下に表示
			DrawWindow(
				Game.WindowSize.Width - MsgNameWidth,
				Game.WindowSize.Height - MsgNameHeight + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				MsgNameWidth,
				MsgNameHeight
			);
			Game.DXLib.DrawStringC(
				Game.WindowSize.Width - MsgNameWidth + MsgNameWidth / 2,
				Game.WindowSize.Height - MsgNameHeight + 10 + Game.MT.GetThread(intHandle).GetValueInteger("speakerOffset"),
				0x6DD57515/*ウィンドウ内テキスト*/,
				WHITE,
				Game.MT.GetThread(intHandle).GetValueString("speaker")
			);
		}
	}

	//改頁インジケーターのアニメーション
	if(Game.MT.GetThread(intHandle).GetValueInteger("isWaiting") == TRUE) {
		local wndhdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*システムグラフィック*/);
		local delta = Game.DXLib.GetNowCount();
		local animIdx = Game.DXLib.GetNowCount() % 800 / 200;
		Game.DXLib.DrawRectGraph(wndhdl, Game.WindowSize.Width - 24, Game.WindowSize.Height - 24, 96 + 16 * (animIdx % 2), 64 + 16 * (animIdx / 2), 16, 16, true, false);
	}

	Game.DXLib.RestoreDrawBlendMode();
	return false;		//スレッド継続
}


//####################  選択肢  ####################
//int["phase"] = 0:フェードイン, 1:処理, 2:フェードアウト
//int["trans"] = ウィンドウ不透明度
//int["Cursor"] = カーソル位置
//str["msg"] = 選択肢内容
function MTChoice(intHandle) {
	local list = split(Game.MT.GetThread(intHandle).GetValueString("msg"), "|");
	local fontSize = Game.DB.FDBMaterial.GetSubDB(DBMaterialIndices.Font).GetCell(0x2ABC7058/*メッセージフォント*/, "行間").Integer;

	//ウィンドウ開閉処理
	switch(Game.MT.GetThread(intHandle).GetValueInteger("phase")) {
		case 0:		//フェードイン
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") + SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") >= 255) {
				//処理フェーズへ移行
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 1);
				Game.MT.GetThread(intHandle).SetValueInteger("trans", 255);
			}
			break;

		case 2:		//フェードアウト
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") - SquareSpeed.Moment
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") < 0) {
				return true;		//スレッド終了
			}
			break;
	}
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans"));

	//ウィンドウ
	DrawWindow(
		ChoiceWndMargin,
		Game.WindowSize.Height * 1 / 3 - (fontSize * list.len()) / 2 - 14,
		Game.WindowSize.Width - ChoiceWndMargin * 2,
		fontSize * list.len() + 22
	);

	//選択肢リスト
	for(local i = 0; i < list.len(); i++) {
		Game.DXLib.DrawString(
			ChoiceWndMargin + 45,
			Game.WindowSize.Height * 1 / 3 - (fontSize * list.len()) / 2 + i * fontSize,
			MsgFontID,
			WHITE,
			list[i]
		);
	}

	//カーソル
	DrawCursorPanel(
		ChoiceWndMargin + 15,
		Game.WindowSize.Height * 1 / 3 - (fontSize * list.len()) / 2 + Game.MT.GetThread(intHandle).GetValueInteger("Cursor") * fontSize - 5 + 2,
		Game.WindowSize.Width - ChoiceWndMargin * 2 - 15 * 2
		fontSize - 2, true
	);

	Game.DXLib.RestoreDrawBlendMode();
	return false;		//スレッド継続
}


//####################  固定式テロップ  ####################
//int["phase"] = 0:フェードイン, 1:待機, 2:フェードアウト
//int["trans"] = 不透明度
//int["timeMS"] = 待機秒数
//int["canKeyBreak"] = 決定キー押下で中断できるようにするかどうか
//int["enabledBack"] = 背景を描画するかどうか
//int["starttime"] = 待機を開始した時刻 (内部用)
//str["msg"] = テロップ内容
function MTFixedSubtitles(intHandle) {
	local hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*システムグラフィック*/);
	local lines = split(Game.MT.GetThread(intHandle).GetValueString("msg"), "\n");

	switch(Game.MT.GetThread(intHandle).GetValueInteger("phase")) {
		case 0:		//フェードイン
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") + SquareSpeed.Normal
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") >= 255) {
				//待機フェーズへ移行
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 1);
				Game.MT.GetThread(intHandle).SetValueInteger("trans", 255);
				Game.MT.GetThread(intHandle).SetValueInteger("starttime", Game.DXLib.GetNowCount());
			}
			break;

		case 1:		//待機
			local now = Game.DXLib.GetNowCount();
			if(CheckDecideKeyInput() > 0 && Game.MT.GetThread(intHandle).GetValueInteger("canKeyBreak") == TRUE) {
				//キー入力で中断
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 2);
			} else if(now - Game.MT.GetThread(intHandle).GetValueInteger("starttime") >= Game.MT.GetThread(intHandle).GetValueInteger("timeMS")) {
				//時間経過で終了
				Game.MT.GetThread(intHandle).SetValueInteger("phase", 2);
			}
			break;

		case 2:		//フェードアウト
			Game.MT.GetThread(intHandle).SetValueInteger("trans",
				Game.MT.GetThread(intHandle).GetValueInteger("trans") - SquareSpeed.Normal
			);
			if(Game.MT.GetThread(intHandle).GetValueInteger("trans") < 0) {
				return true;		//スレッド終了
			}
			break;
	}

	//背景を添えて描画する
	if(Game.MT.GetThread(intHandle).GetValueInteger("enabledBack") == TRUE) {
		Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans") / 3);
		Game.DXLib.DrawRectExtendGraph(hdl, 0, Game.WindowSize.Height * 1 / 4, Game.WindowSize.Width, lines.len() * SubtitleLineHeight, 0, 0, 64, 64, true);
	}

	//文字列を描画する
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, Game.MT.GetThread(intHandle).GetValueInteger("trans"));
	Game.DXLib.DrawStringC(
		Game.WindowSize.Width / 2,
		Game.WindowSize.Height * 1 / 4,
		SubtitleFontID,
		WHITE,
		Game.MT.GetThread(intHandle).GetValueString("msg")
	);

	Game.DXLib.SetDrawBlendMode(-1, -1);
	return false;		//スレッド継続
}


//####################  流れるテロップ  ####################
//int["canSpeedUp"] = 決定キー押下状態で高速化するかどうか
//int["y"] = 表示位置Y座標
//int["allLineHeight"] = 全行高さ
//int["speed"] = 移動速度
//str["msg"] = テロップ内容
function MTFlowSubtitles(intHandle) {
	Game.DXLib.DrawString(
		Game.WindowSize.Width * 1 / 10,
		Game.MT.GetThread(intHandle).GetValueInteger("y"),
		SubtitleFontID,
		WHITE,
		Game.MT.GetThread(intHandle).GetValueString("msg")
	);

	//流す
	if(CheckDecideKeyInput() > 0 && Game.MT.GetThread(intHandle).GetValueInteger("canSpeedUp") == TRUE) {
		//スピードアップ
		Game.MT.GetThread(intHandle).SetValueInteger("y",
			Game.MT.GetThread(intHandle).GetValueInteger("y") - SquareSpeed.High
		);
	} else {
		Game.MT.GetThread(intHandle).SetValueInteger("y",
			Game.MT.GetThread(intHandle).GetValueInteger("y") - Game.MT.GetThread(intHandle).GetValueInteger("speed")
		);
	}

	//終了判定
	if(-Game.MT.GetThread(intHandle).GetValueInteger("allLineHeight") >= Game.MT.GetThread(intHandle).GetValueInteger("y")) {
		return true;		//スレッド終了
	} else {
		return false;		//スレッド継続
	}
}
