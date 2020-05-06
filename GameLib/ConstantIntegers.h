//###################################################################
//			���l�萔��`
//###################################################################

namespace TS {

	//####################  �V�X�e���S��  ####################
	namespace System {
		SQ_CONST const int FixedFPS = 60;								//�Œ�t���[�����[�g
		SQ_NOBIND const int DebugLogCountMax = 999;						//���O�̕\�����E��
		SQ_NOBIND const int BufferLengthMax = 1024 * 2;					//�ϒ������őΉ����镶����
		SQ_NOBIND const int DXBlendRange = 64;							//�u�����h�摜�̋��E��
		SQ_NOBIND const int SQDRecompileKey = KEY_INPUT_F4;				//�X�N���v�g�̍ăR���p�C���L�[
		SQ_NOBIND const int SQDebugKey = KEY_INPUT_F5;					//�X�N���v�g�f�o�b�O�̃g���K�[�L�[
		SQ_NOBIND const int SQDebugStepOverKey = 0x44;					//�X�N���v�g�f�o�b�O�̃X�e�b�v�I�[�o�[�L�[ (getch)
		SQ_NOBIND const int SQDebugStepInKey = 0x85;					//�X�N���v�g�f�o�b�O�̃X�e�b�v�C���L�[ (getch)
		SQ_NOBIND const int SQDebugStepOutKey = 0x86;					//�X�N���v�g�f�o�b�O�̃X�e�b�v�A�E�g�L�[ (getch)

		//�N�����[�h
		SQ_ENUM enum class RunMode {
			MainGame,
			MapTest,
			BattleTest,
			SQCompile,
			SQTest,
			DXArchive,
		};

		//�I���R�[�h
		SQ_ENUM enum class ExitCode {
			Error = -1,
			Success,
			Question,
			InvalidSystemDB,
		};

		//�V�X�e���V�[��
		SQ_ENUM enum class Scene {
			All = -1,		//�S�V�[��
			Title,			//�^�C�g�����
			Map,			//�}�b�v���
			Menu,			//���j���[���
			Shop,			//���X���j���[���
			Battle,			//�o�g�����
			Count
		};

		//���b�Z�[�W�{�b�N�X�̃A�C�R��
		SQ_ENUM enum class MessageBoxIcon {
			None = 0,
			Warning = MB_ICONWARNING,
			Information = MB_ICONINFORMATION,
			Error = MB_ICONERROR,
		};

		//���b�Z�[�W�{�b�N�X�̃{�^�����
		SQ_ENUM enum class MessageBoxButton {
			OK = MB_OK,
			OKCancel = MB_OKCANCEL,
			YesNo = MB_YESNO,
			YesNoCancel = MB_YESNOCANCEL,
			RetryCancel = MB_RETRYCANCEL,
			AbortRetryIgnore = MB_ABORTRETRYIGNORE,
		};

		//���b�Z�[�W�{�b�N�X�̌���
		SQ_ENUM enum class MessageBoxResult {
			OK = IDOK,
			Cancel = IDCANCEL,
			Yes = IDYES,
			No = IDNO,
			Abort = IDABORT,
			Retry = IDRETRY,
			Ignore = IDIGNORE,
		};

		//�\�[�g���@
		SQ_NOBIND enum class SortType {
			ID,					//ID���ɕ��ёւ���
			Name,				//���O���ɕ��ёւ���
		};

		//�G���[ID
		SQ_ENUM enum class ErrorID {
			NotFound,			//���݂��Ȃ��`�`�ł�
			Exists,				//���ɑ��݂���`�`�ł�
			OutOfRange,			//�͈͊O�́`�`�ł�
			Except,				//�`�`�́E�E�E�ł��܂���
			Invalid,			//�����ȁ`�`�ł�
			Failed,				//�`�`�Ɏ��s���܂���
			WinAPI,				//API�n�̃G���[
			SQ,					//�X�N���v�g�n�̃G���[
		};

		//�L�[�R�[�h
		SQ_ENUM enum class KeyID {
			Backspace = KEY_INPUT_BACK,
			Tab = KEY_INPUT_TAB,
			Enter = KEY_INPUT_RETURN,

			LShift = KEY_INPUT_LSHIFT,
			RShift = KEY_INPUT_RSHIFT,
			LCtrl = KEY_INPUT_LCONTROL,
			RCtrl = KEY_INPUT_RCONTROL,
			Escape = KEY_INPUT_ESCAPE,
			Space = KEY_INPUT_SPACE,
			PageUp = KEY_INPUT_PGUP,
			PageDown = KEY_INPUT_PGDN,
			End = KEY_INPUT_END,
			Home = KEY_INPUT_HOME,
			Left = KEY_INPUT_LEFT,
			Up = KEY_INPUT_UP,
			Right = KEY_INPUT_RIGHT,
			Down = KEY_INPUT_DOWN,
			Insert = KEY_INPUT_INSERT,
			Delete = KEY_INPUT_DELETE,

			Minus = KEY_INPUT_MINUS,
			Yen = KEY_INPUT_YEN,
			Prevtrack = KEY_INPUT_PREVTRACK,
			Period = KEY_INPUT_PERIOD,
			Slash = KEY_INPUT_SLASH,
			LAlt = KEY_INPUT_LALT,
			RAlt = KEY_INPUT_RALT,
			ScrollLock = KEY_INPUT_SCROLL,
			SemiColon = KEY_INPUT_SEMICOLON,
			Colon = KEY_INPUT_COLON,
			LBracket = KEY_INPUT_LBRACKET,
			RBracket = KEY_INPUT_RBRACKET,
			At = KEY_INPUT_AT,
			BackSlash = KEY_INPUT_BACKSLASH,
			Comma = KEY_INPUT_COMMA,
			Kanji = KEY_INPUT_KANJI,
			Convert = KEY_INPUT_CONVERT,
			NoConvert = KEY_INPUT_NOCONVERT,
			Kana = KEY_INPUT_KANA,
			Apps = KEY_INPUT_APPS,
			CapsLock = KEY_INPUT_CAPSLOCK,
			PrintScreen = KEY_INPUT_SYSRQ,
			PauseBreak = KEY_INPUT_PAUSE,
			LWin = KEY_INPUT_LWIN,
			RWin = KEY_INPUT_RWIN,

			NumLock = KEY_INPUT_NUMLOCK,
			Num0 = KEY_INPUT_NUMPAD0,
			Num1 = KEY_INPUT_NUMPAD1,
			Num2 = KEY_INPUT_NUMPAD2,
			Num3 = KEY_INPUT_NUMPAD3,
			Num4 = KEY_INPUT_NUMPAD4,
			Num5 = KEY_INPUT_NUMPAD5,
			Num6 = KEY_INPUT_NUMPAD6,
			Num7 = KEY_INPUT_NUMPAD7,
			Num8 = KEY_INPUT_NUMPAD8,
			Num9 = KEY_INPUT_NUMPAD9,
			NumMultiply = KEY_INPUT_MULTIPLY,
			NumAdd = KEY_INPUT_ADD,
			NumSubtract = KEY_INPUT_SUBTRACT,
			NumDecimal = KEY_INPUT_DECIMAL,
			NumDivide = KEY_INPUT_DIVIDE,
			NumEnter = KEY_INPUT_NUMPADENTER,

			F1 = KEY_INPUT_F1,
			F2 = KEY_INPUT_F2,
			F3 = KEY_INPUT_F3,
			F4 = KEY_INPUT_F4,
			F5 = KEY_INPUT_F5,
			F6 = KEY_INPUT_F6,
			F7 = KEY_INPUT_F7,
			F8 = KEY_INPUT_F8,
			F9 = KEY_INPUT_F9,
			F10 = KEY_INPUT_F10,
			F11 = KEY_INPUT_F11,
			F12 = KEY_INPUT_F12,

			A = KEY_INPUT_A,
			B = KEY_INPUT_B,
			C = KEY_INPUT_C,
			D = KEY_INPUT_D,
			E = KEY_INPUT_E,
			F = KEY_INPUT_F,
			G = KEY_INPUT_G,
			H = KEY_INPUT_H,
			I = KEY_INPUT_I,
			J = KEY_INPUT_J,
			K = KEY_INPUT_K,
			L = KEY_INPUT_L,
			M = KEY_INPUT_M,
			N = KEY_INPUT_N,
			O = KEY_INPUT_O,
			P = KEY_INPUT_P,
			Q = KEY_INPUT_Q,
			R = KEY_INPUT_R,
			S = KEY_INPUT_S,
			T = KEY_INPUT_T,
			U = KEY_INPUT_U,
			V = KEY_INPUT_V,
			W = KEY_INPUT_W,
			X = KEY_INPUT_X,
			Y = KEY_INPUT_Y,
			Z = KEY_INPUT_Z,

			Key0 = KEY_INPUT_0,
			Key1 = KEY_INPUT_1,
			Key2 = KEY_INPUT_2,
			Key3 = KEY_INPUT_3,
			Key4 = KEY_INPUT_4,
			Key5 = KEY_INPUT_5,
			Key6 = KEY_INPUT_6,
			Key7 = KEY_INPUT_7,
			Key8 = KEY_INPUT_8,
			Key9 = KEY_INPUT_9,
		};

		//�p�b�h�R�[�h
		SQ_ENUM enum class PadID {
			Down = PAD_INPUT_DOWN,
			Left = PAD_INPUT_LEFT,
			Right = PAD_INPUT_RIGHT,
			Up = PAD_INPUT_UP,
			Button1 = PAD_INPUT_1,
			Button2 = PAD_INPUT_2,
			Button3 = PAD_INPUT_3,
			Button4 = PAD_INPUT_4,
			Button5 = PAD_INPUT_5,
			Button6 = PAD_INPUT_6,
			Button7 = PAD_INPUT_7,
			Button8 = PAD_INPUT_8,
			Button9 = PAD_INPUT_9,
			Button10 = PAD_INPUT_10,
		};

		//�}�E�X�{�^���R�[�h
		SQ_ENUM enum class MouseID {
			Left = MOUSE_INPUT_LEFT,
			Right = MOUSE_INPUT_RIGHT,
			Middle = MOUSE_INPUT_MIDDLE,
		};
	}


	//####################  �X�v���C�g  ####################
	namespace Sprite {
		//�e�L�X�g�̑��������
		SQ_ENUM enum class TextAlign {
			Left,
			Center,
			Right,
		};

		//���C�v���
		SQ_ENUM enum class WipeState {
			None,
			WipeIn,
			WipeOut,
		};
	}


	//####################  �f�ފǗ�  ####################
	namespace Material {
		//�f�ނ̎��
		SQ_ENUM enum class MaterialType {
			Graphics,
			Sound,
			Font,
			Count
		};

		//####################  �f�� - �t�H���g  ####################
		SQ_CONST const int FontNormalSize = 12;				//����t�H���g�T�C�Y
		SQ_CONST const int FontNormalThick = 6;				//����̕����̑���
		SQ_CONST const int FontNormalLineSpace = 13;
		SQ_CONST const int FontNormalType = DX_FONTTYPE_ANTIALIASING_4X4;

		//####################  �f�� - �T�E���h  ####################
		SQ_CONST const int Common_MinPitch = -240;			//���ʃs�b�`���[�g�̍ŏ��l�i�P�I�N�^�[�u���j
		SQ_CONST const int Common_MaxPitch = 240;			//���ʃs�b�`���[�g�̍ŏ��l�i�P�I�N�^�[�u���j
		SQ_CONST const int MIDI_MinVolume = -127;			//MIDI�̍ŏ�����
		SQ_CONST const int MIDI_MaxVolume = 0;				//MIDI�̍ő剹��
		SQ_CONST const int MIDI_MinPitch = -8192;			//MIDI�̍ŏ��s�b�`���[�g
		SQ_CONST const int MIDI_MaxPitch = 8191;			//MIDI�̍ő�s�b�`���[�g
		SQ_CONST const int MinVolume = 0;					//DX���C�u������ɂ�����ŏ�����
		SQ_CONST const int MaxVolume = 255;					//DX���C�u������ɂ�����ő剹��
		SQ_CONST const int MinPitch = -1200;				//�s�b�`���[�g�̍ŏ��l�i�P�I�N�^�[�u���j
		SQ_CONST const int MaxPitch = 1200;					//�s�b�`���[�g�̍ő�l�i�P�I�N�^�[�u��j
		SQ_CONST const int MinPan = -255;					//�p���̍ŏ��l�i���S���j
		SQ_CONST const int MaxPan = 255;					//�p���̍ő�l�i���S�E�j

		//�T�E���h�̍Đ����
		SQ_ENUM enum class PlayStatus {
			Error = -1,				//�G���[
			Stop,					//�Đ�����Ă��Ȃ�
			Playing,				//�Đ���
		};

		//�T�E���h�̍Đ����@
		SQ_ENUM enum class PlayType {
			Wait,					//�Đ����I���܂ő҂�
			Back,					//�o�b�N�O���E���h�Đ�
			Loop,					//�o�b�N�O���E���h�{���[�v�Đ�
		};
	}

	//####################  �X�N���v�g  ####################
	SQ_NOBIND_BEGIN
	namespace SQ {

		//�X�N���v�g�G���[�̎��
		enum class ErrorType {
			NotExists,			//�X�N���v�g�����݂��Ȃ�
			RuntimeError,		//���s���G���[
		};
	}
	SQ_NOBIND_END

		//####################  �f�[�^�x�[�X  ####################
	namespace DB {
		SQ_NOBIND const int AddonArgLength = 3;				//�A�h�I���̏��

		//�f�[�^�x�[�X�̑���
		SQ_ENUM enum class DBType {
			FDB,						//�Œ�f�[�^�x�[�X
			VDB,						//�σf�[�^�x�[�X
			UDB,						//���[�U�[�f�[�^�x�[�X
			Count
		};

		//����
		SQ_ENUM enum class ColumnType {
			None,						//����
			OneNumber,					//�P��l
			Numbers,					//�����l
			String,						//������
			Boolean,					//�_���l
			IDNames,					//ID���X�g
			IDValues,					//ID�ƒl���Z�b�g
		};

		//�Œ�f�[�^�x�[�X�̏���
		SQ_ENUM enum class FDBIndices {
			System,
			Material,
			Element,
			Class,
			Skill,
			Item,
			State,
			Char,
			Enemy,
			Unit,
			Effect,
			Tileset,
			Init,
			Count
		};

		//�σf�[�^�x�[�X�̏���
		SQ_ENUM enum class VDBIndices {
			Value,				//�ϐ��E�t���O
			Party,				//�p�[�e�B���
			Actor,				//�A�N�^�[���
			Count
		};

		//�t�����ʂ̐e: ���̂܂ܗD�揇�ʂɂȂ�i���ɂ����قǍŐV���L�� ���� ���ԒZ���j
		SQ_ENUM enum class AddonFrom {
			Actor,						//��������Ȃ��t������
			Class,						//�G�l�~�[�͖���
			Item,						//�G�l�~�[�͖���
			State,						//�L���x���������ꎞ�I
			Count
		};

		//�X�L���K����
		SQ_NOBIND enum class SkillFrom {
			Actor,						//�A�N�^�[���g�̏K��
			Class,						//�N���X�ɂ��K��
		};
	}

	//####################  �}�b�v�V�[��  ####################
	namespace Map {
		SQ_NOBIND const int EVRandomMoveRange = 5;								//�C�x���g�����_���ړ��̌��E�͈�
		SQ_NOBIND const int EVMoveFrequencyFrameLatest = 180;					//�ړ��p�x�̍ł��x���ꍇ�̑҂����ԃt���[����
		SQ_NOBIND const int SemiTransparent = 128;								//�������ɑ������铧���x
		SQ_CONST const int MapLayerCountWithoutEV = static_cast<int>(Layer::Event);			//�C�x���g���C���[�������}�b�v���C���[��

		//���x�F���t���[�����x  *�C���f�b�N�X������̕ϊ����@ -> pow(2.0F, �C���f�b�N�X�ϐ�)
		SQ_ENUM enum class SquareSpeed {
			VeryLow = 1,
			Low = 2,
			Normal = 4,
			High = 8,
			VeryHigh = 16,
			Moment = 32,
		};
	}
}
