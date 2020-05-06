namespace Editor.Dialog.Map {
	partial class dlgSelectMapTilePosition {
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
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.uctlViewer = new Editor.CtrlComponent.Map.ctlMapViewer();
			this.SuspendLayout();
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(541, 535);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 1;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// uctlViewer
			// 
			this.uctlViewer.DBList = null;
			this.uctlViewer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.uctlViewer.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlViewer.Location = new System.Drawing.Point(0, 0);
			this.uctlViewer.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlViewer.Name = "uctlViewer";
			this.uctlViewer.Padding = new System.Windows.Forms.Padding(0, 0, 0, 50);
			this.uctlViewer.Size = new System.Drawing.Size(760, 575);
			this.uctlViewer.TabIndex = 0;
			this.uctlViewer.SelectedByDoubleClick += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// dlgSelectMapTilePosition
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(760, 575);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.uctlViewer);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.KeyPreview = true;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgSelectMapTilePosition";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "タイル座標の選択";
			this.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.dlgSelectMapTilePosition_PreviewKeyDown);
			this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Map.ctlMapViewer uctlViewer;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
	}
}