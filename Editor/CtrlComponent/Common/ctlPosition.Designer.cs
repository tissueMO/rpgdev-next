namespace Editor.CtrlComponent.Common {
	partial class ctlPosition {
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
			this.lblSplit = new System.Windows.Forms.Label();
			this.numY = new System.Windows.Forms.NumericUpDown();
			this.numX = new System.Windows.Forms.NumericUpDown();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			((System.ComponentModel.ISupportInitialize)(this.numY)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numX)).BeginInit();
			this.tableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// lblSplit
			// 
			this.lblSplit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lblSplit.Location = new System.Drawing.Point(66, 0);
			this.lblSplit.Margin = new System.Windows.Forms.Padding(0);
			this.lblSplit.Name = "lblSplit";
			this.lblSplit.Size = new System.Drawing.Size(17, 23);
			this.lblSplit.TabIndex = 1;
			this.lblSplit.Text = ",";
			this.lblSplit.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// numY
			// 
			this.numY.Dock = System.Windows.Forms.DockStyle.Fill;
			this.numY.Location = new System.Drawing.Point(83, 0);
			this.numY.Margin = new System.Windows.Forms.Padding(0);
			this.numY.Name = "numY";
			this.numY.Size = new System.Drawing.Size(66, 23);
			this.numY.TabIndex = 2;
			this.numY.ValueChanged += new System.EventHandler(this.num_ValueChanged);
			// 
			// numX
			// 
			this.numX.Dock = System.Windows.Forms.DockStyle.Fill;
			this.numX.Location = new System.Drawing.Point(0, 0);
			this.numX.Margin = new System.Windows.Forms.Padding(0);
			this.numX.Name = "numX";
			this.numX.Size = new System.Drawing.Size(66, 23);
			this.numX.TabIndex = 0;
			this.numX.ValueChanged += new System.EventHandler(this.num_ValueChanged);
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 3;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 17F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Controls.Add(this.lblSplit, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.numY, 2, 0);
			this.tableLayoutPanel1.Controls.Add(this.numX, 0, 0);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 1;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(149, 23);
			this.tableLayoutPanel1.TabIndex = 3;
			// 
			// ctlPosition
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tableLayoutPanel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximumSize = new System.Drawing.Size(500, 23);
			this.MinimumSize = new System.Drawing.Size(100, 23);
			this.Name = "ctlPosition";
			this.Size = new System.Drawing.Size(149, 23);
			((System.ComponentModel.ISupportInitialize)(this.numY)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numX)).EndInit();
			this.tableLayoutPanel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label lblSplit;
		private System.Windows.Forms.NumericUpDown numY;
		private System.Windows.Forms.NumericUpDown numX;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
	}
}
