using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgInputTextValue : Form {
        private readonly string rootPath;
        private readonly string fileFilter;
        private readonly bool isFolder;
        private readonly bool allowOverwrite;
        private readonly bool fileNameMode;

        /// <summary>
        /// コンストラクター・単純テキスト入力用
        /// </summary>
        public dlgInputTextValue(string title, string defaultText, List<string> autoCompleteList) {
            this.InitializeComponent();
            this.fileNameMode = false;
            this.Text = title;
            this.cmbText.Text = defaultText;

            //オートコンプリートリストを設定
            this.cmbText.Items.Clear();
            if (autoCompleteList != null) {
                foreach (var item in autoCompleteList) {
                    this.cmbText.Items.Add(item);
                }
            }
        }

        /// <summary>
        /// コンストラクター・ファイル名入力用
        /// </summary>
        public dlgInputTextValue(string rootPath, bool isFolder, bool allowOverwrite, string defaultText, string filter = "") {
            this.InitializeComponent();
            this.rootPath = Common.CutLastChar(rootPath, '\\') + "\\";
            this.isFolder = isFolder;
            this.allowOverwrite = allowOverwrite;
            this.fileFilter = filter;
            this.fileNameMode = true;
            this.cmbText.Text = defaultText;
            this.cmbText.SelectionStart = 0;
        }

        /// <summary>
        /// 入力された文字列
        /// </summary>
        public string ResultText {
            get {
                return this.cmbText.Text;
            }
        }

        /// <summary>
        /// 入力されたファイル名のプロジェクトフォルダーを基準としたパス
        /// </summary>
        public string rPath {
            get {
                return this.rootPath + this.ResultText;
            }
        }

        /// <summary>
        /// 入力されたファイル名のフルパス
        /// </summary>
        public string FullPath {
            get {
                return ProjectManager.ProjectPath + this.rPath;
            }
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            if (!this.fileNameMode) {
                //単なるテキスト入力モードのときはエラーチェックを行わずに通す
                return;
            }

            var isNG = false;
            if (string.IsNullOrEmpty(this.cmbText.Text)) {
                //入力されていない
                MessageBox.Show("名前を入力して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                isNG = true;
            } else if (!Common.CheckEnabledPath(this.cmbText.Text)) {
                //ファイル名に使えない文字が含まれる
                MessageBox.Show(Resources.MsgE_FileNameInvarid.Replace("$", "ファイル・フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                isNG = true;
            } else if (this.isFolder == false && File.Exists(this.FullPath)) {
                //既に存在するファイル
                if (!this.allowOverwrite) {
                    MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "ファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    isNG = true;
                } else if (MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "ファイル").Replace("。", "が、上書きしますか？"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                    isNG = true;
                }
            } else if (this.isFolder && Directory.Exists(this.FullPath)) {
                //既に存在するフォルダー
                if (!this.allowOverwrite) {
                    MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    isNG = true;
                } else if (MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "フォルダー").Replace("。", "が、上書きしますか？"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                    isNG = true;
                }
            } else if (!this.isFolder && this.fileFilter != "" && !Common.CheckFileExtensionFilter(this.ResultText, this.fileFilter)) {
                //該当拡張子が付いていないと、作成してもリストに表示されないので無効にする
                MessageBox.Show("無効な拡張子です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                isNG = true;
            }

            if (isNG) {
                //確定却下する場合
                this.DialogResult = DialogResult.None;
            }
        }
    }
}
