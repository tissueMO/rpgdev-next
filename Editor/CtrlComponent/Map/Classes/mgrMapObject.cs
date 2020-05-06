using DxLibDLL;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;

    /// <summary>
    /// 管理クラス：編集用マップオブジェクト
    /// </summary>
    public partial class mgrMapObject {
        public static int TileSize = 32;
        public event EventHandler EVListChanged;
        public event EventHandler CurrentMapResized;
        public event EventHandler CursorMoved;
        public event EventHandler TilesetChanged;
        public event EventHandler TileEditted;
        private readonly UndoRedoManager UndoRedo;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public mgrMapObject(UndoRedoManager undoRedo) {
            this.SwitchLayers = new bool[Common.GetEnumCount<Map.Layer>()];
            this.UndoRedo = undoRedo;
            this.ToolInit();
        }

        /// <summary>
        /// １タイルに対する操作の前後情報
        /// </summary>
        public class TileBuffer {
            public MapOneData.TileData Before;
            public MapOneData.TileData After;
            public Point TilePosition;
            public Map.Layer Layer;

            public TileBuffer(Map.Layer layer, Point tilePos, MapOneData.TileData before) {
                this.Layer = layer;
                this.TilePosition = tilePos;
                this.Before = before;
            }
        }

        /// <summary>
        /// タイルの編集モードパターン
        /// </summary>
        public enum TileEditMode {
            Pencil,
            Rectangle,
            Ellipse,
            Fill,
        }

        /// <summary>
        /// グリッドの表示有無
        /// </summary>
        public bool VisibleGrid {
            get {
                return this.visibleGrid;
            }
            set {
                this.visibleGrid = value;
            }
        }
        private bool visibleGrid;

        /// <summary>
        /// デザイナーの拡大率
        /// </summary>
        public ViewScale.ScaleRate Scale {
            get {
                return this.scale;
            }
            set {
                //有効な拡大率である場合のみ適用
                if (Enum.IsDefined(typeof(ViewScale.ScaleRate), value)) {
                    this.scale = value;
                    this.CurrentMapResized?.Invoke(this, null);
                }
            }
        }
        private ViewScale.ScaleRate scale;

        /// <summary>
        /// 拡大率を適用したときの１タイルあたりのサイズ
        /// </summary>
        public int ScaledTileSize {
            get {
                return (int) (TileSize * ViewScale.GetRate(this.Scale));
            }
        }

        /// <summary>
        /// デザイナーのピクセル可視領域
        /// </summary>
        public Rectangle ViewRange {
            get; set;
        } = Rectangle.Empty;

        /// <summary>
        /// テスターの初期位置
        /// </summary>
        public Point StartPosition {
            get; set;
        } = Point.Empty;

        /// <summary>
        /// カーソルタイル位置
        /// </summary>
        public Point Cursor {
            get {
                return this.cursor;
            }
            set {
                this.cursor = value;
                if (this.MapData == null) {
                    //編集対象のマップがないときは無効
                    this.cursor = Point.Empty;
                } else {
                    this.cursor = Common.ApplyLimit(value, new Rectangle(Point.Empty, this.MapData.MapSize));
                    //最小値判定
                    if (value.X < 0 || value.Y < 0) {
                        this.cursor = new Point(
                            (value.X < 0) ? 0 : value.X,
                            (value.Y < 0) ? 0 : value.Y
                        );
                    }

                    //最大値判定
                    if (this.MapData.MapSize.Width <= value.X || this.MapData.MapSize.Height <= value.Y) {
                        value = new Point(
                            (this.MapData.MapSize.Width <= value.X) ? this.MapData.MapSize.Width - 1 : value.X,
                            (this.MapData.MapSize.Height <= value.Y) ? this.MapData.MapSize.Height - 1 : value.Y
                        );
                    }
                }
                this.CursorMoved?.Invoke(this, null);
            }
        }
        private Point cursor;

        /// <summary>
        /// 選択タイル内での 1/4 選択位置
        /// </summary>
        public Map.QuarterTile? CursorQuarter {
            get; set;
        } = null;

        /// <summary>
        /// タイルの編集モード
        /// </summary>
        public TileEditMode EditMode {
            get; set;
        } = TileEditMode.Pencil;

        /// <summary>
        /// 編集対象のマップオブジェクト
        /// </summary>
        public MapOneData MapData {
            get {
                return this.mapData;
            }
            set {
                this.mapData = value;
                if (value != null) {
                    this.mapData.EVListChanged += this.evListChangedRelay;
                }
                this.UndoRedo.InitBuffer();     //バッファをクリア
            }
        }
        private MapOneData mapData;

        /// <summary>
        /// 編集対象のレイヤー
        /// </summary>
        public int CurrentLayer {
            get {
                return this.currrentLayerIndex;
            }
            set {
                this.currrentLayerIndex = value;
                if (value == (int) Map.Layer.Event) {
                    //イベントレイヤーではクリッピングを解除する
                    this.ClippingTiles = null;
                    this.ClippingTileStart = null;
                }
            }
        }
        private int currrentLayerIndex = (int) Map.Layer.Low;

        /// <summary>
        /// 一時的にクリップしているイベントのFixedID
        /// </summary>
        public int ClippingEVIndex {
            get; set;
        } = -1;

        /// <summary>
        /// イベントのクリップボードバッファ
        /// </summary>
        public MapOneData.EventOneData EVClipboard {
            get; set;
        } = null;

        /// <summary>
        /// 配置確定前の矩形・楕円におけるタイルの基点
        /// </summary>
        public Point? PuttingTileStart {
            get; set;
        } = null;

        /// <summary>
        /// 配置確定前の矩形・楕円におけるタイルの範囲
        /// </summary>
        public Rectangle PuttingTileRange {
            get; set;
        } = Rectangle.Empty;

        /// <summary>
        /// 一時的にクリッピングしているタイルの基点
        /// </summary>
        public Point? ClippingTileStart {
            get; set;
        } = null;

        /// <summary>
        /// 一時的にクリッピングしているタイルの範囲
        /// </summary>
        public Rectangle ClippingTileRange {
            get; set;
        } = Rectangle.Empty;

        /// <summary>
        /// 一時的にクリッピングしているタイルの情報
        /// </summary>
        public MapOneData.TileData[,] ClippingTiles {
            get; set;
        } = null;

        /// <summary>
        /// パレットの選択範囲
        /// </summary>
        public Rectangle SelectedPalletRange {
            get; set;
        } = new Rectangle(0, 0, 1, 1);

        /// <summary>
        /// 各レイヤーの表示切替
        /// </summary>
        public bool[] SwitchLayers {
            get; set;
        }

        /// <summary>
        /// エディターツールに関わる機能を初期化します。
        /// </summary>
        public void ToolInit() {
            this.Cursor = Point.Empty;
            this.StartPosition = Point.Empty;
            this.ClippingEVIndex = -1;
            this.ClippingTiles = null;
            this.ClippingTileStart = null;
            this.ClippingTileRange = Rectangle.Empty;
            this.PuttingTileStart = null;
            this.EVClipboard = null;
            this.SelectedPalletRange = new Rectangle(0, 0, 1, 1);
        }

        /// <summary>
        /// 保存処理を行います。
        /// </summary>
        public bool DoSave(ref FileStream fsTile, ref FileStream fsSQ) {
            if (this.MapData == null || fsTile == null) {
                return false;
            }

            //タイル部：書き込みモードで開き直す
            var fullPath = fsTile.Name;
            Common.EndFileAccessLock(ref fsTile);
            try {
                fsTile = new FileStream(fullPath, FileMode.Create, FileAccess.ReadWrite, FileShare.Read);
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "タイルデータの保存"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            //タイル部：GZip圧縮
            using (var W = new StreamWriter(fsTile, Common.SJIS)) {
                var buf = "";

                //ヘッダー情報
                W.WriteLine(this.MapData.Name);
                W.WriteLine(this.MapData.GUID);
                W.WriteLine(Common.SizeToString(this.MapData.MapSize));
                W.WriteLine((int) this.MapData.MapLoopOption);
                W.WriteLine(this.MapData.TilesetFixedID);
                W.WriteLine((int) this.MapData.MoveSpeed);
                W.WriteLine(this.MapData.BaseMapFileName);
                W.WriteLine(this.MapData.BGM.ToString());
                W.WriteLine(this.MapData.BGS.ToString());
                W.WriteLine(this.MapData.BackPictureFileName);
                W.WriteLine((int) this.MapData.BackPictureScrollType);
                W.WriteLine(this.MapData.BackPictureScrollSpeed);
                W.WriteLine(this.MapData.FogFileName);
                W.WriteLine((int) this.MapData.FogScrollType);
                W.WriteLine(this.MapData.FogScrollSpeed);
                W.WriteLine((int) this.MapData.FogBlendMode);

                //エンカウントユニット
                buf = "";
                foreach (var unit in this.MapData.EncounterUnits) {
                    buf += unit;
                    buf += Resources.Split_LongData;
                }
                buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
                W.WriteLine(buf);

                W.WriteLine(this.MapData.EncounterRate);
                W.WriteLine(this.MapData.BattleBackFileName);
                W.WriteLine();

                //タイルデータ
                for (var l = (int) Map.Layer.Low; l <= (int) Map.Layer.Shadow; l++) {
                    for (var y = 0; y < this.MapData.MapSize.Height; y++) {
                        for (var x = 0; x < this.MapData.MapSize.Width; x++) {
                            // [LT,RT,LB,RB] を先行して書き込む
                            for (var p = 0; p < (int) Map.QuarterTile.Count; p++) {
                                W.Write(this.MapData.LayerTiles[l, x, y].GetQuarter((Map.QuarterTile) p));
                                if (p < (int) Map.QuarterTile.Count - 1) {
                                    W.Write(Resources.Split_Argument);
                                }
                            }

                            //タイルレイヤーは [,TilePosX:Y] を後ろに付ける
                            if (l != (int) Map.Layer.Shadow) {
                                W.Write(Resources.Split_Argument);
                                W.Write(Common.PointToString(this.MapData.LayerTiles[l, x, y].PalletPosition));
                            }

                            W.Write(Resources.Split_LongData);
                        }
                        W.WriteLine();
                    }
                    W.WriteLine();
                }
            }

            //タイル部：再度読み込みモードで編集権を持ち直す
            Common.StartFileAccessLock(ref fsTile, fsTile.Name.Substring(ProjectManager.ProjectPath.Length));

            //スクリプト部：書き込みモードで開き直す
            if (fsSQ == null) {
                fullPath = ProjectManager.ProjectPath + Resources.Path_Scripts_MapEvents + this.MapData.GUID + Resources.Extension_SQ;
            } else {
                fullPath = fsSQ.Name;
                Common.EndFileAccessLock(ref fsSQ);
            }

            try {
                fsSQ = new FileStream(fullPath, FileMode.Create, FileAccess.ReadWrite, FileShare.Read);
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "イベントスクリプトの保存"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            //スクリプト部：出力
            using (var W = new StreamWriter(fsSQ, Common.SJIS)) {
                W.WriteLine("// " + this.MapData.Name);
                W.WriteLine();

                //全イベントを配置する関数
                W.WriteLine(Resources.SQ_EVSetFunc + " {");
                for (var i = 0; i < this.MapData.EVCount; i++) {
                    this.MapData[i].DoSaveEVSetFunction(W);
                }
                W.WriteLine("}");
                W.WriteLine();

                //各イベントのスクリプト
                for (var i = 0; i < this.MapData.EVCount; i++) {
                    this.MapData[i].DoSaveEVFunction(W);
                }
            }

            //スクリプト部：再度読み込みモードで編集権を持ち直す
            Common.StartFileAccessLock(ref fsSQ, fsSQ.Name.Substring(ProjectManager.ProjectPath.Length));

            return true;
        }

        /// <summary>
        /// 読み込み処理を行います。
        /// </summary>
        public bool DoLoad(string rPath, ref FileStream fsTile, ref FileStream fsSQ) {
            var isReadOnly = false;

            //タイル部：GZip解凍
            var newData = mgrMapObject.LoadTileData(rPath);
            if (newData == null) {
                //タイルファイルが存在しなかった
                return false;
            }

            //タイル部：編集権を得ようとする
            isReadOnly = !Common.StartFileAccessLock(ref fsTile, rPath);

            //スクリプトファイルが存在するかチェック
            if (!File.Exists(ProjectManager.ProjectPath + newData.ScriptFileName)) {
                MessageBox.Show("現在のマップに対応するスクリプトが見つかりませんでした。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            } else {
                //スクリプト部読み込み
                using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + newData.ScriptFileName), Common.SJIS)) {
                    var evIndex = -1;

                    //全イベントを配置する関数から、イベント情報を読み込む
                    R.ReadLine();       //読み捨て マップ名
                    R.ReadLine();       //読み捨て 空行
                    R.ReadLine();       //読み捨て function SetupMapEvents(...) {
                    var evList = MapOneData.EventOneData.DoLoadEVSetFunction(R);

                    //イベントを正式に登録する
                    newData.ClearEV();
                    foreach (var ev in evList) {
                        //ここでリソースも読み込まれる
                        newData.AddEV(ev, ev.FixedID);
                    }

                    //イベント関数が読み込んだ順序通りに並んでいると想定して、見出しを順次追いかける
                    for (evIndex = 0; evIndex < newData.EVCount; evIndex++) {
                        if (!newData[evIndex].DoLoadEVFunction(R)) {
                            //見つからなかった場合は中断して続行する
                            MessageBox.Show(Resources.MsgE_Failed.Replace("$", "イベントスクリプトの読み込み"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            break;
                        }
                    }
                }

                //スクリプト部：編集権を得ようとする
                isReadOnly |= !Common.StartFileAccessLock(ref fsSQ, newData.ScriptFileName);
            }

            //ベースマップを読み込む
            var baseMapData = mgrMapObject.LoadTileData(newData.BaseMapFileName);
            if (baseMapData != null) {
                newData.BaseMapLayerTiles = baseMapData.LayerTiles;
            }

            //エディターツールを初期化して読み込んだマップを反映
            this.ToolInit();
            this.MapData = newData;
            this.EVListChanged?.Invoke(this, null);
            this.CurrentMapResized?.Invoke(this, null);
            this.TilesetChanged?.Invoke(this, null);
            this.MapData.EVListChanged += this.evListChangedRelay;
            this.MapData.MapResized += this.mapResizedRelay;
            this.MapData.TilesetChanged += this.tilesetChangedRelay;
            this.MapData.TileEditted += this.tileEdittedRelay;

            return isReadOnly;
        }

        /// <summary>
        /// マップエディターに登録されているすべてのマップのGUIDリストを返します。
        /// </summary>
        public static string[] GetGUIDList() {
            var files = Directory.GetFiles(ProjectManager.ProjectPath + Common.CutLastChar(Resources.Path_Maps, '\\'), "*.map", SearchOption.AllDirectories);
            var ids = new List<string>();

            //すべてのマップのヘッダー部分だけを読み込む
            foreach (var file in files) {
                using (var R = new StreamReader(Common.OpenFileReadOnly(file), Common.SJIS)) {
                    //ヘッダー情報
                    R.ReadLine();       //マップ名を読み捨て
                    ids.Add(R.ReadLine());
                }
            }
            return ids.ToArray();
        }

        /// <summary>
        /// タイルデータを読み込みます。
        /// </summary>
        public static MapOneData LoadTileData(string rPath) {
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                return null;
            }

            var newData = new MapOneData();
            MapOneData.OutOfEdit = true;

            //タイル部
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath), Common.SJIS)) {
                var buf = "";

                //ヘッダー情報
                newData.Name = R.ReadLine();
                newData.GUID = R.ReadLine();
                newData.MapSize = Common.StringToSize(R.ReadLine());            //ここでタイル配列が初期化される
                newData.MapLoopOption = (Map.MapLoopType) int.Parse(R.ReadLine());
                newData.TilesetFixedID = int.Parse(R.ReadLine());
                newData.MoveSpeed = (Map.Speed) int.Parse(R.ReadLine());
                newData.BaseMapFileName = R.ReadLine();
                newData.BGM = new Media.SoundObject(R.ReadLine());
                newData.BGS = new Media.SoundObject(R.ReadLine());
                newData.BackPictureFileName = R.ReadLine();
                newData.BackPictureScrollType = (Map.ScrollType) int.Parse(R.ReadLine());
                newData.BackPictureScrollSpeed = int.Parse(R.ReadLine());
                newData.FogFileName = R.ReadLine();
                newData.FogScrollType = (Map.ScrollType) int.Parse(R.ReadLine());
                newData.FogScrollSpeed = int.Parse(R.ReadLine());
                newData.FogBlendMode = (Media.BlendMode) int.Parse(R.ReadLine());

                buf = R.ReadLine();
                var spl = buf.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                newData.EncounterUnits = new List<int>();
                foreach (var fixedID in spl) {
                    newData.EncounterUnits.Add(int.Parse(fixedID));
                }

                newData.EncounterRate = int.Parse(R.ReadLine());
                newData.BattleBackFileName = R.ReadLine();

                R.ReadLine();       //空行

                //タイルデータ
                for (var l = (int) Map.Layer.Low; l <= (int) Map.Layer.Shadow; l++) {
                    for (var y = 0; y < newData.MapSize.Height; y++) {
                        //１行丸ごと読み込んでしまうと、サイズが大きすぎた場合にデータが落ちてしまうので１文字ずつ読み込む
                        var x = 0;
                        var temp = "";
                        buf = "";
                        do {
                            temp = ((char) (R.Read())).ToString();
                            if (temp != Resources.Split_LongData) {
                                buf += temp;
                            } else {
                                var sub = buf.Split(Resources.Split_Argument.ToCharArray());

                                // 1/4 タイル情報が先行する
                                newData.LayerTiles[l, x, y].SetQuarters(
                                    int.Parse(sub[(int) Map.QuarterTile.LeftTop]),
                                    int.Parse(sub[(int) Map.QuarterTile.RightTop]),
                                    int.Parse(sub[(int) Map.QuarterTile.LeftBottom]),
                                    int.Parse(sub[(int) Map.QuarterTile.RightBottom])
                                );

                                //タイルレイヤーにはタイルパレット位置が含まれている
                                if (sub.Length > (int) Map.QuarterTile.Count) {
                                    newData.LayerTiles[l, x, y].PalletPosition = Common.StringToPoint(sub[(int) Map.QuarterTile.Count]);
                                }
                                buf = "";
                                x++;
                            }
                        } while (x < newData.MapSize.Width);
                        R.ReadLine();       //残りの改行コードを読み捨て
                    }
                    R.ReadLine();       //空行読み捨て
                }
            }
            MapOneData.OutOfEdit = false;
            return newData;
        }

        /// <summary>
        /// 指定したタイル座標にパレットで選択されたタイルを配置します。
        /// 鉛筆ツールのときのみ、配置したタイルの前後情報をリストにして返します。
        /// </summary>
        /// <param name="absolutely">オートタイルを更新せずに原形を留めるかどうか</param>
        public List<TileBuffer> PutTiles(int dx, int dy, bool absolutely) {
            var list = new List<TileBuffer>();
            if (!Enum.IsDefined(typeof(Map.Layer), this.CurrentLayer)) {
                //無効なレイヤー
                return list;
            }

            //編集モードに従った処理
            switch (this.EditMode) {
                case TileEditMode.Pencil:
                    if (this.ClippingTiles == null) {
                        //パレットの選択領域を使って配置する
                        var put = new TileBuffer((Map.Layer) this.CurrentLayer, new Point(dx, dy), this.MapData.LayerTiles[this.CurrentLayer, dx, dy]);
                        this.MapData.LayerTiles[this.CurrentLayer, dx, dy].PalletPosition = this.SelectedPalletRange.Location;
                        if (!absolutely) {
                            this.ApplyAutoTile((Map.Layer) this.CurrentLayer, dx, dy);
                        }
                        put.After = this.MapData.LayerTiles[this.CurrentLayer, dx, dy];
                        list.Add(put);
                    } else {
                        //クリッピング領域を使って配置する
                        for (var x = 0; x < this.ClippingTiles.GetLength(0) && dx + x < this.MapData.MapSize.Width; x++) {
                            for (var y = 0; y < this.ClippingTiles.GetLength(1) && dy + y < this.MapData.MapSize.Height; y++) {
                                var put = new TileBuffer((Map.Layer) this.CurrentLayer, new Point(dx + x, dy + y), this.MapData.LayerTiles[this.CurrentLayer, dx + x, dy + y]);
                                this.MapData.LayerTiles[this.CurrentLayer, dx + x, dy + y].PalletPosition = this.ClippingTiles[x, y].PalletPosition;
                                this.MapData.LayerTiles[this.CurrentLayer, dx + x, dy + y].SetQuarters(this.ClippingTiles[x, y].GetQuarters());
                                if (!absolutely) {
                                    this.ApplyAutoTile((Map.Layer) this.CurrentLayer, dx + x, dy + y);
                                }
                                put.After = this.MapData.LayerTiles[this.CurrentLayer, dx + x, dy + y];
                                list.Add(put);
                            }
                        }
                    }
                    break;

                case TileEditMode.Rectangle:
                case TileEditMode.Ellipse:
                    //確定前矩形領域の有効範囲を生成する
                    var tempPath = new System.Drawing.Drawing2D.GraphicsPath();
                    switch (this.EditMode) {
                        case TileEditMode.Rectangle:
                            tempPath.AddRectangle(this.PuttingTileRange);
                            break;
                        case TileEditMode.Ellipse:
                            tempPath.AddEllipse(this.PuttingTileRange.X - 1, this.PuttingTileRange.Y - 1, this.PuttingTileRange.Width + 1, this.PuttingTileRange.Height + 1);
                            break;
                    }

                    //有効範囲内にタイルを配置する
                    for (int a = 0, x = this.PuttingTileRange.Left; x < this.PuttingTileRange.Right; x++, a++) {
                        for (int b = 0, y = this.PuttingTileRange.Top; y < this.PuttingTileRange.Bottom; y++, b++) {
                            if (!tempPath.IsVisible(x, y)) {
                                continue;       //描画範囲外
                            } else if (this.ClippingTiles == null) {
                                //パレットの選択領域を使って配置する
                                this.MapData.LayerTiles[this.CurrentLayer, x, y].PalletPosition = this.SelectedPalletRange.Location;
                            } else {
                                //クリッピング領域を使って配置する
                                this.MapData.LayerTiles[this.CurrentLayer, x, y].PalletPosition = this.ClippingTiles[a % this.ClippingTiles.GetLength(0), b % this.ClippingTiles.GetLength(1)].PalletPosition;
                                this.MapData.LayerTiles[this.CurrentLayer, x, y].SetQuarters(this.ClippingTiles[a % this.ClippingTiles.GetLength(0), b % this.ClippingTiles.GetLength(1)].GetQuarters());
                            }
                            if (!absolutely) {
                                this.ApplyAutoTile((Map.Layer) this.CurrentLayer, x, y);
                            }
                        }
                    }
                    break;
            }

            return list;
        }

        /// <summary>
        /// 指定したタイル座標に指定したパレットタイルを配置します。
        /// </summary>
        /// <param name="absolutely">オートタイルを更新せずに原形を留めるかどうか</param>
        private void putTile(int x, int y, bool absolutely) {
            if (this.ClippingTiles != null) {
                //クリッピング領域を使って配置する
                this.MapData.LayerTiles[this.CurrentLayer, x, y].PalletPosition = this.ClippingTiles[x % this.ClippingTiles.GetLength(0), y % this.ClippingTiles.GetLength(1)].PalletPosition;
                this.MapData.LayerTiles[this.CurrentLayer, x, y].SetQuarters(this.ClippingTiles[x % this.ClippingTiles.GetLength(0), y % this.ClippingTiles.GetLength(1)].GetQuarters());
            } else {
                //パレットの選択領域を使って配置する
                this.MapData.LayerTiles[this.CurrentLayer, x, y].PalletPosition = new Point(this.SelectedPalletRange.Left + (x % this.SelectedPalletRange.Width), this.SelectedPalletRange.Top + (y % this.SelectedPalletRange.Height));
            }
            if (!absolutely) {
                this.ApplyAutoTile((Map.Layer) this.CurrentLayer, x, y);
            }
        }

        /// <summary>
        /// 指定位置を基点として、スキャンライン・シードフィルに基づく塗りつぶしを行います。
        /// 塗りつぶしを行ったタイルのリストを返値とします。
        /// </summary>
        public List<TileBuffer> FillTiles(int x, int y, bool absolutely) {
            var list = new List<TileBuffer>();
            if (!Enum.IsDefined(typeof(Map.Layer), this.CurrentLayer)) {
                //無効なレイヤー
                return list;
            }

            var buf = new Queue<Point>();
            var baseTile = this.MapData.LayerTiles[this.CurrentLayer, x, y].PalletPosition;
            Point fillTile;
            if (this.ClippingTiles == null) {
                fillTile = this.SelectedPalletRange.Location;
            } else {
                fillTile = this.ClippingTiles[0, 0].PalletPosition;
            }

            //始点が既に塗りつぶすタイルである場合は処理しない
            if (baseTile == fillTile) {
                return null;
            }

            //初動を追加
            buf.Enqueue(new Point(x, y));

            //バッファにデータが残っている限り処理し続ける	
            while (buf.Count > 0) {
                var p = buf.Dequeue();      //バッファの先頭を取り出す
                int left;
                int right;

                //既に塗ったタイルは処理しない
                if (this.MapData.LayerTiles[this.CurrentLayer, p.X, p.Y].PalletPosition == fillTile) {
                    continue;
                }

                //左端を探索
                for (left = p.X; 0 < left; left--) {
                    if (this.MapData.LayerTiles[this.CurrentLayer, left - 1, p.Y].PalletPosition != baseTile) {
                        break;
                    }
                }

                //右端を探索
                for (right = p.X; right < this.MapData.MapSize.Width - 1; right++) {
                    if (this.MapData.LayerTiles[this.CurrentLayer, right + 1, p.Y].PalletPosition != baseTile) {
                        break;
                    }
                }

                //左端から右端までを塗る
                for (var i = left; i <= right; i++) {
                    //一連の操作に追加する
                    var put = new TileBuffer((Map.Layer) this.CurrentLayer, new Point(i, p.Y), this.MapData.LayerTiles[this.CurrentLayer, i, p.Y]);
                    this.putTile(i, p.Y, absolutely);
                    put.After = this.MapData.LayerTiles[this.CurrentLayer, i, p.Y];
                    list.Add(put);
                }

                //下側の位置を探索
                if (p.Y + 1 < this.MapData.MapSize.Height) {
                    this.scanLine(left, right, p.Y + 1, baseTile, buf);
                }

                //上側の位置を探索
                if (0 <= p.Y - 1) {
                    this.scanLine(left, right, p.Y - 1, baseTile, buf);
                }
            }

            return list;
        }

        /// <summary>
        /// 塗りつぶし：バッファに入れる新しい位置を探索します。
        /// </summary>
        private void scanLine(int left, int right, int y, Point baseTile, Queue<Point> buf) {
            //左から右へ探索する
            while (left <= right) {
                for (; left <= right; left++) {
                    if (this.MapData.LayerTiles[this.CurrentLayer, left, y].PalletPosition == baseTile) {
                        //始点と同じタイルを見つけた
                        break;
                    }
                }

                //右端より右側に来たら処理中断
                if (left > right) {
                    break;
                }

                //基点のタイルがあるブロック列の一番右側にあるブロックを探す
                for (; left <= right; left++) {
                    if (this.MapData.LayerTiles[this.CurrentLayer, left, y].PalletPosition != baseTile) {
                        break;
                    }
                }

                buf.Enqueue(new Point(left - 1, y));
            }
        }

        /// <summary>
        /// 指定タイル座標とその八方のタイルのオートタイルを適用させます。
        /// </summary>
        public void ApplyAutoTile(Map.Layer layer, int tileX, int tileY, bool isParent = true) {
            if ((int) layer >= (int) Map.Layer.Shadow) {
                //オートタイルとは無縁なレイヤーは処理しない
                return;
            }

            var l = (int) layer;
            int vx, vy, px, py, tx, ty, bx, by, cx, cy;
            Point ny, ax, ay, ac;
            Point palPos;
            Map.QuarterTile part;
            tx = tileX;
            ty = tileY;

            //指定されたタイル座標についてセットする
            for (var y = 1; y <= 2; y++) {
                for (var x = 1; x <= 2; x++) {
                    //探索用
                    vx = (x % 2 == 1) ? -1 : 1;
                    vy = (y % 2 == 1) ? -1 : 1;
                    px = tx + (x == 0 ? -1 : (x == 3) ? 1 : 0);
                    py = ty + (y == 0 ? -1 : (y == 3) ? 1 : 0);

                    //周囲のデータ：比較用・範囲外の空間には比較基準と同じタイルがあるものとみなす
                    palPos = this.MapData.LayerTiles[l, px, py].PalletPosition;
                    ny = palPos;        //比較基準
                    ax = (px + vx < 0 || this.MapData.MapSize.Width <= px + vx) ? palPos : this.MapData.LayerTiles[l, px + vx, py].PalletPosition;      //横の比較用
                    ay = (py + vy < 0 || this.MapData.MapSize.Height <= py + vy) ? palPos : this.MapData.LayerTiles[l, px, py + vy].PalletPosition;     //縦の比較用
                    ac = (px + vx < 0 || this.MapData.MapSize.Width <= px + vx || py + vy < 0 || this.MapData.MapSize.Height <= py + vy) ? palPos : this.MapData.LayerTiles[l, px + vx, py + vy].PalletPosition;   //角(斜め)の比較用

                    //対象位置のパラメーター
                    bx = (x % 2 == 0) ? 1 : 0;
                    by = (y % 2 == 0) ? 1 : 0;
                    cx = ((x == 0) ? -1 : (x == 3) ? 1 : 0);
                    cy = ((y == 0) ? -1 : (y == 3) ? 1 : 0);

                    if (tx + cx < 0 || this.MapData.MapSize.Width <= tx + cx || ty + cy < 0 || this.MapData.MapSize.Height <= ty + cy) {
                        //範囲外の部分は処理しない
                        continue;
                    }

                    palPos = this.MapData.LayerTiles[l, tx + cx, ty + cy].PalletPosition;
                    part = (Map.QuarterTile) ((x - 1) + (y - 1) * 2);

                    if (palPos.IsEmpty == false && palPos.Y < Map.AutoTileRowLength) {
                        //オートタイルの場合のみ処理する
                        if (ny == ax && ny == ay && ny == ac) {
                            //縦・横・角がすべて同一タイル
                            this.MapData.LayerTiles[l, tx + cx, ty + cy].SetQuarter(part, (int) Map.AutoTilePattern.AllSide);
                        } else if (ny == ax && ny == ay) {
                            //縦・横が同一タイル
                            this.MapData.LayerTiles[l, tx + cx, ty + cy].SetQuarter(part, (int) Map.AutoTilePattern.FourSide);
                        } else if (ny == ay) {
                            //縦が同一タイル
                            this.MapData.LayerTiles[l, tx + cx, ty + cy].SetQuarter(part, (int) Map.AutoTilePattern.Vertical);
                        } else if (ny == ax) {
                            //横が同一タイル
                            this.MapData.LayerTiles[l, tx + cx, ty + cy].SetQuarter(part, (int) Map.AutoTilePattern.Horizontal);
                        } else {
                            //四方八方が異なるタイル：独立型
                            this.MapData.LayerTiles[l, tx + cx, ty + cy].SetQuarter(part, (int) Map.AutoTilePattern.Isolate);
                        }
                    } else {
                        //オブジェクトタイルは常に独立型にする
                        this.MapData.LayerTiles[l, tx + cx, ty + cy].SetQuarter(part, (int) Map.AutoTilePattern.Isolate);
                    }
                }
            }

            //基点である場合、さらにその八方にあるタイルの接している部分を更新する
            if (isParent) {
                for (var x = -1; x <= +1; x++) {
                    for (var y = -1; y <= +1; y++) {
                        if (x == 0 && y == 0) {
                            //基点は処理しない
                            continue;
                        } else if (tileX + x < 0 || tileY + y < 0 || this.MapData.MapSize.Width <= tileX + x || this.mapData.MapSize.Height <= tileY + y) {
                            //範囲外は処理しない
                            continue;
                        }
                        this.ApplyAutoTile(layer, tileX + x, tileY + y, false);
                    }
                }
            }
        }

        /// <summary>
        /// 現在のカーソル位置に影を設定/解除します。
        /// </summary>
        /// <param name="tilePixelX">カーソル位置のピクセルX座標から何ピクセル離れているか</param>
        /// <param name="tilePixelY">カーソル位置のピクセルY座標から何ピクセル離れているか</param>
        public void PutShadow(int tx, int ty, Map.QuarterTile? quarter, bool remove) {
            if (quarter.HasValue) {
                this.MapData.LayerTiles[(int) Map.Layer.Shadow, tx, ty].SetQuarter(quarter.Value, (remove ? DX.FALSE : DX.TRUE));
            }
        }

        /// <summary>
        /// デザイナー画面で必要となるスクロール幅を算出します。
        /// </summary>
        public bool GetMapPreviewScrollSize(Size viewerSize, out int width, out int height) {
            width = 0;
            height = 0;
            if (this.MapData == null) {
                return false;
            }

            //横スクロール
            if (this.MapData.MapSize.Width * this.ScaledTileSize > viewerSize.Width) {
                width = this.MapData.MapSize.Width * this.ScaledTileSize - viewerSize.Width + ((viewerSize.Width % this.ScaledTileSize == 0) ? 0 : this.ScaledTileSize);
            }

            //縦スクロール
            if (this.MapData.MapSize.Height * this.ScaledTileSize > viewerSize.Height) {
                height = this.MapData.MapSize.Height * this.ScaledTileSize - viewerSize.Height + ((viewerSize.Width % this.ScaledTileSize == 0) ? 0 : this.ScaledTileSize);
            }
            return true;
        }

        /// <summary>
        /// キー入力でカーソルを移動させます。
        /// </summary>
        public void MoveCursorByKeyDown(Keys keyData) {
            switch (keyData) {
                case Keys.A:
                    this.Cursor = new Point(this.Cursor.X - 1, this.Cursor.Y);
                    break;
                case Keys.W:
                    this.Cursor = new Point(this.Cursor.X, this.Cursor.Y - 1);
                    break;
                case Keys.D:
                    this.Cursor = new Point(this.Cursor.X + 1, this.Cursor.Y);
                    break;
                case Keys.S:
                    this.Cursor = new Point(this.Cursor.X, this.Cursor.Y + 1);
                    break;
            }
        }

        /// <summary>
        /// イベントリストが変更されたときのイベントを中継します。
        /// </summary>
        private void evListChangedRelay(object sender, EventArgs e) {
            this.EVListChanged?.Invoke(sender, e);
        }

        /// <summary>
        /// マップのサイズが変更されたときのイベントを中継します。
        /// </summary>
        private void mapResizedRelay(object sender, EventArgs e) {
            //カーソルが範囲外に行ったら修正する
            if (this.MapData.MapSize.Width <= this.Cursor.X || this.MapData.MapSize.Height <= this.Cursor.Y) {
                //プロパティが自動で範囲内に収める
                this.Cursor = new Point(this.Cursor.X, this.Cursor.Y);
            }
            this.CurrentMapResized?.Invoke(sender, e);
        }

        /// <summary>
        /// タイルセットが変更されたときのイベントを中継します。
        /// </summary>
        private void tilesetChangedRelay(object sender, EventArgs e) {
            this.TilesetChanged?.Invoke(sender, e);
        }

        /// <summary>
        /// タイルが変更されたときのイベントを中継します。
        /// </summary>
        public void tileEdittedRelay(object sender, EventArgs e) {
            this.TileEditted?.Invoke(this, null);
        }

    }
}
