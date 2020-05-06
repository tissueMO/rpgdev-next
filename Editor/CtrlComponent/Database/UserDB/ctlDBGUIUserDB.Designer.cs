namespace Editor.CtrlComponent.Database {
	partial class ctlDBGUIUserDB {
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
			this.UDB = new System.Windows.Forms.DataGridView();
			this.tlsUDB = new System.Windows.Forms.ToolStrip();
			this.ToolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.tolID = new System.Windows.Forms.ToolStripTextBox();
			this.ToolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
			this.tolName = new System.Windows.Forms.ToolStripTextBox();
			this.toolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
			this.tolMemo = new System.Windows.Forms.ToolStripTextBox();
			((System.ComponentModel.ISupportInitialize)(this.splMain1)).BeginInit();
			this.splMain1.Panel1.SuspendLayout();
			this.splMain1.Panel2.SuspendLayout();
			this.splMain1.SuspendLayout();
			this.pnlEditor.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.UDB)).BeginInit();
			this.tlsUDB.SuspendLayout();
			this.SuspendLayout();
			// 
			// splMain1
			// 
			// 
			// splMain1.Panel2
			// 
			this.splMain1.Panel2.AutoScrollMinSize = new System.Drawing.Size(0, 0);
			// 
			// ltvDB
			// 
			this.ltvDB.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.ltvDB_ColumnClick);
			this.ltvDB.SelectedIndexChanged += new System.EventHandler(this.ltvDB_SelectedIndexChanged);
			// 
			// pnlEditor
			// 
			this.pnlEditor.Controls.Add(this.UDB);
			this.pnlEditor.Controls.Add(this.tlsUDB);
			this.pnlEditor.Size = new System.Drawing.Size(870, 611);
			// 
			// UDB
			// 
			this.UDB.AllowUserToOrderColumns = true;
			this.UDB.AllowUserToResizeRows = false;
			this.UDB.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.UDB.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.UDB.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.UDB.Dock = System.Windows.Forms.DockStyle.Fill;
			this.UDB.Location = new System.Drawing.Point(0, 25);
			this.UDB.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.UDB.Name = "UDB";
			this.UDB.RowHeadersWidth = 40;
			this.UDB.RowTemplate.Height = 21;
			this.UDB.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.UDB.Size = new System.Drawing.Size(870, 586);
			this.UDB.TabIndex = 3;
			this.UDB.ColumnAdded += new System.Windows.Forms.DataGridViewColumnEventHandler(this.UDB_ColumnAdded);
			this.UDB.ColumnDisplayIndexChanged += new System.Windows.Forms.DataGridViewColumnEventHandler(this.UDB_ColumnDisplayIndexChanged);
			this.UDB.ColumnRemoved += new System.Windows.Forms.DataGridViewColumnEventHandler(this.UDB_ColumnRemoved);
			// 
			// tlsUDB
			// 
			this.tlsUDB.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripLabel1,
            this.tolID,
            this.ToolStripLabel2,
            this.tolName,
            this.toolStripLabel3,
            this.tolMemo});
			this.tlsUDB.Location = new System.Drawing.Point(0, 0);
			this.tlsUDB.Name = "tlsUDB";
			this.tlsUDB.Size = new System.Drawing.Size(870, 25);
			this.tlsUDB.TabIndex = 2;
			this.tlsUDB.Text = "ToolStrip2";
			// 
			// ToolStripLabel1
			// 
			this.ToolStripLabel1.Name = "ToolStripLabel1";
			this.ToolStripLabel1.Size = new System.Drawing.Size(26, 22);
			this.ToolStripLabel1.Text = "ID:";
			// 
			// tolID
			// 
			this.tolID.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tolID.Name = "tolID";
			this.tolID.Size = new System.Drawing.Size(60, 25);
			this.tolID.Validating += new System.ComponentModel.CancelEventHandler(this.tolID_Validating);
			// 
			// ToolStripLabel2
			// 
			this.ToolStripLabel2.Name = "ToolStripLabel2";
			this.ToolStripLabel2.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
			this.ToolStripLabel2.Size = new System.Drawing.Size(41, 22);
			this.ToolStripLabel2.Text = "名前:";
			this.ToolStripLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// tolName
			// 
			this.tolName.AutoToolTip = true;
			this.tolName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tolName.Name = "tolName";
			this.tolName.Size = new System.Drawing.Size(150, 25);
			this.tolName.Validating += new System.ComponentModel.CancelEventHandler(this.tolName_Validating);
			// 
			// toolStripLabel3
			// 
			this.toolStripLabel3.Name = "toolStripLabel3";
			this.toolStripLabel3.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
			this.toolStripLabel3.Size = new System.Drawing.Size(34, 22);
			this.toolStripLabel3.Text = "メモ:";
			this.toolStripLabel3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// tolMemo
			// 
			this.tolMemo.AcceptsReturn = true;
			this.tolMemo.AutoToolTip = true;
			this.tolMemo.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.tolMemo.Name = "tolMemo";
			this.tolMemo.Size = new System.Drawing.Size(250, 25);
			this.tolMemo.Validating += new System.ComponentModel.CancelEventHandler(this.tolMemo_Validating);
			// 
			// ctlDBGUIUserDB
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.Margin = new System.Windows.Forms.Padding(3);
			this.Name = "ctlDBGUIUserDB";
			this.RequestRowAdd += new System.EventHandler(this.ctlDBGUIUserDB_RequestRowAdd);
			this.RequestRowCopy += new System.EventHandler(this.ctlDBGUIUserDB_RequestRowCopy);
			this.RequestRowDelete += new System.EventHandler(this.ctlDBGUIUserDB_RequestRowDelete);
			this.splMain1.Panel1.ResumeLayout(false);
			this.splMain1.Panel1.PerformLayout();
			this.splMain1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splMain1)).EndInit();
			this.splMain1.ResumeLayout(false);
			this.pnlEditor.ResumeLayout(false);
			this.pnlEditor.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.UDB)).EndInit();
			this.tlsUDB.ResumeLayout(false);
			this.tlsUDB.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion
		internal System.Windows.Forms.ToolStripLabel ToolStripLabel1;
		internal System.Windows.Forms.ToolStripTextBox tolID;
		private System.Windows.Forms.ToolStripLabel toolStripLabel3;
		private System.Windows.Forms.ToolStripTextBox tolMemo;
		private System.Windows.Forms.ToolStrip tlsUDB;
		internal System.Windows.Forms.DataGridView UDB;
		internal System.Windows.Forms.ToolStripLabel ToolStripLabel2;
		internal System.Windows.Forms.ToolStripTextBox tolName;
	}
}
