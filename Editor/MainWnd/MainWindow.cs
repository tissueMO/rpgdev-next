using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor {
    using Module;
    using Module.DB;
    using Properties;

    #region <列挙体定義>
    /// <summary>
    /// メインタブのImageIndex
    /// </summary>
    public enum MainTabIndices {
        Schedule,
        Text,
        Database,
        Map,
    }

    /// <summary>
    /// 左か右か
    /// </summary>
    public enum LR {
        Left,
        Right,
        Both
    }

    /// <summary>
    /// 確認ダイアログの結果
    /// </summary>
    public enum ConfirmResult {
        Completed,          //処理が完了した
        Skipped,            //処理は行われなかった
        Canceled,           //その後の処理も中断しなければならない
    }
    #endregion

    public partial class MainWindow : Form {
        public Dialog.Common.dlgOpening SplashScreen = new Dialog.Common.dlgOpening();
        public bool SplashCloseFlag = false;
        private const int IEVersion = 11001;                                //WebBrowserコントロールで有効にするIEのバージョン: IE11/Edgeモード (最新のバージョンでレンダリング)
        private const int BalloonTime = 3000;                               //バルーンを表示するミリ秒
        private const int MinWidthPane = 30;                                //２ペイン表示を維持できる最小のサイズ
        private readonly List<string> conflictDirectories = new List<string>();      //競合を管理する対象フォルダー
        private readonly NotifyIcon balloonObject = new NotifyIcon();
        private Dialog.Common.dlgManual manualForm;

        #region <アプリケーション全般処理>
        /// <summary>
        /// コンストラクター
        /// </summary>
        public MainWindow() {
            this.showSplashScreeen();
            this.InitializeComponent();
            this.Size = Settings.Default.Form_MainSize;
            this.WindowState = Settings.Default.Form_MainWndState;
            Common.SetAutoTabIndices(this);          //タブオーダーを自動で設定する（1:Top値 -> 2:Left値）
            Common.EnableDoubleBuffering(this);      //ダブルバッファリングの設定

            //対象IEバージョンを変更して内部ブラウザーを生成する
            var regKey = Microsoft.Win32.Registry.CurrentUser.CreateSubKey("Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION");
            regKey.SetValue(Path.GetFileName(Application.ExecutablePath), IEVersion, Microsoft.Win32.RegistryValueKind.DWord);
#if DEBUG
            regKey.SetValue(Path.GetFileNameWithoutExtension(Application.ExecutablePath) + ".vshost.exe", IEVersion, Microsoft.Win32.RegistryValueKind.DWord);
#endif
            this.manualForm = new Dialog.Common.dlgManual();

            //NOTE: エディター一括：競合管理フォルダーを登録する。
            this.conflictDirectories.Add(Resources.Path_Texts);
            this.conflictDirectories.Add(Resources.Path_Maps);
            this.conflictDirectories.Add(Resources.Path_DBs);

            //バージョンアップした場合は各種設定を引き継ぐ
            if (!Settings.Default._Upgraded) {
                Settings.Default.Upgrade();
                Settings.Default._Upgraded = true;
                Settings.Default.Save();
            }

            // NOTE: 固定する設定
            Settings.Default.Map_SQEasyMode = false;

            //イベントハンドラーの登録
            MainWindow.ProjectClosed += this.closedProject;
            MainWindow.ProjectStartupped += this.startupProject;

            //全データベースの各列に対してもイベントハンドラーを登録する
            foreach (var db in this.uctlDBEditor.DBList) {
                foreach (var subdb in db.Value.DBs) {
                    foreach (DataGridViewColumn col in subdb.Columns) {
                        if (col.Tag is DBColumnButtonToScript) {
                            ((DBColumnButtonToScript) col.Tag).OpenScriptRequested += this.RequestOpenScriptEventHandler;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 前処理
        /// </summary>
        private void MainWindow_Load(object sender, EventArgs e) {
            //アプリケーション基本設定の復元を行う
            this.Location = Settings.Default.Form_MainLocation;         //Loadイベント内で行わないとマルチディスプレイに対応できないらしい
            this.mspMainMenu.SendToBack();
            this.Activate();
        }

        /// <summary>
        /// 終了処理
        /// </summary>
        private void MainWindow_FormClosing(object sender, FormClosingEventArgs e) {
            this.balloonObject.Visible = false;

            if (this.Project != null) {
                //プロジェクトに対する処理
                try {
                    switch (this.AllSave(true)) {     //プロジェクトを保存
                        case ConfirmResult.Canceled:
                            //保存をキャンセルした場合は終了しない
                            e.Cancel = true;
                            return;

                        case ConfirmResult.Skipped:
                            if (MessageBox.Show(Resources.MsgW_ResultError + "\r\n" + Resources.MsgQ_Confirm.Replace("$", "このまま終了"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                                //自分で原因を調査・解決するまで閉じたくない場合は終了をキャンセルする
                                e.Cancel = true;
                                return;
                            }
                            break;
                    }
                } catch {
                    //終了時にエラーが発生しても無視するのは、ハンドルしていない例外によって終了したときにもプロジェクトを保存できるようにするためである
                    //これを無視しないと、無限ループで例外が発生してしまう
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "プロジェクトの保存"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                //プロジェクト
                Settings.Default.Last_ProjectPath = ProjectManager.ProjectPath;

                //MainWindow
                Settings.Default.Last_MainTabLeft = this.tbcLeftPaneTabs.SelectedIndex;
                Settings.Default.Last_MainTabRight = (this.tbcRightPaneTabs.TabCount > 0) ? this.tbcRightPaneTabs.SelectedIndex : -1;
                Settings.Default.Form_2PaneImgIndices = "";
                foreach (TabPage tab in this.tbcRightPaneTabs.TabPages) {
                    //右ペインにあるタブを記憶する
                    Settings.Default.Form_2PaneImgIndices += tab.ImageIndex + Resources.Split_SimpleList;
                }
                Settings.Default.Form_2PaneImgIndices = Common.CutLastChar(Settings.Default.Form_2PaneImgIndices, Resources.Split_SimpleList.ToCharArray()[0]);

                //テキストエディター
                Settings.Default.Last_TextPath = this.uctlTextEditor.uctlTextFiles.SelectedNodeFullPath;

                //データベース
                Settings.Default.Last_DB = this.uctlDBEditor.tbcDatabaseTab.SelectedIndex;

                //マップエディター
                Settings.Default.Last_MapPath = this.uctlMapEditor.uctlMapTree.SelectedNodeFullPath;
            }

            //DXライブラリ・GuruGuruSMFの破棄
            Media.Dispose();

            //ウィンドウ関連の設定を保存
            if (this.WindowState != FormWindowState.Minimized) {
                Settings.Default.Form_MainWndState = this.WindowState;
                Settings.Default.Form_MainLocation = this.Location;
                if (this.WindowState == FormWindowState.Normal) {
                    Settings.Default.Form_MainSize = this.Size;
                }
            }

            Settings.Default.Save();
        }

        /// <summary>
        /// バックグラウンドでスプラッシュスクリーンを表示する
        /// </summary>
        private async void showSplashScreeen() {
            await Task.Run(() => {
                this.SplashScreen.Show();
                this.SplashCloseFlag = false;
                while (!this.SplashCloseFlag) {
                    //処理中にもウィンドウが表示されるようにメッセージ処理を継続する
                    Application.DoEvents();
                    System.Threading.Thread.Sleep(10);
                }
                System.Threading.Thread.Sleep(50);
                this.SplashScreen.Close();
            });
            this.Activate();
        }

        /// <summary>
        /// メインタブ切替時
        /// </summary>
        private void tbcLeftPaneTabs_Selecting(object sender, TabControlCancelEventArgs e) {
            GC.Collect();
        }

        /// <summary>
        /// 編集中のプロジェクトの有無に関わるコントロール有効無効
        /// </summary>
        private void ctlSwitchMainMenu(bool isEnabled) {
            this.mnuProjectClose.Visible = isEnabled;
            this.mnuProjectFolderOpen.Visible = isEnabled;
            this.mnuProjectSave.Visible = isEnabled;
            this.mnuProjectSettings.Visible = isEnabled;
            this.mnuProjectSettingsReload.Visible = isEnabled;
            this.mnuPublish.Visible = isEnabled;
            this.mnuProjectFolderOpen.Visible = isEnabled;
            this.mnuTestPlay.Visible = isEnabled;
            this.mnuGarbageCleaner.Visible = isEnabled;
            this.mnuProjectSplit1.Visible = isEnabled;
            this.mnuProjectSplit2.Visible = isEnabled;

            this.mnuView2Pane.Enabled = isEnabled;

            this.mnuToolGraphicExpand.Enabled = isEnabled;
            this.mnuToolGraphicTrans.Enabled = isEnabled;
            this.mnuToolRPGTkoolConvert.Enabled = isEnabled;
            this.mnuToolSoundLoop.Enabled = isEnabled;

            //初回プロジェクト起動時にメインタブが隠れるバグを防ぐ
            this.mspMainMenu.SendToBack();
        }

        /// <summary>
        /// すべてのエディターに保存指示を出します。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public ConfirmResult AllSave(bool confirm) {
            //一括変更確認
            var NG = false;
            var mainTabs = this.GetMainTabList();
            foreach (var page in mainTabs) {
                if (((CtrlComponent.Common.ctlEditorParent) page.Controls[0]).IsDirty) {
                    NG = true;
                    break;
                }
            }
            if (!NG) {
                //変更がなければそのまま終了
                return ConfirmResult.Completed;
            }

            if (confirm) {
                //確認を行う
                switch (MessageBox.Show(Resources.MsgQ_Save.Replace("$", "プロジェクト"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question)) {
                    case DialogResult.No:
                        //ユーザーが保存しなくていいといったので、システム的には保存したとみなす
                        return ConfirmResult.Completed;

                    case DialogResult.Cancel:
                        //呼び出し元の処理もキャンセルしなければならない
                        return ConfirmResult.Canceled;
                }
            }

            //一括保存処理
            NG = false;
            foreach (var page in mainTabs) {
                NG &= ((CtrlComponent.Common.ctlEditorParent) page.Controls[0]).DoSave();
            }

            //エラーが起きた場合は一部の処理がスキップしたことになる
            return NG ? ConfirmResult.Skipped : ConfirmResult.Completed;
        }

        /// <summary>
        /// すべてのエディターに閉じる指示を出します。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public ConfirmResult AllClose(bool confirm) {
            var mainTabs = this.GetMainTabList();

            //閉じる前に保存を行う
            if (this.AllSave(confirm) == ConfirmResult.Canceled) {
                //キャンセルを要求されたら以降の処理を行わない
                return ConfirmResult.Canceled;
            }

            //一括クローズ処理
            foreach (var page in mainTabs) {
                ((CtrlComponent.Common.ctlEditorParent) page.Controls[0]).DoClose();
            }
            return ConfirmResult.Completed;
        }

        /// <summary>
        /// すべてのエディターに不要なファイルの除去を指示します。
        /// </summary>
        public void AllCleanup() {
            var mainTabs = this.GetMainTabList();

            //一括クリーンアップ処理
            foreach (var page in mainTabs) {
                ((CtrlComponent.Common.ctlEditorParent) page.Controls[0]).DoCleanup();
            }
        }

        /// <summary>
        /// すべてのメインタブのリストを返します。
        /// </summary>
        /// <returns></returns>
        private List<TabPage> GetMainTabList() {
            //左右ペインからタブを集める
            var list = new List<TabPage>();
            foreach (TabPage lp in this.tbcLeftPaneTabs.TabPages) {
                list.Add(lp);
            }
            foreach (TabPage rp in this.tbcRightPaneTabs.TabPages) {
                list.Add(rp);
            }

            //正規の順序に直す
            list.Sort((x, y) => {
                return x.ImageIndex - y.ImageIndex;
            });

            return list;
        }

        /// <summary>
        /// 各種エディターによって指定のスクリプトを２ペイン表示で開くように指示を受けたときの処理
        /// </summary>
        public void RequestOpenScriptEventHandler(object sender, RequestOpenFileEventArgs e) {
            this.mnuView2Pane.Checked = true;

            //同じペインに両者のタブが含まれている場合は現在のタブを移動する
            this.uctlTextEditor.uctlTextFiles.ReloadTree(e.FileName);
            this.uctlTextEditor.uctlOneText.azkEditor.Focus();
            switch (this.CheckTabLRByImageIndex((int) e.TabIndex, (int) MainTabIndices.Text)) {
                case LR.Left:
                    this.mnuViewPainToRight_Click(sender, null);
                    break;
                case LR.Right:
                    this.mnuViewPainToLeft_Click(sender, null);
                    break;
            }

            this.SuspendLayout();
            this.SelectMainTab(e.TabIndex);             //元のエディタータブを開く
            this.SelectMainTab(MainTabIndices.Text);    //スクリプトタブを開く
            this.ResumeLayout();
        }

        /// <summary>
        /// ２つのタブが同じタブコントロールにあるかを調べます。
        /// </summary>
        private LR CheckTabLRByImageIndex(int imageIndexA, int imageIndexB) {
            bool haveA, haveB;

            //左タブで調べる
            haveA = haveB = false;
            foreach (TabPage tab in this.tbcLeftPaneTabs.TabPages) {
                if (tab.ImageIndex == imageIndexA) {
                    haveA = true;
                } else if (tab.ImageIndex == imageIndexB) {
                    haveB = true;
                }
            }
            if (haveA && haveB) {
                return LR.Left;
            }

            //右タブで調べる
            haveA = haveB = false;
            foreach (TabPage tab in this.tbcRightPaneTabs.TabPages) {
                if (tab.ImageIndex == imageIndexA) {
                    haveA = true;
                } else if (tab.ImageIndex == imageIndexB) {
                    haveB = true;
                }
            }
            if (haveA && haveB) {
                return LR.Right;
            }

            return LR.Both;
        }

        /// <summary>
        /// 指定したタブを開きます。
        /// </summary>
        private void SelectMainTab(MainTabIndices index) {
            //左タブで調べる
            foreach (TabPage tab in this.tbcLeftPaneTabs.TabPages) {
                if (tab.ImageIndex == (int) index) {
                    this.tbcLeftPaneTabs.SelectedTab = tab;
                    return;
                }
            }

            //右タブで調べる
            foreach (TabPage tab in this.tbcRightPaneTabs.TabPages) {
                if (tab.ImageIndex == (int) index) {
                    this.tbcRightPaneTabs.SelectedTab = tab;
                    return;
                }
            }
        }
        #endregion

        #region <上部メニュー>
        /// <summary>
        /// プロジェクト：新規プロジェクトを作成
        /// </summary>
        private void mnuProjectNew_Click(object sender, EventArgs e) {
            //編集中のプロジェクトがある場合、確認した上で保存する
            if (this.Project != null && this.Project.Save(true) != ConfirmResult.Completed) {
                //保存に失敗した場合は中断する
                return;
            }

            var Dlg = new Dialog.Common.dlgNewProject();
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                // サインアップした状態でプロジェクトを作成する
                this.CloseProject();
                Dlg.GetResult(out var path, out var pName, out var loadPreset);
                this.Project = new ProjectManager(this, path, pName, loadPreset);
            }
        }

        /// <summary>
        /// プロジェクト：既存プロジェクトを開く
        /// </summary>
        private void mnuProjectOpen_Click(object sender, EventArgs e) {
            //編集中のプロジェクトがある場合、確認した上で保存する
            if (this.Project != null && this.Project.Save(true) != ConfirmResult.Completed) {
                //保存に失敗した場合は中断する
                return;
            }

            this.folderDlg.Description = "プロジェクトが含まれるフォルダーを指定して下さい。";
            if (this.folderDlg.ShowDialog(this) == DialogResult.OK) {
                //正規のプロジェクトフォルダーであるか検査
                if (!ProjectManager.CheckProjectInfo(this.folderDlg.SelectedPath)) {
                    MessageBox.Show(Resources.MsgE_InvaridPrj, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                var project = new ProjectManager(this, this.folderDlg.SelectedPath);
                project = ProjectManager.LoadProjectFile(project);
                this.CloseProject();
                this.Project = project;
            }
        }

        /// <summary>
        /// プロジェクト：すべて保存
        /// </summary>
        private void mnuProjectSave_Click(object sender, EventArgs e) {
            if (this.Project?.Save(false) == ConfirmResult.Completed) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// プロジェクト：閉じる
        /// </summary>
        private void mnuProjectClose_Click(object sender, EventArgs e) {
            //編集中のプロジェクトがある場合、確認した上で保存する
            if (this.Project?.Save(true) != ConfirmResult.Completed) {
                //保存に失敗した場合は中断する
                return;
            }
            this.CloseProject();
        }

        /// <summary>
        /// プロジェクト：設定ファイルを再読込
        /// </summary>
        private void mnuProjectSettingsReload_Click(object sender, EventArgs e) {
            this.Project = ProjectManager.LoadProjectFile(this.Project);
        }

        /// <summary>
        /// プロジェクト：基準フォルダーをエクスプローラーで開く
        /// </summary>
        private void mnuProjectFolderOpen_Click(object sender, EventArgs e) {
            try {
                System.Diagnostics.Process.Start(Common.CutLastChar(ProjectManager.ProjectPath, '\\'));
            } catch {
                MessageBox.Show("エラーが発生したため、フォルダーを開くことができませんでした。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// プロジェクト：プロジェクト設定
        /// </summary>
        private void mnuProjectSettings_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgEditSettings(this.Project, false) {
                Text = "プロジェクトの設定",
                ControlBox = false         //変更が即時反映されるためOKボタンを必ず経由させる
            };
            Dlg.ShowDialog(this);
            ProjectManager.SaveProjectFile(this.Project);

            // プロジェクト開き直してタイトルを修正する
            var project = this.project;
            this.CloseProject();
            this.Project = project;
        }

        /// <summary>
        /// プロジェクト：不要ファイル除去
        /// </summary>
        private void mnuGarbageCleaner_Click(object sender, EventArgs e) {
            //ダイアログを開く前にファイル編集権をすべて破棄する
            if (this.AllClose(true) == ConfirmResult.Completed) {
                this.Project.CleanGarbage();
            }

            MessageBox.Show("プロジェクト内の不要なファイルの削除が完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
        }

        /// <summary>
        /// プロジェクト：製品版として発行
        /// </summary>
        private void mnuPublish_Click(object sender, EventArgs e) {
            //ダイアログを開く前にファイル編集権をすべて破棄する
            if (this.AllClose(true) == ConfirmResult.Completed) {
                var Dlg = new Dialog.Common.dlgPublish(this.Project);
                Dlg.ShowDialog();
            }
        }

        /// <summary>
        /// プロジェクト：タイトル画面からテストプレイ
        /// </summary>
        private void mnuTestPlay_Click(object sender, EventArgs e) {
            Common.RunTester("", false);
        }

        /// <summary>
        /// 表示：２ペイン表示切替
        /// </summary>
        private void mnuView2Pane_CheckedChanged(object sender, EventArgs e) {
            this.SuspendLayout();

            var selectedTab = this.tbcLeftPaneTabs.SelectedTab;
            this.splMain1.Panel2Collapsed = !this.mnuView2Pane.Checked;
            this.mnuViewPainToLeft.Enabled = this.mnuView2Pane.Checked;
            this.mnuViewPainToRight.Enabled = this.mnuView2Pane.Checked;

            if (this.mnuView2Pane.Checked) {
                this.splMain1.BorderStyle = BorderStyle.Fixed3D;     //ペイン同士の境界線を明確にできるが、同時に上部メニューにも境界線ができて鬱陶しくなる
                this.splMain1.SplitterDistance = this.splMain1.Size.Width / 2;
                if (this.tbcRightPaneTabs.TabCount == 0) {
                    //右タブにまだ一つもない場合は現在のタブを移動させる
                    this.tbcRightPaneTabs.TabPages.Add(selectedTab);
                }
            } else if (this.mnuView2Pane.Checked == false && this.tbcRightPaneTabs.TabCount > 0) {
                this.splMain1.BorderStyle = BorderStyle.None;        //ペイン同士の境界線が必要ないため、上部メニューにできた境界線を消して自然にする
                foreach (TabPage page in this.tbcRightPaneTabs.TabPages) {
                    this.tbcLeftPaneTabs.TabPages.Add(page);
                }
                Common.SortTabByImageIndex(this.tbcLeftPaneTabs.TabPages);    //正規の並び順に戻す
            }

            this.ResumeLayout();
        }

        /// <summary>
        /// 表示：右ペインへ現在のタブを移動させる
        /// </summary>
        private void mnuViewPainToRight_Click(object sender, EventArgs e) {
            this.SuspendLayout();

            if (this.mnuView2Pane.Checked) {
                if (this.tbcLeftPaneTabs.TabCount <= 1) {
                    //左ペインのタブがゼロになるときは１ペインにする
                    this.mnuView2Pane.Checked = false;
                } else {
                    //隣に移動する
                    this.tbcRightPaneTabs.TabPages.Add(this.tbcLeftPaneTabs.SelectedTab);
                }
            }

            this.ResumeLayout();
        }

        /// <summary>
        /// 表示：左ペインへ現在のタブを移動させる
        /// </summary>
        private void mnuViewPainToLeft_Click(object sender, EventArgs e) {
            this.SuspendLayout();

            if (this.mnuView2Pane.Checked) {
                if (this.tbcRightPaneTabs.TabCount <= 1) {
                    //右ペインのタブがゼロになるときは１ペインにする
                    this.mnuView2Pane.Checked = false;
                } else {
                    //隣に移動する
                    this.tbcLeftPaneTabs.TabPages.Add(this.tbcRightPaneTabs.SelectedTab);
                }
            }

            this.ResumeLayout();
        }

        /// <summary>
        /// 表示：片方のペインを一定以上小さく寄せたら２ペイン表示を解除する
        /// </summary>
        private void splMain1_SplitterMoved(object sender, SplitterEventArgs e) {
            this.SuspendLayout();

            var selectIndex = 0;
            if (this.splMain1.Panel1.Width < MinWidthPane) {
                //右ペインのタブを選択状態にする
                selectIndex = this.tbcRightPaneTabs.SelectedTab.ImageIndex;
                this.mnuView2Pane.Checked = false;
            } else if (this.splMain1.Panel2.Width < MinWidthPane) {
                selectIndex = this.tbcLeftPaneTabs.SelectedTab.ImageIndex;
                this.mnuView2Pane.Checked = false;
            } else {
                //単なる境界線移動
                return;
            }

            //改めて現在のタブを選択する
            this.tbcLeftPaneTabs.SelectedIndex = selectIndex;

            this.ResumeLayout();
        }

        /// <summary>
        /// ツール：サウンドループ設定
        /// </summary>
        private void mnuToolSoundLoop_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgToolSoundLoop();
            Dlg.ShowDialog(this);
        }

        /// <summary>
        /// ツール：画像リサイズ
        /// </summary>
        private void mnuToolGraphicExpand_Click(object sender, EventArgs e) {
            //ソースファイル選択が先行する
            this.openFileDlg.Filter = Resources.Filter_Graphics;
            if (this.openFileDlg.ShowDialog(this) == DialogResult.OK) {
                var Dlg = new Dialog.Common.dlgToolPictureResize(this.openFileDlg.FileName, Path.GetFileName(this.openFileDlg.FileName));
                Dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// ツール：画像透過
        /// </summary>
        private void mnuToolGraphicTrans_Click(object sender, EventArgs e) {
            //ソースファイル選択が先行する
            this.openFileDlg.Filter = Resources.Filter_Graphics;
            if (this.openFileDlg.ShowDialog(this) == DialogResult.OK) {
                var Dlg = new Dialog.Common.dlgToolPictureTrans(this.openFileDlg.FileName, Path.GetFileNameWithoutExtension(this.openFileDlg.FileName) + ".png");
                Dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// ツール：オートタイル変換
        /// </summary>
        private void mnuToolAutotileConvert_Click(object sender, EventArgs e) {
            //ソースファイル選択が先行する
            var tileSize = int.Parse(this.uctlDBEditor.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.TileSize, 0].Value?.ToString());
            this.openFileDlg.Filter = Resources.Filter_Graphics;
            if (this.openFileDlg.ShowDialog(this) == DialogResult.OK) {
                var Dlg = new Dialog.Common.dlgToolConvertTkoolMapTile(this.openFileDlg.FileName, Path.GetFileNameWithoutExtension(this.openFileDlg.FileName) + ".png", tileSize);
                Dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// ツール：歩行グラフィック変換
        /// </summary>
        private void mnuToolWalkConvert_Click(object sender, EventArgs e) {
            //ソースファイル選択が先行する
            var oneCharTile = (Rectangle) (this.uctlDBEditor.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.DBCharGraphicsViewRange, 0].Value);
            this.openFileDlg.Filter = Resources.Filter_Graphics;
            if (this.openFileDlg.ShowDialog(this) == DialogResult.OK) {
                var Dlg = new Dialog.Common.dlgToolConvertTkoolWalkChar(this.openFileDlg.FileName, Path.GetFileNameWithoutExtension(this.openFileDlg.FileName) + ".png", oneCharTile.Size);
                Dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// ツール：アプリケーション環境設定
        /// </summary>
        private void mnuSettings_Click(object sender, EventArgs e) {
            var setCtrl = new AppSettings();
            var Dlg = new Dialog.Common.dlgEditSettings(setCtrl, true);
            Dlg.Importing += (sub_sender, sub_e) => {
                AppSettings.Import(ref setCtrl);
                Dlg.SetTargetObject(setCtrl);
            };
            Dlg.Exporting += (sub_sender, sub_e) => {
                setCtrl.Export();
            };
            Dlg.Text = "アプリケーション環境設定";
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                setCtrl.Apply();
                this.uctlTextEditor.uctlOneText.ApplySettings();
            }
        }

        /// <summary>
        /// ヘルプ：マニュアルを開く
        /// </summary>
        private void mnuManual_Click(object sender, EventArgs e) {
            if (this.manualForm.IsDisposed) {
                //破棄されているときはフォームを作り直す
                this.manualForm = new Dialog.Common.dlgManual();
                this.manualForm.Show(this);
            } else if (!this.manualForm.Visible) {
                //インスタンスが残っているときは再表示する
                this.manualForm.Show();
            } else {
                //既に表示されているときはアクティブにする
                this.manualForm.Activate();
            }
        }

        /// <summary>
        /// ヘルプ：バージョン情報
        /// </summary>
        private void mnuVersion_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgVersion();
            Dlg.ShowDialog(this);
        }
        #endregion

        #region <マップエディター>
        /// <summary>
        /// MainWndがすべてのキー入力情報を捕捉し、マップエディターにキー入力情報を伝える
        /// </summary>
        private void MainWindow_KeyDown(object sender, KeyEventArgs e) {
            //uctlMapEditorが表示されていれば、キー情報を送信する
            if (this.uctlMapEditor.Visible) {
                this.uctlMapEditor.pnlPreview_PreviewKeyDown(sender, new PreviewKeyDownEventArgs(e.KeyData));
            }
        }
        #endregion

    }
}
