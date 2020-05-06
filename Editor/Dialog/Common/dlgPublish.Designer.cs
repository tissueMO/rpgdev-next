namespace Editor.Dialog.Common {
	partial class dlgPublish {
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
            this.uctlArchiveTree = new Editor.CtrlComponent.Common.ctlFileTree();
            this.label1 = new System.Windows.Forms.Label();
            this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.label3 = new System.Windows.Forms.Label();
            this.uctlCopyTree = new Editor.CtrlComponent.Common.ctlFileTree();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chkEnabledZip = new System.Windows.Forms.CheckBox();
            this.chkAutorun = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.uctlDestPath = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.pnlProcess = new System.Windows.Forms.Panel();
            this.prgProcess = new System.Windows.Forms.ProgressBar();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.pnlProcess.SuspendLayout();
            this.SuspendLayout();
            // 
            // uctlArchiveTree
            // 
            this.uctlArchiveTree.AutoReload = true;
            this.uctlArchiveTree.CanDelete = false;
            this.uctlArchiveTree.CanRename = false;
            this.uctlArchiveTree.DefaultExnpandDepth = 2;
            this.uctlArchiveTree.FileFilter = "";
            this.uctlArchiveTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlArchiveTree.IncludeFiles = false;
            this.uctlArchiveTree.Location = new System.Drawing.Point(12, 36);
            this.uctlArchiveTree.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlArchiveTree.Name = "uctlArchiveTree";
            this.uctlArchiveTree.RootPath = "";
            this.uctlArchiveTree.Size = new System.Drawing.Size(231, 304);
            this.uctlArchiveTree.TabIndex = 1;
            this.uctlArchiveTree.VisibleCheckBoxes = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(122, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "アーカイブするフォルダー:";
            this.toolTip1.SetToolTip(this.label1, "一つのファイルにまとめる対象フォルダーを設定します。\r\n原則としてデータベース、スクリプト、マップ、各種素材を含めて下さい。\r\nその他、ユーザー定義の読み込み用フ" +
        "ァイル等があればそれを追加して下さい。");
            // 
            // uctlOKCancel
            // 
            this.uctlOKCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.uctlOKCancel.DisenabledAcceptButton = false;
            this.uctlOKCancel.DisenabledCancelButton = false;
            this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlOKCancel.Location = new System.Drawing.Point(502, 367);
            this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlOKCancel.Name = "uctlOKCancel";
            this.uctlOKCancel.OKTitle = "OK";
            this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
            this.uctlOKCancel.TabIndex = 6;
            this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(255, 13);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(142, 15);
            this.label3.TabIndex = 2;
            this.label3.Text = "コピーするファイル/フォルダー:";
            this.toolTip1.SetToolTip(this.label3, "ここでチェックを入れた項目は、発行先フォルダーの直下にコピーされます。\r\n取扱説明書(Readme.txt)や、マニュアルフォルダー等を指定します。\r\nなお、ゲー" +
        "ムプログラムや外部ライブラリ等はリリース用のものに差し替えられるため、\r\nこのリスト上でチェックする必要はありません。");
            // 
            // uctlCopyTree
            // 
            this.uctlCopyTree.AutoReload = true;
            this.uctlCopyTree.CanDelete = false;
            this.uctlCopyTree.CanRename = false;
            this.uctlCopyTree.DefaultExnpandDepth = 2;
            this.uctlCopyTree.FileFilter = "";
            this.uctlCopyTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlCopyTree.IncludeFiles = true;
            this.uctlCopyTree.Location = new System.Drawing.Point(258, 36);
            this.uctlCopyTree.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlCopyTree.Name = "uctlCopyTree";
            this.uctlCopyTree.RootPath = "";
            this.uctlCopyTree.Size = new System.Drawing.Size(231, 304);
            this.uctlCopyTree.TabIndex = 3;
            this.uctlCopyTree.VisibleCheckBoxes = true;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label4.Location = new System.Drawing.Point(12, 348);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(484, 50);
            this.label4.TabIndex = 4;
            this.label4.Text = "ゲーム中に使われるデータを一つのファイルにまとめます。\r\nアーカイブ後はユーザーによって復元できません。\r\n素材を差し替えた、データベースやスクリプトを修正した等" +
    "の際は再度発行し直してください。";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.chkEnabledZip);
            this.groupBox1.Controls.Add(this.chkAutorun);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.uctlDestPath);
            this.groupBox1.ForeColor = System.Drawing.Color.Blue;
            this.groupBox1.Location = new System.Drawing.Point(503, 62);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(206, 158);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "オプション項目";
            // 
            // chkEnabledZip
            // 
            this.chkEnabledZip.AutoSize = true;
            this.chkEnabledZip.ForeColor = System.Drawing.Color.Black;
            this.chkEnabledZip.Location = new System.Drawing.Point(12, 118);
            this.chkEnabledZip.Name = "chkEnabledZip";
            this.chkEnabledZip.Size = new System.Drawing.Size(175, 19);
            this.chkEnabledZip.TabIndex = 3;
            this.chkEnabledZip.Text = "発行先フォルダーをzip圧縮する";
            this.toolTip1.SetToolTip(this.chkEnabledZip, "発行先のフォルダーをzip圧縮した状態で出力します。\r\n各種インストーラーや自己解凍形式等で配布する場合はチェックを外して下さい。");
            this.chkEnabledZip.UseVisualStyleBackColor = true;
            // 
            // chkAutorun
            // 
            this.chkAutorun.AutoSize = true;
            this.chkAutorun.ForeColor = System.Drawing.Color.Black;
            this.chkAutorun.Location = new System.Drawing.Point(12, 90);
            this.chkAutorun.Name = "chkAutorun";
            this.chkAutorun.Size = new System.Drawing.Size(187, 19);
            this.chkAutorun.TabIndex = 2;
            this.chkAutorun.Text = "「aurorun.inf」ファイルを生成する";
            this.toolTip1.SetToolTip(this.chkAutorun, "ゲーム作品をCDに焼くときはチェックを入れて下さい。\r\nディスクをコンピューターに挿入したときに自動で起動されるようになります。\r\nアイコンは既定でRPG Dev" +
        "eloper NEXTのゲームプログラムのものになります。");
            this.chkAutorun.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(6, 30);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(103, 15);
            this.label2.TabIndex = 0;
            this.label2.Text = "発行先のフォルダー:";
            // 
            // uctlDestPath
            // 
            this.uctlDestPath.DialogTitle = "発行先のフォルダーを選択して下さい。";
            this.uctlDestPath.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.Folder;
            this.uctlDestPath.EnabledFiles = false;
            this.uctlDestPath.FileFilter = null;
            this.uctlDestPath.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlDestPath.ForeColor = System.Drawing.Color.Black;
            this.uctlDestPath.Location = new System.Drawing.Point(9, 49);
            this.uctlDestPath.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlDestPath.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlDestPath.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlDestPath.Name = "uctlDestPath";
            this.uctlDestPath.Result = "";
            this.uctlDestPath.RootPath = null;
            this.uctlDestPath.Size = new System.Drawing.Size(187, 25);
            this.uctlDestPath.TabIndex = 1;
            this.toolTip1.SetToolTip(this.uctlDestPath, "このフォルダーの中にプロジェクト名のフォルダー/zipファイルが作成されます。");
            // 
            // pnlProcess
            // 
            this.pnlProcess.Controls.Add(this.prgProcess);
            this.pnlProcess.Controls.Add(this.label5);
            this.pnlProcess.Location = new System.Drawing.Point(502, 244);
            this.pnlProcess.Name = "pnlProcess";
            this.pnlProcess.Size = new System.Drawing.Size(207, 74);
            this.pnlProcess.TabIndex = 7;
            this.pnlProcess.Visible = false;
            // 
            // prgProcess
            // 
            this.prgProcess.Location = new System.Drawing.Point(10, 39);
            this.prgProcess.Name = "prgProcess";
            this.prgProcess.Size = new System.Drawing.Size(188, 24);
            this.prgProcess.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.prgProcess.TabIndex = 10;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 12);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(177, 15);
            this.label5.TabIndex = 9;
            this.label5.Text = "処理中です。しばらくお待ち下さい...";
            // 
            // dlgPublish
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(722, 408);
            this.Controls.Add(this.pnlProcess);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.uctlCopyTree);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.uctlOKCancel);
            this.Controls.Add(this.uctlArchiveTree);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgPublish";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "製品版として発行";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.pnlProcess.ResumeLayout(false);
            this.pnlProcess.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private CtrlComponent.Common.ctlFileTree uctlArchiveTree;
		private System.Windows.Forms.Label label1;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.Label label3;
		private CtrlComponent.Common.ctlFileTree uctlCopyTree;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.CheckBox chkEnabledZip;
		private System.Windows.Forms.CheckBox chkAutorun;
		private System.Windows.Forms.Label label2;
		private CtrlComponent.Common.ctlPathSetter uctlDestPath;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.Panel pnlProcess;
		private System.Windows.Forms.ProgressBar prgProcess;
		private System.Windows.Forms.Label label5;
	}
}