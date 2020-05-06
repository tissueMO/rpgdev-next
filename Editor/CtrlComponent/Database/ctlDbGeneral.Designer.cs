namespace Editor.CtrlComponent.Database {
	partial class ctlDBGeneral {
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
			this.DB = new System.Windows.Forms.DataGridView();
			((System.ComponentModel.ISupportInitialize)(this.DB)).BeginInit();
			this.SuspendLayout();
			// 
			// DB
			// 
			this.DB.AllowUserToAddRows = false;
			this.DB.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
			this.DB.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
			this.DB.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.DB.Dock = System.Windows.Forms.DockStyle.Fill;
			this.DB.Location = new System.Drawing.Point(0, 0);
			this.DB.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.DB.Name = "DB";
			this.DB.RowHeadersWidth = 40;
			this.DB.RowTemplate.Height = 23;
			this.DB.RowTemplate.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.DB.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
			this.DB.Size = new System.Drawing.Size(1015, 490);
			this.DB.TabIndex = 1;
			// 
			// ctlDBGeneral
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.DB);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlDBGeneral";
			this.Size = new System.Drawing.Size(1015, 490);
			((System.ComponentModel.ISupportInitialize)(this.DB)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.DataGridView DB;
	}
}
