//###################################################################
//		バトルシーン: 汎用処理
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\Common.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Classes.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Expression.nut");


///[summary]死亡しているパーティメンバーを全員蘇生
///[comment]戦闘終了後などに蘇生する手段がない場合のために、死亡しているメンバーを全員 HP=1 で蘇生させます。
function RecoverPartyAll() {
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == false) {
			continue;
		}

		local actor = GetActorData(i);
		actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value = 1;
	}
}


///[summary]説明欄のテキストを定義
///[return]説明欄に表示すべきテキスト
function GetBattleComment() {
	switch(Phase) {
		case BattlePhase.Action:
			return ActionMsg;

		case BattlePhase.Command:
			if(TargetUTSelecting == true) {
				return "対象エネミーを選択して下さい。";
			} else if(TargetPTSelecting == true) {
				return "対象メンバーを選択して下さい。";
			} else if(Depth == 0) {
				return "コマンドを選択して下さい。";
			}
			switch(Cursor[0]) {
				case MenuItem.Skill:
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
					switch(Depth) {
						case 1:		return "種別を選択して下さい。";
						case 2:		return db.GetCell(SkillList[Cursor[2] + TopIndex[2]].ID, "注釈").String;
					}
					break;

				case MenuItem.Item:
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
					switch(Depth) {
						case 1:		return "種別を選択して下さい。";
						case 2:
							switch(Cursor[1]) {
								case CommandItemType.Having:
									return db.GetCell(Game.DB.VDBParty.GetItemDataFromIndex(Cursor[2] + TopIndex[2]).ID, "注釈").String;
								case CommandItemType.Equip:
									local id = actor.GetEquipIDFromItemTypeID(EquipTypeList[Cursor[2]]);
									if(id == -1) {
										return "装備していない状態です。";
									}
									return db.GetCell(id, "注釈").String;
							}
					}
					break;
			}
			break;
	}
	return "";
}


///[summary]行動メッセージを表示
///[par]メッセージ内容
///[par]標準の待ち時間に加えて待つ秒数
function ActionMessage(strText, plusTime) {
	ActionMsg = strText;
	ActionWait(plusTime);
}


///[summary]行動後のウェイト処理
///[par]標準の待ち時間に加えて待つ秒数
function ActionWait(plusTime) {
	if(Phase == BattlePhase.Ending) {
		//戦闘終了後は一切のウェイトを挟まない
		return;
	} else if(Game.GetKeyInputCounter(KeyID.A) > 0) {
		//高速化
		Game.Wait(0.1, true, true);
	} else {
		Game.Wait((0.5 + plusTime) * UI315411244/*戦闘メッセージ表示速度*/, true, true);
	}
}


///[summary]パーティメンバー選択
///[par]死者を選択できるようにするかどうか
///[return]選択インデックス。キャンセルされた場合は -1 が返されます。
function SelectPTMember(boolDeadMode) {
	TargetPTAllowDead = boolDeadMode;
	TargetPTSelecting = true;
	TopIndex[TargetPTDepth] = 0;

	//最初のインデックスを決める
	Cursor[TargetPTDepth] = 0;
	while(boolDeadMode == false && IsDead(Cursor[TargetPTDepth]) == true) {
		Cursor[TargetPTDepth]++;
	}

	while(Game.Wait(0, true, true)) {
		if(CheckLeftKeyInput() == true) {
			//前のキャラへ
			for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
				Cursor[TargetPTDepth] = Game.DownAround(Cursor[TargetPTDepth], 0, Game.DB.VDBParty.GetCharCount());

				//死んでいる場合は飛ばす
				if(boolDeadMode == false && IsDead(Cursor[TargetPTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckRightKeyInput() == true) {
			//次のキャラへ
			for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
				Cursor[TargetPTDepth] = Game.UpAround(Cursor[TargetPTDepth], 0, Game.DB.VDBParty.GetCharCount());

				//死んでいる場合は飛ばす
				if(boolDeadMode == false && IsDead(Cursor[TargetPTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckDecideKeyInput() == 1) {
			//決定
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//キャンセル: カーソル位置をマイナスにする
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
			Cursor[TargetPTDepth] = -1;
			break;
		}
	}
	TargetPTSelecting = false;

	//選択結果を返す
	return Cursor[TargetPTDepth];
}


///[summary]エネミー選択
///[return]選択インデックス。キャンセルされた場合は -1 が返されます。
function SelectUTEnemy() {
	TargetUTSelecting = true;
	Cursor[TargetUTDepth] = 0;
	TopIndex[TargetUTDepth] = 0;

	//最初のインデックスを決める
	Cursor[TargetUTDepth] = 0;
	while(IsDead(EnemyIndexOrigin + Cursor[TargetUTDepth]) == true) {
		Cursor[TargetUTDepth]++;
	}

	while(Game.Wait(0, true, true)) {
		if(CheckLeftKeyInput() == true) {
			//前のエネミーへ
			for(local i = 0; i < Enemies.len(); i++) {
				Cursor[TargetUTDepth] = Game.DownAround(Cursor[TargetUTDepth], 0, Enemies.len());

				//死んでいる場合は飛ばす
				if(IsDead(EnemyIndexOrigin + Cursor[TargetUTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckRightKeyInput() == true) {
			//次のエネミーへ
			for(local i = 0; i < Enemies.len(); i++) {
				Cursor[TargetUTDepth] = Game.UpAround(Cursor[TargetUTDepth], 0, Enemies.len());

				//死んでいる場合は飛ばす
				if(IsDead(EnemyIndexOrigin + Cursor[TargetUTDepth]) == true) {
					continue;
				}
				break;
			}
		} else if(CheckDecideKeyInput() == 1) {
			//決定
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			break;
		} else if(CheckCancelKeyInput() == 1) {
			//キャンセル: カーソル位置をマイナスにする
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
			Cursor[TargetUTDepth] = -1;
			break;
		}
	}
	TargetUTSelecting = false;

	//選択結果を返す
	return (Cursor[TargetUTDepth] == -1) ? -1 : Cursor[TargetUTDepth] + EnemyIndexOrigin;
}


///[summary]指定したサイドの対象をランダムに選択する
///[par]対象サイド (AttackSide)
///[return]アクターのインデックス
function SelectRandom(side) {
	local idx = -1;
	if(side == AttackSide.Party) {
		do {
			idx = Game.NextRand(0, Game.DB.VDBParty.GetCharCount() - 1);
		} while(IsDead(idx) == true);
	} else {
		do {
			idx = EnemyIndexOrigin + Game.NextRand(0, Enemies.len() - 1);
		} while(IsDead(idx) == true);
	}
	return idx;
}


///[summary]対象選択中であるかどうかを調べる
///[return]対象選択中かどうか
function IsTargetSelecting() {
	return (TargetPTSelecting || TargetUTSelecting);
}


///[summary]パーティキャラ/エネミーのアクター情報リストを混合させた配列を返す
///[return]CDBActorData配列
function CreateAllActorDataList() {
	local allActorList = array(0);

	//パーティキャラ
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		allActorList.append(Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i)));
	}

	//エネミー
	foreach(enemy in Enemies) {
		allActorList.append(enemy);
	}
	return allActorList;
}


///[summary]パーティキャラ/エネミーのアクターインデックスを混合させた配列を返す
///[return]Integer配列
function CreateAllActorIndexList() {
	local allActorList = array(0);

	//パーティキャラ
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		allActorList.append(i);
	}

	//エネミー
	foreach(idx, enemy in Enemies) {
		allActorList.append(EnemyIndexOrigin + idx);
	}
	return allActorList;
}


///[summary]指定したIDのアイテムを行動予約している個数を調べる
///[par]アイテムID
///[return]行動予約数
function GetReservedItemCount(itemID) {
	//消耗品でない場合は無効
	if(Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(itemID, "消耗品").Boolean == false) {
		return 0;
	}

	//行動予約の中からパーティキャラのアイテムコマンドで同一のアイテムIDを選択しているデータを数える
	local count = 0;
	foreach(data in Actions) {
		if(GetAttackSide(data.ActorIndex) == AttackSide.Party
		&& data.Command == MenuItem.Item
		&& data.Option == itemID) {
			count++;
		}
	}
	return count;
}


///[summary]スキル種別リストを更新
function RefreshSkillTypeList() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.SkillType);
	SkillTypeList.resize(0);
	for(local i = 0; i < db.GetRowCount(); i++) {
		local data = CIDNameData();
		data.ID = db.GetCellFromIndex(i, DBDefaultColumnIndices.FixedID).Integer;
		data.Name = db.GetCellFromIndex(i, DBDefaultColumnIndices.Name).String;
		SkillTypeList.append(data);
	}
}


///[summary]現在選択されている種別に絞ったスキルリストで更新
function RefreshSkillList() {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
	SkillList.resize(0);

	//習得スキルから調べる
	for(local i = 0; i < actor.LearnedSkills.Count(); i++) {
		//現在の種別に一致しないものは処理しない
		if(db.GetCell(actor.LearnedSkills.GetID(i), "種別").Integer != SkillTypeList[Cursor[1]].ID) {
			continue;
		}
		local data = CIDValueData();
		data.ID = db.GetCell(actor.LearnedSkills.GetID(i), db.GetColumnName(DBDefaultColumnIndices.FixedID)).Integer;
		data.Name = db.GetCell(actor.LearnedSkills.GetID(i), db.GetColumnName(DBDefaultColumnIndices.Name)).String;
		data.Value = db.GetCell(actor.LearnedSkills.GetID(i), "コスト").Integers.Get(0);
		SkillList.append(data);
	}
}


///[summary]HPダメージを与える
///[par]アクターのインデックス
///[par]ダメージ量
///[par]メッセージを表示するかどうか
///[return]ダメージ量
function HPDamage(idx, value, enabledMsg) {
	//ダメージ量を整数に直す
	value = value.tointeger();

	//数値表示
	if(value > 0) {
		Game.DB.FDBMaterial.Stop(0x4C6DF6BD/*ダメージ音*/);
		Game.DB.FDBMaterial.Play(0x4C6DF6BD/*ダメージ音*/, 255, 0, 0);
		DamageEffectTasks.append(SQDamageEffectData(idx, value, SQDamageEffectData.DamageEffectType.HPDamage));

		//ダメージモーション
		if(GetAttackSide(idx) == AttackSide.Party) {
			PTPanelShakes[idx].StartShake(7);
		} else {
			DamageMotionUT[idx - EnemyIndexOrigin].StartShake(-1);
		}
	} else if(value < 0) {
		//回復の場合
		HPHeal(idx, -value, enabledMsg);
		return;
	} else {
		//ミス
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
	}

	//クリティカル表示: ミスだった場合は表示しない
	if(CriticalHitFlag == true && value > 0) {
		CriticalHitFlag = false;
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Critical));
	}

	//パラメーター処理
	PlusActorExParameter(idx, ExParameter.HP, -value);

	//メッセージ表示
	local actor = GetActorData(idx);
	if(enabledMsg == true) {
		if(GetAttackSide(idx) == AttackSide.Party) {
			if(value == 0) {
				ActionMessage("ミス！ " + actor.Name + " は ダメージを受けない！", 0.5);
			} else {
				ActionMessage(actor.Name + " は " + value + " のダメージを受けた！", 0.5);
			}
			if(IsDead(idx) == true) {
				ActionMessage(actor.Name + " は 死んでしまった！", 0);
			}
		} else {
			if(value == 0) {
				ActionMessage("ミス！ " + actor.Name + " に ダメージを与えられない！", 0.5);
			} else {
				ActionMessage(actor.Name + " に " + value + " のダメージを与えた！", 0.5);
			}
			if(IsDead(idx) == true) {
				ActionMessage(actor.Name + " を 倒した！", 0);
			}
		}
	}

	//対象が死んだ場合はすべての間接効果を解除する
	if(IsDead(idx) == true) {
		actor.ClearState();
	} else {
		//ダメージを受けて解除されるかもしれない間接効果の判定
		StateJudgeRelease(actor, StateSolve.Damamge);
	}
	return value;
}


///[summary]指定サイド全体にHPダメージを与える
///[par]行動内容 (SQActionData)
///[par]対象サイド (AttackSide)
///[par]ダメージ計算の方法 (UseType)
///[par]攻撃属性IDリスト (CIntegerArray)
///[par]基本値
///[par]分散度
///[par]成功率
function HPDamageSideAll(actData, targetSide, calcType, attrList, base, variance, successRate) {
	local damage = 0;
	local headName = GetHeadActorName(targetSide);
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		local target = GetActorData(i);

		//対象者: 攻撃を受ける前の間接効果処理
		local action = clone actData;
		local target = GetActorData(i);
		action.TargetIndex = i;
		StateBeforeDamage(target, action);
		if(action.Disabled == true) {
			continue;		//攻撃が無効化されたらスキップ
		}
		target = GetActorData(action.TargetIndex);

		local value = 0;
		switch(calcType) {
			case UseType.Physic:
				value = CalcPhysicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
			case UseType.Magic:
				value = CalcMagicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
		}
		damage += HPDamage(action.TargetIndex, value, false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//一括メッセージ表示
	if(targetSide == AttackSide.Party) {
		ActionMessage(headName + " は " + (actors.len() <= 1 ? "" : "平均 ") + damage + " のダメージを受けた！", 0.5);
		foreach(idx in actors) {
			if(IsDead(idx) == true) {
				ActionMessage(GetActorData(idx).Name + " は 死んでしまった！", 0);
			}
		}
	} else {
		ActionMessage(headName + " に " + (actors.len() <= 1 ? "" : "平均 ") + damage + " のダメージ！", 0.5);
		foreach(idx in actors) {
			if(IsDead(idx) == true) {
				ActionMessage(GetActorData(idx).Name + " を 倒した！", 0);
			}
		}
	}
}


///[comment]MPダメージを与える
///[par]アクターのインデックス
///[par]ダメージ量
///[par]メッセージを表示するかどうか
///[return]ダメージ量
function MPDamage(idx, value, enabledMsg) {
	//ダメージ量を整数に直す
	value = value.tointeger();

	//数値表示
	if(value > 0) {
		DamageEffectTasks.append(SQDamageEffectData(idx, value, SQDamageEffectData.DamageEffectType.MPDamage));
	} else if(value < 0) {
		//回復の場合
		MPHeal(idx, -value, enabledMsg);
		return;
	} else {
		//ミス
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
	}

	//クリティカル表示: ミスだった場合でも表示する
	if(CriticalHitFlag == true) {
		CriticalHitFlag = false;
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Critical));
	}

	//パラメーター処理
	local actor = GetActorData(idx);
	local delta = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value - value;
	if(delta < 0) {
		//差し引いた結果がマイナスになる場合はゼロになるように調整する
		value = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
	}
	PlusActorExParameter(idx, ExParameter.MP, -value);

	//メッセージ表示
	if(enabledMsg == true) {
		if(GetAttackSide(idx) == AttackSide.Party) {
			if(value == 0) {
				ActionMessage("ミス！ " + actor.Name + " の " + MPName + "は 抜かれなかった！", 0.5);
			} else {
				ActionMessage(actor.Name + " は " + value + " ポイントの " + MPName + "を 抜かれた！", 0.5);
			}
		} else {
			if(value == 0) {
				ActionMessage("ミス！ " + actor.Name + " の " + MPName + "は 抜かれなかった！", 0.5);
			} else {
				ActionMessage(actor.Name + " の " + MPName + "を " + value + " ポイント 抜いた！", 0.5);
			}
		}
	}
	return value;
}


///[summary]指定サイド全体にMPダメージを与える
///[par]行動内容 (SQActionData)
///[par]対象サイド (AttackSide)
///[par]ダメージ計算の方法 (UseType)
///[par]攻撃属性IDリスト (CIntegerArray)
///[par]基本値
///[par]分散度
///[par]成功率
function MPDamageSideAll(actData, targetSide, calcType, attrList, base, variance, successRate) {
	local damage = 0;
	local headName = GetHeadActorName(targetSide);
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		//対象者: 攻撃を受ける前の間接効果処理
		local action = clone actData;
		local target = GetActorData(i);
		action.TargetIndex = i;
		StateBeforeDamage(target, action);
		if(action.Disabled == true) {
			continue;		//攻撃が無効化されたらスキップ
		}
		target = GetActorData(action.TargetIndex);

		local value = 0;
		switch(calcType) {
			case UseType.Physic:
				value = CalcPhysicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
			case UseType.Magic:
				value = CalcMagicalAttack(actor, target, attrList, base, variance, successRate, false);
				break;
		}
		damage += MPDamage(action.TargetIndex, value, false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//一括メッセージ表示
	if(targetSide == AttackSide.Party) {
		ActionMessage(headName + " の " + MPName + "が " + (actors.len() <= 1 ? "" : "平均 ") + damage + " ポイント 抜かれた！", 0.5);
	} else {
		ActionMessage(headName + " の " + MPName + "を " + (actors.len() <= 1 ? "" : "平均 ") + damage + " ポイント 抜いた！", 0.5);
	}
}


///[comment]間接効果を付与する
///[par]アクターのインデックス
///[par]間接効果ID
///[par]成功率
function SetState(idx, stateID, successRate) {
	local actor = GetActorData(idx);
	local stateDB = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);
	local isFailed = false;

	if(actor.States.Exists(stateID) && stateDB.GetCell(stateID, "上位互換").Integer == -1) {
		//上位互換が無い同一の間接効果は重ね掛けしないようにする
		print("間接効果の重ね掛けを回避(上位互換なく既に罹患しているため): actorID=" + actor.ID + ", stateID=" + stateID);
		isFailed = true;
	}
	if(CheckUpperState(idx, stateID) == true) {
		//上位互換の間接効果に既に罹患している場合は重ね掛けしないようにする
		print("間接効果の重ね掛けを回避(上位互換に罹患しているため): actorID=" + actor.ID + ", stateID=" + stateID);
		isFailed = true;
	}
	if(JudgeHit(actor, successRate) == false) {
		//命中せず
		print("間接効果命中失敗: actorID=" + actor.ID + ", stateID=" + stateID);
		isFailed = true;
	}
	if(isFailed == false && actor.SetState(stateID, false) == false) {
		//罹患失敗
		isFailed = true;
	}

	if(isFailed == true) {
		DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
		ActionMessage("しかし " + actor.Name + " には 効かなかった！", 0.5);
	}
}


///[comment]上位互換の間接効果が付与されているかどうか検証
///[par]アクターのインデックス
///[par]間接効果ID
///[return]上位互換の間接効果が付与されているかどうか
function CheckUpperState(idx, stateID) {
	local actor = GetActorData(idx);
	local stateDB = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0);

	if(stateDB.GetCell(stateID, "上位互換").Integer == -1) {
		// この間接効果に上位互換は存在しない
		return false;
	}

	local upperStateID = stateDB.GetCell(stateID, "上位互換").Integer;
	local hasUpperState = actor.States.Exists(upperStateID);
	return hasUpperState;
}


///[summary]スキル/アイテムの典型的な直接効果
///[par]使用するスキル/アイテムのID
///[par]行動者のインデックス
///[par]対象者のインデックス
///[par]対象種別
///[par]直接効果の種別
///[par]間接効果ID
///[par]効果量
///[par]分散度
///[par]成功率
///[par]属性リスト (CIntegerArray)
///[return]実際に効果があったかどうか
function TypicalUse(intID, intActorIdx, intTargetIdx, targetType, useType, stateID, value, variance, successRate, attrList) {
	local used = true;
	local side = -1;

	//全体対象の場合はどちら側のサイドを対象とするのかを確定する
	switch(targetType) {
		case TargetType.AllParty:
		case TargetType.AllPartyWithDead:
			side = GetAttackSide(intActorIdx);
			break;
		case TargetType.AllEnemy:
			side = GetAgainstAttackSide(GetAttackSide(intActorIdx));
			break;
	}

	switch(useType) {
		case UseType.Physic:
			if(intTargetIdx < 0) {
				//全体攻撃
				HPDamageSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					UseType.Physic,
					attrList,
					value, variance, successRate
				);
			} else {
				//単体攻撃
				HPDamage(
					intTargetIdx,
					CalcPhysicalAttack(GetActorData(intActorIdx), GetActorData(intTargetIdx), attrList, value, variance, successRate, false),
					true
				);
			}
			break;

		case UseType.Magic:
			if(intTargetIdx < 0) {
				//全体攻撃
				HPDamageSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					UseType.Magic,
					attrList,
					value, variance, successRate
				);
			} else {
				//単体攻撃
				HPDamage(
					intTargetIdx,
					CalcMagicalAttack(GetActorData(intActorIdx), GetActorData(intTargetIdx), attrList, value, variance, successRate, false),
					true
				);
			}
			break;

		case UseType.Heal:
			local delta = 0;

			if(intTargetIdx < 0) {
				//全体
				delta = HPHealSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					value, variance
				);
			} else {
				//単体
				delta = HPHeal(
					intTargetIdx,
					CalcHeal(GetActorData(intActorIdx), GetActorData(intTargetIdx), value, variance),
					true
				);
			}

			if(delta == 0) {
				used = false;
			}
			break;

		case UseType.Utility:
			if(intTargetIdx < 0) {
				//全体
				SetStateSideAll(
					SQActionData(intActorIdx, intTargetIdx, MenuItem.Item, intID, -1),
					side,
					stateID, successRate
				);
			} else {
				//単体
				SetState(intTargetIdx, stateID, successRate);
			}
			break;
	}
	return used;
}


///[summary]指定サイド全体に間接効果を付与する
///[par]行動内容 (SQActionData)
///[par]対象サイド (AttackSide)
///[par]間接効果ID
///[par]成功率
function SetStateSideAll(actData, targetSide, stateID, successRate) {
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		//対象者: 攻撃を受ける前の間接効果処理
		local action = clone actData;
		local target = GetActorData(i);
		action.TargetIndex = i;
		StateBeforeDamage(target, action);
		if(action.Disabled == true) {
			continue;		//攻撃が無効化されたらスキップ
		}
		target = GetActorData(action.TargetIndex);

		//間接効果を付与する
		SetState(action.TargetIndex, stateID, successRate);
	}
}


///[summary]サイド取得
///[par]アクターのインデックス
///[return]サイド (AttackSide)
function GetAttackSide(idx) {
	if(idx < 0) {
		return null;
	} else if(idx < EnemyIndexOrigin) {
		return AttackSide.Party;
	} else {
		return AttackSide.Unit;
	}
}


///[summary]指定サイドの反対を返す
///[par]サイド (AttackSide)
///[return]与えられた反対のサイド (AttackSide)
function GetAgainstAttackSide(side) {
	if(side == AttackSide.Party) {
		return AttackSide.Unit;
	} else {
		return AttackSide.Party;
	}
	return null;
}


///[summary]指定サイドの最初のインデックスを返す
///[par]サイド (AttackSide)
///[return]アクターのインデックス。存在しない場合は -1 が返されます。
function GetFirstActorIndex(side) {
	local result = 0;
	if(side == AttackSide.Party) {
		result = 0;
		while(result < Game.DB.VDBParty.GetCharCount() && IsDead(result) == true) {
			result++;
		}
		if(Game.DB.VDBParty.GetCharCount() <= result) {
			return -1;		//全員死んでいる
		}
	} else {
		result = EnemyIndexOrigin;
		while(result - EnemyIndexOrigin < Enemies.len() && IsDead(result) == true) {
			result++;
		}
		if(Enemies.len() <= result - EnemyIndexOrigin) {
			return -1;		//全員死んでいる
		}
	}
	return result;
}


///[summary]指定サイドの次のインデックスを返す
///[par]サイド (AttackSide)
///[return]アクターのインデックス。存在しない場合は -1 が返されます。
function GetNextActorIndex(idx) {
	local result = idx + 1;
	if(GetAttackSide(idx) == AttackSide.Party) {
		while(result < Game.DB.VDBParty.GetCharCount() && IsDead(result) == true) {
			result++;
		}
		if(Game.DB.VDBParty.GetCharCount() <= result) {
			return -1;		//次のアクターがいない
		}
	} else {
		while(result - EnemyIndexOrigin < Enemies.len() && IsDead(result) == true) {
			result++;
		}
		if(Enemies.len() <= result - EnemyIndexOrigin) {
			return -1;		//全員死んでいる
		}
	}
	return result;
}


///[summary]指定サイドの先頭者名を返す
///[par]サイド (AttackSide)
///[return]先頭者名。パーティが複数人いる場合は複数形となる
function GetHeadActorName(side) {
	local idx = GetFirstActorIndex(side);
	if(idx != -1) {
		return GetActorData(idx).Name + " " + GetPluralString();
	}
	return "";
}


///[summary]先頭者名に続く複数形を返す
///[return]パーティが複数人いる場合に限り、複数形の文字列。一人パーティの場合は空文字
function GetPluralString() {
	if(Game.DB.VDBParty.GetCharCount() > 1) {
		return PluralString;
	}
	return "";
}


///[summary]戦闘終了判定
///[return]戦闘結果 (BattleResult)、終了していない場合は -1 が返されます。
function JudgeBattleExit() {
	//勝利判定
	if(GetFirstActorIndex(AttackSide.Unit) == -1) {
		return BattleResult.Win;
	}

	//敗北判定
	if(GetFirstActorIndex(AttackSide.Party) == -1) {
		return BattleResult.Lose;
	}

	//逃走判定
	if(PTEscaped == true) {
		return BattleResult.Escape;
	}

	//その他: フェーズが終了フェーズになっている場合は中断されたとみなす
	if(Phase == BattlePhase.Ending) {
		return BattleResult.Break;
	}

	return -1;
}
