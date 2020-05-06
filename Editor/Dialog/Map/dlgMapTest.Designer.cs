namespace Editor.Dialog.Map {
	partial class dlgMapTest {
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
            this.label2 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.cmbValues = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cmbValueType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txtValue = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.cmbEVs = new System.Windows.Forms.ComboBox();
            this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
            this.uctlListEditor = new Editor.CtrlComponent.Common.ctlListEditor();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(559, 38);
            this.label2.TabIndex = 0;
            this.label2.Text = "スクリプト共通変数・イベント個別変数の値を適用した状態でマップテストを開始します。";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.cmbValues, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.cmbValueType, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.txtValue, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.cmbEVs, 1, 1);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 41);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 5;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(376, 115);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // cmbValues
            // 
            this.cmbValues.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbValues.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbValues.FormattingEnabled = true;
            this.cmbValues.Location = new System.Drawing.Point(58, 59);
            this.cmbValues.Name = "cmbValues";
            this.cmbValues.Size = new System.Drawing.Size(315, 23);
            this.cmbValues.TabIndex = 5;
            this.cmbValues.SelectedIndexChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 6);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 15);
            this.label4.TabIndex = 0;
            this.label4.Text = "種別:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmbValueType
            // 
            this.cmbValueType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbValueType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbValueType.FormattingEnabled = true;
            this.cmbValueType.Location = new System.Drawing.Point(58, 3);
            this.cmbValueType.Name = "cmbValueType";
            this.cmbValueType.Size = new System.Drawing.Size(315, 23);
            this.cmbValueType.TabIndex = 1;
            this.cmbValueType.SelectedIndexChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 90);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 15);
            this.label1.TabIndex = 6;
            this.label1.Text = "設定値:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(49, 15);
            this.label3.TabIndex = 4;
            this.label3.Text = "変数:";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // txtValue
            // 
            this.txtValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.txtValue.Location = new System.Drawing.Point(58, 87);
            this.txtValue.Name = "txtValue";
            this.txtValue.Size = new System.Drawing.Size(315, 23);
            this.txtValue.TabIndex = 7;
            this.toolTip1.SetToolTip(this.txtValue, "フラグの値は 0/false, 1/true で指定します。");
            this.txtValue.TextChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 34);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(49, 15);
            this.label5.TabIndex = 2;
            this.label5.Text = "イベント:";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cmbEVs
            // 
            this.cmbEVs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cmbEVs.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbEVs.FormattingEnabled = true;
            this.cmbEVs.Location = new System.Drawing.Point(58, 31);
            this.cmbEVs.Name = "cmbEVs";
            this.cmbEVs.Size = new System.Drawing.Size(315, 23);
            this.cmbEVs.TabIndex = 3;
            this.cmbEVs.SelectedIndexChanged += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // ctlOKCancel1
            // 
            this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ctlOKCancel1.DisenabledAcceptButton = false;
            this.ctlOKCancel1.DisenabledCancelButton = false;
            this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.ctlOKCancel1.Location = new System.Drawing.Point(340, 355);
            this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.ctlOKCancel1.Name = "ctlOKCancel1";
            this.ctlOKCancel1.OKTitle = "OK";
            this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
            this.ctlOKCancel1.TabIndex = 3;
            // 
            // uctlListEditor
            // 
            this.uctlListEditor.AllowOverlap = true;
            this.uctlListEditor.EnabledAddAll = false;
            this.uctlListEditor.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.uctlListEditor.Location = new System.Drawing.Point(12, 125);
            this.uctlListEditor.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.uctlListEditor.Name = "uctlListEditor";
            this.uctlListEditor.Size = new System.Drawing.Size(540, 228);
            this.uctlListEditor.SortColumnIndex = 0;
            this.uctlListEditor.TabIndex = 2;
            this.uctlListEditor.ListItemSelected += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlListEditor_ListItemSelected);
            this.uctlListEditor.RequestAddItem += new System.EventHandler<Editor.CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs>(this.uctlListEditor_RequestAddItem);
            this.uctlListEditor.RequestClearListEditorSource += new System.EventHandler(this.uctlListEditor_RequestClearListEditorSource);
            this.uctlListEditor.RequestWatchControls += new System.EventHandler(this.uctlListEditor_RequestWatchControls);
            // 
            // dlgMapTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(559, 395);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ctlOKCancel1);
            this.Controls.Add(this.uctlListEditor);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "dlgMapTest";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "マップテスト";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label label2;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox cmbValueType;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txtValue;
		private CtrlComponent.Common.ctlListEditor uctlListEditor;
		private System.Windows.Forms.ComboBox cmbValues;
		private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmbEVs;
    }
}