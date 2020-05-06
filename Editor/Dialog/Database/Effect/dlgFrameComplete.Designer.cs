namespace Editor.Dialog.Database.Effect {
	partial class dlgFrameComplete {
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

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
			this.SuspendLayout();
			// 
			// chkBlendMode
			// 
			this.chkBlendMode.Checked = false;
			this.chkBlendMode.CheckState = System.Windows.Forms.CheckState.Unchecked;
			this.chkBlendMode.Enabled = false;
			// 
			// chkSectionPos
			// 
			this.chkSectionPos.Checked = true;
			this.chkSectionPos.CheckState = System.Windows.Forms.CheckState.Checked;
			// 
			// lblText
			// 
			this.lblText.Text = "";
			// 
			// dlgFrameComplete
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.ClientSize = new System.Drawing.Size(314, 181);
			this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.Name = "dlgFrameComplete";
			this.Text = "エフェクトフレーム補間";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
	}
}
