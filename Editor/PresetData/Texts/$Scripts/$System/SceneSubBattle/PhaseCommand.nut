//###################################################################
//		�o�g���V�[��: �T�u���[�`��: �R�}���h�I���t�F�[�Y
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuStatus.nut");

///[summary]�R�}���h�I���t�F�[�Y�̓��͏���
function Routine_PhaseCommand() {
	//�g�b�v�K�w�̂ݗD�揈�� (����L�[������)
	if(Depth == 0) {
		if(CheckUpKeyInput() == true) {
			Cursor[0] = Game.DownAround(Cursor[0], 0, MenuItem.len());
			return;
		} else if(CheckDownKeyInput() == true) {
			Cursor[0] = Game.UpAround(Cursor[0], 0, MenuItem.len());
			return;
		} else if(CheckCancelKeyInput() == 1) {
			//�O�̐l�̃R�}���h�I���ɖ߂�
			Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
			PhaseCommandSubRoutine.MoveToPreviousActorCommand();
			return;
		}
	}

	//�R�}���h�ʂ̏���
	CommandInput[Cursor[0]]();
}


//�R�}���h�I���t�F�[�Y�̃T�u�֐�
PhaseCommandSubRoutine <- {

	///[summary]�O�̃A�N�^�[�ɃR�}���h�I����߂�
	function MoveToPreviousActorCommand() {
		local startIdx = TopIndex[0];
		Depth = 0;
		Cursor[0] = 0;

		do {
			TopIndex[0]--;
			if(TopIndex[0] < 0) {
				//����ȏ�߂�Ȃ�
				TopIndex[0] = startIdx;
				return;
			}

			//��O�̃L�������s���I���ł��Ȃ�/�����퓬�̏ꍇ�͂���ɑO�ɖ߂�
		} while(PhaseCommandSubRoutine.CheckActorSelectCommand(TopIndex[0]) == false);

		//�߂����L�����̍s���\���j������
		if(Actions.len() > 0) {
			Actions.pop();
		}
	}


	///[summary]���݂̃A�N�^�[�̍s�����m�肵�Ď��̃A�N�^�[�ɃR�}���h�I�����ڂ�
	///[par]�s�����e (SQActionData)
	function MoveToNextActorCommand(actionData) {
		//�s�����X�g�ɓo�^����
		if(actionData != null) {
			Actions.append(actionData);
		}

		Depth = 0;
		Cursor[0] = 0;

		do {
			TopIndex[0]++;

			//�R�}���h�I�����I�������A�s�����t�F�[�Y�Ɉڂ�
			if(TopIndex[0] >= Game.DB.VDBParty.GetCharCount()) {
				MoveToActionPhase();
				return;
			}
			//���̃L�������s���I���ł��Ȃ�/�����퓬�̏ꍇ�͂���Ɏ��֐i��
		} while(PhaseCommandSubRoutine.CheckActorSelectCommand(TopIndex[0]) == false);
	}


	///[summary]�w�肵���C���f�b�N�X�̃A�N�^�[�����݃v���C���[�ɂ���ăR�}���h�I���ł��邩�ǂ����𒲂ׂ�
	///[par]�A�N�^�[�̃C���f�b�N�X
	///[return]�v���C���[�ɂ���ăR�}���h�I���ł��邩�ǂ���
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


//�R�}���h�ʂ̓��͏���
CommandInput <- {

	///[summary]�U��
	[MenuItem.Attack] = function() {
		if(CheckDecideKeyInput() == 1) {
			//�ΏۑI�������Ă���m���
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			local idx = SelectUTEnemy();
			if(idx != -1) {
				local effectID = EmptyEquipEffectID;
				local actor = GetActorData(TopIndex[0]);
				if(actor.GetEquipIDFromItemTypeID(0x12414D21/*����*/) != -1) {
					effectID = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0).GetCell(actor.GetEquipIDFromItemTypeID(0x12414D21/*����*/), "�G�t�F�N�g").Integer;
				}
				PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], idx, Cursor[0], -1, effectID));
			}
		}
	},


	///[summary]�h��
	[MenuItem.Defense] = function() {
		if(CheckDecideKeyInput() == 1) {
			//�R�}���h����A���̐l�ֈڂ�
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], -1, Cursor[0], -1, -1));
		}
	},


	///[summary]�X�L��
	[MenuItem.Skill] = function() {
		switch(Depth) {
			case 0:
				//�g�b�v�K�w: �X�L����ʑI����
				if(CheckDecideKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					MoveToNextDepth();
					RefreshSkillTypeList();		//�X�L����ʃ��X�g���X�V
					RefreshSkillList();			//�X�L�����X�g���X�V
				}
				break;

			case 1:
				//�X�L����ʑI��
				if(CheckUpKeyInput() == true) {
					Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, SkillTypeList.len());
					RefreshSkillList();			//�X�L�����X�g���X�V
				} else if(CheckDownKeyInput() == true) {
					Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, SkillTypeList.len());
					RefreshSkillList();			//�X�L�����X�g���X�V
				} else if(CheckDecideKeyInput() == 1) {
					if(SkillList.len() == 0) {
						//����Ȃ��ꍇ�͐i�߂Ȃ�
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("���̎�ʂ̃X�L��������K�����Ă��܂���B");
						return;
					} else if(GetActorData(TopIndex[0]).CheckUseSkillType(SkillTypeList[Cursor[Depth]].ID) == false) {
						//���̎�ʂ̃X�L�����������Ă���
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("���݂��̎�ʂ̃X�L���͎g���܂���B");
						return;
					}
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					MoveToNextDepth();
				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
					Depth--;
				}
				break;

			case 2:
				//�X�L���I��
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

					//�g�p�۔���
					switch(Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0).GetCell(skillID, "��ʐ���").Integer) {
						case SceneLimit.OnlyMap:
						case SceneLimit.AlwaysNG:
							Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
							ShowErrorMessage("���̃X�L���͐퓬���Ɏg���܂���B");
							return;
					}
					if(actor.SelfValues.Get(0x46A20EE9/*MP*/).Value < db.GetCell(skillID, "�R�X�g").Integers.Get(0)) {
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage(MPName + "������܂���B");
						return;
					}
					if(actor.CheckUseSkill(skillID) == false) {
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("���̃X�L���͌��ݎg���܂���B");
						return;
					}

					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);

					//�ΏۑI��
					local target = -1;
					switch(db.GetCell(skillID, "�Ώێ��").Integer) {
						case TargetType.OneEnemy:
							target = SelectUTEnemy();
							if(target == -1) {
								return;		//�L�����Z��
							}
							break;
						case TargetType.OneParty:
							target = SelectPTMember(false);
							if(target == -1) {
								return;		//�L�����Z��
							}
							break;
						case TargetType.OnePartyWithDead:
							target = SelectPTMember(true);
							if(target == -1) {
								return;		//�L�����Z��
							}
							break;
					}

					//�s���m��
					PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], target, Cursor[0], skillID, -1));
				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
					Depth--;
				}
				break;
		}
	},


	///[summary]�A�C�e��
	[MenuItem.Item] = function() {
		switch(Depth) {
			case 0:
				//�g�b�v�K�w: �����E�����̎�ʑI����
				if(CheckDecideKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					MoveToNextDepth();
					RefreshEquipTypeList();		//�����i�̑��ʃ��X�g���X�V
				}
				break;

			case 1:
				//��ʑI�� (�����i/�����i)
				if(CheckUpKeyInput() == true) {
					Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, CommandItemType.len());
				} else if(CheckDownKeyInput() == true) {
					Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, CommandItemType.len());
				} else if(CheckDecideKeyInput() == 1) {
					if(Cursor[1] == CommandItemType.Having && Game.DB.VDBParty.GetItemCountAll() == 0) {
						//����Ȃ��ꍇ�͐i�߂Ȃ�
						Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
						ShowErrorMessage("�A�C�e������������Ă��܂���B");
						return;
					}
					Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
					MoveToNextDepth();
				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
					Depth--;
				}
				break;

			case 2:
				//�A�C�e���I��
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
						case CommandItemType.Having:	//�����i
							max = Game.DB.VDBParty.GetItemCountAll();
							break;
						case CommandItemType.Equip:		//�����i
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
							//�����i
							local itemData = Game.DB.VDBParty.GetItemDataFromIndex(Cursor[2] + TopIndex[2]);
							itemID = itemData.ID;
							local itemTypeDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Element).GetSubDB(DBElementIndices.ItemType);
							local itemTypeID = db.GetCell(itemID, "�A�C�e������").Integers.Get(0);

							if(itemData.Value - GetReservedItemCount(itemID) <= 0) {
								//���̃L�����̍s���\��ɂ���Ė����ɂ���Ă���ꍇ�͑I���ł��Ȃ�
								Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
								ShowErrorMessage("���̃A�C�e���͑��̃����o�[�ɂ���đI������Ă��܂��B");
								return;
							}

							//�퓬���Ɏg���Ȃ��A�C�e�����ǂ������ׂ�
							local forbiddenUse = false;
							switch(db.GetCell(itemID, "��ʐ���").Integer) {
								case SceneLimit.OnlyMap:
								case SceneLimit.AlwaysNG:
									forbiddenUse = true;
									break;
							}

							if(itemTypeID != 0 && itemTypeDB.GetCell(itemTypeID, "�����i").Boolean == true) {
								//�����i�̏ꍇ�� �g�por���� ��₢���킹��
								Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
								local text = "�����I�����ĉ������B|�g��";
								if(actor.CheckEquip(itemID) == true) {
									//�����ł���ꍇ�̂ݑ����̑����������
									text += "|��������";
								}
								switch(ShowErrorMessage(text)) {
									case -1:	//�L�����Z��
										return;

									case 1:		//��������
										//�X�V�O�ɑ������Ă����A�C�e�������ׂċL������
										local beforeEquipIDs = {};
										foreach(idx, typeID in EquipTypeList) {
											beforeEquipIDs[typeID] <- actor.GetEquipIDFromItemTypeID(typeID);
										}

										local afterItemID = itemID;
										if(actor.SetEquip(afterItemID, false) == false) {
											//����Ɏ��s
											Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
											ShowErrorMessage("���݂��̃L�����͑�����ύX�ł��Ȃ���ԂɂȂ��Ă��܂��B");
											return;
										}

										//HP�EMP���ő�l�𒴂��Ȃ��悤�Ɏ����␳
										FitToMaxParameters(actor);

										//�������Ă����A�C�e���������A�C�e���ɖ߂�
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
												//���ɊO���������i or �������ω����Ȃ��������ʂ̓X�L�b�v
												continue;
											}

											//���Ȃ��Ƃ��������ɕω����������Ƃ��ă}�[�N���Ă���
											isSelected = true;
											if(beforeEquipID == -1) {
												//�O�������i���Ȃ��̂ŃX�L�b�v
												continue;
											}

											//�O���������i���A�C�e���ꗗ�ɖ߂�
											Game.DB.VDBParty.AddItem(beforeEquipID, 1);

											//�����̑��������̂���P��̃A�C�e�����d�����Ė߂��Ȃ��悤�ɂ���
											addItems.append(beforeEquipID);
										}

										//���ۂɑ����i��I�񂾏ꍇ
										if(isSelected == true) {
											//�����A�C�e�����猸�炷
											Game.DB.VDBParty.DeleteItem(afterItemID, 1);
										}

										//�J�[�\���ʒu�␳
										if(Game.DB.VDBParty.GetItemCountAll() <= Cursor[2] + TopIndex[2]) {
											if(0 < TopIndex[2]) {
												TopIndex[2]--;
											} else {
												Cursor[2]--;
											}
											if(Cursor[2] + TopIndex[2] <= 0) {
												//�����i������Ȃ��Ȃ����ꍇ�͊K�w��߂�
												Depth--;
											}
										}
										return;
								}
							}

							if(forbiddenUse == true) {
								Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
								ShowErrorMessage("���̃A�C�e���͐퓬���Ɏg���܂���B");
								return;
							}
							Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
							break;

						case CommandItemType.Equip:
							//�����i
							itemID = actor.GetEquipIDFromItemTypeID(EquipTypeList[Cursor[2]]);
							if(itemID == -1) {
								//�������Ă��Ȃ��ꍇ�͑���ł��Ȃ�
								Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
								return;
							}

							//�퓬���Ɏg���Ȃ��A�C�e�����ǂ������ׂ�
							local forbiddenUse = false;
							switch(db.GetCell(itemID, "��ʐ���").Integer) {
								case SceneLimit.OnlyMap:
								case SceneLimit.AlwaysNG:
									forbiddenUse = true;
									break;
							}

							Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
							switch(ShowErrorMessage("�����I�����ĉ������B|�g��|�O��")) {
								case -1:	//�L�����Z��
									return;

								case 0:		//�g��
									if(forbiddenUse == true) {
										Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
										ShowErrorMessage("���̃A�C�e���͐퓬���Ɏg���܂���B");
										return;
									}
									break;

								case 1:		//�O��
									if(actor.ReleaseEquipType(EquipTypeList[Cursor[2]], false) == false) {
										//����Ɏ��s
										Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
										ShowErrorMessage("���݂��̃L�����͑�����ύX�ł��Ȃ���ԂɂȂ��Ă��܂��B");
										return;
									}
									//�ȑO�̑����i�������i�ɉ�����
									Game.DB.VDBParty.AddItem(itemID, 1);
									return;
							}
							break;
					}

					//�ΏۑI��
					local target = -1;
					switch(db.GetCell(itemID, "�Ώێ��").Integer) {
						case TargetType.OneEnemy:
							target = SelectUTEnemy();
							if(target == -1) {
								return;		//�L�����Z��
							}
							break;
						case TargetType.OneParty:
							target = SelectPTMember(false);
							if(target == -1) {
								return;		//�L�����Z��
							}
							break;
						case TargetType.OnePartyWithDead:
							target = SelectPTMember(true);
							if(target == -1) {
								return;		//�L�����Z��
							}
							break;
					}

					//�s���m��
					PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], target, Cursor[0], itemID, -1));

				} else if(CheckCancelKeyInput() == 1) {
					Game.DB.FDBMaterial.Play(0x79C24514/*�L�����Z����*/, 255, 0, 0);
					Depth--;
				}
				break;
		}
	},


	///[summary]������
	[MenuItem.Escape] = function() {
		if(CheckDecideKeyInput() == 1) {
			if(DisabledEscape == true) {
				//������֎~
				Game.DB.FDBMaterial.Play(0x293AF6A5/*�G���[��*/, 255, 0, 0);
				ShowErrorMessage("���̃o�g���ł� �����邱�Ƃ��ł��Ȃ��I");
				return;
			}

			//�R�}���h����A���̐l�ֈڂ�
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			PhaseCommandSubRoutine.MoveToNextActorCommand(SQActionData(TopIndex[0], -1, Cursor[0], -1, -1));
		}
	},
};
