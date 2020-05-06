//###################################################################
//		�G�f�B�^�[�E�Q�[���v���O�����Ԃŋ��ʂ���萔�Q
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
			Each,           //��
			OnlyOne,        //�ꊇ
			CenterScreen,   //��ʒ��S
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
		SQ_CONST const int TileColumnCount = 8;       //�^�C���Z�b�g�̉��ɕ��ׂ�^�C����
		SQ_CONST const int AutoTileRowLength = 16;     //�^�C���Z�b�g�ɕ��ׂ���I�[�g�^�C���̍s��
		SQ_CONST const int MapSizeMaxLength = 500;    //�}�b�v�̈�ӂ�����̍ő�T�C�Y
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
			SemiTransparent,    //�����g���������ɂȂ�
			SemiTopMost,        //�����g���B���
			TopMost,            //�őO�ʕ\��
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
			Page,           //�y�[�W����
			Trigger,        //�N������
			Move,           //����ړ�
			Run,            //���s���e
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
			Direction,          //�����ύX
			Forward,            //�O�ւP���i��
			Backward,           //���ւP��������
			Move,               //�w�肵�������ւP���i��
			MoveRandom,         //�����_���ȕ����ւP���i��
			MoveToPlayer,       //��l���ɂP���߂Â�
			MoveAgainstPlayer,  //��l������P����������
			TurnLeft,           //��������
			TurnRight,          //�E������
			Turn180,            //���Α�������
			TurnRandom,         //�����_���ȕ���������
			TurnToPlayer,       //��l��������
			TurnAgainstPlayer,  //��l���̔��΂�����
			Wait,               //�E�F�C�g�m�~���b
			SetMoveSpeed,       //�ړ����x�ύX
			SetMoveFrequency,   //�ړ��p�x�ύX
			EnabledWalkAnim,    //���s�A�j���[�V����
			EnabledStopAnim,    //�����݃A�j���[�V����
			NoHit,              //���蔲��
			FixedDirection,     //�����Œ�
			Hide,               //��\��
			ChangeGraphics,     //�O���t�B�b�N�ύX
			SQ,                 //�O���X�N���v�g�Ăяo��
			LockRoute,          //���[�g�Œ�i�ړ��ł��Ȃ��ꍇ�A�X�L�b�v�����ɑ҂悤�ɂ���j
			Repeat,             //�ȏ�J��Ԃ�
		};

	}


}
