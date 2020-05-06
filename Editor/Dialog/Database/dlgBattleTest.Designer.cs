namespace Editor.Dialog.Database {
	partial class dlgBattleTest {
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
            this.uctlPartyEditor = new Editor.CtrlComponent.Common.ctlListEditor();
            this.numLevel = new System.Windows.Forms.NumericUpDown();
            this.cmbDBChar = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numLevel)).BeginInit();
            this.SuspendLayout();
            // 
            // uctlPartyEditor
            // 
            this.uctlPartyEditor.AllowOverlap = false;
            this.uctlPartyEditor.EnabledAddAll = true;
            this.uctlPartyEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPartyEditor.Location = new System.Drawing.Point(10, 65);
            this.uctlPartyEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPartyEditor.Name = "uctlPartyEditor";
            this.uctlPartyEditor.Size = new System.Drawing.Size(332, 184);
            this.uctlPartyEditor.TabIndex = 5;
            this.uctlPartyEditor.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlPartyEditor_ListItemSelected);
            this.uctlPartyEditor.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlPartyEditor_RequestAddItem);
            this.uctlPartyEditor.RequestAddItemAll += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemAllEventArgs>(this.uctlPartyEditor_RequestAddItemAll);
            this.uctlPartyEditor.RequestClearListEditorSource += new System.EventHandler(this.uctlPartyEditor_RequestClearListEditorSource);
            this.uctlPartyEditor.RequestWatchControls += new System.EventHandler(this.uctlPartyEditor_RequestWatchControls);
            // 
            // numLevel
            // 
            this.numLevel.Location = new System.Drawing.Point(93, 69);
            this.numLevel.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.numLevel.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numLevel.Name = "numLevel";
            this.numLevel.Size = new System.Drawing.Size(78, 23);
            this.numLevel.TabIndex = 4;
            this.numLevel.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numLevel.ValueChanged += new System.EventHandler(this.uctlPartyEditor_RequestWatchControls);
            // 
            // cmbDBChar
            // 
            this.cmbDBChar.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDBChar.FormattingEnabled = true;
            this.cmbDBChar.Location = new System.Drawing.Point(93, 40);
            this.cmbDBChar.Name = "cmbDBChar";
            this.cmbDBChar.Size = new System.Drawing.Size(248, 23);
            this.cmbDBChar.TabIndex = 2;
            this.cmbDBChar.SelectedIndexChanged += new System.EventHandler(this.uctlPartyEditor_RequestWatchControls);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 73);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 15);
            this.label1.TabIndex = 3;
            this.label1.Text = "レベル:";
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(135, 258);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 6;
            this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
            // 
            // label2
            // 
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(353, 36);
            this.label2.TabIndex = 0;
            this.label2.Text = "パーティ編成を行い、バトルのテストを開始します。";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 44);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 15);
            this.label3.TabIndex = 1;
            this.label3.Text = "パーティキャラ:";
            // 
            // dlgBattleTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(353, 297);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbDBChar);
            this.Controls.Add(this.numLevel);
            this.Controls.Add(this.uctlPartyEditor);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgBattleTest";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "バトルテスト";
            ((System.ComponentModel.ISupportInitialize)(this.numLevel)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private CtrlComponent.Common.ctlListEditor uctlPartyEditor;
		private System.Windows.Forms.NumericUpDown numLevel;
		private System.Windows.Forms.ComboBox cmbDBChar;
		private System.Windows.Forms.Label label1;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
	}
}