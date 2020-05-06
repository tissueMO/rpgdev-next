using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;
    using Properties;

    /// <summary>
    /// 入力ソースを基にリストを生成します。
    /// 入力ソースは配置元のクラスで実装し、ここではそのソースと対話的に処理します。
    /// </summary>
    public partial class ctlListEditor : UserControl {
        public event EventHandler<RequestAddItemEventArgs> ListItemSelected;       //リストが選択されたときに入力ソースへアイテム情報を反映する指示
        public event EventHandler<RequestAddItemEventArgs> RequestAddItem;         //新規アイテムの情報を生成する指示
        public event EventHandler<RequestAddItemAllEventArgs> RequestAddItemAll;   //すべてのアイテムの情報を生成する指示
        public event EventHandler RequestClearListEditorSource;     //入力ソースをクリアーする指示
        public event EventHandler RequestWatchControls;             //コントロールの有効無効をチェックする指示
        public event EventHandler ListChanged;                      //リストの内容が変更されたことを通知する

        /// <summary>
        /// このクラスが発生させるイベントの引数
        /// </summary>
        public class RequestAddItemEventArgs : EventArgs {
            public ListViewItem Item = new ListViewItem("");
            public bool Canceled = false;
            public bool Editted = false;
            public RequestAddItemEventArgs(ListViewItem item, bool editted) {
                this.Item = item;
                this.Editted = editted;
            }
        }

        /// <summary>
        /// すべての項目追加を要求するイベントの引数
        /// </summary>
        public class RequestAddItemAllEventArgs : EventArgs {
            public List<ListViewItem> Items;
            public RequestAddItemAllEventArgs(List<ListViewItem> items) {
                this.Items = items;
            }
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlListEditor() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
        }

        /// <summary>
        /// 項目の重複を許可するかどうか
        /// </summary>
        public bool AllowOverlap {
            get; set;
        } = true;

        /// <summary>
        /// FixedIDの列を含むかどうか
        /// </summary>
        public bool EnabledFixedIDColumn {
            get;
            private set;
        } = false;

        /// <summary>
        /// ソート対象にする列インデックス
        /// </summary>
        public int SortColumnIndex {
            get; set;
        } = 0;

        /// <summary>
        /// すべての項目を一括で追加できるようにするかどうか
        /// </summary>
        public bool EnabledAddAll {
            get {
                return this.enabledAddAll;
            }
            set {
                this.enabledAddAll = value;
                this.btnAddAll.Visible = value;
                this.tableLayout.RowStyles[2].Height = value ? 31 : 0;
            }
        }

        private bool enabledAddAll = false;

        /// <summary>
        /// ユーザーによって作成されたリスト
        /// </summary>
        public List<ListViewItem> Result {
            get {
                var list = new List<ListViewItem>();
                foreach (ListViewItem item in this.ltvList.Items) {
                    list.Add(item);
                }
                return list;
            }
        }

        /// <summary>
        /// 指定されたアイテムリストをセットします。
        /// </summary>
        public void SetupList(string[] columns, List<ListViewItem> list, bool enabledFixedID = false, int sortColumnIndex = 0) {
            //非表示状態のFixedID列を追加する
            this.ltvList.Columns.Clear();
            if (enabledFixedID) {
                this.EnabledFixedIDColumn = enabledFixedID;
                var newCol = new ColumnHeader {
                    Text = "FixedID",
                    Width = 0
                };
                this.ltvList.Columns.Add(newCol);
            }

            //列を登録: 幅は自動調整
            foreach (var col in columns) {
                var newCol = new ColumnHeader {
                    Text = col,
                    Width = this.ltvList.Width / columns.Length - 15
                };
                this.ltvList.Columns.Add(newCol);
            }

            //列ヘッダー設定
            if (columns.Length > 1) {
                this.ltvList.HeaderStyle = ColumnHeaderStyle.Clickable;
            } else {
                //列が単一の場合はヘッダーを表示しない
                this.ltvList.HeaderStyle = ColumnHeaderStyle.None;
            }

            //初期リスト設定
            this.ltvList.Items.Clear();
            if (list != null) {
                foreach (var item in list) {
                    this.ltvList.Items.Add(item);
                }
            }

            //ソート方法設定
            this.SortColumnIndex = sortColumnIndex;

            this.ListChanged?.Invoke(this, null);
        }

        /// <summary>
        /// 登録または変更の適用ができるかをセットします
        /// </summary>
        public void SetCanApply(bool OK) {
            this.btnAdd.Enabled = OK;
            this.btnEdit.Enabled = (OK && (this.ltvList.SelectedIndices.Count > 0));
        }

        /// <summary>
        /// 選択した項目をテキストに反映する
        /// </summary>
        protected void ltvList_SelectedIndexChanged(object sender, EventArgs e) {
            this.btnDelete.Enabled = (this.ltvList.SelectedIndices.Count > 0);
            this.btnEdit.Enabled = this.btnDelete.Enabled;
            if (this.ltvList.SelectedIndices.Count > 0) {
                this.ListItemSelected?.Invoke(this, new RequestAddItemEventArgs(this.ltvList.SelectedItems[0], false));
            }
        }

        /// <summary>
        /// 項目を昇順に並び替える
        /// </summary>
        public void SortList() {
            //ジェネリックリストに移し替えて昇順ソートする	
            var list = new List<ListViewItem>();
            foreach (ListViewItem item in this.ltvList.Items) {
                list.Add(item);
            }
            list.Sort((x, y) => {
                return string.Compare(x.SubItems[this.SortColumnIndex].Text, y.SubItems[this.SortColumnIndex].Text);
            });

            //ListViewに戻す
            this.ltvList.BeginUpdate();
            this.ltvList.SelectedIndices.Clear();
            this.ltvList.Items.Clear();
            this.ltvList.Items.AddRange(list.ToArray());
            this.ltvList.EndUpdate();
        }

        /// <summary>
        /// 項目を並び替える
        /// </summary>
        private void ltvList_ColumnClick(object sender, ColumnClickEventArgs e) {
            this.SortList();
        }

        /// <summary>
        /// 追加
        /// </summary>
        protected void btnAdd_Click(object sender, EventArgs e) {
            var newItem = new ListViewItem("");
            var eArgs = new RequestAddItemEventArgs(newItem, false);
            this.RequestAddItem?.Invoke(this, eArgs);       //新しい項目を取得し、適用できるか調べる

            if (eArgs.Canceled) {
                //適用できない場合
                return;
            } else if (!this.AllowOverlap) {
                //重複を許さない場合、左端の列テキスト (FixedIDを表示する場合はFixedID) で重複チェック
                foreach (ListViewItem item in this.ltvList.Items) {
                    if (item.Text == newItem.Text) {
                        MessageBox.Show(Resources.MsgE_ForbidOverlap.Replace("$", "項目"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                }
            }

            //追加処理
            this.ltvList.Items.Add(newItem);
            this.ltvList.SelectedIndices.Clear();
            this.RequestClearListEditorSource(this, null);      //ソース入力欄を初期化
            this.RequestWatchControls(this, null);
            this.ListChanged?.Invoke(this, null);
        }

        /// <summary>
        /// すべて追加
        /// </summary>
        private void btnAddAll_Click(object sender, EventArgs e) {
            var newItems = new List<ListViewItem>();
            var eArgs = new RequestAddItemAllEventArgs(newItems);
            this.RequestAddItemAll?.Invoke(this, eArgs);

            if (!this.AllowOverlap) {
                //重複を許さない場合、左端の列テキスト (FixedIDを表示する場合はFixedID) で重複チェック
                foreach (ListViewItem item in this.ltvList.Items) {
                    foreach (var newItem in newItems) {
                        if (item.Text == newItem.Text) {
                            // 既に追加されているものは一括追加の候補リストから除外する
                            newItems.Remove(newItem);
                            break;
                        }
                    }
                }
            }

            //追加処理
            this.ltvList.Items.AddRange(newItems.ToArray());
            this.ltvList.SelectedIndices.Clear();
            this.SortList();
            this.RequestClearListEditorSource(this, null);      //ソース入力欄を初期化
            this.RequestWatchControls(this, null);
            this.ListChanged?.Invoke(this, null);
        }

        /// <summary>
        /// 変更
        /// </summary>
        protected void btnEdit_Click(object sender, EventArgs e) {
            if (this.ltvList.SelectedIndices.Count > 0) {
                var newItem = new ListViewItem("");
                var eArgs = new RequestAddItemEventArgs(newItem, true);
                this.RequestAddItem?.Invoke(this, eArgs);       //新しい項目を取得し、適用できるか調べる

                if (eArgs.Canceled) {
                    return;     //適用できない場合
                } else if (!this.AllowOverlap) {
                    //左端の列テキストで重複チェック
                    foreach (ListViewItem item in this.ltvList.Items) {
                        if (item.Text == newItem.Text && this.ltvList.SelectedItems[0] != item) {
                            MessageBox.Show(Resources.MsgE_ForbidOverlap.Replace("$", "項目"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }
                    }
                }

                //変更処理
                this.ltvList.Items[this.ltvList.SelectedIndices[0]] = newItem;
                this.ltvList.Focus();
                this.ListChanged?.Invoke(this, null);
            }
        }

        /// <summary>
        /// 削除
        /// </summary>
        protected void btnDelete_Click(object sender, EventArgs e) {
            if (this.ltvList.SelectedIndices.Count > 0) {
                this.ltvList.SelectedItems[0].Remove();
                this.RequestWatchControls(this, null);
                this.ListChanged?.Invoke(this, null);
            }
        }

        /// <summary>
        /// 削除コマンド
        /// </summary>
        private void mnuDelete_Click(object sender, EventArgs e) {
            this.btnDelete_Click(sender, e);
        }

        /// <summary>
        /// 右クリックで削除
        /// </summary>
        private void ltvList_MouseClick(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Right) {
                this.btnDelete_Click(sender, e);
            }
        }

        /// <summary>
        /// Ctrl-Up 項目移動
        /// </summary>
        protected void mnuUp_Click(object sender, EventArgs e) {
            if (this.ltvList.SelectedIndices.Count > 0 && 0 < this.ltvList.SelectedIndices[0]) {
                var temp = new KeyValuePair<int, ListViewItem>(this.ltvList.SelectedIndices[0], this.ltvList.SelectedItems[0]);
                this.ltvList.Items.RemoveAt(temp.Key);
                this.ltvList.Items.Insert(temp.Key - 1, temp.Value);
                this.ltvList.SelectedIndices.Add(temp.Key - 1);
                this.ListChanged?.Invoke(this, null);
            }
        }

        /// <summary>
        /// Ctrl-Down 項目移動
        /// </summary>
        protected void mnuDown_Click(object sender, EventArgs e) {
            if (this.ltvList.SelectedIndices.Count > 0 && this.ltvList.SelectedIndices[0] < this.ltvList.Items.Count - 1) {
                var temp = new KeyValuePair<int, ListViewItem>(this.ltvList.SelectedIndices[0], this.ltvList.SelectedItems[0]);
                this.ltvList.Items.RemoveAt(temp.Key);
                this.ltvList.Items.Insert(temp.Key + 1, temp.Value);
                this.ltvList.SelectedIndices.Clear();
                this.ltvList.SelectedIndices.Add(temp.Key + 1);
                this.ListChanged?.Invoke(this, null);
            }
        }

    }
}
