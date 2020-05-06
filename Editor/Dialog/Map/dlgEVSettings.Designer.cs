namespace Editor.Dialog.Map {
	partial class dlgEVSettings {
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
			this.txtName = new System.Windows.Forms.TextBox();
			this.numID = new System.Windows.Forms.NumericUpDown();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.tbcEVPages = new System.Windows.Forms.TabControl();
			this.label3 = new System.Windows.Forms.Label();
			this.btnPageDelete = new System.Windows.Forms.Button();
			this.btnPageCopy = new System.Windows.Forms.Button();
			this.btnPageNew = new System.Windows.Forms.Button();
			this.btnAddToTemplate = new System.Windows.Forms.Button();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.uctlPosition = new Editor.CtrlComponent.Common.ctlPosition();
			this.panel1 = new System.Windows.Forms.Panel();
			this.panel2 = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.numID)).BeginInit();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// txtName
			// 
			this.txtName.Location = new System.Drawing.Point(198, 12);
			this.txtName.Name = "txtName";
			this.txtName.Size = new System.Drawing.Size(169, 23);
			this.txtName.TabIndex = 3;
			// 
			// numID
			// 
			this.numID.Location = new System.Drawing.Point(44, 12);
			this.numID.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
			this.numID.Name = "numID";
			this.numID.Size = new System.Drawing.Size(92, 23);
			this.numID.TabIndex = 1;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(156, 16);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(36, 15);
			this.label2.TabIndex = 2;
			this.label2.Text = "名前:";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(26, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "ID:";
			// 
			// tbcEVPages
			// 
			this.tbcEVPages.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tbcEVPages.ItemSize = new System.Drawing.Size(90, 22);
			this.tbcEVPages.Location = new System.Drawing.Point(0, 46);
			this.tbcEVPages.Multiline = true;
			this.tbcEVPages.Name = "tbcEVPages";
			this.tbcEVPages.SelectedIndex = 0;
			this.tbcEVPages.Size = new System.Drawing.Size(1072, 547);
			this.tbcEVPages.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
			this.tbcEVPages.TabIndex = 1;
			this.tbcEVPages.Selecting += new System.Windows.Forms.TabControlCancelEventHandler(this.tbcEVPages_Selecting);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(392, 16);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(36, 15);
			this.label3.TabIndex = 4;
			this.label3.Text = "座標:";
			// 
			// btnPageDelete
			// 
			this.btnPageDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnPageDelete.Location = new System.Drawing.Point(962, 10);
			this.btnPageDelete.Name = "btnPageDelete";
			this.btnPageDelete.Size = new System.Drawing.Size(100, 28);
			this.btnPageDelete.TabIndex = 8;
			this.btnPageDelete.Text = "ページを削除";
			this.btnPageDelete.UseVisualStyleBackColor = true;
			this.btnPageDelete.Click += new System.EventHandler(this.btnPageDelete_Click);
			// 
			// btnPageCopy
			// 
			this.btnPageCopy.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnPageCopy.Location = new System.Drawing.Point(862, 10);
			this.btnPageCopy.Name = "btnPageCopy";
			this.btnPageCopy.Size = new System.Drawing.Size(100, 28);
			this.btnPageCopy.TabIndex = 7;
			this.btnPageCopy.Text = "ページを複製";
			this.btnPageCopy.UseVisualStyleBackColor = true;
			this.btnPageCopy.Click += new System.EventHandler(this.btnPageCopy_Click);
			// 
			// btnPageNew
			// 
			this.btnPageNew.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnPageNew.Location = new System.Drawing.Point(762, 10);
			this.btnPageNew.Name = "btnPageNew";
			this.btnPageNew.Size = new System.Drawing.Size(100, 28);
			this.btnPageNew.TabIndex = 6;
			this.btnPageNew.Text = "ページを追加";
			this.btnPageNew.UseVisualStyleBackColor = true;
			this.btnPageNew.Click += new System.EventHandler(this.btnPageNew_Click);
			// 
			// btnAddToTemplate
			// 
			this.btnAddToTemplate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnAddToTemplate.Location = new System.Drawing.Point(10, 10);
			this.btnAddToTemplate.Name = "btnAddToTemplate";
			this.btnAddToTemplate.Size = new System.Drawing.Size(124, 28);
			this.btnAddToTemplate.TabIndex = 0;
			this.btnAddToTemplate.Text = "テンプレートへ登録";
			this.btnAddToTemplate.UseVisualStyleBackColor = true;
			this.btnAddToTemplate.Click += new System.EventHandler(this.btnAddToTemplate_Click);
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.DisenabledAcceptButton = false;
			this.ctlOKCancel1.DisenabledCancelButton = false;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(853, 7);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.OKTitle = "OK";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 1;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.uctlOKCancelApply_ClickOK);
			this.ctlOKCancel1.ClickCancel += new System.EventHandler(this.ctlOKCancel1_ClickCancel);
			// 
			// uctlPosition
			// 
			this.uctlPosition.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlPosition.Location = new System.Drawing.Point(434, 12);
			this.uctlPosition.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlPosition.Maximum = new System.Drawing.Point(100, 100);
			this.uctlPosition.MaximumSize = new System.Drawing.Size(500, 23);
			this.uctlPosition.Minimum = new System.Drawing.Point(0, 0);
			this.uctlPosition.MinimumSize = new System.Drawing.Size(100, 23);
			this.uctlPosition.Name = "uctlPosition";
			this.uctlPosition.Result = new System.Drawing.Point(0, 0);
			this.uctlPosition.Size = new System.Drawing.Size(137, 23);
			this.uctlPosition.TabIndex = 5;
			this.uctlPosition.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Point;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.label1);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Controls.Add(this.numID);
			this.panel1.Controls.Add(this.txtName);
			this.panel1.Controls.Add(this.btnPageNew);
			this.panel1.Controls.Add(this.label3);
			this.panel1.Controls.Add(this.btnPageCopy);
			this.panel1.Controls.Add(this.uctlPosition);
			this.panel1.Controls.Add(this.btnPageDelete);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(1072, 46);
			this.panel1.TabIndex = 0;
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.btnAddToTemplate);
			this.panel2.Controls.Add(this.ctlOKCancel1);
			this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel2.Location = new System.Drawing.Point(0, 593);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(1072, 46);
			this.panel2.TabIndex = 2;
			// 
			// dlgEVSettings
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1072, 639);
			this.Controls.Add(this.tbcEVPages);
			this.Controls.Add(this.panel2);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.HelpButton = true;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(1000, 678);
			this.Name = "dlgEVSettings";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "イベントの編集";
			this.HelpButtonClicked += new System.ComponentModel.CancelEventHandler(this.dlgEVSettings_HelpButtonClicked);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgEVSettings_FormClosing);
			this.Load += new System.EventHandler(this.dlgEVSettings_Load);
			((System.ComponentModel.ISupportInitialize)(this.numID)).EndInit();
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.panel2.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TextBox txtName;
		private System.Windows.Forms.NumericUpDown numID;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TabControl tbcEVPages;
		private System.Windows.Forms.Label label3;
		private CtrlComponent.Common.ctlPosition uctlPosition;
		private System.Windows.Forms.Button btnPageDelete;
		private System.Windows.Forms.Button btnPageCopy;
		private System.Windows.Forms.Button btnPageNew;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.Button btnAddToTemplate;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
	}
}