using DxLibDLL;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlMapEditor : CtrlComponent.Common.ctlEditorParent {
        public event Common.RequestOpenFileEventHandler OpenScriptRequested;
        private readonly UndoRedoManager UndoRedo = new UndoRedoManager();
        private FileStream fsTile = null;
        private FileStream fsSQ = null;
        public mgrMapObject mgr = null;
        private List<mgrMapObject.TileBuffer> oncePuts;
        private Point clippingEVStartPosition;
        private bool localEVSelcting = false;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlMapEditor() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.tolScale.Items.AddRange(ViewScale.GetNames());
            this.pnlPreview.MouseWheel += this.pnlPreview_MouseWheel;

            //レイヤーリストを生成
            this.lstMapLayerList.Items.Clear();
            this.lstMapLayerList.Items.AddRange(Map.GetLayerList());

            //管理オブジェクトを初期化
            this.UndoRedo.BufferChanged += this.eventhandler_UndoBufferChanged;
            this.mgr = new mgrMapObject(this.UndoRedo);
            this.mgr.EVListChanged += this.eventhandler_EVListChanged;
            this.mgr.CurrentMapResized += this.tblMapDesigner_Resize;
            this.mgr.CursorMoved += this.cursorMoved;
            this.mgr.TilesetChanged += this.eventhandler_TilesetChanged;
            this.mgr.TileEditted += this.eventhandler_TileEditted;
        }

        /// <summary>
        /// コンストラクターの次に呼び出されます。
        /// </summary>
        private void ctlMapEditor_Load(object sender, EventArgs e) {
            //初期設定
            this.uctlMapTree.FileFilter = Resources.Extension_Map;
            this.tolModePencil.Checked = true;

            //前回の状態を復元
            this.tolScale.SelectedIndex = Settings.Default.Map_Scale;
            this.tolGrid.Checked = Settings.Default.Map_VisibleGrid;
            this.lstMapLayerList.SelectedIndex = Settings.Default.Last_MapLayer;
            var spl = Settings.Default.Map_VisibleLayerIndexList.Split(Resources.Split_SimpleList.ToArray());

            foreach (var index in spl) {
                if (string.IsNullOrEmpty(index)) {
                    continue;
                }
                this.lstMapLayerList.SetItemChecked(int.Parse(index), true);
                this.mgr.SwitchLayers[int.Parse(index)] = true;
            }
        }

        /// <summary>
        /// エディターがアクティブになったときに他のエディター情報を参照する
        /// </summary>
        private void ctlMapEditor_VisibleChanged(object sender, EventArgs e) {
            if (this.DBList == null) {
                return;
            }

            //タイルサイズ
            mgrMapObject.TileSize = int.Parse(this.DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.TileSize, 0].Value?.ToString());
            this.vscDesignerScroller.LargeChange = this.mgr.ScaledTileSize;
            this.hscDesignerScroller.LargeChange = this.mgr.ScaledTileSize;

            //タイルパレット
            if (this.mgr.MapData != null) {
                this.uctlTilePallet.Setup(this.mgr.MapData.TilesetFixedID);
            }
        }

        /// <summary>
        /// データベースエディターへの参照
        /// </summary>
        public CtrlComponent.Database.ctlDatabaseEditor DBCtrl {
            get; set;
        }

        /// <summary>
        /// データベース情報
        /// </summary>
        public AllDB DBList {
            get; set;
        }

        /// <summary>
        /// コマンドツリーへのインスタンス参照
        /// </summary>
        public CtrlComponent.Text.ctlCommandTree comTree {
            get; set;
        }

        /// <summary>
        /// 編集中のファイル名 (rPath)
        /// </summary>
        public string FileName {
            get {
                return this.fileName;
            }
            set {
                //エディターの表示有無、ファイルアクセス制御を開始する
                if (string.IsNullOrEmpty(value)) {
                    this.pnlEditor.Visible = false;
                    this.fileName = null;
                    Common.EndFileAccessLock(ref this.fsTile);
                    Common.EndFileAccessLock(ref this.fsSQ);
                } else {
                    this.pnlEditor.Visible = true;
                    this.fileName = value;
                }
            }
        }
        private string fileName;

        /// <summary>
        /// 読み取り専用であるかどうか
        /// </summary>
        public bool IsReadOnly {
            get {
                return this.isReadOnly;
            }
            private set {
                this.isReadOnly = value;
                this.tolSave.Enabled = !this.isReadOnly;
            }
        }
        public bool isReadOnly;

        /// <summary>
        /// 作業中のデータが変更されているかどうか
        /// </summary>
        public override bool IsDirty {
            get;
            protected set;
        }

        /// <summary>
        /// 閲覧専用モードにするかどうか。
        /// これは一度だけ変更が反映されます。
        /// </summary>
        public bool IsViewerMode {
            get {
                return this.isViewerMode;
            }
            set {
                this.isViewerMode = value;
                if (value) {
                    this.splMap1.Panel1Collapsed = true;
                    this.statusBar.Visible = false;
                    this.toolBar.Visible = false;
                }
            }
        }
        private bool isViewerMode = false;

        /// <summary>
        /// 読み込み処理を行います。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public bool DoLoad(string rPath) {
            this.FileName = rPath;
            this.IsReadOnly = this.mgr.DoLoad(rPath, ref this.fsTile, ref this.fsSQ);
            this.uctlTilePallet_SelectedPalletTile(this, null);     //パレットの選択状態を更新
            this.IsDirty = false;
            this.lblMapName.Text = this.mgr.MapData.Name;
            this.lblMapFixedID.Text = $"FixedID: {this.mgr.MapData.GUID}";
            return true;
        }

        /// <summary>
        /// 保存処理を行います。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public override bool DoSave() {
            if (this.IsReadOnly) {
                MessageBox.Show(Resources.MsgE_ReadOnly, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            } else if (this.IsDirty && !this.IsReadOnly) {
                if (!this.mgr.DoSave(ref this.fsTile, ref this.fsSQ)) {
                    return false;
                }
                this.IsDirty = false;
            }
            return true;
        }

        public override bool Jump(string rPath) {
            if (rPath.IndexOf(Resources.Path_Maps) != -1) {
                this.uctlMapTree.ReloadTree(rPath);
                return true;
            }
            return false;
        }

        public override bool Conflict(string rPath, ProjectManager project) {
            if (rPath == this.FileName && this.IsReadOnly) {
                this.ReloadContent(this.uctlMapTree);
                return true;
            }
            return false;
        }

        public override void DoClose() {
            this.FileName = null;
            this.ctlSwitchMapTree(false);         //選択が解除されると想定してツールバーを無効にする
            this.pnlEditor.Visible = false;
            this.splMap2.Panel2Collapsed = true;
            this.lblMapName.Text = "";
            this.lblMapFixedID.Text = "";
        }

        public override void DoCleanup() {
            //マップ定義ファイルに対応しないイベントスクリプトを除去する
            var GUIDList = mgrMapObject.GetGUIDList();
            var sqList = Directory.GetFiles(ProjectManager.ProjectPath + Common.CutLastChar(Resources.Path_Scripts_MapEvents, '\\'));

            foreach (var sq in sqList) {
                if (GUIDList.Contains(Path.GetFileNameWithoutExtension(sq))) {
                    continue;
                }

                //削除処理
                try {
                    File.Delete(sq);
                } catch {
                    MessageBox.Show(Path.GetFileName(sq) + " の削除に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// マップ新規
        /// </summary>
        private void tolMapNewFile_Click(object sender, EventArgs e) {
            this.uctlMapTree.CreateFile(Resources.Extension_Map);
        }

        /// <summary>
        /// フォルダー新規
        /// </summary>
        private void tolMapNewFolder_Click(object sender, EventArgs e) {
            this.uctlMapTree.CreateFolder();
        }

        /// <summary>
        /// マップをクリップボードへコピー
        /// </summary>
        private void tolMapCopy_Click(object sender, EventArgs e) {
            this.uctlMapTree.CopySelectedNode();
            this.tolMapPaste.Enabled = (Clipboard.GetFileDropList() != null && Clipboard.GetFileDropList().Count > 0);
        }

        /// <summary>
        /// マップ貼り付け
        /// </summary>
        private void tolMapPaste_Click(object sender, EventArgs e) {
            //マップのタイルファイルを貼り付けた時点でソースのイベントスクリプトが読み込まれるので、
            //その状態でGUIDを変更して保存し直すことでスクリプトを含む完全なコピーが簡単に実現できる

            //貼り付けたらすぐに読み込まれる
            this.uctlMapTree.PasteNode();
            if (this.mgr.MapData != null) {
                //グローバル一意識別子を更新
                this.mgr.MapData.SetNewGUID();
                this.fsSQ = null;
                this.DoSave();
            }
        }

        /// <summary>
        /// マップ/フォルダー削除
        /// </summary>
        private void tolMapDelete_Click(object sender, EventArgs e) {
            var isFile = this.uctlMapTree.IsFileSelected;
            var guid = "";

            if (isFile) {
                if (this.mgr.MapData == null) {
                    MessageBox.Show("マップエディターが開かれていません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                } else {
                    guid = this.mgr.MapData.GUID;
                }
            }

            //削除前に実行する作業を登録した上で実行する
            if (this.uctlMapTree.DeleteSelectedNode(() => {
                this.IsDirty = false;       //保存ダイアログを表示させない
                this.DoClose();             //ファイルを閉じる
            })) {
                //タイルファイルの削除に成功したら、イベントスクリプトを削除
                try {
                    File.Delete(ProjectManager.ProjectPath + Resources.Path_Scripts_MapEvents + guid + Resources.Extension_SQ);
                } catch {
                    //削除に失敗したらゴミとして残る
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "イベントスクリプトの削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        /// <summary>
        /// マップリスト再読込
        /// </summary>
        private void tolMapReloadFiles_Click(object sender, EventArgs e) {
            this.uctlMapTree.ReloadTree(this.uctlMapTree.SelectedNodeFullPath);
        }

        /// <summary>
        /// エクスプローラーでフォルダーを開く
        /// </summary>
        private void tolOpenFolder_Click(object sender, EventArgs e) {
            if (this.uctlMapTree.IsFolderSelected) {
                System.Diagnostics.Process.Start(this.uctlMapTree.SelectedNodeFullPathOnFileSystem);
            }
        }

        /// <summary>
        /// ファイル/フォルダー作成時
        /// </summary>
        private void uctlMapTree_CreatedNode(object sender, CtrlComponent.Common.FileTreeNodeEventArgs e) {
            if (e.IsFolder) {
                //フォルダーに対しては特に処理を行わない
                return;
            } else if (e.Failed) {
                //失敗した場合は中断する
                return;
            }

            //ダイアログを表示している間、一時的に編集権を持って操作する
            this.fsTile = Common.OpenFileEditMode(ProjectManager.ProjectPath + e.rPath);
            var mapOBJ = new mgrMapObject.MapOneData();
            var Dlg = new Dialog.Map.dlgMapSettings(this.DBList, "新しいマップ", mapOBJ);
            var DlgResult = Dlg.ShowDialog(this);

            if (DlgResult == DialogResult.OK) {
                this.mgr.MapData = mapOBJ;
                this.mgr.MapData.FileName = e.rPath;
                this.fsSQ = Common.OpenFileWriteOnly(ProjectManager.ProjectPath + this.mgr.MapData.ScriptFileName);

                try {
                    //初期状態で保存する
                    this.mgr.DoSave(ref this.fsTile, ref this.fsSQ);
                } catch {
                    //処理に失敗したら次回から二度と開けない
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "マップの新規作成"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                this.IsDirty = false;
            } else {
                //正常に作成されなかったので削除する
                this.fsTile?.Dispose();

                try {
                    File.Delete(ProjectManager.ProjectPath + e.rPath);
                } catch {
                    //処理に失敗しても大きな問題にはならない
                }
            }
        }

        /// <summary>
        /// ファイル名を変更完了したら開き直す
        /// </summary>
        private void uctlMapTree_RenamedNode(object sender, CtrlComponent.Common.FileTreeNodeEventArgs e) {
            if (!e.IsFolder && !e.Failed) {
                var args = new TreeViewEventArgs(e.SelectedNode, TreeViewAction.ByKeyboard);
                this.uctlMapTree_AfterSelectNode(sender, args);
            }
        }

        /// <summary>
        /// ファイル/フォルダー削除時
        /// </summary>
        private void uctlMapTree_DeletedNode(object sender, CtrlComponent.Common.FileTreeNodeEventArgs e) {
            //削除後に自動選択された項目を反映する
            this.uctlMapTree.FocusToTree();
        }

        /// <summary>
        /// ファイル/フォルダー選択前
        /// </summary>
        private void uctlMapTree_BeforeSelectNode(object sender, TreeViewCancelEventArgs e) {
            //システムによる操作を含めてしまうと、無限確認を引き起こすためスルーする
            if (e.Action != TreeViewAction.Unknown) {
                if (Settings.Default.Common_NoConfirmSave) {
                    //確認せずに必ず保存する設定の場合
                    this.DoSave();
                } else if (this.FileName != null && this.IsDirty && !this.IsReadOnly) {
                    switch (MessageBox.Show(Resources.MsgQ_Save.Replace("$", "マップ"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question)) {
                        case DialogResult.Yes:
                            this.DoSave();
                            break;
                        case DialogResult.Cancel:
                            e.Cancel = true;
                            return;
                    }
                }
            }
            this.DoClose();
        }

        /// <summary>
        /// ファイル/フォルダー選択後
        /// </summary>
        private void uctlMapTree_AfterSelectNode(object sender, TreeViewEventArgs e) {
            //ツールバーを有効にする
            this.ctlSwitchMapTree(true);

            //選択ファイルを読み込み
            if (this.uctlMapTree.IsFileSelected) {
                this.DoLoad(e.Node.FullPath);
                this.hscDesignerScroller.Value = 0;
                this.vscDesignerScroller.Value = 0;
                this.pnlEditor.Visible = true;
                this.splMap2.Panel2Collapsed = false;
            }
        }

        /// <summary>
        /// ファイルツリーの有効無効
        /// </summary>
        private void ctlSwitchMapTree(bool isEnabled) {
            this.tolMapNewFile.Enabled = isEnabled;
            this.tolMapNewFolder.Enabled = isEnabled;
            this.tolMapDelete.Enabled = isEnabled;
            //tolMapCopy.Enabled = isEnabled;
            this.tolMapCopy.Enabled = isEnabled ? this.uctlMapTree.IsFileSelected : false;
            this.tolMapPaste.Enabled = isEnabled ? (Clipboard.GetFileDropList() != null && Clipboard.GetFileDropList().Count > 0) : false;
            this.tolOpenFolder.Enabled = this.uctlMapTree.IsFolderSelected;
        }

        /// <summary>
        /// レイヤー変更時
        /// </summary>
        private void lstMapLayerList_SelectedIndexChanged(object sender, EventArgs e) {
            //コントロール有効無効、内部システムに反映
            var isEnabled = (this.lstMapLayerList.SelectedIndex == (int) Map.Layer.Event);
            this.ltvEVList.Visible = isEnabled;
            this.uctlTilePallet.Visible = (!isEnabled && this.lstMapLayerList.SelectedIndex != (int) Map.Layer.Shadow);
            this.mgr.CurrentLayer = this.lstMapLayerList.SelectedIndex;
            Settings.Default.Last_MapLayer = this.mgr.CurrentLayer;
            this.setModeCheck();
        }

        /// <summary>
        /// レイヤーの可視設定の変更時
        /// </summary>
        private void lstMapLayerList_ItemCheck(object sender, ItemCheckEventArgs e) {
            Settings.Default.Map_VisibleLayerIndexList = "";

            //内部に反映させる
            for (var index = 0; index < this.lstMapLayerList.Items.Count; index++) {
                if (index == e.Index && e.NewValue != CheckState.Checked) {
                    this.mgr.SwitchLayers[index] = false;
                    continue;
                } else if (index != e.Index && !this.lstMapLayerList.CheckedIndices.Contains(index)) {
                    this.mgr.SwitchLayers[index] = false;
                    continue;
                }

                this.mgr.SwitchLayers[index] = true;
                Settings.Default.Map_VisibleLayerIndexList += index + Resources.Split_SimpleList;
            }

            Settings.Default.Map_VisibleLayerIndexList = Common.CutLastChar(Settings.Default.Map_VisibleLayerIndexList, Resources.Split_SimpleList.ToCharArray()[0]);
        }

        /// <summary>
        /// イベントリストを昇順に並び替え
        /// </summary>
        private void ltvEVList_ColumnClick(object sender, ColumnClickEventArgs e) {
            this.mgr.MapData.SortEV(e.Column);
            this.UndoRedo.InitBuffer();
        }

        /// <summary>
        /// イベント選択時
        /// </summary>
        private void ltvEVList_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.ltvEVList.SelectedIndices.Count == 0 || this.localEVSelcting) {
                return;
            }

            //選択したイベントの座標にカーソルを移動させる
            this.localEVSelcting = true;
            this.mgr.Cursor = this.mgr.MapData[this.ltvEVList.SelectedIndices[0]].TilePosition;
            this.localEVSelcting = false;

            //イベントが見えるところまで画面をスクロールする
            Map.MapEditor_ScrollToCursor(this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr);
        }

        /// <summary>
        /// マップ保存
        /// </summary>
        private void tolSave_Click(object sender, EventArgs e) {
            if (this.pnlEditor.Visible && this.IsDirty && this.IsReadOnly == false && this.DoSave()) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// マップを画像で書き出し
        /// </summary>
        private void tolExport_Click(object sender, EventArgs e) {
            // ファイル名を決定する
            if (this.dlgSave.ShowDialog(this) != DialogResult.OK) {
                return;
            }

            // 描画先を一時的に変更して全領域を描画する
            var canvas = new Panel();
            DX.SetScreenFlipTargetWindow(canvas.Handle);
            canvas.Size = new Size(this.mgr.MapData.MapSize.Width * this.mgr.ScaledTileSize, this.mgr.MapData.MapSize.Height * this.mgr.ScaledTileSize);
            var drawer = new drwMapObjectForExport(canvas, this.mgr, new drwDXTilePallet(this.DBList), this.mgr.MapData?.TilesetFixedID ?? -1);
            drawer.LoadMaterial();
            drawer.ClsScreen();
            drawer.Draw();
            DX.SetDrawScreen(DX.DX_SCREEN_BACK);
            DX.ScreenFlip();
            var ret = DX.SaveDrawScreenToJPEG(0, 0, this.mgr.MapData.MapSize.Width * this.mgr.ScaledTileSize, this.mgr.MapData.MapSize.Height * this.mgr.ScaledTileSize, this.dlgSave.FileName, 100);

            // 描画先を復元する
            Media.ChangeDest(new drwMapObject(this.pnlPreview, this.mgr, new drwDXTilePallet(this.DBList), this.mgr.MapData?.TilesetFixedID ?? -1));
            this.dlgSave.FileName = "";

            if (ret == -1) {
                MessageBox.Show("エクスポートに失敗しました。\r\nマップが大きすぎる可能性があります。\r\nスケールを調整して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            } else {
                MessageBox.Show("エクスポートが完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
        }

        /// <summary>
        /// マップ設定
        /// </summary>
        private void tolMapSettings_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Map.dlgMapSettings(this.DBList, "マップ設定", this.mgr.MapData);
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                this.IsDirty = true;
                this.UndoRedo.InitBuffer();
                this.lblMapName.Text = this.mgr.MapData.Name;

                //カーボン元を読み込む
                this.mgr.MapData.BaseMapLayerTiles = mgrMapObject.LoadTileData(this.mgr.MapData.BaseMapFileName)?.LayerTiles;
            }
        }

        /// <summary>
        /// ２ペインでイベントスクリプトを開く
        /// </summary>
        private void tolOpenScript_Click(object sender, EventArgs e) {
            if (MessageBox.Show("イベントスクリプトの編集には十分注意して下さい。\r\n最悪の場合、エディターの動作が不安定になることがあります。\r\nリスクを理解した上で編集する場合は「はい」を押して下さい。", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) != DialogResult.Yes) {
                return;
            }

            var guid = this.mgr.MapData.GUID;
            var fileName = this.FileName;
            this.DoClose();             //アクセス制御の都合で、スクリプトを開く前にマップを閉じる
            this.OpenScriptRequested?.Invoke(this, new RequestOpenFileEventArgs(Resources.Path_Scripts_MapEvents + guid + Resources.Extension_SQ, MainTabIndices.Map));
            this.DoLoad(fileName);      //ここで読み取り専用としてマップが開き直される
        }

        /// <summary>
        /// タイル操作：元に戻す
        /// </summary>
        private void tolUndo_Click(object sender, EventArgs e) {
            if (this.pnlEditor.Visible && this.UndoRedo.CanUndo) {
                this.UndoRedo.Undo();
                this.IsDirty = true;
            }
        }

        /// <summary>
        /// タイル操作：やり直す
        /// </summary>
        private void tolRedo_Click(object sender, EventArgs e) {
            if (this.pnlEditor.Visible && this.UndoRedo.CanRedo) {
                this.UndoRedo.Redo();
                this.IsDirty = true;
            }
        }

        /// <summary>
        /// タイル操作：鉛筆モード
        /// </summary>
        private void tolModePencil_Click(object sender, EventArgs e) {
            this.mgr.EditMode = mgrMapObject.TileEditMode.Pencil;
            this.setModeCheck();
        }

        /// <summary>
        /// タイル操作：矩形モード
        /// </summary>
        private void tolModeRect_Click(object sender, EventArgs e) {
            this.mgr.EditMode = mgrMapObject.TileEditMode.Rectangle;
            this.setModeCheck();
        }

        /// <summary>
        /// タイル操作：楕円モード
        /// </summary>
        private void tolModeCircle_Click(object sender, EventArgs e) {
            this.mgr.EditMode = mgrMapObject.TileEditMode.Ellipse;
            this.setModeCheck();
        }

        /// <summary>
        /// タイル操作：塗りつぶしモード
        /// </summary>
        private void tolModeFill_Click(object sender, EventArgs e) {
            this.mgr.EditMode = mgrMapObject.TileEditMode.Fill;
            this.setModeCheck();
        }

        /// <summary>
        /// タイル操作：現在の編集ツールに従ってツールバーのアイコンにチェックを入れる
        /// </summary>
        private void setModeCheck() {
            if (this.lstMapLayerList.SelectedIndex <= (int) Map.Layer.High) {
                //タイルレイヤーのときのみ有効
                this.tolModePencil.Enabled = true;
                this.tolModePencil.Checked = (this.mgr.EditMode == mgrMapObject.TileEditMode.Pencil);
                this.tolModeRect.Enabled = true;
                this.tolModeRect.Checked = (this.mgr.EditMode == mgrMapObject.TileEditMode.Rectangle);
                this.tolModeCircle.Enabled = true;
                this.tolModeCircle.Checked = (this.mgr.EditMode == mgrMapObject.TileEditMode.Ellipse);
                this.tolModeFill.Enabled = true;
                this.tolModeFill.Checked = (this.mgr.EditMode == mgrMapObject.TileEditMode.Fill);
            } else {
                //それ以外は無効
                this.tolModePencil.Enabled = false;
                this.tolModeRect.Enabled = false;
                this.tolModeCircle.Enabled = false;
                this.tolModeFill.Enabled = false;
            }
        }

        /// <summary>
        /// スケール変更時
        /// </summary>
        private void tolScale_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.mgr != null) {
                this.mgr.Scale = (ViewScale.ScaleRate) this.tolScale.SelectedIndex;
                Settings.Default.Map_Scale = (int) this.mgr.Scale;
            }
        }

        /// <summary>
        /// マップ全体をシフト
        /// </summary>
        private void tolShift_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgShift();
            Dlg.SetLimit(new Point(-this.mgr.MapData.MapSize.Width, -this.mgr.MapData.MapSize.Height), new Point(this.mgr.MapData.MapSize));
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                this.mgr.MapData.Offset(Dlg.Result);
                this.UndoRedo.InitBuffer();     //元に戻せないので変更履歴バッファをクリア
            }
        }

        /// <summary>
        /// グリッド表示切替
        /// </summary>
        private void tolGrid_Click(object sender, EventArgs e) {
            this.mgr.VisibleGrid = this.tolGrid.Checked;
            Settings.Default.Map_VisibleGrid = this.mgr.VisibleGrid;
        }

        /// <summary>
        /// ショートカットキー：グリッド表示切替
        /// </summary>
        private void mnuGrid_Click(object sender, EventArgs e) {
            this.tolGrid.Checked = !this.tolGrid.Checked;
            this.tolGrid_Click(sender, e);
        }

        /// <summary>
        /// マップ自動生成
        /// </summary>
        private void tolGenerateMap_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Map.dlgMapGenerator(this.mgr.MapData.MapSize, this.lstMapLayerList.SelectedIndex);
            if (Dlg.ShowDialog() != DialogResult.OK) {
                return;
            }

            //タイル配置準備
            this.lstMapLayerList.SelectedIndex = Dlg.DestLayerIndex;
            this.mgr.EditMode = mgrMapObject.TileEditMode.Pencil;

            //生成されたデータに基づいてタイルを設定する: ここで設定したタイルは元に戻せるようにする
            this.oncePuts = new List<mgrMapObject.TileBuffer>();
            this.mgr.ClippingTiles = null;
            var tilePosList = Dlg.TilePalletPositions;
            for (var x = 0; x < this.mgr.MapData.MapSize.Width; x++) {
                for (var y = 0; y < this.mgr.MapData.MapSize.Height; y++) {
                    //該当タイルの種別に応じてパレット位置を変更する
                    this.mgr.SelectedPalletRange = new Rectangle(tilePosList[(int) Dlg.GeneratedMapData.TileData[x, y]], new Size(1, 1));
                    var puts = this.mgr.PutTiles(x, y, false);
                    foreach (var put in puts) {
                        this.oncePuts.Add(put);
                    }
                }
            }
            this.addOncePutsToUndoBuffer(this.oncePuts);

            //mgrの変更情報を画面上に反映する
            this.uctlTilePallet.SelectedRange = this.mgr.SelectedPalletRange;
            this.uctlTilePallet_SelectedPalletTile(sender, e);
        }

        /// <summary>
        /// マップテスト
        /// </summary>
        private void tolMapTest_Click(object sender, EventArgs e) {
            if (!this.pnlEditor.Visible) {
                return;
            }

            this.DoSave();

            //Ctrlキーを押していた場合、オプション画面を経由させる
            var buf = $"mt \"{this.FileName}\" {this.mgr.StartPosition.X} {this.mgr.StartPosition.Y}";
            if ((Control.ModifierKeys & Keys.Control) == Keys.Control) {
                this.tolMapTestWithSettings_Click(sender, e);
                return;
            }

            //マップを編集しながら、ゲーム画面も動作させるのはかなり処理が重たい
            if (Settings.Default.Map_WaitForTest) {
                //低スペック用：テスト中に動作を停止させる
                Media.StopDraw = true;
                Common.RunTester(buf, true);
                Media.StopDraw = false;
            } else {
                //高スペック用：テスト中も編集できる
                Common.RunTester(buf, false);
            }
        }

        /// <summary>
        /// 設定を適用してマップテスト
        /// </summary>
        private void tolMapTestWithSettings_Click(object sender, EventArgs e) {
            if (!this.pnlEditor.Visible) {
                return;
            }

            this.DoSave();

            var Dlg = new Dialog.Map.dlgMapTest(this.DBList, this.mgr.MapData);
            if (Dlg.ShowDialog() != DialogResult.OK) {
                return;
            }

            Settings.Default.Last_MapTestValueData = Dlg.Result;
            var buf = $"mt \"{this.FileName}\" {this.mgr.StartPosition.X} {this.mgr.StartPosition.Y}";
            buf += $" \"{Dlg.Result}\"";

            //マップを編集しながら、ゲーム画面も動作させるのはかなり処理が重たい
            if (Settings.Default.Map_WaitForTest) {
                //低スペック用：テスト中に動作を停止させる
                Media.StopDraw = true;
                Common.RunTester(buf, true);
                Media.StopDraw = false;
            } else {
                //高スペック用：テスト中も編集できる
                Common.RunTester(buf, false);
            }
        }

        /// <summary>
        /// イベントのサブメニューを開く
        /// </summary>
        private void EVRightMenu_Opening(object sender, CancelEventArgs e) {
            //コントロール有効無効
            var isEnabled = (this.ltvEVList.SelectedIndices.Count > 0);
            this.ctmEditEV.Visible = isEnabled;
            this.ctmCutEV.Enabled = isEnabled;
            this.ctmCopyEV.Enabled = isEnabled;
            this.ctmPasteEV.Enabled = (this.mgr.EVClipboard != null);
            this.ctmDeleteEV.Enabled = isEnabled;
        }

        /// <summary>
        /// イベント：デザイナー上で編集しようとした場合、同一座標に複数のイベントがないかチェックする
        /// </summary>
        private int selectSamePosEV(object sender) {
            var index = this.ltvEVList.SelectedIndices[0];
            if (this.EVRightMenu.SourceControl == this.pnlPreview || sender == null) {
                var distinguish = new Dialog.Common.dlgSelectInList("イベントの区別");
                var indexPairs = new Dictionary<int, int>();            //区別用リストのインデックスから実際のイベントリスト上のインデックスに変換するためのコレクション
                var count = 0;

                for (var i = 0; i < this.mgr.MapData.EVCount; i++) {
                    if (this.mgr.MapData[i].TilePosition == this.mgr.MapData[this.ltvEVList.SelectedIndices[0]].TilePosition) {
                        count++;
                        distinguish.AddItem(count, $" {this.mgr.MapData[i].VisibleID}:{this.mgr.MapData[i].Name}");
                        indexPairs.Add(count, i);
                    }
                }

                if (distinguish.ItemCount > 1) {
                    //イベントを区別させる
                    if (distinguish.ShowDialog(this) != DialogResult.OK) {
                        return -1;
                    }
                    index = indexPairs[distinguish.GetResultID()];
                } else if (distinguish.ItemCount == 0) {
                    //対象座標にイベントが存在しなかった
                    return -1;
                }

                this.ltvEVList.SelectedIndices.Add(index);
            }

            return index;
        }

        /// <summary>
        /// イベント：編集
        /// </summary>
        private void ctmEditEV_Click(object sender, EventArgs e) {
            if (this.ltvEVList.SelectedIndices.Count == 0) {
                return;
            }

            //デザイナー上で編集しようとした場合、同一座標に複数のイベントがないかチェックする
            var index = this.selectSamePosEV(sender);
            if (index == -1) {
                return;
            }

            //編集画面を開く
            var Dlg = new Dialog.Map.dlgEVSettings(this.mgr.MapData[index].Clone() as mgrMapObject.MapOneData.EventOneData, this.mgr.MapData.GUID, this.comTree, this.DBCtrl);

            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                //変更履歴バッファに追加する
                this.UndoRedo.Do(
                    new Action<mgrMapObject.MapOneData.EventOneData, int>((after, evIndex) => {
                        //これから実行する操作
                        this.mgr.MapData.ReplaceEV(evIndex, after);
                    }),
                    new object[] { Dlg.Result.Clone(), index },
                    new Action<mgrMapObject.MapOneData.EventOneData, int>((before, evIndex) => {
                        //元に戻す操作
                        this.mgr.MapData.ReplaceEV(evIndex, before);
                    }),
                    new object[] { this.mgr.MapData[index].Clone(), index },
                    false
                );
            }
        }

        /// <summary>
        /// イベント：追加
        /// </summary>
        private void ctmAddEV_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Map.dlgEVTemplates(
                new mgrMapObject.MapOneData.EventOneData(0, "新しいイベント", this.mgr.Cursor) {
                    VisibleID = this.mgr.MapData.GenerateVisibleEVID(),
                },
                this.mgr.MapData.GUID,
                this.comTree,
                this.DBCtrl);
            Media.StopDraw = true;

            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                //変更履歴バッファに追加する
                this.UndoRedo.Do(
                    new Action<mgrMapObject.MapOneData.EventOneData>((after) => {
                        //これから実行する操作
                        this.mgr.MapData.AddEV(after);
                    }),
                    new object[] { Dlg.Result.Clone() },
                    new Action<int>((evIndex) => {
                        //元に戻す操作
                        this.mgr.MapData.DeleteEV(evIndex);
                    }),
                    new object[] { this.mgr.MapData.EVCount },
                    false
                );
            }

            this.pnlPreview_VisibleChanged(this, null);
        }

        /// <summary>
        /// イベント：ここを初期位置に設定
        /// </summary>
        private void ctmSetStartPos_Click(object sender, EventArgs e) {
            this.mgr.StartPosition = this.mgr.Cursor;
        }

        /// <summary>
        /// イベント：ここを初期位置にしてテスト開始
        /// </summary>
        private void ctmSetStartPosAndPlay_Click(object sender, EventArgs e) {
            this.ctmSetStartPos_Click(sender, e);
            this.tolMapTest_Click(sender, e);
        }

        /// <summary>
        /// イベント：切り取り
        /// </summary>
        private void ctmCutEV_Click(object sender, EventArgs e) {
            //デザイナー上で編集しようとした場合、同一座標に複数のイベントがないかチェックする
            var index = this.selectSamePosEV(sender);
            if (index == -1) {
                return;
            }
            this.ctmCopyEV_Click(null, e);
            this.ctmDeleteEV_Click(null, e);
        }

        /// <summary>
        /// イベント：コピー
        /// </summary>
        private void ctmCopyEV_Click(object sender, EventArgs e) {
            if (this.ltvEVList.SelectedIndices.Count == 0) {
                return;
            }

            //デザイナー上で編集しようとした場合、同一座標に複数のイベントがないかチェックする
            var index = this.ltvEVList.SelectedIndices[0];
            if (sender != null) {
                index = this.selectSamePosEV(sender);
                if (index == -1) {
                    return;
                }
            }

            //変更履歴バッファに追加する
            this.UndoRedo.Do(
                new Action<mgrMapObject.MapOneData.EventOneData>((after) => {
                    //これから実行する操作
                    this.mgr.EVClipboard = after;
                }),
                new object[] { this.mgr.MapData[this.ltvEVList.SelectedIndices[0]].Clone() },
                new Action<mgrMapObject.MapOneData.EventOneData>((before) => {
                    //元に戻す操作
                    this.mgr.EVClipboard = before;
                }),
                new object[] { this.mgr.EVClipboard?.Clone() },
                false
            );
        }

        /// <summary>
        /// イベント：貼り付け
        /// </summary>
        private void ctmPasteEV_Click(object sender, EventArgs e) {
            if (this.mgr.EVClipboard != null) {
                this.mgr.EVClipboard.TilePosition = this.mgr.Cursor;

                //変更履歴バッファに追加する
                this.UndoRedo.Do(
                    new Action<mgrMapObject.MapOneData.EventOneData>((after) => {
                        //これから実行する操作
                        this.mgr.MapData.AddEV(after);
                    }),
                    new object[] { this.mgr.EVClipboard.Clone() },
                    new Action<int>((evIndex) => {
                        //元に戻す操作
                        this.mgr.MapData.DeleteEV(evIndex);
                    }),
                    new object[] { this.mgr.MapData.EVCount },
                    false
                );
            }
        }

        /// <summary>
        /// イベント：削除
        /// </summary>
        private void ctmDeleteEV_Click(object sender, EventArgs e) {
            if (this.ltvEVList.SelectedIndices.Count == 0) {
                return;
            }

            //デザイナー上で編集しようとした場合、同一座標に複数のイベントがないかチェックする
            var index = this.ltvEVList.SelectedIndices[0];
            if (sender != null) {
                index = this.selectSamePosEV(sender);
                if (index == -1) {
                    return;
                }
            }

            //変更履歴バッファに追加する
            this.UndoRedo.Do(
                new Action<int>((evIndex) => {
                    //これから実行する操作
                    this.mgr.MapData.DeleteEV(index);
                }),
                new object[] { this.ltvEVList.SelectedIndices[0] },
                new Action<mgrMapObject.MapOneData.EventOneData>((before) => {
                    //元に戻す操作
                    this.mgr.MapData.AddEV(before);
                }),
                new object[] { this.mgr.MapData[index].Clone() },
                false
            );
        }

        /// <summary>
        /// マップの固定IDをクリップボードに送る
        /// </summary>
        private void lblMapSize_Click(object sender, EventArgs e) {
            if (this.mgr.MapData?.GUID != null) {
                Clipboard.SetText(this.mgr.MapData.GUID);
            }
        }

        /// <summary>
        /// マップの固定IDをクリップボードに送る
        /// </summary>
        private void lblMapFixedID_Click(object sender, EventArgs e) {
            if (this.mgr.MapData?.GUID != null) {
                Clipboard.SetText(this.mgr.MapData.GUID);
            }
        }

        /// <summary>
        /// パレット：タイルが選択されたらクリッピング領域を更新する
        /// </summary>
        private void uctlTilePallet_SelectedPalletTile(object sender, EventArgs e) {
            if (this.uctlTilePallet.SelectedRange == null) {
                this.lblMapPalletCursor.Text = $"パレット: 無効領域";
            } else {
                if (this.uctlTilePallet.SelectedRange.Value.IsEmpty) {
                    this.mgr.ClippingTiles = null;
                } else {
                    this.mgr.ClippingTiles = new mgrMapObject.MapOneData.TileData[this.uctlTilePallet.SelectedRange.Value.Width, this.uctlTilePallet.SelectedRange.Value.Height];
                    for (var x = 0; x < this.mgr.ClippingTiles.GetLength(0); x++) {
                        for (var y = 0; y < this.mgr.ClippingTiles.GetLength(1); y++) {
                            this.mgr.ClippingTiles[x, y].PalletPosition = new Point(this.uctlTilePallet.SelectedRange.Value.X + x, this.uctlTilePallet.SelectedRange.Value.Y + y);
                        }
                    }
                    this.mgr.SelectedPalletRange = this.uctlTilePallet.SelectedRange.Value;
                }
                this.lblMapPalletCursor.Text = $"パレット: ({this.mgr.SelectedPalletRange.X,3},{this.mgr.SelectedPalletRange.Y,3}) {this.uctlTilePallet.SelectedTileHitInfo} GRP:{this.uctlTilePallet.SelectedTileGroupNumber}";
            }
        }

        /// <summary>
        /// デザイナー：画面上に描画領域が出てきたら描画準備を行う
        /// </summary>
        private void pnlPreview_VisibleChanged(object sender, EventArgs e) {
            if (this.DesignMode == false && this.pnlPreview.Visible) {
                Media.ChangeDest(new drwMapObject(this.pnlPreview, this.mgr, new drwDXTilePallet(this.DBList), this.mgr.MapData?.TilesetFixedID ?? -1));
                this.tblMapDesigner_Resize(sender, null);
            }
        }

        /// <summary>
        /// デザイナー：MainWndが中継したキー入力イベントを受け取る
        /// </summary>
        public void pnlPreview_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            if (!this.pnlPreview.Focused) {
                return;
            }

            //レイヤー切り替えをショートカットする
            switch (e.KeyData) {
                case Keys.D1:
                    this.lstMapLayerList.SelectedIndex = (int) Map.Layer.Low;
                    break;
                case Keys.D2:
                    this.lstMapLayerList.SelectedIndex = (int) Map.Layer.Middle;
                    break;
                case Keys.D3:
                    this.lstMapLayerList.SelectedIndex = (int) Map.Layer.High;
                    break;
                case Keys.D4:
                    this.lstMapLayerList.SelectedIndex = (int) Map.Layer.Shadow;
                    break;
                case Keys.D5:
                    this.lstMapLayerList.SelectedIndex = (int) Map.Layer.Event;
                    break;
            }

            if (this.mgr.CurrentLayer <= (int) Map.Layer.High) {
                //影層を除くタイルレイヤーでは選択ツールをショートカットする
                switch (e.KeyData) {
                    case Keys.Q:
                        this.tolModePencil_Click(this, null);
                        break;
                    case Keys.W:
                        this.tolModeRect_Click(this, null);
                        break;
                    case Keys.E:
                        this.tolModeCircle_Click(this, null);
                        break;
                    case Keys.R:
                        this.tolModeFill_Click(this, null);
                        break;
                }
            } else if (this.mgr.CurrentLayer == (int) Map.Layer.Event) {
                //イベントレイヤーではカーソル移動をショートカットする
                switch (e.KeyData) {
                    case Keys.Q:    //追加
                        this.ctmAddEV_Click(sender, e);
                        break;
                    case Keys.E:    //編集
                        if (this.ltvEVList.SelectedIndices.Count > 0) {
                            //現在の座標にイベントがある場合はリスト上で選択状態にする
                            for (var i = 0; i < this.mgr.MapData.EVCount; i++) {
                                if (this.mgr.MapData[i].TilePosition == this.mgr.MapData[this.ltvEVList.SelectedIndices[0]].TilePosition) {
                                    //最初にヒットしたイベントを選択する
                                    this.clippingEVStartPosition = this.mgr.Cursor;
                                    this.mgr.ClippingEVIndex = this.ltvEVList.SelectedIndices[0];
                                    this.ctmEditEV_Click(sender, e);
                                    break;
                                }
                            }
                        }
                        break;

                    default:
                        this.mgr.MoveCursorByKeyDown(e.KeyData);
                        break;
                }
            }
        }

        /// <summary>
        /// デザイナー：スクロールしたら描画領域を更新する
        /// </summary>
        private void designerScroller_Scroll(object sender, ScrollEventArgs e) {
            this.tblMapDesigner_Resize(sender, null);
        }

        /// <summary>
        /// デザイナー：マウスホイール
        /// </summary>
        private void pnlPreview_MouseWheel(object sender, MouseEventArgs e) {
            Map.MapEditor_MouseWheel(e, this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr, this.tolScale);
        }

        /// <summary>
        /// デザイナー：リサイズやスクロールをしたら描画領域を更新する
        /// </summary>
        private void tblMapDesigner_Resize(object sender, EventArgs e) {
            Map.MapEditor_Resize(this.hscDesignerScroller, this.vscDesignerScroller, this.pnlPreviewContainer, this.mgr);
            if (this.mgr?.MapData != null) {
                //表示を更新
                this.lblMapSize.Text = $"全体サイズ: {this.mgr.MapData.MapSize.Width}x{this.mgr.MapData.MapSize.Height}";
                this.lblMapRange.Text = $"表示範囲: ({this.hscDesignerScroller.Value / this.mgr.ScaledTileSize,3}, {this.vscDesignerScroller.Value / this.mgr.ScaledTileSize,3})~({Math.Ceiling(this.mgr.ViewRange.Right / (double) this.mgr.ScaledTileSize - 1),3},{Math.Ceiling(this.mgr.ViewRange.Bottom / (double) this.mgr.ScaledTileSize - 1),3})";
            }
        }

        /// <summary>
        /// デザイナー：カーソル移動を検知したら情報表示を反映させる
        /// </summary>
        private void cursorMoved(object sender, EventArgs e) {
            this.lblMapCursor.Text = $"カーソル: ({this.mgr.Cursor.X,3}, {this.mgr.Cursor.Y,3})";
            if (this.mgr?.MapData == null || this.localEVSelcting) {
                return;
            }

            //当該座標のイベントを選択する
            this.localEVSelcting = true;
            this.ltvEVList.SelectedIndices.Clear();

            for (var i = 0; i < this.mgr.MapData.EVCount; i++) {
                if (this.mgr.MapData[i].TilePosition == this.mgr.Cursor) {
                    this.ltvEVList.SelectedIndices.Add(i);
                    break;
                }
            }

            this.localEVSelcting = false;
        }

        /// <summary>
        /// デザイナー：コントロールにマウスが入ったらアクティブにする
        /// </summary>
        private void pnlPreview_MouseEnter(object sender, EventArgs e) {
            this.pnlPreview.Focus();
            if (Media.TargetPtr != this.pnlPreview.Handle) {
                //デザイナーの描画が無効になっていたら復元する
                this.pnlPreview_VisibleChanged(sender, e);
            }
        }

        /// <summary>
        /// デザイナー：ダブルクリックされたとき
        /// </summary>
        private void pnlPreview_DoubleClick(object sender, EventArgs e) {
            if (!this.mgr.SwitchLayers[this.mgr.CurrentLayer]) {
                //現在のレイヤーが非表示である場合は処理しない
                return;
            }

            if (this.mgr.CurrentLayer == (int) Map.Layer.Event) {
                //イベントの編集を開始する
                if (this.ltvEVList.SelectedIndices.Count == 0 || (Control.ModifierKeys & Keys.Shift) == Keys.Shift) {
                    this.ctmAddEV_Click(null, null);
                } else {
                    this.ctmEditEV_Click(null, null);
                }
            }
        }

        /// <summary>
        /// デザイナー：マウスボタンが押されたとき
        /// </summary>
        private void pnlPreview_MouseDown(object sender, MouseEventArgs e) {
            if (!this.mgr.SwitchLayers[this.mgr.CurrentLayer]) {
                //現在のレイヤーが非表示である場合は処理しない
                return;
            }

            //イベントレイヤー以外では、MouseMoveイベントでカーソルが予め選択されていると想定する
            switch (this.mgr.CurrentLayer) {
                case (int) Map.Layer.Low:
                case (int) Map.Layer.Middle:
                case (int) Map.Layer.High:
                    switch (e.Button) {
                        case MouseButtons.Left:
                            //現在の編集ツールに従った操作
                            switch (this.mgr.EditMode) {
                                case mgrMapObject.TileEditMode.Pencil:
                                    //タイルを配置し、一連の操作を開始する
                                    this.oncePuts = new List<mgrMapObject.TileBuffer>();
                                    this.putOneTileWithPencil();
                                    break;

                                case mgrMapObject.TileEditMode.Rectangle:
                                case mgrMapObject.TileEditMode.Ellipse:
                                    //確定前領域を生成
                                    this.mgr.PuttingTileStart = this.mgr.Cursor;
                                    this.mgr.PuttingTileRange = new Rectangle(this.mgr.Cursor.X, this.mgr.Cursor.Y, 1, 1);
                                    break;

                                case mgrMapObject.TileEditMode.Fill:
                                    //即座に塗りつぶしを実行し、一連の操作として変更履歴バッファに追加する
                                    var puts = this.mgr.FillTiles(this.mgr.Cursor.X, this.mgr.Cursor.Y, (Control.ModifierKeys & Keys.Shift) == Keys.Shift);
                                    this.addOncePutsToUndoBuffer(puts);
                                    break;
                            }
                            break;

                        case MouseButtons.Right:
                            //クリッピング領域を生成
                            this.mgr.ClippingTileStart = this.mgr.Cursor;
                            this.mgr.ClippingTileRange = new Rectangle(this.mgr.Cursor.X, this.mgr.Cursor.Y, 1, 1);
                            this.mgr.ClippingTiles = new mgrMapObject.MapOneData.TileData[1, 1];
                            this.mgr.ClippingTiles[0, 0] = this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, this.mgr.Cursor.X, this.mgr.Cursor.Y];

                            //スポイト：タイルパレットの対応部分を選択する
                            this.uctlTilePallet.SelectedRange = new Rectangle(this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, this.mgr.Cursor.X, this.mgr.Cursor.Y].PalletPosition, new Size(1, 1));
                            break;
                    }
                    break;

                case (int) Map.Layer.Shadow:
                    //影を配置し、一連の操作を開始する
                    this.oncePuts = new List<mgrMapObject.TileBuffer>();
                    this.putOneShadow(e.Button);
                    break;

                case (int) Map.Layer.Event:
                    //タイル選択する
                    this.mgr.Cursor = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
                    this.mgr.CursorQuarter = null;

                    //指定座標にイベントがある場合はクリッピングする
                    if (e.Button == MouseButtons.Left && this.ltvEVList.SelectedIndices.Count > 0) {
                        for (var i = 0; i < this.mgr.MapData.EVCount; i++) {
                            if (this.mgr.MapData[i].TilePosition == this.mgr.MapData[this.ltvEVList.SelectedIndices[0]].TilePosition) {
                                //最初にヒットしたイベントを移動させる
                                this.clippingEVStartPosition = this.mgr.Cursor;
                                this.mgr.ClippingEVIndex = this.ltvEVList.SelectedIndices[0];
                                break;
                            }
                        }
                    }
                    break;
            }
        }

        /// <summary>
        /// デザイナー：マウスが動いたとき
        /// </summary>
        private void pnlPreview_MouseMove(object sender, MouseEventArgs e) {
            if (!this.mgr.SwitchLayers[this.mgr.CurrentLayer]) {
                //現在のレイヤーが非表示である場合は処理しない
                return;
            }

            var cur = new Point();
            switch (this.mgr.CurrentLayer) {
                case (int) Map.Layer.Low:
                case (int) Map.Layer.Middle:
                case (int) Map.Layer.High:
                    //修飾キーガイドの表示
                    if ((Control.ModifierKeys & Keys.Shift) == Keys.Shift) {
                        this.lblModifierKey.Text = "オートタイル解除";
                    } else if ((Control.ModifierKeys & Keys.Control) == Keys.Control) {
                        this.lblModifierKey.Text = "マウスホイールでスケール変更";
                    } else {
                        this.lblModifierKey.Text = "";
                    }

                    //カーソル位置を更新
                    cur = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
                    if (cur == this.mgr.Cursor) {
                        //カーソルが移動しない場合は処理しない
                        return;
                    }
                    this.mgr.Cursor = cur;
                    this.mgr.CursorQuarter = null;

                    //一連の操作を継続する
                    switch (e.Button) {
                        case MouseButtons.Left:
                            //現在の編集ツールに従った操作
                            switch (this.mgr.EditMode) {
                                case mgrMapObject.TileEditMode.Pencil:
                                    this.putOneTileWithPencil();
                                    break;

                                case mgrMapObject.TileEditMode.Rectangle:
                                case mgrMapObject.TileEditMode.Ellipse:
                                    //確定前領域を更新
                                    cur = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
                                    cur = Common.ApplyLimit(cur, this.mgr.MapData.MapRectangle);
                                    this.mgr.PuttingTileRange = Rectangle.FromLTRB(
                                        (cur.X < this.mgr.PuttingTileStart.Value.X) ? cur.X : this.mgr.PuttingTileStart.Value.X,
                                        (cur.Y < this.mgr.PuttingTileStart.Value.Y) ? cur.Y : this.mgr.PuttingTileStart.Value.Y,
                                        (cur.X < this.mgr.PuttingTileStart.Value.X) ? this.mgr.PuttingTileStart.Value.X + 1 : cur.X + 1,
                                        (cur.Y < this.mgr.PuttingTileStart.Value.Y) ? this.mgr.PuttingTileStart.Value.Y + 1 : cur.Y + 1
                                    );
                                    this.mgr.PuttingTileRange = Common.ApplyLimit(this.mgr.PuttingTileRange, this.mgr.MapData.MapRectangle);
                                    break;

                                case mgrMapObject.TileEditMode.Fill:
                                    //操作が終わっているので処理しない
                                    return;
                            }
                            break;

                        case MouseButtons.Right:
                            //クリッピング領域を更新
                            cur = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
                            cur = Common.ApplyLimit(cur, this.mgr.MapData.MapRectangle);
                            this.mgr.ClippingTileRange = Rectangle.FromLTRB(
                                (cur.X < this.mgr.ClippingTileStart.Value.X) ? cur.X : this.mgr.ClippingTileStart.Value.X,
                                (cur.Y < this.mgr.ClippingTileStart.Value.Y) ? cur.Y : this.mgr.ClippingTileStart.Value.Y,
                                (cur.X < this.mgr.ClippingTileStart.Value.X) ? this.mgr.ClippingTileStart.Value.X + 1 : cur.X + 1,
                                (cur.Y < this.mgr.ClippingTileStart.Value.Y) ? this.mgr.ClippingTileStart.Value.Y + 1 : cur.Y + 1
                            );
                            this.mgr.ClippingTileRange = Common.ApplyLimit(this.mgr.ClippingTileRange, this.mgr.MapData.MapRectangle);

                            //指定範囲をクリッピングする
                            this.mgr.ClippingTiles = new mgrMapObject.MapOneData.TileData[Math.Abs(this.mgr.ClippingTileRange.Width), Math.Abs(this.mgr.ClippingTileRange.Height)];
                            for (var x = 0; x < this.mgr.ClippingTiles.GetLength(0); x++) {
                                for (var y = 0; y < this.mgr.ClippingTiles.GetLength(1); y++) {
                                    this.mgr.ClippingTiles[x, y] = this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, this.mgr.ClippingTileRange.Location.X + x, this.mgr.ClippingTileRange.Location.Y + y];
                                }
                            }

                            //クリップ領域が複数タイルになった時点でパレットの選択を解除する
                            if (this.mgr.ClippingTiles.Length > 1) {
                                this.uctlTilePallet.SelectedRange = null;
                            }
                            break;
                    }
                    break;

                case (int) Map.Layer.Shadow:
                    //タイル選択し、一連の操作を継続する
                    this.mgr.Cursor = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
                    this.mgr.Cursor = Common.ApplyLimit(this.mgr.Cursor, this.mgr.MapData.MapRectangle);
                    cur.X = ((this.mgr.ViewRange.Left + e.X) - this.mgr.Cursor.X * this.mgr.ScaledTileSize) / (this.mgr.ScaledTileSize / 2);
                    cur.Y = ((this.mgr.ViewRange.Top + e.Y) - this.mgr.Cursor.Y * this.mgr.ScaledTileSize) / (this.mgr.ScaledTileSize / 2);
                    this.mgr.CursorQuarter = (Map.QuarterTile) ((cur.X <= 0 ? 0 : 1) + ((cur.Y <= 0 ? 0 : 1) * 2));
                    this.putOneShadow(e.Button);
                    break;

                case (int) Map.Layer.Event:
                    //左クリックされたままであれば、D&D操作を継続する
                    if (e.Button == MouseButtons.Left && this.mgr.ClippingEVIndex != -1) {
                        //タイル選択してその位置にクリッピングしているイベントを移動させる
                        this.localEVSelcting = true;
                        this.mgr.Cursor = new Point((this.mgr.ViewRange.Left + e.X) / this.mgr.ScaledTileSize, (this.mgr.ViewRange.Top + e.Y) / this.mgr.ScaledTileSize);
                        this.mgr.CursorQuarter = null;
                        this.mgr.MapData.MoveEV(this.mgr.ClippingEVIndex, this.mgr.Cursor);
                        this.localEVSelcting = false;
                    }
                    break;
            }
        }

        /// <summary>
        /// デザイナー：マウスボタンが離されたとき
        /// </summary>
        private void pnlPreview_MouseUp(object sender, MouseEventArgs e) {
            if (!this.mgr.SwitchLayers[this.mgr.CurrentLayer]) {
                return;     //現在のレイヤーが非表示である場合は処理しない
            }

            switch (this.mgr.CurrentLayer) {
                case (int) Map.Layer.Low:
                case (int) Map.Layer.Middle:
                case (int) Map.Layer.High:
                    switch (e.Button) {
                        case MouseButtons.Left:
                            //現在の編集ツールに従い、一連の操作を終える
                            switch (this.mgr.EditMode) {
                                case mgrMapObject.TileEditMode.Rectangle:
                                case mgrMapObject.TileEditMode.Ellipse:
                                    //確定前領域を確定させる
                                    this.oncePuts = new List<mgrMapObject.TileBuffer>();
                                    for (var x = this.mgr.PuttingTileRange.Left; x < this.mgr.PuttingTileRange.Right; x++) {
                                        for (var y = this.mgr.PuttingTileRange.Top; y < this.mgr.PuttingTileRange.Bottom; y++) {
                                            this.oncePuts.Add(new mgrMapObject.TileBuffer((Map.Layer) this.mgr.CurrentLayer, new Point(x, y), this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, x, y]));
                                        }
                                    }
                                    this.mgr.PutTiles(this.mgr.Cursor.X, this.mgr.Cursor.Y, (Control.ModifierKeys & Keys.Shift) == Keys.Shift);
                                    for (int x = this.mgr.PuttingTileRange.Left, a = 0; x < this.mgr.PuttingTileRange.Right; x++, a++) {
                                        for (int y = this.mgr.PuttingTileRange.Top, b = 0; y < this.mgr.PuttingTileRange.Bottom; y++, b++) {
                                            this.oncePuts[a * this.mgr.PuttingTileRange.Height + b].After = this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, x, y];
                                        }
                                    }
                                    this.mgr.PuttingTileStart = null;
                                    break;

                                case mgrMapObject.TileEditMode.Fill:
                                    //操作が終わっているので処理しない
                                    return;
                            }

                            //変更履歴バッファに追加する
                            this.addOncePutsToUndoBuffer(this.oncePuts);
                            break;

                        case MouseButtons.Right:
                            //クリッピングを終了する
                            this.mgr.ClippingTileStart = null;
                            break;
                    }
                    break;

                case (int) Map.Layer.Shadow:
                    //一連の操作を終えて、変更履歴バッファに追加する
                    this.addOncePutsToUndoBuffer(this.oncePuts);
                    break;

                case (int) Map.Layer.Event:
                    switch (e.Button) {
                        case MouseButtons.Left:
                            if (this.mgr.ClippingEVIndex == -1) {
                                break;
                            }

                            //実際に移動した場合のみ、変更履歴バッファに追加する
                            if (this.mgr.Cursor != this.clippingEVStartPosition) {
                                this.UndoRedo.Do(
                                    new Action<Point, int>((after, index) => {
                                        //これから実行する操作
                                        this.mgr.MapData[index].TilePosition = after;
                                        this.ltvEVList.SelectedIndices.Clear();
                                        this.ltvEVList.SelectedIndices.Add(index);
                                    }),
                                    new object[] { this.mgr.MapData[this.mgr.ClippingEVIndex].TilePosition, this.mgr.ClippingEVIndex },
                                    new Action<Point, int>((before, index) => {
                                        //元に戻す操作
                                        this.mgr.MapData[index].TilePosition = before;
                                        this.ltvEVList.SelectedIndices.Clear();
                                        this.ltvEVList.SelectedIndices.Add(index);
                                    }),
                                    new object[] { this.clippingEVStartPosition, this.mgr.ClippingEVIndex },
                                    false
                                );
                            }

                            //クリッピングを終了する
                            this.mgr.ClippingEVIndex = -1;
                            break;

                        case MouseButtons.Right:
                            //右クリックメニューを表示する
                            this.EVRightMenu.Show(this.pnlPreview, e.X, e.Y);
                            break;
                    }
                    break;
            }
        }

        /// <summary>
        /// 現在選択しているタイルに鉛筆ツールとしてタイルを配置します。これは一連のタイル操作に組み込まれます。
        /// </summary>
        private void putOneTileWithPencil() {
            var puts = this.mgr.PutTiles(this.mgr.Cursor.X, this.mgr.Cursor.Y, (Control.ModifierKeys & Keys.Shift) == Keys.Shift);
            foreach (var put in puts) {
                this.oncePuts.Add(put);
            }
        }

        /// <summary>
        /// 現在の位置に影を配置します。これは一連のタイル操作に組み込まれます。
        /// </summary>
        private void putOneShadow(MouseButtons eBtn) {
            var onePut = new mgrMapObject.TileBuffer((Map.Layer) this.mgr.CurrentLayer, this.mgr.Cursor, this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, this.mgr.Cursor.X, this.mgr.Cursor.Y]);
            switch (eBtn) {
                case MouseButtons.Left:
                    //影を描く
                    this.mgr.PutShadow(this.mgr.Cursor.X, this.mgr.Cursor.Y, this.mgr.CursorQuarter, false);
                    break;
                case MouseButtons.Right:
                    //影を消す
                    this.mgr.PutShadow(this.mgr.Cursor.X, this.mgr.Cursor.Y, this.mgr.CursorQuarter, true);
                    break;
                default:
                    //操作を行わない
                    return;
            }
            onePut.After = this.mgr.MapData.LayerTiles[this.mgr.CurrentLayer, this.mgr.Cursor.X, this.mgr.Cursor.Y];
            this.oncePuts.Add(onePut);
        }

        /// <summary>
        /// 一連のタイル操作を変更履歴バッファに追加します。
        /// </summary>
        private void addOncePutsToUndoBuffer(List<mgrMapObject.TileBuffer> puts) {
            this.UndoRedo.Do(
                new Action<List<mgrMapObject.TileBuffer>>((once) => {
                    //実際に行った操作
                    for (var i = 0; i < once.Count; i++) {
                        this.mgr.MapData.LayerTiles[(int) once[i].Layer, once[i].TilePosition.X, once[i].TilePosition.Y] = once[i].After;
                        this.mgr.ApplyAutoTile(once[i].Layer, once[i].TilePosition.X, once[i].TilePosition.Y);
                    }
                }),
                new object[] { puts },
                new Action<List<mgrMapObject.TileBuffer>>((once) => {
                    //元に戻す操作
                    for (var i = once.Count - 1; i >= 0; i--) {
                        this.mgr.MapData.LayerTiles[(int) once[i].Layer, once[i].TilePosition.X, once[i].TilePosition.Y] = once[i].Before;
                        this.mgr.ApplyAutoTile(once[i].Layer, once[i].TilePosition.X, once[i].TilePosition.Y);
                    }
                }),
                new object[] { puts },
                true
            );
        }

        /// <summary>
        /// イベントリストが変更されたとき
        /// </summary>
        private void eventhandler_EVListChanged(object sender, EventArgs e) {
            this.ltvEVList.BeginUpdate();
            this.ltvEVList.Items.Clear();
            for (var i = 0; i < this.mgr.MapData.EVCount; i++) {
                var ev = this.mgr.MapData[i];
                var item = new ListViewItem {
                    Text = ev.FixedID.ToString()
                };
                item.SubItems.Add(ev.VisibleID.ToString());
                item.SubItems.Add(ev.Name);
                this.ltvEVList.Items.Add(item);
            }
            this.ltvEVList.EndUpdate();

            this.cursorMoved(sender, e);
            this.IsDirty = true;
        }

        /// <summary>
        /// タイルセットが変更されたとき
        /// </summary>
        private void eventhandler_TilesetChanged(object sender, EventArgs e) {
            this.ctlMapEditor_VisibleChanged(sender, e);
            Media.ChangeDest(new drwMapObject(this.pnlPreview, this.mgr, new drwDXTilePallet(this.DBList), this.mgr.MapData?.TilesetFixedID ?? -1));
        }

        /// <summary>
        /// タイルが変更されたとき
        /// </summary>
        private void eventhandler_TileEditted(object sender, EventArgs e) {
            this.IsDirty = true;
        }

        /// <summary>
        /// 変更履歴バッファが変更されたとき
        /// </summary>
        private void eventhandler_UndoBufferChanged(object sender, EventArgs e) {
            this.tolUndo.Enabled = this.UndoRedo.CanUndo;
            this.tolRedo.Enabled = this.UndoRedo.CanRedo;
        }

    }
}
