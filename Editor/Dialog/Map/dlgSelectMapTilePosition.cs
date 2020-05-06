using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Map {
    using Module;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class dlgSelectMapTilePosition : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgSelectMapTilePosition(string rPath, AllDB DBList) {
            this.InitializeComponent();

            //マップビューアをセットアップ
            this.uctlViewer.DBList = DBList;
            this.uctlViewer.LoadMap(rPath);
            Media.StopDraw = false;
        }

        /// <summary>
        /// タイル座標
        /// </summary>
        public Point TilePosition {
            get {
                return this.uctlViewer.TilePosition;
            }
            set {
                this.uctlViewer.TilePosition = value;
            }
        }

        /// <summary>
        /// マップビューアにキー入力情報を伝える
        /// </summary>
        private void dlgSelectMapTilePosition_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            this.uctlViewer.pnlPreview_PreviewKeyDown(sender, new PreviewKeyDownEventArgs(e.KeyData));
        }

        /// <summary>
        /// タイルビューアがダブルクリックされたときのイベントに対応する
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            this.DialogResult = DialogResult.OK;
        }
    }
}
