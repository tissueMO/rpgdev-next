namespace Editor.Dialog.Database.Effect {
	partial class dlgFrameCopy {
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
			this.Label4 = new System.Windows.Forms.Label();
			this.numBase = new System.Windows.Forms.NumericUpDown();
			this.Label3 = new System.Windows.Forms.Label();
			this.numDestEnd = new System.Windows.Forms.NumericUpDown();
			this.Label2 = new System.Windows.Forms.Label();
			this.numDestStart = new System.Windows.Forms.NumericUpDown();
			this.Label1 = new System.Windows.Forms.Label();
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			((System.ComponentModel.ISupportInitialize)(this.numBase)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numDestEnd)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numDestStart)).BeginInit();
			this.SuspendLayout();
			// 
			// Label4
			// 
			this.Label4.Dock = System.Windows.Forms.DockStyle.Top;
			this.Label4.Location = new System.Drawing.Point(0, 0);
			this.Label4.Name = "Label4";
			this.Label4.Size = new System.Drawing.Size(341, 36);
			this.Label4.TabIndex = 0;
			this.Label4.Text = "基準フレームの設定がコピー先の範囲に適用されます。";
			this.Label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// numBase
			// 
			this.numBase.Enabled = false;
			this.numBase.Location = new System.Drawing.Point(99, 39);
			this.numBase.Name = "numBase";
			this.numBase.Size = new System.Drawing.Size(80, 23);
			this.numBase.TabIndex = 2;
			// 
			// Label3
			// 
			this.Label3.AutoSize = true;
			this.Label3.Location = new System.Drawing.Point(14, 43);
			this.Label3.Name = "Label3";
			this.Label3.Size = new System.Drawing.Size(73, 15);
			this.Label3.TabIndex = 1;
			this.Label3.Text = "基準フレーム:";
			// 
			// numDestEnd
			// 
			this.numDestEnd.Location = new System.Drawing.Point(242, 69);
			this.numDestEnd.Name = "numDestEnd";
			this.numDestEnd.Size = new System.Drawing.Size(80, 23);
			this.numDestEnd.TabIndex = 6;
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Location = new System.Drawing.Point(184, 73);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(52, 15);
			this.Label2.TabIndex = 5;
			this.Label2.Text = "～ 終点:";
			// 
			// numDestStart
			// 
			this.numDestStart.Location = new System.Drawing.Point(99, 69);
			this.numDestStart.Name = "numDestStart";
			this.numDestStart.Size = new System.Drawing.Size(80, 23);
			this.numDestStart.TabIndex = 4;
			this.numDestStart.ValueChanged += new System.EventHandler(this.numDestStart_ValueChanged);
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.Location = new System.Drawing.Point(14, 73);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(79, 15);
			this.Label1.TabIndex = 3;
			this.Label1.Text = "コピー先 始点:";
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(64, 107);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 7;
			// 
			// dlgFrameCopy
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(341, 147);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.Label4);
			this.Controls.Add(this.numBase);
			this.Controls.Add(this.Label3);
			this.Controls.Add(this.numDestEnd);
			this.Controls.Add(this.Label2);
			this.Controls.Add(this.numDestStart);
			this.Controls.Add(this.Label1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgFrameCopy";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "エフェクトフレームコピー";
			((System.ComponentModel.ISupportInitialize)(this.numBase)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numDestEnd)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numDestStart)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.Label Label4;
		private System.Windows.Forms.NumericUpDown numBase;
		private System.Windows.Forms.Label Label3;
		private System.Windows.Forms.NumericUpDown numDestEnd;
		private System.Windows.Forms.Label Label2;
		private System.Windows.Forms.NumericUpDown numDestStart;
		private System.Windows.Forms.Label Label1;
	}
}