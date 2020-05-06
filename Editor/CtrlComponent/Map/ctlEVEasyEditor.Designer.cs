namespace Editor.CtrlComponent.Map {
	partial class ctlEVEasyEditor {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlEVEasyEditor));
			this.ToolStrip1 = new System.Windows.Forms.ToolStrip();
			this.tolCut = new System.Windows.Forms.ToolStripButton();
			this.tolCopy = new System.Windows.Forms.ToolStripButton();
			this.tolPaste = new System.Windows.Forms.ToolStripButton();
			this.tolDelete = new System.Windows.Forms.ToolStripButton();
			this.cmnTextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.ctmDirectEdit = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.ctmCut = new System.Windows.Forms.ToolStripMenuItem();
			this.ctmCopy = new System.Windows.Forms.ToolStripMenuItem();
			this.ctmPaste = new System.Windows.Forms.ToolStripMenuItem();
			this.ctmDelete = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
			this.ctmSelctAll = new System.Windows.Forms.ToolStripMenuItem();
			this.lstEditor = new System.Windows.Forms.ListBox();
			this.ToolStrip1.SuspendLayout();
			this.cmnTextMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// ToolStrip1
			// 
			this.ToolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolCut,
            this.tolCopy,
            this.tolPaste,
            this.tolDelete});
			this.ToolStrip1.Location = new System.Drawing.Point(0, 0);
			this.ToolStrip1.Name = "ToolStrip1";
			this.ToolStrip1.Padding = new System.Windows.Forms.Padding(0);
			this.ToolStrip1.Size = new System.Drawing.Size(235, 25);
			this.ToolStrip1.TabIndex = 0;
			this.ToolStrip1.Text = "ToolStrip1";
			// 
			// tolCut
			// 
			this.tolCut.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolCut.Image = ((System.Drawing.Image)(resources.GetObject("tolCut.Image")));
			this.tolCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolCut.Name = "tolCut";
			this.tolCut.Size = new System.Drawing.Size(23, 22);
			this.tolCut.Text = "切り取り";
			this.tolCut.ToolTipText = "コマンドを切り取り";
			this.tolCut.Click += new System.EventHandler(this.ctmCut_Click);
			// 
			// tolCopy
			// 
			this.tolCopy.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolCopy.Image = ((System.Drawing.Image)(resources.GetObject("tolCopy.Image")));
			this.tolCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolCopy.Name = "tolCopy";
			this.tolCopy.Size = new System.Drawing.Size(23, 22);
			this.tolCopy.Text = "コピー";
			this.tolCopy.ToolTipText = "コマンドをコピー";
			this.tolCopy.Click += new System.EventHandler(this.ctmCopy_Click);
			// 
			// tolPaste
			// 
			this.tolPaste.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolPaste.Image = ((System.Drawing.Image)(resources.GetObject("tolPaste.Image")));
			this.tolPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolPaste.Name = "tolPaste";
			this.tolPaste.Size = new System.Drawing.Size(23, 22);
			this.tolPaste.Text = "貼り付け";
			this.tolPaste.ToolTipText = "コマンドを貼り付け";
			this.tolPaste.Click += new System.EventHandler(this.ctmPaste_Click);
			// 
			// tolDelete
			// 
			this.tolDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.tolDelete.Image = ((System.Drawing.Image)(resources.GetObject("tolDelete.Image")));
			this.tolDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolDelete.Name = "tolDelete";
			this.tolDelete.Size = new System.Drawing.Size(23, 22);
			this.tolDelete.Text = "削除";
			this.tolDelete.ToolTipText = "コマンドを削除";
			this.tolDelete.Click += new System.EventHandler(this.ctmDelete_Click);
			// 
			// cmnTextMenu
			// 
			this.cmnTextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ctmDirectEdit,
            this.toolStripSeparator1,
            this.ctmCut,
            this.ctmCopy,
            this.ctmPaste,
            this.ctmDelete,
            this.ToolStripSeparator7,
            this.ctmSelctAll});
			this.cmnTextMenu.Name = "cmnTextMenu";
			this.cmnTextMenu.Size = new System.Drawing.Size(247, 148);
			// 
			// ctmDirectEdit
			// 
			this.ctmDirectEdit.Image = ((System.Drawing.Image)(resources.GetObject("ctmDirectEdit.Image")));
			this.ctmDirectEdit.Name = "ctmDirectEdit";
			this.ctmDirectEdit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Space)));
			this.ctmDirectEdit.Size = new System.Drawing.Size(246, 22);
			this.ctmDirectEdit.Text = "直接入力で編集(&E)";
			this.ctmDirectEdit.Click += new System.EventHandler(this.ctmEdit_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(243, 6);
			// 
			// ctmCut
			// 
			this.ctmCut.Image = ((System.Drawing.Image)(resources.GetObject("ctmCut.Image")));
			this.ctmCut.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ctmCut.Name = "ctmCut";
			this.ctmCut.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
			this.ctmCut.Size = new System.Drawing.Size(246, 22);
			this.ctmCut.Text = "切り取り(&T)";
			this.ctmCut.Click += new System.EventHandler(this.ctmCut_Click);
			// 
			// ctmCopy
			// 
			this.ctmCopy.Image = ((System.Drawing.Image)(resources.GetObject("ctmCopy.Image")));
			this.ctmCopy.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ctmCopy.Name = "ctmCopy";
			this.ctmCopy.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
			this.ctmCopy.Size = new System.Drawing.Size(246, 22);
			this.ctmCopy.Text = "コピー(&C)";
			this.ctmCopy.Click += new System.EventHandler(this.ctmCopy_Click);
			// 
			// ctmPaste
			// 
			this.ctmPaste.Image = ((System.Drawing.Image)(resources.GetObject("ctmPaste.Image")));
			this.ctmPaste.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ctmPaste.Name = "ctmPaste";
			this.ctmPaste.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
			this.ctmPaste.Size = new System.Drawing.Size(246, 22);
			this.ctmPaste.Text = "貼り付け(&P)";
			this.ctmPaste.Click += new System.EventHandler(this.ctmPaste_Click);
			// 
			// ctmDelete
			// 
			this.ctmDelete.Image = ((System.Drawing.Image)(resources.GetObject("ctmDelete.Image")));
			this.ctmDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ctmDelete.Name = "ctmDelete";
			this.ctmDelete.ShortcutKeys = System.Windows.Forms.Keys.Delete;
			this.ctmDelete.Size = new System.Drawing.Size(246, 22);
			this.ctmDelete.Text = "削除(&D)";
			this.ctmDelete.Click += new System.EventHandler(this.ctmDelete_Click);
			// 
			// ToolStripSeparator7
			// 
			this.ToolStripSeparator7.Name = "ToolStripSeparator7";
			this.ToolStripSeparator7.Size = new System.Drawing.Size(243, 6);
			// 
			// ctmSelctAll
			// 
			this.ctmSelctAll.Image = ((System.Drawing.Image)(resources.GetObject("ctmSelctAll.Image")));
			this.ctmSelctAll.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ctmSelctAll.Name = "ctmSelctAll";
			this.ctmSelctAll.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
			this.ctmSelctAll.Size = new System.Drawing.Size(246, 22);
			this.ctmSelctAll.Text = "すべて選択(&A)";
			this.ctmSelctAll.Click += new System.EventHandler(this.ctmSelctAll_Click);
			// 
			// lstEditor
			// 
			this.lstEditor.ContextMenuStrip = this.cmnTextMenu;
			this.lstEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lstEditor.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
			this.lstEditor.Font = new System.Drawing.Font("メイリオ", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.lstEditor.FormattingEnabled = true;
			this.lstEditor.HorizontalScrollbar = true;
			this.lstEditor.ItemHeight = 16;
			this.lstEditor.Location = new System.Drawing.Point(0, 25);
			this.lstEditor.Name = "lstEditor";
			this.lstEditor.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.lstEditor.Size = new System.Drawing.Size(235, 300);
			this.lstEditor.TabIndex = 1;
			this.lstEditor.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.lstEditor_DrawItem);
			this.lstEditor.DoubleClick += new System.EventHandler(this.lstEditor_DoubleClick);
			// 
			// ctlEVEasyEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.lstEditor);
			this.Controls.Add(this.ToolStrip1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlEVEasyEditor";
			this.Size = new System.Drawing.Size(235, 325);
			this.ToolStrip1.ResumeLayout(false);
			this.ToolStrip1.PerformLayout();
			this.cmnTextMenu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip ToolStrip1;
		private System.Windows.Forms.ToolStripButton tolCopy;
		private System.Windows.Forms.ToolStripButton tolPaste;
		private System.Windows.Forms.ToolStripButton tolCut;
		private System.Windows.Forms.ContextMenuStrip cmnTextMenu;
		private System.Windows.Forms.ToolStripMenuItem ctmCut;
		private System.Windows.Forms.ToolStripMenuItem ctmCopy;
		private System.Windows.Forms.ToolStripMenuItem ctmPaste;
		private System.Windows.Forms.ToolStripSeparator ToolStripSeparator7;
		private System.Windows.Forms.ToolStripMenuItem ctmSelctAll;
		private System.Windows.Forms.ToolStripButton tolDelete;
		private System.Windows.Forms.ToolStripMenuItem ctmDelete;
		private System.Windows.Forms.ToolStripMenuItem ctmDirectEdit;
		private System.Windows.Forms.ListBox lstEditor;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
	}
}
