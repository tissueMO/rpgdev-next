using DxLibDLL;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using CtrlComponent.Database;
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public class drwDXTilePallet {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public drwDXTilePallet(AllDB DBList) {
            this.DBList = DBList;
            this.InitHandle();
        }

        /// <summary>
        /// 全データベースへの参照
        /// </summary>
        public AllDB DBList {
            get; set;
        }

        /// <summary>
        /// 生成されたDXライブラリ用のパレットグラフィック
        /// </summary>
        public int dxPalletHandle;

        /// <summary>
        /// DXライブラリ用のオートタイルグラフィック
        /// </summary>
        public int[] dxAutoTilesHandle = new int[Map.TileColumnCount * Map.AutoTileRowLength];

        /// <summary>
        /// DXライブラリ用のハンドルをすべて初期化します。
        /// </summary>
        public void InitHandle() {
            this.dxPalletHandle = -1;
            for (var i = 0; i < this.dxAutoTilesHandle.Length; i++) {
                this.dxAutoTilesHandle[i] = -1;
            }
        }

        /// <summary>
        /// タイルセットDBの指定した項目からDXライブラリ用の内部パレットを生成します。
        /// </summary>
        public void LoadTileset(int tilesetFixedID) {
            this.InitHandle();
            DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
            foreach (DataGridViewRow row in this.DBList[Database.DBIndices.Tileset].DBs[0].Rows) {
                if (row.IsNewRow
                || tilesetFixedID != int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value?.ToString())) {
                    //新規行 or 該当箇所でないときはスキップ
                    continue;
                }

                //該当箇所を発見した
                var at = row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.AutoTiles].Value?.ToString().Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                var obj = row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.ObjTiles].Value?.ToString().Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

                //オブジェクトタイルの生成準備
                var sizeList = new Size[obj.Length];
                var objHandles = new int[obj.Length];
                var tilesetHeight = mgrDBTileset.TileSize * Map.AutoTileRowLength;

                //オブジェクトタイルのグラフィックとそのサイズを読み込む
                for (var i = 0; i < obj.Length; i++) {
                    var temp = DX.LoadGraph(ProjectManager.ProjectPath + obj[i]);
                    DX.GetGraphSize(temp, out var x, out var y);
                    sizeList[i] = new Size(x, y);
                    tilesetHeight += y;
                    objHandles[i] = temp;
                }

                this.dxPalletHandle = DX.MakeScreen(mgrDBTileset.TileSize * Map.TileColumnCount, tilesetHeight, DX.TRUE);
                DX.SetDrawScreen(this.dxPalletHandle);     //宛先グラフィックをパレットスクリーンにする
                DX.SetUsePremulAlphaConvertLoad(DX.TRUE);

                //オートタイル部分を読み込み
                for (var x = 0; x < Map.TileColumnCount; x++) {
                    for (var y = 0; y < Map.AutoTileRowLength; y++) {
                        this.dxAutoTilesHandle[x + y * Map.TileColumnCount] = -1;
                        if (x == 0 && y == 0) {
                            //左上タイルは空白にする
                            continue;
                        } else if (x + y * Map.TileColumnCount - 1 < at.Length) {
                            //読み込み
                            this.dxAutoTilesHandle[x + y * Map.TileColumnCount] = DX.LoadGraph(ProjectManager.ProjectPath + at[x + y * Map.TileColumnCount - 1]);

                            //オートタイルを描画
                            DX.DrawRectGraph(
                                x * mgrDBTileset.TileSize, y * mgrDBTileset.TileSize,
                                0 * mgrDBTileset.TileSize, 0 * mgrDBTileset.TileSize,
                                mgrDBTileset.TileSize, mgrDBTileset.TileSize,
                                this.dxAutoTilesHandle[x + y * Map.TileColumnCount],
                                DX.TRUE, DX.FALSE
                            );
                        } else {
                            //空き領域は空白タイルにする
                        }
                    }
                }

                //オブジェクトタイル部分を読み込み
                var amount = mgrDBTileset.TileSize * Map.AutoTileRowLength;
                for (var i = 0; i < obj.Length; i++) {
                    //描画したらすぐに破棄する
                    DX.DrawGraph(0, amount, objHandles[i], DX.TRUE);
                    DX.DeleteGraph(objHandles[i]);
                    amount += sizeList[i].Height;
                }

                //通常の描画先に戻す
                DX.SetDrawScreen(DX.DX_SCREEN_BACK);
                DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
                DX.SetUsePremulAlphaConvertLoad(DX.FALSE);
                return;
            }
        }
    }
}
