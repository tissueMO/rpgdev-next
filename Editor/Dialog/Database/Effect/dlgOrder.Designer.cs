namespace Editor.Dialog.Database.Effect {
	partial class dlgOrder {
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
			this.dlgSetColor = new System.Windows.Forms.ColorDialog();
			this.numFlashCount = new System.Windows.Forms.NumericUpDown();
			this.Label6 = new System.Windows.Forms.Label();
			this.numFlashStrength = new System.Windows.Forms.NumericUpDown();
			this.Label5 = new System.Windows.Forms.Label();
			this.pnlFlashColor = new System.Windows.Forms.Panel();
			this.Label4 = new System.Windows.Forms.Label();
			this.numShakeStrength = new System.Windows.Forms.NumericUpDown();
			this.Label3 = new System.Windows.Forms.Label();
			this.rdbShake = new System.Windows.Forms.RadioButton();
			this.numTargetFrame = new System.Windows.Forms.NumericUpDown();
			this.rdbFlash = new System.Windows.Forms.RadioButton();
			this.cmbSEList = new System.Windows.Forms.ComboBox();
			this.rdbScript = new System.Windows.Forms.RadioButton();
			this.rdbSE = new System.Windows.Forms.RadioButton();
			this.Label2 = new System.Windows.Forms.Label();
			this.GroupBox1 = new System.Windows.Forms.GroupBox();
			this.rdbWaitBreak = new System.Windows.Forms.RadioButton();
			this.pnlScript = new System.Windows.Forms.Panel();
			this.cmbCommand = new System.Windows.Forms.ComboBox();
			this.pnlFlash = new System.Windows.Forms.Panel();
			this.pnlShake = new System.Windows.Forms.Panel();
			this.pnlSE = new System.Windows.Forms.Panel();
			this.uctlSEEditor = new Editor.CtrlComponent.Common.ctlSoundEditor();
			this.tips = new System.Windows.Forms.ToolTip(this.components);
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			((System.ComponentModel.ISupportInitialize)(this.numFlashCount)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numFlashStrength)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numShakeStrength)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numTargetFrame)).BeginInit();
			this.GroupBox1.SuspendLayout();
			this.pnlScript.SuspendLayout();
			this.pnlFlash.SuspendLayout();
			this.pnlShake.SuspendLayout();
			this.pnlSE.SuspendLayout();
			this.SuspendLayout();
			// 
			// dlgSetColor
			// 
			this.dlgSetColor.Color = System.Drawing.Color.White;
			this.dlgSetColor.FullOpen = true;
			// 
			// numFlashCount
			// 
			this.numFlashCount.Location = new System.Drawing.Point(241, 0);
			this.numFlashCount.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
			this.numFlashCount.Name = "numFlashCount";
			this.numFlashCount.Size = new System.Drawing.Size(55, 23);
			this.numFlashCount.TabIndex = 5;
			this.numFlashCount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// Label6
			// 
			this.Label6.AutoSize = true;
			this.Label6.ForeColor = System.Drawing.Color.Black;
			this.Label6.Location = new System.Drawing.Point(200, 4);
			this.Label6.Name = "Label6";
			this.Label6.Size = new System.Drawing.Size(36, 15);
			this.Label6.TabIndex = 4;
			this.Label6.Text = "回数:";
			// 
			// numFlashStrength
			// 
			this.numFlashStrength.Location = new System.Drawing.Point(130, 0);
			this.numFlashStrength.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
			this.numFlashStrength.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numFlashStrength.Name = "numFlashStrength";
			this.numFlashStrength.Size = new System.Drawing.Size(55, 23);
			this.numFlashStrength.TabIndex = 3;
			this.numFlashStrength.Value = new decimal(new int[] {
            255,
            0,
            0,
            0});
			// 
			// Label5
			// 
			this.Label5.AutoSize = true;
			this.Label5.ForeColor = System.Drawing.Color.Black;
			this.Label5.Location = new System.Drawing.Point(93, 4);
			this.Label5.Name = "Label5";
			this.Label5.Size = new System.Drawing.Size(32, 15);
			this.Label5.TabIndex = 2;
			this.Label5.Text = "強さ:";
			// 
			// pnlFlashColor
			// 
			this.pnlFlashColor.BackColor = System.Drawing.Color.White;
			this.pnlFlashColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pnlFlashColor.Cursor = System.Windows.Forms.Cursors.Hand;
			this.pnlFlashColor.Location = new System.Drawing.Point(33, 0);
			this.pnlFlashColor.Name = "pnlFlashColor";
			this.pnlFlashColor.Size = new System.Drawing.Size(45, 23);
			this.pnlFlashColor.TabIndex = 1;
			this.pnlFlashColor.MouseClick += new System.Windows.Forms.MouseEventHandler(this.P_FlashColor_MouseClick);
			// 
			// Label4
			// 
			this.Label4.AutoSize = true;
			this.Label4.ForeColor = System.Drawing.Color.Black;
			this.Label4.Location = new System.Drawing.Point(4, 4);
			this.Label4.Name = "Label4";
			this.Label4.Size = new System.Drawing.Size(24, 15);
			this.Label4.TabIndex = 0;
			this.Label4.Text = "色:";
			// 
			// numShakeStrength
			// 
			this.numShakeStrength.Location = new System.Drawing.Point(41, 0);
			this.numShakeStrength.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
			this.numShakeStrength.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numShakeStrength.Name = "numShakeStrength";
			this.numShakeStrength.Size = new System.Drawing.Size(60, 23);
			this.numShakeStrength.TabIndex = 1;
			this.numShakeStrength.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
			// 
			// Label3
			// 
			this.Label3.AutoSize = true;
			this.Label3.ForeColor = System.Drawing.Color.Black;
			this.Label3.Location = new System.Drawing.Point(4, 4);
			this.Label3.Name = "Label3";
			this.Label3.Size = new System.Drawing.Size(32, 15);
			this.Label3.TabIndex = 0;
			this.Label3.Text = "強さ:";
			// 
			// rdbShake
			// 
			this.rdbShake.AutoSize = true;
			this.rdbShake.ForeColor = System.Drawing.Color.Black;
			this.rdbShake.Location = new System.Drawing.Point(15, 145);
			this.rdbShake.Name = "rdbShake";
			this.rdbShake.Size = new System.Drawing.Size(107, 19);
			this.rdbShake.TabIndex = 1;
			this.rdbShake.TabStop = true;
			this.rdbShake.Text = "画面の減衰振動";
			this.rdbShake.UseVisualStyleBackColor = true;
			this.rdbShake.CheckedChanged += new System.EventHandler(this.rdb_CheckedChanged);
			// 
			// numTargetFrame
			// 
			this.numTargetFrame.Enabled = false;
			this.numTargetFrame.Location = new System.Drawing.Point(97, 13);
			this.numTargetFrame.Maximum = new decimal(new int[] {
            3600,
            0,
            0,
            0});
			this.numTargetFrame.Name = "numTargetFrame";
			this.numTargetFrame.Size = new System.Drawing.Size(105, 23);
			this.numTargetFrame.TabIndex = 1;
			this.numTargetFrame.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// rdbFlash
			// 
			this.rdbFlash.AutoSize = true;
			this.rdbFlash.ForeColor = System.Drawing.Color.Black;
			this.rdbFlash.Location = new System.Drawing.Point(15, 211);
			this.rdbFlash.Name = "rdbFlash";
			this.rdbFlash.Size = new System.Drawing.Size(100, 19);
			this.rdbFlash.TabIndex = 3;
			this.rdbFlash.TabStop = true;
			this.rdbFlash.Text = "画面のフラッシュ";
			this.rdbFlash.UseVisualStyleBackColor = true;
			this.rdbFlash.CheckedChanged += new System.EventHandler(this.rdb_CheckedChanged);
			// 
			// cmbSEList
			// 
			this.cmbSEList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbSEList.ForeColor = System.Drawing.Color.Black;
			this.cmbSEList.FormattingEnabled = true;
			this.cmbSEList.Location = new System.Drawing.Point(0, 0);
			this.cmbSEList.Name = "cmbSEList";
			this.cmbSEList.Size = new System.Drawing.Size(293, 23);
			this.cmbSEList.TabIndex = 0;
			this.cmbSEList.SelectedIndexChanged += new System.EventHandler(this.cmbSEList_SelectedIndexChanged);
			// 
			// rdbScript
			// 
			this.rdbScript.AutoSize = true;
			this.rdbScript.ForeColor = System.Drawing.Color.Black;
			this.rdbScript.Location = new System.Drawing.Point(15, 309);
			this.rdbScript.Name = "rdbScript";
			this.rdbScript.Size = new System.Drawing.Size(136, 19);
			this.rdbScript.TabIndex = 6;
			this.rdbScript.TabStop = true;
			this.rdbScript.Text = "独自コマンドを呼び出す";
			this.rdbScript.UseVisualStyleBackColor = true;
			this.rdbScript.CheckedChanged += new System.EventHandler(this.rdb_CheckedChanged);
			// 
			// rdbSE
			// 
			this.rdbSE.AutoSize = true;
			this.rdbSE.ForeColor = System.Drawing.Color.Black;
			this.rdbSE.Location = new System.Drawing.Point(15, 26);
			this.rdbSE.Name = "rdbSE";
			this.rdbSE.Size = new System.Drawing.Size(92, 19);
			this.rdbSE.TabIndex = 0;
			this.rdbSE.TabStop = true;
			this.rdbSE.Text = "SEを再生する";
			this.rdbSE.UseVisualStyleBackColor = true;
			this.rdbSE.CheckedChanged += new System.EventHandler(this.rdb_CheckedChanged);
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Location = new System.Drawing.Point(12, 17);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(73, 15);
			this.Label2.TabIndex = 0;
			this.Label2.Text = "対象フレーム:";
			// 
			// GroupBox1
			// 
			this.GroupBox1.Controls.Add(this.rdbWaitBreak);
			this.GroupBox1.Controls.Add(this.pnlScript);
			this.GroupBox1.Controls.Add(this.pnlFlash);
			this.GroupBox1.Controls.Add(this.pnlShake);
			this.GroupBox1.Controls.Add(this.rdbShake);
			this.GroupBox1.Controls.Add(this.rdbFlash);
			this.GroupBox1.Controls.Add(this.rdbScript);
			this.GroupBox1.Controls.Add(this.rdbSE);
			this.GroupBox1.Controls.Add(this.pnlSE);
			this.GroupBox1.ForeColor = System.Drawing.Color.Blue;
			this.GroupBox1.Location = new System.Drawing.Point(14, 48);
			this.GroupBox1.Name = "GroupBox1";
			this.GroupBox1.Size = new System.Drawing.Size(343, 365);
			this.GroupBox1.TabIndex = 2;
			this.GroupBox1.TabStop = false;
			this.GroupBox1.Text = "命令の内容";
			// 
			// rdbWaitBreak
			// 
			this.rdbWaitBreak.AutoSize = true;
			this.rdbWaitBreak.ForeColor = System.Drawing.Color.Black;
			this.rdbWaitBreak.Location = new System.Drawing.Point(15, 279);
			this.rdbWaitBreak.Name = "rdbWaitBreak";
			this.rdbWaitBreak.Size = new System.Drawing.Size(140, 19);
			this.rdbWaitBreak.TabIndex = 5;
			this.rdbWaitBreak.TabStop = true;
			this.rdbWaitBreak.Text = "同期再生の待機を解除";
			this.tips.SetToolTip(this.rdbWaitBreak, "エフェクトの再生終了まで待つように指定されているときに、その待機をこのフレームで解除します。\r\nこれは、エフェクトとダメージモーションがスムーズに繋がるようにする" +
        "ために使用します。");
			this.rdbWaitBreak.UseVisualStyleBackColor = true;
			this.rdbWaitBreak.CheckedChanged += new System.EventHandler(this.rdb_CheckedChanged);
			// 
			// pnlScript
			// 
			this.pnlScript.Controls.Add(this.cmbCommand);
			this.pnlScript.Location = new System.Drawing.Point(34, 333);
			this.pnlScript.Name = "pnlScript";
			this.pnlScript.Size = new System.Drawing.Size(293, 29);
			this.pnlScript.TabIndex = 7;
			// 
			// cmbCommand
			// 
			this.cmbCommand.FormattingEnabled = true;
			this.cmbCommand.Location = new System.Drawing.Point(0, 0);
			this.cmbCommand.Name = "cmbCommand";
			this.cmbCommand.Size = new System.Drawing.Size(293, 23);
			this.cmbCommand.TabIndex = 0;
			// 
			// pnlFlash
			// 
			this.pnlFlash.Controls.Add(this.Label4);
			this.pnlFlash.Controls.Add(this.pnlFlashColor);
			this.pnlFlash.Controls.Add(this.Label6);
			this.pnlFlash.Controls.Add(this.numFlashCount);
			this.pnlFlash.Controls.Add(this.Label5);
			this.pnlFlash.Controls.Add(this.numFlashStrength);
			this.pnlFlash.Location = new System.Drawing.Point(34, 236);
			this.pnlFlash.Name = "pnlFlash";
			this.pnlFlash.Size = new System.Drawing.Size(303, 31);
			this.pnlFlash.TabIndex = 4;
			// 
			// pnlShake
			// 
			this.pnlShake.Controls.Add(this.numShakeStrength);
			this.pnlShake.Controls.Add(this.Label3);
			this.pnlShake.Location = new System.Drawing.Point(34, 169);
			this.pnlShake.Name = "pnlShake";
			this.pnlShake.Size = new System.Drawing.Size(112, 32);
			this.pnlShake.TabIndex = 2;
			// 
			// pnlSE
			// 
			this.pnlSE.Controls.Add(this.cmbSEList);
			this.pnlSE.Controls.Add(this.uctlSEEditor);
			this.pnlSE.Location = new System.Drawing.Point(34, 51);
			this.pnlSE.Name = "pnlSE";
			this.pnlSE.Size = new System.Drawing.Size(293, 89);
			this.pnlSE.TabIndex = 1;
			// 
			// uctlSEEditor
			// 
			this.uctlSEEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlSEEditor.Location = new System.Drawing.Point(3, 26);
			this.uctlSEEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlSEEditor.MinimumSize = new System.Drawing.Size(0, 60);
			this.uctlSEEditor.Name = "uctlSEEditor";
			this.uctlSEEditor.Size = new System.Drawing.Size(287, 60);
			this.uctlSEEditor.TabIndex = 1;
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.uctlOKCancel.DisenabledAcceptButton = false;
			this.uctlOKCancel.DisenabledCancelButton = false;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(80, 419);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.OKTitle = "OK";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 3;
			this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// dlgOrder
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(371, 457);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.numTargetFrame);
			this.Controls.Add(this.Label2);
			this.Controls.Add(this.GroupBox1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgOrder";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "エフェクト命令";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgOrder_FormClosing);
			((System.ComponentModel.ISupportInitialize)(this.numFlashCount)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numFlashStrength)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numShakeStrength)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numTargetFrame)).EndInit();
			this.GroupBox1.ResumeLayout(false);
			this.GroupBox1.PerformLayout();
			this.pnlScript.ResumeLayout(false);
			this.pnlFlash.ResumeLayout(false);
			this.pnlFlash.PerformLayout();
			this.pnlShake.ResumeLayout(false);
			this.pnlShake.PerformLayout();
			this.pnlSE.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.ColorDialog dlgSetColor;
		private System.Windows.Forms.NumericUpDown numFlashCount;
		private System.Windows.Forms.Label Label6;
		private System.Windows.Forms.NumericUpDown numFlashStrength;
		private System.Windows.Forms.Label Label5;
		private System.Windows.Forms.Panel pnlFlashColor;
		private System.Windows.Forms.Label Label4;
		private System.Windows.Forms.NumericUpDown numShakeStrength;
		private System.Windows.Forms.Label Label3;
		private System.Windows.Forms.RadioButton rdbShake;
		private System.Windows.Forms.NumericUpDown numTargetFrame;
		private System.Windows.Forms.RadioButton rdbFlash;
		private System.Windows.Forms.ComboBox cmbSEList;
		private System.Windows.Forms.RadioButton rdbScript;
		private System.Windows.Forms.RadioButton rdbSE;
		private System.Windows.Forms.Label Label2;
		private System.Windows.Forms.GroupBox GroupBox1;
		private System.Windows.Forms.Panel pnlScript;
		private System.Windows.Forms.ComboBox cmbCommand;
		private System.Windows.Forms.Panel pnlFlash;
		private System.Windows.Forms.Panel pnlShake;
		private System.Windows.Forms.Panel pnlSE;
		private System.Windows.Forms.ToolTip tips;
		private CtrlComponent.Common.ctlSoundEditor uctlSEEditor;
		private System.Windows.Forms.RadioButton rdbWaitBreak;
	}
}