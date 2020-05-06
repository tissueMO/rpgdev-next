using System;
using System.Collections.Generic;
using System.Drawing;

namespace Editor.CtrlComponent.Database {
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBTileset {
        public partial class TilesetOneData {

            /// <summary>
            /// オートタイルのデータ
            /// </summary>
            public class AutoTileData : TileGraphicsData {
                public event EventHandler SubEditted;

                public AutoTileData(AllDB DBList, int fixedID, string rPath) : base(DBList, fixedID) {
                    this.SetFile(rPath);
                }

                public TileData Data {
                    get; set;
                }

                public override void SetFile(string rPath) {
                    this.FileName = rPath;

                    //タイルデータを初期化する
                    this.Data = new TileData();
                    this.Data.DataChanged += this.tileEdittedRelay;
                    this.SubEditted?.Invoke(this, null);
                }

                public override void Draw(Graphics g, Point dest) {
                    if (this.GraphImg != null) {
                        g.DrawImage(this.GraphImg, new Rectangle(dest, new Size(mgrDBTileset.TileSize, mgrDBTileset.TileSize)), 0, 0, mgrDBTileset.TileSize, mgrDBTileset.TileSize, GraphicsUnit.Pixel);
                    }
                }

                public override object Clone() {
                    var newData = this.MemberwiseClone() as AutoTileData;
                    newData.Data = this.Data.Clone() as TileData;
                    return newData;
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
