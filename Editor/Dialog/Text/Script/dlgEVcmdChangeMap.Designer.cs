namespace Editor.Dialog.Text.Script {
	partial class dlgEVcmdChangeMap {
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
			this.uctlMapViewer = new Editor.CtrlComponent.Map.ctlMapViewer();
			this.uctlMapTree = new Editor.CtrlComponent.Common.ctlFileTree();
			this.panel1 = new System.Windows.Forms.Panel();
			this.chkOnlyPosition = new System.Windows.Forms.CheckBox();
			this.cmbDirection = new System.Windows.Forms.ComboBox();
			this.lblDirection = new System.Windows.Forms.Label();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// uctlMapViewer
			// 
			this.uctlMapViewer.DBList = null;
			this.uctlMapViewer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.uctlMapViewer.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlMapViewer.Location = new System.Drawing.Point(213, 0);
			this.uctlMapViewer.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlMapViewer.Name = "uctlMapViewer";
			this.uctlMapViewer.Size = new System.Drawing.Size(651, 472);
			this.uctlMapViewer.TabIndex = 1;
			this.uctlMapViewer.TilePosition = new System.Drawing.Point(0, 0);
			this.uctlMapViewer.Visible = false;
			// 
			// uctlMapTree
			// 
			this.uctlMapTree.AutoReload = false;
			this.uctlMapTree.CanDelete = false;
			this.uctlMapTree.CanRename = false;
			this.uctlMapTree.DefaultExnpandDepth = 2;
			this.uctlMapTree.Dock = System.Windows.Forms.DockStyle.Left;
			this.uctlMapTree.FileFilter = ".map";
			this.uctlMapTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlMapTree.IncludeFiles = true;
			this.uctlMapTree.Location = new System.Drawing.Point(0, 0);
			this.uctlMapTree.Name = "uctlMapTree";
			this.uctlMapTree.RootPath = "Maps\\";
			this.uctlMapTree.Size = new System.Drawing.Size(213, 516);
			this.uctlMapTree.TabIndex = 0;
			this.uctlMapTree.VisibleCheckBoxes = false;
			this.uctlMapTree.BeforeSelectNode += new System.Windows.Forms.TreeViewCancelEventHandler(this.uctlMapTree_BeforeSelectNode);
			this.uctlMapTree.AfterSelectNode += new System.Windows.Forms.TreeViewEventHandler(this.uctlMapTree_AfterSelectNode);
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.chkOnlyPosition);
			this.panel1.Controls.Add(this.cmbDirection);
			this.panel1.Controls.Add(this.lblDirection);
			this.panel1.Controls.Add(this.ctlOKCancel1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel1.Location = new System.Drawing.Point(213, 472);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(651, 44);
			this.panel1.TabIndex = 0;
			// 
			// chkOnlyPosition
			// 
			this.chkOnlyPosition.AutoSize = true;
			this.chkOnlyPosition.Location = new System.Drawing.Point(262, 13);
			this.chkOnlyPosition.Name = "chkOnlyPosition";
			this.chkOnlyPosition.Size = new System.Drawing.Size(132, 19);
			this.chkOnlyPosition.TabIndex = 2;
			this.chkOnlyPosition.Text = "座標のみを有効にする";
			this.chkOnlyPosition.UseVisualStyleBackColor = true;
			this.chkOnlyPosition.CheckedChanged += new System.EventHandler(this.chkOnlyPosition_CheckedChanged);
			// 
			// cmbDirection
			// 
			this.cmbDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbDirection.FormattingEnabled = true;
			this.cmbDirection.Location = new System.Drawing.Point(98, 11);
			this.cmbDirection.Name = "cmbDirection";
			this.cmbDirection.Size = new System.Drawing.Size(136, 23);
			this.cmbDirection.TabIndex = 1;
			// 
			// lblDirection
			// 
			this.lblDirection.AutoSize = true;
			this.lblDirection.Location = new System.Drawing.Point(13, 15);
			this.lblDirection.Name = "lblDirection";
			this.lblDirection.Size = new System.Drawing.Size(79, 15);
			this.lblDirection.TabIndex = 0;
			this.lblDirection.Text = "移動後の向き:";
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(433, 5);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 3;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// dlgEVcmdChangeMap
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(864, 516);
			this.Controls.Add(this.uctlMapViewer);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.uctlMapTree);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(880, 500);
			this.Name = "dlgEVcmdChangeMap";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "マップ座標の参照";
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Map.ctlMapViewer uctlMapViewer;
		private CtrlComponent.Common.ctlFileTree uctlMapTree;
		private System.Windows.Forms.Panel panel1;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.ComboBox cmbDirection;
		private System.Windows.Forms.Label lblDirection;
		private System.Windows.Forms.CheckBox chkOnlyPosition;
	}
}