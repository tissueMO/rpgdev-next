//###################################################################
//		間接効果 スクリプト
//###################################################################

///[summary]罹患したときの処理
///[comment]ゲームプログラムからイベントハンドラーとして呼び出されます。
///[par]間接効果ID
///[par]対象アクター (CDBActorData)
function StateStart(id, actor) {
	if(Game.GetCurrentSubSceneID() != Scene.Battle) {
		return;
	}

	//各種メッセージ表示
	switch(id) {
		case 0xEC71D8F/*睡眠*/:
			ActionMessage(actor.Name + " は 眠ってしまった！", 0.5);
			break;
		case 0x3C6F7958/*毒*/:
			ActionMessage(actor.Name + " は 毒に冒された！", 0.5);
			break;
		case 0x424CADA2/*猛毒*/:
			ActionMessage(actor.Name + " は 猛毒に冒された！", 0.5);
			break;
		case 0x600D76BA/*混乱*/:
			ActionMessage(actor.Name + " は 混乱した！", 0.5);
			break;
		case 0x16AB4BBE/*盲目*/:
			ActionMessage(actor.Name + " は 視力を奪われた！", 0.5);
			break;
		case 0x4CFF4C48/*沈黙*/:
			ActionMessage(actor.Name + " は 魔法を封じられた！", 0.5);
			break;
		case 0x4C3A1FDA/*パワーアップ*/:
			ActionMessage(actor.Name + " は 攻撃力がアップした！", 0.5);
			break;
		case 0x69CE82E8/*ディフェンスアップ*/:
			ActionMessage(actor.Name + " は 守備力がアップした！", 0.5);
			break;
		case 0x5D4CF03C/*スピードアップ*/:
			ActionMessage(actor.Name + " は 敏捷性がアップした！", 0.5);
			break;
	}
}


///[summary]解除されたときの処理
///[comment]ゲームプログラムからイベントハンドラーとして呼び出されます。
///[par]間接効果ID
///[par]対象アクター (CDBActorData)
///[par]上位互換の間接効果に罹患させるために解除されたかどうか
function StateEnd(id, actor, hasUpper) {
	if(hasUpper
	|| Game.GetCurrentSubSceneID() != Scene.Battle
	|| actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value == 0) {
		return;
	}

	//各種メッセージ表示
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	switch(id) {
		case 0xEC71D8F/*睡眠*/:
			ActionMessage(actor.Name + " は 目を覚ました！", 0.5);
			break;
		case 0x3C6F7958/*毒*/:
			ActionMessage(actor.Name + " の 毒が消えた！", 0.5);
			break;
		case 0x424CADA2/*猛毒*/:
			ActionMessage(actor.Name + " の 猛毒が消えた！", 0.5);
			break;
		case 0x600D76BA/*混乱*/:
			ActionMessage(actor.Name + " は 我に返った！", 0.5);
			break;
		case 0x16AB4BBE/*盲目*/:
			ActionMessage(actor.Name + " の 視力が回復した！", 0.5);
			break;

		default:
			ActionMessage(actor.Name + " の " + db.GetCell(id, "名前").String + " が解けた！", 0.5);
			break;
	}
}


///[summary]ターン開始時の処理
///[par]アクターのインデックス
///[par]対象アクター (CDBActorData)
function StateBeforeTurn(actorIdx, actor) {
	//対象アクターが罹患しているすべての間接効果を処理
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {
			case 0xEC71D8F/*睡眠*/:
				ActionMessage(actor.Name + " は 眠っている！", 0);
				break;
		}
	}
}


///[summary]ターン終了時の処理
///[par]アクターのインデックス
///[par]対象アクター (CDBActorData)
function StateAfterTurn(actorIdx, actor) {
	//対象アクターが罹患しているすべての間接効果を処理
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {
			case 0x3C6F7958/*毒*/:
				ActionMessage(actor.Name + " は 毒に冒されている！", 0);
				HPDamage(actorIdx, actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value * 0.1, true);
				break;

			case 0x424CADA2/*猛毒*/:
				ActionMessage(actor.Name + " は 猛毒に冒されている！", 0);
				HPDamage(actorIdx, actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value * 0.2, true);
				break;
		}
	}
}


///[summary]行動前の処理
///[par]対象アクター (CDBActorData)
///[par]行動内容
///[comment]行動を変更するには action を操作します。
function StateBeforeAction(actor, action) {
	//対象アクターが罹患しているすべての間接効果を処理
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {
			case 0x600D76BA/*混乱*/:
				ActionMessage(actor.Name + " は 混乱している！", 0.5);

				//コマンドを強制的に通常攻撃に変更する
				action.Command = MenuItem.Attack;
				action.EffectID = EmptyEquipEffectID;

				//対象を敵味方を問わないランダム選択する
				do {
					if(Game.NextRand(1, 100) <= 50) {
						//パーティを選択
						action.TargetIndex = Game.NextRand(0, Game.DB.VDBParty.GetCharCount() - 1);
					} else {
						//ユニットを選択
						action.TargetIndex = EnemyIndexOrigin + Game.NextRand(0, Enemies.len() - 1);
					}
				} while(IsDead(action.TargetIndex) == true);
				break;

			case 0x4CFF4C48/*沈黙*/:
				if(action.Command == MenuItem.Skill) {
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
					local skillName = db.GetCell(action.Option, "名前").String;
					if(db.GetCell(action.Option, "種別").Integer == 0x297C0F3/*魔法*/) {
						ActionMessage(actor.Name + " は " + skillName + " を唱えようとした！", 0.5);
						ActionMessage("しかし " + actor.Name + " は 言葉を発することができない！", 0.5);
						action.Disabled = true;		//行動を無効化
					}
				}
				break;
		}
	}
}


///[summary]攻撃を受ける前の処理
///[par]対象アクター (CDBActorData)
///[par]行動内容
///[comment]行動を変更するには action を操作します。
function StateBeforeDamage(actor, action) {
	//対象アクターが罹患しているすべての間接効果を処理
	for(local i = 0; i < actor.States.Count(); i++) {
		switch(actor.States.GetID(i)) {

		}
	}
}


///[summary]マップ移動中の処理
///[par]パーティメンバーのインデックス
function StateMap(idx) {
	local actor = GetActorData(idx);
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);

	//対象アクターが罹患しているすべての間接効果を処理
	for(local i = 0; i < actor.States.Count(); ) {
		local id = actor.States.GetID(i);
		actor.States.GetSelfValues(i).Get(0x2C0353DF/*経過歩数*/).Value++;

		// 5歩ごとに処理
		if(actor.States.GetSelfValues(i).Get(0x2C0353DF/*経過歩数*/).Value % 5 == 0) {
			switch(id) {
				case 0x3C6F7958/*毒*/:
				case 0x424CADA2/*猛毒*/:
					Game.DB.FDBMaterial.Play(0x4C6DF6BD/*ダメージ音*/, 100, 0, 0);
					Game.DXLib.Flash(RED, 180, 16, 1);
					if(actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value > 10) {
						PlusActorExParameter(idx, ExParameter.HP, -10);
					}
					break;
			}
		}

		//解除判定
		if(db.GetCell(id, "解除条件").Integer == StateSolve.Walk
		&& db.GetCell(id, "解除条件値").Integer <= actor.States.GetSelfValues(i).Get(0x2C0353DF/*経過歩数*/).Value
		&& Game.NextRand(1, 100) <= db.GetCell(id, "解除確率").Integer) {
			actor.ReleaseState(id);
			continue;
		}
		i++;
	}
}


///[summary]指定した解除条件について解除判定を行い、満たされた場合は解除を行う
///[par]対象アクター (CDBActorData)
///[par]解除条件 (StateSolve)
function StateJudgeRelease(actor, stateSolve) {
	//対象アクターが罹患しているすべての間接効果を処理
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	for(local i = 0; i < actor.States.Count(); ) {
		local id = actor.States.GetID(i);
		local solve = db.GetCell(id, "解除条件").Integer;

		//解除条件が一致する場合のみ解除判定を行う
		if(solve == stateSolve) {
			if(solve == StateSolve.Turn
			&& actor.States.GetSelfValues(i).Get(0x45B7FFE4/*経過ターン数*/).Value < db.GetCell(id, "解除条件値").Integer) {
				//必要ターン数に達していない
				i++;
				continue;
			} else if(Game.NextRand(1, 100) <= db.GetCell(id, "解除確率").Integer) {
				//解除する
				actor.ReleaseState(id);
				continue;
			}
		}
		i++;
	}
}


///[summary]現在のアクター全員について間接効果の経過ターン数を加算して解除判定
function StatePlusTurnActorAll() {
	//パーティとユニットを混合させたリストを生成して一括処理する
	local allActorList = CreateAllActorDataList();
	foreach(actor in allActorList) {
		for(local n = 0; n < actor.States.Count(); n++) {
			actor.States.GetSelfValues(n).Get(0x45B7FFE4/*経過ターン数*/).Value++;
		}
		StateJudgeRelease(actor, StateSolve.Turn);
	}
}


///[summary]パーティ全員の戦闘時のみ有効な間接効果をすべて解除
function StateReleaseOnlyBattle() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == true) {
			continue;		//死んでいるキャラは処理しない
		}

		local actor = GetActorData(i);
		for(local n = 0; n < actor.States.Count(); ) {
			local id = actor.States.GetID(n);
			local onlyBattle = !db.GetCell(id, "移動中も有効").Boolean;
			if(onlyBattle == true) {
				actor.ReleaseState(id);
				continue;
			}
			n++;
		}
	}
}


///[summary]パーティ全員のマップ移動中の処理を実行
function StateMapPartyAll() {
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == true) {
			continue;		//死んでいるキャラは処理しない
		}
		StateMap(i);
	}
}
