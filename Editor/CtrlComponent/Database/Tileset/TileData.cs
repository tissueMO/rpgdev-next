using System;
using System.Drawing;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Properties;

    public partial class mgrDBTileset {
        public partial class TilesetOneData {

            /// <summary>
            /// １タイルあたりの情報
            /// </summary>
            public class TileData : ICloneable {
                public event EventHandler DataChanged;
                public const int GroupMax = 99;

                /// <summary>
                /// コンストラクター
                /// </summary>
                public TileData() {
                }
                public TileData(string param) {
                    this.SetData(param);
                }

                public Map.HitType Hit {
                    get {
                        return this.hit;
                    }
                    set {
                        this.hit = value;
                        this.DataChanged?.Invoke(this, null);
                    }
                }
                private Map.HitType hit = Map.HitType.OK;

                public Map.InOutType InOut {
                    get {
                        return this.inOut;
                    }
                    set {
                        this.inOut = value;
                        this.DataChanged?.Invoke(this, null);
                    }
                }
                private Map.InOutType inOut = Map.InOutType.OKBottom | Map.InOutType.OKLeft | Map.InOutType.OKRight | Map.InOutType.OKTop;

                public Map.TileDrawPriority Priority {
                    get {
                        return this.priority;
                    }
                    set {
                        this.priority = value;
                        this.DataChanged?.Invoke(this, null);
                    }
                }
                private Map.TileDrawPriority priority = Map.TileDrawPriority.Normal;

                public int GroupNumber {
                    get {
                        return this.groupNumber;
                    }
                    set {
                        this.groupNumber = value;
                        this.DataChanged?.Invoke(this, null);
                    }
                }
                private int groupNumber = 0;

                public object Clone() {
                    return this.MemberwiseClone() as TileData;
                }

                /// <summary>
                /// 一連の文字列からタイル情報をセットします。
                /// </summary>
                public void SetData(string param) {
                    var spl = param.Split(Resources.Split_SimpleList.ToCharArray());
                    if (spl.Length != Common.GetEnumCount<Map.TileDataType>()) {
                        return;     //データ数が合わない場合は処理しない
                    }

                    this.Hit = (Map.HitType) int.Parse(spl[(int) Map.TileDataType.Hit]);
                    this.InOut = (Map.InOutType) int.Parse(spl[(int) Map.TileDataType.InOut]);
                    this.Priority = (Map.TileDrawPriority) int.Parse(spl[(int) Map.TileDataType.Priority]);
                    this.GroupNumber = int.Parse(spl[(int) Map.TileDataType.GroupNumber]);
                    this.DataChanged?.Invoke(this, null);
                }

                /// <summary>
                /// タイル情報を一連の文字列にして返します。
                /// </summary>
                public string GetDataString() {
                    var buf = "";

                    for (var i = 0; i < Common.GetEnumCount<Map.TileDataType>(); i++) {
                        switch (i) {
                            case (int) Map.TileDataType.Hit:
                                buf += (int) this.Hit;
                                break;
                            case (int) Map.TileDataType.InOut:
                                buf += (int) this.InOut;
                                break;
                            case (int) Map.TileDataType.Priority:
                                buf += (int) this.Priority;
                                break;
                            case (int) Map.TileDataType.GroupNumber:
                                buf += this.GroupNumber;
                                break;
                        }
                        buf += Resources.Split_SimpleList;
                    }

                    buf = Common.CutLastChar(buf, Resources.Split_SimpleList.ToCharArray()[0]);
                    return buf;
                }

                /// <summary>
                /// タイル設定を１段階動かします。
                /// </summary>
                public void Next(Map.TileDataType type, bool reverse) {
                    switch (type) {
                        case Map.TileDataType.Hit:
                            this.Hit = (Map.HitType) Common.LoopNext((int) this.Hit, 0, Common.GetEnumCount<Map.HitType>(), reverse);
                            break;

                        case Map.TileDataType.InOut:
                            this.InOut = (Map.InOutType) Common.LoopNext((int) this.InOut, 0, Common.GetEnumCount<Map.InOutType>(), reverse);
                            break;

                        case Map.TileDataType.Priority:
                            this.Priority = (Map.TileDrawPriority) Common.LoopNext((int) this.Priority, 0, Common.GetEnumCount<Map.TileDrawPriority>(), reverse);
                            break;

                        case Map.TileDataType.GroupNumber:
                            this.GroupNumber = Common.LoopNext(this.GroupNumber, 0, TileData.GroupMax + 1, reverse);
                            break;
                    }
                    this.DataChanged?.Invoke(this, null);
                }

                /// <summary>
                /// 出入設定の可否を切り替えます。
                /// </summary>
                public void TurnInOut(Map.InOutType direction) {
                    if (this.InOut.HasFlag(direction)) {
                        this.InOut &= ~direction;
                    } else {
                        this.InOut |= direction;
                    }
                }

                /// <summary>
                /// タイル上のピクセル座標から出入設定の方向を取得します。
                /// 斜め入力にも対応しています。
                /// </summary>
                public static Map.InOutType GetDirectionFromPoint(Point pixPos) {
                    var result = Map.InOutType.None;
                    if (new Rectangle(0, 0, TileSize, TileSize / 3).Contains(pixPos)) {
                        //上
                        result |= Map.InOutType.OKTop;
                    }
                    if (new Rectangle(0, 0, TileSize / 3, TileSize).Contains(pixPos)) {
                        //左
                        result |= Map.InOutType.OKLeft;
                    }
                    if (new Rectangle(TileSize * 2 / 3, 0, TileSize / 3, TileSize).Contains(pixPos)) {
                        //右
                        result |= Map.InOutType.OKRight;
                    }
                    if (new Rectangle(0, TileSize * 2 / 3, TileSize, TileSize / 3).Contains(pixPos)) {
                        //下
                        result |= Map.InOutType.OKBottom;
                    }
                    if (new Rectangle(TileSize * 1 / 3, TileSize * 1 / 3, TileSize / 3, TileSize / 3).Contains(pixPos)) {
                        //４方向
                        result = Map.InOutType.OKBottom | Map.InOutType.OKLeft | Map.InOutType.OKRight | Map.InOutType.OKTop;
                    }
                    return result;
                }
            }
        }
    }
}
