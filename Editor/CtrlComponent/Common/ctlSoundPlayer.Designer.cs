namespace Editor.CtrlComponent.Common {
	partial class ctlSoundPlayer {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlSoundPlayer));
			this.btnPlay = new System.Windows.Forms.Button();
			this.btnStop = new System.Windows.Forms.Button();
			this.TableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.TableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnPlay
			// 
			this.btnPlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.btnPlay.Image = ((System.Drawing.Image)(resources.GetObject("btnPlay.Image")));
			this.btnPlay.Location = new System.Drawing.Point(0, 1);
			this.btnPlay.Margin = new System.Windows.Forms.Padding(0);
			this.btnPlay.Name = "btnPlay";
			this.btnPlay.Size = new System.Drawing.Size(48, 28);
			this.btnPlay.TabIndex = 0;
			this.btnPlay.UseVisualStyleBackColor = true;
			this.btnPlay.Click += new System.EventHandler(this.btnPlay_Click);
			// 
			// btnStop
			// 
			this.btnStop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.btnStop.Image = ((System.Drawing.Image)(resources.GetObject("btnStop.Image")));
			this.btnStop.Location = new System.Drawing.Point(48, 1);
			this.btnStop.Margin = new System.Windows.Forms.Padding(0);
			this.btnStop.Name = "btnStop";
			this.btnStop.Size = new System.Drawing.Size(49, 28);
			this.btnStop.TabIndex = 1;
			this.btnStop.UseVisualStyleBackColor = true;
			this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
			// 
			// TableLayoutPanel1
			// 
			this.TableLayoutPanel1.ColumnCount = 2;
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50.28571F));
			this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 49.71429F));
			this.TableLayoutPanel1.Controls.Add(this.btnPlay, 0, 0);
			this.TableLayoutPanel1.Controls.Add(this.btnStop, 1, 0);
			this.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.TableLayoutPanel1.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
			this.TableLayoutPanel1.Name = "TableLayoutPanel1";
			this.TableLayoutPanel1.RowCount = 1;
			this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.TableLayoutPanel1.Size = new System.Drawing.Size(97, 30);
			this.TableLayoutPanel1.TabIndex = 0;
			// 
			// ctlSoundPlayer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.TableLayoutPanel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlSoundPlayer";
			this.Size = new System.Drawing.Size(97, 30);
			this.TableLayoutPanel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button btnPlay;
		private System.Windows.Forms.Button btnStop;
		private System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
	}
}
