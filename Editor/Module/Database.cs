using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Editor.Module.DB {
    using Properties;
    using DBListType = KeyValuePair<string, int>;

    /// <summary>
    /// データベースの共通機能を提供します。
    /// </summary>
    public static partial class Database {
        public const int NotFoundItemID = -1;       //不明な参照先のFixedID
        public const int UserDBColumnWidth = 180;   //UserDBの列幅
        private const int RowHeight = 26;           //行の高さ

        #region <雑多なコンテンツ>
        /// <summary>
        /// データベースのタブの並び順
        /// </summary>
        public enum DBIndices {
            System,
            Init,
            Char,
            Class,
            Skill,
            Item,
            State,
            Enemy,
            Unit,
            Effect,
            Tileset,
            Material,
            Value,
            Element,
            UserDB,
        }

        /// <summary>
        /// システムDBの登録順序
        /// </summary>
        public enum DBSystemIndices {
            GameTitle,
            GameVersion,
            WindowSize,
            AllowFullScreen,
            EnabledSystemResource,
            NowLoadingFileName,
            IconFileName,
            StandardTransparentColor,
            DrawInterpolation,
            EffectAsyncLoadMode,
            TileSize,
            IconSize,
            MaxPartyCount,
            MaxLevel,
            MaxSaveDataCount,
            EnemyLevelExpression,
            DBCharGraphicsViewRange,
            EffectFPS,
            ShadowTransparent,
            FogTransparent,
            EnabledLimitedDebug,
        }

        /// <summary>
        /// データベースへのアドレス情報を示すためのクラス
        /// </summary>
        public class DBAddress {
            public DBIndices MainID;        //データベースのタブIndexに相当
            public int SubID;               //格納している個々のデータベースを特定する

            /// <summary>
            /// コンストラクター
            /// </summary>
            public DBAddress(DBIndices mainID, int subID = 0) {
                this.MainID = mainID;
                this.SubID = subID;
            }

            public static bool operator ==(DBAddress a, DBAddress b) {
                if (object.ReferenceEquals(a, null) && object.ReferenceEquals(b, null)) {
                    return true;        //どちらもnull
                } else if (object.ReferenceEquals(a, null) || object.ReferenceEquals(b, null)) {
                    return false;       //どちらかがnull
                }
                return (a.MainID == b.MainID && a.SubID == b.SubID);
            }
            public static bool operator !=(DBAddress a, DBAddress b) {
                if (a == null || b == null) {
                    return false;
                }
                return !(a == b);
            }
            public override bool Equals(object obj) {
                if (obj is DBAddress) {
                    return (this == (DBAddress) obj);
                }
                return false;
            }
            public override int GetHashCode() {
                return base.GetHashCode();
            }
        }

        /// <summary>
        /// 曲線データのクラス
        /// </summary>
        public class CurveData {
            public int Base;                //基底値
            public int Index;               //ミリ指数
            public int Coefficient;         //ミリ係数

            /// <summary>
            /// コンストラクター
            /// </summary>
            public CurveData(int baseval = 0, int indexval = 1000, int coefficientval = 1000) {
                this.Base = baseval;
                this.Index = indexval;
                this.Coefficient = coefficientval;
            }
        }

        /// <summary>
        /// アドオン種別のクラス
        /// </summary>
        public class DBAddonType {

            /// <summary>
            /// コンストラクター
            /// </summary>
            public DBAddonType(string name, DBAddress src, bool optValue, bool boolMode) {
                this.Name = name;
                this.SrcDB = src;
                this.OptionValue = optValue;
                this.OptionValueBoolMode = (optValue && boolMode);
            }

            /// <summary>
            /// このアドオンの表示名
            /// </summary>
            public string Name {
                get;
                private set;
            }

            /// <summary>
            /// パラメーター対象となるデータベースの依存先
            /// </summary>
            public DBAddress SrcDB {
                get;
                private set;
            }

            /// <summary>
            /// 値の情報を含むかどうか
            /// </summary>
            public bool OptionValue {
                get;
                private set;
            }

            /// <summary>
            /// 値が論理値であるかどうか
            /// </summary>
            public bool OptionValueBoolMode {
                get;
                private set;
            }
        }
        #endregion

        /// <summary>
        /// 与えられた変数データベースをListViewItemのリストにして返します。
        /// 1列目=型名、2列目=識別子、3列目=変数名、4列目=検出個数
        /// </summary>
        public static List<ListViewItem> CreateValuesListViewItemList(string typeName, string nameFormat, DataGridView db) {
            var list = new List<ListViewItem>();
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow) {
                    continue;
                }

                var item = new ListViewItem {
                    Text = typeName
                };
                item.SubItems.Add(nameFormat.Replace("$", row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value?.ToString()));
                item.SubItems.Add(
                    row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value?.ToString() +
                    ":" +
                    row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value?.ToString()
                );
                item.SubItems.Add("0");
                list.Add(item);
            }
            return list;
        }

        /// <summary>
        /// ファイル名を扱うデータベース項目で、FixedIDからファイル名を探します。
        /// </summary>
        public static string GetFileNameFromFixedID(DataGridView db, int fixedID) {
            //FixedIDが一致する項目を探す
            foreach (DataGridViewRow row in db.Rows) {
                if (row.Cells[(int) DBDefaultColumnIndices.FixedID]?.Value == null) {
                    //無効な項目はスキップ
                    continue;
                }

                if (int.Parse(row.Cells[(int) DBDefaultColumnIndices.FixedID].Value.ToString()) == fixedID) {
                    //ファイル名をセットするボタン列を探し、その一つ前の列をファイル名とみなす
                    for (var i = (int) Database.DBDefaultColumnIndices.Count; i < row.Cells.Count; i++) {
                        var col = db.Columns[i]?.Tag as DBColumn;
                        if (col is DBColumnButtonSelectFile) {
                            return row.Cells[i - 1]?.Value.ToString();
                        }
                    }
                }
            }

            return "";
        }

        /// <summary>
        /// ソースDBのComboBoxリストを生成します。
        /// </summary>
        public static void CreateComboBoxListFromSrcDB(ComboBox cmb, DataGridView srcDB, bool includeNothing = false) {
            //キーと値のペアにして初期化する
            Common.SetListControlKeyValuePairMode(cmb);

            if (srcDB.Columns.Count < (int) Database.DBDefaultColumnIndices.Count) {
                //IDName形式のデータベースでなければ処理しない
                return;
            }

            if (includeNothing) {
                //「なし」を含む場合
                cmb.Items.Add(new DBListType("(なし)", -1));
                cmb.SelectedIndex = 0;
            }

            foreach (DataGridViewRow row in srcDB.Rows) {
                if (row.IsNewRow) {
                    continue;
                }
                cmb.Items.Add(new DBListType(
                    row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value?.ToString() + Resources.Split_IDNamePair + row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value?.ToString(),
                    int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString())
                ));
            }
        }

        /// <summary>
        /// IDと名前のペアから、ソースDBのComboBoxリスト項目を選択します。
        /// </summary>
        public static void SelectComboBoxItemFromIDName(ComboBox cmb, string IDName) {
            foreach (DBListType item in cmb.Items) {
                if (item.Key == IDName) {
                    cmb.SelectedItem = item;
                    return;
                }
            }
        }

        /// <summary>
        /// DataGridViewの行数を返します。
        /// </summary>
        public static int GetRowCount(DataGridView db) {
            if (db.AllowUserToAddRows) {
                //新規行は行数に含めない
                return db.RowCount - 1;
            } else {
                return db.RowCount;
            }
        }

        /// <summary>
        /// DataGridView列の左端にFixedID列を追加します。
        /// </summary>
        public static void AddFixedIDColumn(DataGridView db) {
            var fixedID = new DataGridViewTextBoxColumn {
                ValueType = typeof(int),
                Visible = Settings.Default.DB_VisibleFixedID,
                Frozen = true,
                ReadOnly = true
            };

            //書式設定
            if (Settings.Default.Script_FixedIDHexMode) {
                // 16進数で表示する設定のときは、表示もそれに合わせる
                fixedID.DefaultCellStyle.Format = "X8";
            }

            fixedID.HeaderText = "FixedID";
            fixedID.ToolTipText = "この項目に与えられた一意のIDです。この値は自動で設定され、ユーザーは変更できません。";
            db.Columns.Insert(0, fixedID);
        }

        /// <summary>
        /// すべてのデータベースをセットアップします。
        /// </summary>
        public static void AllDBSetup(List<Control> dbList, CtrlComponent.Database.ctlDatabaseEditor editor) {
            foreach (DataGridView db in dbList) {
                //初期設定
                if (db.AllowUserToAddRows && db.Name != "UDB") {
                    //左端に不可視のFixedID列を生成する
                    Database.AddFixedIDColumn(db);
                }

                //共通イベントハンドラーの関連付け：ユーザーが編集するコントロールに限る
                if (db.Enabled) {
                    db.DataError += editor.dataErrorHandler;
                    db.RowValidating += editor.errorRowCheckHandler;
                    db.RowValidated += editor.errorRowCheckedHandler;
                    db.CellContentClick += editor.clickButtonCell;
                    db.RowsAdded += editor.newRowCreated;
                    db.DefaultValuesNeeded += editor.setDefaultValues;
                    db.SortCompare += editor.sortCompareOnColumnHeader;
                }
            }
        }

        /// <summary>
        /// DataGridViewに統一設定を適用します。
        /// </summary>
        public static void InitDataGridView(DataGridView db) {
            db.Dock = DockStyle.Fill;
            db.SelectionMode = DataGridViewSelectionMode.RowHeaderSelect;
            db.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            db.ClipboardCopyMode = DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            db.RowHeadersWidthSizeMode = DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            db.RowHeadersWidth = 40;
            db.RowTemplate.Height = Database.RowHeight;
            db.RowTemplate.MinimumHeight = Database.RowHeight;
            db.ShowCellErrors = true;
            db.ShowCellToolTips = true;
            db.ShowRowErrors = true;
            db.AllowUserToAddRows = true;
            db.AllowUserToDeleteRows = true;
            db.AllowUserToResizeRows = true;
            db.AllowUserToOrderColumns = false;
            db.ShowCellErrors = true;
            db.ShowRowErrors = true;
            db.ShowCellToolTips = true;
            db.ShowEditingIcon = true;
            db.MultiSelect = false;
        }

        /// <summary>
        /// 新規行のFixedIDを生成します。
        /// 初期値を与えると、重複がある場合のみ変更されて返されます。
        /// </summary>
        public static int GenerateFixedRowID(DataGridView db, int defaultID = -1) {
            int id;
            bool NG;
            var rand = new Random();

            //初期値を与える
            if (defaultID == -1) {
                id = rand.Next(0, int.MaxValue);
            } else {
                id = defaultID;
            }

            //重複しない値をランダムに生成する
            while (true) {
                NG = false;
                foreach (DataGridViewRow row in db.Rows) {
                    if (row.IsNewRow == false && row.Cells[0].Value != null && int.Parse(row.Cells[0].Value?.ToString()) == id) {
                        NG = true;
                        break;
                    }
                }
                if (!NG) {
                    break;
                }
                id = rand.Next(0, int.MaxValue);
            }

            return id;
        }

        /// <summary>
        /// 指定行の項目を [可視ID：名前] のテキスト形式で返します。
        /// </summary>
        /// <param name="row">対象行オブジェクト</param>
        public static string GetIDNameFromRow(DataGridViewRow row) {
            var buf = "";
            if (row.Cells.Count >= (int) DBDefaultColumnIndices.Count) {
                buf += row.Cells[(int) DBDefaultColumnIndices.VisibleID].Value != null ? row.Cells[(int) DBDefaultColumnIndices.VisibleID].Value.ToString() : $"{NotFoundItemID}";
                buf += Resources.Split_IDNamePair;
                buf += row.Cells[(int) DBDefaultColumnIndices.Name].Value != null ? row.Cells[(int) DBDefaultColumnIndices.Name].Value.ToString() : "";
            }
            return buf;
        }

        /// <summary>
        /// [可視ID：名前] のテキスト形式から該当する行のFixedIDを返します。
        /// 見つからなかった場合は -1 を返します。
        /// </summary>
        /// <param name="db">項目が含まれる対象オブジェクト</param>
        public static int GetFixedID(DataGridView db, string IDName) {
            var split = IDName.Split(Resources.Split_IDNamePair.ToCharArray());

            //不正な形式
            if (split.Length < Common.GetEnumCount<IDNameDataIndices>()) {
                return NotFoundItemID;
            }

            //探索開始
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow) {
                    continue;
                }
                if (row.Cells[(int) DBDefaultColumnIndices.VisibleID].Value.ToString() == split[(int) IDNameDataIndices.ID]
                && row.Cells[(int) DBDefaultColumnIndices.Name].Value.ToString() == split[(int) IDNameDataIndices.Name]) {
                    return int.Parse(row.Cells[(int) DBDefaultColumnIndices.FixedID].Value.ToString());
                }
            }
            return NotFoundItemID;
        }

        /// <summary>
        /// [可視ID：名前] のテキスト形式から該当する行の FixedID/*名前*/ の文字列を返します。
        /// 見つからなかった場合は -1/*なし*/ を返します。
        /// </summary>
        /// <param name="db">項目が含まれる対象オブジェクト</param>
        public static string GetFixedIDWithCommentName(DataGridView db, string IDName) {
            var split = IDName.Split(Resources.Split_IDNamePair.ToCharArray());

            //不正な形式
            if (split.Length < Common.GetEnumCount<IDNameDataIndices>()) {
                return "-1/*なし*/";
            }

            //探索開始
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow) {
                    continue;
                }
                if (row.Cells[(int) DBDefaultColumnIndices.VisibleID].Value.ToString() == split[(int) IDNameDataIndices.ID]
                && row.Cells[(int) DBDefaultColumnIndices.Name].Value.ToString() == split[(int) IDNameDataIndices.Name]) {
                    return row.Cells[(int) DBDefaultColumnIndices.FixedID].Value.ToString() + "/*" + row.Cells[(int) DBDefaultColumnIndices.Name].Value.ToString() + "*/";
                }
            }
            return "-1/*なし*/";
        }

        /// <summary>
        /// [可視ID：名前] のテキスト形式から可視ID部分を返します。
        /// </summary>
        public static int GetIDFromIDName(string IDName) {
            var split = IDName.Split(Resources.Split_IDNamePair.ToCharArray());

            //不正な形式
            if (split.Length < Common.GetEnumCount<IDNameDataIndices>()) {
                return NotFoundItemID;
            }
            return int.Parse(split[(int) IDNameDataIndices.ID]);
        }

        /// <summary>
        /// [可視ID：名前] のテキスト形式から名前部分を返します。
        /// </summary>
        public static string GetNameFromIDName(string IDName) {
            var split = IDName.Split(Resources.Split_IDNamePair.ToCharArray());

            //不正な形式
            if (split.Length < Common.GetEnumCount<IDNameDataIndices>()) {
                return "";
            }
            return split[(int) IDNameDataIndices.Name];
        }

        /// <summary>
        /// FixedIDから [可視ID：名前] のテキスト形式を返します。
        /// 見つからなかった場合は 空の文字列 を返します。
        /// </summary>
        /// <param name="db">対象オブジェクト</param>
        public static string GetIDNameFromFixedID(DataGridView db, int fixedID) {
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow) {
                    continue;
                }

                if (row.Cells[(int) DBDefaultColumnIndices.FixedID].Value?.ToString() == fixedID.ToString()) {
                    var buf = "";
                    buf += row.Cells[(int) DBDefaultColumnIndices.VisibleID].Value.ToString() ?? $"{NotFoundItemID}";
                    buf += Resources.Split_IDNamePair;
                    buf += row.Cells[(int) DBDefaultColumnIndices.Name].Value.ToString() ?? "";
                    return buf;
                }
            }

            return "";
        }

        /// <summary>
        /// リスト列のセルで選択されている項目のインデックスを返します。
        /// 見つからなかった場合は -1 を返します。
        /// </summary>
        /// <param name="cell">対象セル</param>
        /// <returns>選択インデックス</returns>
        public static int GetIndexListCell(DataGridView db, DataGridViewComboBoxCell cell) {
            var list = (db.Columns[cell.ColumnIndex] as DataGridViewComboBoxColumn).Items;
            for (var i = 0; i < list.Count; i++) {
                if (list[i].ToString() == cell.Value?.ToString()) {
                    return i;
                }
            }
            return NotFoundItemID;
        }

        /// <summary>
        /// リスト列のセルで指定したインデックスの項目を選択します。
        /// </summary>
        /// <param name="cell">対象セル</param>
        /// <param name="index">選択するインデックス</param>
        public static void SelectIndexListCell(DataGridView db, DataGridViewComboBoxCell cell, int index) {
            var list = (db.Columns[cell.ColumnIndex] as DataGridViewComboBoxColumn).Items;
            if (0 <= index && index < list.Count) {
                cell.Value = list[index];
            }
        }

        /// <summary>
        /// 対象データベース内にエラーが含まれるかどうかを検証します。
        /// </summary>
        public static bool HasError(DataGridView db) {
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow) {
                    continue;
                }
                if (!string.IsNullOrEmpty(row.ErrorText)) {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 対象データベースに列を追加します。
        /// </summary>
        public static void AddDBColumn(DataGridView db, DBColumn obj) {
            var col = obj.CreateColumnObject();
            col.DefaultCellStyle.WrapMode = DataGridViewTriState.True;
            db.Columns.Add(col);

            //各セルに初期値を入れる
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow) {
                    continue;
                }
                row.Cells[db.ColumnCount - 1].Value = obj.DefaultValue;
            }
        }

        /// <summary>
        /// ストリームにデータベースを書き込みます。
        /// </summary>
        /// <param name="db">対象のオブジェクト</param>
        /// <param name="w">書き込みストリームのオブジェクト</param>
        public static void WriteDatabase(DataGridView db, StreamWriter w) {
            //１行目：列の型名
            for (var i = 0; i < db.Columns.Count; i++) {
                if (i > 0) {
                    w.Write("\t");
                }
                if (db.Columns[i].Tag != null) {
                    w.Write(db.Columns[i].Tag.GetType().Name);
                } else {
                    w.Write("FixedID");
                }
            }
            w.WriteLine();

            //２行目：列の表示名
            for (var i = 0; i < db.Columns.Count; i++) {
                if (i > 0) {
                    w.Write("\t");
                }
                w.Write(db.Columns[i].HeaderText);
            }
            w.WriteLine();

            //以降、各行・セル情報
            foreach (DataGridViewRow row in db.Rows) {
                if (row.IsNewRow || !string.IsNullOrEmpty(row.ErrorText)) {
                    //新しい行 or エラー行 は無視する
                    continue;
                }

                //列ごとに定義された書き込み方法を適用する
                foreach (DataGridViewCell cell in row.Cells) {
                    var col = db.Columns[cell.ColumnIndex]?.Tag as DBColumn;
                    if (col == null) {
                        //FixedID列
                        w.Write(cell.Value?.ToString());
                    } else {
                        col.WriteFromCell(w, cell);
                    }
                    if (cell.ColumnIndex < row.Cells.Count - 1) {
                        w.Write("\t");
                    }
                }
                w.WriteLine();
            }
            w.WriteLine();
        }

        /// <summary>
        /// ストリームからデータベースを読み込みます。
        /// このメソッドは、一つにまとめられた複数のデータベースを順次読み込む場合に使います。
        /// この時点では、他のデータベースに依存する項目が正しく設定されません。
        /// </summary>
        /// <param name="db">対象のオブジェクト</param>
        /// <param name="r">読み込みストリームのオブジェクト</param>
        /// <param name="addMode">追加的に読み込むかどうか</param>
        public static Dictionary<int, int> ReadDatabase(DataGridView db, StreamReader r, bool addMode = false) {
            var renameIDList = new Dictionary<int, int>();        //FixedIDを変更した項目のリスト / Key=After, Value=Before

            if (!addMode) {
                db.Rows.Clear();
            }
            r.ReadLine();               //列：型名を読み捨てる
            r.ReadLine();               //列：表示名を読み捨てる

            //空行が出るまで読み込む
            var buf = r.ReadLine();
            while (!string.IsNullOrEmpty(buf)) {
                db.Rows.Add(new DataGridViewRow());
                DataGridViewRow newRow;
                if (db.AllowUserToAddRows) {
                    newRow = db.Rows[db.RowCount - 2];      //末尾の行は新規行のため、その一つ前を指すようにする
                } else {
                    newRow = db.Rows[db.RowCount - 1];      //こちらには新規行はない
                }
                newRow.Height = Database.RowHeight;
                newRow.MinimumHeight = Database.RowHeight;
                var cells = buf.Split('\t');
                for (var i = 0; i < db.Columns.Count; i++) {
                    var col = db.Columns[i]?.Tag as DBColumn;
                    if (col == null) {
                        //FixedID列
                        var oldID = int.Parse(cells[i]);
                        var newID = int.Parse(cells[i]);

                        if (addMode) {
                            //追加モードのときはFixedIDの重複をチェックし、自動で修正する
                            newID = Database.GenerateFixedRowID(db, newID);
                            if (newID != int.Parse(cells[i])) {
                                renameIDList.Add(newID, oldID);
                            }
                        }

                        newRow.Cells[i].Value = newID;
                    } else {
                        col.ReadToCell(cells[i], newRow.Cells[i]);
                    }
                }
                buf = r.ReadLine();
            }

            return renameIDList;
        }
    }
}
