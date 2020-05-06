namespace Editor.CtrlComponent.Map {
	partial class ctlMapViewer {
		/// <summary> 
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region コンポーネント デザイナーで生成されたコード

		/// <summary> 
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
			this.ToolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
			this.tolScale = new System.Windows.Forms.ToolStripComboBox();
			this.pnlPreviewScroller = new System.Windows.Forms.Panel();
			this.tblMapDesigner = new System.Windows.Forms.TableLayoutPanel();
			this.hscDesignerScroller = new System.Windows.Forms.HScrollBar();
			this.vscDesignerScroller = new System.Windows.Forms.VScrollBar();
			this.pnlPreviewContainer = new System.Windows.Forms.Panel();
			this.pnlPreview = new System.Windows.Forms.Panel();
			this.ToolStrip1.SuspendLayout();
			this.pnlPreviewScroller.SuspendLayout();
			this.tblMapDesigner.SuspendLayout();
			this.pnlPreviewContainer.SuspendLayout();
			this.SuspendLayout();
			// 
			// ToolStrip1
			// 
			this.ToolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel4,
            this.tolScale});
			this.ToolStrip1.Location = new System.Drawing.Point(0, 0);
			this.ToolStrip1.Name = "ToolStrip1";
			this.ToolStrip1.Size = new System.Drawing.Size(571, 25);
			this.ToolStrip1.TabIndex = 0;
			this.ToolStrip1.Text = "ToolStrip1";
			// 
			// toolStripLabel4
			// 
			this.toolStripLabel4.Name = "toolStripLabel4";
			this.toolStripLabel4.Size = new System.Drawing.Size(50, 22);
			this.toolStripLabel4.Text = "スケール:";
			// 
			// tolScale
			// 
			this.tolScale.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.tolScale.FlatStyle = System.Windows.Forms.FlatStyle.Standard;
			this.tolScale.Name = "tolScale";
			this.tolScale.Size = new System.Drawing.Size(80, 25);
			this.tolScale.SelectedIndexChanged += new System.EventHandler(this.tolScale_SelectedIndexChanged);
			// 
			// pnlPreviewScroller
			// 
			this.pnlPreviewScroller.BackColor = System.Drawing.Color.White;
			this.pnlPreviewScroller.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pnlPreviewScroller.Controls.Add(this.tblMapDesigner);
			this.pnlPreviewScroller.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlPreviewScroller.Location = new System.Drawing.Point(0, 25);
			this.pnlPreviewScroller.Name = "pnlPreviewScroller";
			this.pnlPreviewScroller.Size = new System.Drawing.Size(571, 380);
			this.pnlPreviewScroller.TabIndex = 1;
			// 
			// tblMapDesigner
			// 
			this.tblMapDesigner.BackColor = System.Drawing.SystemColors.Control;
			this.tblMapDesigner.ColumnCount = 2;
			this.tblMapDesigner.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tblMapDesigner.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 17F));
			this.tblMapDesigner.Controls.Add(this.hscDesignerScroller, 0, 1);
			this.tblMapDesigner.Controls.Add(this.vscDesignerScroller, 1, 0);
			this.tblMapDesigner.Controls.Add(this.pnlPreviewContainer, 0, 0);
			this.tblMapDesigner.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tblMapDesigner.Location = new System.Drawing.Point(0, 0);
			this.tblMapDesigner.Margin = new System.Windows.Forms.Padding(0);
			this.tblMapDesigner.Name = "tblMapDesigner";
			this.tblMapDesigner.RowCount = 2;
			this.tblMapDesigner.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tblMapDesigner.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 17F));
			this.tblMapDesigner.Size = new System.Drawing.Size(567, 376);
			this.tblMapDesigner.TabIndex = 4;
			// 
			// hscDesignerScroller
			// 
			this.hscDesignerScroller.Dock = System.Windows.Forms.DockStyle.Fill;
			this.hscDesignerScroller.LargeChange = 1;
			this.hscDesignerScroller.Location = new System.Drawing.Point(0, 359);
			this.hscDesignerScroller.Maximum = 0;
			this.hscDesignerScroller.Name = "hscDesignerScroller";
			this.hscDesignerScroller.Size = new System.Drawing.Size(550, 17);
			this.hscDesignerScroller.TabIndex = 1;
			this.hscDesignerScroller.Scroll += new System.Windows.Forms.ScrollEventHandler(this.designerScroller_Scroll);
			// 
			// vscDesignerScroller
			// 
			this.vscDesignerScroller.Dock = System.Windows.Forms.DockStyle.Fill;
			this.vscDesignerScroller.LargeChange = 1;
			this.vscDesignerScroller.Location = new System.Drawing.Point(550, 0);
			this.vscDesignerScroller.Maximum = 0;
			this.vscDesignerScroller.Name = "vscDesignerScroller";
			this.vscDesignerScroller.Padding = new System.Windows.Forms.Padding(0, 0, 0, 17);
			this.vscDesignerScroller.Size = new System.Drawing.Size(17, 359);
			this.vscDesignerScroller.TabIndex = 0;
			this.vscDesignerScroller.Scroll += new System.Windows.Forms.ScrollEventHandler(this.designerScroller_Scroll);
			// 
			// pnlPreviewContainer
			// 
			this.pnlPreviewContainer.Controls.Add(this.pnlPreview);
			this.pnlPreviewContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlPreviewContainer.Location = new System.Drawing.Point(0, 0);
			this.pnlPreviewContainer.Margin = new System.Windows.Forms.Padding(0);
			this.pnlPreviewContainer.Name = "pnlPreviewContainer";
			this.pnlPreviewContainer.Size = new System.Drawing.Size(550, 359);
			this.pnlPreviewContainer.TabIndex = 4;
			// 
			// pnlPreview
			// 
			this.pnlPreview.BackColor = System.Drawing.Color.Black;
			this.pnlPreview.Location = new System.Drawing.Point(0, 0);
			this.pnlPreview.Margin = new System.Windows.Forms.Padding(0);
			this.pnlPreview.Name = "pnlPreview";
			this.pnlPreview.Size = new System.Drawing.Size(3840, 2160);
			this.pnlPreview.TabIndex = 0;
			this.pnlPreview.TabStop = true;
			this.pnlPreview.VisibleChanged += new System.EventHandler(this.pnlPreview_VisibleChanged);
			this.pnlPreview.DoubleClick += new System.EventHandler(this.pnlPreview_DoubleClick);
			this.pnlPreview.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pnlPreview_MouseDown);
			this.pnlPreview.MouseEnter += new System.EventHandler(this.pnlPreview_MouseEnter);
			this.pnlPreview.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.pnlPreview_PreviewKeyDown);
			// 
			// ctlMapViewer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.pnlPreviewScroller);
			this.Controls.Add(this.ToolStrip1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlMapViewer";
			this.Size = new System.Drawing.Size(571, 405);
			this.Load += new System.EventHandler(this.ctlMapViewer_Load);
			this.ToolStrip1.ResumeLayout(false);
			this.ToolStrip1.PerformLayout();
			this.pnlPreviewScroller.ResumeLayout(false);
			this.tblMapDesigner.ResumeLayout(false);
			this.pnlPreviewContainer.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.ToolStrip ToolStrip1;
		private System.Windows.Forms.ToolStripLabel toolStripLabel4;
		private System.Windows.Forms.ToolStripComboBox tolScale;
		private System.Windows.Forms.Panel pnlPreviewScroller;
		private System.Windows.Forms.TableLayoutPanel tblMapDesigner;
		private System.Windows.Forms.HScrollBar hscDesignerScroller;
		private System.Windows.Forms.VScrollBar vscDesignerScroller;
		private System.Windows.Forms.Panel pnlPreviewContainer;
		private System.Windows.Forms.Panel pnlPreview;
	}
}
