//###################################################################
//		タイトルシーン
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\!SceneMainTitle_var.nut");

//####################  シーン起動  ####################
function SceneMainSetup() {
	//その他、初期化処理
	Game.Material.Sounds.StandardVolume = Material_MaxVolume;
	HealPartyAll();

	//素材読み込み
	Game.SceneTitle.BGM.Load(Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.TitleBGM, "ファイル名").String, -1, 0, 0);
	Game.SceneTitle.BGM.Play(PlayType.Loop, false);
	Graphs[0] = Game.Material.Graphics.Add(Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.TitleGraph, "ファイル名").String);

	Game.FadeIn(SquareSpeed.High);
}


//####################  シーン描画  ####################
function SceneMainDraw() {
	//メイングラフィック描画
	Game.DXLib.DrawExtendGraph(Graphs[0], 0, 0, 0.8, 0.8, false);

	//トップ画面の優先処理
	if(Depth == 0) {
		DrawWindow(Game.WindowSize.Width / 2 - 230, 330, 200, 110);
		foreach(idx in MenuItem) {
			local text = "";
			switch(idx) {
				case MenuItem.Start:
					text = "はじめから";
					break;
				case MenuItem.Load:
					text = "つづきから";
					break;
				case MenuItem.Exit:
					text = "ゲーム終了";
					break;
			}
			Game.DXLib.DrawStringC(Game.WindowSize.Width / 2 - 130, 348 + 27 * idx, 0x1B6F049F/*標準フォント*/, WHITE, text);
		}
		DrawCursorPanel(Game.WindowSize.Width / 2 - 230 + 15, 345 + Cursor[Depth] * 27, 200 - 15 * 2, 27, true);
		return;
	}

	//セーブリスト
	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, 255 - WindowTransparent);
	DrawWindow(Game.WindowSize.Width / 2 - 200, 250, 400, 195);
	for(local i = TopIndex[Depth], n = 0; n < AreaMax; i++, n++) {
		if(Game.SaveLoad.GetSaveHeader(i) == "") {
			Game.DXLib.DrawString(Game.WindowSize.Width / 2 - 200 + 30, 265 + n * 43, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
				format("%02d: No Data", i + 1)
			);
		} else {
			Game.DXLib.DrawString(Game.WindowSize.Width / 2 - 200 + 30, 265 + n * 43, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
				format("%02d: ", i + 1) + Game.SaveLoad.GetSaveDateTime(i)
			);
			Game.DXLib.DrawString(Game.WindowSize.Width / 2 - 200 + 50, 265 + n * 43 + 17, 0x2E19E6A3/*セーブデータリスト用*/, WHITE,
				Game.SaveLoad.GetSaveHeader(i)
			);
		}
	}

	//複写元カーソル
	if(Depth == 2) {
		if(0 <= Cursor[1] + TopIndex[1] - TopIndex[Depth] && Cursor[1] + TopIndex[1] - TopIndex[Depth] < AreaMax) {
			DrawCursorArrow(Game.WindowSize.Width / 2 - 200 + 20, 256 + Cursor[1] * 43, Direction4.East, false);
		}
	}

	//カーソル・スクロールバー
	DrawCursorPanel(Game.WindowSize.Width / 2 - 200 + 15, 250 + 12 + Cursor[Depth] * 43, 400 - 45, 37, true);
	DrawVScrollBar(Game.WindowSize.Width / 2 + 200, 250, 195, SaveCount, TopIndex[Depth], AreaMax);

	Game.DXLib.SetDrawBlendMode(-1, 0);
}


//####################  シーン入力  ####################
function SceneMainInput() {
	switch(Depth) {
		case 0:
			//トップ画面
			if(CheckUpKeyInput() == true) {
				Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDownKeyInput() == true) {
				Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDecideKeyInput() == 1) {
				//項目決定
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);
				switch(Cursor[Depth]) {
					case MenuItem.Start:
						//はじめから
						Game.FadeOut(SquareSpeed.High);
						Game.Material.Sounds.Stop(Game.SceneTitle.BGM.Handle, 1.0, false);
						Game.Wait(1.0, true, false);

						//初期化・マップへ移行
						UI1874437940/*起動時の時刻秒*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();
						UF1358433584/*マップ移動時にフェードアウトしない*/ = true;
						Game.ChangeToMapScene();
						Game.SceneMap.ChangeMap(
							TitleStart.MapFileName,
							TitleStart.PositionX,
							TitleStart.PositionY,
							TitleStart.Direction
						);
						break;

					case MenuItem.Load:
						//つづきから
						Depth++;
						TopIndex[Depth] = 0;
						Cursor[Depth] = 0;

						//セーブデータのヘッダー更新
						Game.SaveLoad.GetAllSaveHeader();
						DoFadeObject(
							function() { WindowTransparent = 255; },
							function() { return ((WindowTransparent -= SquareSpeed.Moment) <= 0); }
						);
						break;

					case MenuItem.Exit:
						//ゲーム終了
						Game.FadeOut(SquareSpeed.High);
						Game.ApplicationExit(ExitCode.Success);
						break;
				}
			}
			break;

		case 1:
			//つづきから: セーブリスト
			if(CheckUpKeyInput() == true) {
				if(0 <= TopIndex[Depth] + Cursor[Depth] - 1) {
					Cursor[Depth]--;
					if(Cursor[Depth] < 0) {
						Cursor[Depth] = 0;
						TopIndex[Depth]--;
						if(TopIndex[Depth] + Cursor[Depth] < 0) {
							TopIndex[Depth]++;		//これ以上上に行けない
						}
					}
				}
			} else if(CheckDownKeyInput() == true) {
				if(TopIndex[Depth] + Cursor[Depth] + 1 < SaveCount) {
					Cursor[Depth]++;
					if(Cursor[Depth] >= AreaMax) {
						Cursor[Depth] = AreaMax - 1;
						TopIndex[Depth]++;
						if(TopIndex[Depth] + Cursor[Depth] >= SaveCount) {
							TopIndex[Depth]--;		//これ以上下に行けない
						}
					}
				}
			} else if(CheckDecideKeyInput() == 1) {
				//決定: ロード実行
				if(Game.SaveLoad.IsEnabled(TopIndex[Depth] + Cursor[Depth]) == true) {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);
					Game.FadeOut(SquareSpeed.High);
					Game.Material.Sounds.Stop(Game.SceneTitle.BGM.Handle, 1.0, false);
					Game.Wait(1.0, true, false);
					Game.ChangeToMapScene();
					Game.SaveLoad.LoadData(TopIndex[Depth] + Cursor[Depth]);
					UI1874437940/*起動時の時刻秒*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();

					//マップ移動後の共通処理
					DoAfterChangeMapProcedure();
				} else {
					//無効なセーブデータ
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, -1, 0, 0);
				}
			} else if(CheckCancelKeyInput() == 1) {
				//キャンセル
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, -1, 0, 0);
				DoFadeObject(
					function() { WindowTransparent = 0; },
					function() { return ((WindowTransparent += SquareSpeed.Moment) >= 255); }
				);
				Depth--;
			} else if(Game.GetKeyInputCounter(KeyID.D) == 1) {
				//Dキー: 削除実行
				if(Game.SaveLoad.IsEnabled(TopIndex[Depth] + Cursor[Depth]) == false) {
					//無効なデータベース
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, -1, 0, 0);
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);

					//確認を取る
					CSceneMap.ShowMessage(-1, "システム", "セーブデータ " + (TopIndex[Depth] + Cursor[Depth] + 1).tostring() + "番 を削除してもよろしいですか？\a");
					if(Choice("はい|いいえ", true) == 0) {
						Game.SaveLoad.DeleteData(TopIndex[Depth] + Cursor[Depth]);
						Game.SaveLoad.GetAllSaveHeader();
					}
				}
			} else if(Game.GetKeyInputCounter(KeyID.C) == 1) {
				//Cキー: 複写へ移行
				if(Game.SaveLoad.IsEnabled(TopIndex[Depth] + Cursor[Depth]) == false) {
					//無効なデータベース
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, -1, 0, 0);
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);
					Depth++;
					Cursor[Depth] = Cursor[Depth - 1];
					TopIndex[Depth] = TopIndex[Depth - 1];
				}
			}
			break;

		case 2:
			//つづきから: 複写先選択
			if(CheckUpKeyInput() == true) {
				if(0 <= TopIndex[Depth] + Cursor[Depth] - 1) {
					Cursor[Depth]--;
					if(Cursor[Depth] < 0) {
						Cursor[Depth] = 0;
						TopIndex[Depth]--;
						if(TopIndex[Depth] + Cursor[Depth] < 0) {
							TopIndex[Depth]++;		//これ以上上に行けない
						}
					}
				}
			} else if(CheckDownKeyInput() == true) {
				if(TopIndex[Depth] + Cursor[Depth] + 1 < SaveCount) {
					Cursor[Depth]++;
					if(Cursor[Depth] >= AreaMax) {
						Cursor[Depth] = AreaMax - 1;
						TopIndex[Depth]++;
						if(TopIndex[Depth] + Cursor[Depth] >= SaveCount) {
							TopIndex[Depth]--;		//これ以上下に行けない
						}
					}
				}
			} else if(CheckDecideKeyInput() == 1) {
				//決定: 複写実行
				if(TopIndex[Depth] + Cursor[Depth] == TopIndex[Depth - 1] + Cursor[Depth - 1]) {
					//複写元と複写先が同一
					Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, -1, 0, 0);
				} else {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);
					Game.SaveLoad.CopyData(TopIndex[Depth - 1] + Cursor[Depth - 1], TopIndex[Depth] + Cursor[Depth]);
					Game.SaveLoad.GetAllSaveHeader();
					Depth--;
					Cursor[Depth] = Cursor[Depth + 1];
					TopIndex[Depth] = TopIndex[Depth + 1];
				}
			} else if(CheckCancelKeyInput() == 1) {
				//キャンセル
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, -1, 0, 0);
				Depth--;
			}
			break;
	}
}


//####################  シーン破棄  ####################
function SceneMainDispose() {
	foreach(hdl in Graphs) {
		Game.Material.Graphics.Delete(hdl);
	}
}
