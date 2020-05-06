using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlMapViewer : UserControl {
        public event EventHandler SelectedByDoubleClick;
        private readonly UndoRedoManager UndoRedo = new UndoRedoManager();
        private readonly mgrMapObject mgr;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlMapViewer() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.tolScale.Items.AddRange(ViewScale.GetNames());
            this.pnlPreview.MouseWheel += this.pnlPreview_MouseWheel;
            this.mgr = new mgrMapObject(this.UndoRedo) {
                //ビューア専用の設定
                CurrentLayer = -1,
                VisibleGrid = false
            };
            for (var i = 0; i < (int) Map.Layer.Event; i++) {
                this.mgr.SwitchLayers[i] = true;
            }
        }

        /// <summary>
        /// コンストラクターの後に実行されます。
        /// </summary>
        private void ctlMapViewer_Load(object sender, EventArgs e) {
            this.tolScale.SelectedIndex = (int) ViewScale.ScaleRate.ThreeQuarters;

            //選択したタイル座標が見えるところまでスクロールする
            Map.MapEditor_Resize(this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr);
            Map.MapEditor_ScrollToCursor(this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr);
        }

        /// <summary>
        /// 全データベースへの参照
        /// </summary>
        public AllDB DBList {
            get; set;
        }

        /// <summary>
        /// タイル座標
        /// </summary>
        public Point TilePosition {
            get {
                return this.mgr.Cursor;
            }
            set {
                this.mgr.Cursor = value;
            }
        }

        /// <summary>
        /// 指定したマップファイルを読み込みます。
        /// </summary>
        public void LoadMap(string rPath) {
            this.TilePosition = new Point();
            this.mgr.MapData = mgrMapObject.LoadTileData(rPath);

            if (this.mgr.MapData != null) {
                //カーボン元マップを読み込む
                var baseMapData = mgrMapObject.LoadTileData(this.mgr.MapData.BaseMapFileName);
                if (baseMapData != null) {
                    this.mgr.MapData.BaseMapLayerTiles = baseMapData.LayerTiles;
                }
                this.pnlPreview_VisibleChanged(this, null);
            }
        }

        /// <summary>
        /// スケール変更時
        /// </summary>
        private void tolScale_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.mgr != null) {
                this.mgr.Scale = (ViewScale.ScaleRate) this.tolScale.SelectedIndex;
            }
        }

        /// <summary>
        /// 画面上に描画領域が出てきたら描画準備を行う
        /// </summary>
        private void pnlPreview_VisibleChanged(object sender, EventArgs e) {
            if (this.DesignMode == false && this.pnlPreview.Visible) {
                Media.ChangeDest(new drwMapObject(this.pnlPreview, this.mgr, new drwDXTilePallet(this.DBList), this.mgr.MapData?.TilesetFixedID ?? -1));
                Map.MapEditor_Resize(this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr);
            }
        }

        /// <summary>
        /// デザイナー：親フォームが中継したキー入力イベントを受け取る
        /// </summary>
        public void pnlPreview_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            if (this.pnlPreview.Focused) {
                this.mgr.MoveCursorByKeyDown(e.KeyData);
            }
        }

        /// <summary>
        /// マウスが押されたとき
        /// </summary>
        private void pnlPreview_MouseDown(object sender, MouseEventArgs e) {
            this.mgr.Cursor = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
        }

        /// <summary>
        /// デザイナー：マウスホイール
        /// </summary>
        private void pnlPreview_MouseWheel(object sender, MouseEventArgs e) {
            Map.MapEditor_MouseWheel(e, this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr, this.tolScale);
        }

        /// <summary>
        /// スクロールしたら描画領域を更新する
        /// </summary>
        private void designerScroller_Scroll(object sender, ScrollEventArgs e) {
            Map.MapEditor_Resize(this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr);
        }

        /// <summary>
        /// コントロールにマウスが入ったらアクティブにする
        /// </summary>
        private void pnlPreview_MouseEnter(object sender, EventArgs e) {
            this.pnlPreview.Focus();
        }

        /// <summary>
        /// ダブルクリックされたら確定する
        /// </summary>
        private void pnlPreview_DoubleClick(object sender, EventArgs e) {
            this.SelectedByDoubleClick?.Invoke(this, null);
        }
    }
}
