//###################################################################
//		スクリプト共通定数を定義します。
//###################################################################

//基本システム
const MsgFontID = 0x2ABC7058/*メッセージフォント*/;		//メッセージフォント
const SubtitleFontID = 0x7F6CF4B3/*テロップフォント*/;		//テロップフォント
const MarqueeMinimum = -160;						//NowLoading: マーキー最小値
const MarqueeMaximum = 400;							//NowLoading: マーキー最大値

const IsFixedSizedFace = 0;				//メッセージ: フェイスグラフィックがバストアップではなく、ウィンドウ内に収める形式の場合は 1 にして下さい。
const MsgFaceStandardWidth = 190;		//メッセージ: フェイスグラフィックの想定幅: この分だけメッセージ部分が横にずれます。
const MsgTextOffset = 2;				//メッセージ: テキストの固定オフセット: この分だけメッセージ部分が下にずれます。
const MsgWndHeight = 128;				//メッセージ: メッセージウィンドウの高さ
const MsgNameWidth = 200;				//メッセージ: 名前ウィンドウの幅
const MsgNameHeight = 36;				//メッセージ: 名前ウィンドウの高さ

const ChoiceWndMargin = 100;			//選択肢: ウィンドウの左右余白

const SubtitleLineHeight = 25;			//テロップ: １行あたりの高さ


//デバッグ用
const ItemCountForTester = 5;		//テスター専用: 所持させるアイテム数


//マップ関連
const IntoMenuSEID = 0x61301713/*決定音*/;	//メニュー画面を開くときの音
const NormalEncounterStepCount = 40;		//エンカウント係数=100 のときの目標歩数
const MapNamePanelTime = 5000;				//マップ名表示パネルの表示時間ミリ秒
const MapNameFadeTime = 500;				//マップ名表示パネルのフェード時間ミリ秒
const LadderTileGroupNum = 99;				//梯子属性のタイルグループ番号


//メニュー関連
const EnabledIcons = 1;				//アイテムとスキルにアイコンを表示させるかどうか


//ショップ関連
const SellRate = 0.5;				//売却時の価格落ち率


//バトル関連
const EnemyIndexOrigin = 100;		//ユニット側のアクターインデックスの開始番号
const CriticalDamageRate = 2.0;		//クリティカルヒット時のダメージ倍率
const CriticalRate = 3;				//クリティカルヒット発生率
const SpeedVarianceRate = 20;		//行動順番で考慮される敏捷性の分散度
const EscapeMissRate = 5;			//全員で逃げたときの失敗確率
const EmptyEquipEffectID = 0x2C31DE4D/*素手攻撃*/;		//素手攻撃のエフェクトID
const PluralString = "たち";			//先頭者名に続く複数形


//アクター関連
const MaxBaseParameter = 999;		//基本パラメーター(パラメーターDB)の最大値
const MaxExParameter = 9999;		//拡張パラメーター(個別変数)の最大値
const HPMeterPatternStart = 4;		//HPメーターのパターン開始インデックス
const HPMeterPatternCount = 10;		//HPメーターのパターン数
const MPMeterPattern = 3;			//MPメーターのパターンインデックス
const StateIconMaxCount = 3;		//間接効果アイコンの表示数上限
const StateIconSize = 16;			//間接効果アイコンの実際の表示サイズ
const UseIconSize = 16;				//アイテム・スキルアイコンの実際の表示サイズ


//拡張パラメーター
enum ExParameter {
	HP,
	MP,
};


//イベント拡張移動の種別
enum EVMoveExType {
	Jump,				//ジャンプ
	Oni,				//鬼ごっこ鬼モード
};


//スプライトワイプ種別
enum SpriteWipeType {
	Fade,				//フェード
	SlideRight,			//スライド・右
	SlideLeft,			//スライド・左
	SlideDown,			//スライド・下
	SlideUp,			//スライド・上
	Transition,			//合成画像を利用
};


//バトルイベント種別
enum BattleEventType {
	StartBattle,		//戦闘開始時
	StartTurn,			//ターン開始時
	BeforeAction,		//行動前
	AfterAction,		//行動後
	EndingTurn,			//ターン終了時
	EndingBattleWin,	//戦闘勝利時
	EndingBattleLose,	//戦闘敗北時
	EndingBattleEscape,	//戦闘逃走時
};


//戦闘結果
enum BattleResult  {
	Win,				//勝利
	Lose,				//敗北
	Escape,				//逃走
	Break,				//中断
};


//選択肢結果
enum YesNoResult {
	Yes,				//「はい」
	No,					//「いいえ」
	Cancel = -1,		//「キャンセル」
};


//既定素材DBのID
enum DefaultMaterialID {
	WipeBlendGraph,		//ワイプ用合成画像
	TitleGraph,			//タイトル画面グラフィック
	TitleBGM,			//タイトル画面BGM
	BattleTestGraph,	//バトルテスト用背景グラフィック
	BattleBGM,			//通常戦闘BGM
	BattleStartWipeGraph,		//戦闘開始時のワイプ画像
	BattleEndingWipeGraph,		//戦闘勝利時のワイプ画像
	GameOverBGM,		//ゲームオーバーBGM
};


//攻撃サイド
enum AttackSide {
	Party,				//パーティ
	Unit,				//ユニット
};
