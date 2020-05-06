namespace Editor.Dialog.Common {
	partial class dlgInputTextValue {
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
			this.cmbText = new System.Windows.Forms.ComboBox();
			this.SuspendLayout();
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(35, 45);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 1;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// cmbText
			// 
			this.cmbText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.cmbText.FormattingEnabled = true;
			this.cmbText.Location = new System.Drawing.Point(12, 12);
			this.cmbText.Name = "cmbText";
			this.cmbText.Size = new System.Drawing.Size(256, 23);
			this.cmbText.TabIndex = 0;
			// 
			// dlgInputTextValue
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(280, 86);
			this.Controls.Add(this.cmbText);
			this.Controls.Add(this.ctlOKCancel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgInputTextValue";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ファイル名の入力";
			this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.ComboBox cmbText;
	}
}