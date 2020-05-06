using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    /// <summary>
    /// デザイナータイプのデータベースコントロールの抽象クラス
    /// </summary>
    public partial class ctlDBGUIBase : ctlDBBase {
        public event EventHandler RequestRowAdd;
        public event EventHandler RequestRowCopy;
        public event EventHandler RequestRowDelete;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBGUIBase() {
            this.InitializeComponent();
            this.SupportedRowTools = false;
            this.SupportedStandardEditFlag = false;
        }

        public virtual AllDB DBList {
            get; set;
        }

        /// <summary>
        /// 変更のあった項目のリスト上での背景色
        /// </summary>
        public Color EdittingBackColor {
            get {
                return Color.LightYellow;
            }
        }

        /// <summary>
        /// 保存処理のGUI拡張版
        /// </summary>
        public override bool DoSave() {
            try {
                //代表DBだけを保存する
                using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + this.FileName), Common.SJIS)) {
                    this.DBs[0].EndEdit();
                    Database.WriteDatabase(this.DBs[0], W);
                }
            } catch {
                return false;
            }

            //this.edittingListChanged(this, null);
            this.IsDirty = false;
            return true;
        }

        /// <summary>
        /// 読み込み処理のGUI拡張版
        /// </summary>
        public override void DoLoad() {
            this.ltvDB.SelectedIndices.Clear();
            base.DoLoad();
            this.edittingListChanged(this, null);
        }

        public override void OpenManagedFile(string rPath) {
            var fixedID = Path.GetFileNameWithoutExtension(rPath);

            for (var i = 0; i < this.ltvDB.Items.Count; i++) {
                if (this.ltvDB.Items[i].SubItems[(int) Database.DBDefaultColumnIndices.FixedID].Text == fixedID) {
                    this.ltvDB.SelectedIndices.Add(i);
                    return;
                }
            }
        }

        /// <summary>
        /// 読み込みやインポートの後に別途必要となる処理です。
        /// </summary>
        protected void ApplyToManager<T1, T2>(T1 mgr) where T1 : DBGUIManagerBase<T2> where T2 : DBGUIOneDatabase {
            mgr.Data.Clear();

            foreach (DataGridViewRow row in this.DBs[0].Rows) {
                if (row.IsNewRow) {
                    continue;
                }
                mgr.SetData(row);
                mgr.Data[row.Index].IsDirty = false;
            }

            this.edittingListChanged(this, null);
        }

        /// <summary>
        /// GUIタイプのDBにおける検索処理を行います。
        /// </summary>
        public override void SearchNext(string text) {
            var index = -1;
            var start = 0;
            if (this.ltvDB.SelectedIndices.Count > 0) {
                //次のサブDBを検索
                start = this.ltvDB.SelectedIndices[0] + 1;
            }

            //検索処理
            for (var i = start; i < this.ltvDB.Items.Count; i++) {
                if (this.ltvDB.Items[i].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text.IndexOf(text) != -1) {
                    index = i;
                    break;
                }
            }

            //結果を反映
            if (index == -1) {
                System.Media.SystemSounds.Beep.Play();
                this.ltvDB.SelectedIndices.Clear();
            } else {
                this.ltvDB.SelectedIndices.Add(index);
            }
        }

        /// <summary>
        /// 付属ファイルを伴うインポート処理を行います。
        /// 付属ファイル名はFixedIDに指定の拡張子を付けたものとし、このメソッドを実行する前に予め読み込むべき項目が判っているものとします。
        /// </summary>
        protected DialogResult ImportWithFiles<T1, T2>(string folderPath, string extension, T1 mgr) where T1 : DBGUIManagerBase<T2> where T2 : DBGUIOneDatabase {
            string fullPath;
            string dir;

            //読み込むファイルをユーザーに選択してもらう
            this.dlgImporter.Filter = $"|{Path.GetFileName(this.FileName)}";
            if (this.dlgImporter.ShowDialog() != DialogResult.OK) {
                return DialogResult.Cancel;
            }

            fullPath = this.dlgImporter.FileName;
            dir = Path.GetDirectoryName(fullPath) + "\\";

            //インポート元のファイルを開く
            try {
                using (var R = new StreamReader(Common.OpenFileReadOnly(fullPath), Common.SJIS)) {
                    var startRowIndex = Database.GetRowCount(this.DBs[0]);
                    var changeList = Database.ReadDatabase(this.DBs[0], R, true);

                    //付属ファイルをコピーする
                    for (var i = startRowIndex; i < this.DBs[0].RowCount; i++) {
                        if (this.DBs[0].Rows[i].IsNewRow) {
                            continue;
                        }

                        var fixedID = int.Parse(this.DBs[0].Rows[i].Cells[(int) Database.DBDefaultColumnIndices.FixedID]?.Value.ToString());
                        if (!changeList.ContainsKey(fixedID)) {
                            //そのままコピーする
                            File.Copy(
                                dir + Path.GetFileName(Common.CutLastChar(folderPath, '\\')) + "\\" + fixedID.ToString() + extension,
                                ProjectManager.ProjectPath + folderPath + fixedID.ToString() + extension
                            );
                        } else {
                            //FixedIDが変更されたものは、ファイル名を変更してコピーする
                            File.Copy(
                                dir + Path.GetFileName(Common.CutLastChar(folderPath, '\\')) + "\\" + changeList[fixedID].ToString() + extension,
                                ProjectManager.ProjectPath + folderPath + fixedID.ToString() + extension
                            );
                        }
                    }
                }
            } catch {
                return DialogResult.No;
            }
            this.ApplyToManager<T1, T2>(mgr);

            //付属ファイルを読み直す
            foreach (var one in mgr.Data) {
                one.LoadFile();
            }

            //インポート後にその変更を破棄するとコピーしたファイルがゴミになってしまうため、GUIタイプは必ずインポート後に保存する
            this.DoSave();

            return DialogResult.OK;
        }

        /// <summary>
        /// 付属ファイルを伴うエクスポート処理を行います。
        /// 付属ファイル名はFixedIDに指定の拡張子を付けたものとします。
        /// </summary>
        protected DialogResult ExportWithFiles<T>(string folderPath, string extension, List<T> list) where T : DBGUIOneDatabase {
            var result = base.Export();
            if (result != DialogResult.OK) {
                //処理が行われなかった場合は続行しない
                return result;
            }

            //付属ファイルをエクスポートする
            var dir = Path.GetDirectoryName(this.dlgExporter.FileName);
            dir = dir + "\\" + Path.GetFileName(Common.CutLastChar(folderPath, '\\')) + "\\";
            Directory.CreateDirectory(dir);

            foreach (var data in list) {
                try {
                    File.Copy(ProjectManager.ProjectPath + folderPath + data.FixedID + extension, dir + data.FixedID + extension);
                } catch {
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "付属ファイルのエクスポート") + "\r\n操作を中断します。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return DialogResult.Cancel;
                }
            }

            return DialogResult.OK;
        }

        /// <summary>
        /// 項目リストが変更されたときのイベントハンドラーです。
        /// このメソッドはオーバーライドすることを推奨します。
        /// </summary>
        protected virtual void edittingListChanged(object sender, EventArgs e) {
        }

        /// <summary>
        /// 何かしらの変更があったことを検知したときのイベントハンドラーです。
        /// 内部DBを超えたデータを扱う場合は、ManagerオブジェクトのEdittedイベントにこのメソッドを関連付けて下さい。
        /// </summary>
        protected void managerEditted(object sender, EventArgs e) {
            this.IsDirty = true;
        }

        /// <summary>
        /// ToolStripは継承先でイベント捕捉できないため、ここでイベントを中継します。
        /// </summary>
        private void tolRowNew_Click(object sender, EventArgs e) {
            if (!this.HasError) {
                this.ltvDB.SelectedIndices.Clear();
                this.RequestRowAdd?.Invoke(this, null);
            }
        }
        private void tolRowCopy_Click(object sender, EventArgs e) {
            if (!this.HasError) {
                if (this.ltvDB.SelectedIndices.Count > 0) {
                    this.RequestRowCopy?.Invoke(this, null);
                }
            }
        }
        private void tolRowDelete_Click(object sender, EventArgs e) {
            if (!this.HasError) {
                if (this.ltvDB.SelectedIndices.Count > 0) {
                    if (MessageBox.Show("選択された項目を削除してもよろしいですか？", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.Yes) {
                        this.RequestRowDelete?.Invoke(this, null);
                    }
                }
            }
        }

        /// <summary>
        /// FixedIDをコピーする
        /// </summary>
        private void tolCopyFixedID_Click(object sender, EventArgs e) {
            if (this.ltvDB.SelectedIndices.Count > 0) {
                Clipboard.SetText(this.ltvDB.SelectedItems[0].Text + "/*" + this.ltvDB.SelectedItems[0].SubItems[2].Text + "*/");
            }
        }

        /// <summary>
        /// 項目の選択変更時に行うコントロール有効無効
        /// </summary>
        private void ltvDB_SelectedIndexChanged(object sender, EventArgs e) {
            var isEnabled = (this.ltvDB.SelectedIndices.Count > 0);
            this.pnlEditor.Visible = isEnabled;
            this.tolRowCopy.Enabled = isEnabled;
            this.tolRowDelete.Enabled = isEnabled;
        }
    }
}
