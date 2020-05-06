using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AddonItem = KeyValuePair<string, Module.DB.Database.DBAddonType>;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, int>;

    public partial class dlgAddOn : Form {
        private readonly AllDB Manager;

        /// <summary>
        /// アドオン情報の表示順序
        /// </summary>
        private enum AddonInfoIndices {
            AddonType,
            DBItem,
            Option,
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgAddOn(AllDB mgr, string nowList) {
            this.InitializeComponent();
            this.Manager = mgr;

            //アドオンの効果内容リストを生成する
            this.cmbType.Items.Clear();
            Common.SetListControlKeyValuePairMode(this.cmbType);
            var addonList = DBLiteral.GetListAddon();
            foreach (var item in addonList) {
                this.cmbType.Items.Add(new AddonItem(item.Name, item));
            }

            //既存リストを生成する
            var list = new List<ListViewItem>();
            if (!string.IsNullOrEmpty(nowList)) {
                var split = nowList.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

                if (split != null) {
                    foreach (var item in split) {
                        var add = new ListViewItem();
                        var subsplit = item.Split(Resources.Split_SimpleList.ToCharArray());
                        add.Text = ((AddonItem) this.cmbType.Items[int.Parse(subsplit[(int) AddonInfoIndices.AddonType])]).Key;

                        //実際にユーザーが作成するのと同じ手順を踏むことで、DBSrcも反映される
                        //対応する関連IDを取得する
                        this.cmbType.SelectedItem = ((AddonItem) this.cmbType.Items[int.Parse(subsplit[(int) AddonInfoIndices.AddonType])]);
                        this.cmbDBSrc.SelectedIndex = -1;
                        add.SubItems.Add("");
                        add.SubItems[add.SubItems.Count - 1].Tag = "";

                        foreach (DBListType dbitem in this.cmbDBSrc.Items) {
                            //FixedIDで探索する
                            if (dbitem.Value.ToString() == subsplit[(int) AddonInfoIndices.DBItem]) {
                                add.SubItems[add.SubItems.Count - 1].Text = dbitem.Key;
                                add.SubItems[add.SubItems.Count - 1].Tag = dbitem.Value.ToString();
                                break;
                            }
                        }

                        add.SubItems.Add(subsplit[(int) AddonInfoIndices.Option]);
                        list.Add(add);
                    }
                }
            }

            this.uctlListEditor.SetupList(new string[] { "内容", "関連ID", "値" }, list);
            this.cmbType.SelectedIndex = -1;
            this.cmbType_SelectedIndexChanged(this, null);
        }

        /// <summary>
        /// 結果を返します。
        /// そのまま保存できる形式（データベースの項目はFixedID）で出力されます。
        /// </summary>
        public string Result {
            get {
                var buf = "";

                foreach (var item in this.uctlListEditor.Result) {
                    buf += Common.GetComboBoxIndexFromText<Database.DBAddonType>(this.cmbType, item.Text);
                    buf += Resources.Split_SimpleList;
                    buf += item.SubItems[(int) AddonInfoIndices.DBItem].Tag?.ToString();       //FixedID
                    buf += Resources.Split_SimpleList;
                    buf += item.SubItems[(int) AddonInfoIndices.Option].Text;
                    buf += Resources.Split_LongData;
                }

                buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
                return buf;
            }
        }

        /// <summary>
        /// リストを選択すると入力ソース欄に反映する
        /// </summary>
        private void uctlListEditor_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            var addons = DBLiteral.GetListAddon();
            for (var i = 0; i < addons.Count; i++) {
                if (addons[i].Name == e.Item.SubItems[(int) AddonInfoIndices.AddonType].Text) {
                    this.cmbType.SelectedIndex = i;
                    break;
                }
            }

            //対応する関連IDを選択する
            this.cmbDBSrc.SelectedIndex = -1;
            Database.SelectComboBoxItemFromIDName(this.cmbDBSrc, e.Item.SubItems[(int) AddonInfoIndices.DBItem].Text);
            this.numOption.Value = int.Parse(e.Item.SubItems[(int) AddonInfoIndices.Option].Text);
        }

        /// <summary>
        /// アドオン情報をListViewItemにセット
        /// </summary>
        private void uctlListEditor_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (this.cmbType.SelectedIndex == -1) {
                e.Canceled = true;
                return;
            } else if (this.cmbDBSrc.Enabled && this.cmbDBSrc.SelectedIndex == -1) {
                e.Canceled = true;
                return;
            }

            e.Item.Text = ((AddonItem) this.cmbType.SelectedItem).Key;

            if (this.cmbDBSrc.SelectedIndex == -1) {
                e.Item.SubItems.Add("");
                e.Item.SubItems[(int) AddonInfoIndices.DBItem].Tag = "";
            } else {
                e.Item.SubItems.Add(((DBListType) this.cmbDBSrc.SelectedItem).Key);
                e.Item.SubItems[(int) AddonInfoIndices.DBItem].Tag = ((DBListType) this.cmbDBSrc.SelectedItem).Value;         //TagにFixedIDを隠し込む
            }

            e.Item.SubItems.Add(this.numOption.Value.ToString());
        }

        /// <summary>
        /// 入力ソース欄を初期化
        /// </summary>
        private void uctlListEditor_RequestClearListEditorSource(object sender, EventArgs e) {
            this.cmbType.SelectedIndex = -1;
            this.cmbDBSrc.SelectedIndex = -1;
            this.numOption.Value = 0;
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void uctlListEditor_RequestWatchControls(object sender, EventArgs e) {
            var OK = true;
            OK &= (this.cmbType.SelectedIndex != -1);
            OK &= (this.cmbDBSrc.Enabled == false || this.cmbDBSrc.SelectedIndex != -1);
            this.uctlListEditor.SetCanApply(OK);
        }

        /// <summary>
        /// オプション値を初期化する
        /// </summary>
        private void ChangeOptionValue(bool enabled, bool isBoolMode) {
            this.numOption.Enabled = enabled;

            if (isBoolMode) {
                this.numOption.Minimum = 0;
                this.numOption.Maximum = 1;
            } else {
                this.numOption.Minimum = int.MinValue;
                this.numOption.Maximum = int.MaxValue;
            }

            this.numOption.Value = 0;
        }

        /// <summary>
        /// データベースのソースを設定する
        /// </summary>
        private void ChangeDBSrc(Database.DBAddress address) {
            this.cmbDBSrc.SelectedIndex = -1;

            if (address == null) {
                this.cmbDBSrc.Items.Clear();
                this.cmbDBSrc.Enabled = false;
            } else {
                Database.CreateComboBoxListFromSrcDB(this.cmbDBSrc, this.Manager[address.MainID].DBs[address.SubID]);
                this.cmbDBSrc.Enabled = true;
            }
        }

        /// <summary>
        /// 選択されたアドオン情報に応じてオプションの入力可否を変える
        /// </summary>
        private void cmbType_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.cmbType.SelectedIndex == -1) {
                return;
            }

            var item = ((AddonItem) this.cmbType.SelectedItem).Value;
            this.ChangeOptionValue(item.OptionValue, item.OptionValueBoolMode);
            this.ChangeDBSrc(item.SrcDB);
            this.uctlListEditor_RequestWatchControls(sender, e);
        }
    }
}
