//###################################################################
//		メニューシーン
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\!var.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\Common.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuStatus.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuItem.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuEquip.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuSkill.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuMember.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuSave.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubMenu\\MenuOption.nut");

//####################  シーン起動  ####################
function SceneSubSetup() {
	//セーブデータのヘッダー更新
	Game.SaveLoad.GetAllSaveHeader();

	//パーティキャラのグラフィック読み込み
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
		actor.LoadGraphics(actor.Graphics);
	}
}


//####################  シーン描画  ####################
function SceneSubDraw() {
	local offsetX = 30;
	local offsetY = 15;

	Game.DXLib.SetDrawBlendMode(BlendMode.Alpha, 255 - WindowTransparent);
	if(OneMenuMode == false) {
		//サイドメニュー表示
		DrawWindow(0, 0, 200, 235);
		foreach(idx in MenuItem) {
			local lineSpace = 30;
			local text = "";
			switch(idx) {
				case MenuItem.Status:
					text = "ステータス";
					break;
				case MenuItem.Item:
					text = "アイテム";
					break;
				case MenuItem.Equip:
					text = "装備";
					break;
				case MenuItem.Skill:
					text = "スキル";
					break;
				case MenuItem.Member:
					text = "並び替え";
					break;
				case MenuItem.Save:
					text = "セーブ";
					break;
				case MenuItem.Option:
					text = "環境設定";
					break;
			}
			Game.DXLib.DrawString(offsetX, offsetY + lineSpace * idx, 0x1B6F049F/*標準フォント*/, WHITE, text);

			//カーソル
			if(idx == Cursor[0]) {
				DrawCursorPanel(15, 12 + lineSpace * idx, 200 - 15 * 2, 28, (Depth == 0));
			}
		}

		//現在情報
		offsetY = 235;
		DrawWindow(0, offsetY, 200, 155);

		Game.DXLib.DrawString(offsetX - 15, offsetY + 15, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, "現在地:");
		Game.DXLib.DrawString(offsetX, offsetY + 35, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
			Game.SceneMap.MapData.CurrentMapName
		);
		Game.DXLib.DrawString(offsetX - 15, offsetY + 60, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, "プレイ時間:");
		Game.DXLib.DrawString(offsetX, offsetY + 80, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
			GetPlayTime()
		);
		Game.DXLib.DrawString(offsetX - 15, offsetY + 105, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE, "所持金:");
		Game.DXLib.DrawString(offsetX, offsetY + 125, 0x6DD57515/*ウィンドウ内テキスト*/, WHITE,
			format("%7d ", Game.DB.VDBParty.Money) + US1246572451/*通貨名称*/
		);
	}

	DrawWindow(200, 0, Game.WindowSize.Width - 200, Game.WindowSize.Height - 30);	//大枠
	DrawWindow(0, Game.WindowSize.Height - 32, Game.WindowSize.Width, 32);			//説明欄

	//優先処理: 説明欄 or エラーメッセージ/選択肢
	DrawComment(GetMenuComment());

	if(TargetMemberSelecting == true) {
		//優先処理: 対象メンバー選択
		DrawTargetMemberSelect();
		return;
	} else if(PlaceSelecting == true) {
		//優先処理: 場所選択
		DrawPlaceSelect();
		return;
	}

	//階層 0~1 は共通処理
	switch(Depth) {
		case 0:
		case 1:
			switch(Cursor[0]) {
				case MenuItem.Status:
				case MenuItem.Equip:
				case MenuItem.Skill:
				case MenuItem.Member:
					//アクターパネルを描画
					for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
						DrawActorPanel(i, false);
					}
					if(Depth == 1) {
						DrawCursorPanel(200 + 15, 12 + 110 * Cursor[Depth], Game.WindowSize.Width - 200 - 15 * 2, 95, true);
					}
					return;
			}
			break;
	}

	//階層 2~ はメニューごとに処理
	switch(Cursor[0]) {
		case MenuItem.Status:
			DrawCharStatus(true);
			break;
		case MenuItem.Item:
			DrawItemList((Depth == 0) ? 0 : TopIndex[1]);
			break;
		case MenuItem.Equip:
			DrawEquipMenu();
			break;
		case MenuItem.Skill:
			DrawSkillList(TopIndex[2]);
			break;
		case MenuItem.Member:
			DrawMemberMenu();
			break;
		case MenuItem.Save:
			DrawSaveList((Depth == 0) ? 0 : TopIndex[Depth]);
			break;
		case MenuItem.Option:
			DrawOptionMenu();
			break;
	}
}


//####################  シーン入力  ####################
function SceneSubInput() {
	//イベントから呼び出されたメニューではリセットできないようにする
	if(Game.GetKeyInputCounter(KeyID.F12) == 1 && OneMenuMode == false) {
		GameReset();
		return;
	}

	//階層 0~1 は共通処理
	switch(Depth) {
		case 0:
			//トップ画面
			if(CheckUpKeyInput() == true) {
				Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDownKeyInput() == true) {
				Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, MenuItem.len());
			} else if(CheckDecideKeyInput() == 1) {
				//エラーチェック
				switch(Cursor[0]) {
					case MenuItem.Item:
						if(Game.DB.VDBParty.GetItemCountAll() == 0) {
							//アイテムを持っていない場合はエラー出して戻る
							Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
							ShowErrorMessage("アイテムを一つも持っていません。");
							return;
						}
						break;

					case MenuItem.Save:
						if(UF1110905879/*セーブ禁止*/ == true) {
							Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
							ShowErrorMessage("現在セーブが禁止されています。");
							return;
						}
						break;
				}
				Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
				MoveToNextDepth();
			} else if(CheckCancelKeyInput() == 1) {
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
				CloseMenuScene();
			}
			return;

		case 1:
			if(CheckCancelKeyInput() == 1) {
				//キャンセルの優先処理
				Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, 255, 0, 0);
				if(OneMenuMode == false) {
					//トップ階層に戻る
					Depth--;
				} else {
					//単一メニューのときはトップ階層に戻らずメニューを閉じる
					CloseMenuScene();
				}
				return;
			}

			switch(Cursor[0]) {
				case MenuItem.Status:
				case MenuItem.Equip:
				case MenuItem.Skill:
				case MenuItem.Member:
					//パーティメンバー選択
					if(CheckUpKeyInput() == true) {
						Cursor[Depth] = Game.DownAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
					} else if(CheckDownKeyInput() == true) {
						Cursor[Depth] = Game.UpAround(Cursor[Depth], 0, Game.DB.VDBParty.GetCharCount());
					} else if(CheckDecideKeyInput() == 1) {
						//決定
						local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(Cursor[1]));

						//エラーチェック
						switch(Cursor[0]) {
							case MenuItem.Skill:
								if(actor.LearnedSkills.Count() == 0) {
									Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
									ShowErrorMessage("選択されたキャラはスキルを一つも習得していません。");
									return;
								} else if(IsDead(Cursor[1]) == true) {
									Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
									ShowErrorMessage("選択されたキャラは死んでいるため、スキルを使用できません。");
									return;
								}
								break;
						}

						Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
						MoveToNextDepth();
						RefreshEquipTypeList();					//ステータス用: 装備大種別のリストを更新
						RefreshEquipList(EquipTypeList[0]);		//装備用: 装備品リストを絞り込み

						//個別処理
						switch(Cursor[0]) {
							case MenuItem.Member:
								Cursor[Depth] = Cursor[Depth - 1];
								break;
						}
					}
					return;
			}
			break;
	}

	//階層 2~ はメニューごとに処理
	switch(Cursor[0]) {
		case MenuItem.Status:
			InputCharStatus();
			break;
		case MenuItem.Item:
			InputItemMenu();
			break;
		case MenuItem.Equip:
			InputEquipMenu();
			break;
		case MenuItem.Skill:
			InputSkillMenu();
			break;
		case MenuItem.Member:
			InputMemberMenu();
			break;
		case MenuItem.Save:
			InputSaveMenu();
			break;
		case MenuItem.Option:
			InputOptionMenu();
			break;
	}
}


//####################  シーン破棄  ####################
function SceneSubDispose() {
	//パーティキャラのグラフィック破棄
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
		actor.DisposeGraphics();
	}
}


//####################  シーンをフェードイン  ####################
function SceneFadeIn() {
	//フェードイン
	DoFadeObject(
		function() { WindowTransparent = 255; },
		function() { return ((WindowTransparent -= SquareSpeed.Moment) <= 0); }
	);
}
