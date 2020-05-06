using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgToolConvertTkoolMapTile : Form {
        private readonly string srcFileFullPath;           //編集対象のソースファイル

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgToolConvertTkoolMapTile(string _srcFileFullPath, string _destFileName, int afterTileSize) {
            this.InitializeComponent();
            this.srcFileFullPath = _srcFileFullPath;
            this.txtDestFileName.Text = _destFileName;
            this.cmbPartNum.SelectedIndex = 0;
            this.uctlFileTree.RootPath = Resources.Path_Materials;
            this.cmbTkoolVersion.SelectedIndex = Settings.Default.Tkool_TileVer;
            this.cmbInterpolation.SelectedIndex = Settings.Default.Tkool_Interpolation;
            this.numTileSize.Value = afterTileSize;
        }

        /// <summary>
        /// 対応ツクールバージョン
        /// </summary>
        private enum TkoolVersion {
            VX,
            MV,
        }

        /// <summary>
        /// 新規フォルダー作成
        /// </summary>
        private void tolNewFolder_Click(object sender, EventArgs e) {
            this.uctlFileTree.CreateFolder();
        }

        /// <summary>
        /// 入力情報を検証
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

            //ツクールのバージョンに合わせて元のタイルサイズを決定する
            var destTileSize = (int) this.numTileSize.Value;
            var srcTileSize = 24;
            switch ((TkoolVersion) this.cmbTkoolVersion.SelectedIndex) {
                case TkoolVersion.VX:
                    srcTileSize = 32;
                    break;
                case TkoolVersion.MV:
                    srcTileSize = 48;
                    break;
            }

            //実際に出力処理を行う
            var src = new Bitmap(Image.FromFile(this.srcFileFullPath));
            var failedFiles = new List<string>();
            Bitmap dest = null;
            Graphics g = null;

            //ベースファイルもセットでコピーする場合は以下をコメント解除すること
            //File.Copy(this.srcFileFullPath, filename, true);        

            switch (this.cmbPartNum.SelectedIndex + 1) {
                #region <パーツ１：海>
                case 1:
                    for (var gy = 0; gy < 4; gy++) {
                        var gx = 0;         //オートタイル１セット単位で横に数える（アニメーションタイルも分ける）
                        for (var px = 0; px < 4; px++) {
                            var id = px + gy * 4;
                            var isShallow = false;     //浅瀬かどうか

                            if (id == 1 || id == 5) {
                                isShallow = true;
                                dest = new Bitmap((int) (1 * this.numTileSize.Value), (int) (Common.GetEnumCount<Map.AutoTilePattern>() * this.numTileSize.Value));
                            } else {
                                dest = new Bitmap((int) (3 * this.numTileSize.Value), (int) (Common.GetEnumCount<Map.AutoTilePattern>() * this.numTileSize.Value));
                            }

                            g = Graphics.FromImage(dest);
                            Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);

                            if (isShallow) {
                                //浅瀬タイルは合成用タイルなので透過する
                                this.ConvertNormal(src, g, srcTileSize, destTileSize, gx, gy * 3, 1);

                                //pngは元々透過情報が入っているので以下の処理は不要
                                //dest.MakeTransparent(dest.GetPixel(0, 0));

                                gx++;
                            } else if (px % 2 == 1) {
                                //滝タイル
                                this.ConvertWaterfall(src, g, srcTileSize, destTileSize, gx, gy * 3);
                                gx++;
                            } else {
                                //海タイル
                                this.ConvertNormal(src, g, srcTileSize, destTileSize, gx, gy * 3, 3);
                                gx += 3;
                            }

                            try {
                                dest.Save(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{id + 1:D2}" + Path.GetExtension(filename), System.Drawing.Imaging.ImageFormat.Png);
                            } catch {
                                failedFiles.Add(Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{id + 1:D2}" + Path.GetExtension(filename));
                            }
                        }
                    }
                    break;
                #endregion

                #region <パーツ２：陸>
                case 2:
                    for (var gy = 0; gy < 4; gy++) {
                        for (var gx = 0; gx < 8; gx++) {
                            dest = new Bitmap((int) (1 * this.numTileSize.Value), (int) (Common.GetEnumCount<Map.AutoTilePattern>() * this.numTileSize.Value));
                            g = Graphics.FromImage(dest);
                            Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);

                            this.ConvertNormal(src, g, srcTileSize, destTileSize, gx, gy * 3, 1);
                            if (gx < 6 && gx % 3 > 0) {
                                //合成用タイルの場合は透過する
                                //pngは元々透過情報が入っているので以下の処理は不要
                                //dest.MakeTransparent(dest.GetPixel(0, 0));		
                            }

                            try {
                                dest.Save(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{gx + gy * 8 + 1:D2}" + Path.GetExtension(filename), System.Drawing.Imaging.ImageFormat.Png);
                            } catch {
                                failedFiles.Add(Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{gx + gy * 8 + 1:D2}" + Path.GetExtension(filename));
                            }
                        }
                    }
                    break;
                #endregion

                #region <パーツ３：建物>
                case 3:
                    for (var gy = 0; gy < 4; gy++) {
                        for (var gx = 0; gx < 8; gx++) {
                            dest = new Bitmap((int) (1 * this.numTileSize.Value), (int) (Common.GetEnumCount<Map.AutoTilePattern>() * this.numTileSize.Value));
                            g = Graphics.FromImage(dest);
                            Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);

                            this.ConvertSquare(src, g, srcTileSize, destTileSize, gx, gy * 2, 1);

                            try {
                                dest.Save(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{gx + gy * 8 + 1:D2}" + Path.GetExtension(filename), System.Drawing.Imaging.ImageFormat.Png);
                            } catch {
                                failedFiles.Add(Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{gx + gy * 8 + 1:D2}" + Path.GetExtension(filename));
                            }
                        }
                    }
                    break;
                #endregion

                #region <パーツ４：壁>
                case 4:
                    for (var gx = 0; gx < 8; gx++) {
                        var ty = 0;         //タイル単位
                        for (var gy = 0; gy < 6; gy++) {
                            dest = new Bitmap((int) (1 * this.numTileSize.Value), (int) (Common.GetEnumCount<Map.AutoTilePattern>() * this.numTileSize.Value));
                            g = Graphics.FromImage(dest);
                            Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);

                            if (gy % 2 == 0) {
                                //基本構造
                                this.ConvertNormal(src, g, srcTileSize, destTileSize, gx, ty, 1);
                                ty += 3;
                            } else {
                                //集合パターン
                                this.ConvertSquare(src, g, srcTileSize, destTileSize, gx, ty, 1);
                                ty += 2;
                            }

                            try {
                                dest.Save(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{gx + gy * 8 + 1:D2}" + Path.GetExtension(filename), System.Drawing.Imaging.ImageFormat.Png);
                            } catch {
                                failedFiles.Add(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{gx + gy * 8 + 1:D2}" + Path.GetExtension(filename));
                            }
                        }
                    }
                    break;
                #endregion

                #region <パーツB～E：上層用タイル>
                case 5:
                    //横に２セット並んでいるのでこれを分割する
                    for (var i = 0; i < 2; i++) {
                        dest = new Bitmap(Map.TileColumnCount * (int) this.numTileSize.Value, 16 * (int) this.numTileSize.Value);
                        g = Graphics.FromImage(dest);
                        Common.ApplyInterpolation(g, (PictureInterPolation) this.cmbInterpolation.SelectedIndex);

                        g.DrawImage(src,
                            new Rectangle(0, 0, dest.Width, dest.Height),
                            new Rectangle(i * Map.TileColumnCount * srcTileSize, 0, Map.TileColumnCount * srcTileSize, 16 * srcTileSize),
                            GraphicsUnit.Pixel
                        );

                        try {
                            dest.Save(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{i + 1}" + Path.GetExtension(filename), System.Drawing.Imaging.ImageFormat.Png);
                        } catch {
                            failedFiles.Add(Path.GetDirectoryName(filename) + "\\" + Path.GetFileNameWithoutExtension(filename) + $"_{this.cmbPartNum.SelectedIndex + 1}-{i + 1}" + Path.GetExtension(filename));
                        }
                    }
                    break;
                    #endregion
            }

            if (failedFiles.Count == 0) {
                MessageBox.Show("変換が完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } else {
                MessageBox.Show($"I/Oエラーのため、{failedFiles.Count} 件 変換に失敗しています。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.DialogResult = DialogResult.None;
            }
            g.Dispose();
            src.Dispose();
            dest.Dispose();

            //設定を保存
            Settings.Default.Tkool_TileVer = this.cmbTkoolVersion.SelectedIndex;
            Settings.Default.Tkool_Interpolation = this.cmbInterpolation.SelectedIndex;
        }

        /// <summary>
        /// オートタイル基本構造の変換
        /// </summary>
        private void ConvertNormal(Image src, Graphics g, int srcTileSize, int destTileSize, int gx, int ty, int animLength) {
            for (var p = 0; p < Common.GetEnumCount<Map.AutoTilePattern>(); p++) {
                for (var i = 0; i < animLength; i++) {
                    switch ((Map.AutoTilePattern) p) {
                        case Map.AutoTilePattern.Isolate:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2,
                                    ty * srcTileSize,
                                    srcTileSize, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.Vertical:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize / 2, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2,
                                    ty * srcTileSize + srcTileSize + srcTileSize / 2,
                                    srcTileSize / 2, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize + destTileSize / 2,
                                    p * destTileSize,
                                    destTileSize / 2, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2 * 3,
                                    ty * srcTileSize + srcTileSize + srcTileSize / 2,
                                    srcTileSize / 2, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.Horizontal:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize / 2
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + srcTileSize,
                                    srcTileSize, srcTileSize / 2
                                ),
                                GraphicsUnit.Pixel
                            );
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize + destTileSize / 2,
                                    destTileSize, destTileSize / 2
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + srcTileSize * 3 - srcTileSize / 2,
                                    srcTileSize, srcTileSize / 2
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.FourSide:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize,
                                    ty * srcTileSize,
                                    srcTileSize, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.AllSide:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + srcTileSize + srcTileSize / 2,
                                    srcTileSize, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;
                    }
                }
            }
        }

        /// <summary>
        /// オートタイル集合パターンの変換
        /// </summary>
        private void ConvertSquare(Image src, Graphics g, int srcTileSize, int destTileSize, int gx, int ty, int animLength) {
            for (var p = 0; p < Common.GetEnumCount<Map.AutoTilePattern>(); p++) {
                for (var i = 0; i < animLength; i++) {
                    switch ((Map.AutoTilePattern) p) {
                        case Map.AutoTilePattern.Isolate:
                            //四隅を集める
                            for (var n = 0; n < 4; n++) {
                                g.DrawImage(src,
                                    new Rectangle(
                                        i * destTileSize + destTileSize / 2 * (n % 2),
                                        p * destTileSize + destTileSize / 2 * (n / 2),
                                        destTileSize / 2, destTileSize / 2
                                    ),
                                    new Rectangle(
                                        (gx + i) * srcTileSize * 2 + srcTileSize / 2 * 3 * (n % 2),
                                        ty * srcTileSize + srcTileSize / 2 * 3 * (n / 2),
                                        srcTileSize / 2, srcTileSize / 2
                                    ),
                                    GraphicsUnit.Pixel
                                );
                            }
                            break;

                        case Map.AutoTilePattern.Vertical:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize / 2, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2,
                                    ty * srcTileSize + srcTileSize * 2 / 3 - 5,
                                    srcTileSize / 2, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize + destTileSize / 2,
                                    p * destTileSize,
                                    destTileSize / 2, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2 * 3,
                                    ty * srcTileSize + srcTileSize * 2 / 3 - 5,
                                    srcTileSize / 2, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.Horizontal:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize / 2
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize,
                                    srcTileSize, srcTileSize / 2
                                ),
                                GraphicsUnit.Pixel
                            );
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize + destTileSize / 2,
                                    destTileSize, destTileSize / 2
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + srcTileSize * 2 - srcTileSize / 2,
                                    srcTileSize, srcTileSize / 2
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.FourSide:
                        case Map.AutoTilePattern.AllSide:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize
                                ),
                                new Rectangle(
                                    (gx + i) * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + srcTileSize / 2,
                                    srcTileSize, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;
                    }
                }
            }
        }

        /// <summary>
        /// 滝パーツの変換
        /// </summary>
        private void ConvertWaterfall(Image src, Graphics g, int srcTileSize, int destTileSize, int gx, int ty) {
            for (var p = 0; p < Common.GetEnumCount<Map.AutoTilePattern>(); p++) {
                for (var i = 0; i < 3; i++) {
                    switch ((Map.AutoTilePattern) p) {
                        case Map.AutoTilePattern.Isolate:
                        case Map.AutoTilePattern.Vertical:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize / 2, destTileSize
                                ),
                                new Rectangle(
                                    gx * srcTileSize * 2,
                                    ty * srcTileSize + i * srcTileSize,
                                    srcTileSize / 2, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize + destTileSize / 2,
                                    p * destTileSize,
                                    destTileSize / 2, destTileSize
                                ),
                                new Rectangle(
                                    (gx + 1) * srcTileSize * 2 - srcTileSize / 2,
                                    ty * srcTileSize + i * srcTileSize,
                                    srcTileSize / 2, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.Horizontal:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize / 2
                                ),
                                new Rectangle(
                                    gx * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + i * srcTileSize,
                                    srcTileSize, srcTileSize / 2
                                ),
                                GraphicsUnit.Pixel
                            );
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize + destTileSize / 2,
                                    destTileSize, destTileSize / 2
                                ),
                                new Rectangle(
                                    gx * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + srcTileSize / 2 + i * srcTileSize,
                                    srcTileSize, srcTileSize / 2
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;

                        case Map.AutoTilePattern.FourSide:
                        case Map.AutoTilePattern.AllSide:
                            g.DrawImage(src,
                                new Rectangle(
                                    i * destTileSize,
                                    p * destTileSize,
                                    destTileSize, destTileSize
                                ),
                                new Rectangle(
                                    gx * srcTileSize * 2 + srcTileSize / 2,
                                    ty * srcTileSize + i * srcTileSize,
                                    srcTileSize, srcTileSize
                                ),
                                GraphicsUnit.Pixel
                            );
                            break;
                    }
                }
            }
        }
    }
}
