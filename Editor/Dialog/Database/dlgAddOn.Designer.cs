namespace Editor.Dialog.Database {
	partial class dlgAddOn {
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label4 = new System.Windows.Forms.Label();
            this.cmbType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cmbDBSrc = new System.Windows.Forms.ComboBox();
            this.numOption = new System.Windows.Forms.NumericUpDown();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.uctlListEditor = new Editor.CtrlComponent.Common.ctlListEditor();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numOption)).BeginInit();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.cmbType, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.cmbDBSrc, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.numOption, 1, 2);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 12);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(244, 88);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 6);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(64, 15);
            this.label4.TabIndex = 0;
            this.label4.Text = "効果内容:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmbType
            // 
            this.cmbType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbType.FormattingEnabled = true;
            this.cmbType.Location = new System.Drawing.Point(73, 3);
            this.cmbType.Name = "cmbType";
            this.cmbType.Size = new System.Drawing.Size(168, 23);
            this.cmbType.TabIndex = 1;
            this.cmbType.SelectedIndexChanged += new System.EventHandler(this.cmbType_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 62);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 15);
            this.label1.TabIndex = 4;
            this.label1.Text = "設定値:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 15);
            this.label2.TabIndex = 2;
            this.label2.Text = "関連ID:";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmbDBSrc
            // 
            this.cmbDBSrc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbDBSrc.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDBSrc.FormattingEnabled = true;
            this.cmbDBSrc.Location = new System.Drawing.Point(73, 31);
            this.cmbDBSrc.Name = "cmbDBSrc";
            this.cmbDBSrc.Size = new System.Drawing.Size(168, 23);
            this.cmbDBSrc.TabIndex = 3;
            this.cmbDBSrc.SelectedIndexChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // numOption
            // 
            this.numOption.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.numOption.Location = new System.Drawing.Point(73, 59);
            this.numOption.Maximum = new decimal(new int[] {
            99999999,
            0,
            0,
            0});
            this.numOption.Minimum = new decimal(new int[] {
            99999999,
            0,
            0,
            -2147483648});
            this.numOption.Name = "numOption";
            this.numOption.Size = new System.Drawing.Size(168, 23);
            this.numOption.TabIndex = 5;
            this.toolTip1.SetToolTip(this.numOption, "可否を設定する項目は、\r\nON=1/OFF=0 を示します。");
            this.numOption.ValueChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(212, 312);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 2;
            // 
            // uctlListEditor
            // 
            this.uctlListEditor.AllowOverlap = true;
            this.uctlListEditor.EnabledAddAll = false;
            this.uctlListEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlListEditor.Location = new System.Drawing.Point(12, 68);
            this.uctlListEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlListEditor.Name = "uctlListEditor";
            this.uctlListEditor.Size = new System.Drawing.Size(412, 238);
            this.uctlListEditor.TabIndex = 1;
            this.uctlListEditor.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlListEditor_ListItemSelected);
            this.uctlListEditor.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlListEditor_RequestAddItem);
            this.uctlListEditor.RequestClearListEditorSource += new System.EventHandler(this.uctlListEditor_RequestClearListEditorSource);
            this.uctlListEditor.RequestWatchControls += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // dlgAddOn
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(433, 352);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.uctlListEditor);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgAddOn";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "アドオンの設定";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numOption)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.NumericUpDown numOption;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox cmbType;
		private System.Windows.Forms.ComboBox cmbDBSrc;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private CtrlComponent.Common.ctlListEditor uctlListEditor;
		private System.Windows.Forms.ToolTip toolTip1;
	}
}