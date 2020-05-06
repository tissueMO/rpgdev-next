//###################################################################
//			数値定数定義
//###################################################################

namespace TS {

	//####################  システム全般  ####################
	namespace System {
		SQ_CONST const int FixedFPS = 60;								//固定フレームレート
		SQ_NOBIND const int DebugLogCountMax = 999;						//ログの表示限界数
		SQ_NOBIND const int BufferLengthMax = 1024 * 2;					//可変長引数で対応する文字数
		SQ_NOBIND const int DXBlendRange = 64;							//ブレンド画像の境界幅
		SQ_NOBIND const int SQDRecompileKey = KEY_INPUT_F4;				//スクリプトの再コンパイルキー
		SQ_NOBIND const int SQDebugKey = KEY_INPUT_F5;					//スクリプトデバッグのトリガーキー
		SQ_NOBIND const int SQDebugStepOverKey = 0x44;					//スクリプトデバッグのステップオーバーキー (getch)
		SQ_NOBIND const int SQDebugStepInKey = 0x85;					//スクリプトデバッグのステップインキー (getch)
		SQ_NOBIND const int SQDebugStepOutKey = 0x86;					//スクリプトデバッグのステップアウトキー (getch)

		//起動モード
		SQ_ENUM enum class RunMode {
			MainGame,
			MapTest,
			BattleTest,
			SQCompile,
			SQTest,
			DXArchive,
		};

		//終了コード
		SQ_ENUM enum class ExitCode {
			Error = -1,
			Success,
			Question,
			InvalidSystemDB,
		};

		//システムシーン
		SQ_ENUM enum class Scene {
			All = -1,		//全シーン
			Title,			//タイトル画面
			Map,			//マップ画面
			Menu,			//メニュー画面
			Shop,			//お店メニュー画面
			Battle,			//バトル画面
			Count
		};

		//メッセージボックスのアイコン
		SQ_ENUM enum class MessageBoxIcon {
			None = 0,
			Warning = MB_ICONWARNING,
			Information = MB_ICONINFORMATION,
			Error = MB_ICONERROR,
		};

		//メッセージボックスのボタン種別
		SQ_ENUM enum class MessageBoxButton {
			OK = MB_OK,
			OKCancel = MB_OKCANCEL,
			YesNo = MB_YESNO,
			YesNoCancel = MB_YESNOCANCEL,
			RetryCancel = MB_RETRYCANCEL,
			AbortRetryIgnore = MB_ABORTRETRYIGNORE,
		};

		//メッセージボックスの結果
		SQ_ENUM enum class MessageBoxResult {
			OK = IDOK,
			Cancel = IDCANCEL,
			Yes = IDYES,
			No = IDNO,
			Abort = IDABORT,
			Retry = IDRETRY,
			Ignore = IDIGNORE,
		};

		//ソート方法
		SQ_NOBIND enum class SortType {
			ID,					//ID順に並び替える
			Name,				//名前順に並び替える
		};

		//エラーID
		SQ_ENUM enum class ErrorID {
			NotFound,			//存在しない～～です
			Exists,				//既に存在する～～です
			OutOfRange,			//範囲外の～～です
			Except,				//～～は・・・できません
			Invalid,			//無効な～～です
			Failed,				//～～に失敗しました
			WinAPI,				//API系のエラー
			SQ,					//スクリプト系のエラー
		};

		//キーコード
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

		//パッドコード
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

		//マウスボタンコード
		SQ_ENUM enum class MouseID {
			Left = MOUSE_INPUT_LEFT,
			Right = MOUSE_INPUT_RIGHT,
			Middle = MOUSE_INPUT_MIDDLE,
		};
	}


	//####################  スプライト  ####################
	namespace Sprite {
		//テキストの揃える方向
		SQ_ENUM enum class TextAlign {
			Left,
			Center,
			Right,
		};

		//ワイプ状態
		SQ_ENUM enum class WipeState {
			None,
			WipeIn,
			WipeOut,
		};
	}


	//####################  素材管理  ####################
	namespace Material {
		//素材の種別
		SQ_ENUM enum class MaterialType {
			Graphics,
			Sound,
			Font,
			Count
		};

		//####################  素材 - フォント  ####################
		SQ_CONST const int FontNormalSize = 12;				//既定フォントサイズ
		SQ_CONST const int FontNormalThick = 6;				//既定の文字の太さ
		SQ_CONST const int FontNormalLineSpace = 13;
		SQ_CONST const int FontNormalType = DX_FONTTYPE_ANTIALIASING_4X4;

		//####################  素材 - サウンド  ####################
		SQ_CONST const int Common_MinPitch = -240;			//共通ピッチレートの最小値（１オクターブ下）
		SQ_CONST const int Common_MaxPitch = 240;			//共通ピッチレートの最小値（１オクターブ下）
		SQ_CONST const int MIDI_MinVolume = -127;			//MIDIの最小音量
		SQ_CONST const int MIDI_MaxVolume = 0;				//MIDIの最大音量
		SQ_CONST const int MIDI_MinPitch = -8192;			//MIDIの最小ピッチレート
		SQ_CONST const int MIDI_MaxPitch = 8191;			//MIDIの最大ピッチレート
		SQ_CONST const int MinVolume = 0;					//DXライブラリ上における最小音量
		SQ_CONST const int MaxVolume = 255;					//DXライブラリ上における最大音量
		SQ_CONST const int MinPitch = -1200;				//ピッチレートの最小値（１オクターブ下）
		SQ_CONST const int MaxPitch = 1200;					//ピッチレートの最大値（１オクターブ上）
		SQ_CONST const int MinPan = -255;					//パンの最小値（完全左）
		SQ_CONST const int MaxPan = 255;					//パンの最大値（完全右）

		//サウンドの再生状態
		SQ_ENUM enum class PlayStatus {
			Error = -1,				//エラー
			Stop,					//再生されていない
			Playing,				//再生中
		};

		//サウンドの再生方法
		SQ_ENUM enum class PlayType {
			Wait,					//再生が終わるまで待つ
			Back,					//バックグラウンド再生
			Loop,					//バックグラウンド＋ループ再生
		};
	}

	//####################  スクリプト  ####################
	SQ_NOBIND_BEGIN
	namespace SQ {

		//スクリプトエラーの種別
		enum class ErrorType {
			NotExists,			//スクリプトが存在しない
			RuntimeError,		//実行時エラー
		};
	}
	SQ_NOBIND_END

		//####################  データベース  ####################
	namespace DB {
		SQ_NOBIND const int AddonArgLength = 3;				//アドオンの情報数

		//データベースの大種別
		SQ_ENUM enum class DBType {
			FDB,						//固定データベース
			VDB,						//可変データベース
			UDB,						//ユーザーデータベース
			Count
		};

		//列種別
		SQ_ENUM enum class ColumnType {
			None,						//無効
			OneNumber,					//単一値
			Numbers,					//複数値
			String,						//文字列
			Boolean,					//論理値
			IDNames,					//IDリスト
			IDValues,					//IDと値がセット
		};

		//固定データベースの順序
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

		//可変データベースの順序
		SQ_ENUM enum class VDBIndices {
			Value,				//変数・フラグ
			Party,				//パーティ情報
			Actor,				//アクター情報
			Count
		};

		//付加効果の親: そのまま優先順位になる（下にいくほど最新＝有効 かつ 期間短い）
		SQ_ENUM enum class AddonFrom {
			Actor,						//解除されない付加効果
			Class,						//エネミーは無効
			Item,						//エネミーは無効
			State,						//有効度が強いが一時的
			Count
		};

		//スキル習得元
		SQ_NOBIND enum class SkillFrom {
			Actor,						//アクター自身の習得
			Class,						//クラスによる習得
		};
	}

	//####################  マップシーン  ####################
	namespace Map {
		SQ_NOBIND const int EVRandomMoveRange = 5;								//イベントランダム移動の限界範囲
		SQ_NOBIND const int EVMoveFrequencyFrameLatest = 180;					//移動頻度の最も遅い場合の待ち時間フレーム数
		SQ_NOBIND const int SemiTransparent = 128;								//半透明に相当する透明度
		SQ_CONST const int MapLayerCountWithoutEV = static_cast<int>(Layer::Event);			//イベントレイヤーを除くマップレイヤー数

		//速度：実フレーム速度  *インデックス式からの変換方法 -> pow(2.0F, インデックス変数)
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
