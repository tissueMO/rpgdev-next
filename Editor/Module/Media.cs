using DxLibDLL;
using System;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace Editor.Module {
    using Properties;

    public static partial class Media {
        private static DrawTarget Target = null;
        private static DrawTarget NextTarget = null;
        private static readonly Timer DrawTask = new Timer();
        private static bool Setupped = false;
        public static bool StopDraw = false;
        public static GuruGuruSmf.IGuruGuruSmf4Api GGS;

        //GuruGuruSMF用の定数
        public const int GGSMidiHandle = -65535;
        public const int GGSPitchMin = -8192;
        public const int GGSPitchMax = 8191;

        //Luminoライブラリ用の定数
        //public const int Sound3DDistance = 500;
        //public const int SoundVolumeMin = 0;
        //public const int SoundVolumeMax = 100;
        //public const int SoundVolumeDefault = 100;
        //public const int SoundPitchMin = 50;
        //public const int SoundPitchMax = 200;
        //public const int SoundPitchDefault = 100;
        //public const int SoundPanMin = -100;
        //public const int SoundPanMax = +100;
        //public const int SoundPanDefault = 0;

        //DXライブラリ用の定数
        public const int SoundVolumeMin = 0;
        public const int SoundVolumeMax = 255;
        public const int SoundVolumeDefault = 255;
        public const int Common_SoundPitchMin = -240;
        public const int Common_SoundPitchMax = +240;
        public const int DX_PitchMin = -1200;
        public const int DX_PitchMax = 1200;
        public const int SoundPitchDefault = 0;
        public const int SoundPanMin = -255;
        public const int SoundPanMax = +255;
        public const int SoundPanDefault = 0;

        /// <summary>
        /// DXライブラリの描画先のポインタ
        /// </summary>
        public static IntPtr TargetPtr {
            get;
            private set;
        }

        /// <summary>
        /// 合成方法の種類を文字列のリストにして返します。
        /// </summary>
        public static string[] GetBlendModeList() {
            var list = new string[Common.GetEnumCount<Media.BlendMode>()];
            for (var i = 0; i < list.Length; i++) {
                switch (i) {
                    case (int) Media.BlendMode.Alpha:
                        list[i] = "アルファ";
                        break;
                    case (int) Media.BlendMode.Add:
                        list[i] = "加算";
                        break;
                    case (int) Media.BlendMode.Subtract:
                        list[i] = "減算";
                        break;
                    case (int) Media.BlendMode.Multiply:
                        list[i] = "乗算";
                        break;
                    case (int) Media.BlendMode.Invsrc:
                        list[i] = "反転色";
                        break;
                }
            }
            return list;
        }


        /// <summary>
        /// サウンド情報
        /// </summary>
        public class SoundObject {

            /// <summary>
            /// コンストラクター
            /// </summary>
            public SoundObject(string rPath, int vol, int pitch = SoundPitchDefault, int pan = SoundPanDefault) {
                this.FileName = rPath;
                this.SetData(vol, pitch, pan);
            }
            public SoundObject(string textData) {
                this.SetData(textData);
            }

            /// <summary>
            /// 一括で情報を格納します。
            /// </summary>
            public void SetData(int vol, int pitch, int pan) {
                this.Volume = vol;
                this.Pitch = pitch;
                this.Panpot = pan;
            }

            /// <summary>
            /// 一括で情報を格納します。
            /// </summary>
            public void SetData(string textData) {
                var spl = textData.Split(Resources.Split_LongData.ToCharArray());
                for (var i = 0; i < spl.Length; i++) {
                    switch (i) {
                        case (int) DataOrder.FileName:
                            this.FileName = spl[i];
                            break;
                        case (int) DataOrder.Volume:
                            this.Volume = int.Parse(spl[i]);
                            break;
                        case (int) DataOrder.Pitch:
                            this.Pitch = int.Parse(spl[i]);
                            break;
                        case (int) DataOrder.Panpot:
                            this.Panpot = int.Parse(spl[i]);
                            break;
                    }
                }
            }

            /// <summary>
            /// ファイル名 (rPath)
            /// </summary>
            public string FileName {
                get; set;
            } = "";

            /// <summary>
            /// 音量
            /// </summary>
            public int Volume {
                get {
                    return this.volume;
                }
                set {
                    this.volume = Math.Min(Math.Max(value, SoundVolumeMin), SoundVolumeMax);
                }
            }
            private int volume = SoundVolumeDefault;

            /// <summary>
            /// 音程
            /// </summary>
            public int Pitch {
                get {
                    return this.pitch;
                }
                set {
                    this.pitch = Math.Min(Math.Max(value, Common_SoundPitchMin), Common_SoundPitchMax);
                }
            }
            private int pitch = SoundPitchDefault;

            /// <summary>
            /// 定位
            /// </summary>
            public int Panpot {
                get {
                    return this.panpot;
                }
                set {
                    this.panpot = Math.Min(Math.Max(value, SoundPanMin), SoundPanMax);
                }
            }
            private int panpot = SoundPanDefault;

            /// <summary>
            /// サウンド情報を文字列の形式にして返します。
            /// </summary>
            public override string ToString() {
                var buf = "";
                for (var i = 0; i < Common.GetEnumCount<DataOrder>(); i++) {
                    switch (i) {
                        case (int) DataOrder.FileName:
                            buf += this.FileName;
                            break;
                        case (int) DataOrder.Volume:
                            buf += this.Volume;
                            break;
                        case (int) DataOrder.Pitch:
                            buf += this.Pitch;
                            break;
                        case (int) DataOrder.Panpot:
                            buf += this.Panpot;
                            break;
                    }
                    if (i < Common.GetEnumCount<DataOrder>() - 1) {
                        buf += Resources.Split_LongData;
                    }
                }
                return buf;
            }

            /// <summary>
            /// 文字列の形式からサウンド情報を生成して返します。
            /// </summary>
            public static SoundObject Parse(string val) {
                var spl = val.Split(Resources.Split_LongData.ToArray());
                return new SoundObject(
                    spl[(int) DataOrder.FileName],
                    int.Parse(spl[(int) DataOrder.Volume]),
                    int.Parse(spl[(int) DataOrder.Pitch]),
                    int.Parse(spl[(int) DataOrder.Panpot])
                );
            }
        }

        /// <summary>
        /// DXライブラリを初期化します。
        /// 描画領域のサイズは、初期化に使用した宛先コントロールの初期サイズで固定されます。
        /// </summary>
        public static void Init(IntPtr dest) {
            Media.TargetPtr = dest;

            //描画スレッドを停止
            Media.DrawTask.Enabled = false;

            //リソース初期化
            Media.ClearResources();

            if (DX.DxLib_IsInit() == DX.TRUE) {
                //画面をクリア
                DX.SetScreenFlipTargetWindow(dest);     //NOTE: DXライブラリ管理人より「DXLib_Init以降の描画先の切り替えはこの関数のみの実行で良い」とのこと
            } else {
                //無効にする
                DX.SetUseDirectInputFlag(DX.FALSE);

                //有効にする
                DX.SetDoubleStartValidFlag(DX.TRUE);
                DX.SetUseVramFlag(DX.TRUE);
                DX.SetUseIMEFlag(DX.TRUE);              //NOTE: これを FALSE にするとアプリケーション全体でIMEが使用できなくなってしまう
                DX.SetUseTSFFlag(DX.FALSE);             //NOTE: v3.18b よりTSFを使用する変更が入り、アプリケーション内で漢字かな変換すると変換候補が出なくなってしまったためTSFを無効化して回避する
                DX.ChangeWindowMode(DX.TRUE);

                //その他
                DX.ChangeFontType(DX.DX_FONTTYPE_ANTIALIASING);
                DX.SetUserWindow(dest);                 //NOTE: DXライブラリ管理人より「この関数はDXLib_Initの前に一度だけ実行する」とのこと

                //初期化実行
                if (DX.DxLib_Init() == -1) {
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "描画エリアの設定"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Application.Exit();
                    return;     //初期化に失敗
                }
            }

            //NOTE: DXライブラリ管理人より「DXLib_Initの後に実行、描画先を切り替えたらその都度実行する必要がある」とのこと
            DX.SetDrawScreen(DX.DX_SCREEN_BACK);

            //描画スレッドを実行
            if (!Media.Setupped) {
                Media.Setupped = true;
                Media.DrawTask.Tick += Media.Draw;
                Media.DrawTask.Interval = 1;
            }
        }

        /// <summary>
        /// 描画先を変更します。
        /// </summary>
        /// <param name="hdl">描画先のハンドル</param>
        public static void ChangeDest(DrawTarget obj) {
            Media.StopDraw = false;
            Media.Target = null;
            Media.NextTarget = obj;
            if (!Media.Setupped) {
                Media.Init(obj.DrawDest);
            }
            Media.DrawTask.Enabled = true;
        }

        /// <summary>
        /// 描画を行います。
        /// </summary>
        private static void Draw(object sender, EventArgs e) {
            if (Media.NextTarget != null) {
                //シングルスレッドで描画準備を行う
                Media.DrawTask.Enabled = false;
                Media.Target = Media.NextTarget;
                Media.Init(Media.NextTarget.DrawDest);
                Media.Target.LoadMaterial();
                Media.NextTarget = null;
                Media.DrawTask.Enabled = true;
            } else if (Media.Target != null && !Media.StopDraw) {
                Target.SetDXInterpolation();
                if (DX.GetASyncLoadNum() == 0 && Target.TargetControl.Visible) {
                    //読み込みが完了している場合のみ描画
                    Target.ClsScreen();
                    Target.Draw();
                    DX.ScreenFlip();
                } else {
                    //非同期読み込みを続行
                    DX.ProcessMessage();
                }
            }
        }

        /// <summary>
        /// 破棄処理を行います。
        /// </summary>
        public static void Dispose() {
            Media.DrawTask.Enabled = false;
            try {
                DX.DxLib_End();
                GGS.CloseDevice();
            } catch {
                // エラーが発生しても無視する
            }
        }

        /// <summary>
        /// 読み込んだリソースをすべて解放します。
        /// </summary>
        public static void ClearResources() {
            DX.InitGraph();
            DX.InitSoundMem();
            DX.InitFontToHandle();
        }

        /// <summary>
        /// DXライブラリのサウンドハンドルに各種設定をセットします。
        /// </summary>
        public static void SetSoundData(ref int sndHdl, SoundObject snd) {
            SetSoundData(ref sndHdl, snd.FileName, snd.Volume, snd.Pitch, snd.Panpot);
        }
        public static void SetSoundData(ref int sndHdl, string rPath, int volume = Media.SoundVolumeDefault, int pitch = Media.SoundPitchDefault, int pan = Media.SoundPanDefault) {
            if (sndHdl != -1) {
                DX.DeleteSoundMem(sndHdl);
            }
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                //存在しないファイルは処理しない
                return;
            }

            /*NOTE: １週間の努力の成果...
				・Luminoは親機以外のどの環境でも動作しないので絶対に使わない (XAudio2に対応していないからか？)
					・ただし、こちらのライブラリが改良される可能性も考えて、コードは残しておく
			*/

            if (Path.GetExtension(rPath).ToLower() == ".mid") {
                //MIDIはGuruGuruSMFを使う
                sndHdl = Media.GGSMidiHandle;
                GGS.AddListFromFile(ProjectManager.ProjectPath + rPath, 0, sndHdl);
                GGS.SetMasterVolume(volume / 2 - 127);
                GGS.SetMasterPitch(Math.Min(Math.Max((int) (34.134 * pitch), GGSPitchMin), GGSPitchMax));
                //NOTE: MIDIはパン設定に対応しない。
            } else {
                //それ以外はDXライブラリを使う
                DX.SetUseASyncLoadFlag(DX.FALSE);           //音程変えると落ちる問題への対策
                DX.SetCreateSoundPitchRate(Math.Min(Math.Max(5 * pitch, GGSPitchMin), GGSPitchMax));
                sndHdl = DX.LoadSoundMem(ProjectManager.ProjectPath + rPath);
                DX.ChangeVolumeSoundMem(volume, sndHdl);
                DX.ChangePanSoundMem(pan, sndHdl);
                DX.SetCreateSoundPitchRate(0);
            }
        }

        /// <summary>
        /// Luminoサウンドオブジェクトに各種設定をセットします。
        /// 音量 0.0~1.0, ピッチ 0.5~2.0, パン -100~+100
        /// </summary>
        //public static void SetSoundData(Lumino.Sound snd, string rPath, int volume = Media.SoundVolumeDefault, int pitch = Media.SoundPitchDefault, int pan = Media.SoundPanDefault) {
        //	if(snd != null) {
        //		snd.Dispose();
        //	}

        //	snd = new Lumino.Sound(ProjectManager.ProjectPath + rPath);
        //	snd.Is3DEnabled = true;
        //	snd.EmitterMaxDistance = Media.Sound3DDistance;
        //	snd.Volume = (float)(Common.ApplyLimit(volume, Media.SoundVolumeMin, Media.SoundVolumeMax) / 100.0);
        //	snd.Pitch = (float)(Common.ApplyLimit(pitch, Media.SoundPitchMin, Media.SoundPitchMax) / 100.0);

        //	//パンを設定する
        //	var tempPan = Common.ApplyLimit(pan, Media.SoundPanMin, Media.SoundPanMax);
        //	if(pan == 0) {
        //		snd.SetEmitterPosition(0, 0, 0);
        //	} else {
        //		snd.SetEmitterPosition(
        //			-(float)(Math.Cos(((float)(tempPan - Media.SoundPanMin) / (Media.SoundPanMax - Media.SoundPanMin)) * Math.PI) * 100),
        //			0,
        //			(float)(Math.Sin(((float)(tempPan - Media.SoundPanMin) / (Media.SoundPanMax - Media.SoundPanMin)) * Math.PI) * 100)
        //		);
        //	}
        //}

        /// <summary>
        /// 描画対象オブジェクトの抽象クラス
        /// </summary>
        public abstract class DrawTarget {
            private const int GridPatternSize = 10;
            protected const int NormalFPS = 60;
            protected DXInterpolation polation;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public DrawTarget(Control destCtrl, DXInterpolation pol) {
                this.DrawDest = destCtrl.Handle;
                this.TargetControl = destCtrl;
                this.polation = pol;
            }

            /// <summary>
            /// 描画対象コントロールのポインタ
            /// </summary>
            public IntPtr DrawDest {
                get;
                private set;
            }

            /// <summary>
            /// 描画対象コントロール
            /// </summary>
            public Control TargetControl {
                get;
                private set;
            }

            //描画処理
            public abstract void Draw();

            /// <summary>
            /// 現在の補間方法に従って描画モードをセットします。
            /// </summary>
            public void SetDXInterpolation() {
                switch (this.polation) {
                    case DXInterpolation.Bilinear:
                        DX.SetDrawMode(DX.DX_DRAWMODE_BILINEAR);
                        break;

                    case DXInterpolation.Nearest:
                        DX.SetDrawMode(DX.DX_DRAWMODE_NEAREST);
                        break;
                }
            }

            /// <summary>
            /// このオブジェクトが使う素材を読み込みます。
            /// </summary>
            public virtual void LoadMaterial() {
                Media.ClearResources();
            }

            /// <summary>
            /// このオブジェクトが破棄されるときに呼び出されます。
            /// </summary>
            public virtual void Unloading() {
            }

            /// <summary>
            /// 画面をクリアします。
            /// </summary>
            public void ClsScreen() {
                DX.DrawBox(0, 0, this.TargetControl.Size.Width, this.TargetControl.Size.Height, DX.GetColor(0, 0, 0), DX.TRUE);
            }

            /// <summary>
            /// 背景となる格子状の模様を描画します。
            /// </summary>
            public void DrawGridPattern() {
                for (var x = 0; x < this.TargetControl.Size.Width; x += GridPatternSize * 2) {
                    for (var y = 0; y < this.TargetControl.Size.Height; y += GridPatternSize) {
                        DX.DrawBox(
                            x + (y / GridPatternSize % 2 == 1 ? GridPatternSize : 0),
                            y,
                            x + (y / GridPatternSize % 2 == 1 ? GridPatternSize : 0) + GridPatternSize,
                            y + GridPatternSize,
                            DX.GetColor(60, 60, 60),
                            DX.TRUE
                        );
                    }
                }
            }
        }
    }
}
