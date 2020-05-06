using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace Editor.Module.DB {
    using Properties;
    using AllDB = Dictionary<Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBCombo = KeyValuePair<int, string>;

    /// <summary>
    /// データベース列の抽象クラス
    /// </summary>
    public abstract class DBColumn {
        protected AllDB DBList = null;
        public const int textMaxLength = 65536;          //テキスト列の最大文字数
        public int Width = 100;
        public bool Frozen = false;
        public bool Visible = true;
        public bool ReadOnly = false;
        public string HeaderText = "untitled";
        public object DefaultValue = null;
        public static CtrlComponent.Common.ctlSoundPlayer SoundPlayer = new CtrlComponent.Common.ctlSoundPlayer();

        /// <summary>
        /// 現在の情報をもとにしてDataGridView列を生成します。
        /// </summary>
        /// <returns>DataGridViewColumnオブジェクト</returns>
        public abstract DataGridViewColumn CreateColumnObject();

        /// <summary>
        /// 現在の情報を文字列にします。
        /// </summary>
        /// <returns>CreateColumnObjectメソッドのパラメーターに設定できる文字列</returns>
        public abstract override string ToString();

        /// <summary>
        /// クリックされたときの挙動を定義します。
        /// </summary>
        /// <param name="row">対象行</param>
        public virtual void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
        }

        /// <summary>
        /// 検証時にエラーチェックを行います。
        /// </summary>
        /// <param name="value">対象セルの値</param>
        /// <returns>エラーテキスト</returns>
        public abstract string ErrorCheck(object value);

        /// <summary>
        /// 他のDBに依存するセルに対し、情報を正式に適用します。
        /// </summary>
        /// <param name="rows">当該列が持つ行リスト</param>
        public abstract bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col);

        /// <summary>
        /// ファイルから読み込んだ文字列をもとにセルへデータを格納します。
        /// </summary>
        /// <param name="readBuf">読み込んだ文字列</param>
        /// <param name="destCell">代入先セル</param>
        public abstract void ReadToCell(string readBuf, DataGridViewCell destCell);

        /// <summary>
        /// 現在の値をもとにファイルへ書き出します。
        /// </summary>
        /// <param name="W">StreamWriterオブジェクト</param>
        /// <param name="value">書き込む値</param>
        public abstract void WriteFromCell(StreamWriter W, DataGridViewCell cell);

        /// <summary>
        /// 列の標準設定を施します。
        /// </summary>
        /// <param name="col">対象オブジェクト</param>
        protected void SetColumnStandardSettings(DataGridViewColumn col) {
            col.Tag = this;
            col.Width = this.Width;
            col.HeaderText = this.HeaderText;
            col.Frozen = this.Frozen;
            col.ReadOnly = this.ReadOnly;
            col.Visible = this.Visible;
        }
    }

    /// <summary>
    /// 文字列値
    /// </summary>
    public class DBColumnText : DBColumn {
        public DBColumnText() {
        }

        public DBColumnText(AllDB DBList, string text, int width, bool readOnly, string defVal = "") {
            this.HeaderText = text.Replace(Resources.Split_LongData, "");       //区切り記号は自動で削除する
            this.Width = width;
            this.ReadOnly = readOnly;
            this.DefaultValue = defVal;
            this.DBList = DBList;
        }

        public DBColumnText(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.ReadOnly = bool.Parse(pars[3]);
            this.DefaultValue = pars[4];
            this.DBList = DBList;
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = new DataGridViewTextBoxColumn();
            this.SetColumnStandardSettings(col);
            col.MaxInputLength = textMaxLength;
            col.ValueType = typeof(string);
            col.SortMode = DataGridViewColumnSortMode.Automatic;
            return col;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, this.ReadOnly, this.DefaultValue);
        }

        public override string ErrorCheck(object value) {
            if (value != null) {        //空白は許可する
                if (value?.ToString().Length > textMaxLength) {
                    return $"{textMaxLength} 文字以内で入力して下さい。\r\n";
                } else if (value?.ToString().IndexOf('\t') != -1) {
                    return $"タブ文字を含めることはできません。\r\n";
                }
            }
            return "";
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            return false;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            destCell.Value = readBuf;
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write(cell.Value?.ToString());
        }
    }

    /// <summary>
    /// 単一値
    /// </summary>
    public class DBColumnTextOneNumber : DBColumnText {
        public int MaxValue;
        public int MinValue;

        public DBColumnTextOneNumber() {
        }

        public DBColumnTextOneNumber(AllDB DBList, string text, int width, int min, int max, int defVal) : base(DBList, text, width, false) {
            this.DefaultValue = Math.Min(max, Math.Max(min, defVal));
            this.MaxValue = max;
            this.MinValue = min;
        }

        public DBColumnTextOneNumber(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.MaxValue = int.Parse(pars[3]);
            this.MinValue = int.Parse(pars[4]);
            this.DefaultValue = int.Parse(pars[5]);
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, this.MaxValue, this.MinValue, this.DefaultValue);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = base.CreateColumnObject();
            col.ValueType = typeof(int);
            return col;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            if (int.TryParse(readBuf, out var val)) {
                //整数として変換できる場合は数値として格納する
                destCell.Value = val;
            } else {
                //失敗した場合は空にする
                destCell.Value = null;
            }
        }

        public override string ErrorCheck(object value) {
            if (string.IsNullOrEmpty(value?.ToString()) || !int.TryParse(value.ToString(), out var intValue)) {
                return $"数値を入力して下さい。\r\n";
            } else if (intValue < this.MinValue || this.MaxValue < intValue) {
                return $"値は {this.MinValue}～{this.MaxValue} までの範囲で入力して下さい。\r\n";
            }
            return "";
        }
    }

    /// <summary>
    /// 複数値
    /// </summary>
    public class DBColumnTextNumbers : DBColumnTextOneNumber {

        public DBColumnTextNumbers(AllDB DBList, string text, int width, int min, int max, int[] defVal) : base(DBList, text, width, min, max, 0) {
            if (defVal == null) {
                return;
            }

            //初期値を設定する
            var buf = "";
            for (var i = 0; i < defVal.Length; i++) {
                buf += defVal[i];
                if (i < defVal.Length - 1) {
                    buf += Resources.Split_SimpleList;
                }
            }
            this.DefaultValue = buf;
        }

        public DBColumnTextNumbers(AllDB DBList, string param) : base(DBList, param) {
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = base.CreateColumnObject();
            col.ValueType = typeof(string);
            col.ToolTipText = "「;」で区切ると複数の値を入力できます。";
            col.SortMode = DataGridViewColumnSortMode.NotSortable;
            return col;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            destCell.Value = readBuf;
        }

        public override string ErrorCheck(object value) {
            var eBuf = "";
            if (!string.IsNullOrEmpty(value?.ToString())) {
                var list = value?.ToString().Split(Resources.Split_SimpleList.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                if (list != null) {
                    for (var i = 0; i < list.Count(); i++) {
                        if (!int.TryParse(list[i], out var intValue)) {
                            eBuf += $"{i + 1}番目: 数値を入力して下さい。\r\n";
                        } else if (intValue < this.MinValue || this.MaxValue < intValue) {
                            eBuf += $"{i + 1}番目: 値は {this.MinValue}～{this.MaxValue} までの範囲で入力して下さい。\r\n";
                        }
                    }
                }
            }
            return eBuf;
        }
    }

    /// <summary>
    /// IDと名前のペアリスト
    /// </summary>
    public class DBColumnTextIDNames : DBColumnText {
        public Database.DBAddress SrcDBAddress;

        public DBColumnTextIDNames(AllDB DBList, string text, int width, Database.DBAddress src) : base(DBList, text, width, true) {
            this.SrcDBAddress = src;
        }

        public DBColumnTextIDNames(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.SrcDBAddress = new Database.DBAddress((Database.DBIndices) Enum.Parse(typeof(Database.DBIndices), pars[3]), int.Parse(pars[4]));
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, this.SrcDBAddress.MainID, this.SrcDBAddress.SubID);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = base.CreateColumnObject();
            col.SortMode = DataGridViewColumnSortMode.NotSortable;
            return col;
        }

        public override string ErrorCheck(object value) {
            return "";      //システムがセットするのでエラーチェックを行わない
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            if (this.SrcDBAddress == null) {
                return false;
            }

            var removed = false;

            //FixedIDのリストを [可視ID:名前] の形式で表示する
            //各行を走査する
            foreach (DataGridViewRow destRow in rows) {
                if (destRow.Cells[col.Index].Tag == null) {
                    continue;
                }

                var split = destRow.Cells[col.Index].Tag.ToString().Split(Resources.Split_LongData.ToCharArray());
                var oldValue = destRow.Cells[col.Index].Value.ToString();
                var newValue = "";
                var newTag = "";

                for (var cnt = 0; cnt < split.Length; cnt++) {
                    if (string.IsNullOrEmpty(split[cnt])) {
                        continue;
                    }

                    var idName = Database.GetIDNameFromFixedID(this.DBList[this.SrcDBAddress.MainID].DBs[this.SrcDBAddress.SubID], int.Parse(split[cnt]));
                    if (string.IsNullOrEmpty(idName)) {
                        //削除された項目が残っていたらここで除名する
                        removed = true;
                        continue;
                    }

                    newValue += idName;
                    newValue += Resources.Split_LongData;
                    newTag += split[cnt];
                    newTag += Resources.Split_LongData;
                }

                if (oldValue != newValue || removed) {
                    destRow.Cells[col.Index].Value = Common.CutLastChar(newValue, Resources.Split_LongData.ToCharArray()[0]);
                    destRow.Cells[col.Index].Tag = Common.CutLastChar(newTag, Resources.Split_LongData.ToCharArray()[0]);
                }
            }
            return removed;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            //仮読み込みを行う
            var dest = ((DataGridViewTextBoxCell) destCell);
            dest.Value = "";
            dest.Tag = readBuf;
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write(cell.Tag?.ToString());
        }
    }

    /// <summary>
    /// ID項目と値のペアリスト
    /// </summary>
    public class DBColumnTextIDValues : DBColumnText {
        public Database.DBAddress SrcDBAddress;
        private const int OneDataTagSplitCount = 2;

        public DBColumnTextIDValues(AllDB DBList, string text, int width, Database.DBAddress src) : base(DBList, text, width, true) {
            this.SrcDBAddress = src;
        }

        public DBColumnTextIDValues(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.SrcDBAddress = new Database.DBAddress((Database.DBIndices) Enum.Parse(typeof(Database.DBIndices), pars[3]), int.Parse(pars[4]));
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, this.SrcDBAddress.MainID, this.SrcDBAddress.SubID);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = base.CreateColumnObject();
            col.SortMode = DataGridViewColumnSortMode.NotSortable;
            return col;
        }

        public override string ErrorCheck(object value) {
            return "";      //システムがセットするのでエラーチェックを行わない
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            if (this.SrcDBAddress == null) {
                return false;
            }

            var removed = false;

            //[FixedID;値]のリストを [可視ID:名前;値] の形式で表示する
            //各行を走査する
            foreach (DataGridViewRow destRow in rows) {
                if (destRow.Cells[col.Index].Tag == null) {
                    continue;
                }

                var split = destRow.Cells[col.Index].Tag.ToString().Split(Resources.Split_LongData.ToCharArray());
                var oldValue = destRow.Cells[col.Index].Value.ToString();
                var newValue = "";
                var newTag = "";

                for (var cnt = 0; cnt < split.Length; cnt++) {
                    var subsplit = split[cnt].Split(Resources.Split_SimpleList.ToCharArray());
                    if (subsplit.Length < OneDataTagSplitCount) {
                        continue;
                    }

                    var idName = Database.GetIDNameFromFixedID(this.DBList[this.SrcDBAddress.MainID].DBs[this.SrcDBAddress.SubID], int.Parse(subsplit[0]));
                    if (string.IsNullOrEmpty(idName)) {
                        //削除された項目が残っていたらここで除名する
                        removed = true;
                        continue;
                    }

                    newValue += idName;
                    newValue += Resources.Split_SimpleList + subsplit[1];
                    newValue += Resources.Split_LongData;
                    newTag += split[cnt];
                    newTag += Resources.Split_LongData;
                }

                if (oldValue != newValue || removed) {
                    destRow.Cells[col.Index].Value = Common.CutLastChar(newValue, Resources.Split_LongData.ToCharArray()[0]);
                    destRow.Cells[col.Index].Tag = Common.CutLastChar(newTag, Resources.Split_LongData.ToCharArray()[0]);
                }
            }
            return removed;
        }

        /// <summary>
        /// 仮読み込みを行う
        /// </summary>
        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            var dest = ((DataGridViewTextBoxCell) destCell);
            dest.Value = "";
            dest.Tag = readBuf;
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write(cell.Tag?.ToString());
        }
    }

    /// <summary>
    /// 論理値
    /// </summary>
    public class DBColumnBool : DBColumn {

        public DBColumnBool(AllDB DBList, string text, int width, bool devVal = false) {
            this.HeaderText = text.Replace(Resources.Split_LongData, "");       //区切り記号は自動で削除する;
            this.Width = width;
            this.DefaultValue = devVal;
            this.DBList = DBList;
        }

        public DBColumnBool(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.DefaultValue = bool.Parse(pars[3]);
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, this.DefaultValue);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = new DataGridViewCheckBoxColumn();
            this.SetColumnStandardSettings(col);
            col.TrueValue = Convert.ToInt32(true);
            col.FalseValue = Convert.ToInt32(false);
            col.SortMode = DataGridViewColumnSortMode.Automatic;
            return col;
        }

        public override string ErrorCheck(object value) {
            //ON-OFFの切り替えだけなので無効な入力は発生しない
            return "";
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            return false;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            var dest = ((DataGridViewCheckBoxCell) destCell);
            dest.Value = int.Parse(readBuf);
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write(Convert.ToInt32(cell.Value));
        }
    }

    /// <summary>
    /// リスト列のベース
    /// </summary>
    public abstract class DBColumnList : DBColumn {
        public DBColumnList() {
        }

        public DBColumnList(AllDB DBList, string text, int width) {
            this.HeaderText = text.Replace(Resources.Split_LongData, "");       //区切り記号は自動で削除する;
            this.Width = width;
            this.DBList = DBList;
        }

        public DBColumnList(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = new DataGridViewComboBoxColumn {
                SortMode = DataGridViewColumnSortMode.NotSortable
            };
            this.SetColumnStandardSettings(col);
            return col;
        }

        public override string ErrorCheck(object value) {
            if (value == null) {
                return $"項目を選択して下さい。\r\n";
            }
            return "";
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            return false;
        }

        /// <summary>
        /// 仮読み込みを行う
        /// </summary>
        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            var dest = ((DataGridViewComboBoxCell) destCell);
            dest.Value = int.Parse(readBuf);       //FixedIDの部分だけセットする
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write("");
        }
    }

    /// <summary>
    /// 既存リスト
    /// </summary>
    public class DBColumnListDB : DBColumnList {
        public Database.DBAddress SrcDBAddress;

        public DBColumnListDB(AllDB DBList, string text, int width, Database.DBAddress src) : base(DBList, text, width) {
            this.HeaderText = text.Replace(Resources.Split_LongData, "");       //区切り記号は自動で削除する;
            this.Width = width;
            this.SrcDBAddress = src;
        }

        public DBColumnListDB(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.SrcDBAddress = new Database.DBAddress((Database.DBIndices) Enum.Parse(typeof(Database.DBIndices), pars[3]), int.Parse(pars[4]));
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, this.SrcDBAddress.MainID, this.SrcDBAddress.SubID);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = new DataGridViewComboBoxColumn();
            this.SetColumnStandardSettings(col);
            this.DefaultValue = Database.NotFoundItemID;
            col.SortMode = DataGridViewColumnSortMode.NotSortable;
            col.DisplayMember = "Value";
            col.ValueMember = "Key";
            col.ValueType = typeof(int);

            //この時点では依存先が読み込まれていない可能性があるため、リストの生成は行わない

            return col;
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            if (this.SrcDBAddress == null) {
                return false;
            }

            var cmb = (col as DataGridViewComboBoxColumn);

            //既存リストを生成する
            cmb.Items.Clear();
            cmb.Items.Add(new DBCombo(Database.NotFoundItemID, Resources.DB_NoneItem));       //選択しない場合の項目を用意する
            var srcDB = this.DBList[this.SrcDBAddress.MainID].DBs[this.SrcDBAddress.SubID];
            foreach (DataGridViewRow srcRow in srcDB.Rows) {
                if (srcRow.IsNewRow) {
                    continue;
                }
                cmb.Items.Add(new DBCombo(Database.GetFixedID(srcDB, Database.GetIDNameFromRow(srcRow)), Database.GetIDNameFromRow(srcRow)));
            }

            //項目を再選択する
            foreach (DataGridViewRow destRow in rows) {
                var OK = false;
                foreach (DBCombo item in cmb.Items) {
                    //FixedIDが一致していれば、改めてこのアイテムを選択した状態にする
                    if (destRow.Cells[col.Index].Value != null && (int) destRow.Cells[col.Index].Value == item.Key) {
                        destRow.Cells[col.Index].Value = item.Key;
                        OK = true;
                        break;
                    }
                }
                if (!OK) {
                    //既定で「なし」を選択する
                    destRow.Cells[col.Index].Value = ((DBCombo) cmb.Items[0]).Key;
                }
            }
            return false;
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            //FixedIDを保存する
            W.Write((cell as DataGridViewComboBoxCell).Value?.ToString());
        }
    }

    /// <summary>
    /// 独自リスト
    /// </summary>
    public class DBColumnListUser : DBColumnList {
        public List<string> SrcMyList;

        public DBColumnListUser(AllDB DBList, string text, int width, List<string> src, string defaultValue = null) : base(DBList, text, width) {
            if (src.Count == 0) {
                throw new Exception("独自リストに項目が一つもありません。");
            }
            this.HeaderText = text.Replace(Resources.Split_LongData, "");       //区切り記号は自動で削除する;
            this.Width = width;
            this.SrcMyList = src;
            this.DefaultValue = defaultValue ?? src[0];
        }

        public DBColumnListUser(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);

            var list = pars[3].Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
            this.SrcMyList = new List<string>();
            if (list != null) {
                foreach (var item in list) {
                    this.SrcMyList.Add(item);
                }
            }

            this.DBList = DBList;
        }

        public override string ToString() {
            var flatList = "";
            foreach (var item in this.SrcMyList) {
                flatList += item;
                flatList += "\t";       //リストのアイテムはタブで区切る
            }
            flatList = Common.CutLastChar(flatList, '\t');
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width, flatList);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = new DataGridViewComboBoxColumn {
                SortMode = DataGridViewColumnSortMode.NotSortable
            };
            this.SetColumnStandardSettings(col);
            foreach (var one in this.SrcMyList) {
                col.Items.Add(one);
            }
            return col;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            //項目を選択する
            var dest = ((DataGridViewComboBoxCell) destCell);
            dest.Value = this.SrcMyList[int.Parse(readBuf)];
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            //インデックスを保存する
            var selectedItem = (cell as DataGridViewComboBoxCell).Value?.ToString();
            for (var i = 0; i < this.SrcMyList.Count; i++) {
                if (this.SrcMyList[i] == selectedItem) {
                    W.Write(i);
                    return;
                }
            }
            W.Write(Database.NotFoundItemID);
        }
    }

    /// <summary>
    /// ボタン列のベース
    /// </summary>
    public abstract class DBColumnButton : DBColumn {
        public int DestColumnIndex;
        protected int ButtonWidth = 30;

        public DBColumnButton() {
        }

        public DBColumnButton(AllDB DBList) {
            this.HeaderText = "";
            this.Width = this.ButtonWidth;
            this.DBList = DBList;
        }

        public DBColumnButton(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.HeaderText, this.Width);
        }

        public override DataGridViewColumn CreateColumnObject() {
            return this.CreateButtonColumnObject("");
        }

        protected DataGridViewButtonColumn CreateButtonColumnObject(string toolTipText) {
            var col = new DataGridViewButtonColumn();
            var def = new DataGridViewCellStyle {
                NullValue = "...",
                Alignment = DataGridViewContentAlignment.MiddleCenter
            };
            col.SortMode = DataGridViewColumnSortMode.NotSortable;
            col.DefaultCellStyle = def;
            col.ToolTipText = toolTipText;
            this.SetColumnStandardSettings(col);
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
        }

        /// <summary>
        /// 対象セルが指定した型の入力セルであるかを調べます。
        /// このメソッドは、想定しない列への入力を防止するために使います。
        /// </summary>
        protected bool CheckInputCell(DataGridViewColumnCollection cols, int columnIndex, Type columnType, bool requiredReadOnly) {
            if (columnIndex < 0 || cols.Count <= columnIndex) {
                //列インデックスが範囲外
                return false;
            } else if (cols[columnIndex].Tag.GetType() != columnType) {
                //列型が合わない
                return false;
            } else if (columnType == typeof(DBColumnText) && cols[columnIndex].ReadOnly == false && requiredReadOnly) {
                //ユーザーが自由に文字列を入力できる列はシステムが介入できない
                return false;
            }
            return true;
        }

        public override string ErrorCheck(object value) {
            //ボタンには入力できない
            return "";
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            return false;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write("");
        }
    }

    /// <summary>
    /// スクリプトに移行するボタン
    /// </summary>
    public class DBColumnButtonToScript : DBColumnButton {
        public string DestScriptFile;
        public event Common.RequestOpenFileEventHandler OpenScriptRequested;

        public DBColumnButtonToScript(AllDB DBList, string param) : base(DBList) {
            this.HeaderText = Resources.SQ_ScriptShortName;

            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            if (pars.Length == 1) {
                this.DestScriptFile = param;
            } else {
                this.DestScriptFile = pars[1];
            }
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestScriptFile);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("スクリプトエディターを２ペインで開きます。\r\n自動でID情報がクリップボードに送られます。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value != null && row.Cells.Count >= (int) Database.DBDefaultColumnIndices.Count) {
                var clipBuf = "";

                //FixedID
                if (!Settings.Default.Script_FixedIDHexMode) {
                    //FixedID=10進数モード
                    clipBuf += row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString();
                } else {
                    //FixedID=16進数モード
                    clipBuf += "0x" + int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString()).ToString("X");
                }

                clipBuf += Resources.Code_CommentStart;

                //可変IDは頻繁に変更されるのでスクリプトには表記しない
                //clipBuf += row.Cells[(int)Database.DBDefaultColumnIndices.VisibleID].Value.ToString();
                //clipBuf += Resources.Split_IDNamePair + " ";

                clipBuf += row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value.ToString();
                clipBuf += Resources.Code_CommentEnd;
                Clipboard.SetText(clipBuf);
            }

            //実際に開く処理は委譲する
            this.OpenScriptRequested?.Invoke(this, new RequestOpenFileEventArgs(this.DestScriptFile, MainTabIndices.Database));
        }
    }

    /// <summary>
    /// ファイルを選択するボタン
    /// </summary>
    public class DBColumnButtonSelectFile : DBColumnButton {
        public string FileFilter;
        public string BaseDirectory;

        public DBColumnButtonSelectFile(AllDB DBList, int destIndex, string fileFilter, string baseDir) : base(DBList) {
            this.DestColumnIndex = destIndex;
            this.FileFilter = fileFilter;
            this.BaseDirectory = string.IsNullOrEmpty(baseDir) ? "" : Common.AddToPathSplit(baseDir);
        }

        public DBColumnButtonSelectFile(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
            this.FileFilter = pars[2];
            this.BaseDirectory = pars[3];
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex, this.FileFilter, this.BaseDirectory);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("ファイルを選択するためのダイアログを開きます。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnText), false)) {
                MessageBox.Show("代入先の列にはファイル名を格納できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //代入先が文字列型でなければ処理しない
            }

            var DlgFileSelect = new Dialog.Common.dlgFileSelect(this.BaseDirectory, this.FileFilter, true, false, false, row.Cells[this.DestColumnIndex].Value?.ToString());
            if (DlgFileSelect.ShowDialog() == DialogResult.OK) {
                row.Cells[this.DestColumnIndex].Value = DlgFileSelect.FileName;
            }
        }
    }

    /// <summary>
    /// サウンドを再生するボタン
    /// </summary>
    public class DBColumnButtonPlaySound : DBColumnButton {
        protected new int ButtonWidth = 70;

        public DBColumnButtonPlaySound(AllDB DBList, int destIndex) : base(DBList) {
            this.DestColumnIndex = destIndex;
            this.Width = this.ButtonWidth;
            this.HeaderText = "再生/停止";
        }

        public DBColumnButtonPlaySound(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("サウンドを再生/停止します。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnText), false)) {
                MessageBox.Show("ソース列はファイル名を示しません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //依存先が文字列型でなければ処理しない
            }

            if (SoundPlayer.IsPlaying) {
                DBColumn.SoundPlayer.UnloadMedia();
            } else {
                DBColumn.SoundPlayer.SoundFileName = row.Cells[this.DestColumnIndex].Value?.ToString();
                DBColumn.SoundPlayer.DoPlay();
            }
        }
    }

    /// <summary>
    /// データベースのIDと名前のペアを複数入力するボタン
    /// </summary>
    public class DBColumnButtonInputIDNames : DBColumnButton {
        public Database.DBAddress SrcDBAddress;

        public DBColumnButtonInputIDNames(AllDB DBList, int destIndex, Database.DBAddress srcAddress) : base(DBList) {
            this.DestColumnIndex = destIndex;
            this.SrcDBAddress = srcAddress;
        }

        public DBColumnButtonInputIDNames(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
            this.SrcDBAddress = new Database.DBAddress((Database.DBIndices) Enum.Parse(typeof(Database.DBIndices), pars[2]), int.Parse(pars[3]));
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex, this.SrcDBAddress.MainID, this.SrcDBAddress.SubID);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("データベースを参照し、複数のデータを入力します。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnTextIDNames), true)) {
                MessageBox.Show("代入先の列にはIDデータを格納できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //代入先がIDName型でなければ処理しない
            }

            var DlgIDName = new Dialog.Database.dlgIDValues(this.DBList[this.SrcDBAddress.MainID].DBs[this.SrcDBAddress.SubID], row.Cells[this.DestColumnIndex].Value?.ToString(), false);
            if (DlgIDName.ShowDialog() == DialogResult.OK) {
                row.Cells[this.DestColumnIndex].Value = DlgIDName.Result.Key;
                row.Cells[this.DestColumnIndex].Tag = DlgIDName.Result.Value;       //裏側にFixedIDを隠し込む
            }
        }
    }

    /// <summary>
    /// データベースの項目と値のペアを複数入力するボタン
    /// </summary>
    public class DBColumnButtonInputIDValues : DBColumnButton {
        public Database.DBAddress SrcDBAddress;

        public DBColumnButtonInputIDValues(AllDB DBList, int destIndex, Database.DBAddress srcAddress) : base(DBList) {
            this.DestColumnIndex = destIndex;
            this.SrcDBAddress = srcAddress;
        }

        public DBColumnButtonInputIDValues(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
            this.SrcDBAddress = new Database.DBAddress((Database.DBIndices) Enum.Parse(typeof(Database.DBIndices), pars[2]), int.Parse(pars[3]));
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex, this.SrcDBAddress.MainID, this.SrcDBAddress.SubID);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("データベースの項目と値を一対一で関連付けた複数のデータを入力します。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnTextIDValues), true)) {
                MessageBox.Show("代入先の列にはIDと値のデータを格納できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //代入先が文字列型でなければ処理しない
            }

            var DlgIDVal = new Dialog.Database.dlgIDValues(this.DBList[this.SrcDBAddress.MainID].DBs[this.SrcDBAddress.SubID], row.Cells[this.DestColumnIndex].Value?.ToString(), true);
            if (DlgIDVal.ShowDialog() == DialogResult.OK) {
                row.Cells[this.DestColumnIndex].Value = DlgIDVal.Result.Key;
                row.Cells[this.DestColumnIndex].Tag = DlgIDVal.Result.Value;       //裏側にFixedIDを隠し込む
            }
        }
    }

    /// <summary>
    /// 色情報を入力するボタン
    /// </summary>
    public class DBColumnButtonInputColor : DBColumnButton {
        private static readonly ColorDialog Dlg = new ColorDialog();

        public DBColumnButtonInputColor(AllDB DBList, int destIndex) : base(DBList) {
            this.DestColumnIndex = destIndex;
        }

        public DBColumnButtonInputColor(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("ダイアログから色情報を入力します。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnTextNumbers), true)) {
                MessageBox.Show("代入先の列には色情報を格納できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //代入先が複数値型でなければ処理しない
            }

            var def = Color.Black;
            var buf = row.Cells[this.DestColumnIndex].Value?.ToString();
            var spl = buf.Split(Resources.Split_SimpleList.ToCharArray());
            if (spl.Length == 3) {
                //色の３要素が代入されている場合のみ適用する
                def = Color.FromArgb(int.Parse(spl[0]), int.Parse(spl[1]), int.Parse(spl[2]));
            }
            DBColumnButtonInputColor.Dlg.Color = def;
            DBColumnButtonInputColor.Dlg.AllowFullOpen = true;
            DBColumnButtonInputColor.Dlg.FullOpen = true;
            if (DBColumnButtonInputColor.Dlg.ShowDialog() == DialogResult.OK) {
                row.Cells[this.DestColumnIndex].Value = DBColumnButtonInputColor.Dlg.Color.R + Resources.Split_SimpleList + DBColumnButtonInputColor.Dlg.Color.G + Resources.Split_SimpleList + DBColumnButtonInputColor.Dlg.Color.B;
            }
        }
    }

    /// <summary>
    /// 曲線データを入力するボタン
    /// </summary>
    public class DBColumnButtonInputCurve : DBColumnButton {
        public Database.DBAddress SrcDBAddress;

        public DBColumnButtonInputCurve(AllDB DBList, int destIndex, Database.DBAddress srcAddress) : base(DBList) {
            this.DestColumnIndex = destIndex;
            this.SrcDBAddress = srcAddress;
        }

        public DBColumnButtonInputCurve(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
            this.SrcDBAddress = new Database.DBAddress((Database.DBIndices) Enum.Parse(typeof(Database.DBIndices), pars[2]), int.Parse(pars[3]));
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex, this.SrcDBAddress.MainID, this.SrcDBAddress.SubID);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("曲線を生成します。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnText), true)) {
                MessageBox.Show("代入先の列には曲線データを格納できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //代入先が文字列型でなければ処理しない
            }

            Dialog.Database.dlgEditCurve DlgCurve;
            if (this.SrcDBAddress == null) {
                //単一の曲線を編集する
                DlgCurve = new Dialog.Database.dlgEditCurve(
                    null,
                    int.Parse(this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.MaxLevel, 0].Value.ToString()),
                    row.Cells[this.DestColumnIndex].Value?.ToString()
                );
            } else {
                //データソースの数だけ曲線を編集する
                DlgCurve = new Dialog.Database.dlgEditCurve(
                                        this.DBList[this.SrcDBAddress.MainID].DBs[this.SrcDBAddress.SubID],
                                        int.Parse(this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.MaxLevel, 0].Value.ToString()),
                                        row.Cells[this.DestColumnIndex].Value?.ToString()
                );
            }
            //開けない場合はスルーする
            if (DlgCurve.IsDisposed == false && DlgCurve?.ShowDialog() == DialogResult.OK) {
                row.Cells[this.DestColumnIndex].Value = DlgCurve.Result.Key;
                row.Cells[this.DestColumnIndex].Tag = DlgCurve.Result.Value;       //裏側にFixedIDを隠し込む
            }
        }
    }

    /// <summary>
    /// アドオンを入力するボタン
    /// </summary>
    public class DBColumnButtonInputAddon : DBColumnButton {

        public DBColumnButtonInputAddon(AllDB DBList, int destIndex) : base(DBList) {
            this.DestColumnIndex = destIndex;
        }

        public DBColumnButtonInputAddon(AllDB DBList, string param) : base(DBList) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.DestColumnIndex = int.Parse(pars[1]);
        }

        public override string ToString() {
            return Common.CreateSplittedString(Resources.Split_LongData.ToCharArray()[0], this.GetType().FullName, this.DestColumnIndex);
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = this.CreateButtonColumnObject("アドオンを設定します。");
            return col;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            if (!this.CheckInputCell(cols, this.DestColumnIndex, typeof(DBColumnText), true)) {
                MessageBox.Show("代入先の列にはアドオンデータを格納できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;      //代入先が文字列型でなければ処理しない
            }

            var DlgAddon = new Dialog.Database.dlgAddOn(this.DBList, row.Cells[this.DestColumnIndex].Value?.ToString());
            if (DlgAddon.ShowDialog() == DialogResult.OK) {
                row.Cells[this.DestColumnIndex].Value = DlgAddon.Result;
            }
        }
    }

    /// <summary>
    /// ピクチャー列のベース
    /// </summary>
    public abstract class DBColumnPicture : DBColumn {
        public Rectangle VisibleRange = Rectangle.Empty;

        public DBColumnPicture() {
        }

        public DBColumnPicture(AllDB DBList, string text, int width, Rectangle visibleRange) {
            this.HeaderText = text.Replace(Resources.Split_LongData, "");       //区切り記号は自動で削除する;
            this.Width = width;
            this.VisibleRange = visibleRange;
            this.DBList = DBList;
        }

        public DBColumnPicture(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.VisibleRange = Rectangle.FromLTRB(int.Parse(pars[3]), int.Parse(pars[4]), int.Parse(pars[5]), int.Parse(pars[6]));
            this.DBList = DBList;
        }

        public override string ToString() {
            return Common.CreateSplittedString(
                Resources.Split_LongData.ToCharArray()[0],
                this.GetType().FullName,
                this.HeaderText,
                this.Width,
                this.VisibleRange.Left,
                this.VisibleRange.Top,
                this.VisibleRange.Right,
                this.VisibleRange.Bottom
            );
        }

        public override DataGridViewColumn CreateColumnObject() {
            var col = new DataGridViewImageColumn();
            this.SetColumnStandardSettings(col);
            col.Tag = this;
            col.SortMode = DataGridViewColumnSortMode.NotSortable;
            col.ImageLayout = DataGridViewImageCellLayout.Zoom;
            return col;
        }

        public override string ErrorCheck(object value) {
            //ピクチャーには入力できない
            return "";
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            return false;
        }

        public override void WriteFromCell(StreamWriter W, DataGridViewCell cell) {
            W.Write(cell.Tag?.ToString());
        }

        /// <summary>
        /// 画像をセルに描画します。
        /// </summary>
        protected virtual Bitmap getDrawBitmap(Image img) {
            return null;
        }
    }

    /// <summary>
    /// 画像ファイルを選択するピクチャー
    /// </summary>
    public class DBColumnPictureFile : DBColumnPicture {
        public bool VisibleRangeDependMode;        //表示範囲をシステムDBで設定したものに依存させるかどうか

        public DBColumnPictureFile(AllDB DBList, string text, int width, Rectangle visibleRange, bool dependRange = false) : base(DBList, text, width, visibleRange) {
            this.VisibleRangeDependMode = dependRange;
        }

        public DBColumnPictureFile(AllDB DBList, string param) {
            var pars = param.Split(Resources.Split_LongData.ToCharArray());
            this.HeaderText = pars[1];
            this.Width = int.Parse(pars[2]);
            this.VisibleRange = Rectangle.FromLTRB(int.Parse(pars[3]), int.Parse(pars[4]), int.Parse(pars[5]), int.Parse(pars[6]));
            this.DBList = DBList;
            this.VisibleRangeDependMode = false;
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            //イメージをセットする
            var DlgPictureFile = new Dialog.Common.dlgFileSelect(Resources.Path_Materials, Resources.Extension_Graphics, true, false, false, row.Cells[cell.ColumnIndex]?.Tag?.ToString());
            if (DlgPictureFile.ShowDialog() == DialogResult.OK) {
                ((DataGridViewImageCell) row.Cells[cell.ColumnIndex]).Value = this.getDrawBitmap(Image.FromFile(ProjectManager.ProjectPath + DlgPictureFile.FileName));
                row.Cells[cell.ColumnIndex].Tag = DlgPictureFile.FileName;        //実際に保存されるデータはTagに格納
            }
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            //イメージを読み込んで描画を行う
            var dest = ((DataGridViewImageCell) destCell);
            dest.Tag = readBuf;
            if (File.Exists(ProjectManager.ProjectPath + readBuf)) {
                dest.Value = this.getDrawBitmap(Image.FromFile(ProjectManager.ProjectPath + readBuf));
            }
        }

        protected override Bitmap getDrawBitmap(Image img) {
            Bitmap bmp = null;
            Graphics g = null;
            try {
                if (this.VisibleRange == Rectangle.Empty && !this.VisibleRangeDependMode) {
                    //全域表示: アスペクト比を維持する

                    bmp = new Bitmap(this.Width, (img.Height * this.Width / img.Width) < this.Width ? (img.Height * this.Width / img.Width) : this.Width);
                    g = Graphics.FromImage(bmp);
                    Size size;

                    if (img.Width >= img.Height) {
                        //幅を基準にする
                        size = new Size(bmp.Width, bmp.Width * img.Height / img.Width);
                    } else {
                        //高さを基準にする
                        size = new Size(bmp.Height * img.Width / img.Height, bmp.Height);
                    }

                    g.DrawImage(img, new Rectangle(bmp.Width / 2 - size.Width / 2, bmp.Height / 2 - size.Height / 2, size.Width, size.Height), new Rectangle(0, 0, img.Width, img.Height), GraphicsUnit.Pixel);
                } else {
                    //部分表示

                    if (this.VisibleRangeDependMode) {
                        //表示範囲がシステムDBに依存している場合はデータを持ってくる
                        this.VisibleRange = (Rectangle) (this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.DBCharGraphicsViewRange, 0].Value);
                    }

                    bmp = new Bitmap(this.VisibleRange.Width, this.VisibleRange.Height);
                    g = Graphics.FromImage(bmp);
                    g.DrawImage(img, new Rectangle(0, 0, bmp.Width, bmp.Height), this.VisibleRange, GraphicsUnit.Pixel);
                }
            } catch {
            }
            g.Dispose();
            return bmp;
        }

        /// <summary>
        /// セルの画像を再描画します。
        /// </summary>
        public void Paint(DataGridViewRow row, DataGridViewCell cell) {
            if (File.Exists(ProjectManager.ProjectPath + row.Cells[cell.ColumnIndex].Tag?.ToString())) {
                ((DataGridViewImageCell) row.Cells[cell.ColumnIndex]).Value = this.getDrawBitmap(Image.FromFile(ProjectManager.ProjectPath + row.Cells[cell.ColumnIndex].Tag?.ToString()));
            } else {
                ((DataGridViewImageCell) row.Cells[cell.ColumnIndex]).Value = null;
            }
        }
    }

    /// <summary>
    /// アイコンタイルを選択するピクチャー
    /// </summary>
    public class DBColumnPictureIcon : DBColumnPicture {
        private string beforeIconFileName = "";
        private int beforeIconSize = -1;

        public DBColumnPictureIcon(AllDB DBList, string text, int width, Rectangle visibleRange) : base(DBList, text, width, visibleRange) {
        }

        public override void Click(DataGridViewColumnCollection cols, DataGridViewRow row, DataGridViewCell cell) {
            //イメージをセットする
            if (this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value == null
            || !File.Exists(ProjectManager.ProjectPath + this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value.ToString())) {
                MessageBox.Show(Resources.MsgE_NothingInput.Replace("$", "システムデータベースでアイコンファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            try {
                var DlgTileSelect = new Dialog.Database.dlgSelectIcon(
                                                Image.FromFile(ProjectManager.ProjectPath + this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value.ToString()),
                                                new Size(this.VisibleRange.Width, this.VisibleRange.Height),
                                                Common.StringToPoint(cell.Tag?.ToString())
                );
                if (DlgTileSelect.ShowDialog() == DialogResult.OK) {
                    var result = DlgTileSelect.Result;
                    cell.Value = result.Value;
                    cell.Tag = result.Key.X + Resources.Split_IDNamePair + result.Key.Y;       //実際に保存されるデータはTagに格納
                }
            } catch {
            }
        }

        public override bool RestoreDependColumn(DataGridViewRowCollection rows, DataGridViewColumn col) {
            var isNoImage = false;
            var tileSize = (int) this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconSize, 0].Value;

            if (this.beforeIconFileName == this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value?.ToString()
            && this.beforeIconSize == tileSize) {
                //アイコンに変更がない場合
                return false;
            } else if (this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value == null
            || !File.Exists(ProjectManager.ProjectPath + this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value.ToString())) {
                //イメージファイルが見つからない場合は表示しない
                isNoImage = true;
            }

            this.beforeIconFileName = this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value?.ToString();
            this.beforeIconSize = tileSize;

            try {
                Image img = null;
                if (!isNoImage) {
                    img = Image.FromFile(ProjectManager.ProjectPath + this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value.ToString());
                }

                foreach (DataGridViewRow row in rows) {
                    if (row.Cells[col.Index].Tag == null) {
                        break;
                    }

                    if (!isNoImage) {
                        row.Cells[col.Index].Value = this.getDrawBitmap(img, row.Cells[col.Index].Tag.ToString(), tileSize);
                    } else {
                        row.Cells[col.Index].Value = null;
                    }
                }

                img?.Dispose();
            } catch {
            }
            return false;
        }

        public override void ReadToCell(string readBuf, DataGridViewCell destCell) {
            //仮読み込みを行う
            var dest = ((DataGridViewImageCell) destCell);
            dest.Tag = readBuf;

            if (this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value != null
            && File.Exists(ProjectManager.ProjectPath + this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value.ToString())) {
                //アイコンファイルが設定されている場合はここで読み込む
                var tileSize = (int) this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconSize, 0].Value;
                dest.Value = this.getDrawBitmap(Image.FromFile(ProjectManager.ProjectPath + this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconFileName, 0].Value.ToString()), readBuf, tileSize);
                this.beforeIconSize = tileSize;
            }
        }

        protected Bitmap getDrawBitmap(Image img, string param, int tileSize) {
            var bmp = new Bitmap(this.VisibleRange.Width, this.VisibleRange.Height);
            var g = Graphics.FromImage(bmp);
            try {
                var split = param.Split(Resources.Split_IDNamePair.ToCharArray());
                g.DrawImage(
                    img,
                    this.VisibleRange,
                    new Rectangle(
                        int.Parse(split[(int) XYPointIndices.X]) * tileSize,
                        int.Parse(split[(int) XYPointIndices.Y]) * tileSize,
                        tileSize, tileSize
                    ),
                    GraphicsUnit.Pixel
                );
            } catch {
            }
            g.Dispose();
            return bmp;
        }
    }
}
