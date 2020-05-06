using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBUserDB {

        /// <summary>
        /// ユーザーデータベース単体情報
        /// </summary>
        public class UserDBOneData : ctlDBGUIBase.DBGUIOneDatabase {

            /// <summary>
            /// コンストラクター
            /// </summary>
            public UserDBOneData(int fixedID, int visibleID, string name, string comment, int colCount, AllDB dbList) : base(fixedID, visibleID, name) {
                this.DBList = dbList;
                this.Comment = comment;
                this.ColumnCount = colCount;
            }

            public AllDB DBList {
                get;
                private set;
            }

            public string FileName {
                get {
                    return Resources.Path_DBs_UserDBs + this.FixedID + Resources.Extension_DB;
                }
            }

            public string Comment {
                get {
                    return this.comment;
                }
                set {
                    this.comment = value;
                    this.IsDirty = true;
                }
            }
            private string comment = "";

            public int ColumnCount {
                get {
                    return this.columnCount;
                }
                set {
                    this.columnCount = value;
                    this.IsDirty = true;
                }
            }
            private int columnCount = 0;

            /// <summary>
            /// 現在編集しているデータベースを書き出します。
            /// </summary>
            public override bool SaveFile(string rPath = null) {
                if (mgrDBUserDB.TargetDB.IsCurrentCellInEditMode) {
                    //セルがコミットされていなかった場合は読み込みを中止する
                    MessageBox.Show("編集中のセルがまだ確定されていないため、保存を実行できません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                string fileName;
                if (string.IsNullOrEmpty(rPath)) {
                    fileName = this.FileName;
                } else {
                    fileName = rPath;
                }

                using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                    //先頭行に列数を格納する
                    W.WriteLine(mgrDBUserDB.TargetDB.Columns.Count);

                    foreach (DataGridViewColumn col in mgrDBUserDB.TargetDB.Columns) {
                        W.WriteLine(col.Tag.ToString());
                    }
                    Database.WriteDatabase(mgrDBUserDB.TargetDB, W);
                }

                //変更履歴に追加
                this.IsDirty = false;
                return true;
            }

            /// <summary>
            /// 指定インデックスのデータを編集用のデータベースとして読み込みます。
            /// </summary>
            public override void LoadFile() {
                if (mgrDBUserDB.TargetDB.IsCurrentCellInEditMode) {
                    //前回のデータベースがコミットされていなかった場合は読み込みを中止する
                    return;
                }

                ctlDBGUIUserDB.flagBySystem = true;
                mgrDBUserDB.TargetDB.Columns.Clear();
                mgrDBUserDB.TargetDB.Rows.Clear();

                var fileName = Resources.Path_DBs_UserDBs + this.FixedID + Resources.Extension_DB;
                var isRemoved = false;

                if (File.Exists(ProjectManager.ProjectPath + fileName)) {
                    string buf;

                    using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + fileName), Common.SJIS)) {
                        buf = R.ReadLine();
                        if (int.Parse(buf) != this.ColumnCount) {
                            this.ColumnCount = int.Parse(buf);      //何らかの不具合などで、列数が不一致になってしまった場合はここで修正する
                        }

                        if (this.ColumnCount > 0) {
                            //テキストから対応する列のインスタンスを生成 -> メタプログラミングの応用！
                            for (var i = 0; i < this.ColumnCount; i++) {
                                buf = R.ReadLine();
                                var spl = buf.Split(Resources.Split_LongData.ToCharArray());
                                Database.AddDBColumn(mgrDBUserDB.TargetDB, ((DBColumn) Activator.CreateInstance(Type.GetType(spl[0]), new object[] { this.DBList, buf })));
                            }

                            //以降：各行のデータ
                            Database.ReadDatabase(mgrDBUserDB.TargetDB, R);

                            //依存列を適用
                            foreach (DataGridViewColumn col in mgrDBUserDB.TargetDB.Columns) {
                                isRemoved |= ((DBColumn) col.Tag).RestoreDependColumn(mgrDBUserDB.TargetDB.Rows, col);
                            }
                        }
                    }
                }
                this.IsDirty = isRemoved;
                ctlDBGUIUserDB.flagBySystem = false;
            }
        }
    }
}
