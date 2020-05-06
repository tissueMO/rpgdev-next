namespace Editor.Dialog.Database.Effect {
	partial class dlgEditLayers {
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
            this.uctlLayerListEditor = new Editor.CtrlComponent.Common.ctlListEditor();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.uctlPathWithSet = new Editor.CtrlComponent.Common.ctlPathSetter();
            this.uctlSectionSize = new Editor.CtrlComponent.Common.ctlPosition();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.txtFixedID = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // uctlLayerListEditor
            // 
            this.uctlLayerListEditor.AllowOverlap = true;
            this.uctlLayerListEditor.EnabledAddAll = false;
            this.uctlLayerListEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlLayerListEditor.Location = new System.Drawing.Point(12, 37);
            this.uctlLayerListEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlLayerListEditor.Name = "uctlLayerListEditor";
            this.uctlLayerListEditor.Size = new System.Drawing.Size(380, 204);
            this.uctlLayerListEditor.TabIndex = 4;
            this.uctlLayerListEditor.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlLayerListEditor_ListItemSelected);
            this.uctlLayerListEditor.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlLayerListEditor_RequestAddItem);
            this.uctlLayerListEditor.RequestClearListEditorSource += new System.EventHandler(this.uctlLayerListEditor_RequestClearListEditorSource);
            this.uctlLayerListEditor.RequestWatchControls += new System.EventHandler(this.uctlLayerListEditor_RequestWatchControls);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "ファイル名:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 45);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "区画サイズ:";
            // 
            // uctlPathWithSet
            // 
            this.uctlPathWithSet.DialogTitle = "レイヤーグラフィックの選択";
            this.uctlPathWithSet.DialogType = Editor.CtrlComponent.Common.ctlPathSetter.SetterDialogType.ProjectRoot;
            this.uctlPathWithSet.EnabledFiles = true;
            this.uctlPathWithSet.FileFilter = null;
            this.uctlPathWithSet.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlPathWithSet.Location = new System.Drawing.Point(83, 11);
            this.uctlPathWithSet.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlPathWithSet.MaximumSize = new System.Drawing.Size(1000, 25);
            this.uctlPathWithSet.MinimumSize = new System.Drawing.Size(80, 25);
            this.uctlPathWithSet.Name = "uctlPathWithSet";
            this.uctlPathWithSet.Result = "";
            this.uctlPathWithSet.RootPath = null;
            this.uctlPathWithSet.Size = new System.Drawing.Size(309, 25);
            this.uctlPathWithSet.TabIndex = 1;
            this.uctlPathWithSet.PathChanged += new System.EventHandler(this.uctlLayerListEditor_RequestWatchControls);
            // 
            // uctlSectionSize
            // 
            this.uctlSectionSize.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlSectionSize.Location = new System.Drawing.Point(83, 41);
            this.uctlSectionSize.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlSectionSize.Maximum = new System.Drawing.Point(99999, 99999);
            this.uctlSectionSize.MaximumSize = new System.Drawing.Size(500, 23);
            this.uctlSectionSize.Minimum = new System.Drawing.Point(10, 10);
            this.uctlSectionSize.MinimumSize = new System.Drawing.Size(100, 23);
            this.uctlSectionSize.Name = "uctlSectionSize";
            this.uctlSectionSize.Result = new System.Drawing.Point(192, 192);
            this.uctlSectionSize.Size = new System.Drawing.Size(137, 23);
            this.uctlSectionSize.TabIndex = 3;
            this.uctlSectionSize.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Size;
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(184, 249);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 5;
            // 
            // txtFixedID
            // 
            this.txtFixedID.Location = new System.Drawing.Point(15, 256);
            this.txtFixedID.Name = "txtFixedID";
            this.txtFixedID.Size = new System.Drawing.Size(100, 23);
            this.txtFixedID.TabIndex = 6;
            this.txtFixedID.Visible = false;
            // 
            // dlgEditLayers
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(404, 289);
            this.Controls.Add(this.txtFixedID);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.uctlSectionSize);
            this.Controls.Add(this.uctlPathWithSet);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.uctlLayerListEditor);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgEditLayers";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "レイヤーの編集";
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private CtrlComponent.Common.ctlListEditor uctlLayerListEditor;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private CtrlComponent.Common.ctlPathSetter uctlPathWithSet;
		private CtrlComponent.Common.ctlPosition uctlSectionSize;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.TextBox txtFixedID;
	}
}