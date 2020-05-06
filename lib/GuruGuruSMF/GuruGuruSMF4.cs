// GuruGuruSMF ver.4.0.0 CSユニット

using System;
using System.Runtime.InteropServices;

namespace GuruGuruSmf
{
	/// <summary>
	/// GuruGuruSMF4API
	/// </summary>
	public interface IGuruGuruSmf4Api
	{
		GgsError OpenDevice(int deviceId, IntPtr handle);
		void CloseDevice();
		GgsError AddDls(string Filename);
		GgsError Play(int option, int musicId, int seek, int fadeInTime, int keyShift);
		void Stop(int fadeOutTime);
		void Pause();
		void Restart();
		void SetMasterTempo(float tempo);
		void SetMasterVolume(int volume);
		void SetMasterPitch(int pitch);
		PlayerStatus GetPlayerStatus();
		GgsError AddListFromFile(string fileName, int option, int musicId);
		GgsError AddListFromMemory(byte[] smfBinary, int option, int musicId);
		void DeleteListItem(int musicId);
		void ClearList();
		int[] EnumList();
		GgsError GetSmfInformation(out SmfInformation info, int musicId);
	}
	
	/// <summary>
	/// GuruGuruSMF4 DLL版
	/// </summary>
	public sealed class Ggs4Dll : IGuruGuruSmf4Api
	{
		// デバイスを開く
		public GgsError OpenDevice(int deviceId, IntPtr handle)
		{
			return GGS4OpenDevice(deviceId, handle);
		}
		
		// デバイスを閉じる
		public void CloseDevice()
		{
			GGS4CloseDevice();
		}
		
		// DLSを追加
		public GgsError AddDls(string fileName)
		{
			return GGS4AddDlsW(fileName);
		}

		// 演奏！
		public GgsError Play(int option, int musicId, int seek, int fadeInTime, int keyShift)
		{
			return GGS4Play(option, musicId, seek, fadeInTime, keyShift);
		}
		
		// 演奏停止
		public void Stop(int fadeOutTime)
		{
			GGS4Stop(fadeOutTime);
		}
		
		// 一時停止
		public void Pause()
		{
			GGS4Pause();
		}
		
		// 一時停止解除
		public void Restart()
		{
			GGS4Restart();
		}
		
		// マスターテンポ
		public void SetMasterTempo(float tempo)
		{
			GGS4SetMasterTempo(tempo);
		}
		
		// マスターボリューム
		public void SetMasterVolume(int volume)
		{
			GGS4SetMasterVolume(volume);
		}
		
		// マスターピッチ
		public void SetMasterPitch(int pitch)
		{
			GGS4SetMasterPitch(pitch);
		}
		
		// 演奏情報を取得
		public PlayerStatus GetPlayerStatus()
		{
			PlayerStatus status = new PlayerStatus();
			GGS4GetPlayerStatus(ref status, Marshal.SizeOf(status));
			return status;
		}
		
		// リストにファイルからSMFを追加
		public GgsError AddListFromFile(string fileName, int option, int musicId)
		{
			return GGS4AddListFromFileW(fileName, option, musicId);
		}
		
		// リストにメモリから追加
		public GgsError AddListFromMemory(byte[] smfBinary, int option, int musicId)
		{
			option |= LoadOption.Buffered;	// 強制
			return GGS4AddListFromMemory(smfBinary, smfBinary.Length, option, musicId);
		}
		
		// リスト内のアイテムを1つクリア
		public void DeleteListItem(int musicId)
		{
			GGS4DeleteListItem(musicId);
		}

		// リストをクリア
		public void ClearList()
		{
			GGS4ClearList();
		}

		// リスト内の登録IDを列挙
		public int[] EnumList()
		{
			IntPtr keysPtr = GGS4EnumList();
			int[] keys = new int[2];
			Marshal.Copy(keysPtr, keys, 0, 2);
			// keys = SmfListKeys{ [0] = Count, [1] = *Keys }
			
			int count = keys[0];
			int[] keysArray = new int[count];
			Marshal.Copy((IntPtr)keys[1], keysArray, 0, count);
			
			return keysArray;
		}

		// SMFの情報を取得
		public GgsError GetSmfInformation(out SmfInformation info, int musicId)
		{
			info = new SmfInformation();
			return GGS4GetSmfInformation(ref info, Marshal.SizeOf(info), musicId);
		}
		
		// コンストラクタ(インスタンス化禁止)
		private Ggs4Dll(){}

		// インスタンス置き場
		private static Ggs4Dll instance = new Ggs4Dll();
		// インスタンス取得(singleton)
		public static Ggs4Dll GetInstance()
		{
			return instance;
		}
		
		// DLLエントリー
		[DllImport("GuruGuruSMF4.dll")]
		private static extern GgsError GGS4OpenDevice(int deviceId, IntPtr handle);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4CloseDevice();
		[DllImport("GuruGuruSMF4.dll", CharSet=CharSet.Unicode)]
		private static extern GgsError GGS4AddDlsW(string fileName);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern GgsError GGS4Play(int option, int musicId, int seek, int fadeInTime, int keyShift);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4Stop(int fadeOutTime);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4Pause();
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4Restart();
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4SetMasterTempo(float tempo);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4SetMasterVolume(int volume);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4SetMasterPitch(int pitch);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4GetPlayerStatus(ref PlayerStatus status, int size);
		[DllImport("GuruGuruSMF4.dll", CharSet=CharSet.Unicode)]
		private static extern GgsError GGS4AddListFromFileW(string fileName, int option, int musicId);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern GgsError GGS4AddListFromMemory(byte[] smfBinary, int length, int option, int musicId);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4DeleteListItem(int musicId);
		[DllImport("GuruGuruSMF4.dll")]
		private static extern void GGS4ClearList();
		[DllImport("GuruGuruSMF4.dll")]
		private static extern IntPtr GGS4EnumList();
		[DllImport("GuruGuruSMF4.dll")]
		private static extern GgsError GGS4GetSmfInformation(ref SmfInformation info, int size, int musicId);
		
	}

	public class PlayOption
	{
		public const int Loop = 1;			// ループ再生する
		public const int SkipBeginning = 2;	// 冒頭の空白をスキップ
		public const int SkipEnding = 4;		// 末尾の空白をスキップ
		public const int MasterPitch = 8;		// マスターピッチを有効にする
		public const int Normalize = 16;		// 最大音量を127に引き上げる
		public const int Exclusive = 32;		// エクスクルーシブを送信する
	}
	
	public class LoadOption
	{
		public const int Buffered = 1;		// データを内部に保持するか
	}
	
	public class Device
	{
		public const int DirectMusic = -1100;
	}

	/// <summary>
	/// エラーコード
	/// </summary>
	public enum GgsError
	{
		NoError, 			// エラーなし
		Failed,				// とにかく失敗
		FileNotFound,		// ファイルが存在しない
		CannotOpenFile,		// ファイルが開けない
		BadFileSize,		// ファイルサイズが不正
		NoSmf,				// SMFではありません
		BadSmf,				// 不正なSMF
		UnmatchedSmf,		// 未対応のSMF
		CannotOpenDevice,	// デバイスが開けない
		CannotStartTimer,	// タイマーが開始できない
		IllegalPointer,		// 範囲外を参照しようとした
		TrackLimit,			// メッセージの途中でトラックが終端に達した
		NotReady,			// 演奏可能ではない
		UndefinedId,		// 未定義のID
		ApiNull				// APIが有効でない
	};
	
	/// <summary>
	/// プレイヤーの状態
	/// </summary>
	public enum PlayerState
	{
		Stop,		// 停止中
		Play,		// 演奏中
		Pause		// 一時停止中
	};
	
	[StructLayout(LayoutKind.Sequential)]
	public struct PlayerStatus
	{
		public PlayerState State;
		public int Tempo;
		public int Tick;
		public int Time;
		public int LoopCount;
	}
	
	[StructLayout(LayoutKind.Sequential)]
	public struct SmfInformation
	{
		public int TimeBase;		// ベースカウント
		public int TotalTick;		// 総Tick
		public int TotalTime;		// 総演奏時間
		public int FirstNoteTick;	// 最初の発音tick
		public int FirstNoteTime;	// 最初の発音の時間
		public int LastNoteTick;	// 最後の発音tick
		public int LastNoteTime;	// 最後の発音の時間
		public int LoopTick;		// CC#111の位置
		public int LoopTime;		// CC#111の位置の時間
	}
	
}

