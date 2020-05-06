//###################################################################
//		ゲームプログラムの一部について委託を受けて処理します。
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\EVMoveExtend.nut");


//バトルテストを開始する
IsBattleTestMode <- false;
function/*void*/ EventHandler_StartBattleTest(intUnitID) {
	IsBattleTestMode = true;
	HealPartyAll();
	TakeItemsAll(ItemCountForTester);
	StartBattle(
		intUnitID,
		Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.BattleTestGraph, "ファイル名").String,
		null,		//標準のBGM
		false,		//逃げられる
		false		//負けたらゲームオーバー
	);
	Game.ApplicationExit(ExitCode.Success);
}


//マップテストを開始する
function/*void*/ EventHandler_StartMapTest(strMapFileName, intX, intY) {
	HealPartyAll();
	TakeItemsAll(ItemCountForTester);
	UF1358433584/*マップ移動時にフェードアウトしない*/ = true;
	UI1874437940/*起動時の時刻秒*/ = (Game.DXLib.GetNowCount() / 1000).tointeger();
	EventHandler_ChangeMap(strMapFileName, intX, intY, Direction4.South);
}


//データ読み込み中に行う描画処理
MarqueeValue <- 400;		//マーキー位置
function/*void*/ EventHandler_NowLoading(intGraphHandle) {
	Game.DXLib.DrawRectGraph(intGraphHandle, 0, 0, 0, 0, Game.WindowSize.Width, Game.WindowSize.Height, false, false);

	//マーキーを描画
	MarqueeValue = Game.UpAround(MarqueeValue + SquareSpeed.Normal, MarqueeMinimum, MarqueeMaximum);
	Game.DXLib.SetDrawArea(120, 203, 120 + 400, 203 + 16);
	Game.DXLib.DrawRectGraph(intGraphHandle, 120 + MarqueeValue, 203, 0, 480, 640, 16, true, false);
	Game.DXLib.ResetDrawArea();
}


//スプライトワイプを処理する
function/*void*/ EventHandler_WipeSprite(spriteData) {
	local SlideOffset = 50;		//スライド幅

	switch(spriteData.WipeType) {
		case SpriteWipeType.Fade:		//フェード
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				spriteData.GetGraphics(i).Draw(CPoint());
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideRight:		//スライド: 右
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.X = (SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideLeft:		//スライド: 左
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.X = -(SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideDown:		//スライド: 下
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.Y = (SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.SlideUp:		//スライド: 上
			Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, spriteData.WipeCounter);
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local offset = CPoint();
				offset.Y = -(SlideOffset - spriteData.WipeCounter * SlideOffset / 255);
				spriteData.GetGraphics(i).Draw(offset);
			}
			Game.DXLib.SetDrawBlendMode(-1, 0);
			break;

		case SpriteWipeType.Transition:		//トランジション画像を使用
			for(local i = 0; i < spriteData.GetGraphicsCount(); i++) {
				local g = spriteData.GetGraphics(i);
				local hdl = g.ID;
				Game.DXLib.DrawBlendGraph(
					spriteData.Location.X + g.Location.X,
					spriteData.Location.Y + g.Location.Y,
					hdl,
					true,
					Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.WipeBlendGraph, "ファイル名").String,
					255 - spriteData.WipeCounter
				);
			}
			break;

		default:
			//そのまま描画
			spriteData.Draw();
			break;
	}
}


//イベント方向移動の拡張的なコマンドを処理する
function/*bool*/ EventHandler_EVMove(intEVID, intOption) {
	local ev = null;
	local getSV = null;
	local setSV = null;

	if(intEVID != -1) {
		//イベントは通常通りの取得
		ev = Game.SceneMap.MapData.EVGetData(intEVID);
		getSV = function(ev) {
			return ev.SelfValues.Get(0x5CA27058/*ジャンプカウンター*/).Value;
		}
		setSV = function(ev, val) {
			ev.SelfValues.Get(0x5CA27058/*ジャンプカウンター*/).Value = val;
		}
	} else {
		//主人公はイベント互換のオブジェクト
		ev = Game.SceneMap.MapData.Player.MapObject;
		getSV = function(ev) {
			return UI1734005777/*主人公ジャンプカウンター*/;
		}
		setSV = function(ev, val) {
			UI1734005777/*主人公ジャンプカウンター*/ = val;
		}
	}

	switch(intOption) {
		case EVMoveExType.Jump:		//ジャンプ
			//移動カウンターを描画位置の微調整に使う
			ev.ForbiddenAutoMove = true;
			if(getSV(ev) == 1) {
				//着地モーション
				ev.MoveCounter.Y += SquareSpeed.Normal;
				if(ev.MoveCounter.Y >= 0) {
					//モーション終了
					ev.MoveCounter.Y = 0;
					ev.ForbiddenAutoMove = false;
					setSV(ev, 0);
					return true;
				}
			} else {
				//跳躍モーション
				ev.MoveCounter.Y -= SquareSpeed.Normal;
				if(ev.MoveCounter.Y <= -Game.SceneMap.MapData.TileSize) {
					//着地モーションへ移行
					setSV(ev, 1);
				}
			}
			return false;

		case EVMoveExType.Oni:		//鬼モード
			//A*法で追跡する
			if(Astar.MoveTasks.len() == 0 || Astar.MoveTaskIndex >= Astar.MoveTasks.len()) {
				//経路が残っていない場合は最新の経路を作成する
				Astar.ResetRouteByAstar(intEVID, Game.SceneMap.MapData.Player.MapObject.Position, Astar.GetDistance(-1, intEVID));
			}
			if(Astar.MoveTasks.len() > 0 && Astar.MoveTaskIndex < Astar.MoveTasks.len()) {
				//経路がまだ残っている場合はそれを処理する
				local dir = Astar.MoveTasks[Astar.MoveTaskIndex];
				ev.SetDirection4(dir);
				if(Game.SceneMap.MapData.CheckBeforeMove(intEVID, ev.Position.X, ev.Position.Y, dir, true) == true) {
					//移動できる場合のみ処理
					local rpos = Game.SceneMap.Direction4ToPoint(dir);
					ev.AutoSetMoveCounter(dir);
					ev.Position.Set(ev.Position.X + rpos.X, ev.Position.Y + rpos.Y);
				}
				Astar.MoveTaskIndex++;
			}
			return true;
	}
	return true;
}


//セーブデータのヘッダー情報を生成する
function/*string*/ EventHandler_RequiredSaveHeader() {
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(0));
	return (GetPlayTime() + "  " + actor.Name + format("  Lv. %2d", actor.Level));
}


//セーブデータのユーザー定義の領域をセットする
function/*string*/ EventHandler_SaveUserData() {
	local buf = "";

	//設定をシリアライズ
	buf += format("%d;", Game.Material.Sounds.StandardVolume);

	return buf;
}


//セーブデータのユーザー定義の領域をロードする
function/*void*/ EventHandler_LoadUserData(strSaveData) {
	local spl = split(strSaveData, ";");
	if(spl.len() <= 1) {
		return;		//データなし
	}

	//設定をデシリアライズ
	Game.Material.Sounds.StandardVolume = spl[0].tointeger();
}


//メッセージウィンドウを表示する
function/*void*/ EventHandler_ShowMessage(intFaceID, strSpeaker, strMsg) {
	//実際の描画は並列実行スクリプトで行われる
	ShowMessage(intFaceID, strSpeaker, strMsg);
}


//マップを移動する
function/*void*/ EventHandler_ChangeMap(strMapFileName, intX, intY, intDirection) {
	local oldBGM = Game.SceneMap.MapData.FirstMapBGMRawString;
	local oldBGS = Game.SceneMap.MapData.FirstMapBGSRawString;

	if(UF1358433584/*マップ移動時にフェードアウトしない*/ == false) {
		Game.FadeOut(SquareSpeed.High);
	}

	//マップ名表示を削除
	Game.MT.DeleteThread("マップ名パネル表示");

	Game.SceneMap.MapData.LoadMap(strMapFileName, false);
	Game.SceneMap.MapData.Player.MapObject.Position.Set(intX, intY);
	if(intDirection != Direction4.None) {
		Game.SceneMap.MapData.Player.CharData.Direction = intDirection;
	}

	local soundStopping = false;
	if(Game.BGMKeep == false
	&& oldBGM.len() > 0
	&& Game.SceneMap.MapData.FirstMapBGMRawString.len() > 0
	&& oldBGM != Game.SceneMap.MapData.FirstMapBGMRawString) {
		soundStopping = true;
		Game.Material.Sounds.Stop(Game.SceneMap.BGM.Handle, 0.5, false);
	}
	if(Game.BGSKeep == false
	&& oldBGS.len() > 0
	&& Game.SceneMap.MapData.FirstMapBGSRawString.len() > 0
	&& oldBGS != Game.SceneMap.MapData.FirstMapBGSRawString) {
		soundStopping = true;
		Game.Material.Sounds.Stop(Game.SceneMap.BGS.Handle, 0.5, false);
	}
	if(soundStopping == true) {
		Game.Wait(0.5, true, false);
	}

	//マップ移動後の共通処理
	DoAfterChangeMapProcedure();
}


//デバッグコンソールの拡張的なコマンドを処理する
function/*bool*/ EventHandler_DebugCommand(strCommandArg) {
	local stateDB = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	local selfValueDB = Game.DB.GetDB(DBType.VDB, VDBIndices.Value).GetSubDB(DBValueIndices.SelfActor);
	local parameterDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.Parameter);

	local spl = split(strCommandArg, " ");
	switch(spl[0]) {
		case "h":
			//ヘルプ
			print("	グローバル変数出力:		gbl [(絞り込む型名)]");
			print("	戦闘勝利にする:			bwin");
			print("	戦闘敗北にする:			blose");
			print("	戦闘逃走にする:			besc");
			print("	戦闘中断にする:			bbrk");
			print("	ユニット情報列挙:		lunit");
			print("	PT/UTアクター個別変数設定:		esval [インデックス] [個別変数ID] [値]");
			print("	PT/UTアクターパラメーター設定:		espar [インデックス] [パラメーターID] [値]");
			print("	PT/UTアクター間接効果設定:		esstt [インデックス] [間接効果ID]");
			print("	PT/UTアクター間接効果全解除:		ersttall [インデックス]");
			print("	PT/UTアクターバステ全解除:		ersttbad [インデックス]");
			return true;

		case "gbl":
			//グローバル変数出力
			local typeName = "";
			if(spl.len() == 1) {
				//すべて表示
				typeName = null;
			} else if(spl.len() == 2) {
				//型を制限して表示
				typeName = spl[1];
			}

			//ルートテーブルを走査
			foreach(key, var in getroottable()) {
				local varType = type(var);
				if(typeName == null || varType == typeName) {
					if(varType == "null") {
						continue;
					} else if(varType == "instance") {
						print("\t[" + key + "] = instance");
					} else {
						print("\t[" + key + "] = " + var.tostring());
					}
				}
			}
			return true;

		case "lunit":
			//ユニットアクター列挙 [List Unit]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			}
			print("ユニットエネミー一覧:");
			foreach(enemy in Enemies) {
				enemy.PrintData();
			}
			return true;

		case "esval":
			//ユニット個別変数設定 [Enemy SetSelfValue]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			} else if(spl.len() != 4) {
				print("cmdError: 引数の個数が不正です。");
				return true;
			}
			GetActorData(spl[1].tointeger()).SelfValues.Get(selfValueDB.ConvertToFixedID(spl[2].tointeger())).Value = spl[3].tointeger();
			return true;

		case "espar":
			//ユニットパラメーター設定 [Enemy SetParameter]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			} else if(spl.len() != 4) {
				print("cmdError: 引数の個数が不正です。");
				return true;
			}
			GetActorData(spl[1].tointeger()).GetParameter(parameterDB.ConvertToFixedID(spl[2].tointeger())).Value = spl[3].tointeger();
			return true;

		case "esstt":
			//ユニット間接効果設定 [Enemy SetState]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			} else if(spl.len() != 3) {
				print("cmdError: 引数の個数が不正です。");
				return true;
			}
			GetActorData(spl[1].tointeger()).SetState(stateDB.ConvertToFixedID(spl[2].tointeger()), true);
			return true;

		case "ersttall":
			//ユニット間接効果全解除 [Enemy ResetState All]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			} else if(spl.len() != 2) {
				print("cmdError: 引数の個数が不正です。");
				return true;
			}
			GetActorData(spl[1].tointeger()).ClearState();
			return true;

		case "ersttbad":
			//ユニットバステ全解除 [Enemy ResetState Bad]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			} else if(spl.len() != 2) {
				print("cmdError: 引数の個数が不正です。");
				return true;
			}
			GetActorData(spl[1].tointeger()).ClearStateOnlyBad();
			return true;

		case "bwin":
			//戦闘勝利にする [Battle Win]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			}
			MoveToEndingPhase(BattleResult.Win);
			return true;

		case "blose":
			//戦闘敗北にする [Battle Lose]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			}
			MoveToEndingPhase(BattleResult.Lose);
			return true;

		case "besc":
			//戦闘逃走にする [Battle Escape]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			}
			MoveToEndingPhase(BattleResult.Escape);
			return true;

		case "bbrk":
			//戦闘中断にする [Battle Break]
			if(Game.GetCurrentSubSceneID() != Scene.Battle) {
				print("バトルシーンではないため、処理できません。");
				return true;
			}
			MoveToEndingPhase(BattleResult.Break);
			return true;
	}
	return false;
}
