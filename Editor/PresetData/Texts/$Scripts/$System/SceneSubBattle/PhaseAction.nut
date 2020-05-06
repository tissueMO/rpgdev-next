//###################################################################
//		バトルシーン: サブルーチン: 行動フェーズ
//###################################################################

///[summary]行動フェーズの処理
function Routine_PhaseAction() {
	//行動を逐次取り出して処理する
	while(Actions.len() > 0 && Game.Wait(0, true, false)) {
		local actData = Actions.pop();
		local actor = GetActorData(actData.ActorIndex);
		local actorName = actor.Name;
		ActingIndex = actData.ActorIndex;		//行動中のアクターがハイライトされる

		//行動者が現在行動できない状態の時はスキップする
		if(IsForbiddenAction(actData.ActorIndex) == true) {
			continue;
		}

		//無効な行動であるときはスキップする
		if(actData.Command == -1) {
			continue;
		}

		//行動者: 行動前の間接効果処理
		StateBeforeAction(actor, actData);
		if(actData.Disabled == true) {
			continue;			//間接効果によって行動が無効化されたらスキップする
		}

		//ユニットイベント: 行動前
		UnitEvent(UnitID, BattleEventType.BeforeAction, actData);

		//対象者: 攻撃を受ける前の間接効果処理
		local target = (actData.TargetIndex == -1) ? null : GetActorData(actData.TargetIndex);
		local targetName = (target == null) ? "" : target.Name;
		if(target != null) {
			StateBeforeDamage(target, actData);
			if(actData.Disabled == true) {
				continue;		//間接効果によって行動が無効化されたらスキップする
			}
			target = GetActorData(actData.TargetIndex);
		}

		//コマンドに応じた処理
		ActionCommands[actData.Command](actData, actor, actorName, target, targetName);

		//ユニットイベント: 行動後
		UnitEvent(UnitID, BattleEventType.AfterAction, actData);

		//戦闘終了判定
		local result = JudgeBattleExit();
		if(result != -1) {
			MoveToEndingPhase(result);
			return;
		}
	}

	//ターン経過で解除されるかもしれない間接効果の判定
	StatePlusTurnActorAll();

	//ユニットイベント: ターン終了時
	UnitEvent(UnitID, BattleEventType.EndingTurn, null);

	//ターン終了
	TurnCount++;
	DoTurnEndingTasks();

	//戦闘終了判定
	local result = JudgeBattleExit();
	if(result != -1) {
		MoveToEndingPhase(result);
		return;
	}

	MoveToCommandPhase();
}


//コマンドに応じた行動処理
ActionCommands <- {

	///[summary]攻撃
	///[par]行動内容
	///[par]行動者 (CDBActorData)
	///[par]行動者名
	///[par]対象者 (CDBActorData)
	///[par]対象者名
	[MenuItem.Attack] = function(actData, actor, actorName, target, targetName) {
		//物理攻撃
		ActionMessage(actorName + " の攻撃！", 0);
		if(actData.TargetIndex != -1 && IsDead(actData.TargetIndex) == true) {
			//対象が死んでいる場合はランダム選択する
			actData.TargetIndex = SelectRandom(GetAttackSide(actData.TargetIndex));
		}

		//エフェクト表示
		PlayActionEffect(actData.EffectID, GetAgainstAttackSide(actData.ActorIndex), null, actData);

		//武器属性を取得
		local attrList = CIntegerArray();
		local equipID = actor.GetEquipIDFromItemTypeID(0x12414D21/*武器*/);
		if(equipID != -1) {
			attrList = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(equipID, "属性").Integers;
		}

		//効果内容
		if(actData.TargetIndex < 0) {
			//全体攻撃
			HPDamageSideAll(
				actData,
				GetAgainstAttackSide(GetAttackSide(actData.ActorIndex)),
				UseType.Physic,
				attrList,
				0, 10, 100
			);
		} else {
			//単体攻撃
			HPDamage(
				actData.TargetIndex,
				CalcPhysicalAttack(actor, target, attrList, 0, 10, 100, true),
				true
			);
		}
	},


	///[summary]防御
	///[par]行動内容
	///[par]行動者 (CDBActorData)
	///[par]行動者名
	///[par]対象者 (CDBActorData)
	///[par]対象者名
	[MenuItem.Defense] = function(actData, actor, actorName, target, targetName) {
		ActionMessage(actorName + " は身を固めている！", 0);
	},


	///[summary]スキル
	///[par]行動内容
	///[par]行動者 (CDBActorData)
	///[par]行動者名
	///[par]対象者 (CDBActorData)
	///[par]対象者名
	[MenuItem.Skill] = function(actData, actor, actorName, target, targetName) {
		local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
		local skillName = db.GetCell(actData.Option, db.GetColumnName(DBDefaultColumnIndices.Name)).String;
		local skillTypeID = db.GetCell(actData.Option, "種別").Integer;
		local targetType = db.GetCell(actData.Option, "対象種別").Integer;

		switch(skillTypeID) {
			case 0x297C0F3/*魔法*/:
				ActionMessage(actorName + " は " + skillName + " を 唱えた！", 0);
				break;

			case 0x5D2DCD84/*特技*/:
				ActionMessage(actorName + " は " + skillName + " を 放った！", 0);
				break;
		}

		//対象が死んでいる場合の修正処理
		switch(db.GetCell(actData.Option, "対象種別").Integer) {
			case TargetType.OneEnemy:
			case TargetType.OneParty:
				if(actData.TargetIndex != -1 && IsDead(actData.TargetIndex) == true) {
					actData.TargetIndex = SelectRandom(GetAttackSide(actData.TargetIndex));
				}
				break;
		}

		//エフェクト表示
		if(targetType != TargetType.None) {
			PlayActionEffect(db.GetCell(actData.Option, "エフェクト").Integer, null, db, actData);
		}

		//使用処理
		if(UseSkill(actData.Option, actData.ActorIndex, actData.TargetIndex) == true
		&& GetAttackSide(actData.ActorIndex) == AttackSide.Party) {
			//パーティのみMP消費
			local mp = actor.SelfValues.Get(0x46A20EE9/*MP*/);
			mp.Value = Game.WithIn(mp.Value - db.GetCell(actData.Option, "コスト").Integers.Get(0), 0, MaxExParameter);
		}
	},


	///[summary]アイテム
	///[par]行動内容
	///[par]行動者 (CDBActorData)
	///[par]行動者名
	///[par]対象者 (CDBActorData)
	///[par]対象者名
	[MenuItem.Item] = function(actData, actor, actorName, target, targetName) {
		local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
		local itemName = db.GetCell(actData.Option, db.GetColumnName(DBDefaultColumnIndices.Name)).String;
		local targetType = db.GetCell(actData.Option, "対象種別").Integer;

		ActionMessage(actorName + " は " + itemName + " を 使った！", 0);

		//対象が死んでいる場合の修正処理
		switch(db.GetCell(actData.Option, "対象種別").Integer) {
			case TargetType.OneEnemy:
			case TargetType.OneParty:
				if(actData.TargetIndex != -1 && IsDead(actData.TargetIndex) == true) {
					actData.TargetIndex = SelectRandom(GetAttackSide(actData.TargetIndex));
				}
				break;
		}

		//エフェクト表示
		if(targetType != TargetType.None) {
			PlayActionEffect(db.GetCell(actData.Option, "エフェクト").Integer, null, db, actData);
		}

		//使用処理
		if(UseItem(actData.Option, actData.ActorIndex, actData.TargetIndex, false) == true
		&& GetAttackSide(actData.ActorIndex) == AttackSide.Party) {
			//パーティのみ消費処理
			Game.DB.VDBParty.DeleteItem(actData.Option, 1);
		}
	},


	///[summary]逃げる
	///[par]行動内容
	///[par]行動者 (CDBActorData)
	///[par]行動者名
	///[par]対象者 (CDBActorData)
	///[par]対象者名
	[MenuItem.Escape] = function(actData, actor, actorName, target, targetName) {
		Game.DB.FDBMaterial.Play(0x7C1FDDE0/*逃げる*/, 255, 0, 0);
		if(GetAttackSide(actData.ActorIndex) == AttackSide.Party) {
			//パーティの場合は先頭者名で表示
			ActionMessage(GetHeadActorName(AttackSide.Party) + " は 逃げ出した！", 0);

			//逃走判定
			if(JudgeEscape(actData.Option) == true) {
				//逃走成功
				PTEscaped = true;
				return;
			} else {
				//逃走失敗
				ActionMessage("しかし 逃げ損ねてしまった…！", 0.5);
			}
		} else  {
			//エネミーの場合はフラグを立てる
			ActionMessage(actorName + " は 逃げ出した！", 0.5);
			actor.SelfValues.Get(0x5486E688/*逃走したかどうか*/).Value = TRUE;
		}
	},
};


///[summary]行動リストをアクターの敏捷性の昇順に並び替える
function SortActions() {
	//各アクターの敏捷性に速度補正をかけて、分散値を加える
	foreach(act in Actions) {
		local actor = GetActorData(act.ActorIndex);

		switch(act.Command) {
			case MenuItem.Skill:
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value *=
					Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(act.Option, "速度補正").Integer;
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value /= 100;
				break;

			case MenuItem.Item:
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value *=
					Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(act.Option, "速度補正").Integer;
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value /= 100;
				break;

			case MenuItem.Escape:
				//逃げる行動はターンの最初に行う
				GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value = MaxExParameter + 1;
				break;
		}

		GetActorData(act.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value +=
			Game.NextRand(0, actor.GetParameter(0x3884DE0B/*敏捷性*/).Value * SpeedVarianceRate / 100);
	}

	//分散値を考慮した上で並び替えを実行する
	Actions.sort(function(a, b) {
		local speedA = GetActorData(a.ActorIndex).GetParameter(0x3884DE0B/*敏捷性*/).Value +
						GetActorData(a.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value;
		local speedB = GetActorData(b.ActorIndex).GetParameter(0x3884DE0B/*敏捷性*/).Value +
						GetActorData(b.ActorIndex).SelfValues.Get(0x335C5037/*敏捷性の分散値*/).Value;
		return ((speedA < speedB) ? -1 : ((speedA > speedB) ? 1 : 0));
	});

	//デバッグ出力
	if(DEBUG) {
		print(format("[%d ターン目]", TurnCount));
		print("行動順番:");
		foreach(idx, act in Actions) {
			local actor = GetActorData(act.ActorIndex);
			print("\t" + (Actions.len() - idx).tostring() + ": " + actor.Name);
		}
	}
}


///[summary]ターン開始時の処理
function DoTurnStartingTasks() {
	//防御するアクターの処理: 防御フラグの操作
	foreach(act in Actions) {
		if(act.Command == MenuItem.Defense) {
			GetActorData(act.ActorIndex).SelfValues.Get(0x7BD2A434/*防御中かどうか*/).Value = TRUE;

			//復元操作を予約する
			AddTurnEndingTask(
				{ ["idx"] = act.ActorIndex, },
				function(args) {
					GetActorData(args["idx"]).SelfValues.Get(0x7BD2A434/*防御中かどうか*/).Value = FALSE;
				}
			);
		}
	}

	//パーティキャラの「逃げる」は一つにまとめる
	local counter = 0;
	local escapeActIndex = -1;
	for(local i = 0; i < Actions.len(); ) {
		if(GetAttackSide(Actions[i].ActorIndex) == AttackSide.Party
		&& Actions[i].Command == MenuItem.Escape) {
			counter++;
			if(counter == 1) {
				escapeActIndex = i;
				Actions[escapeActIndex].Option = 1;
			} else if(counter >= 2) {
				//2人目以降の逃げる行動は無効にして、逃げる人数を更新する
				Actions.remove(i);
				Actions[escapeActIndex].Option++;
				continue;
			}
		}
		i++;
	}

	//ユニットイベント: ターン開始時
	UnitEvent(UnitID, BattleEventType.StartTurn, null);

	//間接効果処理: ターン開始時
	local actors = CreateAllActorDataList();
	local actorIdxs = CreateAllActorIndexList();
	foreach(idx, actor in actors) {
		StateBeforeTurn(actorIdxs[idx], actor);
	}
}


///[summary]ターン終了時の処理を追加する
///[par]処理に必要な引数 (テーブル)
///[par]処理内容を定義した関数
function AddTurnEndingTask(args, func) {
	TurnEndingTaskArgs.append(args);
	TurnEndingTasks.append(func);
}


///[summary]ターン終了時の処理
function DoTurnEndingTasks() {
	//間接効果処理: ターン終了時
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		StateAfterTurn(i, GetActorData(i));
	}
	for(local i = 0; i < Enemies.len(); i++) {
		StateAfterTurn(EnemyIndexOrigin + i, GetActorData(EnemyIndexOrigin + i));
	}

	while(TurnEndingTasks.len() > 0 && TurnEndingTaskArgs.len() > 0) {
		local task = TurnEndingTasks.pop();
		local args = TurnEndingTaskArgs.pop();
		task(args);
	}

	//タスクを初期化
	TurnEndingTasks.resize(0);
	TurnEndingTaskArgs.resize(0);
}


///[summary]アクターのインデックスからエフェクトの表示座標を返す
///[par]アクターのインデックス (マイナスのときはサイド全体を示す)
///[par]対象サイド
///[return]表示座標 (CPoint)
function GetEffectLocation(idx, destSide) {
	local result = CPoint();
	if(idx <= -1) {
		if(destSide == AttackSide.Party) {
			//パーティ全体
			result.Set(
				Game.WindowSize.Width / 2,
				GetActorData(0).Location.Y + PartyPanelHeight / 2
			);
		} else {
			//ユニット全体
			result.Set(
				Game.WindowSize.Width / 2,
				Game.WindowSize.Height / 2
			);
		}
	} else if(destSide == AttackSide.Party) {
		//パーティ
		result.Set(
			GetActorData(idx).Location.X + (Game.WindowSize.Width / Game.DB.FDBSystem.GetInteger(DB_System_MaxPartyCount)) / 2,
			GetActorData(idx).Location.Y + PartyPanelHeight / 2
		);
	} else {
		//ユニット
		result.Set(
			GetActorData(idx).Location.X + GetActorData(idx).GraphSize.Width / 2,
			GetActorData(idx).Location.Y + GetActorData(idx).GraphSize.Height / 2
		);
	}
	return result;
}


///[summary]エフェクト再生
///[par]エフェクトID
///[par]対象サイド (AttackSide)、単体対象の行動はnullで構いません。
///[par]アイテム/スキルのデータベース
///[par]行動内容 (SQActionData)
function PlayActionEffect(effectID, destSide, db, actData) {
	if(Game.GetKeyInputCounter(KeyID.A) > 0) {
		//高速化しているときは表示しない
		return;
	}

	if(0 <= actData.TargetIndex) {
		//単一選択時は対象者のインデックスから対象サイドを持ってくる
		destSide = GetAttackSide(actData.TargetIndex);
	} else if(db != null) {
		switch(db.GetCell(actData.Option, "対象種別").Integer) {
			case TargetType.AllParty:
			case TargetType.AllPartyWithDead:
				//パーティはパーティ、ユニットはユニットを対象とする
				destSide = GetAttackSide(actData.ActorIndex);
				break;
			case TargetType.AllEnemy:
				//パーティはユニット、ユニットはパーティを対象とする
				destSide = GetAgainstAttackSide(GetAttackSide(actData.ActorIndex));
				break;
		}
	}

	local effectLocation = null;
	if(Game.DB.FDBEffect.GetSubDB(0).GetCell(effectID, "描画対象").Integer == DBEffectViewPosition.Each && actData.TargetIndex < 0) {
		//全体攻撃でエフェクトが対象個別である場合は一人ひとりに表示する
		local effectHandle = -1;
		for(local i = GetFirstActorIndex(destSide); i != -1; i = GetNextActorIndex(i)) {
			effectLocation = GetEffectLocation(i, destSide);
			effectHandle = Game.DB.FDBEffect.Show(effectID, effectLocation.X, effectLocation.Y);
		}

		//最後に再生したエフェクトの再生が終了するまで待つ
		while(Game.DB.FDBEffect.GetEffectData(effectHandle).IsNull() == false
		&& Game.DB.FDBEffect.GetEffectData(effectHandle).WaitBreakFlag == false
		&& Game.Wait(0, true, true) == true) {
			//処理終了待ち
		}
	} else {
		//単一エフェクト表示
		effectLocation = GetEffectLocation(actData.TargetIndex, destSide);
		Game.DB.FDBEffect.ShowForWait(effectID, effectLocation.X, effectLocation.Y);
	}
}
