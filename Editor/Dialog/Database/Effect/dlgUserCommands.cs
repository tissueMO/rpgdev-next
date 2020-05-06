using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Database.Effect {
    using Properties;

    public partial class dlgUserCommands : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgUserCommands(List<string> fileList) {
            this.InitializeComponent();
            this.uctlPathWithSetButton.RootPath = Resources.Path_Scripts;

            //既存リストを生成する
            var list = new List<ListViewItem>();
            foreach (var file in fileList) {
                list.Add(new ListViewItem(file));
            }
            this.uctlListEditor.SetupList(new string[] { "ファイル名" }, list, false);
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public List<string> Result {
            get {
                var list = new List<string>();
                foreach (var item in this.uctlListEditor.Result) {
                    list.Add(item.Text);
                }
                return list;
            }
        }

        /// <summary>
        /// 選択時に入力欄へ反映
        /// </summary>
        private void uctlListEditor_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            this.uctlPathWithSetButton.Result = e.Item.Text;
        }

        /// <summary>
        /// 項目にセット
        /// </summary>
        private void uctlListEditor_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            e.Item.Text = this.uctlPathWithSetButton.Result;
        }

        /// <summary>
        /// 入力欄をクリア
        /// </summary>
        private void uctlListEditor_RequestClearListEditorSource(object sender, EventArgs e) {
            this.uctlPathWithSetButton.Result = "";
        }

        /// <summary>
        /// コントロール有効無効
        /// </summary>
        private void uctlListEditor_RequestWatchControls(object sender, EventArgs e) {
            this.uctlListEditor.SetCanApply(!string.IsNullOrEmpty(this.uctlPathWithSetButton.Result));
        }
    }
}
