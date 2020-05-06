using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBUnit {
        public partial class UnitOneData {

            /// <summary>
            /// ユニットに格納されるエネミーの単体情報
            /// </summary>
            public class EnemyDataInUnit {
                public event EventHandler DataChanged;
                private readonly AllDB dbList;

                public EnemyDataInUnit(int fixedID, Point pos, AllDB DBList) {
                    this.FixedID = fixedID;
                    this.Position = pos;
                    this.dbList = DBList;
                    this.Img = null;
                }

                public int FixedID {
                    get {
                        return this.fixedID;
                    }
                    private set {
                        this.fixedID = value;
                        this.DataChanged?.Invoke(this, null);
                    }
                }
                private int fixedID = -1;

                public Point Position {
                    get {
                        return this.position;
                    }
                    set {
                        this.position = value;
                        this.DataChanged?.Invoke(this, null);
                    }
                }
                private Point position;

                public Size Size {
                    get;
                    private set;
                }

                public string Name {
                    get;
                    private set;
                }

                public Image Img {
                    get;
                    private set;
                }

                /// <summary>
                /// このエネミーの指標です。
                /// </summary>
                public int EnemyLevel {
                    get {
                        return CalcLevel(this.dbList, this.FixedID);
                    }
                }

                /// <summary>
                /// このエネミーの位置を指定分だけ動かします。
                /// </summary>
                /// <param name="pos">移動量</param>
                public void OffsetPosition(Point pos) {
                    this.Position = new Point(this.Position.X + pos.X, this.Position.Y + pos.Y);
                }

                /// <summary>
                /// グラフィック情報をセットします。
                /// </summary>
                public void LoadGraphics() {
                    //エネミーグラフィックを読み込む
                    var enemyDB = this.dbList[Database.DBIndices.Enemy].DBs[0];
                    int index;

                    for (index = 0; index < enemyDB.Rows.Count; index++) {
                        if (enemyDB[(int) Database.DBDefaultColumnIndices.FixedID, index].Value?.ToString() == this.FixedID.ToString()) {
                            break;
                        }
                    }

                    if (index >= enemyDB.Rows.Count) {
                        //エネミーが見つからない
                        this.Name = "無効なエネミー";
                        return;
                    }

                    //エネミー名をセット
                    this.Name = enemyDB[(int) Database.DBDefaultColumnIndices.Name, index].Value?.ToString();

                    //グラフィックファイル名を探す
                    var fileName = "";
                    foreach (DataGridViewColumn col in enemyDB.Columns) {
                        if (col.Tag is DBColumnPictureFile) {
                            fileName = enemyDB[col.Index, index].Tag?.ToString();
                            break;
                        }
                    }

                    //グラフィックをセット
                    if (File.Exists(ProjectManager.ProjectPath + fileName)) {
                        this.Img = Image.FromFile(ProjectManager.ProjectPath + fileName);
                        this.Size = this.Img.Size;
                    } else {
                        this.Img = null;
                        this.Size = Size.Empty;
                    }
                }

                /// <summary>
                /// グラフィック情報を解放します。
                /// </summary>
                public void UnloadGraphics() {
                    this.Img?.Dispose();
                    this.Img = null;
                }

                /// <summary>
                /// 指定したエネミーの強さ指標を算出します。
                /// </summary>
                public static int CalcLevel(AllDB DBList, int enemyFixedID) {
                    var pars = new Dictionary<int, int>();

                    //対象エネミーのパラメーター情報をセットする
                    foreach (DataGridViewRow enemy in DBList[Database.DBIndices.Enemy].DBs[0].Rows) {
                        if (enemy.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value == null
                        || enemyFixedID != int.Parse(enemy.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value?.ToString())) {
                            continue;
                        }

                        //エネミーDBのパラメーター情報を格納している列インデックスを探す
                        var parcol = -1;
                        foreach (DataGridViewColumn col in DBList[Database.DBIndices.Enemy].DBs[0].Columns) {
                            if (col.Tag is DBColumnTextIDValues
                            && ((DBColumnTextIDValues) col.Tag).SrcDBAddress == new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)) {
                                parcol = col.Index;
                                break;
                            }
                        }
                        if (parcol == -1) {
                            //パラメーター列が見つからなかった場合は登録せずに中断
                            break;
                        }

                        //IDと値のペアに絞って登録する
                        var spl = enemy.Cells[parcol].Value?.ToString().Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                        if (spl != null) {
                            foreach (var item in spl) {
                                var subspl = item.Split(Resources.Split_SimpleList.ToCharArray());
                                pars.Add(Database.GetIDFromIDName(subspl[(int) IDValueDataIndices.IDName]), int.Parse(subspl[(int) IDValueDataIndices.Value]));
                            }
                        }
                    }

                    //計算式を取り出す
                    var expression = mgrDBUnit.LevelExpression;
                    expression = expression.Replace(" ", "");       //空白を除去

                    //式中に含まれるパラメーター記号へ実際の値を代入する
                    try {
                        //正規表現ループ開始
                        while (true) {
                            var col = -1;
                            var length = expression.Length;
                            var hitWord = "";
                            var reg = new System.Text.RegularExpressions.Regex(Resources.Regux_ParameterInExpression);
                            var regMatch = reg.Matches(expression);
                            if (regMatch.Count > 0) {
                                col = regMatch[0].Groups[0].Index;
                                length = regMatch[0].Groups[0].Length;
                                hitWord = regMatch[0].Groups["ID"].Value;         //正規表現中に含めたグループ名を使ってID値だけを抽出する
                            }
                            if (col == -1) {
                                break;      //置換完了
                            }

                            //置換実行
                            var value = 0;      //無効なIDに対応する既定値
                            if (int.TryParse(hitWord, out var visibleID) && pars.ContainsKey(visibleID)) {
                                value = pars[visibleID];        //パラメーター値を代入する
                            }
                            expression = expression.Substring(0, col) + value + expression.Substring(col + length);
                        }
                    } catch {
                        //正規表現が不正
                        return 0;
                    }

                    //計算実行
                    try {
                        var dt = new DataTable();
                        var result = dt.Compute(expression, "");
                        if (result is int) {
                            return (int) result;
                        } else if (result is decimal) {
                            return (int) ((decimal) result);
                        } else {
                            //計算結果がおかしい
                            return 0;
                        }
                    } catch {
                        //計算途中でエラーが発生した
                        return 0;
                    }
                }
            }
        }
    }
}
