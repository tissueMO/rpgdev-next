namespace Editor.CtrlComponent.Database {
	partial class ctlDBBase {
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
			this.dlgImporter = new System.Windows.Forms.OpenFileDialog();
			this.dlgExporter = new System.Windows.Forms.SaveFileDialog();
			this.SuspendLayout();
			// 
			// dlgImporter
			// 
			this.dlgImporter.Filter = "タブ区切りファイル (*.tsv)|*.tsv";
			this.dlgImporter.RestoreDirectory = true;
			this.dlgImporter.Title = "データベースのインポート";
			// 
			// dlgExporter
			// 
			this.dlgExporter.DefaultExt = "tsv";
			this.dlgExporter.Filter = "タブ区切りファイル (*.tsv)|*.tsv";
			this.dlgExporter.RestoreDirectory = true;
			this.dlgExporter.Title = "データベースのエクスポート";
			// 
			// ctlDBBase
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Name = "ctlDBBase";
			this.ResumeLayout(false);

		}

		#endregion

		protected System.Windows.Forms.OpenFileDialog dlgImporter;
		protected System.Windows.Forms.SaveFileDialog dlgExporter;
	}
}
