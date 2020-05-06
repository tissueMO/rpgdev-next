using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgToolPictureTrans : Form {
        private readonly string srcFileFullPath;           //編集対象のソースファイル

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgToolPictureTrans(string _srcFileFullPath, string _destFileName) {
            this.InitializeComponent();
            this.srcFileFullPath = _srcFileFullPath;
        }

        /// <summary>
        /// 初期設定
        /// </summary>
        private void dlgToolPictureTrans_Load(object sender, EventArgs e) {
            this.picPreview.LoadAsync(this.srcFileFullPath);
            this.uctlFileTree.RootPath = Resources.Path_Materials;
            this.uctlFileTree.SelectTreeNode(this.uctlFileTree.RootNode.FullPath);
        }

        /// <summary>
        /// 色をスポイト
        /// </summary>
        private void picPreview_MouseClick(object sender, MouseEventArgs e) {
            this.pnlTransColor.BackColor = (this.picPreview.Image as Bitmap).GetPixel(e.X, e.Y);
        }

        /// <summary>
        /// 色を直接指定
        /// </summary>
        private void pnlTransColor_MouseClick(object sender, MouseEventArgs e) {
            this.dlgColor.Color = this.pnlTransColor.BackColor;
            if (this.dlgColor.ShowDialog(this) == DialogResult.OK) {
                this.pnlTransColor.BackColor = this.dlgColor.Color;
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
            var bmp = new Bitmap(this.picPreview.Image);
            bmp.MakeTransparent(this.pnlTransColor.BackColor);       //透過色をセット

            try {
                bmp.Save(filename, System.Drawing.Imaging.ImageFormat.Png);
                MessageBox.Show("イメージを保存しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                this.picPreview.Image.Dispose();
            } catch {
                MessageBox.Show("I/Oエラーのため、イメージの保存に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            }
            bmp.Dispose();
        }
    }
}
