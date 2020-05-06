namespace Editor.Dialog.Common {
	partial class dlgShift {
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
			this.btnLeft = new System.Windows.Forms.Button();
			this.btnRight = new System.Windows.Forms.Button();
			this.btnDown = new System.Windows.Forms.Button();
			this.btnUp = new System.Windows.Forms.Button();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.uctlPos = new Editor.CtrlComponent.Common.ctlPosition();
			this.SuspendLayout();
			// 
			// btnLeft
			// 
			this.btnLeft.Font = new System.Drawing.Font("Meiryo UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.btnLeft.Location = new System.Drawing.Point(15, 62);
			this.btnLeft.Name = "btnLeft";
			this.btnLeft.Size = new System.Drawing.Size(55, 45);
			this.btnLeft.TabIndex = 2;
			this.btnLeft.Text = "←";
			this.btnLeft.UseVisualStyleBackColor = true;
			this.btnLeft.Click += new System.EventHandler(this.btnLeft_Click);
			// 
			// btnRight
			// 
			this.btnRight.Font = new System.Drawing.Font("Meiryo UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.btnRight.Location = new System.Drawing.Point(219, 62);
			this.btnRight.Name = "btnRight";
			this.btnRight.Size = new System.Drawing.Size(55, 45);
			this.btnRight.TabIndex = 3;
			this.btnRight.Text = "→";
			this.btnRight.UseVisualStyleBackColor = true;
			this.btnRight.Click += new System.EventHandler(this.btnRight_Click);
			// 
			// btnDown
			// 
			this.btnDown.Font = new System.Drawing.Font("Meiryo UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.btnDown.Location = new System.Drawing.Point(116, 112);
			this.btnDown.Name = "btnDown";
			this.btnDown.Size = new System.Drawing.Size(55, 45);
			this.btnDown.TabIndex = 4;
			this.btnDown.Text = "↓";
			this.btnDown.UseVisualStyleBackColor = true;
			this.btnDown.Click += new System.EventHandler(this.btnDown_Click);
			// 
			// btnUp
			// 
			this.btnUp.Font = new System.Drawing.Font("Meiryo UI", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.btnUp.Location = new System.Drawing.Point(116, 12);
			this.btnUp.Name = "btnUp";
			this.btnUp.Size = new System.Drawing.Size(55, 45);
			this.btnUp.TabIndex = 1;
			this.btnUp.Text = "↑";
			this.btnUp.UseVisualStyleBackColor = true;
			this.btnUp.Click += new System.EventHandler(this.btnUp_Click);
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(37, 172);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 5;
			// 
			// uctlPos
			// 
			this.uctlPos.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlPos.Location = new System.Drawing.Point(77, 74);
			this.uctlPos.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlPos.Maximum = new System.Drawing.Point(100, 100);
			this.uctlPos.MaximumSize = new System.Drawing.Size(500, 23);
			this.uctlPos.Minimum = new System.Drawing.Point(0, 0);
			this.uctlPos.MinimumSize = new System.Drawing.Size(100, 23);
			this.uctlPos.Name = "uctlPos";
			this.uctlPos.Result = new System.Drawing.Point(0, 0);
			this.uctlPos.Size = new System.Drawing.Size(137, 23);
			this.uctlPos.TabIndex = 0;
			this.uctlPos.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Point;
			// 
			// dlgShift
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(289, 214);
			this.Controls.Add(this.uctlPos);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.btnLeft);
			this.Controls.Add(this.btnRight);
			this.Controls.Add(this.btnDown);
			this.Controls.Add(this.btnUp);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgShift";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "シフト";
			this.ResumeLayout(false);

		}

		#endregion
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.Button btnLeft;
		private System.Windows.Forms.Button btnRight;
		private System.Windows.Forms.Button btnDown;
		private System.Windows.Forms.Button btnUp;
		private CtrlComponent.Common.ctlPosition uctlPos;
	}
}