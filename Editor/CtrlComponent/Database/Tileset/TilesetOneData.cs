using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBTileset {

        /// <summary>
        /// タイルセット単体情報
        /// </summary>
        public partial class TilesetOneData : ctlDBGUIBase.DBGUIOneDatabase {
            private readonly AllDB dbList;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public TilesetOneData(int fixedID, int visibleID, string name, string comment, AllDB DBList) : base(fixedID, visibleID, name) {
                this.Comment = comment;
                this.dbList = DBList;
                this.AutoTiles = new List<AutoTileData>();
                this.ObjTiles = new List<ObjTileData>();
            }

            public List<AutoTileData> AutoTiles {
                get {
                    return this.autoTiles;
                }
                set {
                    this.autoTiles = value;
                    this.IsDirty = true;
                }
            }
            private List<AutoTileData> autoTiles;

            public List<ObjTileData> ObjTiles {
                get {
                    return this.objTiles;
                }
                set {
                    this.objTiles = value;
                    this.IsDirty = true;
                }
            }
            private List<ObjTileData> objTiles;

            public Bitmap Canvas {
                get;
                private set;
            }

            public string FileName {
                get {
                    return Resources.Path_DBs_Tilesets + this.FixedID + Resources.Extension_Tileset;
                }
            }

            public string Comment {
                get {
                    return this.comment;
                }
                set {
                    this.comment = value;
                    this.IsDirty = true;
                }
            }
            private string comment = "";

            public override bool SaveFile(string rPath = null) {
                if (!this.IsDirty) {
                    //変更がない場合は処理しない
                    return false;
                }

                string fileName;
                if (string.IsNullOrEmpty(rPath)) {
                    fileName = this.FileName;
                } else {
                    fileName = rPath;
                }

                using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                    //オートタイル
                    for (var i = 0; i < Map.AutoTileRowLength * Map.TileColumnCount - 1; i++) {
                        //データが存在する場合のみ書き出す
                        if (i < this.AutoTiles.Count) {
                            //オートタイルは１行でタイル情報を出力
                            W.Write(this.AutoTiles[i].FixedID);
                            W.Write(Resources.Split_LongData);
                            W.Write(Common.SizeToString(this.AutoTiles[i].TileLength));
                            W.Write(Resources.Split_LongData);
                            W.Write(this.AutoTiles[i].Data.GetDataString());
                        }
                        W.WriteLine();
                    }

                    //オブジェクトタイル
                    foreach (var graph in this.ObjTiles) {
                        //オブジェクトタイルは行/列でタイル情報を出力、１行目にメタ情報
                        W.Write(graph.FixedID);
                        W.Write(Resources.Split_LongData);
                        W.Write(Common.SizeToString(graph.TileLength));
                        W.WriteLine();

                        for (var y = 0; y < graph.TileLength.Height; y++) {
                            for (var x = 0; x < graph.TileLength.Width; x++) {
                                W.Write(graph.Data[x, y].GetDataString());

                                //タイルデータ区切り記号を付加
                                if (x < graph.TileLength.Width - 1) {
                                    W.Write(Resources.Split_Argument);
                                }
                            }
                            W.WriteLine();
                        }
                    }
                }

                //変更履歴に追加
                this.IsDirty = false;
                return true;
            }

            public override void LoadFile() {
                var fileName = Resources.Path_DBs_Tilesets + this.FixedID + Resources.Extension_Tileset;
                if (!File.Exists(ProjectManager.ProjectPath + fileName)) {
                    //ファイルが見つからない場合は初期状態のままにする
                    return;
                }

                var buf = "";
                using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                    //オートタイル
                    for (var i = 0; i < Map.AutoTileRowLength * Map.TileColumnCount - 1; i++) {
                        buf = R.ReadLine();

                        //データが存在する場合のみ取り出す
                        if (!string.IsNullOrEmpty(buf)) {
                            var spl = buf.Split(Resources.Split_LongData.ToCharArray());
                            this.AutoTiles[i].FixedID = int.Parse(spl[(int) TileGraphicsData.DataIndices.FixedID]);
                            this.AutoTiles[i].Data.SetData(spl[(int) TileGraphicsData.DataIndices.Data]);
                        }
                    }

                    //オブジェクトタイル
                    foreach (var graph in this.ObjTiles) {
                        graph.ReloadGraph();
                        buf = R.ReadLine();
                        var spl = buf.Split(Resources.Split_LongData.ToCharArray());
                        graph.FixedID = int.Parse(spl[(int) TileGraphicsData.DataIndices.FixedID]);
                        var tileLen = Common.StringToSize(spl[(int) TileGraphicsData.DataIndices.TileLength]);

                        //あくまでも保存時のサイズで処理する
                        for (var y = 0; y < tileLen.Height; y++) {
                            //行単位で処理
                            buf = R.ReadLine();
                            if (y >= graph.TileLength.Height) {
                                //実際のサイズが縮小している場合はその先を処理しない
                                continue;
                            }

                            var cols = buf.Split(Resources.Split_Argument.ToCharArray());
                            for (var x = 0; x < tileLen.Width; x++) {
                                if (x >= graph.TileLength.Width) {
                                    //実際のサイズが縮小している場合はその先を処理しない
                                    continue;
                                }
                                graph.Data[x, y].SetData(cols[x]);
                            }
                        }
                    }
                }

                this.IsDirty = false;
            }

            /// <summary>
            /// 一意のIDを生成します。
            /// これはオートタイル、オブジェクトタイル共通で一意です。
            /// </summary>
            private int generateFixedID() {
                int id;
                var NG = false;
                var rand = new Random();

                do {
                    id = rand.Next(0, int.MaxValue);
                    NG = false;
                    foreach (var item in this.AutoTiles) {
                        if (item.FixedID == id) {
                            NG = true;
                            break;
                        }
                    }
                    if (NG) {
                        continue;
                    }
                    foreach (var item in this.ObjTiles) {
                        if (item.FixedID == id) {
                            NG = true;
                            break;
                        }
                    }
                } while (NG);

                return id;
            }

            /// <summary>
            /// オートタイルを追加します。上限数に達している場合は追加できません。
            /// </summary>
            public void AddAutoTile(string rPath) {
                if (!Common.CheckFileExtensionFilter(rPath, Resources.Extension_Graphics)) {
                    //画像ファイル以外はNG
                    return;
                } else if (!this.CheckAutoTileSize(rPath, true)) {
                    //規格に合わなければNG
                    return;
                } else if (this.AutoTiles.Count >= Map.TileColumnCount * Map.AutoTileRowLength - 1) {
                    //上限数に達したらNG
                    MessageBox.Show("登録できるオートタイルの上限数に達しています。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                this.AutoTiles.Add(new AutoTileData(this.dbList, this.generateFixedID(), rPath));
                this.AutoTiles[this.AutoTiles.Count - 1].SubEditted += this.tilesetEdittedRelay;
                this.AutoTiles[this.AutoTiles.Count - 1].TileEditted += this.tilesetEdittedRelay;
                this.IsDirty = true;
            }

            /// <summary>
            /// フォルダーからオートタイルを一括追加します。不正な形式や上限数に達するとスキップします。
            /// </summary>
            public void AddAutoTileInFolder(string rPath) {
                rPath = Common.AddToPathSplit(rPath);
                var files = Directory.GetFiles(ProjectManager.ProjectPath + rPath, "*.*", SearchOption.AllDirectories);
                var exceptionRule = false;

                foreach (var file in files) {
                    if (!Common.CheckFileExtensionFilter(file, Resources.Extension_Graphics)) {
                        //画像ファイル以外はスキップ
                        continue;
                    } else if (!this.CheckAutoTileSize(file.Substring(ProjectManager.ProjectPath.Length), false)) {
                        //規格に合わなければスキップ
                        exceptionRule = true;
                        continue;
                    } else if (this.AutoTiles.Count >= Map.TileColumnCount * Map.AutoTileRowLength - 1) {
                        //上限数に達したら中断
                        MessageBox.Show("登録できるオートタイルの上限数に達したため、処理が中断されました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        break;
                    }

                    this.AutoTiles.Add(new AutoTileData(this.dbList, this.generateFixedID(), file.Substring(ProjectManager.ProjectPath.Length)));
                    this.AutoTiles[this.AutoTiles.Count - 1].SubEditted += this.tilesetEdittedRelay;
                    this.AutoTiles[this.AutoTiles.Count - 1].TileEditted += this.tilesetEdittedRelay;
                }

                if (exceptionRule) {
                    MessageBox.Show("オートタイルの規格に合わない素材が存在したため、一部のファイルは登録されていません。\r\nシステムデータベースから１タイルあたりのサイズを確認して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }

                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのオートタイルを変更します。
            /// </summary>
            public void ReplaceAutoTile(int index, string rPath) {
                if (!this.CheckAutoTileSize(rPath, true)) {
                    //素材形式が不正な場合は却下
                    return;
                }
                this.AutoTiles[index].SetFile(rPath);
                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのオートタイルを削除します。
            /// </summary>
            public void DeleteAutoTile(int index) {
                this.AutoTiles.RemoveAt(index);
                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのオートタイルを一つ上に移動させます。
            /// </summary>
            public bool ToUpAutoTile(int index) {
                if (0 < index) {
                    var temp = new KeyValuePair<int, AutoTileData>(index, this.AutoTiles[index]);
                    this.AutoTiles.RemoveAt(index);
                    this.AutoTiles.Insert(temp.Key - 1, temp.Value);
                    this.IsDirty = true;
                    return true;
                }
                return false;
            }

            /// <summary>
            /// 指定したインデックスのオートタイルを一つ下に移動させます。
            /// </summary>
            public bool ToDownAutoTile(int index) {
                if (index < this.AutoTiles.Count - 1) {
                    var temp = new KeyValuePair<int, AutoTileData>(index, this.AutoTiles[index]);
                    this.AutoTiles.RemoveAt(index);
                    this.AutoTiles.Insert(temp.Key + 1, temp.Value);
                    this.IsDirty = true;
                    return true;
                }
                return false;
            }

            /// <summary>
            /// オートタイル素材規格に合っているかどうかを調べます。
            /// </summary>
            /// <returns>問題ない場合はtrue</returns>
            public bool CheckAutoTileSize(string rPath, bool alert) {
                var result = true;
                if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                    if (alert) {
                        MessageBox.Show("存在しないファイルです。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    return false;
                }

                var img = Image.FromFile(ProjectManager.ProjectPath + rPath);
                if (img.Size.Width % mgrDBTileset.TileSize != 0) {
                    if (alert) {
                        MessageBox.Show($"オートタイル素材の横幅はタイルサイズ [{mgrDBTileset.TileSize}] の倍数である必要があります。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    result = false;
                } else if (img.Size.Height != mgrDBTileset.TileSize * Common.GetEnumCount<Map.AutoTilePattern>()) {
                    if (alert) {
                        MessageBox.Show($"オートタイル素材の縦幅は {Common.GetEnumCount<Map.AutoTilePattern>()} タイル分のサイズ [{mgrDBTileset.TileSize * Common.GetEnumCount<Map.AutoTilePattern>()}] である必要があります。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    result = false;
                }

                img.Dispose();
                return result;
            }

            /// <summary>
            /// オートタイルのリストのコピーを返します。
            /// </summary>
            public List<AutoTileData> CopyAutoTiles() {
                var newList = new List<AutoTileData>();
                foreach (var item in this.AutoTiles) {
                    newList.Add(item.Clone() as AutoTileData);
                }
                return newList;
            }

            /// <summary>
            /// オブジェクトタイルを追加します。
            /// </summary>
            public void AddObjTile(string rPath) {
                this.ObjTiles.Add(new ObjTileData(this.dbList, this.generateFixedID(), rPath));
                this.ObjTiles[this.ObjTiles.Count - 1].SubEditted += this.tilesetEdittedRelay;
                this.ObjTiles[this.ObjTiles.Count - 1].TileEditted += this.tilesetEdittedRelay;
                this.IsDirty = true;
            }

            /// <summary>
            /// フォルダーからオブジェクトタイルを追加します。不正な形式はスキップします。
            /// </summary>
            public void AddObjTileInFolder(string rPath) {
                rPath = Common.AddToPathSplit(rPath);
                var files = Directory.GetFiles(ProjectManager.ProjectPath + rPath, "*.*");

                foreach (var file in files) {
                    if (!Common.CheckFileExtensionFilter(file, Resources.Extension_Graphics)) {
                        continue;       //画像ファイル以外はスキップ
                    } else if (!this.CheckObjTileSize(file.Substring(ProjectManager.ProjectPath.Length), false)) {
                        continue;       //規格に合わなければスキップ
                    }

                    this.ObjTiles.Add(new ObjTileData(this.dbList, this.generateFixedID(), file.Substring(ProjectManager.ProjectPath.Length)));
                    this.ObjTiles[this.ObjTiles.Count - 1].SubEditted += this.tilesetEdittedRelay;
                    this.ObjTiles[this.ObjTiles.Count - 1].TileEditted += this.tilesetEdittedRelay;
                }

                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのオブジェクトタイルを変更します。
            /// </summary>
            public void ReplaceObjTile(int index, string rPath) {
                if (!this.CheckObjTileSize(rPath, true)) {
                    //素材形式が不正な場合は却下
                    return;
                }
                this.ObjTiles[index].SetFile(rPath);
                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのオブジェクトタイルを削除します。
            /// </summary>
            public void DeleteObjTile(int index) {
                this.ObjTiles.RemoveAt(index);
                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのオブジェクトタイルを一つ上に移動させます。
            /// </summary>
            public bool ToUpObjTile(int index) {
                if (0 < index) {
                    var temp = new KeyValuePair<int, ObjTileData>(index, this.ObjTiles[index]);
                    this.ObjTiles.RemoveAt(index);
                    this.ObjTiles.Insert(temp.Key - 1, temp.Value);
                    this.IsDirty = true;
                    return true;
                }
                return false;
            }

            /// <summary>
            /// 指定したインデックスのオブジェクトタイルを一つ下に移動させます。
            /// </summary>
            public bool ToDownObjTile(int index) {
                if (index < this.ObjTiles.Count - 1) {
                    var temp = new KeyValuePair<int, ObjTileData>(index, this.ObjTiles[index]);
                    this.ObjTiles.RemoveAt(index);
                    this.ObjTiles.Insert(temp.Key + 1, temp.Value);
                    this.IsDirty = true;
                    return true;
                }
                return false;
            }

            /// <summary>
            /// オブジェクトタイル素材規格に合っているかどうかを調べます。
            /// </summary>
            /// <returns>問題ない場合はtrue</returns>
            public bool CheckObjTileSize(string rPath, bool alert) {
                var result = true;
                if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                    if (alert) {
                        MessageBox.Show("存在しないファイルです。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                    return false;
                }

                var img = Image.FromFile(ProjectManager.ProjectPath + rPath);
                if (img.Size.Width != mgrDBTileset.TileSize * Map.TileColumnCount) {
                    if (alert) {
                        MessageBox.Show($"オブジェクトタイル素材の横幅は {Map.TileColumnCount} タイル分のサイズ [{mgrDBTileset.TileSize * Map.TileColumnCount}] である必要があります。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    result = false;
                } else if (img.Size.Height % mgrDBTileset.TileSize != 0) {
                    if (alert) {
                        MessageBox.Show($"オブジェクトタイル素材の縦幅はタイルサイズ [{mgrDBTileset.TileSize}] の倍数である必要があります。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    result = false;
                }

                img.Dispose();
                return result;
            }

            /// <summary>
            /// オブジェクトタイルのリストのコピーを返します。
            /// </summary>
            public List<ObjTileData> CopyObjTiles() {
                var newList = new List<ObjTileData>();
                foreach (var item in this.ObjTiles) {
                    newList.Add(item.Clone() as ObjTileData);
                }
                return newList;
            }

            /// <summary>
            /// このタイルセットの描画領域を再生成します。
            /// </summary>
            public void ResetCanvas() {
                var canvasSize = new Size {
                    Width = mgrDBTileset.TileSize * Map.TileColumnCount,
                    Height = mgrDBTileset.TileSize * Map.AutoTileRowLength
                };

                foreach (var at in this.AutoTiles) {
                    at.ReloadGraph();
                }
                foreach (var obj in this.ObjTiles) {
                    obj.ReloadGraph();
                    canvasSize.Height += (obj.GraphImg?.Height ?? 0);
                }

                this.Canvas = new Bitmap(canvasSize.Width, canvasSize.Height);
            }

            /// <summary>
            /// タイルセットのピクセル座標から、該当タイル情報へのインスタンスを返します。
            /// </summary>
            public TileData GetTileFromPixel(Point pixPos) {
                var tilePos = Point.Empty;

                if (pixPos.Y <= mgrDBTileset.TileSize * Map.AutoTileRowLength) {
                    //オートタイル
                    tilePos = new Point(pixPos.X / mgrDBTileset.TileSize, pixPos.Y / mgrDBTileset.TileSize);

                    //空白タイルは設定不可
                    if (tilePos.IsEmpty == false && tilePos.X + tilePos.Y * Map.TileColumnCount - 1 < this.AutoTiles.Count) {
                        return this.AutoTiles[tilePos.X + tilePos.Y * Map.TileColumnCount - 1].Data;
                    }
                } else {
                    //オブジェクトタイル
                    var height = mgrDBTileset.TileSize * Map.AutoTileRowLength;
                    foreach (var obj in this.ObjTiles) {
                        height += obj.GraphImg.Height;

                        //該当グラフィックかどうかを判定
                        if (pixPos.Y <= height) {
                            //該当グラフィック上のタイル座標に換算
                            tilePos.X = pixPos.X / mgrDBTileset.TileSize;
                            tilePos.Y = (pixPos.Y - (height - obj.GraphImg.Height)) / mgrDBTileset.TileSize;
                            return obj.Data[tilePos.X, tilePos.Y];
                        }
                    }
                }
                return null;
            }

            /// <summary>
            /// タイルセットのタイル座標から、該当タイル情報へのインスタンスを返します。
            /// </summary>
            public TileData GetTileFromTilePos(Point tilePos) {
                if (tilePos.Y < Map.AutoTileRowLength) {
                    //オートタイル
                    //空白タイルは設定不可
                    if (tilePos.IsEmpty == false && tilePos.X + tilePos.Y * Map.TileColumnCount - 1 < this.AutoTiles.Count) {
                        return this.AutoTiles[tilePos.X + tilePos.Y * Map.TileColumnCount - 1].Data;
                    }
                } else {
                    //オブジェクトタイル
                    var height = mgrDBTileset.TileSize * Map.AutoTileRowLength;
                    foreach (var obj in this.ObjTiles) {
                        height += obj.GraphImg.Height;

                        //該当グラフィックかどうかを判定
                        if (tilePos.Y * mgrDBTileset.TileSize < height) {
                            //該当グラフィック上のタイル座標に換算
                            return obj.Data[tilePos.X, tilePos.Y - (height - obj.GraphImg.Height) / mgrDBTileset.TileSize];
                        }
                    }
                }
                return null;
            }

            /// <summary>
            /// このタイルセットを描画します。
            /// </summary>
            public void DrawTileset(Graphics g, Map.TileDataType? editMode) {
                Common.ApplyHighQualityDrawing(g);

                //罫線の色
                var linePen = Pens.LightGray;

                //オートタイル部
                for (var x = 0; x < Map.TileColumnCount; x++) {
                    for (var y = 0; y < Map.AutoTileRowLength; y++) {
                        if (x == 0 && y == 0) {
                            //左上タイルは空白にする
                            continue;
                        } else if (x + y * Map.TileColumnCount - 1 < this.AutoTiles.Count) {
                            //オートタイルを描画
                            this.AutoTiles[x + y * Map.TileColumnCount - 1].Draw(
                                g,
                                new Point(x * mgrDBTileset.TileSize, y * mgrDBTileset.TileSize)
                            );
                            if (editMode != null) {
                                //設定情報を描画
                                this.DrawTileData(g, editMode.Value, new Point(x * mgrDBTileset.TileSize, y * mgrDBTileset.TileSize), this.AutoTiles[x + y * Map.TileColumnCount - 1].Data);
                            }
                        } else {
                            //空き領域は空白タイルにする
                        }

                        //横罫線
                        g.DrawLine(linePen, new Point(0, y * mgrDBTileset.TileSize), new Point(Map.TileColumnCount * mgrDBTileset.TileSize, y * mgrDBTileset.TileSize));
                    }

                    //縦罫線
                    g.DrawLine(linePen, new Point(x * mgrDBTileset.TileSize, 0), new Point(x * mgrDBTileset.TileSize, Map.AutoTileRowLength * mgrDBTileset.TileSize));
                }

                //オブジェクトタイル部
                var amount = mgrDBTileset.TileSize * Map.AutoTileRowLength;
                for (var i = 0; i < this.ObjTiles.Count; i++) {
                    this.ObjTiles[i].Draw(g, new Point(0, amount));

                    if (editMode != null) {
                        //設定情報を描画する
                        for (var x = 0; x < Map.TileColumnCount; x++) {
                            for (var y = 0; y < this.ObjTiles[i].TileLength.Height; y++) {
                                this.DrawTileData(g, editMode.Value, new Point(x * mgrDBTileset.TileSize, amount + y * mgrDBTileset.TileSize), this.ObjTiles[i].Data[x, y]);

                                //横罫線
                                g.DrawLine(linePen, new Point(0, amount + y * mgrDBTileset.TileSize), new Point(Map.TileColumnCount * mgrDBTileset.TileSize, amount + y * mgrDBTileset.TileSize));
                            }

                            //縦罫線
                            g.DrawLine(linePen, new Point(x * mgrDBTileset.TileSize, amount), new Point(x * mgrDBTileset.TileSize, amount + this.ObjTiles[i].TileLength.Height * mgrDBTileset.TileSize));
                        }
                    }

                    amount += (this.ObjTiles[i].GraphImg?.Height ?? 0);
                }
            }


            /// <summary>
            /// オートタイル/オブジェクトタイルに設定された各種情報を描画します。
            /// </summary>
            public void DrawTileData(Graphics g, Map.TileDataType editMode, Point pos, TileData tile) {
                const int OneCharOffset = -13;
                const int InOutOffset = 1;
                const int FontStyleNum = (int) FontStyle.Bold;
                //var font = new Font("ＭＳ ゴシック", 12.0f, FontStyle.Bold, GraphicsUnit.Point);
                var font = new Font("ＭＳ ゴシック", 14.0f, FontStyle.Bold, GraphicsUnit.Point);
                var buf = "";
                SizeF mes;

                //縁取り準備
                var gp = new System.Drawing.Drawing2D.GraphicsPath();

                switch (editMode) {
                    case Map.TileDataType.Hit:
                        buf = Map.GetTileHitTypeList()[(int) tile.Hit];
                        //g.DrawString(
                        gp.AddString(
                            buf,
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                InOutOffset + pos.X + mgrDBTileset.TileSize / 2 + OneCharOffset,
                                InOutOffset + pos.Y + mgrDBTileset.TileSize / 2 + OneCharOffset * 2 / 3
                            ),
                            StringFormat.GenericTypographic
                        );
                        break;

                    case Map.TileDataType.InOut:
                        if (tile.InOut.HasFlag(Map.InOutType.OKTop)) {
                            buf = "↑";
                        } else {
                            buf = "･";
                        }

                        mes = g.MeasureString(buf, font);
                        //g.DrawString(
                        gp.AddString(
                            buf,
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                InOutOffset + pos.X + mgrDBTileset.TileSize / 2 - (int) (mes.Width / 2),
                                InOutOffset + pos.Y + 1
                            ),
                            StringFormat.GenericTypographic
                        );

                        if (tile.InOut.HasFlag(Map.InOutType.OKBottom)) {
                            buf = "↓";
                        } else {
                            buf = "･";
                        }

                        mes = g.MeasureString(buf, font);
                        //g.DrawString(
                        gp.AddString(
                            buf,
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                InOutOffset + pos.X + mgrDBTileset.TileSize / 2 - (int) (mes.Width / 2),
                                InOutOffset + pos.Y + mgrDBTileset.TileSize - (int) mes.Height
                            ),
                            StringFormat.GenericTypographic
                        );

                        if (tile.InOut.HasFlag(Map.InOutType.OKRight)) {
                            buf = "→";
                        } else {
                            buf = "･";
                        }

                        mes = g.MeasureString(buf, font);
                        //g.DrawString(
                        gp.AddString(
                            buf,
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                InOutOffset + pos.X + mgrDBTileset.TileSize - (int) mes.Width,
                                InOutOffset + pos.Y + mgrDBTileset.TileSize / 2 - (int) (mes.Height / 2)
                            ),
                            StringFormat.GenericTypographic
                        );

                        if (tile.InOut.HasFlag(Map.InOutType.OKLeft)) {
                            buf = "←";
                        } else {
                            buf = "･";
                        }

                        mes = g.MeasureString(buf, font);
                        //g.DrawString(
                        gp.AddString(
                            buf,
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                InOutOffset + pos.X + 1,
                                InOutOffset + pos.Y + mgrDBTileset.TileSize / 2 - (int) (mes.Height / 2)
                            ),
                            StringFormat.GenericTypographic
                        );
                        break;

                    case Map.TileDataType.Priority:
                        buf = Map.GetTileDrawPriorityList()[(int) tile.Priority];
                        //g.DrawString(
                        gp.AddString(
                            buf,
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                pos.X + mgrDBTileset.TileSize / 2 + OneCharOffset,
                                pos.Y + mgrDBTileset.TileSize / 2 + OneCharOffset * 2 / 3
                            ),
                            StringFormat.GenericTypographic
                        );
                        break;

                    case Map.TileDataType.GroupNumber:
                        //g.DrawString(
                        gp.AddString(
                            $"{tile.GroupNumber,2}",
                            font.FontFamily,
                            FontStyleNum,
                            font.Height,
                            new Point(
                                pos.X + mgrDBTileset.TileSize / 2 + OneCharOffset,
                                pos.Y + mgrDBTileset.TileSize / 2 + OneCharOffset * 2 / 3
                            ),
                            StringFormat.GenericTypographic
                        );
                        break;
                }

                //縁取り実行
                //g.DrawPath(new Pen(Color.Black, 2.0f), gp);
                g.DrawPath(new Pen(Color.Black, 3.0f), gp);
                g.FillPath(Brushes.White, gp);
            }

            /// <summary>
            /// タイルセットの変更イベントを中継します。
            /// </summary>
            public void tilesetEdittedRelay(object sender, EventArgs e) {
                this.IsDirty = true;
            }

        }
    }
}
