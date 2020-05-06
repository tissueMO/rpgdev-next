using System;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;

    public partial class ctlPathSetter : UserControl {
        public event EventHandler PathChanged;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlPathSetter() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
        }

        /// <summary>
        /// ダイアログの種別
        /// </summary>
        public enum SetterDialogType {
            ProjectRoot,        //プロジェクトフォルダーをルートとした相対パス
            Folder,
            OpenFile,
            SaveFile,
        }

        /// <summary>
        /// ボタンを押して表示されるダイアログの種別
        /// </summary>
        public SetterDialogType DialogType {
            set; get;
        }

        /// <summary>
        /// ファイルを選ぶダイアログで有効な拡張子
        /// </summary>
        public string FileFilter {
            set; get;
        }

        /// <summary>
        /// ボタンを押して表示されるダイアログのタイトル
        /// </summary>
        public string DialogTitle {
            set; get;
        }

        /// <summary>
        /// 相対パスダイアログの基点フォルダー（空白の場合はプロジェクトフォルダー直下）
        /// </summary>
        public string RootPath {
            get; set;
        }

        /// <summary>
        /// ファイルを選択するかどうか
        /// </summary>
        public bool EnabledFiles {
            get; set;
        } = true;

        /// <summary>
        /// 選択されたファイル名
        /// </summary>
        public string Result {
            get {
                return this.txtFileName.Text;
            }
            set {
                this.txtFileName.Text = value;
                this.toolHint.SetToolTip(this.txtFileName, value);
            }
        }

        /// <summary>
        /// ダイアログを開いてパスをセットします。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public bool OpenPathDialog() {
            switch (this.DialogType) {
                case SetterDialogType.ProjectRoot:
                    var Dlg = new Dialog.Common.dlgFileSelect(this.RootPath, this.FileFilter, this.EnabledFiles, false, false, this.Result);
                    if (Dlg.ShowDialog() == DialogResult.OK) {
                        this.Result = Dlg.FileName;
                    } else {
                        return false;
                    }
                    break;

                case SetterDialogType.Folder:
                    this.folderDlg.SelectedPath = this.Result;
                    this.folderDlg.Description = this.DialogTitle;
                    if (this.folderDlg.ShowDialog(this) == DialogResult.OK) {
                        this.Result = this.folderDlg.SelectedPath;
                    } else {
                        return false;
                    }
                    break;

                case SetterDialogType.OpenFile:
                    this.openFileDlg.FileName = this.Result;
                    this.openFileDlg.Title = this.DialogTitle;
                    this.openFileDlg.Filter = this.FileFilter;
                    if (this.openFileDlg.ShowDialog(this) == DialogResult.OK) {
                        this.Result = this.openFileDlg.FileName;
                    } else {
                        return false;
                    }
                    break;

                case SetterDialogType.SaveFile:
                    this.saveFileDlg.FileName = this.Result;
                    this.saveFileDlg.Title = this.DialogTitle;
                    this.saveFileDlg.Filter = this.FileFilter;
                    if (this.saveFileDlg.ShowDialog(this) == DialogResult.OK) {
                        this.Result = this.saveFileDlg.FileName;
                    } else {
                        return false;
                    }
                    break;
            }
            return true;
        }

        /// <summary>
        /// ボタンを押したとき
        /// </summary>
        private void btnPathSetter_Click(object sender, EventArgs e) {
            this.OpenPathDialog();
        }

        /// <summary>
        /// パスが変更されたときに発生するイベントを中継します。
        /// </summary>
        private void txtFileName_TextChanged(object sender, EventArgs e) {
            this.PathChanged?.Invoke(this, null);
        }
    }
}
