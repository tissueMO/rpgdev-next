using System;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Properties;

    public partial class dlgSelectInList : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgSelectInList(string title) {
            this.InitializeComponent();
            if (this.ItemCount > 0) {
                this.lstJump.SelectedIndex = 0;
            }
            this.Text = title;
        }

        /// <summary>
        /// リストの項目数
        /// </summary>
        public int ItemCount {
            get {
                return this.lstJump.Items.Count;
            }
        }

        /// <summary>
        /// リストに項目を追加します。
        /// </summary>
        /// <param name="id">行数、インデックスなどの数値的識別情報</param>
        /// <param name="title">表示名</param>
        public void AddItem(int id, string title) {
            this.lstJump.Items.Add(id + Resources.Split_IDNamePair + title);
        }

        /// <summary>
        /// 確定された項目のインデックスを返します。
        /// </summary>
        /// <returns></returns>
        public int GetResultIndex() {
            if (this.lstJump.SelectedIndex != -1) {
                return this.lstJump.SelectedIndex;
            } else {
                return -1;
            }
        }

        /// <summary>
        /// 確定された項目のIDを返します。
        /// </summary>
        public int GetResultID() {
            if (this.lstJump.SelectedIndex != -1) {
                var buf = this.lstJump.SelectedItem.ToString();
                var splpt = buf.IndexOf(Resources.Split_IDNamePair.ToCharArray()[0]);
                return int.Parse(buf.Substring(0, splpt));
            } else {
                return -1;
            }
        }

        /// <summary>
        /// 確定された項目の表示名を返します。
        /// </summary>
        public string GetResultTitle() {
            if (this.lstJump.SelectedIndex != -1) {
                var buf = this.lstJump.SelectedItem.ToString();
                var splpt = buf.IndexOf(Resources.Split_IDNamePair.ToCharArray()[0]);
                return buf.Substring(splpt + 1);
            } else {
                return null;
            }
        }

        /// <summary>
        /// 確定された項目を返します。
        /// </summary>
        /// <param name="id">数値的識別情報 (ID, 行数など)</param>
        /// <param name="title">表示名</param>
        public void GetResults(out int id, out string title) {
            if (this.lstJump.SelectedIndex != -1) {
                var buf = this.lstJump.SelectedItem.ToString();
                var splpt = buf.IndexOf(Resources.Split_IDNamePair.ToCharArray()[0]);
                id = int.Parse(buf.Substring(0, splpt));
                title = buf.Substring(splpt + 1);
            } else {
                id = -1;
                title = null;
            }
        }

        /// <summary>
        /// 確定された項目を [ID:名前] の形式で返します。
        /// </summary>
        public string GetResultIDName() {
            if (this.lstJump.SelectedIndex != -1) {
                return this.lstJump.SelectedItem.ToString();
            } else {
                return "-1:なし";
            }
        }

        /// <summary>
        /// ダブルクリックしても確定
        /// </summary>
        private void lstJump_DoubleClick(object sender, EventArgs e) {
            this.DialogResult = DialogResult.OK;
            this.uctlOKCancel_ClickOK(sender, e);
        }

        /// <summary>
        /// 選択項目を検証
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (this.lstJump.SelectedIndex == -1) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "項目"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            }
        }
    }
}
