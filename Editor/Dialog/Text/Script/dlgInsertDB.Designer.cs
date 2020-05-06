namespace Editor.Dialog.Text.Script {
	partial class dlgInsertDB {
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
			this.label2 = new System.Windows.Forms.Label();
			this.lstDB = new System.Windows.Forms.ListBox();
			this.ltvDBItem = new System.Windows.Forms.ListView();
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.chkColumnNameMode = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(8, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(69, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "データベース:";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(193, 9);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(60, 15);
			this.label2.TabIndex = 2;
			this.label2.Text = "項目リスト:";
			// 
			// lstDB
			// 
			this.lstDB.FormattingEnabled = true;
			this.lstDB.ItemHeight = 15;
			this.lstDB.Location = new System.Drawing.Point(15, 29);
			this.lstDB.Name = "lstDB";
			this.lstDB.Size = new System.Drawing.Size(179, 244);
			this.lstDB.TabIndex = 1;
			this.lstDB.SelectedIndexChanged += new System.EventHandler(this.lstDB_SelectedIndexChanged);
			// 
			// ltvDBItem
			// 
			this.ltvDBItem.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader1,
            this.columnHeader2});
			this.ltvDBItem.FullRowSelect = true;
			this.ltvDBItem.GridLines = true;
			this.ltvDBItem.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.ltvDBItem.HideSelection = false;
			this.ltvDBItem.Location = new System.Drawing.Point(200, 29);
			this.ltvDBItem.MultiSelect = false;
			this.ltvDBItem.Name = "ltvDBItem";
			this.ltvDBItem.Size = new System.Drawing.Size(245, 244);
			this.ltvDBItem.TabIndex = 3;
			this.ltvDBItem.UseCompatibleStateImageBehavior = false;
			this.ltvDBItem.View = System.Windows.Forms.View.Details;
			this.ltvDBItem.DoubleClick += new System.EventHandler(this.ltvDBItem_DoubleClick);
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "FixedID";
			this.columnHeader3.Width = 0;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "ID";
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "名前";
			this.columnHeader2.Width = 155;
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(242, 283);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 5;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// chkColumNameMode
			// 
			this.chkColumnNameMode.AutoSize = true;
			this.chkColumnNameMode.Location = new System.Drawing.Point(24, 291);
			this.chkColumnNameMode.Name = "chkColumNameMode";
			this.chkColumnNameMode.Size = new System.Drawing.Size(102, 19);
			this.chkColumnNameMode.TabIndex = 4;
			this.chkColumnNameMode.Text = "列名を挿入する";
			this.chkColumnNameMode.UseVisualStyleBackColor = true;
			this.chkColumnNameMode.Visible = false;
			// 
			// dlgInsertDB
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(461, 326);
			this.Controls.Add(this.chkColumnNameMode);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.ltvDBItem);
			this.Controls.Add(this.lstDB);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgInsertDB";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "データベース項目を挿入";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ListBox lstDB;
		private System.Windows.Forms.ListView ltvDBItem;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.CheckBox chkColumnNameMode;
	}
}