namespace Editor.CtrlComponent.Map {
	partial class ctlEVPage {
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
			this.S_StartIf = new System.Windows.Forms.Label();
			this.Label1 = new System.Windows.Forms.Label();
			this.cmbFrequency = new System.Windows.Forms.ComboBox();
			this.S_Speed = new System.Windows.Forms.Label();
			this.S_Direction = new System.Windows.Forms.Label();
			this.cmbSpeed = new System.Windows.Forms.ComboBox();
			this.S_MoveType = new System.Windows.Forms.Label();
			this.GroupBox3 = new System.Windows.Forms.GroupBox();
			this.uctlHitRange = new Editor.CtrlComponent.Common.ctlPosition();
			this.label2 = new System.Windows.Forms.Label();
			this.uctlGraphics = new Editor.CtrlComponent.Map.ctlEVGraphics();
			this.cmbAnimNum = new System.Windows.Forms.ComboBox();
			this.chkNoHit = new System.Windows.Forms.CheckBox();
			this.chkIsWalkAnim = new System.Windows.Forms.CheckBox();
			this.chkFixedDirection = new System.Windows.Forms.CheckBox();
			this.S_AnimNum = new System.Windows.Forms.Label();
			this.chkIsStopAnim = new System.Windows.Forms.CheckBox();
			this.S_DrawPriority = new System.Windows.Forms.Label();
			this.cmbDrawPriority = new System.Windows.Forms.ComboBox();
			this.cmbMoveType = new System.Windows.Forms.ComboBox();
			this.SplitContainer1 = new System.Windows.Forms.SplitContainer();
			this.GroupBox2 = new System.Windows.Forms.GroupBox();
			this.cmbStartIf = new System.Windows.Forms.ComboBox();
			this.GroupBox1 = new System.Windows.Forms.GroupBox();
			this.cmbDirection = new System.Windows.Forms.ComboBox();
			this.tbcEVScripts = new System.Windows.Forms.TabControl();
			this.TabPage1 = new System.Windows.Forms.TabPage();
			this.pnlEditor = new System.Windows.Forms.Panel();
			this.uctlTextEditor = new Editor.CtrlComponent.Text.ctlOneTextEditor();
			this.uctlEVCommandEditor = new Editor.CtrlComponent.Map.ctlEVEasyEditor();
			this.TabPage2 = new System.Windows.Forms.TabPage();
			this.TabPage3 = new System.Windows.Forms.TabPage();
			this.TabPage4 = new System.Windows.Forms.TabPage();
			this.lblComment = new System.Windows.Forms.Label();
			this.pnlCommandTree = new System.Windows.Forms.Panel();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.GroupBox3.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.SplitContainer1)).BeginInit();
			this.SplitContainer1.Panel1.SuspendLayout();
			this.SplitContainer1.Panel2.SuspendLayout();
			this.SplitContainer1.SuspendLayout();
			this.GroupBox2.SuspendLayout();
			this.GroupBox1.SuspendLayout();
			this.tbcEVScripts.SuspendLayout();
			this.TabPage1.SuspendLayout();
			this.pnlEditor.SuspendLayout();
			this.SuspendLayout();
			// 
			// S_StartIf
			// 
			this.S_StartIf.AutoSize = true;
			this.S_StartIf.ForeColor = System.Drawing.Color.Black;
			this.S_StartIf.Location = new System.Drawing.Point(10, 27);
			this.S_StartIf.Name = "S_StartIf";
			this.S_StartIf.Size = new System.Drawing.Size(60, 15);
			this.S_StartIf.TabIndex = 0;
			this.S_StartIf.Text = "起動条件:";
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.ForeColor = System.Drawing.Color.Black;
			this.Label1.Location = new System.Drawing.Point(10, 82);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(36, 15);
			this.Label1.TabIndex = 4;
			this.Label1.Text = "頻度:";
			// 
			// cmbFrequency
			// 
			this.cmbFrequency.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbFrequency.FormattingEnabled = true;
			this.cmbFrequency.Items.AddRange(new object[] {
            "すごく遅い",
            "遅い",
            "普通",
            "速い",
            "すごく速い"});
			this.cmbFrequency.Location = new System.Drawing.Point(73, 78);
			this.cmbFrequency.Name = "cmbFrequency";
			this.cmbFrequency.Size = new System.Drawing.Size(120, 23);
			this.cmbFrequency.TabIndex = 5;
			this.toolTip1.SetToolTip(this.cmbFrequency, "日常移動を「スクリプト指定」にしている場合は無効となります。");
			// 
			// S_Speed
			// 
			this.S_Speed.AutoSize = true;
			this.S_Speed.ForeColor = System.Drawing.Color.Black;
			this.S_Speed.Location = new System.Drawing.Point(10, 54);
			this.S_Speed.Name = "S_Speed";
			this.S_Speed.Size = new System.Drawing.Size(36, 15);
			this.S_Speed.TabIndex = 2;
			this.S_Speed.Text = "速度:";
			// 
			// S_Direction
			// 
			this.S_Direction.AutoSize = true;
			this.S_Direction.ForeColor = System.Drawing.Color.Black;
			this.S_Direction.Location = new System.Drawing.Point(10, 26);
			this.S_Direction.Name = "S_Direction";
			this.S_Direction.Size = new System.Drawing.Size(36, 15);
			this.S_Direction.TabIndex = 0;
			this.S_Direction.Text = "方向:";
			// 
			// cmbSpeed
			// 
			this.cmbSpeed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbSpeed.FormattingEnabled = true;
			this.cmbSpeed.Items.AddRange(new object[] {
            "すごく遅い",
            "遅い",
            "普通",
            "速い",
            "すごく速い"});
			this.cmbSpeed.Location = new System.Drawing.Point(73, 50);
			this.cmbSpeed.Name = "cmbSpeed";
			this.cmbSpeed.Size = new System.Drawing.Size(120, 23);
			this.cmbSpeed.TabIndex = 3;
			this.toolTip1.SetToolTip(this.cmbSpeed, "日常移動を「スクリプト指定」にしている場合は無効となります。");
			// 
			// S_MoveType
			// 
			this.S_MoveType.AutoSize = true;
			this.S_MoveType.ForeColor = System.Drawing.Color.Black;
			this.S_MoveType.Location = new System.Drawing.Point(10, 55);
			this.S_MoveType.Name = "S_MoveType";
			this.S_MoveType.Size = new System.Drawing.Size(60, 15);
			this.S_MoveType.TabIndex = 2;
			this.S_MoveType.Text = "日常移動:";
			// 
			// GroupBox3
			// 
			this.GroupBox3.Controls.Add(this.uctlHitRange);
			this.GroupBox3.Controls.Add(this.label2);
			this.GroupBox3.Controls.Add(this.uctlGraphics);
			this.GroupBox3.Controls.Add(this.cmbAnimNum);
			this.GroupBox3.Controls.Add(this.chkNoHit);
			this.GroupBox3.Controls.Add(this.chkIsWalkAnim);
			this.GroupBox3.Controls.Add(this.chkFixedDirection);
			this.GroupBox3.Controls.Add(this.S_AnimNum);
			this.GroupBox3.Controls.Add(this.chkIsStopAnim);
			this.GroupBox3.Controls.Add(this.S_DrawPriority);
			this.GroupBox3.Controls.Add(this.cmbDrawPriority);
			this.GroupBox3.ForeColor = System.Drawing.Color.Blue;
			this.GroupBox3.Location = new System.Drawing.Point(5, 231);
			this.GroupBox3.Name = "GroupBox3";
			this.GroupBox3.Size = new System.Drawing.Size(205, 278);
			this.GroupBox3.TabIndex = 2;
			this.GroupBox3.TabStop = false;
			this.GroupBox3.Text = "表示設定";
			// 
			// uctlHitRange
			// 
			this.uctlHitRange.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlHitRange.ForeColor = System.Drawing.Color.Black;
			this.uctlHitRange.Location = new System.Drawing.Point(13, 144);
			this.uctlHitRange.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlHitRange.Maximum = new System.Drawing.Point(500, 500);
			this.uctlHitRange.MaximumSize = new System.Drawing.Size(500, 23);
			this.uctlHitRange.Minimum = new System.Drawing.Point(0, 0);
			this.uctlHitRange.MinimumSize = new System.Drawing.Size(100, 23);
			this.uctlHitRange.Name = "uctlHitRange";
			this.uctlHitRange.Result = new System.Drawing.Point(0, 0);
			this.uctlHitRange.Size = new System.Drawing.Size(137, 23);
			this.uctlHitRange.TabIndex = 6;
			this.toolTip1.SetToolTip(this.uctlHitRange, "サイズがゼロのとき、イベントの座標に対してのみ接触判定が適用されます。");
			this.uctlHitRange.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Size;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.ForeColor = System.Drawing.Color.Black;
			this.label2.Location = new System.Drawing.Point(10, 124);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(94, 15);
			this.label2.TabIndex = 5;
			this.label2.Text = "接触範囲の拡張:";
			// 
			// uctlGraphics
			// 
			this.uctlGraphics.EnabledTileset = true;
			this.uctlGraphics.FileName = "";
			this.uctlGraphics.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlGraphics.Location = new System.Drawing.Point(10, 30);
			this.uctlGraphics.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlGraphics.Name = "uctlGraphics";
			this.uctlGraphics.OnlyTileset = false;
			this.uctlGraphics.Size = new System.Drawing.Size(80, 80);
			this.uctlGraphics.TabIndex = 0;
			// 
			// cmbAnimNum
			// 
			this.cmbAnimNum.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbAnimNum.FormattingEnabled = true;
			this.cmbAnimNum.Items.AddRange(new object[] {
            "0",
            "1（静止状態）",
            "2"});
			this.cmbAnimNum.Location = new System.Drawing.Point(106, 40);
			this.cmbAnimNum.Name = "cmbAnimNum";
			this.cmbAnimNum.Size = new System.Drawing.Size(87, 23);
			this.cmbAnimNum.TabIndex = 2;
			// 
			// chkNoHit
			// 
			this.chkNoHit.AutoSize = true;
			this.chkNoHit.ForeColor = System.Drawing.Color.Black;
			this.chkNoHit.Location = new System.Drawing.Point(13, 228);
			this.chkNoHit.Name = "chkNoHit";
			this.chkNoHit.Size = new System.Drawing.Size(75, 19);
			this.chkNoHit.TabIndex = 9;
			this.chkNoHit.Text = "すり抜ける";
			this.chkNoHit.UseVisualStyleBackColor = true;
			// 
			// chkIsWalkAnim
			// 
			this.chkIsWalkAnim.AutoSize = true;
			this.chkIsWalkAnim.ForeColor = System.Drawing.Color.Black;
			this.chkIsWalkAnim.Location = new System.Drawing.Point(13, 204);
			this.chkIsWalkAnim.Name = "chkIsWalkAnim";
			this.chkIsWalkAnim.Size = new System.Drawing.Size(151, 19);
			this.chkIsWalkAnim.TabIndex = 8;
			this.chkIsWalkAnim.Text = "移動時にアニメーションする";
			this.chkIsWalkAnim.UseVisualStyleBackColor = true;
			// 
			// chkFixedDirection
			// 
			this.chkFixedDirection.AutoSize = true;
			this.chkFixedDirection.ForeColor = System.Drawing.Color.Black;
			this.chkFixedDirection.Location = new System.Drawing.Point(13, 252);
			this.chkFixedDirection.Name = "chkFixedDirection";
			this.chkFixedDirection.Size = new System.Drawing.Size(102, 19);
			this.chkFixedDirection.TabIndex = 10;
			this.chkFixedDirection.Text = "方向を固定する";
			this.chkFixedDirection.UseVisualStyleBackColor = true;
			// 
			// S_AnimNum
			// 
			this.S_AnimNum.AutoSize = true;
			this.S_AnimNum.ForeColor = System.Drawing.Color.Black;
			this.S_AnimNum.Location = new System.Drawing.Point(95, 20);
			this.S_AnimNum.Name = "S_AnimNum";
			this.S_AnimNum.Size = new System.Drawing.Size(86, 15);
			this.S_AnimNum.TabIndex = 1;
			this.S_AnimNum.Text = "既定アニメ状態:";
			// 
			// chkIsStopAnim
			// 
			this.chkIsStopAnim.AutoSize = true;
			this.chkIsStopAnim.ForeColor = System.Drawing.Color.Black;
			this.chkIsStopAnim.Location = new System.Drawing.Point(13, 180);
			this.chkIsStopAnim.Name = "chkIsStopAnim";
			this.chkIsStopAnim.Size = new System.Drawing.Size(151, 19);
			this.chkIsStopAnim.TabIndex = 7;
			this.chkIsStopAnim.Text = "静止時にアニメーションする";
			this.chkIsStopAnim.UseVisualStyleBackColor = true;
			// 
			// S_DrawPriority
			// 
			this.S_DrawPriority.AutoSize = true;
			this.S_DrawPriority.ForeColor = System.Drawing.Color.Black;
			this.S_DrawPriority.Location = new System.Drawing.Point(95, 74);
			this.S_DrawPriority.Name = "S_DrawPriority";
			this.S_DrawPriority.Size = new System.Drawing.Size(72, 15);
			this.S_DrawPriority.TabIndex = 3;
			this.S_DrawPriority.Text = "描画優先度:";
			// 
			// cmbDrawPriority
			// 
			this.cmbDrawPriority.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbDrawPriority.FormattingEnabled = true;
			this.cmbDrawPriority.Items.AddRange(new object[] {
            "低：主人公の下",
            "中：主人公と同じ",
            "高：主人公の上",
            "最高：最前面表示"});
			this.cmbDrawPriority.Location = new System.Drawing.Point(106, 94);
			this.cmbDrawPriority.Name = "cmbDrawPriority";
			this.cmbDrawPriority.Size = new System.Drawing.Size(87, 23);
			this.cmbDrawPriority.TabIndex = 4;
			// 
			// cmbMoveType
			// 
			this.cmbMoveType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbMoveType.FormattingEnabled = true;
			this.cmbMoveType.Items.AddRange(new object[] {
            "スクリプト指定",
            "ランダム",
            "主人公に近づく",
            "主人公に本気で近づく",
            "主人公から遠ざかる",
            "主人公から本気で遠ざかる"});
			this.cmbMoveType.Location = new System.Drawing.Point(73, 50);
			this.cmbMoveType.Name = "cmbMoveType";
			this.cmbMoveType.Size = new System.Drawing.Size(120, 23);
			this.cmbMoveType.TabIndex = 3;
			// 
			// SplitContainer1
			// 
			this.SplitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.SplitContainer1.IsSplitterFixed = true;
			this.SplitContainer1.Location = new System.Drawing.Point(0, 0);
			this.SplitContainer1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.SplitContainer1.Name = "SplitContainer1";
			// 
			// SplitContainer1.Panel1
			// 
			this.SplitContainer1.Panel1.Controls.Add(this.GroupBox3);
			this.SplitContainer1.Panel1.Controls.Add(this.GroupBox2);
			this.SplitContainer1.Panel1.Controls.Add(this.GroupBox1);
			this.SplitContainer1.Panel1MinSize = 218;
			// 
			// SplitContainer1.Panel2
			// 
			this.SplitContainer1.Panel2.Controls.Add(this.tbcEVScripts);
			this.SplitContainer1.Panel2.Controls.Add(this.lblComment);
			this.SplitContainer1.Panel2.Controls.Add(this.pnlCommandTree);
			this.SplitContainer1.Size = new System.Drawing.Size(1040, 519);
			this.SplitContainer1.SplitterDistance = 218;
			this.SplitContainer1.SplitterWidth = 1;
			this.SplitContainer1.TabIndex = 1;
			// 
			// GroupBox2
			// 
			this.GroupBox2.Controls.Add(this.cmbStartIf);
			this.GroupBox2.Controls.Add(this.S_StartIf);
			this.GroupBox2.Controls.Add(this.cmbMoveType);
			this.GroupBox2.Controls.Add(this.S_MoveType);
			this.GroupBox2.ForeColor = System.Drawing.Color.Blue;
			this.GroupBox2.Location = new System.Drawing.Point(5, 10);
			this.GroupBox2.Name = "GroupBox2";
			this.GroupBox2.Size = new System.Drawing.Size(205, 86);
			this.GroupBox2.TabIndex = 0;
			this.GroupBox2.TabStop = false;
			this.GroupBox2.Text = "スクリプト設定";
			// 
			// cmbStartIf
			// 
			this.cmbStartIf.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbStartIf.FormattingEnabled = true;
			this.cmbStartIf.Items.AddRange(new object[] {
            "起動しない",
            "一致したら",
            "主人公から接触",
            "イベントから接触",
            "目の前で決定キー",
            "一致して決定キー",
            "マップ移動直後",
            "自動実行",
            "並列実行"});
			this.cmbStartIf.Location = new System.Drawing.Point(73, 22);
			this.cmbStartIf.Name = "cmbStartIf";
			this.cmbStartIf.Size = new System.Drawing.Size(120, 23);
			this.cmbStartIf.TabIndex = 1;
			// 
			// GroupBox1
			// 
			this.GroupBox1.Controls.Add(this.Label1);
			this.GroupBox1.Controls.Add(this.cmbFrequency);
			this.GroupBox1.Controls.Add(this.S_Speed);
			this.GroupBox1.Controls.Add(this.S_Direction);
			this.GroupBox1.Controls.Add(this.cmbSpeed);
			this.GroupBox1.Controls.Add(this.cmbDirection);
			this.GroupBox1.ForeColor = System.Drawing.Color.Blue;
			this.GroupBox1.Location = new System.Drawing.Point(5, 106);
			this.GroupBox1.Name = "GroupBox1";
			this.GroupBox1.Size = new System.Drawing.Size(205, 115);
			this.GroupBox1.TabIndex = 1;
			this.GroupBox1.TabStop = false;
			this.GroupBox1.Text = "移動設定";
			// 
			// cmbDirection
			// 
			this.cmbDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbDirection.FormattingEnabled = true;
			this.cmbDirection.Location = new System.Drawing.Point(73, 22);
			this.cmbDirection.Name = "cmbDirection";
			this.cmbDirection.Size = new System.Drawing.Size(120, 23);
			this.cmbDirection.TabIndex = 1;
			// 
			// tbcEVScripts
			// 
			this.tbcEVScripts.Controls.Add(this.TabPage1);
			this.tbcEVScripts.Controls.Add(this.TabPage2);
			this.tbcEVScripts.Controls.Add(this.TabPage3);
			this.tbcEVScripts.Controls.Add(this.TabPage4);
			this.tbcEVScripts.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tbcEVScripts.HotTrack = true;
			this.tbcEVScripts.ItemSize = new System.Drawing.Size(100, 22);
			this.tbcEVScripts.Location = new System.Drawing.Point(217, 37);
			this.tbcEVScripts.Name = "tbcEVScripts";
			this.tbcEVScripts.SelectedIndex = 0;
			this.tbcEVScripts.Size = new System.Drawing.Size(604, 482);
			this.tbcEVScripts.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
			this.tbcEVScripts.TabIndex = 1;
			this.tbcEVScripts.Selected += new System.Windows.Forms.TabControlEventHandler(this.tbcEVScripts_Selected);
			// 
			// TabPage1
			// 
			this.TabPage1.Controls.Add(this.pnlEditor);
			this.TabPage1.Location = new System.Drawing.Point(4, 26);
			this.TabPage1.Name = "TabPage1";
			this.TabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.TabPage1.Size = new System.Drawing.Size(596, 452);
			this.TabPage1.TabIndex = 0;
			this.TabPage1.Text = "ページ条件";
			this.TabPage1.UseVisualStyleBackColor = true;
			// 
			// pnlEditor
			// 
			this.pnlEditor.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pnlEditor.Controls.Add(this.uctlTextEditor);
			this.pnlEditor.Controls.Add(this.uctlEVCommandEditor);
			this.pnlEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlEditor.Location = new System.Drawing.Point(3, 3);
			this.pnlEditor.Margin = new System.Windows.Forms.Padding(0);
			this.pnlEditor.Name = "pnlEditor";
			this.pnlEditor.Size = new System.Drawing.Size(590, 446);
			this.pnlEditor.TabIndex = 0;
			// 
			// uctlTextEditor
			// 
			this.uctlTextEditor.CanSQTest = false;
			this.uctlTextEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.uctlTextEditor.EdittingText = "";
			this.uctlTextEditor.FileName = null;
			this.uctlTextEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlTextEditor.IsDirty = false;
			this.uctlTextEditor.IsReadOnly = false;
			this.uctlTextEditor.IsScriptMode = true;
			this.uctlTextEditor.Location = new System.Drawing.Point(0, 0);
			this.uctlTextEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlTextEditor.Name = "uctlTextEditor";
			this.uctlTextEditor.Size = new System.Drawing.Size(573, 479);
			this.uctlTextEditor.TabIndex = 1;
			this.uctlTextEditor.Visible = false;
			// 
			// uctlEVCommandEditor
			// 
			this.uctlEVCommandEditor.CurrentIndex = 0;
			this.uctlEVCommandEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.uctlEVCommandEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlEVCommandEditor.IsDirty = false;
			this.uctlEVCommandEditor.Location = new System.Drawing.Point(0, 0);
			this.uctlEVCommandEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlEVCommandEditor.Name = "uctlEVCommandEditor";
			this.uctlEVCommandEditor.Size = new System.Drawing.Size(586, 442);
			this.uctlEVCommandEditor.TabIndex = 0;
			this.uctlEVCommandEditor.Visible = false;
			// 
			// TabPage2
			// 
			this.TabPage2.Location = new System.Drawing.Point(4, 26);
			this.TabPage2.Name = "TabPage2";
			this.TabPage2.Padding = new System.Windows.Forms.Padding(3);
			this.TabPage2.Size = new System.Drawing.Size(596, 452);
			this.TabPage2.TabIndex = 1;
			this.TabPage2.Text = "起動条件";
			this.TabPage2.UseVisualStyleBackColor = true;
			// 
			// TabPage3
			// 
			this.TabPage3.Location = new System.Drawing.Point(4, 26);
			this.TabPage3.Name = "TabPage3";
			this.TabPage3.Padding = new System.Windows.Forms.Padding(3);
			this.TabPage3.Size = new System.Drawing.Size(596, 452);
			this.TabPage3.TabIndex = 2;
			this.TabPage3.Text = "日常移動";
			this.TabPage3.UseVisualStyleBackColor = true;
			// 
			// TabPage4
			// 
			this.TabPage4.Location = new System.Drawing.Point(4, 26);
			this.TabPage4.Name = "TabPage4";
			this.TabPage4.Padding = new System.Windows.Forms.Padding(3);
			this.TabPage4.Size = new System.Drawing.Size(596, 452);
			this.TabPage4.TabIndex = 3;
			this.TabPage4.Text = "実行内容";
			this.TabPage4.UseVisualStyleBackColor = true;
			// 
			// lblComment
			// 
			this.lblComment.BackColor = System.Drawing.SystemColors.Control;
			this.lblComment.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.lblComment.Dock = System.Windows.Forms.DockStyle.Top;
			this.lblComment.Location = new System.Drawing.Point(217, 0);
			this.lblComment.Name = "lblComment";
			this.lblComment.Size = new System.Drawing.Size(604, 37);
			this.lblComment.TabIndex = 2;
			// 
			// pnlCommandTree
			// 
			this.pnlCommandTree.Dock = System.Windows.Forms.DockStyle.Left;
			this.pnlCommandTree.Location = new System.Drawing.Point(0, 0);
			this.pnlCommandTree.Name = "pnlCommandTree";
			this.pnlCommandTree.Size = new System.Drawing.Size(217, 519);
			this.pnlCommandTree.TabIndex = 3;
			// 
			// ctlEVPage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Control;
			this.Controls.Add(this.SplitContainer1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlEVPage";
			this.Size = new System.Drawing.Size(1040, 519);
			this.Load += new System.EventHandler(this.ctlEVPage_Load);
			this.GroupBox3.ResumeLayout(false);
			this.GroupBox3.PerformLayout();
			this.SplitContainer1.Panel1.ResumeLayout(false);
			this.SplitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.SplitContainer1)).EndInit();
			this.SplitContainer1.ResumeLayout(false);
			this.GroupBox2.ResumeLayout(false);
			this.GroupBox2.PerformLayout();
			this.GroupBox1.ResumeLayout(false);
			this.GroupBox1.PerformLayout();
			this.tbcEVScripts.ResumeLayout(false);
			this.TabPage1.ResumeLayout(false);
			this.pnlEditor.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		internal System.Windows.Forms.Label S_StartIf;
		internal System.Windows.Forms.Label Label1;
		internal System.Windows.Forms.ComboBox cmbFrequency;
		internal System.Windows.Forms.Label S_Speed;
		internal System.Windows.Forms.Label S_Direction;
		internal System.Windows.Forms.ComboBox cmbSpeed;
		internal System.Windows.Forms.Label S_MoveType;
		internal System.Windows.Forms.GroupBox GroupBox3;
		internal System.Windows.Forms.ComboBox cmbAnimNum;
		internal System.Windows.Forms.CheckBox chkNoHit;
		internal System.Windows.Forms.CheckBox chkIsWalkAnim;
		internal System.Windows.Forms.CheckBox chkFixedDirection;
		internal System.Windows.Forms.Label S_AnimNum;
		internal System.Windows.Forms.CheckBox chkIsStopAnim;
		internal System.Windows.Forms.Label S_DrawPriority;
		internal System.Windows.Forms.ComboBox cmbDrawPriority;
		internal System.Windows.Forms.ComboBox cmbMoveType;
		internal System.Windows.Forms.SplitContainer SplitContainer1;
		internal System.Windows.Forms.GroupBox GroupBox2;
		internal System.Windows.Forms.ComboBox cmbStartIf;
		internal System.Windows.Forms.GroupBox GroupBox1;
		internal System.Windows.Forms.ComboBox cmbDirection;
		internal System.Windows.Forms.TabControl tbcEVScripts;
		internal System.Windows.Forms.TabPage TabPage1;
		internal System.Windows.Forms.TabPage TabPage2;
		internal System.Windows.Forms.TabPage TabPage3;
		internal System.Windows.Forms.TabPage TabPage4;
		private ctlEVGraphics uctlGraphics;
		private Common.ctlPosition uctlHitRange;
		internal System.Windows.Forms.Label label2;
		private System.Windows.Forms.Panel pnlEditor;
		private System.Windows.Forms.Label lblComment;
		private System.Windows.Forms.ToolTip toolTip1;
		public System.Windows.Forms.Panel pnlCommandTree;
		public ctlEVEasyEditor uctlEVCommandEditor;
		public Text.ctlOneTextEditor uctlTextEditor;
	}
}
