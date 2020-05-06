using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlSQChangeMap : UserControl {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlSQChangeMap() {
            this.InitializeComponent();
            this.uctlFileTree.RootPath = Resources.Path_Maps;
            this.uctlPosition.Maximum = new Point(Map.MaxMapSize);
            this.cmbDirection.Items.Clear();
            this.cmbDirection.Items.AddRange(Map.GetDirection4List(true));
            this.cmbDirection.SelectedIndex = (int) Map.Direction4.None;
        }

        /// <summary>
        /// 全データベースへの参照
        /// </summary>
        public AllDB DBList {
            get; set;
        }

        public string ResultMapFileName {
            get {
                return "\"" + this.uctlFileTree.SelectedNodeFullPath.Replace("\\", "\\\\") + "\"";
            }
        }

        public Point ResultPosition {
            get {
                return this.uctlPosition.Result;
            }
        }

        public string ResultDirection {
            get {
                return SQ.ToSQEnum(typeof(Map.Direction4), this.cmbDirection.SelectedIndex);
            }
        }

        /// <summary>
        /// 移動後の向きを設定するかどうか
        /// </summary>
        public bool EnabledDirection {
            get {
                return this.enabledDirection;
            }
            set {
                this.enabledDirection = value;
                this.lblDirection.Visible = value;
                this.cmbDirection.Visible = value;
            }
        }

        private bool enabledDirection = true;

        /// <summary>
        /// ビューアから座標をセット
        /// </summary>
        private void btnMapPosSetter_Click(object sender, EventArgs e) {
            if (this.uctlFileTree.IsFileSelected) {
                var Dlg = new Dialog.Map.dlgSelectMapTilePosition(this.uctlFileTree.SelectedNodeFullPath, this.DBList) {
                    TilePosition = this.uctlPosition.Result
                };
                if (Dlg.ShowDialog(this) == DialogResult.OK) {
                    this.uctlPosition.Result = Dlg.TilePosition;
                }
            } else {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "マップファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 入力チェックを行い、不備がある場合はエラーメッセージを表示してtrueを返す
        /// </summary>
        /// <returns>不備があるかどうか</returns>
        public bool CheckError() {
            if (!this.uctlFileTree.IsFileSelected) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "マップファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return true;
            } else {
                return false;
            }
        }
    }
}
