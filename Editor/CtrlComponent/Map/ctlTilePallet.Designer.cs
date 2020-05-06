namespace Editor.CtrlComponent.Map {
	partial class ctlTilePallet {
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
			this.picTargetGraphics = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.picTargetGraphics)).BeginInit();
			this.SuspendLayout();
			// 
			// picTargetGraphics
			// 
			this.picTargetGraphics.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.picTargetGraphics.Dock = System.Windows.Forms.DockStyle.Top;
			this.picTargetGraphics.Location = new System.Drawing.Point(0, 0);
			this.picTargetGraphics.Margin = new System.Windows.Forms.Padding(0);
			this.picTargetGraphics.Name = "picTargetGraphics";
			this.picTargetGraphics.Size = new System.Drawing.Size(203, 177);
			this.picTargetGraphics.TabIndex = 1;
			this.picTargetGraphics.TabStop = false;
			this.picTargetGraphics.Paint += new System.Windows.Forms.PaintEventHandler(this.picTargetGraphics_Paint);
			this.picTargetGraphics.MouseDown += new System.Windows.Forms.MouseEventHandler(this.picTargetGraphics_MouseDown);
			this.picTargetGraphics.MouseEnter += new System.EventHandler(this.picTargetGraphics_MouseEnter);
			this.picTargetGraphics.MouseMove += new System.Windows.Forms.MouseEventHandler(this.picTargetGraphics_MouseMove);
			// 
			// ctlTilePallet
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoScroll = true;
			this.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.Controls.Add(this.picTargetGraphics);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "ctlTilePallet";
			this.Size = new System.Drawing.Size(203, 362);
			this.Resize += new System.EventHandler(this.ctlTilePallet_Resize);
			((System.ComponentModel.ISupportInitialize)(this.picTargetGraphics)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PictureBox picTargetGraphics;
	}
}
