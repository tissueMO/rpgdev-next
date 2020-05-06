namespace Editor.Dialog.Text {
	partial class dlgLocalSearchReplace {
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
			this.chkCase = new System.Windows.Forms.CheckBox();
			this.btnFindNext = new System.Windows.Forms.Button();
			this.btnReplaceAll = new System.Windows.Forms.Button();
			this.txtReplace = new System.Windows.Forms.TextBox();
			this.txtFind = new System.Windows.Forms.TextBox();
			this.Label2 = new System.Windows.Forms.Label();
			this.Label1 = new System.Windows.Forms.Label();
			this.chkRegux = new System.Windows.Forms.CheckBox();
			this.btnReplaceOne = new System.Windows.Forms.Button();
			this.btnClose = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// chkCase
			// 
			this.chkCase.AutoSize = true;
			this.chkCase.Location = new System.Drawing.Point(33, 109);
			this.chkCase.Name = "chkCase";
			this.chkCase.Size = new System.Drawing.Size(138, 19);
			this.chkCase.TabIndex = 7;
			this.chkCase.Text = "大文字・小文字の区別";
			this.chkCase.UseVisualStyleBackColor = true;
			// 
			// btnFindNext
			// 
			this.btnFindNext.Location = new System.Drawing.Point(202, 72);
			this.btnFindNext.Name = "btnFindNext";
			this.btnFindNext.Size = new System.Drawing.Size(90, 28);
			this.btnFindNext.TabIndex = 6;
			this.btnFindNext.Text = "次を検索";
			this.btnFindNext.UseVisualStyleBackColor = true;
			this.btnFindNext.Click += new System.EventHandler(this.btnFindNext_Click);
			// 
			// btnReplaceAll
			// 
			this.btnReplaceAll.Location = new System.Drawing.Point(10, 72);
			this.btnReplaceAll.Name = "btnReplaceAll";
			this.btnReplaceAll.Size = new System.Drawing.Size(90, 28);
			this.btnReplaceAll.TabIndex = 4;
			this.btnReplaceAll.Text = "すべて置換";
			this.btnReplaceAll.UseVisualStyleBackColor = true;
			this.btnReplaceAll.Click += new System.EventHandler(this.btnReplaceAll_Click);
			// 
			// txtReplace
			// 
			this.txtReplace.Location = new System.Drawing.Point(90, 38);
			this.txtReplace.Name = "txtReplace";
			this.txtReplace.Size = new System.Drawing.Size(202, 23);
			this.txtReplace.TabIndex = 3;
			// 
			// txtFind
			// 
			this.txtFind.Location = new System.Drawing.Point(90, 10);
			this.txtFind.Name = "txtFind";
			this.txtFind.Size = new System.Drawing.Size(202, 23);
			this.txtFind.TabIndex = 1;
			// 
			// Label2
			// 
			this.Label2.AutoSize = true;
			this.Label2.Location = new System.Drawing.Point(12, 42);
			this.Label2.Name = "Label2";
			this.Label2.Size = new System.Drawing.Size(72, 15);
			this.Label2.TabIndex = 2;
			this.Label2.Text = "置換テキスト:";
			// 
			// Label1
			// 
			this.Label1.AutoSize = true;
			this.Label1.Location = new System.Drawing.Point(12, 14);
			this.Label1.Name = "Label1";
			this.Label1.Size = new System.Drawing.Size(72, 15);
			this.Label1.TabIndex = 0;
			this.Label1.Text = "検索テキスト:";
			// 
			// chkRegux
			// 
			this.chkRegux.Checked = true;
			this.chkRegux.CheckState = System.Windows.Forms.CheckState.Checked;
			this.chkRegux.Location = new System.Drawing.Point(198, 108);
			this.chkRegux.Name = "chkRegux";
			this.chkRegux.Size = new System.Drawing.Size(74, 19);
			this.chkRegux.TabIndex = 8;
			this.chkRegux.Text = "正規表現";
			this.chkRegux.UseVisualStyleBackColor = true;
			// 
			// btnReplaceOne
			// 
			this.btnReplaceOne.Location = new System.Drawing.Point(106, 72);
			this.btnReplaceOne.Name = "btnReplaceOne";
			this.btnReplaceOne.Size = new System.Drawing.Size(90, 28);
			this.btnReplaceOne.TabIndex = 5;
			this.btnReplaceOne.Text = "置換して次へ";
			this.btnReplaceOne.UseVisualStyleBackColor = true;
			this.btnReplaceOne.Click += new System.EventHandler(this.btnReplaceOne_Click);
			// 
			// btnClose
			// 
			this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnClose.Location = new System.Drawing.Point(999, 999);
			this.btnClose.Name = "btnClose";
			this.btnClose.Size = new System.Drawing.Size(21, 17);
			this.btnClose.TabIndex = 9;
			this.btnClose.Text = "Close";
			this.btnClose.UseVisualStyleBackColor = true;
			this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
			// 
			// dlgLocalSearchReplace
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.btnClose;
			this.ClientSize = new System.Drawing.Size(303, 136);
			this.Controls.Add(this.btnClose);
			this.Controls.Add(this.btnReplaceOne);
			this.Controls.Add(this.chkRegux);
			this.Controls.Add(this.chkCase);
			this.Controls.Add(this.btnFindNext);
			this.Controls.Add(this.btnReplaceAll);
			this.Controls.Add(this.txtReplace);
			this.Controls.Add(this.txtFind);
			this.Controls.Add(this.Label2);
			this.Controls.Add(this.Label1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgLocalSearchReplace";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "検索と置換";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgLocalSearchReplace_FormClosing);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		internal System.Windows.Forms.CheckBox chkCase;
		internal System.Windows.Forms.Button btnFindNext;
		internal System.Windows.Forms.Button btnReplaceAll;
		internal System.Windows.Forms.TextBox txtReplace;
		internal System.Windows.Forms.TextBox txtFind;
		internal System.Windows.Forms.Label Label2;
		internal System.Windows.Forms.Label Label1;
		internal System.Windows.Forms.CheckBox chkRegux;
		internal System.Windows.Forms.Button btnReplaceOne;
		private System.Windows.Forms.Button btnClose;
	}
}