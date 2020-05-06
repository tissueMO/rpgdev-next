using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace Editor.CtrlComponent.Database {
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBTileset {
        public partial class TilesetOneData {

            /// <summary>
            /// タイル素材の情報スーパークラス
            /// </summary>
            public abstract class TileGraphicsData : ICloneable {
                public event EventHandler TileEditted;
                protected event EventHandler GraphResized;
                private readonly AllDB dbList;

                /// <summary>
                /// コンストラクター
                /// </summary>
                public TileGraphicsData(AllDB DBlist, int fixedID, string rPath = null) {
                    this.dbList = DBlist;
                    this.FixedID = fixedID;
                    if (!string.IsNullOrEmpty(rPath)) {
                        this.FileName = rPath;
                    }
                }

                /// <summary>
                /// 保存時のデータ順序
                /// </summary>
                public enum DataIndices {
                    FixedID,
                    TileLength,
                    Data,
                }

                public int FixedID {
                    get {
                        return this.fixedID;
                    }
                    set {
                        this.fixedID = value;
                        this.TileEditted?.Invoke(this, null);
                    }
                }
                private int fixedID = -1;

                public string FileName {
                    get {
                        return this.fileName;
                    }
                    protected set {
                        this.fileName = value;
                        //this.ReloadGraph();
                        this.TileEditted?.Invoke(this, null);
                    }
                }
                private string fileName = "";

                public Size TileLength {
                    get;
                    protected set;
                }

                public Size GraphSize {
                    get {
                        return this.graphSize;
                    }
                    private set {
                        this.graphSize = value;

                        //タイル数もセットする
                        this.TileLength = new Size(this.GraphSize.Width / mgrDBTileset.TileSize, this.GraphSize.Height / mgrDBTileset.TileSize);
                    }
                }
                public Size graphSize = Size.Empty;

                public Image GraphImg {
                    get;
                    protected set;
                }

                /// <summary>
                /// グラフィックを再読込します。
                /// </summary>
                public void ReloadGraph() {
                    this.GraphImg?.Dispose();

                    if (!File.Exists(ProjectManager.ProjectPath + this.fileName)) {
                        //グラフィック削除
                        this.GraphImg = null;
                        if (this.GraphSize != Size.Empty) {
                            this.GraphSize = Size.Empty;
                            this.GraphResized?.Invoke(this, null);
                        }
                    } else {
                        //グラフィック読込
                        this.GraphImg = Image.FromFile(ProjectManager.ProjectPath + this.fileName);
                        if (this.GraphSize != this.GraphImg.Size) {
                            this.GraphSize = this.GraphImg.Size;
                            this.GraphResized?.Invoke(this, null);
                        }
                    }
                }

                /// <summary>
                /// グラフィックをセットします。
                /// </summary>
                public abstract void SetFile(string rPath);

                /// <summary>
                /// 指定座標にグラフィックを描画します。
                /// </summary>
                public abstract void Draw(Graphics g, Point dest);

                /// <summary>
                /// このグラフィック領域のデータのコピーを返します。
                /// </summary>
                public abstract object Clone();
            }
        }
    }
}
