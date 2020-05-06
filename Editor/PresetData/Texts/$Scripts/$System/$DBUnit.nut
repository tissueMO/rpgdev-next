//###################################################################
//		ユニット スクリプト
//###################################################################

///[summary]バトルイベントを実行する
///[par]ユニットID
///[par]バトルイベントの種別 (BattleEventType)
///[par]行動内容 (行動に関わらないイベントは null が与えられます)
function UnitEvent(intUnitID, eventType, actData) {
	switch(intUnitID) {
		case 0x25206B4F/*チュートリアル*/:
			switch(eventType) {
				case BattleEventType.StartBattle:
					Game.SceneMap.ShowMessage(-1/*なし*/, "", "戦闘のチュートリアルです。\fまずはキャラの行動を決定します。\nここでは「攻撃」を選んでみましょう。");
					break;

				case BattleEventType.StartTurn:
					if(UnitSelfValues.Get(0xC46E1C6/*汎用A*/).Value == 0) {
						Game.SceneMap.ShowMessage(-1/*なし*/, "", "コマンドを選択し終えると、実際に行動が始まります。\nパーティ/エネミーを問わず素早さの早いキャラ順に行動します。");
						UnitSelfValues.Get(0xC46E1C6/*汎用A*/).Value = 1;
					}
					break;

				case BattleEventType.AfterAction:
					if(GetAttackSide(actData.TargetIndex) == AttackSide.Party
					&& UnitSelfValues.Get(0xC46E1C6/*汎用A*/).Value == 1) {
						Game.SceneMap.ShowMessage(-1/*なし*/, "", "ダメージを受けてしまいました。\f" + HPName + "がゼロになると死亡扱いとなり、\n行動したり回復させたりすることができなくなります。");
						UnitSelfValues.Get(0xC46E1C6/*汎用A*/).Value = 2;
					}
					break;

				case BattleEventType.EndingBattleWin:
					Game.SceneMap.ShowMessage(-1/*なし*/, "", "おめでとうございます！\n戦闘に勝利しました。\f倒したエネミーから経験値とお金を得ます。\nレベルアップやスキル習得、拾得アイテムの判定も行われます。");
					break;

				case BattleEventType.EndingBattleLose:
					Game.SceneMap.ShowMessage(-1/*なし*/, "", "残念…。戦闘に敗北してしまいました。\n通常、ゲームオーバーになるとタイトルシーンに戻ります。\fイベント戦闘では設定によって\n強制的にシナリオを進行させることもできます。\fプリセットスクリプトの改造次第では\nHP=1 で常にパーティを復活させることもできます。");
					break;

				case BattleEventType.EndingBattleEscape:
					Game.SceneMap.ShowMessage(-1/*なし*/, "", "戦闘から逃げ出すことに成功しました。\n途中まで倒したエネミーの経験値やお金は一切入手できません。");
					break;
			}
			break;
	}
}
