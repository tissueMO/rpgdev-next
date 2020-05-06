using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;

    public partial class ctlSelectTile : UserControl {
        public event EventHandler TileDoubleClicked;        //タイルがダブルクリックされたときに発生するイベント

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlSelectTile() {
            this.InitializeComponent();
        }

        /// <summary>
        /// 選択タイル位置
        /// </summary>
        public Point Position {
            get {
                return this.position;
            }
            set {
                this.position = value;
                this.picSrc.Refresh();
            }
        }
        private Point position = Point.Empty;

        /// <summary>
        /// １タイルあたりのサイズ
        /// </summary>
        public Size TileSize {
            get; set;
        }

        /// <summary>
        /// ソース画像を設定します。
        /// </summary>
        public void Setup(Image img, Size tileSize, Point defaultPosition) {
            this.picSrc.Image = img;
            this.TileSize = tileSize;
            this.Position = defaultPosition;
        }

        /// <summary>
        /// タイル位置を指定した方向に一つ移動させます。
        /// </summary>
        public void MovePosition(AnchorStyles direction) {
            switch (direction) {
                case AnchorStyles.Right:
                    this.Position = new Point(this.Position.X + 1, this.Position.Y);
                    break;
                case AnchorStyles.Bottom:
                    this.Position = new Point(this.Position.X, this.Position.Y + 1);
                    break;
                case AnchorStyles.Left:
                    this.Position = new Point(this.Position.X - 1, this.Position.Y);
                    break;
                case AnchorStyles.Top:
                    this.Position = new Point(this.Position.X, this.Position.Y - 1);
                    break;
            }
        }

        /// <summary>
        /// タイル位置を直接指定します。
        /// </summary>
        private void picSrc_MouseClick(object sender, MouseEventArgs e) {
            this.Position = new Point(e.X / this.TileSize.Width, e.Y / this.TileSize.Height);
            this.picSrc.Refresh();
        }

        /// <summary>
        /// 選択位置にカーソルを描画します。
        /// </summary>
        private void picSrc_Paint(object sender, PaintEventArgs e) {
            Common.ApplyHighQualityDrawing(e.Graphics);

            e.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            e.Graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.Default;
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.Default;

            for (var i = 1; i <= 2; i++) {
                e.Graphics.DrawRectangle(Pens.White, this.Position.X * this.TileSize.Width + i, this.Position.Y * this.TileSize.Height + i, this.TileSize.Width - i * 2, this.TileSize.Height - i * 2);
            }

            e.Graphics.DrawRectangle(Pens.Black, this.Position.X * this.TileSize.Width, this.Position.Y * this.TileSize.Height, this.TileSize.Width, this.TileSize.Height);
            e.Graphics.DrawRectangle(Pens.Black, this.Position.X * this.TileSize.Width + 3, this.Position.Y * this.TileSize.Height + 3, this.TileSize.Width - 3 * 2, this.TileSize.Height - 3 * 2);
        }

        /// <summary>
        /// ダブルクリックされたらイベントを発生させる
        /// </summary>
        private void picSrc_MouseDoubleClick(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                this.TileDoubleClicked?.Invoke(sender, e);
            }
        }

        /// <summary>
        /// マウスカーソルがコントロールに入ると自動でタイル一覧にフォーカスを当てる
        /// </summary>
        private void picSrc_MouseEnter(object sender, EventArgs e) {
            this.picSrc.Focus();
        }
    }
}
