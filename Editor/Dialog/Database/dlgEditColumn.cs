using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, Module.DB.Database.DBAddress>;

    public partial class dlgEditColumn : Form {
        private readonly AllDB dbList = null;

        /// <summary>
        /// このダイアログで編集できる列の種別
        /// </summary>
        private enum DBColumnType {
            TextOneNumber,
            TextNumbers,
            Text,
            Bool,
            List,
            TextIDNames,
            Button,
            PictureFile,
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEditColumn(DBColumn col, DataGridView dgv, CtrlComponent.Database.ctlDatabaseEditor mgr) {
            this.InitializeComponent();
            this.dbList = mgr.DBList;
            this.uctlMyList.SetupList(new string[] { "項目名" }, null);

            //依存先データベースリストを生成
            var dblist = mgr.GetAllDBList();
            ComboBox[] tgt = { this.cmbDBsList, this.cmbDBsIDNames, this.cmbButtonSrcDB };
            foreach (var cmb in tgt) {
                Common.SetListControlKeyValuePairMode(cmb);
                foreach (var db in dblist) {
                    var name = mgr.GetDBName(db.Key);
                    if (string.IsNullOrEmpty(name) == false && db.Value.AllowUserToAddRows) {
                        cmb.Items.Add(new DBListType(name, db.Key));
                    }
                }
            }

            //ComboBoxの項目を選択する処理の定義
            var restoreSrcDB = new Action<ComboBox, Database.DBAddress>((cmb, address) => {
                var i = 0;
                foreach (DBListType cmbList in cmb.Items) {
                    if (address == new Database.DBAddress(cmbList.Value.MainID, cmbList.Value.SubID)) {
                        cmb.SelectedIndex = i;
                        break;
                    }
                    i++;
                }
            });

            //既存データを復元する
            if (col == null) {
                return;     //新規作成の場合は処理しない
            }

            this.txtHeaderText.Text = col.HeaderText;

            //クラスの継承関係の都合上、末端の派生先から先に確認していく
            if (col is DBColumnTextNumbers) {
                //テキスト系
                this.tbcFormType.SelectedIndex = (int) DBColumnType.TextNumbers;
                this.numIntsMax.Value = ((DBColumnTextOneNumber) col).MaxValue;
                this.numIntsMin.Value = ((DBColumnTextOneNumber) col).MinValue;
            } else if (col is DBColumnTextOneNumber) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.TextOneNumber;
                this.numOneIntMax.Value = ((DBColumnTextOneNumber) col).MaxValue;
                this.numOneIntMin.Value = ((DBColumnTextOneNumber) col).MinValue;
            } else if (col is DBColumnTextIDNames) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.TextIDNames;
                this.chkTextIDValueMode.Checked = false;
                restoreSrcDB(this.cmbDBsIDNames, ((DBColumnTextIDNames) col).SrcDBAddress);
            } else if (col is DBColumnTextIDValues) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.TextIDNames;
                this.chkTextIDValueMode.Checked = true;
                restoreSrcDB(this.cmbDBsIDNames, ((DBColumnTextIDValues) col).SrcDBAddress);
            } else if (col is DBColumnListDB) {
                //リスト系
                this.tbcFormType.SelectedIndex = (int) DBColumnType.List;
                this.rdbFromDB.Checked = true;
                restoreSrcDB(this.cmbDBsList, ((DBColumnListDB) col).SrcDBAddress);
            } else if (col is DBColumnListUser) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.List;
                this.rdbFromMyList.Checked = true;
                var editList = new List<ListViewItem>();
                foreach (var item in ((DBColumnListUser) col).SrcMyList) {
                    editList.Add(new ListViewItem(item));
                }
                this.uctlMyList.SetupList(new string[] { "項目名" }, editList);
            } else if (col is DBColumnButtonInputIDNames) {
                //ボタン系
                this.tbcFormType.SelectedIndex = (int) DBColumnType.Button;
                this.rdbButtonIDNames.Checked = true;
                this.chkButtonIDValueMode.Checked = false;
                restoreSrcDB(this.cmbButtonSrcDB, ((DBColumnButtonInputIDNames) col).SrcDBAddress);
                this.numIDNameSetterDestColumnIndex.Value = ((DBColumnButtonInputIDNames) col).DestColumnIndex;
            } else if (col is DBColumnButtonInputIDValues) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.Button;
                this.rdbButtonIDNames.Checked = true;
                this.chkButtonIDValueMode.Checked = true;
                restoreSrcDB(this.cmbButtonSrcDB, ((DBColumnButtonInputIDValues) col).SrcDBAddress);
                this.numIDNameSetterDestColumnIndex.Value = ((DBColumnButtonInputIDValues) col).DestColumnIndex;
            } else if (col is DBColumnButtonSelectFile) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.Button;
                this.rdbButtonFileSelect.Checked = true;
                this.numFileSetterDestColumnIndex.Value = ((DBColumnButtonSelectFile) col).DestColumnIndex;
            } else if (col is DBColumnText) {
                //基本系
                this.tbcFormType.SelectedIndex = (int) DBColumnType.Text;
            } else if (col is DBColumnBool) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.Bool;
            } else if (col is DBColumnPictureFile) {
                this.tbcFormType.SelectedIndex = (int) DBColumnType.PictureFile;
            }
        }

        /// <summary>
        /// 操作の結果を返します。
        /// </summary>
        public DBColumn Result {
            get {
                switch (this.tbcFormType.SelectedIndex) {
                    case (int) DBColumnType.TextOneNumber:
                        return new DBColumnTextOneNumber(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, (int) this.numOneIntMin.Value, (int) this.numOneIntMax.Value, (int) this.numOneIntMin.Value);

                    case (int) DBColumnType.TextNumbers:
                        return new DBColumnTextNumbers(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, (int) this.numIntsMin.Value, (int) this.numIntsMax.Value, new int[] { (int) this.numOneIntMin.Value });

                    case (int) DBColumnType.Text:
                        return new DBColumnText(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, false);

                    case (int) DBColumnType.Bool:
                        return new DBColumnBool(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth);

                    case (int) DBColumnType.List:
                        if (this.rdbFromDB.Checked) {
                            //DBリスト
                            return new DBColumnListDB(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, ((KeyValuePair<string, Database.DBAddress>) this.cmbDBsList.SelectedItem).Value);
                        } else if (this.rdbFromMyList.Checked) {
                            //ユーザー定義リスト
                            var src = new List<string>();
                            foreach (var item in this.uctlMyList.Result) {
                                src.Add(item.Text);
                            }
                            return new DBColumnListUser(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, src);
                        }
                        break;

                    case (int) DBColumnType.TextIDNames:
                        if (this.chkTextIDValueMode.Checked) {
                            return new DBColumnTextIDValues(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, ((KeyValuePair<string, Database.DBAddress>) this.cmbDBsIDNames.SelectedItem).Value);
                        } else {
                            return new DBColumnTextIDNames(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, ((KeyValuePair<string, Database.DBAddress>) this.cmbDBsIDNames.SelectedItem).Value);
                        }

                    case (int) DBColumnType.Button:
                        if (this.rdbButtonIDNames.Checked) {
                            if (this.chkButtonIDValueMode.Checked) {
                                return new DBColumnButtonInputIDValues(this.dbList, (int) this.numIDNameSetterDestColumnIndex.Value, ((KeyValuePair<string, Database.DBAddress>) this.cmbButtonSrcDB.SelectedItem).Value);
                            } else {
                                return new DBColumnButtonInputIDNames(this.dbList, (int) this.numIDNameSetterDestColumnIndex.Value, ((KeyValuePair<string, Database.DBAddress>) this.cmbButtonSrcDB.SelectedItem).Value);
                            }
                        } else if (this.rdbButtonFileSelect.Checked) {
                            return new DBColumnButtonSelectFile(this.dbList, (int) this.numFileSetterDestColumnIndex.Value, "", "");
                        }
                        break;

                    case (int) DBColumnType.PictureFile:
                        return new DBColumnPictureFile(this.dbList, this.txtHeaderText.Text, Database.UserDBColumnWidth, Rectangle.Empty);
                }
                return null;
            }
        }

        /// <summary>
        /// 切替：ボタン
        /// </summary>
        private void rdbToScript_CheckedChanged(object sender, EventArgs e) {
            this.numIDNameSetterDestColumnIndex.Enabled = this.rdbButtonIDNames.Checked;
            this.cmbButtonSrcDB.Enabled = this.rdbButtonIDNames.Checked;
            this.numFileSetterDestColumnIndex.Enabled = this.rdbButtonFileSelect.Checked;
        }

        /// <summary>
        /// 切替：リスト
        /// </summary>
        private void rdbFromMyList_CheckedChanged(object sender, EventArgs e) {
            this.cmbDBsList.Enabled = this.rdbFromDB.Checked;
            this.txtEditMyList.Enabled = this.rdbFromMyList.Checked;
            this.uctlMyList.Enabled = this.rdbFromMyList.Checked;
        }

        /// <summary>
        /// リストを選択すると入力ソース欄に反映する
        /// </summary>
        private void uctlMyList_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            this.txtEditMyList.Text = e.Item.Text;
        }

        /// <summary>
        /// アイテム情報をセット
        /// </summary>
        private void uctlMyList_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (string.IsNullOrEmpty(this.txtEditMyList.Text)) {
                e.Canceled = true;
                return;
            }
            e.Item.Text = this.txtEditMyList.Text;
        }

        /// <summary>
        /// 入力ソース欄を初期化
        /// </summary>
        private void uctlMyList_RequestClearListEditorSource(object sender, EventArgs e) {
            this.txtEditMyList.Text = "";
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void uctlMyList_RequestWatchControls(object sender, EventArgs e) {
            this.uctlMyList.SetCanApply(!string.IsNullOrEmpty(this.txtEditMyList.Text));
        }

        /// <summary>
        /// 確定前にエラーチェックを行う
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            switch (this.tbcFormType.SelectedIndex) {
                case (int) DBColumnType.TextOneNumber:
                    if (this.numOneIntMax.Value < this.numOneIntMin.Value) {
                        MessageBox.Show("最小値と最大値の大小関係が不正です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        this.DialogResult = DialogResult.None;
                    }
                    break;

                case (int) DBColumnType.TextNumbers:
                    if (this.numIntsMax.Value < this.numIntsMin.Value) {
                        MessageBox.Show("最小値と最大値の大小関係が不正です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        this.DialogResult = DialogResult.None;
                    }
                    break;

                case (int) DBColumnType.List:
                    if (this.rdbFromDB.Checked) {
                        if (this.cmbDBsList.SelectedIndex == -1) {
                            MessageBox.Show("既存リストが選択されていません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            this.DialogResult = DialogResult.None;
                        }
                    } else if (this.rdbFromMyList.Checked) {
                        if (this.uctlMyList.Result.Count == 0) {
                            MessageBox.Show("独自リストに項目が一つもありません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            this.DialogResult = DialogResult.None;
                        }
                    }
                    break;

                case (int) DBColumnType.TextIDNames:
                    if (this.cmbDBsIDNames.SelectedIndex == -1) {
                        MessageBox.Show("値のソースを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        this.DialogResult = DialogResult.None;
                    }
                    break;

                case (int) DBColumnType.Button:
                    if (this.rdbButtonIDNames.Checked && this.cmbButtonSrcDB.SelectedIndex == -1) {
                        MessageBox.Show("値のソースを選択して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        this.DialogResult = DialogResult.None;
                    }
                    break;
            }
        }
    }
}
