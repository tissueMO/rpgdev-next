namespace Editor.CtrlComponent.Database {
	partial class ctlDBMaterial {
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
            this.TableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabDBMaterialFace = new System.Windows.Forms.TabPage();
            this.tabDBMaterialGraphic = new System.Windows.Forms.TabPage();
            this.tabDBMaterialSound = new System.Windows.Forms.TabPage();
            this.DBFaces = new System.Windows.Forms.DataGridView();
            this.tabDBMaterialFont = new System.Windows.Forms.TabPage();
            this.DBGraphics = new System.Windows.Forms.DataGridView();
            this.DBSounds = new System.Windows.Forms.DataGridView();
            this.DBFonts = new System.Windows.Forms.DataGridView();
            this.TableLayoutPanel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabDBMaterialFace.SuspendLayout();
            this.tabDBMaterialGraphic.SuspendLayout();
            this.tabDBMaterialSound.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBFaces)).BeginInit();
            this.tabDBMaterialFont.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBGraphics)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.DBSounds)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.DBFonts)).BeginInit();
            this.SuspendLayout();
            // 
            // TableLayoutPanel1
            // 
            this.TableLayoutPanel1.AutoScroll = true;
            this.TableLayoutPanel1.AutoSize = true;
            this.TableLayoutPanel1.BackColor = System.Drawing.SystemColors.Control;
            this.TableLayoutPanel1.ColumnCount = 1;
            this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.Controls.Add(this.tabControl1, 0, 0);
            this.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TableLayoutPanel1.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.TableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.TableLayoutPanel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.TableLayoutPanel1.Name = "TableLayoutPanel1";
            this.TableLayoutPanel1.RowCount = 1;
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.TableLayoutPanel1.Size = new System.Drawing.Size(792, 452);
            this.TableLayoutPanel1.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabDBMaterialFace);
            this.tabControl1.Controls.Add(this.tabDBMaterialGraphic);
            this.tabControl1.Controls.Add(this.tabDBMaterialSound);
            this.tabControl1.Controls.Add(this.tabDBMaterialFont);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.ItemSize = new System.Drawing.Size(120, 23);
            this.tabControl1.Location = new System.Drawing.Point(3, 3);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(786, 446);
            this.tabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tabControl1.TabIndex = 21;
            // 
            // tabDBMaterialFace
            // 
            this.tabDBMaterialFace.Controls.Add(this.DBFaces);
            this.tabDBMaterialFace.Location = new System.Drawing.Point(4, 27);
            this.tabDBMaterialFace.Name = "tabDBMaterialFace";
            this.tabDBMaterialFace.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBMaterialFace.Size = new System.Drawing.Size(778, 415);
            this.tabDBMaterialFace.TabIndex = 0;
            this.tabDBMaterialFace.Text = "フェイスグラフィック";
            this.tabDBMaterialFace.UseVisualStyleBackColor = true;
            // 
            // tabDBMaterialGraphic
            // 
            this.tabDBMaterialGraphic.Controls.Add(this.DBGraphics);
            this.tabDBMaterialGraphic.Location = new System.Drawing.Point(4, 27);
            this.tabDBMaterialGraphic.Name = "tabDBMaterialGraphic";
            this.tabDBMaterialGraphic.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBMaterialGraphic.Size = new System.Drawing.Size(778, 415);
            this.tabDBMaterialGraphic.TabIndex = 1;
            this.tabDBMaterialGraphic.Text = "汎用グラフィック";
            this.tabDBMaterialGraphic.UseVisualStyleBackColor = true;
            // 
            // tabDBMaterialSound
            // 
            this.tabDBMaterialSound.Controls.Add(this.DBSounds);
            this.tabDBMaterialSound.Location = new System.Drawing.Point(4, 27);
            this.tabDBMaterialSound.Name = "tabDBMaterialSound";
            this.tabDBMaterialSound.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBMaterialSound.Size = new System.Drawing.Size(778, 415);
            this.tabDBMaterialSound.TabIndex = 2;
            this.tabDBMaterialSound.Text = "汎用サウンド";
            this.tabDBMaterialSound.UseVisualStyleBackColor = true;
            // 
            // DBFaces
            // 
            this.DBFaces.AllowUserToAddRows = false;
            this.DBFaces.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBFaces.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBFaces.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBFaces.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBFaces.Location = new System.Drawing.Point(3, 3);
            this.DBFaces.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBFaces.Name = "DBFaces";
            this.DBFaces.RowHeadersWidth = 40;
            this.DBFaces.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBFaces.Size = new System.Drawing.Size(772, 409);
            this.DBFaces.TabIndex = 2;
            // 
            // tabDBMaterialFont
            // 
            this.tabDBMaterialFont.Controls.Add(this.DBFonts);
            this.tabDBMaterialFont.Location = new System.Drawing.Point(4, 27);
            this.tabDBMaterialFont.Name = "tabDBMaterialFont";
            this.tabDBMaterialFont.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBMaterialFont.Size = new System.Drawing.Size(778, 415);
            this.tabDBMaterialFont.TabIndex = 3;
            this.tabDBMaterialFont.Text = "汎用フォント";
            this.tabDBMaterialFont.UseVisualStyleBackColor = true;
            // 
            // DBGraphics
            // 
            this.DBGraphics.AllowUserToAddRows = false;
            this.DBGraphics.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBGraphics.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBGraphics.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBGraphics.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBGraphics.Location = new System.Drawing.Point(3, 3);
            this.DBGraphics.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBGraphics.Name = "DBGraphics";
            this.DBGraphics.RowHeadersWidth = 40;
            this.DBGraphics.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBGraphics.Size = new System.Drawing.Size(772, 409);
            this.DBGraphics.TabIndex = 4;
            // 
            // DBSounds
            // 
            this.DBSounds.AllowUserToAddRows = false;
            this.DBSounds.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBSounds.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBSounds.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBSounds.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBSounds.Location = new System.Drawing.Point(3, 3);
            this.DBSounds.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBSounds.Name = "DBSounds";
            this.DBSounds.RowHeadersWidth = 40;
            this.DBSounds.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBSounds.Size = new System.Drawing.Size(772, 409);
            this.DBSounds.TabIndex = 6;
            // 
            // DBFonts
            // 
            this.DBFonts.AllowUserToAddRows = false;
            this.DBFonts.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBFonts.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBFonts.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBFonts.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBFonts.Location = new System.Drawing.Point(3, 3);
            this.DBFonts.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBFonts.Name = "DBFonts";
            this.DBFonts.RowHeadersWidth = 40;
            this.DBFonts.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBFonts.Size = new System.Drawing.Size(772, 409);
            this.DBFonts.TabIndex = 8;
            // 
            // ctlDBMaterial
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.TableLayoutPanel1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ctlDBMaterial";
            this.Size = new System.Drawing.Size(792, 452);
            this.TableLayoutPanel1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabDBMaterialFace.ResumeLayout(false);
            this.tabDBMaterialGraphic.ResumeLayout(false);
            this.tabDBMaterialSound.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBFaces)).EndInit();
            this.tabDBMaterialFont.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBGraphics)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.DBSounds)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.DBFonts)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		internal System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabDBMaterialFace;
        internal System.Windows.Forms.DataGridView DBFaces;
        private System.Windows.Forms.TabPage tabDBMaterialGraphic;
        internal System.Windows.Forms.DataGridView DBGraphics;
        private System.Windows.Forms.TabPage tabDBMaterialSound;
        internal System.Windows.Forms.DataGridView DBSounds;
        private System.Windows.Forms.TabPage tabDBMaterialFont;
        internal System.Windows.Forms.DataGridView DBFonts;
    }
}
