namespace Editor.Dialog.Common {
	partial class dlgSelectInList {
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
            this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.lstJump = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // uctlOKCancel
            // 
            this.uctlOKCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.uctlOKCancel.DisenabledAcceptButton = false;
            this.uctlOKCancel.DisenabledCancelButton = false;
            this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlOKCancel.Location = new System.Drawing.Point(40, 295);
            this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlOKCancel.Name = "uctlOKCancel";
            this.uctlOKCancel.OKTitle = "OK";
            this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
            this.uctlOKCancel.TabIndex = 1;
            this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
            // 
            // lstJump
            // 
            this.lstJump.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lstJump.FormattingEnabled = true;
            this.lstJump.ItemHeight = 15;
            this.lstJump.Location = new System.Drawing.Point(12, 12);
            this.lstJump.Name = "lstJump";
            this.lstJump.Size = new System.Drawing.Size(269, 274);
            this.lstJump.TabIndex = 0;
            this.lstJump.DoubleClick += new System.EventHandler(this.lstJump_DoubleClick);
            // 
            // dlgSelectInList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(293, 334);
            this.Controls.Add(this.lstJump);
            this.Controls.Add(this.uctlOKCancel);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgSelectInList";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.ListBox lstJump;
	}
}