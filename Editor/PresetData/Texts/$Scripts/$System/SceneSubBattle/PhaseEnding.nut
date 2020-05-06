//###################################################################
//		バトルシーン: サブルーチン: 戦闘終了フェーズ
//###################################################################

//戦闘終了フェーズのサブ関数
PhaseEndingSubRoutine <- {

	///[summary]勝利時のサブルーチン
	[BattleResult.Win] = function() {
		//ユニットイベント: 勝利時
		UnitEvent(UnitID, BattleEventType.EndingBattleWin, null);

		local enemyDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Enemy).GetSubDB(0);
		local wipeGraphFileName = Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.BattleEndingWipeGraph, "ファイル名").String;

		//獲得経験値、金額の算出
		Results.EXP = 0;
		Results.Money = 0;
		foreach(enemy in Enemies) {
			if(enemy.SelfValues.Get(0x5486E688/*逃走したかどうか*/).Value == TRUE) {
				continue;		//逃走したエネミーは対象外となる
			}
			Results.EXP += enemyDB.GetCell(enemy.ID, "経験値").Integer;
			Results.Money += enemyDB.GetCell(enemy.ID, "所持金").Integer;
		}

		//お金加算
		Game.DB.VDBParty.Money += Results.Money;

		//画面に反映
		ActionMessage("戦闘勝利！", 0);
		Game.DB.FDBMaterial.Play(0x5F5E6646/*戦闘終了*/, 255, 0, 0);
		Game.PrepareTransition();
		Depth = EndingDepth.EXPMoney;
		Phase = BattlePhase.Ending;
		Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
		Game.WaitKey(true);

		//経験値加算、レベルアップ判定
		for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
			local actor = GetActorData(i);
			if(IsDead(i) == true) {
				continue;		//死んでいるキャラは対象外となる
			}

			local beforeLv = actor.Level;
			local lv = actor.PlusEXP(Results.EXP);		//この中でレベルアップ処理が行われる
			if(lv <= 0) {
				continue;		//レベルは上がらなかった
			}

			//以降、レベルアップ時の処理
			Results.LevelUpName = actor.Name;
			Results.BeforeLevel = beforeLv;
			Results.AfterLevel = actor.Level;

			//パラメーター上昇リスト
			Results.LevelUpParameters = array(actor.LevelUpParameters.Count());
			foreach(idx, par in Results.LevelUpParameters) {
				Results.LevelUpParameters[idx] = CIDValueData();
				Results.LevelUpParameters[idx].ID = actor.LevelUpParameters.GetID(idx);
				Results.LevelUpParameters[idx].Name = actor.LevelUpParameters.GetName(idx);
				Results.LevelUpParameters[idx].Value = actor.LevelUpParameters.GetValue(idx);
			}

			Game.DB.FDBMaterial.Stop(0x5F5E6646/*戦闘終了*/);
			Game.DB.FDBMaterial.Stop(0x7706A0ED/*レベルアップ*/);
			Game.DB.FDBMaterial.Play(0x7706A0ED/*レベルアップ*/, 255, 0, 0);
			Game.PrepareTransition();
			Depth = EndingDepth.LevelUp;
			Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
			Game.WaitKey(true);

			//習得スキルリスト
			Results.LevelUpSkills = array(actor.LevelUpSkills.Count());
			foreach(idx, par in Results.LevelUpSkills) {
				Results.LevelUpSkills[idx] = CIDNameData();
				Results.LevelUpSkills[idx].ID = actor.LevelUpSkills.GetID(idx);
				Results.LevelUpSkills[idx].Name = actor.LevelUpSkills.GetName(idx);
			}
			if(Results.LevelUpSkills.len() > 0) {
				//習得スキルがある場合のみ
				Game.PrepareTransition();
				Depth = EndingDepth.LearnSkill;
				Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
				Game.WaitKey(true);
			}
		}

		//拾得アイテム判定
		local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
		Results.DropItems = array(0);
		foreach(enemy in Enemies) {
			local items = enemyDB.GetCell(enemy.ID, "ドロップアイテム").IDValues;
			if(items.Count() == 0) {
				continue;		//ドロップアイテムがない場合はスキップ
			}

			//データの一番上から判定する
			local id = -1;
			for(local i = 0; i < items.Count(); i++) {
				if(Game.NextRand(1, 100) <= items.Get(i).Value) {
					//確定したらそれ以降の判定を行わない
					id = items.Get(i).ID;
					break;
				}
			}
			if(id == -1) {
				continue;
			}

			//拾得処理
			local item = CIDNameData()
			item.ID = id;
			item.Name = itemDB.GetCell(id, itemDB.GetColumnName(DBDefaultColumnIndices.Name)).String;
			Results.DropItems.append(item);
			Game.DB.VDBParty.AddItem(id, 1);
		}
		if(Results.DropItems.len() > 0) {
			//拾得した場合のみ
			Game.PrepareTransition();
			Depth = EndingDepth.DropItem;
			Game.DoTransition(wipeGraphFileName, SquareSpeed.High);
			Game.WaitKey(true);
		}

		Game.DB.FDBMaterial.Stop(0x5F5E6646/*戦闘終了*/);
		Game.DB.FDBMaterial.Stop(0x7706A0ED/*レベルアップ*/);
	},


	///[summary]敗北時のサブルーチン
	[BattleResult.Lose] = function() {
		//ユニットイベント: 敗北時
		UnitEvent(UnitID, BattleEventType.EndingBattleLose, null);

		ActionMessage(GetActorData(0).Name + "は 全滅してしまった...", 1.0);
		Game.Wait(5.0, false, true);
		Phase = BattlePhase.Ending;
		Game.FadeOut(SquareSpeed.High);
		if(DisabledGameOver == false) {
			GoGameOver();
		}
	},


	///[summary]逃走時のサブルーチン
	[BattleResult.Escape] = function() {
		//ユニットイベント: 逃走時
		Phase = BattlePhase.Ending;
		UnitEvent(UnitID, BattleEventType.EndingBattleEscape, null);
	},


	///[summary]中断時のサブルーチン
	[BattleResult.Break] = function() {
		//特に処理を行わない
		Phase = BattlePhase.Ending;
	},
};
