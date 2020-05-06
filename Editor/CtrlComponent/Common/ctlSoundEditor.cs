#pragma warning disable 0197            //参照マーシャリングがうんたらかんたら
#pragma warning disable IDE0009         // メンバー アクセスを修飾する必要があります。

using System;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;

    public partial class ctlSoundEditor : UserControl {

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlSoundEditor() {
            InitializeComponent();

            //値の設定を行う
            this.trkVolume.Minimum = Media.SoundVolumeMin;
            this.trkVolume.Maximum = Media.SoundVolumeMax;
            this.trkVolume.Value = Media.SoundVolumeDefault;
            this.trkVolume.TickFrequency = (this.trkVolume.Maximum - this.trkVolume.Minimum) / 5;
            this.trkVolume.LargeChange = this.trkVolume.TickFrequency;
            this.trkPan.Minimum = Media.SoundPanMin;
            this.trkPan.Maximum = Media.SoundPanMax;
            this.trkPan.Value = Media.SoundPanDefault;
            this.trkPan.TickFrequency = (this.trkPan.Maximum - this.trkPan.Minimum) / 5;
            this.trkPan.LargeChange = this.trkPan.TickFrequency;
            this.trkPitch.Minimum = Media.Common_SoundPitchMin;
            this.trkPitch.Maximum = Media.Common_SoundPitchMax;
            this.trkPitch.Value = Media.SoundPitchDefault;
            this.trkPitch.TickFrequency = (this.trkPitch.Maximum - this.trkPitch.Minimum) / 5;
            this.trkPitch.LargeChange = this.trkPitch.TickFrequency;
            this.SoundData = new Media.SoundObject("");
        }

        /// <summary>
        /// プロパティの再定義
        /// </summary>
        public new bool DesignMode {
            get {
                var returnFlag = base.DesignMode;
#if DEBUG
                //入れ子のユーザーコントロールが、デザイナー時であるにも関わらず DesignMode=False になる問題を解決する
                if (System.ComponentModel.LicenseManager.UsageMode == System.ComponentModel.LicenseUsageMode.Designtime) {
                    returnFlag = true;
                } else if (System.Diagnostics.Process.GetCurrentProcess().ProcessName.ToUpper().Equals("DEVENV")) {
                    returnFlag = true;
                }
#endif
                return returnFlag;
            }
        }

        /// <summary>
        /// 現在のサウンド情報
        /// </summary>
        public Media.SoundObject SoundData {
            get {
                return new Media.SoundObject(this.uctlPlayer.SoundFileName, this.trkVolume.Value, this.trkPitch.Value, this.trkPan.Value);
            }
            set {
                this.soundData = value;
                if (value == null) {
                    //初期状態に戻す
                    this.uctlPlayer.SoundFileName = "";
                    this.trkVolume.Value = Media.SoundVolumeDefault;
                    this.trkPitch.Value = Media.SoundPitchDefault;
                    this.trkPan.Value = Media.SoundPanDefault;
                } else {
                    //詳細情報も反映させる
                    this.uctlPlayer.SoundFileName = value.FileName;
                    this.trkVolume.Value = value.Volume;
                    this.trkPitch.Value = value.Pitch;
                    this.trkPan.Value = value.Panpot;
                    if (!this.DesignMode) {
                        Media.SetSoundData(ref this.uctlPlayer.SoundHandle, value);
                    }
                }
            }
        }
        private Media.SoundObject soundData;

        /// <summary>
        /// 保持しているサウンドを破棄します。
        /// </summary>
        public void UnloadMedia() {
            this.uctlPlayer.UnloadMedia();
        }

        /// <summary>
        /// 再生前に読み込みを行います。
        /// </summary>
        private void uctlPlayer_BeforePlaySound(object sender, EventArgs e) {
            this.SoundData = new Media.SoundObject(this.uctlPlayer.SoundFileName, this.trkVolume.Value, this.trkPitch.Value, this.trkPan.Value);
        }

        /// <summary>
        /// 設定を既定値に戻します。
        /// </summary>
        private void btnInit_Click(object sender, EventArgs e) {
            this.trkVolume.Value = this.trkVolume.Maximum;
            this.trkPitch.Value = 0;
            this.trkPan.Value = 0;
        }
    }
}
