using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlDBGUIEffect : ctlDBGUIBase {
        public mgrDBEffect mgr;
        private DrugObject drugObj = null;
        private bool valueChangingBySystem = false;    //システムによってフレーム情報が変更されているかどうか

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBGUIEffect() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this, new Control[] { this.pnlPreview });

            this.DBs = new DataGridView[] { this.CreateDefaultDataGridView() };
            this.tolScale.Items.AddRange(ViewScale.GetNames());
            this.tolDrawDest.Items.AddRange(mgrDBEffect.EffectOneData.GetViewPositionList());
            this.uctlViewPos.Minimum = new Point(short.MinValue, short.MinValue);
            this.uctlViewPos.Maximum = new Point(short.MaxValue, short.MaxValue);
        }

        /// <summary>
        /// コンストラクターの次に呼び出されます。
        /// </summary>
        private void ctlDBGUIEffect_Load(object sender, EventArgs e) {
            this.tolScale.SelectedIndex = (Settings.Default.DB_EffectScale == -1) ? (int) ViewScale.ScaleRate.Normal : Settings.Default.DB_EffectScale;
        }

        public override AllDB DBList {
            get {
                return this.dbList;
            }
            set {
                this.dbList = value;
                this.mgr = new mgrDBEffect(this.dbList, this.DBs[0], null);
                this.mgr.Drawer = new mgrDBEffect.EffectDrawObject(this.dbList, this.pnlPreview, this.mgr);
                this.mgr.ListChanged += this.edittingListChanged;
                this.mgr.DataChangedEx += this.edittingDataChanged;
                this.mgr.Editted += this.managerEditted;
            }
        }
        private AllDB dbList;

        public override bool DoSave() {
            //独自コマンドとして使用するスクリプトのリスト
            if (this.mgr.IsUserCommandsDirty) {
                using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + Resources.Path_DBs_EffectScriptCenter), Common.SJIS)) {
                    W.WriteLine("// 独自コマンドとして使用するスクリプトをインクルードします。すべてのエフェクトスクリプトはこのファイルをインクルードします。");
                    foreach (var file in this.mgr.UserCommandScripts) {
                        W.WriteLine(Resources.SQ_Include.Replace("$", "\"" + file.Replace("\\", "\\\\") + "\""));
                    }
                }

                //変更履歴に追加
                this.mgr.IsUserCommandsDirty = false;
            }

            //単独エフェクトスクリプト
            foreach (var effect in this.mgr.Data) {
                effect.SaveFile();
            }

            //内部DB
            base.DoSave();

            //リストの背景を初期化する
            this.edittingListChanged(this, null);

            return true;
        }

        public override void DoLoad() {
            //DXライブラリのリソースを破棄
            Media.ClearResources();

            //内部DB
            this.FlagBySystem = true;
            base.DoLoad();
            this.ApplyToManager<mgrDBEffect, mgrDBEffect.EffectOneData>(this.mgr);

            //独自コマンドとして使用するスクリプトのリスト
            this.mgr.UserCommandScripts.Clear();
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + Resources.Path_DBs_EffectScriptCenter), Common.SJIS)) {
                var buf = R.ReadLine();     //１行目：コメント
                buf = R.ReadLine();
                while (buf != null) {
                    this.mgr.UserCommandScripts.Add(SQ.CutDoubleQuotation(SQ.GetArgumentListByCall(buf)[0]).Replace("\\\\", "\\"));
                    buf = R.ReadLine();
                }
            }
            this.mgr.IsUserCommandsDirty = false;

            //単独エフェクトスクリプト
            foreach (var effect in this.mgr.Data) {
                effect.LoadFile();
            }

            this.edittingListChanged(this, null);
            this.FlagBySystem = false;
            this.IsDirty = false;
        }

        public override DialogResult Import() {
            this.ltvDB.SelectedIndices.Clear();
            return this.ImportWithFiles<mgrDBEffect, mgrDBEffect.EffectOneData>(Resources.Path_DBs_EffectScripts, Resources.Extension_SQ, this.mgr);
        }

        public override DialogResult Export() {
            return this.ExportWithFiles(Resources.Path_DBs_EffectScripts, Resources.Extension_SQ, this.mgr.Data);
        }

        public override bool IsManagedFile(string rPath) {
            if (base.IsManagedFile(rPath)) {
                return true;
            }

            //付属ファイルを調べる
            if (rPath == Resources.Path_DBs_EffectScriptCenter) {
                return true;
            }
            foreach (var data in this.mgr.Data) {
                if (rPath == data.FileName) {
                    return true;
                }
            }
            return false;
        }

        public override void SrcReset(AllDB DBList) {
            //デザイナービューのサイズをリセットする
            var wndSize = (Size) DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.WindowSize, 0].Value;
            this.pnlPreview.Size = wndSize;
            mgrDBEffect.ScreenSize = wndSize;
            mgrDBEffect.FPS = (int) DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.EffectFPS, 0]?.Value;

            //サウンド命令のファイル名を修正する：これは保存対象項目だがIsDirtyに関わる操作ではない
            foreach (var data in this.mgr.Data) {
                foreach (var order in data.Orders) {
                    if (order.Type == mgrDBEffect.EffectOneData.EffectOrder.OrderType.Sound) {
                        //ファイル名が変更されている場合は修正する
                        var idName = Database.GetIDNameFromFixedID(DBList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], int.Parse(order.Options[(int) mgrDBEffect.EffectOneData.EffectOrder.SoundOptionIndices.FixedID]));
                        if (order.Options[(int) mgrDBEffect.EffectOneData.EffectOrder.SoundOptionIndices.IDName] != idName) {
                            order.Options[(int) mgrDBEffect.EffectOneData.EffectOrder.SoundOptionIndices.IDName] = idName;
                            if (this.mgr.EdittingData == data) {
                                this.edittingDataChanged(this, new mgrDBEffect.DataChangedEventArgs(false, true, false, false));
                            }
                        }
                    }
                }
            }

            //エフェクトリストを更新
            this.edittingListChanged(this, null);
        }

        public override void Cleanup() {
            var files = Directory.GetFiles(ProjectManager.ProjectPath + Common.CutLastChar(Resources.Path_DBs_EffectScripts, '\\'));
            foreach (var file in files) {
                if (int.TryParse(Path.GetFileNameWithoutExtension(file), out var id)) {
                    //ファイル名が数値である場合に限りID判定する
                    if (this.mgr.ExistsData(id)) {
                        continue;
                    }
                } else if (Path.GetFileName(file) == Path.GetFileName(Common.CutLastChar(Resources.Path_DBs_EffectScriptCenter, '\\'))) {
                    //例外的に認めるファイル
                    continue;
                }

                //削除処理
                try {
                    File.Delete(file);
                } catch {
                    MessageBox.Show(Path.GetFileName(file) + " の削除に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// エフェクト新規
        /// </summary>
        private void ctlDBGUIEffect_RequestRowAdd(object sender, EventArgs e) {
            this.ltvDB.SelectedIndices.Add(this.mgr.AddData(new mgrDBEffect.EffectOneData(this.mgr.GenerateFixedID(), 0, "新しいエフェクト", mgrDBEffect.FrameDefaultLength, Database.DBEffectViewPosition.Each, this.DBList)));
        }

        /// <summary>
        /// エフェクト複製
        /// </summary>
        private void ctlDBGUIEffect_RequestRowCopy(object sender, EventArgs e) {
            var index = this.mgr.CopyData(this.ltvDB.SelectedIndices[0]);
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.SelectedIndices.Add(index);
        }

        /// <summary>
        /// エフェクト削除
        /// </summary>
        private void ctlDBGUIEffect_RequestRowDelete(object sender, EventArgs e) {
            //選択されているうちに選択解除を行わないとイベントが発生しない
            var index = this.ltvDB.SelectedIndices[0];
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.DeleteData(index);
        }

        /// <summary>
        /// 独自コマンドの編集
        /// </summary>
        private void tolEditUserCommands_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Database.Effect.dlgUserCommands(this.mgr.UserCommandScripts);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.UserCommandScripts = Dlg.Result;
            }
        }

        /// <summary>
        /// エフェクト選択変更
        /// </summary>
        private void ltvDB_SelectedIndexChanged(object sender, EventArgs e) {
            this.LockDirty = true;
            if (this.ltvDB.SelectedIndices.Count == 0) {
                this.mgr.EndEdit();
            } else {
                this.mgr.BeginEdit(this.ltvDB.SelectedIndices[0]);
                this.lstFrameList.SelectedIndex = 0;
            }
            this.LockDirty = false;
        }

        /// <summary>
        /// エフェクトリストの列をクリックすると、対応列に従ってソートする
        /// </summary>
        private void ltvDB_ColumnClick(object sender, ColumnClickEventArgs e) {
            if (this.mgr.EdittingData != null) {
                this.mgr.EndEdit();
            }
            this.ltvDB.SelectedIndices.Clear();
            this.mgr.SortDataList(e.Column);
            this.IsDirty = true;
        }

        /// <summary>
        /// ID変更時
        /// </summary>
        private void tmnEffectID_Validating(object sender, CancelEventArgs e) {
            if (!int.TryParse(this.tolEffectID.Text, out var val)) {
                //数値に変換できない場合は既定値へ
                this.tolEffectID.Text = Database.NotFoundItemID.ToString();
            }
            e.Cancel = false;
            this.mgr.EdittingData.VisibleID = int.Parse(this.tolEffectID.Text);
        }

        /// <summary>
        /// 名前変更時
        /// </summary>
        private void tolEffectName_Validating(object sender, CancelEventArgs e) {
            this.mgr.EdittingData.Name = this.tolEffectName.Text;
            e.Cancel = false;
        }

        /// <summary>
        /// 描画対象を変更
        /// </summary>
        private void tmnDrawDest_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null || this.valueChangingBySystem) {
                return;
            }
            this.mgr.EdittingData.DrawDest = (Database.DBEffectViewPosition) this.tolDrawDest.SelectedIndex;
        }

        /// <summary>
        /// フレーム数変更
        /// </summary>
        private void tolSetFrameLength_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgInputNumValue(mgrDBEffect.FrameMinLength, mgrDBEffect.FrameMaxLength, this.mgr.EdittingData.FrameLength, "フレーム数の変更");
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.EdittingData.FrameLength = Dlg.Result;
            }
        }

        /// <summary>
        /// レイヤー設定
        /// </summary>
        private void tmnLayerSettings_Click(object sender, EventArgs e) {
            if (this.ltvDB.SelectedIndices.Count == 0) {
                return;
            }

            var Dlg = new Dialog.Database.Effect.dlgEditLayers(this.DBs[0][(int) Database.DBDefaultColumnIndices.Count + (int) Database.DBEffectColumnIndices.Layers, this.mgr.EdittingIndex].Value?.ToString());
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.EdittingData.SetupLayers(Dlg.Result);
            }
        }

        /// <summary>
        /// ターゲットグラフィック変更
        /// </summary>
        private void tmnChangeTargetGraph_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgFileSelect(Resources.Path_Materials, Resources.Extension_Graphics, true, false, false, Settings.Default.DB_EffectTargetPicture);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.ChangeTarget(Dlg.FileName);
            }
        }

        /// <summary>
        /// 再生
        /// </summary>
        private void tmnPlay_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData == null) {
                return;
            }
            this.mgr.Drawer.LoadMaterialSounds();
            this.mgr.Drawer.Play();
            this.mgr.Drawer.FrameNumber = this.lstFrameList.SelectedIndex;
        }

        /// <summary>
        /// デザイナーのスケール変更
        /// </summary>
        private void tolScale_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.mgr == null) {
                return;
            }
            this.mgr.Scale = (ViewScale.ScaleRate) this.tolScale.SelectedIndex;

            //デザイナーを更新：プレビュー画面をゲームウィンドウの解像度と一致させる
            var scaledWndSize = mgrDBEffect.ScreenSize;
            ViewScale.ApplyRateToSize(ref scaledWndSize, this.mgr.Scale);
            this.pnlPreview.Size = scaledWndSize;

            //パレットサイズを更新
            if (this.mgr.EdittingData?.CurrentLayer?.LayerImage != null) {
                this.picPallet.Image = new Bitmap(
                    (int) (this.mgr.EdittingData.CurrentLayer.LayerImage.Width * ViewScale.GetRate(this.mgr.Scale)),
                    (int) (this.mgr.EdittingData.CurrentLayer.LayerImage.Height * ViewScale.GetRate(this.mgr.Scale))
                );
                this.picPallet.Refresh();
            }
        }

        /// <summary>
        /// 編集対象のレイヤーを変更
        /// </summary>
        private void tmnCurrentLayer_SelectedIndexChanged(object sender, EventArgs e) {
            this.mgr.EdittingData.CurrentLayerIndex = this.tolCurrentLayer.SelectedIndex;
            this.tolScale_SelectedIndexChanged(sender, e);          //パレットのスケールを再適用
            this.lstFrameList_SelectedIndexChanged(sender, e);      //現在のフレーム情報を画面に反映
        }

        /// <summary>
        /// 現在のレイヤーの全フレームの表示位置をシフト
        /// </summary>
        private void tmnFrameShift_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgShift();
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.EdittingData.CurrentLayer.Shift(Dlg.Result);
                this.lstFrameList_SelectedIndexChanged(this, null);     //画面に反映
            }
        }

        /// <summary>
        /// 直前のフレームから引継
        /// </summary>
        private void tmnFromPrevFrame_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null || this.lstFrameList.SelectedIndex <= 0) {
                return;
            }
            this.mgr.EdittingData.CurrentLayer.ReplaceFrame(this.lstFrameList.SelectedIndex, this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex - 1].GenerateClone());
            this.lstFrameList_SelectedIndexChanged(this, null);     //画面に反映
        }

        /// <summary>
        /// フレームコピー
        /// </summary>
        private void tmnFrameCopy_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null || this.lstFrameList.SelectedIndex == -1) {
                return;
            }

            var Dlg = new Dialog.Database.Effect.dlgFrameCopy(this.mgr.EdittingData.FrameLength, this.lstFrameList.SelectedIndex);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                Dlg.GetResult(out var start, out var end);
                this.mgr.EdittingData.CurrentLayer.FrameCopy(start, end, this.lstFrameList.SelectedIndex);
                this.lstFrameList_SelectedIndexChanged(this, null);     //画面に反映
            }
        }

        /// <summary>
        /// フレームクリア
        /// </summary>
        private void tmnFrameClear_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null || this.lstFrameList.SelectedIndex == -1) {
                return;
            }
            this.mgr.EdittingData.CurrentLayer.ReplaceFrame(this.lstFrameList.SelectedIndex, new mgrDBEffect.EffectOneData.EffectLayer.EffectFrame());
            this.lstFrameList_SelectedIndexChanged(this, null);     //画面に反映
        }

        /// <summary>
        /// フレーム一括
        /// </summary>
        private void tmnFrameFill_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null) {
                return;
            }

            var Dlg = new Dialog.Database.Effect.dlgFrameFill(this.mgr.EdittingData.FrameLength);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                Dlg.GetResult(out var start, out var end, out var param);
                this.mgr.EdittingData.CurrentLayer.FrameFill(start, end, param);
                this.lstFrameList_SelectedIndexChanged(this, null);     //画面に反映
            }
        }

        /// <summary>
        /// フレーム補間
        /// </summary>
        private void tmnFrameComplete_Click(object sender, EventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null) {
                return;
            }

            var Dlg = new Dialog.Database.Effect.dlgFrameComplete(this.mgr.EdittingData.FrameLength);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                Dlg.GetResult(out var start, out var end, out var param);
                this.mgr.EdittingData.CurrentLayer.FrameComplete(start, end, param);
                this.lstFrameList_SelectedIndexChanged(this, null);     //画面に反映
            }
        }

        /// <summary>
        /// 命令追加
        /// </summary>
        private void tolOrderAdd_Click(object sender, EventArgs e) {
            if (this.lstFrameList.SelectedIndex == -1) {
                return;
            }

            var Dlg = new Dialog.Database.Effect.dlgOrder(
                this.DBList,
                null,
                this.mgr.EdittingData.FrameLength,
                this.lstFrameList.SelectedIndex,
                false,
                this.mgr.UserCommandScripts
            );
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.EdittingData.AddOrder(Dlg.Result);
            }
        }

        /// <summary>
        /// 命令編集
        /// </summary>
        private void tolOrderEdit_Click(object sender, EventArgs e) {
            if (this.ltvOrderList.SelectedIndices.Count == 0) {
                return;
            }

            var Dlg = new Dialog.Database.Effect.dlgOrder(
                this.DBList,
                this.mgr.EdittingData.Orders[this.ltvOrderList.SelectedIndices[0]],
                this.mgr.EdittingData.FrameLength,
                this.ltvOrderList.SelectedIndices[0],
                true,
                this.mgr.UserCommandScripts
            );
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.mgr.EdittingData.ReplaceOrder(this.ltvOrderList.SelectedIndices[0], Dlg.Result);
            }
        }

        /// <summary>
        /// 命令削除
        /// </summary>
        private void tolOrderDelete_Click(object sender, EventArgs e) {
            if (this.ltvOrderList.SelectedIndices.Count == 0) {
                return;
            }
            this.mgr.EdittingData.DeleteOrder(this.ltvOrderList.SelectedIndices[0]);
            this.ltvOrderList_SelectedIndexChanged(this, null);
        }

        /// <summary>
        /// 命令選択時
        /// </summary>
        private void ltvOrderList_SelectedIndexChanged(object sender, EventArgs e) {
            this.tolOrderEdit.Enabled = (this.ltvOrderList.SelectedIndices.Count > 0);
            this.tolOrderDelete.Enabled = this.tolOrderEdit.Enabled;
        }

        /// <summary>
        /// フレーム選択変更
        /// </summary>
        private void lstFrameList_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.mgr.EdittingData.CurrentLayer == null) {
                //レイヤーが存在しないときは処理しない
                return;
            } else if (this.lstFrameList.SelectedIndex < 0 || this.mgr.EdittingData.CurrentLayer.Frames.Length <= this.lstFrameList.SelectedIndex) {
                //選択位置が範囲外のときは処理しない
                return;
            }

            this.tolFromPrevFrame.Enabled = (this.lstFrameList.SelectedIndex != 0);
            this.tolOrderAdd.Enabled = (this.lstFrameList.SelectedIndex != -1);
            this.tspFrame.Enabled = (this.mgr.EdittingData?.CurrentLayer != null && this.lstFrameList.SelectedIndex != -1);
            this.tolLayerShift.Enabled = this.tspFrame.Enabled;
            this.pnlFrameRawData.Enabled = this.tspFrame.Enabled;
            this.splMain2.Enabled = this.tspFrame.Enabled;

            this.mgr.Drawer.FrameNumber = this.lstFrameList.SelectedIndex;

            //フレーム情報を画面に反映する
            if (this.mgr.EdittingData == null || this.mgr.EdittingData.CurrentLayer == null || this.lstFrameList.SelectedIndex == -1) {
                return;
            }
            this.valueChangingBySystem = true;
            this.uctlViewPos.Result = this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].ViewPosition;
            this.uctlSectionPos.Result = this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].SectionPosition;
            this.numTrans.Value = this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].Transparent;
            this.numExt.Value = this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].Extend;
            this.numAngle.Value = this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].Angle;
            this.cmbBlendMode.SelectedIndex = (int) this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].Blend;
            this.valueChangingBySystem = false;
            this.picPallet.Refresh();
        }

        /// <summary>
        /// 画面上に描画領域が出てきたら描画準備を行う
        /// </summary>
        private void pnlPreview_VisibleChanged(object sender, EventArgs e) {
            if (this.DesignMode == false && this.pnlPreview.Visible && this.mgr.EdittingData != null) {
                this.mgr.Drawer.TargetEffectData = this.mgr.EdittingData;
                Media.ChangeDest(this.mgr.Drawer);
            }
        }

        /// <summary>
        /// デザイナーの描画が無効になっていたら復元する
        /// </summary>
        private void pnlPreview_MouseEnter(object sender, EventArgs e) {
            if (Media.TargetPtr != this.pnlPreview.Handle) {
                this.pnlPreview_VisibleChanged(sender, e);
            }
        }

        /// <summary>
        /// パレット位置をセット
        /// </summary>
        private void picPallet_MouseClick(object sender, MouseEventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null || this.picPallet.Image == null) {
                return;
            }
            var scaled = new Point(e.X, e.Y);
            ViewScale.RestoreOriginalPoint(ref scaled, this.mgr.Scale);
            this.uctlSectionPos.Result = new Point(scaled.X / this.mgr.EdittingData.CurrentLayer.SectionSize.Width, scaled.Y / this.mgr.EdittingData.CurrentLayer.SectionSize.Height);
            this.frameValuesChanged(this.uctlSectionPos, e);
            this.picPallet.Refresh();
        }

        /// <summary>
        /// パレットを再描画
        /// </summary>
        private void picPallet_Paint(object sender, PaintEventArgs e) {
            if (this.mgr.EdittingData?.CurrentLayer == null || this.picPallet.Image == null) {
                return;
            }
            Common.ApplyHighQualityDrawing(e.Graphics);
            e.Graphics.DrawImage(this.mgr.EdittingData.CurrentLayer.LayerImage, new Rectangle(Point.Empty, this.picPallet.Size));
            e.Graphics.DrawRectangle(
                new Pen(Color.Red, 3),
                (int) (this.uctlSectionPos.Result.X * this.mgr.EdittingData.CurrentLayer.SectionSize.Width * ViewScale.GetRate(this.mgr.Scale)),
                (int) (this.uctlSectionPos.Result.Y * this.mgr.EdittingData.CurrentLayer.SectionSize.Height * ViewScale.GetRate(this.mgr.Scale)),
                (int) (this.mgr.EdittingData.CurrentLayer.SectionSize.Width * ViewScale.GetRate(this.mgr.Scale)),
                (int) (this.mgr.EdittingData.CurrentLayer.SectionSize.Height * ViewScale.GetRate(this.mgr.Scale))
            );
        }

        /// <summary>
        /// デザイナー：ドラッグ開始
        /// </summary>
        private void pnlPreview_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Left || this.mgr.EdittingData?.CurrentLayer == null || this.lstFrameList.SelectedIndex == -1) {
                return;
            }

            var NotFound = true;

            //現在のレイヤーを掴めているか判定する
            var layerSize = new Size(
                (int) (this.mgr.EdittingData.CurrentLayer.SectionSize.Width * this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].Extend / 100.0),
                (int) (this.mgr.EdittingData.CurrentLayer.SectionSize.Height * this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].Extend / 100.0)
            );
            var layerRect = new Rectangle(
                this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].ViewPosition.X + mgrDBEffect.ScreenSize.Width / 2 - layerSize.Width / 2,
                this.mgr.EdittingData.CurrentLayer.Frames[this.lstFrameList.SelectedIndex].ViewPosition.Y + mgrDBEffect.ScreenSize.Height / 2 - layerSize.Height / 2,
                layerSize.Width,
                layerSize.Height
            );

            //ドラッグ可能な領域である場合
            if (layerRect.Contains(e.X, e.Y)) {
                this.drugObj = new DrugObject(-1, new Point(e.X, e.Y));
                NotFound = false;
            }

            if (NotFound) {
                this.drugObj = null;
                this.picPallet.Refresh();
            }
        }

        /// <summary>
        /// デザイナー：ドラッグ中
        /// </summary>
        private void pnlPreview_MouseMove(object sender, MouseEventArgs e) {
            if (this.drugObj == null) {
                return;
            }
            var distance = this.drugObj.GetDistance(new Point(e.X, e.Y));
            this.uctlViewPos.Result = new Point(this.uctlViewPos.Result.X + distance.X, this.uctlViewPos.Result.Y + distance.Y);
        }

        /// <summary>
        /// デザイナー：ドラッグ終了
        /// </summary>
        private void pnlPreview_MouseUp(object sender, MouseEventArgs e) {
            this.pnlPreview_MouseMove(sender, e);
            this.drugObj = null;
        }

        /// <summary>
        /// デザイナー：ドラッグ中断
        /// </summary>
        private void pnlPreview_MouseLeave(object sender, EventArgs e) {
            this.drugObj = null;
        }

        /// <summary>
        /// フレーム情報値が変更されたとき
        /// </summary>
        private void frameValuesChanged(object sender, EventArgs e) {
            if (this.lstFrameList.SelectedIndex == -1 || this.valueChangingBySystem) {
                return;
            }

            if (sender == this.uctlSectionPos) {
                //セクション座標の範囲外チェック
                if (this.uctlSectionPos.Result.X < 0) {
                    this.uctlSectionPos.Result = new Point(0, this.uctlSectionPos.Result.Y);
                } else if (this.uctlSectionPos.Result.X >= this.mgr.EdittingData.CurrentLayer.LastSectionPosition.X) {
                    this.uctlSectionPos.Result = new Point(this.mgr.EdittingData.CurrentLayer.LastSectionPosition.X - 1, this.uctlSectionPos.Result.Y);
                }

                if (this.uctlSectionPos.Result.Y < 0) {
                    this.uctlSectionPos.Result = new Point(this.uctlSectionPos.Result.X, 0);
                } else if (this.uctlSectionPos.Result.Y >= this.mgr.EdittingData.CurrentLayer.LastSectionPosition.Y) {
                    this.uctlSectionPos.Result = new Point(this.uctlSectionPos.Result.X, this.mgr.EdittingData.CurrentLayer.LastSectionPosition.Y - 1);
                }

                this.picPallet.Refresh();
            }

            this.mgr.EdittingData.CurrentLayer.ReplaceFrame(this.lstFrameList.SelectedIndex, new mgrDBEffect.EffectOneData.EffectLayer.EffectFrame(
                this.uctlViewPos.Result,
                this.uctlSectionPos.Result,
                (Media.BlendMode) this.cmbBlendMode.SelectedIndex,
                (int) this.numTrans.Value,
                (int) this.numExt.Value,
                (int) this.numAngle.Value
            ));
        }

        /// <summary>
        /// エフェクトリストに変更が起きたときにコントロールへ更新する
        /// </summary>
        protected override void edittingListChanged(object sender, EventArgs e) {
            var beforeIndex = (this.ltvDB.SelectedIndices.Count > 0) ? this.ltvDB.SelectedIndices[0] : -1;

            this.ltvDB.BeginUpdate();
            this.ltvDB.SelectedIndices.Clear();
            this.ltvDB.Items.Clear();

            foreach (var item in this.mgr.Data) {
                var newItem = new ListViewItem {
                    Text = (!Settings.Default.Script_FixedIDHexMode) ? item.FixedID.ToString() : "0x" + item.FixedID.ToString("X")
                };
                newItem.SubItems.Add(item.VisibleID.ToString());
                newItem.SubItems.Add(item.Name);

                if (item.IsDirty) {
                    newItem.BackColor = this.EdittingBackColor;
                } else {
                    newItem.BackColor = SystemColors.Window;
                }

                this.ltvDB.Items.Add(newItem);
            }
            this.ltvDB.EndUpdate();

            if (beforeIndex != -1 && beforeIndex < this.ltvDB.Items.Count) {
                this.ltvDB.SelectedIndices.Add(beforeIndex);
            }
        }

        /// <summary>
        /// 編集中のエフェクトに変更が起きたときにコントロールへ更新する
        /// </summary>
        private void edittingDataChanged(object sender, mgrDBEffect.DataChangedEventArgs e) {
            if (this.mgr.EdittingData == null) {
                return;
            }

            this.valueChangingBySystem = true;

            //変更が生じた場合は該当リスト項目の背景色を変える
            if (this.mgr.EdittingData.IsDirty) {
                this.ltvDB.Items[this.mgr.EdittingIndex].BackColor = this.EdittingBackColor;
            } else {
                this.ltvDB.Items[this.mgr.EdittingIndex].BackColor = SystemColors.Window;
            }

            //エフェクト単位
            if (e.Effects) {
                //ヘッダー情報
                this.tolEffectID.Text = this.mgr.EdittingData.VisibleID.ToString();
                this.tolEffectName.Text = this.mgr.EdittingData.Name;
                this.tolDrawDest.SelectedIndex = (int) this.mgr.EdittingData.DrawDest;

                //エフェクトリスト上の選択項目
                if (this.ltvDB.SelectedIndices.Count > 0) {
                    this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.VisibleID].Text = this.mgr.EdittingData.VisibleID.ToString();
                    this.ltvDB.SelectedItems[0].SubItems[(int) Database.DBDefaultColumnIndices.Name].Text = this.mgr.EdittingData.Name.ToString();
                }
            }

            //命令単位
            if (e.Orders) {
                //命令リスト
                this.ltvOrderList.BeginUpdate();
                this.ltvOrderList.SelectedIndices.Clear();
                this.ltvOrderList.Items.Clear();

                foreach (var order in this.mgr.EdittingData.Orders) {
                    var item = new ListViewItem {
                        Text = order.Frame.ToString()
                    };
                    item.SubItems.Add(mgrDBEffect.EffectOneData.EffectOrder.GetOrderTypeList()[(int) order.Type]);

                    //パラメーターを列挙する
                    var buf = "";
                    foreach (var param in order.Options) {
                        buf += param;
                        buf += Resources.Split_SimpleList;
                    }
                    item.SubItems.Add(buf);

                    this.ltvOrderList.Items.Add(item);
                }
                this.ltvOrderList.EndUpdate();
            }

            //レイヤー単位
            if (e.Layers) {
                //レイヤーリスト
                this.tolCurrentLayer.Items.Clear();
                for (var i = 0; i < this.mgr.EdittingData.Layers.Count; i++) {
                    this.tolCurrentLayer.Items.Add($"{i}: {Path.GetFileName(this.mgr.EdittingData.Layers[i].FileName)}");
                }
                this.tolCurrentLayer.SelectedIndex = (this.mgr.EdittingData.Layers.Count > 0) ? 0 : -1;
            }

            //フレーム単位
            if (e.Frame) {
                //フレームリスト
                var frame = this.lstFrameList.SelectedIndex;
                this.lstFrameList.BeginUpdate();
                this.lstFrameList.Items.Clear();

                for (var i = 0; i < this.mgr.EdittingData.FrameLength; i++) {
                    this.lstFrameList.Items.Add($"#{i}");
                }
                this.lstFrameList.EndUpdate();

                if (0 <= frame) {
                    //再選択
                    if (frame < this.mgr.EdittingData.FrameLength) {
                        this.lstFrameList.SelectedIndex = frame;
                    } else {
                        this.lstFrameList.SelectedIndex = this.mgr.EdittingData.FrameLength - 1;
                    }
                } else {
                    this.lstFrameList.SelectedIndex = 0;
                }
            }

            this.valueChangingBySystem = false;
        }
    }
}
