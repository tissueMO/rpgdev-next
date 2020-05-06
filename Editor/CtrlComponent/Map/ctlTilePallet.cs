using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Module.DB;

    public partial class ctlTilePallet : UserControl {
        public event EventHandler SelectedPalletTile;
        private CtrlComponent.Database.mgrDBTileset.TilesetOneData tileset;
        private static Bitmap rawBmp;
        private SizeF tileSize;
        private int fixedID = -1;                           //対象とするタイルセットのFixedID
        private int heightTile;                             //縦にタイルがいくつ並べられているか
        private double scaleRate;
        private bool resizingFlag = false;
        private Rectangle tileRectangle;
        private Point selectedTileStartPosition;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlTilePallet() {
            this.InitializeComponent();
            this.picTargetGraphics.MouseWheel += this.picTargetGraphics_MouseWheel;
        }

        /// <summary>
        /// 原寸サイズのパレットグラフィック
        /// </summary>
        public static Bitmap PalletGraphics {
            get {
                return ctlTilePallet.rawBmp?.Clone(new Rectangle(Point.Empty, ctlTilePallet.rawBmp.Size), System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            }
        }

        /// <summary>
        /// 選択範囲
        /// </summary>
        public Rectangle? SelectedRange {
            get {
                return this.selectedRange;
            }
            set {
                this.selectedRange = value;
                this.picTargetGraphics.Refresh();
                this.SelectedPalletTile?.Invoke(this, null);
            }
        }
        private Rectangle? selectedRange = new Rectangle(0, 0, 1, 1);

        /// <summary>
        /// 選択タイルの当たり判定
        /// </summary>
        public string SelectedTileHitInfo {
            get {
                if (this.SelectedRange == null || this.SelectedRange.Value.Location.IsEmpty) {
                    //非選択時
                    return "";
                } else if (this.SelectedRange.Value.Width > 1 || this.SelectedRange.Value.Height > 1) {
                    //範囲選択時
                    return "";
                } else {
                    var tileData = this.tileset.GetTileFromTilePos(this.SelectedRange.Value.Location);
                    if (tileData == null) {
                        //無効なタイル選択時
                        return "";
                    }
                    return Map.GetTileHitTypeList()[(int) tileData.Hit];
                }
            }
        }

        /// <summary>
        /// 選択タイルのグループ番号
        /// </summary>
        public int SelectedTileGroupNumber {
            get {
                if (this.SelectedRange == null || this.SelectedRange.Value.Location.IsEmpty) {
                    //非選択時
                    return -1;
                } else if (this.SelectedRange.Value.Width > 1 || this.SelectedRange.Value.Height > 1) {
                    //範囲選択時
                    return -1;
                } else {
                    return this.tileset.GetTileFromTilePos(this.SelectedRange.Value.Location)?.GroupNumber ?? -1;
                }
            }
        }

        /// <summary>
        /// 指定したタイルセットで描画準備を行います。
        /// </summary>
        public void Setup(int fixedID) {
            //タイルセットDBのインスタンスを頑張って探し当てる
            var mainWnd = Common.GetHigherControl(this, typeof(MainWindow)) as MainWindow;
            var tilesetDB = mainWnd.uctlDBEditor.tbcDatabaseTab.TabPages[(int) Database.DBIndices.Tileset].Controls[0] as CtrlComponent.Database.ctlDBGUITileset;
            this.fixedID = fixedID;

            foreach (var data in tilesetDB.mgr.Data) {
                if (data.FixedID != fixedID) {
                    //該当しないタイルセットはスキップ
                    continue;
                }

                //ベースとなるタイルセット画像を生成する
                this.tileset = data;
                data.ResetCanvas();
                ctlTilePallet.rawBmp = new Bitmap(data.Canvas.Width, data.Canvas.Height);
                var g = Graphics.FromImage(ctlTilePallet.rawBmp);
                data.DrawTileset(g, null);
                g.Dispose();

                //通常描画を行う
                this.ctlTilePallet_Resize(this, null);
                break;
            }
        }

        /// <summary>
        /// コントロールがリサイズされたらパレットも併せてリサイズします。
        /// </summary>
        private void ctlTilePallet_Resize(object sender, EventArgs e) {
            if (ctlTilePallet.rawBmp == null || this.resizingFlag) {
                return;
            }

            //PictureBoxの性質ゆえ、同じスケールで描画しても微妙に縦・横の比が崩れてしまうため、１タイルあたりのサイズは縦・横別々に算出する
            this.resizingFlag = true;       //多重にリサイズイベントが発生するのを防ぐ
            this.picTargetGraphics.Size = new Size(1, this.Height * 2);                                      //一旦強制的にスクロールバーを表示させる
            this.scaleRate = this.picTargetGraphics.Size.Width / (double) ctlTilePallet.rawBmp.Width;         //Dock.Topであるため、Widthは自動設定されている
            this.picTargetGraphics.Height = (int) (ctlTilePallet.rawBmp.Height * this.scaleRate);

            this.heightTile = ctlTilePallet.rawBmp.Height / (ctlTilePallet.rawBmp.Width / Map.TileColumnCount);
            this.tileSize = new SizeF((float) (ctlTilePallet.rawBmp.Width / Map.TileColumnCount * this.scaleRate), (float) (ctlTilePallet.rawBmp.Height / this.heightTile * this.scaleRate));
            this.tileRectangle = new Rectangle(0, 0, Map.TileColumnCount, this.heightTile);

            this.picTargetGraphics.Image = new Bitmap(this.picTargetGraphics.Size.Width, this.picTargetGraphics.Size.Height);
            this.picTargetGraphics.Refresh();
            this.resizingFlag = false;
        }

        /// <summary>
        /// パレットを描画します。
        /// </summary>
        private void picTargetGraphics_Paint(object sender, PaintEventArgs e) {
            if (ctlTilePallet.rawBmp == null) {
                return;
            }
            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.DrawImage(ctlTilePallet.rawBmp, new Rectangle(0, 0, this.picTargetGraphics.Width, this.picTargetGraphics.Height));
            this.drawCursor(e.Graphics);
        }

        /// <summary>
        /// 選択カーソルを描画します。
        /// </summary>
        private void drawCursor(Graphics g) {
            if (this.SelectedRange == null) {
                return;
            }

            for (var i = 1; i <= 2; i++) {
                g.DrawRectangle(Pens.White,
                    this.SelectedRange.Value.Left * this.tileSize.Width + i,
                    this.SelectedRange.Value.Top * this.tileSize.Height + i,
                    this.SelectedRange.Value.Width * this.tileSize.Width - i * 2,
                    this.SelectedRange.Value.Height * this.tileSize.Height - i * 2
                );
            }

            g.DrawRectangle(Pens.Black, this.SelectedRange.Value.Left * this.tileSize.Width, this.SelectedRange.Value.Top * this.tileSize.Height, this.SelectedRange.Value.Width * this.tileSize.Width, this.SelectedRange.Value.Height * this.tileSize.Height);
            g.DrawRectangle(Pens.Black, this.SelectedRange.Value.Left * this.tileSize.Width + 3, this.SelectedRange.Value.Top * this.tileSize.Height + 3, this.SelectedRange.Value.Width * this.tileSize.Width - 3 * 2, this.SelectedRange.Value.Height * this.tileSize.Height - 3 * 2);
        }

        /// <summary>
        /// パレット上でマウスボタンが押されたとき
        /// </summary>
        private void picTargetGraphics_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                this.SelectedRange = Common.ApplyLimit(new Rectangle((int) (e.X / this.tileSize.Width), (int) (e.Y / this.tileSize.Height), 1, 1), this.tileRectangle);
                this.selectedTileStartPosition = this.SelectedRange.Value.Location;
                this.picTargetGraphics.Refresh();
            }
        }

        /// <summary>
        /// パレット上でマウスカーソルが移動したとき
        /// </summary>
        private void picTargetGraphics_MouseMove(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                //クリップ領域を更新
                var cur = new Point((int) (e.X / this.tileSize.Width), (int) (e.Y / this.tileSize.Height));
                cur = Common.ApplyLimit(cur, this.tileRectangle);

                this.SelectedRange = Rectangle.FromLTRB(
                    (cur.X < this.selectedTileStartPosition.X) ? cur.X : this.selectedTileStartPosition.X,
                    (cur.Y < this.selectedTileStartPosition.Y) ? cur.Y : this.selectedTileStartPosition.Y,
                    (cur.X < this.selectedTileStartPosition.X) ? this.selectedTileStartPosition.X + 1 : cur.X + 1,
                    (cur.Y < this.selectedTileStartPosition.Y) ? this.selectedTileStartPosition.Y + 1 : cur.Y + 1
                );
                this.SelectedRange = Common.ApplyLimit(this.SelectedRange.Value, this.tileRectangle);

                this.picTargetGraphics.Refresh();
            }
        }

        /// <summary>
        /// パレット上でマウスホイールしたとき
        /// </summary>
        private void picTargetGraphics_MouseWheel(object sender, MouseEventArgs e) {
            this.AutoScrollPosition = new Point(0, -this.AutoScrollPosition.Y - e.Delta);
        }

        /// <summary>
        /// パレットにマウスカーソルが入ったとき
        /// </summary>
        private void picTargetGraphics_MouseEnter(object sender, EventArgs e) {
            this.Focus();
        }
    }
}
