using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgToolConvertTkoolWalkChar : Form {
        private readonly string srcFileFullPath;           //編集対象のソースファイル

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgToolConvertTkoolWalkChar(string _srcFileFullPath, string _destFileName, Size afterTileSize) {
            this.InitializeComponent();
            this.srcFileFullPath = _srcFileFullPath;
            this.txtDestFileName.Text = _destFileName;
            this.uctlCharSize.Result = new Point(afterTileSize);
            this.uctlFileTree.RootPath = Resources.Path_Materials;
            this.cmbTkoolVersion.SelectedIndex = Settings.Default.Tkool_WalkVer;

            //歩行グラフィックの変換で使用できる補間方法は最近傍補間に限定する
            this.cmbInterpolation.SelectedIndex = (int) PictureInterPolation.None;
            this.cmbInterpolation.Enabled = false;
        }

        /// <summary>
        /// 対応ツクールバージョン
        /// </summary>
        private enum TkoolVersion {
            v2000_2003,
            VX_MV,
        }

        /// <summary>
        /// ツクール2000/2003の方向順序
        /// </summary>
        private enum OldTkoolDirection4 {
            North,
            East,
            South,
            West,
            None,
        }

        /// <summary>
        /// ４方向をツクール2000/2003の方向順序に変換します。
        /// </summary>
        /// <param name="newDir4">現在の規格の４方向</param>
        /// <returns>旧規格の４方向</returns>
        private OldTkoolDirection4 ConvertToOldDirection(Map.Direction4 newDir4) {
            switch (newDir4) {
                case Map.Direction4.East:
                    return OldTkoolDirection4.East;
                case Map.Direction4.West:
                    return OldTkoolDirection4.West;
                case Map.Direction4.South:
                    return OldTkoolDirection4.South;
                case Map.Direction4.North:
                    return OldTkoolDirection4.North;
            }
            return OldTkoolDirection4.None;
        }

        /// <summary>
        /// 新規フォルダー作成
        /// </summary>
        private void tolNewFolder_Click(object sender, EventArgs e) {
            this.uctlFileTree.CreateFolder();
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (!Common.CheckEnabledPath(this.txtDestFileName.Text)) {
                //ファイル名に使えない文字は無効
                MessageBox.Show(Resources.MsgE_FileNameInvarid.Replace("$", "ファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return;
            } else if (!this.uctlFileTree.IsSelected) {
                //フォルダーが選択されていない
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "保存先フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return;
            }

            //上書き確認
            var filename = ProjectManager.ProjectPath + Common.AddToPathSplit(this.uctlFileTree.SelectedNodeFullPath) + this.txtDestFileName.Text;
            if (File.Exists(filename)) {
                if (MessageBox.Show("同名のファイルが存在します。" + Resources.MsgQ_Confirm.Replace("$", "上書き"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                    this.DialogResult = DialogResult.None;
                    return;
                }
            }

            //実際に出力処理を行う
            var src = new Bitmap(Image.FromFile(this.srcFileFullPath));
            var failed = false;
            Bitmap dest = null;
            Graphics g = null;
            var oneCharSize = new Size(src.Width / 4 / 3, src.Height / 2 / 4);
            var oneCharAfterHeight = (int) (((double) this.uctlCharSize.Result.X / oneCharSize.Width) * oneCharSize.Height);       //アスペクト比を維持するために高さは自動で調整する

            //ベースファイルもセットでコピーする場合は以下をコメント解除すること
            //File.Copy(this.srcFileFullPath, filename, true);        

            //キャラクター単位
            for (var cx = 0; cx < 4; cx++) {
                for (var cy = 0; cy < 2; cy++) {
                    dest = new Bitmap(this.uctlCharSize.Result.X * 3, this.uctlCharSize.Result.Y * 4);
                    g = Graphics.FromImage(dest);
                    Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);

                    //パーツ単位: pxはアニメパターン、pyは方向に対応
                    for (var px = 0; px < 3; px++) {
                        for (var py = 0; py < 4; py++) {
                            switch ((TkoolVersion) this.cmbTkoolVersion.SelectedIndex) {
                                case TkoolVersion.v2000_2003:
                                    //向きを並び替える必要がある
                                    g.DrawImage(src,
                                        new Rectangle(px * this.uctlCharSize.Result.X, py * this.uctlCharSize.Result.Y + (this.uctlCharSize.Result.Y - oneCharAfterHeight), this.uctlCharSize.Result.X, oneCharAfterHeight),
                                        new Rectangle(cx * oneCharSize.Width * 3 + px * oneCharSize.Width, cy * oneCharSize.Height * 4 + (int) this.ConvertToOldDirection((Map.Direction4) py) * oneCharSize.Height, oneCharSize.Width, oneCharSize.Height),
                                        GraphicsUnit.Pixel
                                    );
                                    break;

                                case TkoolVersion.VX_MV:
                                    //そのまま対応
                                    g.DrawImage(src,
                                        new Rectangle(px * this.uctlCharSize.Result.X, py * this.uctlCharSize.Result.Y + (this.uctlCharSize.Result.Y - oneCharAfterHeight), this.uctlCharSize.Result.X, oneCharAfterHeight),
                                        new Rectangle(cx * oneCharSize.Width * 3 + px * oneCharSize.Width, cy * oneCharSize.Height * 4 + py * oneCharSize.Height, oneCharSize.Width, oneCharSize.Height),
                                        GraphicsUnit.Pixel
                                    );
                                    break;
                            }
                        }
                    }

                    //左上の１ピクセルを透明色として認識する
                    dest.MakeTransparent(src.GetPixel(0, 0));

                    //書き出し処理
                    try {
                        dest.Save(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{cx + cy * 4 + 1}" + Path.GetExtension(filename), System.Drawing.Imaging.ImageFormat.Png);
                    } catch {
                        failed = true;
                        goto AfterConvert;
                    } finally {
                        dest.Dispose();
                    }
                }
            }

            AfterConvert:
            if (!failed) {
                MessageBox.Show("変換が完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } else {
                MessageBox.Show($"I/Oエラーのため、変換に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.DialogResult = DialogResult.None;
            }
            g.Dispose();
            src.Dispose();

            //設定を保存
            Settings.Default.Tkool_WalkVer = this.cmbTkoolVersion.SelectedIndex;
            Settings.Default.Tkool_Interpolation = this.cmbInterpolation.SelectedIndex;
        }
    }
}
