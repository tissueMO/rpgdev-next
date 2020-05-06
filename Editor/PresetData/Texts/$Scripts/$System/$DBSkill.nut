//###################################################################
//		スキル スクリプト
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Common.nut");

///[summary]スキル使用時の効果内容
///[par]使用するスキルのID
///[par]使用者インデックス
///[par]対象者インデックス
///[return]消費したかどうか
function UseSkill(intID, intActorIdx, intTargetIdx) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	local stateID = db.GetCell(intID, "間接効果").Integer;
	local value = db.GetCell(intID, "効果量").Integers.Get(0);
	local variance = db.GetCell(intID, "分散度").Integer;
	local successRate = db.GetCell(intID, "個別変数").IDValues.GetValue(0x48CA9CB8/*成功率*/);
	local attrList = db.GetCell(intID, "属性").Integers;
	local targetType = db.GetCell(intID, "対象種別").Integer;
	local used = true;

	//直接効果に基づいて処理する
	local useType = db.GetCell(intID, "直接効果").Integer;
	switch(useType) {
		case UseType.Script:
			return UseSkillOthers(intID, intActorIdx, intTargetIdx);
		default:
			used = TypicalUse(intID, intActorIdx, intTargetIdx, targetType, useType, stateID, value, variance, successRate, attrList);
			break;
	}
	return used;
}


///[summary]個別的な効果内容
///[par]使用するスキルのID
///[par]使用者インデックス
///[par]対象者インデックス
///[return]消費したかどうか
function UseSkillOthers(intID, intActorIdx, intTargetIdx) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	local value = db.GetCell(intID, "効果量").Integers.Get(0);
	local used = true;

	//IDごとに処理を定義する
	switch(intID) {
		// NOTE: データベースで直接効果を「スクリプト指定」にした場合は処理内容をここに定義します。

	}

	return used;
}
