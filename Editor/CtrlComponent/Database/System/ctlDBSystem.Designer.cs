namespace Editor.CtrlComponent.Database {
	partial class ctlDBSystem {
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
			this.DB = new System.Windows.Forms.PropertyGrid();
			this.SuspendLayout();
			// 
			// DB
			// 
			this.DB.CategoryForeColor = System.Drawing.SystemColors.InactiveCaptionText;
			this.DB.Dock = System.Windows.Forms.DockStyle.Fill;
			this.DB.Location = new System.Drawing.Point(0, 0);
			this.DB.Name = "DB";
			this.DB.Size = new System.Drawing.Size(393, 473);
			this.DB.TabIndex = 1;
			this.DB.ToolbarVisible = false;
			this.DB.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.DB_PropertyValueChanged);
			// 
			// ctlDBSystem
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 18F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.DB);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlDBSystem";
			this.Size = new System.Drawing.Size(393, 473);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PropertyGrid DB;
	}
}
