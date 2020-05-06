using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;

    public partial class ctlDBElementDB : ctlDBBase {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBElementDB() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //各データベースのバインド
            var list = new List<DataGridView>();
            for (var i = 0; i < Common.GetEnumCount<Database.DBElementIndices>(); i++) {
                switch (i) {
                    case (int) Database.DBElementIndices.Attribute:
                        list.Add(this.DBAttribute);
                        break;
                    case (int) Database.DBElementIndices.ItemSubType:
                        list.Add(this.DBItemSubType);
                        break;
                    case (int) Database.DBElementIndices.ItemType:
                        list.Add(this.DBItemMainType);
                        break;
                    case (int) Database.DBElementIndices.Parameter:
                        list.Add(this.DBParameter);
                        break;
                    case (int) Database.DBElementIndices.SkillType:
                        list.Add(this.DBSkillType);
                        break;
                }
            }
            this.DBs = list.ToArray();
        }

        public override DataGridView CurrentSubDB {
            get {
                if (this.ActiveControl is DataGridView) {
                    return this.ActiveControl as DataGridView;
                }
                return null;
            }
        }

        /// <summary>
        /// 小種別を編集する前に、大種別リストを更新する
        /// </summary>
        private void DBItemSubType_CellEnter(object sender, DataGridViewCellEventArgs e) {
            var db = sender as DataGridView;
            if (db == null) {
                return;
            }
            foreach (DataGridViewColumn col in db.Columns) {
                var colInfo = col.Tag as DBColumn;
                if (colInfo != null) {
                    colInfo.RestoreDependColumn(db.Rows, col);
                }
            }
        }
    }
}
