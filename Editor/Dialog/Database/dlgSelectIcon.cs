using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Database {
    public partial class dlgSelectIcon : Form {
        private readonly Image img;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgSelectIcon(Image img, Size tileSize, Point defaultPosition) {
            this.InitializeComponent();
            this.img = img;
            this.uctlSelectTile.Setup(img, tileSize, defaultPosition);
        }

        /// <summary>
        /// 結果を返します。
        /// タイル座標とアイコン画像をセットにします。
        /// </summary>
        public KeyValuePair<Point, Image> Result {
            get {
                var bmp = new Bitmap(this.uctlSelectTile.TileSize.Width, this.uctlSelectTile.TileSize.Height);
                var g = Graphics.FromImage(bmp);
                g.DrawImage(this.img, new Rectangle(Point.Empty, this.uctlSelectTile.TileSize), new Rectangle(this.uctlSelectTile.Position.X * this.uctlSelectTile.TileSize.Width, this.uctlSelectTile.Position.Y * this.uctlSelectTile.TileSize.Height, this.uctlSelectTile.TileSize.Width, this.uctlSelectTile.TileSize.Height), GraphicsUnit.Pixel);
                g.Dispose();
                this.img.Dispose();
                return new KeyValuePair<Point, Image>(this.uctlSelectTile.Position, bmp);
            }
        }

        /// <summary>
        /// 選択タイルを移動させる
        /// </summary>
        private void dlgSelectIcon_KeyDown(object sender, KeyEventArgs e) {
            switch (e.KeyCode) {
                case Keys.Right:
                    this.uctlSelectTile.MovePosition(AnchorStyles.Right);
                    break;
                case Keys.Down:
                    this.uctlSelectTile.MovePosition(AnchorStyles.Bottom);
                    break;
                case Keys.Left:
                    this.uctlSelectTile.MovePosition(AnchorStyles.Left);
                    break;
                case Keys.Up:
                    this.uctlSelectTile.MovePosition(AnchorStyles.Top);
                    break;
            }
        }
    }
}
