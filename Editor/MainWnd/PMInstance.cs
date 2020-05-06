using System;
using System.Windows.Forms;

namespace Editor {
    using Properties;

    /// <summary>
    /// エディターで用いられるインスタンスを宣言します。
    /// </summary>
    public partial class MainWindow : Form {
        public static event EventHandler ProjectClosed;         //プロジェクトが閉じられたときの呼び出されるイベント
        public static event EventHandler ProjectStartupped;     //プロジェクトが立ち上がったときに呼び出されるイベント
        private ProjectManager project = null;

        /// <summary>
        /// プロジェクトのインスタンスにアクセスするためのプロパティ
        /// </summary>
        public ProjectManager Project {
            get {
                return this.project;
            }
            set {
                if (this.project == value) {
                    return;
                }

                this.project = value;

                if (value == null) {
                    //プロジェクトを閉じる場合はイベントを発生させる
                    ProjectClosed?.Invoke(this, null);
                } else {
                    //プロジェクトを開く場合にもイベントを発生させる
                    ProjectStartupped?.Invoke(this, null);
                }
            }
        }

        /// <summary>
        /// プロジェクトを閉じる
        /// </summary>
        public void CloseProject() {
            this.Project = null;
        }

        /// <summary>
        /// プロジェクト始動時
        /// </summary>
        private void startupProject(object sender, EventArgs e) {
            this.Text = $"{this.Project.ProjectName} - {Resources.AppName}";
            this.ctlSwitchMainMenu(true);       //各種コントロール有効

            //２ペインの右タブを復元する
            this.tbcRightPaneTabs.TabPages.Clear();
            var RightTabs = Settings.Default.Form_2PaneImgIndices.Split(Resources.Split_SimpleList.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            if (RightTabs != null) {
                foreach (var tab in RightTabs) {
                    foreach (TabPage finder in this.tbcLeftPaneTabs.TabPages) {
                        if (finder.ImageIndex == Convert.ToInt32(tab)) {
                            this.tbcRightPaneTabs.TabPages.Add(finder);
                            break;
                        }
                    }
                }
                this.tbcRightPaneTabs.SelectedIndex = Settings.Default.Last_MainTabRight;
            }
            this.splMain1.Panel2Collapsed = (this.tbcRightPaneTabs.SelectedIndex == -1);
            this.tbcLeftPaneTabs.SelectedIndex = Settings.Default.Last_MainTabLeft;
            this.tbcLeftPaneTabs.Visible = true;
            this.mnuView2Pane.Checked = (Settings.Default.Form_2PaneImgIndices != "");            //既に右タブを追加した状態でチェックを変えると、現在の左タブが移動しない
            this.mspMainMenu.SendToBack();

            //各エディターの設定を行う
            //スケジュール
            this.uctlSchedule.DoLoad();
            //テキスト
            this.uctlTextEditor.DBList = this.uctlDBEditor.DBList;
            this.uctlTextEditor.uctlTextFiles.RootPath = Resources.Path_Texts;
            this.uctlTextEditor.cmbTextSearchRange.SelectedIndex = 0;
            this.uctlTextEditor.uctlTextFiles.SelectTreeNode(Settings.Default.Last_TextPath);
            this.uctlTextEditor.cmbTextSearchRange.SelectedIndex = Settings.Default.Last_TextSearchRange;
            this.uctlTextEditor.chkTextCapital.Checked = Settings.Default.Last_TextSearchCase;
            this.uctlTextEditor.chkTextRegex.Checked = Settings.Default.Last_TextSearchRegux;
            //データベース
            this.uctlDBEditor.DoLoad();
            this.uctlDBEditor.tbcDatabaseTab.SelectedIndex = Settings.Default.Last_DB;
            //マップ
            this.uctlMapEditor.DBCtrl = this.uctlDBEditor;
            this.uctlMapEditor.DBList = this.uctlDBEditor.DBList;
            this.uctlMapEditor.comTree = this.uctlTextEditor.uctlComTree;
            this.uctlMapEditor.uctlMapTree.RootPath = Resources.Path_Maps;
            this.uctlMapEditor.uctlMapTree.SelectTreeNode(Settings.Default.Last_MapPath);

            //エディターをまたぐ操作の準備
            this.uctlTextEditor.SetupSubControls(this.uctlDBEditor, this.uctlMapEditor.mgr);
        }

        /// <summary>
        /// プロジェクトが閉じられたとき
        /// </summary>
        private void closedProject(object sender, EventArgs e) {
            this.Text = Resources.AppName;

            // メインタブを非表示にする
            this.mnuView2Pane.Checked = false;
            this.tbcLeftPaneTabs.Visible = false;
            Settings.Default.Last_MainTabLeft = 0;
            Settings.Default.Last_MainTabRight = -1;
            Settings.Default.Form_2PaneImgIndices = "";

            //各種コントロール無効
            this.ctlSwitchMainMenu(false);
        }
    }
}
