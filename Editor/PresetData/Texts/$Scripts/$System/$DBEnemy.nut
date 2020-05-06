//###################################################################
//		エネミー スクリプト
//###################################################################

///[summary]エネミーの行動選択
///[par]エネミーID
///[par]行動内容(SQActionData)の配列
///[comment]行動内容リストには既定で一つだけ代入されています。配列actions の変更結果が行動内容として登録されます。複数行動にする場合はappendを、行動しない場合は要素数をゼロにします。
function SelectCommandEnemy(intID, actions) {
	local rand = Game.NextRand(1, 100);

	switch(intID) {
		case 0x1B5682FD/*チュートリアル専用スライム*/:
			actions[0].Command = MenuItem.Attack;
			actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			actions[0].EffectID = EmptyEquipEffectID;
			break;

		case 0x409C361F/*スライム*/:
			if(rand <= 15) {
				actions[0].Command = MenuItem.Escape;
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x1DC9F561/*コウモリ*/:
			actions[0].Command = MenuItem.Attack;
			actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			actions[0].EffectID = EmptyEquipEffectID;
			break;

		case 0x37B4A5F7/*おばけキノコ*/:
			if(rand <= 20) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x22A28BE5/*スリープ*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x50918D5E/*桜トレント*/:
			if(rand <= 20) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x365496DB/*パニック*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x57B1F7CB/*棍棒野郎*/:
			if(rand <= 50 && TurnCount <= 3
			&& GetActorData(actions[0].ActorIndex).States.Exists(0x4C3A1FDA/*パワーアップ*/) == false) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x23C6D92A/*パワープラス*/;
				actions[0].TargetIndex = actions[0].ActorIndex/*自分*/;
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x286772AD/*ひとつ目小僧*/:
			if(TurnCount == 1) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4EB43EA/*言葉奪い*/;
				//女戦士がパーティにいる場合は女戦士優先
				local womanSoldierIndex = Game.DB.VDBParty.CheckCharInParty(0x1DA3A3F7/*女戦士*/);
				actions[0].TargetIndex = (womanSoldierIndex != -1) ? womanSoldierIndex : 0;
			} else if(rand <= 35) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*ファイア*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x2089C2F1/*グリフォン*/:
			if(rand <= 35) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x754A7AE6/*火炎ブレス*/;
				actions[0].TargetIndex = -1;
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x10847557/*ネクロマンサー*/:
			if(rand <= 50 && TurnCount <= 3
			&& GetActorData(actions[0].ActorIndex).States.Exists(0x69CE82E8/*ディフェンスアップ*/) == false) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4160663C/*ガードプラス*/;
				actions[0].TargetIndex = actions[0].ActorIndex/*自分*/;
			} else if(rand <= 40) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*ファイア*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else if(rand <= 55) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x56D945DC/*ポイズン*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else if(rand <= 70) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x2335FAE0/*スモーク*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x569A84D6/*人食い箱*/:
			if(rand <= 50 && TurnCount <= 3
			&& GetActorData(actions[0].ActorIndex).States.Exists(0x5D4CF03C/*スピードアップ*/) == false) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x510301FE/*スピードプラス*/;
				actions[0].TargetIndex = actions[0].ActorIndex/*自分*/;
			} else {
				//完全2回行動
				//行動1
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;

				//行動2
				actions.append(SQActionData(actions[0].ActorIndex))
				actions[1].Command = MenuItem.Attack;
				actions[1].TargetIndex = SelectRandom(AttackSide.Party);
				actions[1].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x4F74D7F0/*ダークレオナルド*/:
			if(rand <= 20) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x754A7AE6/*火炎ブレス*/;
				actions[0].TargetIndex = -1;
			} else if(rand <= 40) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*ファイア*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else if(rand <= 55) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x141F1FBB/*破滅の燐光*/;
				actions[0].TargetIndex = -1;
			} else {
				//行動1
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;

				//行動2
				actions.append(SQActionData(actions[0].ActorIndex))
				actions[1].Command = MenuItem.Attack;
				actions[1].TargetIndex = SelectRandom(AttackSide.Party);
				actions[1].EffectID = EmptyEquipEffectID;
			}
			break;
	}
}
