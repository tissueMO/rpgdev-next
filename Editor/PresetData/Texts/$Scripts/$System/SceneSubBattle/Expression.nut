//###################################################################
//		バトルシーン: 戦闘計算式
//###################################################################

///[summary]アクターの成功率を取得
///[par]アクターのインデックス
///[return]成功率
function GetActorSuccessRate(idx) {
	//アクターの個別変数を取り出す
	return GetActorData(idx).SelfValues.Get(0x3D4228D9/*成功率*/).Value;
}


///[summary]物理攻撃の計算
///[par]行動者 (CDBActorData)
///[par]対象者 (CDBActorData)
///[par]攻撃属性リスト (CIntegerArray)
///[par]基本値
///[par]分散度
///[par]成功率
///[par]クリティカルヒットを判定するかどうか
///[return]ダメージ量
function CalcPhysicalAttack(actor, target, attrList, base, variance, successRate, enabledCritical) {
	local value = base;
	value += actor.GetParameter(0x48EB70FA/*物理攻撃力*/).Value / 2;

	//クリティカル判定
	CriticalHitFlag = enabledCritical ? JudgeCritical() : false;
	if(CriticalHitFlag == true) {
		//ダメージ量を 2倍 にして、相手の守備力・防御を無視する
		value *= CriticalDamageRate;
	} else {
		value -= target.GetParameter(0x469C1AF5/*物理守備力*/).Value / 4;

		//属性耐性を考慮
		local attrDefense = 100;
		for(local i = 0; i < attrList.Count(); i++) {
			attrDefense = attrDefense * target.GetAttributeResistance(attrList.Get(i)) / 100;
		}
		value = (value / attrDefense.tofloat() * 100.0).tointeger();

		//防御判定
		if(IsDefensing(target) == true) {
			value /= 2;
		}
	}

	//分散
	value += Game.NextRand(0, value * variance / 100);

	// 0 以下の場合は 0～1 に修正する
	if(value <= 0) {
		value = Game.NextRand(0, 1);
	}

	//命中判定
	if(JudgeHit(actor, successRate) == false) {
		//命中せず
		value = 0;
	}
	return value;
}


///[summary]魔法攻撃の計算
///[par]行動者 (CDBActorData)
///[par]対象者 (CDBActorData)
///[par]攻撃属性リスト (CIntegerArray)
///[par]基本値
///[par]分散度
///[par]成功率
///[par]クリティカルヒットを判定するかどうか
///[return]ダメージ量
function CalcMagicalAttack(actor, target, attrList, base, variance, successRate, enabledCritical) {
	local value = base;
	value += actor.GetParameter(0x236EB043/*魔法攻撃力*/).Value / 2;

	//クリティカル判定
	CriticalHitFlag = enabledCritical ? JudgeCritical() : false;
	if(CriticalHitFlag == true) {
		//ダメージ量を 2倍 にして、相手の守備力・防御を無視する
		value *= CriticalDamageRate;
	} else {
		value -= target.GetParameter(0x99D0AC5/*魔法守備力*/).Value / 4;

		//属性耐性を考慮
		local attrDefense = 100;
		for(local i = 0; i < attrList.Count(); i++) {
			attrDefense = attrDefense * target.GetAttributeResistance(attrList.Get(i)) / 100;
		}
		value = (value / attrDefense.tofloat() * 100.0).tointeger();

		//防御判定
		if(IsDefensing(target) == true) {
			value /= 2;
		}
	}

	//分散
	value += Game.NextRand(0, value * variance / 100);

	// 0 以下の場合は 0～1 に修正する
	if(value <= 0) {
		value = Game.NextRand(0, 1);
	}

	//命中判定
	if(JudgeHit(actor, successRate) == false) {
		//命中せず
		value = 0;
	}
	return value;
}


///[summary]回復の計算
///[par]行動者 (CDBActorData)
///[par]対象者 (CDBActorData)
///[par]基本値
///[par]分散度
///[return]回復量
function CalcHeal(actor, target, base, variance) {
	local value = base;
	value += Game.NextRand(0, value * variance / 100);
	return value;
}


///[summary]防御判定
///[par]対象アクター (CDBActorData)
///[return]防御しているかどうか
function IsDefensing(actor) {
	return (actor.SelfValues.Get(0x7BD2A434/*防御中かどうか*/).Value == TRUE);
}


///[summary]命中判定を行う
///[comment]指定したアクターの行動が命中したかどうかを判定します。
///[par]アクターインスタンス
///[par]命中率
///[return]命中したかどうか
function JudgeHit(actor, successRate) {
	//成功判定: アクター自身の成功率と、この攻撃の成功率を合わせる
	if(Game.NextRand(1, 100) <= (100 - actor.SelfValues.Get(0x3D4228D9/*成功率*/).Value * successRate / 100)) {
		//命中せず
		return false;
	}
	return true;
}


///[summary]クリティカル判定
///[comment]自動的にグローバル変数のフラグにも反映されます。
///[return]クリティカルヒットが発生したかどうか
function JudgeCritical() {
	CriticalHitFlag = (Game.NextRand(1, 100) <= CriticalRate);
	return CriticalHitFlag;
}


///[summary]逃走判定
///[par]逃げる人数
///[return]逃走に成功したかどうか
function JudgeEscape(count) {
	local maxCount = 0;		//生きているメンバーの総数
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == false) {
			maxCount++;
		}
	}
	local oneRate = 100.0 / maxCount;
	return (Game.NextRand(1, 100) <= oneRate * count - EscapeMissRate / (maxCount - count + 1));
}

