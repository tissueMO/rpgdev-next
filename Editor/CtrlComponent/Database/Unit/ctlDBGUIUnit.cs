using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlDBGUIUnit : Editor.CtrlComponent.Database.ctlDBGUIBase {
        public mgrDBUnit mgr;
        private DrugObject drugObj = null;

        /// <summary>
        /// このコントロールで使われるListViewの列順序
        /// </summary>
        public enum UnitColumns {
            FixedID,
            VisibleID,
            Name,
            Level,
            Position,
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBGUIUnit() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.DBs = new DataGridView[] { this.CreateDefaultDataGridView() };
            this.tolScale.Items.AddRange(ViewScale.GetNames());
        }

        /// <summary>
        /// コンストラクターの次に呼び出されます。
        /// </summary>
        private void ctlDBGUIUnit_Load(object sender, EventArgs e) {
            this.tolScale.SelectedIndex = (Settings.Default.DB_UnitScale == -1) ? (int) ViewScale.ScaleRate.Normal : Settings.Default.DB_UnitScale;
        }

        public override AllDB DBList {
            get {
                return this.dbList;
            }
            set {
                this.dbList = value;
                this.mgr = new mgrDBUnit(this.dbList, this.DBs[0]);
                this.mgr.ListChanged += this.edittingListChanged;
                this.mgr.DataChanged += this.edittingDataChanged;
                this.mgr.Editted += this.managerEditted;
            }
        }
        private AllDB dbList;

        public override bool DoSave() {
            //すべて変更フラグを解除する
            foreach (var unit in this.mgr.Data) {
                unit.IsDirty = false;
            }

            //リストの背景を初期化する
            this.edittingListChanged(null, null);

            return base.DoSave();
        }

        public override void DoLoad() {
            //すべて変更フラグを解除する
            foreach (var unit in this.mgr.Data) {
                unit.IsDirty = false;
            }

            base.DoLoad();      //既定の処理
            this.ApplyToManager<mgrDBUnit, mgrDBUnit.UnitOneData>(this.mgr);
        }

        public override DialogResult Import() {
            var result = base.Import();      //既定の処理
            if (result == DialogResult.OK) {
                this.ApplyToManager<mgrDBUnit, mgrDBUnit.UnitOneData>(this.mgr);
                this.IsDirty = true;
            }
            return result;
        }

        public override void SrcReset(AllDB DBList) {
            this.ltvDB.SelectedIndices.Clear();

            //デザイナーを更新
            var wndSize = (Size) DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.WindowSize, 0].Value;
            mgrDBUnit.WindowSize = wndSize;
            mgrDBUnit.LevelExpression = DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.EnemyLevelExpression, 0].Value?.ToString() ?? "";
            this.tolScale_SelectedIndexChanged(this, null);

            //エネミーリストを更新
            this.ltvEnemyList.BeginUpdate();
            this.ltvEnemyList.SelectedIndices.Clear();
            this.ltvEnemyList.Items.Clear();
            foreach (DataGridViewRow row in DBList[Database.DBIndices.Enemy].DBs[0].Rows) {
                if (row.IsNewRow) {
                    continue;
                }

                var newItem = new ListViewItem {
                    Text = row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value?.ToString()
                };
                newItem.SubItems.Add(row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value?.ToString());
                newItem.SubItems.Add(row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value?.ToString());
                newItem.SubItems.Add(mgrDBUnit.UnitOneData.EnemyDataInUnit.CalcLevel(DBList, int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value?.ToString())).ToString());
                this.ltvEnemyList.Items.Add(newItem);
            }
            this.ltvEnemyList.EndUpdate();

            //ユニットリストを更新
            this.edittingListChanged(this, null);
        }

        /// <summary>
        /// ユニット新規
        /// </summary>
        private void ctlDBGUIUnit_RequestRowAdd(object sender, EventArgs e) {
            this.ltvDB.SelectedIndices.Add(this.mgr.AddData(new mgrDBUnit.UnitOneData(this.mgr.GenerateFixedID(), 0, "新しいユニット", this.DBList)));
        }

        /// <summary>
        /// ユニット複製
        /// </summary>
        private void ctlDBGUIUnit_RequestRowCopy(object sender, EventArgs e) {
            var index = this.mgr.CopyData(this.ltvDB.SelectedIndices[0]);
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.SelectedIndices.Add(index);
        }

        /// <summary>
        /// ユニット削除
        /// </summary>
        private void ctlDBGUIUnit_RequestRowDelete(object sender, EventArgs e) {
            //選択されているうちに選択解除を行わないとイベントが発生しない
            var index = this.ltvDB.SelectedIndices[0];
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.DeleteData(index);
        }

        /// <summary>
        /// ユニット選択変更
        /// </summary>
        private void ltvDB_SelectedIndexChanged(object sender, EventArgs e) {
            this.LockDirty = true;

            if (this.ltvDB.SelectedIndices.Count == 0) {
                this.mgr.EndEdit();
            } else {
                this.mgr.BeginEdit(this.ltvDB.SelectedIndices[0]);
            }

            this.LockDirty = false;
        }

        /// <summary>
        /// ユニットリストの列をクリックすると、対応列に従ってソートする
        /// </summary>
        private void ltvDB_ColumnClick(object sender, ColumnClickEventArgs e) {
            if (this.mgr.EdittingData != null) {
                this.mgr.EndEdit();
            }
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.SortDataList(e.Column);
            this.IsDirty = true;
        }

        /// <summary>
        /// ID変更時
        /// </summary>
        private void tolUnitID_Validating(object sender, CancelEventArgs e) {
            if (!int.TryParse(this.tolUnitID.Text, out var val)) {
                //変換できない場合は既定値へ
                this.tolUnitID.Text = Database.NotFoundItemID.ToString();
            }
            this.mgr.EdittingData.VisibleID = int.Parse(this.tolUnitID.Text);
            e.Cancel = false;
        }

        /// <summary>
        /// 名前変更時
        /// </summary>
        private void tolUnitName_Validating(object sender, CancelEventArgs e) {
            this.mgr.EdittingData.Name = this.tolUnitName.Text;
            e.Cancel = false;
        }

        /// <summary>
        /// デザイナーのスケール変更
        /// </summary>
        private void tolScale_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.mgr == null) {
                return;
            }
            this.mgr.Scale = (ViewScale.ScaleRate) this.tolScale.SelectedIndex;

            //デザイナーを更新：プレビュー画面をゲームウィンドウの解像度と一致させる
            var scaledWndSize = mgrDBUnit.WindowSize;
            ViewScale.ApplyRateToSize(ref scaledWndSize, this.mgr.Scale);
            Bitmap bmp = null;
            if (!scaledWndSize.IsEmpty) {
                bmp = new Bitmap(scaledWndSize.Width, scaledWndSize.Height);
            }
            this.picPreview.Image = bmp;
            this.picPreview.Refresh();
        }

        /// <summary>
        /// 背景変更
        /// </summary>
        private void tolChangeBackGraph_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgFileSelect(Resources.Path_Materials, Resources.Extension_Graphics, true, false, false, Settings.Default.DB_UnitBackPicture);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.ChangeBack(Dlg.FileName);
            }
        }

        /// <summary>
        /// バトルテスト
        /// </summary>
        private void tolBattleTest_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Database.dlgBattleTest(this.DBList);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                var buf = $"bt {this.mgr.EdittingData.FixedID} \"{Dlg.Result}\"";
                Settings.Default.Last_BattleTestPTData = Dlg.Result;
                Common.RunTester(buf, false);
            }
        }

        /// <summary>
        /// ユニットスクリプトを開く
        /// </summary>
        private void tolEditScript_Click(object sender, EventArgs e) {
            this.mgr.OpenScript();
        }

        /// <summary>
        /// 名前自動生成
        /// </summary>
        private void tolAutoName_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null) {
                return;
            }
            this.mgr.EdittingData.GenerateAutoUnitName();
        }

        /// <summary>
        /// エネミーを登録
        /// </summary>
        private void btnAddEnemy_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null || this.ltvEnemyList.SelectedIndices.Count == 0) {
                return;
            }
            this.mgr.EdittingData.AddEnemy(int.Parse(this.ltvEnemyList.SelectedItems[0].Text));
        }

        /// <summary>
        /// ダブルクリックでエネミーを登録
        /// </summary>
        private void ltvEnemyList_DoubleClick(object sender, EventArgs e) {
            this.btnAddEnemy_Click(sender, e);
        }

        /// <summary>
        /// 登録済みのエネミーを削除
        /// </summary>
        private void btnDeleteEnemy_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null || this.ltvAnalyzer.SelectedIndices.Count == 0) {
                return;
            }

            //選択されているうちに選択解除を行わないとイベントが発生しない
            var index = this.ltvAnalyzer.SelectedIndices[0];
            this.ltvAnalyzer.SelectedIndices.Clear();
            this.mgr.EdittingData.DeleteEnemy(index);
        }

        /// <summary>
        /// ダブルクリックで登録済みのエネミーを削除
        /// </summary>
        private void ltvAnalyzer_DoubleClick(object sender, EventArgs e) {
            this.btnDeleteEnemy_Click(sender, e);
        }

        /// <summary>
        /// エネミーリスト選択時
        /// </summary>
        private void ltvEnemyList_SelectedIndexChanged(object sender, EventArgs e) {
            this.btnAddEnemy.Enabled = (this.ltvEnemyList.SelectedIndices.Count > 0);
        }

        /// <summary>
        /// 登録済みエネミーリスト選択時
        /// </summary>
        private void ltvAnalyzer_SelectedIndexChanged(object sender, EventArgs e) {
            this.btnDeleteEnemy.Enabled = (this.ltvAnalyzer.SelectedIndices.Count > 0);
        }

        /// <summary>
        /// デザイナー：ドラッグ開始
        /// </summary>
        private void picPreview_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Left) {
                return;
            }

            var NotFound = true;

            //レイヤーはインデックスが後ろになるほど上に描画される構造であるため、逆から判定する
            for (var i = this.mgr.EdittingData.Enemies.Count - 1; i >= 0; i--) {
                //拡大率を適用する
                var enemyPos = this.mgr.EdittingData.Enemies[i].Position;
                var enemySize = this.mgr.EdittingData.Enemies[i].Size;
                var pos = new Point(e.X, e.Y);
                ViewScale.RestoreOriginalPoint(ref pos, this.mgr.Scale);
                ViewScale.ApplyRateToPoint(ref enemyPos, this.mgr.Scale);
                ViewScale.ApplyRateToSize(ref enemySize, this.mgr.Scale);

                //ドラッグ可能な領域である場合
                if (new Rectangle(enemyPos, enemySize).Contains(e.X, e.Y)) {
                    this.drugObj = new DrugObject(i, new Point(e.X, e.Y));
                    this.ltvAnalyzer.SelectedIndices.Add(i);
                    NotFound = false;
                    break;
                }
            }

            if (NotFound) {
                this.drugObj = null;
                this.ltvAnalyzer.SelectedIndices.Clear();
                this.picPreview.Refresh();
            }
        }

        /// <summary>
        /// デザイナー：ドラッグ中
        /// </summary>
        private void picPreview_MouseMove(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Left || this.drugObj == null) {
                return;
            }
            this.mgr.EdittingData.OffsetPosition(this.drugObj.TargetIndex, this.drugObj.GetDistance(new Point(e.X, e.Y)), this.picPreview.Image.Size, this.mgr.Scale);
        }

        /// <summary>
        /// デザイナー：ドラッグ終了
        /// </summary>
        private void picPreview_MouseUp(object sender, MouseEventArgs e) {
            this.picPreview_MouseMove(sender, e);
            this.drugObj = null;
        }

        /// <summary>
        /// デザイナー：ドラッグ中止
        /// </summary>
        private void picPreview_MouseLeave(object sender, EventArgs e) {
            this.drugObj = null;
        }

        /// <summary>
        /// デザイナー：描画
        /// </summary>
        private void picPreview_Paint(object sender, PaintEventArgs e) {
            this.mgr.DrawDesigner(e.Graphics, this.picPreview.Image.Size, (this.ltvAnalyzer.SelectedIndices.Count == 0) ? -1 : this.ltvAnalyzer.SelectedIndices[0]);
        }

        /// <summary>
        /// ユニットリストに変更が起きたときにコントロールへ更新する
        /// </summary>
        protected override void edittingListChanged(object sender, EventArgs e) {
            var beforeIndex = (this.ltvDB.SelectedIndices.Count > 0) ? this.ltvDB.SelectedIndices[0] : -1;
            this.ltvDB.BeginUpdate();
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.Items.Clear();

            foreach (var item in this.mgr.Data) {
                var newItem = new ListViewItem {
                    Text = (!Settings.Default.Script_FixedIDHexMode) ? item.FixedID.ToString() : "0x" + item.FixedID.ToString("X")
                };
                newItem.SubItems.Add(item.VisibleID.ToString());
                newItem.SubItems.Add(item.Name);
                newItem.SubItems.Add(item.UnitLevel.ToString());

                //変更が生じた場合は該当リスト項目の背景色を変える
                if (item.IsDirty) {
                    newItem.BackColor = this.EdittingBackColor;
                } else {
                    newItem.BackColor = SystemColors.Window;
                }

                this.ltvDB.Items.Add(newItem);
            }
            this.ltvDB.EndUpdate();

            if (beforeIndex != -1 && beforeIndex < this.ltvDB.Items.Count) {
                this.ltvDB.SelectedIndices.Add(beforeIndex);
            }
        }

        /// <summary>
        /// 編集中のユニットに変更が起きたときにコントロールへ更新する
        /// </summary>
        private void edittingDataChanged(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null) {
                return;
            }

            //変更が生じた場合は該当リスト項目の背景色を変える
            if (this.mgr.EdittingData.IsDirty) {
                this.ltvDB.Items[this.mgr.EdittingIndex].BackColor = this.EdittingBackColor;
            } else {
                this.ltvDB.Items[this.mgr.EdittingIndex].BackColor = SystemColors.Window;
            }

            //ヘッダー情報
            this.tolUnitID.Text = this.mgr.EdittingData.VisibleID.ToString();
            this.tolUnitName.Text = this.mgr.EdittingData.Name;
            this.tolUnitLevel.Text = this.mgr.EdittingData.UnitLevel.ToString();

            //登録済みエネミーリスト
            var index = (this.ltvAnalyzer.SelectedIndices.Count == 0) ? -1 : this.ltvAnalyzer.SelectedIndices[0];
            this.ltvAnalyzer.BeginUpdate();
            if (this.drugObj == null) {
                this.ltvAnalyzer.SelectedIndices.Clear();
            }

            //全エネミーリスト
            this.ltvAnalyzer.Items.Clear();
            var enemyDB = this.DBList[Database.DBIndices.Enemy].DBs[0];

            foreach (var item in this.mgr.EdittingData.Enemies) {
                var newItem = new ListViewItem();
                var IDName = Database.GetIDNameFromFixedID(enemyDB, item.FixedID);
                newItem.Text = item.FixedID.ToString();
                newItem.SubItems.Add(Database.GetIDFromIDName(IDName).ToString());
                newItem.SubItems.Add(Database.GetNameFromIDName(IDName));
                newItem.SubItems.Add(item.EnemyLevel.ToString());
                newItem.SubItems.Add(Common.PointToString(item.Position));
                this.ltvAnalyzer.Items.Add(newItem);
            }

            if (0 <= index && index < this.ltvAnalyzer.Items.Count) {
                this.ltvAnalyzer.SelectedIndices.Add(index);     //再選択
            }

            this.ltvAnalyzer.EndUpdate();

            //ユニットリスト上の選択項目
            if (this.ltvDB.SelectedIndices.Count > 0) {
                this.ltvDB.SelectedItems[0].SubItems[(int) UnitColumns.VisibleID].Text = this.mgr.EdittingData.VisibleID.ToString();
                this.ltvDB.SelectedItems[0].SubItems[(int) UnitColumns.Name].Text = this.mgr.EdittingData.Name.ToString();
                this.ltvDB.SelectedItems[0].SubItems[(int) UnitColumns.Level].Text = this.mgr.EdittingData.UnitLevel.ToString();
            }

            //デザイナー描画
            this.picPreview.Refresh();
        }

    }
}
