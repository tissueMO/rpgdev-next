using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Map {
    using Module;
    using Properties;
    using EVData = CtrlComponent.Map.mgrMapObject.MapOneData.EventOneData;

    public partial class dlgEVSettings : Form {
        private readonly string MapGUID;
        private readonly CtrlComponent.Text.ctlCommandTree comTree;
        private readonly CtrlComponent.Database.ctlDatabaseEditor dbCtl;
        private readonly Control comTreeParent;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEVSettings(EVData ev, string mapGUID, CtrlComponent.Text.ctlCommandTree comTree, CtrlComponent.Database.ctlDatabaseEditor dbCtl) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.Result = ev;
            this.MapGUID = mapGUID;
            this.comTree = comTree;
            this.comTreeParent = comTree.Parent;            //元々コマンドツリーを格納していたコントロールを覚えておく
            this.comTree.RequestInsertCode += this.uctlComTree_RequestInsertCode;
            this.comTree.EnabledEasyEditorTools = true;     //Easyエディターツール群を有効にする
            this.dbCtl = dbCtl;

            //NOTE: イベント情報 IN
            //初期設定
            this.numID.Value = ev.VisibleID;
            this.txtName.Text = ev.Name;
            this.uctlPosition.Maximum = new Point(Map.MaxMapSize);
            this.uctlPosition.Result = ev.TilePosition;

            //イベント情報をもとにセットする
            foreach (var page in ev.Pages) {
                if (this.addPage()) {
                    ((CtrlComponent.Map.ctlEVPage) this.tbcEVPages.TabPages[this.tbcEVPages.TabCount - 1].Controls[0]).SetEVPageData(page.Clone() as EVData.EventOnePage);
                }
            }
            var ctl = (this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage);
            ctl.uctlEVCommandEditor.Setup(comTree);
            ctl.uctlTextEditor.Setup(dbCtl);
            this.tbcEVPages_Selecting(this, new TabControlCancelEventArgs(this.tbcEVPages.SelectedTab, this.tbcEVPages.SelectedIndex, false, TabControlAction.Selecting));
        }

        /// <summary>
        /// ウィンドウ表示時
        /// </summary>
        private void dlgEVSettings_Load(object sender, EventArgs e) {
            //テンプレートから作成したとき、呼び出し元（テンプレート作成ダイアログ）を非表示にする
            if (this.Tag?.GetType() == typeof(dlgEVTemplates)) {
                ((Form) this.Tag).Hide();
            }
            this.Activate();
        }

        /// <summary>
        /// ウィンドウ消去時
        /// </summary>
        private void dlgEVSettings_FormClosing(object sender, FormClosingEventArgs e) {
            //借りたものは返す（コマンドツリー）
            this.comTree.EnabledEasyEditorTools = false;
            this.comTreeParent.Controls.Add(this.comTree);
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public EVData Result {
            get {
                return this.ev;
            }
            private set {
                this.ev = value;
            }
        }
        private EVData ev;

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void uctlOKCancelApply_ClickOK(object sender, EventArgs e) {
            //NOTE: イベント情報 OUT
            //現在のイベントオブジェクトに現在の情報を格納する
            this.ev.VisibleID = (int) this.numID.Value;
            this.ev.Name = this.txtName.Text;
            this.ev.TilePosition = this.uctlPosition.Result;
            this.ev.dxHandle = -1;
            this.ev.Pages.Clear();

            for (var i = 0; i < this.tbcEVPages.TabCount; i++) {
                var ctlPage = this.tbcEVPages.TabPages[i].Controls[0] as CtrlComponent.Map.ctlEVPage;
                ctlPage.ApplyCurrentScript();       //編集中のスクリプトを確定させる
                this.ev.Pages.Add(ctlPage.GetEVPageData());
            }
        }

        /// <summary>
        /// 意図しないキャンセルでないか問い合わせる
        /// </summary>
        private void ctlOKCancel1_ClickCancel(object sender, EventArgs e) {
            //エディターを取得する
            IDirtyable obj = null;
            if (!Settings.Default.Map_SQEasyMode) {
                obj = ((this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage).uctlTextEditor);
            } else {
                obj = ((this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage).uctlEVCommandEditor);
            }

            //変更を破棄しても良いか確認し、破棄する場合は IsDirty を解除する
            if (!obj.IsDirty) {
                return;
            } else if (MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "編集内容を破棄"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning) != DialogResult.Yes) {
                this.DialogResult = DialogResult.None;
            } else {
                obj.IsDirty = false;
            }
        }

        /// <summary>
        /// タブが切り替わるときにコマンドツリーとテキストエディターを移動させる
        /// </summary>
        private void tbcEVPages_Selecting(object sender, TabControlCancelEventArgs e) {
            this.SuspendLayout();
            var ctl = this.tbcEVPages.TabPages[e.TabPageIndex].Controls[0] as CtrlComponent.Map.ctlEVPage;

            if (ctl != null) {
                ctl.uctlTextEditor.FoundCommandNode += this.uctlOneText_FoundCommandNode;
                ctl.pnlCommandTree.Controls.Add(this.comTree);
                ctl.uctlEVCommandEditor.Setup(this.comTree);
                ctl.uctlTextEditor.Setup(this.dbCtl);
            }

            this.ResumeLayout();
        }

        /// <summary>
        /// コマンドツリーからのコマンド挿入を受け付ける
        /// </summary>
        private void uctlComTree_RequestInsertCode(object sender, CtrlComponent.Text.ctlCommandTree.RequestInsertCodeEventArgs e) {
            if (this.comTree.TopLevelControl as dlgEVSettings == null) {
                //コマンドツリーが出張中の場合は処理しない
                return;
            }

            //編集先にコマンドを挿入する
            (this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage).InsertCommand(e.Code, e.ForbiddenCRLF);
        }

        /// <summary>
        /// 要求を受けたコマンドツリーを選択する
        /// </summary>
        private void uctlOneText_FoundCommandNode(object sender, CtrlComponent.Text.ctlOneTextEditor.FoundCommandNodeEventArgs e) {
            Common.FoundCommandNode(
                (this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage).uctlTextEditor,
                this.comTree,
                (this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage).uctlTextEditor.DBCtrl.DBList,
                e);
            if (!Settings.Default.Map_SQEasyMode) {
                ((this.tbcEVPages.SelectedTab.Controls[0] as CtrlComponent.Map.ctlEVPage).uctlTextEditor).Focus();
            }
        }

        /// <summary>
        /// 画面上のイベントページを追加します。
        /// これに合わせて内部データ上のページも追加して下さい。
        /// 返値は成功したかどうかを示します。
        /// </summary>
        private bool addPage() {
            if (this.tbcEVPages.TabCount >= Map.MaxEVPageCount) {
                MessageBox.Show("イベントページの限界数に達しています。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            var ctlPage = new CtrlComponent.Map.ctlEVPage();
            this.tbcEVPages.TabPages.Add($"ページ {this.tbcEVPages.TabCount + 1}");
            this.tbcEVPages.TabPages[this.tbcEVPages.TabCount - 1].Controls.Add(ctlPage);
            ctlPage.Dock = DockStyle.Fill;
            this.tbcEVPages.SelectedIndex = this.tbcEVPages.TabCount - 1;

            return true;
        }

        /// <summary>
        /// イベントページ新規
        /// </summary>
        private void btnPageNew_Click(object sender, EventArgs e) {
            if (this.addPage()) {
                this.ev.Pages.Add(new EVData.EventOnePage());
                ((CtrlComponent.Map.ctlEVPage) this.tbcEVPages.TabPages[this.tbcEVPages.TabCount - 1].Controls[0]).SetEVPageData(this.ev.Pages[this.ev.Pages.Count - 1]);
            }
        }

        /// <summary>
        /// イベントページ複製
        /// </summary>
        private void btnPageCopy_Click(object sender, EventArgs e) {
            if (this.tbcEVPages.TabCount >= Map.MaxEVPageCount) {
                MessageBox.Show("イベントページの限界数に達しています。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            var ctlPage = new CtrlComponent.Map.ctlEVPage();
            ctlPage.SetEVPageData(((CtrlComponent.Map.ctlEVPage) this.tbcEVPages.SelectedTab.Controls[0]).GetEVPageData().Clone() as EVData.EventOnePage);
            this.tbcEVPages.TabPages.Add($"ページ {this.tbcEVPages.TabCount + 1}");
            this.tbcEVPages.TabPages[this.tbcEVPages.TabCount - 1].Controls.Add(ctlPage);
            ctlPage.Dock = DockStyle.Fill;
            this.tbcEVPages.SelectedIndex = this.tbcEVPages.TabCount - 1;
        }

        /// <summary>
        /// イベントページ削除
        /// </summary>
        private void btnPageDelete_Click(object sender, EventArgs e) {
            if (this.tbcEVPages.TabCount <= 1) {
                MessageBox.Show("イベントページは少なくとも１つは必要です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            var index = this.tbcEVPages.SelectedIndex;
            this.tbcEVPages.TabPages.RemoveAt(this.tbcEVPages.SelectedIndex);

            //隣のタブに逃げる
            if (this.tbcEVPages.TabCount <= index) {
                this.tbcEVPages.SelectedIndex = this.tbcEVPages.TabCount - 1;
            } else {
                this.tbcEVPages.SelectedIndex = index;
            }

            //ページ番号を振り直す
            for (var i = 0; i < this.tbcEVPages.TabCount; i++) {
                this.tbcEVPages.TabPages[i].Text = $"ページ {i + 1}";
            }
        }

        /// <summary>
        /// 現在のイベントをテンプレートとして登録
        /// </summary>
        private void btnAddToTemplate_Click(object sender, EventArgs e) {
            //イベントテンプレートは配置関数とイベント関数をセットにして書き出す
            var Dlg = new Dialog.Common.dlgInputTextValue(Resources.Path_PrjSys_EVTemplate, false, true, Resources.Extension_SQTemplate, Resources.Extension_SQTemplate);
            if (Dlg.ShowDialog(this) != DialogResult.OK) {
                return;
            }

            //実際に登録する
            this.uctlOKCancelApply_ClickOK(sender, e);      //編集内容を確定させる
            using (var W = new StreamWriter(Common.OpenFileWriteOnly(Dlg.FullPath), Common.SJIS)) {
                //イベントを配置する関数
                W.WriteLine(Resources.SQ_EVSetFunc + " {");
                this.ev.DoSaveEVSetFunction(W);
                W.WriteLine("}");
                W.WriteLine();
                this.ev.DoSaveEVFunction(W);
            }
            MessageBox.Show("現在編集中のイベントをテンプレートとして登録しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
        }

        /// <summary>
        /// 取扱説明書を開く
        /// </summary>
        private void dlgEVSettings_HelpButtonClicked(object sender, CancelEventArgs e) {
            var Dlg = new Dialog.Common.dlgManual();
            Dlg.Show(this);
            e.Cancel = true;
        }
    }
}
