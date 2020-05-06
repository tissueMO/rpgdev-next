using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    /// <summary>
    /// ユーザーデータベース管理クラス
    /// </summary>
    public partial class mgrDBUserDB : ctlDBGUIBase.DBGUIManagerBase<mgrDBUserDB.UserDBOneData> {
        public static DataGridView TargetDB;       //現在操作対象としているUserDB
        private readonly ctlDBGUIUserDB parent;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public mgrDBUserDB(AllDB DBList, DataGridView mydb, DataGridView targetDB, ctlDBGUIUserDB parent) : base(DBList, mydb) {
            mgrDBUserDB.TargetDB = targetDB;
            this.parent = parent;
            this.parent.IsDirtyChanged += this.isDirtyChanged;
        }

        /// <summary>
        /// 内部DB：標準列の後ろに続くUserDBの列順序
        /// </summary>
        public enum DBUserDBColumnIndices {
            Comment,
            ColumnCount,
        }

        public override void DeleteData(int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }

            if (!this.DeleteFileWithData(Resources.Path_DBs_UserDBs, Resources.Extension_DB, index)) {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "TSVファイルの削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            } else {
                base.DeleteData(index);
            }
        }

        public override void SetData(DataGridViewRow row) {
            //UserDBの原型を生成
            var newItem = new UserDBOneData(
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString()),
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value.ToString()),
                row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value.ToString(),
                row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) DBUserDBColumnIndices.Comment].Value.ToString(),
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) DBUserDBColumnIndices.ColumnCount].Value.ToString()),
                this.DBList
            );

            newItem.DataChanged += this.dataChangedRelay;
            this.Data.Add(newItem);
            this.OnListChanged();
            this.OnEditted();
        }

        public override void PushData(DataGridViewRow row, int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }

            //標準列情報
            base.PushData(row, index);

            //独自情報
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) DBUserDBColumnIndices.Comment].Value = this.Data[index].Comment;
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) DBUserDBColumnIndices.ColumnCount].Value = this.Data[index].ColumnCount;
        }

        public override void BeginEdit(int index) {
            base.BeginEdit(index, new Action(() => {
                this.EdittingData?.LoadFile();
            }));
        }

        public override void EndEdit() {
            base.EndEdit(new Action(() => {
                this.EdittingData?.SaveFile();
            }));
        }

        public override bool ExistsData(int fixedID) {
            foreach (var data in this.Data) {
                if (data.FixedID == fixedID) {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 現在編集中のデータベースが変更されたことを検知したら、編集中のオブジェクトのIsDirtyフラグを立てます。
        /// 管理クラス側でDataGridViewの変更が検知できないためです。
        /// このイベントは実際にフラグが反転するときのみ実行されるため、変更の度にコントロールを再設定することはありません。
        /// </summary>
        private void isDirtyChanged(object sender, EventArgs e) {
            if (this.EdittingData != null && this.parent.IsDirty && !this.EdittingData.IsDirty) {
                this.EdittingData.IsDirty = true;
            }
        }
    }
}
