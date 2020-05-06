namespace Editor.CtrlComponent.Common {
	partial class ctlPathSetter {
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
			this.txtFileName = new System.Windows.Forms.TextBox();
			this.btnPathSetter = new System.Windows.Forms.Button();
			this.TableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.folderDlg = new System.Windows.Forms.FolderBrowserDialog();
			this.openFileDlg = new System.Windows.Forms.OpenFileDialog();
			this.saveFileDlg = new System.Windows.Forms.SaveFileDialog();
			this.toolHint = new System.Windows.Forms.ToolTip(this.components);
			this.TableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// txtFileName
			// 
			this.txtFileName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.txtFileName.Enabled = false;
			this.txtFileName.Location = new System.Drawing.Point(0, 1);
			this.txtFileName.Margin = new System.Windows.Forms.Padding(0);
			this.txtFileName.MaxLength = 256;
			this.txtFileName.Name = "txtFileName";
			this.txtFileName.Size = new System.Drawing.Size(159, 23);
			this.txtFileName.TabIndex = 0;
			this.txtFileName.TextChanged += new System.EventHandler(this.txtFileName_TextChanged);
			// 
			// btnPathSetter
			// 
			this.btnPathSetter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.btnPathSetter.Location = new System.Drawing.Point(159, 0);
			this.btnPathSetter.Margin = new System.Windows.Forms.Padding(0);
			this.btnPathSetter.Name = "btnPathSetter";
			this.btnPathSetter.Size = new System.Drawing.Size(34, 25);
			this.btnPathSetter.TabIndex = 1;
			this.btnPathSetter.Text = "...";
			this.btnPathSetter.UseVisualStyleBackColor = true;
			this.btnPathSetter.Click += new System.EventHandler(this.btnPathSetter_Click);
			// 
			// TableLayoutPanel1
			// 
			this.TableLayoutPanel1.ColumnCount = 2;
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 34F));
			this.TableLayoutPanel1.Controls.Add(this.txtFileName, 0, 0);
			this.TableLayoutPanel1.Controls.Add(this.btnPathSetter, 1, 0);
			this.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.TableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
			this.TableLayoutPanel1.Name = "TableLayoutPanel1";
			this.TableLayoutPanel1.RowCount = 1;
			this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutPanel1.Size = new System.Drawing.Size(193, 25);
			this.TableLayoutPanel1.TabIndex = 0;
			// 
			// openFileDlg
			// 
			this.openFileDlg.FileName = "openFileDialog1";
			// 
			// ctlPathSetter
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.TableLayoutPanel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(0);
			this.MaximumSize = new System.Drawing.Size(1000, 25);
			this.MinimumSize = new System.Drawing.Size(80, 25);
			this.Name = "ctlPathSetter";
			this.Size = new System.Drawing.Size(193, 25);
			this.TableLayoutPanel1.ResumeLayout(false);
			this.TableLayoutPanel1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.FolderBrowserDialog folderDlg;
		private System.Windows.Forms.OpenFileDialog openFileDlg;
		private System.Windows.Forms.SaveFileDialog saveFileDlg;
		private System.Windows.Forms.Button btnPathSetter;
		private System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
		private System.Windows.Forms.TextBox txtFileName;
		private System.Windows.Forms.ToolTip toolHint;
	}
}
