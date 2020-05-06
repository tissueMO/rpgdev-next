using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Text.Script {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, Module.DB.Database.DBAddress>;

    public partial class dlgInsertDB : Form {
        private readonly AllDB dbList;
        private readonly bool columnMode;            //項目ではなく列を選択するモードであるかどうか
        private readonly bool valueMode;             //共通変数を挿入するモードであるかどうか

        /// <summary>
        /// コンストラクター
        /// </summary>
        /// <param name="DBList">すべてのデータベース</param>
        /// <param name="isColumnMode">行ではなく列を選択するモードにするかどうか</param>
        /// <param name="isValueMode">共通変数を選択するモードにするかどうか</param>
        public dlgInsertDB(AllDB DBList, bool isColumnMode, bool isValueMode) {
            if (DBList == null) {
                //データベースのインスタンスがないと、ダイアログをセットアップできないのでキャンセル
                this.Close();
                return;
            }

            this.InitializeComponent();
            this.dbList = DBList;
            this.valueMode = isValueMode;

            if (isValueMode) {
                //共通変数を挿入するモード
                this.columnMode = false;
                this.chkColumnNameMode.Visible = false;       //列名は挿入できない

                //共通変数限定のデータベースリストを生成する
                this.lstDB.Items.Clear();
                Common.SetListControlKeyValuePairMode(this.lstDB);
                for (var i = 0; i <= (int) Database.DBValueIndices.CommonString; i++) {
                    var dbKey = new Database.DBAddress(Database.DBIndices.Value, i);
                    this.lstDB.Items.Add(new DBListType(CtrlComponent.Database.ctlDatabaseEditor.GetDBName(this.dbList, dbKey), dbKey));
                }
            } else {
                //データベース項目を挿入するモード
                this.columnMode = isColumnMode;
                this.chkColumnNameMode.Visible = isColumnMode;
                this.chkColumnNameMode.Checked = false;

                //データベースリストを生成する
                this.lstDB.Items.Clear();
                Common.SetListControlKeyValuePairMode(this.lstDB);
                var dblist = CtrlComponent.Database.ctlDatabaseEditor.GetAllDBList(this.dbList);
                foreach (var db in dblist) {
                    var name = CtrlComponent.Database.ctlDatabaseEditor.GetDBName(this.dbList, db.Key);
                    if (string.IsNullOrEmpty(name) == false && db.Value.AllowUserToAddRows) {
                        this.lstDB.Items.Add(new DBListType(name, db.Key));
                    }
                }
            }

            //項目リストの列をセットアップする
            this.ltvDBItem.Columns.Clear();
            if (!isColumnMode) {
                //項目モード
                this.ltvDBItem.Columns.Add("FixedID", 0);
                this.ltvDBItem.Columns.Add("ID", 60);
                this.ltvDBItem.Columns.Add("名前", 150);
            } else {
                //列名モード
                this.ltvDBItem.Columns.Add("Index", 60);
                this.ltvDBItem.Columns.Add("名前", 150);
            }

            //既定のデータベースを選択する
            this.lstDB.SelectedIndex = (Settings.Default.Last_DBType < this.lstDB.Items.Count) ? Settings.Default.Last_DBType : 0;
        }

        /// <summary>
        /// 列選択モードのときの項目リストの列インデックス
        /// </summary>
        private enum ColumnModeColumnIndices {
            Index,
            Name,
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                if (this.valueMode) {
                    //２文字のサフィックス＋10進数のFixedID
                    var suffix = "";
                    switch (this.lstDB.SelectedIndex) {
                        case (int) Database.DBValueIndices.CommonFlag:
                            suffix = Resources.SQ_UserFlag;
                            break;
                        case (int) Database.DBValueIndices.CommonInteger:
                            suffix = Resources.SQ_UserInteger;
                            break;
                        case (int) Database.DBValueIndices.CommonString:
                            suffix = Resources.SQ_UserString;
                            break;
                    }
                    return suffix + this.ltvDBItem.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.FixedID].Text
                        + "/*" + this.ltvDBItem.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text + "*/";
                } else if (!this.columnMode) {
                    return this.ResultIDName;
                } else if (this.chkColumnNameMode.Checked) {
                    return "\"" + this.ResultColumnName + "\"";
                } else {
                    return this.ResultColumnIndexName;
                }
            }
        }

        /// <summary>
        /// 選択項目を [FixedID/*名前*/] の形式で返す
        /// </summary>
        private string ResultIDName {
            get {
                if (this.columnMode == false && this.ltvDBItem.SelectedIndices.Count > 0) {
                    if (!Settings.Default.Script_FixedIDHexMode) {
                        //FixedID=10進数モード
                        return this.ltvDBItem.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.FixedID].Text
                             + "/*" + this.ltvDBItem.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text + "*/";
                    } else {
                        //FixedID=16進数モード
                        return "0x" + int.Parse(this.ltvDBItem.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.FixedID].Text).ToString("X")
                             + "/*" + this.ltvDBItem.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text + "*/";
                    }
                } else {
                    return "";
                }
            }
        }

        /// <summary>
        /// 選択項目を [Index/*列名*/] の形式で返す
        /// </summary>
        private string ResultColumnIndexName {
            get {
                if (this.columnMode && this.ltvDBItem.SelectedIndices.Count > 0) {
                    return this.ltvDBItem.SelectedItems[0].SubItems[(int) ColumnModeColumnIndices.Index].Text
                         + "/*" + this.ltvDBItem.SelectedItems[0].SubItems[(int) ColumnModeColumnIndices.Name].Text + "*/";
                } else {
                    return "";
                }
            }
        }

        /// <summary>
        /// 選択項目を [列名] の形式で返す
        /// </summary>
        private string ResultColumnName {
            get {
                if (this.columnMode && this.ltvDBItem.SelectedIndices.Count > 0) {
                    return this.ltvDBItem.SelectedItems[0].SubItems[(int) ColumnModeColumnIndices.Name].Text;
                } else {
                    return "";
                }
            }
        }

        /// <summary>
        /// DBを選択したら項目を一覧する
        /// </summary>
        private void lstDB_SelectedIndexChanged(object sender, EventArgs e) {
            this.ltvDBItem.BeginUpdate();
            this.ltvDBItem.Items.Clear();

            if (this.lstDB.SelectedIndex != -1) {
                var address = ((DBListType) this.lstDB.SelectedItem).Value;
                if (!this.columnMode) {
                    //行リスト
                    foreach (DataGridViewRow row in this.dbList[address.MainID].DBs[address.SubID].Rows) {
                        if (row.IsNewRow) {
                            //新規行は処理しない
                            continue;
                        }
                        var newItem = new ListViewItem {
                            Text = row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value?.ToString()
                        };
                        newItem.SubItems.Add(row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value?.ToString());
                        newItem.SubItems.Add(row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value?.ToString());
                        this.ltvDBItem.Items.Add(newItem);
                    }
                } else {
                    //列リスト
                    foreach (DataGridViewColumn col in this.dbList[address.MainID].DBs[address.SubID].Columns) {
                        var newItem = new ListViewItem {
                            Text = col.DisplayIndex.ToString()
                        };
                        newItem.SubItems.Add(col.HeaderText);
                        this.ltvDBItem.Items.Add(newItem);
                    }
                }
            }
            this.ltvDBItem.EndUpdate();
        }

        /// <summary>
        /// ダブルクリックで確定する
        /// </summary>
        private void ltvDBItem_DoubleClick(object sender, EventArgs e) {
            if (this.ltvDBItem.SelectedIndices.Count > 0) {
                this.ctlOKCancel1.ApplyOK(sender, e);
            }
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            if (this.ltvDBItem.SelectedIndices.Count == 0) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "項目"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            } else {
                Settings.Default.Last_DBType = this.lstDB.SelectedIndex;
            }
        }
    }
}
