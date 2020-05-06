namespace Editor.Dialog.Schedule {
	partial class dlgEditTask {
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.cmbSection = new System.Windows.Forms.ComboBox();
			this.cmbWorker = new System.Windows.Forms.ComboBox();
			this.txtName = new System.Windows.Forms.TextBox();
			this.dtmStart = new System.Windows.Forms.DateTimePicker();
			this.numSpan = new System.Windows.Forms.NumericUpDown();
			this.label9 = new System.Windows.Forms.Label();
			this.pnlColor = new System.Windows.Forms.Panel();
			this.cmbLink = new System.Windows.Forms.ComboBox();
			this.numProgress = new System.Windows.Forms.NumericUpDown();
			this.label10 = new System.Windows.Forms.Label();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.dlgColor = new System.Windows.Forms.ColorDialog();
			((System.ComponentModel.ISupportInitialize)(this.numSpan)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numProgress)).BeginInit();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(16, 17);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(36, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "部門:";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(16, 48);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(48, 15);
			this.label2.TabIndex = 2;
			this.label2.Text = "担当者:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(16, 78);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(49, 15);
			this.label3.TabIndex = 4;
			this.label3.Text = "タスク名:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(16, 139);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(48, 15);
			this.label4.TabIndex = 8;
			this.label4.Text = "開始日:";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(16, 169);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(36, 15);
			this.label5.TabIndex = 10;
			this.label5.Text = "期間:";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(16, 199);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(48, 15);
			this.label6.TabIndex = 13;
			this.label6.Text = "表示色:";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(16, 109);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(48, 15);
			this.label7.TabIndex = 6;
			this.label7.Text = "依存先:";
			this.toolTip1.SetToolTip(this.label7, "指定したタスクが完了してから始めます。");
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(16, 229);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(48, 15);
			this.label8.TabIndex = 15;
			this.label8.Text = "進捗率:";
			// 
			// cmbSection
			// 
			this.cmbSection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbSection.FormattingEnabled = true;
			this.cmbSection.Location = new System.Drawing.Point(73, 13);
			this.cmbSection.Name = "cmbSection";
			this.cmbSection.Size = new System.Drawing.Size(151, 23);
			this.cmbSection.TabIndex = 1;
			this.cmbSection.SelectedIndexChanged += new System.EventHandler(this.cmbSection_SelectedIndexChanged);
			// 
			// cmbWorker
			// 
			this.cmbWorker.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbWorker.FormattingEnabled = true;
			this.cmbWorker.Location = new System.Drawing.Point(73, 44);
			this.cmbWorker.Name = "cmbWorker";
			this.cmbWorker.Size = new System.Drawing.Size(151, 23);
			this.cmbWorker.TabIndex = 3;
			// 
			// txtName
			// 
			this.txtName.Location = new System.Drawing.Point(73, 75);
			this.txtName.Name = "txtName";
			this.txtName.Size = new System.Drawing.Size(151, 23);
			this.txtName.TabIndex = 5;
			// 
			// dtmStart
			// 
			this.dtmStart.Location = new System.Drawing.Point(73, 136);
			this.dtmStart.Name = "dtmStart";
			this.dtmStart.Size = new System.Drawing.Size(151, 23);
			this.dtmStart.TabIndex = 9;
			this.toolTip1.SetToolTip(this.dtmStart, "依存するタスクがある場合は、それらにも影響します。");
			// 
			// numSpan
			// 
			this.numSpan.Location = new System.Drawing.Point(73, 166);
			this.numSpan.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
			this.numSpan.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numSpan.Name = "numSpan";
			this.numSpan.Size = new System.Drawing.Size(80, 23);
			this.numSpan.TabIndex = 11;
			this.toolTip1.SetToolTip(this.numSpan, "依存するタスクがある場合は、それらにも影響します。");
			this.numSpan.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(158, 169);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(19, 15);
			this.label9.TabIndex = 12;
			this.label9.Text = "日";
			// 
			// pnlColor
			// 
			this.pnlColor.BackColor = System.Drawing.Color.RoyalBlue;
			this.pnlColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pnlColor.Cursor = System.Windows.Forms.Cursors.Hand;
			this.pnlColor.Location = new System.Drawing.Point(73, 196);
			this.pnlColor.Name = "pnlColor";
			this.pnlColor.Size = new System.Drawing.Size(80, 25);
			this.pnlColor.TabIndex = 14;
			this.pnlColor.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pnlColor_MouseClick);
			// 
			// cmbLink
			// 
			this.cmbLink.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbLink.DropDownWidth = 300;
			this.cmbLink.FormattingEnabled = true;
			this.cmbLink.Location = new System.Drawing.Point(73, 105);
			this.cmbLink.Name = "cmbLink";
			this.cmbLink.Size = new System.Drawing.Size(151, 23);
			this.cmbLink.TabIndex = 7;
			this.toolTip1.SetToolTip(this.cmbLink, "指定したタスクが完了してから始めます。");
			this.cmbLink.SelectedIndexChanged += new System.EventHandler(this.cmbLink_SelectedIndexChanged);
			// 
			// numProgress
			// 
			this.numProgress.Location = new System.Drawing.Point(73, 226);
			this.numProgress.Name = "numProgress";
			this.numProgress.Size = new System.Drawing.Size(80, 23);
			this.numProgress.TabIndex = 16;
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(158, 229);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(19, 15);
			this.label10.TabIndex = 17;
			this.label10.Text = "％";
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(15, 269);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 18;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// dlgColor
			// 
			this.dlgColor.AnyColor = true;
			this.dlgColor.FullOpen = true;
			// 
			// dlgEditTask
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(240, 309);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.label10);
			this.Controls.Add(this.numProgress);
			this.Controls.Add(this.cmbLink);
			this.Controls.Add(this.pnlColor);
			this.Controls.Add(this.label9);
			this.Controls.Add(this.numSpan);
			this.Controls.Add(this.dtmStart);
			this.Controls.Add(this.txtName);
			this.Controls.Add(this.cmbWorker);
			this.Controls.Add(this.cmbSection);
			this.Controls.Add(this.label8);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgEditTask";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "タスクの編集";
			((System.ComponentModel.ISupportInitialize)(this.numSpan)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numProgress)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.ColorDialog dlgColor;
		public System.Windows.Forms.ComboBox cmbSection;
		public System.Windows.Forms.ComboBox cmbWorker;
		public System.Windows.Forms.TextBox txtName;
		public System.Windows.Forms.DateTimePicker dtmStart;
		public System.Windows.Forms.NumericUpDown numSpan;
		public System.Windows.Forms.Label label9;
		public System.Windows.Forms.Panel pnlColor;
		public System.Windows.Forms.ComboBox cmbLink;
		public System.Windows.Forms.NumericUpDown numProgress;
		public System.Windows.Forms.Label label10;
	}
}