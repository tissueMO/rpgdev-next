using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlDBGUIUserDB : ctlDBGUIBase {
        public event EventHandler UserDBSelected;
        public mgrDBUserDB mgr;

        /// <summary>
        /// システムによる変更を行っているかどうか
        /// </summary>
        public static bool flagBySystem = false;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBGUIUserDB() {
            this.InitializeComponent();
            this.ltvDB.ShowItemToolTips = true;
            Common.EnableDoubleBuffering(this.UDB);
            this.SupportedRowTools = true;
            this.SupportedStandardEditFlag = true;
            this.DBs = new DataGridView[] { this.CreateDefaultDataGridView()/*項目リスト*/, this.UDB/*編集用コントロール*/ };
        }

        public override AllDB DBList {
            get {
                return this.dbList;
            }
            set {
                this.dbList = value;
                this.mgr = new mgrDBUserDB(this.dbList, this.DBs[0], this.UDB, this);
                this.mgr.ListChanged += this.edittingListChanged;
                this.mgr.DataChanged += this.dbDataChanged;
                this.mgr.Editted += this.managerEditted;
            }
        }
        private AllDB dbList;

        public override DataGridView CurrentSubDB {
            get {
                if (this.mgr.EdittingData != null) {
                    return this.UDB;
                }
                return null;
            }
        }

        /// <summary>
        /// 選択行インデックス
        /// </summary>
        public int SelectedRowIndex {
            get {
                if (this.ltvDB.SelectedIndices.Count > 0) {
                    return this.ltvDB.SelectedIndices[0];
                }
                return -1;
            }
        }

        public override bool DoSave() {
            if (this.IsDirty && this.mgr.EdittingData != null) {
                if (!this.mgr.EdittingData.SaveFile()) {
                    return false;
                }
            }

            //内部DB
            base.DoSave();
            return true;
        }

        /// <summary>
        /// データベースの項目リストを読み込みます。
        /// 実際にデータベースの中身を読み込むのは特定の項目が選択されたときです。
        /// </summary>
        public override void DoLoad() {
            if (!File.Exists(ProjectManager.ProjectPath + this.FileName)) {
                //TSVファイルが存在しない場合は保存するためにフラグを立てておく
                this.NewCreated = true;
                return;
            }

            //読み込み操作がUndo/Redoバッファに反映されないようにする
            this.FlagBySystem = true;
            ctlDBGUIUserDB.flagBySystem = true;
            try {
                using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + this.FileName), Common.SJIS)) {
                    Database.ReadDatabase(this.DBs[0], R);
                }
            } catch {
                MessageBox.Show("データベースの読み込みに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            this.IsDirty = false;
            this.mgr.IsEditIgnoreMode = true;
            this.ApplyToManager<mgrDBUserDB, mgrDBUserDB.UserDBOneData>(this.mgr);
            this.mgr.IsEditIgnoreMode = false;
            this.FlagBySystem = false;
            ctlDBGUIUserDB.flagBySystem = false;

            this.ltvDB.SelectedIndices.Clear();
        }

        public override DialogResult Import() {
            this.ltvDB.SelectedIndices.Clear();
            return this.ImportWithFiles<mgrDBUserDB, mgrDBUserDB.UserDBOneData>(Resources.Path_DBs_UserDBs, Resources.Extension_DB, this.mgr);
        }

        public override DialogResult Export() {
            return this.ExportWithFiles(Resources.Path_DBs_UserDBs, Resources.Extension_DB, this.mgr.Data);
        }

        public override bool IsManagedFile(string rPath) {
            if (base.IsManagedFile(rPath)) {
                return true;
            }

            //付属ファイルを調べる
            if (this.mgr.EdittingData != null) {
                if (rPath == this.mgr.EdittingData.FileName) {
                    return true;
                }
            }
            return false;
        }

        public override void Cleanup() {
            var files = Directory.GetFiles(ProjectManager.ProjectPath + Common.CutLastChar(Resources.Path_DBs_UserDBs, '\\'));
            foreach (var file in files) {
                if (int.TryParse(Path.GetFileNameWithoutExtension(file), out var id)) {
                    //ファイル名が数値である場合に限りID判定する
                    if (this.mgr.ExistsData(id)) {
                        continue;
                    }
                }

                //削除処理
                try {
                    File.Delete(file);
                } catch {
                    MessageBox.Show(Path.GetFileName(file) + " の削除に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// UserDB新規
        /// </summary>
        private void ctlDBGUIUserDB_RequestRowAdd(object sender, EventArgs e) {
            this.ltvDB.SelectedIndices.Add(this.mgr.AddData(new mgrDBUserDB.UserDBOneData(this.mgr.GenerateFixedID(), 0, "新しいデータベース", "", 0, this.DBList)));
        }

        /// <summary>
        /// UserDB複製
        /// </summary>
        private void ctlDBGUIUserDB_RequestRowCopy(object sender, EventArgs e) {
            var index = this.mgr.CopyData(this.ltvDB.SelectedIndices[0]);
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.SelectedIndices.Add(index);
        }

        /// <summary>
        /// UserDB削除
        /// </summary>
        private void ctlDBGUIUserDB_RequestRowDelete(object sender, EventArgs e) {
            //選択されているうちに選択解除を行わないとイベントが発生しない
            var index = this.ltvDB.SelectedIndices[0];
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.DeleteData(index);
        }

        /// <summary>
        /// UserDB選択変更
        /// </summary>
        private void ltvDB_SelectedIndexChanged(object sender, EventArgs e) {
            if (Database.HasError(this.UDB)) {
                return;
            }

            if (this.ltvDB.SelectedIndices.Count == 0) {
                var isSaving = !(ctlDBGUIUserDB.flagBySystem || this.FlagBySystem || this.LockDirty);
                if (isSaving && (this.mgr.EdittingData?.IsDirty ?? false)) {
                    isSaving = MessageBox.Show(
                        Resources.MsgQ_Save.Replace("$", $"ユーザー定義DB「{this.mgr.EdittingData.Name}」"),
                        Resources.AppName,
                        MessageBoxButtons.YesNo,
                        MessageBoxIcon.Question,
                        MessageBoxDefaultButton.Button1) == DialogResult.Yes;
                }

                this.LockDirty = false;
                var oldFlagBySystem = ctlDBGUIUserDB.flagBySystem;
                ctlDBGUIUserDB.flagBySystem = true;

                if (!isSaving) {
                    // この時点で編集した内容を手放して保存させないようにする
                    this.ltvDB.SelectedIndices.Clear();
                    this.mgr.EdittingIndex = -1;
                }
                this.mgr.EndEdit();

                //親:false の状態でないと EdittingData:true にならないので、このタイミングで親ごと保存する
                //編集中のDBが保存されなかった場合は親だけ保存される
                this.DoSave();

                ctlDBGUIUserDB.flagBySystem = oldFlagBySystem;
                this.IsDirty = false;
                this.UndoRedo.InitBuffer();
            } else {
                this.LockDirty = true;

                //読み込み操作がUndo/Redoバッファに反映されないようにする
                this.FlagBySystem = true;
                this.mgr.BeginEdit(this.ltvDB.SelectedIndices[0]);
                this.FlagBySystem = false;
            }
            this.UserDBSelected?.Invoke(this, null);
            this.LockDirty = false;
        }

        /// <summary>
        /// UserDBリストの列をクリックすると、対応列に従ってソートする
        /// </summary>
        private void ltvDB_ColumnClick(object sender, ColumnClickEventArgs e) {
            if (Database.HasError(this.UDB)) {
                return;
            }

            if (this.mgr.EdittingData != null) {
                this.mgr.EndEdit();
            }
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.SortDataList(e.Column);
            this.IsDirty = true;
        }

        /// <summary>
        /// ID変更時
        /// </summary>
        private void tolID_Validating(object sender, CancelEventArgs e) {
            if (!int.TryParse(this.tolID.Text, out var val)) {
                //無効なIDのときは既定値にする
                this.tolID.Text = Database.NotFoundItemID.ToString();
            } else {
                //一意のIDに自動調整する
                this.tolID.Text = this.GenerateID(val).ToString();
            }
            this.mgr.EdittingData.VisibleID = int.Parse(this.tolID.Text);
            e.Cancel = false;
        }

        /// <summary>
        /// 名前変更時
        /// </summary>
        private void tolName_Validating(object sender, CancelEventArgs e) {
            if (this.mgr.EdittingData != null) {
                this.mgr.EdittingData.Name = this.tolName.Text;
            }
            e.Cancel = false;
        }

        /// <summary>
        /// メモ変更時
        /// </summary>
        private void tolMemo_Validating(object sender, CancelEventArgs e) {
            if (this.mgr.EdittingData != null) {
                this.mgr.EdittingData.Comment = this.tolMemo.Text;
            }
            e.Cancel = false;
        }

        /// <summary>
        /// 列が追加されたとき
        /// </summary>
        private void UDB_ColumnAdded(object sender, DataGridViewColumnEventArgs e) {
            if (ctlDBGUIUserDB.flagBySystem) {
                return;
            } else if (this.ltvDB.SelectedIndices.Count == 0) {
                return;
            }
            this.mgr.EdittingData.ColumnCount++;
        }

        /// <summary>
        /// 列が削除されたとき
        /// </summary>
        private void UDB_ColumnRemoved(object sender, DataGridViewColumnEventArgs e) {
            if (ctlDBGUIUserDB.flagBySystem) {
                return;
            } else if (this.ltvDB.SelectedIndices.Count == 0) {
                return;
            }
            this.mgr.EdittingData.ColumnCount--;
        }

        /// <summary>
        /// 列の順序変更が発生したとき
        /// </summary>
        private void UDB_ColumnDisplayIndexChanged(object sender, DataGridViewColumnEventArgs e) {
            this.IsDirty = true;
        }

        /// <summary>
        /// 一意のIDを生成します。
        /// </summary>
        private int GenerateID(int ID) {
            var id = ID;
            bool NG;

            do {    //基点から始めて重複しない最小の値を生成する
                NG = false;
                foreach (ListViewItem item in this.ltvDB.Items) {
                    if (item.SubItems[1].Text == id.ToString()) {
                        NG = true;
                        id++;
                        break;
                    }
                }
            } while (NG);

            return id;
        }

        /// <summary>
        /// UserDBリストに変更が起きたときにコントロールへ更新する
        /// </summary>
        protected override void edittingListChanged(object sender, EventArgs e) {
            var beforeIndex = (this.ltvDB.SelectedIndices.Count > 0) ? this.ltvDB.SelectedIndices[0] : -1;

            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.BeginUpdate();
            this.ltvDB.Items.Clear();
            foreach (var item in this.mgr.Data) {
                var newItem = new ListViewItem {
                    Text = (!Settings.Default.Script_FixedIDHexMode) ? item.FixedID.ToString() : "0x" + item.FixedID.ToString("X")
                };
                newItem.SubItems.Add(item.VisibleID.ToString());
                newItem.SubItems.Add(item.Name);
                this.ltvDB.Items.Add(newItem);
            }
            this.ltvDB.EndUpdate();

            if (beforeIndex != -1 && beforeIndex < this.ltvDB.Items.Count) {
                this.ltvDB.SelectedIndices.Add(beforeIndex);
            }
        }

        /// <summary>
        /// 編集中のデータベースに変更が起きたときにコントロールへ更新する
        /// </summary>
        private void dbDataChanged(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null) {
                return;
            }

            //ヘッダー情報
            this.tolID.Text = this.mgr.EdittingData.VisibleID.ToString();
            this.tolName.Text = this.mgr.EdittingData.Name;
            this.tolMemo.Text = this.mgr.EdittingData.Comment;

            //UserDBリスト上の選択項目
            if (this.ltvDB.SelectedIndices.Count > 0) {
                this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.VisibleID].Text = this.mgr.EdittingData.VisibleID.ToString();
                this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text = this.mgr.EdittingData.Name.ToString();
            }

            if (!ctlDBGUIUserDB.flagBySystem) {
                this.IsDirty = true;
            }
        }
    }
}
