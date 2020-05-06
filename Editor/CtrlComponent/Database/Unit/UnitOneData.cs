using System;
using System.Collections.Generic;
using System.Drawing;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBUnit {

        /// <summary>
        /// ユニット単体情報
        /// </summary>
        public partial class UnitOneData : ctlDBGUIBase.DBGUIOneDatabase {
            /// <summary>
            /// コンストラクター
            /// </summary>
            public UnitOneData(int fixedID, int visibleID, string name, AllDB DBList) : base(fixedID, visibleID, name) {
                this.DBList = DBList;
                this.Enemies = new List<EnemyDataInUnit>();
            }

            public AllDB DBList {
                get; set;
            }

            public List<EnemyDataInUnit> Enemies {
                get;
                private set;
            }

            /// <summary>
            /// ユニット単位の指標です。
            /// </summary>
            public int UnitLevel {
                get {
                    //各エネミーの指標を合計したものをユニット単位の指標とする
                    var result = 0;
                    foreach (var enemy in this.Enemies) {
                        result += enemy.EnemyLevel;
                    }
                    return result;
                }
            }

            /// <summary>
            /// エネミーを追加し、そのインデックスを返します。
            /// このとき、追加されるエネミーは常に画面の中心に置かれます。
            /// </summary>
            public int AddEnemy(int enemyFixedID) {
                this.Enemies.Add(new EnemyDataInUnit(enemyFixedID, new Point(mgrDBUnit.WindowSize.Width / 2, mgrDBUnit.WindowSize.Height / 2), this.DBList));
                this.Enemies[this.Enemies.Count - 1].DataChanged += this.unitEdittedRelay;
                this.Enemies[this.Enemies.Count - 1].LoadGraphics();
                this.IsDirty = true;
                return this.Enemies.Count - 1;
            }

            /// <summary>
            /// 指定したインデックスのエネミーを削除します。
            /// </summary>
            public void DeleteEnemy(int index) {
                this.Enemies.RemoveAt(index);
                this.IsDirty = true;
            }

            /// <summary>
            /// 指定したインデックスのエネミーの位置を指定した分だけ移動させます。
            /// </summary>
            public void OffsetPosition(int index, Point scaledposoffset, Size scaledwndSize, ViewScale.ScaleRate scale) {
                var enemyPos = this.Enemies[index].Position;
                var enemySize = this.Enemies[index].Size;
                ViewScale.ApplyRateToPoint(ref enemyPos, scale);
                ViewScale.ApplyRateToSize(ref enemySize, scale);
                var afterRange = new Rectangle(enemyPos.X + scaledposoffset.X, enemyPos.Y + scaledposoffset.Y, enemySize.Width, enemySize.Height);

                //拡大率を解除する
                var originalposoffset = scaledposoffset;
                var originalwndSize = scaledwndSize;
                ViewScale.RestoreOriginalPoint(ref originalposoffset, scale);
                ViewScale.RestoreOriginalSize(ref originalwndSize, scale);

                //端点に範囲外になる場合は調整する
                var limit = (int) (limitPosition * ViewScale.GetRate(scale));
                if (afterRange.Right < limit) {
                    this.Enemies[index].Position = new Point(-this.Enemies[index].Size.Width + limitPosition, this.Enemies[index].Position.Y);
                } else if (afterRange.Bottom < limit) {
                    this.Enemies[index].Position = new Point(this.Enemies[index].Position.X, -this.Enemies[index].Size.Height + limitPosition);
                } else if (afterRange.Left > scaledwndSize.Width - limit) {
                    this.Enemies[index].Position = new Point(originalwndSize.Width - limitPosition, this.Enemies[index].Position.Y);
                } else if (afterRange.Top > scaledwndSize.Height - limit) {
                    this.Enemies[index].Position = new Point(this.Enemies[index].Position.X, originalwndSize.Height - limitPosition);
                } else {
                    //調整しなかった場合のみ差分移動する
                    this.Enemies[index].OffsetPosition(originalposoffset);
                }

                this.IsDirty = true;
            }

            /// <summary>
            /// ユニット名を自動生成します。
            /// [エネミー名*匹数, ...] の形で生成します。
            /// </summary>
            /// <returns></returns>
            public void GenerateAutoUnitName() {
                var buf = "";
                var chkList = new bool[this.Enemies.Count];

                while (true) {
                    //まだチェックしていないエネミーを探す
                    int index;
                    for (index = 0; index < chkList.Length; index++) {
                        if (!chkList[index]) {
                            break;
                        }
                    }
                    if (index == chkList.Length) {
                        break;      //チェック完了
                    }

                    //同類を探す
                    var count = 0;
                    var fixedID = this.Enemies[index].FixedID;
                    for (var i = 0; i < this.Enemies.Count; i++) {
                        if (chkList[i] == false && this.Enemies[i].FixedID == fixedID) {
                            count++;
                            chkList[i] = true;
                        }
                    }

                    if (count > 1) {
                        buf += Database.GetNameFromIDName(Database.GetIDNameFromFixedID(this.DBList[Database.DBIndices.Enemy].DBs[0], fixedID)) + $"*{count}" + Resources.Split_Argument;
                    } else {
                        buf += Database.GetNameFromIDName(Database.GetIDNameFromFixedID(this.DBList[Database.DBIndices.Enemy].DBs[0], fixedID)) + Resources.Split_Argument;
                    }
                }

                buf = Common.CutLastChar(buf, Resources.Split_Argument.ToCharArray()[0]);
                this.Name = buf;
            }

            /// <summary>
            /// ユニットの変更イベントを中継します。
            /// </summary>
            private void unitEdittedRelay(object sender, EventArgs e) {
                this.IsDirty = true;
            }
        }
    }
}
