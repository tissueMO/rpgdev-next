using DxLibDLL;
using System;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;

    public partial class ctlSoundPlayer : UserControl {
        public event EventHandler BeforePlaySound;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlSoundPlayer() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
        }

        /// <summary>
        /// 使用できるライブラリの一覧
        /// </summary>
        private enum LibraryType {
            DX,
            Lumino,
        }

        /// <summary>
        /// 再生に使用するライブラリ
        /// </summary>
        private LibraryType TargetLibrary {
            get; set;
        } = LibraryType.DX;

        /// <summary>
        /// DXライブラリ用のサウンドハンドル
        /// </summary>
        public int SoundHandle = -1;

        /// <summary>
        /// Luminoライブラリ用のサウンドオブジェクト
        /// </summary>
        //public Lumino.Sound SoundObj {
        //	get; set;
        //}

        /// <summary>
        /// 再生対象のファイル名（プロジェクトフォルダー基準）
        /// </summary>
        public string SoundFileName {
            get; set;
        }

        /// <summary>
        /// 読み込みを手動でやるかどうか
        /// </summary>
        public bool SelfLoadMode {
            get; set;
        } = false;

        /// <summary>
        /// 再生中かどうか
        /// </summary>
        public bool IsPlaying {
            get {
                if (this.DesignMode) {
                    return false;
                }

                switch (this.TargetLibrary) {
                    case LibraryType.DX:
                        if (this.SoundHandle == Media.GGSMidiHandle) {
                            return Media.GGS.GetPlayerStatus().State == GuruGuruSmf.PlayerState.Play;
                        } else {
                            return DX.CheckSoundMem(this.SoundHandle) == DX.TRUE;
                        }

                    case LibraryType.Lumino:
                        //if(this.SoundObj != null) {
                        //	return this.SoundObj.PlayingState == Lumino.SoundPlayingState.Playing;
                        //}
                        return false;
                }
                return false;
            }
        }

        /// <summary>
        /// 再生準備を行います。
        /// </summary>
        /// <param name="rPath">プロジェクトフォルダーを基準としたパス</param>
        public void LoadMedia(string rPath) {
            var hdl = -1;
            var fPath = ProjectManager.ProjectPath + rPath;
            this.SoundFileName = rPath;

            this.UnloadMedia();
            if (!File.Exists(fPath)) {
                return;
            }

            switch (this.TargetLibrary) {
                case LibraryType.DX:
                    Media.SetSoundData(ref hdl, rPath);
                    this.SoundHandle = hdl;
                    break;

                case LibraryType.Lumino:
                    //Media.SetSoundData(this.SoundObj, rPath);
                    break;
            }
        }

        /// <summary>
        /// サウンドリソースを破棄します。
        /// </summary>
        public void UnloadMedia() {
            switch (this.TargetLibrary) {
                case LibraryType.DX:
                    if (this.SoundHandle == Media.GGSMidiHandle) {
                        Media.GGS.Stop(0);
                        Media.GGS.ClearList();
                    } else {
                        DX.StopSoundMem(this.SoundHandle);
                        DX.DeleteSoundMem(this.SoundHandle);
                    }
                    break;

                case LibraryType.Lumino:
                    //if(this.SoundObj != null) {
                    //	this.SoundObj.Dispose();
                    //	this.SoundObj = null;
                    //}
                    break;
            }
        }

        /// <summary>
        /// 再生します。
        /// </summary>
        public void DoPlay() {
            //事前準備
            DX.SetUseASyncLoadFlag(DX.FALSE);
            if (!this.SelfLoadMode) {
                this.LoadMedia(this.SoundFileName);
            } else {
                this.BeforePlaySound?.Invoke(this, null);
            }

            //再生処理
            switch (this.TargetLibrary) {
                case LibraryType.DX:
                    if (this.SoundHandle == Media.GGSMidiHandle) {
                        //GuruGuruSMFでMIDI再生
                        Media.GGS.Play(GuruGuruSmf.PlayOption.Loop | GuruGuruSmf.PlayOption.MasterPitch, Media.GGSMidiHandle, 0, 0, 0);
                    } else {
                        DX.PlaySoundMem(this.SoundHandle, DX.DX_PLAYTYPE_BACK);
                    }
                    break;

                case LibraryType.Lumino:
                    //if(this.SoundObj != null) {
                    //	this.SoundObj.Play();
                    //}
                    break;
            }
        }

        /// <summary>
        /// 停止します。
        /// </summary>
        public void DoStop() {
            this.UnloadMedia();
        }

        /// <summary>
        /// 再生ボタン
        /// </summary>
        private void btnPlay_Click(object sender, EventArgs e) {
            this.DoPlay();
        }

        /// <summary>
        /// 停止ボタン
        /// </summary>
        private void btnStop_Click(object sender, EventArgs e) {
            this.DoStop();
        }
    }
}
