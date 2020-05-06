using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Database {
    using Module;
    using Module.DB;
    using Properties;
    using DBListType = KeyValuePair<string, int>;

    public partial class dlgIDValues : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgIDValues(DataGridView srcDB, string nowList, bool idValueMode) {
            this.InitializeComponent();
            this.IDValueMode = idValueMode;

            //ソースDBのリストを生成する
            Common.SetListControlKeyValuePairMode(this.cmbDBItem);
            Database.CreateComboBoxListFromSrcDB(this.cmbDBItem, srcDB);

            //既存リストを生成する
            var list = new List<ListViewItem>();
            if (!string.IsNullOrEmpty(nowList)) {
                var split = nowList.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                if (split != null) {
                    foreach (var item in split) {
                        var add = new ListViewItem();
                        var subsplit = item.Split(Resources.Split_SimpleList.ToCharArray());
                        add.Text = subsplit[0];
                        for (var i = 1; i < subsplit.Length; i++) {
                            add.SubItems.Add(subsplit[i]);
                        }
                        list.Add(add);
                    }
                }
            }

            //リスト列を設定する
            if (idValueMode) {
                this.uctlListEditor.SetupList(new string[] { "項目", "値" }, list);
            } else {
                this.uctlListEditor.SetupList(new string[] { "項目" }, list);
            }
        }

        /// <summary>
        /// 結果を返します。
        /// Keyには項目名を含む表示用のテキスト、ValueにはFixedIDと値のペアリストが格納されます。
        /// </summary>
        public KeyValuePair<string, string> Result {
            get {
                var viewbuf = "";
                var tagbuf = "";

                foreach (var item in this.uctlListEditor.Result) {
                    var subbuf = "";

                    if (this.IDValueMode) {
                        //IDValueモードのときはValueも付ける
                        subbuf += Resources.Split_SimpleList;
                        subbuf += item.SubItems[1].Text;
                    }

                    subbuf += Resources.Split_LongData;
                    viewbuf += item.Text + subbuf;
                    tagbuf += this.getFixedIDFromListItem(item).ToString() + subbuf;
                }

                viewbuf = Common.CutLastChar(viewbuf, Resources.Split_LongData.ToCharArray()[0]);
                tagbuf = Common.CutLastChar(tagbuf, Resources.Split_LongData.ToCharArray()[0]);
                return new KeyValuePair<string, string>(viewbuf, tagbuf);
            }
        }

        /// <summary>
        /// 項目に対して値を関連付けるどうかを示します。
        /// </summary>
        public bool IDValueMode {
            get {
                return this.idValueMode;
            }
            set {
                this.idValueMode = value;
                this.numValue.Visible = this.idValueMode;
                this.lblValue.Visible = this.idValueMode;
            }
        }
        private bool idValueMode;

        /// <summary>
        /// 指定したListViewItemから、その項目のFixedIDを取得します。
        /// </summary>
        private int getFixedIDFromListItem(ListViewItem item) {
            foreach (DBListType cmbitem in this.cmbDBItem.Items) {
                if (cmbitem.Key == item.Text) {
                    return cmbitem.Value;
                }
            }
            return -1;
        }

        /// <summary>
        /// 確定前に自動で昇順ソートする
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            //リストは任意で並び順を変えられるようにしており、ユーザーの意図を無視して並び替えを行うのはナンセンスであると判断した
            //uctlListEditor.SortList();
        }

        /// <summary>
        /// リストを選択すると入力ソース欄に反映する
        /// </summary>
        private void ctlListEditor1_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            //DB項目を選択
            Database.SelectComboBoxItemFromIDName(this.cmbDBItem, e.Item.Text);
            if (this.IDValueMode) {
                this.numValue.Value = int.Parse(e.Item.SubItems[1].Text);
            }
        }

        /// <summary>
        /// アイテム情報をセット
        /// </summary>
        private void ctlListEditor1_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (this.cmbDBItem.SelectedIndex == -1) {
                e.Canceled = true;
                return;
            }
            e.Item.Text = ((DBListType) this.cmbDBItem.SelectedItem).Key;
            e.Item.SubItems.Add(this.numValue.Value.ToString());
        }

        /// <summary>
        /// すべてのアイテムをセット
        /// </summary>
        private void uctlListEditor_RequestAddItemAll(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemAllEventArgs e) {
            foreach(var item in this.cmbDBItem.Items) {
                var newItem = new ListViewItem(((DBListType) item).Key);
                newItem.SubItems.Add(this.numValue.Value.ToString());
                e.Items.Add(newItem);
            }
        }

        /// <summary>
        /// 入力ソース欄を初期化
        /// </summary>
        private void ctlListEditor1_RequestClearListEditorSource(object sender, EventArgs e) {
            this.cmbDBItem.SelectedIndex = -1;
            this.numValue.Value = 0;
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void ctlListEditor1_RequestWatchControls(object sender, EventArgs e) {
            this.uctlListEditor.SetCanApply(this.cmbDBItem.SelectedIndex != -1);
        }

    }
}
