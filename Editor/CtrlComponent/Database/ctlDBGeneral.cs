using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;

    /// <summary>
    /// 汎用的なデータベースです。
    /// </summary>
    public partial class ctlDBGeneral : ctlDBBase {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBGeneral() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this.DB);
            this.DBs = new DataGridView[] { this.DB };
        }

        public override void SrcReset(Dictionary<Database.DBIndices, ctlDBBase> DBList) {
            //表示範囲が変更された場合は画像セルを読み直す
            foreach (DataGridViewColumn col in this.DB.Columns) {
                var colTag = col.Tag as DBColumnPictureFile;

                if (colTag == null) {
                    //該当しないセルはスキップ
                    continue;
                } else if (!colTag.VisibleRangeDependMode
                || colTag.VisibleRange == (Rectangle) (DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.DBCharGraphicsViewRange, 0].Value)) {
                    //表示が変更されていないか、固定範囲の場合はスキップ
                    continue;
                }

                //この列のすべてのセルを再描画
                foreach (DataGridViewRow row in this.DB.Rows) {
                    if (!row.IsNewRow) {
                        colTag.Paint(row, this.DB[col.Index, row.Index]);
                    }
                }
            }
        }
    }
}
