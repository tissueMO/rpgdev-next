//###################################################################
//		スクリプト共通関数を定義します。
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\$MTFunctions.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBChar.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBClass.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBEnemy.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBItem.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBSkill.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBState.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\$DBUnit.nut");


//####################  グローバル変数  ####################
WindowMTHandle <- -1;			//メッセージウィンドウのMTハンドル
HPName <- Game.DB.VDBValue.GetSubDB(DBValueIndices.SelfActor).GetCell(0x7BAC0D68/*HP*/, "名前").String;
MPName <- Game.DB.VDBValue.GetSubDB(DBValueIndices.SelfActor).GetCell(0x46A20EE9/*MP*/, "名前").String;
EmotionSpriteIDList <- array(0);		//エモーションバルーンのスプライトリスト


///[node]デバッグ
///[summary]ログ出力して返す
///[comment]値をログに出した上でその値をそのまま返します。
///[return]引数に与えた値
function d(value) {
	print("" + value);
	return value;
}


///[node]デバッグ
///[summary]すべてのアイテムを所持
///[par]それぞれのアイテムの所持数
///[comment]アイテムDBに登録されているすべてのアイテムを所持した状態にします。
function TakeItemsAll(intCount) {
	print("すべてのアイテムを " + intCount + " 個ずつ所持させます。");
	local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	for(local i = 0; i < itemDB.GetRowCount(); i++) {
		Game.DB.VDBParty.AddItem(itemDB.GetCellFromIndex(i, itemDB.GetColumnIndex("FixedID")).Integer, intCount);
	}
}


///[node]システム
///[summary]プレイ時間取得
///[comment]現在のプレイ時間を [HH:MM:SS] の書式で返します。
///[return]書式化された経過時間
function GetPlayTime() {
	local playTimeSecond = (Game.DXLib.GetNowCount() / 1000 - UI1874437940/*起動時の時刻秒*/ + UI248120655/*前回までのプレイ時間*/).tointeger();
	return format("%02d:%02d:%02d",
		playTimeSecond / 60 / 60, 	//時間
		playTimeSecond / 60 % 60, 	//分
		playTimeSecond % 60);		//秒
}


///[node]システム
///[summary]ゲームリセット
///[comment]ゲームをただちにタイトル画面に戻します。イベント実行中やループ処理中など、スクリプトコードが続く可能性がある箇所では絶対に呼び出さないで下さい。
function GameReset() {
	print("ゲームをリセットします。");
	Game.MT.Clear(false);
	Game.DrawBright.Set(255, 255, 255);
	Game.ForbidSceneDraw = false;
	Game.ForbidSceneInput = false;
	Game.Material.Sounds.StopAll(0, false);
	Game.ChangeToTitleScene();
}


///[node]システム
///[summary]マップBGMとBGS一時停止
///[comment]マップ画面上で流れているBGMとBGSを一時停止します。
function PauseMapBGMBGS() {
	if(Game.GetCurrentMainSceneID() != Scene.Map) {
		return;
	}

	if(Game.SceneMap.BGM.Handle != -1) {
		Game.Material.Sounds.Pause(Game.SceneMap.BGM.Handle, 1.0);
	}
	if(Game.SceneMap.BGS.Handle != -1) {
		Game.Material.Sounds.Pause(Game.SceneMap.BGS.Handle, 1.0);
	}
}


///[node]システム
///[summary]マップBGMとBGS演奏再開
///[comment]マップ画面上で流れているBGMとBGSの演奏を再開します。
function ResumeMapBGMBGS() {
	if(Game.GetCurrentMainSceneID() != Scene.Map) {
		return;
	}

	if(Game.SceneMap.BGM.Handle != -1) {
		Game.Material.Sounds.Resume(Game.SceneMap.BGM.Handle, true, 1.0);
	}
	if(Game.SceneMap.BGS.Handle != -1) {
		Game.Material.Sounds.Resume(Game.SceneMap.BGS.Handle, true, 1.0);
	}
}


///[node]システム
///[summary]マップ移動後共通処理
///[comment]マップ移動後の共通処理を行います。
function DoAfterChangeMapProcedure() {
	Game.SceneMap.MapData.AfterLoadMap();

	if(UF24400435/*マップ移動後にフェードインしない*/ == false) {
		Game.FadeIn(SquareSpeed.High);
	}

	UF1358433584/*マップ移動時にフェードアウトしない*/ = false;
	UF24400435/*マップ移動後にフェードインしない*/ = false;
	StepCount = 0;

	//A*法のアルゴリズムを初期化する
	Astar.InitAstar();

	//マップ名のパネルを一定時間表示する
	Game.MT.AddThread("マップ名パネル表示", "MTMapNamePanel", Scene.Map, false)
		.AddValueInteger("starttime", Game.DXLib.GetNowCount());
}


///[node]システム
///[summary]最大HPおよび最大MPを超えないように個別変数を調整
///[par]アクターデータ
function FitToMaxParameters(actor) {
	if (actor == null) {
		return;
	}

	local maxHP = actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value;
	local currentHP = actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value;
	currentHP = Game.WithIn(currentHP, 0, maxHP);
	actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value = currentHP;

	local maxMP = actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value;
	local currentMP = actor.SelfValues.Get(0x46A20EE9/*MP*/).Value;
	currentMP = Game.WithIn(currentMP, 0, maxMP);
	actor.SelfValues.Get(0x46A20EE9/*MP*/).Value = currentMP;
}


///[node]描画
///[summary]ウィンドウ描画
///[comment]指定領域にウィンドウを描画します。
///[par]表示位置X座標
///[par]表示位置Y座標
///[par]ウィンドウ幅
///[par]ウィンドウ高さ
///[return]なし
function DrawWindow(intX, intY, intWidth, intHeight) {
	Game.DXLib.DrawWindow(
		Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*システムグラフィック*/),
		intX, intY, intWidth, intHeight
	);
}


///[node]描画
///[summary]パネル型カーソル描画
///[comment]指定位置にパネル型のカーソルを描画します。
///[par]表示位置X座標
///[par]表示位置Y座標
///[par]幅
///[par]高さ
///[par]アクティブかどうか
///[return]なし
function DrawCursorPanel(intX, intY, intWidth, intHeight, boolActive) {
	//アクティブなカーソルは変化を付ける
	if(boolActive == true) {
		Game.DXLib.SetDrawBlendMode(BlendMode.Add, 255 * fabs(sin(Game.DXLib.GetNowCount() / 250.0)));
	}

	Game.DXLib.DrawCursorPanel(
		Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*システムグラフィック*/),
		intX, intY, intWidth, intHeight
	);

	if(boolActive == true) {
		Game.DXLib.RestoreDrawBlendMode();
	}
}


///[node]描画
///[summary]矢印型カーソル描画
///[comment]指定位置に指定方向の矢印カーソルを描画します。
///[par]表示位置X座標
///[par]表示位置Y座標
///[par]４方向
///[return]なし
function DrawCursorArrow(intX, intY, intDir4, boolActive) {
	if(boolActive == true) {
		//アクティブなカーソルは点滅表示する
		if((Game.DXLib.GetNowCount() / 500 % 2) == 1) {
			return;
		}
	}

	Game.DXLib.DrawCursorArrow(
		Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*システムグラフィック*/),
		intX, intY, intDir4
	);
}


///[node]描画
///[summary]縦スクロールバー描画
///[comment]指定位置に縦スクロールバーを描画します。
///[par]表示位置X座標: ウィンドウX＋幅
///[par]表示位置Y座標: ウィンドウY
///[par]スクロールバー高さ
///[par]スクロール全体: 最大要素数
///[par]スクロール領域: 領域内にある先頭の要素位置
///[par]スクロール領域: 格納できる要素数
///[return]なし
function DrawVScrollBar(intX, intY, intHeight, intMax, intTop, intAreaCount) {
	intX -= 35;		//ウィンドウX+幅 からのオフセット
	if(intMax <= intAreaCount) {
		return;		//スクロールしなくてもよい状況
	}
	intTop = Game.WithIn(intTop, 0, intMax);		//範囲内へ調整
	local nowY = intY + 32 + intTop * (intHeight - 64) / (intMax - intAreaCount);
	local hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x594E551D/*システムグラフィック*/);

	//上下方向にスクロールできることを示すアイコン
	Game.DXLib.DrawRectGraph(hdl, intX, intY + 16, 64 + 16, 16, 32, 8, true, false);
	Game.DXLib.DrawRectGraph(hdl, intX, intY + intHeight - 8 - 16, 64 + 16, 64 - 16 - 8, 32, 8, true, false);

	//スクロールの現在位置を示すアイコン
	Game.DXLib.DrawRectGraph(hdl, intX, nowY - 8, 64 + 16, 16, 32, 8, true, false);
	Game.DXLib.DrawRectGraph(hdl, intX, nowY, 64 + 16, 64 - 16 - 8, 32, 8, true, false);
}


///[node]描画
///[summary]メーター単色描画
///[comment]指定位置に単色のメーターを描画します。
///[par]表示位置X座標
///[par]表示位置Y座標
///[par]幅
///[par]高さ
///[par]パターン番号
///[par]メーターの塗りつぶす幅
function DrawMeterSolid(intX, intY, intWidth, intHeight, intPattern, intEffectWidth) {
	DrawMeterPattern(intX, intY, intWidth, intHeight, intPattern, 0, intEffectWidth);
}


///[node]描画
///[summary]メーター変色描画
///[comment]指定位置に割合に応じて変色するメーターを描画します。
///[par]表示位置X座標
///[par]表示位置Y座標
///[par]幅
///[par]高さ
///[par]パターン開始番号: このパターン番号がメーターの割合が最も大きいときの表示に一致します。
///[par]パターン数
///[par]メーターの塗りつぶす幅
function DrawMeterPattern(intX, intY, intWidth, intHeight, intPatternStart, intPatternCount, intEffectWidth) {
	local hdl = Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x3C2F0B98/*メーターグラフィック*/);
	local MeterPatternOneWidth = Game.DXLib.GetGraphSize(hdl).Width;
	local MeterPatternOneHeight = 100;		//メーターパターングラフィックにおけるメーターあたりの高さ

	//メーターの上限を超えないようにする
	if(intEffectWidth > intWidth) {
		intEffectWidth = intWidth;
	}

	Game.DXLib.DrawRect(intX, intY, intWidth, intHeight, DARKGRAY, true, 1);		//背景
	Game.DXLib.DrawRect(intX, intY, intWidth, intHeight, WHITE, false, 1);			//枠
	Game.DXLib.DrawRectExtendGraph(
		hdl,
		intX + 1,
		intY + 1,
		intEffectWidth - 2,
		intHeight - 2,
		0,
		MeterPatternOneHeight * (intPatternStart + intPatternCount - intPatternCount * intEffectWidth / intWidth),
		MeterPatternOneWidth * intEffectWidth / intWidth,
		MeterPatternOneHeight,
		true
	);
}


///[node]入力
///[summary]押下チェック/決定キー
///[comment]Zキー、Enterキーのどちらかの押下状態値を取得します。
///[return]どちらかのキーの押下状態値
function CheckDecideKeyInput() {
	return ((Game.GetKeyInputCounter(KeyID.Enter) > 0) ? Game.GetKeyInputCounter(KeyID.Enter) :
		((Game.GetKeyInputCounter(KeyID.Z) > 0) ? Game.GetKeyInputCounter(KeyID.Z) : 0));
}


///[node]入力
///[summary]押下チェック/キャンセルキー
///[comment]Xキー、右Ctrlキーのどちらかの押下状態値を取得します。
///[return]どちらかのキーの押下状態値
function CheckCancelKeyInput() {
	return ((Game.GetKeyInputCounter(KeyID.RCtrl) > 0) ? Game.GetKeyInputCounter(KeyID.RCtrl) :
		((Game.GetKeyInputCounter(KeyID.X) > 0) ? Game.GetKeyInputCounter(KeyID.X) : 0)
	);
}


///[node]入力
///[summary]押下チェック/上キー
///[comment]上キーが押されているかどうかを検証します。
///[return]押下されているかどうか
function CheckUpKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Up) == 1
		|| (Game.GetKeyInputCounter(KeyID.Up) > 30 && Game.GetKeyInputCounter(KeyID.Up) % 2 == 0)
	);
}


///[node]入力
///[summary]押下チェック/下キー
///[comment]上キーが押されているかどうかを検証します。
///[return]押下されているかどうか
function CheckDownKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Down) == 1
		|| (Game.GetKeyInputCounter(KeyID.Down) > 30 && Game.GetKeyInputCounter(KeyID.Down) % 2 == 0)
	);
}


///[node]入力
///[summary]押下チェック/右キー
///[comment]上キーが押されているかどうかを検証します。
///[return]押下されているかどうか
function CheckRightKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Right) == 1
		|| (Game.GetKeyInputCounter(KeyID.Right) > 30 && Game.GetKeyInputCounter(KeyID.Right) % 2 == 0)
	);
}


///[node]入力
///[summary]押下チェック/左キー
///[comment]上キーが押されているかどうかを検証します。
///[return]押下されているかどうか
function CheckLeftKeyInput() {
	return (Game.GetKeyInputCounter(KeyID.Left) == 1
		|| (Game.GetKeyInputCounter(KeyID.Left) > 30 && Game.GetKeyInputCounter(KeyID.Left) % 2 == 0)
	);
}


///[node]入力
///[summary]間接フェード
///[par]フェード変数を初期化する関数
///[par]フェード変数を毎フレーム操作する関数。trueを返すまで処理を継続します。
///[comment]指定した関数がtrueを返すまで毎フレーム実行して、間接的なフェードを行います。この関数は必ず入力メソッドの中から呼び出して下さい。
function DoFadeObject(funcFadeInit, funcFadeCount) {
	funcFadeInit();
	while(funcFadeCount() == false && Game.Wait(0, true, false)) {
		//フェード処理終了待ち
	}
}


///[node]イベント
///[summary]選択肢
///[comment]選択肢を選択させます。
///[par]選択肢のリスト。区切り記号は | です。
///[par]キャンセルできるようにするかどうか
///[return]選択された項目のインデックス。キャンセルされた場合は -1 が返されます。
function Choice(strChoiceList, boolCanCancel) {
	local list = split(strChoiceList, "|");
	if(strChoiceList == "") {
		return -1;		//選択できるものがない
	}

	//選択肢を閉じるローカル関数を定義する
	local closeChoice = function(mt, list, boolCanceled) {
		local result = mt.GetValueInteger("Cursor");
		print(mt.GetValueInteger("Cursor") + ": " + (boolCanceled == true ? "選択肢がキャンセルされました。" : "[" + list[mt.GetValueInteger("Cursor")] + "] が選択されました。"));
		mt.SetValueInteger("phase", 2);
		CloseMessage();		//メッセージウィンドウも同時に閉じる
		return (boolCanceled == true) ? -1 : result;
	}

	//MT登録
	local mt = Game.MT.AddThread("選択肢", "MTChoice", -1, false);
	mt.AddValueInteger("phase", 0);
	mt.AddValueInteger("trans", 0);
	mt.AddValueInteger("Cursor", 0);
	mt.AddValueString("msg", strChoiceList);

	//フェードイン
	while(mt.GetValueInteger("phase") != 1 && Game.Wait(0, true, false)) {
		//フェード処理終了待ち
	}

	//メインルーチン
	while(Game.Wait(0, true, true)) {
		//入力状態をチェック
		if(CheckUpKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.DownAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDownKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.UpAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDecideKeyInput() == 1) {
			//決定
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);
			return closeChoice(mt, list, false);
		} else if(CheckCancelKeyInput() == 1 && boolCanCancel == true) {
			//キャンセル: 可能な場合のみ
			Game.DB.FDBMaterial.Play(0x79C24514/*キャンセル音*/, -1, 0, 0);
			mt.SetValueInteger("Cursor", -1);
			return closeChoice(mt, list, true);
		}
	}
}


///[node]イベント
///[summary]選択肢/タイマー付き
///[comment]選択肢を選択させます。指定時間が経過した場合、選択しなかったことになります。
///[par]選択肢のリスト。区切り記号は | です。
///[par]制限時間/ミリ秒
///[return]選択された項目のインデックス。選択しなかった場合は -1 が返されます。
function ChoiceWithTimer(strChoiceList, intWaitTimeMS) {
	local list = split(strChoiceList, "|");
	if(strChoiceList == "") {
		return -1;		//選択できるものがない
	}

	//選択肢を閉じるローカル関数を定義する
	local closeChoice = function(mt, list, boolCanceled) {
		local result = mt.GetValueInteger("Cursor");
		print(mt.GetValueInteger("Cursor") + ": " + (boolCanceled == true ? "選択肢がキャンセルされました。" : "[" + list[mt.GetValueInteger("Cursor")] + "] が選択されました。"));
		mt.SetValueInteger("phase", 2);
		CloseMessage();		//メッセージウィンドウも同時に閉じる
		return (boolCanceled == true) ? -1 : result;
	}

	//MT登録
	local mt = Game.MT.AddThread("選択肢", "MTChoice", -1, false);
	mt.AddValueInteger("phase", 0);
	mt.AddValueInteger("trans", 0);
	mt.AddValueInteger("Cursor", 0);
	mt.AddValueString("msg", strChoiceList);

	//フェードイン
	while(mt.GetValueInteger("phase") != 1 && Game.Wait(0, true, false)) {
		//フェード処理終了待ち
	}

	//メインルーチン
	local startTime = Game.DXLib.GetNowCount();
	while(Game.Wait(0, true, true)) {
		//入力状態をチェック
		if(Game.DXLib.GetNowCount() - startTime >= intWaitTimeMS) {
			//タイムアップ
			mt.SetValueInteger("Cursor", -1);
			return closeChoice(mt, list, true);
		} else if(CheckUpKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.DownAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDownKeyInput() == true) {
			mt.SetValueInteger("Cursor", Game.UpAround(mt.GetValueInteger("Cursor"), 0, list.len()));
		} else if(CheckDecideKeyInput() == 1) {
			//決定
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, -1, 0, 0);
			return closeChoice(mt, list, false);
		}
	}
}


///[node]イベント
///[summary]メッセージ
///[comment]標準のメッセージ表示の本体メソッドです。表示中にリセットボタンが押されても対応できません。
///[par]フェイスID (指定しない場合は -1)
///[par]話者名
///[par]メッセージ内容
///[return]なし
function ShowMessage(intFaceID, strSpeaker, strMsg) {
	//表示速度がマイナスのときはデバッグコンソールへの出力にする
	local msgSpeed = UI2084302691/*メッセージ表示速度*/;
	if(msgSpeed < 0) {
		//改行は消して、改頁を改行にする
		strMsg = Game.Replace(Game.Replace(strMsg, "\n", ""), "\f", "\n");
		print(strSpeeker + ":\n" + strMsg + "\n");
		return;
	}

	local withFace = (intFaceID <= -1) ? false : true;
	local rightFace = (intFaceID <= -1) ? false : Game.DB.FDBMaterial.GetSubDB(DBMaterialIndices.Face).GetCell(intFaceID, "反対側").Boolean;

	//既にウィンドウを表示している場合は閉じる
	if(Game.MT.CountThread("MTMessage") > 0) {
		CloseMessage();
	}

	//MT登録
	local mt = Game.MT.AddThread("メッセージ", "MTMessage", -1, false);
	mt.AddValueInteger("isWaiting", FALSE);
	mt.AddValueInteger("phase", 0);
	mt.AddValueInteger("trans", 0);
	mt.AddValueInteger("faceID", intFaceID);
	mt.AddValueInteger("isRightFace", rightFace ? TRUE : FALSE);
	mt.AddValueInteger("offset", MsgTextOffset);
	mt.AddValueInteger("speakerOffset", 2);
	mt.AddValueString("speaker", strSpeaker);
	mt.AddValueString("msg", "");
	WindowMTHandle = mt.Handle;

	//フェードイン
	while(mt.GetValueInteger("phase") != 1 && Game.Wait(0, true, false)) {
		//フェード処理終了待ち
	}

	//パラメーター初期化
	local showingLength = -1;		//表示位置累計
	local point = 0;				//横にどれだけ進んでいるか
	local lineIndex = 0;			//縦にどれだけ進んでいるか
	local maxLine = withFace ? 4 : 3;		//１頁当たりの最大行数
	local maxPoint = withFace ? 17: 25;		//１行当たりの最大文字数
	local newPageCnt = 0;					//改頁で上げる行数

	local showEnd = false;			//最後まで表示したかどうか
	local newPaging = false;		//改頁中であるかどうか
	local newPageStart = false;		//改頁時の既存テキスト持ち上げ処理実行フラグ
	local newPageReserve = false;	//改頁予約 (改頁中の改頁用、次のループで文字を進めないようにするフラグ)

	local drawChar = "";			//現在の行で描画している部分の文字列
	local oneChar = "";				//次に描画する文字列

	local code = "";				//スクリプトコード部分
	local inCode = false;			//スクリプトコードを取り出しているかどうか

	local lines = array(maxLine, "");		//行ごとのメッセージ内容

	//行ごとのメッセージ内容を実際の表示テキストに反映するローカル関数
	local applyDrawText = function(mt, lines) {
		mt.SetValueString("msg", "");
		foreach(idx, line in lines) {
			mt.SetValueString("msg", mt.GetValueString("msg") + line);
			if(idx < line.len() - 1) {
				mt.SetValueString("msg", mt.GetValueString("msg") + "\n");
			}
		}
	}

	//末尾の文字を削除するローカル関数: 末尾の制御文字を削除するのに使う
	local cutLastChar = function(text) {
		return Game.SubString(text, 0, Game.GetStringLength(text) - 1);
	}

	//メッセージ表示ルーチン: ウェイト処理が強制中断されたときは処理全体を即座に中断する
	while(true) {
		//改頁中: 既存テキスト持ち上げ
		if(newPageStart == true) {
			mt.SetValueInteger("offset", mt.GetValueInteger("offset") - 5);
			if(mt.GetValueInteger("offset") > -27/*描画定数*/ + MsgTextOffset) {
				if(Game.Wait(0, true, false) == false) {
					return;
				}
				continue;
			} else {
				//一行分持ち上げたら再開する
				newPageCnt--;
				mt.SetValueInteger("offset", MsgTextOffset);
				newPageStart = false;

				//一行上にテキストを移行し、最後の行を空ける
				for(local line = 0; line < maxLine; line++) {
					if(line == maxLine - 1) {
						lines[line] = "";
					} else {
						lines[line] = lines[line + 1];
					}
				}

				if(newPageCnt < 0) {
					//改頁完了
					newPageCnt = 0;
					newPaging = false;
				}
				applyDrawText(mt, lines);		//表示テキストに反映
				if(Game.Wait(0, true, false) == false) {
					return;
				}
			}
		}

		//進行中: 次の文字へ進む
		if(showEnd == false) {
			//改頁が予約されていなければ進む
			if(newPageReserve == false) {
				showingLength++;
				point++;
				drawChar += Game.SubString(strMsg, showingLength, 1);
				if(drawChar != "") {
					//検証用の文字列としてセットする
					oneChar = Game.SubString(drawChar, Game.GetStringLength(drawChar) - 1, 1);
				} else {
					oneChar = "";
				}
			}

			//制御文字かどうかを検証する
			if(oneChar == "{") {
				//コード開始点
				drawChar = cutLastChar(drawChar);		//末尾の制御文字を消す
				code = "return (";
				inCode = true;
				continue;
			} else if(oneChar == "}" && inCode == true) {
				//コード終端
				code += ")";
				drawChar = cutLastChar(drawChar);		//末尾の制御文字を消す
				inCode = false;

				//コードを実行し、得られた文字列を現在位置に挿入する
				local temp = compilestring(code)().tostring();
				strMsg = Game.Insert(strMsg, showingLength + 1, temp);
			} else if(inCode == true) {
				//コード読み取り
				drawChar = cutLastChar(drawChar);		//末尾の制御文字を消す
				code += oneChar;
				continue;
			} else if(oneChar == "\n" && lineIndex < maxLine - 1) {
				//最後の行を除く改行記号
				drawChar = cutLastChar(drawChar);		//末尾の制御文字を消す
				lineIndex++;
				point = 0;
				drawChar = "";
				newPageReserve = false;		//もう一度同じ改行記号を読まない
			} else if(oneChar == "\f" || (oneChar == "\n" && lineIndex >= maxLine -1)) {
				//改頁（最後の行での改行も含む）
				drawChar = cutLastChar(drawChar);		//末尾の制御文字を消す
				if(msgSpeed == 0) {
					//瞬間表示の場合はここで描画する
					if(Game.Wait(0, true, false) == false) {
						return;
					}
				}
				point = 0;
				drawChar = "";
				if(newPaging == false) {
					//改頁を行っていない場合: ユーザー入力を待つ
					mt.SetValueInteger("isWaiting", TRUE);
					Game.WaitKey(true);
					mt.SetValueInteger("isWaiting", FALSE);
					newPaging = true;
					newPageCnt = lineIndex;
					newPageReserve = false;

					if(lineIndex >= maxLine - 1) {
						//最後の行にいる場合: 先に頁送りが必要
						newPageStart = true;
						mt.SetValueInteger("offset", MsgTextOffset);
					} else {
						//最後の行がまだ空いている場合: 先に続きの行を進める
						lineIndex++;
					}
				} else {
					//改頁中に改頁しようとしたとき
					newPageReserve = true;		//後でもう一度ここに帰ってくるようにする
					newPageStart = true;
					lineIndex--;
					continue;
				}
			} else if(oneChar == "\a") {
				//中断記号: ウィンドウを閉じないで処理を中断する
				if(newPaging == true) {
					//改頁が終わるまで待つ
					newPageReserve = true;		//後でもう一度ここに帰ってくるようにする
					newPageStart = true;
					continue;
				}
				Game.Wait(0, true, false);
				return;
			} else {
				//一般文字: そのまま表示
				lines[lineIndex] = drawChar;
				applyDrawText(mt, lines);		//表示テキストに反映
			}
		} else if(newPaging == true) {
			//表示が終わっていて、改頁がまだ途中である場合
			newPageStart = true;
		} else {
			//表示完了: ユーザー入力を待つフェーズ
			mt.SetValueInteger("isWaiting", FALSE);		//最後は改頁予告記号が出ないようにする
			Game.WaitKey(true);
			CloseMessage();
			return;
		}

		if(showingLength + 1 >= Game.GetStringLength(strMsg)) {
			//表示完了を判定
			showEnd = true;
			if(msgSpeed == 0) {
				//瞬間表示の場合はここで描画する
				if(Game.Wait(0, true, false) == false) {
					return;
				}
			}
		}

		//実際の描画処理、表示速度の調整
		if(msgSpeed > 0) {
			//表示速度は 1 増えるごとに 1フレーム分ずつ待機時間が増える
			if(Game.Wait(0.01 * msgSpeed, true, false) == false) {
				return;
			}
		}
	}
}


///[node]イベント
///[summary]メッセージ/声付き
///[comment]標準のメッセージ表示にボイスを乗せます。
///[par]フェイスID (指定しない場合は -1)
///[par]ユーザーDB[ボイス]上のID
///[par]話者名
///[par]メッセージ内容
///[return]なし
function ShowVoiceMessage(intFaceID, intVoiceID, strSpeaker, strMsg) {
	//ボイスIDからサウンドファイル名を探す
	local fileName = Game.DB.UDB.GetUserDB(0x49BEBA9A/*ボイス*/).GetCell(intVoiceID, "ファイル名").String;
	local hdl = Game.Material.Sounds.Add(fileName);
	Game.Material.Sounds.PlayOnce(hdl, PlayType.Back, -1, 0, 0);
	ShowMessage(intFaceID, strSpeaker, strMsg);
	Game.Material.Sounds.Delete(hdl);
}


///[node]イベント
///[summary]メッセージを閉じる
///[comment]現在表示しているメッセージウィンドウを閉じます。
///[return]なし
function CloseMessage() {
	if(Game.MT.CountThread("メッセージ") == 0) {
		//ウィンドウが表示されていない場合は処理しない
		WindowMTHandle = -1;
		return;
	} else if(WindowMTHandle == -1) {
		//ウィンドウが表示されているがハンドルを見失っている場合は即消し
		Game.MT.DeleteThread("メッセージ");
		return;
	}

	local mt = Game.MT.GetThread(WindowMTHandle);

	//フェードアウト
	mt.SetValueInteger("phase", 2);
	while(Game.MT.CountThread("メッセージ") > 0 && Game.Wait(0, true, false)) {
		//フェード処理終了待ち
	}
	WindowMTHandle = -1;
}


///[node]イベント
///[summary]固定式テロップ
///[comment]画面の中央にテロップを表示します。
///[par]表示時間ミリ秒
///[par]キー入力で中断できるかどうか
///[par]背景を付けるかどうか (黒画面に表示する場合は false 推奨)
///[par]表示テロップ
///[return]なし
function ShowFixedSubtitles(intTimeMS, boolKeyBreak, boolEnabledBack, strMsg) {
	if(strMsg == "") {
		return;		//表示できるテキストがない
	}

	//MT登録
	Game.MT.AddThread("固定式テロップ", "MTFixedSubtitles", -1, false)
		.AddValueInteger("phase", 0)
		.AddValueInteger("trans", 0)
		.AddValueInteger("timeMS", intTimeMS)
		.AddValueInteger("canKeyBreak", boolKeyBreak.tointeger())
		.AddValueInteger("enabledBack", boolEnabledBack.tointeger())
		.AddValueInteger("starttime", -1)
		.AddValueString("msg", strMsg);

	//テロップ表示が終わるまで待つ
	while(Game.MT.CountThread("固定式テロップ") > 0 && Game.Wait(0, true, true)) {
		//処理終了待ち
	}
}


///[node]イベント
///[summary]流れるテロップ
///[comment]下から上に流れるテロップを表示します。
///[par]移動速度
///[par]決定キーでスピードアップできるようにするかどうか
///[par]テロップ内容
///[return]なし
function ShowFlowSubtitles(intFlowSpeed, boolCanSpeedUp, strMsg) {
	if(strMsg == "") {
		return;		//表示できるテキストがない
	}

	//パラメーター初期化
	local LineHeight = 25;
	local allLineHeight = LineHeight;
	local list = list(strMsg, "\n");
	for(local line = 0; line < list.len(); line++) {
		allLineHeight += LineHeight;
	}

	//MT登録
	Game.MT.AddThread("流れるテロップ", "MTFlowSubtitles", -1, false)
		.AddValueInteger("canSpeedUp", boolCanSpeedUp.tointeger())
		.AddValueInteger("y", Game.WindowSize.Height + LineHeight)
		.AddValueInteger("allLineHeight", allLineHeight)
		.AddValueInteger("speed", intFlowSpeed)
		.AddValueString("msg", strMsg);

	//テロップ表示が終わるまで待つ
	while(Game.MT.CountThread("流れるテロップ") > 0 && Game.Wait(0, true, true)) {
		//処理終了待ち
	}
}


///[node]イベント
///[summary]エモーションバルーン
///[comment]フキダシアイコンを表示します。引数の設定に注意して下さい。
///[par]対象イベントID (プレイヤーは -1)
///[par]アイコン番号X
///[par]アイコン番号Y
///[par]表示を待つかどうか
///[par]表示を待つ場合のみ: 表示秒数
///[par]表示を待つ場合のみ: すべてのフキダシを消去するかどうか
function ShowEmotionBalloon(intEVID, intX, intY, boolWaitMode, intTime, boolDeleteAll) {
	local tileSize = Game.SceneMap.MapData.TileSize;

	//対象イベントの画面上の位置を取得する
	local pos = null;
	if(intEVID == -1) {
		pos = Game.SceneMap.MapData.Player.MapObject.PositionOnPixel;
	} else {
		pos = Game.SceneMap.MapData.EVGetData(intEVID).PositionOnPixel;
	}

	//対象イベントの真上にスプライトをセットする
	local spid = Game.NextRand(INT_MIN, -2);
	Game.Sprite.AddLoadedGraphics(spid, Game.DB.FDBMaterial.GetHandle(DBMaterialIndices.Graphics, 0x63E6D287/*エモーションアイコン*/), pos.X, pos.Y - tileSize);
	local sp = Game.Sprite.Get(spid);
	sp.GetGraphics(0).VisibleRange.Set(intX * tileSize, intY * tileSize, tileSize, tileSize);
	sp.WipeIn(SpriteWipeType.SlideDown, 20);
	EmotionSpriteIDList.append(spid);

	//表示を待つ場合
	if(boolWaitMode == true) {
		Game.Wait(0.5, true, false);
		Game.Wait(intTime, true, false);

		if(boolDeleteAll == true) {
			//表示されているすべてのフキダシを消去する場合
			while(EmotionSpriteIDList.len() > 0) {
				local id = EmotionSpriteIDList.pop();
				Game.Sprite.Delete(id);
			}
		} else {
			//今出したフキダシを消去する
			EmotionSpriteIDList.pop();
			Game.Sprite.Delete(spid);
		}
	}
}


///[node]イベント
///[summary]宿屋処理
///[comment]宿屋のサービスを行います。
///[par]客単価
///[par]メッセージのフェイスID (指定しない場合は -1)
///[par]メッセージの話者名
///[par]オープニングトーク (料金を $ と置くこと)
///[par]泊まる前のメッセージ
///[par]泊まった後のメッセージ
///[par]泊まらなかった場合のメッセージ
///[return]-1=お金が足りず泊まれなかった, 0=泊まらなかった, 1=泊まった
function Inn(intPricePerPerson, intFaceID, strSpeaker, strOpening, strBefore, strAfter, strNotStay) {
	local charge = intPricePerPerson * Game.DB.VDBParty.GetCharCount();
	strOpening = Game.Replace(strOpening, "$", charge.tostring());
	local result = 0;
	UF1222958890/*所持金表示*/ = true;

	ShowMessage(intFaceID, strSpeaker, strOpening);

	switch(Choice("はい|いいえ", true)) {
		case YesNoResult.Yes:
			//金額チェック
			if(Game.DB.VDBParty.Money < charge) {
				//お金が足りない
				ShowMessage(-1, "", "お金が足りません。");
				ShowMessage(intFaceID, strSpeaker, strNotStay);
				result = -1;
				break;
			}
			//泊まる処理
			Game.DB.VDBParty.Money -= charge;
			ShowMessage(intFaceID, strSpeaker, strBefore);
			Game.FadeOut(SquareSpeed.High);
			Game.MT.SetSuspend("マップ名パネル表示", true);
			PauseMapBGMBGS();
			Game.Wait(1.0, true, false)
			Game.DB.FDBMaterial.PlayForWait(0x69C45725/*宿屋*/, 255, 0, 0);
			HealPartyAll();
			ResumeMapBGMBGS();
			Game.FadeIn(SquareSpeed.High);
			Game.MT.SetSuspend("マップ名パネル表示", false);
			ShowMessage(intFaceID, strSpeaker, strAfter);
			result = 1;
			break;

		default:
			//泊まらない
			ShowMessage(intFaceID, strSpeaker, strNotStay);
			result = 0;
			break;
	}

	UF1222958890/*所持金表示*/ = false;

	return result;
}


///[node]メニュー
///[summary]メニュー画面を開く
///[comment]メニュー画面を呼び出します。処理が終わるまで待機します。
function StartMenu() {
	//シーンから抜けるまで待つ
	Game.StartSubScene(Scene.Menu, true, false);
	SceneFadeIn();
	while(Game.CheckCurrentSceneID(Scene.Menu) == true
	&& Game.Wait(0, true, true)) {
		SceneSubInput();
	}
}


///[node]メニュー
///[summary]セーブメニューを開く
///[comment]セーブせずに閉じることもできます。処理が終わるまで待機します。
function ShowSaveMenu() {
	Game.StartSubScene(Scene.Menu, true, false);
	OneMenuMode = true;
	Depth = 1;
	Cursor[0] = MenuItem.Save;
	SceneFadeIn();

	//階層がトップに戻されるまで待つ
	while(Depth >= 1
	&& Game.CheckCurrentSceneID(Scene.Menu) == true
	&& Game.Wait(0, true, true)) {
		SceneSubInput();
	}
}


///[node]メニュー
///[summary]階層を深める
function MoveToNextDepth() {
	Depth++;
	Cursor[Depth] = 0;
	TopIndex[Depth] = 0;
}


///[node]ショップ
///[summary]ショップ画面を開く
///[comment]購入・売却ができるお店を呼び出します。処理が終わるまで待機します。
function StartShop() {
	if(Game.SceneMap.ShopData.GetCount() == 0) {
		Game.DB.FDBMaterial.Play(0x293AF6A5/*エラー音*/, 255, 0, 0);
		print("商品が一つもありません。");
		return;
	}

	//シーンから抜けるまで待つ
	Game.StartSubScene(Scene.Shop, true, false);
	while(Game.CheckCurrentSceneID(Scene.Shop) == true
	&& Game.Wait(0, true, true)) {
		SceneSubInput();
	}
}


///[node]バトル
///[summary]戦闘開始
///[comment]バトルシーンに移行します。
///[par]ユニットID
///[par]戦闘背景ファイル名 (既定のものを使用する場合はnull)
///[par]戦闘BGMファイル名 (既定のものを使用する場合はnull)
///[par]逃げられない戦闘かどうか
///[par]敗北しても続行するかどうか
///[return]戦闘結果
function StartBattle(intUnitID, strBack, strBGM, boolDisabledEscape, boolDisabledGameOver) {
	Game.StartSubScene(Scene.Battle, false, false);
	InitBattle(intUnitID, strBack, strBGM, boolDisabledEscape, boolDisabledGameOver);

	//シーンから抜けるまで待つ
	while(Game.CheckCurrentSceneID(Scene.Battle) == true
	&& Game.Wait(0, true, false)) {
		SceneSubInput();
	}

	if(IsBattleTestMode == true) {
		//バトルテスト時は即終了
		return Result;
	}

	if(Result != BattleResult.Lose || boolDisabledGameOver == true) {
		//ゲームオーバー時以外はマップへ復帰
		SetHeadCharGraphics();		//先頭者のグラフィックをセット
		ResumeMapBGMBGS();
		Game.Wait(1.0, true, false);
		Game.FadeIn(SquareSpeed.High);
	}

	return Result;
}


///[node]バトル
///[summary]ゲームオーバーにする
///[comment]ゲームオーバー画面を表示して、タイトルシーンに移行します。
function GoGameOver() {
	local gmSHdl = Game.Material.Sounds.Add(Game.DB.UDB.GetUserDB(0x3F0A52DA/*既定素材リスト*/).GetCell(DefaultMaterialID.GameOverBGM, "ファイル名").String);
	Game.Material.Sounds.Play(gmSHdl, PlayType.Loop, 255, 0, 0, 0);

	//全画面のスプライトを表示する
	Game.Sprite.AddWipeLoadedGraphics(
		0,
		Game.DB.FDBMaterial.GetSubDB(DBMaterialIndices.Graphics).GetCell(0x2EB7A344/*ゲームオーバー背景*/, DB_ColumnNameMaterialHandle).Integer,
		0, 0, SpriteWipeType.Fade, Speed.High
	);
	Game.WaitKey(true);
	Game.FadeOut(SquareSpeed.High);

	//後片付けしてタイトルシーンへ
	Game.EndSubScene();
	Game.Material.Sounds.Delete(gmSHdl);
	Game.Sprite.Delete(0);
	Game.ChangeToTitleScene();
}


///[node]バトル
///[summary]アクター情報取得
///[comment]指定したインデックスのアクター情報を取得します。パーティ・ユニットの両方に対応しています。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
///[return]アクター情報 (CDBActorData)
function GetActorData(idx) {
	if(idx < 0) {
		return null;
	} else if(idx < EnemyIndexOrigin) {
		//パーティから取得
		return Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(idx));
	} else {
		//ユニットから取得
		return Enemies[idx - EnemyIndexOrigin];
	}
}


///[node]バトル
///[summary]死亡判定
///[comment]指定したアクターが死んでいるかどうかを検証します。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
function IsDead(idx) {
	local actor = GetActorData(idx);
	return (
		actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value <= 0
		|| actor.SelfValues.Get(0x5486E688/*逃走したかどうか*/).Value == TRUE
		|| actor.CheckStateLimit(StateLimit.KO) == true
	);
}


///[node]バトル
///[summary]行動不能判定
///[comment]指定したアクターが行動できない状態であるかどうかを検証します。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
function IsForbiddenAction(idx) {
	local actor = GetActorData(idx);
	return (
		IsDead(idx) == true
		|| actor.CheckStateLimit(StateLimit.ForbiddenAction) == true
	);
}


///[node]バトル
///[summary]基本パラメーター増減
///[comment]指定したパラメーターの値をカンスト値を考慮しながら増減します。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
///[par]パラメーターID
///[par]増減値
function PlusActorBaseParameter(idx, parID, val) {
	local data = GetActorData(idx).GetParameter(parID);
	data.Value += val;
	data.Value = Game.WithIn(data.Value, 0, MaxBaseParameter);
}


///[node]バトル
///[summary]拡張パラメーター増減
///[comment]指定したアクター個別変数の値をカンスト値を考慮しながら増減します。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
///[par]拡張パラメーター列挙体メンバー
///[par]増減値
///[return]実際に増減した値
function PlusActorExParameter(idx, exPar, val) {
	local valID = -1;
	local maxVal = MaxExParameter;
	switch(exPar) {
		case ExParameter.HP:
			valID = 0x7BAC0D68/*HP*/;
			maxVal = GetActorData(idx).GetParameter(0x2019EC10/*最大ＨＰ*/).Value;
			break;
		case ExParameter.MP:
			valID = 0x46A20EE9/*MP*/;
			maxVal = GetActorData(idx).GetParameter(0x9A3FB31/*最大ＭＰ*/).Value;
			break;
	}
	local data = GetActorData(idx).SelfValues.Get(valID);
	local oldValue = data.Value;
	data.Value += val;
	data.Value = Game.WithIn(data.Value, 0, maxVal);
	return data.Value - oldValue;
}


///[node]バトル
///[summary]全快/パーティ全体
///[comment]パーティ全員を全回復させます。
function HealPartyAll() {
	for(local i = 0 ; i < Game.DB.VDBParty.GetCharCount(); i++) {
		HealActor(i);
	}
}


///[node]バトル
///[summary]全快/アクター
///[comment]指定したアクターを全回復させます。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
function HealActor(idx) {
	local actor = GetActorData(idx);
	actor.SelfValues.Get(0x7BAC0D68/*HP*/).Value = actor.GetParameter(0x2019EC10/*最大ＨＰ*/).Value;
	actor.SelfValues.Get(0x46A20EE9/*MP*/).Value = actor.GetParameter(0x9A3FB31/*最大ＭＰ*/).Value;
	actor.ClearStateOnlyBad();
}


///[node]バトル
///[summary]HP回復/アクター
///[comment]指定したアクターのHPを回復させます。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
///[par]回復量
///[par]メッセージ表示を行うかどうか
///[return]回復量
function HPHeal(idx, intValue, boolEnabledMsg) {
	//数値表示
	if(Game.GetCurrentSubSceneID() == Scene.Battle && intValue < 0) {
		//ダメージの場合
		HPDamage(idx, -intValue, boolEnabledMsg);
		return;
	}

	//パラメーター処理
	local delta = PlusActorExParameter(idx, ExParameter.HP, intValue);

	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		if(delta != 0) {
			DamageEffectTasks.append(SQDamageEffectData(idx, delta, SQDamageEffectData.DamageEffectType.HPHeal));

			if(boolEnabledMsg == true) {
				//メッセージ表示
				local actor = GetActorData(idx);
				ActionMessage(actor.Name + " の " + HPName + "が " + delta + " 回復した！", 0.5);
			}
		} else {
			//ミス
			DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));

			if(boolEnabledMsg == true) {
				//メッセージ表示
				local actor = GetActorData(idx);
				ActionMessage("しかし なにも 起こらなかった！", 0.5);
			}
		}
	}

	return delta;
}


///[node]バトル
///[summary]HP回復/サイド全体
///[comment]指定したサイドのアクター全員のHPを回復させます。
///[par]行動内容
///[par]対象サイド
///[par]回復量の基本値
///[par]回復量の分散度
///[return]回復量の平均値
function HPHealSideAll(actData, targetSide, intBase, intVariance) {
	local damage = 0;
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		local target = GetActorData(i);
		damage += HPHeal(i, CalcHeal(actor, target, intBase, intVariance), false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//一括メッセージ表示
	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		local headName = GetHeadActorName(targetSide);
		if (damage != 0) {
			ActionMessage(headName + " の " + HPName + "が " + (actors.len() <= 1 ? "" : "平均 ") + damage + " 回復した！", 0.5);
		} else {
			ActionMessage("しかし なにも 起こらなかった！", 0.5);
		}
	}
	return damage;
}


///[node]バトル
///[summary]MP回復/アクター
///[comment]指定したアクターのMPを回復させます。
///[par]アクターのインデックス（ユニットは EnemyIndexOrigin から始まる）
///[par]回復量
///[par]メッセージ表示を行うかどうか
///[return]回復量
function MPHeal(idx, intValue, boolEnabledMsg) {
	//数値表示
	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		if(intValue > 0) {
			DamageEffectTasks.append(SQDamageEffectData(idx, intValue, SQDamageEffectData.DamageEffectType.MPHeal));
		} else if(intValue < 0) {
			//ダメージの場合
			MPDamage(idx, -intValue, boolEnabledMsg);
			return;
		} else {
			//ミス
			DamageEffectTasks.append(SQDamageEffectData(idx, 0, SQDamageEffectData.DamageEffectType.Miss));
		}
	}

	//パラメーター処理
	PlusActorExParameter(idx, ExParameter.MP, intValue);

	//メッセージ表示
	if(Game.GetCurrentSubSceneID() == Scene.Battle && boolEnabledMsg == true) {
		local actor = GetActorData(idx);
		ActionMessage(actor.Name + " の " + MPName + "が " + intValue + " 回復した！", 0.5);
	}
	return intValue;
}


///[node]バトル
///[summary]MP回復/サイド全体
///[comment]指定したサイドのアクター全員のMPを回復させます。
///[par]行動内容
///[par]対象サイド
///[par]回復量の基本値
///[par]回復量の分散度
function MPHealSideAll(actData, targetSide, intBase, intVariance) {
	local damage = 0;
	local actors = array(0);
	local actor = GetActorData(actData.ActorIndex);

	for(local i = GetFirstActorIndex(targetSide); i != -1; i = GetNextActorIndex(i)) {
		local target = GetActorData(i);
		damage += MPHeal(i, CalcHeal(actor, target, intBase, intVariance), false);
		actors.append(i);
	}
	if(actors.len() > 0) {
		damage /= actors.len();
	}

	//一括メッセージ表示
	if(Game.GetCurrentSubSceneID() == Scene.Battle) {
		local headName = GetHeadActorName(targetSide);
		ActionMessage(headName + " の " + MPName + "が " + (actors.len() <= 1 ? "" : "平均 ") + damage + " 回復した！", 0.5);
	}
	return damage;
}
