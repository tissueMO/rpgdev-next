namespace Editor.Dialog.Common {
	partial class dlgInputNumValue {
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
			this.numInput = new System.Windows.Forms.NumericUpDown();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			((System.ComponentModel.ISupportInitialize)(this.numInput)).BeginInit();
			this.SuspendLayout();
			// 
			// numInput
			// 
			this.numInput.Location = new System.Drawing.Point(38, 11);
			this.numInput.Name = "numInput";
			this.numInput.Size = new System.Drawing.Size(156, 23);
			this.numInput.TabIndex = 0;
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(8, 45);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 1;
			// 
			// dlgInputNumValue
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(227, 86);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.numInput);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgInputNumValue";
			this.Padding = new System.Windows.Forms.Padding(8);
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "数値の入力";
			((System.ComponentModel.ISupportInitialize)(this.numInput)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.NumericUpDown numInput;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
	}
}