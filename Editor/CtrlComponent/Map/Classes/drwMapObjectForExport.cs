using DxLibDLL;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;

    /// <summary>
    /// DXライブラリの描画対象オブジェクト
    /// </summary>
    public class drwMapObjectForExport : drwMapObject {

        /// <summary>
        /// コンストラクター
        /// </summary>
        public drwMapObjectForExport(Control ctl, mgrMapObject parent, drwDXTilePallet pallet, int tilesetFixedID) : base(ctl, parent, pallet, tilesetFixedID) { }

        public override void Draw() {
            if (this.parent.MapData == null) {
                return;
            }
            this.drawVisibleAllTiles();
        }

        /// <summary>
        /// 全領域のタイルを描画
        /// </summary>
        private void drawVisibleAllTiles() {
            for (var l = (int) Map.Layer.Low; l <= (int) Map.Layer.Shadow; l++) {
                if (l != (int) Map.Layer.Shadow) {
                    // 影層以外: 一発描画
                    DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
                    for (var x = 0; x < this.parent.MapData.MapSize.Width; x++) {
                        for (var y = 0; y < this.parent.MapData.MapSize.Height; y++) {
                            if (this.parent.MapData.LayerTiles[l, x, y].PalletPosition.IsEmpty && this.parent.MapData.BaseMapLayerTiles == null) {
                                //カーボン元がない場合、空白タイルは描画しない
                                continue;
                            }
                            this.drawTile(l, x, y);
                        }
                    }
                } else {
                    //影層：1/4 単位で描画
                    DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA_X4, Map.ShadowTransparent);
                    for (var x = 0; x < this.parent.MapData.MapSize.Width; x++) {
                        for (var y = 0; y < this.parent.MapData.MapSize.Height; y++) {
                            for (var q = 0; q < 4; q++) {
                                if (!this.parent.MapData.LayerTiles[l, x, y].HaveQuarter) {
                                    break;
                                } else if (this.parent.MapData.LayerTiles[l, x, y].GetQuarter((Map.QuarterTile) q) == DX.FALSE ^ (this.parent.MapData.BaseMapLayerTiles == null ? DX.FALSE : this.parent.MapData.BaseMapLayerTiles[l, x, y].GetQuarter((Map.QuarterTile) q)) == DX.TRUE) {
                                    //親マップか、子マップの両方に影が設定されていないか、両方とも影が設定されている場合は打ち消し合って消える
                                    continue;
                                }

                                //影を描画
                                DX.DrawBox(
                                    x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Left,
                                    y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Top,
                                    x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Left,
                                    y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Top,
                                    DX.GetColor(0, 0, 0), DX.TRUE
                                );
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 指定したタイル座標に配置されているタイルを描画します。
        /// </summary>
        protected override void drawTile(int l, int x, int y) {
            var rawTileSize = (int) (this.parent.ScaledTileSize / ViewScale.GetRate(this.parent.Scale));
            var tile = this.parent.MapData.LayerTiles[l, x, y];

            //カーボン元があるマップで、空白タイルの部分はカーボン元の同一座標にあるタイルを描画する
            if (tile.PalletPosition.IsEmpty && this.parent.MapData.BaseMapLayerTiles != null
            && x < this.parent.MapData.BaseMapLayerTiles.GetLength(1)
            && y < this.parent.MapData.BaseMapLayerTiles.GetLength(2)) {
                tile = this.parent.MapData.BaseMapLayerTiles[l, x, y];
            }

            if (tile.PalletPosition.Y >= Map.AutoTileRowLength) {
                //単一タイルを描画
                DX.DrawRectExtendGraph(
                    x * this.parent.ScaledTileSize,
                    y * this.parent.ScaledTileSize,
                    (x + 1) * this.parent.ScaledTileSize,
                    (y + 1) * this.parent.ScaledTileSize,
                    tile.PalletPosition.X * rawTileSize,
                    tile.PalletPosition.Y * rawTileSize,
                    rawTileSize,
                    rawTileSize,
                    this.pallet.dxPalletHandle,
                    DX.TRUE
                );
            } else {
                //オートタイル用の1/4描画
                for (var q = 0; q < 4; q++) {
                    DX.DrawRectExtendGraph(
                        x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2),
                        y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2),
                        x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2),
                        y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2),
                        (q % 2) * (this.parent.ScaledTileSize / 2),
                        (q / 2) * (this.parent.ScaledTileSize / 2) + tile.GetQuarter((Map.QuarterTile) q) * rawTileSize,
                        rawTileSize / 2,
                        rawTileSize / 2,
                        this.pallet.dxAutoTilesHandle[tile.PalletPosition.X + tile.PalletPosition.Y * Map.TileColumnCount],
                        DX.TRUE
                    );
                }
            }
        }

    }
}
