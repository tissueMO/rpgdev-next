namespace Editor.Dialog.Database {
	partial class dlgEditColumn {
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
            this.label1 = new System.Windows.Forms.Label();
            this.txtHeaderText = new System.Windows.Forms.TextBox();
            this.rdbFromMyList = new System.Windows.Forms.RadioButton();
            this.rdbFromDB = new System.Windows.Forms.RadioButton();
            this.cmbDBsList = new System.Windows.Forms.ComboBox();
            this.tbcFormType = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label4 = new System.Windows.Forms.Label();
            this.numOneIntMax = new System.Windows.Forms.NumericUpDown();
            this.numOneIntMin = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.tabPage8 = new System.Windows.Forms.TabPage();
            this.label17 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.numIntsMax = new System.Windows.Forms.NumericUpDown();
            this.numIntsMin = new System.Windows.Forms.NumericUpDown();
            this.label16 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label5 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.label2 = new System.Windows.Forms.Label();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.txtEditMyList = new System.Windows.Forms.TextBox();
            this.uctlMyList = new Editor.CtrlComponent.Common.ctlListEditor();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.chkTextIDValueMode = new System.Windows.Forms.CheckBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.cmbDBsIDNames = new System.Windows.Forms.ComboBox();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.chkButtonIDValueMode = new System.Windows.Forms.CheckBox();
            this.label14 = new System.Windows.Forms.Label();
            this.cmbButtonSrcDB = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.numFileSetterDestColumnIndex = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.rdbButtonFileSelect = new System.Windows.Forms.RadioButton();
            this.numIDNameSetterDestColumnIndex = new System.Windows.Forms.NumericUpDown();
            this.label11 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.rdbButtonIDNames = new System.Windows.Forms.RadioButton();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.label13 = new System.Windows.Forms.Label();
            this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.tbcFormType.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numOneIntMax)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numOneIntMin)).BeginInit();
            this.tabPage8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numIntsMax)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIntsMin)).BeginInit();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.tabPage6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numFileSetterDestColumnIndex)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIDNameSetterDestColumnIndex)).BeginInit();
            this.tabPage7.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "表示名:";
            // 
            // txtHeaderText
            // 
            this.txtHeaderText.Location = new System.Drawing.Point(68, 16);
            this.txtHeaderText.Name = "txtHeaderText";
            this.txtHeaderText.Size = new System.Drawing.Size(213, 23);
            this.txtHeaderText.TabIndex = 1;
            // 
            // rdbFromMyList
            // 
            this.rdbFromMyList.AutoSize = true;
            this.rdbFromMyList.Location = new System.Drawing.Point(15, 82);
            this.rdbFromMyList.Name = "rdbFromMyList";
            this.rdbFromMyList.Size = new System.Drawing.Size(125, 19);
            this.rdbFromMyList.TabIndex = 2;
            this.rdbFromMyList.Text = "独自リストを使用する";
            this.rdbFromMyList.UseVisualStyleBackColor = true;
            this.rdbFromMyList.CheckedChanged += new System.EventHandler(this.rdbFromMyList_CheckedChanged);
            // 
            // rdbFromDB
            // 
            this.rdbFromDB.AutoSize = true;
            this.rdbFromDB.Checked = true;
            this.rdbFromDB.Location = new System.Drawing.Point(15, 13);
            this.rdbFromDB.Name = "rdbFromDB";
            this.rdbFromDB.Size = new System.Drawing.Size(125, 19);
            this.rdbFromDB.TabIndex = 0;
            this.rdbFromDB.TabStop = true;
            this.rdbFromDB.Text = "既存リストを使用する";
            this.rdbFromDB.UseVisualStyleBackColor = true;
            this.rdbFromDB.CheckedChanged += new System.EventHandler(this.rdbFromMyList_CheckedChanged);
            // 
            // cmbDBsList
            // 
            this.cmbDBsList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDBsList.FormattingEnabled = true;
            this.cmbDBsList.Location = new System.Drawing.Point(37, 41);
            this.cmbDBsList.Name = "cmbDBsList";
            this.cmbDBsList.Size = new System.Drawing.Size(222, 23);
            this.cmbDBsList.TabIndex = 1;
            // 
            // tbcFormType
            // 
            this.tbcFormType.Controls.Add(this.tabPage1);
            this.tbcFormType.Controls.Add(this.tabPage8);
            this.tbcFormType.Controls.Add(this.tabPage2);
            this.tbcFormType.Controls.Add(this.tabPage3);
            this.tbcFormType.Controls.Add(this.tabPage4);
            this.tbcFormType.Controls.Add(this.tabPage5);
            this.tbcFormType.Controls.Add(this.tabPage6);
            this.tbcFormType.Controls.Add(this.tabPage7);
            this.tbcFormType.HotTrack = true;
            this.tbcFormType.ItemSize = new System.Drawing.Size(110, 23);
            this.tbcFormType.Location = new System.Drawing.Point(13, 52);
            this.tbcFormType.Multiline = true;
            this.tbcFormType.Name = "tbcFormType";
            this.tbcFormType.SelectedIndex = 0;
            this.tbcFormType.Size = new System.Drawing.Size(462, 410);
            this.tbcFormType.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tbcFormType.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.numOneIntMax);
            this.tabPage1.Controls.Add(this.numOneIntMin);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Location = new System.Drawing.Point(4, 50);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(454, 356);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "単一値";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(168, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(19, 15);
            this.label4.TabIndex = 2;
            this.label4.Text = "～";
            // 
            // numOneIntMax
            // 
            this.numOneIntMax.Location = new System.Drawing.Point(194, 13);
            this.numOneIntMax.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numOneIntMax.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numOneIntMax.Name = "numOneIntMax";
            this.numOneIntMax.Size = new System.Drawing.Size(82, 23);
            this.numOneIntMax.TabIndex = 3;
            this.numOneIntMax.Value = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            // 
            // numOneIntMin
            // 
            this.numOneIntMin.Location = new System.Drawing.Point(80, 13);
            this.numOneIntMin.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numOneIntMin.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numOneIntMin.Name = "numOneIntMin";
            this.numOneIntMin.Size = new System.Drawing.Size(82, 23);
            this.numOneIntMin.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 16);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 15);
            this.label3.TabIndex = 0;
            this.label3.Text = "有効範囲:";
            // 
            // tabPage8
            // 
            this.tabPage8.Controls.Add(this.label17);
            this.tabPage8.Controls.Add(this.label15);
            this.tabPage8.Controls.Add(this.numIntsMax);
            this.tabPage8.Controls.Add(this.numIntsMin);
            this.tabPage8.Controls.Add(this.label16);
            this.tabPage8.Location = new System.Drawing.Point(4, 50);
            this.tabPage8.Name = "tabPage8";
            this.tabPage8.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage8.Size = new System.Drawing.Size(454, 356);
            this.tabPage8.TabIndex = 7;
            this.tabPage8.Text = "複数値";
            this.tabPage8.UseVisualStyleBackColor = true;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label17.Location = new System.Drawing.Point(43, 48);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(217, 30);
            this.label17.TabIndex = 4;
            this.label17.Text = "値同士の区切りには ; を使います。\r\n入力できる個数を制限することはできません。";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(168, 16);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(19, 15);
            this.label15.TabIndex = 2;
            this.label15.Text = "～";
            // 
            // numIntsMax
            // 
            this.numIntsMax.Location = new System.Drawing.Point(194, 13);
            this.numIntsMax.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numIntsMax.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numIntsMax.Name = "numIntsMax";
            this.numIntsMax.Size = new System.Drawing.Size(82, 23);
            this.numIntsMax.TabIndex = 3;
            this.numIntsMax.Value = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            // 
            // numIntsMin
            // 
            this.numIntsMin.Location = new System.Drawing.Point(80, 13);
            this.numIntsMin.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numIntsMin.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numIntsMin.Name = "numIntsMin";
            this.numIntsMin.Size = new System.Drawing.Size(82, 23);
            this.numIntsMin.TabIndex = 1;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(15, 16);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(60, 15);
            this.label16.TabIndex = 0;
            this.label16.Text = "有効範囲:";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label5);
            this.tabPage2.Location = new System.Drawing.Point(4, 50);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(454, 356);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "文字列値";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label5.Location = new System.Drawing.Point(15, 16);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(158, 15);
            this.label5.TabIndex = 0;
            this.label5.Text = "設定するオプションはありません。";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.label2);
            this.tabPage3.Location = new System.Drawing.Point(4, 50);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(454, 356);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "ON/OFF 値";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label2.Location = new System.Drawing.Point(15, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(158, 15);
            this.label2.TabIndex = 0;
            this.label2.Text = "設定するオプションはありません。";
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.txtEditMyList);
            this.tabPage4.Controls.Add(this.rdbFromDB);
            this.tabPage4.Controls.Add(this.rdbFromMyList);
            this.tabPage4.Controls.Add(this.cmbDBsList);
            this.tabPage4.Controls.Add(this.uctlMyList);
            this.tabPage4.Location = new System.Drawing.Point(4, 50);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(454, 356);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "リスト";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // txtEditMyList
            // 
            this.txtEditMyList.Location = new System.Drawing.Point(37, 111);
            this.txtEditMyList.Name = "txtEditMyList";
            this.txtEditMyList.Size = new System.Drawing.Size(222, 23);
            this.txtEditMyList.TabIndex = 3;
            this.txtEditMyList.TextChanged += new System.EventHandler(this.uctlMyList_RequestWatchControls);
            // 
            // uctlMyList
            // 
            this.uctlMyList.AllowOverlap = true;
            this.uctlMyList.Enabled = false;
            this.uctlMyList.EnabledAddAll = false;
            this.uctlMyList.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlMyList.Location = new System.Drawing.Point(34, 107);
            this.uctlMyList.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlMyList.Name = "uctlMyList";
            this.uctlMyList.Size = new System.Drawing.Size(393, 207);
            this.uctlMyList.TabIndex = 4;
            this.uctlMyList.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlMyList_ListItemSelected);
            this.uctlMyList.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlMyList_RequestAddItem);
            this.uctlMyList.RequestClearListEditorSource += new System.EventHandler(this.uctlMyList_RequestClearListEditorSource);
            this.uctlMyList.RequestWatchControls += new System.EventHandler(this.uctlMyList_RequestWatchControls);
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.chkTextIDValueMode);
            this.tabPage5.Controls.Add(this.label10);
            this.tabPage5.Controls.Add(this.label9);
            this.tabPage5.Controls.Add(this.cmbDBsIDNames);
            this.tabPage5.Location = new System.Drawing.Point(4, 50);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(454, 356);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "IDリスト";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // chkTextIDValueMode
            // 
            this.chkTextIDValueMode.AutoSize = true;
            this.chkTextIDValueMode.Location = new System.Drawing.Point(304, 16);
            this.chkTextIDValueMode.Name = "chkTextIDValueMode";
            this.chkTextIDValueMode.Size = new System.Drawing.Size(102, 19);
            this.chkTextIDValueMode.TabIndex = 2;
            this.chkTextIDValueMode.Text = "値を関連付ける";
            this.chkTextIDValueMode.UseVisualStyleBackColor = true;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label10.Location = new System.Drawing.Point(32, 51);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(357, 30);
            this.label10.TabIndex = 3;
            this.label10.Text = "ソースで指定したID項目以外を入力できない列です。\r\n手動では入力できないため、入力補助のボタンを配置する必要があります。";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(16, 16);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(62, 15);
            this.label9.TabIndex = 0;
            this.label9.Text = "値のソース:";
            // 
            // cmbDBsIDNames
            // 
            this.cmbDBsIDNames.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDBsIDNames.FormattingEnabled = true;
            this.cmbDBsIDNames.Location = new System.Drawing.Point(89, 13);
            this.cmbDBsIDNames.Name = "cmbDBsIDNames";
            this.cmbDBsIDNames.Size = new System.Drawing.Size(198, 23);
            this.cmbDBsIDNames.TabIndex = 1;
            // 
            // tabPage6
            // 
            this.tabPage6.Controls.Add(this.chkButtonIDValueMode);
            this.tabPage6.Controls.Add(this.label14);
            this.tabPage6.Controls.Add(this.cmbButtonSrcDB);
            this.tabPage6.Controls.Add(this.label12);
            this.tabPage6.Controls.Add(this.numFileSetterDestColumnIndex);
            this.tabPage6.Controls.Add(this.label8);
            this.tabPage6.Controls.Add(this.rdbButtonFileSelect);
            this.tabPage6.Controls.Add(this.numIDNameSetterDestColumnIndex);
            this.tabPage6.Controls.Add(this.label11);
            this.tabPage6.Controls.Add(this.label7);
            this.tabPage6.Controls.Add(this.rdbButtonIDNames);
            this.tabPage6.Location = new System.Drawing.Point(4, 50);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage6.Size = new System.Drawing.Size(454, 356);
            this.tabPage6.TabIndex = 5;
            this.tabPage6.Text = "ボタン";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // chkButtonIDValueMode
            // 
            this.chkButtonIDValueMode.AutoSize = true;
            this.chkButtonIDValueMode.Location = new System.Drawing.Point(339, 45);
            this.chkButtonIDValueMode.Name = "chkButtonIDValueMode";
            this.chkButtonIDValueMode.Size = new System.Drawing.Size(102, 19);
            this.chkButtonIDValueMode.TabIndex = 6;
            this.chkButtonIDValueMode.Text = "値を関連付ける";
            this.chkButtonIDValueMode.UseVisualStyleBackColor = true;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(24, 47);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(62, 15);
            this.label14.TabIndex = 4;
            this.label14.Text = "値のソース:";
            // 
            // cmbButtonSrcDB
            // 
            this.cmbButtonSrcDB.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbButtonSrcDB.FormattingEnabled = true;
            this.cmbButtonSrcDB.Location = new System.Drawing.Point(154, 43);
            this.cmbButtonSrcDB.Name = "cmbButtonSrcDB";
            this.cmbButtonSrcDB.Size = new System.Drawing.Size(169, 23);
            this.cmbButtonSrcDB.TabIndex = 5;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label12.Location = new System.Drawing.Point(49, 218);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(227, 30);
            this.label12.TabIndex = 13;
            this.label12.Text = "ファイル名を格納するための列を指定します。\r\nインデックスは左端が０から始まる整数値です。";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // numFileSetterDestColumnIndex
            // 
            this.numFileSetterDestColumnIndex.Enabled = false;
            this.numFileSetterDestColumnIndex.Location = new System.Drawing.Point(154, 183);
            this.numFileSetterDestColumnIndex.Name = "numFileSetterDestColumnIndex";
            this.numFileSetterDestColumnIndex.Size = new System.Drawing.Size(69, 23);
            this.numFileSetterDestColumnIndex.TabIndex = 12;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(24, 187);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(122, 15);
            this.label8.TabIndex = 11;
            this.label8.Text = "代入先の列インデックス:";
            // 
            // rdbButtonFileSelect
            // 
            this.rdbButtonFileSelect.AutoSize = true;
            this.rdbButtonFileSelect.Location = new System.Drawing.Point(15, 154);
            this.rdbButtonFileSelect.Name = "rdbButtonFileSelect";
            this.rdbButtonFileSelect.Size = new System.Drawing.Size(111, 19);
            this.rdbButtonFileSelect.TabIndex = 10;
            this.rdbButtonFileSelect.Text = "ファイルを選択する";
            this.rdbButtonFileSelect.UseVisualStyleBackColor = true;
            this.rdbButtonFileSelect.CheckedChanged += new System.EventHandler(this.rdbToScript_CheckedChanged);
            // 
            // numIDNameSetterDestColumnIndex
            // 
            this.numIDNameSetterDestColumnIndex.Location = new System.Drawing.Point(154, 71);
            this.numIDNameSetterDestColumnIndex.Name = "numIDNameSetterDestColumnIndex";
            this.numIDNameSetterDestColumnIndex.Size = new System.Drawing.Size(69, 23);
            this.numIDNameSetterDestColumnIndex.TabIndex = 8;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label11.Location = new System.Drawing.Point(49, 106);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(227, 30);
            this.label11.TabIndex = 9;
            this.label11.Text = "格納先のIDリスト列を指定します。\r\nインデックスは左端が０から始まる整数値です。";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(24, 75);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(122, 15);
            this.label7.TabIndex = 7;
            this.label7.Text = "代入先の列インデックス:";
            // 
            // rdbButtonIDNames
            // 
            this.rdbButtonIDNames.AutoSize = true;
            this.rdbButtonIDNames.Checked = true;
            this.rdbButtonIDNames.Location = new System.Drawing.Point(15, 13);
            this.rdbButtonIDNames.Name = "rdbButtonIDNames";
            this.rdbButtonIDNames.Size = new System.Drawing.Size(219, 19);
            this.rdbButtonIDNames.TabIndex = 3;
            this.rdbButtonIDNames.TabStop = true;
            this.rdbButtonIDNames.Text = "IDリストを入力するための補助画面を開く";
            this.rdbButtonIDNames.UseVisualStyleBackColor = true;
            this.rdbButtonIDNames.CheckedChanged += new System.EventHandler(this.rdbToScript_CheckedChanged);
            // 
            // tabPage7
            // 
            this.tabPage7.Controls.Add(this.label13);
            this.tabPage7.Location = new System.Drawing.Point(4, 50);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage7.Size = new System.Drawing.Size(454, 356);
            this.tabPage7.TabIndex = 6;
            this.tabPage7.Text = "ピクチャー";
            this.tabPage7.UseVisualStyleBackColor = true;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.ForeColor = System.Drawing.SystemColors.GrayText;
            this.label13.Location = new System.Drawing.Point(15, 16);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(264, 30);
            this.label13.TabIndex = 0;
            this.label13.Text = "設定するオプションはありません。\r\nこの列では、セルをクリックするとファイルを選択できます。";
            // 
            // uctlOKCancel
            // 
            this.uctlOKCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.uctlOKCancel.DisenabledAcceptButton = false;
            this.uctlOKCancel.DisenabledCancelButton = false;
            this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlOKCancel.Location = new System.Drawing.Point(263, 475);
            this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlOKCancel.Name = "uctlOKCancel";
            this.uctlOKCancel.OKTitle = "OK";
            this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
            this.uctlOKCancel.TabIndex = 3;
            this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
            // 
            // dlgEditColumn
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(489, 516);
            this.Controls.Add(this.tbcFormType);
            this.Controls.Add(this.uctlOKCancel);
            this.Controls.Add(this.txtHeaderText);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgEditColumn";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "列の編集";
            this.tbcFormType.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numOneIntMax)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numOneIntMin)).EndInit();
            this.tabPage8.ResumeLayout(false);
            this.tabPage8.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numIntsMax)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIntsMin)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            this.tabPage5.ResumeLayout(false);
            this.tabPage5.PerformLayout();
            this.tabPage6.ResumeLayout(false);
            this.tabPage6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numFileSetterDestColumnIndex)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numIDNameSetterDestColumnIndex)).EndInit();
            this.tabPage7.ResumeLayout(false);
            this.tabPage7.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtHeaderText;
		private System.Windows.Forms.RadioButton rdbFromMyList;
		private System.Windows.Forms.RadioButton rdbFromDB;
		private System.Windows.Forms.ComboBox cmbDBsList;
		private CtrlComponent.Common.ctlListEditor uctlMyList;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.TabControl tbcFormType;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.TabPage tabPage3;
		private System.Windows.Forms.TabPage tabPage4;
		private System.Windows.Forms.TabPage tabPage5;
		private System.Windows.Forms.TabPage tabPage6;
		private System.Windows.Forms.TabPage tabPage7;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.NumericUpDown numOneIntMax;
		private System.Windows.Forms.NumericUpDown numOneIntMin;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.ComboBox cmbDBsIDNames;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.RadioButton rdbButtonIDNames;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtEditMyList;
		private System.Windows.Forms.NumericUpDown numIDNameSetterDestColumnIndex;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.NumericUpDown numFileSetterDestColumnIndex;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.RadioButton rdbButtonFileSelect;
		private System.Windows.Forms.TabPage tabPage8;
		private System.Windows.Forms.Label label17;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.NumericUpDown numIntsMax;
		private System.Windows.Forms.NumericUpDown numIntsMin;
		private System.Windows.Forms.Label label16;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.ComboBox cmbButtonSrcDB;
		private System.Windows.Forms.CheckBox chkTextIDValueMode;
		private System.Windows.Forms.CheckBox chkButtonIDValueMode;
		private System.Windows.Forms.Label label5;
	}
}