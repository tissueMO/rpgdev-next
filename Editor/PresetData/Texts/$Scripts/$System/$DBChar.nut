//###################################################################
//		パーティキャラ スクリプト
//###################################################################

///[summary]自律戦闘が有効なキャラの行動選択
///[par]パーティキャラID
///[par]行動内容(SQActionData)の配列
///[comment]行動内容リストには既定で一つだけ代入されています。配列actions の変更結果が行動内容として登録されます。複数行動にする場合はappendを、行動しない場合は要素数をゼロにします。
function SelectCommandChar(intID, actions) {
	local me = Game.DB.VDBActor.GetData(intID);
	local rand = Game.NextRand(1, 100);

	switch(intID) {
		case 0x1DA3A3F7/*女戦士*/:
			local leoActorIndex = Game.DB.VDBParty.CheckCharInParty(0x27264F10/*レオナルド*/);
			local containsLeoInParty = (leoActorIndex != -1);
			local leo = Game.DB.VDBActor.GetData(0x27264F10/*レオナルド*/);
			local skillDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
			local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
			local isBossMode = (Enemies.len() == 1 && GetActorData(EnemyIndexOrigin).GetParameter(0x2019EC10/*最大ＨＰ*/).Value >= 150);

			if(containsLeoInParty
			&& IsDead(leoActorIndex) == false
			&& leo.SelfValues.Get(0x7BAC0D68/*HP*/).Value <= leo.GetParameter(0x2019EC10/*最大ＨＰ*/).Value * 0.5
			&& skillDB.GetCell(0x4D07D04B/*ヒール*/, "コスト").Integers.Get(0) <= me.SelfValues.Get(0x46A20EE9/*MP*/).Value
			&& rand <= 100) {
				// レオナルド回復
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4D07D04B/*ヒール*/;
				actions[0].TargetIndex = leoActorIndex;
			} else if(me.SelfValues.Get(0x7BAC0D68/*HP*/).Value <= me.GetParameter(0x2019EC10/*最大ＨＰ*/).Value * 0.5
			&& skillDB.GetCell(0x4D07D04B/*ヒール*/, "コスト").Integers.Get(0) <= me.SelfValues.Get(0x46A20EE9/*MP*/).Value
			&& rand <= 75) {
				// 女戦士回復
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4D07D04B/*ヒール*/;
				actions[0].TargetIndex = Game.DB.VDBParty.CheckCharInParty(intID);
			} else if(containsLeoInParty
			&& IsDead(leoActorIndex) == false
			&& isBossMode
			&& TurnCount <= 5
			&& leo.States.Exists(0x69CE82E8/*ディフェンスアップ*/) == false
			&& rand <= 50) {
				// レオナルドガードプラス
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4160663C/*ガードプラス*/;
				actions[0].TargetIndex = leoActorIndex;
			} else if(containsLeoInParty
			&& IsDead(leoActorIndex) == false
			&& isBossMode
			&& TurnCount <= 5
			&& leo.States.Exists(0x4C3A1FDA/*パワーアップ*/) == false
			&& rand <= 50) {
				// レオナルドパワープラス
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x23C6D92A/*パワープラス*/;
				actions[0].TargetIndex = leoActorIndex;
			} else if(rand <= 25
			&& skillDB.GetCell(0x7E5A58BF/*ファイア*/, "コスト").Integers.Get(0) <= me.SelfValues.Get(0x46A20EE9/*MP*/).Value) {
				// 攻撃魔法
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*ファイア*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Unit);
			} else {
				// 通常攻撃
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Unit);
				if(me.GetEquipIDFromItemTypeID(0x12414D21/*武器*/) != -1) {
					actions[0].EffectID = itemDB.GetCell(me.GetEquipIDFromItemTypeID(0x12414D21/*武器*/), "エフェクト").Integer;
				} else {
					actions[0].EffectID = EmptyEquipEffectID;
				}
			}

			break;
	}
}
