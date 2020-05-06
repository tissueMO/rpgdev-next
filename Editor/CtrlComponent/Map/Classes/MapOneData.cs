using DxLibDLL;
using System;
using System.Collections.Generic;
using System.Drawing;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Module.DB;
    using Properties;

    public partial class mgrMapObject {

        /// <summary>
        /// マップの単一データ
        /// </summary>
        public partial class MapOneData {
            public event EventHandler EVListChanged;
            public event EventHandler MapResized;
            public event EventHandler TilesetChanged;
            public event EventHandler TileEditted;
            private Guid guidGen = Guid.NewGuid();
            public static bool OutOfEdit;

            public MapOneData() {
                this.SetNewGUID();
                this.LayerTiles = new TileData[Common.GetEnumCount<Map.Layer>() - 1, this.MapSize.Width, this.MapSize.Height];

                //タイル情報初期化
                MapOneData.OutOfEdit = true;
                for (var layer = (int) Map.Layer.Low; layer <= (int) Map.Layer.Shadow; layer++) {
                    for (var x = 0; x < this.MapSize.Width; x++) {
                        for (var y = 0; y < this.MapSize.Height; y++) {
                            this.LayerTiles[layer, x, y].SetQuarters(0, 0, 0, 0);
                        }
                    }
                }
                MapOneData.OutOfEdit = false;

                //既定のイベントハンドラーをセットする
                TileData.Editted += this.tileEdittedRelay;
            }

            /// <summary>
            /// スクリプト名に対応するグローバル一意識別子
            /// </summary>
            public string GUID {
                get; set;
            }

            /// <summary>
            /// このマップと対応するイベントスクリプトの場所
            /// </summary>
            public string ScriptFileName {
                get {
                    return Resources.Path_Scripts_MapEvents + this.GUID + Resources.Extension_SQ;
                }
            }

            /// <summary>
            /// マップ表示名
            /// </summary>
            public string Name {
                get; set;
            }

            /// <summary>
            /// マップファイルの場所
            /// </summary>
            public string FileName {
                get; set;
            }

            /// <summary>
            /// マップのサイズ
            /// </summary>
            public Size MapSize {
                get {
                    return this.mapSize;
                }
                set {
                    this.mapSize = value;

                    //元のタイルデータを保持しながら縮小/拡大する
                    var newTiles = new TileData[Common.GetEnumCount<Map.Layer>() - 1, this.mapSize.Width, this.mapSize.Height];

                    for (var l = 0; l < newTiles.GetLength(0); l++) {
                        for (var x = 0; x < this.mapSize.Width && x < this.LayerTiles.GetLength(1); x++) {
                            for (var y = 0; y < this.mapSize.Height && y < this.LayerTiles.GetLength(2); y++) {
                                newTiles[l, x, y] = this.LayerTiles[l, x, y];
                            }
                        }
                    }

                    this.LayerTiles = newTiles;
                    this.MapResized?.Invoke(this, null);
                }
            }
            private Size mapSize = new Size(20, 20);

            /// <summary>
            /// マップの有効タイル範囲
            /// </summary>
            public Rectangle MapRectangle {
                get {
                    return new Rectangle(Point.Empty, this.MapSize);
                }
            }

            /// <summary>
            /// 使用するタイルセット
            /// </summary>
            public int TilesetFixedID {
                get {
                    return this.tilesetFixedID;
                }
                set {
                    if (this.tilesetFixedID != value) {
                        this.tilesetFixedID = value;
                        this.TilesetChanged?.Invoke(this, null);
                    }
                }
            }
            private int tilesetFixedID = -1;

            /// <summary>
            /// プレイヤーの移動速度
            /// </summary>
            public Map.Speed MoveSpeed {
                get; set;
            } = Map.Speed.Normal;

            /// <summary>
            /// カーボンのベースとなるマップファイル名
            /// </summary>
            public string BaseMapFileName {
                get; set;
            }

            /// <summary>
            /// マップのループ設定
            /// </summary>
            public Map.MapLoopType MapLoopOption {
                get; set;
            } = Map.MapLoopType.None;

            /// <summary>
            /// BGM情報
            /// </summary>
            public Media.SoundObject BGM {
                get; set;
            } = new Media.SoundObject("");

            /// <summary>
            /// BGS情報
            /// </summary>
            public Media.SoundObject BGS {
                get; set;
            } = new Media.SoundObject("");

            /// <summary>
            /// 遠景ファイル名
            /// </summary>
            public string BackPictureFileName {
                get; set;
            }

            /// <summary>
            /// 遠景スクロール方法
            /// </summary>
            public Map.ScrollType BackPictureScrollType {
                get; set;
            } = Map.ScrollType.None;

            /// <summary>
            /// 遠景スクロール速度
            /// </summary>
            public int BackPictureScrollSpeed {
                get; set;
            } = 0;

            /// <summary>
            /// フォグファイル名
            /// </summary>
            public string FogFileName {
                get; set;
            }

            /// <summary>
            /// フォグスクロール方法
            /// </summary>
            public Map.ScrollType FogScrollType {
                get; set;
            } = Map.ScrollType.None;

            /// <summary>
            /// フォグスクロール速度
            /// </summary>
            public int FogScrollSpeed {
                get; set;
            } = 0;

            /// <summary>
            /// フォグ合成方法
            /// </summary>
            public Media.BlendMode FogBlendMode {
                get; set;
            } = Media.BlendMode.Alpha;

            /// <summary>
            /// エンカウントするユニットのFixedIDリスト
            /// </summary>
            public List<int> EncounterUnits {
                get; set;
            } = new List<int>();

            /// <summary>
            /// エンカウント係数
            /// </summary>
            public int EncounterRate {
                get; set;
            } = 100;

            /// <summary>
            /// 戦闘背景ファイル名
            /// </summary>
            public string BattleBackFileName {
                get; set;
            }

            /// <summary>
            /// タイル情報
            /// </summary>
            public TileData[,,] LayerTiles {
                get;
                private set;
            }

            /// <summary>
            /// カーボン元のタイル情報
            /// </summary>
            public TileData[,,] BaseMapLayerTiles {
                get; set;
            }

            /// <summary>
            /// 内部で管理するイベントリスト
            /// </summary>
            private List<EventOneData> EVs {
                get; set;
            } = new List<EventOneData>();

            /// <summary>
            /// 外部に公開するイベントリストはインデクサーで実装します。
            /// </summary>
            public EventOneData this[int index] {
                get {
                    if (index < 0 || this.EVs.Count <= index) {
                        return null;
                    } else {
                        return this.EVs[index];
                    }
                }
                set {
                    if (index < 0 || this.EVs.Count <= index) {
                        return;
                    } else {
                        this.EVs[index] = value;
                    }
                }
            }

            /// <summary>
            /// イベントの総数
            /// </summary>
            public int EVCount {
                get {
                    return this.EVs.Count;
                }
            }

            /// <summary>
            /// マップ全体をシフト：イベントを含む全レイヤーに対して適用します。
            /// </summary>
            public void Offset(Point offset) {
                //X軸方向
                if (offset.X < 0) {
                    //左へシフト
                    for (var i = Math.Abs(offset.X); i > 0; i--) {
                        for (var y = 0; y < this.MapSize.Height; y++) {
                            for (var layer = (int) Map.Layer.Low; layer <= (int) Map.Layer.Shadow; layer++) {
                                var temp = this.LayerTiles[layer, 0, y];
                                for (var x = 0; x < this.MapSize.Width - 1; x++) {
                                    this.LayerTiles[layer, x, y] = this.LayerTiles[layer, x + 1, y];
                                }
                                this.LayerTiles[layer, this.MapSize.Width - 1, y] = temp;
                            }
                        }
                    }
                } else if (offset.X > 0) {
                    //右へシフト
                    for (var i = Math.Abs(offset.X); i > 0; i--) {
                        for (var y = 0; y < this.MapSize.Height; y++) {
                            for (var layer = (int) Map.Layer.Low; layer <= (int) Map.Layer.Shadow; layer++) {
                                var temp = this.LayerTiles[layer, this.MapSize.Width - 1, y];
                                for (var x = this.MapSize.Width - 1; x > 0; x--) {
                                    this.LayerTiles[layer, x, y] = this.LayerTiles[layer, x - 1, y];
                                }
                                this.LayerTiles[layer, 0, y] = temp;
                            }
                        }
                    }
                }

                //Y軸方向
                if (offset.Y < 0) {
                    //上へシフト
                    for (var i = Math.Abs(offset.Y); i > 0; i--) {
                        for (var x = 0; x < this.MapSize.Width; x++) {
                            for (var layer = (int) Map.Layer.Low; layer <= (int) Map.Layer.Shadow; layer++) {
                                var temp = this.LayerTiles[layer, x, 0];
                                for (var y = 0; y < this.MapSize.Height - 1; y++) {
                                    this.LayerTiles[layer, x, y] = this.LayerTiles[layer, x, y + 1];
                                }
                                this.LayerTiles[layer, x, this.MapSize.Height - 1] = temp;
                            }
                        }
                    }
                } else if (offset.Y > 0) {
                    //下へシフト
                    for (var i = Math.Abs(offset.Y); i > 0; i--) {
                        for (var x = 0; x < this.MapSize.Width; x++) {
                            for (var layer = (int) Map.Layer.Low; layer <= (int) Map.Layer.Shadow; layer++) {
                                var temp = this.LayerTiles[layer, x, this.MapSize.Height - 1];
                                for (var y = this.MapSize.Height - 1; y > 0; y--) {
                                    this.LayerTiles[layer, x, y] = this.LayerTiles[layer, x, y - 1];
                                }
                                this.LayerTiles[layer, x, 0] = temp;
                            }
                        }
                    }
                }

                //イベントをシフト
                foreach (var ev in this.EVs) {
                    var after = Point.Add(ev.TilePosition, new Size(offset));
                    if (after.X < 0) {
                        after.X += this.MapSize.Width;
                    } else if (this.MapSize.Width <= after.X) {
                        after.X -= this.MapSize.Width;
                    }
                    if (after.Y < 0) {
                        after.Y += this.MapSize.Height;
                    } else if (this.MapSize.Height <= after.Y) {
                        after.Y -= this.MapSize.Height;
                    }
                    ev.TilePosition = after;
                }
            }

            /// <summary>
            /// イベントリストをソート
            /// </summary>
            public void SortEV(int columnIndex) {
                switch (columnIndex) {
                    case (int) Database.DBDefaultColumnIndices.VisibleID:
                        this.EVs.Sort((x, y) => {
                            return x.VisibleID - y.VisibleID;
                        });
                        break;
                    case (int) Database.DBDefaultColumnIndices.Name:
                        this.EVs.Sort((x, y) => {
                            return string.Compare(x.Name, y.Name);
                        });
                        break;
                }
                this.EVListChanged?.Invoke(this, null);
            }

            /// <summary>
            /// イベントを追加
            /// </summary>
            public void AddEV(EventOneData data, int fixedID = -1) {
                this.LoadEV(data);
                if (fixedID == -1) {
                    data.FixedID = this.GenerateFixedEVID(data.FixedID);
                } else {
                    data.FixedID = fixedID;
                }
                this.EVs.Add(data);
                this.EVListChanged?.Invoke(this, null);
            }

            /// <summary>
            /// 指定したインデックスのイベントを置換
            /// </summary>
            public void ReplaceEV(int index, EventOneData data) {
                if (index < 0 || this.EVs.Count <= index) {
                    return;
                }
                this.UnloadEV(this.EVs[index]);
                this.LoadEV(data);
                this.EVs[index] = data;
                this.EVListChanged?.Invoke(this, null);
            }

            /// <summary>
            /// イベントを削除
            /// </summary>
            public void DeleteEV(int index) {
                if (index < 0 || this.EVs.Count <= index) {
                    return;
                }
                this.UnloadEV(this.EVs[index]);
                this.EVs.RemoveAt(index);
                this.EVListChanged?.Invoke(this, null);
            }

            /// <summary>
            /// イベントをすべて削除
            /// </summary>
            public void ClearEV() {
                while (this.EVs.Count > 0) {
                    this.UnloadEV(this.EVs[0]);
                    this.EVs.RemoveAt(0);
                }
                this.EVListChanged?.Invoke(this, null);
            }

            /// <summary>
            /// イベントを指定したタイル座標へ移動させます。
            /// </summary>
            public void MoveEV(int index, Point after) {
                if (index < 0 || this.EVs.Count <= index) {
                    return;
                }
                this.EVs[index].TilePosition = after;
                this.TileEditted?.Invoke(this, null);
            }

            /// <summary>
            /// イベントリソースを読み込みます。
            /// </summary>
            public void LoadEV(EventOneData data) {
                if (data.dxHandle != -1) {
                    //以前のグラフィックを破棄する
                    this.UnloadEV(data);
                } else if (data.Pages.Count == 0) {
                    return;
                }

                //先頭ページのファイルグラフィックのみ読み込む
                if (string.IsNullOrEmpty(data.Pages[0].Graphic) == false
                && data.Pages[0].Graphic.IndexOf(Resources.Split_IDNamePair) == -1) {
                    data.dxHandle = DX.LoadGraph(ProjectManager.ProjectPath + data.Pages[0].Graphic);
                }
            }

            /// <summary>
            /// イベントリソースを破棄します。
            /// このメソッドはイベントリストから削除する前に実行して下さい。
            /// </summary>
            public void UnloadEV(EventOneData data) {
                if (data.dxHandle == -1) {
                    return;
                }
                DX.DeleteGraph(data.dxHandle);
                data.dxHandle = -1;
            }

            /// <summary>
            /// 一意のイベントIDを生成します。
            /// </summary>
            public int GenerateFixedEVID(int start = -1) {
                var id = start;
                var rand = new Random();

                if (id < 0) {
                    //初期値がない場合はここで最初のランダム生成を行う
                    id = rand.Next(0, int.MaxValue);
                }

                bool NG;
                do {
                    NG = false;
                    foreach (var ev in this.EVs) {
                        if (ev.FixedID == id) {
                            NG = true;
                            break;
                        }
                    }
                    if (NG) {
                        id = rand.Next(0, int.MaxValue);
                    }
                } while (NG);

                return id;
            }

            /// <summary>
            /// 一意のイベント可視IDを生成します。
            /// </summary>
            public int GenerateVisibleEVID(int start = 0, int max = 999999) {
                var id = start;

                bool NG;
                do {
                    NG = false;
                    foreach (var ev in this.EVs) {
                        if (ev.VisibleID == id) {
                            NG = true;
                            break;
                        }
                    }
                    if (NG) {
                        id++;
                        if (999999 <= id) {
                            // 可視IDカンスト
                            break;
                        }
                    }
                } while (NG);

                return id;
            }

            /// <summary>
            /// マップのグローバル一意識別子を更新します。
            /// </summary>
            public void SetNewGUID() {
                this.GUID = this.guidGen.ToString("N");
                this.TileEditted?.Invoke(this, null);
            }

            /// <summary>
            /// タイルが変更されたときに発生するイベントを中継します。
            /// </summary>
            public void tileEdittedRelay(object sender, EventArgs e) {
                if (!MapOneData.OutOfEdit) {
                    this.TileEditted?.Invoke(this, null);
                }
            }
        }

    }
}
