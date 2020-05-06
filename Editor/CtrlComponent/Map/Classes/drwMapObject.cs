using DxLibDLL;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;

    /// <summary>
    /// DXライブラリの描画対象オブジェクト
    /// </summary>
    public class drwMapObject : Media.DrawTarget {
        protected mgrMapObject parent = null;
        protected drwDXTilePallet pallet = null;
        protected int[] Fonts;
        protected int tilesetFixedID = -1;

        /// <summary>
        /// マップ上で表示される特殊オブジェクトの種別
        /// </summary>
        private enum SpecialFont {
            StartMark,
            EVMark,
            QuarterDebug,
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public drwMapObject(Control ctl, mgrMapObject parent, drwDXTilePallet pallet, int tilesetFixedID) : base(ctl, Media.DXInterpolation.Nearest) {
            this.parent = parent;
            this.pallet = pallet;
            this.Fonts = new int[Common.GetEnumCount<SpecialFont>()];
            this.tilesetFixedID = tilesetFixedID;
        }

        public override void Draw() {
            if (this.parent.MapData == null) {
                return;
            }

            //ユーザーによって急にスクロールされると描画されない部分がちょっと見えてしまうので、ちょっと多めに描画する
            const int margin = 2;

            var rawTileSize = (int) (this.parent.ScaledTileSize / ViewScale.GetRate(this.parent.Scale));
            var left = this.parent.ViewRange.Left / this.parent.ScaledTileSize;
            var top = this.parent.ViewRange.Top / this.parent.ScaledTileSize;

            //各種描画処理
            this.drawVisibleRangeTiles(margin, left, top);
            this.drawTemporaryTiles();
            this.drawGrid();
            this.drawVisibleEVs(rawTileSize);
            this.drawMapFrame();
            this.drawStartPoint();
            this.drawCursor();
        }

        /// <summary>
        /// カーソル描画：黒枠の白矩形
        /// </summary>
        private void drawCursor() {
            if (this.parent.CurrentLayer == (int) Map.Layer.Shadow) {
                //影層のときは描画しない
                return;
            }

            var curPos = (this.parent.ClippingTileStart == null) ? this.parent.Cursor : this.parent.ClippingTileRange.Location;
            var curSize = (this.parent.ClippingTiles == null) ? new Size(1, 1) : new Size(this.parent.ClippingTiles.GetLength(0), this.parent.ClippingTiles.GetLength(1));

            for (var i = 1; i <= 2; i++) {
                DX.DrawBox(
                    curPos.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + i,
                    curPos.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + i,
                    (curPos.X + curSize.Width) * this.parent.ScaledTileSize - this.parent.ViewRange.Left - i,
                    (curPos.Y + curSize.Height) * this.parent.ScaledTileSize - this.parent.ViewRange.Top - i,
                    DX.GetColor(255, 255, 255), DX.FALSE);
            }

            DX.DrawBox(
                curPos.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                curPos.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                (curPos.X + curSize.Width) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                (curPos.Y + curSize.Height) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                DX.GetColor(0, 0, 0), DX.FALSE);
            DX.DrawBox(
                curPos.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + 3,
                curPos.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + 3,
                (curPos.X + curSize.Width) * this.parent.ScaledTileSize - this.parent.ViewRange.Left - 3,
                (curPos.Y + curSize.Height) * this.parent.ScaledTileSize - this.parent.ViewRange.Top - 3,
                DX.GetColor(0, 0, 0), DX.FALSE);
        }

        /// <summary>
        /// 初期位置カーソル描画：白枠の半透明Ｓ
        /// </summary>
        protected virtual void drawStartPoint() {
            DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, 80);
            DX.DrawBox(
                this.parent.StartPosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + 1,
                this.parent.StartPosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + 1,
                (this.parent.StartPosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left - 1,
                (this.parent.StartPosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top - 1,
                DX.GetColor(0, 0, 0), DX.TRUE);

            DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
            DX.DrawBox(
                this.parent.StartPosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                this.parent.StartPosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                (this.parent.StartPosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                (this.parent.StartPosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                DX.GetColor(255, 255, 255), DX.FALSE);

            if (ViewScale.GetRate(this.parent.Scale) >= 0.5) {
                //縮小しすぎているときは S を表示しない
                DX.DrawStringToHandle(
                    this.parent.StartPosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + 1,
                    this.parent.StartPosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + 1,
                    "Ｓ", DX.GetColor(0, 0, 255), this.Fonts[(int) SpecialFont.StartMark], DX.GetColor(255, 255, 255)
                );
            }
        }

        /// <summary>
        /// 外枠を描画
        /// </summary>
        protected virtual void drawMapFrame() {
            //左端
            DX.DrawLine(
                -this.parent.ViewRange.Left,
                -this.parent.ViewRange.Top,
                -this.parent.ViewRange.Left,
                (this.parent.ViewRange.Top + this.parent.ViewRange.Height <= this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize) ? this.parent.ViewRange.Height : this.parent.ViewRange.Height - ((this.parent.ViewRange.Top + this.parent.ViewRange.Height) - (this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize)),
                DX.GetColor(255, 255, 255), 2);

            //上端
            DX.DrawLine(
                -this.parent.ViewRange.Left,
                -this.parent.ViewRange.Top,
                (this.parent.ViewRange.Left + this.parent.ViewRange.Width <= this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize) ? this.parent.ViewRange.Width : this.parent.ViewRange.Width - ((this.parent.ViewRange.Left + this.parent.ViewRange.Width) - (this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize)),
                -this.parent.ViewRange.Top,
                DX.GetColor(255, 255, 255), 2);

            //右端
            DX.DrawLine(
                this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                -this.parent.ViewRange.Top,
                this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                (this.parent.ViewRange.Top + this.parent.ViewRange.Height <= this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize) ? this.parent.ViewRange.Height : this.parent.ViewRange.Height - ((this.parent.ViewRange.Top + this.parent.ViewRange.Height) - (this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize)),
                DX.GetColor(255, 255, 255), 2);

            //下端
            DX.DrawLine(
                -this.parent.ViewRange.Left,
                this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                (this.parent.ViewRange.Left + this.parent.ViewRange.Width <= this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize) ? this.parent.ViewRange.Width : this.parent.ViewRange.Width - ((this.parent.ViewRange.Left + this.parent.ViewRange.Width) - (this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize)),
                this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                DX.GetColor(255, 255, 255), 2);
        }

        /// <summary>
        /// 可視領域のイベントを描画
        /// </summary>
        protected virtual void drawVisibleEVs(int rawTileSize) {
            //現在のレイヤーがイベントレイヤーでなければ暗くする
            if (this.parent.SwitchLayers[(int) Map.Layer.Event]) {
                for (var i = 0; i < this.parent.MapData.EVCount; i++) {
                    var ev = this.parent.MapData[i];

                    if (ev.TilePosition.X < this.parent.ViewRange.Left / this.parent.ScaledTileSize
                    || this.parent.ViewRange.Right / this.parent.ScaledTileSize < ev.TilePosition.X
                    || ev.TilePosition.Y < this.parent.ViewRange.Top / this.parent.ScaledTileSize
                    || this.parent.ViewRange.Bottom / this.parent.ScaledTileSize < ev.TilePosition.Y) {
                        //範囲外の場合は描画しない
                        continue;
                    }

                    //イベント下地を描画
                    DX.SetDrawBlendMode(
                        DX.DX_BLENDMODE_ALPHA,
                        (this.parent.CurrentLayer == (int) Map.Layer.Event) ? 80 : 80 - Settings.Default.Map_NonActiveLayerTransparency * 80 / 255
                    );
                    DX.DrawBox(
                        ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + 1,
                        ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + 1,
                        (ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left - 1,
                        (ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top - 1,
                        DX.GetColor(0, 0, 0), DX.TRUE
                    );

                    if (this.parent.CurrentLayer == (int) Map.Layer.Event) {
                        DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
                    } else {
                        DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, byte.MaxValue - Settings.Default.Map_NonActiveLayerTransparency);
                    }

                    //イベントグラフィックを描画
                    if (string.IsNullOrEmpty(ev.Pages[0].Graphic) && ViewScale.GetRate(this.parent.Scale) >= 0.5) {
                        //グラフィックをもたない：白枠の半透明EV・縮小しすぎているときは表示しない
                        DX.DrawStringToHandle(
                            ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + 1,
                            ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + 1,
                            "EV", DX.GetColor(255, 0, 0), this.Fonts[(int) SpecialFont.EVMark], DX.GetColor(255, 255, 255)
                        );
                    } else if (ev.Pages[0].Graphic.IndexOf(Resources.Split_IDNamePair) != -1) {
                        //タイルグラフィックを持つ：オートタイルは代表タイルで描画
                        var srcPos = Common.StringToPoint(ev.Pages[0].Graphic);

                        DX.DrawRectExtendGraph(
                            ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                            ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                            ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + this.parent.ScaledTileSize,
                            ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + this.parent.ScaledTileSize,
                            srcPos.X * rawTileSize,
                            srcPos.Y * rawTileSize,
                            rawTileSize,
                            rawTileSize,
                            this.pallet.dxPalletHandle,
                            DX.TRUE
                        );
                    } else {
                        DX.GetGraphSize(ev.dxHandle, out var width, out var height);
                        DX.DrawRectExtendGraph(
                            ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                            ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                            ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left + this.parent.ScaledTileSize,
                            ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top + this.parent.ScaledTileSize,
                            width / (int) Map.EventAnimation.Count * 1, height / (Common.GetEnumCount<Map.Direction4>() - 1) * 0,
                            width / (int) Map.EventAnimation.Count, height / (Common.GetEnumCount<Map.Direction4>() - 1),
                            ev.dxHandle, DX.TRUE
                        );
                    }

                    //イベント外枠
                    DX.DrawBox(
                        ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                        ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                        (ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                        (ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                        DX.GetColor(255, 255, 255), DX.FALSE
                    );

                    //接触範囲
                    if (!ev.Pages[0].HitRect.IsEmpty) {
                        //イベント外枠からの接続線 [LT, RT, LB, RB の順]
                        DX.DrawLine(
                            (ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left) - ev.Pages[0].HitRect.Width * this.parent.ScaledTileSize,
                            (ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top) - ev.Pages[0].HitRect.Height * this.parent.ScaledTileSize,
                          ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                          ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                          DX.GetColor(255, 0, 0)
                        );
                        DX.DrawLine(
                            ((ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left),
                            ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                            ((ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left) + ev.Pages[0].HitRect.Width * this.parent.ScaledTileSize,
                            (ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top) - ev.Pages[0].HitRect.Height * this.parent.ScaledTileSize,
                           DX.GetColor(255, 0, 0)
                        );
                        DX.DrawLine(
                            ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                            (ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                            (ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left) - ev.Pages[0].HitRect.Width * this.parent.ScaledTileSize,
                            ((ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top) + ev.Pages[0].HitRect.Height * this.parent.ScaledTileSize,
                           DX.GetColor(255, 0, 0)
                        );
                        DX.DrawLine(
                            (ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                            (ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                            ((ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left) + ev.Pages[0].HitRect.Width * this.parent.ScaledTileSize,
                            ((ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top) + ev.Pages[0].HitRect.Height * this.parent.ScaledTileSize,
                            DX.GetColor(255, 0, 0)
                        );

                        //接触範囲枠
                        DX.DrawBox(
                            (ev.TilePosition.X * this.parent.ScaledTileSize - this.parent.ViewRange.Left) - ev.Pages[0].HitRect.Width * this.parent.ScaledTileSize,
                            (ev.TilePosition.Y * this.parent.ScaledTileSize - this.parent.ViewRange.Top) - ev.Pages[0].HitRect.Height * this.parent.ScaledTileSize,
                            ((ev.TilePosition.X + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left) + ev.Pages[0].HitRect.Width * this.parent.ScaledTileSize,
                            ((ev.TilePosition.Y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top) + ev.Pages[0].HitRect.Height * this.parent.ScaledTileSize,
                          DX.GetColor(255, 0, 0), DX.FALSE
                        );
                    }
                }
            }
            DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
        }

        /// <summary>
        /// グリッドを描画
        /// </summary>
        protected virtual void drawGrid() {
            if (this.parent.VisibleGrid) {
                //縦線
                for (int i = this.parent.ViewRange.Left / this.parent.ScaledTileSize, x = -(this.parent.ViewRange.Left % this.parent.ScaledTileSize); x < this.parent.ViewRange.Width && x + this.parent.ViewRange.Left < this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize; i++, x += this.parent.ScaledTileSize) {
                    DX.DrawLine(x, 0, x, (this.parent.ViewRange.Top + this.parent.ViewRange.Height < this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize) ? this.parent.ViewRange.Height : this.parent.ViewRange.Height - ((this.parent.ViewRange.Top + this.parent.ViewRange.Height) - (this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize)), DX.GetColor(100, 100, 100));
                }
                //横線
                for (int i = this.parent.ViewRange.Top / this.parent.ScaledTileSize, y = -(this.parent.ViewRange.Top % this.parent.ScaledTileSize); y < this.parent.ViewRange.Height && y + this.parent.ViewRange.Top < this.parent.MapData.MapSize.Height * this.parent.ScaledTileSize; i++, y += this.parent.ScaledTileSize) {
                    DX.DrawLine(0, y, (this.parent.ViewRange.Left + this.parent.ViewRange.Width < this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize) ? this.parent.ViewRange.Width : this.parent.ViewRange.Width - ((this.parent.ViewRange.Left + this.parent.ViewRange.Width) - (this.parent.MapData.MapSize.Width * this.parent.ScaledTileSize)), y, DX.GetColor(100, 100, 100));
                }
            }
        }

        /// <summary>
        /// 確定前のタイルを描画
        /// </summary>
        protected virtual void drawTemporaryTiles() {
            DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
            if (this.parent.PuttingTileStart != null) {
                //実際の描画領域を生成する
                var tempPath = new System.Drawing.Drawing2D.GraphicsPath();
                switch (this.parent.EditMode) {
                    case mgrMapObject.TileEditMode.Rectangle:
                        tempPath.AddRectangle(this.parent.PuttingTileRange);
                        break;
                    case mgrMapObject.TileEditMode.Ellipse:
                        tempPath.AddEllipse(this.parent.PuttingTileRange.X - 1, this.parent.PuttingTileRange.Y - 1, this.parent.PuttingTileRange.Width + 1, this.parent.PuttingTileRange.Height + 1);
                        break;
                }

                //確定前のタイルを描画する
                for (int a = 0, x = this.parent.PuttingTileRange.Left; x < this.parent.PuttingTileRange.Right; x++, a++) {
                    for (int b = 0, y = this.parent.PuttingTileRange.Top; y < this.parent.PuttingTileRange.Bottom; y++, b++) {
                        if (!tempPath.IsVisible(x, y)) {
                            //描画領域に該当しない部分はスキップする
                            continue;
                        }

                        //単一タイルを描画
                        if (this.parent.ClippingTiles != null) {
                            //クリッピングしている場合
                            this.drawTileWithAutotilePattern(x, y, this.parent.ClippingTiles[a % this.parent.ClippingTiles.GetLength(0), b % this.parent.ClippingTiles.GetLength(1)].PalletPosition, Map.AutoTilePattern.AllSide);
                        } else {
                            //パレットで選択している場合
                            this.drawTileWithAutotilePattern(x, y, new Point((this.parent.SelectedPalletRange.Location.X + (a % this.parent.SelectedPalletRange.Width)), (this.parent.SelectedPalletRange.Location.Y + (b % this.parent.SelectedPalletRange.Height))), Map.AutoTilePattern.AllSide);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 可視領域のタイルを描画
        /// </summary>
        private void drawVisibleRangeTiles(int margin, int left, int top) {
            for (var l = (int) Map.Layer.Low; l <= (int) Map.Layer.Shadow; l++) {
                if (!this.parent.SwitchLayers[l]) {
                    continue;
                }
                if (l != (int) Map.Layer.Shadow) {
                    //タイルレイヤー：アクティブレイヤー以外は暗くする
                    if (this.parent.CurrentLayer == l || this.parent.CurrentLayer == -1) {
                        DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
                    } else {
                        DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, byte.MaxValue - Settings.Default.Map_NonActiveLayerTransparency);
                    }

                    for (var x = left; x < left + this.parent.ViewRange.Width / this.parent.ScaledTileSize + margin && x < this.parent.MapData.MapSize.Width; x++) {
                        for (var y = top; y < top + this.parent.ViewRange.Height / this.parent.ScaledTileSize + margin && y < this.parent.MapData.MapSize.Height; y++) {
                            if (this.parent.MapData.LayerTiles[l, x, y].PalletPosition.IsEmpty && this.parent.MapData.BaseMapLayerTiles == null) {
                                //カーボン元がない場合、空白タイルは描画しない
                                continue;
                            }
                            this.drawTile(l, x, y);
                        }
                    }
                } else {
                    //影層：1/4 単位で描画
                    if (this.parent.CurrentLayer == l || this.parent.CurrentLayer == -1) {
                        DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, byte.MaxValue - Map.ShadowTransparent);
                    } else {
                        DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, (byte.MaxValue - Map.ShadowTransparent) / 2);
                    }
                    for (var x = left; x < left + this.parent.ViewRange.Width / this.parent.ScaledTileSize + margin && x < this.parent.MapData.MapSize.Width; x++) {
                        for (var y = top; y < top + this.parent.ViewRange.Height / this.parent.ScaledTileSize + margin && y < this.parent.MapData.MapSize.Height; y++) {
                            for (var q = 0; q < 4; q++) {
                                if (!this.parent.MapData.LayerTiles[l, x, y].HaveQuarter) {
                                    break;
                                } else if (this.parent.MapData.LayerTiles[l, x, y].GetQuarter((Map.QuarterTile) q) == DX.FALSE ^ (this.parent.MapData.BaseMapLayerTiles == null ? DX.FALSE : this.parent.MapData.BaseMapLayerTiles[l, x, y].GetQuarter((Map.QuarterTile) q)) == DX.TRUE) {
                                    //親マップか、子マップの両方に影が設定されていないか、両方とも影が設定されている場合は打ち消し合って消える
                                    continue;
                                }

                                //影を描画
                                DX.DrawBox(
                                    x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Left,
                                    y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Top,
                                    x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Left,
                                    y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Top,
                                    DX.GetColor(0, 0, 0), DX.TRUE
                                );
                            }
                        }
                    }
                }
            }
        }

        public override void LoadMaterial() {
            //フォント生成
            this.Fonts[(int) SpecialFont.StartMark] = DX.CreateFontToHandle("ＭＳ ゴシック", 16, 6, DX.DX_FONTTYPE_ANTIALIASING_EDGE);
            this.Fonts[(int) SpecialFont.EVMark] = DX.CreateFontToHandle("ＭＳ ゴシック", 12, 6, DX.DX_FONTTYPE_ANTIALIASING_EDGE);
            this.Fonts[(int) SpecialFont.QuarterDebug] = DX.CreateFontToHandle("ＭＳ ゴシック", 10, 4, DX.DX_FONTTYPE_ANTIALIASING_EDGE);

            //タイルセット生成
            this.pallet.LoadTileset(this.tilesetFixedID);

            //イベントグラフィック読み込み
            if (this.parent.MapData != null) {
                for (var i = 0; i < this.parent.MapData.EVCount; i++) {
                    this.parent.MapData.LoadEV(this.parent.MapData[i]);
                }
            }
        }

        public override void Unloading() {
            //タイルパレットをすべて無効化する
            this.pallet.InitHandle();

            //イベントグラフィックハンドルをすべて無効化する
            if (this.parent.MapData != null) {
                for (var i = 0; i < this.parent.MapData.EVCount; i++) {
                    this.parent.MapData[i].dxHandle = -1;
                }
            }
        }

        /// <summary>
        /// 指定したタイル座標に配置されているタイルを描画します。
        /// </summary>
        protected virtual void drawTile(int l, int x, int y) {
            var rawTileSize = (int) (this.parent.ScaledTileSize / ViewScale.GetRate(this.parent.Scale));
            var tile = this.parent.MapData.LayerTiles[l, x, y];

            //カーボン元があるマップで、空白タイルの部分はカーボン元の同一座標にあるタイルを描画する
            if (tile.PalletPosition.IsEmpty && this.parent.MapData.BaseMapLayerTiles != null
            && x < this.parent.MapData.BaseMapLayerTiles.GetLength(1)
            && y < this.parent.MapData.BaseMapLayerTiles.GetLength(2)) {
                tile = this.parent.MapData.BaseMapLayerTiles[l, x, y];
            }

            if (tile.PalletPosition.Y >= Map.AutoTileRowLength) {
                //単一タイルを描画
                DX.DrawRectExtendGraph(
                    x * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                    y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                    (x + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                    (y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                    tile.PalletPosition.X * rawTileSize,
                    tile.PalletPosition.Y * rawTileSize,
                    rawTileSize,
                    rawTileSize,
                    this.pallet.dxPalletHandle,
                    DX.TRUE
                );
            } else {
                //オートタイル用の1/4描画
                for (var q = 0; q < 4; q++) {
                    DX.DrawRectExtendGraph(
                        x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Left,
                        y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Top,
                        x * this.parent.ScaledTileSize + (q % 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Left,
                        y * this.parent.ScaledTileSize + (q / 2) * (this.parent.ScaledTileSize / 2) + (this.parent.ScaledTileSize / 2) - this.parent.ViewRange.Top,
                        (q % 2) * (this.parent.ScaledTileSize / 2),
                        (q / 2) * (this.parent.ScaledTileSize / 2) + tile.GetQuarter((Map.QuarterTile) q) * rawTileSize,
                        rawTileSize / 2,
                        rawTileSize / 2,
                        this.pallet.dxAutoTilesHandle[tile.PalletPosition.X + tile.PalletPosition.Y * Map.TileColumnCount],
                        DX.TRUE
                    );
                }
            }
        }

        /// <summary>
        /// 指定したタイル座標に指定したタイルを上書き描画します。
        /// </summary>
        protected void drawTileWithAutotilePattern(int x, int y, Point palPos, Map.AutoTilePattern pattern) {
            var rawTileSize = (int) (this.parent.ScaledTileSize / ViewScale.GetRate(this.parent.Scale));

            if (palPos.Y >= Map.AutoTileRowLength) {
                //単一タイルを描画
                DX.DrawRectExtendGraph(
                    x * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                    y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                    (x + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                    (y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                    palPos.X * rawTileSize,
                    palPos.Y * rawTileSize,
                    rawTileSize,
                    rawTileSize,
                    this.pallet.dxPalletHandle,
                    DX.TRUE
                );
            } else {
                //オートタイルの指定されたパターンを描画
                DX.DrawRectExtendGraph(
                    x * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                    y * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                    (x + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Left,
                    (y + 1) * this.parent.ScaledTileSize - this.parent.ViewRange.Top,
                    0,
                    (int) pattern * rawTileSize,
                    rawTileSize,
                    rawTileSize,
                    this.pallet.dxAutoTilesHandle[palPos.X + palPos.Y * Map.TileColumnCount],
                    DX.TRUE
                );
            }
        }
    }
}
