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
    /// ユニットの管理クラス
    /// </summary>
    public partial class mgrDBUnit : ctlDBGUIBase.DBGUIManagerBase<mgrDBUnit.UnitOneData> {
        public static Size WindowSize;
        public static string LevelExpression = "";
        private Image backPicture;
        private const int limitPosition = 10;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public mgrDBUnit(AllDB DBList, DataGridView mydb) : base(DBList, mydb) { }

        public override ViewScale.ScaleRate Scale {
            get {
                return this.scale;
            }
            set {
                //有効な拡大率である場合のみ適用
                if (Enum.IsDefined(typeof(ViewScale.ScaleRate), value)) {
                    this.scale = value;
                    Settings.Default.DB_UnitScale = (int) value;
                    this.OnDataChanged();
                }
            }
        }
        private ViewScale.ScaleRate scale;

        public override void SortDataList(int index) {
            this.EndEdit();

            switch (index) {
                case (int) ctlDBGUIUnit.UnitColumns.VisibleID:
                    this.Data.Sort((x, y) => {
                        return x.VisibleID - y.VisibleID;
                    });
                    break;

                case (int) ctlDBGUIUnit.UnitColumns.Name:
                    this.Data.Sort((x, y) => {
                        return string.Compare(x.Name, y.Name);
                    });
                    break;

                case (int) ctlDBGUIUnit.UnitColumns.Level:
                    this.Data.Sort((x, y) => {
                        return x.UnitLevel - y.UnitLevel;
                    });
                    break;
            }

            this.OnListChanged();
        }

        public override void SetData(DataGridViewRow row) {
            //ユニットを生成
            var newItem = new UnitOneData(
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.FixedID].Value.ToString()),
                int.Parse(row.Cells[(int) Database.DBDefaultColumnIndices.VisibleID].Value.ToString()),
                row.Cells[(int) Database.DBDefaultColumnIndices.Name].Value.ToString(),
                this.DBList
            );
            newItem.DataChanged += this.dataChangedRelay;

            //ユニット内のエネミーを生成
            var buf = row.Cells[(int) Database.DBDefaultColumnIndices.Count].Value.ToString();
            var spl = buf.Split(Resources.Split_LongData.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

            if (spl != null) {
                foreach (var enemy in spl) {
                    var subspl = enemy.Split(Resources.Split_SimpleList.ToCharArray());
                    var newEnemy = new UnitOneData.EnemyDataInUnit(
                        int.Parse(subspl[(int) Database.DBUnitEnemyIndices.FixedID]),
                        Common.StringToPoint(subspl[(int) Database.DBUnitEnemyIndices.Position]),
                        this.DBList
                    );
                    newItem.Enemies.Add(newEnemy);
                }
            }

            this.Data.Add(newItem);
            this.OnListChanged();
        }

        public override void PushData(DataGridViewRow row, int index) {
            if (index < 0 || this.Data.Count <= index) {
                return;
            }

            //標準列情報
            base.PushData(row, index);

            //独自情報: [EnemyFixedID;X:Y|...]
            var buf = "";
            foreach (var enemy in this.Data[index].Enemies) {
                for (var i = 0; i < Common.GetEnumCount<Database.DBUnitEnemyIndices>(); i++) {
                    switch (i) {
                        case (int) Database.DBUnitEnemyIndices.FixedID:
                            buf += enemy.FixedID;
                            break;
                        case (int) Database.DBUnitEnemyIndices.Position:
                            buf += enemy.Position.X + Resources.Split_IDNamePair + enemy.Position.Y;
                            break;
                    }
                    buf += Resources.Split_SimpleList;
                }
                buf = Common.CutLastChar(buf, Resources.Split_SimpleList.ToCharArray()[0]);
                buf += Resources.Split_LongData;
            }

            buf = Common.CutLastChar(buf, Resources.Split_LongData.ToCharArray()[0]);
            row.Cells[(int) Database.DBDefaultColumnIndices.Count].Value = buf;
        }

        public override void BeginEdit(int index) {
            base.BeginEdit(index, new Action(() => {
                //リソース読み込み
                foreach (var enemy in this.EdittingData.Enemies) {
                    enemy.LoadGraphics();
                }
                this.ChangeBack(Settings.Default.DB_UnitBackPicture);
            }));
        }

        public override void EndEdit() {
            base.EndEdit(new Action(() => {
                if (this.EdittingData == null) {
                    return;
                }

                //リソース破棄
                foreach (var enemy in this.EdittingData.Enemies) {
                    enemy?.UnloadGraphics();
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

        /// <summary>
        /// 背景を変更します。
        /// </summary>
        public void ChangeBack(string rPath) {
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                //存在しないファイルを指定したら背景を非表示にする
                this.backPicture = null;
                Settings.Default.DB_UnitBackPicture = "";
                return;
            }

            Settings.Default.DB_UnitBackPicture = rPath;
            this.backPicture = Image.FromFile(ProjectManager.ProjectPath + rPath);
            this.OnDataChanged();
        }

        /// <summary>
        /// 指定したグラフィック描画オブジェクトを用いてデザイナー用の描画を行います。
        /// </summary>
        public void DrawDesigner(Graphics g, Size scaledwndSize, int activeIndex) {
            Common.ApplyHighQualityDrawing(g);

            //背景描画
            if (this.backPicture != null) {
                g.DrawImage(this.backPicture, new Rectangle(Point.Empty, scaledwndSize));
            }

            //各エネミーを描画
            if (this.EdittingData != null) {
                for (var i = 0; i < this.EdittingData.Enemies.Count; i++) {
                    //現在の表示倍率に合わせる
                    var pos = this.EdittingData.Enemies[i].Position;
                    var size = this.EdittingData.Enemies[i].Img?.Size ?? Size.Empty;
                    ViewScale.ApplyRateToPoint(ref pos, this.Scale);
                    ViewScale.ApplyRateToSize(ref size, this.Scale);

                    if (this.EdittingData.Enemies[i].Img != null) {
                        g.FillRectangle(new SolidBrush(Color.FromArgb(100, (i == activeIndex) ? Color.Yellow : Color.White)), new Rectangle(pos, size));
                        g.DrawImage(this.EdittingData.Enemies[i].Img, new Rectangle(pos, size));
                        var strSize = g.MeasureString(this.EdittingData.Enemies[i].Name, Settings.Default.Text_Font);
                        g.DrawString(this.EdittingData.Enemies[i].Name, Settings.Default.Text_Font, Brushes.White, new Point(pos.X, pos.Y - (int) strSize.Height));
                    }
                }
            }

            //ガイド線描画
            g.DrawRectangle(new Pen(Color.LightGray, 3), 0, 0, scaledwndSize.Width, scaledwndSize.Height);
            g.DrawLine(Pens.LightGray, scaledwndSize.Width / 2, 0, scaledwndSize.Width / 2, scaledwndSize.Height);
            g.DrawLine(Pens.LightGray, 0, scaledwndSize.Height / 2, scaledwndSize.Width, scaledwndSize.Height / 2);
        }

        /// <summary>
        /// ユニットスクリプトを開きます。
        /// </summary>
        public void OpenScript() {
            //スクリプトに移行するボタンを探して自分で押す
            var unitDB = this.DBList[Database.DBIndices.Unit].DBs[0];
            foreach (DataGridViewColumn col in unitDB.Columns) {
                if (col.Tag is DBColumnButtonToScript) {
                    ((DBColumnButtonToScript) col.Tag).Click(unitDB.Columns, unitDB.Rows[this.EdittingIndex], unitDB[col.Index, this.EdittingIndex]);
                }
            }
        }
    }
}
