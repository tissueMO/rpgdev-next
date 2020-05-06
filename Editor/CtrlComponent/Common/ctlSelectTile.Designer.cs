namespace Editor.CtrlComponent.Common {
	partial class ctlSelectTile {
		/// <summary> 
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region コンポーネント デザイナーで生成されたコード

		/// <summary> 
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
			this.picSrc = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.picSrc)).BeginInit();
			this.SuspendLayout();
			// 
			// picSrc
			// 
			this.picSrc.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.picSrc.ErrorImage = null;
			this.picSrc.Location = new System.Drawing.Point(0, 0);
			this.picSrc.Name = "picSrc";
			this.picSrc.Size = new System.Drawing.Size(80, 80);
			this.picSrc.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picSrc.TabIndex = 0;
			this.picSrc.TabStop = false;
			this.picSrc.Paint += new System.Windows.Forms.PaintEventHandler(this.picSrc_Paint);
			this.picSrc.MouseClick += new System.Windows.Forms.MouseEventHandler(this.picSrc_MouseClick);
			this.picSrc.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.picSrc_MouseDoubleClick);
			this.picSrc.MouseEnter += new System.EventHandler(this.picSrc_MouseEnter);
			// 
			// ctlSelectTile
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoScroll = true;
			this.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.Controls.Add(this.picSrc);
			this.DoubleBuffered = true;
			this.Name = "ctlSelectTile";
			this.Size = new System.Drawing.Size(214, 163);
			((System.ComponentModel.ISupportInitialize)(this.picSrc)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.PictureBox picSrc;
	}
}
