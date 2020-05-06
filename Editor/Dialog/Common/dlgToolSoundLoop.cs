using DxLibDLL;
using System;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgToolSoundLoop : Form {
        private int dxHandle = -1;
        private int samplePerSecond;                    //サンプリングレートHz
        private bool isDirty = false;
        private bool SeekBySystem = false;
        private const decimal BeforeEndTime = 2.0M;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgToolSoundLoop() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            this.uctlFileTree.FileFilter = Resources.Extension_LoopSounds;
            this.uctlFileTree.RootPath = Resources.Path_Materials;
            this.uctlFileTree.SelectTreeNode(this.uctlFileTree.RootNode.FullPath);
        }

        /// <summary>
        /// ウィンドウ消去時
        /// </summary>
        private void dlgToolSoundLoop_FormClosing(object sender, FormClosingEventArgs e) {
            DX.DeleteSoundMem(this.dxHandle);
            this.dxHandle = -1;
        }

        /// <summary>
        /// 再生処理
        /// </summary>
        private void playSound(bool loopMode) {
            DX.StopSoundMem(this.dxHandle);

            //ループ位置を設定する
            if (loopMode) {
                DX.SetLoopAreaSamplePosSoundMem((int) this.numLoopStart.Value, (int) this.numLoopEnd.Value, this.dxHandle);
            } else {
                DX.SetLoopAreaSamplePosSoundMem(0, this.prgSeekBar.Maximum, this.dxHandle);
            }

            DX.SetCurrentPositionSoundMem((int) this.numNowPos.Value, this.dxHandle);
            DX.PlaySoundMem(this.dxHandle, DX.DX_PLAYTYPE_BACK, DX.FALSE);
        }

        /// <summary>
        /// ループ再生
        /// </summary>
        private void btnLoopPlay_Click(object sender, EventArgs e) {
            this.playSound(true);
        }

        /// <summary>
        /// 最後まで再生
        /// </summary>
        private void btnLoopNormalPlay_Click(object sender, EventArgs e) {
            this.playSound(false);
        }

        /// <summary>
        /// ループ始点へ移動
        /// </summary>
        private void btnLoopSetStartPos_Click(object sender, EventArgs e) {
            this.numNowPos.Value = this.numLoopStart.Value;
        }

        /// <summary>
        /// 停止
        /// </summary>
        private void btnLoopPlayStop_Click(object sender, EventArgs e) {
            DX.StopSoundMem(this.dxHandle);
            this.btnLoopPlay.Enabled = true;
            this.btnLoopNormalPlay.Enabled = true;
        }

        /// <summary>
        /// ループ終点２秒前へ移動
        /// </summary>
        private void btnLoopSetEndPos_Click(object sender, EventArgs e) {
            this.numNowPos.Value = Math.Max(this.numLoopEnd.Value - BeforeEndTime * this.samplePerSecond, 0);
        }

        /// <summary>
        /// ０秒へ移動
        /// </summary>
        private void btunSetZero_Click(object sender, EventArgs e) {
            this.numNowPos.Value = 0;
        }

        /// <summary>
        /// 設定を保存
        /// </summary>
        private void btnLoopSave_Click(object sender, EventArgs e) {
            if (!this.isDirty) {
                return;     //変更がない場合は処理しない
            }

            //書き込み処理
            var fileName = this.GetLoopFileName(this.uctlFileTree.SelectedNodeFullPath);
            using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                W.WriteLine($"LOOPSTART={(int) (this.numLoopStart.Value)}");
                W.WriteLine($"LOOPEND={(int) (this.numLoopEnd.Value)}");
            }

            this.isDirty = false;
            System.Media.SystemSounds.Asterisk.Play();
        }

        /// <summary>
        /// サウンドプレイヤー
        /// </summary>
        private void loopSoundWatcher_Tick(object sender, EventArgs e) {
            DX.ProcessMessage();
            if (this.dxHandle != -1 && DX.CheckSoundMem(this.dxHandle) == DX.TRUE) {
                //再生中のみシークバーを更新
                this.SeekBySystem = true;
                this.prgSeekBar.Value = (int) DX.GetCurrentPositionSoundMem(this.dxHandle);
                this.numNowPos.Value = this.prgSeekBar.Value;
                this.SeekBySystem = false;
            }
        }

        /// <summary>
        /// 再生位置変更時
        /// </summary>
        private void numNowPos_ValueChanged(object sender, EventArgs e) {
            if (!this.SeekBySystem) {
                var afterPlay = (DX.CheckSoundMem(this.dxHandle) == DX.TRUE);
                this.prgSeekBar.Value = (int) this.numNowPos.Value;

                //変更処理
                DX.StopSoundMem(this.dxHandle);
                DX.SetCurrentPositionSoundMem(this.prgSeekBar.Value, this.dxHandle);
                if (afterPlay) {
                    DX.PlaySoundMem(this.dxHandle, DX.DX_PLAYTYPE_BACK, DX.FALSE);
                }
            }
        }

        /// <summary>
        /// ループ位置変更時
        /// </summary>
        private void numLoopValue_ValueChanged(object sender, EventArgs e) {
            this.isDirty = true;
            if (this.numLoopStart.Value > this.numLoopEnd.Value) {
                //始点と終点が逆転したら、変更していない方を調整する
                if (sender == this.numLoopStart) {
                    this.numLoopEnd.Value = this.numLoopStart.Value;
                } else {
                    this.numLoopStart.Value = this.numLoopEnd.Value;
                }
            }
        }

        /// <summary>
        /// シークバー変更時
        /// </summary>
        private void prgSeekBar_MouseClick(object sender, MouseEventArgs e) {
            switch (e.Button) {
                case MouseButtons.Left:
                    //再生位置セット
                    this.numNowPos.Value = (long) e.X * this.prgSeekBar.Maximum / this.prgSeekBar.Size.Width;
                    break;

                case MouseButtons.Right:
                    //終点セット
                    this.numLoopEnd.Value = (long) e.X * this.prgSeekBar.Maximum / this.prgSeekBar.Size.Width;
                    break;

                case MouseButtons.Middle:
                    //始点セット
                    this.numLoopStart.Value = (long) e.X * this.prgSeekBar.Maximum / this.prgSeekBar.Size.Width;
                    break;
            }
        }

        /// <summary>
        /// ファイル変更前
        /// </summary>
        private void uctlFileTree_BeforeSelectNode(object sender, TreeViewCancelEventArgs e) {
            if (this.isDirty) {
                switch (MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "変更を保存"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question)) {
                    case DialogResult.Yes:
                        this.btnLoopSave_Click(sender, null);
                        break;
                    case DialogResult.Cancel:
                        e.Cancel = true;
                        return;
                }
            }
            DX.DeleteSoundMem(this.dxHandle);
            this.dxHandle = -1;
            this.pnlWorkspace.Enabled = false;
        }

        /// <summary>
        /// ファイル変更後
        /// </summary>
        private void uctlFileTree_AfterSelectNode(object sender, TreeViewEventArgs e) {
            this.pnlWorkspace.Enabled = this.uctlFileTree.IsFileSelected;
            if (this.uctlFileTree.IsFileSelected) {
                //サウンドを読み込む
                DX.SetUseASyncLoadFlag(DX.FALSE);
                this.dxHandle = DX.LoadSoundMem(ProjectManager.ProjectPath + this.uctlFileTree.SelectedNodeFullPath);
                if (this.dxHandle == -1) {
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "サウンドの読み込み"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.pnlWorkspace.Enabled = false;
                    return;
                }

                //各種コントロール初期化
                this.numNowPos.Value = 0;
                this.prgSeekBar.Value = 0;
                this.prgSeekBar.Maximum = (int) DX.GetSoundTotalSample(this.dxHandle);
                this.numNowPos.Maximum = this.prgSeekBar.Maximum;
                this.numLoopStart.Maximum = this.prgSeekBar.Maximum;
                this.numLoopEnd.Maximum = this.prgSeekBar.Maximum;
                this.samplePerSecond = (int) ((decimal) DX.GetSoundTotalSample(this.dxHandle) / DX.GetSoundTotalTime(this.dxHandle) * 1000.0M);       //Hz単位にする

                //ループ設定ファイルを読み込む
                if (File.Exists(ProjectManager.ProjectPath + this.GetLoopFileName(this.uctlFileTree.SelectedNodeFullPath))) {
                    using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + this.GetLoopFileName(this.uctlFileTree.SelectedNodeFullPath)), Common.SJIS)) {
                        string buf;
                        buf = R.ReadLine();
                        this.numLoopStart.Value = decimal.Parse(buf.Substring(buf.IndexOf("=") + 1));
                        buf = R.ReadLine();
                        this.numLoopEnd.Value = decimal.Parse(buf.Substring(buf.IndexOf("=") + 1));
                    }
                } else {
                    this.numLoopStart.Value = 0;
                    this.numLoopEnd.Value = this.numLoopEnd.Maximum;
                }

                this.isDirty = false;
            }
        }

        /// <summary>
        /// ループファイル名 (rPath) を返します。
        /// </summary>
        /// <param name="rPath">元のサウンドファイル名 (rPath)</param>
        private string GetLoopFileName(string rPath) {
            return Path.GetDirectoryName(rPath) + "\\" + Path.GetFileNameWithoutExtension(rPath) + "_loop.txt";
        }
    }
}
