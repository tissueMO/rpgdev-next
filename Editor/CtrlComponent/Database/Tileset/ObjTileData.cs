using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace Editor.CtrlComponent.Database {
    using Module;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBTileset {
        public partial class TilesetOneData {

            /// <summary>
            /// オブジェクトタイルのデータ
            /// </summary>
            public class ObjTileData : TileGraphicsData {
                public event EventHandler SubEditted;
                public TileData[,] Data;

                public ObjTileData(AllDB DBList, int fixedID, string rPath) : base(DBList, fixedID) {
                    this.SetFile(rPath);
                }

                public override void SetFile(string rPath) {
                    this.FileName = rPath;

                    //タイルデータを初期化する
                    if (File.Exists(ProjectManager.ProjectPath + this.FileName)) {
                        var img = Image.FromFile(ProjectManager.ProjectPath + this.FileName);
                        this.Data = new TileData[Map.TileColumnCount, img.Height];
                        for (var x = 0; x < this.Data.GetLength((int) XYPointIndices.X); x++) {
                            for (var y = 0; y < this.Data.GetLength((int) XYPointIndices.Y); y++) {
                                this.Data[x, y] = new TileData();
                                this.Data[x, y].DataChanged += this.tileEdittedRelay;
                            }
                        }
                        img.Dispose();
                    }
                    this.SubEditted?.Invoke(this, null);
                }

                public override void Draw(Graphics g, Point dest) {
                    if (this.GraphImg != null) {
                        g.DrawImage(this.GraphImg, new Rectangle(dest, this.GraphImg.Size));
                    }
                }

                public override object Clone() {
                    var newData = this.MemberwiseClone() as ObjTileData;
                    newData.Data = (TileData[,]) this.Data.Clone();
                    return newData;
                }

                /// <summary>
                /// NOTE: 未設定のイベントハンドラー
                /// タイルグラフィックのサイズが変更されたときのイベントハンドラーです。
                /// 縮小する場合は情報が一部失われますが、拡大する場合は元の部分の情報を維持します。
                /// </summary>
                private void tileResized(object sender, EventArgs e) {
                    var newData = new TileData[this.TileLength.Width, this.TileLength.Height];
                    for (var x = 0; x < newData.GetLength((int) XYPointIndices.X); x++) {
                        for (var y = 0; y < newData.GetLength((int) XYPointIndices.Y); y++) {
                            if (x < this.Data.GetLength((int) XYPointIndices.X) && y < this.Data.GetLength((int) XYPointIndices.Y) && this.Data[x, y] != null) {
                                newData[x, y] = this.Data[x, y];
                            } else {
                                newData[x, y] = new TileData();
                            }
                            newData[x, y].DataChanged += this.tileEdittedRelay;
                        }
                    }
                    this.Data = newData;
                    this.SubEditted?.Invoke(this, null);
                }

                /// <summary>
                /// イベント中継
                /// </summary>
                private void tileEdittedRelay(object sender, EventArgs e) {
                    this.SubEditted?.Invoke(this, e);
                }
            }

        }
    }
}
