namespace Editor {
	partial class MainWindow {
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

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            this.imlMainTabIcons = new System.Windows.Forms.ImageList(this.components);
            this.mnuManual = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuVersion = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolGraphicTrans = new System.Windows.Forms.ToolStripMenuItem();
            this.mspMainMenu = new System.Windows.Forms.MenuStrip();
            this.mnuProject = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectNew = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectClose = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectSettingsReload = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectSplit1 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuProjectFolderOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuProjectSplit2 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuPublish = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuTestPlay = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuGarbageCleaner = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuView = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuView2Pane = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewPainToRight = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuViewPainToLeft = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuTool = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolGraphicExpand = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolRPGTkoolConvert = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolAutotileConvert = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolWalkConvert = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuToolSoundLoop = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.folderDlg = new System.Windows.Forms.FolderBrowserDialog();
            this.openFileDlg = new System.Windows.Forms.OpenFileDialog();
            this.miniToolStrip = new System.Windows.Forms.ToolStrip();
            this.pnlDefaultDrawTarget = new System.Windows.Forms.Panel();
            this.splMain1 = new System.Windows.Forms.SplitContainer();
            this.tbcLeftPaneTabs = new System.Windows.Forms.TabControl();
            this.tbpSchedule = new System.Windows.Forms.TabPage();
            this.uctlSchedule = new Editor.CtrlComponent.Schedule.ctlScheduleEditor();
            this.tbpText = new System.Windows.Forms.TabPage();
            this.uctlTextEditor = new Editor.CtrlComponent.Text.ctlTextEditor();
            this.tbpDB = new System.Windows.Forms.TabPage();
            this.uctlDBEditor = new Editor.CtrlComponent.Database.ctlDatabaseEditor();
            this.tbpMap = new System.Windows.Forms.TabPage();
            this.uctlMapEditor = new Editor.CtrlComponent.Map.ctlMapEditor();
            this.tbcRightPaneTabs = new System.Windows.Forms.TabControl();
            this.mspMainMenu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splMain1)).BeginInit();
            this.splMain1.Panel1.SuspendLayout();
            this.splMain1.Panel2.SuspendLayout();
            this.splMain1.SuspendLayout();
            this.tbcLeftPaneTabs.SuspendLayout();
            this.tbpSchedule.SuspendLayout();
            this.tbpText.SuspendLayout();
            this.tbpDB.SuspendLayout();
            this.tbpMap.SuspendLayout();
            this.SuspendLayout();
            // 
            // imlMainTabIcons
            // 
            this.imlMainTabIcons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imlMainTabIcons.ImageStream")));
            this.imlMainTabIcons.TransparentColor = System.Drawing.Color.Transparent;
            this.imlMainTabIcons.Images.SetKeyName(0, "view-calendar-timeline.png");
            this.imlMainTabIcons.Images.SetKeyName(1, "accessories-text-editor-5.png");
            this.imlMainTabIcons.Images.SetKeyName(2, "db.png");
            this.imlMainTabIcons.Images.SetKeyName(3, "applications-graphics-2.png");
            // 
            // mnuManual
            // 
            this.mnuManual.Image = ((System.Drawing.Image)(resources.GetObject("mnuManual.Image")));
            this.mnuManual.Name = "mnuManual";
            this.mnuManual.Size = new System.Drawing.Size(173, 22);
            this.mnuManual.Text = "取扱説明書(&H)...";
            this.mnuManual.Click += new System.EventHandler(this.mnuManual_Click);
            // 
            // mnuVersion
            // 
            this.mnuVersion.Name = "mnuVersion";
            this.mnuVersion.Size = new System.Drawing.Size(173, 22);
            this.mnuVersion.Text = "バージョン情報(&V)...";
            this.mnuVersion.Click += new System.EventHandler(this.mnuVersion_Click);
            // 
            // mnuHelp
            // 
            this.mnuHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuManual,
            this.mnuVersion});
            this.mnuHelp.Name = "mnuHelp";
            this.mnuHelp.Size = new System.Drawing.Size(67, 19);
            this.mnuHelp.Text = "ヘルプ(&H)";
            // 
            // mnuToolGraphicTrans
            // 
            this.mnuToolGraphicTrans.Enabled = false;
            this.mnuToolGraphicTrans.Image = ((System.Drawing.Image)(resources.GetObject("mnuToolGraphicTrans.Image")));
            this.mnuToolGraphicTrans.Name = "mnuToolGraphicTrans";
            this.mnuToolGraphicTrans.Size = new System.Drawing.Size(211, 22);
            this.mnuToolGraphicTrans.Text = "画像を透過(&T)...";
            this.mnuToolGraphicTrans.Click += new System.EventHandler(this.mnuToolGraphicTrans_Click);
            // 
            // mspMainMenu
            // 
            this.mspMainMenu.BackColor = System.Drawing.SystemColors.Control;
            this.mspMainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuProject,
            this.mnuView,
            this.mnuTool,
            this.mnuHelp});
            this.mspMainMenu.Location = new System.Drawing.Point(0, 0);
            this.mspMainMenu.Name = "mspMainMenu";
            this.mspMainMenu.Padding = new System.Windows.Forms.Padding(7, 3, 0, 3);
            this.mspMainMenu.Size = new System.Drawing.Size(1268, 25);
            this.mspMainMenu.TabIndex = 0;
            this.mspMainMenu.Text = "MenuStrip1";
            // 
            // mnuProject
            // 
            this.mnuProject.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuProjectNew,
            this.mnuProjectOpen,
            this.mnuProjectSave,
            this.mnuProjectClose,
            this.mnuProjectSettingsReload,
            this.mnuProjectSplit1,
            this.mnuProjectFolderOpen,
            this.mnuProjectSettings,
            this.mnuProjectSplit2,
            this.mnuPublish,
            this.mnuTestPlay,
            this.mnuGarbageCleaner});
            this.mnuProject.Name = "mnuProject";
            this.mnuProject.Size = new System.Drawing.Size(87, 19);
            this.mnuProject.Text = "プロジェクト(&P)";
            // 
            // mnuProjectNew
            // 
            this.mnuProjectNew.Image = ((System.Drawing.Image)(resources.GetObject("mnuProjectNew.Image")));
            this.mnuProjectNew.Name = "mnuProjectNew";
            this.mnuProjectNew.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectNew.Text = "プロジェクトを新規作成(&N)...";
            this.mnuProjectNew.Click += new System.EventHandler(this.mnuProjectNew_Click);
            // 
            // mnuProjectOpen
            // 
            this.mnuProjectOpen.Image = ((System.Drawing.Image)(resources.GetObject("mnuProjectOpen.Image")));
            this.mnuProjectOpen.Name = "mnuProjectOpen";
            this.mnuProjectOpen.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectOpen.Text = "プロジェクトを開く(&O)...";
            this.mnuProjectOpen.Click += new System.EventHandler(this.mnuProjectOpen_Click);
            // 
            // mnuProjectSave
            // 
            this.mnuProjectSave.Image = ((System.Drawing.Image)(resources.GetObject("mnuProjectSave.Image")));
            this.mnuProjectSave.Name = "mnuProjectSave";
            this.mnuProjectSave.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectSave.Text = "プロジェクトを保存(&S)";
            this.mnuProjectSave.Visible = false;
            this.mnuProjectSave.Click += new System.EventHandler(this.mnuProjectSave_Click);
            // 
            // mnuProjectClose
            // 
            this.mnuProjectClose.Name = "mnuProjectClose";
            this.mnuProjectClose.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectClose.Text = "プロジェクトを閉じる(&C)";
            this.mnuProjectClose.Visible = false;
            this.mnuProjectClose.Click += new System.EventHandler(this.mnuProjectClose_Click);
            // 
            // mnuProjectSettingsReload
            // 
            this.mnuProjectSettingsReload.Name = "mnuProjectSettingsReload";
            this.mnuProjectSettingsReload.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectSettingsReload.Text = "プロジェクトの設定を再読み込み(&R)";
            this.mnuProjectSettingsReload.Visible = false;
            this.mnuProjectSettingsReload.Click += new System.EventHandler(this.mnuProjectSettingsReload_Click);
            // 
            // mnuProjectSplit1
            // 
            this.mnuProjectSplit1.Name = "mnuProjectSplit1";
            this.mnuProjectSplit1.Size = new System.Drawing.Size(241, 6);
            this.mnuProjectSplit1.Visible = false;
            // 
            // mnuProjectFolderOpen
            // 
            this.mnuProjectFolderOpen.Image = ((System.Drawing.Image)(resources.GetObject("mnuProjectFolderOpen.Image")));
            this.mnuProjectFolderOpen.Name = "mnuProjectFolderOpen";
            this.mnuProjectFolderOpen.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectFolderOpen.Text = "プロジェクトフォルダーを開く(&F)";
            this.mnuProjectFolderOpen.Visible = false;
            this.mnuProjectFolderOpen.Click += new System.EventHandler(this.mnuProjectFolderOpen_Click);
            // 
            // mnuProjectSettings
            // 
            this.mnuProjectSettings.Image = ((System.Drawing.Image)(resources.GetObject("mnuProjectSettings.Image")));
            this.mnuProjectSettings.Name = "mnuProjectSettings";
            this.mnuProjectSettings.Size = new System.Drawing.Size(244, 22);
            this.mnuProjectSettings.Text = "プロジェクトの設定(&O)...";
            this.mnuProjectSettings.Visible = false;
            this.mnuProjectSettings.Click += new System.EventHandler(this.mnuProjectSettings_Click);
            // 
            // mnuProjectSplit2
            // 
            this.mnuProjectSplit2.Name = "mnuProjectSplit2";
            this.mnuProjectSplit2.Size = new System.Drawing.Size(241, 6);
            this.mnuProjectSplit2.Visible = false;
            // 
            // mnuPublish
            // 
            this.mnuPublish.Image = ((System.Drawing.Image)(resources.GetObject("mnuPublish.Image")));
            this.mnuPublish.Name = "mnuPublish";
            this.mnuPublish.Size = new System.Drawing.Size(244, 22);
            this.mnuPublish.Text = "製品版として発行(&P)...";
            this.mnuPublish.Visible = false;
            this.mnuPublish.Click += new System.EventHandler(this.mnuPublish_Click);
            // 
            // mnuTestPlay
            // 
            this.mnuTestPlay.Image = ((System.Drawing.Image)(resources.GetObject("mnuTestPlay.Image")));
            this.mnuTestPlay.Name = "mnuTestPlay";
            this.mnuTestPlay.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.mnuTestPlay.Size = new System.Drawing.Size(244, 22);
            this.mnuTestPlay.Text = "タイトル画面からテストプレイ(&T)";
            this.mnuTestPlay.Visible = false;
            this.mnuTestPlay.Click += new System.EventHandler(this.mnuTestPlay_Click);
            // 
            // mnuGarbageCleaner
            // 
            this.mnuGarbageCleaner.Name = "mnuGarbageCleaner";
            this.mnuGarbageCleaner.Size = new System.Drawing.Size(244, 22);
            this.mnuGarbageCleaner.Text = "不要なファイルを除去(&G)";
            this.mnuGarbageCleaner.Visible = false;
            this.mnuGarbageCleaner.Click += new System.EventHandler(this.mnuGarbageCleaner_Click);
            // 
            // mnuView
            // 
            this.mnuView.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuView2Pane,
            this.mnuViewPainToRight,
            this.mnuViewPainToLeft});
            this.mnuView.Name = "mnuView";
            this.mnuView.Size = new System.Drawing.Size(61, 19);
            this.mnuView.Text = "表示(&V)";
            // 
            // mnuView2Pane
            // 
            this.mnuView2Pane.CheckOnClick = true;
            this.mnuView2Pane.Enabled = false;
            this.mnuView2Pane.Image = ((System.Drawing.Image)(resources.GetObject("mnuView2Pane.Image")));
            this.mnuView2Pane.Name = "mnuView2Pane";
            this.mnuView2Pane.Size = new System.Drawing.Size(226, 22);
            this.mnuView2Pane.Text = "２ペイン表示(&D)";
            this.mnuView2Pane.CheckedChanged += new System.EventHandler(this.mnuView2Pane_CheckedChanged);
            // 
            // mnuViewPainToRight
            // 
            this.mnuViewPainToRight.Enabled = false;
            this.mnuViewPainToRight.Image = ((System.Drawing.Image)(resources.GetObject("mnuViewPainToRight.Image")));
            this.mnuViewPainToRight.Name = "mnuViewPainToRight";
            this.mnuViewPainToRight.Size = new System.Drawing.Size(226, 22);
            this.mnuViewPainToRight.Text = "現在のタブを右ペインへ移動(&R)";
            this.mnuViewPainToRight.Click += new System.EventHandler(this.mnuViewPainToRight_Click);
            // 
            // mnuViewPainToLeft
            // 
            this.mnuViewPainToLeft.Enabled = false;
            this.mnuViewPainToLeft.Image = ((System.Drawing.Image)(resources.GetObject("mnuViewPainToLeft.Image")));
            this.mnuViewPainToLeft.Name = "mnuViewPainToLeft";
            this.mnuViewPainToLeft.Size = new System.Drawing.Size(226, 22);
            this.mnuViewPainToLeft.Text = "現在のタブを左ペインへ移動(&L)";
            this.mnuViewPainToLeft.Click += new System.EventHandler(this.mnuViewPainToLeft_Click);
            // 
            // mnuTool
            // 
            this.mnuTool.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuToolGraphicExpand,
            this.mnuToolGraphicTrans,
            this.mnuToolRPGTkoolConvert,
            this.mnuToolSoundLoop,
            this.toolStripSeparator2,
            this.mnuSettings});
            this.mnuTool.Name = "mnuTool";
            this.mnuTool.Size = new System.Drawing.Size(66, 19);
            this.mnuTool.Text = "ツール(&T)";
            // 
            // mnuToolGraphicExpand
            // 
            this.mnuToolGraphicExpand.Enabled = false;
            this.mnuToolGraphicExpand.Image = ((System.Drawing.Image)(resources.GetObject("mnuToolGraphicExpand.Image")));
            this.mnuToolGraphicExpand.Name = "mnuToolGraphicExpand";
            this.mnuToolGraphicExpand.Size = new System.Drawing.Size(211, 22);
            this.mnuToolGraphicExpand.Text = "画像をリサイズ(&R)...";
            this.mnuToolGraphicExpand.Click += new System.EventHandler(this.mnuToolGraphicExpand_Click);
            // 
            // mnuToolRPGTkoolConvert
            // 
            this.mnuToolRPGTkoolConvert.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuToolAutotileConvert,
            this.mnuToolWalkConvert});
            this.mnuToolRPGTkoolConvert.Enabled = false;
            this.mnuToolRPGTkoolConvert.Name = "mnuToolRPGTkoolConvert";
            this.mnuToolRPGTkoolConvert.Size = new System.Drawing.Size(211, 22);
            this.mnuToolRPGTkoolConvert.Text = "RPGツクール素材の変換(&K)";
            // 
            // mnuToolAutotileConvert
            // 
            this.mnuToolAutotileConvert.Name = "mnuToolAutotileConvert";
            this.mnuToolAutotileConvert.Size = new System.Drawing.Size(203, 22);
            this.mnuToolAutotileConvert.Text = "オートタイル素材(&A)...";
            this.mnuToolAutotileConvert.Click += new System.EventHandler(this.mnuToolAutotileConvert_Click);
            // 
            // mnuToolWalkConvert
            // 
            this.mnuToolWalkConvert.Name = "mnuToolWalkConvert";
            this.mnuToolWalkConvert.Size = new System.Drawing.Size(203, 22);
            this.mnuToolWalkConvert.Text = "歩行グラフィック素材(&W)...";
            this.mnuToolWalkConvert.Click += new System.EventHandler(this.mnuToolWalkConvert_Click);
            // 
            // mnuToolSoundLoop
            // 
            this.mnuToolSoundLoop.Enabled = false;
            this.mnuToolSoundLoop.Image = ((System.Drawing.Image)(resources.GetObject("mnuToolSoundLoop.Image")));
            this.mnuToolSoundLoop.Name = "mnuToolSoundLoop";
            this.mnuToolSoundLoop.Size = new System.Drawing.Size(211, 22);
            this.mnuToolSoundLoop.Text = "サウンドループ設定(&L)...";
            this.mnuToolSoundLoop.Click += new System.EventHandler(this.mnuToolSoundLoop_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(208, 6);
            // 
            // mnuSettings
            // 
            this.mnuSettings.Image = ((System.Drawing.Image)(resources.GetObject("mnuSettings.Image")));
            this.mnuSettings.Name = "mnuSettings";
            this.mnuSettings.Size = new System.Drawing.Size(211, 22);
            this.mnuSettings.Text = "環境設定(&C)...";
            this.mnuSettings.Click += new System.EventHandler(this.mnuSettings_Click);
            // 
            // openFileDlg
            // 
            this.openFileDlg.Filter = "すべてのファイル (*.*)|*.*";
            this.openFileDlg.RestoreDirectory = true;
            this.openFileDlg.Title = "参照";
            // 
            // miniToolStrip
            // 
            this.miniToolStrip.AutoSize = false;
            this.miniToolStrip.CanOverflow = false;
            this.miniToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.miniToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.miniToolStrip.Location = new System.Drawing.Point(159, 3);
            this.miniToolStrip.Name = "miniToolStrip";
            this.miniToolStrip.Size = new System.Drawing.Size(226, 25);
            this.miniToolStrip.TabIndex = 2;
            // 
            // pnlDefaultDrawTarget
            // 
            this.pnlDefaultDrawTarget.BackColor = System.Drawing.Color.White;
            this.pnlDefaultDrawTarget.Location = new System.Drawing.Point(0, 0);
            this.pnlDefaultDrawTarget.Name = "pnlDefaultDrawTarget";
            this.pnlDefaultDrawTarget.Size = new System.Drawing.Size(3840, 2160);
            this.pnlDefaultDrawTarget.TabIndex = 4;
            this.pnlDefaultDrawTarget.Visible = false;
            // 
            // splMain1
            // 
            this.splMain1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splMain1.Location = new System.Drawing.Point(0, 25);
            this.splMain1.Margin = new System.Windows.Forms.Padding(0);
            this.splMain1.Name = "splMain1";
            // 
            // splMain1.Panel1
            // 
            this.splMain1.Panel1.Controls.Add(this.tbcLeftPaneTabs);
            this.splMain1.Panel1MinSize = 0;
            // 
            // splMain1.Panel2
            // 
            this.splMain1.Panel2.Controls.Add(this.tbcRightPaneTabs);
            this.splMain1.Panel2Collapsed = true;
            this.splMain1.Panel2MinSize = 0;
            this.splMain1.Size = new System.Drawing.Size(1268, 539);
            this.splMain1.SplitterDistance = 602;
            this.splMain1.SplitterIncrement = 2;
            this.splMain1.SplitterWidth = 3;
            this.splMain1.TabIndex = 6;
            this.splMain1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splMain1_SplitterMoved);
            // 
            // tbcLeftPaneTabs
            // 
            this.tbcLeftPaneTabs.Appearance = System.Windows.Forms.TabAppearance.Buttons;
            this.tbcLeftPaneTabs.Controls.Add(this.tbpSchedule);
            this.tbcLeftPaneTabs.Controls.Add(this.tbpText);
            this.tbcLeftPaneTabs.Controls.Add(this.tbpDB);
            this.tbcLeftPaneTabs.Controls.Add(this.tbpMap);
            this.tbcLeftPaneTabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbcLeftPaneTabs.HotTrack = true;
            this.tbcLeftPaneTabs.ImageList = this.imlMainTabIcons;
            this.tbcLeftPaneTabs.ItemSize = new System.Drawing.Size(120, 26);
            this.tbcLeftPaneTabs.Location = new System.Drawing.Point(0, 0);
            this.tbcLeftPaneTabs.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tbcLeftPaneTabs.Name = "tbcLeftPaneTabs";
            this.tbcLeftPaneTabs.SelectedIndex = 0;
            this.tbcLeftPaneTabs.Size = new System.Drawing.Size(1268, 539);
            this.tbcLeftPaneTabs.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tbcLeftPaneTabs.TabIndex = 0;
            this.tbcLeftPaneTabs.Visible = false;
            // 
            // tbpSchedule
            // 
            this.tbpSchedule.Controls.Add(this.uctlSchedule);
            this.tbpSchedule.ImageIndex = 0;
            this.tbpSchedule.Location = new System.Drawing.Point(4, 30);
            this.tbpSchedule.Name = "tbpSchedule";
            this.tbpSchedule.Padding = new System.Windows.Forms.Padding(3);
            this.tbpSchedule.Size = new System.Drawing.Size(1260, 505);
            this.tbpSchedule.TabIndex = 4;
            this.tbpSchedule.Text = "スケジュール";
            this.tbpSchedule.UseVisualStyleBackColor = true;
            // 
            // uctlSchedule
            // 
            this.uctlSchedule.Dock = System.Windows.Forms.DockStyle.Fill;
            this.uctlSchedule.EnabledEditMode = true;
            this.uctlSchedule.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlSchedule.Location = new System.Drawing.Point(3, 3);
            this.uctlSchedule.Margin = new System.Windows.Forms.Padding(0);
            this.uctlSchedule.Name = "uctlSchedule";
            this.uctlSchedule.Size = new System.Drawing.Size(1254, 499);
            this.uctlSchedule.TabIndex = 0;
            // 
            // tbpText
            // 
            this.tbpText.Controls.Add(this.uctlTextEditor);
            this.tbpText.ImageIndex = 1;
            this.tbpText.Location = new System.Drawing.Point(4, 30);
            this.tbpText.Name = "tbpText";
            this.tbpText.Size = new System.Drawing.Size(1260, 505);
            this.tbpText.TabIndex = 0;
            this.tbpText.Text = "テキスト";
            this.tbpText.UseVisualStyleBackColor = true;
            // 
            // uctlTextEditor
            // 
            this.uctlTextEditor.DBList = null;
            this.uctlTextEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.uctlTextEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlTextEditor.Location = new System.Drawing.Point(0, 0);
            this.uctlTextEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlTextEditor.Name = "uctlTextEditor";
            this.uctlTextEditor.Size = new System.Drawing.Size(1260, 505);
            this.uctlTextEditor.TabIndex = 0;
            // 
            // tbpDB
            // 
            this.tbpDB.Controls.Add(this.uctlDBEditor);
            this.tbpDB.ImageIndex = 2;
            this.tbpDB.Location = new System.Drawing.Point(4, 30);
            this.tbpDB.Name = "tbpDB";
            this.tbpDB.Size = new System.Drawing.Size(1260, 505);
            this.tbpDB.TabIndex = 1;
            this.tbpDB.Text = "データベース";
            this.tbpDB.UseVisualStyleBackColor = true;
            // 
            // uctlDBEditor
            // 
            this.uctlDBEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.uctlDBEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlDBEditor.Location = new System.Drawing.Point(0, 0);
            this.uctlDBEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlDBEditor.Name = "uctlDBEditor";
            this.uctlDBEditor.Size = new System.Drawing.Size(1260, 505);
            this.uctlDBEditor.TabIndex = 0;
            // 
            // tbpMap
            // 
            this.tbpMap.Controls.Add(this.uctlMapEditor);
            this.tbpMap.ImageIndex = 3;
            this.tbpMap.Location = new System.Drawing.Point(4, 30);
            this.tbpMap.Name = "tbpMap";
            this.tbpMap.Size = new System.Drawing.Size(1260, 505);
            this.tbpMap.TabIndex = 3;
            this.tbpMap.Text = "マップ";
            this.tbpMap.UseVisualStyleBackColor = true;
            // 
            // uctlMapEditor
            // 
            this.uctlMapEditor.comTree = null;
            this.uctlMapEditor.DBCtrl = null;
            this.uctlMapEditor.DBList = null;
            this.uctlMapEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.uctlMapEditor.FileName = null;
            this.uctlMapEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlMapEditor.IsViewerMode = false;
            this.uctlMapEditor.Location = new System.Drawing.Point(0, 0);
            this.uctlMapEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlMapEditor.Name = "uctlMapEditor";
            this.uctlMapEditor.Size = new System.Drawing.Size(1260, 505);
            this.uctlMapEditor.TabIndex = 0;
            this.uctlMapEditor.OpenScriptRequested += new Editor.Module.Common.RequestOpenFileEventHandler(this.RequestOpenScriptEventHandler);
            // 
            // tbcRightPaneTabs
            // 
            this.tbcRightPaneTabs.Appearance = System.Windows.Forms.TabAppearance.Buttons;
            this.tbcRightPaneTabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbcRightPaneTabs.HotTrack = true;
            this.tbcRightPaneTabs.ImageList = this.imlMainTabIcons;
            this.tbcRightPaneTabs.ItemSize = new System.Drawing.Size(130, 30);
            this.tbcRightPaneTabs.Location = new System.Drawing.Point(0, 0);
            this.tbcRightPaneTabs.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tbcRightPaneTabs.Name = "tbcRightPaneTabs";
            this.tbcRightPaneTabs.SelectedIndex = 0;
            this.tbcRightPaneTabs.Size = new System.Drawing.Size(96, 100);
            this.tbcRightPaneTabs.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tbcRightPaneTabs.TabIndex = 4;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.AppWorkspace;
            this.ClientSize = new System.Drawing.Size(1268, 564);
            this.Controls.Add(this.splMain1);
            this.Controls.Add(this.mspMainMenu);
            this.Controls.Add(this.pnlDefaultDrawTarget);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MinimumSize = new System.Drawing.Size(800, 600);
            this.Name = "MainWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "RPG Developer NEXT";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainWindow_FormClosing);
            this.Load += new System.EventHandler(this.MainWindow_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainWindow_KeyDown);
            this.mspMainMenu.ResumeLayout(false);
            this.mspMainMenu.PerformLayout();
            this.splMain1.Panel1.ResumeLayout(false);
            this.splMain1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splMain1)).EndInit();
            this.splMain1.ResumeLayout(false);
            this.tbcLeftPaneTabs.ResumeLayout(false);
            this.tbpSchedule.ResumeLayout(false);
            this.tbpText.ResumeLayout(false);
            this.tbpDB.ResumeLayout(false);
            this.tbpMap.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		public System.Windows.Forms.ImageList imlMainTabIcons;
		public System.Windows.Forms.ToolStripMenuItem mnuManual;
		public System.Windows.Forms.ToolStripMenuItem mnuVersion;
		public System.Windows.Forms.ToolStripMenuItem mnuHelp;
		public System.Windows.Forms.ToolStripMenuItem mnuToolGraphicTrans;
		public System.Windows.Forms.MenuStrip mspMainMenu;
		public System.Windows.Forms.ToolStripMenuItem mnuProject;
		public System.Windows.Forms.ToolStripMenuItem mnuProjectNew;
		public System.Windows.Forms.ToolStripMenuItem mnuProjectOpen;
		public System.Windows.Forms.ToolStripMenuItem mnuProjectSave;
		public System.Windows.Forms.ToolStripSeparator mnuProjectSplit1;
		public System.Windows.Forms.ToolStripMenuItem mnuProjectFolderOpen;
		public System.Windows.Forms.ToolStripMenuItem mnuProjectSettings;
		public System.Windows.Forms.ToolStripSeparator mnuProjectSplit2;
		public System.Windows.Forms.ToolStripMenuItem mnuPublish;
		public System.Windows.Forms.ToolStripMenuItem mnuTestPlay;
		public System.Windows.Forms.ToolStripMenuItem mnuView;
		public System.Windows.Forms.ToolStripMenuItem mnuView2Pane;
		public System.Windows.Forms.ToolStripMenuItem mnuTool;
		public System.Windows.Forms.ToolStripMenuItem mnuToolSoundLoop;
		public System.Windows.Forms.ToolStripMenuItem mnuToolGraphicExpand;
		public System.Windows.Forms.ToolStripMenuItem mnuSettings;
		public System.Windows.Forms.FolderBrowserDialog folderDlg;
		public System.Windows.Forms.OpenFileDialog openFileDlg;
		private System.Windows.Forms.ToolStripMenuItem mnuViewPainToRight;
		private System.Windows.Forms.ToolStripMenuItem mnuViewPainToLeft;
		private System.Windows.Forms.ToolStripMenuItem mnuProjectClose;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStrip miniToolStrip;
		private System.Windows.Forms.ToolStripMenuItem mnuToolRPGTkoolConvert;
		public System.Windows.Forms.ToolStripMenuItem mnuToolAutotileConvert;
		public System.Windows.Forms.ToolStripMenuItem mnuToolWalkConvert;
		public System.Windows.Forms.Panel pnlDefaultDrawTarget;
		private System.Windows.Forms.ToolStripMenuItem mnuProjectSettingsReload;
		private System.Windows.Forms.ToolStripMenuItem mnuGarbageCleaner;
        public System.Windows.Forms.SplitContainer splMain1;
        public System.Windows.Forms.TabControl tbcLeftPaneTabs;
        public System.Windows.Forms.TabPage tbpSchedule;
        public CtrlComponent.Schedule.ctlScheduleEditor uctlSchedule;
        public System.Windows.Forms.TabPage tbpText;
        public CtrlComponent.Text.ctlTextEditor uctlTextEditor;
        public System.Windows.Forms.TabPage tbpDB;
        public CtrlComponent.Database.ctlDatabaseEditor uctlDBEditor;
        public System.Windows.Forms.TabPage tbpMap;
        public CtrlComponent.Map.ctlMapEditor uctlMapEditor;
        public System.Windows.Forms.TabControl tbcRightPaneTabs;
    }
}

