using System;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgNewProject : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgNewProject() {
            this.InitializeComponent();
        }

        /// <summary>
        /// 入力結果を返します。
        /// </summary>
        /// <param name="path">プロジェクトフォルダーへのフルパス</param>
        /// <param name="pName">プロジェクト名</param>
        /// <param name="bLoadPreset">プリセットデータをロードするかどうか</param>
        public void GetResult(out string path, out string pName, out bool bLoadPreset) {
            path = this.ctlPathSet.Result;
            pName = this.txtProjectName.Text;
            bLoadPreset = this.chkLoadPresetData.Checked;
        }

        /// <summary>
        /// 入力情報を検証します。
        /// </summary>
        private void ctlCloseButton_ClickOK(object sender, EventArgs e) {
            if (!Common.CheckEnabledPath(this.txtProjectName.Text)) {
                //ファイル名に使えない文字が含まれるプロジェクト名
                MessageBox.Show(Resources.MsgE_FileNameInvarid.Replace("$", "プロジェクト"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            } else if (Directory.Exists(Common.AddToPathSplit(this.ctlPathSet.Result) + this.txtProjectName.Text)) {
                //既に存在するプロジェクトフォルダー
                MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            } else if (this.ctlPathSet.Result == "") {
                //作成場所が指定されていない
                MessageBox.Show(Resources.MsgE_NothingInput.Replace("$", "作成場所"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            }
        }

        /// <summary>
        /// テキストボックスがアクティブになったら全選択する
        /// </summary>
        private void TxtProjectName_Enter(object sender, EventArgs e) {
            ((TextBox) sender).SelectAll();
        }
    }
}
