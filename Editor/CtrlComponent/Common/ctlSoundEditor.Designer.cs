namespace Editor.CtrlComponent.Common {
	partial class ctlSoundEditor {
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.trkVolume = new System.Windows.Forms.TrackBar();
			this.trkPitch = new System.Windows.Forms.TrackBar();
			this.trkPan = new System.Windows.Forms.TrackBar();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.btnInit = new System.Windows.Forms.Button();
			this.uctlPlayer = new Editor.CtrlComponent.Common.ctlSoundPlayer();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			((System.ComponentModel.ISupportInitialize)(this.trkVolume)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trkPitch)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trkPan)).BeginInit();
			this.tableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.ForeColor = System.Drawing.Color.Black;
			this.label1.Location = new System.Drawing.Point(3, 8);
			this.label1.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(76, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "音量";
			this.label1.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.ForeColor = System.Drawing.Color.Black;
			this.label2.Location = new System.Drawing.Point(85, 8);
			this.label2.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(77, 16);
			this.label2.TabIndex = 2;
			this.label2.Text = "音程";
			this.label2.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			this.toolTip1.SetToolTip(this.label2, "音程を変更すると、読み込み時間が増えます。\r\nBGMなど30秒を超えるサウンドの音程変更は推奨されません。");
			// 
			// label3
			// 
			this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.label3.ForeColor = System.Drawing.Color.Black;
			this.label3.Location = new System.Drawing.Point(168, 8);
			this.label3.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(77, 16);
			this.label3.TabIndex = 4;
			this.label3.Text = "パン";
			this.label3.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			// 
			// trkVolume
			// 
			this.trkVolume.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.trkVolume.LargeChange = 20;
			this.trkVolume.Location = new System.Drawing.Point(0, 30);
			this.trkVolume.Margin = new System.Windows.Forms.Padding(0);
			this.trkVolume.Maximum = 100;
			this.trkVolume.Name = "trkVolume";
			this.trkVolume.Size = new System.Drawing.Size(82, 30);
			this.trkVolume.TabIndex = 1;
			this.trkVolume.TickFrequency = 20;
			this.trkVolume.Value = 100;
			// 
			// trkPitch
			// 
			this.trkPitch.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.trkPitch.LargeChange = 12;
			this.trkPitch.Location = new System.Drawing.Point(82, 30);
			this.trkPitch.Margin = new System.Windows.Forms.Padding(0);
			this.trkPitch.Maximum = 240;
			this.trkPitch.Minimum = -240;
			this.trkPitch.Name = "trkPitch";
			this.trkPitch.Size = new System.Drawing.Size(83, 30);
			this.trkPitch.TabIndex = 3;
			this.trkPitch.TickFrequency = 80;
			// 
			// trkPan
			// 
			this.trkPan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.trkPan.LargeChange = 40;
			this.trkPan.Location = new System.Drawing.Point(165, 30);
			this.trkPan.Margin = new System.Windows.Forms.Padding(0);
			this.trkPan.Maximum = 100;
			this.trkPan.Minimum = -100;
			this.trkPan.Name = "trkPan";
			this.trkPan.Size = new System.Drawing.Size(83, 30);
			this.trkPan.TabIndex = 5;
			this.trkPan.TickFrequency = 40;
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.ColumnCount = 4;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33332F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 81F));
			this.tableLayoutPanel1.Controls.Add(this.trkPitch, 1, 1);
			this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.trkVolume, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.label2, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.label3, 2, 0);
			this.tableLayoutPanel1.Controls.Add(this.trkPan, 2, 1);
			this.tableLayoutPanel1.Controls.Add(this.uctlPlayer, 3, 1);
			this.tableLayoutPanel1.Controls.Add(this.btnInit, 3, 0);
			this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
			this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 3;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.Size = new System.Drawing.Size(330, 60);
			this.tableLayoutPanel1.TabIndex = 7;
			// 
			// btnInit
			// 
			this.btnInit.Dock = System.Windows.Forms.DockStyle.Fill;
			this.btnInit.ForeColor = System.Drawing.Color.Black;
			this.btnInit.Location = new System.Drawing.Point(248, 2);
			this.btnInit.Margin = new System.Windows.Forms.Padding(0, 2, 0, 0);
			this.btnInit.Name = "btnInit";
			this.btnInit.Size = new System.Drawing.Size(82, 28);
			this.btnInit.TabIndex = 7;
			this.btnInit.Text = "初期化";
			this.btnInit.UseVisualStyleBackColor = true;
			this.btnInit.Click += new System.EventHandler(this.btnInit_Click);
			// 
			// uctlPlayer
			// 
			this.uctlPlayer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
			this.uctlPlayer.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlPlayer.Location = new System.Drawing.Point(248, 30);
			this.uctlPlayer.Margin = new System.Windows.Forms.Padding(0);
			this.uctlPlayer.Name = "uctlPlayer";
			this.uctlPlayer.SelfLoadMode = true;
			this.uctlPlayer.Size = new System.Drawing.Size(82, 29);
			this.uctlPlayer.SoundFileName = null;
			this.uctlPlayer.TabIndex = 6;
			this.uctlPlayer.BeforePlaySound += new System.EventHandler(this.uctlPlayer_BeforePlaySound);
			// 
			// ctlSoundEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.tableLayoutPanel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MinimumSize = new System.Drawing.Size(0, 60);
			this.Name = "ctlSoundEditor";
			this.Size = new System.Drawing.Size(330, 60);
			((System.ComponentModel.ISupportInitialize)(this.trkVolume)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trkPitch)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trkPan)).EndInit();
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private ctlSoundPlayer uctlPlayer;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TrackBar trkVolume;
		private System.Windows.Forms.TrackBar trkPitch;
		private System.Windows.Forms.TrackBar trkPan;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Button btnInit;
		private System.Windows.Forms.ToolTip toolTip1;
	}
}
