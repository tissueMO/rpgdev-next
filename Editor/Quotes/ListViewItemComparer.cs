using System;
using System.Collections;
using System.Windows.Forms;

namespace Editor.Module {

    /// <summary>
    /// ListViewの列ヘッダーをクリックすることで柔軟なソートを行えるようにするソートクラスです。
    /// 使用するには 各列について ColumnModes を設定し、ListViewのソート方法としてこのクラスのオブジェクトを設定して下さい。
    /// </summary>
    public class ListViewItemComparer : IComparer {
        /// <summary>
        /// 比較する方法
        /// </summary>
        public enum ComparerMode {
            String,     //文字列比較
            Integer,    //数値比較
            DateTime    //日時比較
        };

        /// <summary>
        /// 並び替えるListView列のインデックス
        /// </summary>
        public int ColumnIndex {
            get {
                return this.columnIndex;
            }
            set {
                if (this.columnIndex == value) {
                    //現在と同じ列の時は、昇順・降順を切り替える
                    switch (this.Order) {
                        case SortOrder.None:
                            this.Order = SortOrder.Ascending;
                            break;

                        case SortOrder.Ascending:
                            this.Order = SortOrder.Descending;
                            break;

                        case SortOrder.Descending:
                            this.Order = SortOrder.None;
                            break;
                    }
                } else {
                    //現在の列と異なる場合は、昇順に切り替える
                    this.Order = SortOrder.Ascending;
                }
                this.columnIndex = value;
            }
        }
        private int columnIndex = 0;

        /// <summary>
        /// 昇順か降順か
        /// </summary>
        public SortOrder Order {
            get; set;
        } = SortOrder.None;

        /// <summary>
        /// 並び替えの方法
        /// </summary>
        public ComparerMode Mode {
            get; set;
        } = ComparerMode.String;

        /// <summary>
        /// 列ごとの並び替えの方法
        /// </summary>
        public ComparerMode[] ColumnModes {
            set {
                this.columnModes = value;
            }
            private get {
                return this.columnModes;
            }
        }
        private ComparerMode[] columnModes;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ListViewItemComparer(ComparerMode[] columnModes) {
            this.ColumnModes = columnModes;
        }

        /// <summary>
        /// 比較実行
        /// </summary>
        public int Compare(object x, object y) {
            if (this.Order == SortOrder.None) {
                return 0;
            }

            var result = 0;
            var itemx = x as ListViewItem;
            var itemy = y as ListViewItem;

            //並べ替えの方法を決定
            if (this.ColumnModes != null && this.ColumnModes.Length > this.ColumnIndex) {
                this.Mode = this.ColumnModes[this.ColumnIndex];
            }

            //並び替え方法に従って検討する
            switch (this.Mode) {
                case ComparerMode.String:
                    //文字列をとして比較
                    result = string.Compare(
                        itemx.SubItems[this.ColumnIndex].Text,
                        itemy.SubItems[this.ColumnIndex].Text
                    );
                    break;

                case ComparerMode.Integer:
                    //Int32に変換して比較: 整数に変換できない文字列が含まれているときは、そのセルの値をゼロにする
                    int ix, iy;
                    if (!int.TryParse(itemx.SubItems[this.ColumnIndex].Text, out ix)) {
                        ix = 0;
                    }
                    if (!int.TryParse(itemy.SubItems[this.ColumnIndex].Text, out iy)) {
                        iy = 0;
                    }
                    result = ix.CompareTo(iy);
                    break;

                case ComparerMode.DateTime:
                    //DateTimeに変換して比較: 日時に変換できない文字列が含まれているときは、そのセルの値をゼロにする
                    DateTime dx, dy;
                    if (!DateTime.TryParse(itemx.SubItems[this.ColumnIndex].Text, out dx)) {
                        dx = DateTime.MinValue;
                    }
                    if (!DateTime.TryParse(itemy.SubItems[this.ColumnIndex].Text, out dy)) {
                        dy = DateTime.MinValue;
                    }
                    result = DateTime.Compare(dx, dy);
                    break;
            }

            //結果を返す
            if (this.Order == SortOrder.Descending) {
                return -result;
            } else {
                return result;
            }
        }
    }
}
