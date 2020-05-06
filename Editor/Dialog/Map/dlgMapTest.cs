using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Map {
    using Module;
    using Module.DB;
    using Properties;
    using System.Text.RegularExpressions;
    using static Editor.CtrlComponent.Map.mgrMapObject;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, int>;

    public partial class dlgMapTest : Form {
        private readonly AllDB dbList = null;
        private readonly MapOneData mapData = null;
        private const string NotFoundEVIDPrefix = "編集中のマップには存在しません";

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgMapTest(AllDB dbList, MapOneData mapData) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.dbList = dbList;
            this.mapData = mapData;

            //変数種別リストを生成する
            this.cmbValueType.Items.Clear();
            for (var i = 0; i <= (int) Database.DBValueIndices.CommonString + 1; i++) {
                switch ((Database.DBValueIndices) i) {
                    case Database.DBValueIndices.CommonFlag:
                        this.cmbValueType.Items.Add($"{i}:スクリプト共通フラグ");
                        break;
                    case Database.DBValueIndices.CommonInteger:
                        this.cmbValueType.Items.Add($"{i}:スクリプト共通整数");
                        break;
                    case Database.DBValueIndices.CommonString:
                        this.cmbValueType.Items.Add($"{i}:スクリプト共通文字列");
                        break;
                    default:
                        this.cmbValueType.Items.Add($"{i}:イベント個別変数");
                        break;
                }
            }

            //変数リストは暫定的なので無効化
            this.cmbValues.Items.Clear();
            this.cmbValues.Enabled = false;

            //イベントリストを生成
            //イベントリストはイベント個別変数が選択されたときのみ有効なので初期状態では無効化とする
            this.cmbEVs.Items.Clear();
            for (int i = 0; i < mapData.EVCount; i++) {
                this.cmbEVs.Items.Add($"{mapData[i].VisibleID}:{mapData[i].Name}");
            }
            this.cmbEVs.Enabled = false;

            //既存リストを生成する
            var list = new List<ListViewItem>();
            var buf = Settings.Default.Last_MapTestValueData;
            var spl = buf.Split(Resources.Split_SimpleList.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

            foreach (var statement in spl) {
                var add = new ListViewItem();
                buf = statement;

                //FixedIDを取得する
                string fixedID = null;
                int evFixedID = -1;
                string value = "";
                if (statement.Contains("=")) {
                    //スクリプト共通変数
                    fixedID = statement.Substring(2, statement.IndexOf("=") - 2);    //頭２文字以降の文字列から = まで
                    value = Common.CutDoubleQuotation(statement.Substring(statement.IndexOf("=") + 1));
                } else {
                    //イベント個別変数
                    var regux = new Regex(Resources.Requx_SQSelfValueForMapTest);
                    var match = regux.Match(statement);
                    if (!match.Success || match.Groups.Count != 4) {
                        //無効なフォーマット
                        continue;
                    }
                    if (!int.TryParse(match.Groups[1].Value, out evFixedID)) {
                        //無効なフォーマット
                        continue;
                    }
                    fixedID = match.Groups[2].Value;
                    value = match.Groups[3].Value;
                }
                add.Text = fixedID.ToString();

                //種別を判定する
                var subDB = -1;
                var valueTypeIndex = -1;
                var type = buf.Substring(0, 2);
                if (type == Resources.SQ_UserFlag) {
                    subDB = (int) Database.DBValueIndices.CommonFlag;
                    valueTypeIndex = subDB;
                } else if (type == Resources.SQ_UserInteger) {
                    subDB = (int) Database.DBValueIndices.CommonInteger;
                    valueTypeIndex = subDB;
                } else if (type == Resources.SQ_UserString) {
                    subDB = (int) Database.DBValueIndices.CommonString;
                    valueTypeIndex = subDB;
                } else {
                    //イベント個別変数
                    subDB = (int) Database.DBValueIndices.SelfEvent;
                    valueTypeIndex = this.cmbValueType.Items.Count - 1;
                }

                //種別と変数名をセットする
                add.SubItems.Add(this.cmbValueType.Items[valueTypeIndex].ToString());
                add.SubItems.Add(Database.GetIDNameFromFixedID(this.dbList[Database.DBIndices.Value].DBs[subDB], int.Parse(fixedID)));

                //値をセットする
                add.SubItems.Add(value);

                if (evFixedID != -1) {
                    //イベントをセットする
                    add.SubItems.Add(this.getEVVisibleIDNamePairFromFixedID(evFixedID) ?? $"{dlgMapTest.NotFoundEVIDPrefix}:{evFixedID}");
                } else {
                    add.SubItems.Add("");
                }

                list.Add(add);
            }

            //リスト列を設定する
            this.uctlListEditor.SetupList(new string[] { "種別", "変数", "値", "イベント" }, list, true, 1);
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                var buf = "";

                //この文字列はそのままスクリプトコードとなる
                foreach (var item in this.uctlListEditor.Result) {
                    var prefix = "";
                    var value = "";
                    string temp;
                    int i;
                    var OK = true;

                    //種別に応じて値の記法を変える
                    var type = Database.GetIDFromIDName(item.SubItems[1].Text);
                    switch ((Database.DBValueIndices) type) {
                        case Database.DBValueIndices.CommonFlag:
                            prefix = Resources.SQ_UserFlag;
                            temp = item.SubItems[3].Text.ToLower();
                            if (int.TryParse(temp, out i)) {
                                //数値に変換できる場合は 0=false/それ以外=trueとする
                                value = (i != 0) ? "true" : "false";
                            } else {
                                //数値に変換できない場合は true/false の文字列で判定する
                                value = (temp == "true") ? "true" : "false";
                            }
                            break;

                        case Database.DBValueIndices.CommonInteger:
                            prefix = Resources.SQ_UserInteger;
                            if (int.TryParse(item.SubItems[3].Text, out i)) {
                                value = i.ToString();
                            } else {
                                value = "0";
                            }
                            break;

                        case Database.DBValueIndices.CommonString:
                            prefix = Resources.SQ_UserString;
                            value = $"\"{item.SubItems[3].Text}\"";
                            break;

                        default:
                            //イベント個別変数専用の構文を使用
                            prefix = Resources.SQ_EVSelfValue;
                            buf += prefix
                                .Replace("$1", item.SubItems[4].Text.Contains(dlgMapTest.NotFoundEVIDPrefix) ?
                                    //現在のマップに該当する固定IDのイベントが存在していなくても追加する
                                    item.SubItems[4].Text.Substring(item.SubItems[4].Text.IndexOf(":") + 1) :
                                    this.getEVFixedIDFromVisibleIDNamePair(item.SubItems[4].Text).ToString())
                                .Replace("$2", item.Text)
                                .Replace("$3", item.SubItems[3].Text) + ";";
                            OK = false;
                            break;
                    }

                    if (OK) {
                        buf += $"{prefix}{item.Text}={value};";
                    }
                }

                Settings.Default.Last_MapTestValueData = buf;
                return buf;
            }
        }

        /// <summary>
        /// イベントの固定IDから [可視ID：名前] のテキスト形式に変換したものを返します。
        /// ただし、同一の可視IDと同一の名前が重複している場合は、変換後のテキストから必ずしも同一の固定IDに逆変換できるとは限りません。
        /// </summary>
        /// <param name="fixedID">イベントの固定ID</param>
        /// <returns>[可視ID：名前] のテキスト形式に変換したもの。見つからなかった場合は null</returns>
        private string getEVVisibleIDNamePairFromFixedID(int fixedID) {
            for (int i = 0; i < this.mapData.EVCount; i++) {
                var ev = this.mapData[i];
                if (ev.FixedID == fixedID) {
                    return $"{ev.VisibleID}:{ev.Name}";
                }
            }

            return null;
        }

        /// <summary>
        /// [可視ID：名前] のテキスト形式からイベントの固定IDを返します。
        /// ただし、同一の可視IDと同一の名前が重複している場合は正確性に欠けることがあります。
        /// </summary>
        /// <param name="visibleIDNamePair">[可視ID：名前] のテキスト形式を成すイベント情報</param>
        /// <returns>該当するイベントの固定ID。見つからなかった場合は -1</returns>
        private int getEVFixedIDFromVisibleIDNamePair(string visibleIDNamePair) {
            var visibleID = Database.GetIDFromIDName(visibleIDNamePair);
            var name = Database.GetNameFromIDName(visibleIDNamePair);

            for (int i = 0; i < this.mapData.EVCount; i++) {
                var ev = this.mapData[i];
                if (ev.VisibleID == visibleID && ev.Name == name) {
                    return ev.FixedID;
                }
            }

            return -1;
        }

        /// <summary>
        /// リストを選択すると入力ソース欄に反映する
        /// </summary>
        private void uctlListEditor_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            this.cmbValueType.SelectedItem = e.Item.SubItems[1].Text;
            this.cmbEVs.SelectedIndex = -1;

            if (Database.GetIDFromIDName(e.Item.SubItems[1].Text) <= (int) Database.DBValueIndices.CommonString) {
                Database.SelectComboBoxItemFromIDName(
                    this.cmbValues,
                    Database.GetIDNameFromFixedID(
                        this.dbList[Database.DBIndices.Value].DBs[Database.GetIDFromIDName(e.Item.SubItems[1].Text)],
                        int.Parse(e.Item.Text)
                    )
                );
            } else {
                Database.SelectComboBoxItemFromIDName(
                    this.cmbValues,
                    Database.GetIDNameFromFixedID(
                        this.dbList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.SelfEvent],
                        int.Parse(e.Item.Text)
                    )
                );

                //選択イベントを復元
                for (int i = 0; i < this.mapData.EVCount; i++) {
                    if ($"{this.mapData[i].VisibleID}:{this.mapData[i].Name}" == e.Item.SubItems[4].Text) {
                        this.cmbEVs.SelectedIndex = i;
                        break;
                    }
                }

                //変更可能かどうかを更新
                this.uctlListEditor.SetCanApply(
                    this.cmbValues.SelectedIndex != -1 &&
                    this.cmbValueType.SelectedIndex != -1 &&
                    this.cmbEVs.SelectedIndex != -1
                );
            }

            this.txtValue.Text = e.Item.SubItems[3].Text;
        }

        /// <summary>
        /// 情報をセット
        /// </summary>
        private void uctlListEditor_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (this.cmbValueType.SelectedIndex <= (int) Database.DBValueIndices.CommonString) {
                if (this.cmbValues.SelectedIndex == -1 || this.cmbValueType.SelectedIndex == -1) {
                    e.Canceled = true;
                    return;
                }
            } else {
                if (this.cmbValues.SelectedIndex == -1 || this.cmbValueType.SelectedIndex == -1 || this.cmbEVs.SelectedIndex == -1) {
                    e.Canceled = true;
                    return;
                }
            }

            e.Item.Text = ((DBListType) this.cmbValues.SelectedItem).Value.ToString();
            e.Item.SubItems.Add(this.cmbValueType.SelectedItem.ToString());
            e.Item.SubItems.Add(((DBListType) this.cmbValues.SelectedItem).Key);
            e.Item.SubItems.Add(this.txtValue.Text.Replace("\"", ""));      //ダブルクォートは無効とする
            if (this.cmbEVs.Enabled) {
                e.Item.SubItems.Add(this.cmbEVs.SelectedItem.ToString());
            } else {
                e.Item.SubItems.Add("");
            }
        }

        /// <summary>
        /// 入力ソース欄を初期化
        /// </summary>
        private void uctlListEditor_RequestClearListEditorSource(object sender, EventArgs e) {
            this.cmbValues.SelectedIndex = -1;
            this.cmbValueType.SelectedIndex = -1;
            this.cmbEVs.SelectedIndex = -1;
            this.txtValue.Text = "";
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void uctlListEditor_RequestWatchControls(object sender, EventArgs e) {
            if (object.ReferenceEquals(sender, this.cmbValueType)) {
                //種別が変更されたときは変数リストを更新する
                this.cmbValues.Items.Clear();
                this.cmbValues.SelectedIndex = -1;
                this.cmbValues.Enabled = (this.cmbValueType.SelectedIndex != -1);
                this.txtValue.Text = "";

                if (this.cmbValueType.SelectedIndex != -1) {
                    //ソースDBのリストを生成する
                    Common.SetListControlKeyValuePairMode(this.cmbValues);

                    if (this.cmbValueType.SelectedIndex <= (int) Database.DBValueIndices.CommonString) {
                        Database.CreateComboBoxListFromSrcDB(this.cmbValues, this.dbList[Database.DBIndices.Value].DBs[this.cmbValueType.SelectedIndex]);
                        this.cmbEVs.Enabled = false;
                        this.cmbEVs.SelectedIndex = -1;
                    } else {
                        Database.CreateComboBoxListFromSrcDB(this.cmbValues, this.dbList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.SelfEvent]);
                        this.cmbEVs.Enabled = true;
                    }
                }
            }

            if (this.cmbValueType.SelectedIndex <= (int) Database.DBValueIndices.CommonString) {
                this.uctlListEditor.SetCanApply(this.cmbValues.SelectedIndex != -1 && this.cmbValueType.SelectedIndex != -1);
            } else {
                this.uctlListEditor.SetCanApply(
                    this.cmbValues.SelectedIndex != -1 &&
                    this.cmbValueType.SelectedIndex != -1 &&
                    this.cmbEVs.SelectedIndex != -1
                );
            }
        }
    }
}
