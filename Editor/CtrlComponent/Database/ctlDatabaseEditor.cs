using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    /// <summary>
    /// すべてのデータベースを統括するクラスです。
    /// </summary>
    public partial class ctlDatabaseEditor : CtrlComponent.Common.ctlEditorParent {
        public AllDB DBList = new AllDB();

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDatabaseEditor() {
            this.InitializeComponent();

            //すべてのデータベースをセットアップする
            DBLiteral.SetupDatabases(this.tbcDatabaseTab, this.DBList);
            var dbList = Common.GetControlsOfType(this, typeof(DataGridView));
            Database.AllDBSetup(dbList, this);

            //各DBにイベントハンドラーをセットする
            foreach (TabPage tab in this.tbcDatabaseTab.TabPages) {
                if (tab.Controls[0] is ctlDBBase) {
                    ((ctlDBBase) tab.Controls[0]).UndoRedoBufferChanged += this.UndoRedoBufferChanged;
                }
            }

            //UserDBリストの選択状態が変更されたときの挙動を追加的に定義する
            ((ctlDBGUIUserDB) this.DBList[Database.DBIndices.UserDB]).UserDBSelected += this.userDBselectedIndexChanged;
        }

        /// <summary>
        /// 現在アクティブなデータベースの管理クラスを返します。
        /// このプロパティはMainWindowクラスから参照されることを想定します。
        /// </summary>
        public ctlDBBase CurrentDB {
            get {
                return this.DBList[(Database.DBIndices) this.tbcDatabaseTab.SelectedIndex];
            }
        }

        /// <summary>
        /// すべて読み込み
        /// </summary>
        public void DoLoad() {
            //競合先に移行している可能性も考慮して、元のファイルで読み直す
            foreach (var db in this.DBList.Values) {
                db.FileName = db.OriginalFileName;
                db.DoLoad();
            }

            //他のDBに依存する部分を適用
            this.SetupDependColumnAll();

            //すべて読み込んだ状態は編集されていない状態...とは限らないので、新規作成フラグが立っていたら変更状態とする
            foreach (var db in this.DBList.Values) {
                db.IsDirty = db.NewCreated;
                db.EnabledUndoBuffer = true;
            }
        }

        public override bool DoSave() {
            var OK = true;

            foreach (var db in this.DBList) {
                //他のデータベースに依存する部分を反映
                this.applyToDependColumn(db.Value);
                if (!db.Value.IsDirty) {
                    //変更がない場合は保存しない
                    continue;
                }

                //子DBは各コントロールの中で保存を行う
                OK &= db.Value.DoSave();
            }

            return OK;
        }

        public override bool Jump(string rPath) {
            if (rPath.IndexOf(Resources.Path_DBs) == -1) {
                return false;
            }

            //当該ファイルを管轄するデータベースを探し、その中で処理を行う
            for (var i = 0; i < this.tbcDatabaseTab.TabCount; i++) {
                var db = this.tbcDatabaseTab.TabPages[i].Controls[0] as ctlDBBase;
                if (db.IsManagedFile(rPath)) {
                    //該当データベースの中で項目を選択できる場合は実行する
                    this.tbcDatabaseTab.SelectedIndex = i;
                    db.OpenManagedFile(rPath);
                    return true;
                }
            }

            return false;
        }

        public override void DoCleanup() {
            foreach (var db in this.DBList) {
                db.Value.Cleanup();
            }
        }

        /// <summary>
        /// 管轄しているデータベースが一つでも変更されているかどうか
        /// </summary>
        public override bool IsDirty {
            get {
                var dirty = false;
                foreach (var db in this.DBList) {
                    dirty |= db.Value.IsDirty;
                }
                return dirty;
            }
        }

        /// <summary>
        /// データベースのロードが終わった後、全てのデータベースを対象に、他のデータベースに依存する列の表示を復元します。
        /// </summary>
        private void SetupDependColumnAll() {
            //全データベースを走査
            for (var i = 0; i < Common.GetEnumCount<Database.DBIndices>(); i++) {
                this.SetupDependColumn((Database.DBIndices) i);
            }
        }

        /// <summary>
        /// 他のデータベースに依存する列の表示を復元します。
        /// </summary>
        public void SetupDependColumn(Database.DBIndices index) {
            //各サブデータベースを走査
            var dbList = this.DBList[index].DBs;
            var isRemoved = false;

            foreach (var subdb in dbList) {
                //各列を走査
                for (var i = 0; i < subdb.Columns.Count; i++) {
                    var colInfo = (subdb.Columns[i]?.Tag as DBColumn);
                    if (colInfo == null) {
                        continue;
                    }
                    isRemoved |= colInfo.RestoreDependColumn(subdb.Rows, subdb.Columns[i]);
                }
            }

            this.DBList[index].IsDirty = isRemoved;
            this.DBList[index].NewCreated = isRemoved;
        }

        /// <summary>
        /// サブデータベースも含めたすべてのデータベースを列挙します。
        /// </summary>
        public Dictionary<Database.DBAddress, DataGridView> GetAllDBList() {
            return GetAllDBList(this.DBList);
        }
        public static Dictionary<Database.DBAddress, DataGridView> GetAllDBList(AllDB dbList) {
            var list = new Dictionary<Database.DBAddress, DataGridView>();

            for (var mainID = 0; mainID < dbList.Count; mainID++) {
                var DBs = dbList[(Database.DBIndices) mainID].DBs;
                for (var subID = 0; subID < DBs.Length; subID++) {
                    //KeyにはDataGridViewへのアドレスを格納する
                    list.Add(new Database.DBAddress((Database.DBIndices) mainID, subID), DBs[subID]);
                }
            }

            return list;
        }

        /// <summary>
        /// データベースのアドレスからデータベースのタイトルを返します。
        /// </summary>
        /// <param name="FormatID">DataGridViewへのアドレス [ID;SubID]</param>
        /// <returns>データベースのタイトル</returns>
        public string GetDBName(Database.DBAddress address) {
            return this.DBList[address.MainID]?.DBs[address.SubID]?.Tag?.ToString();
        }
        public static string GetDBName(AllDB dbList, Database.DBAddress address) {
            return dbList[address.MainID]?.DBs[address.SubID]?.Tag?.ToString();
        }

        /// <summary>
        /// FixedIDから該当するデータベースを検索します。
        /// </summary>
        public List<Tuple<string, DataGridView>> FindDBRow(int fixedID) {
            var result = new List<Tuple<string, DataGridView>>();

            //すべてのデータベースを走査
            for (var mainID = 0; mainID < this.DBList.Count; mainID++) {
                var DBs = this.DBList[(Database.DBIndices) mainID].DBs;

                //すべてのサブデータベースを走査	
                for (var subID = 0; subID < DBs.Length; subID++) {
                    var IDName = Database.GetIDNameFromFixedID(DBs[subID], fixedID);
                    if (!string.IsNullOrEmpty(IDName)) {
                        //ヒットしたらリストに追加する
                        result.Add(new Tuple<string, DataGridView>(IDName, DBs[subID]));
                    }
                }
            }

            return result;
        }

        /// <summary>
        /// 現在の状態を指定したデータベースに反映します。
        /// この処理では、依存している部分が更新されます。
        /// </summary>
        private void applyToDependColumn(ctlDBBase tgtDB) {
            foreach (var db in tgtDB.DBs) {
                foreach (DataGridViewColumn col in db.Columns) {
                    var colInfo = col.Tag as DBColumn;
                    if (colInfo != null) {
                        colInfo.RestoreDependColumn(db.Rows, col);
                    }
                }
            }
        }

        /// <summary>
        /// データベースが切り替わるときに行う処理
        /// </summary>
        private void tbcDatabaseTab_Selecting(object sender, TabControlCancelEventArgs e) {
            var ctlDB = (this.tbcDatabaseTab.TabPages[e.TabPageIndex].Controls[0] as ctlDBBase);
            this.tlsColumn.Enabled = (e.Action == TabControlAction.Selecting && e.TabPageIndex == (int) Database.DBIndices.UserDB);
            this.tlsRow.Enabled = (e.Action == TabControlAction.Selecting && ctlDB.SupportedRowTools);
            this.tlsSearch.Enabled = (e.Action == TabControlAction.Selecting && ctlDB.SupportedNameSearch);

            //ユーザーデータベースの追加的な処理
            if (e.Action == TabControlAction.Selecting && e.TabPageIndex == (int) Database.DBIndices.UserDB) {
                this.userDBselectedIndexChanged(this, null);
            }

            //他のデータベースに依存する部分を反映する
            this.applyToDependColumn(ctlDB);

            //各エディターの更新処理
            this.UndoRedoBufferChanged(null, null);
            ctlDB.SrcReset(this.DBList);
        }

        /// <summary>
        /// データベースのタブをセルフ描画
        /// </summary>
        private void tbcDatabaseTab_DrawItem(object sender, DrawItemEventArgs e) {
            //対象のTabControlを取得
            var tab = sender as TabControl;
            var page = tab.TabPages[e.Index];
            var sf = new StringFormat();
            var txt = page.Text;
            const int NormalFontSize = 9;
            const int SelectedFontSize = 11;

            //タブテキストのプラスアルファ
            if (this.DBList[(Database.DBIndices) e.Index].FileName != this.DBList[(Database.DBIndices) e.Index].OriginalFileName) {
                //競合ファイルを編集続行している
                txt = "！" + txt;
            }
            if (this.DBList[(Database.DBIndices) e.Index].IsDirty) {
                //現在のファイルが変更されている
                txt = txt + "*";
            }

            //描画の初期設定
            Common.ApplyHighQualityDrawing(e.Graphics);

            //水平垂直方向の中央に、行が完全に表示されるようにする
            sf.LineAlignment = StringAlignment.Center;
            sf.Alignment = StringAlignment.Center;
            sf.FormatFlags = sf.FormatFlags | StringFormatFlags.LineLimit;

            //背景とテキストの描画
            Brush backBrush;
            if (e.State == DrawItemState.Selected) {
                //選択タブはハイライト表示
                backBrush = new SolidBrush(SystemColors.Window);
                e.Graphics.FillRectangle(backBrush, e.Bounds);
                e.Graphics.DrawString(txt, new Font(page.Font.OriginalFontName, SelectedFontSize, FontStyle.Bold), new SolidBrush(SystemColors.HotTrack), e.Bounds, sf);
            } else {
                //非アクティブタブは通常表示
                backBrush = new SolidBrush(SystemColors.ActiveBorder);
                e.Graphics.FillRectangle(backBrush, e.Bounds);
                e.Graphics.DrawString(txt, new Font(page.Font.OriginalFontName, NormalFontSize, FontStyle.Bold), new SolidBrush(Color.Black), e.Bounds, sf);
            }
        }

        /// <summary>
        /// すべてのDBを保存
        /// </summary>
        private void tolDBSaveAll_Click(object sender, EventArgs e) {
            if (this.DoSave()) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// 現在のDBを保存
        /// </summary>
        private void tolDBSave_Click(object sender, EventArgs e) {
            //他のデータベースに依存する部分を反映
            foreach (var db in this.DBList) {
                this.applyToDependColumn(db.Value);
            }

            //保存処理
            if (this.CurrentDB.IsDirty && this.CurrentDB.DoSave()) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// 現在のDBを再読込
        /// </summary>
        private void tolDBReload_Click(object sender, EventArgs e) {
            if (this.CurrentDB.IsDirty) {
                switch (MessageBox.Show("再読込を行う前に、変更を保存しますか？", Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question)) {
                    case DialogResult.Cancel:
                        return;

                    case DialogResult.Yes:
                        if (!this.CurrentDB.DoSave()) {
                            return;
                        }
                        break;
                }
            }

            //競合先に移行している可能性も考慮して、元のファイルで読み直す
            this.CurrentDB.FileName = this.CurrentDB.OriginalFileName;
            this.CurrentDB.DoLoad();

            //他のDBに依存する部分を適用
            this.SetupDependColumnAll();
        }

        /// <summary>
        /// すべてのDBを再読込
        /// </summary>
        private void tolDBReloadAll_Click(object sender, EventArgs e) {
            if (this.IsDirty) {
                switch (MessageBox.Show("再読込を行う前に、変更を保存しますか？", Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question)) {
                    case DialogResult.Cancel:
                        return;

                    case DialogResult.Yes:
                        if (!this.DoSave()) {
                            return;
                        }
                        break;
                }
            }
            this.DoLoad();
        }

        /// <summary>
        /// 外部からインポート
        /// </summary>
        private void tolDBImport_Click(object sender, EventArgs e) {
            if (this.CurrentDB.IsDirty) {
                MessageBox.Show("インポートを行う前に、現在の編集を保存する必要があります。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            //インポート処理
            var ret = this.CurrentDB.Import();
            if (ret == DialogResult.OK) {
                this.applyToDependColumn(this.CurrentDB);
                this.CurrentDB.SrcReset(this.DBList);
                MessageBox.Show("インポートが完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } else if (ret == DialogResult.No) {
                MessageBox.Show("インポートに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 外部へエクスポート
        /// </summary>
        private void tolDBExport_Click(object sender, EventArgs e) {
            if (this.CurrentDB.IsDirty) {
                MessageBox.Show("エクスポートを行う前に、現在の編集を保存する必要があります。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            //エクスポート処理
            var ret = this.CurrentDB.Export();
            if (ret == DialogResult.OK) {
                MessageBox.Show("エクスポートが完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } else if (ret == DialogResult.No) {
                MessageBox.Show("エクスポートに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 元に戻す
        /// </summary>
        private void tolDBUndo_Click(object sender, EventArgs e) {
            this.CurrentDB.Undo();
        }

        /// <summary>
        /// やり直す
        /// </summary>
        private void tolDBRedo_Click(object sender, EventArgs e) {
            this.CurrentDB.Redo();
        }

        /// <summary>
        /// 行をコピー
        /// </summary>
        private void tolDBCopy_Click(object sender, EventArgs e) {
            if (this.ActiveControl is ctlDBBase && this.CurrentDB.CurrentSubDB?.CurrentCell != null) {
                this.CurrentDB.RowCopy(this.CurrentDB.CurrentSubDB);
            }
        }

        /// <summary>
        /// 行を削除
        /// </summary>
        private void tolDBDelete_Click(object sender, EventArgs e) {
            if (this.ActiveControl is ctlDBBase && this.CurrentDB.CurrentSubDB?.CurrentCell != null) {
                this.CurrentDB.RowDelete(this.CurrentDB.CurrentSubDB);
            }
        }

        /// <summary>
        /// 検索窓の中でEnterキーを押すと検索を実行する
        /// </summary>
        private void tolSearchText_KeyUp(object sender, KeyEventArgs e) {
            if (e.KeyData == Keys.Enter) {
                this.tolDoSearch_Click(sender, e);
                e.Handled = true;
            }
        }

        /// <summary>
        /// 項目名で検索する
        /// </summary>
        private void tolDoSearch_Click(object sender, EventArgs e) {
            if (!string.IsNullOrEmpty(this.tolSearchText.Text)) {
                this.CurrentDB.SearchNext(this.tolSearchText.Text);
            }
        }

        /// <summary>
        /// UserDBのみ：列を追加
        /// </summary>
        private void tolDBAddColumn_Click(object sender, EventArgs e) {
            if (this.tbcDatabaseTab.SelectedIndex != (int) Database.DBIndices.UserDB) {
                return;
            }

            var userDBobj = (this.tbcDatabaseTab.TabPages[(int) Database.DBIndices.UserDB].Controls[0] as ctlDBGUIUserDB);
            var Dlg = new Dialog.Database.dlgEditColumn(null, userDBobj.UDB, this);
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                //列操作後はUndo/Redoバッファーを初期化する
                Database.AddDBColumn(userDBobj.UDB, Dlg.Result);
                this.SetupDependColumnAll();
                userDBobj.UndoRedo.InitBuffer();
            }
        }

        /// <summary>
        /// UserDBのみ：列を編集
        /// </summary>
        private void tolDBEditColumn_Click(object sender, EventArgs e) {
            if (this.tbcDatabaseTab.SelectedIndex != (int) Database.DBIndices.UserDB) {
                return;
            }

            var userDBobj = (this.tbcDatabaseTab.TabPages[(int) Database.DBIndices.UserDB].Controls[0] as ctlDBGUIUserDB);
            if (userDBobj.UDB.SelectedCells.Count == 0) {
                //編集対象が選択されていない
                MessageBox.Show("編集する列内のセルを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            } else if (userDBobj.UDB.Columns[userDBobj.UDB.CurrentCell.ColumnIndex].Tag == null) {
                //不正な列形式
                MessageBox.Show("現在選択されている列は編集できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            } else if (MessageBox.Show(Resources.MsgW_ImportantWork.Replace("$", "列を変更すると、操作対象となる列のデータはすべて削除されます"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.No) {
                //警告に怖気づいた
                return;
            }

            var Dlg = new Dialog.Database.dlgEditColumn(userDBobj.UDB.Columns[userDBobj.UDB.CurrentCell.ColumnIndex].Tag as DBColumn, userDBobj.UDB, this);
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                //列の変更を適用
                var newCol = Dlg.Result;
                var index = userDBobj.UDB.CurrentCell.ColumnIndex;
                userDBobj.UDB.Columns.RemoveAt(index);
                userDBobj.UDB.Columns.Insert(index, newCol.CreateColumnObject());
                this.SetupDependColumnAll();
                userDBobj.UndoRedo.InitBuffer();
            }
        }

        /// <summary>
        /// UserDBのみ：列名を変更
        /// </summary>
        private void tolEditColumnName_Click(object sender, EventArgs e) {
            if (this.tbcDatabaseTab.SelectedIndex != (int) Database.DBIndices.UserDB) {
                return;
            }

            var userDBobj = (this.tbcDatabaseTab.TabPages[(int) Database.DBIndices.UserDB].Controls[0] as ctlDBGUIUserDB);
            if (userDBobj.UDB.SelectedCells.Count == 0) {
                //編集対象が選択されていない
                MessageBox.Show("編集する列内のセルを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            } else if (userDBobj.UDB.Columns[userDBobj.UDB.CurrentCell.ColumnIndex].Tag == null) {
                //不正な列形式
                MessageBox.Show("現在選択されている列は編集できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            var Dlg = new Dialog.Common.dlgInputTextValue("列名の変更", userDBobj.UDB.Columns[userDBobj.UDB.SelectedCells[0].ColumnIndex].HeaderText, null);
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                userDBobj.UDB.Columns[userDBobj.UDB.SelectedCells[0].ColumnIndex].HeaderText = Dlg.ResultText;
                userDBobj.UndoRedo.InitBuffer();
            }
        }

        /// <summary>
        /// UserDBのみ：列を削除
        /// </summary>
        private void tolDBDeleteColumn_Click(object sender, EventArgs e) {
            if (this.tbcDatabaseTab.SelectedIndex != (int) Database.DBIndices.UserDB) {
                return;
            }

            var userDBobj = (this.tbcDatabaseTab.TabPages[(int) Database.DBIndices.UserDB].Controls[0] as ctlDBGUIUserDB);
            if (userDBobj.UDB.SelectedCells.Count == 0) {
                //編集対象が選択されていない
                MessageBox.Show("削除する列内のセルを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            } else if (MessageBox.Show(Resources.MsgW_ImportantWork.Replace("$", "現在の列を削除します"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) != DialogResult.Yes) {
                //警告に怖気づいた
                return;
            }

            //列を削除
            userDBobj.UDB.Columns.RemoveAt(userDBobj.UDB.SelectedCells[0].ColumnIndex);
            userDBobj.UndoRedo.InitBuffer();
        }

        /// <summary>
        /// UserDBのみ：列を左へ
        /// </summary>
        private void tolDBColumnToLeft_Click(object sender, EventArgs e) {
            if (this.tbcDatabaseTab.SelectedIndex != (int) Database.DBIndices.UserDB) {
                return;
            }

            var userDBobj = (this.tbcDatabaseTab.TabPages[(int) Database.DBIndices.UserDB].Controls[0] as ctlDBGUIUserDB);
            if (userDBobj.UDB.SelectedCells.Count == 0) {
                //編集対象が選択されていない
                MessageBox.Show("削除する列内のセルを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            var index = userDBobj.UDB.SelectedCells[0].ColumnIndex;
            if (index == 0) {
                //左端の列は移動できない
                return;
            }

            //各セルの値をバックアップする
            var cells = new List<object>();
            foreach (DataGridViewRow row in userDBobj.UDB.Rows) {
                if (!row.IsNewRow) {
                    cells.Add(row.Cells[index].Value);
                }
            }

            //移動処理
            var col = userDBobj.UDB.Columns[index];
            userDBobj.UDB.Columns.RemoveAt(index);
            userDBobj.UDB.Columns.Insert(index - 1, col);
            col.DisplayIndex = col.Index;

            //各セルの値を復元する
            foreach (DataGridViewRow row in userDBobj.UDB.Rows) {
                if (!row.IsNewRow) {
                    row.Cells[index - 1].Value = cells[row.Index];
                }
            }
        }

        /// <summary>
        /// UserDBのみ：列を右へ
        /// </summary>
        private void tolDBColumnToRight_Click(object sender, EventArgs e) {
            if (this.tbcDatabaseTab.SelectedIndex != (int) Database.DBIndices.UserDB) {
                return;
            }

            var userDBobj = (this.tbcDatabaseTab.TabPages[(int) Database.DBIndices.UserDB].Controls[0] as ctlDBGUIUserDB);
            if (userDBobj.UDB.SelectedCells.Count == 0) {
                //編集対象が選択されていない
                MessageBox.Show("削除する列内のセルを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            var index = userDBobj.UDB.SelectedCells[0].ColumnIndex;
            if (index >= userDBobj.UDB.ColumnCount - 1) {
                //右端の列は移動できない
                return;
            }

            //各セルの値をバックアップする
            var cells = new List<object>();
            foreach (DataGridViewRow row in userDBobj.UDB.Rows) {
                if (!row.IsNewRow) {
                    cells.Add(row.Cells[index].Value);
                }
            }

            //移動処理
            var col = userDBobj.UDB.Columns[index];
            userDBobj.UDB.Columns.RemoveAt(index);
            userDBobj.UDB.Columns.Insert(index + 1, col);
            col.DisplayIndex = col.Index;

            //各セルの値を復元する
            foreach (DataGridViewRow row in userDBobj.UDB.Rows) {
                if (!row.IsNewRow) {
                    row.Cells[index + 1].Value = cells[row.Index];
                }
            }
        }

        /// <summary>
        /// ユーザーデータベースが変更されたときの追加的な挙動
        /// </summary>
        private void userDBselectedIndexChanged(object sender, EventArgs e) {
            this.tlsRow.Enabled = ((ctlDBGUIUserDB) this.DBList[Database.DBIndices.UserDB]).SelectedRowIndex != -1;
            this.tlsColumn.Enabled = this.tlsRow.Enabled;
        }

        /// <summary>
        /// 編集中のデータベースのUndo/Redoバッファが更新されたらコントロール有効無効を更新
        /// </summary>
        private void UndoRedoBufferChanged(object sender, EventArgs e) {
            //他のデータベースによるイベントかもしれないので、改めて現在のデータベースを確認する
            if (this.CurrentDB.DBs.Length > 1) {
                this.tolDBUndo.Enabled = true;
                this.tolDBRedo.Enabled = true;
            } else {
                this.tolDBUndo.Enabled = this.CurrentDB.UndoRedo.CanUndo;
                this.tolDBRedo.Enabled = this.CurrentDB.UndoRedo.CanRedo;
            }
        }

        /// <summary>
        /// 全DB統一設定：新規行が追加されるとFixedIDを自動で生成する
        /// </summary>
        public void newRowCreated(object sender, DataGridViewRowsAddedEventArgs e) {
            var db = (sender as DataGridView);
            if (db.Columns[0].Tag == null && db.Rows[e.RowIndex].Cells[0].Value == null) {
                db.Rows[e.RowIndex].Cells[0].Value = Database.GenerateFixedRowID(db);
            }
        }

        /// <summary>
        /// 全DB統一設定：新規行に既定値を設定する
        /// </summary>
        public void setDefaultValues(object sender, DataGridViewRowEventArgs e) {
            //DBColumnに設定された初期値を取り出してセットする
            //このとき、FixedID列は無視する
            var db = (sender as DataGridView);
            for (var i = 1; i < db.Columns.Count; i++) {
                var colInfo = (db.Columns[i].Tag as DBColumn);
                e.Row.Cells[i].Value = colInfo?.DefaultValue;
            }
        }

        /// <summary>
        /// 全DB統一設定：ソートが要求されたとき
        /// </summary>
        public void sortCompareOnColumnHeader(object sender, DataGridViewSortCompareEventArgs e) {
            e.Handled = true;
            e.SortResult = 0;

            if (e.Column.ValueType == typeof(int)) {
                //整数比較
                if (int.TryParse(e.CellValue1.ToString() ?? "", out var x) && int.TryParse(e.CellValue2.ToString() ?? "", out var y)) {
                    e.SortResult = x - y;
                }
            } else if (e.Column.ValueType == typeof(bool)) {
                //論理値比較
                e.SortResult = Convert.ToInt32(e.CellValue1) - Convert.ToInt32(e.CellValue2);
            } else if (e.Column.ValueType == typeof(string)) {
                //文字列比較
                e.SortResult = string.Compare(e.CellValue1?.ToString(), e.CellValue2?.ToString());
            }
        }

        /// <summary>
        /// 全DB統一設定：セルのクリックを捕捉する
        /// </summary>
        public void clickButtonCell(object sender, DataGridViewCellEventArgs e) {
            var db = (sender as DataGridView);
            var col = db.Columns[e.ColumnIndex]?.Tag as DBColumn;
            if (e.RowIndex >= 0) {
                col?.Click(db.Columns, db.Rows[e.RowIndex], db[e.ColumnIndex, e.RowIndex]);
            }
        }

        /// <summary>
        /// 全DB統一設定：入力値のチェックを独自に行うためにエラーメッセージを無効化する
        /// </summary>
        public void dataErrorHandler(object sender, DataGridViewDataErrorEventArgs e) {
            if ((sender as DataGridView)[e.ColumnIndex, e.RowIndex] is DataGridViewComboBoxCell) {
                //ComboBoxのエラーは無視する
            } else {
                //System.Diagnostics.Debug.WriteLine($"{(sender as DataGridView).Tag}: {e.RowIndex}行{e.ColumnIndex}列: {e.Exception.Message}");
                System.Media.SystemSounds.Beep.Play();
            }
        }

        /// <summary>
        /// 全DB統一設定：入力値を含む行全体のチェックを行う
        /// </summary>
        public void errorRowCheckHandler(object sender, DataGridViewCellCancelEventArgs e) {
            var db = (sender as DataGridView);
            var eBuf = "";

            //新規行は適用しない
            if (db.Rows[e.RowIndex].IsNewRow) {
                return;
            }

            //FixedIDが設定されていない場合はここで設定する
            if (db.Columns[0].Tag == null && db.Rows[e.RowIndex].Cells[0].Value == null) {
                db.Rows[e.RowIndex].Cells[0].Value = Database.GenerateFixedRowID(db);
            }

            //すべての列をチェックする
            foreach (DataGridViewCell cell in db.Rows[e.RowIndex].Cells) {
                var col = db.Columns[cell.ColumnIndex]?.Tag as DBColumn;
                var temp = col?.ErrorCheck(cell.Value);

                if (!string.IsNullOrEmpty(temp)) {
                    eBuf += $"{db.Columns[cell.ColumnIndex].HeaderText}: " + temp;
                }
            }

            //エラーにする場合は入力をキャンセルする
            if (!string.IsNullOrEmpty(eBuf)) {
                db.Rows[e.RowIndex].ErrorText = eBuf;
                e.Cancel = true;

                //データベース管理コントロールにエラーがあることを知らせる
                Control ctl;
                for (ctl = db.Parent; ctl != null && (ctl as ctlDBBase) == null; ctl = ctl.Parent) {
                }
                if (ctl != null) {
                    (ctl as ctlDBBase).HasError = true;
                }
            }
        }

        /// <summary>
        /// 全DB統一設定：検証を通過できたらエラーテキストを消す
        /// </summary>
        public void errorRowCheckedHandler(object sender, DataGridViewCellEventArgs e) {
            var db = (sender as DataGridView);
            db.Rows[e.RowIndex].ErrorText = "";

            //データベース管理コントロールにエラーがなくなったことを知らせる
            Control ctl;
            for (ctl = db.Parent; ctl != null && (ctl as ctlDBBase) == null; ctl = ctl.Parent) {
            }
            if (ctl != null) {
                (ctl as ctlDBBase).HasError = false;
            }
        }
    }
}
