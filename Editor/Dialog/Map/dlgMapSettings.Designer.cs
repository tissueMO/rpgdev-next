namespace Editor.Dialog.Map {
	partial class dlgMapSettings {
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.uctlPathBaseMap = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.label15 = new System.Windows.Forms.Label();
            this.cmbMapLoopType = new System.Windows.Forms.ComboBox();
            this.cmbPTMoveSpeed = new System.Windows.Forms.ComboBox();
            this.cmbTileset = new System.Windows.Forms.ComboBox();
            this.txtName = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.uctlMapSize = new Editor.CtrlComponent.Common.ctlPosition();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.uctlPathBGM = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.uctlPathBGS = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.uctlBGSEditor = new Editor.CtrlComponent.Common.ctlSoundEditor();
            this.uctlBGMEditor = new Editor.CtrlComponent.Common.ctlSoundEditor();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.numBackSpeed = new System.Windows.Forms.NumericUpDown();
            this.cmbBackScrollType = new System.Windows.Forms.ComboBox();
            this.uctlPathBack = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.numFogSpeed = new System.Windows.Forms.NumericUpDown();
            this.cmbFogScrollType = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.cmbFogBlend = new System.Windows.Forms.ComboBox();
            this.uctlPathFog = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.numEncounterRate = new System.Windows.Forms.NumericUpDown();
            this.cmbUnitDB = new System.Windows.Forms.ComboBox();
            this.uctlPathBattleBack = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.uctlUnitList = new Editor.CtrlComponent.Common.ctlListEditor();
            this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numBackSpeed)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numFogSpeed)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numEncounterRate)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.uctlPathBaseMap);
            this.groupBox1.Controls.Add(this.label15);
            this.groupBox1.Controls.Add(this.cmbMapLoopType);
            this.groupBox1.Controls.Add(this.cmbPTMoveSpeed);
            this.groupBox1.Controls.Add(this.cmbTileset);
            this.groupBox1.Controls.Add(this.txtName);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.uctlMapSize);
            this.groupBox1.ForeColor = System.Drawing.Color.Blue;
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(560, 108);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "基本設定";
            // 
            // uctlPathBaseMap
            // 
            this.uctlPathBaseMap.DialogTitle = null;
            this.uctlPathBaseMap.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathBaseMap.EnabledFiles = true;
            this.uctlPathBaseMap.FileFilter = null;
            this.uctlPathBaseMap.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathBaseMap.ForeColor = System.Drawing.Color.Black;
            this.uctlPathBaseMap.Location = new System.Drawing.Point(367, 77);
            this.uctlPathBaseMap.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathBaseMap.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathBaseMap.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathBaseMap.Name = "uctlPathBaseMap";
            this.uctlPathBaseMap.Result = "";
            this.uctlPathBaseMap.RootPath = null;
            this.uctlPathBaseMap.Size = new System.Drawing.Size(183, 25);
            this.uctlPathBaseMap.TabIndex = 11;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.ForeColor = System.Drawing.Color.Black;
            this.label15.Location = new System.Drawing.Point(294, 81);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(62, 15);
            this.label15.TabIndex = 10;
            this.label15.Text = "カーボン元:";
            // 
            // cmbMapLoopType
            // 
            this.cmbMapLoopType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbMapLoopType.ForeColor = System.Drawing.Color.Black;
            this.cmbMapLoopType.FormattingEnabled = true;
            this.cmbMapLoopType.Location = new System.Drawing.Point(78, 77);
            this.cmbMapLoopType.Name = "cmbMapLoopType";
            this.cmbMapLoopType.Size = new System.Drawing.Size(190, 23);
            this.cmbMapLoopType.TabIndex = 5;
            // 
            // cmbPTMoveSpeed
            // 
            this.cmbPTMoveSpeed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbPTMoveSpeed.ForeColor = System.Drawing.Color.Black;
            this.cmbPTMoveSpeed.FormattingEnabled = true;
            this.cmbPTMoveSpeed.Location = new System.Drawing.Point(367, 49);
            this.cmbPTMoveSpeed.Name = "cmbPTMoveSpeed";
            this.cmbPTMoveSpeed.Size = new System.Drawing.Size(183, 23);
            this.cmbPTMoveSpeed.TabIndex = 9;
            // 
            // cmbTileset
            // 
            this.cmbTileset.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbTileset.ForeColor = System.Drawing.Color.Black;
            this.cmbTileset.FormattingEnabled = true;
            this.cmbTileset.Location = new System.Drawing.Point(367, 21);
            this.cmbTileset.Name = "cmbTileset";
            this.cmbTileset.Size = new System.Drawing.Size(183, 23);
            this.cmbTileset.TabIndex = 7;
            // 
            // txtName
            // 
            this.txtName.ForeColor = System.Drawing.Color.Black;
            this.txtName.Location = new System.Drawing.Point(78, 21);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(188, 23);
            this.txtName.TabIndex = 1;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(12, 81);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 15);
            this.label5.TabIndex = 4;
            this.label5.Text = "ループ:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(294, 53);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 15);
            this.label4.TabIndex = 8;
            this.label4.Text = "移動速度:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(12, 53);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 15);
            this.label3.TabIndex = 2;
            this.label3.Text = "サイズ:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(294, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 15);
            this.label2.TabIndex = 6;
            this.label2.Text = "タイルセット:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(12, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "名前:";
            // 
            // uctlMapSize
            // 
            this.uctlMapSize.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlMapSize.ForeColor = System.Drawing.Color.Black;
            this.uctlMapSize.Location = new System.Drawing.Point(78, 49);
            this.uctlMapSize.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlMapSize.Maximum = new System.Drawing.Point(500, 500);
            this.uctlMapSize.MaximumSize = new System.Drawing.Size(500, 23);
            this.uctlMapSize.Minimum = new System.Drawing.Point(1, 1);
            this.uctlMapSize.MinimumSize = new System.Drawing.Size(100, 23);
            this.uctlMapSize.Name = "uctlMapSize";
            this.uctlMapSize.Result = new System.Drawing.Point(10, 10);
            this.uctlMapSize.Size = new System.Drawing.Size(137, 23);
            this.uctlMapSize.TabIndex = 3;
            this.uctlMapSize.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Size;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.uctlPathBGM);
            this.groupBox2.Controls.Add(this.uctlPathBGS);
            this.groupBox2.Controls.Add(this.uctlBGSEditor);
            this.groupBox2.Controls.Add(this.uctlBGMEditor);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.ForeColor = System.Drawing.Color.Blue;
            this.groupBox2.Location = new System.Drawing.Point(12, 126);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(560, 138);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "サウンド";
            // 
            // uctlPathBGM
            // 
            this.uctlPathBGM.DialogTitle = null;
            this.uctlPathBGM.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathBGM.EnabledFiles = true;
            this.uctlPathBGM.FileFilter = null;
            this.uctlPathBGM.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathBGM.ForeColor = System.Drawing.Color.Black;
            this.uctlPathBGM.Location = new System.Drawing.Point(78, 31);
            this.uctlPathBGM.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathBGM.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathBGM.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathBGM.Name = "uctlPathBGM";
            this.uctlPathBGM.Result = "";
            this.uctlPathBGM.RootPath = null;
            this.uctlPathBGM.Size = new System.Drawing.Size(169, 25);
            this.uctlPathBGM.TabIndex = 1;
            this.uctlPathBGM.PathChanged += new System.EventHandler(this.uctlPathBGM_PathChanged);
            // 
            // uctlPathBGS
            // 
            this.uctlPathBGS.DialogTitle = null;
            this.uctlPathBGS.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathBGS.EnabledFiles = true;
            this.uctlPathBGS.FileFilter = null;
            this.uctlPathBGS.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathBGS.ForeColor = System.Drawing.Color.Black;
            this.uctlPathBGS.Location = new System.Drawing.Point(78, 94);
            this.uctlPathBGS.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathBGS.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathBGS.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathBGS.Name = "uctlPathBGS";
            this.uctlPathBGS.Result = "";
            this.uctlPathBGS.RootPath = null;
            this.uctlPathBGS.Size = new System.Drawing.Size(169, 25);
            this.uctlPathBGS.TabIndex = 4;
            this.uctlPathBGS.PathChanged += new System.EventHandler(this.uctlPathBGS_PathChanged);
            // 
            // uctlBGSEditor
            // 
            this.uctlBGSEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlBGSEditor.ForeColor = System.Drawing.Color.Black;
            this.uctlBGSEditor.Location = new System.Drawing.Point(247, 72);
            this.uctlBGSEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlBGSEditor.MinimumSize = new System.Drawing.Size(0, 60);
            this.uctlBGSEditor.Name = "uctlBGSEditor";
            this.uctlBGSEditor.Size = new System.Drawing.Size(307, 60);
            this.uctlBGSEditor.TabIndex = 5;
            // 
            // uctlBGMEditor
            // 
            this.uctlBGMEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlBGMEditor.ForeColor = System.Drawing.Color.Black;
            this.uctlBGMEditor.Location = new System.Drawing.Point(247, 9);
            this.uctlBGMEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlBGMEditor.MinimumSize = new System.Drawing.Size(0, 60);
            this.uctlBGMEditor.Name = "uctlBGMEditor";
            this.uctlBGMEditor.Size = new System.Drawing.Size(307, 60);
            this.uctlBGMEditor.TabIndex = 2;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.ForeColor = System.Drawing.Color.Black;
            this.label7.Location = new System.Drawing.Point(12, 99);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(37, 15);
            this.label7.TabIndex = 3;
            this.label7.Text = "BGS:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.ForeColor = System.Drawing.Color.Black;
            this.label6.Location = new System.Drawing.Point(12, 36);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(39, 15);
            this.label6.TabIndex = 0;
            this.label6.Text = "BGM:";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.numBackSpeed);
            this.groupBox3.Controls.Add(this.cmbBackScrollType);
            this.groupBox3.Controls.Add(this.uctlPathBack);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.ForeColor = System.Drawing.Color.Blue;
            this.groupBox3.Location = new System.Drawing.Point(12, 270);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(274, 114);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "遠景";
            // 
            // numBackSpeed
            // 
            this.numBackSpeed.ForeColor = System.Drawing.Color.Black;
            this.numBackSpeed.Location = new System.Drawing.Point(78, 81);
            this.numBackSpeed.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.numBackSpeed.Name = "numBackSpeed";
            this.numBackSpeed.Size = new System.Drawing.Size(76, 23);
            this.numBackSpeed.TabIndex = 4;
            // 
            // cmbBackScrollType
            // 
            this.cmbBackScrollType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbBackScrollType.ForeColor = System.Drawing.Color.Black;
            this.cmbBackScrollType.FormattingEnabled = true;
            this.cmbBackScrollType.Location = new System.Drawing.Point(78, 53);
            this.cmbBackScrollType.Name = "cmbBackScrollType";
            this.cmbBackScrollType.Size = new System.Drawing.Size(156, 23);
            this.cmbBackScrollType.TabIndex = 2;
            // 
            // uctlPathBack
            // 
            this.uctlPathBack.DialogTitle = null;
            this.uctlPathBack.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathBack.EnabledFiles = true;
            this.uctlPathBack.FileFilter = null;
            this.uctlPathBack.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathBack.ForeColor = System.Drawing.Color.Black;
            this.uctlPathBack.Location = new System.Drawing.Point(6, 20);
            this.uctlPathBack.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathBack.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathBack.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathBack.Name = "uctlPathBack";
            this.uctlPathBack.Result = "";
            this.uctlPathBack.RootPath = null;
            this.uctlPathBack.Size = new System.Drawing.Size(262, 25);
            this.uctlPathBack.TabIndex = 0;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.ForeColor = System.Drawing.Color.Black;
            this.label10.Location = new System.Drawing.Point(12, 85);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(36, 15);
            this.label10.TabIndex = 3;
            this.label10.Text = "速度:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.ForeColor = System.Drawing.Color.Black;
            this.label9.Location = new System.Drawing.Point(12, 57);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(58, 15);
            this.label9.TabIndex = 1;
            this.label9.Text = "スクロール:";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.numFogSpeed);
            this.groupBox4.Controls.Add(this.cmbFogScrollType);
            this.groupBox4.Controls.Add(this.label13);
            this.groupBox4.Controls.Add(this.label14);
            this.groupBox4.Controls.Add(this.cmbFogBlend);
            this.groupBox4.Controls.Add(this.uctlPathFog);
            this.groupBox4.Controls.Add(this.label8);
            this.groupBox4.ForeColor = System.Drawing.Color.Blue;
            this.groupBox4.Location = new System.Drawing.Point(298, 270);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(274, 139);
            this.groupBox4.TabIndex = 3;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "フォグ";
            // 
            // numFogSpeed
            // 
            this.numFogSpeed.ForeColor = System.Drawing.Color.Black;
            this.numFogSpeed.Location = new System.Drawing.Point(74, 80);
            this.numFogSpeed.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.numFogSpeed.Name = "numFogSpeed";
            this.numFogSpeed.Size = new System.Drawing.Size(76, 23);
            this.numFogSpeed.TabIndex = 4;
            // 
            // cmbFogScrollType
            // 
            this.cmbFogScrollType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbFogScrollType.ForeColor = System.Drawing.Color.Black;
            this.cmbFogScrollType.FormattingEnabled = true;
            this.cmbFogScrollType.Location = new System.Drawing.Point(74, 52);
            this.cmbFogScrollType.Name = "cmbFogScrollType";
            this.cmbFogScrollType.Size = new System.Drawing.Size(160, 23);
            this.cmbFogScrollType.TabIndex = 2;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.ForeColor = System.Drawing.Color.Black;
            this.label13.Location = new System.Drawing.Point(8, 84);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(36, 15);
            this.label13.TabIndex = 3;
            this.label13.Text = "速度:";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.ForeColor = System.Drawing.Color.Black;
            this.label14.Location = new System.Drawing.Point(8, 56);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(58, 15);
            this.label14.TabIndex = 1;
            this.label14.Text = "スクロール:";
            // 
            // cmbFogBlend
            // 
            this.cmbFogBlend.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbFogBlend.ForeColor = System.Drawing.Color.Black;
            this.cmbFogBlend.FormattingEnabled = true;
            this.cmbFogBlend.Location = new System.Drawing.Point(74, 108);
            this.cmbFogBlend.Name = "cmbFogBlend";
            this.cmbFogBlend.Size = new System.Drawing.Size(160, 23);
            this.cmbFogBlend.TabIndex = 6;
            // 
            // uctlPathFog
            // 
            this.uctlPathFog.DialogTitle = null;
            this.uctlPathFog.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathFog.EnabledFiles = true;
            this.uctlPathFog.FileFilter = null;
            this.uctlPathFog.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathFog.ForeColor = System.Drawing.Color.Black;
            this.uctlPathFog.Location = new System.Drawing.Point(6, 20);
            this.uctlPathFog.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathFog.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathFog.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathFog.Name = "uctlPathFog";
            this.uctlPathFog.Result = "";
            this.uctlPathFog.RootPath = null;
            this.uctlPathFog.Size = new System.Drawing.Size(262, 25);
            this.uctlPathFog.TabIndex = 0;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.ForeColor = System.Drawing.Color.Black;
            this.label8.Location = new System.Drawing.Point(8, 112);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(60, 15);
            this.label8.TabIndex = 5;
            this.label8.Text = "合成方法:";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label12);
            this.groupBox5.Controls.Add(this.label11);
            this.groupBox5.Controls.Add(this.numEncounterRate);
            this.groupBox5.Controls.Add(this.cmbUnitDB);
            this.groupBox5.Controls.Add(this.uctlPathBattleBack);
            this.groupBox5.Controls.Add(this.uctlUnitList);
            this.groupBox5.ForeColor = System.Drawing.Color.Blue;
            this.groupBox5.Location = new System.Drawing.Point(12, 415);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(560, 180);
            this.groupBox5.TabIndex = 4;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "エンカウント";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.ForeColor = System.Drawing.Color.Black;
            this.label12.Location = new System.Drawing.Point(368, 75);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(60, 15);
            this.label12.TabIndex = 4;
            this.label12.Text = "戦闘背景:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.ForeColor = System.Drawing.Color.Black;
            this.label11.Location = new System.Drawing.Point(368, 24);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(89, 15);
            this.label11.TabIndex = 2;
            this.label11.Text = "エンカウント係数:";
            // 
            // numEncounterRate
            // 
            this.numEncounterRate.ForeColor = System.Drawing.Color.Black;
            this.numEncounterRate.Location = new System.Drawing.Point(377, 42);
            this.numEncounterRate.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.numEncounterRate.Name = "numEncounterRate";
            this.numEncounterRate.Size = new System.Drawing.Size(76, 23);
            this.numEncounterRate.TabIndex = 3;
            // 
            // cmbUnitDB
            // 
            this.cmbUnitDB.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbUnitDB.ForeColor = System.Drawing.Color.Black;
            this.cmbUnitDB.FormattingEnabled = true;
            this.cmbUnitDB.Location = new System.Drawing.Point(11, 21);
            this.cmbUnitDB.Name = "cmbUnitDB";
            this.cmbUnitDB.Size = new System.Drawing.Size(174, 23);
            this.cmbUnitDB.TabIndex = 0;
            this.cmbUnitDB.SelectedIndexChanged += new System.EventHandler(this.uctlUnitList_RequestWatchControls);
            // 
            // uctlPathBattleBack
            // 
            this.uctlPathBattleBack.DialogTitle = null;
            this.uctlPathBattleBack.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathBattleBack.EnabledFiles = true;
            this.uctlPathBattleBack.FileFilter = null;
            this.uctlPathBattleBack.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathBattleBack.ForeColor = System.Drawing.Color.Black;
            this.uctlPathBattleBack.Location = new System.Drawing.Point(377, 95);
            this.uctlPathBattleBack.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathBattleBack.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathBattleBack.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathBattleBack.Name = "uctlPathBattleBack";
            this.uctlPathBattleBack.Result = "";
            this.uctlPathBattleBack.RootPath = null;
            this.uctlPathBattleBack.Size = new System.Drawing.Size(177, 25);
            this.uctlPathBattleBack.TabIndex = 5;
            // 
            // uctlUnitList
            // 
            this.uctlUnitList.AllowOverlap = true;
            this.uctlUnitList.EnabledAddAll = false;
            this.uctlUnitList.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlUnitList.ForeColor = System.Drawing.Color.Black;
            this.uctlUnitList.Location = new System.Drawing.Point(8, 17);
            this.uctlUnitList.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlUnitList.Name = "uctlUnitList";
            this.uctlUnitList.Size = new System.Drawing.Size(344, 159);
            this.uctlUnitList.SortColumnIndex = 0;
            this.uctlUnitList.TabIndex = 1;
            this.uctlUnitList.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlUnitList_ListItemSelected);
            this.uctlUnitList.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlUnitList_RequestAddItem);
            this.uctlUnitList.RequestClearListEditorSource += new System.EventHandler(this.uctlUnitList_RequestClearListEditorSource);
            this.uctlUnitList.RequestWatchControls += new System.EventHandler(this.uctlUnitList_RequestWatchControls);
            // 
            // uctlOKCancel
            // 
            this.uctlOKCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.uctlOKCancel.DisenabledAcceptButton = false;
            this.uctlOKCancel.DisenabledCancelButton = false;
            this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlOKCancel.Location = new System.Drawing.Point(367, 608);
            this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlOKCancel.Name = "uctlOKCancel";
            this.uctlOKCancel.OKTitle = "OK";
            this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
            this.uctlOKCancel.TabIndex = 5;
            this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
            // 
            // dlgMapSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 646);
            this.Controls.Add(this.uctlOKCancel);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgMapSettings";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "マップの設定";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgMapSettings_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numBackSpeed)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numFogSpeed)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numEncounterRate)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private CtrlComponent.Common.ctlPosition uctlMapSize;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.GroupBox groupBox5;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private CtrlComponent.Common.ctlPathSetter uctlPathBGS;
		private CtrlComponent.Common.ctlPathSetter uctlPathBGM;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private CtrlComponent.Common.ctlPathSetter uctlPathBack;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label9;
		private CtrlComponent.Common.ctlPathSetter uctlPathFog;
		private System.Windows.Forms.Label label8;
		private CtrlComponent.Common.ctlPathSetter uctlPathBattleBack;
		private CtrlComponent.Common.ctlListEditor uctlUnitList;
		private System.Windows.Forms.TextBox txtName;
		private CtrlComponent.Common.ctlSoundEditor uctlBGSEditor;
		private CtrlComponent.Common.ctlSoundEditor uctlBGMEditor;
		private System.Windows.Forms.ComboBox cmbMapLoopType;
		private System.Windows.Forms.ComboBox cmbPTMoveSpeed;
		private System.Windows.Forms.ComboBox cmbTileset;
		private System.Windows.Forms.ComboBox cmbUnitDB;
		private System.Windows.Forms.NumericUpDown numBackSpeed;
		private System.Windows.Forms.ComboBox cmbBackScrollType;
		private System.Windows.Forms.ComboBox cmbFogBlend;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.NumericUpDown numEncounterRate;
		private System.Windows.Forms.NumericUpDown numFogSpeed;
		private System.Windows.Forms.ComboBox cmbFogScrollType;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.Label label14;
		private CtrlComponent.Common.ctlPathSetter uctlPathBaseMap;
		private System.Windows.Forms.Label label15;
	}
}