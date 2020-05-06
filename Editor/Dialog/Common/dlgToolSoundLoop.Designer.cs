namespace Editor.Dialog.Common {
	partial class dlgToolSoundLoop {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.components = new System.ComponentModel.Container();
			this.btunSetZero = new System.Windows.Forms.Button();
			this.btnLoopSetEndPos = new System.Windows.Forms.Button();
			this.btnLoopSetStartPos = new System.Windows.Forms.Button();
			this.numNowPos = new System.Windows.Forms.NumericUpDown();
			this.Label7 = new System.Windows.Forms.Label();
			this.numLoopEnd = new System.Windows.Forms.NumericUpDown();
			this.numLoopStart = new System.Windows.Forms.NumericUpDown();
			this.Label3 = new System.Windows.Forms.Label();
			this.Label2 = new System.Windows.Forms.Label();
			this.btnLoopSave = new System.Windows.Forms.Button();
			this.btnLoopPlayStop = new System.Windows.Forms.Button();
			this.btnLoopNormalPlay = new System.Windows.Forms.Button();
			this.btnLoopPlay = new System.Windows.Forms.Button();
			this.prgSeekBar = new System.Windows.Forms.ProgressBar();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.loopSoundWatcher = new System.Windows.Forms.Timer(this.components);
			this.pnlWorkspace = new System.Windows.Forms.Panel();
			this.uctlFileTree = new Editor.CtrlComponent.Common.ctlFileTree();
			((System.ComponentModel.ISupportInitialize)(this.numNowPos)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numLoopEnd)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numLoopStart)).BeginInit();
			this.pnlWorkspace.SuspendLayout();
			this.SuspendLayout();
			// 
			// btunSetZero
			// 
			this.btunSetZero.Location = new System.Drawing.Point(117, 207);
			this.btunSetZero.Name = "btunSetZero";
			this.btunSetZero.Size = new System.Drawing.Size(100, 30);
			this.btunSetZero.TabIndex = 15;
			this.btunSetZero.Text = "０秒へ";
			this.btunSetZero.UseVisualStyleBackColor = true;
			this.btunSetZero.Click += new System.EventHandler(this.btunSetZero_Click);
			// 
			// btnLoopSetEndPos
			// 
			this.btnLoopSetEndPos.Location = new System.Drawing.Point(12, 207);
			this.btnLoopSetEndPos.Name = "btnLoopSetEndPos";
			this.btnLoopSetEndPos.Size = new System.Drawing.Size(100, 30);
			this.btnLoopSetEndPos.TabIndex = 14;
			this.btnLoopSetEndPos.Text = "終点２秒前へ";
			this.btnLoopSetEndPos.UseVisualStyleBackColor = true;
			this.btnLoopSetEndPos.Click += new System.EventHandler(this.btnLoopSetEndPos_Click);
			// 
			// btnLoopSetStartPos
			// 
			this.btnLoopSetStartPos.Location = new System.Drawing.Point(12, 172);
			this.btnLoopSetStartPos.Name = "btnLoopSetStartPos";
			this.btnLoopSetStartPos.Size = new System.Drawing.Size(100, 30);
			this.btnLoopSetStartPos.TabIndex = 12;
			this.btnLoopSetStartPos.Text = "始点へ";
			this.btnLoopSetStartPos.UseVisualStyleBackColor = true;
			this.btnLoopSetStartPos.Click += new System.EventHandler(this.btnLoopSetStartPos_Click);
			// 
			// numNowPos
			// 
			this.numNowPos.Enabled = false;
			this.numNowPos.Location = new System.Drawing.Point(87, 73);
			this.numNowPos.Name = "numNowPos";
			this.numNowPos.Size = new System.Drawing.Size(80, 23);
			this.numNowPos.TabIndex = 2;
			this.toolTip1.SetToolTip(this.numNowPos, "サンプリング位置を示します。");
			this.numNowPos.ValueChanged += new System.EventHandler(this.numNowPos_ValueChanged);
			// 
			// Label7
			// 
			this.Label7.AutoSize = true;
			this.Label7.Location = new System.Drawing.Point(46, 77);
			this.Label7.Name = "Label7";
			this.Label7.Size = new System.Drawing.Size(36, 15);
			this.Label7.TabIndex = 1;
			this.Label7.Text = "現在:";
			// 
			// numLoopEnd
			// 
			this.numLoopEnd.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
			this.numLoopEnd.Location = new System.Drawing.Point(233, 88);
			this.numLoopEnd.Name = "numLoopEnd";
			this.numLoopEnd.Size = new System.Drawing.Size(80, 23);
			this.numLoopEnd.TabIndex = 8;
			this.toolTip1.SetToolTip(this.numLoopEnd, "セットした後、「ループ再生」ボタンを押すまで反映されません。");
			this.numLoopEnd.ValueChanged += new System.EventHandler(this.numLoopValue_ValueChanged);
			// 
			// numLoopStart
			// 
			this.numLoopStart.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
			this.numLoopStart.Location = new System.Drawing.Point(233, 58);
			this.numLoopStart.Name = "numLoopStart";
			this.numLoopStart.Size = new System.Drawing.Size(80, 23);
			this.numLoopStart.TabIndex = 5;
			this.toolTip1.SetToolTip(this.numLoopStart, "セットした後、「ループ再生」ボタンを押すまで反映されません。");
			this.numLoopStart.ValueChanged += new System.EventHandler(this.numLoopValue_ValueChanged);
			// 
			// Label3
			// 
			this.Label3.AutoSize = true;
			this.Label3.Location = new System.Drawing.Point(192, 92);
			this.Label3.Name = "Label3";
			this.Label3.Size = new System.Drawing.Size(36, 15);
			this.Label3.TabIndex = 7;
			this.Label3.Text = "終点:";
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Location = new System.Drawing.Point(192, 62);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(36, 15);
			this.Label2.TabIndex = 4;
			this.Label2.Text = "始点:";
			// 
			// btnLoopSave
			// 
			this.btnLoopSave.Font = new System.Drawing.Font("Meiryo UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.btnLoopSave.Location = new System.Drawing.Point(227, 157);
			this.btnLoopSave.Name = "btnLoopSave";
			this.btnLoopSave.Size = new System.Drawing.Size(125, 60);
			this.btnLoopSave.TabIndex = 16;
			this.btnLoopSave.Text = "確定して保存";
			this.btnLoopSave.UseVisualStyleBackColor = true;
			this.btnLoopSave.Click += new System.EventHandler(this.btnLoopSave_Click);
			// 
			// btnLoopPlayStop
			// 
			this.btnLoopPlayStop.Location = new System.Drawing.Point(117, 172);
			this.btnLoopPlayStop.Name = "btnLoopPlayStop";
			this.btnLoopPlayStop.Size = new System.Drawing.Size(100, 30);
			this.btnLoopPlayStop.TabIndex = 13;
			this.btnLoopPlayStop.Text = "停止";
			this.btnLoopPlayStop.UseVisualStyleBackColor = true;
			this.btnLoopPlayStop.Click += new System.EventHandler(this.btnLoopPlayStop_Click);
			// 
			// btnLoopNormalPlay
			// 
			this.btnLoopNormalPlay.Location = new System.Drawing.Point(117, 137);
			this.btnLoopNormalPlay.Name = "btnLoopNormalPlay";
			this.btnLoopNormalPlay.Size = new System.Drawing.Size(100, 30);
			this.btnLoopNormalPlay.TabIndex = 11;
			this.btnLoopNormalPlay.Text = "最後まで再生";
			this.toolTip1.SetToolTip(this.btnLoopNormalPlay, "ループせずに通常再生します。");
			this.btnLoopNormalPlay.UseVisualStyleBackColor = true;
			this.btnLoopNormalPlay.Click += new System.EventHandler(this.btnLoopNormalPlay_Click);
			// 
			// btnLoopPlay
			// 
			this.btnLoopPlay.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.btnLoopPlay.Location = new System.Drawing.Point(12, 137);
			this.btnLoopPlay.Name = "btnLoopPlay";
			this.btnLoopPlay.Size = new System.Drawing.Size(100, 30);
			this.btnLoopPlay.TabIndex = 10;
			this.btnLoopPlay.Text = "ループ再生";
			this.toolTip1.SetToolTip(this.btnLoopPlay, "既に再生している状態でも設定できます。");
			this.btnLoopPlay.UseVisualStyleBackColor = true;
			this.btnLoopPlay.Click += new System.EventHandler(this.btnLoopPlay_Click);
			// 
			// prgSeekBar
			// 
			this.prgSeekBar.Location = new System.Drawing.Point(7, 7);
			this.prgSeekBar.Name = "prgSeekBar";
			this.prgSeekBar.Size = new System.Drawing.Size(345, 30);
			this.prgSeekBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
			this.prgSeekBar.TabIndex = 0;
			this.toolTip1.SetToolTip(this.prgSeekBar, "【シークバーの操作方法】\r\n＊左クリック：再生位置を移動\r\n＊中クリック：始点に設定\r\n＊右クリック：終点に設定\r\nなお、始点と終点をセットした後、\r\n「ループ再" +
        "生」ボタンを押すまで反映されませんのでご注意下さい。");
			this.prgSeekBar.MouseClick += new System.Windows.Forms.MouseEventHandler(this.prgSeekBar_MouseClick);
			// 
			// loopSoundWatcher
			// 
			this.loopSoundWatcher.Enabled = true;
			this.loopSoundWatcher.Interval = 10;
			this.loopSoundWatcher.Tick += new System.EventHandler(this.loopSoundWatcher_Tick);
			// 
			// pnlWorkspace
			// 
			this.pnlWorkspace.Controls.Add(this.btunSetZero);
			this.pnlWorkspace.Controls.Add(this.btnLoopSetEndPos);
			this.pnlWorkspace.Controls.Add(this.btnLoopSetStartPos);
			this.pnlWorkspace.Controls.Add(this.numNowPos);
			this.pnlWorkspace.Controls.Add(this.Label7);
			this.pnlWorkspace.Controls.Add(this.numLoopEnd);
			this.pnlWorkspace.Controls.Add(this.numLoopStart);
			this.pnlWorkspace.Controls.Add(this.Label3);
			this.pnlWorkspace.Controls.Add(this.Label2);
			this.pnlWorkspace.Controls.Add(this.btnLoopSave);
			this.pnlWorkspace.Controls.Add(this.btnLoopPlayStop);
			this.pnlWorkspace.Controls.Add(this.btnLoopNormalPlay);
			this.pnlWorkspace.Controls.Add(this.btnLoopPlay);
			this.pnlWorkspace.Controls.Add(this.prgSeekBar);
			this.pnlWorkspace.Location = new System.Drawing.Point(303, 8);
			this.pnlWorkspace.Name = "pnlWorkspace";
			this.pnlWorkspace.Size = new System.Drawing.Size(363, 246);
			this.pnlWorkspace.TabIndex = 1;
			// 
			// uctlFileTree
			// 
			this.uctlFileTree.AutoReload = true;
			this.uctlFileTree.CanDelete = false;
			this.uctlFileTree.CanRename = false;
			this.uctlFileTree.DefaultExnpandDepth = 2;
			this.uctlFileTree.Dock = System.Windows.Forms.DockStyle.Left;
			this.uctlFileTree.FileFilter = "";
			this.uctlFileTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlFileTree.IncludeFiles = true;
			this.uctlFileTree.Location = new System.Drawing.Point(0, 0);
			this.uctlFileTree.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
			this.uctlFileTree.Name = "uctlFileTree";
			this.uctlFileTree.RootPath = "";
			this.uctlFileTree.Size = new System.Drawing.Size(297, 259);
			this.uctlFileTree.TabIndex = 0;
			this.uctlFileTree.VisibleCheckBoxes = false;
			this.uctlFileTree.BeforeSelectNode += new System.Windows.Forms.TreeViewCancelEventHandler(this.uctlFileTree_BeforeSelectNode);
			this.uctlFileTree.AfterSelectNode += new System.Windows.Forms.TreeViewEventHandler(this.uctlFileTree_AfterSelectNode);
			// 
			// dlgToolSoundLoop
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(669, 259);
			this.Controls.Add(this.pnlWorkspace);
			this.Controls.Add(this.uctlFileTree);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgToolSoundLoop";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "サウンドループ設定";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgToolSoundLoop_FormClosing);
			((System.ComponentModel.ISupportInitialize)(this.numNowPos)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numLoopEnd)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numLoopStart)).EndInit();
			this.pnlWorkspace.ResumeLayout(false);
			this.pnlWorkspace.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.Button btunSetZero;
		private System.Windows.Forms.Button btnLoopSetEndPos;
		private System.Windows.Forms.Button btnLoopSetStartPos;
		private System.Windows.Forms.NumericUpDown numNowPos;
		private System.Windows.Forms.Label Label7;
		private System.Windows.Forms.NumericUpDown numLoopEnd;
		private System.Windows.Forms.NumericUpDown numLoopStart;
		private System.Windows.Forms.Label Label3;
		private System.Windows.Forms.Label Label2;
		private System.Windows.Forms.Button btnLoopSave;
		private System.Windows.Forms.Button btnLoopPlayStop;
		private System.Windows.Forms.Button btnLoopNormalPlay;
		private System.Windows.Forms.Button btnLoopPlay;
		private System.Windows.Forms.ProgressBar prgSeekBar;
		private CtrlComponent.Common.ctlFileTree uctlFileTree;
		private System.Windows.Forms.Timer loopSoundWatcher;
		private System.Windows.Forms.Panel pnlWorkspace;
	}
}