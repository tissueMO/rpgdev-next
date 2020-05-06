using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    /// <summary>
    /// タイルセットの管理クラス
    /// </summary>
    public partial class mgrDBTileset : ctlDBGUIBase.DBGUIManagerBase<mgrDBTileset.TilesetOneData> {
        public static int TileSize;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public mgrDBTileset(AllDB DBList, DataGridView mydb) : base(DBList, mydb) { }

        public override int CopyData(int index) {
            return base.CopyData(index, new Action(() => {
                //各タイル情報もコピーする
                this.EdittingData.AutoTiles = this.Data[index].CopyAutoTiles();
                this.EdittingData.ObjTiles = this.Data[index].CopyObjTiles();
            }));
        }

        public override void DeleteData(int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }
            if (!this.DeleteFileWithData(Resources.Path_DBs_Tilesets, Resources.Extension_Tileset, index)) {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "タイルセット定義ファイルの削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            } else {
                base.DeleteData(index);
            }
        }

        public override void SetData(DataGridViewRow row) {
            string[] spl;

            //タイルセットを生成
            var newItem = new TilesetOneData(
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString()),
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value.ToString()),
                row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value.ToString(),
                row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.Comment].Value.ToString(),
                this.DBList
            );

            //オートタイルリストを復元
            spl = row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.AutoTiles].Value.ToString().Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            if (spl != null) {
                foreach (var file in spl) {
                    newItem.AddAutoTile(file);
                }
            }

            //オブジェクトタイルリストを復元
            spl = row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.ObjTiles].Value.ToString().Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            if (spl != null) {
                foreach (var file in spl) {
                    newItem.AddObjTile(file);
                }
            }

            newItem.DataChanged += this.dataChangedRelay;
            this.Data.Add(newItem);
            this.OnListChanged();
            this.OnEditted();
        }

        public override void PushData(DataGridViewRow row, int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }

            base.PushData(row, index);

            //独自情報: メモ
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.Comment].Value = this.Data[index].Comment;

            //独自情報: [FileName|...]
            string buf;

            //オートタイル
            buf = "";
            foreach (var atFile in this.Data[index].AutoTiles) {
                buf += atFile.FileName;
                buf += Resources.Split_LongData;
            }
            buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.AutoTiles].Value = buf;

            //オブジェクトタイル
            buf = "";
            foreach (var objFile in this.Data[index].ObjTiles) {
                buf += objFile.FileName;
                buf += Resources.Split_LongData;
            }
            buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBTilesetColumnIndices.ObjTiles].Value = buf;
        }

        public override void BeginEdit(int index) {
            base.BeginEdit(index, new Action(() => {
                this.EdittingData.ResetCanvas();
            }));
        }

        public override void EndEdit() {
            base.EndEdit(new Action(() => {
                if (this.EdittingData == null) {
                    return;
                }

                //リソース破棄
                this.EdittingData.Canvas?.Dispose();
                foreach (var at in this.EdittingData.AutoTiles) {
                    at?.GraphImg?.Dispose();
                }
                foreach (var obj in this.EdittingData.ObjTiles) {
                    obj?.GraphImg?.Dispose();
                }
            }));
        }

        public override bool ExistsData(int fixedID) {
            foreach (var data in this.Data) {
                if (data.FixedID == fixedID) {
                    return true;
                }
            }
            return false;
        }

        protected override void dataChangedRelay(object sender, EventArgs e) {
            if (this.EdittingIndex == -1) {
                return;
            }
            this.EdittingData.ResetCanvas();
            this.PushData(this.MyDB.Rows[this.EdittingIndex], this.EdittingIndex);
            this.OnDataChanged();
            this.OnEditted();
        }

        /// <summary>
        /// タイルセットの変更イベントを中継します。
        /// </summary>
        public void tilesetEdittedRelay(object sender, EventArgs e) {
            this.OnEditted();
        }
    }
}
