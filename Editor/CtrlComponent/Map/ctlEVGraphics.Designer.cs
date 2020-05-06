namespace Editor.CtrlComponent.Map {
	partial class ctlEVGraphics {
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
			this.components = new System.ComponentModel.Container();
			this.picTargetGraphics = new System.Windows.Forms.PictureBox();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			((System.ComponentModel.ISupportInitialize)(this.picTargetGraphics)).BeginInit();
			this.SuspendLayout();
			// 
			// picTargetGraphics
			// 
			this.picTargetGraphics.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.picTargetGraphics.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.picTargetGraphics.Cursor = System.Windows.Forms.Cursors.Hand;
			this.picTargetGraphics.Dock = System.Windows.Forms.DockStyle.Fill;
			this.picTargetGraphics.ErrorImage = null;
			this.picTargetGraphics.InitialImage = null;
			this.picTargetGraphics.Location = new System.Drawing.Point(0, 0);
			this.picTargetGraphics.Margin = new System.Windows.Forms.Padding(0);
			this.picTargetGraphics.Name = "picTargetGraphics";
			this.picTargetGraphics.Size = new System.Drawing.Size(135, 123);
			this.picTargetGraphics.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.picTargetGraphics.TabIndex = 1;
			this.picTargetGraphics.TabStop = false;
			this.toolTip1.SetToolTip(this.picTargetGraphics, "左クリックで画像変更、右クリックで消去します。");
			this.picTargetGraphics.MouseClick += new System.Windows.Forms.MouseEventHandler(this.TargetGraphics_MouseClick);
			// 
			// ctlGraphicsSelectArea
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.picTargetGraphics);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlGraphicsSelectArea";
			this.Size = new System.Drawing.Size(135, 123);
			((System.ComponentModel.ISupportInitialize)(this.picTargetGraphics)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.PictureBox picTargetGraphics;
	}
}
