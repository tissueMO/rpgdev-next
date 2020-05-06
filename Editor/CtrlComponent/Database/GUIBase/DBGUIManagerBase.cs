using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlDBGUIBase {

        /// <summary>
        /// 各種GUIエディターが保持する内部システム機構のインターフェース
        /// </summary>
        public abstract class DBGUIManagerBase<T> where T : DBGUIOneDatabase {
            public event EventHandler ListChanged;
            public event EventHandler DataChanged;
            public event EventHandler Editted;

            /// <summary>
            /// 編集したかどうかを確認しないモードであるかどうか
            /// </summary>
            public bool IsEditIgnoreMode {
                get; set;
            } = false;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public DBGUIManagerBase(AllDB DBList, DataGridView mydb) {
                this.Data = new List<T>();
                this.DBList = DBList;
                this.MyDB = mydb;
            }

            /// <summary>
            /// すべてのデータベースへの参照
            /// </summary>
            public AllDB DBList {
                get;
                protected set;
            }

            /// <summary>
            /// 管理リスト
            /// </summary>
            public List<T> Data {
                get;
                protected set;
            }

            /// <summary>
            /// 管理リストと関連付けられたデータベース
            /// </summary>
            public DataGridView MyDB {
                get;
                protected set;
            }

            /// <summary>
            /// 編集中のデータのインデックス
            /// </summary>
            public int EdittingIndex {
                get {
                    if (this.edittingIndex <= -1 || this.Data.Count <= this.edittingIndex) {
                        this.edittingIndex = -1;
                        return this.edittingIndex;
                    }
                    return this.edittingIndex;
                }
                set {
                    this.edittingIndex = value;
                }
            }
            public int edittingIndex = -1;

            /// <summary>
            /// 編集中のデータ
            /// </summary>
            public T EdittingData {
                get {
                    if (this.EdittingIndex <= -1 || this.Data.Count <= this.EdittingIndex) {
                        this.EdittingIndex = -1;
                        return null;
                    } else {
                        return this.Data[this.EdittingIndex];
                    }
                }
            }

            /// <summary>
            /// デザイナービューの拡大率
            /// </summary>
            public virtual ViewScale.ScaleRate Scale {
                get; set;
            } = ViewScale.ScaleRate.Normal;

            /// <summary>
            /// ListChangedイベントを発生させます。
            /// </summary>
            protected void OnListChanged() {
                if (!this.IsEditIgnoreMode) {
                    this.ListChanged?.Invoke(this, null);
                }
            }

            /// <summary>
            /// DataChangedイベントを発生させます。
            /// </summary>
            protected void OnDataChanged() {
                if (!this.IsEditIgnoreMode) {
                    this.DataChanged?.Invoke(this, null);
                }
            }

            /// <summary>
			/// Edittedイベントを発生させます。
			/// </summary>
			protected void OnEditted() {
                if (!this.IsEditIgnoreMode) {
                    this.Editted?.Invoke(this, null);
                }
            }

            /// <summary>
            /// 適切なFixedIDを生成します。
            /// </summary>
            public int GenerateFixedID() {
                return Database.GenerateFixedRowID(this.MyDB);
            }

            /// <summary>
            /// 管理リストに一つデータを追加します。
            /// </summary>
            public virtual int AddData(T newData) {
                this.Data.Add(newData);
                this.MyDB.Rows.Add();

                this.PushData(this.MyDB.Rows[this.Data.Count - 1], this.Data.Count - 1);
                this.PushDB();          //変更を内部DBに反映
                this.OnListChanged();   //新規項目を画面上に反映

                this.BeginEdit(this.Data.Count - 1);
                this.EdittingIndex = this.Data.Count - 1;
                this.EdittingData.IsDirty = true;
                this.EdittingData.DataChanged += this.dataChangedRelay;

                if (!this.IsEditIgnoreMode) {
                    this.OnListChanged();
                    this.OnEditted();
                }

                return this.EdittingIndex;
            }

            /// <summary>
            /// 管理リスト上の指定インデックスのデータを複製します。
            /// </summary>
            public virtual int CopyData(int index) {
                return this.CopyData(index, null);
            }

            /// <summary>
            /// CopyDataメソッドをオーバーライドする場合はこのメソッドを呼び出します。
            /// </summary>
            protected int CopyData(int index, Action subRoutine) {
                if (index < 0 || this.Data.Count <= index) {
                    return -1;
                }

                //データを新規行にコピー
                this.MyDB.Rows.Add();
                this.PushData(this.MyDB.Rows[this.Data.Count], this.EdittingIndex);

                //FixedIDを生成する
                this.MyDB.Rows[this.Data.Count].Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value = Database.GenerateFixedRowID(this.MyDB);
                this.SetData(this.MyDB.Rows[this.Data.Count]);      //コピーした行をソースにしてデータを生成する

                this.PushDB();          //変更を内部DBに反映
                this.EndEdit();         //コピー元の変更を保存

                //this.BeginEdit(this.Data.Count - 1);		//このメソッドの戻り値を使ってインデックスを変更し、このメソッドが実行されるのでここでは処理しない

                this.EdittingIndex = this.Data.Count - 1;
                this.EdittingData.IsDirty = true;
                this.EdittingData.DataChanged += this.dataChangedRelay;
                subRoutine?.Invoke();       //必要な処理を実行する

                //ここで選択インデックスが元に戻されてしまう弊害があるため、戻り値のインデックスは一時的に記憶する必要がある
                var idx = this.EdittingIndex;
                this.OnListChanged();
                this.OnEditted();

                return idx;
            }

            /// <summary>
            /// 管理リスト上の指定インデックスのデータを削除します。
            /// </summary>
            public virtual void DeleteData(int index) {
                if (index < 0 || this.Data.Count <= index) {
                    return;
                }
                this.EndEdit();
                this.Data.RemoveAt(index);
                this.PushDB();          //変更を内部DBに反映
                this.OnListChanged();
                this.OnEditted();
            }

            /// <summary>
            /// 管理リスト上の指定インデックスのデータに付属するファイルを削除します。
            /// 削除に成功した場合: 管理リスト上からも削除されます。
            /// 削除に失敗した場合: 処理は中断され、ゴミとして残ります。
            /// </summary>
            protected bool DeleteFileWithData(string folder, string extension, int index) {
                if (index < 0 || this.Data.Count <= index) {
                    return false;
                }

                try {
                    var fileName = ProjectManager.ProjectPath + folder + this.Data[index].FixedID + extension;
                    if (File.Exists(fileName)) {
                        File.Delete(fileName);
                    }
                } catch {
                    return false;
                }

                return true;
            }

            /// <summary>
            /// 管理リストを列番号に基づいて昇順に並び替えます。
            /// </summary>
            /// <param name="columnIndex"></param>
            public virtual void SortDataList(int columnIndex) {
                this.EndEdit();

                switch (columnIndex) {
                    case (int) Database.DBDefaultColumnIndices.VisibleID:
                        this.Data.Sort((x, y) => {
                            return x.VisibleID - y.VisibleID;
                            //return string.Compare(x.VisibleID.ToString(), y.VisibleID.ToString());
                        });
                        break;
                    case (int) Database.DBDefaultColumnIndices.Name:
                        this.Data.Sort((x, y) => {
                            return string.Compare(x.Name, y.Name);
                        });
                        break;
                }

                //内部DBは常に文字列としてソートされる
                this.MyDB.Sort(this.MyDB.Columns[columnIndex], ListSortDirection.Ascending);

                this.OnListChanged();
                this.OnEditted();
            }

            /// <summary>
            /// DataGridViewからの情報に基づいて、管理リストにデータを生成します。
            /// </summary>
            /// <param name="row">ソース行</param>
            public abstract void SetData(DataGridViewRow row);

            /// <summary>
            /// 指定したIDのデータが存在するかどうかを調べます。
            /// </summary>
            public abstract bool ExistsData(int fixedID);

            /// <summary>
            /// 管理リスト上の指定したインデックスの情報を出力用のDataGridView行にしてセットします。
            /// </summary>
            /// <param name="row">代入先オブジェクト</param>
            public virtual void PushData(DataGridViewRow row, int index) {
                if (index < 0 || this.Data.Count <= index) {
                    return;
                }

                //標準列情報
                row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value = this.Data[index].FixedID;
                row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value = this.Data[index].VisibleID;
                row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value = this.Data[index].Name;
            }

            /// <summary>
            /// 管理リストを出力用のDataGridViewに反映させます。
            /// </summary>
            public virtual void PushDB() {
                this.MyDB.Rows.Clear();
                for (var i = 0; i < this.Data.Count; i++) {
                    this.MyDB.Rows.Add();
                    this.PushData(this.MyDB.Rows[i], i);
                }
            }

            /// <summary>
            /// 管理リスト上の指定したインデックスの項目の編集を開始します。
            /// </summary>
            public virtual void BeginEdit(int index) {
                this.BeginEdit(index, null);
            }

            /// <summary>
            /// BeginEditメソッドをオーバーライドする場合はこのメソッドを呼び出します。
            /// </summary>
            protected void BeginEdit(int index, Action subRoutine) {
                this.EndEdit();
                this.EdittingIndex = index;
                subRoutine?.Invoke();       //必要な処理を実行する
                this.OnDataChanged();       //画面に反映する
            }

            /// <summary>
            /// 現在の項目の編集を確定させて終了します。
            /// </summary>
            public virtual void EndEdit() {
                this.EndEdit(null);
            }

            /// <summary>
            /// EndEditメソッドをオーバーライドする場合はこのメソッドを呼び出します。
            /// </summary>
            protected void EndEdit(Action subRoutine) {
                if (this.EdittingIndex == -1) {
                    return;
                } else if (this.EdittingData?.IsDirty == false) {
                    this.EdittingIndex = -1;        //変更がない場合は最低限の処理だけを行う
                    return;
                }

                subRoutine?.Invoke();           //必要な処理を実行する
                this.PushData(this.MyDB.Rows[this.EdittingIndex], this.EdittingIndex);
                this.EdittingIndex = -1;
            }

            /// <summary>
            /// 管理リスト上のデータが変更を発したときのイベントを中継します。
            /// </summary>
            protected virtual void dataChangedRelay(object sender, EventArgs e) {
                if (this.EdittingIndex != -1 && this.EdittingIndex < this.Data.Count) {
                    this.PushData(this.MyDB.Rows[this.EdittingIndex], this.EdittingIndex);
                }
                this.OnDataChanged();
                this.OnEditted();
            }
        }
    }
}
