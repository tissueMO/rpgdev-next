namespace Editor.Dialog.Common {
	partial class dlgToolPictureResize {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(dlgToolPictureResize));
			this.panel1 = new System.Windows.Forms.Panel();
			this.uctlFileTree = new Editor.CtrlComponent.Common.ctlFileTree();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.tolNewFolder = new System.Windows.Forms.ToolStripButton();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.numAfterHeight = new System.Windows.Forms.NumericUpDown();
			this.numBeforeHeight = new System.Windows.Forms.NumericUpDown();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.numBeforeWidth = new System.Windows.Forms.NumericUpDown();
			this.numAfterWidth = new System.Windows.Forms.NumericUpDown();
			this.chkKeepAspect = new System.Windows.Forms.CheckBox();
			this.label6 = new System.Windows.Forms.Label();
			this.txtFileName = new System.Windows.Forms.TextBox();
			this.picPreview = new System.Windows.Forms.PictureBox();
			this.label1 = new System.Windows.Forms.Label();
			this.cmbInterpolation = new System.Windows.Forms.ComboBox();
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.panel1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.tableLayoutPanel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numAfterHeight)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numBeforeHeight)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numBeforeWidth)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numAfterWidth)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.picPreview)).BeginInit();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.uctlFileTree);
			this.panel1.Controls.Add(this.toolStrip1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(295, 461);
			this.panel1.TabIndex = 0;
			// 
			// uctlFileTree
			// 
			this.uctlFileTree.AutoReload = true;
			this.uctlFileTree.CanDelete = false;
			this.uctlFileTree.CanRename = true;
			this.uctlFileTree.DefaultExnpandDepth = 2;
			this.uctlFileTree.Dock = System.Windows.Forms.DockStyle.Fill;
			this.uctlFileTree.FileFilter = "";
			this.uctlFileTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlFileTree.IncludeFiles = false;
			this.uctlFileTree.Location = new System.Drawing.Point(0, 25);
			this.uctlFileTree.Margin = new System.Windows.Forms.Padding(3, 9, 3, 9);
			this.uctlFileTree.Name = "uctlFileTree";
			this.uctlFileTree.RootPath = "";
			this.uctlFileTree.Size = new System.Drawing.Size(295, 436);
			this.uctlFileTree.TabIndex = 1;
			this.uctlFileTree.VisibleCheckBoxes = false;
			// 
			// toolStrip1
			// 
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1,
            this.toolStripSeparator1,
            this.tolNewFolder});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(295, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// toolStripLabel1
			// 
			this.toolStripLabel1.Name = "toolStripLabel1";
			this.toolStripLabel1.Size = new System.Drawing.Size(85, 22);
			this.toolStripLabel1.Text = "保存先フォルダー";
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// tolNewFolder
			// 
			this.tolNewFolder.Image = ((System.Drawing.Image)(resources.GetObject("tolNewFolder.Image")));
			this.tolNewFolder.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolNewFolder.Name = "tolNewFolder";
			this.tolNewFolder.Size = new System.Drawing.Size(99, 22);
			this.tolNewFolder.Text = "新しいフォルダー";
			this.tolNewFolder.Click += new System.EventHandler(this.tolNewFolder_Click);
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 4;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Controls.Add(this.numAfterHeight, 3, 1);
			this.tableLayoutPanel1.Controls.Add(this.numBeforeHeight, 3, 0);
			this.tableLayoutPanel1.Controls.Add(this.label2, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.label3, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.label4, 2, 0);
			this.tableLayoutPanel1.Controls.Add(this.label5, 2, 1);
			this.tableLayoutPanel1.Controls.Add(this.numBeforeWidth, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.numAfterWidth, 1, 1);
			this.tableLayoutPanel1.Location = new System.Drawing.Point(319, 378);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 2;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(235, 60);
			this.tableLayoutPanel1.TabIndex = 1;
			// 
			// numAfterHeight
			// 
			this.numAfterHeight.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.numAfterHeight.Location = new System.Drawing.Point(158, 33);
			this.numAfterHeight.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
			this.numAfterHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numAfterHeight.Name = "numAfterHeight";
			this.numAfterHeight.Size = new System.Drawing.Size(74, 23);
			this.numAfterHeight.TabIndex = 7;
			this.numAfterHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numAfterHeight.ValueChanged += new System.EventHandler(this.numAfterWidth_ValueChanged);
			// 
			// numBeforeHeight
			// 
			this.numBeforeHeight.Anchor = System.Windows.Forms.AnchorStyles.Left;
			this.numBeforeHeight.Enabled = false;
			this.numBeforeHeight.Location = new System.Drawing.Point(158, 3);
			this.numBeforeHeight.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
			this.numBeforeHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numBeforeHeight.Name = "numBeforeHeight";
			this.numBeforeHeight.Size = new System.Drawing.Size(74, 23);
			this.numBeforeHeight.TabIndex = 3;
			this.numBeforeHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// label2
			// 
			this.label2.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(4, 7);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(48, 15);
			this.label2.TabIndex = 0;
			this.label2.Text = "元画像:";
			// 
			// label3
			// 
			this.label3.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(4, 37);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(48, 15);
			this.label3.TabIndex = 4;
			this.label3.Text = "加工後:";
			// 
			// label4
			// 
			this.label4.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(138, 7);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(14, 15);
			this.label4.TabIndex = 2;
			this.label4.Text = "x";
			// 
			// label5
			// 
			this.label5.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(138, 37);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(14, 15);
			this.label5.TabIndex = 6;
			this.label5.Text = "x";
			// 
			// numBeforeWidth
			// 
			this.numBeforeWidth.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.numBeforeWidth.Enabled = false;
			this.numBeforeWidth.Location = new System.Drawing.Point(58, 3);
			this.numBeforeWidth.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
			this.numBeforeWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numBeforeWidth.Name = "numBeforeWidth";
			this.numBeforeWidth.Size = new System.Drawing.Size(74, 23);
			this.numBeforeWidth.TabIndex = 1;
			this.numBeforeWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// numAfterWidth
			// 
			this.numAfterWidth.Anchor = System.Windows.Forms.AnchorStyles.Right;
			this.numAfterWidth.Location = new System.Drawing.Point(58, 33);
			this.numAfterWidth.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
			this.numAfterWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numAfterWidth.Name = "numAfterWidth";
			this.numAfterWidth.Size = new System.Drawing.Size(74, 23);
			this.numAfterWidth.TabIndex = 5;
			this.numAfterWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numAfterWidth.ValueChanged += new System.EventHandler(this.numAfterHeight_ValueChanged);
			// 
			// chkKeepAspect
			// 
			this.chkKeepAspect.AutoSize = true;
			this.chkKeepAspect.Location = new System.Drawing.Point(316, 354);
			this.chkKeepAspect.Name = "chkKeepAspect";
			this.chkKeepAspect.Size = new System.Drawing.Size(112, 19);
			this.chkKeepAspect.TabIndex = 0;
			this.chkKeepAspect.Text = "アスペクト比を保つ";
			this.chkKeepAspect.UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(577, 386);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(58, 15);
			this.label6.TabIndex = 4;
			this.label6.Text = "ファイル名:";
			// 
			// txtFileName
			// 
			this.txtFileName.Location = new System.Drawing.Point(639, 382);
			this.txtFileName.MaxLength = 256;
			this.txtFileName.Name = "txtFileName";
			this.txtFileName.Size = new System.Drawing.Size(174, 23);
			this.txtFileName.TabIndex = 5;
			// 
			// picPreview
			// 
			this.picPreview.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.picPreview.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.picPreview.Location = new System.Drawing.Point(305, 10);
			this.picPreview.Name = "picPreview";
			this.picPreview.Size = new System.Drawing.Size(530, 325);
			this.picPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.picPreview.TabIndex = 5;
			this.picPreview.TabStop = false;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(577, 358);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(60, 15);
			this.label1.TabIndex = 2;
			this.label1.Text = "補間方法:";
			// 
			// cmbInterpolation
			// 
			this.cmbInterpolation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbInterpolation.FormattingEnabled = true;
			this.cmbInterpolation.Items.AddRange(new object[] {
            "最近傍補間",
            "双一次補間",
            "双三次補間"});
			this.cmbInterpolation.Location = new System.Drawing.Point(639, 354);
			this.cmbInterpolation.Name = "cmbInterpolation";
			this.cmbInterpolation.Size = new System.Drawing.Size(174, 23);
			this.cmbInterpolation.TabIndex = 3;
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.uctlOKCancel.DisenabledAcceptButton = false;
			this.uctlOKCancel.DisenabledCancelButton = false;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(623, 420);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.OKTitle = "OK";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 6;
			this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// dlgToolPictureResize
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(845, 461);
			this.Controls.Add(this.cmbInterpolation);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.txtFileName);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.chkKeepAspect);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.picPreview);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgToolPictureResize";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "画像のリサイズ";
			this.Load += new System.EventHandler(this.dlgToolPictureResize_Load);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numAfterHeight)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numBeforeHeight)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numBeforeWidth)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numAfterWidth)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.picPreview)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.NumericUpDown numAfterHeight;
		private System.Windows.Forms.NumericUpDown numBeforeHeight;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.NumericUpDown numBeforeWidth;
		private System.Windows.Forms.NumericUpDown numAfterWidth;
		private System.Windows.Forms.CheckBox chkKeepAspect;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.PictureBox picPreview;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tolNewFolder;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox cmbInterpolation;
		private System.Windows.Forms.TextBox txtFileName;
		private CtrlComponent.Common.ctlFileTree uctlFileTree;
	}
}