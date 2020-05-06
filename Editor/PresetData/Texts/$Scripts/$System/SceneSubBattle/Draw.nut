//###################################################################
//		バトルシーン: 描画処理
//###################################################################

///[summary]戦闘シーンの描画
function BattleDraw() {
	//戦闘背景
	Game.DXLib.DrawGraph(HdlBack, 0, 0, false, false);

	//エネミー
	foreach(idx, enemy in Enemies) {
		if(DamageMotionUT[idx].Doing == true && DamageMotionUT[idx].FrameCounter / 5 % 2 == 0) {
			continue;		//ダメージモーションの点滅状態
		} else if(DamageMotionUT[idx].Doing == false
		&& (IsDead(idx + EnemyIndexOrigin) == true || enemy.SelfValues.Get(0x5486E688/*逃走したかどうか*/).Value == TRUE)) {
			continue;		//フィールドにいない
		}

		Game.DXLib.DrawGraph(enemy.GraphHandle, enemy.Location.X, enemy.Location.Y, true, false);
		if(Phase == BattlePhase.Action && ActingIndex == idx + EnemyIndexOrigin) {
			//行動中のエネミーをハイライト
			Game.DXLib.SetDrawBlendMode(BlendMode.Add, 200);
			Game.DXLib.DrawGraph(enemy.GraphHandle, enemy.Location.X, enemy.Location.Y, true, false);
		}
		Game.DXLib.SetDrawBlendMode(-1, 0);

		//間接効果アイコン
		for(local i = 0; i < enemy.States.Count(); i++) {
			local pos = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0).GetCell(enemy.States.GetID(i), "アイコン").ConvertToPoint();
			Game.DXLib.DrawRectExtendGraph(
				Game.Material.Graphics.IconGraphicHandle,
				enemy.Location.X + i * (StateIconSize + 2),
				enemy.Location.Y,
				StateIconSize, StateIconSize,
				pos.X * Game.Material.Graphics.IconSize,
				pos.Y * Game.Material.Graphics.IconSize,
				Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
				true
			);
		}

		//デバッグ時のみ: HPメーター
		if(DEBUG) {
			DrawMeterSolid(
				enemy.Location.X + enemy.GraphSize.Width / 2 - 80 / 2,
				enemy.Location.Y + enemy.GraphSize.Height - 10,
				80, 8,
				HPMeterPatternStart,
				(enemy.GetParameter(0x2019EC10/*最大ＨＰ*/).Value == 0) ? 0 : (80 * enemy.SelfValues.Get(0x7BAC0D68/*HP*/).Value / enemy.GetParameter(0x2019EC10/*最大ＨＰ*/).Value)
			);
		}
	}

	//エネミー選択
	if(TargetUTSelecting == true) {
		DrawWindow(
			Enemies[Cursor[TargetUTDepth]].Location.X + Enemies[Cursor[TargetUTDepth]].GraphSize.Width / 2 - 200 / 2,
			Enemies[Cursor[TargetUTDepth]].Location.Y - 34,
			200, 34
		);
		Game.DXLib.DrawStringC(
			Enemies[Cursor[TargetUTDepth]].Location.X + Enemies[Cursor[TargetUTDepth]].GraphSize.Width / 2,
			Enemies[Cursor[TargetUTDepth]].Location.Y - 34 + 9,
			0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
			Enemies[Cursor[TargetUTDepth]].Name
		);
		
		//カーソルは点滅させる
		if(Game.DXLib.GetNowCount() / 250 % 2 == 0) {
			DrawCursorArrow(
				Enemies[Cursor[TargetUTDepth]].Location.X + Enemies[Cursor[TargetUTDepth]].GraphSize.Width / 2 - 16,
				Enemies[Cursor[TargetUTDepth]].Location.Y - 3,
				Direction4.South, false
			);
		}
	}

	//パーティパネル
	local width = Game.WindowSize.Width / Game.DB.FDBSystem.GetInteger(DB_System_MaxPartyCount);
	local meterWidth = width - OffsetCorner * 2;
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local x = width * i;
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));

		//ウィンドウ
		DrawWindow(PTPanelShakes[i].PositionX + x, PTPanelShakes[i].PositionY + actor.Location.Y, width, PartyPanelHeight);

		//パネルのアクティブ表示
		switch(Phase) {
			case BattlePhase.Command:
				if(TargetPTSelecting == false) {
					//コマンド選択しているキャラのパネルをハイライト
					if(i == TopIndex[0]) {
						Game.DXLib.SetDrawBlendMode(BlendMode.Add, 255);		//アクティブ表示
					} else {
						Game.DXLib.SetDrawBlendMode(BlendMode.Subtract, 50);	//非アクティブ表示
					}
				} else {
					//対象選択しているキャラのパネルを点滅させる
					if(i == Cursor[TargetPTDepth]) {
						Game.DXLib.SetDrawBlendMode(BlendMode.Add, 255 * fabs(sin(Game.DXLib.GetNowCount() / 300.0)));
					} else {
						Game.DXLib.SetDrawBlendMode(BlendMode.Subtract, 50);	//非アクティブ表示
					}
				}
				DrawWindow(PTPanelShakes[i].PositionX + x, PTPanelShakes[i].PositionY + actor.Location.Y, width, PartyPanelHeight);
				break;

			case BattlePhase.Action:
				//行動中のキャラのパネルをハイライト
				if(i == ActingIndex) {
					Game.DXLib.SetDrawBlendMode(BlendMode.Add, 255);			//アクティブ表示
				}
				DrawWindow(PTPanelShakes[i].PositionX + x, PTPanelShakes[i].PositionY + actor.Location.Y, width, PartyPanelHeight);
				break;
		}
		Game.DXLib.SetDrawBlendMode(-1, 0);

		//名前
		Game.DXLib.DrawString(
			PTPanelShakes[i].PositionX + x + OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + OffsetCorner,
			0x2E19E6A3/*セーブデータリスト用*/, WHITE,
			actor.Name
		);

		//レベル表示
		//Game.DXLib.DrawStringR(
		//	PTPanelShakes[i].PositionX + x + width - OffsetCorner,
		//	PTPanelShakes[i].PositionY + actor.Location.Y + OffsetCorner + 20,
		//	0x2E19E6A3/*セーブデータリスト用*/, WHITE,
		//	format("Lv: %2d", actor.Level)
		//);

		//HP表示
		Game.DXLib.DrawString(
			PTPanelShakes[i].PositionX + x + OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + 36,
			0x2E19E6A3/*セーブデータリスト用*/, WHITE,
			HPName + ":"
		);
		Game.DXLib.DrawStringR(
			PTPanelShakes[i].PositionX + x + width - OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + 36,
			0x2E19E6A3/*セーブデータリスト用*/, WHITE,
			format("%4d /%4d", actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value, actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value)
		);
		DrawMeterPattern(
			PTPanelShakes[i].PositionX + x + OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + 50, meterWidth,
			MeterHeight,
			HPMeterPatternStart,
			HPMeterPatternCount,
			(actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value == 0) ? 0 : (meterWidth * actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value / actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value)
		);

		//MP表示
		Game.DXLib.DrawString(
			PTPanelShakes[i].PositionX + x + OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + 61,
			0x2E19E6A3/*セーブデータリスト用*/, WHITE,
			MPName + ":"
		);
		Game.DXLib.DrawStringR(
			PTPanelShakes[i].PositionX + x + width - OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + 61,
			0x2E19E6A3/*セーブデータリスト用*/, WHITE,
			format("%4d /%4d", actor.SelfValues.Get(0x46A20EE9/*MP*/).Value, actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value)
		);
		DrawMeterSolid(
			PTPanelShakes[i].PositionX + x + OffsetCorner,
			PTPanelShakes[i].PositionY + actor.Location.Y + 75,
			meterWidth,
			MeterHeight,
			MPMeterPattern,
			(actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value == 0) ? 0 : (meterWidth * actor.SelfValues.Get(0x46A20EE9/*MP*/).Value / actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value)
		);

		//間接効果アイコン
		for(local n = 0; n < actor.States.Count() && n < StateIconMaxCount; n++) {
			local pos = Game.DB.GetDB(DBType.FDB, FDBIndices.State).GetSubDB(0).GetCell(actor.States.GetID(n), "アイコン").ConvertToPoint();
			Game.DXLib.DrawRectExtendGraph(
				Game.Material.Graphics.IconGraphicHandle,
				PTPanelShakes[i].PositionX + x + OffsetCorner + width - 90 + (StateIconSize + 1) * n,
				PTPanelShakes[i].PositionY + actor.Location.Y + 14,
				StateIconSize, StateIconSize,
				pos.X * Game.Material.Graphics.IconSize,
				pos.Y * Game.Material.Graphics.IconSize,
				Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
				true
			);
		}

		//コマンドパネル
		if(Phase == BattlePhase.Command && i == TopIndex[0]) {
			DrawWindow(x + (width - CommandPanelWidth) / 2, actor.Location.Y - MenuItem.len() * LineHeight, CommandPanelWidth, MenuItem.len() * LineHeight);
			foreach(idx in MenuItem) {
				local text = "";
				switch(idx) {
					case MenuItem.Attack:
						text = "攻　撃";
						break;
					case MenuItem.Defense:
						text = "防　御";
						break;
					case MenuItem.Skill:
						text = "スキル";
						break;
					case MenuItem.Item:
						text = "アイテム";
						break;
					case MenuItem.Escape:
						text = "逃げる";
						break;
				}
				Game.DXLib.DrawStringC(
					x + width / 2,
					actor.Location.Y - MenuItem.len() * LineHeight + OffsetCorner + idx * 22,
					0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
					text
				);
			}
			DrawCursorPanel(
				x + (width - CommandPanelWidth) / 2 + OffsetCorner,
				actor.Location.Y - MenuItem.len() * LineHeight + OffsetCorner + Cursor[0] * 22 - 3,
				CommandPanelWidth - OffsetCorner * 2,
				22,
				(Depth == 0 && IsTargetSelecting() == false)
			);
		}
	}

	//コマンド選択関連ウィンドウ
	if(Phase == BattlePhase.Command && Depth > 0 && TargetUTSelecting == false) {
		//リスト領域のウィンドウ
		DrawWindow(150, 0, Game.WindowSize.Width - 150 * 2, Game.WindowSize.Height - CommentPanelHeight - PartyPanelHeight - MenuItem.len() * LineHeight);

		switch(Cursor[0]) {
			case MenuItem.Skill:
				//スキル種別
				DrawWindow(0, 0, 150, 35 * SkillTypeList.len());
				foreach(idx, skillType in SkillTypeList) {
					Game.DXLib.DrawStringC(150 / 2, 16 + 22 * idx, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, skillType.Name)
				}

				//スキル一覧
				local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
				local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
				for(local i = (Depth < 2) ? 0 : TopIndex[2], n = 0; i < SkillList.len() && n < AreaMax; i++, n++) {
					local skill = SkillList[i];
					local color = WHITE;

					//使用可否判定
					if(actor.SelfValues.Get(0x46A20EE9/*MP*/).Value < skill.Value || actor.CheckUseSkill(skill.ID) == false) {
						color = DARKGRAY;
					} else {
						switch(db.GetCell(skill.ID, "場面制限").Integer) {
							case SceneLimit.OnlyMap:
							case SceneLimit.AlwaysNG:
								color = DARKGRAY;
								break;
						}
					}

					//アイコン
					if(EnabledIcons == TRUE) {
						local pos = db.GetCell(skill.ID, "アイコン").ConvertToPoint();
						Game.DXLib.DrawRectExtendGraph(
							Game.Material.Graphics.IconGraphicHandle,
							185,
							19 + n * 22,
							UseIconSize, UseIconSize,
							pos.X * Game.Material.Graphics.IconSize,
							pos.Y * Game.Material.Graphics.IconSize,
							Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
							true
						);
					}

					Game.DXLib.DrawString(150 + 55, 19 + 22 * n, 0x6DD57515/*ウィンドウ内テキスト*/, color, skill.Name);
					Game.DXLib.DrawStringR(
						150 + Game.WindowSize.Width - 150 * 2 - 50,
						19 + 22 * n,
						0x6DD57515/*ウィンドウ内テキスト*/,
						color,
						format("消費" + MPName + ":%3d", skill.Value)
					);
				}

				//スクロールバー
				if(Depth >= 2) {
					DrawVScrollBar(
						150 + Game.WindowSize.Width - 150 * 2,
						0,
						Game.WindowSize.Height - CommentPanelHeight - PartyPanelHeight - MenuItem.len() * LineHeight,
						SkillList.len(),
						TopIndex[2],
						AreaMax
					);
				}
				break;

			case MenuItem.Item:
				//選択種別
				DrawWindow(0, 0, 150, 35 * CommandItemType.len());
				foreach(idx in CommandItemType) {
					switch(idx) {
						case CommandItemType.Having:
							Game.DXLib.DrawStringC(150 / 2, 16 + 22 * idx, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, "所持品")
							break;
						case CommandItemType.Equip:
							Game.DXLib.DrawStringC(150 / 2, 16 + 22 * idx, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, "装備品")
							break;
					}
				}

				//アイテム一覧
				local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
				switch(Cursor[1]) {
					case CommandItemType.Having:	//所持品
						//所持品リスト
						for(local i = (Depth < 2) ? 0 : TopIndex[2], n = 0; i < Game.DB.VDBParty.GetItemCountAll() && n < AreaMax; i++, n++) {
							local item = Game.DB.VDBParty.GetItemDataFromIndex(i);
							local color = WHITE;
							local count = item.Value - GetReservedItemCount(item.ID);
							if(count <= 0) {
								color = DARKGRAY;
							}

							//アイコン
							if(EnabledIcons == TRUE) {
								local pos = db.GetCell(item.ID, "アイコン").ConvertToPoint();
								Game.DXLib.DrawRectExtendGraph(
									Game.Material.Graphics.IconGraphicHandle,
									185,
									19 + n * 22,
									UseIconSize, UseIconSize,
									pos.X * Game.Material.Graphics.IconSize,
									pos.Y * Game.Material.Graphics.IconSize,
									Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
									true
								);
							}

							Game.DXLib.DrawString(150 + 55, 19 + 22 * n, 0x6DD57515/*ウィンドウ内テキスト*/, color, item.Name);
							Game.DXLib.DrawString(150 + Game.WindowSize.Width - 150 * 2 - 90, 19 + 22 * n, 0x6DD57515/*ウィンドウ内テキスト*/, color, format("× %d", count));
						}

						//スクロールバー
						if(Depth >= 2) {
							DrawVScrollBar(
								150 + Game.WindowSize.Width - 150 * 2,
								0,
								Game.WindowSize.Height - CommentPanelHeight - PartyPanelHeight - MenuItem.len() * LineHeight,
								Game.DB.VDBParty.GetItemCountAll(),
								TopIndex[2],
								AreaMax
							);
						}
						break;

					case CommandItemType.Equip:		//装備品
						local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));

						//装備品リスト
						local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
						foreach(idx, equipTypeID in EquipTypeList) {
							local equipID = actor.GetEquipIDFromItemTypeID(equipTypeID);
							local equipName = "---------";
							local typeName = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetCell(
								equipTypeID,
								Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType).GetColumnName(DBDefaultColumnIndices.Name)
								).String;
							if(equipID != -1) {
								equipName = itemDB.GetCell(equipID, itemDB.GetColumnName(DBDefaultColumnIndices.Name)).String;

								//アイコン
								if(EnabledIcons == TRUE) {
									local pos = itemDB.GetCell(equipID, "アイコン").ConvertToPoint();
									Game.DXLib.DrawRectExtendGraph(
										Game.Material.Graphics.IconGraphicHandle,
										150 + 150,
										19 + idx * 22,
										UseIconSize, UseIconSize,
										pos.X * Game.Material.Graphics.IconSize,
										pos.Y * Game.Material.Graphics.IconSize,
										Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
										true
									);
								}
							}
							Game.DXLib.DrawString(150 + 30, 19 + 22 * idx, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, typeName + ":");
							Game.DXLib.DrawString(150 + 170, 19 + 22 * idx, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, equipName);
						}
						break;
				}
				break;
		}

		//カーソル
		DrawCursorPanel(16, 16 + Cursor[1] * 22 - 3, 150 - 16 * 2, 22, (Depth == 1));
		if(Depth >= 2) {
			DrawCursorPanel(150 + 16, 16 + Cursor[2] * 22, Game.WindowSize.Width - 150 * 2 - 50, 22, (Depth == 2 && IsTargetSelecting() == false));
		}
	}

	//ダメージモーション処理
	foreach(val in PTPanelShakes) {
		if(val.Doing == true) {
			val.DoShake();
		}
	}
	foreach(val in DamageMotionUT) {
		if(val.Doing == true) {
			val.FrameCounter++;
			if(val.FrameCounter >= 40) {
				val.Doing = false;		//モーション終了
			}
		}
	}

	//ダメージ数値表示
	for(local i = 0; i < DamageEffectTasks.len(); ) {
		local task = DamageEffectTasks[i];
		local color = -1;
		local value = task.Value.tostring();
		local offset = 0;
		switch(task.Type) {
			case SQDamageEffectData.DamageEffectType.HPDamage:
				color = RED;
				break;
			case SQDamageEffectData.DamageEffectType.HPHeal:
				color = GREEN;
				break;
			case SQDamageEffectData.DamageEffectType.MPDamage:
				color = BLUE;
				break;
			case SQDamageEffectData.DamageEffectType.MPHeal:
				color = GREEN;
				break;
			case SQDamageEffectData.DamageEffectType.Critical:
				color = BLUE;
				value = "Critical!";
				offset = -30;
				break;
			case SQDamageEffectData.DamageEffectType.Miss:
				color = DARKGRAY;
				value = "Miss";
				break;
			case SQDamageEffectData.DamageEffectType.Reflect:
				color = GREENYELLOW;
				value = "Reflected!";
				offset = -30;
				break;
		}

		local deltaTime = Game.DXLib.GetNowCount() - task.StartTimeMS;
		local deltaPos = sin(deltaTime / 800.0 + (5/7.0 * PI));
		task.PositionY += 1.8 * deltaPos;

		Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, (deltaPos > -0.2) ? 255 : (255 - (deltaTime - 1000) * 255 / 1000));
		if(GetAttackSide(task.ActorIndex) == AttackSide.Party) {
			Game.DXLib.DrawStringC(
				width * task.ActorIndex + width / 2,
				GetActorData(task.ActorIndex).Location.Y + task.PositionY + offset,
				0xC3A97C0/*ダメージ数値表示用*/, color, value
			);
		} else {
			Game.DXLib.DrawStringC(
				GetActorData(task.ActorIndex).Location.X + GetActorData(task.ActorIndex).GraphSize.Width / 2,
				GetActorData(task.ActorIndex).Location.Y + task.PositionY + offset,
				0xC3A97C0/*ダメージ数値表示用*/ color, value
			);
		}
		Game.DXLib.SetDrawBlendMode(-1, 0);

		if(deltaTime >= 2500) {
			//エフェクト終了
			DamageEffectTasks.remove(i);
			continue;
		}
		i++;
	}

	//フェーズごとの処理
	switch(Phase) {
		case BattlePhase.Ending:
			//Victoryパネル
			if(Result != BattleResult.Win) {
				break;		//勝利時以外は特に描画しない
			}
			local offset = 20;
			local width = 300;
			local top = 50;
			local left = Game.WindowSize.Width / 2 - width / 2;
			DrawWindow(left, top, width, 210);

			switch(Depth) {
				case EndingDepth.EXPMoney:		//経験値、お金獲得
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						"獲得経験値:"
					);
					Game.DXLib.DrawStringR(
						left + width - offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						Results.EXP.tostring()
					);
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 1,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						"獲得金額:"
					);
					Game.DXLib.DrawStringR(
						left + width - offset,
						top + offset + 25 * 1,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						Results.Money + " " + US1246572451/*通貨名称*/
					);
					break;

				case EndingDepth.LevelUp:		//レベルアップ
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						Results.LevelUpName
					);
					Game.DXLib.DrawStringR(
						left + width - offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						format("Lv: %d → %d", Results.BeforeLevel, Results.AfterLevel)
					);
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 1,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						"パラメーター上昇:"
					);
					foreach(idx, par in Results.LevelUpParameters) {
						Game.DXLib.DrawString(
							left + offset * 2,
							top + 20 + 25 * 2 + 17 * idx,
							0x2E19E6A3/*セーブデータリスト用*/, WHITE,
							par.Name
						);
						if(par.Value > 0) {
							Game.DXLib.DrawStringR(
								left + width - offset * 2,
								top + 20 + 25 * 2 + 17 * idx,
								0x2E19E6A3/*セーブデータリスト用*/, GREENYELLOW,
								format("+%d", par.Value)
							);
						} else {
							Game.DXLib.DrawStringR(
								left + width - offset * 2,
								top + 20 + 25 * 2 + 17 * idx,
								0x2E19E6A3/*セーブデータリスト用*/, GRAY,
								par.Value.tostring()
							);
						}
					}
					break;

				case EndingDepth.LearnSkill:	//スキル習得
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						Results.LevelUpName
					);
					Game.DXLib.DrawStringR(
						left + width - offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						format("Lv: %d → %d", Results.BeforeLevel, Results.AfterLevel)
					);
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 1,
						0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
						"習得スキル:"
					);

					//習得スキル一覧
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
					foreach(idx, skill in Results.LevelUpSkills) {
						//アイコン
						if(EnabledIcons == TRUE) {
							local pos = db.GetCell(skill.ID, "アイコン").ConvertToPoint();
							Game.DXLib.DrawRectExtendGraph(
								Game.Material.Graphics.IconGraphicHandle,
								left + offset * 2,
								top + 20 + 25 * 2 + 20 * idx,
								UseIconSize, UseIconSize,
								pos.X * Game.Material.Graphics.IconSize,
								pos.Y * Game.Material.Graphics.IconSize,
								Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
								true
							);
						}

						Game.DXLib.DrawString(
							left + offset * 2 + 20,
							top + 20 + 25 * 2 + 20 * idx,
							0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
							skill.Name
						);
					}
					break;

				case EndingDepth.DropItem:		//ドロップアイテム
					Game.DXLib.DrawString(
						left + offset,
						top + offset + 25 * 0,
						0x6DD57515/*ウィンドウ内テキスト*/,
						WHITE,
						"拾得アイテム:"
					);
					
					//拾得アイテム一覧
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					foreach(idx, item in Results.DropItems) {
						//アイコン
						if(EnabledIcons == TRUE) {
							local pos = db.GetCell(item.ID, "アイコン").ConvertToPoint();
							Game.DXLib.DrawRectExtendGraph(
								Game.Material.Graphics.IconGraphicHandle,
								left + offset * 2,
								top + 20 + 25 * 1 + 20 * idx,
								UseIconSize, UseIconSize,
								pos.X * Game.Material.Graphics.IconSize,
								pos.Y * Game.Material.Graphics.IconSize,
								Game.Material.Graphics.IconSize, Game.Material.Graphics.IconSize,
								true
							);
						}

						Game.DXLib.DrawString(
							left + offset * 2 + 20,
							top + 20 + 25 * 1 + 20 * idx,
							0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
							item.Name
						);
					}
					break;
			}
			break;

		default:
			//メッセージパネル
			DrawWindow(0, Game.WindowSize.Height - CommentPanelHeight, Game.WindowSize.Width, CommentPanelHeight);
			DrawComment(GetBattleComment());
			break;
	}
}
