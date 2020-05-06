using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Map {
    using Module;
    using Properties;

    public partial class dlgMapGenerator : Form {
        private Size mapSize;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgMapGenerator(Size mapSize, int defaultLayerIndex) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //初期設定
            this.mapSize = mapSize;
            this.uctlTileFloor.FileName = Settings.Default.Last_MapRandomFloorTile;
            this.uctlTileWall.FileName = Settings.Default.Last_MapRandomWallTile;
            this.uctlTileCeil.FileName = Settings.Default.Last_MapRandomCeilTile;

            //レイヤーリスト生成
            this.cmbDestLayer.Items.Clear();
            for (var l = 0; l < (int) Map.Layer.Shadow; l++) {
                this.cmbDestLayer.Items.Add(Map.GetLayerList()[l]);
            }
            if (defaultLayerIndex < (int) Map.Layer.Shadow) {
                this.cmbDestLayer.SelectedIndex = defaultLayerIndex;
            } else {
                this.cmbDestLayer.SelectedIndex = 0;
            }
        }

        /// <summary>
        /// 描画先のレイヤー
        /// </summary>
        public int DestLayerIndex {
            get {
                return this.cmbDestLayer.SelectedIndex;
            }
        }

        /// <summary>
        /// 生成されたタイルの種別に対応するタイルグラフィックのパレット上の位置を示したリスト
        /// </summary>
        public Point[] TilePalletPositions {
            get {
                var list = new List<Point>();

                for (var i = 0; i < Common.GetEnumCount<DungeonGenerator.GeneratedMapTile>(); i++) {
                    switch ((DungeonGenerator.GeneratedMapTile) i) {
                        case DungeonGenerator.GeneratedMapTile.Floor:
                            list.Add(Common.StringToPoint(this.uctlTileFloor.FileName));
                            break;
                        case DungeonGenerator.GeneratedMapTile.Wall:
                            list.Add(Common.StringToPoint(this.uctlTileWall.FileName));
                            break;
                        case DungeonGenerator.GeneratedMapTile.Ceil:
                            list.Add(Common.StringToPoint(this.uctlTileCeil.FileName));
                            break;
                    }
                }

                return list.ToArray();
            }
        }

        /// <summary>
        /// 生成されたマップデータ
        /// </summary>
        public DungeonGenerator.DungeonData GeneratedMapData {
            get {
                return this.generatedMapData;
            }
            private set {
                this.generatedMapData = value;
            }
        }
        private DungeonGenerator.DungeonData generatedMapData = null;

        /// <summary>
        /// ランダム生成実行
        /// </summary>
        private void btnDoGenerate_Click(object sender, EventArgs e) {
            this.GeneratedMapData = DungeonGenerator.DoGenerateDungeon(this.mapSize, (int) this.numComplex.Value);
            if (this.GeneratedMapData != null) {
                this.lblPreview.Text = this.GeneratedMapData.TileDataToString();
            }
        }

        /// <summary>
        /// 確定検証
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            if (this.generatedMapData == null) {
                MessageBox.Show("「ランダム生成実行」を行って下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.DialogResult = DialogResult.None;
                return;
            }

            //設定保存
            Settings.Default.Last_MapRandomCeilTile = this.uctlTileCeil.FileName;
            Settings.Default.Last_MapRandomFloorTile = this.uctlTileFloor.FileName;
            Settings.Default.Last_MapRandomWallTile = this.uctlTileWall.FileName;
        }
    }
}
