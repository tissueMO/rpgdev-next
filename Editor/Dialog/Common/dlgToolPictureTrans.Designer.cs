namespace Editor.Dialog.Common {
	partial class dlgToolPictureTrans {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(dlgToolPictureTrans));
			this.txtFileName = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.uctlFileTree = new Editor.CtrlComponent.Common.ctlFileTree();
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.panel1 = new System.Windows.Forms.Panel();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.tolNewFolder = new System.Windows.Forms.ToolStripButton();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.pnlTransColor = new System.Windows.Forms.Panel();
			this.panel3 = new System.Windows.Forms.Panel();
			this.picPreview = new System.Windows.Forms.PictureBox();
			this.dlgColor = new System.Windows.Forms.ColorDialog();
			this.panel1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.panel3.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.picPreview)).BeginInit();
			this.SuspendLayout();
			// 
			// txtFileName
			// 
			this.txtFileName.Location = new System.Drawing.Point(428, 383);
			this.txtFileName.MaxLength = 256;
			this.txtFileName.Name = "txtFileName";
			this.txtFileName.Size = new System.Drawing.Size(185, 23);
			this.txtFileName.TabIndex = 5;
			this.txtFileName.Text = ".png";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(417, 363);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(94, 15);
			this.label6.TabIndex = 4;
			this.label6.Text = "保存先ファイル名:";
			// 
			// uctlFileTree
			// 
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
			this.uctlFileTree.Size = new System.Drawing.Size(295, 397);
			this.uctlFileTree.TabIndex = 1;
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(633, 380);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 6;
			this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.uctlFileTree);
			this.panel1.Controls.Add(this.toolStrip1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(295, 422);
			this.panel1.TabIndex = 0;
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
			this.toolStripLabel1.Size = new System.Drawing.Size(88, 22);
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
			this.tolNewFolder.Size = new System.Drawing.Size(103, 22);
			this.tolNewFolder.Text = "新しいフォルダー";
			this.tolNewFolder.Click += new System.EventHandler(this.tolNewFolder_Click);
			// 
			// label2
			// 
			this.label2.Dock = System.Windows.Forms.DockStyle.Top;
			this.label2.Location = new System.Drawing.Point(295, 0);
			this.label2.Name = "label2";
			this.label2.Padding = new System.Windows.Forms.Padding(10, 10, 0, 0);
			this.label2.Size = new System.Drawing.Size(558, 30);
			this.label2.TabIndex = 0;
			this.label2.Text = "画像上でクリックしたピクセルの色が透明色としてセットされます。";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(307, 376);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(48, 15);
			this.label3.TabIndex = 2;
			this.label3.Text = "透明色:";
			// 
			// pnlTransColor
			// 
			this.pnlTransColor.BackColor = System.Drawing.Color.White;
			this.pnlTransColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pnlTransColor.Cursor = System.Windows.Forms.Cursors.Hand;
			this.pnlTransColor.Location = new System.Drawing.Point(358, 363);
			this.pnlTransColor.Name = "pnlTransColor";
			this.pnlTransColor.Size = new System.Drawing.Size(45, 45);
			this.pnlTransColor.TabIndex = 3;
			this.pnlTransColor.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pnlTransColor_MouseClick);
			// 
			// panel3
			// 
			this.panel3.AutoScroll = true;
			this.panel3.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel3.Controls.Add(this.picPreview);
			this.panel3.Location = new System.Drawing.Point(310, 30);
			this.panel3.Name = "panel3";
			this.panel3.Size = new System.Drawing.Size(530, 320);
			this.panel3.TabIndex = 1;
			// 
			// picPreview
			// 
			this.picPreview.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.picPreview.Location = new System.Drawing.Point(0, 0);
			this.picPreview.Name = "picPreview";
			this.picPreview.Size = new System.Drawing.Size(100, 100);
			this.picPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picPreview.TabIndex = 13;
			this.picPreview.TabStop = false;
			this.picPreview.MouseClick += new System.Windows.Forms.MouseEventHandler(this.picPreview_MouseClick);
			// 
			// dlgColor
			// 
			this.dlgColor.FullOpen = true;
			// 
			// dlgToolPictureTrans
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(853, 422);
			this.Controls.Add(this.panel3);
			this.Controls.Add(this.pnlTransColor);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.txtFileName);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgToolPictureTrans";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "透過PNGを作成";
			this.Load += new System.EventHandler(this.dlgToolPictureTrans_Load);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.panel3.ResumeLayout(false);
			this.panel3.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.picPreview)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.Label label6;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Panel pnlTransColor;
		private System.Windows.Forms.Panel panel3;
		private System.Windows.Forms.PictureBox picPreview;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tolNewFolder;
		private System.Windows.Forms.ColorDialog dlgColor;
		private System.Windows.Forms.TextBox txtFileName;
		private CtrlComponent.Common.ctlFileTree uctlFileTree;
	}
}