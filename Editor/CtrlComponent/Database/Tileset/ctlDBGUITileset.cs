using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlDBGUITileset : ctlDBGUIBase {
        public mgrDBTileset mgr;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBGUITileset() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.DBs = new DataGridView[] { this.CreateDefaultDataGridView() };
            this.tbcTileSettingTab_Selecting(this, null);
        }

        public override AllDB DBList {
            get {
                return this.dbList;
            }
            set {
                this.dbList = value;
                this.mgr = new mgrDBTileset(this.dbList, this.DBs[0]);
                this.mgr.ListChanged += this.edittingListChanged;
                this.mgr.DataChanged += this.edittingDataChanged;
                this.mgr.Editted += this.managerEditted;
            }
        }
        private AllDB dbList;

        public override bool DoSave() {
            //タイルセット定義ファイル
            foreach (var tileset in this.mgr.Data) {
                tileset.SaveFile();
            }

            //内部DB
            base.DoSave();

            //リストの背景を初期化する
            this.edittingListChanged(null, null);

            return true;
        }

        public override void DoLoad() {
            //重要：システムDBから基本情報を取得する
            mgrDBTileset.TileSize = int.Parse(this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.TileSize, 0].Value.ToString());

            //内部DB
            this.FlagBySystem = true;
            base.DoLoad();
            this.ApplyToManager<mgrDBTileset, mgrDBTileset.TilesetOneData>(this.mgr);

            //タイルセット定義ファイル
            foreach (var tileset in this.mgr.Data) {
                tileset.LoadFile();
            }

            this.FlagBySystem = false;
            this.IsDirty = false;
        }

        public override DialogResult Import() {
            this.ltvDB.SelectedIndices.Clear();
            return this.ImportWithFiles<mgrDBTileset, mgrDBTileset.TilesetOneData>(Resources.Path_DBs_Tilesets, Resources.Extension_Tileset, this.mgr);
        }

        public override DialogResult Export() {
            return this.ExportWithFiles(Resources.Path_DBs_Tilesets, Resources.Extension_Tileset, this.mgr.Data);
        }

        public override bool IsManagedFile(string rPath) {
            if (base.IsManagedFile(rPath)) {
                return true;
            }

            //付属ファイルを調べる
            foreach (var data in this.mgr.Data) {
                if (rPath == data.FileName) {
                    return true;
                }
            }
            return false;
        }

        public override void SrcReset(AllDB DBList) {
            //グラフィックが更新されて、大きさや内容が変化する可能性があるため、ここでロードし直して更新する
            mgrDBTileset.TileSize = int.Parse(this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.TileSize, 0].Value.ToString());
            if (this.mgr.EdittingData != null) {
                this.mgr.EdittingData.ResetCanvas();
                this.picPreview.Image = this.mgr.EdittingData.Canvas;
            }

            //タイルセットリストを更新
            this.edittingListChanged(this, null);
        }

        public override void Cleanup() {
            var files = Directory.GetFiles(ProjectManager.ProjectPath + Common.CutLastChar(Resources.Path_DBs_Tilesets, '\\'));
            foreach (var file in files) {
                if (int.TryParse(Path.GetFileNameWithoutExtension(file), out var id)) {
                    //ファイル名が数値である場合に限りID判定する
                    if (this.mgr.ExistsData(id)) {
                        continue;
                    }
                }

                //削除処理
                try {
                    File.Delete(file);
                } catch {
                    MessageBox.Show(Path.GetFileName(file) + " の削除に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// タイルセット新規
        /// </summary>
        private void ctlDBGUITileset_RequestRowAdd(object sender, EventArgs e) {
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.SelectedIndices.Add(this.mgr.AddData(new mgrDBTileset.TilesetOneData(this.mgr.GenerateFixedID(), 0, "新しいタイルセット", "", this.DBList)));
        }

        /// <summary>
        /// タイルセット複製
        /// </summary>
        private void ctlDBGUITileset_RequestRowCopy(object sender, EventArgs e) {
            var index = this.mgr.CopyData(this.ltvDB.SelectedIndices[0]);
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.SelectedIndices.Add(index);
        }

        /// <summary>
        /// タイルセット削除
        /// </summary>
        private void ctlDBGUITileset_RequestRowDelete(object sender, EventArgs e) {
            //選択されているうちに選択解除を行わないとイベントが発生しない
            var index = this.ltvDB.SelectedIndices[0];
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.DeleteData(index);
        }

        /// <summary>
        /// タイルセット選択変更
        /// </summary>
        private void ltvDB_SelectedIndexChanged(object sender, EventArgs e) {
            this.LockDirty = true;

            if (this.ltvDB.SelectedIndices.Count == 0) {
                this.mgr.EndEdit();
                this.picPreview.Image = null;
            } else {
                this.mgr.BeginEdit(this.ltvDB.SelectedIndices[0]);
            }

            this.LockDirty = false;
        }

        /// <summary>
        /// タイルセットリストの列をクリックすると、対応列に従ってソートする
        /// </summary>
        private void ltvDB_ColumnClick(object sender, ColumnClickEventArgs e) {
            if (this.mgr.EdittingData != null) {
                this.mgr.EndEdit();
                this.picPreview.Image = null;
            }
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.SortDataList(e.Column);
            this.IsDirty = true;
        }

        /// <summary>
        /// ID変更時
        /// </summary>
        private void tmnTilesetID_Validating(object sender, CancelEventArgs e) {
            if (!int.TryParse(this.tolTilesetID.Text, out var val)) {
                //変換できない場合は既定値へ
                this.tolTilesetID.Text = Database.NotFoundItemID.ToString();
            }
            e.Cancel = false;
            this.mgr.EdittingData.VisibleID = int.Parse(this.tolTilesetID.Text);
        }

        /// <summary>
        /// 名前変更時
        /// </summary>
        private void tmnTilesetName_Validating(object sender, CancelEventArgs e) {
            this.mgr.EdittingData.Name = this.tolTilesetName.Text;
            e.Cancel = false;
        }

        /// <summary>
        /// メモ変更時
        /// </summary>
        private void tolMemo_Validating(object sender, CancelEventArgs e) {
            this.mgr.EdittingData.Comment = this.tolMemo.Text;
            e.Cancel = false;
        }

        /// <summary>
        /// オートタイル追加
        /// </summary>
        private void tolAutoTileAdd_Click(object sender, EventArgs e) {
            var path = new CtrlComponent.Common.ctlPathSetter {
                DialogType = CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot,
                DialogTitle = "オートタイルの追加",
                FileFilter = Resources.Extension_Graphics,
                RootPath = Resources.Path_Materials
            };
            if (path.OpenPathDialog()) {
                this.mgr.EdittingData.AddAutoTile(path.Result);
            }
        }

        /// <summary>
        /// フォルダーからオートタイル追加
        /// </summary>
        private void tolAutoTileFolderAdd_Click(object sender, EventArgs e) {
            var path = new CtrlComponent.Common.ctlPathSetter {
                DialogType = CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot,
                DialogTitle = "フォルダーからオートタイル一括追加",
                FileFilter = Resources.Extension_Graphics,
                EnabledFiles = false,
                RootPath = Resources.Path_Materials
            };
            if (path.OpenPathDialog()) {
                //素材形式が不正な場合や上限数に達した場合はスキップされる
                this.mgr.EdittingData.AddAutoTileInFolder(path.Result);
            }
        }

        /// <summary>
        /// オートタイル編集
        /// </summary>
        private void tolAutoTileEdit_Click(object sender, EventArgs e) {
            if (this.ltvAutoTiles.SelectedIndices.Count != 1) {
                return;
            }
            var path = new CtrlComponent.Common.ctlPathSetter {
                DialogType = CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot,
                DialogTitle = "オートタイルの変更",
                FileFilter = Resources.Extension_Graphics,
                Result = this.ltvAutoTiles.SelectedItems[0].Text,
                RootPath = Resources.Path_Materials
            };
            if (path.OpenPathDialog()) {
                this.mgr.EdittingData.ReplaceAutoTile(this.ltvAutoTiles.SelectedIndices[0], path.Result);
            }
        }

        /// <summary>
        /// オートタイル削除
        /// </summary>
        private void tolAutoTileDelete_Click(object sender, EventArgs e) {
            if (this.ltvAutoTiles.SelectedIndices.Count == 0) {
                return;
            }

            var length = this.ltvAutoTiles.SelectedIndices.Count;
            for (var i = 0; i < length; i++) {
                this.mgr.EdittingData.DeleteAutoTile(this.ltvAutoTiles.SelectedIndices[0]);
            }
        }

        /// <summary>
        /// オートタイル上へ
        /// </summary>
        private void tolAutoTileUp_Click(object sender, EventArgs e) {
            if (this.ltvAutoTiles.SelectedIndices.Count != 1) {
                return;
            }

            var index = this.ltvAutoTiles.SelectedIndices[0];
            if (this.mgr.EdittingData.ToUpAutoTile(this.ltvAutoTiles.SelectedIndices[0])) {
                this.ltvAutoTiles.SelectedIndices.Clear();
                this.ltvAutoTiles.SelectedIndices.Add(index - 1);
            }
        }

        /// <summary>
        /// オートタイル下へ
        /// </summary>
        private void tolAutoTileDown_Click(object sender, EventArgs e) {
            if (this.ltvAutoTiles.SelectedIndices.Count != 1) {
                return;
            }

            var index = this.ltvAutoTiles.SelectedIndices[0];
            if (this.mgr.EdittingData.ToDownAutoTile(this.ltvAutoTiles.SelectedIndices[0])) {
                this.ltvAutoTiles.SelectedIndices.Clear();
                this.ltvAutoTiles.SelectedIndices.Add(index + 1);
            }
        }

        /// <summary>
        /// オートタイル選択変更
        /// </summary>
        private void ltvAutoTiles_SelectedIndexChanged(object sender, EventArgs e) {
            this.tolAutoTileDelete.Enabled = (this.ltvAutoTiles.SelectedIndices.Count > 0);
            this.tolAutoTileEdit.Enabled = (this.ltvAutoTiles.SelectedIndices.Count == 1);
            this.tolAutoTileDown.Enabled = (this.tolAutoTileEdit.Enabled && this.ltvAutoTiles.SelectedIndices[0] < this.mgr.EdittingData.AutoTiles.Count - 1);
            this.tolAutoTileUp.Enabled = (this.tolAutoTileEdit.Enabled && 0 < this.ltvAutoTiles.SelectedIndices[0]);
        }

        /// <summary>
        /// オブジェクトタイル追加
        /// </summary>
        private void tolOBJAdd_Click(object sender, EventArgs e) {
            var path = new CtrlComponent.Common.ctlPathSetter {
                DialogType = CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot,
                DialogTitle = "オブジェクトタイルの追加",
                FileFilter = Resources.Extension_Graphics,
                RootPath = Resources.Path_Materials
            };
            if (path.OpenPathDialog()) {
                //素材形式が不正な場合は却下
                if (this.mgr.EdittingData.CheckObjTileSize(path.Result, true)) {
                    this.mgr.EdittingData.AddObjTile(path.Result);
                }
            }
        }

        /// <summary>
        /// フォルダーからオブジェクトタイル追加
        /// </summary>
        private void tolOBJFolderAdd_Click(object sender, EventArgs e) {
            var path = new CtrlComponent.Common.ctlPathSetter {
                DialogType = CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot,
                DialogTitle = "フォルダーからオートタイル一括追加",
                FileFilter = Resources.Extension_Graphics,
                EnabledFiles = false,
                RootPath = Resources.Path_Materials
            };
            if (path.OpenPathDialog()) {
                //素材形式が不正な場合や上限数に達した場合はスキップされる
                this.mgr.EdittingData.AddObjTileInFolder(path.Result);
            }
        }

        /// <summary>
        /// オブジェクトタイル編集
        /// </summary>
        private void tolOBJEdit_Click(object sender, EventArgs e) {
            if (this.ltvObjTiles.SelectedIndices.Count != 1) {
                return;
            }

            var path = new CtrlComponent.Common.ctlPathSetter {
                DialogType = CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot,
                DialogTitle = "オブジェクトタイルの変更",
                FileFilter = Resources.Extension_Graphics,
                Result = this.ltvObjTiles.SelectedItems[0].Text,
                RootPath = Resources.Path_Materials
            };
            if (path.OpenPathDialog()) {
                this.mgr.EdittingData.ReplaceObjTile(this.ltvObjTiles.SelectedIndices[0], path.Result);
            }
        }

        /// <summary>
        /// オブジェクトタイル削除
        /// </summary>
        private void tolOBJDelete_Click(object sender, EventArgs e) {
            if (this.ltvObjTiles.SelectedIndices.Count == 0) {
                return;
            }

            var length = this.ltvObjTiles.SelectedIndices.Count;
            for (var i = 0; i < length; i++) {
                this.mgr.EdittingData.DeleteObjTile(this.ltvObjTiles.SelectedIndices[0]);
            }
        }

        /// <summary>
        /// オブジェクトタイル上へ
        /// </summary>
        private void tolOBJUp_Click(object sender, EventArgs e) {
            if (this.ltvObjTiles.SelectedIndices.Count != 1) {
                return;
            }

            var index = this.ltvObjTiles.SelectedIndices[0];
            if (this.mgr.EdittingData.ToUpObjTile(this.ltvObjTiles.SelectedIndices[0])) {
                this.ltvObjTiles.SelectedIndices.Clear();
                this.ltvObjTiles.SelectedIndices.Add(index - 1);
            }
        }

        /// <summary>
        /// オブジェクトタイル下へ
        /// </summary>
        private void tolOBJDown_Click(object sender, EventArgs e) {
            if (this.ltvObjTiles.SelectedIndices.Count != 1) {
                return;
            }

            var index = this.ltvObjTiles.SelectedIndices[0];
            if (this.mgr.EdittingData.ToDownObjTile(this.ltvObjTiles.SelectedIndices[0])) {
                this.ltvObjTiles.SelectedIndices.Clear();
                this.ltvObjTiles.SelectedIndices.Add(index + 1);
            }
        }

        /// <summary>
        /// オブジェクトタイル選択変更
        /// </summary>
        private void ltvObjects_SelectedIndexChanged(object sender, EventArgs e) {
            this.tolOBJDelete.Enabled = (this.ltvObjTiles.SelectedIndices.Count > 0);
            this.tolOBJEdit.Enabled = (this.ltvObjTiles.SelectedIndices.Count == 1);
            this.tolOBJDown.Enabled = (this.tolOBJEdit.Enabled && this.ltvObjTiles.SelectedIndices[0] < this.mgr.EdittingData.ObjTiles.Count - 1);
            this.tolOBJUp.Enabled = (this.tolOBJEdit.Enabled && 0 < this.ltvObjTiles.SelectedIndices[0]);
        }

        /// <summary>
        /// タイル情報の種別タブ切替時
        /// </summary>
        private void tbcTileSettingTab_Selecting(object sender, TabControlCancelEventArgs e) {
            //唯一のpictureBoxをアクティブなタブへ移動させる
            this.tbcTileSettingTab.SelectedTab.Controls.Add(this.picPreview);
            this.picPreview.Location = Point.Empty;
            this.picPreview.Refresh();

            //タイル記号の説明を表示する
            switch (this.tbcTileSettingTab.SelectedIndex) {
                case (int) Map.TileDataType.Hit:
                    this.lblTileDataComment.Text = "○: 通行可能  ×: 通行不可  ↓: 下位レイヤーに依存";
                    break;
                case (int) Map.TileDataType.InOut:
                    this.lblTileDataComment.Text = "その方向に出入りできるかどうかを設定します。";
                    break;
                case (int) Map.TileDataType.Priority:
                    this.lblTileDataComment.Text = "無印: 通常描画  ▲: そのタイルに重なったキャラを隠す（キャラが裏に回る）\r\n□: 重なったキャラの下半身を半透明表示  ☆: このタイルを最前面表示";
                    break;
                case (int) Map.TileDataType.GroupNumber:
                    this.lblTileDataComment.Text = "タイルに対して番号を付けます。\r\n梯子設定など、独自の処理をする場合に使います。";
                    break;
            }
        }

        /// <summary>
        /// タイル情報を変更
        /// </summary>
        private void picTilePreview_MouseUp(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Left && e.Button == MouseButtons.Right && e.Button == MouseButtons.Middle) {
                return;
            }

            var tile = this.mgr.EdittingData.GetTileFromPixel(new Point(e.X, e.Y));
            if (tile == null) {
                return;     //該当タイルが存在しない場合は処理しない
            }

            if (e.Button == MouseButtons.Middle
            && this.tbcTileSettingTab.SelectedIndex == (int) Map.TileDataType.GroupNumber) {
                //グループ番号で中央クリックされたときは番号を直接入力させる
                var Dlg = new Dialog.Common.dlgInputNumValue(0, mgrDBTileset.TilesetOneData.TileData.GroupMax, tile.GroupNumber, "グループ番号を入力");
                if (Dlg.ShowDialog() == DialogResult.OK) {
                    tile.GroupNumber = Dlg.Result;
                }
            } else if ((Map.TileDataType) this.tbcTileSettingTab.SelectedIndex == Map.TileDataType.InOut) {
                //出入設定の場合は押された向きの部分のON/OFF切替を行う
                var tilePos = new Point(e.X % mgrDBTileset.TileSize, e.Y % mgrDBTileset.TileSize);
                tile.TurnInOut(mgrDBTileset.TilesetOneData.TileData.GetDirectionFromPoint(tilePos));
            } else {
                //次の属性に切り替える
                tile.Next(
                    (Map.TileDataType) this.tbcTileSettingTab.SelectedIndex,
                    (e.Button == MouseButtons.Left) ? false : true
                );
            }
            this.picPreview.Refresh();
        }

        /// <summary>
        /// タイルデザイナーを描画
        /// </summary>
        private void picTilePreview_Paint(object sender, PaintEventArgs e) {
            this.mgr.EdittingData.DrawTileset(e.Graphics, (Map.TileDataType) this.tbcTileSettingTab.SelectedIndex);
        }

        /// <summary>
        /// タイルセットリストに変更が起きたときにコントロールへ更新する
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
                newItem.SubItems.Add(item.Comment.ToString());

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
        /// 編集中のタイルセットに変更が起きたときにコントロールへ更新する
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
            this.tolTilesetID.Text = this.mgr.EdittingData.VisibleID.ToString();
            this.tolTilesetName.Text = this.mgr.EdittingData.Name;
            this.tolMemo.Text = this.mgr.EdittingData.Comment;

            //オートタイルリスト
            var index = (this.ltvAutoTiles.SelectedIndices.Count == 0) ? -1 : this.ltvAutoTiles.SelectedIndices[0];

            this.ltvAutoTiles.BeginUpdate();
            this.ltvAutoTiles.SelectedIndices.Clear();
            this.ltvAutoTiles.Items.Clear();

            foreach (var atFile in this.mgr.EdittingData.AutoTiles) {
                var newItem = new ListViewItem {
                    Text = atFile.FileName
                };
                newItem.SubItems.Add(Path.GetFileName(newItem.Text));
                this.ltvAutoTiles.Items.Add(newItem);
            }

            if (0 <= index && this.mgr.EdittingData.AutoTiles.Count > 0) {
                //再選択
                if (index < this.mgr.EdittingData.AutoTiles.Count) {
                    this.ltvAutoTiles.SelectedIndices.Add(index);
                } else {
                    this.ltvAutoTiles.SelectedIndices.Add(this.mgr.EdittingData.AutoTiles.Count - 1);
                }
            }

            this.ltvAutoTiles.EndUpdate();

            //オブジェクトタイルリスト
            index = (this.ltvObjTiles.SelectedIndices.Count == 0) ? -1 : this.ltvObjTiles.SelectedIndices[0];

            this.ltvObjTiles.BeginUpdate();
            this.ltvObjTiles.SelectedIndices.Clear();
            this.ltvObjTiles.Items.Clear();

            foreach (var objFile in this.mgr.EdittingData.ObjTiles) {
                var newItem = new ListViewItem {
                    Text = objFile.FileName
                };
                newItem.SubItems.Add(Path.GetFileName(newItem.Text));
                this.ltvObjTiles.Items.Add(newItem);
            }

            if (0 <= index && this.mgr.EdittingData.ObjTiles.Count > 0) {
                //再選択
                if (index < this.mgr.EdittingData.ObjTiles.Count) {
                    this.ltvObjTiles.SelectedIndices.Add(index);
                } else {
                    this.ltvObjTiles.SelectedIndices.Add(this.mgr.EdittingData.ObjTiles.Count - 1);
                }
            }

            this.ltvObjTiles.EndUpdate();

            //ユニットリスト上の選択項目
            if (this.ltvDB.SelectedIndices.Count > 0) {
                this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.VisibleID].Text = this.mgr.EdittingData.VisibleID.ToString();
                this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text = this.mgr.EdittingData.Name.ToString();
                this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Count].Text = this.mgr.EdittingData.Comment;
            }

            //デザイナー描画
            this.picPreview.Image?.Dispose();
            this.picPreview.Image = this.mgr.EdittingData.Canvas;
        }
    }
}
