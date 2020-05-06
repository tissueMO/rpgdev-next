namespace Editor.Dialog.Common {
	partial class dlgManual {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(dlgManual));
			this.browser = new System.Windows.Forms.WebBrowser();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.tolPreviousPage = new System.Windows.Forms.ToolStripButton();
			this.tolNextPage = new System.Windows.Forms.ToolStripButton();
			this.tolHome = new System.Windows.Forms.ToolStripButton();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// browser
			// 
			this.browser.AllowWebBrowserDrop = false;
			this.browser.Dock = System.Windows.Forms.DockStyle.Fill;
			this.browser.Location = new System.Drawing.Point(0, 25);
			this.browser.MinimumSize = new System.Drawing.Size(23, 25);
			this.browser.Name = "browser";
			this.browser.ScriptErrorsSuppressed = true;
			this.browser.Size = new System.Drawing.Size(784, 436);
			this.browser.TabIndex = 1;
			this.browser.Navigated += new System.Windows.Forms.WebBrowserNavigatedEventHandler(this.browser_Navigated);
			// 
			// toolStrip1
			// 
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tolPreviousPage,
            this.tolNextPage,
            this.tolHome});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(784, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// tolPreviousPage
			// 
			this.tolPreviousPage.Image = ((System.Drawing.Image)(resources.GetObject("tolPreviousPage.Image")));
			this.tolPreviousPage.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolPreviousPage.Name = "tolPreviousPage";
			this.tolPreviousPage.Size = new System.Drawing.Size(48, 22);
			this.tolPreviousPage.Text = "戻る";
			this.tolPreviousPage.Click += new System.EventHandler(this.tolPreviousPage_Click);
			// 
			// tolNextPage
			// 
			this.tolNextPage.Image = ((System.Drawing.Image)(resources.GetObject("tolNextPage.Image")));
			this.tolNextPage.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolNextPage.Name = "tolNextPage";
			this.tolNextPage.Size = new System.Drawing.Size(49, 22);
			this.tolNextPage.Text = "進む";
			this.tolNextPage.Click += new System.EventHandler(this.tolNextPage_Click);
			// 
			// tolHome
			// 
			this.tolHome.Image = ((System.Drawing.Image)(resources.GetObject("tolHome.Image")));
			this.tolHome.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.tolHome.Name = "tolHome";
			this.tolHome.Size = new System.Drawing.Size(57, 22);
			this.tolHome.Text = "ホーム";
			this.tolHome.Click += new System.EventHandler(this.tolHome_Click);
			// 
			// dlgManual
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(784, 461);
			this.Controls.Add(this.browser);
			this.Controls.Add(this.toolStrip1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(480, 280);
			this.Name = "dlgManual";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "取扱説明書";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgManual_FormClosing);
			this.Load += new System.EventHandler(this.dlgManual_Load);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.WebBrowser browser;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton tolPreviousPage;
		private System.Windows.Forms.ToolStripButton tolNextPage;
		private System.Windows.Forms.ToolStripButton tolHome;
	}
}