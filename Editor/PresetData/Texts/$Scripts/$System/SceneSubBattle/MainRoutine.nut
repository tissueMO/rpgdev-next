//###################################################################
//		バトルシーン: メインルーチン
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\PhaseAction.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\PhaseCommand.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\PhaseEnding.nut");

///[comment]入力処理から呼ばれるメインルーチン
function BattleInput() {
	//イベントから呼び出される戦闘も考えられるため、リセットは禁止する
	//if(Game.GetKeyInputCounter(KeyID.F12) == 1) {
	//	GameReset();
	//	return;
	//}

	switch(Phase) {
		case BattlePhase.Start:
			// NOTE: 先制攻撃・不意打ち等の処理はここに挿入します。
			MoveToCommandPhase();
			break;

		case BattlePhase.Command:
			Routine_PhaseCommand();
			break;

		case BattlePhase.Action:
			Routine_PhaseAction();
			break;
	}
}


///[summary]コマンド選択フェーズに移る
function MoveToCommandPhase() {
	Actions.resize(0);
	Cursor = array(DepthCount, 0);
	TopIndex = array(DepthCount, 0);
	Phase = BattlePhase.Command;

	//最初のキャラのコマンド選択に移る
	TopIndex[0] = -1;
	PhaseCommandSubRoutine.MoveToNextActorCommand(null);
}


///[summary]行動フェーズに移る
function MoveToActionPhase() {
	//自律戦闘のパーティキャラはAIを呼び出して行動内容を決定する
	print("パーティNPCの行動内容:");
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
		if(IsForbiddenAction(i) == true) {
			continue;		//行動できない状態
		} else if(actor.IsNPC == false) {
			continue;		//プレイヤーによってコマンド選択されている
		}
		local actions = array(0);
		actions.append(SQActionData(i, -1, -1, -1, -1));
		SelectCommandChar(actor.ID, actions);
		if(actions.len() == 0) {
			continue;		//行動しないことになった
		}

		//行動決定
		foreach(act in actions) {
			act.Print();
			Actions.append(act);
		}
	}

	//ユニットのAIを呼び出して行動内容を決定する
	print("エネミーの行動内容:");
	foreach(idx, enemy in Enemies) {
		if(IsForbiddenAction(EnemyIndexOrigin + idx) == true) {
			continue;		//行動できない状態
		}

		local actions = array(0);
		actions.append(SQActionData(EnemyIndexOrigin + idx, -1, -1, -1, -1));
		SelectCommandEnemy(enemy.ID, actions);
		if(actions.len() == 0) {
			continue;		//行動しないことになった
		}

		//行動決定
		foreach(act in actions) {
			act.Print();
			Actions.append(act);
		}
	}

	SortActions();				//行動順番を決定する
	Phase = BattlePhase.Action;
	DoTurnStartingTasks();		//ターン開始時の処理が先行する
}


///[comment]戦闘終了フェーズに移る
///[par]戦闘結果 (BattleResult)
function MoveToEndingPhase(result) {
	Result = result;
	Game.Material.Sounds.Stop(HdlBGM, 0.5, false);
	Game.Wait(0.5, true, false);
	Game.Material.Sounds.Delete(HdlBGM);

	//戦闘結果に基づいてサブルーチンに移行する
	PhaseEndingSubRoutine[result]();
	if(result == BattleResult.Lose && DisabledGameOver == false) {
		return;		//ゲームオーバーなのでタイトル画面へ移行する
	}

	//戦闘時のみ有効な間接効果をすべて解除する
	StateReleaseOnlyBattle();

	//サブシーンを終了してマップに復帰する
	switch(result) {
		case BattleResult.Win:
			print("戦闘結果: 勝利");
			break;
		case BattleResult.Lose:
			print("戦闘結果: 敗北");
			break;
		case BattleResult.Escape:
			print("戦闘結果: 逃走");
			break;
		case BattleResult.Break:
			print("戦闘結果: 中断");
			break;
	}
	Game.FadeOut(SquareSpeed.High);
	Game.EndSubScene();

	//死んでしまったパーティメンバーをすべて蘇生する
	RecoverPartyAll();

	//戦闘の呼び出し側でフェードインするのでここでは処理しない
	//Game.FadeIn(SquareSpeed.High);
}
