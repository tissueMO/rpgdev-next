namespace Editor.Dialog.Database.Effect {
	partial class dlgFrameFill {
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
			this.chkBlendMode = new System.Windows.Forms.CheckBox();
			this.chkAngle = new System.Windows.Forms.CheckBox();
			this.chkExt = new System.Windows.Forms.CheckBox();
			this.chkTrans = new System.Windows.Forms.CheckBox();
			this.chkSectionPos = new System.Windows.Forms.CheckBox();
			this.chkViewPos = new System.Windows.Forms.CheckBox();
			this.lblText = new System.Windows.Forms.Label();
			this.numEnd = new System.Windows.Forms.NumericUpDown();
			this.Label2 = new System.Windows.Forms.Label();
			this.numStart = new System.Windows.Forms.NumericUpDown();
			this.Label1 = new System.Windows.Forms.Label();
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			((System.ComponentModel.ISupportInitialize)(this.numEnd)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numStart)).BeginInit();
			this.SuspendLayout();
			// 
			// chkBlendMode
			// 
			this.chkBlendMode.AutoSize = true;
			this.chkBlendMode.Checked = true;
			this.chkBlendMode.CheckState = System.Windows.Forms.CheckState.Checked;
			this.chkBlendMode.Location = new System.Drawing.Point(216, 105);
			this.chkBlendMode.Name = "chkBlendMode";
			this.chkBlendMode.Size = new System.Drawing.Size(74, 19);
			this.chkBlendMode.TabIndex = 10;
			this.chkBlendMode.Text = "合成方法";
			this.chkBlendMode.UseVisualStyleBackColor = true;
			// 
			// chkAngle
			// 
			this.chkAngle.AutoSize = true;
			this.chkAngle.Location = new System.Drawing.Point(122, 105);
			this.chkAngle.Name = "chkAngle";
			this.chkAngle.Size = new System.Drawing.Size(74, 19);
			this.chkAngle.TabIndex = 9;
			this.chkAngle.Text = "回転角度";
			this.chkAngle.UseVisualStyleBackColor = true;
			// 
			// chkExt
			// 
			this.chkExt.AutoSize = true;
			this.chkExt.Location = new System.Drawing.Point(32, 105);
			this.chkExt.Name = "chkExt";
			this.chkExt.Size = new System.Drawing.Size(62, 19);
			this.chkExt.TabIndex = 8;
			this.chkExt.Text = "拡大率";
			this.chkExt.UseVisualStyleBackColor = true;
			// 
			// chkTrans
			// 
			this.chkTrans.AutoSize = true;
			this.chkTrans.Location = new System.Drawing.Point(216, 77);
			this.chkTrans.Name = "chkTrans";
			this.chkTrans.Size = new System.Drawing.Size(62, 19);
			this.chkTrans.TabIndex = 7;
			this.chkTrans.Text = "透明度";
			this.chkTrans.UseVisualStyleBackColor = true;
			// 
			// chkSectionPos
			// 
			this.chkSectionPos.AutoSize = true;
			this.chkSectionPos.Location = new System.Drawing.Point(122, 77);
			this.chkSectionPos.Name = "chkSectionPos";
			this.chkSectionPos.Size = new System.Drawing.Size(74, 19);
			this.chkSectionPos.TabIndex = 6;
			this.chkSectionPos.Text = "区画座標";
			this.chkSectionPos.UseVisualStyleBackColor = true;
			// 
			// chkViewPos
			// 
			this.chkViewPos.AutoSize = true;
			this.chkViewPos.Location = new System.Drawing.Point(32, 77);
			this.chkViewPos.Name = "chkViewPos";
			this.chkViewPos.Size = new System.Drawing.Size(74, 19);
			this.chkViewPos.TabIndex = 5;
			this.chkViewPos.Text = "表示座標";
			this.chkViewPos.UseVisualStyleBackColor = true;
			// 
			// lblText
			// 
			this.lblText.Dock = System.Windows.Forms.DockStyle.Top;
			this.lblText.Location = new System.Drawing.Point(0, 0);
			this.lblText.Name = "lblText";
			this.lblText.Size = new System.Drawing.Size(314, 36);
			this.lblText.TabIndex = 0;
			this.lblText.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// numEnd
			// 
			this.numEnd.Location = new System.Drawing.Point(206, 39);
			this.numEnd.Name = "numEnd";
			this.numEnd.Size = new System.Drawing.Size(80, 23);
			this.numEnd.TabIndex = 4;
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Location = new System.Drawing.Point(149, 43);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(52, 15);
			this.Label2.TabIndex = 3;
			this.Label2.Text = "～ 終点:";
			// 
			// numStart
			// 
			this.numStart.Location = new System.Drawing.Point(64, 39);
			this.numStart.Name = "numStart";
			this.numStart.Size = new System.Drawing.Size(80, 23);
			this.numStart.TabIndex = 2;
			this.numStart.ValueChanged += new System.EventHandler(this.numStart_ValueChanged);
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.Location = new System.Drawing.Point(23, 43);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(36, 15);
			this.Label1.TabIndex = 1;
			this.Label1.Text = "始点:";
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.uctlOKCancel.DisenabledAcceptButton = false;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(52, 140);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 11;
			// 
			// dlgFrameFill
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(314, 181);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.chkBlendMode);
			this.Controls.Add(this.chkAngle);
			this.Controls.Add(this.chkExt);
			this.Controls.Add(this.chkTrans);
			this.Controls.Add(this.chkSectionPos);
			this.Controls.Add(this.chkViewPos);
			this.Controls.Add(this.lblText);
			this.Controls.Add(this.numEnd);
			this.Controls.Add(this.Label2);
			this.Controls.Add(this.numStart);
			this.Controls.Add(this.Label1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgFrameFill";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "エフェクトフレーム一括";
			((System.ComponentModel.ISupportInitialize)(this.numEnd)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numStart)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		protected System.Windows.Forms.CheckBox chkBlendMode;
		protected System.Windows.Forms.CheckBox chkAngle;
		protected System.Windows.Forms.CheckBox chkExt;
		protected System.Windows.Forms.CheckBox chkTrans;
		protected System.Windows.Forms.CheckBox chkSectionPos;
		protected System.Windows.Forms.CheckBox chkViewPos;
		protected System.Windows.Forms.Label lblText;
		private System.Windows.Forms.NumericUpDown numEnd;
		private System.Windows.Forms.Label Label2;
		private System.Windows.Forms.NumericUpDown numStart;
		private System.Windows.Forms.Label Label1;
	}
}