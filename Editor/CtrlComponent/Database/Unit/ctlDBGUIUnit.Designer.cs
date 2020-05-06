namespace Editor.CtrlComponent.Database {
	partial class ctlDBGUIUnit {
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

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlDBGUIUnit));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.TabControl1 = new System.Windows.Forms.TabControl();
            this.TabPage2 = new System.Windows.Forms.TabPage();
            this.ltvAnalyzer = new System.Windows.Forms.ListView();
            this.columnHeader10 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ColumnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ColumnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader13 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TabPage1 = new System.Windows.Forms.TabPage();
            this.picPreview = new System.Windows.Forms.PictureBox();
            this.TableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddEnemy = new System.Windows.Forms.Button();
            this.btnDeleteEnemy = new System.Windows.Forms.Button();
            this.ltvEnemyList = new System.Windows.Forms.ListView();
            this.columnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader11 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader12 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ToolStrip1 = new System.Windows.Forms.ToolStrip();
            this.ToolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.tolUnitID = new System.Windows.Forms.ToolStripTextBox();
            this.ToolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.tolUnitName = new System.Windows.Forms.ToolStripTextBox();
            this.tolAutoName = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.ToolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
            this.tolUnitLevel = new System.Windows.Forms.ToolStripTextBox();
            this.ToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
            this.tolScale = new System.Windows.Forms.ToolStripComboBox();
            this.tolChangeBackGraph = new System.Windows.Forms.ToolStripButton();
            this.tolBattleTest = new System.Windows.Forms.ToolStripButton();
            this.tolEditScript = new System.Windows.Forms.ToolStripButton();
            this.Label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.splMain1)).BeginInit();
            this.splMain1.Panel1.SuspendLayout();
            this.splMain1.Panel2.SuspendLayout();
            this.splMain1.SuspendLayout();
            this.pnlEditor.SuspendLayout();
            this.TableLayoutPanel1.SuspendLayout();
            this.TabControl1.SuspendLayout();
            this.TabPage2.SuspendLayout();
            this.TabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picPreview)).BeginInit();
            this.TableLayoutPanel2.SuspendLayout();
            this.ToolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splMain1
            // 
            // 
            // splMain1.Panel2
            // 
            this.splMain1.Panel2.AutoScrollMinSize = new System.Drawing.Size(740, 0);
            this.splMain1.SplitterDistance = 250;
            // 
            // ltvDB
            // 
            this.ltvDB.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3});
            this.ltvDB.Margin = new System.Windows.Forms.Padding(3);
            this.ltvDB.Size = new System.Drawing.Size(248, 586);
            this.ltvDB.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.ltvDB_ColumnClick);
            this.ltvDB.SelectedIndexChanged += new System.EventHandler(this.ltvDB_SelectedIndexChanged);
            // 
            // pnlEditor
            // 
            this.pnlEditor.Controls.Add(this.TableLayoutPanel1);
            this.pnlEditor.Controls.Add(this.ltvEnemyList);
            this.pnlEditor.Controls.Add(this.Label1);
            this.pnlEditor.Controls.Add(this.ToolStrip1);
            this.pnlEditor.Size = new System.Drawing.Size(820, 611);
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "指標";
            this.columnHeader3.Width = 50;
            // 
            // TableLayoutPanel1
            // 
            this.TableLayoutPanel1.ColumnCount = 2;
            this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutPanel1.Controls.Add(this.TabControl1, 1, 0);
            this.TableLayoutPanel1.Controls.Add(this.TableLayoutPanel2, 0, 0);
            this.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TableLayoutPanel1.Location = new System.Drawing.Point(251, 47);
            this.TableLayoutPanel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TableLayoutPanel1.Name = "TableLayoutPanel1";
            this.TableLayoutPanel1.RowCount = 1;
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutPanel1.Size = new System.Drawing.Size(569, 564);
            this.TableLayoutPanel1.TabIndex = 3;
            // 
            // TabControl1
            // 
            this.TabControl1.Controls.Add(this.TabPage2);
            this.TabControl1.Controls.Add(this.TabPage1);
            this.TabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TabControl1.Location = new System.Drawing.Point(103, 2);
            this.TabControl1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TabControl1.Name = "TabControl1";
            this.TabControl1.SelectedIndex = 0;
            this.TabControl1.Size = new System.Drawing.Size(463, 560);
            this.TabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.TabControl1.TabIndex = 1;
            // 
            // TabPage2
            // 
            this.TabPage2.Controls.Add(this.ltvAnalyzer);
            this.TabPage2.Location = new System.Drawing.Point(4, 24);
            this.TabPage2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TabPage2.Name = "TabPage2";
            this.TabPage2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TabPage2.Size = new System.Drawing.Size(455, 532);
            this.TabPage2.TabIndex = 1;
            this.TabPage2.Text = "登録リスト";
            this.TabPage2.UseVisualStyleBackColor = true;
            // 
            // ltvAnalyzer
            // 
            this.ltvAnalyzer.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader10,
            this.columnHeader4,
            this.ColumnHeader6,
            this.ColumnHeader7,
            this.columnHeader13});
            this.ltvAnalyzer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ltvAnalyzer.ForeColor = System.Drawing.Color.Black;
            this.ltvAnalyzer.FullRowSelect = true;
            this.ltvAnalyzer.GridLines = true;
            this.ltvAnalyzer.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.ltvAnalyzer.HideSelection = false;
            this.ltvAnalyzer.Location = new System.Drawing.Point(3, 2);
            this.ltvAnalyzer.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.ltvAnalyzer.MultiSelect = false;
            this.ltvAnalyzer.Name = "ltvAnalyzer";
            this.ltvAnalyzer.ShowItemToolTips = true;
            this.ltvAnalyzer.Size = new System.Drawing.Size(449, 528);
            this.ltvAnalyzer.TabIndex = 0;
            this.ltvAnalyzer.UseCompatibleStateImageBehavior = false;
            this.ltvAnalyzer.View = System.Windows.Forms.View.Details;
            this.ltvAnalyzer.SelectedIndexChanged += new System.EventHandler(this.ltvAnalyzer_SelectedIndexChanged);
            this.ltvAnalyzer.DoubleClick += new System.EventHandler(this.ltvAnalyzer_DoubleClick);
            // 
            // columnHeader10
            // 
            this.columnHeader10.Text = "FixedID";
            this.columnHeader10.Width = 0;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "ID";
            this.columnHeader4.Width = 50;
            // 
            // ColumnHeader6
            // 
            this.ColumnHeader6.Text = "名前";
            this.ColumnHeader6.Width = 150;
            // 
            // ColumnHeader7
            // 
            this.ColumnHeader7.Text = "指標";
            this.ColumnHeader7.Width = 50;
            // 
            // columnHeader13
            // 
            this.columnHeader13.Text = "位置";
            this.columnHeader13.Width = 120;
            // 
            // TabPage1
            // 
            this.TabPage1.AutoScroll = true;
            this.TabPage1.BackColor = System.Drawing.Color.Black;
            this.TabPage1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.TabPage1.Controls.Add(this.picPreview);
            this.TabPage1.Location = new System.Drawing.Point(4, 24);
            this.TabPage1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TabPage1.Name = "TabPage1";
            this.TabPage1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TabPage1.Size = new System.Drawing.Size(455, 532);
            this.TabPage1.TabIndex = 0;
            this.TabPage1.Text = "デザイナー";
            // 
            // picPreview
            // 
            this.picPreview.BackColor = System.Drawing.Color.Black;
            this.picPreview.Location = new System.Drawing.Point(0, 0);
            this.picPreview.Margin = new System.Windows.Forms.Padding(0);
            this.picPreview.Name = "picPreview";
            this.picPreview.Size = new System.Drawing.Size(100, 100);
            this.picPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.picPreview.TabIndex = 2;
            this.picPreview.TabStop = false;
            this.picPreview.Paint += new System.Windows.Forms.PaintEventHandler(this.picPreview_Paint);
            this.picPreview.MouseDown += new System.Windows.Forms.MouseEventHandler(this.picPreview_MouseDown);
            this.picPreview.MouseLeave += new System.EventHandler(this.picPreview_MouseLeave);
            this.picPreview.MouseMove += new System.Windows.Forms.MouseEventHandler(this.picPreview_MouseMove);
            this.picPreview.MouseUp += new System.Windows.Forms.MouseEventHandler(this.picPreview_MouseUp);
            // 
            // TableLayoutPanel2
            // 
            this.TableLayoutPanel2.ColumnCount = 1;
            this.TableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutPanel2.Controls.Add(this.btnAddEnemy, 0, 1);
            this.TableLayoutPanel2.Controls.Add(this.btnDeleteEnemy, 0, 2);
            this.TableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TableLayoutPanel2.Location = new System.Drawing.Point(3, 2);
            this.TableLayoutPanel2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.TableLayoutPanel2.Name = "TableLayoutPanel2";
            this.TableLayoutPanel2.RowCount = 4;
            this.TableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 30F));
            this.TableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.TableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.TableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 70F));
            this.TableLayoutPanel2.Size = new System.Drawing.Size(94, 560);
            this.TableLayoutPanel2.TabIndex = 0;
            // 
            // btnAddEnemy
            // 
            this.btnAddEnemy.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddEnemy.Enabled = false;
            this.btnAddEnemy.Location = new System.Drawing.Point(3, 150);
            this.btnAddEnemy.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnAddEnemy.Name = "btnAddEnemy";
            this.btnAddEnemy.Size = new System.Drawing.Size(88, 28);
            this.btnAddEnemy.TabIndex = 0;
            this.btnAddEnemy.Text = "追加 >>";
            this.btnAddEnemy.UseVisualStyleBackColor = true;
            this.btnAddEnemy.Click += new System.EventHandler(this.btnAddEnemy_Click);
            // 
            // btnDeleteEnemy
            // 
            this.btnDeleteEnemy.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDeleteEnemy.Enabled = false;
            this.btnDeleteEnemy.Location = new System.Drawing.Point(3, 182);
            this.btnDeleteEnemy.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnDeleteEnemy.Name = "btnDeleteEnemy";
            this.btnDeleteEnemy.Size = new System.Drawing.Size(88, 28);
            this.btnDeleteEnemy.TabIndex = 1;
            this.btnDeleteEnemy.Text = "<< 削除";
            this.btnDeleteEnemy.UseVisualStyleBackColor = true;
            this.btnDeleteEnemy.Click += new System.EventHandler(this.btnDeleteEnemy_Click);
            // 
            // ltvEnemyList
            // 
            this.ltvEnemyList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader9,
            this.columnHeader8,
            this.columnHeader11,
            this.columnHeader12});
            this.ltvEnemyList.Dock = System.Windows.Forms.DockStyle.Left;
            this.ltvEnemyList.ForeColor = System.Drawing.Color.Black;
            this.ltvEnemyList.FullRowSelect = true;
            this.ltvEnemyList.GridLines = true;
            this.ltvEnemyList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.ltvEnemyList.HideSelection = false;
            this.ltvEnemyList.Location = new System.Drawing.Point(0, 47);
            this.ltvEnemyList.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.ltvEnemyList.MultiSelect = false;
            this.ltvEnemyList.Name = "ltvEnemyList";
            this.ltvEnemyList.ShowItemToolTips = true;
            this.ltvEnemyList.Size = new System.Drawing.Size(251, 564);
            this.ltvEnemyList.TabIndex = 3;
            this.ltvEnemyList.UseCompatibleStateImageBehavior = false;
            this.ltvEnemyList.View = System.Windows.Forms.View.Details;
            this.ltvEnemyList.SelectedIndexChanged += new System.EventHandler(this.ltvEnemyList_SelectedIndexChanged);
            this.ltvEnemyList.DoubleClick += new System.EventHandler(this.ltvEnemyList_DoubleClick);
            // 
            // columnHeader9
            // 
            this.columnHeader9.Text = "FixedID";
            this.columnHeader9.Width = 0;
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "ID";
            this.columnHeader8.Width = 50;
            // 
            // columnHeader11
            // 
            this.columnHeader11.Text = "名前";
            this.columnHeader11.Width = 120;
            // 
            // columnHeader12
            // 
            this.columnHeader12.Text = "指標";
            this.columnHeader12.Width = 50;
            // 
            // ToolStrip1
            // 
            this.ToolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripLabel1,
            this.tolUnitID,
            this.ToolStripLabel2,
            this.tolUnitName,
            this.tolAutoName,
            this.ToolStripSeparator2,
            this.ToolStripLabel3,
            this.tolUnitLevel,
            this.ToolStripSeparator1,
            this.toolStripLabel4,
            this.tolScale,
            this.tolChangeBackGraph,
            this.tolBattleTest,
            this.tolEditScript});
            this.ToolStrip1.Location = new System.Drawing.Point(0, 0);
            this.ToolStrip1.Name = "ToolStrip1";
            this.ToolStrip1.Size = new System.Drawing.Size(820, 25);
            this.ToolStrip1.TabIndex = 0;
            this.ToolStrip1.Text = "ToolStrip1";
            // 
            // ToolStripLabel1
            // 
            this.ToolStripLabel1.Name = "ToolStripLabel1";
            this.ToolStripLabel1.Size = new System.Drawing.Size(26, 22);
            this.ToolStripLabel1.Text = "ID:";
            // 
            // tolUnitID
            // 
            this.tolUnitID.AutoToolTip = true;
            this.tolUnitID.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tolUnitID.Name = "tolUnitID";
            this.tolUnitID.Size = new System.Drawing.Size(60, 25);
            this.tolUnitID.Validating += new System.ComponentModel.CancelEventHandler(this.tolUnitID_Validating);
            // 
            // ToolStripLabel2
            // 
            this.ToolStripLabel2.Name = "ToolStripLabel2";
            this.ToolStripLabel2.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this.ToolStripLabel2.Size = new System.Drawing.Size(41, 22);
            this.ToolStripLabel2.Text = "名前:";
            this.ToolStripLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tolUnitName
            // 
            this.tolUnitName.AutoToolTip = true;
            this.tolUnitName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tolUnitName.Name = "tolUnitName";
            this.tolUnitName.Size = new System.Drawing.Size(150, 25);
            this.tolUnitName.Validating += new System.ComponentModel.CancelEventHandler(this.tolUnitName_Validating);
            // 
            // tolAutoName
            // 
            this.tolAutoName.Image = ((System.Drawing.Image)(resources.GetObject("tolAutoName.Image")));
            this.tolAutoName.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolAutoName.Name = "tolAutoName";
            this.tolAutoName.Size = new System.Drawing.Size(75, 22);
            this.tolAutoName.Text = "名前生成";
            this.tolAutoName.ToolTipText = "登録されているエネミーに基づいて機械的に項目名を生成します。";
            this.tolAutoName.Click += new System.EventHandler(this.tolAutoName_Click);
            // 
            // ToolStripSeparator2
            // 
            this.ToolStripSeparator2.Name = "ToolStripSeparator2";
            this.ToolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // ToolStripLabel3
            // 
            this.ToolStripLabel3.Name = "ToolStripLabel3";
            this.ToolStripLabel3.Size = new System.Drawing.Size(36, 22);
            this.ToolStripLabel3.Text = "指標:";
            // 
            // tolUnitLevel
            // 
            this.tolUnitLevel.AutoToolTip = true;
            this.tolUnitLevel.Enabled = false;
            this.tolUnitLevel.Name = "tolUnitLevel";
            this.tolUnitLevel.Size = new System.Drawing.Size(50, 25);
            this.tolUnitLevel.ToolTipText = "ユニット全体の強さを示す指標です。\r\nこの指標の計算式はシステムデータベースで自由に変更できます。";
            // 
            // ToolStripSeparator1
            // 
            this.ToolStripSeparator1.Name = "ToolStripSeparator1";
            this.ToolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripLabel4
            // 
            this.toolStripLabel4.Name = "toolStripLabel4";
            this.toolStripLabel4.Size = new System.Drawing.Size(50, 22);
            this.toolStripLabel4.Text = "スケール:";
            // 
            // tolScale
            // 
            this.tolScale.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.tolScale.FlatStyle = System.Windows.Forms.FlatStyle.Standard;
            this.tolScale.Name = "tolScale";
            this.tolScale.Size = new System.Drawing.Size(80, 25);
            this.tolScale.SelectedIndexChanged += new System.EventHandler(this.tolScale_SelectedIndexChanged);
            // 
            // tolChangeBackGraph
            // 
            this.tolChangeBackGraph.Image = ((System.Drawing.Image)(resources.GetObject("tolChangeBackGraph.Image")));
            this.tolChangeBackGraph.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolChangeBackGraph.Name = "tolChangeBackGraph";
            this.tolChangeBackGraph.Size = new System.Drawing.Size(75, 22);
            this.tolChangeBackGraph.Text = "背景変更";
            this.tolChangeBackGraph.Click += new System.EventHandler(this.tolChangeBackGraph_Click);
            // 
            // tolBattleTest
            // 
            this.tolBattleTest.Image = ((System.Drawing.Image)(resources.GetObject("tolBattleTest.Image")));
            this.tolBattleTest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolBattleTest.Name = "tolBattleTest";
            this.tolBattleTest.Size = new System.Drawing.Size(81, 22);
            this.tolBattleTest.Text = "バトルテスト";
            this.tolBattleTest.Click += new System.EventHandler(this.tolBattleTest_Click);
            // 
            // tolEditScript
            // 
            this.tolEditScript.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolEditScript.Image = ((System.Drawing.Image)(resources.GetObject("tolEditScript.Image")));
            this.tolEditScript.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolEditScript.Name = "tolEditScript";
            this.tolEditScript.Size = new System.Drawing.Size(23, 22);
            this.tolEditScript.Text = "ユニットスクリプト編集";
            this.tolEditScript.ToolTipText = "ユニットスクリプトを編集";
            this.tolEditScript.Click += new System.EventHandler(this.tolEditScript_Click);
            // 
            // Label1
            // 
            this.Label1.AutoSize = true;
            this.Label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.Label1.Location = new System.Drawing.Point(0, 25);
            this.Label1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 0);
            this.Label1.Name = "Label1";
            this.Label1.Padding = new System.Windows.Forms.Padding(0, 7, 0, 0);
            this.Label1.Size = new System.Drawing.Size(105, 22);
            this.Label1.TabIndex = 6;
            this.Label1.Text = "エネミーデータベース:";
            // 
            // ctlDBGUIUnit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.Margin = new System.Windows.Forms.Padding(3);
            this.Name = "ctlDBGUIUnit";
            this.RequestRowAdd += new System.EventHandler(this.ctlDBGUIUnit_RequestRowAdd);
            this.RequestRowCopy += new System.EventHandler(this.ctlDBGUIUnit_RequestRowCopy);
            this.RequestRowDelete += new System.EventHandler(this.ctlDBGUIUnit_RequestRowDelete);
            this.Load += new System.EventHandler(this.ctlDBGUIUnit_Load);
            this.splMain1.Panel1.ResumeLayout(false);
            this.splMain1.Panel1.PerformLayout();
            this.splMain1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splMain1)).EndInit();
            this.splMain1.ResumeLayout(false);
            this.pnlEditor.ResumeLayout(false);
            this.pnlEditor.PerformLayout();
            this.TableLayoutPanel1.ResumeLayout(false);
            this.TabControl1.ResumeLayout(false);
            this.TabPage2.ResumeLayout(false);
            this.TabPage1.ResumeLayout(false);
            this.TabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picPreview)).EndInit();
            this.TableLayoutPanel2.ResumeLayout(false);
            this.ToolStrip1.ResumeLayout(false);
            this.ToolStrip1.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ColumnHeader columnHeader3;
		internal System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
		internal System.Windows.Forms.TableLayoutPanel TableLayoutPanel2;
		internal System.Windows.Forms.Button btnAddEnemy;
		internal System.Windows.Forms.Button btnDeleteEnemy;
		internal System.Windows.Forms.TabControl TabControl1;
		internal System.Windows.Forms.TabPage TabPage2;
		internal System.Windows.Forms.ListView ltvAnalyzer;
		private System.Windows.Forms.ColumnHeader columnHeader10;
		internal System.Windows.Forms.ColumnHeader columnHeader4;
		internal System.Windows.Forms.ColumnHeader ColumnHeader6;
		internal System.Windows.Forms.ColumnHeader ColumnHeader7;
		private System.Windows.Forms.ColumnHeader columnHeader13;
		internal System.Windows.Forms.TabPage TabPage1;
		internal System.Windows.Forms.PictureBox picPreview;
		public System.Windows.Forms.ListView ltvEnemyList;
		private System.Windows.Forms.ColumnHeader columnHeader9;
		internal System.Windows.Forms.ColumnHeader columnHeader8;
		internal System.Windows.Forms.ColumnHeader columnHeader11;
		private System.Windows.Forms.ColumnHeader columnHeader12;
		internal System.Windows.Forms.ToolStrip ToolStrip1;
		internal System.Windows.Forms.ToolStripLabel ToolStripLabel1;
		internal System.Windows.Forms.ToolStripTextBox tolUnitID;
		internal System.Windows.Forms.ToolStripLabel ToolStripLabel2;
		internal System.Windows.Forms.ToolStripTextBox tolUnitName;
		internal System.Windows.Forms.ToolStripButton tolAutoName;
		internal System.Windows.Forms.ToolStripSeparator ToolStripSeparator2;
		internal System.Windows.Forms.ToolStripLabel ToolStripLabel3;
		internal System.Windows.Forms.ToolStripTextBox tolUnitLevel;
		internal System.Windows.Forms.ToolStripSeparator ToolStripSeparator1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel4;
		private System.Windows.Forms.ToolStripComboBox tolScale;
		internal System.Windows.Forms.ToolStripButton tolChangeBackGraph;
		private System.Windows.Forms.ToolStripButton tolEditScript;
		internal System.Windows.Forms.ToolStripButton tolBattleTest;
		internal System.Windows.Forms.Label Label1;
	}
}
