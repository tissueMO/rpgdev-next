namespace Editor.Dialog.Database {
	partial class dlgIDValues {
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
            this.cmbDBItem = new System.Windows.Forms.ComboBox();
            this.numValue = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.lblValue = new System.Windows.Forms.Label();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.uctlListEditor = new Editor.CtrlComponent.Common.ctlListEditor();
            ((System.ComponentModel.ISupportInitialize)(this.numValue)).BeginInit();
            this.SuspendLayout();
            // 
            // cmbDBItem
            // 
            this.cmbDBItem.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDBItem.FormattingEnabled = true;
            this.cmbDBItem.Location = new System.Drawing.Point(68, 12);
            this.cmbDBItem.Name = "cmbDBItem";
            this.cmbDBItem.Size = new System.Drawing.Size(298, 23);
            this.cmbDBItem.TabIndex = 1;
            this.cmbDBItem.SelectedIndexChanged += new System.EventHandler(this.ctlListEditor1_RequestWatchControls);
            // 
            // numValue
            // 
            this.numValue.Location = new System.Drawing.Point(68, 41);
            this.numValue.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numValue.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numValue.Name = "numValue";
            this.numValue.Size = new System.Drawing.Size(131, 23);
            this.numValue.TabIndex = 3;
            this.numValue.ValueChanged += new System.EventHandler(this.ctlListEditor1_RequestWatchControls);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "項目:";
            // 
            // lblValue
            // 
            this.lblValue.AutoSize = true;
            this.lblValue.Location = new System.Drawing.Point(14, 45);
            this.lblValue.Name = "lblValue";
            this.lblValue.Size = new System.Drawing.Size(48, 15);
            this.lblValue.TabIndex = 2;
            this.lblValue.Text = "設定値:";
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(164, 290);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 5;
            this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
            // 
            // uctlListEditor
            // 
            this.uctlListEditor.AllowOverlap = false;
            this.uctlListEditor.EnabledAddAll = true;
            this.uctlListEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlListEditor.Location = new System.Drawing.Point(14, 37);
            this.uctlListEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlListEditor.Name = "uctlListEditor";
            this.uctlListEditor.Size = new System.Drawing.Size(355, 248);
            this.uctlListEditor.TabIndex = 4;
            this.uctlListEditor.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.ctlListEditor1_ListItemSelected);
            this.uctlListEditor.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.ctlListEditor1_RequestAddItem);
            this.uctlListEditor.RequestAddItemAll += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemAllEventArgs>(this.uctlListEditor_RequestAddItemAll);
            this.uctlListEditor.RequestClearListEditorSource += new System.EventHandler(this.ctlListEditor1_RequestClearListEditorSource);
            this.uctlListEditor.RequestWatchControls += new System.EventHandler(this.ctlListEditor1_RequestWatchControls);
            // 
            // dlgIDValues
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 329);
            this.Controls.Add(this.lblValue);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbDBItem);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.numValue);
            this.Controls.Add(this.uctlListEditor);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgIDValues";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "データベース項目の入力補助";
            ((System.ComponentModel.ISupportInitialize)(this.numValue)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.NumericUpDown numValue;
		private System.Windows.Forms.ComboBox cmbDBItem;
		private CtrlComponent.Common.ctlListEditor uctlListEditor;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label lblValue;
	}
}