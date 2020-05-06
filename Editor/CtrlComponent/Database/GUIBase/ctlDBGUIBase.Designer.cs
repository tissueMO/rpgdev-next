namespace Editor.CtrlComponent.Database {
	partial class ctlDBGUIBase {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlDBGUIBase));
			this.splMain1 = new System.Windows.Forms.SplitContainer();
			this.ltvDB = new System.Windows.Forms.ListView();
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ColumnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.tosFileToolBar = new System.Windows.Forms.ToolStrip();
			this.tolRowNew = new System.Windows.Forms.ToolStripButton();
			this.tolRowCopy = new System.Windows.Forms.ToolStripButton();
			this.tolRowDelete = new System.Windows.Forms.ToolStripButton();
			this.pnlEditor = new System.Windows.Forms.Panel();
			this.rmnList = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.tolCopyFixedID = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.splMain1)).BeginInit();
			this.splMain1.Panel1.SuspendLayout();
			this.splMain1.Panel2.SuspendLayout();
			this.splMain1.SuspendLayout();
			this.tosFileToolBar.SuspendLayout();
			this.rmnList.SuspendLayout();
			this.SuspendLayout();
			// 
			// splMain1
			// 
			this.splMain1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.splMain1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splMain1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.splMain1.IsSplitterFixed = true;
			this.splMain1.Location = new System.Drawing.Point(0, 0);
			this.splMain1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.splMain1.Name = "splMain1";
			// 
			// splMain1.Panel1
			// 
			this.splMain1.Panel1.Controls.Add(this.ltvDB);
			this.splMain1.Panel1.Controls.Add(this.tosFileToolBar);
			// 
			// splMain1.Panel2
			// 
			this.splMain1.Panel2.AutoScroll = true;
			this.splMain1.Panel2.AutoScrollMinSize = new System.Drawing.Size(900, 0);
			this.splMain1.Panel2.Controls.Add(this.pnlEditor);
			this.splMain1.Size = new System.Drawing.Size(1077, 613);
			this.splMain1.SplitterDistance = 200;
			this.splMain1.SplitterWidth = 5;
			this.splMain1.TabIndex = 3;
			// 
			// ltvDB
			// 
			this.ltvDB.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.ColumnHeader2,
            this.ColumnHeader1});
			this.ltvDB.ContextMenuStrip = this.rmnList;
			this.ltvDB.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ltvDB.ForeColor = System.Drawing.Color.Black;
			this.ltvDB.FullRowSelect = true;
			this.ltvDB.GridLines = true;
			this.ltvDB.HideSelection = false;
			this.ltvDB.Location = new System.Drawing.Point(0, 25);
			this.ltvDB.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ltvDB.MultiSelect = false;
			this.ltvDB.Name = "ltvDB";
			this.ltvDB.ShowItemToolTips = true;
			this.ltvDB.Size = new System.Drawing.Size(198, 586);
			this.ltvDB.TabIndex = 1;
			this.ltvDB.UseCompatibleStateImageBehavior = false;
			this.ltvDB.View = System.Windows.Forms.View.Details;
			this.ltvDB.SelectedIndexChanged += new System.EventHandler(this.ltvDB_SelectedIndexChanged);
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "FixedID";
			this.columnHeader5.Width = 0;
			// 
			// ColumnHeader2
			// 
			this.ColumnHeader2.Text = "ID";
			this.ColumnHeader2.Width = 50;
			// 
			// ColumnHeader1
			// 
			this.ColumnHeader1.Text = "名前";
			this.ColumnHeader1.Width = 120;
			// 
			// tosFileToolBar
			// 
			this.tosFileToolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolRowNew,
            this.tolRowCopy,
            this.tolRowDelete});
			this.tosFileToolBar.Location = new System.Drawing.Point(0, 0);
			this.tosFileToolBar.Name = "tosFileToolBar";
			this.tosFileToolBar.Size = new System.Drawing.Size(198, 25);
			this.tosFileToolBar.TabIndex = 2;
			this.tosFileToolBar.Text = "ToolStrip1";
			// 
			// tolRowNew
			// 
			this.tolRowNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolRowNew.Image = ((System.Drawing.Image)(resources.GetObject("tolRowNew.Image")));
			this.tolRowNew.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolRowNew.Name = "tolRowNew";
			this.tolRowNew.Size = new System.Drawing.Size(23, 22);
			this.tolRowNew.Text = "追加";
			this.tolRowNew.ToolTipText = "項目を追加";
			this.tolRowNew.Click += new System.EventHandler(this.tolRowNew_Click);
			// 
			// tolRowCopy
			// 
			this.tolRowCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolRowCopy.Enabled = false;
			this.tolRowCopy.Image = ((System.Drawing.Image)(resources.GetObject("tolRowCopy.Image")));
			this.tolRowCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolRowCopy.Name = "tolRowCopy";
			this.tolRowCopy.Size = new System.Drawing.Size(23, 22);
			this.tolRowCopy.Text = "コピー";
			this.tolRowCopy.ToolTipText = "項目をコピー";
			this.tolRowCopy.Click += new System.EventHandler(this.tolRowCopy_Click);
			// 
			// tolRowDelete
			// 
			this.tolRowDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolRowDelete.Enabled = false;
			this.tolRowDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolRowDelete.Image")));
			this.tolRowDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolRowDelete.Name = "tolRowDelete";
			this.tolRowDelete.Size = new System.Drawing.Size(23, 22);
			this.tolRowDelete.Text = "削除";
			this.tolRowDelete.ToolTipText = "項目を削除";
			this.tolRowDelete.Click += new System.EventHandler(this.tolRowDelete_Click);
			// 
			// pnlEditor
			// 
			this.pnlEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlEditor.Location = new System.Drawing.Point(0, 0);
			this.pnlEditor.Margin = new System.Windows.Forms.Padding(0);
			this.pnlEditor.Name = "pnlEditor";
			this.pnlEditor.Size = new System.Drawing.Size(900, 594);
			this.pnlEditor.TabIndex = 3;
			this.pnlEditor.Visible = false;
			// 
			// rmnList
			// 
			this.rmnList.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolCopyFixedID});
			this.rmnList.Name = "rmnList";
			this.rmnList.Size = new System.Drawing.Size(174, 48);
			// 
			// tolCopyFixedID
			// 
			this.tolCopyFixedID.Image = ((System.Drawing.Image)(resources.GetObject("tolCopyFixedID.Image")));
			this.tolCopyFixedID.Name = "tolCopyFixedID";
			this.tolCopyFixedID.Size = new System.Drawing.Size(173, 22);
			this.tolCopyFixedID.Text = "FixedIDをコピー(&C)";
			this.tolCopyFixedID.Click += new System.EventHandler(this.tolCopyFixedID_Click);
			// 
			// ctlDBGUIBase
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.splMain1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlDBGUIBase";
			this.Size = new System.Drawing.Size(1077, 613);
			this.splMain1.Panel1.ResumeLayout(false);
			this.splMain1.Panel1.PerformLayout();
			this.splMain1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splMain1)).EndInit();
			this.splMain1.ResumeLayout(false);
			this.tosFileToolBar.ResumeLayout(false);
			this.tosFileToolBar.PerformLayout();
			this.rmnList.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		protected System.Windows.Forms.SplitContainer splMain1;
		protected System.Windows.Forms.ListView ltvDB;
		protected System.Windows.Forms.ColumnHeader columnHeader5;
		protected System.Windows.Forms.ColumnHeader ColumnHeader2;
		protected System.Windows.Forms.ColumnHeader ColumnHeader1;
		protected System.Windows.Forms.Panel pnlEditor;
		internal System.Windows.Forms.ToolStripButton tolRowCopy;
		internal System.Windows.Forms.ToolStripButton tolRowDelete;
		internal System.Windows.Forms.ToolStripButton tolRowNew;
		protected System.Windows.Forms.ToolStrip tosFileToolBar;
		private System.Windows.Forms.ContextMenuStrip rmnList;
		private System.Windows.Forms.ToolStripMenuItem tolCopyFixedID;
	}
}
