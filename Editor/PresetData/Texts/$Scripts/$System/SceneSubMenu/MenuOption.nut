//###################################################################
//		メニューシーン: オプションメニュー
//###################################################################

///[summary]オプション項目の描画処理
function DrawOptionMenu() {
	for(local i = 0; i < OptionMenu.len(); i++) {
		switch(i) {
			case OptionMenu.MapMessageSpeed:
				Game.DXLib.DrawString(240, 20 + 25 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					"会話メッセージ表示速度"
				);
				Game.DXLib.DrawString(450, 20 + 25 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					format("[ %d ]", UI2084302691/*会話メッセージ表示速度*/)
				);
				break;
			case OptionMenu.BattleMessageSpeed:
				Game.DXLib.DrawString(240, 20 + 25 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					"戦闘メッセージ表示速度"
				);
				Game.DXLib.DrawString(450, 20 + 25 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					format("[ %d ]", UI315411244/*戦闘メッセージ表示速度*/)
				);
				break;
			case OptionMenu.SoundVolume:
				Game.DXLib.DrawString(240, 20 + 25 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					"サウンド音量"
				);
				Game.DXLib.DrawString(450, 20 + 25 * i, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
					format("[%3d]", Game.Material.Sounds.StandardVolume)
				);
				break;
		}
	}
	if(Depth > 0) {
		DrawCursorPanel(200 + 15, 12 + 4 + 25 * Cursor[1], Game.WindowSize.Width - 200 - 15 * 2, 22, true);
	}
}


///[summary]オプション項目上の入力処理
function InputOptionMenu() {
	if(CheckUpKeyInput() == true) {
		Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, OptionMenu.len());
	} else if(CheckDownKeyInput() == true) {
		Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, OptionMenu.len());
	} else if(CheckDecideKeyInput() == 1) {
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		Depth = 0;
	} else if(CheckCancelKeyInput() == 1) {
		Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
		Depth = 0;
	} else if(CheckLeftKeyInput() == true) {
		//設定値ダウン
		switch(Cursor[1]) {
			case OptionMenu.MapMessageSpeed:
				if(0 <= UI2084302691/*メッセージ表示速度*/ - 1) {
					UI2084302691/*メッセージ表示速度*/--;
				}
				break;
			case OptionMenu.BattleMessageSpeed:
				if(0 < UI315411244/*戦闘メッセージ表示速度*/ - 1) {
					UI315411244/*戦闘メッセージ表示速度*/--;
				}
				break;
			case OptionMenu.SoundVolume:
				if(0 < Game.Material.Sounds.StandardVolume) {
					Game.Material.Sounds.StandardVolume--;
				}
				if(Game.SceneMap.BGM.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGM.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGM.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGM.Handle, true, 0);
				}
				if(Game.SceneMap.BGS.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGS.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGS.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGS.Handle, true, 0);
				}
				break;
		}
	} else if(CheckRightKeyInput() == true) {
		//設定値アップ
		switch(Cursor[1]) {
			case OptionMenu.MapMessageSpeed:
				if(UI2084302691/*メッセージ表示速度*/ + 1 < 10) {
					UI2084302691/*メッセージ表示速度*/++;
				}
				break;
			case OptionMenu.BattleMessageSpeed:
				if(UI315411244/*戦闘メッセージ表示速度*/ + 1 < 10) {
					UI315411244/*戦闘メッセージ表示速度*/++;
				}
				break;
			case OptionMenu.SoundVolume:
				if(Game.Material.Sounds.StandardVolume + 1 <= 255) {
					Game.Material.Sounds.StandardVolume++;
				}
				if(Game.SceneMap.BGM.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGM.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGM.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGM.Handle, true, 0);
				}
				if(Game.SceneMap.BGS.Handle != -1) {
					Game.Material.Sounds.Pause(Game.SceneMap.BGS.Handle, 0);
					Game.Material.Sounds.Play(
						Game.SceneMap.BGS.Handle, PlayType.Loop,
						Game.Material.Sounds.StandardVolume, Game.SceneMap.BGM.Pitch, Game.SceneMap.BGM.Panpot, 0);
					Game.Material.Sounds.Resume(Game.SceneMap.BGS.Handle, true, 0);
				}
				break;
		}
	}
}
