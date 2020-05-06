using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Text.Script {
    using Module;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class dlgEVcmdChangeMap : Form {
        private Point? reservedPosition = null;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEVcmdChangeMap(AllDB DBList, string fileName = "", int x = 0, int y = 0, int dir = (int) Map.Direction4.None) {
            this.InitializeComponent();
            this.uctlMapViewer.DBList = DBList;

            //既定のマップファイルを選択
            this.uctlMapTree.SelectTreeNode(fileName);

            //選択座標を予約
            this.reservedPosition = new Point(x, y);

            //向きリストを生成
            this.cmbDirection.Items.Clear();
            this.cmbDirection.Items.AddRange(Map.GetDirection4List(true));
            this.cmbDirection.SelectedIndex = dir;
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                if (this.chkOnlyPosition.Checked) {
                    //座標のみを出力
                    return $"{this.uctlMapViewer.TilePosition.X}, {this.uctlMapViewer.TilePosition.Y}";
                } else {
                    //マップ移動のコマンドとして出力
                    return Resources.SQ_ChangeMap
                            .Replace("$1", "\"" + this.uctlMapTree.SelectedNodeFullPath.Replace("\\", "\\\\") + "\"")
                            .Replace("$2", this.uctlMapViewer.TilePosition.X.ToString())
                            .Replace("$3", this.uctlMapViewer.TilePosition.Y.ToString())
                            .Replace("$4", SQ.ToSQEnum(typeof(Map.Direction4), this.cmbDirection.SelectedIndex));
                }
            }
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            if (!this.uctlMapTree.IsFileSelected) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "マップ"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            }
        }

        /// <summary>
        /// マップを閉じる
        /// </summary>
        private void uctlMapTree_BeforeSelectNode(object sender, TreeViewCancelEventArgs e) {
            this.uctlMapViewer.Visible = false;
        }

        /// <summary>
        /// マップを読み込む
        /// </summary>
        private void uctlMapTree_AfterSelectNode(object sender, TreeViewEventArgs e) {
            if (!this.uctlMapTree.IsFileSelected) {
                return;
            }

            //読み込み処理
            this.uctlMapViewer.LoadMap(this.uctlMapTree.SelectedNodeFullPath);
            this.uctlMapViewer.Visible = true;

            //選択座標が予約されているときはその座標を選択状態にする
            if (this.reservedPosition != null) {
                this.uctlMapViewer.TilePosition = this.reservedPosition.Value;
                this.reservedPosition = null;
            }
        }

        /// <summary>
        /// 座標のみを有効にするかどうか
        /// </summary>
        private void chkOnlyPosition_CheckedChanged(object sender, EventArgs e) {
            this.cmbDirection.Enabled = !this.chkOnlyPosition.Checked;
        }
    }
}
