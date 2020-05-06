namespace Editor.CtrlComponent.Common {
	partial class ctlListEditor {
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
            this.tableLayout = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddAll = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.btnEdit = new System.Windows.Forms.Button();
            this.btnDelete = new System.Windows.Forms.Button();
            this.ltvList = new System.Windows.Forms.ListView();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.操作ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuUp = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDown = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.tableLayout.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayout
            // 
            this.tableLayout.ColumnCount = 4;
            this.tableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayout.Controls.Add(this.btnAddAll, 0, 2);
            this.tableLayout.Controls.Add(this.btnAdd, 1, 0);
            this.tableLayout.Controls.Add(this.btnEdit, 2, 0);
            this.tableLayout.Controls.Add(this.btnDelete, 3, 0);
            this.tableLayout.Controls.Add(this.ltvList, 0, 1);
            this.tableLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayout.Location = new System.Drawing.Point(0, 0);
            this.tableLayout.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayout.Name = "tableLayout";
            this.tableLayout.RowCount = 3;
            this.tableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayout.Size = new System.Drawing.Size(269, 322);
            this.tableLayout.TabIndex = 0;
            // 
            // btnAddAll
            // 
            this.btnAddAll.Dock = System.Windows.Forms.DockStyle.Left;
            this.btnAddAll.Location = new System.Drawing.Point(2, 293);
            this.btnAddAll.Margin = new System.Windows.Forms.Padding(2, 2, 0, 1);
            this.btnAddAll.Name = "btnAddAll";
            this.btnAddAll.Size = new System.Drawing.Size(100, 28);
            this.btnAddAll.TabIndex = 5;
            this.btnAddAll.Text = "すべて追加";
            this.btnAddAll.UseVisualStyleBackColor = true;
            this.btnAddAll.Click += new System.EventHandler(this.btnAddAll_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Enabled = false;
            this.btnAdd.Location = new System.Drawing.Point(106, 2);
            this.btnAdd.Margin = new System.Windows.Forms.Padding(2, 2, 0, 1);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(53, 28);
            this.btnAdd.TabIndex = 1;
            this.btnAdd.Text = "追加";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnEdit
            // 
            this.btnEdit.Enabled = false;
            this.btnEdit.Location = new System.Drawing.Point(161, 2);
            this.btnEdit.Margin = new System.Windows.Forms.Padding(2, 2, 0, 1);
            this.btnEdit.Name = "btnEdit";
            this.btnEdit.Size = new System.Drawing.Size(53, 28);
            this.btnEdit.TabIndex = 2;
            this.btnEdit.Text = "変更";
            this.toolTip1.SetToolTip(this.btnEdit, "現在選択されている項目に対して変更を適用します。");
            this.btnEdit.UseVisualStyleBackColor = true;
            this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.Enabled = false;
            this.btnDelete.Location = new System.Drawing.Point(216, 2);
            this.btnDelete.Margin = new System.Windows.Forms.Padding(2, 2, 0, 1);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(53, 28);
            this.btnDelete.TabIndex = 3;
            this.btnDelete.Text = "削除";
            this.toolTip1.SetToolTip(this.btnDelete, "現在入力されている項目を削除します。");
            this.btnDelete.UseVisualStyleBackColor = true;
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // ltvList
            // 
            this.tableLayout.SetColumnSpan(this.ltvList, 4);
            this.ltvList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ltvList.FullRowSelect = true;
            this.ltvList.GridLines = true;
            this.ltvList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.ltvList.HideSelection = false;
            this.ltvList.Location = new System.Drawing.Point(3, 34);
            this.ltvList.MultiSelect = false;
            this.ltvList.Name = "ltvList";
            this.ltvList.Size = new System.Drawing.Size(263, 254);
            this.ltvList.TabIndex = 4;
            this.toolTip1.SetToolTip(this.ltvList, "Ctrlキー＋上下キーで項目を前後移動します。\r\nDeleteキーで項目を削除します。");
            this.ltvList.UseCompatibleStateImageBehavior = false;
            this.ltvList.View = System.Windows.Forms.View.Details;
            this.ltvList.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.ltvList_ColumnClick);
            this.ltvList.SelectedIndexChanged += new System.EventHandler(this.ltvList_SelectedIndexChanged);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.操作ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(269, 24);
            this.menuStrip1.TabIndex = 1;
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
            // ctlListEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tableLayout);
            this.Controls.Add(this.menuStrip1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ctlListEditor";
            this.Size = new System.Drawing.Size(269, 322);
            this.tableLayout.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem 操作ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem mnuUp;
		private System.Windows.Forms.ToolStripMenuItem mnuDown;
		private System.Windows.Forms.ToolStripMenuItem mnuDelete;
		protected System.Windows.Forms.Button btnAdd;
		protected System.Windows.Forms.Button btnEdit;
		protected System.Windows.Forms.Button btnDelete;
		protected System.Windows.Forms.ListView ltvList;
		private System.Windows.Forms.TableLayoutPanel tableLayout;
		protected System.Windows.Forms.Button btnAddAll;
	}
}
