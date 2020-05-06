using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBEffect {

        /// <summary>
        /// エフェクト単体情報
        /// </summary>
        public partial class EffectOneData : ctlDBGUIBase.DBGUIOneDatabase {
            public event DataChangedEventHandler DataChangedEx;
            public event EventHandler DrawerReload;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public EffectOneData(int fixedID, int visibleID, string name, int frameLangth, Database.DBEffectViewPosition dest, AllDB DBList) : base(fixedID, visibleID, name) {
                this.Layers = new List<EffectLayer>();
                this.Orders = new List<EffectOrder>();
                this.FrameLength = frameLangth;
                this.DrawDest = dest;
                this.DBList = DBList;
            }

            /// <summary>
            /// 描画対象の範囲を文字列リストにして返します。
            /// </summary>
            public static string[] GetViewPositionList() {
                var list = new List<string>();
                for (var i = 0; i < Common.GetEnumCount<Database.DBEffectViewPosition>(); i++) {
                    switch (i) {
                        case (int) Database.DBEffectViewPosition.Each:
                            list.Add("対象個別");
                            break;
                        case (int) Database.DBEffectViewPosition.OnlyOne:
                            list.Add("対象一括");
                            break;
                        case (int) Database.DBEffectViewPosition.CenterScreen:
                            list.Add("画面中心");
                            break;
                    }
                }
                return list.ToArray();
            }

            public List<EffectLayer> Layers {
                get; set;
            }

            public List<EffectOrder> Orders {
                get; set;
            }

            public AllDB DBList {
                get; set;
            }

            public string FileName {
                get {
                    return Resources.Path_DBs_EffectScripts + this.FixedID + Resources.Extension_SQ;
                }
            }

            public override int VisibleID {
                get {
                    return this.visibleID;
                }
                set {
                    this.visibleID = value;
                    this.IsDirty = true;
                    this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(true, false, false, false));
                }
            }

            public override string Name {
                get {
                    return this.name;
                }
                set {
                    this.name = value;
                    this.IsDirty = true;
                    this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(true, false, false, false));
                    this.DrawerReload?.Invoke(this, null);
                }
            }

            /// <summary>
            /// フレーム数
            /// </summary>
            public int FrameLength {
                get {
                    return this.frameLength;
                }
                set {
                    if (value <= 0) {
                        throw new Exception("エフェクトのフレーム数を０以下にすることはできません。");
                    }

                    //全レイヤーのフレーム数をリセットする
                    foreach (var layer in this.Layers) {
                        layer.SetFrameLength(value);
                    }

                    this.frameLength = value;
                    this.IsDirty = true;
                    this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(true, false, false, true));
                    this.DrawerReload?.Invoke(this, null);
                }
            }
            private int frameLength;

            /// <summary>
            /// 描画基準位置
            /// </summary>
            public Database.DBEffectViewPosition DrawDest {
                get {
                    return this.drawDest;
                }
                set {
                    this.drawDest = value;
                    this.IsDirty = true;
                    this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(true, false, false, false));
                }
            }
            private Database.DBEffectViewPosition drawDest;

            /// <summary>
            /// 編集レイヤーのインデックス
            /// </summary>
            public int CurrentLayerIndex {
                get; set;
            } = -1;

            /// <summary>
            /// 編集レイヤーのオブジェクト
            /// </summary>
            public EffectLayer CurrentLayer {
                get {
                    if (this.CurrentLayerIndex == -1 || this.Layers.Count == 0) {
                        return null;
                    } else {
                        return this.Layers[this.CurrentLayerIndex];
                    }
                }
            }

            /// <summary>
            /// レイヤーを追加し、そのインデックスを返します。
            /// </summary>
            public int AddLayer(int frameLength, int fixedID, string rPath, Size secSize) {
                this.Layers.Add(new EffectLayer(frameLength, fixedID, rPath, secSize));
                this.Layers[this.Layers.Count - 1].DataChanged += this.dataChangedExRelaySub;
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(false, false, true, false));
                return this.Layers.Count - 1;
            }

            /// <summary>
            /// 指定したインデックスのレイヤーを削除します。
            /// </summary>
            public void DeleteLayer(int index) {
                this.Layers.RemoveAt(index);
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(false, false, true, false));
            }

            /// <summary>
            /// 内部DB用の文字列に従ってレイヤーを構成します。
            /// </summary>
            public void SetupLayers(string param) {
                var spl = param.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                var newLayerList = new List<EffectLayer>();

                if (spl != null) {
                    foreach (var layer in spl) {
                        //既存のレイヤーであるかをチェックする
                        var subspl = layer.Split(Resources.Split_IDNamePair.ToCharArray());
                        var newFlag = true;

                        foreach (var oldLayer in this.Layers) {
                            if (oldLayer.FixedID.ToString() == subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.FixedID]) {
                                //このレイヤーを変更してそのまま使い回すことで、フレーム情報などのデータが失われないようにする
                                oldLayer.FixedID = int.Parse(subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.FixedID]);
                                oldLayer.FileName = subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.FileName];
                                oldLayer.SectionSize = Common.StringToSize(subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.SectionSize]);
                                newLayerList.Add(oldLayer);
                                newFlag = false;
                                break;
                            }
                        }

                        //既存レイヤーでない場合は新規作成
                        if (newFlag) {
                            newLayerList.Add(new EffectLayer(
                                this.FrameLength,
                                int.Parse(subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.FixedID]),
                                subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.FileName],
                                Common.StringToSize(subspl[(int) Dialog.Database.Effect.dlgEditLayers.ListColumnIndices.SectionSize])
                            ));
                            newLayerList[newLayerList.Count - 1].DataChanged += this.dataChangedExRelaySub;
                        }
                    }
                }

                this.Layers = newLayerList;     //新リストに置き換える
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(false, false, true, true));
                this.DrawerReload?.Invoke(this, null);
            }

            public override bool SaveFile(string rPath = null) {
                if (!this.IsDirty) {
                    //変更されていない場合は処理しない
                    return false;
                }

                string fileName;
                if (string.IsNullOrEmpty(rPath)) {
                    fileName = this.FileName;
                } else {
                    fileName = rPath;
                }

                using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                    W.WriteLine(Resources.SQ_Include.Replace("$", $"\"{Resources.Path_DBs_EffectScriptCenter.Replace("\\", "\\\\")}\""));
                    W.WriteLine();
                    W.WriteLine($"// {this.VisibleID}: {this.Name}");
                    W.WriteLine(Resources.SQ_EffectFunc.Replace("$1", Resources.SQ_EffectPlayingHandle).Replace("$2", Resources.SQ_EffectFrameCounter).Replace("$3", Resources.SQ_EffectSameCounter) + " {");
                    W.WriteLine($"	{Resources.SQ_EffectGetData.Replace("$", Resources.SQ_EffectPlayingHandle)}");
                    W.WriteLine($"	switch({Resources.SQ_EffectFrameCounter}) {{");

                    for (var i = -1; i < this.FrameLength; i++) {
                        var buf = $"		case {i}:";

                        if (i == -1) {
                            //レイヤーグラフィック読み込み処理を記述
                            for (var n = 0; n < this.Layers.Count; n++) {
                                buf += Resources.SQ_EffectLoadGraph.Replace("$", $"{n},\"{this.Layers[n].FileName.Replace("\\", "\\\\")}\",{this.Layers[n].SectionSize.Width},{this.Layers[n].SectionSize.Height}");
                            }
                        } else {
                            //各フレームの処理を記述
                            for (var n = 0; n < this.Layers.Count; n++) {
                                var frame = this.Layers[n].Frames[i];
                                buf += Resources.SQ_EffectDraw.Replace("$",
                                    $"{n}," +
                                    $"{frame.ViewPosition.X},{frame.ViewPosition.Y},{frame.SectionPosition.X},{frame.SectionPosition.Y}," +
                                    $"{(int) frame.Blend},{frame.Transparent},{frame.Extend},{frame.Angle}"
                                );
                            }

                            //このフレームに命令がある場合はそれも併せて記述する
                            foreach (var order in this.Orders) {
                                if (order.Frame != i) {
                                    continue;
                                }

                                switch (order.Type) {
                                    case EffectOrder.OrderType.Sound:
                                        buf += Resources.SQ_EffectSound.Replace("$",
                                            $"{order.Options[(int) EffectOrder.SoundOptionIndices.FixedID]}," +
                                            $"{order.Options[(int) EffectOrder.SoundOptionIndices.Volume]}," +
                                            $"{order.Options[(int) EffectOrder.SoundOptionIndices.Pitch]}," +
                                            $"{order.Options[(int) EffectOrder.SoundOptionIndices.Pan]}," +
                                            $"{Resources.SQ_EffectSameCounter}"
                                        );
                                        break;

                                    case EffectOrder.OrderType.Flash:
                                        buf += Resources.SQ_EffectFlash.Replace("$",
                                            $"{Resources.SQ_GetColor.Replace("$", order.Options[(int) EffectOrder.FlashOptionIndices.Color])}," +
                                            $"{order.Options[(int) EffectOrder.FlashOptionIndices.Strength]}," +
                                            $"{order.Options[(int) EffectOrder.FlashOptionIndices.Count]}," +
                                            $"{Resources.SQ_EffectSameCounter}"
                                        );
                                        break;

                                    case EffectOrder.OrderType.Shake:
                                        buf += Resources.SQ_EffectShake.Replace("$",
                                            $"{order.Options[0]}," +
                                            $"{Resources.SQ_EffectSameCounter}"
                                        );
                                        break;

                                    case EffectOrder.OrderType.WaitBreak:
                                        buf += Resources.SQ_EffectWaitBreak;
                                        break;

                                    case EffectOrder.OrderType.User:
                                        buf += Common.CutLastChar(order.Options[0], ';') + ";";
                                        break;
                                }
                            }
                        }

                        buf += Resources.SQ_BlockBreak;
                        W.WriteLine(buf);
                    }

                    W.WriteLine("	}");
                    W.WriteLine("}");
                }

                //変更履歴に追加
                this.IsDirty = false;
                return true;
            }

            public override void LoadFile() {
                var fileName = Resources.Path_DBs_EffectScripts + this.FixedID + Resources.Extension_SQ;
                if (!File.Exists(ProjectManager.ProjectPath + fileName)) {
                    return;       //ファイルが見つからない場合は初期状態
                }

                //読み込み開始
                using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                    var buf = "";
                    R.ReadLine();   //１行目：共通スクリプトインクルード
                    R.ReadLine();   //２行目：空行
                    R.ReadLine();   //３行目：コメント
                    R.ReadLine();   //４行目：関数名
                    R.ReadLine();   //５行目：local Effect = ...
                    R.ReadLine();   //６行目：switch分岐
                    R.ReadLine();   //７行目：case -1: ...

                    //以降読み込み開始
                    buf = R.ReadLine();
                    var i = 0;      //現在のフレームインデックス
                    while (buf.Replace("\t", "").Replace(" ", "") != "}") {
                        buf = buf.Substring(buf.IndexOf(":") + 1);      //case N: の後ろ以降に絞る

                        //文ごとに区切る
                        var spl = buf.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                        if (spl != null) {
                            for (var n = 0; n < spl.Length; n++) {
                                if ((spl[n] + ";") == Resources.SQ_BlockBreak) {
                                    continue;       //break; は無視する
                                }

                                var args = SQ.GetArgumentListByCall(spl[n]);       //引数リストにする
                                if (n < this.Layers.Count) {
                                    //各レイヤーのフレーム情報
                                    this.Layers[int.Parse(args[0])].Frames[i] = new mgrDBEffect.EffectOneData.EffectLayer.EffectFrame(
                                        new Point(int.Parse(args[1]), int.Parse(args[2])),
                                        new Point(int.Parse(args[3]), int.Parse(args[4])),
                                        (Media.BlendMode) int.Parse(args[5]),
                                        int.Parse(args[6]),
                                        int.Parse(args[7]),
                                        int.Parse(args[8])
                                    );
                                } else {
                                    //命令情報
                                    var funcName = SQ.GetFunctionNameByCall(spl[n]);
                                    if (funcName == SQ.GetFunctionNameByCall(Resources.SQ_EffectSound)) {
                                        var pars = new string[Common.GetEnumCount<mgrDBEffect.EffectOneData.EffectOrder.SoundOptionIndices>()];
                                        for (var p = 0; p < pars.Length; p++) {
                                            if (p == (int) mgrDBEffect.EffectOneData.EffectOrder.SoundOptionIndices.IDName) {
                                                //この時点では空白にする
                                                pars[p] = "";
                                            } else {
                                                pars[p] = args[p];
                                            }
                                        }

                                        this.AddOrder(new mgrDBEffect.EffectOneData.EffectOrder(null,
                                            i, mgrDBEffect.EffectOneData.EffectOrder.OrderType.Sound,
                                            pars
                                        ));
                                    } else if (funcName == SQ.GetFunctionNameByCall(Resources.SQ_EffectFlash)) {
                                        var pars = new string[Common.GetEnumCount<mgrDBEffect.EffectOneData.EffectOrder.FlashOptionIndices>()];
                                        for (var p = 0; p < pars.Length; p++) {
                                            if (p == (int) mgrDBEffect.EffectOneData.EffectOrder.FlashOptionIndices.Color) {
                                                pars[p] = Common.LinkStringArray(SQ.GetArgumentListByCall(args[p]), Resources.Split_ColorRGB.ToCharArray()[0]);
                                            } else {
                                                pars[p] = args[p];
                                            }
                                        }

                                        this.AddOrder(new mgrDBEffect.EffectOneData.EffectOrder(null,
                                            i, mgrDBEffect.EffectOneData.EffectOrder.OrderType.Flash,
                                            pars
                                        ));
                                    } else if (funcName == SQ.GetFunctionNameByCall(Resources.SQ_EffectShake)) {
                                        this.AddOrder(new mgrDBEffect.EffectOneData.EffectOrder(null,
                                            i, mgrDBEffect.EffectOneData.EffectOrder.OrderType.Shake,
                                            new string[] { args[0] })
                                        );
                                    } else if (funcName == SQ.GetFunctionNameByCall(Resources.SQ_EffectWaitBreak)) {
                                        this.AddOrder(new mgrDBEffect.EffectOneData.EffectOrder(null,
                                            i, mgrDBEffect.EffectOneData.EffectOrder.OrderType.WaitBreak,
                                            new string[] { "" })
                                        );
                                    } else {
                                        this.AddOrder(new mgrDBEffect.EffectOneData.EffectOrder(null,
                                            i, mgrDBEffect.EffectOneData.EffectOrder.OrderType.User,
                                            new string[] { spl[n] })
                                        );
                                    }
                                }
                            }
                        }

                        buf = R.ReadLine();
                        i++;
                    }
                }
                this.IsDirty = false;
            }

            /// <summary>
            /// レイヤー情報のコピーを返します。
            /// </summary>
            /// <returns></returns>
            public List<EffectLayer> CloneLayers() {
                var list = new List<EffectLayer>();
                foreach (var layer in this.Layers) {
                    var newLayer = new EffectLayer(this.FrameLength, layer.FixedID, layer.FileName, layer.SectionSize);
                    newLayer.DataChanged += this.dataChangedExRelaySub;

                    //レイヤーに含まれるフレーム情報もコピーする
                    for (var i = 0; i < layer.Frames.Length; i++) {
                        newLayer.Frames[i] = ((EffectLayer.EffectFrame) layer.Frames[i].Clone());
                    }

                    list.Add(newLayer);
                }
                return list;
            }

            /// <summary>
            /// 命令情報のコピーを返します。
            /// </summary>
            /// <returns></returns>
            public List<EffectOrder> CloneOrders() {
                var list = new List<EffectOrder>();
                foreach (var order in this.Orders) {
                    list.Add(new EffectOrder(this.DBList, order.Frame, order.Type, ((string[]) order.Options.Clone())));
                }
                return list;
            }

            /// <summary>
            /// 命令を追加します。
            /// </summary>
            public void AddOrder(EffectOrder order) {
                this.Orders.Add(new EffectOrder(this.DBList, order));
                this.sortOrders();
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(false, true, false, false));
            }

            /// <summary>
            /// 命令を削除します。
            /// </summary>
            public void DeleteOrder(int index) {
                this.Orders.RemoveAt(index);
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(false, true, false, false));
            }

            /// <summary>
            /// 指定したインデックスの命令を置き換えます。
            /// </summary>
            public void ReplaceOrder(int index, EffectOrder order) {
                this.Orders[index] = order;
                this.sortOrders();
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(false, true, false, false));
            }

            /// <summary>
            /// 命令リストをフレーム順に並び替えます。
            /// </summary>
            private void sortOrders() {
                this.Orders.Sort((x, y) => {
                    return x.Frame - y.Frame;
                });
            }

            /// <summary>
            /// 編集中のデータが変更されたときに発生するイベントを中継します。
            /// </summary>
            private void dataChangedExRelaySub(object sender, DataChangedEventArgs e) {
                this.IsDirty = true;
                this.DataChangedEx?.Invoke(sender, e);
            }
        }

    }
}
