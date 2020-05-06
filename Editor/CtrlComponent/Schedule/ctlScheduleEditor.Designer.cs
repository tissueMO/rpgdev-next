namespace Editor.CtrlComponent.Schedule {
	partial class ctlScheduleEditor {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlScheduleEditor));
            this.tbcScheduleTabs = new System.Windows.Forms.TabControl();
            this.tbpViewer = new System.Windows.Forms.TabPage();
            this.tblViewerPart = new System.Windows.Forms.TableLayoutPanel();
            this.picTitle = new System.Windows.Forms.PictureBox();
            this.pnlTask = new System.Windows.Forms.Panel();
            this.picTask = new System.Windows.Forms.PictureBox();
            this.pnlDate = new System.Windows.Forms.Panel();
            this.picDate = new System.Windows.Forms.PictureBox();
            this.pnlSection = new System.Windows.Forms.Panel();
            this.picSection = new System.Windows.Forms.PictureBox();
            this.tbpEditor = new System.Windows.Forms.TabPage();
            this.ltvData = new System.Windows.Forms.ListView();
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colProgress = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tspEditorToolbar = new System.Windows.Forms.ToolStrip();
            this.tolSave = new System.Windows.Forms.ToolStripButton();
            this.tolSaveImage = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tolSection = new System.Windows.Forms.ToolStripButton();
            this.tolAddTask = new System.Windows.Forms.ToolStripButton();
            this.tolDeleteTask = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.tolSortType = new System.Windows.Forms.ToolStripComboBox();
            this.dlgSave = new System.Windows.Forms.SaveFileDialog();
            this.tbcScheduleTabs.SuspendLayout();
            this.tbpViewer.SuspendLayout();
            this.tblViewerPart.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picTitle)).BeginInit();
            this.pnlTask.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picTask)).BeginInit();
            this.pnlDate.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picDate)).BeginInit();
            this.pnlSection.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picSection)).BeginInit();
            this.tbpEditor.SuspendLayout();
            this.tspEditorToolbar.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbcScheduleTabs
            // 
            this.tbcScheduleTabs.Controls.Add(this.tbpViewer);
            this.tbcScheduleTabs.Controls.Add(this.tbpEditor);
            this.tbcScheduleTabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbcScheduleTabs.ItemSize = new System.Drawing.Size(120, 22);
            this.tbcScheduleTabs.Location = new System.Drawing.Point(0, 0);
            this.tbcScheduleTabs.Name = "tbcScheduleTabs";
            this.tbcScheduleTabs.SelectedIndex = 0;
            this.tbcScheduleTabs.Size = new System.Drawing.Size(1021, 519);
            this.tbcScheduleTabs.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tbcScheduleTabs.TabIndex = 0;
            // 
            // tbpViewer
            // 
            this.tbpViewer.AutoScroll = true;
            this.tbpViewer.Controls.Add(this.tblViewerPart);
            this.tbpViewer.Location = new System.Drawing.Point(4, 26);
            this.tbpViewer.Name = "tbpViewer";
            this.tbpViewer.Padding = new System.Windows.Forms.Padding(3);
            this.tbpViewer.Size = new System.Drawing.Size(1013, 489);
            this.tbpViewer.TabIndex = 1;
            this.tbpViewer.Text = "ビューア";
            this.tbpViewer.UseVisualStyleBackColor = true;
            // 
            // tblViewerPart
            // 
            this.tblViewerPart.ColumnCount = 2;
            this.tblViewerPart.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tblViewerPart.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblViewerPart.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tblViewerPart.Controls.Add(this.picTitle, 0, 0);
            this.tblViewerPart.Controls.Add(this.pnlTask, 1, 1);
            this.tblViewerPart.Controls.Add(this.pnlDate, 1, 0);
            this.tblViewerPart.Controls.Add(this.pnlSection, 0, 1);
            this.tblViewerPart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tblViewerPart.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.tblViewerPart.Location = new System.Drawing.Point(3, 3);
            this.tblViewerPart.Margin = new System.Windows.Forms.Padding(0);
            this.tblViewerPart.Name = "tblViewerPart";
            this.tblViewerPart.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this.tblViewerPart.RowCount = 2;
            this.tblViewerPart.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 85F));
            this.tblViewerPart.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tblViewerPart.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tblViewerPart.Size = new System.Drawing.Size(1007, 483);
            this.tblViewerPart.TabIndex = 0;
            // 
            // picTitle
            // 
            this.picTitle.BackColor = System.Drawing.SystemColors.Window;
            this.picTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.picTitle.Location = new System.Drawing.Point(5, 0);
            this.picTitle.Margin = new System.Windows.Forms.Padding(0);
            this.picTitle.Name = "picTitle";
            this.picTitle.Size = new System.Drawing.Size(150, 85);
            this.picTitle.TabIndex = 10;
            this.picTitle.TabStop = false;
            this.picTitle.Paint += new System.Windows.Forms.PaintEventHandler(this.picTitle_Paint);
            // 
            // pnlTask
            // 
            this.pnlTask.AutoScroll = true;
            this.pnlTask.Controls.Add(this.picTask);
            this.pnlTask.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlTask.Location = new System.Drawing.Point(155, 85);
            this.pnlTask.Margin = new System.Windows.Forms.Padding(0);
            this.pnlTask.Name = "pnlTask";
            this.pnlTask.Size = new System.Drawing.Size(852, 398);
            this.pnlTask.TabIndex = 0;
            this.pnlTask.Scroll += new System.Windows.Forms.ScrollEventHandler(this.pnlTask_Scroll);
            this.pnlTask.Paint += new System.Windows.Forms.PaintEventHandler(this.pnlTask_Paint);
            // 
            // picTask
            // 
            this.picTask.BackColor = System.Drawing.SystemColors.Window;
            this.picTask.Location = new System.Drawing.Point(0, 0);
            this.picTask.Margin = new System.Windows.Forms.Padding(0);
            this.picTask.Name = "picTask";
            this.picTask.Size = new System.Drawing.Size(100, 100);
            this.picTask.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.picTask.TabIndex = 10;
            this.picTask.TabStop = false;
            this.picTask.Paint += new System.Windows.Forms.PaintEventHandler(this.picTask_Paint);
            // 
            // pnlDate
            // 
            this.pnlDate.Controls.Add(this.picDate);
            this.pnlDate.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlDate.Location = new System.Drawing.Point(155, 0);
            this.pnlDate.Margin = new System.Windows.Forms.Padding(0);
            this.pnlDate.Name = "pnlDate";
            this.pnlDate.Size = new System.Drawing.Size(852, 85);
            this.pnlDate.TabIndex = 11;
            // 
            // picDate
            // 
            this.picDate.BackColor = System.Drawing.SystemColors.Window;
            this.picDate.Location = new System.Drawing.Point(0, 0);
            this.picDate.Margin = new System.Windows.Forms.Padding(0);
            this.picDate.Name = "picDate";
            this.picDate.Size = new System.Drawing.Size(100, 50);
            this.picDate.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.picDate.TabIndex = 12;
            this.picDate.TabStop = false;
            this.picDate.Paint += new System.Windows.Forms.PaintEventHandler(this.picDate_Paint);
            // 
            // pnlSection
            // 
            this.pnlSection.Controls.Add(this.picSection);
            this.pnlSection.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlSection.Location = new System.Drawing.Point(5, 85);
            this.pnlSection.Margin = new System.Windows.Forms.Padding(0);
            this.pnlSection.Name = "pnlSection";
            this.pnlSection.Size = new System.Drawing.Size(150, 398);
            this.pnlSection.TabIndex = 12;
            // 
            // picSection
            // 
            this.picSection.BackColor = System.Drawing.SystemColors.Window;
            this.picSection.Location = new System.Drawing.Point(0, 0);
            this.picSection.Margin = new System.Windows.Forms.Padding(0);
            this.picSection.Name = "picSection";
            this.picSection.Size = new System.Drawing.Size(100, 100);
            this.picSection.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.picSection.TabIndex = 8;
            this.picSection.TabStop = false;
            this.picSection.Paint += new System.Windows.Forms.PaintEventHandler(this.picSection_Paint);
            // 
            // tbpEditor
            // 
            this.tbpEditor.BackColor = System.Drawing.SystemColors.Window;
            this.tbpEditor.Controls.Add(this.ltvData);
            this.tbpEditor.Controls.Add(this.tspEditorToolbar);
            this.tbpEditor.Location = new System.Drawing.Point(4, 26);
            this.tbpEditor.Name = "tbpEditor";
            this.tbpEditor.Padding = new System.Windows.Forms.Padding(3);
            this.tbpEditor.Size = new System.Drawing.Size(1013, 489);
            this.tbpEditor.TabIndex = 0;
            this.tbpEditor.Text = "エディター";
            // 
            // ltvData
            // 
            this.ltvData.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader8,
            this.colProgress,
            this.columnHeader2});
            this.ltvData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ltvData.FullRowSelect = true;
            this.ltvData.GridLines = true;
            this.ltvData.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.ltvData.Location = new System.Drawing.Point(3, 28);
            this.ltvData.Name = "ltvData";
            this.ltvData.ShowItemToolTips = true;
            this.ltvData.Size = new System.Drawing.Size(1007, 458);
            this.ltvData.TabIndex = 1;
            this.ltvData.UseCompatibleStateImageBehavior = false;
            this.ltvData.View = System.Windows.Forms.View.Details;
            this.ltvData.SelectedIndexChanged += new System.EventHandler(this.ltvData_SelectedIndexChanged);
            this.ltvData.DoubleClick += new System.EventHandler(this.ltvData_DoubleClick);
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "部門";
            this.columnHeader4.Width = 180;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "担当者";
            this.columnHeader5.Width = 140;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "作業内容";
            this.columnHeader6.Width = 250;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "開始日/依存先";
            this.columnHeader7.Width = 120;
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "期間";
            this.columnHeader8.Width = 80;
            // 
            // colProgress
            // 
            this.colProgress.Text = "進捗率";
            this.colProgress.Width = 80;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "表示色";
            // 
            // tspEditorToolbar
            // 
            this.tspEditorToolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolSave,
            this.tolSaveImage,
            this.toolStripSeparator1,
            this.tolSection,
            this.tolAddTask,
            this.tolDeleteTask,
            this.toolStripSeparator2,
            this.toolStripLabel1,
            this.tolSortType});
            this.tspEditorToolbar.Location = new System.Drawing.Point(3, 3);
            this.tspEditorToolbar.Name = "tspEditorToolbar";
            this.tspEditorToolbar.Size = new System.Drawing.Size(1007, 25);
            this.tspEditorToolbar.TabIndex = 0;
            this.tspEditorToolbar.Text = "toolStrip1";
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
            // tolSaveImage
            // 
            this.tolSaveImage.Image = ((System.Drawing.Image)(resources.GetObject("tolSaveImage.Image")));
            this.tolSaveImage.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolSaveImage.Name = "tolSaveImage";
            this.tolSaveImage.Size = new System.Drawing.Size(121, 22);
            this.tolSaveImage.Text = "描画イメージを保存";
            this.tolSaveImage.Click += new System.EventHandler(this.tolSaveImage_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // tolSection
            // 
            this.tolSection.Image = ((System.Drawing.Image)(resources.GetObject("tolSection.Image")));
            this.tolSection.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolSection.Name = "tolSection";
            this.tolSection.Size = new System.Drawing.Size(104, 22);
            this.tolSection.Text = "セクションの編集";
            this.tolSection.Click += new System.EventHandler(this.tolSection_Click);
            // 
            // tolAddTask
            // 
            this.tolAddTask.Image = ((System.Drawing.Image)(resources.GetObject("tolAddTask.Image")));
            this.tolAddTask.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolAddTask.Name = "tolAddTask";
            this.tolAddTask.Size = new System.Drawing.Size(86, 22);
            this.tolAddTask.Text = "タスクの追加";
            this.tolAddTask.Click += new System.EventHandler(this.tolAddTask_Click);
            // 
            // tolDeleteTask
            // 
            this.tolDeleteTask.Enabled = false;
            this.tolDeleteTask.Image = ((System.Drawing.Image)(resources.GetObject("tolDeleteTask.Image")));
            this.tolDeleteTask.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDeleteTask.Name = "tolDeleteTask";
            this.tolDeleteTask.Size = new System.Drawing.Size(86, 22);
            this.tolDeleteTask.Text = "タスクの削除";
            this.tolDeleteTask.Click += new System.EventHandler(this.tolDeleteTask_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(36, 22);
            this.toolStripLabel1.Text = "整列:";
            // 
            // tolSortType
            // 
            this.tolSortType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tolSortType.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.tolSortType.Items.AddRange(new object[] {
            "部門->担当者->開始日順",
            "開始日順"});
            this.tolSortType.Name = "tolSortType";
            this.tolSortType.Size = new System.Drawing.Size(200, 25);
            this.tolSortType.SelectedIndexChanged += new System.EventHandler(this.tolSortType_SelectedIndexChanged);
            // 
            // dlgSave
            // 
            this.dlgSave.DefaultExt = "jpg";
            this.dlgSave.Filter = "JPG ファイル (*.jpg)|*.jpg";
            this.dlgSave.Title = "イメージの保存";
            // 
            // ctlScheduleEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tbcScheduleTabs);
            this.Name = "ctlScheduleEditor";
            this.Size = new System.Drawing.Size(1021, 519);
            this.tbcScheduleTabs.ResumeLayout(false);
            this.tbpViewer.ResumeLayout(false);
            this.tblViewerPart.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picTitle)).EndInit();
            this.pnlTask.ResumeLayout(false);
            this.pnlTask.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picTask)).EndInit();
            this.pnlDate.ResumeLayout(false);
            this.pnlDate.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picDate)).EndInit();
            this.pnlSection.ResumeLayout(false);
            this.pnlSection.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picSection)).EndInit();
            this.tbpEditor.ResumeLayout(false);
            this.tbpEditor.PerformLayout();
            this.tspEditorToolbar.ResumeLayout(false);
            this.tspEditorToolbar.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader7;
		private System.Windows.Forms.ColumnHeader columnHeader8;
		private System.Windows.Forms.ToolStrip tspEditorToolbar;
		private System.Windows.Forms.ToolStripButton tolSave;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tolSection;
		private System.Windows.Forms.ToolStripButton tolAddTask;
		private System.Windows.Forms.ToolStripButton tolDeleteTask;
		private System.Windows.Forms.ColumnHeader colProgress;
		private System.Windows.Forms.ListView ltvData;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ToolStripButton tolSaveImage;
		private System.Windows.Forms.SaveFileDialog dlgSave;
		private System.Windows.Forms.TableLayoutPanel tblViewerPart;
		private System.Windows.Forms.Panel pnlTask;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.Panel pnlDate;
		private System.Windows.Forms.Panel pnlSection;
		private System.Windows.Forms.TabPage tbpViewer;
		private System.Windows.Forms.TabPage tbpEditor;
		private System.Windows.Forms.TabControl tbcScheduleTabs;
		private System.Windows.Forms.PictureBox picTask;
		private System.Windows.Forms.ToolStripComboBox tolSortType;
		private System.Windows.Forms.PictureBox picTitle;
		private System.Windows.Forms.PictureBox picDate;
		private System.Windows.Forms.PictureBox picSection;
	}
}
