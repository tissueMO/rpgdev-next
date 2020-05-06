//###################################################################
//		アイテム スクリプト
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Common.nut");

///[summary]アイテム使用時の効果内容
///[par]使用するアイテムのID
///[par]使用者インデックス
///[par]対象者インデックス
///[par]消費しないようにするかどうか
///[return]消費したかどうか
function UseItem(intID, intActorIdx, intTargetIdx, boolIsInfinity) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local stateID = db.GetCell(intID, "間接効果").Integer;
	local value = db.GetCell(intID, "効果量").Integers.Get(0);
	local variance = db.GetCell(intID, "分散度").Integer;
	local successRate = db.GetCell(intID, "個別変数").IDValues.GetValue(0x48CA9CB8/*成功率*/);
	local attrList = db.GetCell(intID, "属性").Integers;
	local targetType = db.GetCell(intID, "対象種別").Integer;
	local used = true;
	if(db.GetCell(intID, "消耗品").Boolean == false) {
		//消耗品でない場合は無限に使える
		boolIsInfinity = true;
	}

	//直接効果に基づいて処理する
	local useType = db.GetCell(intID, "直接効果").Integer;
	switch(useType) {
		case UseType.Script:
			return UseItemOthers(intID, intActorIdx, intTargetIdx, boolIsInfinity);
		default:
			used = TypicalUse(intID, intActorIdx, intTargetIdx, targetType, useType, stateID, value, variance, successRate, attrList);
			break;
	}
	return (boolIsInfinity ? false : used);
}


///[summary]個別的な効果内容
///[par]使用するアイテムのID
///[par]使用者インデックス
///[par]対象者インデックス
///[par]消費しないようにするかどうか
///[return]消費したかどうか
function UseItemOthers(intID, intActorIdx, intTargetIdx, boolIsInfinity) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local value = db.GetCell(intID, "効果量").Integers.Get(0);
	local used = true;

	//IDごとに処理を定義する
	switch(intID) {
		// NOTE: データベースで直接効果を「スクリプト指定」にした場合は処理内容をここに定義します。

		case 0x6F0C5D44/*飛翔の輝石*/:
			CloseMenuScene();
			local db = Game.DB.UDB.GetUserDB(0x2C80399D/*場所リスト*/);
			Game.SceneMap.ChangeMap(
				db.GetCellFromIndex(intTargetIdx, db.GetColumnIndex("マップファイル名")).String,
				db.GetCellFromIndex(intTargetIdx, db.GetColumnIndex("X")).Integer,
				db.GetCellFromIndex(intTargetIdx, db.GetColumnIndex("Y")).Integer,
				Direction4.South
			);
			break;

	}

	return (boolIsInfinity ? false : used);
}
