namespace Editor.Dialog.Common {
	partial class dlgNewProject {
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
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.txtProjectName = new System.Windows.Forms.TextBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.ctlCloseButton = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.ctlPathSet = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.chkLoadPresetData = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(75, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "プロジェクト名:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 45);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(60, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "作成場所:";
            // 
            // txtProjectName
            // 
            this.txtProjectName.Location = new System.Drawing.Point(99, 12);
            this.txtProjectName.Name = "txtProjectName";
            this.txtProjectName.Size = new System.Drawing.Size(203, 23);
            this.txtProjectName.TabIndex = 1;
            this.txtProjectName.Enter += new System.EventHandler(this.TxtProjectName_Enter);
            // 
            // ctlCloseButton
            // 
            this.ctlCloseButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.ctlCloseButton.DisenabledAcceptButton = false;
            this.ctlCloseButton.DisenabledCancelButton = false;
            this.ctlCloseButton.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlCloseButton.Location = new System.Drawing.Point(52, 108);
            this.ctlCloseButton.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlCloseButton.Name = "ctlCloseButton";
            this.ctlCloseButton.OKTitle = "OK";
            this.ctlCloseButton.Size = new System.Drawing.Size(212, 34);
            this.ctlCloseButton.TabIndex = 14;
            this.ctlCloseButton.ClickOK += new System.EventHandler(this.ctlCloseButton_ClickOK);
            // 
            // ctlPathSet
            // 
            this.ctlPathSet.DialogTitle = "プロジェクトフォルダーを作成する場所を指定して下さい。プロジェクト名のフォルダーは自動で生成されます。";
            this.ctlPathSet.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.Folder;
            this.ctlPathSet.EnabledFiles = true;
            this.ctlPathSet.FileFilter = null;
            this.ctlPathSet.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlPathSet.Location = new System.Drawing.Point(99, 41);
            this.ctlPathSet.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlPathSet.MaximumSize = new System.Drawing.Size(1000, 25);
            this.ctlPathSet.MinimumSize = new System.Drawing.Size(80, 25);
            this.ctlPathSet.Name = "ctlPathSet";
            this.ctlPathSet.Result = "";
            this.ctlPathSet.RootPath = null;
            this.ctlPathSet.Size = new System.Drawing.Size(203, 25);
            this.ctlPathSet.TabIndex = 3;
            // 
            // chkLoadPresetData
            // 
            this.chkLoadPresetData.AutoSize = true;
            this.chkLoadPresetData.Checked = true;
            this.chkLoadPresetData.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkLoadPresetData.Location = new System.Drawing.Point(14, 77);
            this.chkLoadPresetData.Name = "chkLoadPresetData";
            this.chkLoadPresetData.Size = new System.Drawing.Size(146, 19);
            this.chkLoadPresetData.TabIndex = 12;
            this.chkLoadPresetData.Text = "プリセットデータを取り込む";
            this.chkLoadPresetData.UseVisualStyleBackColor = true;
            // 
            // dlgNewProject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(316, 147);
            this.Controls.Add(this.chkLoadPresetData);
            this.Controls.Add(this.ctlCloseButton);
            this.Controls.Add(this.ctlPathSet);
            this.Controls.Add(this.txtProjectName);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgNewProject";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "新規プロジェクトを作成";
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ToolTip toolTip1;
		private CtrlComponent.Common.ctlOKCancel ctlCloseButton;
		private System.Windows.Forms.TextBox txtProjectName;
		private CtrlComponent.Common.ctlPathSetter ctlPathSet;
		private System.Windows.Forms.CheckBox chkLoadPresetData;
    }
}