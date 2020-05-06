namespace Editor.Dialog.Schedule {
	partial class dlgEditSection {
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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("担当者名", 1, 1);
			System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("部門名", new System.Windows.Forms.TreeNode[] {
            treeNode3});
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(dlgEditSection));
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.trvSection = new System.Windows.Forms.TreeView();
			this.imlNodeIcons = new System.Windows.Forms.ImageList(this.components);
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.tlbToolbar = new System.Windows.Forms.ToolStrip();
			this.tolSection = new System.Windows.Forms.ToolStripButton();
			this.tolWorker = new System.Windows.Forms.ToolStripButton();
			this.tolDelete = new System.Windows.Forms.ToolStripButton();
			this.tolUp = new System.Windows.Forms.ToolStripButton();
			this.tolDown = new System.Windows.Forms.ToolStripButton();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.tableLayoutPanel1.SuspendLayout();
			this.tlbToolbar.SuspendLayout();
			this.SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 1;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Controls.Add(this.trvSection, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.uctlOKCancel, 0, 1);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 25);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 2;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 44F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(331, 390);
			this.tableLayoutPanel1.TabIndex = 1;
			// 
			// trvSection
			// 
			this.trvSection.Dock = System.Windows.Forms.DockStyle.Fill;
			this.trvSection.FullRowSelect = true;
			this.trvSection.HotTracking = true;
			this.trvSection.ImageIndex = 0;
			this.trvSection.ImageList = this.imlNodeIcons;
			this.trvSection.LabelEdit = true;
			this.trvSection.Location = new System.Drawing.Point(3, 3);
			this.trvSection.Name = "trvSection";
			treeNode3.ImageIndex = 1;
			treeNode3.Name = "ノード2";
			treeNode3.SelectedImageIndex = 1;
			treeNode3.Text = "担当者名";
			treeNode4.Name = "ノード0";
			treeNode4.Text = "部門名";
			this.trvSection.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode4});
			this.trvSection.SelectedImageIndex = 0;
			this.trvSection.Size = new System.Drawing.Size(325, 340);
			this.trvSection.StateImageList = this.imlNodeIcons;
			this.trvSection.TabIndex = 0;
			this.toolTip1.SetToolTip(this.trvSection, "選択項目をさらにクリックすると名前を変更できます。");
			this.trvSection.BeforeLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.trvSection_BeforeLabelEdit);
			this.trvSection.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.trvSection_AfterLabelEdit);
			this.trvSection.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.trvSection_AfterSelect);
			// 
			// imlNodeIcons
			// 
			this.imlNodeIcons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imlNodeIcons.ImageStream")));
			this.imlNodeIcons.TransparentColor = System.Drawing.Color.Transparent;
			this.imlNodeIcons.Images.SetKeyName(0, "Class_489.png");
			this.imlNodeIcons.Images.SetKeyName(1, "UserProfilenode_8706.png");
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(59, 351);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 1;
			this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// tlbToolbar
			// 
			this.tlbToolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolSection,
            this.tolWorker,
            this.tolDelete,
            this.tolUp,
            this.tolDown});
			this.tlbToolbar.Location = new System.Drawing.Point(0, 0);
			this.tlbToolbar.Name = "tlbToolbar";
			this.tlbToolbar.Size = new System.Drawing.Size(331, 25);
			this.tlbToolbar.TabIndex = 0;
			this.tlbToolbar.Text = "toolStrip1";
			// 
			// tolSection
			// 
			this.tolSection.Image = ((System.Drawing.Image)(resources.GetObject("tolSection.Image")));
			this.tolSection.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolSection.Name = "tolSection";
			this.tolSection.Size = new System.Drawing.Size(84, 22);
			this.tolSection.Text = "部門を追加";
			this.tolSection.Click += new System.EventHandler(this.tolSection_Click);
			// 
			// tolWorker
			// 
			this.tolWorker.Enabled = false;
			this.tolWorker.Image = ((System.Drawing.Image)(resources.GetObject("tolWorker.Image")));
			this.tolWorker.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolWorker.Name = "tolWorker";
			this.tolWorker.Size = new System.Drawing.Size(96, 22);
			this.tolWorker.Text = "担当者を追加";
			this.tolWorker.Click += new System.EventHandler(this.tolWorker_Click);
			// 
			// tolDelete
			// 
			this.tolDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolDelete.Enabled = false;
			this.tolDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolDelete.Image")));
			this.tolDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolDelete.Name = "tolDelete";
			this.tolDelete.Size = new System.Drawing.Size(23, 22);
			this.tolDelete.Text = "削除";
			this.tolDelete.Click += new System.EventHandler(this.tolDeleteTask_Click);
			// 
			// tolUp
			// 
			this.tolUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolUp.Image = ((System.Drawing.Image)(resources.GetObject("tolUp.Image")));
			this.tolUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolUp.Name = "tolUp";
			this.tolUp.Size = new System.Drawing.Size(23, 22);
			this.tolUp.Text = "上へ";
			this.tolUp.Click += new System.EventHandler(this.tolUp_Click);
			// 
			// tolDown
			// 
			this.tolDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolDown.Image = ((System.Drawing.Image)(resources.GetObject("tolDown.Image")));
			this.tolDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolDown.Name = "tolDown";
			this.tolDown.Size = new System.Drawing.Size(23, 22);
			this.tolDown.Text = "下へ";
			this.tolDown.Click += new System.EventHandler(this.tolDown_Click);
			// 
			// dlgEditSection
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 18F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(331, 415);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Controls.Add(this.tlbToolbar);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgEditSection";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "セクションの編集";
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tlbToolbar.ResumeLayout(false);
			this.tlbToolbar.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.ImageList imlNodeIcons;
		private System.Windows.Forms.ToolStrip tlbToolbar;
		private System.Windows.Forms.ToolStripButton tolSection;
		private System.Windows.Forms.ToolStripButton tolWorker;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.ToolStripButton tolDelete;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		public System.Windows.Forms.TreeView trvSection;
		private System.Windows.Forms.ToolStripButton tolUp;
		private System.Windows.Forms.ToolStripButton tolDown;
	}
}