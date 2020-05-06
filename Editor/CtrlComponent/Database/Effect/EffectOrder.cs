using System.Collections.Generic;

namespace Editor.CtrlComponent.Database {
    using Module;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBEffect {
        public partial class EffectOneData {

            /// <summary>
            /// 命令の単体情報
            /// </summary>
            public class EffectOrder {
                private readonly AllDB dbList;

                /// <summary>
                /// 指定した命令を新規作成
                /// </summary>
                public EffectOrder(AllDB DBList, int frame, OrderType type, string[] opt) {
                    this.dbList = DBList;
                    this.Frame = frame;
                    this.Type = type;
                    this.Options = opt;
                }

                /// <summary>
                /// 仮命令を正式に作成
                /// </summary>
                public EffectOrder(AllDB DBList, EffectOrder order) {
                    this.dbList = DBList;
                    this.Frame = order.Frame;
                    this.Type = order.Type;
                    this.Options = order.Options;
                }

                /// <summary>
                /// 命令の内容一覧
                /// </summary>
                public enum OrderType {
                    Sound,
                    Flash,
                    Shake,
                    WaitBreak,
                    User,
                }

                /// <summary>
                /// 命令の内容を文字列リストにして返します。
                /// </summary>
                public static string[] GetOrderTypeList() {
                    var list = new List<string>();
                    for (var i = 0; i < Common.GetEnumCount<OrderType>(); i++) {
                        switch (i) {
                            case (int) OrderType.Sound:
                                list.Add("効果音再生");
                                break;
                            case (int) OrderType.Shake:
                                list.Add("画面振動");
                                break;
                            case (int) OrderType.Flash:
                                list.Add("フラッシュ");
                                break;
                            case (int) OrderType.WaitBreak:
                                list.Add("再生終了待機を解除");
                                break;
                            case (int) OrderType.User:
                                list.Add("独自コマンド");
                                break;
                        }
                    }
                    return list.ToArray();
                }

                /// <summary>
                /// サウンド命令のパラメーター順序
                /// </summary>
                public enum SoundOptionIndices {
                    FixedID,
                    Volume,
                    Pitch,
                    Pan,
                    IDName,
                }

                /// <summary>
                /// フラッシュ命令のパラメーター順序
                /// </summary>
                public enum FlashOptionIndices {
                    Color,
                    Strength,
                    Count,
                }

                /// <summary>
                /// この命令の内容
                /// </summary>
                public OrderType Type {
                    get {
                        return this.order;
                    }
                    set {
                        //変更される場合のみ処理する
                        if (this.order != value) {
                            //パラメーターを初期化する
                            this.order = value;
                            this.Options = null;
                        }
                    }
                }
                private OrderType order;


                /// <summary>
                /// 命令へのパラメーター
                /// </summary>
                public string[] Options {
                    get; set;
                }

                /// <summary>
                /// 命令が実行されるフレーム
                /// </summary>
                public int Frame {
                    get; set;
                }

                /// <summary>
                /// DXライブラリ用のサウンドハンドル
                /// </summary>
                public int dxHandle = -1;

                /// <summary>
                /// Luminoライブラリ用のサウンドオブジェクト
                /// </summary>
                //public Lumino.Sound SoundObj {
                //	get; set;
                //}
            }

        }
    }
}
