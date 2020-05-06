//###################################################################
//		エディター・ゲームプログラム間で共通する定数群
//###################################################################

namespace TS {

	namespace System {
		SQ_ENUM enum class DXInterpolation {
			Nearest,
			Bilinear,
		};

		SQ_ENUM enum class BlendMode {
			Alpha,
			Add,
			Subtract,
			Multiply,
			Invsrc,
		};

	}


	namespace DB {
		SQ_ENUM enum class DBDefaultColumnIndices {
			FixedID,
			VisibleID,
			Name,
			Count
		};

		SQ_ENUM enum class DBCurveDataIndices {
			IDName,
			Base,
			Coefficient,
			Index,
			Count,
		};

		SQ_ENUM enum class DBElementIndices {
			Parameter,
			Attribute,
			SkillType,
			ItemType,
			ItemSubType,
			Count
		};

		SQ_ENUM enum class DBValueIndices {
			CommonFlag,
			CommonInteger,
			CommonString,
			SelfActor,
			SelfClass,
			SelfUse,
			SelfState,
			SelfUnit,
			SelfEvent,
			Count
		};

		SQ_ENUM enum class DBInitIndices {
			Party,
			PartyButton,
			Item,
			ItemButton,
			Money,
		};

		SQ_ENUM enum class DBMaterialIndices {
			Face,
			Graphics,
			Sound,
			Font,
			Count
		};

		SQ_ENUM enum class DBUnitEnemyIndices {
			FixedID,
			Position,
		};

		SQ_ENUM enum class DBEffectColumnIndices {
			FrameLength,
			DrawDest,
			Layers,
		};

		SQ_ENUM enum class DBTilesetColumnIndices {
			Comment,
			AutoTiles,
			ObjTiles,
		};

		SQ_ENUM enum class DBEffectViewPosition {
			Each,           //個別
			OnlyOne,        //一括
			CenterScreen,   //画面中心
		};

	}


	namespace DBLiteral {
		SQ_ENUM enum class UseType {
			Script,
			Physic,
			Magic,
			Heal,
			Utility,
		};

		SQ_ENUM enum class SceneLimit {
			AlwaysNG,
			OnlyMap,
			OnlyBattle,
			AlwaysOK,
		};

		SQ_ENUM enum class TargetType {
			None,
			OneParty,
			OnePartyWithDead,
			AllParty,
			AllPartyWithDead,
			OneEnemy,
			AllEnemy,
			Place,
		};

		SQ_ENUM enum class StateLimit {
			None,
			KO,
			ForbiddenAction,
		};

		SQ_ENUM enum class StateSolve {
			None,
			Damamge,
			Turn,
			Walk,
		};

		SQ_ENUM enum class FontDrawing {
			Normal,
			Edge,
			AS,
			EdgeAS,
		};

		SQ_ENUM enum class Addon {
			SelfValuePlus,
			ParameterPlus,
			AttributeResistanceSet,
			StateResistanceSet,
			LockEquipType,
			SetEquipType,
			SetEquipSubType,
			SetSkillUse,
			SetSkillType,
		};

	}


	namespace Map {
		SQ_CONST const int TileColumnCount = 8;       //タイルセットの横に並べるタイル数
		SQ_CONST const int AutoTileRowLength = 16;     //タイルセットに並べられるオートタイルの行数
		SQ_CONST const int MapSizeMaxLength = 500;    //マップの一辺あたりの最大サイズ
		SQ_ENUM enum class Layer {
			Low,
			Middle,
			High,
			Shadow,
			Event,
		};

		SQ_ENUM enum class QuarterTile {
			LeftTop,
			RightTop,
			LeftBottom,
			RightBottom,
			Count
		};

		SQ_ENUM enum class Direction4 {
			South,
			West,
			East,
			North,
			None,
		};

		SQ_ENUM enum class Direction8 {
			SouthWest,
			South,
			SouthEast,
			West,
			None,
			East,
			NorthWest,
			North,
			NorthEast,
		};

		SQ_ENUM enum class Speed {
			VeryLow,
			Low,
			Normal,
			High,
			VeryHigh,
			Moment,
		};

		SQ_ENUM enum class TileDataType {
			Hit,
			InOut,
			Priority,
			GroupNumber,
		};

		SQ_ENUM enum class HitType {
			OK,
			NG,
			Depend,
		};

		SQ_ENUM enum class InOutType {
			None = 0,
			OKBottom = 2,
			OKLeft = 4,
			OKRight = 8,
			OKTop = 16,
		};

		SQ_ENUM enum class TileDrawPriority {
			Normal,
			SemiTransparent,    //下半身が半透明になる
			SemiTopMost,        //下半身が隠れる
			TopMost,            //最前面表示
		};

		SQ_ENUM enum class AutoTilePattern {
			Isolate,
			Vertical,
			Horizontal,
			FourSide,
			AllSide,
		};

		SQ_ENUM enum class MapLoopType {
			None,
			Vertical,
			Horizontal,
			Both,
		};

		SQ_ENUM enum class ScrollType {
			None,
			FollowChar,
			LeftDown,
			Down,
			RightDown,
			Left,
			Right,
			LeftUp,
			Up,
			RightUp,
		};

		SQ_ENUM enum class EventScriptType {
			Page,           //ページ条件
			Trigger,        //起動条件
			Move,           //日常移動
			Run,            //実行内容
		};

		SQ_ENUM enum class EventTriggerType {
			None,
			PressEnterKey,
			Push,
			Pushed,
			Auto,
			AfterMoveMap,
			Parallel,
		};

		SQ_ENUM enum class EventMoveType {
			None,
			Random,
			Near,
			Away,
			Script,
		};

		SQ_ENUM enum class EventDrawPriority {
			Low,
			Middle,
			High,
		};

		SQ_ENUM enum class EventAnimation {
			Move1,
			Stop,
			Move2,
			Count,
		};

		SQ_ENUM enum class EventMoveTask {
			Direction,          //向き変更
			Forward,            //前へ１歩進む
			Backward,           //後ろへ１歩下がる
			Move,               //指定した方向へ１歩進む
			MoveRandom,         //ランダムな方向へ１歩進む
			MoveToPlayer,       //主人公に１歩近づく
			MoveAgainstPlayer,  //主人公から１歩遠ざかる
			TurnLeft,           //左を向く
			TurnRight,          //右を向く
			Turn180,            //反対側を向く
			TurnRandom,         //ランダムな方向を向く
			TurnToPlayer,       //主人公を向く
			TurnAgainstPlayer,  //主人公の反対を向く
			Wait,               //ウェイトＮミリ秒
			SetMoveSpeed,       //移動速度変更
			SetMoveFrequency,   //移動頻度変更
			EnabledWalkAnim,    //歩行アニメーション
			EnabledStopAnim,    //足踏みアニメーション
			NoHit,              //すり抜け
			FixedDirection,     //向き固定
			Hide,               //非表示
			ChangeGraphics,     //グラフィック変更
			SQ,                 //外部スクリプト呼び出し
			LockRoute,          //ルート固定（移動できない場合、スキップせずに待つようにする）
			Repeat,             //以上繰り返し
		};

	}


}
