namespace Editor.Dialog.Text.Script {
	partial class dlgEVcmdMoveTask {
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
            this.lstTasks = new System.Windows.Forms.ListBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnMoveAgainstPlayer = new System.Windows.Forms.Button();
            this.btnMoveToPlayer = new System.Windows.Forms.Button();
            this.btnMoveRandom = new System.Windows.Forms.Button();
            this.btnMoveBack = new System.Windows.Forms.Button();
            this.btnMoveForward = new System.Windows.Forms.Button();
            this.btnMoveWest = new System.Windows.Forms.Button();
            this.btnMoveEast = new System.Windows.Forms.Button();
            this.btnMoveSouth = new System.Windows.Forms.Button();
            this.btnMoveNorth = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnDirBack = new System.Windows.Forms.Button();
            this.btnDirAgainstPlayer = new System.Windows.Forms.Button();
            this.btnDirRight = new System.Windows.Forms.Button();
            this.btnDirRandom = new System.Windows.Forms.Button();
            this.btnDirToPlayer = new System.Windows.Forms.Button();
            this.btnDirLeft = new System.Windows.Forms.Button();
            this.btnDirWest = new System.Windows.Forms.Button();
            this.btnDirNorth = new System.Windows.Forms.Button();
            this.btnDirEast = new System.Windows.Forms.Button();
            this.btnDirSouth = new System.Windows.Forms.Button();
            this.chkRepeat = new System.Windows.Forms.CheckBox();
            this.chkRouteLock = new System.Windows.Forms.CheckBox();
            this.chkWaitMode = new System.Windows.Forms.CheckBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.btnHide = new System.Windows.Forms.Button();
            this.btnFixedDirection = new System.Windows.Forms.Button();
            this.btnNoHit = new System.Windows.Forms.Button();
            this.btnWalkAnim = new System.Windows.Forms.Button();
            this.btnStopAnim = new System.Windows.Forms.Button();
            this.chkValue = new System.Windows.Forms.CheckBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.numArg = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.btnChangeGraph = new System.Windows.Forms.Button();
            this.uctlGraphSetter = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.btnSQ = new System.Windows.Forms.Button();
            this.btnWait = new System.Windows.Forms.Button();
            this.numWaitTime = new System.Windows.Forms.NumericUpDown();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.btnMoveFrequency = new System.Windows.Forms.Button();
            this.cmbSpeed = new System.Windows.Forms.ComboBox();
            this.btnMoveSpeed = new System.Windows.Forms.Button();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.操作ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuUp = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDown = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numArg)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numWaitTime)).BeginInit();
            this.groupBox6.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lstTasks
            // 
            this.lstTasks.FormattingEnabled = true;
            this.lstTasks.ItemHeight = 15;
            this.lstTasks.Location = new System.Drawing.Point(12, 12);
            this.lstTasks.Name = "lstTasks";
            this.lstTasks.Size = new System.Drawing.Size(208, 274);
            this.lstTasks.TabIndex = 0;
            this.toolTip1.SetToolTip(this.lstTasks, "Ctrlキー＋上下キーで項目を前後移動します。\r\nDeleteキーで項目を削除します。");
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnMoveAgainstPlayer);
            this.groupBox1.Controls.Add(this.btnMoveToPlayer);
            this.groupBox1.Controls.Add(this.btnMoveRandom);
            this.groupBox1.Controls.Add(this.btnMoveBack);
            this.groupBox1.Controls.Add(this.btnMoveForward);
            this.groupBox1.Controls.Add(this.btnMoveWest);
            this.groupBox1.Controls.Add(this.btnMoveEast);
            this.groupBox1.Controls.Add(this.btnMoveSouth);
            this.groupBox1.Controls.Add(this.btnMoveNorth);
            this.groupBox1.ForeColor = System.Drawing.Color.Blue;
            this.groupBox1.Location = new System.Drawing.Point(235, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(332, 114);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "移動コマンド";
            // 
            // btnMoveAgainstPlayer
            // 
            this.btnMoveAgainstPlayer.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveAgainstPlayer.Location = new System.Drawing.Point(212, 49);
            this.btnMoveAgainstPlayer.Name = "btnMoveAgainstPlayer";
            this.btnMoveAgainstPlayer.Size = new System.Drawing.Size(110, 28);
            this.btnMoveAgainstPlayer.TabIndex = 7;
            this.btnMoveAgainstPlayer.Text = "主人公から離れる";
            this.btnMoveAgainstPlayer.UseVisualStyleBackColor = true;
            this.btnMoveAgainstPlayer.Click += new System.EventHandler(this.btnMoveAgainstPlayer_Click);
            // 
            // btnMoveToPlayer
            // 
            this.btnMoveToPlayer.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveToPlayer.Location = new System.Drawing.Point(101, 49);
            this.btnMoveToPlayer.Name = "btnMoveToPlayer";
            this.btnMoveToPlayer.Size = new System.Drawing.Size(110, 28);
            this.btnMoveToPlayer.TabIndex = 6;
            this.btnMoveToPlayer.Text = "主人公へ近づく";
            this.btnMoveToPlayer.UseVisualStyleBackColor = true;
            this.btnMoveToPlayer.Click += new System.EventHandler(this.btnMoveToPlayer_Click);
            // 
            // btnMoveRandom
            // 
            this.btnMoveRandom.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveRandom.Location = new System.Drawing.Point(101, 77);
            this.btnMoveRandom.Name = "btnMoveRandom";
            this.btnMoveRandom.Size = new System.Drawing.Size(110, 28);
            this.btnMoveRandom.TabIndex = 8;
            this.btnMoveRandom.Text = "ランダム一歩";
            this.btnMoveRandom.UseVisualStyleBackColor = true;
            this.btnMoveRandom.Click += new System.EventHandler(this.btnMoveRandom_Click);
            // 
            // btnMoveBack
            // 
            this.btnMoveBack.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveBack.Location = new System.Drawing.Point(212, 21);
            this.btnMoveBack.Name = "btnMoveBack";
            this.btnMoveBack.Size = new System.Drawing.Size(110, 28);
            this.btnMoveBack.TabIndex = 5;
            this.btnMoveBack.Text = "一歩後退";
            this.btnMoveBack.UseVisualStyleBackColor = true;
            this.btnMoveBack.Click += new System.EventHandler(this.btnMoveBack_Click);
            // 
            // btnMoveForward
            // 
            this.btnMoveForward.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveForward.Location = new System.Drawing.Point(101, 21);
            this.btnMoveForward.Name = "btnMoveForward";
            this.btnMoveForward.Size = new System.Drawing.Size(110, 28);
            this.btnMoveForward.TabIndex = 4;
            this.btnMoveForward.Text = "一歩前進";
            this.btnMoveForward.UseVisualStyleBackColor = true;
            this.btnMoveForward.Click += new System.EventHandler(this.btnMoveForward_Click);
            // 
            // btnMoveWest
            // 
            this.btnMoveWest.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnMoveWest.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveWest.Location = new System.Drawing.Point(10, 49);
            this.btnMoveWest.Name = "btnMoveWest";
            this.btnMoveWest.Size = new System.Drawing.Size(28, 28);
            this.btnMoveWest.TabIndex = 1;
            this.btnMoveWest.Text = "←";
            this.btnMoveWest.UseVisualStyleBackColor = true;
            this.btnMoveWest.Click += new System.EventHandler(this.btnMoveWest_Click);
            // 
            // btnMoveEast
            // 
            this.btnMoveEast.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnMoveEast.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveEast.Location = new System.Drawing.Point(64, 49);
            this.btnMoveEast.Name = "btnMoveEast";
            this.btnMoveEast.Size = new System.Drawing.Size(28, 28);
            this.btnMoveEast.TabIndex = 2;
            this.btnMoveEast.Text = "→";
            this.btnMoveEast.UseVisualStyleBackColor = true;
            this.btnMoveEast.Click += new System.EventHandler(this.btnMoveEast_Click);
            // 
            // btnMoveSouth
            // 
            this.btnMoveSouth.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnMoveSouth.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveSouth.Location = new System.Drawing.Point(37, 76);
            this.btnMoveSouth.Name = "btnMoveSouth";
            this.btnMoveSouth.Size = new System.Drawing.Size(28, 28);
            this.btnMoveSouth.TabIndex = 3;
            this.btnMoveSouth.Text = "↓";
            this.btnMoveSouth.UseVisualStyleBackColor = true;
            this.btnMoveSouth.Click += new System.EventHandler(this.btnMoveSouth_Click);
            // 
            // btnMoveNorth
            // 
            this.btnMoveNorth.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnMoveNorth.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveNorth.Location = new System.Drawing.Point(37, 22);
            this.btnMoveNorth.Name = "btnMoveNorth";
            this.btnMoveNorth.Size = new System.Drawing.Size(28, 28);
            this.btnMoveNorth.TabIndex = 0;
            this.btnMoveNorth.Text = "↑";
            this.btnMoveNorth.UseVisualStyleBackColor = true;
            this.btnMoveNorth.Click += new System.EventHandler(this.btnMoveNorth_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnDirBack);
            this.groupBox2.Controls.Add(this.btnDirAgainstPlayer);
            this.groupBox2.Controls.Add(this.btnDirRight);
            this.groupBox2.Controls.Add(this.btnDirRandom);
            this.groupBox2.Controls.Add(this.btnDirToPlayer);
            this.groupBox2.Controls.Add(this.btnDirLeft);
            this.groupBox2.Controls.Add(this.btnDirWest);
            this.groupBox2.Controls.Add(this.btnDirNorth);
            this.groupBox2.Controls.Add(this.btnDirEast);
            this.groupBox2.Controls.Add(this.btnDirSouth);
            this.groupBox2.ForeColor = System.Drawing.Color.Blue;
            this.groupBox2.Location = new System.Drawing.Point(235, 132);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(332, 116);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "方向コマンド";
            // 
            // btnDirBack
            // 
            this.btnDirBack.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirBack.Location = new System.Drawing.Point(212, 77);
            this.btnDirBack.Name = "btnDirBack";
            this.btnDirBack.Size = new System.Drawing.Size(110, 28);
            this.btnDirBack.TabIndex = 9;
            this.btnDirBack.Text = "後ろを向く";
            this.btnDirBack.UseVisualStyleBackColor = true;
            this.btnDirBack.Click += new System.EventHandler(this.btnDirBack_Click);
            // 
            // btnDirAgainstPlayer
            // 
            this.btnDirAgainstPlayer.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirAgainstPlayer.Location = new System.Drawing.Point(212, 49);
            this.btnDirAgainstPlayer.Name = "btnDirAgainstPlayer";
            this.btnDirAgainstPlayer.Size = new System.Drawing.Size(110, 28);
            this.btnDirAgainstPlayer.TabIndex = 7;
            this.btnDirAgainstPlayer.Text = "主人公から背ける";
            this.btnDirAgainstPlayer.UseVisualStyleBackColor = true;
            this.btnDirAgainstPlayer.Click += new System.EventHandler(this.btnDirAgainstPlayer_Click);
            // 
            // btnDirRight
            // 
            this.btnDirRight.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirRight.Location = new System.Drawing.Point(212, 21);
            this.btnDirRight.Name = "btnDirRight";
            this.btnDirRight.Size = new System.Drawing.Size(110, 28);
            this.btnDirRight.TabIndex = 5;
            this.btnDirRight.Text = "右を向く";
            this.btnDirRight.UseVisualStyleBackColor = true;
            this.btnDirRight.Click += new System.EventHandler(this.btnDirRight_Click);
            // 
            // btnDirRandom
            // 
            this.btnDirRandom.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirRandom.Location = new System.Drawing.Point(101, 77);
            this.btnDirRandom.Name = "btnDirRandom";
            this.btnDirRandom.Size = new System.Drawing.Size(110, 28);
            this.btnDirRandom.TabIndex = 8;
            this.btnDirRandom.Text = "ランダム方向";
            this.btnDirRandom.UseVisualStyleBackColor = true;
            this.btnDirRandom.Click += new System.EventHandler(this.btnDirRandom_Click);
            // 
            // btnDirToPlayer
            // 
            this.btnDirToPlayer.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirToPlayer.Location = new System.Drawing.Point(101, 49);
            this.btnDirToPlayer.Name = "btnDirToPlayer";
            this.btnDirToPlayer.Size = new System.Drawing.Size(110, 28);
            this.btnDirToPlayer.TabIndex = 6;
            this.btnDirToPlayer.Text = "主人公を向く";
            this.btnDirToPlayer.UseVisualStyleBackColor = true;
            this.btnDirToPlayer.Click += new System.EventHandler(this.btnDirToPlayer_Click);
            // 
            // btnDirLeft
            // 
            this.btnDirLeft.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirLeft.Location = new System.Drawing.Point(101, 21);
            this.btnDirLeft.Name = "btnDirLeft";
            this.btnDirLeft.Size = new System.Drawing.Size(110, 28);
            this.btnDirLeft.TabIndex = 4;
            this.btnDirLeft.Text = "左を向く";
            this.btnDirLeft.UseVisualStyleBackColor = true;
            this.btnDirLeft.Click += new System.EventHandler(this.btnDirLeft_Click);
            // 
            // btnDirWest
            // 
            this.btnDirWest.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnDirWest.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirWest.Location = new System.Drawing.Point(10, 49);
            this.btnDirWest.Name = "btnDirWest";
            this.btnDirWest.Size = new System.Drawing.Size(28, 28);
            this.btnDirWest.TabIndex = 1;
            this.btnDirWest.Text = "←";
            this.btnDirWest.UseVisualStyleBackColor = true;
            this.btnDirWest.Click += new System.EventHandler(this.btnDirWest_Click);
            // 
            // btnDirNorth
            // 
            this.btnDirNorth.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnDirNorth.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirNorth.Location = new System.Drawing.Point(37, 22);
            this.btnDirNorth.Name = "btnDirNorth";
            this.btnDirNorth.Size = new System.Drawing.Size(28, 28);
            this.btnDirNorth.TabIndex = 0;
            this.btnDirNorth.Text = "↑";
            this.btnDirNorth.UseVisualStyleBackColor = true;
            this.btnDirNorth.Click += new System.EventHandler(this.btnDirNorth_Click);
            // 
            // btnDirEast
            // 
            this.btnDirEast.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnDirEast.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirEast.Location = new System.Drawing.Point(64, 49);
            this.btnDirEast.Name = "btnDirEast";
            this.btnDirEast.Size = new System.Drawing.Size(28, 28);
            this.btnDirEast.TabIndex = 2;
            this.btnDirEast.Text = "→";
            this.btnDirEast.UseVisualStyleBackColor = true;
            this.btnDirEast.Click += new System.EventHandler(this.btnDirEast_Click);
            // 
            // btnDirSouth
            // 
            this.btnDirSouth.Font = new System.Drawing.Font("Meiryo UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnDirSouth.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnDirSouth.Location = new System.Drawing.Point(37, 76);
            this.btnDirSouth.Name = "btnDirSouth";
            this.btnDirSouth.Size = new System.Drawing.Size(28, 28);
            this.btnDirSouth.TabIndex = 3;
            this.btnDirSouth.Text = "↓";
            this.btnDirSouth.UseVisualStyleBackColor = true;
            this.btnDirSouth.Click += new System.EventHandler(this.btnDirSouth_Click);
            // 
            // chkRepeat
            // 
            this.chkRepeat.AutoSize = true;
            this.chkRepeat.Location = new System.Drawing.Point(27, 303);
            this.chkRepeat.Name = "chkRepeat";
            this.chkRepeat.Size = new System.Drawing.Size(101, 19);
            this.chkRepeat.TabIndex = 1;
            this.chkRepeat.Text = "動作を繰り返す";
            this.chkRepeat.UseVisualStyleBackColor = true;
            // 
            // chkRouteLock
            // 
            this.chkRouteLock.AutoSize = true;
            this.chkRouteLock.Location = new System.Drawing.Point(27, 328);
            this.chkRouteLock.Name = "chkRouteLock";
            this.chkRouteLock.Size = new System.Drawing.Size(167, 19);
            this.chkRouteLock.TabIndex = 2;
            this.chkRouteLock.Text = "移動できない場合は次へ進む";
            this.toolTip1.SetToolTip(this.chkRouteLock, "チェックを外した場合、障害物がなくなるまで待機します。");
            this.chkRouteLock.UseVisualStyleBackColor = true;
            // 
            // chkWaitMode
            // 
            this.chkWaitMode.AutoSize = true;
            this.chkWaitMode.Checked = true;
            this.chkWaitMode.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkWaitMode.Location = new System.Drawing.Point(27, 353);
            this.chkWaitMode.Name = "chkWaitMode";
            this.chkWaitMode.Size = new System.Drawing.Size(131, 19);
            this.chkWaitMode.TabIndex = 3;
            this.chkWaitMode.Text = "移動が終わるまで待つ";
            this.chkWaitMode.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.btnHide);
            this.groupBox4.Controls.Add(this.btnFixedDirection);
            this.groupBox4.Controls.Add(this.btnNoHit);
            this.groupBox4.Controls.Add(this.btnWalkAnim);
            this.groupBox4.Controls.Add(this.btnStopAnim);
            this.groupBox4.Controls.Add(this.chkValue);
            this.groupBox4.ForeColor = System.Drawing.Color.Blue;
            this.groupBox4.Location = new System.Drawing.Point(577, 132);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(122, 200);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "フラグ切替";
            // 
            // btnHide
            // 
            this.btnHide.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnHide.Location = new System.Drawing.Point(10, 161);
            this.btnHide.Name = "btnHide";
            this.btnHide.Size = new System.Drawing.Size(100, 28);
            this.btnHide.TabIndex = 5;
            this.btnHide.Text = "非表示";
            this.toolTip1.SetToolTip(this.btnHide, "非表示にすると、実質的には「すり抜け」と同じ状態になります。");
            this.btnHide.UseVisualStyleBackColor = true;
            this.btnHide.Click += new System.EventHandler(this.btnHide_Click);
            // 
            // btnFixedDirection
            // 
            this.btnFixedDirection.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnFixedDirection.Location = new System.Drawing.Point(10, 133);
            this.btnFixedDirection.Name = "btnFixedDirection";
            this.btnFixedDirection.Size = new System.Drawing.Size(100, 28);
            this.btnFixedDirection.TabIndex = 4;
            this.btnFixedDirection.Text = "向き固定";
            this.btnFixedDirection.UseVisualStyleBackColor = true;
            this.btnFixedDirection.Click += new System.EventHandler(this.btnFixedDirection_Click);
            // 
            // btnNoHit
            // 
            this.btnNoHit.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnNoHit.Location = new System.Drawing.Point(10, 105);
            this.btnNoHit.Name = "btnNoHit";
            this.btnNoHit.Size = new System.Drawing.Size(100, 28);
            this.btnNoHit.TabIndex = 3;
            this.btnNoHit.Text = "すり抜け";
            this.btnNoHit.UseVisualStyleBackColor = true;
            this.btnNoHit.Click += new System.EventHandler(this.btnNoHit_Click);
            // 
            // btnWalkAnim
            // 
            this.btnWalkAnim.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnWalkAnim.Location = new System.Drawing.Point(10, 77);
            this.btnWalkAnim.Name = "btnWalkAnim";
            this.btnWalkAnim.Size = new System.Drawing.Size(100, 28);
            this.btnWalkAnim.TabIndex = 2;
            this.btnWalkAnim.Text = "歩行アニメ";
            this.btnWalkAnim.UseVisualStyleBackColor = true;
            this.btnWalkAnim.Click += new System.EventHandler(this.btnWalkAnim_Click);
            // 
            // btnStopAnim
            // 
            this.btnStopAnim.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnStopAnim.Location = new System.Drawing.Point(10, 49);
            this.btnStopAnim.Name = "btnStopAnim";
            this.btnStopAnim.Size = new System.Drawing.Size(100, 28);
            this.btnStopAnim.TabIndex = 1;
            this.btnStopAnim.Text = "静止アニメ";
            this.btnStopAnim.UseVisualStyleBackColor = true;
            this.btnStopAnim.Click += new System.EventHandler(this.btnStopAnim_Click);
            // 
            // chkValue
            // 
            this.chkValue.AutoSize = true;
            this.chkValue.ForeColor = System.Drawing.SystemColors.ControlText;
            this.chkValue.Location = new System.Drawing.Point(21, 25);
            this.chkValue.Name = "chkValue";
            this.chkValue.Size = new System.Drawing.Size(72, 19);
            this.chkValue.TabIndex = 0;
            this.chkValue.Text = "ONにする";
            this.chkValue.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.numArg);
            this.groupBox5.Controls.Add(this.label2);
            this.groupBox5.Controls.Add(this.btnChangeGraph);
            this.groupBox5.Controls.Add(this.uctlGraphSetter);
            this.groupBox5.Controls.Add(this.btnSQ);
            this.groupBox5.Controls.Add(this.btnWait);
            this.groupBox5.Controls.Add(this.numWaitTime);
            this.groupBox5.ForeColor = System.Drawing.Color.Blue;
            this.groupBox5.Location = new System.Drawing.Point(235, 254);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(257, 124);
            this.groupBox5.TabIndex = 8;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "その他";
            // 
            // numArg
            // 
            this.numArg.ForeColor = System.Drawing.SystemColors.ControlText;
            this.numArg.Location = new System.Drawing.Point(52, 20);
            this.numArg.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numArg.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numArg.Name = "numArg";
            this.numArg.Size = new System.Drawing.Size(90, 23);
            this.numArg.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label2.Location = new System.Drawing.Point(10, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 15);
            this.label2.TabIndex = 0;
            this.label2.Text = "引数:";
            // 
            // btnChangeGraph
            // 
            this.btnChangeGraph.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnChangeGraph.Location = new System.Drawing.Point(147, 52);
            this.btnChangeGraph.Name = "btnChangeGraph";
            this.btnChangeGraph.Size = new System.Drawing.Size(100, 28);
            this.btnChangeGraph.TabIndex = 4;
            this.btnChangeGraph.Text = "グラフィック変更";
            this.btnChangeGraph.UseVisualStyleBackColor = true;
            this.btnChangeGraph.Click += new System.EventHandler(this.btnChangeGraph_Click);
            // 
            // uctlGraphSetter
            // 
            this.uctlGraphSetter.DialogTitle = "グラフィック変更";
            this.uctlGraphSetter.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlGraphSetter.EnabledFiles = true;
            this.uctlGraphSetter.FileFilter = null;
            this.uctlGraphSetter.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlGraphSetter.ForeColor = System.Drawing.SystemColors.ControlText;
            this.uctlGraphSetter.Location = new System.Drawing.Point(10, 53);
            this.uctlGraphSetter.Margin = new System.Windows.Forms.Padding(0);
            this.uctlGraphSetter.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlGraphSetter.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlGraphSetter.Name = "uctlGraphSetter";
            this.uctlGraphSetter.Result = "";
            this.uctlGraphSetter.RootPath = null;
            this.uctlGraphSetter.Size = new System.Drawing.Size(132, 25);
            this.uctlGraphSetter.TabIndex = 3;
            // 
            // btnSQ
            // 
            this.btnSQ.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnSQ.Location = new System.Drawing.Point(147, 17);
            this.btnSQ.Name = "btnSQ";
            this.btnSQ.Size = new System.Drawing.Size(100, 28);
            this.btnSQ.TabIndex = 2;
            this.btnSQ.Text = "スクリプト呼出";
            this.toolTip1.SetToolTip(this.btnSQ, "スクリプトのイベントハンドラーが呼び出されます。\r\n引数の値に応じて処理を変えるなどの拡張的な処理ができます。");
            this.btnSQ.UseVisualStyleBackColor = true;
            this.btnSQ.Click += new System.EventHandler(this.btnSQ_Click);
            // 
            // btnWait
            // 
            this.btnWait.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnWait.Location = new System.Drawing.Point(87, 86);
            this.btnWait.Name = "btnWait";
            this.btnWait.Size = new System.Drawing.Size(100, 28);
            this.btnWait.TabIndex = 6;
            this.btnWait.Text = "ミリ秒 ウェイト";
            this.btnWait.UseVisualStyleBackColor = true;
            this.btnWait.Click += new System.EventHandler(this.btnWait_Click);
            // 
            // numWaitTime
            // 
            this.numWaitTime.ForeColor = System.Drawing.SystemColors.ControlText;
            this.numWaitTime.Location = new System.Drawing.Point(10, 89);
            this.numWaitTime.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numWaitTime.Name = "numWaitTime";
            this.numWaitTime.Size = new System.Drawing.Size(72, 23);
            this.numWaitTime.TabIndex = 5;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.btnMoveFrequency);
            this.groupBox6.Controls.Add(this.cmbSpeed);
            this.groupBox6.Controls.Add(this.btnMoveSpeed);
            this.groupBox6.ForeColor = System.Drawing.Color.Blue;
            this.groupBox6.Location = new System.Drawing.Point(577, 12);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(122, 114);
            this.groupBox6.TabIndex = 6;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "速さ変更";
            // 
            // btnMoveFrequency
            // 
            this.btnMoveFrequency.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveFrequency.Location = new System.Drawing.Point(10, 77);
            this.btnMoveFrequency.Name = "btnMoveFrequency";
            this.btnMoveFrequency.Size = new System.Drawing.Size(100, 28);
            this.btnMoveFrequency.TabIndex = 2;
            this.btnMoveFrequency.Text = "移動頻度 変更";
            this.btnMoveFrequency.UseVisualStyleBackColor = true;
            this.btnMoveFrequency.Click += new System.EventHandler(this.btnMoveFrequency_Click);
            // 
            // cmbSpeed
            // 
            this.cmbSpeed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbSpeed.ForeColor = System.Drawing.SystemColors.ControlText;
            this.cmbSpeed.FormattingEnabled = true;
            this.cmbSpeed.Location = new System.Drawing.Point(10, 22);
            this.cmbSpeed.Name = "cmbSpeed";
            this.cmbSpeed.Size = new System.Drawing.Size(100, 23);
            this.cmbSpeed.TabIndex = 0;
            // 
            // btnMoveSpeed
            // 
            this.btnMoveSpeed.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnMoveSpeed.Location = new System.Drawing.Point(10, 49);
            this.btnMoveSpeed.Name = "btnMoveSpeed";
            this.btnMoveSpeed.Size = new System.Drawing.Size(100, 28);
            this.btnMoveSpeed.TabIndex = 1;
            this.btnMoveSpeed.Text = "移動速度 変更";
            this.btnMoveSpeed.UseVisualStyleBackColor = true;
            this.btnMoveSpeed.Click += new System.EventHandler(this.btnMoveSpeed_Click);
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(507, 350);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(197, 34);
            this.ctlOKCancel1.TabIndex = 9;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.操作ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(710, 24);
            this.menuStrip1.TabIndex = 10;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.Visible = false;
            // 
            // 操作ToolStripMenuItem
            // 
            this.操作ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuUp,
            this.mnuDown,
            this.mnuDelete});
            this.操作ToolStripMenuItem.Name = "操作ToolStripMenuItem";
            this.操作ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.操作ToolStripMenuItem.Text = "操作";
            // 
            // mnuUp
            // 
            this.mnuUp.Name = "mnuUp";
            this.mnuUp.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Up)));
            this.mnuUp.Size = new System.Drawing.Size(167, 22);
            this.mnuUp.Text = "上へ";
            this.mnuUp.Click += new System.EventHandler(this.mnuUp_Click);
            // 
            // mnuDown
            // 
            this.mnuDown.Name = "mnuDown";
            this.mnuDown.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Down)));
            this.mnuDown.Size = new System.Drawing.Size(167, 22);
            this.mnuDown.Text = "下へ";
            this.mnuDown.Click += new System.EventHandler(this.mnuDown_Click);
            // 
            // mnuDelete
            // 
            this.mnuDelete.Name = "mnuDelete";
            this.mnuDelete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.mnuDelete.Size = new System.Drawing.Size(167, 22);
            this.mnuDelete.Text = "削除";
            this.mnuDelete.Click += new System.EventHandler(this.mnuDelete_Click);
            // 
            // dlgEVcmdMoveTask
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(710, 388);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.chkWaitMode);
            this.Controls.Add(this.chkRouteLock);
            this.Controls.Add(this.chkRepeat);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.lstTasks);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgEVcmdMoveTask";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "移動経路の設定";
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numArg)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numWaitTime)).EndInit();
            this.groupBox6.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListBox lstTasks;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button btnMoveRandom;
		private System.Windows.Forms.Button btnMoveBack;
		private System.Windows.Forms.Button btnMoveForward;
		private System.Windows.Forms.Button btnMoveWest;
		private System.Windows.Forms.Button btnMoveEast;
		private System.Windows.Forms.Button btnMoveSouth;
		private System.Windows.Forms.Button btnMoveNorth;
		private System.Windows.Forms.Button btnDirWest;
		private System.Windows.Forms.Button btnDirNorth;
		private System.Windows.Forms.Button btnDirEast;
		private System.Windows.Forms.Button btnDirSouth;
		private System.Windows.Forms.Button btnDirBack;
		private System.Windows.Forms.Button btnDirRight;
		private System.Windows.Forms.Button btnDirRandom;
		private System.Windows.Forms.Button btnDirLeft;
		private System.Windows.Forms.CheckBox chkWaitMode;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Button btnHide;
		private System.Windows.Forms.Button btnFixedDirection;
		private System.Windows.Forms.Button btnNoHit;
		private System.Windows.Forms.Button btnWalkAnim;
		private System.Windows.Forms.Button btnStopAnim;
		private System.Windows.Forms.CheckBox chkValue;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.NumericUpDown numArg;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button btnChangeGraph;
		private CtrlComponent.Common.ctlPathSetter uctlGraphSetter;
		private System.Windows.Forms.Button btnSQ;
		private System.Windows.Forms.Button btnWait;
		private System.Windows.Forms.NumericUpDown numWaitTime;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.Button btnMoveFrequency;
		private System.Windows.Forms.ComboBox cmbSpeed;
		private System.Windows.Forms.Button btnMoveSpeed;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem 操作ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem mnuUp;
		private System.Windows.Forms.ToolStripMenuItem mnuDown;
		private System.Windows.Forms.ToolStripMenuItem mnuDelete;
		private System.Windows.Forms.ToolTip toolTip1;
		protected System.Windows.Forms.Button btnMoveAgainstPlayer;
		protected System.Windows.Forms.Button btnMoveToPlayer;
		protected System.Windows.Forms.Button btnDirAgainstPlayer;
		protected System.Windows.Forms.Button btnDirToPlayer;
        private System.Windows.Forms.CheckBox chkRepeat;
        private System.Windows.Forms.CheckBox chkRouteLock;
    }
}