namespace Editor.Dialog.Database.Effect {
	partial class dlgUserCommands {
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
            this.uctlListEditor = new Editor.CtrlComponent.Common.ctlListEditor();
            this.label1 = new System.Windows.Forms.Label();
            this.uctlPathWithSetButton = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.SuspendLayout();
            // 
            // uctlListEditor
            // 
            this.uctlListEditor.AllowOverlap = true;
            this.uctlListEditor.EnabledAddAll = false;
            this.uctlListEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlListEditor.Location = new System.Drawing.Point(11, 37);
            this.uctlListEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlListEditor.Name = "uctlListEditor";
            this.uctlListEditor.Size = new System.Drawing.Size(351, 163);
            this.uctlListEditor.SortColumnIndex = 0;
            this.uctlListEditor.TabIndex = 2;
            this.uctlListEditor.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlListEditor_ListItemSelected);
            this.uctlListEditor.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlListEditor_RequestAddItem);
            this.uctlListEditor.RequestClearListEditorSource += new System.EventHandler(this.uctlListEditor_RequestClearListEditorSource);
            this.uctlListEditor.RequestWatchControls += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(373, 36);
            this.label1.TabIndex = 0;
            this.label1.Text = "独自コマンドを記述したソースファイルのリストを編集します。";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // uctlPathWithSetButton
            // 
            this.uctlPathWithSetButton.DialogTitle = null;
            this.uctlPathWithSetButton.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathWithSetButton.EnabledFiles = true;
            this.uctlPathWithSetButton.FileFilter = null;
            this.uctlPathWithSetButton.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathWithSetButton.Location = new System.Drawing.Point(11, 40);
            this.uctlPathWithSetButton.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathWithSetButton.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathWithSetButton.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathWithSetButton.Name = "uctlPathWithSetButton";
            this.uctlPathWithSetButton.Result = "";
            this.uctlPathWithSetButton.RootPath = null;
            this.uctlPathWithSetButton.Size = new System.Drawing.Size(184, 25);
            this.uctlPathWithSetButton.TabIndex = 1;
            this.uctlPathWithSetButton.PathChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(153, 208);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 3;
            // 
            // dlgUserCommands
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(373, 248);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.uctlPathWithSetButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.uctlListEditor);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgUserCommands";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "独自コマンドの編集";
            this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Common.ctlListEditor uctlListEditor;
		private System.Windows.Forms.Label label1;
		private CtrlComponent.Common.ctlPathSetter uctlPathWithSetButton;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
	}
}