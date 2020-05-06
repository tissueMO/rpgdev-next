//###################################################################
//		バトルシーン: サブルーチン: コマンド選択フェーズ
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuStatus.nut");

///[summary]コマンド選択フェーズの入力処理
function Routine_PhaseCommand() {
	//トップ階層のみ優先処理 (決定キーを除く)
	if(Depth == 0) {
		if(CheckUpKeyInput() == true) {
			Cursor[0] = Game.DownAround(Cursor[0], 0, MenuItem.len());
			return;
		} else if(CheckDownKeyInput() == true) {
			Cursor[0] = Game.UpAround(Cursor[0], 0, MenuItem.len());
			return;
		} else if(CheckCancelKeyInput() == 1) {
			//前の人のコマンド選択に戻る
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
			PhaseCommandSubRoutine.MoveToPreviousActorCommand();
			return;
		}
	}

	//コマンド別の処理
	CommandInput[Cursor[0]]();
}


//コマンド選択フェーズのサブ関数
PhaseCommandSubRoutine <- {

	///[summary]前のアクターにコマンド選択を戻す
	function MoveToPreviousActorCommand() {
		local startIdx = TopIndex[0];
		Depth = 0;
		Cursor[0] = 0;

		do {
			TopIndex[0]--;
			if(TopIndex[0] < 0) {
				//これ以上戻れない
				TopIndex[0] = startIdx;
				return;
			}

			//一つ前のキャラが行動選択できない/自律戦闘の場合はさらに前に戻る
		} while(PhaseCommandSubRoutine.CheckActorSelectCommand(TopIndex[0]) == false);

		//戻したキャラの行動予約を破棄する
		if(Actions.len() > 0) {
			Actions.pop();
		}
	}


	///[summary]現在のアクターの行動を確定して次のアクターにコマンド選択を移す
	///[par]行動内容 (SQActionData)
	function MoveToNextActorCommand(actionData) {
		//行動リストに登録する
		if(actionData != null) {
			Actions.append(actionData);
		}

		Depth = 0;
		Cursor[0] = 0;

		do {
			TopIndex[0]++;

			//コマンド選択が終わったら、行動中フェーズに移る
			if(TopIndex[0] >= Game.DB.VDBParty.GetCharCount()) {
				MoveToActionPhase();
				return;
			}
			//次のキャラが行動選択できない/自律戦闘の場合はさらに次へ進む
		} while(PhaseCommandSubRoutine.CheckActorSelectCommand(TopIndex[0]) == false);
	}


	///[summary]指定したインデックスのアクターが現在プレイヤーによってコマンド選択できるかどうかを調べる
	///[par]アクターのインデックス
	///[return]プレイヤーによってコマンド選択できるかどうか
	function CheckActorSelectCommand(idx) {
		local actor = GetActorData(idx);
		if(actor.IsNPC == true) {
			return false;
		} else if(IsForbiddenAction(idx) == true) {
			return false;
		}
		return true;
	}
};


//コマンド別の入力処理
CommandInput <- {

	///[summary]攻撃
	[MenuItem.Attack] = function() {
		if(CheckDecideKeyInput() == 1) {
			//対象選択させてから確定へ
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			local idx = SelectUTEnemy();
			if(idx != -1) {
				local effectID = EmptyEquipEffectID;
				local actor = GetActorData(TopIndex[0]);
				if(actor.GetEquipIDFromItemTypeID(0x12414D21/*武器*/) != -1) {
					effectID = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(actor.GetEquipIDFromItemTypeID(0x12414D21/*武器*/), "エフェクト").Integer;
				}
				PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], idx, Cursor[0], -1, effectID));
			}
		}
	},


	///[summary]防御
	[MenuItem.Defense] = function() {
		if(CheckDecideKeyInput() == 1) {
			//コマンド決定、次の人へ移る
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], -1, Cursor[0], -1, -1));
		}
	},


	///[summary]スキル
	[MenuItem.Skill] = function() {
		switch(Depth) {
			case 0:
				//トップ階層: スキル種別選択へ
				if(CheckDecideKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					MoveToNextDepth();
					RefreshSkillTypeList();		//スキル種別リストを更新
					RefreshSkillList();			//スキルリストを更新
				}
				break;

			case 1:
				//スキル種別選択
				if(CheckUpKeyInput() == true) {
					Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, SkillTypeList.len());
					RefreshSkillList();			//スキルリストを更新
				} else if(CheckDownKeyInput() == true) {
					Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, SkillTypeList.len());
					RefreshSkillList();			//スキルリストを更新
				} else if(CheckDecideKeyInput() == 1) {
					if(SkillList.len() == 0) {
						//一つもない場合は進めない
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("この種別のスキルを一つも習得していません。");
						return;
					} else if(GetActorData(TopIndex[0]).CheckUseSkillType(SkillTypeList[Cursor[Depth]].ID) == false) {
						//この種別のスキルが封じられている
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("現在この種別のスキルは使えません。");
						return;
					}
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					MoveToNextDepth();
				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
					Depth--;
				}
				break;

			case 2:
				//スキル選択
				if(CheckUpKeyInput() == true) {
					if(0 < Cursor[Depth] + TopIndex[Depth]) {
						if(0 < Cursor[Depth]) {
							Cursor[Depth]--;
						} else {
							TopIndex[Depth]--;
						}
					}
				} else if(CheckDownKeyInput() == true) {
					if(Cursor[Depth] + TopIndex[Depth] + 1 < SkillList.len()) {
						if(Cursor[Depth] + 1 < AreaMax) {
							Cursor[Depth]++;
						} else {
							TopIndex[Depth]++;
						}
					}
				} else if(CheckDecideKeyInput() == 1) {
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
					local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
					local skillID = SkillList[Cursor[2] + TopIndex[2]].ID;

					//使用可否判定
					switch(Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(skillID, "場面制限").Integer) {
						case SceneLimit.OnlyMap:
						case SceneLimit.AlwaysNG:
							Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
							ShowErrorMessage("このスキルは戦闘中に使えません。");
							return;
					}
					if(actor.SelfValues.Get(0x46A20EE9/*MP*/).Value < db.GetCell(skillID, "コスト").Integers.Get(0)) {
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage(MPName + "が足りません。");
						return;
					}
					if(actor.CheckUseSkill(skillID) == false) {
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("このスキルは現在使えません。");
						return;
					}

					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);

					//対象選択
					local target = -1;
					switch(db.GetCell(skillID, "対象種別").Integer) {
						case TargetType.OneEnemy:
							target = SelectUTEnemy();
							if(target == -1) {
								return;		//キャンセル
							}
							break;
						case TargetType.OneParty:
							target = SelectPTMember(false);
							if(target == -1) {
								return;		//キャンセル
							}
							break;
						case TargetType.OnePartyWithDead:
							target = SelectPTMember(true);
							if(target == -1) {
								return;		//キャンセル
							}
							break;
					}

					//行動確定
					PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], target, Cursor[0], skillID, -1));
				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
					Depth--;
				}
				break;
		}
	},


	///[summary]アイテム
	[MenuItem.Item] = function() {
		switch(Depth) {
			case 0:
				//トップ階層: 所持・装備の種別選択へ
				if(CheckDecideKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					MoveToNextDepth();
					RefreshEquipTypeList();		//装備品の大種別リストを更新
				}
				break;

			case 1:
				//種別選択 (所持品/装備品)
				if(CheckUpKeyInput() == true) {
					Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, CommandItemType.len());
				} else if(CheckDownKeyInput() == true) {
					Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, CommandItemType.len());
				} else if(CheckDecideKeyInput() == 1) {
					if(Cursor[1] == CommandItemType.Having && Game.DB.VDBParty.GetItemCountAll() == 0) {
						//一つもない場合は進めない
						Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
						ShowErrorMessage("アイテムを一つも持っていません。");
						return;
					}
					Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
					MoveToNextDepth();
				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
					Depth--;
				}
				break;

			case 2:
				//アイテム選択
				if(CheckUpKeyInput() == true) {
					if(0 < Cursor[Depth] + TopIndex[Depth]) {
						if(0 < Cursor[Depth]) {
							Cursor[Depth]--;
						} else {
							TopIndex[Depth]--;
						}
					}
				} else if(CheckDownKeyInput() == true) {
					local max = 0;
					switch(Cursor[1]) {
						case CommandItemType.Having:	//所持品
							max = Game.DB.VDBParty.GetItemCountAll();
							break;
						case CommandItemType.Equip:		//装備品
							max = EquipTypeList.len();
							break;
					}
					if(Cursor[Depth] + TopIndex[Depth] + 1 < max) {
						if(Cursor[Depth] + 1 < AreaMax) {
							Cursor[Depth]++;
						} else {
							TopIndex[Depth]++;
						}
					}
				} else if(CheckDecideKeyInput() == 1) {
					local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
					local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(TopIndex[0]));
					local itemID = -1;

					switch(Cursor[1]) {
						case CommandItemType.Having:
							//所持品
							local itemData = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[2] + TopIndex[2]);
							itemID = itemData.ID;
							local itemTypeDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType);
							local itemTypeID = db.GetCell(itemID, "アイテム大種別").Integers.Get(0);

							if(itemData.Value - GetReservedItemCount(itemID) <= 0) {
								//他のキャラの行動予約によって無効にされている場合は選択できない
								Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
								ShowErrorMessage("このアイテムは他のメンバーによって選択されています。");
								return;
							}

							//戦闘中に使えないアイテムかどうか調べる
							local forbiddenUse = false;
							switch(db.GetCell(itemID, "場面制限").Integer) {
								case SceneLimit.OnlyMap:
								case SceneLimit.AlwaysNG:
									forbiddenUse = true;
									break;
							}

							if(itemTypeID != 0 && itemTypeDB.GetCell(itemTypeID, "装備品").Boolean == true) {
								//装備品の場合は 使用or装備 を問い合わせる
								Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
								local text = "操作を選択して下さい。|使う";
								if(actor.CheckEquip(itemID) == true) {
									//装備できる場合のみ装備の操作を加える
									text += "|装備する";
								}
								switch(ShowErrorMessage(text)) {
									case -1:	//キャンセル
										return;

									case 1:		//装備する
										//更新前に装備していたアイテムをすべて記憶する
										local beforeEquipIDs = {};
										foreach(idx, typeID in EquipTypeList) {
											beforeEquipIDs[typeID] <- actor.GetEquipIDFromItemTypeID(typeID);
										}

										local afterItemID = itemID;
										if(actor.SetEquip(afterItemID, false) == false) {
											//操作に失敗
											Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
											ShowErrorMessage("現在このキャラは装備を変更できない状態になっています。");
											return;
										}

										//HP・MPが最大値を超えないように自動補正
										FitToMaxParameters(actor);

										//装備していたアイテムを所持アイテムに戻す
										local isSelected = false;
										local addItems = array(0);
										foreach(equipTypeID, beforeEquipID in beforeEquipIDs) {
											local appended = false;
											if(beforeEquipID != -1) {
												foreach(i, itemID in addItems) {
													if(beforeEquipID == itemID) {
														appended = true;
														break;
													}
												}
											}
											if(appended || beforeEquipID == actor.GetEquipIDFromItemTypeID(equipTypeID)) {
												//既に外した装備品 or 装備が変化しなかった部位はスキップ
												continue;
											}

											//少なくとも装備欄に変化があったとしてマークしておく
											isSelected = true;
											if(beforeEquipID == -1) {
												//外す装備品がないのでスキップ
												continue;
											}

											//外した装備品をアイテム一覧に戻す
											Game.DB.VDBParty.AddItem(beforeEquipID, 1);

											//複数の装備欄を占領する単一のアイテムを重複して戻さないようにする
											addItems.append(beforeEquipID);
										}

										//実際に装備品を選んだ場合
										if(isSelected == true) {
											//所持アイテムから減らす
											Game.DB.VDBParty.DeleteItem(afterItemID, 1);
										}

										//カーソル位置補正
										if(Game.DB.VDBParty.GetItemCountAll() <= Cursor[2] + TopIndex[2]) {
											if(0 < TopIndex[2]) {
												TopIndex[2]--;
											} else {
												Cursor[2]--;
											}
											if(Cursor[2] + TopIndex[2] <= 0) {
												//所持品が一つもなくなった場合は階層を戻す
												Depth--;
											}
										}
										return;
								}
							}

							if(forbiddenUse == true) {
								Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
								ShowErrorMessage("このアイテムは戦闘中に使えません。");
								return;
							}
							Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
							break;

						case CommandItemType.Equip:
							//装備品
							itemID = actor.GetEquipIDFromItemTypeID(EquipTypeList[Cursor[2]]);
							if(itemID == -1) {
								//装備していない場合は操作できない
								Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
								return;
							}

							//戦闘中に使えないアイテムかどうか調べる
							local forbiddenUse = false;
							switch(db.GetCell(itemID, "場面制限").Integer) {
								case SceneLimit.OnlyMap:
								case SceneLimit.AlwaysNG:
									forbiddenUse = true;
									break;
							}

							Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
							switch(ShowErrorMessage("操作を選択して下さい。|使う|外す")) {
								case -1:	//キャンセル
									return;

								case 0:		//使う
									if(forbiddenUse == true) {
										Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
										ShowErrorMessage("このアイテムは戦闘中に使えません。");
										return;
									}
									break;

								case 1:		//外す
									if(actor.ReleaseEquipType(EquipTypeList[Cursor[2]], false) == false) {
										//操作に失敗
										Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
										ShowErrorMessage("現在このキャラは装備を変更できない状態になっています。");
										return;
									}
									//以前の装備品を所持品に加える
									Game.DB.VDBParty.AddItem(itemID, 1);
									return;
							}
							break;
					}

					//対象選択
					local target = -1;
					switch(db.GetCell(itemID, "対象種別").Integer) {
						case TargetType.OneEnemy:
							target = SelectUTEnemy();
							if(target == -1) {
								return;		//キャンセル
							}
							break;
						case TargetType.OneParty:
							target = SelectPTMember(false);
							if(target == -1) {
								return;		//キャンセル
							}
							break;
						case TargetType.OnePartyWithDead:
							target = SelectPTMember(true);
							if(target == -1) {
								return;		//キャンセル
							}
							break;
					}

					//行動確定
					PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], target, Cursor[0], itemID, -1));

				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
					Depth--;
				}
				break;
		}
	},


	///[summary]逃げる
	[MenuItem.Escape] = function() {
		if(CheckDecideKeyInput() == 1) {
			if(DisabledEscape == true) {
				//逃げる禁止
				Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
				ShowErrorMessage("このバトルでは 逃げることができない！");
				return;
			}

			//コマンド決定、次の人へ移る
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], -1, Cursor[0], -1, -1));
		}
	},
};
