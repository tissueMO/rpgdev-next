namespace Editor.CtrlComponent.Database {
	partial class ctlDatabaseEditor {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlDatabaseEditor));
            this.tbcDatabaseTab = new System.Windows.Forms.TabControl();
            this.tlsDatabase = new System.Windows.Forms.ToolStrip();
            this.tolDBSave = new System.Windows.Forms.ToolStripButton();
            this.tolDBSaveAll = new System.Windows.Forms.ToolStripButton();
            this.tolDBReload = new System.Windows.Forms.ToolStripButton();
            this.tolDBReloadAll = new System.Windows.Forms.ToolStripButton();
            this.tolDBImport = new System.Windows.Forms.ToolStripButton();
            this.tolDBExport = new System.Windows.Forms.ToolStripButton();
            this.tlsRow = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.tolDBUndo = new System.Windows.Forms.ToolStripButton();
            this.tolDBRedo = new System.Windows.Forms.ToolStripButton();
            this.tolDBCopy = new System.Windows.Forms.ToolStripButton();
            this.tolDBDelete = new System.Windows.Forms.ToolStripButton();
            this.tlsColumn = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.tolDBAddColumn = new System.Windows.Forms.ToolStripButton();
            this.tolDBEditColumn = new System.Windows.Forms.ToolStripButton();
            this.tolEditColumnName = new System.Windows.Forms.ToolStripButton();
            this.tolDBDeleteColumn = new System.Windows.Forms.ToolStripButton();
            this.tolDBColumnToLeft = new System.Windows.Forms.ToolStripButton();
            this.tolDBColumnToRight = new System.Windows.Forms.ToolStripButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tlsSearch = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
            this.tolSearchText = new System.Windows.Forms.ToolStripTextBox();
            this.tolDoSearch = new System.Windows.Forms.ToolStripButton();
            this.tlsDatabase.SuspendLayout();
            this.tlsRow.SuspendLayout();
            this.tlsColumn.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tlsSearch.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbcDatabaseTab
            // 
            this.tbcDatabaseTab.Alignment = System.Windows.Forms.TabAlignment.Left;
            this.tbcDatabaseTab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbcDatabaseTab.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
            this.tbcDatabaseTab.HotTrack = true;
            this.tbcDatabaseTab.ItemSize = new System.Drawing.Size(30, 120);
            this.tbcDatabaseTab.Location = new System.Drawing.Point(0, 53);
            this.tbcDatabaseTab.Multiline = true;
            this.tbcDatabaseTab.Name = "tbcDatabaseTab";
            this.tbcDatabaseTab.SelectedIndex = 0;
            this.tbcDatabaseTab.Size = new System.Drawing.Size(1029, 497);
            this.tbcDatabaseTab.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tbcDatabaseTab.TabIndex = 1;
            this.tbcDatabaseTab.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.tbcDatabaseTab_DrawItem);
            this.tbcDatabaseTab.Selecting += new System.Windows.Forms.TabControlCancelEventHandler(this.tbcDatabaseTab_Selecting);
            // 
            // tlsDatabase
            // 
            this.tlsDatabase.Dock = System.Windows.Forms.DockStyle.None;
            this.tlsDatabase.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolDBSave,
            this.tolDBSaveAll,
            this.tolDBReload,
            this.tolDBReloadAll,
            this.tolDBImport,
            this.tolDBExport});
            this.tlsDatabase.Location = new System.Drawing.Point(0, 1);
            this.tlsDatabase.Name = "tlsDatabase";
            this.tlsDatabase.Size = new System.Drawing.Size(384, 25);
            this.tlsDatabase.TabIndex = 0;
            this.tlsDatabase.Text = "toolStrip3";
            // 
            // tolDBSave
            // 
            this.tolDBSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBSave.Image = ((System.Drawing.Image)(resources.GetObject("tolDBSave.Image")));
            this.tolDBSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBSave.Name = "tolDBSave";
            this.tolDBSave.Size = new System.Drawing.Size(23, 22);
            this.tolDBSave.Text = "現在のデータベースを保存";
            this.tolDBSave.Click += new System.EventHandler(this.tolDBSave_Click);
            // 
            // tolDBSaveAll
            // 
            this.tolDBSaveAll.Image = ((System.Drawing.Image)(resources.GetObject("tolDBSaveAll.Image")));
            this.tolDBSaveAll.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBSaveAll.Name = "tolDBSaveAll";
            this.tolDBSaveAll.Size = new System.Drawing.Size(80, 22);
            this.tolDBSaveAll.Text = "すべて保存";
            this.tolDBSaveAll.Click += new System.EventHandler(this.tolDBSaveAll_Click);
            // 
            // tolDBReload
            // 
            this.tolDBReload.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBReload.Image = ((System.Drawing.Image)(resources.GetObject("tolDBReload.Image")));
            this.tolDBReload.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBReload.Name = "tolDBReload";
            this.tolDBReload.Size = new System.Drawing.Size(23, 22);
            this.tolDBReload.Text = "現在のデータベースを再読込";
            this.tolDBReload.Click += new System.EventHandler(this.tolDBReload_Click);
            // 
            // tolDBReloadAll
            // 
            this.tolDBReloadAll.Image = ((System.Drawing.Image)(resources.GetObject("tolDBReloadAll.Image")));
            this.tolDBReloadAll.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBReloadAll.Name = "tolDBReloadAll";
            this.tolDBReloadAll.Size = new System.Drawing.Size(92, 22);
            this.tolDBReloadAll.Text = "すべて再読込";
            this.tolDBReloadAll.Click += new System.EventHandler(this.tolDBReloadAll_Click);
            // 
            // tolDBImport
            // 
            this.tolDBImport.Image = ((System.Drawing.Image)(resources.GetObject("tolDBImport.Image")));
            this.tolDBImport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBImport.Name = "tolDBImport";
            this.tolDBImport.Size = new System.Drawing.Size(73, 22);
            this.tolDBImport.Text = "インポート";
            this.tolDBImport.Click += new System.EventHandler(this.tolDBImport_Click);
            // 
            // tolDBExport
            // 
            this.tolDBExport.Image = ((System.Drawing.Image)(resources.GetObject("tolDBExport.Image")));
            this.tolDBExport.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBExport.Name = "tolDBExport";
            this.tolDBExport.Size = new System.Drawing.Size(81, 22);
            this.tolDBExport.Text = "エクスポート";
            this.tolDBExport.Click += new System.EventHandler(this.tolDBExport_Click);
            // 
            // tlsRow
            // 
            this.tlsRow.Dock = System.Windows.Forms.DockStyle.None;
            this.tlsRow.Enabled = false;
            this.tlsRow.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel2,
            this.toolStripSeparator2,
            this.tolDBUndo,
            this.tolDBRedo,
            this.tolDBCopy,
            this.tolDBDelete});
            this.tlsRow.Location = new System.Drawing.Point(384, 1);
            this.tlsRow.Name = "tlsRow";
            this.tlsRow.Size = new System.Drawing.Size(153, 25);
            this.tlsRow.TabIndex = 1;
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(43, 22);
            this.toolStripLabel2.Text = "行操作";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // tolDBUndo
            // 
            this.tolDBUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBUndo.Image = ((System.Drawing.Image)(resources.GetObject("tolDBUndo.Image")));
            this.tolDBUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBUndo.Name = "tolDBUndo";
            this.tolDBUndo.Size = new System.Drawing.Size(23, 22);
            this.tolDBUndo.Text = "元に戻す";
            this.tolDBUndo.Click += new System.EventHandler(this.tolDBUndo_Click);
            // 
            // tolDBRedo
            // 
            this.tolDBRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBRedo.Image = ((System.Drawing.Image)(resources.GetObject("tolDBRedo.Image")));
            this.tolDBRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBRedo.Name = "tolDBRedo";
            this.tolDBRedo.Size = new System.Drawing.Size(23, 22);
            this.tolDBRedo.Text = "やり直し";
            this.tolDBRedo.Click += new System.EventHandler(this.tolDBRedo_Click);
            // 
            // tolDBCopy
            // 
            this.tolDBCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBCopy.Image = ((System.Drawing.Image)(resources.GetObject("tolDBCopy.Image")));
            this.tolDBCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBCopy.Name = "tolDBCopy";
            this.tolDBCopy.Size = new System.Drawing.Size(23, 22);
            this.tolDBCopy.Text = "行のコピー";
            this.tolDBCopy.Click += new System.EventHandler(this.tolDBCopy_Click);
            // 
            // tolDBDelete
            // 
            this.tolDBDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolDBDelete.Image")));
            this.tolDBDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBDelete.Name = "tolDBDelete";
            this.tolDBDelete.Size = new System.Drawing.Size(23, 22);
            this.tolDBDelete.Text = "行の削除";
            this.tolDBDelete.Click += new System.EventHandler(this.tolDBDelete_Click);
            // 
            // tlsColumn
            // 
            this.tlsColumn.Dock = System.Windows.Forms.DockStyle.None;
            this.tlsColumn.Enabled = false;
            this.tlsColumn.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel3,
            this.toolStripSeparator3,
            this.tolDBAddColumn,
            this.tolDBEditColumn,
            this.tolEditColumnName,
            this.tolDBDeleteColumn,
            this.tolDBColumnToLeft,
            this.tolDBColumnToRight});
            this.tlsColumn.Location = new System.Drawing.Point(0, 25);
            this.tlsColumn.Name = "tlsColumn";
            this.tlsColumn.Size = new System.Drawing.Size(307, 25);
            this.tlsColumn.TabIndex = 2;
            // 
            // toolStripLabel3
            // 
            this.toolStripLabel3.Name = "toolStripLabel3";
            this.toolStripLabel3.Size = new System.Drawing.Size(43, 22);
            this.toolStripLabel3.Text = "列操作";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 25);
            // 
            // tolDBAddColumn
            // 
            this.tolDBAddColumn.Image = ((System.Drawing.Image)(resources.GetObject("tolDBAddColumn.Image")));
            this.tolDBAddColumn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBAddColumn.Name = "tolDBAddColumn";
            this.tolDBAddColumn.Size = new System.Drawing.Size(51, 22);
            this.tolDBAddColumn.Text = "追加";
            this.tolDBAddColumn.Click += new System.EventHandler(this.tolDBAddColumn_Click);
            // 
            // tolDBEditColumn
            // 
            this.tolDBEditColumn.Image = ((System.Drawing.Image)(resources.GetObject("tolDBEditColumn.Image")));
            this.tolDBEditColumn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBEditColumn.Name = "tolDBEditColumn";
            this.tolDBEditColumn.Size = new System.Drawing.Size(51, 22);
            this.tolDBEditColumn.Text = "編集";
            this.tolDBEditColumn.Click += new System.EventHandler(this.tolDBEditColumn_Click);
            // 
            // tolEditColumnName
            // 
            this.tolEditColumnName.Image = ((System.Drawing.Image)(resources.GetObject("tolEditColumnName.Image")));
            this.tolEditColumnName.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolEditColumnName.Name = "tolEditColumnName";
            this.tolEditColumnName.Size = new System.Drawing.Size(75, 22);
            this.tolEditColumnName.Text = "列名変更";
            this.tolEditColumnName.Click += new System.EventHandler(this.tolEditColumnName_Click);
            // 
            // tolDBDeleteColumn
            // 
            this.tolDBDeleteColumn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBDeleteColumn.Image = ((System.Drawing.Image)(resources.GetObject("tolDBDeleteColumn.Image")));
            this.tolDBDeleteColumn.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBDeleteColumn.Name = "tolDBDeleteColumn";
            this.tolDBDeleteColumn.Size = new System.Drawing.Size(23, 22);
            this.tolDBDeleteColumn.Text = "列の削除";
            this.tolDBDeleteColumn.Click += new System.EventHandler(this.tolDBDeleteColumn_Click);
            // 
            // tolDBColumnToLeft
            // 
            this.tolDBColumnToLeft.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBColumnToLeft.Image = ((System.Drawing.Image)(resources.GetObject("tolDBColumnToLeft.Image")));
            this.tolDBColumnToLeft.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBColumnToLeft.Name = "tolDBColumnToLeft";
            this.tolDBColumnToLeft.Size = new System.Drawing.Size(23, 22);
            this.tolDBColumnToLeft.Text = "列を左へ移動";
            this.tolDBColumnToLeft.Click += new System.EventHandler(this.tolDBColumnToLeft_Click);
            // 
            // tolDBColumnToRight
            // 
            this.tolDBColumnToRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDBColumnToRight.Image = ((System.Drawing.Image)(resources.GetObject("tolDBColumnToRight.Image")));
            this.tolDBColumnToRight.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDBColumnToRight.Name = "tolDBColumnToRight";
            this.tolDBColumnToRight.Size = new System.Drawing.Size(23, 22);
            this.tolDBColumnToRight.Text = "列を右へ移動";
            this.tolDBColumnToRight.Click += new System.EventHandler(this.tolDBColumnToRight_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.tlsSearch);
            this.panel1.Controls.Add(this.tlsDatabase);
            this.panel1.Controls.Add(this.tlsColumn);
            this.panel1.Controls.Add(this.tlsRow);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1029, 53);
            this.panel1.TabIndex = 0;
            // 
            // tlsSearch
            // 
            this.tlsSearch.Dock = System.Windows.Forms.DockStyle.None;
            this.tlsSearch.Enabled = false;
            this.tlsSearch.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel4,
            this.tolSearchText,
            this.tolDoSearch});
            this.tlsSearch.Location = new System.Drawing.Point(307, 25);
            this.tlsSearch.Name = "tlsSearch";
            this.tlsSearch.Size = new System.Drawing.Size(254, 25);
            this.tlsSearch.TabIndex = 3;
            // 
            // toolStripLabel4
            // 
            this.toolStripLabel4.Name = "toolStripLabel4";
            this.toolStripLabel4.Size = new System.Drawing.Size(36, 22);
            this.toolStripLabel4.Text = "検索:";
            // 
            // tolSearchText
            // 
            this.tolSearchText.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tolSearchText.Name = "tolSearchText";
            this.tolSearchText.Size = new System.Drawing.Size(150, 25);
            this.tolSearchText.ToolTipText = "データベースの項目名で検索を行います。";
            this.tolSearchText.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tolSearchText_KeyUp);
            // 
            // tolDoSearch
            // 
            this.tolDoSearch.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolDoSearch.Image = ((System.Drawing.Image)(resources.GetObject("tolDoSearch.Image")));
            this.tolDoSearch.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDoSearch.Name = "tolDoSearch";
            this.tolDoSearch.Size = new System.Drawing.Size(23, 22);
            this.tolDoSearch.Text = "次を検索";
            this.tolDoSearch.Click += new System.EventHandler(this.tolDoSearch_Click);
            // 
            // ctlDatabaseEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tbcDatabaseTab);
            this.Controls.Add(this.panel1);
            this.Name = "ctlDatabaseEditor";
            this.Size = new System.Drawing.Size(1029, 550);
            this.tlsDatabase.ResumeLayout(false);
            this.tlsDatabase.PerformLayout();
            this.tlsRow.ResumeLayout(false);
            this.tlsRow.PerformLayout();
            this.tlsColumn.ResumeLayout(false);
            this.tlsColumn.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tlsSearch.ResumeLayout(false);
            this.tlsSearch.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		public System.Windows.Forms.TabControl tbcDatabaseTab;
		private System.Windows.Forms.ToolStrip tlsDatabase;
		internal System.Windows.Forms.ToolStripButton tolDBSave;
		internal System.Windows.Forms.ToolStripButton tolDBSaveAll;
		private System.Windows.Forms.ToolStrip tlsColumn;
		private System.Windows.Forms.ToolStripButton tolDBAddColumn;
		private System.Windows.Forms.ToolStripButton tolDBEditColumn;
		internal System.Windows.Forms.ToolStripButton tolDBDeleteColumn;
		private System.Windows.Forms.ToolStrip tlsRow;
		internal System.Windows.Forms.ToolStripButton tolDBUndo;
		internal System.Windows.Forms.ToolStripButton tolDBRedo;
		internal System.Windows.Forms.ToolStripButton tolDBCopy;
		internal System.Windows.Forms.ToolStripButton tolDBDelete;
		private System.Windows.Forms.Panel panel1;
		public System.Windows.Forms.ToolStripButton tolDBReloadAll;
		private System.Windows.Forms.ToolStripLabel toolStripLabel2;
		private System.Windows.Forms.ToolStripLabel toolStripLabel3;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStrip tlsSearch;
		private System.Windows.Forms.ToolStripLabel toolStripLabel4;
		private System.Windows.Forms.ToolStripTextBox tolSearchText;
		private System.Windows.Forms.ToolStripButton tolDoSearch;
		private System.Windows.Forms.ToolStripButton tolDBImport;
		private System.Windows.Forms.ToolStripButton tolDBExport;
		public System.Windows.Forms.ToolStripButton tolDBReload;
		private System.Windows.Forms.ToolStripButton tolDBColumnToLeft;
		private System.Windows.Forms.ToolStripButton tolDBColumnToRight;
		private System.Windows.Forms.ToolStripButton tolEditColumnName;
	}
}
