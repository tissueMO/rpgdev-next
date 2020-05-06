namespace Editor.Dialog.Database {
	partial class dlgSelectIcon {
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
			this.uctlSelectTile = new Editor.CtrlComponent.Common.ctlSelectTile();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.SuspendLayout();
			// 
			// uctlSelectTile
			// 
			this.uctlSelectTile.AutoScroll = true;
			this.uctlSelectTile.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.uctlSelectTile.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.uctlSelectTile.Location = new System.Drawing.Point(12, 13);
			this.uctlSelectTile.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlSelectTile.Name = "uctlSelectTile";
			this.uctlSelectTile.Position = new System.Drawing.Point(0, 0);
			this.uctlSelectTile.Size = new System.Drawing.Size(574, 375);
			this.uctlSelectTile.TabIndex = 0;
			this.uctlSelectTile.TileSize = new System.Drawing.Size(0, 0);
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(378, 398);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 1;
			this.ctlOKCancel1.TabStop = false;
			// 
			// dlgSelectIcon
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(598, 438);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.uctlSelectTile);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.KeyPreview = true;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgSelectIcon";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "アイコンの選択";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dlgSelectIcon_KeyDown);
			this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Common.ctlSelectTile uctlSelectTile;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
	}
}