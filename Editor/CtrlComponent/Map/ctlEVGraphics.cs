using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;

    public partial class ctlEVGraphics : UserControl {
        public event EventHandler GraphicsChanged;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlEVGraphics() {
            this.InitializeComponent();
        }

        /// <summary>
        /// タイルセットを有効にするか
        /// </summary>
        public bool EnabledTileset {
            get {
                return this.enabledTileset;
            }
            set {
                this.enabledTileset = value;
                if (!value) {
                    this.OnlyTileset = false;
                }
            }
        }
        private bool enabledTileset = false;

        /// <summary>
        /// タイルセットだけを選択するモードにするか
        /// </summary>
        public bool OnlyTileset {
            get {
                return this.onlyTileset;
            }
            set {
                this.onlyTileset = value;
                if (value) {
                    this.EnabledTileset = true;
                }
            }
        }
        private bool onlyTileset = false;

        /// <summary>
        /// 選択された画像ファイル名（プロジェクトフォルダーを基準としたパス）
        /// </summary>
        public string FileName {
            get {
                return this.fileName;
            }
            set {
                this.fileName = value;
                this.reloadImage();
            }
        }
        private string fileName = "";

        /// <summary>
        /// タイルセットが選択されているか
        /// </summary>
        public bool IsTilesetSelected {
            get {
                return (this.FileName?.IndexOf(Resources.Split_IDNamePair) != -1);
            }
        }

        /// <summary>
        /// 画像の再表示
        /// </summary>
        private void reloadImage() {
            if (this.IsTilesetSelected) {
                //タイル表示
                var tileset = ctlTilePallet.PalletGraphics;
                if (tileset == null) {
                    return;
                }

                //選択タイルをセットする
                var tilePos = Common.StringToPoint(this.FileName);
                var tempBmp = new Bitmap(mgrMapObject.TileSize, mgrMapObject.TileSize);
                var g = Graphics.FromImage(tempBmp);

                g.DrawImage(
                    tileset,
                    new Rectangle(0, 0, tempBmp.Width, tempBmp.Height),
                    new Rectangle(tilePos.X * mgrMapObject.TileSize, tilePos.Y * mgrMapObject.TileSize, mgrMapObject.TileSize, mgrMapObject.TileSize),
                    GraphicsUnit.Pixel
                );

                g.Dispose();
                this.picTargetGraphics.SizeMode = PictureBoxSizeMode.CenterImage;
                this.picTargetGraphics.Image = tempBmp;
            } else {
                //キャラ表示
                if (!File.Exists(ProjectManager.ProjectPath + this.FileName)) {
                    this.picTargetGraphics.Image = null;
                    return;
                }

                //１キャラ分を表示する
                var srcBmp = new Bitmap(ProjectManager.ProjectPath + this.FileName);
                var tempBmp = new Bitmap(srcBmp.Width / (int) Map.EventAnimation.Count, srcBmp.Height / (Common.GetEnumCount<Map.Direction4>() - 1));
                var g = Graphics.FromImage(tempBmp);

                g.DrawImage(
                    srcBmp,
                    new Rectangle(0, 0, tempBmp.Width, tempBmp.Height),
                    new Rectangle((int) Map.EventAnimation.Stop * tempBmp.Width, (int) Map.Direction4.South * tempBmp.Height, tempBmp.Width, tempBmp.Height),
                    GraphicsUnit.Pixel
                );

                g.Dispose();
                this.picTargetGraphics.SizeMode = PictureBoxSizeMode.Zoom;
                this.picTargetGraphics.Image = tempBmp;
            }
        }

        /// <summary>
        /// 画像の変更
        /// </summary>
        private void TargetGraphics_MouseClick(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Right) {
                //右クリックで削除する
                this.picTargetGraphics.Image = null;
                this.FileName = "";
                this.GraphicsChanged?.Invoke(this, null);
                return;
            }

            var Dlg = new Dialog.Common.dlgFileSelect(Resources.Path_Materials, Resources.Extension_Graphics, this.EnabledTileset, true, this.OnlyTileset, this.FileName);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.FileName = Dlg.FileName;
                this.reloadImage();
                this.GraphicsChanged?.Invoke(this, null);
            }
        }
    }
}
