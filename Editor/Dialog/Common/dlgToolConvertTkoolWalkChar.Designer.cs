namespace Editor.Dialog.Common {
	partial class dlgToolConvertTkoolWalkChar {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(dlgToolConvertTkoolWalkChar));
			this.cmbInterpolation = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.txtDestFileName = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.tolNewFolder = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
			this.uctlFileTree = new Editor.CtrlComponent.Common.ctlFileTree();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.panel1 = new System.Windows.Forms.Panel();
			this.uctlCharSize = new Editor.CtrlComponent.Common.ctlPosition();
			this.label1 = new System.Windows.Forms.Label();
			this.cmbTkoolVersion = new System.Windows.Forms.ComboBox();
			this.toolStrip1.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// cmbInterpolation
			// 
			this.cmbInterpolation.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbInterpolation.FormattingEnabled = true;
			this.cmbInterpolation.Items.AddRange(new object[] {
            "最近傍補間",
            "双一次補間",
            "双三次補間"});
			this.cmbInterpolation.Location = new System.Drawing.Point(406, 145);
			this.cmbInterpolation.Name = "cmbInterpolation";
			this.cmbInterpolation.Size = new System.Drawing.Size(161, 23);
			this.cmbInterpolation.TabIndex = 7;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(308, 149);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(60, 15);
			this.label5.TabIndex = 6;
			this.label5.Text = "補間方法:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(308, 121);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(93, 15);
			this.label4.TabIndex = 4;
			this.label4.Text = "キャラクターサイズ:";
			// 
			// txtDestFileName
			// 
			this.txtDestFileName.Location = new System.Drawing.Point(406, 89);
			this.txtDestFileName.Name = "txtDestFileName";
			this.txtDestFileName.Size = new System.Drawing.Size(161, 23);
			this.txtDestFileName.TabIndex = 3;
			this.txtDestFileName.Text = ".png";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(308, 93);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(82, 15);
			this.label3.TabIndex = 2;
			this.label3.Text = "出力ファイル名:";
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
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// toolStripLabel1
			// 
			this.toolStripLabel1.Name = "toolStripLabel1";
			this.toolStripLabel1.Size = new System.Drawing.Size(85, 22);
			this.toolStripLabel1.Text = "保存先フォルダー";
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
			this.uctlFileTree.Size = new System.Drawing.Size(295, 260);
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
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.uctlOKCancel.DisenabledAcceptButton = false;
			this.uctlOKCancel.DisenabledCancelButton = false;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(363, 243);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.OKTitle = "OK";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 8;
			this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.uctlFileTree);
			this.panel1.Controls.Add(this.toolStrip1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Left;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(295, 285);
			this.panel1.TabIndex = 0;
			// 
			// uctlCharSize
			// 
			this.uctlCharSize.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlCharSize.Location = new System.Drawing.Point(406, 117);
			this.uctlCharSize.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlCharSize.Maximum = new System.Drawing.Point(9999, 9999);
			this.uctlCharSize.MaximumSize = new System.Drawing.Size(500, 23);
			this.uctlCharSize.Minimum = new System.Drawing.Point(1, 1);
			this.uctlCharSize.MinimumSize = new System.Drawing.Size(100, 23);
			this.uctlCharSize.Name = "uctlCharSize";
			this.uctlCharSize.Result = new System.Drawing.Point(32, 64);
			this.uctlCharSize.Size = new System.Drawing.Size(161, 23);
			this.uctlCharSize.TabIndex = 5;
			this.uctlCharSize.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Size;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(308, 65);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(81, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "想定バージョン:";
			// 
			// cmbTkoolVersion
			// 
			this.cmbTkoolVersion.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbTkoolVersion.FormattingEnabled = true;
			this.cmbTkoolVersion.Items.AddRange(new object[] {
            "RPGツクール2000/2003",
            "RPGツクールVX/MV"});
			this.cmbTkoolVersion.Location = new System.Drawing.Point(406, 61);
			this.cmbTkoolVersion.Name = "cmbTkoolVersion";
			this.cmbTkoolVersion.Size = new System.Drawing.Size(161, 23);
			this.cmbTkoolVersion.TabIndex = 1;
			// 
			// dlgToolConvertTkoolWalkChar
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(584, 285);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.cmbTkoolVersion);
			this.Controls.Add(this.uctlCharSize);
			this.Controls.Add(this.cmbInterpolation);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.txtDestFileName);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgToolConvertTkoolWalkChar";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "RPGツクールの歩行グラフィックを変換";
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ComboBox cmbInterpolation;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TextBox txtDestFileName;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ToolStripButton tolNewFolder;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private CtrlComponent.Common.ctlFileTree uctlFileTree;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.Panel panel1;
		private CtrlComponent.Common.ctlPosition uctlCharSize;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox cmbTkoolVersion;
	}
}