using DxLibDLL;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;

    public partial class mgrMapObject {
        public partial class MapOneData {

            /// <summary>
            /// イベントの単一データ
            /// </summary>
            public partial class EventOneData : ICloneable {
                /// <summary>
                /// コンストラクター
                /// </summary>
                public EventOneData(int visibleID, string name, Point tilePos) {
                    this.VisibleID = visibleID;
                    this.Name = name;
                    this.TilePosition = tilePos;
                    this.Pages.Add(new EventOnePage());      //既定のページを追加する
                }

                /// <summary>
                /// イベントのFixedID
                /// </summary>
                public int FixedID {
                    get; set;
                } = -1;

                /// <summary>
                /// イベントの可視ID
                /// </summary>
                public int VisibleID {
                    get; set;
                }

                /// <summary>
                /// イベントの名前
                /// </summary>
                public string Name {
                    get; set;
                }

                /// <summary>
                /// イベントのタイル座標
                /// </summary>
                public Point TilePosition {
                    get; set;
                } = Point.Empty;

                /// <summary>
                /// DXライブラリ用のグラフィックハンドル
                /// </summary>
                public int dxHandle {
                    get; set;
                } = -1;

                /// <summary>
                /// イベントページ
                /// </summary>
                public List<EventOnePage> Pages {
                    get; set;
                } = new List<EventOnePage>();

                /// <summary>
                /// イベントのコピーを返します。
                /// </summary>
                public object Clone() {
                    var newObject = this.MemberwiseClone() as EventOneData;
                    newObject.dxHandle = -1;

                    //ページをコピーする
                    newObject.Pages = new List<EventOnePage>();
                    for (var i = 0; i < this.Pages.Count; i++) {
                        newObject.Pages.Add(this.Pages[i].Clone() as EventOnePage);
                    }

                    return newObject;
                }

                /// <summary>
                /// ストリームにイベントを配置するスクリプトコードを書き込みます。
                /// </summary>
                public void DoSaveEVSetFunction(StreamWriter W) {
                    //イベントを配置する関数が先行する
                    W.Write("\t");
                    W.Write(Resources.SQ_EVSetCommand + "(");
                    W.Write(this.FixedID);
                    W.Write(Resources.Split_Argument);
                    W.Write(this.VisibleID);
                    W.Write(Resources.Split_Argument);
                    W.Write(this.TilePosition.X);
                    W.Write(Resources.Split_Argument);
                    W.Write(this.TilePosition.Y);
                    W.Write(Resources.Split_Argument);
                    W.Write("\"" + this.Name.Replace("\\", "\\\\").Replace("\"", "\\\"") + "\"");
                    W.WriteLine(");");

                    //インデントした上で、イベントのページを追加する：引数が多いので数値情報は文字列にまとめる
                    foreach (var page in this.Pages) {
                        W.Write("\t\t");
                        W.Write(Resources.SQ_EVPageSetCommand + "(");
                        W.Write(this.FixedID);
                        W.Write(Resources.Split_Argument);
                        W.Write("\"");
                        W.Write((int) page.Trigger);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write((int) page.MoveType);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write((int) page.Direction);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write((int) page.MoveSpeed);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write((int) page.MoveFrequency);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write((int) page.AnimationPattern);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write((int) page.DrawPriority);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write(page.HitRect.Width);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write(page.HitRect.Height);
                        W.Write(Resources.Split_IDNamePair);
                        W.Write(Convert.ToInt32(page.StoppingAnimation));
                        W.Write(Resources.Split_IDNamePair);
                        W.Write(Convert.ToInt32(page.MovingAnimation));
                        W.Write(Resources.Split_IDNamePair);
                        W.Write(Convert.ToInt32(page.NoHit));
                        W.Write(Resources.Split_IDNamePair);
                        W.Write(Convert.ToInt32(page.FixedDirection));
                        W.Write("\"");
                        W.Write(Resources.Split_Argument);
                        W.Write("\"" + page.Graphic.Replace("\\", "\\\\").Replace("\"", "\\\"") + "\"");
                        W.WriteLine(");");
                    }
                }

                /// <summary>
                /// ストリームにイベント関数を書き込みます。
                /// </summary>
                public void DoSaveEVFunction(StreamWriter W) {
                    W.WriteLine("// " + this.Name);
                    W.Write(Resources.SQ_EVScript
                        .Replace("$", this.FixedID.ToString())
                        .Replace("%1", Resources.SQ_EVScript_TypeID)
                        .Replace("%2", Resources.SQ_EVScript_PageIndex)
                    );
                    W.WriteLine(" {");

                    // 1. ページごとに分岐
                    W.WriteLine("	switch(" + Resources.SQ_EVScript_PageIndex + ") {");
                    for (var p = 0; p < this.Pages.Count; p++) {
                        W.WriteLine($"		case {p}:");
                        W.WriteLine("			switch(" + Resources.SQ_EVScript_TypeID + ") {");

                        // 2. スクリプト種別ごとに分岐
                        for (var type = 0; type < this.Pages[p].SQ.Length; type++) {
                            W.WriteLine($"				case {type}:");

                            //ユーザー定義のテキスト部分開始
                            W.WriteLine(Resources.SQ_SignUserStart);

                            //ユーザー定義のテキスト部分
                            if (!string.IsNullOrEmpty(this.Pages[p].SQ[type])) {
                                W.WriteLine(this.Pages[p].SQ[type]);
                            }

                            //ユーザー定義のテキスト部分終端
                            W.WriteLine(Resources.SQ_SignUserEnd);

                            W.WriteLine("				break;");
                        }
                        W.WriteLine("			}");
                        W.WriteLine("			break;");
                    }
                    W.WriteLine("	}");
                    W.WriteLine("	return TRUE;");
                    W.WriteLine("}");
                    W.WriteLine();
                }

                /// <summary>
                /// ストリームからイベントを配置するスクリプトコードを読み込み、イベントリストを返します。
                /// ストリームの現在位置が１行目がイベントを配置する関数である必要があります。
                /// </summary>
                public static List<EventOneData> DoLoadEVSetFunction(StreamReader R) {
                    int index;
                    var evIndex = -1;
                    var buf = R.ReadLine();
                    var list = new List<EventOneData>();

                    //関数の終わりまで読み込む
                    while (buf != "}") {
                        index = buf.IndexOf(Resources.SQ_EVSetCommand);

                        if (index != -1) {
                            //イベントを配置する関数がヒット
                            buf = buf.Replace("\t", "").Trim().Substring(Resources.SQ_EVSetCommand.Length);
                            var reg = new System.Text.RegularExpressions.Regex(Resources.Regux_ArgumentList);
                            var regMatch = reg.Matches(buf);

                            if (regMatch.Count > 0) {
                                //引数リストだけを抜粋する
                                buf = regMatch[0].Groups["here"].Value;
                                var spl = buf.Split(Resources.Split_Argument.ToCharArray());

                                //イベント名を抜き出す
                                buf = buf.Substring(buf.IndexOf(",\"") + 2);
                                buf = buf.Substring(0, buf.Length - 2).Replace("\\\"", "\"").Replace("\\\\", "\\");

                                var evData = new MapOneData.EventOneData(
                                    int.Parse(spl[1]),
                                    buf,
                                    new Point(int.Parse(spl[2]), int.Parse(spl[3]))
                                );

                                evData.Pages.Clear();
                                evData.FixedID = int.Parse(spl[0]);
                                list.Add(evData);
                                evIndex = list.Count - 1;
                            }

                            buf = R.ReadLine();
                            continue;
                        }

                        index = buf.IndexOf(Resources.SQ_EVPageSetCommand);
                        if (index != -1) {
                            //イベントページを追加する関数がヒット
                            buf = buf.Replace("\t", "").Trim().Substring(Resources.SQ_EVPageSetCommand.Length);
                            var reg = new System.Text.RegularExpressions.Regex(Resources.Regux_ArgumentList);
                            var regMatch = reg.Matches(buf);

                            //引数リストだけを抜粋する
                            buf = regMatch[0].Groups["here"].Value;
                            var spl = buf.Split(Resources.Split_Argument.ToCharArray());
                            var evPage = new MapOneData.EventOneData.EventOnePage();

                            //両端のダブルクォートを排除する
                            for (var i = 0; i < spl.Length; i++) {
                                if (i == 2) {
                                    //グラフィック名のときは末尾の ) を先に取る
                                    spl[i] = spl[i].Substring(0, spl[i].Length - 1);
                                }
                                spl[i] = Common.CutDoubleQuotation(spl[i]);
                            }

                            var sub = spl[1].Split(Resources.Split_IDNamePair.ToCharArray());
                            evPage.Trigger = (Map.EventTriggerType) int.Parse(sub[0]);
                            evPage.MoveType = (Map.EventMoveType) int.Parse(sub[1]);
                            evPage.Direction = (Map.Direction4) int.Parse(sub[2]);
                            evPage.MoveSpeed = (Map.Speed) int.Parse(sub[3]);
                            evPage.MoveFrequency = (Map.Speed) int.Parse(sub[4]);
                            evPage.AnimationPattern = (Map.EventAnimation) int.Parse(sub[5]);
                            evPage.DrawPriority = (Map.EventDrawPriority) int.Parse(sub[6]);
                            evPage.HitRect = new Size(int.Parse(sub[7]), int.Parse(sub[8]));
                            evPage.StoppingAnimation = (int.Parse(sub[9]) == DX.TRUE) ? true : false;
                            evPage.MovingAnimation = (int.Parse(sub[10]) == DX.TRUE) ? true : false;
                            evPage.NoHit = (int.Parse(sub[11]) == DX.TRUE) ? true : false;
                            evPage.FixedDirection = (int.Parse(sub[12]) == DX.TRUE) ? true : false;
                            evPage.Graphic = spl[2].Replace("\\\"", "\"").Replace("\\\\", "\\");

                            list[evIndex].Pages.Add(evPage);
                        }

                        buf = R.ReadLine();
                        continue;
                    }

                    return list;
                }

                /// <summary>
                /// ストリームからイベント関数を読み込みます。
                /// 読み込む箇所が見つからなかった場合はfalseを返します。
                /// </summary>
                public bool DoLoadEVFunction(StreamReader R) {
                    var buf = "";

                    while (!R.EndOfStream) {
                        buf = R.ReadLine();

                        if (string.IsNullOrEmpty(buf) == false && buf.IndexOf(Resources.SQ_Function) == 0) {
                            //イベント関数がヒットしたら、それが自分のスクリプトかどうかを関数名に付けられたFixedIDで調べる
                            var temp = buf.Substring(Resources.SQ_EVScript.IndexOf("$"));
                            temp = temp.Substring(0, temp.IndexOf("("));
                            if (int.Parse(temp) != this.FixedID) {
                                //違うイベントの関数だったらスキップする
                                continue;
                            }

                            //読み込み開始: ページごとに [ページ条件 -> 起動条件 -> 日常移動 -> 実行内容]
                            var pageIndex = 0;
                            var typeIndex = 0;
                            var script = "";

                            while (buf != "}" && pageIndex < this.Pages.Count) {
                                buf = R.ReadLine();

                                //ユーザー入力部分の始点
                                if (buf == Resources.SQ_SignUserStart) {
                                    script = "";

                                    //ユーザー入力部分の終点まで読み込む
                                    buf = R.ReadLine();
                                    while (buf != Resources.SQ_SignUserEnd) {
                                        script += buf;
                                        buf = R.ReadLine();
                                        if (buf != Resources.SQ_SignUserEnd) {
                                            //次の行がある場合は改行する
                                            script += "\r\n";
                                        }
                                    }
                                    this.Pages[pageIndex].SQ[typeIndex] = script;

                                    //次のページへ移動する
                                    typeIndex++;
                                    if (typeIndex >= Common.GetEnumCount<Map.EventScriptType>()) {
                                        typeIndex = 0;
                                        pageIndex++;
                                    }
                                }
                            }

                            //読み込み完了
                            return true;
                        }
                    }

                    //読み込む箇所が見つからなかった
                    return false;
                }
            }

        }
    }
}
