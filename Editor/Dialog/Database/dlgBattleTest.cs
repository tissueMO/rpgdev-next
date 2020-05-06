using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, int>;

    public partial class dlgBattleTest : Form {
        private readonly AllDB dbList = null;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgBattleTest(AllDB dbList) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.dbList = dbList;

            //ソースDBのリストを生成する
            Common.SetListControlKeyValuePairMode(this.cmbDBChar);
            Database.CreateComboBoxListFromSrcDB(this.cmbDBChar, dbList[Database.DBIndices.Char].DBs[0]);

            //既存リストを生成する
            var list = new List<ListViewItem>();
            if (!string.IsNullOrEmpty(Settings.Default.Last_BattleTestPTData)) {
                var split = Settings.Default.Last_BattleTestPTData.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                if (split != null) {
                    foreach (var charData in split) {
                        var subspl = charData.Split(Resources.Split_IDNamePair.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                        if (subspl.Length != 2) {
                            continue;       //不正な形式
                        }
                        var idName = Database.GetIDNameFromFixedID(this.dbList[Database.DBIndices.Char].DBs[0], int.Parse(subspl[0]));
                        if (!string.IsNullOrEmpty(idName)) {
                            var add = new ListViewItem {
                                Text = subspl[0]
                            };
                            add.SubItems.Add(Database.GetIDFromIDName(idName).ToString());
                            add.SubItems.Add(Database.GetNameFromIDName(idName));
                            add.SubItems.Add(subspl[1]);
                            list.Add(add);
                        }
                    }
                }
            }

            //リスト列を設定する
            this.uctlPartyEditor.SetupList(new string[] { "ID", "名前", "レベル" }, list, true, 1);
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get;
            private set;
        } = "";

        /// <summary>
        /// テスト開始
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            //パーティ情報を作る
            var PTData = "";

            foreach (var item in this.uctlPartyEditor.Result) {
                PTData += item.SubItems[(int) Database.DBDefaultColumnIndices.FixedID].Text;
                PTData += Resources.Split_IDNamePair;
                PTData += item.SubItems[(int) Database.DBDefaultColumnIndices.Count].Text;       //レベル
                PTData += Resources.Split_LongData;
            }

            PTData = Common.CutLastChar(PTData, Resources.Split_LongData.ToCharArray()[0]);
            this.Result = PTData;
            this.Close();
        }

        /// <summary>
        /// リストを選択すると入力ソース欄に反映する
        /// </summary>
        private void uctlPartyEditor_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            Database.SelectComboBoxItemFromIDName(this.cmbDBChar, e.Item.SubItems[(int) Database.DBDefaultColumnIndices.VisibleID].Text + Resources.Split_IDNamePair + e.Item.SubItems[(int) Database.DBDefaultColumnIndices.Name].Text);
            this.numLevel.Value = int.Parse(e.Item.SubItems[(int) Database.DBDefaultColumnIndices.Count].Text);
        }

        /// <summary>
        /// パーティキャラ情報をセット
        /// </summary>
        private void uctlPartyEditor_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (this.cmbDBChar.SelectedIndex == -1) {
                e.Canceled = true;
                return;
            }
            e.Item.Text = ((DBListType) this.cmbDBChar.SelectedItem).Value.ToString();
            e.Item.SubItems.Add(Database.GetIDFromIDName(((DBListType) this.cmbDBChar.SelectedItem).Key).ToString());
            e.Item.SubItems.Add(Database.GetNameFromIDName(((DBListType) this.cmbDBChar.SelectedItem).Key).ToString());
            e.Item.SubItems.Add(((int) this.numLevel.Value).ToString());
        }

        /// <summary>
        /// パーティキャラ全員をセット
        /// </summary>
        private void uctlPartyEditor_RequestAddItemAll(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemAllEventArgs e) {
            foreach (var item in this.cmbDBChar.Items) {
                var newItem = new ListViewItem(((DBListType) item).Value.ToString());
                newItem.SubItems.Add(Database.GetIDFromIDName(((DBListType) item).Key).ToString());
                newItem.SubItems.Add(Database.GetNameFromIDName(((DBListType) item).Key).ToString());
                newItem.SubItems.Add(((int) this.numLevel.Value).ToString());
                e.Items.Add(newItem);
            }
            this.uctlPartyEditor.SortList();
        }

        /// <summary>
        /// 入力ソース欄を初期化
        /// </summary>
        private void uctlPartyEditor_RequestClearListEditorSource(object sender, EventArgs e) {
            this.cmbDBChar.SelectedIndex = -1;
            this.numLevel.Value = 1.0M;
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void uctlPartyEditor_RequestWatchControls(object sender, EventArgs e) {
            this.uctlPartyEditor.SetCanApply(this.cmbDBChar.SelectedIndex != -1);
        }

    }
}
