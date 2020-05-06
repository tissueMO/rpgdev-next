using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Database.Effect {
    using Module;
    using Properties;

    public partial class dlgEditLayers : Form {

        /// <summary>
        /// リスト列の表示順序
        /// </summary>
        public enum ListColumnIndices {
            FixedID,
            FileName,
            SectionSize,
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEditLayers(string layerList = "") {
            this.InitializeComponent();
            this.uctlPathWithSet.FileFilter = Resources.Extension_Graphics;
            this.uctlPathWithSet.RootPath = Resources.Path_Materials;

            //既存リストを生成する
            var list = new List<ListViewItem>();
            if (!string.IsNullOrEmpty(layerList)) {
                var spl = layerList.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                if (spl != null) {
                    foreach (var layer in spl) {
                        var subspl = layer.Split(Resources.Split_IDNamePair.ToCharArray());
                        var item = new ListViewItem {
                            Text = subspl[(int) ListColumnIndices.FixedID]
                        };
                        item.SubItems.Add(subspl[(int) ListColumnIndices.FileName]);
                        item.SubItems.Add(subspl[(int) ListColumnIndices.SectionSize]);
                        list.Add(item);
                    }
                }
            }

            //FixedID列を含める
            this.uctlLayerListEditor.SetupList(new string[] { "ファイル名", "区画サイズ" }, list, true);
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                var buf = "";
                foreach (var item in this.uctlLayerListEditor.Result) {
                    buf += item.SubItems[(int) ListColumnIndices.FixedID].Text;
                    buf += Resources.Split_IDNamePair;
                    buf += item.SubItems[(int) ListColumnIndices.FileName].Text;
                    buf += Resources.Split_IDNamePair;
                    buf += item.SubItems[(int) ListColumnIndices.SectionSize].Text;
                    buf += Resources.Split_LongData;
                }
                buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
                return buf;
            }
        }

        /// <summary>
        /// 一意のIDを生成します。
        /// </summary>
        private string generateFixedID() {
            int id;
            var rand = new Random();
            var NG = false;

            do {
                id = rand.Next(0, int.MaxValue);
                NG = false;
                foreach (var item in this.uctlLayerListEditor.Result) {
                    if (item.Text == id.ToString()) {
                        NG = true;
                        break;
                    }
                }
            } while (NG);

            return id.ToString();
        }

        /// <summary>
        /// 選択時に入力欄へ反映
        /// </summary>
        private void uctlLayerListEditor_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            this.txtFixedID.Text = e.Item.SubItems[(int) ListColumnIndices.FixedID].Text;
            this.uctlPathWithSet.Result = e.Item.SubItems[(int) ListColumnIndices.FileName].Text;
            this.uctlSectionSize.Result = new Point(Common.StringToSize(e.Item.SubItems[(int) ListColumnIndices.SectionSize].Text));
        }

        /// <summary>
        /// 追加と変更
        /// </summary>
        private void uctlLayerListEditor_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (!e.Editted) {
                //新規作成する場合はFixedIDを生成する
                e.Item.Text = this.generateFixedID();
            } else {
                //変更する場合は
                e.Item.Text = this.txtFixedID.Text;
            }

            e.Item.SubItems.Add(this.uctlPathWithSet.Result);
            e.Item.SubItems.Add(Common.SizeToString(new Size(this.uctlSectionSize.Result)));
        }

        /// <summary>
        /// 入力欄をクリア
        /// </summary>
        private void uctlLayerListEditor_RequestClearListEditorSource(object sender, EventArgs e) {
            this.uctlPathWithSet.Result = "";
            this.txtFixedID.Text = "";
        }

        /// <summary>
        /// 追加と変更のコントロール有効/無効
        /// </summary>
        private void uctlLayerListEditor_RequestWatchControls(object sender, EventArgs e) {
            this.uctlLayerListEditor.SetCanApply(!string.IsNullOrEmpty(this.uctlPathWithSet.Result));
        }
    }
}
