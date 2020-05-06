using System.Collections;
using System.Windows.Forms;

namespace Editor.Module {

    /// <summary>
    /// ListViewの並び替えに文字列比較を使用する比較クラス
    /// </summary>
    public class ListViewItemStringComparer : IComparer {

        /// <summary>
        /// 並び替える対象の列インデックス
        /// </summary>
        private readonly int columnIndex;

        /// <summary>
        /// 並び替えの順序
        /// </summary>
        private readonly SortOrder sortOrder;

        /// <summary>
        /// コンストラクター
        /// </summary>
        /// <param name="columnIndex">並び替える対象の列インデックス</param>
        /// <param name="sortOrder">並び替えの順序</param>
        public ListViewItemStringComparer(int columnIndex, SortOrder sortOrder) {
            this.columnIndex = columnIndex;
            this.sortOrder = sortOrder;
        }

        /// <summary>
        /// 比較実行
        /// </summary>
        /// <param name="x">ListViewItem-X</param>
        /// <param name="y">ListViewItem-Y</param>
        /// <returns>比較結果</returns>
        public int Compare(object x, object y) {
            var _x = (ListViewItem) x;
            var _y = (ListViewItem) y;
            var compareValue = _x.SubItems[this.columnIndex].Text.CompareTo(_y.SubItems[this.columnIndex].Text);
            if (this.sortOrder == SortOrder.Descending) {
                return -compareValue;
            } else {
                return compareValue;
            }
        }

    }

}
