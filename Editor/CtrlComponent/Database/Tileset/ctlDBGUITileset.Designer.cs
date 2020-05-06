namespace Editor.CtrlComponent.Database {
	partial class ctlDBGUITileset {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlDBGUITileset));
			this.TableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.Panel1 = new System.Windows.Forms.Panel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.ltvAutoTiles = new System.Windows.Forms.ListView();
			this.ColumnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ToolStrip2 = new System.Windows.Forms.ToolStrip();
			this.ToolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
			this.ToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.tolAutoTileAdd = new System.Windows.Forms.ToolStripButton();
			this.tolAutoTileFolderAdd = new System.Windows.Forms.ToolStripButton();
			this.tolAutoTileEdit = new System.Windows.Forms.ToolStripButton();
			this.tolAutoTileDelete = new System.Windows.Forms.ToolStripButton();
			this.tolAutoTileUp = new System.Windows.Forms.ToolStripButton();
			this.tolAutoTileDown = new System.Windows.Forms.ToolStripButton();
			this.ltvObjTiles = new System.Windows.Forms.ListView();
			this.ColumnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ToolStrip3 = new System.Windows.Forms.ToolStrip();
			this.ToolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
			this.ToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.tolOBJAdd = new System.Windows.Forms.ToolStripButton();
			this.tolOBJFolderAdd = new System.Windows.Forms.ToolStripButton();
			this.tolOBJEdit = new System.Windows.Forms.ToolStripButton();
			this.tolOBJDelete = new System.Windows.Forms.ToolStripButton();
			this.tolOBJUp = new System.Windows.Forms.ToolStripButton();
			this.tolOBJDown = new System.Windows.Forms.ToolStripButton();
			this.Panel2 = new System.Windows.Forms.Panel();
			this.tbcTileSettingTab = new System.Windows.Forms.TabControl();
			this.TabPage1 = new System.Windows.Forms.TabPage();
			this.picPreview = new System.Windows.Forms.PictureBox();
			this.TabPage4 = new System.Windows.Forms.TabPage();
			this.TabPage2 = new System.Windows.Forms.TabPage();
			this.tabPage5 = new System.Windows.Forms.TabPage();
			this.lblTileDataComment = new System.Windows.Forms.Label();
			this.ToolStrip1 = new System.Windows.Forms.ToolStrip();
			this.ToolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.tolTilesetID = new System.Windows.Forms.ToolStripTextBox();
			this.ToolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
			this.tolTilesetName = new System.Windows.Forms.ToolStripTextBox();
			this.ToolStripLabel5 = new System.Windows.Forms.ToolStripLabel();
			this.tolMemo = new System.Windows.Forms.ToolStripTextBox();
			this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			((System.ComponentModel.ISupportInitialize)(this.splMain1)).BeginInit();
			this.splMain1.Panel1.SuspendLayout();
			this.splMain1.Panel2.SuspendLayout();
			this.splMain1.SuspendLayout();
			this.pnlEditor.SuspendLayout();
			this.TableLayoutPanel1.SuspendLayout();
			this.Panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.ToolStrip2.SuspendLayout();
			this.ToolStrip3.SuspendLayout();
			this.Panel2.SuspendLayout();
			this.tbcTileSettingTab.SuspendLayout();
			this.TabPage1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.picPreview)).BeginInit();
			this.ToolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// splMain1
			// 
			// 
			// splMain1.Panel2
			// 
			this.splMain1.Panel2.AutoScrollMinSize = new System.Drawing.Size(580, 0);
			// 
			// ltvDB
			// 
			this.ltvDB.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.ltvDB_ColumnClick);
			this.ltvDB.SelectedIndexChanged += new System.EventHandler(this.ltvDB_SelectedIndexChanged);
			// 
			// pnlEditor
			// 
			this.pnlEditor.Controls.Add(this.TableLayoutPanel1);
			this.pnlEditor.Controls.Add(this.ToolStrip1);
			this.pnlEditor.Size = new System.Drawing.Size(870, 611);
			// 
			// TableLayoutPanel1
			// 
			this.TableLayoutPanel1.ColumnCount = 2;
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 230F));
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutPanel1.Controls.Add(this.Panel1, 0, 0);
			this.TableLayoutPanel1.Controls.Add(this.Panel2, 1, 0);
			this.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TableLayoutPanel1.Location = new System.Drawing.Point(0, 25);
			this.TableLayoutPanel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TableLayoutPanel1.Name = "TableLayoutPanel1";
			this.TableLayoutPanel1.RowCount = 1;
			this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutPanel1.Size = new System.Drawing.Size(870, 586);
			this.TableLayoutPanel1.TabIndex = 3;
			// 
			// Panel1
			// 
			this.Panel1.Controls.Add(this.splitContainer1);
			this.Panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Panel1.Location = new System.Drawing.Point(0, 0);
			this.Panel1.Margin = new System.Windows.Forms.Padding(0);
			this.Panel1.Name = "Panel1";
			this.Panel1.Size = new System.Drawing.Size(230, 586);
			this.Panel1.TabIndex = 3;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.IsSplitterFixed = true;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
			this.splitContainer1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.ltvAutoTiles);
			this.splitContainer1.Panel1.Controls.Add(this.ToolStrip2);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.ltvObjTiles);
			this.splitContainer1.Panel2.Controls.Add(this.ToolStrip3);
			this.splitContainer1.Size = new System.Drawing.Size(230, 586);
			this.splitContainer1.SplitterDistance = 335;
			this.splitContainer1.SplitterWidth = 1;
			this.splitContainer1.TabIndex = 4;
			// 
			// ltvAutoTiles
			// 
			this.ltvAutoTiles.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader6,
            this.ColumnHeader3});
			this.ltvAutoTiles.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ltvAutoTiles.FullRowSelect = true;
			this.ltvAutoTiles.GridLines = true;
			this.ltvAutoTiles.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ltvAutoTiles.HideSelection = false;
			this.ltvAutoTiles.Location = new System.Drawing.Point(0, 25);
			this.ltvAutoTiles.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.ltvAutoTiles.Name = "ltvAutoTiles";
			this.ltvAutoTiles.ShowItemToolTips = true;
			this.ltvAutoTiles.Size = new System.Drawing.Size(230, 310);
			this.ltvAutoTiles.TabIndex = 1;
			this.ltvAutoTiles.UseCompatibleStateImageBehavior = false;
			this.ltvAutoTiles.View = System.Windows.Forms.View.Details;
			this.ltvAutoTiles.SelectedIndexChanged += new System.EventHandler(this.ltvAutoTiles_SelectedIndexChanged);
			// 
			// ColumnHeader3
			// 
			this.ColumnHeader3.DisplayIndex = 0;
			this.ColumnHeader3.Text = "ファイル名";
			this.ColumnHeader3.Width = 200;
			// 
			// ToolStrip2
			// 
			this.ToolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripLabel3,
            this.ToolStripSeparator2,
            this.tolAutoTileAdd,
            this.tolAutoTileFolderAdd,
            this.tolAutoTileEdit,
            this.tolAutoTileDelete,
            this.tolAutoTileUp,
            this.tolAutoTileDown});
			this.ToolStrip2.Location = new System.Drawing.Point(0, 0);
			this.ToolStrip2.Name = "ToolStrip2";
			this.ToolStrip2.Size = new System.Drawing.Size(230, 25);
			this.ToolStrip2.TabIndex = 0;
			this.ToolStrip2.Text = "ToolStrip1";
			// 
			// ToolStripLabel3
			// 
			this.ToolStripLabel3.Name = "ToolStripLabel3";
			this.ToolStripLabel3.Size = new System.Drawing.Size(61, 22);
			this.ToolStripLabel3.Text = "オートタイル";
			// 
			// ToolStripSeparator2
			// 
			this.ToolStripSeparator2.Name = "ToolStripSeparator2";
			this.ToolStripSeparator2.Size = new System.Drawing.Size(6, 25);
			// 
			// tolAutoTileAdd
			// 
			this.tolAutoTileAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolAutoTileAdd.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoTileAdd.Image")));
			this.tolAutoTileAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolAutoTileAdd.Name = "tolAutoTileAdd";
			this.tolAutoTileAdd.Size = new System.Drawing.Size(23, 22);
			this.tolAutoTileAdd.Text = "追加";
			this.tolAutoTileAdd.ToolTipText = "オートタイルを追加";
			this.tolAutoTileAdd.Click += new System.EventHandler(this.tolAutoTileAdd_Click);
			// 
			// tolAutoTileFolderAdd
			// 
			this.tolAutoTileFolderAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolAutoTileFolderAdd.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoTileFolderAdd.Image")));
			this.tolAutoTileFolderAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolAutoTileFolderAdd.Name = "tolAutoTileFolderAdd";
			this.tolAutoTileFolderAdd.Size = new System.Drawing.Size(23, 22);
			this.tolAutoTileFolderAdd.Text = "フォルダーから追加";
			this.tolAutoTileFolderAdd.ToolTipText = "オートタイルをフォルダーから追加";
			this.tolAutoTileFolderAdd.Click += new System.EventHandler(this.tolAutoTileFolderAdd_Click);
			// 
			// tolAutoTileEdit
			// 
			this.tolAutoTileEdit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolAutoTileEdit.Enabled = false;
			this.tolAutoTileEdit.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoTileEdit.Image")));
			this.tolAutoTileEdit.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolAutoTileEdit.Name = "tolAutoTileEdit";
			this.tolAutoTileEdit.Size = new System.Drawing.Size(23, 22);
			this.tolAutoTileEdit.Text = "変更";
			this.tolAutoTileEdit.ToolTipText = "選択しているオートタイルを変更";
			this.tolAutoTileEdit.Click += new System.EventHandler(this.tolAutoTileEdit_Click);
			// 
			// tolAutoTileDelete
			// 
			this.tolAutoTileDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolAutoTileDelete.Enabled = false;
			this.tolAutoTileDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoTileDelete.Image")));
			this.tolAutoTileDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolAutoTileDelete.Name = "tolAutoTileDelete";
			this.tolAutoTileDelete.Size = new System.Drawing.Size(23, 22);
			this.tolAutoTileDelete.Text = "削除";
			this.tolAutoTileDelete.ToolTipText = "選択しているオートタイルを削除";
			this.tolAutoTileDelete.Click += new System.EventHandler(this.tolAutoTileDelete_Click);
			// 
			// tolAutoTileUp
			// 
			this.tolAutoTileUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolAutoTileUp.Enabled = false;
			this.tolAutoTileUp.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoTileUp.Image")));
			this.tolAutoTileUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolAutoTileUp.Name = "tolAutoTileUp";
			this.tolAutoTileUp.Size = new System.Drawing.Size(23, 22);
			this.tolAutoTileUp.Text = "上へ";
			this.tolAutoTileUp.ToolTipText = "選択しているオートタイルを上へ";
			this.tolAutoTileUp.Click += new System.EventHandler(this.tolAutoTileUp_Click);
			// 
			// tolAutoTileDown
			// 
			this.tolAutoTileDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolAutoTileDown.Enabled = false;
			this.tolAutoTileDown.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoTileDown.Image")));
			this.tolAutoTileDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolAutoTileDown.Name = "tolAutoTileDown";
			this.tolAutoTileDown.Size = new System.Drawing.Size(23, 22);
			this.tolAutoTileDown.Text = "下へ";
			this.tolAutoTileDown.ToolTipText = "選択しているオートタイルを下へ";
			this.tolAutoTileDown.Click += new System.EventHandler(this.tolAutoTileDown_Click);
			// 
			// ltvObjTiles
			// 
			this.ltvObjTiles.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader7,
            this.ColumnHeader4});
			this.ltvObjTiles.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ltvObjTiles.FullRowSelect = true;
			this.ltvObjTiles.GridLines = true;
			this.ltvObjTiles.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ltvObjTiles.HideSelection = false;
			this.ltvObjTiles.Location = new System.Drawing.Point(0, 25);
			this.ltvObjTiles.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.ltvObjTiles.Name = "ltvObjTiles";
			this.ltvObjTiles.ShowItemToolTips = true;
			this.ltvObjTiles.Size = new System.Drawing.Size(230, 225);
			this.ltvObjTiles.TabIndex = 3;
			this.ltvObjTiles.UseCompatibleStateImageBehavior = false;
			this.ltvObjTiles.View = System.Windows.Forms.View.Details;
			this.ltvObjTiles.SelectedIndexChanged += new System.EventHandler(this.ltvObjects_SelectedIndexChanged);
			// 
			// ColumnHeader4
			// 
			this.ColumnHeader4.DisplayIndex = 0;
			this.ColumnHeader4.Text = "ファイル名";
			this.ColumnHeader4.Width = 200;
			// 
			// ToolStrip3
			// 
			this.ToolStrip3.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripLabel4,
            this.ToolStripSeparator1,
            this.tolOBJAdd,
            this.tolOBJFolderAdd,
            this.tolOBJEdit,
            this.tolOBJDelete,
            this.tolOBJUp,
            this.tolOBJDown});
			this.ToolStrip3.Location = new System.Drawing.Point(0, 0);
			this.ToolStrip3.Name = "ToolStrip3";
			this.ToolStrip3.Size = new System.Drawing.Size(230, 25);
			this.ToolStrip3.TabIndex = 2;
			this.ToolStrip3.Text = "ToolStrip3";
			// 
			// ToolStripLabel4
			// 
			this.ToolStripLabel4.Name = "ToolStripLabel4";
			this.ToolStripLabel4.Size = new System.Drawing.Size(58, 22);
			this.ToolStripLabel4.Text = "オブジェクト";
			// 
			// ToolStripSeparator1
			// 
			this.ToolStripSeparator1.Name = "ToolStripSeparator1";
			this.ToolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// tolOBJAdd
			// 
			this.tolOBJAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolOBJAdd.Image = ((System.Drawing.Image)(resources.GetObject("tolOBJAdd.Image")));
			this.tolOBJAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolOBJAdd.Name = "tolOBJAdd";
			this.tolOBJAdd.Size = new System.Drawing.Size(23, 22);
			this.tolOBJAdd.Text = "追加";
			this.tolOBJAdd.ToolTipText = "オブジェクトタイルを追加";
			this.tolOBJAdd.Click += new System.EventHandler(this.tolOBJAdd_Click);
			// 
			// tolOBJFolderAdd
			// 
			this.tolOBJFolderAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolOBJFolderAdd.Image = ((System.Drawing.Image)(resources.GetObject("tolOBJFolderAdd.Image")));
			this.tolOBJFolderAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolOBJFolderAdd.Name = "tolOBJFolderAdd";
			this.tolOBJFolderAdd.Size = new System.Drawing.Size(23, 22);
			this.tolOBJFolderAdd.Text = "フォルダーから追加";
			this.tolOBJFolderAdd.ToolTipText = "オブジェクトタイルをフォルダーから追加";
			this.tolOBJFolderAdd.Click += new System.EventHandler(this.tolOBJFolderAdd_Click);
			// 
			// tolOBJEdit
			// 
			this.tolOBJEdit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolOBJEdit.Enabled = false;
			this.tolOBJEdit.Image = ((System.Drawing.Image)(resources.GetObject("tolOBJEdit.Image")));
			this.tolOBJEdit.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolOBJEdit.Name = "tolOBJEdit";
			this.tolOBJEdit.Size = new System.Drawing.Size(23, 22);
			this.tolOBJEdit.Text = "変更";
			this.tolOBJEdit.ToolTipText = "選択しているオブジェクトタイルを変更";
			this.tolOBJEdit.Click += new System.EventHandler(this.tolOBJEdit_Click);
			// 
			// tolOBJDelete
			// 
			this.tolOBJDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolOBJDelete.Enabled = false;
			this.tolOBJDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolOBJDelete.Image")));
			this.tolOBJDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolOBJDelete.Name = "tolOBJDelete";
			this.tolOBJDelete.Size = new System.Drawing.Size(23, 22);
			this.tolOBJDelete.Text = "削除";
			this.tolOBJDelete.ToolTipText = "選択しているオブジェクトタイルを削除";
			this.tolOBJDelete.Click += new System.EventHandler(this.tolOBJDelete_Click);
			// 
			// tolOBJUp
			// 
			this.tolOBJUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolOBJUp.Enabled = false;
			this.tolOBJUp.Image = ((System.Drawing.Image)(resources.GetObject("tolOBJUp.Image")));
			this.tolOBJUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolOBJUp.Name = "tolOBJUp";
			this.tolOBJUp.Size = new System.Drawing.Size(23, 22);
			this.tolOBJUp.Text = "上へ";
			this.tolOBJUp.ToolTipText = "選択しているオブジェクトタイルを上へ";
			this.tolOBJUp.Click += new System.EventHandler(this.tolOBJUp_Click);
			// 
			// tolOBJDown
			// 
			this.tolOBJDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolOBJDown.Enabled = false;
			this.tolOBJDown.Image = ((System.Drawing.Image)(resources.GetObject("tolOBJDown.Image")));
			this.tolOBJDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolOBJDown.Name = "tolOBJDown";
			this.tolOBJDown.Size = new System.Drawing.Size(23, 22);
			this.tolOBJDown.Text = "下へ";
			this.tolOBJDown.ToolTipText = "選択しているオブジェクトタイルを下へ";
			this.tolOBJDown.Click += new System.EventHandler(this.tolOBJDown_Click);
			// 
			// Panel2
			// 
			this.Panel2.Controls.Add(this.tbcTileSettingTab);
			this.Panel2.Controls.Add(this.lblTileDataComment);
			this.Panel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Panel2.Location = new System.Drawing.Point(230, 0);
			this.Panel2.Margin = new System.Windows.Forms.Padding(0);
			this.Panel2.Name = "Panel2";
			this.Panel2.Size = new System.Drawing.Size(640, 586);
			this.Panel2.TabIndex = 4;
			// 
			// tbcTileSettingTab
			// 
			this.tbcTileSettingTab.Controls.Add(this.TabPage1);
			this.tbcTileSettingTab.Controls.Add(this.TabPage4);
			this.tbcTileSettingTab.Controls.Add(this.TabPage2);
			this.tbcTileSettingTab.Controls.Add(this.tabPage5);
			this.tbcTileSettingTab.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tbcTileSettingTab.ItemSize = new System.Drawing.Size(85, 23);
			this.tbcTileSettingTab.Location = new System.Drawing.Point(0, 39);
			this.tbcTileSettingTab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.tbcTileSettingTab.Name = "tbcTileSettingTab";
			this.tbcTileSettingTab.SelectedIndex = 0;
			this.tbcTileSettingTab.Size = new System.Drawing.Size(640, 547);
			this.tbcTileSettingTab.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
			this.tbcTileSettingTab.TabIndex = 1;
			this.tbcTileSettingTab.Selecting += new System.Windows.Forms.TabControlCancelEventHandler(this.tbcTileSettingTab_Selecting);
			// 
			// TabPage1
			// 
			this.TabPage1.AutoScroll = true;
			this.TabPage1.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.TabPage1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.TabPage1.Controls.Add(this.picPreview);
			this.TabPage1.Location = new System.Drawing.Point(4, 27);
			this.TabPage1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TabPage1.Name = "TabPage1";
			this.TabPage1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TabPage1.Size = new System.Drawing.Size(632, 516);
			this.TabPage1.TabIndex = 0;
			this.TabPage1.Text = "当たり判定";
			// 
			// picPreview
			// 
			this.picPreview.BackColor = System.Drawing.Color.Black;
			this.picPreview.Location = new System.Drawing.Point(0, 0);
			this.picPreview.Margin = new System.Windows.Forms.Padding(0);
			this.picPreview.Name = "picPreview";
			this.picPreview.Size = new System.Drawing.Size(100, 100);
			this.picPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picPreview.TabIndex = 0;
			this.picPreview.TabStop = false;
			this.picPreview.Paint += new System.Windows.Forms.PaintEventHandler(this.picTilePreview_Paint);
			this.picPreview.MouseUp += new System.Windows.Forms.MouseEventHandler(this.picTilePreview_MouseUp);
			// 
			// TabPage4
			// 
			this.TabPage4.AutoScroll = true;
			this.TabPage4.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.TabPage4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.TabPage4.Location = new System.Drawing.Point(4, 27);
			this.TabPage4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TabPage4.Name = "TabPage4";
			this.TabPage4.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TabPage4.Size = new System.Drawing.Size(632, 516);
			this.TabPage4.TabIndex = 3;
			this.TabPage4.Text = "出入設定";
			// 
			// TabPage2
			// 
			this.TabPage2.AutoScroll = true;
			this.TabPage2.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.TabPage2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.TabPage2.Location = new System.Drawing.Point(4, 27);
			this.TabPage2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TabPage2.Name = "TabPage2";
			this.TabPage2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.TabPage2.Size = new System.Drawing.Size(632, 516);
			this.TabPage2.TabIndex = 1;
			this.TabPage2.Text = "描画優先度";
			// 
			// tabPage5
			// 
			this.tabPage5.AutoScroll = true;
			this.tabPage5.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.tabPage5.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.tabPage5.Location = new System.Drawing.Point(4, 27);
			this.tabPage5.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.tabPage5.Name = "tabPage5";
			this.tabPage5.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.tabPage5.Size = new System.Drawing.Size(632, 516);
			this.tabPage5.TabIndex = 4;
			this.tabPage5.Text = "グループ番号";
			// 
			// lblTileDataComment
			// 
			this.lblTileDataComment.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.lblTileDataComment.Dock = System.Windows.Forms.DockStyle.Top;
			this.lblTileDataComment.Location = new System.Drawing.Point(0, 0);
			this.lblTileDataComment.Margin = new System.Windows.Forms.Padding(3, 2, 3, 0);
			this.lblTileDataComment.Name = "lblTileDataComment";
			this.lblTileDataComment.Size = new System.Drawing.Size(640, 39);
			this.lblTileDataComment.TabIndex = 0;
			this.lblTileDataComment.Text = "設定の説明\r\n２行目";
			// 
			// ToolStrip1
			// 
			this.ToolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripLabel1,
            this.tolTilesetID,
            this.ToolStripLabel2,
            this.tolTilesetName,
            this.ToolStripLabel5,
            this.tolMemo});
			this.ToolStrip1.Location = new System.Drawing.Point(0, 0);
			this.ToolStrip1.Name = "ToolStrip1";
			this.ToolStrip1.Size = new System.Drawing.Size(870, 25);
			this.ToolStrip1.TabIndex = 2;
			this.ToolStrip1.Text = "ToolStrip1";
			// 
			// ToolStripLabel1
			// 
			this.ToolStripLabel1.Name = "ToolStripLabel1";
			this.ToolStripLabel1.Size = new System.Drawing.Size(26, 22);
			this.ToolStripLabel1.Text = "ID:";
			// 
			// tolTilesetID
			// 
			this.tolTilesetID.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tolTilesetID.Name = "tolTilesetID";
			this.tolTilesetID.Size = new System.Drawing.Size(60, 25);
			this.tolTilesetID.Validating += new System.ComponentModel.CancelEventHandler(this.tmnTilesetID_Validating);
			// 
			// ToolStripLabel2
			// 
			this.ToolStripLabel2.Name = "ToolStripLabel2";
			this.ToolStripLabel2.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
			this.ToolStripLabel2.Size = new System.Drawing.Size(41, 22);
			this.ToolStripLabel2.Text = "名前:";
			this.ToolStripLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// tolTilesetName
			// 
			this.tolTilesetName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tolTilesetName.Name = "tolTilesetName";
			this.tolTilesetName.Size = new System.Drawing.Size(150, 25);
			this.tolTilesetName.Validating += new System.ComponentModel.CancelEventHandler(this.tmnTilesetName_Validating);
			// 
			// ToolStripLabel5
			// 
			this.ToolStripLabel5.Name = "ToolStripLabel5";
			this.ToolStripLabel5.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
			this.ToolStripLabel5.Size = new System.Drawing.Size(34, 22);
			this.ToolStripLabel5.Text = "メモ:";
			this.ToolStripLabel5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// tolMemo
			// 
			this.tolMemo.AcceptsReturn = true;
			this.tolMemo.AutoToolTip = true;
			this.tolMemo.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tolMemo.Name = "tolMemo";
			this.tolMemo.Size = new System.Drawing.Size(300, 25);
			this.tolMemo.Validating += new System.ComponentModel.CancelEventHandler(this.tolMemo_Validating);
			// 
			// columnHeader6
			// 
			this.columnHeader6.DisplayIndex = 1;
			this.columnHeader6.Text = "フルパス";
			this.columnHeader6.Width = 0;
			// 
			// columnHeader7
			// 
			this.columnHeader7.DisplayIndex = 1;
			this.columnHeader7.Text = "フルパス";
			this.columnHeader7.Width = 0;
			// 
			// ctlDBGUITileset
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.Margin = new System.Windows.Forms.Padding(3);
			this.Name = "ctlDBGUITileset";
			this.RequestRowAdd += new System.EventHandler(this.ctlDBGUITileset_RequestRowAdd);
			this.RequestRowCopy += new System.EventHandler(this.ctlDBGUITileset_RequestRowCopy);
			this.RequestRowDelete += new System.EventHandler(this.ctlDBGUITileset_RequestRowDelete);
			this.splMain1.Panel1.ResumeLayout(false);
			this.splMain1.Panel1.PerformLayout();
			this.splMain1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splMain1)).EndInit();
			this.splMain1.ResumeLayout(false);
			this.pnlEditor.ResumeLayout(false);
			this.pnlEditor.PerformLayout();
			this.TableLayoutPanel1.ResumeLayout(false);
			this.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.PerformLayout();
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.Panel2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this.ToolStrip2.ResumeLayout(false);
			this.ToolStrip2.PerformLayout();
			this.ToolStrip3.ResumeLayout(false);
			this.ToolStrip3.PerformLayout();
			this.Panel2.ResumeLayout(false);
			this.tbcTileSettingTab.ResumeLayout(false);
			this.TabPage1.ResumeLayout(false);
			this.TabPage1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.picPreview)).EndInit();
			this.ToolStrip1.ResumeLayout(false);
			this.ToolStrip1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
		private System.Windows.Forms.Panel Panel1;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.ListView ltvAutoTiles;
		private System.Windows.Forms.ColumnHeader ColumnHeader3;
		private System.Windows.Forms.ToolStrip ToolStrip2;
		private System.Windows.Forms.ToolStripLabel ToolStripLabel3;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator2;
		private System.Windows.Forms.ToolStripButton tolAutoTileAdd;
		private System.Windows.Forms.ToolStripButton tolAutoTileEdit;
		private System.Windows.Forms.ToolStripButton tolAutoTileDelete;
		private System.Windows.Forms.ToolStripButton tolAutoTileUp;
		private System.Windows.Forms.ToolStripButton tolAutoTileDown;
		private System.Windows.Forms.ListView ltvObjTiles;
		private System.Windows.Forms.ColumnHeader ColumnHeader4;
		private System.Windows.Forms.ToolStrip ToolStrip3;
		private System.Windows.Forms.ToolStripLabel ToolStripLabel4;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tolOBJAdd;
		private System.Windows.Forms.ToolStripButton tolOBJEdit;
		private System.Windows.Forms.ToolStripButton tolOBJDelete;
		private System.Windows.Forms.ToolStripButton tolOBJUp;
		private System.Windows.Forms.ToolStripButton tolOBJDown;
		private System.Windows.Forms.Panel Panel2;
		private System.Windows.Forms.TabControl tbcTileSettingTab;
		private System.Windows.Forms.TabPage TabPage1;
		private System.Windows.Forms.PictureBox picPreview;
		private System.Windows.Forms.TabPage TabPage4;
		private System.Windows.Forms.TabPage TabPage2;
		private System.Windows.Forms.TabPage tabPage5;
		private System.Windows.Forms.Label lblTileDataComment;
		private System.Windows.Forms.ToolStrip ToolStrip1;
		private System.Windows.Forms.ToolStripLabel ToolStripLabel1;
		private System.Windows.Forms.ToolStripTextBox tolTilesetID;
		private System.Windows.Forms.ToolStripLabel ToolStripLabel2;
		private System.Windows.Forms.ToolStripTextBox tolTilesetName;
		private System.Windows.Forms.ToolStripLabel ToolStripLabel5;
		private System.Windows.Forms.ToolStripTextBox tolMemo;
		private System.Windows.Forms.ToolStripButton tolAutoTileFolderAdd;
		private System.Windows.Forms.ToolStripButton tolOBJFolderAdd;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader7;
	}
}
