using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgToolPictureResize : Form {
        private readonly string srcFileFullPath;           //編集対象のソースファイル

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgToolPictureResize(string _srcFileFullPath, string _destFileName) {
            this.InitializeComponent();
            this.srcFileFullPath = _srcFileFullPath;
            this.txtFileName.Text = _destFileName;
        }

        /// <summary>
        /// 初期設定
        /// </summary>
        private void dlgToolPictureResize_Load(object sender, EventArgs e) {
            this.picPreview.Load(this.srcFileFullPath);
            this.numBeforeWidth.Value = this.picPreview.Image.Width;
            this.numAfterWidth.Value = this.picPreview.Image.Width;
            this.numBeforeHeight.Value = this.picPreview.Image.Height;
            this.numAfterHeight.Value = this.picPreview.Image.Height;
            this.cmbInterpolation.SelectedIndex = 0;
            this.uctlFileTree.RootPath = Resources.Path_Materials;
            this.uctlFileTree.SelectTreeNode(this.uctlFileTree.RootNode.FullPath);
        }

        /// <summary>
        /// アスペクト比を維持して幅変更
        /// </summary>
        private void numAfterWidth_ValueChanged(object sender, EventArgs e) {
            if (this.chkKeepAspect.Checked) {
                //500:100=x:1 の式変形から導出している
                this.numAfterWidth.Value = Math.Round(this.numBeforeWidth.Value * this.numAfterHeight.Value / this.numBeforeHeight.Value);
            }
        }

        /// <summary>
        /// アスペクト比を維持して高さ変更
        /// </summary>
        private void numAfterHeight_ValueChanged(object sender, EventArgs e) {
            if (this.chkKeepAspect.Checked) {
                //500:100=5:y の式変形から導出している
                this.numAfterHeight.Value = Math.Round(this.numBeforeHeight.Value * this.numAfterWidth.Value / this.numBeforeWidth.Value);
            }
        }

        /// <summary>
        /// 新規フォルダー作成
        /// </summary>
        private void tolNewFolder_Click(object sender, EventArgs e) {
            this.uctlFileTree.CreateFolder();
        }

        /// <summary>
        /// 入力情報を検証
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (!Common.CheckEnabledPath(this.txtFileName.Text)) {
                //ファイル名に使えない文字は無効
                MessageBox.Show(Resources.MsgE_FileNameInvarid.Replace("$", "ファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return;
            } else if (!this.uctlFileTree.IsSelected) {
                //フォルダーが選択されていない
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "保存先フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return;
            }

            //上書き確認
            var filename = ProjectManager.ProjectPath + Common.AddToPathSplit(this.uctlFileTree.SelectedNodeFullPath) + this.txtFileName.Text;
            if (File.Exists(filename)
            && MessageBox.Show("同名のファイルが存在します。" + Resources.MsgQ_Confirm.Replace("$", "上書き"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                this.DialogResult = DialogResult.None;
                return;
            }

            //実際に出力処理を行う
            var src = new Bitmap(this.picPreview.Image);
            var dest = new Bitmap((int) this.numAfterWidth.Value, (int) this.numAfterHeight.Value);
            var g = Graphics.FromImage(dest);
            Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);
            g.DrawImage(src, new Rectangle(0, 0, (int) this.numAfterWidth.Value, (int) this.numAfterHeight.Value));

            try {
                dest.Save(filename, System.Drawing.Imaging.ImageFormat.Png);
                MessageBox.Show("イメージを保存しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                this.picPreview.Image.Dispose();
            } catch {
                MessageBox.Show("I/Oエラーのため、イメージの保存に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            }
            src.Dispose();
            dest.Dispose();
            g.Dispose();
        }
    }
}
