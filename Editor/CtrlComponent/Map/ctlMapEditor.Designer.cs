namespace Editor.CtrlComponent.Map {
	partial class ctlMapEditor {
		/// <summary> 
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region コンポーネント デザイナーで生成されたコード

		/// <summary> 
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlMapEditor));
            this.tolGrid = new System.Windows.Forms.ToolStripButton();
            this.statusBar = new System.Windows.Forms.StatusStrip();
            this.lblMapName = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblMapPalletCursor = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblMapCursor = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblMapRange = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblMapSize = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblMapFixedID = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblDummyForSpring = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblModifierKey = new System.Windows.Forms.ToolStripStatusLabel();
            this.EVRightMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ctmEditEV = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmAddEV = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmSetStartPos = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmSetStartPosAndPlay = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
            this.ctmCutEV = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmCopyEV = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmPasteEV = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmDeleteEV = new System.Windows.Forms.ToolStripMenuItem();
            this.tolShift = new System.Windows.Forms.ToolStripButton();
            this.toolBar = new System.Windows.Forms.ToolStrip();
            this.tolSave = new System.Windows.Forms.ToolStripButton();
            this.tolExport = new System.Windows.Forms.ToolStripButton();
            this.tolMapSettings = new System.Windows.Forms.ToolStripButton();
            this.tolOpenScript = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.tolUndo = new System.Windows.Forms.ToolStripButton();
            this.tolRedo = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tolModePencil = new System.Windows.Forms.ToolStripButton();
            this.tolModeRect = new System.Windows.Forms.ToolStripButton();
            this.tolModeCircle = new System.Windows.Forms.ToolStripButton();
            this.tolModeFill = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
            this.tolScale = new System.Windows.Forms.ToolStripComboBox();
            this.ToolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.tolGenerateMap = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.tolMapTest = new System.Windows.Forms.ToolStripSplitButton();
            this.tolMapTestWithSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.pnlPreviewScroller = new System.Windows.Forms.Panel();
            this.tblMapDesigner = new System.Windows.Forms.TableLayoutPanel();
            this.hscDesignerScroller = new System.Windows.Forms.HScrollBar();
            this.vscDesignerScroller = new System.Windows.Forms.VScrollBar();
            this.pnlPreviewContainer = new System.Windows.Forms.Panel();
            this.pnlPreview = new System.Windows.Forms.Panel();
            this.splMap1 = new System.Windows.Forms.SplitContainer();
            this.splMap2 = new System.Windows.Forms.SplitContainer();
            this.panel6 = new System.Windows.Forms.Panel();
            this.uctlMapTree = new Editor.CtrlComponent.Common.ctlFileTree();
            this.tosMapFileToolBar = new System.Windows.Forms.ToolStrip();
            this.tolMapNewFile = new System.Windows.Forms.ToolStripButton();
            this.tolMapNewFolder = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.tolMapCopy = new System.Windows.Forms.ToolStripButton();
            this.tolMapPaste = new System.Windows.Forms.ToolStripButton();
            this.tolMapDelete = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.tolMapReloadFiles = new System.Windows.Forms.ToolStripButton();
            this.tolOpenFolder = new System.Windows.Forms.ToolStripButton();
            this.pnlPallet = new System.Windows.Forms.Panel();
            this.ltvEVList = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ColumnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ColumnHeader16 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.uctlTilePallet = new Editor.CtrlComponent.Map.ctlTilePallet();
            this.lstMapLayerList = new System.Windows.Forms.CheckedListBox();
            this.pnlEditor = new System.Windows.Forms.Panel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.ショートカットキーToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuMapSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuUndo = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuRedo = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuGrid = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuMapTest = new System.Windows.Forms.ToolStripMenuItem();
            this.dlgSave = new System.Windows.Forms.SaveFileDialog();
            this.statusBar.SuspendLayout();
            this.EVRightMenu.SuspendLayout();
            this.toolBar.SuspendLayout();
            this.pnlPreviewScroller.SuspendLayout();
            this.tblMapDesigner.SuspendLayout();
            this.pnlPreviewContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splMap1)).BeginInit();
            this.splMap1.Panel1.SuspendLayout();
            this.splMap1.Panel2.SuspendLayout();
            this.splMap1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splMap2)).BeginInit();
            this.splMap2.Panel1.SuspendLayout();
            this.splMap2.Panel2.SuspendLayout();
            this.splMap2.SuspendLayout();
            this.panel6.SuspendLayout();
            this.tosMapFileToolBar.SuspendLayout();
            this.pnlPallet.SuspendLayout();
            this.pnlEditor.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tolGrid
            // 
            this.tolGrid.CheckOnClick = true;
            this.tolGrid.Image = ((System.Drawing.Image)(resources.GetObject("tolGrid.Image")));
            this.tolGrid.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolGrid.Name = "tolGrid";
            this.tolGrid.Size = new System.Drawing.Size(58, 22);
            this.tolGrid.Text = "グリッド";
            this.tolGrid.Click += new System.EventHandler(this.tolGrid_Click);
            // 
            // statusBar
            // 
            this.statusBar.AutoSize = false;
            this.statusBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblMapName,
            this.lblMapPalletCursor,
            this.lblMapCursor,
            this.lblMapRange,
            this.lblMapSize,
            this.lblMapFixedID,
            this.lblDummyForSpring,
            this.lblModifierKey});
            this.statusBar.Location = new System.Drawing.Point(0, 495);
            this.statusBar.Name = "statusBar";
            this.statusBar.Padding = new System.Windows.Forms.Padding(1, 0, 16, 0);
            this.statusBar.Size = new System.Drawing.Size(1086, 24);
            this.statusBar.SizingGrip = false;
            this.statusBar.TabIndex = 6;
            // 
            // lblMapName
            // 
            this.lblMapName.AutoSize = false;
            this.lblMapName.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.lblMapName.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblMapName.Name = "lblMapName";
            this.lblMapName.Size = new System.Drawing.Size(200, 19);
            this.lblMapName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblMapPalletCursor
            // 
            this.lblMapPalletCursor.AutoSize = false;
            this.lblMapPalletCursor.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.lblMapPalletCursor.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblMapPalletCursor.Name = "lblMapPalletCursor";
            this.lblMapPalletCursor.Size = new System.Drawing.Size(200, 19);
            this.lblMapPalletCursor.Text = "パレット:";
            this.lblMapPalletCursor.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblMapCursor
            // 
            this.lblMapCursor.AutoSize = false;
            this.lblMapCursor.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.lblMapCursor.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblMapCursor.Name = "lblMapCursor";
            this.lblMapCursor.Size = new System.Drawing.Size(150, 19);
            this.lblMapCursor.Text = "カーソル:";
            this.lblMapCursor.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblMapRange
            // 
            this.lblMapRange.AutoSize = false;
            this.lblMapRange.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.lblMapRange.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblMapRange.Name = "lblMapRange";
            this.lblMapRange.Size = new System.Drawing.Size(220, 19);
            this.lblMapRange.Text = "表示範囲:";
            this.lblMapRange.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblMapSize
            // 
            this.lblMapSize.AutoSize = false;
            this.lblMapSize.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.lblMapSize.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblMapSize.Name = "lblMapSize";
            this.lblMapSize.Size = new System.Drawing.Size(145, 19);
            this.lblMapSize.Text = "全体サイズ:";
            this.lblMapSize.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // lblMapFixedID
            // 
            this.lblMapFixedID.AutoSize = false;
            this.lblMapFixedID.BorderSides = System.Windows.Forms.ToolStripStatusLabelBorderSides.Left;
            this.lblMapFixedID.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblMapFixedID.Name = "lblMapFixedID";
            this.lblMapFixedID.Size = new System.Drawing.Size(300, 19);
            this.lblMapFixedID.Text = "FixedID:";
            this.lblMapFixedID.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblMapFixedID.Click += new System.EventHandler(this.lblMapFixedID_Click);
            // 
            // lblDummyForSpring
            // 
            this.lblDummyForSpring.Name = "lblDummyForSpring";
            this.lblDummyForSpring.Size = new System.Drawing.Size(0, 0);
            this.lblDummyForSpring.Spring = true;
            // 
            // lblModifierKey
            // 
            this.lblModifierKey.BorderStyle = System.Windows.Forms.Border3DStyle.SunkenOuter;
            this.lblModifierKey.Name = "lblModifierKey";
            this.lblModifierKey.Size = new System.Drawing.Size(0, 0);
            this.lblModifierKey.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // EVRightMenu
            // 
            this.EVRightMenu.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.EVRightMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ctmEditEV,
            this.ctmAddEV,
            this.ctmSetStartPos,
            this.ctmSetStartPosAndPlay,
            this.ToolStripSeparator10,
            this.ctmCutEV,
            this.ctmCopyEV,
            this.ctmPasteEV,
            this.ctmDeleteEV});
            this.EVRightMenu.Name = "CodeListRightMenu";
            this.EVRightMenu.Size = new System.Drawing.Size(199, 186);
            this.EVRightMenu.Opening += new System.ComponentModel.CancelEventHandler(this.EVRightMenu_Opening);
            // 
            // ctmEditEV
            // 
            this.ctmEditEV.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctmEditEV.Image = ((System.Drawing.Image)(resources.GetObject("ctmEditEV.Image")));
            this.ctmEditEV.Name = "ctmEditEV";
            this.ctmEditEV.Size = new System.Drawing.Size(198, 22);
            this.ctmEditEV.Text = "イベントを編集(&E)...";
            this.ctmEditEV.Click += new System.EventHandler(this.ctmEditEV_Click);
            // 
            // ctmAddEV
            // 
            this.ctmAddEV.Image = ((System.Drawing.Image)(resources.GetObject("ctmAddEV.Image")));
            this.ctmAddEV.Name = "ctmAddEV";
            this.ctmAddEV.Size = new System.Drawing.Size(198, 22);
            this.ctmAddEV.Text = "ここに追加(&A)...";
            this.ctmAddEV.Click += new System.EventHandler(this.ctmAddEV_Click);
            // 
            // ctmSetStartPos
            // 
            this.ctmSetStartPos.Image = ((System.Drawing.Image)(resources.GetObject("ctmSetStartPos.Image")));
            this.ctmSetStartPos.Name = "ctmSetStartPos";
            this.ctmSetStartPos.Size = new System.Drawing.Size(198, 22);
            this.ctmSetStartPos.Text = "ここを初期位置に設定(&S)";
            this.ctmSetStartPos.Click += new System.EventHandler(this.ctmSetStartPos_Click);
            // 
            // ctmSetStartPosAndPlay
            // 
            this.ctmSetStartPosAndPlay.Image = ((System.Drawing.Image)(resources.GetObject("ctmSetStartPosAndPlay.Image")));
            this.ctmSetStartPosAndPlay.Name = "ctmSetStartPosAndPlay";
            this.ctmSetStartPosAndPlay.Size = new System.Drawing.Size(198, 22);
            this.ctmSetStartPosAndPlay.Text = "ここからマップテスト(&P)...";
            this.ctmSetStartPosAndPlay.Click += new System.EventHandler(this.ctmSetStartPosAndPlay_Click);
            // 
            // ToolStripSeparator10
            // 
            this.ToolStripSeparator10.Name = "ToolStripSeparator10";
            this.ToolStripSeparator10.Size = new System.Drawing.Size(195, 6);
            // 
            // ctmCutEV
            // 
            this.ctmCutEV.Image = ((System.Drawing.Image)(resources.GetObject("ctmCutEV.Image")));
            this.ctmCutEV.Name = "ctmCutEV";
            this.ctmCutEV.Size = new System.Drawing.Size(198, 22);
            this.ctmCutEV.Text = "切り取り(&T)";
            this.ctmCutEV.Click += new System.EventHandler(this.ctmCutEV_Click);
            // 
            // ctmCopyEV
            // 
            this.ctmCopyEV.Image = ((System.Drawing.Image)(resources.GetObject("ctmCopyEV.Image")));
            this.ctmCopyEV.Name = "ctmCopyEV";
            this.ctmCopyEV.Size = new System.Drawing.Size(198, 22);
            this.ctmCopyEV.Text = "コピー(&C)";
            this.ctmCopyEV.Click += new System.EventHandler(this.ctmCopyEV_Click);
            // 
            // ctmPasteEV
            // 
            this.ctmPasteEV.Image = ((System.Drawing.Image)(resources.GetObject("ctmPasteEV.Image")));
            this.ctmPasteEV.Name = "ctmPasteEV";
            this.ctmPasteEV.Size = new System.Drawing.Size(198, 22);
            this.ctmPasteEV.Text = "貼り付け(&P)";
            this.ctmPasteEV.Click += new System.EventHandler(this.ctmPasteEV_Click);
            // 
            // ctmDeleteEV
            // 
            this.ctmDeleteEV.Image = ((System.Drawing.Image)(resources.GetObject("ctmDeleteEV.Image")));
            this.ctmDeleteEV.Name = "ctmDeleteEV";
            this.ctmDeleteEV.Size = new System.Drawing.Size(198, 22);
            this.ctmDeleteEV.Text = "削除(&D)";
            this.ctmDeleteEV.Click += new System.EventHandler(this.ctmDeleteEV_Click);
            // 
            // tolShift
            // 
            this.tolShift.Image = ((System.Drawing.Image)(resources.GetObject("tolShift.Image")));
            this.tolShift.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolShift.Name = "tolShift";
            this.tolShift.Size = new System.Drawing.Size(53, 22);
            this.tolShift.Text = "シフト";
            this.tolShift.Click += new System.EventHandler(this.tolShift_Click);
            // 
            // toolBar
            // 
            this.toolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolSave,
            this.tolExport,
            this.tolMapSettings,
            this.tolOpenScript,
            this.ToolStripSeparator4,
            this.tolUndo,
            this.tolRedo,
            this.ToolStripSeparator1,
            this.tolModePencil,
            this.tolModeRect,
            this.tolModeCircle,
            this.tolModeFill,
            this.ToolStripSeparator5,
            this.toolStripLabel4,
            this.tolScale,
            this.ToolStripSeparator3,
            this.tolShift,
            this.tolGrid,
            this.tolGenerateMap,
            this.toolStripSeparator6,
            this.tolMapTest});
            this.toolBar.Location = new System.Drawing.Point(0, 0);
            this.toolBar.Name = "toolBar";
            this.toolBar.Size = new System.Drawing.Size(849, 25);
            this.toolBar.TabIndex = 4;
            this.toolBar.Text = "ToolStrip1";
            // 
            // tolSave
            // 
            this.tolSave.Image = ((System.Drawing.Image)(resources.GetObject("tolSave.Image")));
            this.tolSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolSave.Name = "tolSave";
            this.tolSave.Size = new System.Drawing.Size(51, 22);
            this.tolSave.Text = "保存";
            this.tolSave.Click += new System.EventHandler(this.tolSave_Click);
            // 
            // tolExport
            // 
            this.tolExport.Image = ((System.Drawing.Image)(resources.GetObject("tolExport.Image")));
            this.tolExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolExport.Name = "tolExport";
            this.tolExport.Size = new System.Drawing.Size(115, 22);
            this.tolExport.Text = "画像でエクスポート";
            this.tolExport.Click += new System.EventHandler(this.tolExport_Click);
            // 
            // tolMapSettings
            // 
            this.tolMapSettings.Image = ((System.Drawing.Image)(resources.GetObject("tolMapSettings.Image")));
            this.tolMapSettings.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapSettings.Name = "tolMapSettings";
            this.tolMapSettings.Size = new System.Drawing.Size(76, 22);
            this.tolMapSettings.Text = "マップ設定";
            this.tolMapSettings.Click += new System.EventHandler(this.tolMapSettings_Click);
            // 
            // tolOpenScript
            // 
            this.tolOpenScript.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolOpenScript.Image = ((System.Drawing.Image)(resources.GetObject("tolOpenScript.Image")));
            this.tolOpenScript.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolOpenScript.Name = "tolOpenScript";
            this.tolOpenScript.Size = new System.Drawing.Size(23, 22);
            this.tolOpenScript.Text = "イベントスクリプトを開く";
            this.tolOpenScript.ToolTipText = "イベントスクリプトを編集";
            this.tolOpenScript.Click += new System.EventHandler(this.tolOpenScript_Click);
            // 
            // ToolStripSeparator4
            // 
            this.ToolStripSeparator4.Name = "ToolStripSeparator4";
            this.ToolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // tolUndo
            // 
            this.tolUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolUndo.Enabled = false;
            this.tolUndo.Image = ((System.Drawing.Image)(resources.GetObject("tolUndo.Image")));
            this.tolUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolUndo.Name = "tolUndo";
            this.tolUndo.Size = new System.Drawing.Size(23, 22);
            this.tolUndo.Text = "元に戻す";
            this.tolUndo.Click += new System.EventHandler(this.tolUndo_Click);
            // 
            // tolRedo
            // 
            this.tolRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolRedo.Enabled = false;
            this.tolRedo.Image = ((System.Drawing.Image)(resources.GetObject("tolRedo.Image")));
            this.tolRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolRedo.Name = "tolRedo";
            this.tolRedo.Size = new System.Drawing.Size(23, 22);
            this.tolRedo.Text = "やり直し";
            this.tolRedo.Click += new System.EventHandler(this.tolRedo_Click);
            // 
            // ToolStripSeparator1
            // 
            this.ToolStripSeparator1.Name = "ToolStripSeparator1";
            this.ToolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // tolModePencil
            // 
            this.tolModePencil.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolModePencil.Image = ((System.Drawing.Image)(resources.GetObject("tolModePencil.Image")));
            this.tolModePencil.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolModePencil.Name = "tolModePencil";
            this.tolModePencil.Size = new System.Drawing.Size(23, 22);
            this.tolModePencil.Text = "鉛筆ツール";
            this.tolModePencil.Click += new System.EventHandler(this.tolModePencil_Click);
            // 
            // tolModeRect
            // 
            this.tolModeRect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolModeRect.Image = ((System.Drawing.Image)(resources.GetObject("tolModeRect.Image")));
            this.tolModeRect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolModeRect.Name = "tolModeRect";
            this.tolModeRect.Size = new System.Drawing.Size(23, 22);
            this.tolModeRect.Text = "矩形ツール";
            this.tolModeRect.Click += new System.EventHandler(this.tolModeRect_Click);
            // 
            // tolModeCircle
            // 
            this.tolModeCircle.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolModeCircle.Image = ((System.Drawing.Image)(resources.GetObject("tolModeCircle.Image")));
            this.tolModeCircle.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolModeCircle.Name = "tolModeCircle";
            this.tolModeCircle.Size = new System.Drawing.Size(23, 22);
            this.tolModeCircle.Text = "楕円ツール";
            this.tolModeCircle.Click += new System.EventHandler(this.tolModeCircle_Click);
            // 
            // tolModeFill
            // 
            this.tolModeFill.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolModeFill.Image = ((System.Drawing.Image)(resources.GetObject("tolModeFill.Image")));
            this.tolModeFill.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolModeFill.Name = "tolModeFill";
            this.tolModeFill.Size = new System.Drawing.Size(23, 22);
            this.tolModeFill.Text = "塗りつぶしツール";
            this.tolModeFill.Click += new System.EventHandler(this.tolModeFill_Click);
            // 
            // ToolStripSeparator5
            // 
            this.ToolStripSeparator5.Name = "ToolStripSeparator5";
            this.ToolStripSeparator5.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel4
            // 
            this.toolStripLabel4.Name = "toolStripLabel4";
            this.toolStripLabel4.Size = new System.Drawing.Size(50, 22);
            this.toolStripLabel4.Text = "スケール:";
            // 
            // tolScale
            // 
            this.tolScale.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tolScale.FlatStyle = System.Windows.Forms.FlatStyle.Standard;
            this.tolScale.Name = "tolScale";
            this.tolScale.Size = new System.Drawing.Size(80, 25);
            this.tolScale.SelectedIndexChanged += new System.EventHandler(this.tolScale_SelectedIndexChanged);
            // 
            // ToolStripSeparator3
            // 
            this.ToolStripSeparator3.Name = "ToolStripSeparator3";
            this.ToolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // tolGenerateMap
            // 
            this.tolGenerateMap.Image = ((System.Drawing.Image)(resources.GetObject("tolGenerateMap.Image")));
            this.tolGenerateMap.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolGenerateMap.Name = "tolGenerateMap";
            this.tolGenerateMap.Size = new System.Drawing.Size(75, 22);
            this.tolGenerateMap.Text = "自動生成";
            this.tolGenerateMap.ToolTipText = "ダンジョン自動生成";
            this.tolGenerateMap.Click += new System.EventHandler(this.tolGenerateMap_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 25);
            // 
            // tolMapTest
            // 
            this.tolMapTest.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolMapTestWithSettings});
            this.tolMapTest.Image = ((System.Drawing.Image)(resources.GetObject("tolMapTest.Image")));
            this.tolMapTest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapTest.Name = "tolMapTest";
            this.tolMapTest.Size = new System.Drawing.Size(90, 20);
            this.tolMapTest.Text = "マップテスト";
            this.tolMapTest.ButtonClick += new System.EventHandler(this.tolMapTest_Click);
            // 
            // tolMapTestWithSettings
            // 
            this.tolMapTestWithSettings.Image = ((System.Drawing.Image)(resources.GetObject("tolMapTestWithSettings.Image")));
            this.tolMapTestWithSettings.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapTestWithSettings.Name = "tolMapTestWithSettings";
            this.tolMapTestWithSettings.Size = new System.Drawing.Size(200, 22);
            this.tolMapTestWithSettings.Text = "設定を適用してマップテスト";
            this.tolMapTestWithSettings.ToolTipText = "各種フラグ・変数等をセットした状態でマップテストを開始します。";
            this.tolMapTestWithSettings.Click += new System.EventHandler(this.tolMapTestWithSettings_Click);
            // 
            // pnlPreviewScroller
            // 
            this.pnlPreviewScroller.BackColor = System.Drawing.Color.White;
            this.pnlPreviewScroller.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pnlPreviewScroller.Controls.Add(this.tblMapDesigner);
            this.pnlPreviewScroller.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlPreviewScroller.Location = new System.Drawing.Point(0, 25);
            this.pnlPreviewScroller.Name = "pnlPreviewScroller";
            this.pnlPreviewScroller.Size = new System.Drawing.Size(849, 466);
            this.pnlPreviewScroller.TabIndex = 8;
            // 
            // tblMapDesigner
            // 
            this.tblMapDesigner.BackColor = System.Drawing.SystemColors.Control;
            this.tblMapDesigner.ColumnCount = 2;
            this.tblMapDesigner.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblMapDesigner.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 17F));
            this.tblMapDesigner.Controls.Add(this.hscDesignerScroller, 0, 1);
            this.tblMapDesigner.Controls.Add(this.vscDesignerScroller, 1, 0);
            this.tblMapDesigner.Controls.Add(this.pnlPreviewContainer, 0, 0);
            this.tblMapDesigner.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tblMapDesigner.Location = new System.Drawing.Point(0, 0);
            this.tblMapDesigner.Margin = new System.Windows.Forms.Padding(0);
            this.tblMapDesigner.Name = "tblMapDesigner";
            this.tblMapDesigner.RowCount = 2;
            this.tblMapDesigner.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblMapDesigner.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 17F));
            this.tblMapDesigner.Size = new System.Drawing.Size(845, 462);
            this.tblMapDesigner.TabIndex = 4;
            this.tblMapDesigner.Resize += new System.EventHandler(this.tblMapDesigner_Resize);
            // 
            // hscDesignerScroller
            // 
            this.hscDesignerScroller.Dock = System.Windows.Forms.DockStyle.Fill;
            this.hscDesignerScroller.LargeChange = 1;
            this.hscDesignerScroller.Location = new System.Drawing.Point(0, 445);
            this.hscDesignerScroller.Maximum = 0;
            this.hscDesignerScroller.Name = "hscDesignerScroller";
            this.hscDesignerScroller.Size = new System.Drawing.Size(828, 17);
            this.hscDesignerScroller.TabIndex = 2;
            this.hscDesignerScroller.Scroll += new System.Windows.Forms.ScrollEventHandler(this.designerScroller_Scroll);
            // 
            // vscDesignerScroller
            // 
            this.vscDesignerScroller.Dock = System.Windows.Forms.DockStyle.Fill;
            this.vscDesignerScroller.LargeChange = 1;
            this.vscDesignerScroller.Location = new System.Drawing.Point(828, 0);
            this.vscDesignerScroller.Maximum = 0;
            this.vscDesignerScroller.Name = "vscDesignerScroller";
            this.vscDesignerScroller.Padding = new System.Windows.Forms.Padding(0, 0, 0, 17);
            this.vscDesignerScroller.Size = new System.Drawing.Size(17, 445);
            this.vscDesignerScroller.TabIndex = 3;
            this.vscDesignerScroller.Scroll += new System.Windows.Forms.ScrollEventHandler(this.designerScroller_Scroll);
            // 
            // pnlPreviewContainer
            // 
            this.pnlPreviewContainer.Controls.Add(this.pnlPreview);
            this.pnlPreviewContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlPreviewContainer.Location = new System.Drawing.Point(0, 0);
            this.pnlPreviewContainer.Margin = new System.Windows.Forms.Padding(0);
            this.pnlPreviewContainer.Name = "pnlPreviewContainer";
            this.pnlPreviewContainer.Size = new System.Drawing.Size(828, 445);
            this.pnlPreviewContainer.TabIndex = 4;
            // 
            // pnlPreview
            // 
            this.pnlPreview.BackColor = System.Drawing.Color.Black;
            this.pnlPreview.Location = new System.Drawing.Point(0, 0);
            this.pnlPreview.Margin = new System.Windows.Forms.Padding(0);
            this.pnlPreview.Name = "pnlPreview";
            this.pnlPreview.Size = new System.Drawing.Size(3840, 2160);
            this.pnlPreview.TabIndex = 2;
            this.pnlPreview.TabStop = true;
            this.pnlPreview.VisibleChanged += new System.EventHandler(this.pnlPreview_VisibleChanged);
            this.pnlPreview.DoubleClick += new System.EventHandler(this.pnlPreview_DoubleClick);
            this.pnlPreview.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pnlPreview_MouseDown);
            this.pnlPreview.MouseEnter += new System.EventHandler(this.pnlPreview_MouseEnter);
            this.pnlPreview.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pnlPreview_MouseMove);
            this.pnlPreview.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pnlPreview_MouseUp);
            // 
            // splMap1
            // 
            this.splMap1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splMap1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splMap1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splMap1.Location = new System.Drawing.Point(0, 0);
            this.splMap1.Name = "splMap1";
            // 
            // splMap1.Panel1
            // 
            this.splMap1.Panel1.Controls.Add(this.splMap2);
            // 
            // splMap1.Panel2
            // 
            this.splMap1.Panel2.Controls.Add(this.pnlEditor);
            this.splMap1.Size = new System.Drawing.Size(1086, 495);
            this.splMap1.SplitterDistance = 230;
            this.splMap1.SplitterIncrement = 2;
            this.splMap1.SplitterWidth = 3;
            this.splMap1.TabIndex = 9;
            // 
            // splMap2
            // 
            this.splMap2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splMap2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splMap2.Location = new System.Drawing.Point(0, 0);
            this.splMap2.Margin = new System.Windows.Forms.Padding(0);
            this.splMap2.Name = "splMap2";
            this.splMap2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splMap2.Panel1
            // 
            this.splMap2.Panel1.Controls.Add(this.panel6);
            // 
            // splMap2.Panel2
            // 
            this.splMap2.Panel2.Controls.Add(this.pnlPallet);
            this.splMap2.Panel2.Controls.Add(this.lstMapLayerList);
            this.splMap2.Size = new System.Drawing.Size(230, 495);
            this.splMap2.SplitterDistance = 203;
            this.splMap2.SplitterIncrement = 2;
            this.splMap2.SplitterWidth = 3;
            this.splMap2.TabIndex = 2;
            // 
            // panel6
            // 
            this.panel6.Controls.Add(this.uctlMapTree);
            this.panel6.Controls.Add(this.tosMapFileToolBar);
            this.panel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel6.Location = new System.Drawing.Point(0, 0);
            this.panel6.Margin = new System.Windows.Forms.Padding(0);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(226, 199);
            this.panel6.TabIndex = 0;
            // 
            // uctlMapTree
            // 
            this.uctlMapTree.AutoReload = true;
            this.uctlMapTree.CanDelete = true;
            this.uctlMapTree.CanRename = true;
            this.uctlMapTree.DefaultExnpandDepth = 2;
            this.uctlMapTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.uctlMapTree.FileFilter = "";
            this.uctlMapTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlMapTree.IncludeFiles = true;
            this.uctlMapTree.Location = new System.Drawing.Point(0, 25);
            this.uctlMapTree.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlMapTree.Name = "uctlMapTree";
            this.uctlMapTree.RootPath = "";
            this.uctlMapTree.Size = new System.Drawing.Size(226, 174);
            this.uctlMapTree.TabIndex = 3;
            this.uctlMapTree.VisibleCheckBoxes = false;
            this.uctlMapTree.CreatedNode += new Editor.CtrlComponent.Common.FileTreeNodeEventHandler(this.uctlMapTree_CreatedNode);
            this.uctlMapTree.RenamedNode += new Editor.CtrlComponent.Common.FileTreeNodeEventHandler(this.uctlMapTree_RenamedNode);
            this.uctlMapTree.DeletedNode += new Editor.CtrlComponent.Common.FileTreeNodeEventHandler(this.uctlMapTree_DeletedNode);
            this.uctlMapTree.BeforeSelectNode += new System.Windows.Forms.TreeViewCancelEventHandler(this.uctlMapTree_BeforeSelectNode);
            this.uctlMapTree.AfterSelectNode += new System.Windows.Forms.TreeViewEventHandler(this.uctlMapTree_AfterSelectNode);
            // 
            // tosMapFileToolBar
            // 
            this.tosMapFileToolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolMapNewFile,
            this.tolMapNewFolder,
            this.toolStripSeparator7,
            this.tolMapCopy,
            this.tolMapPaste,
            this.tolMapDelete,
            this.toolStripSeparator8,
            this.tolMapReloadFiles,
            this.tolOpenFolder});
            this.tosMapFileToolBar.Location = new System.Drawing.Point(0, 0);
            this.tosMapFileToolBar.Name = "tosMapFileToolBar";
            this.tosMapFileToolBar.Size = new System.Drawing.Size(226, 25);
            this.tosMapFileToolBar.TabIndex = 2;
            this.tosMapFileToolBar.Text = "ToolStrip2";
            // 
            // tolMapNewFile
            // 
            this.tolMapNewFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolMapNewFile.Enabled = false;
            this.tolMapNewFile.Image = ((System.Drawing.Image)(resources.GetObject("tolMapNewFile.Image")));
            this.tolMapNewFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapNewFile.Name = "tolMapNewFile";
            this.tolMapNewFile.Size = new System.Drawing.Size(23, 22);
            this.tolMapNewFile.Text = "新しいマップ";
            this.tolMapNewFile.Click += new System.EventHandler(this.tolMapNewFile_Click);
            // 
            // tolMapNewFolder
            // 
            this.tolMapNewFolder.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolMapNewFolder.Enabled = false;
            this.tolMapNewFolder.Image = ((System.Drawing.Image)(resources.GetObject("tolMapNewFolder.Image")));
            this.tolMapNewFolder.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapNewFolder.Name = "tolMapNewFolder";
            this.tolMapNewFolder.Size = new System.Drawing.Size(23, 22);
            this.tolMapNewFolder.Text = "新しいフォルダー";
            this.tolMapNewFolder.Click += new System.EventHandler(this.tolMapNewFolder_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 25);
            // 
            // tolMapCopy
            // 
            this.tolMapCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolMapCopy.Enabled = false;
            this.tolMapCopy.Image = ((System.Drawing.Image)(resources.GetObject("tolMapCopy.Image")));
            this.tolMapCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapCopy.Name = "tolMapCopy";
            this.tolMapCopy.Size = new System.Drawing.Size(23, 22);
            this.tolMapCopy.Text = "コピー";
            this.tolMapCopy.ToolTipText = "マップをコピー";
            this.tolMapCopy.Click += new System.EventHandler(this.tolMapCopy_Click);
            // 
            // tolMapPaste
            // 
            this.tolMapPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolMapPaste.Enabled = false;
            this.tolMapPaste.Image = ((System.Drawing.Image)(resources.GetObject("tolMapPaste.Image")));
            this.tolMapPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapPaste.Name = "tolMapPaste";
            this.tolMapPaste.Size = new System.Drawing.Size(23, 22);
            this.tolMapPaste.Text = "貼り付け";
            this.tolMapPaste.ToolTipText = "マップを貼り付け";
            this.tolMapPaste.Click += new System.EventHandler(this.tolMapPaste_Click);
            // 
            // tolMapDelete
            // 
            this.tolMapDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolMapDelete.Enabled = false;
            this.tolMapDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolMapDelete.Image")));
            this.tolMapDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapDelete.Name = "tolMapDelete";
            this.tolMapDelete.Size = new System.Drawing.Size(23, 22);
            this.tolMapDelete.Text = "削除";
            this.tolMapDelete.ToolTipText = "マップを削除";
            this.tolMapDelete.Click += new System.EventHandler(this.tolMapDelete_Click);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 25);
            // 
            // tolMapReloadFiles
            // 
            this.tolMapReloadFiles.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolMapReloadFiles.Image = ((System.Drawing.Image)(resources.GetObject("tolMapReloadFiles.Image")));
            this.tolMapReloadFiles.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolMapReloadFiles.Name = "tolMapReloadFiles";
            this.tolMapReloadFiles.Size = new System.Drawing.Size(23, 22);
            this.tolMapReloadFiles.Text = "最新の情報に更新";
            this.tolMapReloadFiles.Click += new System.EventHandler(this.tolMapReloadFiles_Click);
            // 
            // tolOpenFolder
            // 
            this.tolOpenFolder.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolOpenFolder.Enabled = false;
            this.tolOpenFolder.Image = ((System.Drawing.Image)(resources.GetObject("tolOpenFolder.Image")));
            this.tolOpenFolder.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolOpenFolder.Name = "tolOpenFolder";
            this.tolOpenFolder.Size = new System.Drawing.Size(23, 22);
            this.tolOpenFolder.Text = "エクスプローラーでフォルダーを開く";
            this.tolOpenFolder.Click += new System.EventHandler(this.tolOpenFolder_Click);
            // 
            // pnlPallet
            // 
            this.pnlPallet.Controls.Add(this.ltvEVList);
            this.pnlPallet.Controls.Add(this.uctlTilePallet);
            this.pnlPallet.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlPallet.Location = new System.Drawing.Point(0, 94);
            this.pnlPallet.Margin = new System.Windows.Forms.Padding(0);
            this.pnlPallet.Name = "pnlPallet";
            this.pnlPallet.Size = new System.Drawing.Size(226, 191);
            this.pnlPallet.TabIndex = 1;
            // 
            // ltvEVList
            // 
            this.ltvEVList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.ColumnHeader9,
            this.ColumnHeader16});
            this.ltvEVList.ContextMenuStrip = this.EVRightMenu;
            this.ltvEVList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ltvEVList.FullRowSelect = true;
            this.ltvEVList.GridLines = true;
            this.ltvEVList.HideSelection = false;
            this.ltvEVList.Location = new System.Drawing.Point(0, 0);
            this.ltvEVList.MultiSelect = false;
            this.ltvEVList.Name = "ltvEVList";
            this.ltvEVList.ShowItemToolTips = true;
            this.ltvEVList.Size = new System.Drawing.Size(226, 191);
            this.ltvEVList.TabIndex = 2;
            this.ltvEVList.UseCompatibleStateImageBehavior = false;
            this.ltvEVList.View = System.Windows.Forms.View.Details;
            this.ltvEVList.Visible = false;
            this.ltvEVList.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.ltvEVList_ColumnClick);
            this.ltvEVList.SelectedIndexChanged += new System.EventHandler(this.ltvEVList_SelectedIndexChanged);
            this.ltvEVList.DoubleClick += new System.EventHandler(this.ctmEditEV_Click);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "FixedID";
            this.columnHeader1.Width = 0;
            // 
            // ColumnHeader9
            // 
            this.ColumnHeader9.Text = "ID";
            this.ColumnHeader9.Width = 50;
            // 
            // ColumnHeader16
            // 
            this.ColumnHeader16.Text = "イベント名";
            this.ColumnHeader16.Width = 140;
            // 
            // uctlTilePallet
            // 
            this.uctlTilePallet.AutoScroll = true;
            this.uctlTilePallet.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.uctlTilePallet.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.uctlTilePallet.Dock = System.Windows.Forms.DockStyle.Fill;
            this.uctlTilePallet.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlTilePallet.Location = new System.Drawing.Point(0, 0);
            this.uctlTilePallet.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlTilePallet.Name = "uctlTilePallet";
            this.uctlTilePallet.SelectedRange = new System.Drawing.Rectangle(0, 0, 1, 1);
            this.uctlTilePallet.Size = new System.Drawing.Size(226, 191);
            this.uctlTilePallet.TabIndex = 3;
            this.uctlTilePallet.Visible = false;
            this.uctlTilePallet.SelectedPalletTile += new System.EventHandler(this.uctlTilePallet_SelectedPalletTile);
            // 
            // lstMapLayerList
            // 
            this.lstMapLayerList.Dock = System.Windows.Forms.DockStyle.Top;
            this.lstMapLayerList.FormattingEnabled = true;
            this.lstMapLayerList.Location = new System.Drawing.Point(0, 0);
            this.lstMapLayerList.Name = "lstMapLayerList";
            this.lstMapLayerList.Size = new System.Drawing.Size(226, 94);
            this.lstMapLayerList.TabIndex = 0;
            this.lstMapLayerList.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lstMapLayerList_ItemCheck);
            this.lstMapLayerList.SelectedIndexChanged += new System.EventHandler(this.lstMapLayerList_SelectedIndexChanged);
            // 
            // pnlEditor
            // 
            this.pnlEditor.Controls.Add(this.pnlPreviewScroller);
            this.pnlEditor.Controls.Add(this.toolBar);
            this.pnlEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlEditor.Location = new System.Drawing.Point(0, 0);
            this.pnlEditor.Margin = new System.Windows.Forms.Padding(0);
            this.pnlEditor.Name = "pnlEditor";
            this.pnlEditor.Size = new System.Drawing.Size(849, 491);
            this.pnlEditor.TabIndex = 9;
            this.pnlEditor.Visible = false;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ショートカットキーToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(948, 24);
            this.menuStrip1.TabIndex = 4;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.Visible = false;
            // 
            // ショートカットキーToolStripMenuItem
            // 
            this.ショートカットキーToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuMapSave,
            this.mnuUndo,
            this.mnuRedo,
            this.mnuGrid,
            this.mnuMapTest});
            this.ショートカットキーToolStripMenuItem.Name = "ショートカットキーToolStripMenuItem";
            this.ショートカットキーToolStripMenuItem.Size = new System.Drawing.Size(97, 20);
            this.ショートカットキーToolStripMenuItem.Text = "ショートカットキー";
            // 
            // mnuMapSave
            // 
            this.mnuMapSave.Image = ((System.Drawing.Image)(resources.GetObject("mnuMapSave.Image")));
            this.mnuMapSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.mnuMapSave.Name = "mnuMapSave";
            this.mnuMapSave.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.mnuMapSave.Size = new System.Drawing.Size(163, 22);
            this.mnuMapSave.Text = "保存";
            this.mnuMapSave.Click += new System.EventHandler(this.tolSave_Click);
            // 
            // mnuUndo
            // 
            this.mnuUndo.Image = ((System.Drawing.Image)(resources.GetObject("mnuUndo.Image")));
            this.mnuUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.mnuUndo.Name = "mnuUndo";
            this.mnuUndo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.mnuUndo.Size = new System.Drawing.Size(163, 22);
            this.mnuUndo.Text = "元に戻す";
            this.mnuUndo.Click += new System.EventHandler(this.tolUndo_Click);
            // 
            // mnuRedo
            // 
            this.mnuRedo.Image = ((System.Drawing.Image)(resources.GetObject("mnuRedo.Image")));
            this.mnuRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.mnuRedo.Name = "mnuRedo";
            this.mnuRedo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.mnuRedo.Size = new System.Drawing.Size(163, 22);
            this.mnuRedo.Text = "やり直し";
            this.mnuRedo.Click += new System.EventHandler(this.tolRedo_Click);
            // 
            // mnuGrid
            // 
            this.mnuGrid.CheckOnClick = true;
            this.mnuGrid.Image = ((System.Drawing.Image)(resources.GetObject("mnuGrid.Image")));
            this.mnuGrid.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.mnuGrid.Name = "mnuGrid";
            this.mnuGrid.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.G)));
            this.mnuGrid.Size = new System.Drawing.Size(163, 22);
            this.mnuGrid.Text = "グリッド";
            this.mnuGrid.Click += new System.EventHandler(this.mnuGrid_Click);
            // 
            // mnuMapTest
            // 
            this.mnuMapTest.Image = ((System.Drawing.Image)(resources.GetObject("mnuMapTest.Image")));
            this.mnuMapTest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.mnuMapTest.Name = "mnuMapTest";
            this.mnuMapTest.ShortcutKeys = System.Windows.Forms.Keys.F10;
            this.mnuMapTest.Size = new System.Drawing.Size(163, 22);
            this.mnuMapTest.Text = "マップテスト";
            this.mnuMapTest.Click += new System.EventHandler(this.tolMapTest_Click);
            // 
            // dlgSave
            // 
            this.dlgSave.DefaultExt = "jpg";
            this.dlgSave.Filter = "JPG ファイル|*.jpg";
            this.dlgSave.Title = "画像としてエクスポート";
            // 
            // ctlMapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.splMap1);
            this.Controls.Add(this.statusBar);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "ctlMapEditor";
            this.Size = new System.Drawing.Size(1086, 519);
            this.Load += new System.EventHandler(this.ctlMapEditor_Load);
            this.VisibleChanged += new System.EventHandler(this.ctlMapEditor_VisibleChanged);
            this.statusBar.ResumeLayout(false);
            this.statusBar.PerformLayout();
            this.EVRightMenu.ResumeLayout(false);
            this.toolBar.ResumeLayout(false);
            this.toolBar.PerformLayout();
            this.pnlPreviewScroller.ResumeLayout(false);
            this.tblMapDesigner.ResumeLayout(false);
            this.pnlPreviewContainer.ResumeLayout(false);
            this.splMap1.Panel1.ResumeLayout(false);
            this.splMap1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splMap1)).EndInit();
            this.splMap1.ResumeLayout(false);
            this.splMap2.Panel1.ResumeLayout(false);
            this.splMap2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splMap2)).EndInit();
            this.splMap2.ResumeLayout(false);
            this.panel6.ResumeLayout(false);
            this.panel6.PerformLayout();
            this.tosMapFileToolBar.ResumeLayout(false);
            this.tosMapFileToolBar.PerformLayout();
            this.pnlPallet.ResumeLayout(false);
            this.pnlEditor.ResumeLayout(false);
            this.pnlEditor.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStripButton tolGrid;
		private System.Windows.Forms.StatusStrip statusBar;
		private System.Windows.Forms.ToolStripStatusLabel lblMapCursor;
		private System.Windows.Forms.ToolStripStatusLabel lblMapRange;
		private System.Windows.Forms.ToolStripStatusLabel lblMapSize;
		private System.Windows.Forms.ContextMenuStrip EVRightMenu;
		private System.Windows.Forms.ToolStripMenuItem ctmEditEV;
		private System.Windows.Forms.ToolStripMenuItem ctmAddEV;
		private System.Windows.Forms.ToolStripMenuItem ctmSetStartPos;
		private System.Windows.Forms.ToolStripMenuItem ctmSetStartPosAndPlay;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator10;
		private System.Windows.Forms.ToolStripMenuItem ctmCutEV;
		private System.Windows.Forms.ToolStripMenuItem ctmCopyEV;
		private System.Windows.Forms.ToolStripMenuItem ctmPasteEV;
		private System.Windows.Forms.ToolStripMenuItem ctmDeleteEV;
		private System.Windows.Forms.ToolStripButton tolShift;
		private System.Windows.Forms.ToolStrip toolBar;
		private System.Windows.Forms.ToolStripButton tolSave;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator4;
		private System.Windows.Forms.ToolStripButton tolUndo;
		private System.Windows.Forms.ToolStripButton tolRedo;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tolModePencil;
		private System.Windows.Forms.ToolStripButton tolModeRect;
		private System.Windows.Forms.ToolStripButton tolModeCircle;
		private System.Windows.Forms.ToolStripButton tolModeFill;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator5;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator3;
		private System.Windows.Forms.ToolStripButton tolMapSettings;
		private System.Windows.Forms.Panel pnlPreviewScroller;
		public System.Windows.Forms.SplitContainer splMap1;
		private System.Windows.Forms.Panel panel6;
		public System.Windows.Forms.ToolStrip tosMapFileToolBar;
		public System.Windows.Forms.ToolStripButton tolMapNewFile;
		public System.Windows.Forms.ToolStripButton tolMapNewFolder;
		public System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
		public System.Windows.Forms.ToolStripButton tolMapCopy;
		public System.Windows.Forms.ToolStripButton tolMapPaste;
		public System.Windows.Forms.ToolStripButton tolMapDelete;
		public System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
		public System.Windows.Forms.ToolStripButton tolMapReloadFiles;
		public System.Windows.Forms.CheckedListBox lstMapLayerList;
		private System.Windows.Forms.ToolStripLabel toolStripLabel4;
		private System.Windows.Forms.ToolStripComboBox tolScale;
		private System.Windows.Forms.Panel pnlPallet;
		private ctlTilePallet uctlTilePallet;
		public System.Windows.Forms.ListView ltvEVList;
		public System.Windows.Forms.ColumnHeader ColumnHeader9;
		public System.Windows.Forms.ColumnHeader ColumnHeader16;
		public Common.ctlFileTree uctlMapTree;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.Panel pnlEditor;
		private System.Windows.Forms.SplitContainer splMap2;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
		private System.Windows.Forms.TableLayoutPanel tblMapDesigner;
		private System.Windows.Forms.HScrollBar hscDesignerScroller;
		private System.Windows.Forms.VScrollBar vscDesignerScroller;
		private System.Windows.Forms.Panel pnlPreviewContainer;
		private System.Windows.Forms.Panel pnlPreview;
		private System.Windows.Forms.ToolStripStatusLabel lblMapPalletCursor;
		private System.Windows.Forms.ToolStripStatusLabel lblModifierKey;
		private System.Windows.Forms.ToolStripButton tolOpenScript;
		private System.Windows.Forms.ToolStripStatusLabel lblMapName;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem ショートカットキーToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem mnuMapSave;
		private System.Windows.Forms.ToolStripMenuItem mnuUndo;
		private System.Windows.Forms.ToolStripMenuItem mnuRedo;
		private System.Windows.Forms.ToolStripMenuItem mnuGrid;
		private System.Windows.Forms.ToolStripMenuItem mnuMapTest;
		private System.Windows.Forms.ToolStripButton tolGenerateMap;
		private System.Windows.Forms.ToolStripButton tolOpenFolder;
		private System.Windows.Forms.ToolStripButton tolExport;
		private System.Windows.Forms.SaveFileDialog dlgSave;
		private System.Windows.Forms.ToolStripStatusLabel lblMapFixedID;
        private System.Windows.Forms.ToolStripStatusLabel lblDummyForSpring;
        private System.Windows.Forms.ToolStripSplitButton tolMapTest;
        private System.Windows.Forms.ToolStripMenuItem tolMapTestWithSettings;
    }
}
