namespace Editor.CtrlComponent.Text {
	partial class ctlOneTextEditor {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlOneTextEditor));
            Sgry.Azuki.FontInfo fontInfo1 = new Sgry.Azuki.FontInfo();
            this.ctmInsertReserchCode = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.ctmSelctAll = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.ctmPaste = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmCopy = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmCut = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.ctmRedo = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmUndo = new System.Windows.Forms.ToolStripMenuItem();
            this.cmnTextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ctmFindReplace = new System.Windows.Forms.ToolStripMenuItem();
            this.ctmInsertCode = new System.Windows.Forms.ToolStripMenuItem();
            this.tolUndoComment = new System.Windows.Forms.ToolStripButton();
            this.tolDoComment = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.tolJumpList = new System.Windows.Forms.ToolStripButton();
            this.tolInsertCode = new System.Windows.Forms.ToolStripButton();
            this.tolSplitSQCode = new System.Windows.Forms.ToolStripSeparator();
            this.ToolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.tolPaste = new System.Windows.Forms.ToolStripButton();
            this.tolCopy = new System.Windows.Forms.ToolStripButton();
            this.tolCut = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.tolRedo = new System.Windows.Forms.ToolStripButton();
            this.tolUndo = new System.Windows.Forms.ToolStripButton();
            this.ToolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.tolSave = new System.Windows.Forms.ToolStripButton();
            this.ToolStrip1 = new System.Windows.Forms.ToolStrip();
            this.tolInsertReserchCode = new System.Windows.Forms.ToolStripButton();
            this.tolSplitSQTest = new System.Windows.Forms.ToolStripSeparator();
            this.tolTestCommand = new System.Windows.Forms.ToolStripComboBox();
            this.tolSQTest = new System.Windows.Forms.ToolStripButton();
            this.azkEditor = new Sgry.Azuki.WinForms.AzukiControl();
            this.txtCodeComment = new System.Windows.Forms.ToolStripTextBox();
            this.txtAutoCompleteInserter = new System.Windows.Forms.ToolStripTextBox();
            this.autoComplete = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.txtCodeTypeName = new System.Windows.Forms.ToolStripTextBox();
            this.cmnTextMenu.SuspendLayout();
            this.ToolStrip1.SuspendLayout();
            this.autoComplete.SuspendLayout();
            this.SuspendLayout();
            // 
            // ctmInsertReserchCode
            // 
            this.ctmInsertReserchCode.Image = ((System.Drawing.Image)(resources.GetObject("ctmInsertReserchCode.Image")));
            this.ctmInsertReserchCode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmInsertReserchCode.Name = "ctmInsertReserchCode";
            this.ctmInsertReserchCode.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.Space)));
            this.ctmInsertReserchCode.Size = new System.Drawing.Size(221, 22);
            this.ctmInsertReserchCode.Tag = "Script";
            this.ctmInsertReserchCode.Text = "コード逆検索(&E)";
            this.ctmInsertReserchCode.Click += new System.EventHandler(this.tolInsertReserchCode_Click);
            // 
            // ToolStripSeparator8
            // 
            this.ToolStripSeparator8.Name = "ToolStripSeparator8";
            this.ToolStripSeparator8.Size = new System.Drawing.Size(218, 6);
            this.ToolStripSeparator8.Tag = "Script";
            // 
            // ctmSelctAll
            // 
            this.ctmSelctAll.Image = ((System.Drawing.Image)(resources.GetObject("ctmSelctAll.Image")));
            this.ctmSelctAll.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmSelctAll.Name = "ctmSelctAll";
            this.ctmSelctAll.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.ctmSelctAll.Size = new System.Drawing.Size(221, 22);
            this.ctmSelctAll.Text = "すべて選択(&A)";
            this.ctmSelctAll.Click += new System.EventHandler(this.ctmSelctAll_Click);
            // 
            // ToolStripSeparator7
            // 
            this.ToolStripSeparator7.Name = "ToolStripSeparator7";
            this.ToolStripSeparator7.Size = new System.Drawing.Size(218, 6);
            // 
            // ctmPaste
            // 
            this.ctmPaste.Image = ((System.Drawing.Image)(resources.GetObject("ctmPaste.Image")));
            this.ctmPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmPaste.Name = "ctmPaste";
            this.ctmPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.ctmPaste.Size = new System.Drawing.Size(221, 22);
            this.ctmPaste.Text = "貼り付け(&P)";
            this.ctmPaste.Click += new System.EventHandler(this.tolPaste_Click);
            // 
            // ctmCopy
            // 
            this.ctmCopy.Image = ((System.Drawing.Image)(resources.GetObject("ctmCopy.Image")));
            this.ctmCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmCopy.Name = "ctmCopy";
            this.ctmCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.ctmCopy.Size = new System.Drawing.Size(221, 22);
            this.ctmCopy.Text = "コピー(&C)";
            this.ctmCopy.Click += new System.EventHandler(this.tolCopy_Click);
            // 
            // ctmCut
            // 
            this.ctmCut.Image = ((System.Drawing.Image)(resources.GetObject("ctmCut.Image")));
            this.ctmCut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmCut.Name = "ctmCut";
            this.ctmCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.ctmCut.Size = new System.Drawing.Size(221, 22);
            this.ctmCut.Text = "切り取り(&T)";
            this.ctmCut.Click += new System.EventHandler(this.tolCut_Click);
            // 
            // ToolStripSeparator3
            // 
            this.ToolStripSeparator3.Name = "ToolStripSeparator3";
            this.ToolStripSeparator3.Size = new System.Drawing.Size(218, 6);
            // 
            // ctmRedo
            // 
            this.ctmRedo.Image = ((System.Drawing.Image)(resources.GetObject("ctmRedo.Image")));
            this.ctmRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmRedo.Name = "ctmRedo";
            this.ctmRedo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.ctmRedo.Size = new System.Drawing.Size(221, 22);
            this.ctmRedo.Text = "やり直し(&R)";
            this.ctmRedo.Click += new System.EventHandler(this.tolRedo_Click);
            // 
            // ctmUndo
            // 
            this.ctmUndo.Image = ((System.Drawing.Image)(resources.GetObject("ctmUndo.Image")));
            this.ctmUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmUndo.Name = "ctmUndo";
            this.ctmUndo.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.ctmUndo.Size = new System.Drawing.Size(221, 22);
            this.ctmUndo.Text = "元に戻す(&U)";
            this.ctmUndo.Click += new System.EventHandler(this.tolUndo_Click);
            // 
            // cmnTextMenu
            // 
            this.cmnTextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ctmUndo,
            this.ctmRedo,
            this.ToolStripSeparator3,
            this.ctmCut,
            this.ctmCopy,
            this.ctmPaste,
            this.ToolStripSeparator7,
            this.ctmSelctAll,
            this.ToolStripSeparator8,
            this.ctmFindReplace,
            this.ctmInsertCode,
            this.ctmInsertReserchCode});
            this.cmnTextMenu.Name = "cmnTextMenu";
            this.cmnTextMenu.Size = new System.Drawing.Size(222, 220);
            // 
            // ctmFindReplace
            // 
            this.ctmFindReplace.Image = ((System.Drawing.Image)(resources.GetObject("ctmFindReplace.Image")));
            this.ctmFindReplace.Name = "ctmFindReplace";
            this.ctmFindReplace.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.ctmFindReplace.Size = new System.Drawing.Size(221, 22);
            this.ctmFindReplace.Text = "検索と置換(&F)";
            this.ctmFindReplace.Click += new System.EventHandler(this.ctmFindReplace_Click);
            // 
            // ctmInsertCode
            // 
            this.ctmInsertCode.Image = ((System.Drawing.Image)(resources.GetObject("ctmInsertCode.Image")));
            this.ctmInsertCode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ctmInsertCode.Name = "ctmInsertCode";
            this.ctmInsertCode.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Space)));
            this.ctmInsertCode.Size = new System.Drawing.Size(221, 22);
            this.ctmInsertCode.Tag = "Script";
            this.ctmInsertCode.Text = "コード補完(&S)";
            this.ctmInsertCode.Click += new System.EventHandler(this.tolInsertCode_Click);
            // 
            // tolUndoComment
            // 
            this.tolUndoComment.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolUndoComment.Image = ((System.Drawing.Image)(resources.GetObject("tolUndoComment.Image")));
            this.tolUndoComment.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolUndoComment.Name = "tolUndoComment";
            this.tolUndoComment.Size = new System.Drawing.Size(23, 22);
            this.tolUndoComment.Tag = "Script";
            this.tolUndoComment.Text = "コメントアウトを解除";
            this.tolUndoComment.Click += new System.EventHandler(this.tolUndoComment_Click);
            // 
            // tolDoComment
            // 
            this.tolDoComment.Image = ((System.Drawing.Image)(resources.GetObject("tolDoComment.Image")));
            this.tolDoComment.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolDoComment.Name = "tolDoComment";
            this.tolDoComment.Size = new System.Drawing.Size(86, 22);
            this.tolDoComment.Tag = "Script";
            this.tolDoComment.Text = "コメントアウト";
            this.tolDoComment.Click += new System.EventHandler(this.tolDoComment_Click);
            // 
            // ToolStripSeparator5
            // 
            this.ToolStripSeparator5.Name = "ToolStripSeparator5";
            this.ToolStripSeparator5.Size = new System.Drawing.Size(6, 25);
            this.ToolStripSeparator5.Tag = "Script";
            // 
            // tolJumpList
            // 
            this.tolJumpList.Image = ((System.Drawing.Image)(resources.GetObject("tolJumpList.Image")));
            this.tolJumpList.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolJumpList.Name = "tolJumpList";
            this.tolJumpList.Size = new System.Drawing.Size(79, 22);
            this.tolJumpList.Text = "ラベル一覧";
            this.tolJumpList.Click += new System.EventHandler(this.tolJumpList_Click);
            // 
            // tolInsertCode
            // 
            this.tolInsertCode.Image = ((System.Drawing.Image)(resources.GetObject("tolInsertCode.Image")));
            this.tolInsertCode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolInsertCode.Name = "tolInsertCode";
            this.tolInsertCode.Size = new System.Drawing.Size(77, 22);
            this.tolInsertCode.Tag = "Script";
            this.tolInsertCode.Text = "コード補完";
            this.tolInsertCode.ToolTipText = "スクリプトコードの入力補完を行います。";
            this.tolInsertCode.Click += new System.EventHandler(this.tolInsertCode_Click);
            // 
            // tolSplitSQCode
            // 
            this.tolSplitSQCode.Name = "tolSplitSQCode";
            this.tolSplitSQCode.Size = new System.Drawing.Size(6, 25);
            this.tolSplitSQCode.Tag = "Script";
            // 
            // ToolStripSeparator2
            // 
            this.ToolStripSeparator2.Name = "ToolStripSeparator2";
            this.ToolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // tolPaste
            // 
            this.tolPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolPaste.Image = ((System.Drawing.Image)(resources.GetObject("tolPaste.Image")));
            this.tolPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolPaste.Name = "tolPaste";
            this.tolPaste.Size = new System.Drawing.Size(23, 22);
            this.tolPaste.Text = "貼り付け";
            this.tolPaste.Click += new System.EventHandler(this.tolPaste_Click);
            // 
            // tolCopy
            // 
            this.tolCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolCopy.Image = ((System.Drawing.Image)(resources.GetObject("tolCopy.Image")));
            this.tolCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolCopy.Name = "tolCopy";
            this.tolCopy.Size = new System.Drawing.Size(23, 22);
            this.tolCopy.Text = "コピー";
            this.tolCopy.Click += new System.EventHandler(this.tolCopy_Click);
            // 
            // tolCut
            // 
            this.tolCut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolCut.Image = ((System.Drawing.Image)(resources.GetObject("tolCut.Image")));
            this.tolCut.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolCut.Name = "tolCut";
            this.tolCut.Size = new System.Drawing.Size(23, 22);
            this.tolCut.Text = "切り取り";
            this.tolCut.Click += new System.EventHandler(this.tolCut_Click);
            // 
            // ToolStripSeparator1
            // 
            this.ToolStripSeparator1.Name = "ToolStripSeparator1";
            this.ToolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // tolRedo
            // 
            this.tolRedo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolRedo.Enabled = false;
            this.tolRedo.Image = ((System.Drawing.Image)(resources.GetObject("tolRedo.Image")));
            this.tolRedo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolRedo.Name = "tolRedo";
            this.tolRedo.Size = new System.Drawing.Size(23, 22);
            this.tolRedo.Text = "やり直し";
            this.tolRedo.Click += new System.EventHandler(this.tolRedo_Click);
            // 
            // tolUndo
            // 
            this.tolUndo.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tolUndo.Enabled = false;
            this.tolUndo.Image = ((System.Drawing.Image)(resources.GetObject("tolUndo.Image")));
            this.tolUndo.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolUndo.Name = "tolUndo";
            this.tolUndo.Size = new System.Drawing.Size(23, 22);
            this.tolUndo.Text = "元に戻す";
            this.tolUndo.Click += new System.EventHandler(this.tolUndo_Click);
            // 
            // ToolStripSeparator4
            // 
            this.ToolStripSeparator4.Name = "ToolStripSeparator4";
            this.ToolStripSeparator4.Size = new System.Drawing.Size(6, 25);
            // 
            // tolSave
            // 
            this.tolSave.Image = ((System.Drawing.Image)(resources.GetObject("tolSave.Image")));
            this.tolSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolSave.Name = "tolSave";
            this.tolSave.Size = new System.Drawing.Size(51, 22);
            this.tolSave.Text = "保存";
            this.tolSave.Click += new System.EventHandler(this.tolSave_Click);
            // 
            // ToolStrip1
            // 
            this.ToolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolSave,
            this.ToolStripSeparator4,
            this.tolUndo,
            this.tolRedo,
            this.ToolStripSeparator1,
            this.tolCut,
            this.tolCopy,
            this.tolPaste,
            this.ToolStripSeparator2,
            this.tolJumpList,
            this.tolSplitSQCode,
            this.tolInsertCode,
            this.tolInsertReserchCode,
            this.ToolStripSeparator5,
            this.tolDoComment,
            this.tolUndoComment,
            this.tolSplitSQTest,
            this.tolTestCommand,
            this.tolSQTest});
            this.ToolStrip1.Location = new System.Drawing.Point(0, 0);
            this.ToolStrip1.Name = "ToolStrip1";
            this.ToolStrip1.Padding = new System.Windows.Forms.Padding(0);
            this.ToolStrip1.Size = new System.Drawing.Size(974, 25);
            this.ToolStrip1.TabIndex = 0;
            this.ToolStrip1.Text = "ToolStrip1";
            // 
            // tolInsertReserchCode
            // 
            this.tolInsertReserchCode.Image = ((System.Drawing.Image)(resources.GetObject("tolInsertReserchCode.Image")));
            this.tolInsertReserchCode.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolInsertReserchCode.Name = "tolInsertReserchCode";
            this.tolInsertReserchCode.Size = new System.Drawing.Size(89, 22);
            this.tolInsertReserchCode.Tag = "Script";
            this.tolInsertReserchCode.Text = "コード逆検索";
            this.tolInsertReserchCode.ToolTipText = "選択しているコードから対応するコマンドツリーやデータベースのIDを検索します。";
            this.tolInsertReserchCode.Click += new System.EventHandler(this.tolInsertReserchCode_Click);
            // 
            // tolSplitSQTest
            // 
            this.tolSplitSQTest.Name = "tolSplitSQTest";
            this.tolSplitSQTest.Size = new System.Drawing.Size(6, 25);
            // 
            // tolTestCommand
            // 
            this.tolTestCommand.Name = "tolTestCommand";
            this.tolTestCommand.Size = new System.Drawing.Size(200, 25);
            this.tolTestCommand.ToolTipText = "テスターに送るコマンドを記述します。";
            this.tolTestCommand.DropDown += new System.EventHandler(this.tolTestCommand_DropDown);
            this.tolTestCommand.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tolTestCommand_KeyDown);
            // 
            // tolSQTest
            // 
            this.tolSQTest.Image = ((System.Drawing.Image)(resources.GetObject("tolSQTest.Image")));
            this.tolSQTest.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tolSQTest.Name = "tolSQTest";
            this.tolSQTest.Size = new System.Drawing.Size(77, 22);
            this.tolSQTest.Text = "即席テスト";
            this.tolSQTest.ToolTipText = "このスクリプトについてのテストを行います。\r\n左隣の入力欄に任意の呼び出しコードを記述してテストを実行して下さい。";
            this.tolSQTest.Click += new System.EventHandler(this.tolSQTest_Click);
            // 
            // azkEditor
            // 
            this.azkEditor.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(250)))), ((int)(((byte)(240)))));
            this.azkEditor.ContextMenuStrip = this.cmnTextMenu;
            this.azkEditor.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.azkEditor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.azkEditor.DrawingOption = ((Sgry.Azuki.DrawingOption)(((((Sgry.Azuki.DrawingOption.HighlightCurrentLine | Sgry.Azuki.DrawingOption.ShowsLineNumber) 
            | Sgry.Azuki.DrawingOption.ShowsHRuler) 
            | Sgry.Azuki.DrawingOption.ShowsDirtBar) 
            | Sgry.Azuki.DrawingOption.HighlightsMatchedBracket)));
            this.azkEditor.DrawsEolCode = false;
            this.azkEditor.DrawsFullWidthSpace = false;
            this.azkEditor.DrawsTab = false;
            this.azkEditor.FirstVisibleLine = 0;
            this.azkEditor.Font = new System.Drawing.Font("Meiryo UI", 9F);
            fontInfo1.Name = "Meiryo UI";
            fontInfo1.Size = 9;
            fontInfo1.Style = System.Drawing.FontStyle.Regular;
            this.azkEditor.FontInfo = fontInfo1;
            this.azkEditor.ForeColor = System.Drawing.Color.Black;
            this.azkEditor.Location = new System.Drawing.Point(0, 25);
            this.azkEditor.Name = "azkEditor";
            this.azkEditor.ScrollPos = new System.Drawing.Point(0, 0);
            this.azkEditor.ShowsHRuler = true;
            this.azkEditor.Size = new System.Drawing.Size(974, 467);
            this.azkEditor.TabIndex = 1;
            this.azkEditor.TabWidth = 4;
            this.azkEditor.ViewWidth = 4133;
            this.azkEditor.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.azkEditor_MouseDoubleClick);
            // 
            // txtCodeComment
            // 
            this.txtCodeComment.AutoSize = false;
            this.txtCodeComment.AutoToolTip = true;
            this.txtCodeComment.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtCodeComment.Enabled = false;
            this.txtCodeComment.Name = "txtCodeComment";
            this.txtCodeComment.ShortcutsEnabled = false;
            this.txtCodeComment.Size = new System.Drawing.Size(300, 23);
            // 
            // txtAutoCompleteInserter
            // 
            this.txtAutoCompleteInserter.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.txtAutoCompleteInserter.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
            this.txtAutoCompleteInserter.AutoSize = false;
            this.txtAutoCompleteInserter.Margin = new System.Windows.Forms.Padding(0);
            this.txtAutoCompleteInserter.MaxLength = 255;
            this.txtAutoCompleteInserter.Name = "txtAutoCompleteInserter";
            this.txtAutoCompleteInserter.Overflow = System.Windows.Forms.ToolStripItemOverflow.Never;
            this.txtAutoCompleteInserter.Size = new System.Drawing.Size(300, 23);
            // 
            // autoComplete
            // 
            this.autoComplete.AllowMerge = false;
            this.autoComplete.AutoSize = false;
            this.autoComplete.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.autoComplete.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.txtCodeTypeName,
            this.txtCodeComment,
            this.txtAutoCompleteInserter});
            this.autoComplete.Name = "ContextMenuStrip1";
            this.autoComplete.ShowImageMargin = false;
            this.autoComplete.ShowItemToolTips = false;
            this.autoComplete.Size = new System.Drawing.Size(321, 90);
            // 
            // txtCodeTypeName
            // 
            this.txtCodeTypeName.AutoSize = false;
            this.txtCodeTypeName.AutoToolTip = true;
            this.txtCodeTypeName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtCodeTypeName.Enabled = false;
            this.txtCodeTypeName.Name = "txtCodeTypeName";
            this.txtCodeTypeName.ShortcutsEnabled = false;
            this.txtCodeTypeName.Size = new System.Drawing.Size(300, 23);
            // 
            // ctlOneTextEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.azkEditor);
            this.Controls.Add(this.ToolStrip1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ctlOneTextEditor";
            this.Size = new System.Drawing.Size(974, 492);
            this.cmnTextMenu.ResumeLayout(false);
            this.ToolStrip1.ResumeLayout(false);
            this.ToolStrip1.PerformLayout();
            this.autoComplete.ResumeLayout(false);
            this.autoComplete.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.ToolStripMenuItem ctmFindReplace;
		private System.Windows.Forms.ToolStripMenuItem ctmInsertReserchCode;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator8;
		private System.Windows.Forms.ToolStripMenuItem ctmSelctAll;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator7;
		private System.Windows.Forms.ToolStripMenuItem ctmPaste;
		private System.Windows.Forms.ToolStripMenuItem ctmCopy;
		private System.Windows.Forms.ToolStripMenuItem ctmCut;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator3;
		private System.Windows.Forms.ToolStripMenuItem ctmRedo;
		private System.Windows.Forms.ToolStripMenuItem ctmUndo;
		private System.Windows.Forms.ContextMenuStrip cmnTextMenu;
		private System.Windows.Forms.ToolStripMenuItem ctmInsertCode;
		private System.Windows.Forms.ToolStripButton tolUndoComment;
		private System.Windows.Forms.ToolStripButton tolDoComment;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator5;
		private System.Windows.Forms.ToolStripButton tolJumpList;
		private System.Windows.Forms.ToolStripButton tolInsertCode;
		private System.Windows.Forms.ToolStripSeparator tolSplitSQCode;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator2;
		private System.Windows.Forms.ToolStripButton tolPaste;
		private System.Windows.Forms.ToolStripButton tolCopy;
		private System.Windows.Forms.ToolStripButton tolCut;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator1;
		private System.Windows.Forms.ToolStripButton tolRedo;
		private System.Windows.Forms.ToolStripButton tolUndo;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator4;
		private System.Windows.Forms.ToolStripButton tolSave;
		private System.Windows.Forms.ToolStrip ToolStrip1;
		private System.Windows.Forms.ToolStripButton tolInsertReserchCode;
		public Sgry.Azuki.WinForms.AzukiControl azkEditor;
		private System.Windows.Forms.ToolStripSeparator tolSplitSQTest;
		internal System.Windows.Forms.ToolStripButton tolSQTest;
		private System.Windows.Forms.ToolStripTextBox txtCodeComment;
		public System.Windows.Forms.ContextMenuStrip autoComplete;
		public System.Windows.Forms.ToolStripTextBox txtAutoCompleteInserter;
		private System.Windows.Forms.ToolStripTextBox txtCodeTypeName;
		private System.Windows.Forms.ToolStripComboBox tolTestCommand;
	}
}
