namespace Editor.Dialog.Common {
	partial class dlgEditSettings {
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
			this.SettingsCtrl = new System.Windows.Forms.PropertyGrid();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnExport = new System.Windows.Forms.Button();
			this.btnImport = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// SettingsCtrl
			// 
			this.SettingsCtrl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.SettingsCtrl.CategoryForeColor = System.Drawing.SystemColors.InactiveCaptionText;
			this.SettingsCtrl.Location = new System.Drawing.Point(0, 0);
			this.SettingsCtrl.Name = "SettingsCtrl";
			this.SettingsCtrl.Size = new System.Drawing.Size(485, 383);
			this.SettingsCtrl.TabIndex = 0;
			this.SettingsCtrl.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.settingsCtrl_PropertyValueChanged);
			// 
			// btnOK
			// 
			this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.ForeColor = System.Drawing.Color.Black;
			this.btnOK.Location = new System.Drawing.Point(374, 394);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(100, 28);
			this.btnOK.TabIndex = 3;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnExport
			// 
			this.btnExport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnExport.ForeColor = System.Drawing.Color.Black;
			this.btnExport.Location = new System.Drawing.Point(268, 394);
			this.btnExport.Name = "btnExport";
			this.btnExport.Size = new System.Drawing.Size(100, 28);
			this.btnExport.TabIndex = 2;
			this.btnExport.Text = "エクスポート";
			this.btnExport.UseVisualStyleBackColor = true;
			this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
			// 
			// btnImport
			// 
			this.btnImport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnImport.ForeColor = System.Drawing.Color.Black;
			this.btnImport.Location = new System.Drawing.Point(162, 394);
			this.btnImport.Name = "btnImport";
			this.btnImport.Size = new System.Drawing.Size(100, 28);
			this.btnImport.TabIndex = 1;
			this.btnImport.Text = "インポート";
			this.btnImport.UseVisualStyleBackColor = true;
			this.btnImport.Click += new System.EventHandler(this.btnImport_Click);
			// 
			// dlgEditSettings
			// 
			this.AcceptButton = this.btnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(486, 434);
			this.Controls.Add(this.btnImport);
			this.Controls.Add(this.btnExport);
			this.Controls.Add(this.SettingsCtrl);
			this.Controls.Add(this.btnOK);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgEditSettings";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "設定";
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.PropertyGrid SettingsCtrl;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnExport;
		private System.Windows.Forms.Button btnImport;
	}
}