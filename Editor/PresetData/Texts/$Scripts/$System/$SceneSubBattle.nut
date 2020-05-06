//###################################################################
//		バトルシーン
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\!var.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Common.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Draw.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\MainRoutine.nut");

//####################  シーン起動  ####################
function SceneSubSetup() {
	Phase = BattlePhase.Setup;
	Game.PrepareTransition();
	Game.DoTransition(
		Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.BattleStartWipeGraph, "ファイル名").String,
		SquareSpeed.High
	);
	TurnCount = 1;
}


//####################  シーン描画  ####################
function SceneSubDraw() {
	if(Phase == BattlePhase.Setup) {
		return;		//準備中は描画しない
	}
	BattleDraw();
}


//####################  シーン入力  ####################
function SceneSubInput() {
	BattleInput();
}


//####################  シーン破棄  ####################
function SceneSubDispose() {
	//エネミーグラフィック破棄
	foreach(enemy in Enemies) {
		enemy.DisposeGraphics();
	}
	print("戦闘を終了します。");
}


//####################  シーンを初期化して開始  ####################
function InitBattle(intUnitID, strBack, strBGM, boolDisabledEscape, boolDisabledGameOver) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Unit).GetSubDB(0);

	if (db.GetCell(intUnitID, "ID").IsNull() == true) {
		//ユニットIDが存在しない場合はエラーで落とす
		print("指定されたユニットIDが存在しないため、戦闘を開始できませんでした。UnitID=" + intUnitID);
		Game.FadeOut(SquareSpeed.High);
		Game.EndSubScene();
		return;
	}

	local src = db.GetCell(intUnitID, "配置情報").String;
	local enemyList = split(src, "|");

	//引数をグローバル変数に代入する
	UnitID = intUnitID;
	FileBack = strBack;
	FileBGM = strBGM;
	DisabledEscape = boolDisabledEscape;
	DisabledGameOver = boolDisabledGameOver;

	//ユニットの個別変数データを初期値でセットする
	UnitSelfValues = Game.DB.VDBValue.GetSelfValues(DBValueIndices.SelfUnit);

	//エネミーをアクターとして生成
	Enemies.resize(0);
	foreach(enemy in enemyList) {
		local spl = "";
		spl = split(enemy, ";");		// [ID], [X:Y] に分割する
		local enemyData = CDBActorData.CreateActorFromEnemy(spl[0].tointeger());
		spl = split(spl[1], ":");
		enemyData.Location.Set(spl[0].tointeger(), spl[1].tointeger());
		enemyData.LoadGraphics(enemyData.Graphics);

		//パラメーター設定
		enemyData.SelfValues.Get(0x7BAC0D68/*HP*/).Value = enemyData.GetParameter(0x2019EC10/*最大ＨＰ*/).Value;
		enemyData.SelfValues.Get(0x7BD2A434/*防御中かどうか*/).Value = FALSE;

		Enemies.append(enemyData);
		DamageMotionUT.append(SQShakeData());
	}

	//同一IDのエネミーにナンバリングする
	foreach(idx, enemy in Enemies) {
		if(enemy.SelfValues.Get(0x725E9664/*ナンバリング完了*/).Value == TRUE) {
			continue;		//ナンバリング済みのエネミーはスキップ
		}

		//同一IDのエネミー数を数える
		local sames = array(0);
		foreach(subEnemy in Enemies) {
			if(subEnemy.Name == enemy.Name) {
				sames.append(subEnemy);
			}
		}
		if(sames.len() <= 1) {
			//唯一のエネミーにはナンバリングしない
			enemy.SelfValues.Get(0x725E9664/*ナンバリング完了*/).Value = TRUE;
			continue;
		}

		//該当したエネミーに対してナンバリングする
		foreach(sub, subEnemy in sames) {
			subEnemy.Name = subEnemy.Name + Game.IntToAlphabet(sub);
			subEnemy.SelfValues.Get(0x725E9664/*ナンバリング完了*/).Value = TRUE;
		}
	}

	//パーティに対する処理
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = GetActorData(i);
		actor.Location.Set(
			Game.WindowSize.Width / Game.DB.FDBSystem.GetInteger(DB_System_MaxPartyCount) * i,
			Game.WindowSize.Height - CommentPanelHeight - PartyPanelHeight
		);
		actor.SelfValues.Get(0x7BD2A434/*防御中かどうか*/).Value = FALSE;
		PTPanelShakes.append(SQShakeData());
	}

	//戦闘背景グラフィックの読み込み
	if(FileBack == null || FileBack.len() == 0) {
		//標準の戦闘背景を使う
		HdlBack = Game.Material.Graphics.Add(Game.SceneMap.MapData.BattleBackFileName);
	} else {
		HdlBack = Game.Material.Graphics.Add(FileBack);
	}

	//戦闘BGMの再生
	if(Game.BGMKeep == false) {
		PauseMapBGMBGS();
		Game.Wait(0.75, true, false);

		if(FileBGM == null || FileBGM.len() == 0) {
			//標準の戦闘BGMを使う
			HdlBGM = Game.Material.Sounds.Add(Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.BattleBGM, "ファイル名").String);
		} else {
			HdlBGM = Game.Material.Sounds.Add(FileBGM)
		}
		Game.Material.Sounds.Play(HdlBGM, PlayType.Loop, 255, 0, 0, 0);
	}

	Phase = BattlePhase.Start;
	Game.FadeIn(SquareSpeed.High);

	//デバッグ用出力
	print("ユニット一覧:");
	foreach(idx, enemy in Enemies) {
		print("\t" + idx.tostring() + ": " + enemy.ID.tostring() + " [" + enemy.Name + "] (" + enemy.Location.X.tostring() + "," + enemy.Location.Y.tostring() + ")");
	}
	print("戦闘が開始されました。");

	//ユニットイベント: 戦闘開始時
	UnitEvent(UnitID, BattleEventType.StartBattle, null);
}
