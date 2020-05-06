//###################################################################
//		バトルシーン: グローバル変数
//###################################################################

//メニュー固定
DepthCount <- 13;				//メニュー階層数
TargetPTDepth <- 10;			//対象パーティキャラ選択の階層番号
TargetUTDepth <- 11;			//対象エネミー選択の階層番号
ChoiceDepth <- 12;				//選択肢の階層番号


//メニュー汎用
Cursor <- array(DepthCount, 0);		//各階層のカーソル位置
TopIndex <- array(DepthCount, 0);	//各階層のウィンドウ領域にある先頭要素のインデックス
Depth <- 0;							//メニュー階層
ErrorMsg <- "";						//エラーメッセージ内容
AreaMax <- 8;						//リスト領域に表示できる項目数
WindowTransparent <- 0;				//フェードするウィンドウの透明度


//メニュー独自
TargetPTSelecting <- false;			//対象パーティキャラ選択中かどうか
TargetPTAllowDead <- false;			//対象パーティキャラの選択に死者を含められるかどうか
TargetUTSelecting <- false;			//対象エネミー選択中かどうか


//基本情報
UnitID <- -1;					//ユニットID
UnitSelfValues <- null;			//ユニット個別変数

DisabledEscape <- false;		//逃げられない戦闘か
DisabledGameOver <- false;		//ゲームオーバーを無効にするか

FileBack <- "";					//戦闘背景のファイル名
HdlBack <- -1;					//戦闘背景のハンドル
FileBGM <- "";					//戦闘BGMのファイル名
HdlBGM <- -1;					//戦闘BGMのハンドル

TurnCount <- 1;					//経過ターン数
Phase <- -1;					//現在の戦闘フェーズ
Result <- -1;					//戦闘結果


//描画情報
PartyPanelHeight <- 100;		//パーティパネルの高さ
CommentPanelHeight <- 32;		//メッセージパネルの高さ
LineHeight <- 27;				//コマンドリストのウィンドウ一行分の高さ
CommandPanelWidth <- 120;		//コマンドパネルの幅
MeterHeight <- 8;				//メーターの高さ
OffsetCorner <- 16;				//ウィンドウ内の文字列の左上オフセット


//戦闘フェーズ
BattlePhase <- {
	Setup = 0,			//黒画面・初期化
	Start = 1,			//戦闘開始フェーズ
	Command = 2,		//コマンド選択フェーズ
	Action = 3,			//行動中フェーズ
	Ending = 4,			//戦闘終了フェーズ
};


//コマンド
MenuItem <- {
	Attack = 0,			//攻撃
	Defense = 1,		//防御
	Skill = 2,			//スキル
	Item = 3,			//アイテム
	Escape = 4,			//逃げる
};


//アイテムの選択種別
CommandItemType <- {
	Having = 0,			//所持品
	Equip = 1,			//装備品
};


//戦闘勝利時の階層
EndingDepth <- {
	EXPMoney = 0,		//経験値、お金獲得
	LevelUp = 1,		//レベルアップ
	LearnSkill = 2,		//スキル習得
	DropItem = 3,		//ドロップアイテム
};


//アクターリスト
Enemies <- array(0);			//ユニット側のアクターリスト (CDBActorData)


//コマンド選択フェーズに必要なもの
SkillTypeList <- array(0);		//スキル種別リスト (CIDNameData)
SkillList <- array(0);			//スキルリスト (CIDValueData)

EquipTypeList <- array(0);		//装備大種別のIDリスト
EquipList <- array(0);			//装備大種別で絞り込まれた装備品リスト (CIDValueData)


//行動フェーズに必要なもの
Actions <- array(0);			//行動リスト (SQActionData)
TurnEndingTasks <- array(0);	//ターン終了時の処理リスト (function)
TurnEndingTaskArgs <- array(0);	//ターン終了時の処理の引数 (table)

DamageEffectTasks <- array(0);	//ダメージ数値表示リスト (SQDamageEffectData)
PTPanelShakes <- array(0);		//パーティパネル表示位置微調整用 (SQShakeData)
DamageMotionUT <- array(0);		//エネミーのダメージモーション用 (SQShakeData)

ActionMsg <- "";				//行動メッセージの内容
ActingIndex <- -1;				//行動中のアクターのインデックス

CriticalHitFlag <- false;		//クリティカルヒットが起きたときに立てられるフラグ
PTEscaped <- false;				//パーティが逃走に成功したときに立てられるフラグ


//戦闘結果フェーズに必要なもの
Results <- {
	Money = 0,					//獲得金額
	EXP = 0,					//獲得経験値

	LevelUpName = "",			//レベルアップしたキャラの名前
	BeforeLevel = -1,			//レベルアップする前のレベル
	AfterLevel = -1,			//レベルアップした後のレベル
	LevelUpParameters = null,	//レベルアップによるパラメーター変動リスト (CIDValueData)
	LevelUpSkills = null,		//レベルアップによる習得スキルリスト (CIDNameData)

	DropItems = null,			//ドロップアイテムリスト (CIDNameData)
};
