using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    /// <summary>
    /// 各データベースコントロールの抽象クラス
    /// </summary>
    public partial class ctlDBBase : UserControl, IDirtyable {
        public event EventHandler IsDirtyChanged;
        public event EventHandler UndoRedoBufferChanged;
        public bool SupportedRowTools = true;               //行操作をサポートするかどうか
        public bool SupportedStandardEditFlag = true;       //標準の編集有無チェック機能を使用するかどうか
        public bool SupportedNameSearch = true;             //項目名での検索機能を使用するかどうか
        public bool FlagBySystem = false;                   //システムによる変更を行っているかどうか
        public UndoRedoManager UndoRedo = new UndoRedoManager();
        public bool HasError = false;                       //データベース内にエラーが含まれる場合はデータベース単位の操作を禁止する
        private List<DataGridView> dbList = new List<DataGridView>();
        private DataGridViewCell beforeCellValue;
        private DataGridViewRow beforeRowData;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBBase() {
            this.InitializeComponent();
            this.UndoRedo.BufferChanged += this.UndoRedoBufferChangedRelay;
        }

        /// <summary>
        /// 管理しているデータベースをDataGridView配列の形式で取得/設定します。
        /// </summary>
        public DataGridView[] DBs {
            get {
                return this.dbList.ToArray<DataGridView>();
            }
            protected set {
                this.dbList = value.ToList<DataGridView>();

                //自動で標準のイベントハンドラーをセットする
                foreach (var db in this.dbList) {
                    db.CellEnter += this.cellEnter;
                    db.RowEnter += this.rowEnter;

                    if (this.SupportedStandardEditFlag) {
                        //編集操作に関わるイベントハンドラー
                        db.CellValueChanged += this.cellValueChanged;
                        db.RowsRemoved += this.rowRemoving;
                        db.RowsAdded += this.newRowAdded;
                        db.ColumnHeaderMouseClick += this.columnHeaderMouseClick;
                    }
                }
            }
        }

        /// <summary>
        /// 列を持たない既定のDataGridViewオブジェクトを生成します。
        /// </summary>
        protected DataGridView CreateDefaultDataGridView() {
            var obj = new DataGridView() {
                AllowUserToAddRows = true,
                Enabled = false,
            };
            this.Controls.Add(obj);
            return obj;
        }

        /// <summary>
        /// 現在の操作がUndoバッファーに記録されるかどうか
        /// </summary>
        public bool EnabledUndoBuffer {
            get; set;
        } = false;

        /// <summary>
        /// 変更されているかどうか
        /// </summary>
        public virtual bool IsDirty {
            get {
                return this.isDirty;
            }
            set {
                //状態を変更するときのみ適用する
                if (this.isDirty != value && !this.LockDirty) {
                    this.isDirty = value;
                    this.IsDirtyChanged?.Invoke(this, null);

                    if (!value) {
                        //Undoバッファーを初期化する
                        this.UndoRedo = new UndoRedoManager();
                    }
                }
            }
        }
        private bool isDirty = false;

        /// <summary>
        /// このデータベースが新規に作成されたかどうか
        /// </summary>
        public bool NewCreated {
            get; set;
        } = false;

        /// <summary>
        /// システムによる変更につき、IsDirtyを固定するためのフラグ
        /// </summary>
        public bool LockDirty {
            get; set;
        } = false;

        /// <summary>
        /// このデータベースが対象としているファイルです。
        /// </summary>
        public string FileName {
            get {
                return this.fileName;
            }
            set {
                this.fileName = value;
                if (this.OriginalFileName == null) {
                    this.OriginalFileName = value;      //初回設定時のみ設定するファイル名をセットする
                }
            }
        }
        private string fileName = "";

        public string OriginalFileName {
            get; set;
        } = null;

        /// <summary>
        /// 現在操作対象のデータベースです。
        /// </summary>
        public virtual DataGridView CurrentSubDB {
            get {
                if (this.DBs == null) {
                    return null;
                }

                if (this.DBs.Length == 1) {
                    return this.DBs[0];
                } else {
                    // 複数のサブDBを持つ場合は現在アクティブになっているDataGridViewを探す
                    if (!(this.ActiveControl is DataGridView)) {
                        return null;
                    }
                    foreach (var db in this.DBs) {
                        if (this.ActiveControl == db) {
                            return db;
                        }
                    }
                    return null;
                }
            }
        }

        /// <summary>
        /// 書き込みを行います。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public virtual bool DoSave() {
            try {
                using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + this.FileName), Common.SJIS)) {
                    foreach (var db in this.DBs) {
                        db.EndEdit();
                        Database.WriteDatabase(db, W);
                    }
                }
            } catch {
                return false;
            }
            this.IsDirty = false;
            return true;
        }

        /// <summary>
        /// 読み込みを行います。
        /// </summary>
        public virtual void DoLoad() {
            if (!File.Exists(ProjectManager.ProjectPath + this.FileName)) {
                this.NewCreated = true;            //TSVファイルが存在しない場合は、保存フラグを立てておく
                return;
            }

            //読み込み操作をUndo/Redoバッファーに登録しないようにする
            this.FlagBySystem = true;
            try {
                using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + this.FileName), Common.SJIS)) {
                    foreach (var db in this.DBs) {
                        Database.ReadDatabase(db, R);
                    }
                }
            } catch {
                MessageBox.Show($"データベース [{this.FileName}] の読み込みに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            this.FlagBySystem = false;

            this.IsDirty = false;
        }

        /// <summary>
        /// 外部からデータを取り込みます。
        /// </summary>
        public virtual DialogResult Import() {
            var tempDB = new DataGridView();
            var index = 0;
            string fullPath;

            //読み込むファイルをユーザーに選択してもらう
            this.dlgImporter.Filter = $"|{Path.GetFileName(this.FileName)}";
            if (this.dlgImporter.ShowDialog() != DialogResult.OK) {
                return DialogResult.Cancel;
            }
            fullPath = this.dlgImporter.FileName;

            //複数のサブDBがある場合はユーザーに選択してもらう
            if (this.DBs.Length > 1) {
                var Dlg = new Dialog.Common.dlgSelectInList("サブデータベースの選択");
                for (var i = 0; i < this.DBs.Length; i++) {
                    Dlg.AddItem(i, this.DBs[i].Tag?.ToString());
                }
                if (Dlg.ShowDialog() != DialogResult.OK) {
                    return DialogResult.Cancel;
                }
                index = Dlg.GetResultID();
            }

            //インポート元のファイルを開く
            try {
                using (var R = new StreamReader(Common.OpenFileReadOnly(fullPath), Common.SJIS)) {
                    //読み込むサブDBのところまで進める
                    for (var i = 0; i < index; i++) {
                        //一時的に生成したDataGridViewに列をセットする
                        tempDB.Columns.Clear();
                        tempDB.Rows.Clear();
                        foreach (DataGridViewColumn col in this.DBs[i].Columns) {
                            tempDB.Columns.Add(col.Clone() as DataGridViewColumn);
                        }
                        Database.ReadDatabase(tempDB, R);
                    }

                    //追加読み込みを行う（ただし、行を追加できないタイプのものは置き換える）
                    Database.ReadDatabase(this.DBs[index], R, this.DBs[index].AllowUserToAddRows);
                }
                return DialogResult.OK;
            } catch {
                return DialogResult.No;
            }
        }

        /// <summary>
        /// 外部へデータを書き出します。
        /// </summary>
        public virtual DialogResult Export() {
            //書き出し先をユーザーに選択してもらう
            this.dlgImporter.Filter = $"|{Path.GetFileName(this.FileName)}";
            this.dlgExporter.FileName = Path.GetFileName(this.FileName);
            if (this.dlgExporter.ShowDialog() != DialogResult.OK) {
                return DialogResult.Cancel;
            }

            //インポート先に書き出す
            try {
                using (var W = new StreamWriter(Common.OpenFileWriteOnly(this.dlgExporter.FileName), Common.SJIS)) {
                    foreach (var db in this.DBs) {
                        db.EndEdit();
                        Database.WriteDatabase(db, W);
                    }
                }
                return DialogResult.OK;
            } catch {
                return DialogResult.No;
            }
        }

        /// <summary>
        /// このデータベース編集画面に切り替わったときに他のデータベースを参照する部分を更新します。
        /// </summary>
        public virtual void SrcReset(AllDB DBList) {
        }

        /// <summary>
        /// このデータベースが管理する付属ファイルの不要なものを除去します。
        /// </summary>
        public virtual void Cleanup() {
        }

        /// <summary>
        /// 与えられたファイル名が管轄しているファイルであるかどうかを調べます。
        /// </summary>
        public virtual bool IsManagedFile(string rPath) {
            return (this.FileName == rPath);
        }

        /// <summary>
        /// 与えられたファイル名に対応する項目を選択します。
        /// </summary>
        public virtual void OpenManagedFile(string rPath) {
        }

        /// <summary>
        /// 次を検索します。
        /// </summary>
        public virtual void SearchNext(string text) {
            var changedDB = false;

            //データベース単位で進めていく
            var db = this.CurrentSubDB ?? this.NextDB();
            var firstDB = db;
            var firstIndex = -1;
            if (firstDB != null && firstDB.SelectedRows.Count > 0) {
                firstIndex = firstDB.SelectedRows[0].Index;
            }
            while (db != null) {
                // このサブDBがタブコントロールの中にある場合は、該当するタブに切り替える
                ((db.Parent as TabPage)?.Parent as TabControl)?.SelectTab(db.Parent as TabPage);
                db.Focus();

                var index = -1;
                if (db.SelectedRows.Count > 0 && !changedDB) {
                    //現在のインデックスを設定しておく
                    index = db.SelectedRows[0].Index;
                }

                //次のインデックスから検索を始める
                var isLooped = false;
                for (index += 1; index < db.Rows.Count; index++) {
                    if (db.Rows[index].IsNewRow) {
                        //新規行は処理しない
                        continue;
                    }
                    if (db == firstDB && firstIndex == index) {
                        // 検索開始時点のDBの行まで戻ってきたら中断
                        isLooped = true;
                        break;
                    }

                    if (db.Rows[index].Cells[(int) Database.DBDefaultColumnIndices.Name].Value?.ToString().IndexOf(text) != -1) {
                        //該当行を発見
                        db.ClearSelection();
                        db.Rows[index].Selected = true;
                        return;
                    }
                }
                if (isLooped) {
                    break;
                }

                //該当行を発見できなかった場合は、次のサブDBに移る
                db.ClearSelection();
                db = this.NextDB();
                changedDB = true;
                if (db == firstDB && firstIndex == -1) {
                    // 選択行無しで最初のDBに戻ってきたら中断
                    break;
                }
            }

            //見つからなかった
            System.Media.SystemSounds.Beep.Play();
            this.Controls[0].Focus();
        }

        /// <summary>
        /// 現在のサブDBの次のサブDBを返します。
        /// 末尾のサブDBに対する次のサブDBは先頭を指します。
        /// </summary>
        private DataGridView NextDB() {
            if (this.ActiveControl is DataGridView) {
                var currentFound = false;

                foreach (var db in this.DBs) {
                    if (this.ActiveControl == db) {
                        currentFound = true;
                    } else if (currentFound) {
                        return db;
                    }
                }

                //末尾のサブDBまで完了した
                return this.DBs[0];
            }

            //アクティブなDBがない場合は最初のDBを返す
            return this.DBs[0];
        }

        /// <summary>
        /// Undo/Redoバッファが変化したときのイベントを中継します。
        /// </summary>
        private void UndoRedoBufferChangedRelay(object sender, EventArgs e) {
            this.UndoRedoBufferChanged?.Invoke(sender, e);
        }

        /// <summary>
        /// 操作を元に戻します。
        /// </summary>
        public void Undo() {
            //元に戻す操作がUndo/Redoバッファーに登録されないようにする
            this.FlagBySystem = true;
            this.UndoRedo.Undo();
            this.FlagBySystem = false;
        }

        /// <summary>
        /// 操作をやり直します。
        /// </summary>
        public void Redo() {
            //やり直す操作がUndo/Redoバッファーに登録されないようにする
            this.FlagBySystem = true;
            this.UndoRedo.Redo();
            this.FlagBySystem = false;
        }

        /// <summary>
        /// 現在の行を複製します。
        /// </summary>
        public void RowCopy(DataGridView tgt) {
            if (tgt == null || tgt.CurrentRow == null || tgt.CurrentRow.IsNewRow) {
                return;
            }

            //Undo-Redo操作を自前で定義する
            var redo = new Action<int, DataGridViewRow>((rowIndex, row) => {
                //これから行う操作を定義する
                tgt.Rows.Insert(rowIndex);
                for (var i = 0; i < row.Cells.Count; i++) {
                    if (i == 0 && tgt.Columns[i].Tag == null) {
                        continue;       //FixedID列はコピーしない
                    }
                    tgt.Rows[rowIndex].Cells[i].Value = row.Cells[i].Value;
                    tgt.Rows[rowIndex].Cells[i].Tag = row.Cells[i].Tag;
                }
            });
            var undo = new Action<int>((rowIndex) => {
                //コピーした行を削除する
                tgt.Rows.RemoveAt(rowIndex);
            });

            //操作実行: ここで行う操作が重複してUndo/Redoバッファーに登録されないようにする
            this.FlagBySystem = true;
            var index = tgt.CurrentRow.Index;
            this.UndoRedo.Do(
                redo, new object[] { index, this.beforeRowData },
                undo, new object[] { index },
                false
            );
            this.FlagBySystem = false;
            this.IsDirty = true;
        }

        /// <summary>
        /// 現在の行を削除します。
        /// </summary>
        public void RowDelete(DataGridView tgt) {
            if (tgt == null || tgt.CurrentRow == null || tgt.CurrentRow.IsNewRow) {
                return;
            }
            tgt.Rows.Remove(tgt.CurrentRow);
        }

        /// <summary>
        /// セルへのフォーカスが当たったとき
        /// </summary>
        private void cellEnter(object sender, DataGridViewCellEventArgs e) {
            //編集開始前の値を控えておく
            this.beforeCellValue = Activator.CreateInstance((sender as DataGridView)[e.ColumnIndex, e.RowIndex].GetType()) as DataGridViewCell;
            this.beforeCellValue.Value = (sender as DataGridView)[e.ColumnIndex, e.RowIndex].Value;
            this.beforeCellValue.Tag = (sender as DataGridView)[e.ColumnIndex, e.RowIndex].Tag;
        }

        /// <summary>
        /// 列をクリックしたとき
        /// </summary>
        private void columnHeaderMouseClick(object sender, EventArgs e) {
            //並び替えは自動で行われる
            this.IsDirty = true;
        }

        /// <summary>
        /// 行へのフォーカスが当たったとき
        /// </summary>
        private void rowEnter(object sender, DataGridViewCellEventArgs e) {
            //編集開始前の値を控えておく
            var db = (sender as DataGridView);
            this.beforeRowData = new DataGridViewRow();

            for (var i = 0; i < db.Columns.Count; i++) {
                var cell = Activator.CreateInstance((sender as DataGridView)[i, e.RowIndex].GetType()) as DataGridViewCell;
                cell.Value = (sender as DataGridView)[i, e.RowIndex].Value;
                cell.Tag = (sender as DataGridView)[i, e.RowIndex].Tag;
                this.beforeRowData.Cells.Add(cell);
            }
        }

        /// <summary>
        /// 値が変更されたとき
        /// </summary>
        private void cellValueChanged(object sender, DataGridViewCellEventArgs e) {
            if (!this.EnabledUndoBuffer
            || this.FlagBySystem
            || e.RowIndex < 0
            || e.ColumnIndex < 0) {
                return;

                //新規行での編集を無視してしまうと、新規行で最初に画像セルを変更したときに処理されず、変更が反映されなくなってしまう
                //} else if((sender as DataGridView).Rows[e.RowIndex].IsNewRow) {
                //	return;
            }

            var newCell = Activator.CreateInstance((sender as DataGridView)[e.ColumnIndex, e.RowIndex].GetType()) as DataGridViewCell;
            newCell.Value = (sender as DataGridView)[e.ColumnIndex, e.RowIndex].Value;
            newCell.Tag = (sender as DataGridView)[e.ColumnIndex, e.RowIndex].Tag;

            this.IsDirty = true;
            var index = e.RowIndex;
            var db = sender as DataGridView;

            //Undo-Redo操作を自前で定義する
            var act = new Action<int, int, DataGridViewCell>((col, row, cell) => {
                //ユーザーによる操作を再現する
                db[col, row].Value = cell.Value;
                db[col, row].Tag = cell.Tag;
            });

            //操作実行
            this.UndoRedo.Do(
                act, new object[] { e.ColumnIndex, e.RowIndex, newCell as DataGridViewCell },
                act, new object[] { e.ColumnIndex, e.RowIndex, this.beforeCellValue as DataGridViewCell },
                true
            );
        }

        /// <summary>
        /// 行が追加されたとき
        /// </summary>
        private void newRowAdded(object sender, DataGridViewRowsAddedEventArgs e) {
            if (!this.EnabledUndoBuffer || this.FlagBySystem) {
                return;
            }

            this.IsDirty = true;
            var db = sender as DataGridView;

            //Undo-Redo操作を自前で定義する
            var redo = new Action<int, object>((rowIndex, row) => {
                //ユーザーによる操作を再現する
                db.Rows.Insert(rowIndex);
                for (var i = 0; i < db.Columns.Count; i++) {
                    db.Rows[rowIndex].Cells[i].Value = ((DataGridViewRow) row).Cells[i].Value;
                    db.Rows[rowIndex].Cells[i].Tag = ((DataGridViewRow) row).Cells[i].Tag;
                }
            });
            var undo = new Action<int>((rowIndex) => {
                //ユーザーによる変更を取り消す
                db.Rows.RemoveAt(rowIndex);
            });

            //操作実行: ユーザーが追加する意図を持ったのは、実際に追加された行の一つ前であることに注意する
            this.UndoRedo.Do(
                redo, new object[] { e.RowIndex - 1, this.beforeRowData },
                undo, new object[] { e.RowIndex - 1 },
                true
            );
        }

        /// <summary>
        /// 行が削除されたとき
        /// </summary>
        private void rowRemoving(object sender, DataGridViewRowsRemovedEventArgs e) {
            if (!this.EnabledUndoBuffer || this.FlagBySystem) {
                return;
            }

            this.IsDirty = true;
            var db = sender as DataGridView;

            //Undo-Redo操作を自前で定義する
            var redo = new Action<int>((index) => {
                //ユーザーによる操作を再現する
                db.Rows.RemoveAt(index);
            });
            var undo = new Action<int, DataGridViewRow>((rowIndex, row) => {
                //削除した行を復元する
                db.Rows.Insert(rowIndex);
                for (var i = 0; i < db.Columns.Count; i++) {
                    db[i, rowIndex].Value = row.Cells[i].Value;
                    db[i, rowIndex].Tag = row.Cells[i].Tag;
                }
            });

            //操作実行
            this.UndoRedo.Do(
                redo, new object[] { e.RowIndex },
                undo, new object[] { e.RowIndex, this.beforeRowData },
                true
            );
        }
    }
}
