//###################################################################
//		メニューシーン: 並び替えメニュー
//###################################################################

///[summary]メンバーリストの描画処理
function DrawMemberMenu() {
	//アクターパネル
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		DrawActorPanel(i, false);
	}

	//カーソル
	switch(Depth) {
		case 1:
			DrawCursorPanel(200 + 15, 12 + 110 * Cursor[1], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
			break;
		case 2:
			//移動元
			DrawCursorArrow(225, 12 + 110 * Cursor[1], Direction4.East, false);
			//移動先
			DrawCursorPanel(200 + 15, 12 + 110 * Cursor[2], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
			break;
	}
}


///[summary]メンバーリスト上の入力処理
function InputMemberMenu() {
	//移動先選択
	if(CheckUpKeyInput() == true) {
		Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
	} else if(CheckDownKeyInput() == true) {
		Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
	} else if(CheckDecideKeyInput() == 1) {
		//決定: 交換実行
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		local id = Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]);

		//キャラ移動
		Game.DB.VDBParty.DeleteMemberFromIndex(Cursor[1]);
		if(Cursor[2] + 1 <= Game.DB.VDBParty.GetCharCount()) {
			Game.DB.VDBParty.AddMember(id, Cursor[2]);
		} else {
			Game.DB.VDBParty.AddMember(id, -1);
		}
		SetHeadCharGraphics();		//先頭キャラのグラフィックを変更

		//移動対象の選択に戻る
		Depth = 1;
		Cursor[Depth] = Cursor[Depth + 1];
	} else if(CheckCancelKeyInput() == 1) {
		//キャンセル
		Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
		Depth--;
	}
}
