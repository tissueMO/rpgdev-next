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
    /// エフェクトの管理クラス
    /// </summary>
    public partial class mgrDBEffect : ctlDBGUIBase.DBGUIManagerBase<mgrDBEffect.EffectOneData> {
        public delegate void DataChangedEventHandler(object sender, DataChangedEventArgs e);
        public event DataChangedEventHandler DataChangedEx;
        public static int dxTargetHdl = -1;
        public static int FPS = 60;
        public static Size ScreenSize;
        public EffectDrawObject Drawer;
        public const int FrameMaxLength = 3600;         //60FPSで最大１分、30FPSで最大２分までのアニメーションに対応する
        public const int FrameMinLength = 1;            //新規作成したときに設定される既定のエフェクトのフレーム数
        public const int FrameDefaultLength = 10;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public mgrDBEffect(AllDB DBList, DataGridView mydb, EffectDrawObject drawer) : base(DBList, mydb) {
            this.Drawer = drawer;
        }

        /// <summary>
        /// 変更されたデータの種別を示します。
        /// </summary>
        public class DataChangedEventArgs : EventArgs {
            public readonly bool Effects;
            public readonly bool Orders;
            public readonly bool Layers;
            public readonly bool Frame;

            public DataChangedEventArgs(bool eff, bool odr, bool lyr, bool frm) {
                this.Effects = eff;
                this.Orders = odr;
                this.Layers = lyr;
                this.Frame = frm;
            }
        }

        public override ViewScale.ScaleRate Scale {
            get {
                return this.scale;
            }
            set {
                //有効な拡大率である場合のみ適用
                if (Enum.IsDefined(typeof(ViewScale.ScaleRate), value)) {
                    this.scale = value;
                    Settings.Default.DB_EffectScale = (int) value;
                }
            }
        }
        private ViewScale.ScaleRate scale;

        /// <summary>
        /// 独自コマンドのソースファイルリスト
        /// </summary>
        public List<string> UserCommandScripts {
            get {
                return this.userCommandScripts;
            }
            set {
                this.userCommandScripts = value;
                this.IsUserCommandsDirty = true;
                this.OnEditted();
            }
        }
        private List<string> userCommandScripts = new List<string>();

        public bool IsUserCommandsDirty {
            get; set;
        } = false;

        public override int AddData(EffectOneData newData) {
            base.AddData(newData);
            this.EdittingData.DataChangedEx += this.dataChangedRelay;
            this.EdittingData.DrawerReload += this.dataDrawerReloadRelay;
            return this.EdittingIndex;
        }

        public override int CopyData(int index) {
            return base.CopyData(index, new Action(() => {
                //命令とフレームもコピーする
                this.EdittingData.Layers = this.Data[index].CloneLayers();
                this.EdittingData.Orders = this.Data[index].CloneOrders();

                //拡張部分
                this.EdittingData.DataChangedEx += this.dataChangedRelay;
                this.EdittingData.DrawerReload += this.dataDrawerReloadRelay;
            }));
        }

        public override void DeleteData(int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }

            if (!this.DeleteFileWithData(Resources.Path_DBs_EffectScripts, Resources.Extension_SQ, index)) {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "エフェクトスクリプトの削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            } else {
                base.DeleteData(index);
            }
        }

        public override void SetData(DataGridViewRow row) {
            //エフェクトを生成
            var newItem = new EffectOneData(
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString()),
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value.ToString()),
                row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value.ToString(),
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.FrameLength].Value.ToString()),
                (Database.DBEffectViewPosition) int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.DrawDest].Value.ToString()),
                this.DBList
            );

            newItem.DataChangedEx += this.dataChangedRelay;
            newItem.DrawerReload += this.dataDrawerReloadRelay;

            //エフェクト内のレイヤーを生成
            var buf = row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.Layers].Value.ToString();
            newItem.SetupLayers(buf);

            this.Data.Add(newItem);
            this.OnListChanged();
            this.OnEditted();
        }

        public override void PushData(DataGridViewRow row, int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }

            //標準列情報
            base.PushData(row, index);

            //独自情報: フレーム数、描画基準
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.FrameLength].Value = this.Data[index].FrameLength;
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.DrawDest].Value = (int) this.Data[index].DrawDest;

            //独自情報: [FixedID:ファイル名:WxH|...]
            var buf = "";
            foreach (var layer in this.Data[index].Layers) {
                buf += layer.FixedID;
                buf += Resources.Split_IDNamePair;
                buf += layer.FileName;
                buf += Resources.Split_IDNamePair;
                buf += Common.SizeToString(layer.SectionSize);
                buf += Resources.Split_LongData;
            }
            buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
            row.Cells[(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.Layers].Value = buf;
        }

        public override void BeginEdit(int index) {
            base.BeginEdit(index, new Action(() => {
                //リソース読み込み
                this.Drawer.TargetEffectData = this.EdittingData;
                Media.ChangeDest(this.Drawer);
            }));
            this.DataChangedEx?.Invoke(this, new DataChangedEventArgs(true, true, true, true));
        }

        public override bool ExistsData(int fixedID) {
            foreach (var data in this.Data) {
                if (data.FixedID == fixedID) {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 対象グラフィックを変更します。
        /// </summary>
        public void ChangeTarget(string rPath) {
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                dxTargetHdl = -1;
                Settings.Default.DB_EffectTargetPicture = "";
                return;
            }
            Settings.Default.DB_EffectTargetPicture = rPath;
            this.dataDrawerReloadRelay(this, null);
        }

        /// <summary>
        /// 編集中のデータが変更されたときに発生するイベントを中継します。
        /// </summary>
        private void dataChangedRelay(object sender, DataChangedEventArgs e) {
            if (this.EdittingData != null) {
                this.EdittingData.IsDirty = true;
                this.PushData(this.MyDB.Rows[this.EdittingIndex], this.EdittingIndex);
            }
            this.DataChangedEx?.Invoke(sender, e);
            this.OnEditted();
        }

        /// <summary>
        /// 描画オブジェクトを更新する必要があるときに発生するイベントを中継します。
        /// </summary>
        private void dataDrawerReloadRelay(object sender, EventArgs e) {
            this.Drawer.LoadMaterial();
        }
    }
}
