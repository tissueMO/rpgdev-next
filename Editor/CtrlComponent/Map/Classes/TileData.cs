using DxLibDLL;
using System;
using System.Drawing;

namespace Editor.CtrlComponent.Map {
    using Module;

    public partial class mgrMapObject {
        public partial class MapOneData {

            /// <summary>
            /// １タイルあたりのデータ
            /// </summary>
            public struct TileData {
                public static event EventHandler Editted;
                private int[] Quarter;

                /// <summary>
                /// すべての 1/4タイル に代入
                /// </summary>
                public void SetQuarters(int lt, int rt, int lb, int rb) {
                    this.SetQuarter(Map.QuarterTile.LeftTop, lt);
                    this.SetQuarter(Map.QuarterTile.RightTop, rt);
                    this.SetQuarter(Map.QuarterTile.LeftBottom, lb);
                    this.SetQuarter(Map.QuarterTile.RightBottom, rb);
                }

                /// <summary>
                /// すべての 1/4 タイルに代入
                /// </summary>
                public void SetQuarters(int[] value) {
                    if (value == null || value.Length != (int) Map.QuarterTile.Count) {
                        return;
                    }

                    for (var i = 0; i < this.Quarter.Length; i++) {
                        this.Quarter[i] = value[i];
                    }

                    TileData.Editted?.Invoke(this, null);
                }

                /// <summary>
                /// 指定した 1/4タイル に代入
                /// </summary>
                public void SetQuarter(Map.QuarterTile pos, int value) {
                    var values = new int[(int) Map.QuarterTile.Count];
                    if (!this.HaveQuarter) {
                        this.Quarter = new int[(int) Map.QuarterTile.Count];
                    }

                    //配列は参照型であるため、以前の情報と関連付けさせないようにコピーを作って丸ごと置き換える
                    // 元配列 -> コピー -> 新配列 -> 変更 -> 元配列
                    Array.Copy(this.Quarter, values, values.Length);
                    values[(int) pos] = value;
                    this.Quarter = values;

                    TileData.Editted?.Invoke(this, null);
                }

                /// <summary>
                /// パレットに対応するタイル位置
                /// </summary>
                public Point PalletPosition {
                    get {
                        return this.palletPosition;
                    }
                    set {
                        this.palletPosition = value;
                        TileData.Editted?.Invoke(this, null);
                    }
                }
                private Point palletPosition;

                /// <summary>
                /// 1/4 単位の情報を持っているかどうか
                /// </summary>
                public bool HaveQuarter {
                    get {
                        return this.Quarter != null;
                    }
                }

                /// <summary>
                /// 1/4 単位の情報を取得します。
                /// </summary>
                public int GetQuarter(Map.QuarterTile pos) {
                    if (!this.HaveQuarter) {
                        return DX.FALSE;
                    } else {
                        return this.Quarter[(int) pos];
                    }
                }

                /// <summary>
                /// すべての 1/4タイル情報 をコピーして返します。
                /// </summary>
                public int[] GetQuarters() {
                    if (!this.HaveQuarter) {
                        return null;
                    } else {
                        return (int[]) this.Quarter.Clone();
                    }
                }
            }
        }
    }
}
