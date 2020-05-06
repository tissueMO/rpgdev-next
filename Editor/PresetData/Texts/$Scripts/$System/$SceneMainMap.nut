//###################################################################
//		マップシーン
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\!SceneMainMap_var.nut");

//####################  シーン起動  ####################
function SceneMainSetup() {
	SetHeadCharGraphics();		//先頭者のグラフィックをセット
	IsWaitingForFirstFrame = true;
}


//####################  シーン描画  ####################
function SceneMainDraw() {
	//マップの標準描画
	Game.SceneMap.MapData.DrawMap(60);

	//所持金表示
	if(UF1222958890/*所持金表示*/ == true) {
		DrawWindow(Game.WindowSize.Width - 180, 0, 180, 35);
		Game.DXLib.DrawString(Game.WindowSize.Width - 140, 10, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
			format("%10d ", Game.DB.VDBParty.Money) + US1246572451/*通貨名称*/
		);
	}

	//デバッグ用表示
	if(DEBUG && UF358059746/*$デバッグ用ガイド表示*/ == true) {
		Game.DXLib.DrawStringR(Game.WindowSize.Width, 0, -1, WHITE,
			"[A]ダッシュ [S]すり抜け [X/Ctrl]メニュー開閉\n[F3]マップ再読込 [F4]ＳＱ再読込 [F5]デバッグ\n[F8]リソース使用量 [F11]ポーズ [F12]リセット"
		);
	}
}


//####################  シーン入力  ####################
function SceneMainInput() {
	if(IsWaitingForFirstFrame) {
		// マップ移動直後 1フレーム の間は、自動起動イベント判定待ちのため、シーン入力を受け付けない
		IsWaitingForFirstFrame = false;
		return;
	}

	local mapData = Game.SceneMap.MapData;
	local noEncounterFlag = false;

	if(mapData.IsEVRunning == true || mapData.EVReservedCount() > 0) {
		return;		//イベント実行中or予約中は操作できない
	} else if(mapData.Player.MapObject.IsMoving == true) {
		return;		//移動中は操作できない
	}

	//デバッグ時のみ有効な操作
	if(DEBUG == TRUE) {
		//当たり判定無視
		mapData.Player.CharData.NoHit = (Game.GetKeyInputCounter(KeyID.S) > 0);

		//エンカウント判定無効化
		noEncounterFlag = mapData.Player.CharData.NoHit;

		//デバッグ用コマンド表示切替
		if(Game.GetKeyInputCounter(KeyID.F8) == 1) {
			UF358059746/*$デバッグ用ガイド表示*/ = !UF358059746/*$デバッグ用ガイド表示*/;
		}
	}

	//一歩歩き終わった直後の処理
	if(MovingFlag == true) {
		MovingFlag = false;

		//マップ上で有効な間接効果の処理
		StateMapPartyAll();

		//ランダムエンカウント判定
		if(noEncounterFlag == false) {
			local minStepCount = (NormalEncounterStepCount / mapData.EncounterRate.tofloat() * 100).tointeger();
			if(StepCount >= minStepCount
			&& mapData.Units.Count() > 0
			&& Game.NextRand(1, 100) <= (StepCount - minStepCount)) {
				//エンカウント発生: どのユニットと遭遇したかをランダムに決定する
				local unitID = mapData.Units.Get(Game.NextRand(0, mapData.Units.Count() - 1));
				print(StepCount + " 歩で エンカウント [ID: " + unitID + "] が発生しました。");
				StepCount = 0;
				StartBattle(unitID, null, null, false, false);
				return;
			}
		}
	}

	if(CheckDecideKeyInput() == 1) {
		//決定キーで起動するイベントはここで判定する
		mapData.CheckPressEnterTriggerEV();
		return;
	} else if(CheckCancelKeyInput() == 1) {
		//メニュー画面を開く
		Game.DB.FDBMaterial.Play(IntoMenuSEID, 255, 0, 0);
		StartMenu();
		return;
	} else if(Game.GetKeyInputCounter(KeyID.F12) == 1) {
		//リセット
		GameReset();
		return;
	} else {
		//新規移動: 一番最後に押された方向キーを見る
		local dir = Direction4.None;
		local cnt = INT_MAX;
		if(Game.GetKeyInputCounter(KeyID.Up) > 0 && Game.GetKeyInputCounter(KeyID.Up) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Up);
			dir = Direction4.North;
		}
		if(Game.GetKeyInputCounter(KeyID.Down) > 0 && Game.GetKeyInputCounter(KeyID.Down) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Down);
			dir = Direction4.South;
		}
		if(Game.GetKeyInputCounter(KeyID.Left) > 0 && Game.GetKeyInputCounter(KeyID.Left) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Left);
			dir = Direction4.West;
		}
		if(Game.GetKeyInputCounter(KeyID.Right) > 0 && Game.GetKeyInputCounter(KeyID.Right) < cnt) {
			cnt = Game.GetKeyInputCounter(KeyID.Right);
			dir = Direction4.East;
		}

		if(dir == Direction4.None) {
			return;		//方向入力がなかった
		}

		//ダッシュ判定
		local dash = (Game.GetKeyInputCounter(KeyID.A) > 0);
		if(dash == false) {
			mapData.Player.CharData.MoveSpeed = mapData.MoveSpeed;
		} else {
			mapData.Player.CharData.MoveSpeed = mapData.MoveSpeed + 1;
		}

		//移動方向が決定: 移動できるかどうかの判定を行い、ここで同時に接触イベントの判定も行われる
		mapData.Player.CharData.Direction = dir;
		if(mapData.CheckBeforeMove(-1, mapData.Player.MapObject.Position.X, mapData.Player.MapObject.Position.Y, dir, true) == false) {
			return;		//移動できない
		}

		//移動処理
		StepCount++;
		MovingFlag = true;
		mapData.Player.MapObject.AutoSetMoveCounter(dir);
		switch(dir) {
			case Direction4.North:
				mapData.Player.MapObject.Position.Y -= 1;
				break;
			case Direction4.South:
				mapData.Player.MapObject.Position.Y += 1;
				break;
			case Direction4.West:
				mapData.Player.MapObject.Position.X -= 1;
				break;
			case Direction4.East:
				mapData.Player.MapObject.Position.X += 1;
				break;
		}

		//マップの範囲外に出たらマップのループ設定に従って補正する
		mapData.Player.MapObject.ApplyMapLoopPosition();
	}

	//梯子の処理
	if(mapData.CheckTileGroupNum(mapData.Player.MapObject.Position.X, mapData.Player.MapObject.Position.Y, LadderTileGroupNum) != -1) {
		//現在のタイルが梯子である場合、向きを北向きに固定する
		mapData.Player.CharData.FixedDirection = false;
		mapData.Player.CharData.Direction = Direction4.North;		//向きの固定を一旦解除しないと適用されない
		mapData.Player.CharData.FixedDirection = true;
	} else {
		mapData.Player.CharData.FixedDirection = false;
	}
}


//####################  シーン破棄  ####################
function SceneDispose() {
}


//####################  先頭者の歩行グラフィックを設定  ####################
function SetHeadCharGraphics() {
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == true) {
			continue;		//死んでいるキャラは表示しない
		}
		Game.SceneMap.MapData.Player.CharData.LoadGraphics(
			Game.DB.GetDB(DBType.FDB, FDBIndices.Char).GetSubDB(0).GetCell(Game.DB.VDBParty.GetCharIDFromPartyIndex(i), "グラフィック").String
		);
		return;
	}

	//デバッグ時: 全員死んでいる場合、便宜的に一番前のキャラにする
	if(DEBUG) {
		Game.SceneMap.MapData.Player.CharData.LoadGraphics(
			Game.DB.GetDB(DBType.FDB, FDBIndices.Char).GetSubDB(0).GetCell(Game.DB.VDBParty.GetCharIDFromPartyIndex(0), "グラフィック").String
		);
	} else {
		Game.SceneMap.MapData.Player.CharData.LoadGraphics("");
	}
}


//####################  MT実行: マップ名表示パネル  ####################
//["starttime"] = 開始時の時刻
function MTMapNamePanel(intHandle) {
	local delta = Game.DXLib.GetNowCount() - Game.MT.GetThread(intHandle).GetValueInteger("starttime");
	local offset = 0;

	if(delta < MapNameFadeTime) {
		//フェードインフェーズ
		Game.DXLib.SetDrawBlendMode(
			BlendMode.Alpha,
			delta * 255 / MapNameFadeTime
		);
		offset = -(200 / 2) + (delta * (200 / 2) / MapNameFadeTime);
	} else if(delta > MapNamePanelTime + MapNameFadeTime) {
		//フェードアウトフェーズ
		Game.DXLib.SetDrawBlendMode(
			BlendMode.Alpha,
			255 - ((delta - (MapNamePanelTime + MapNameFadeTime)) * 255 / MapNameFadeTime)
		);
		if(delta > MapNamePanelTime + MapNameFadeTime * 2) {
			return true;		//スレッド終了
		}
		offset = -(delta - (MapNamePanelTime + MapNameFadeTime)) * (200 / 2) / MapNameFadeTime;
	}

	DrawWindow(offset, 0, 230, 35);
	Game.DXLib.DrawStringC(offset + 230 / 2, 10, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, Game.SceneMap.MapData.CurrentMapName);
	Game.DXLib.RestoreDrawBlendMode();

	return false;		//スレッド継続
}
