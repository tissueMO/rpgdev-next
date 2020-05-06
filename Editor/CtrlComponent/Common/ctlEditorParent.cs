using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Properties;

    public partial class ctlEditorParent : UserControl {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlEditorParent() {
            this.InitializeComponent();
        }

        /// <summary>
        /// エディターが変更されているかどうか
        /// </summary>
        public virtual bool IsDirty {
            get;
            protected set;
        }

        /// <summary>
        /// 保存操作を実行します。返値は成功したかどうかを示します。
        /// </summary>
        public virtual bool DoSave() {
            return true;
        }

        /// <summary>
        /// 閉じる操作を実行します。
        /// </summary>
        public virtual void DoClose() {
        }

        /// <summary>
        /// 不要なファイルを除去します。
        /// </summary>
        public virtual void DoCleanup() {
        }

        /// <summary>
        /// 他ユーザーに変更されたファイル名をもとにエディターの該当画面にジャンプします。
        /// 返値は何らかの処理を行ったかどうかを示します。
        /// </summary>
        public virtual bool Jump(string rPath) {
            return false;
        }

        /// <summary>
        /// 他ユーザーに変更されたファイル名をもとに競合発生やリロードを行います。
        /// 返値は何らかの処理を行ったかどうかを示します。
        /// </summary>
        public virtual bool Conflict(string rPath, ProjectManager project) {
            return false;
        }

        /// <summary>
        /// 閲覧中のファイルを環境設定に従ったプロセスで再読み込みします。
        /// </summary>
        protected void ReloadContent(ctlFileTree fileTree) {
            if (Settings.Default.Common_CurrentFileChanged_Confirm
            && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "現在のファイルが他のユーザーによって変更されました。\r\n再読込"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes) {
                fileTree.ReloadTree(fileTree.SelectedNodeFullPath);
            } else if (Settings.Default.Common_CurrentFileChanged_AutoReload) {
                //自動で再読み込みする設定
                fileTree.ReloadTree(fileTree.SelectedNodeFullPath);
            }
        }
    }
}
