namespace Editor.CtrlComponent.Database {
	partial class ctlDBElementDB {
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
            this.tabDBElementParameter = new System.Windows.Forms.TabPage();
            this.DBParameter = new System.Windows.Forms.DataGridView();
            this.tabDBElementAttribute = new System.Windows.Forms.TabPage();
            this.DBAttribute = new System.Windows.Forms.DataGridView();
            this.tabDBElementSkillType = new System.Windows.Forms.TabPage();
            this.DBSkillType = new System.Windows.Forms.DataGridView();
            this.tabDBElementItemMainType = new System.Windows.Forms.TabPage();
            this.DBItemMainType = new System.Windows.Forms.DataGridView();
            this.tabDBElementItemSubType = new System.Windows.Forms.TabPage();
            this.DBItemSubType = new System.Windows.Forms.DataGridView();
            this.TableLayoutPanel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabDBElementParameter.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBParameter)).BeginInit();
            this.tabDBElementAttribute.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBAttribute)).BeginInit();
            this.tabDBElementSkillType.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBSkillType)).BeginInit();
            this.tabDBElementItemMainType.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBItemMainType)).BeginInit();
            this.tabDBElementItemSubType.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DBItemSubType)).BeginInit();
            this.SuspendLayout();
            // 
            // TableLayoutPanel1
            // 
            this.TableLayoutPanel1.AutoSize = true;
            this.TableLayoutPanel1.BackColor = System.Drawing.SystemColors.Control;
            this.TableLayoutPanel1.ColumnCount = 1;
            this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.TableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
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
            this.TableLayoutPanel1.Size = new System.Drawing.Size(800, 638);
            this.TableLayoutPanel1.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabDBElementParameter);
            this.tabControl1.Controls.Add(this.tabDBElementAttribute);
            this.tabControl1.Controls.Add(this.tabDBElementSkillType);
            this.tabControl1.Controls.Add(this.tabDBElementItemMainType);
            this.tabControl1.Controls.Add(this.tabDBElementItemSubType);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.ItemSize = new System.Drawing.Size(120, 23);
            this.tabControl1.Location = new System.Drawing.Point(3, 3);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(794, 632);
            this.tabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.tabControl1.TabIndex = 21;
            // 
            // tabDBElementParameter
            // 
            this.tabDBElementParameter.Controls.Add(this.DBParameter);
            this.tabDBElementParameter.Location = new System.Drawing.Point(4, 27);
            this.tabDBElementParameter.Name = "tabDBElementParameter";
            this.tabDBElementParameter.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBElementParameter.Size = new System.Drawing.Size(786, 601);
            this.tabDBElementParameter.TabIndex = 0;
            this.tabDBElementParameter.Text = "パラメーター";
            this.tabDBElementParameter.UseVisualStyleBackColor = true;
            // 
            // DBParameter
            // 
            this.DBParameter.AllowUserToAddRows = false;
            this.DBParameter.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBParameter.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBParameter.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBParameter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBParameter.Location = new System.Drawing.Point(3, 3);
            this.DBParameter.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBParameter.Name = "DBParameter";
            this.DBParameter.RowHeadersWidth = 30;
            this.DBParameter.RowTemplate.Height = 30;
            this.DBParameter.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBParameter.Size = new System.Drawing.Size(780, 595);
            this.DBParameter.TabIndex = 2;
            // 
            // tabDBElementAttribute
            // 
            this.tabDBElementAttribute.Controls.Add(this.DBAttribute);
            this.tabDBElementAttribute.Location = new System.Drawing.Point(4, 27);
            this.tabDBElementAttribute.Name = "tabDBElementAttribute";
            this.tabDBElementAttribute.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBElementAttribute.Size = new System.Drawing.Size(786, 601);
            this.tabDBElementAttribute.TabIndex = 1;
            this.tabDBElementAttribute.Text = "属性";
            this.tabDBElementAttribute.UseVisualStyleBackColor = true;
            // 
            // DBAttribute
            // 
            this.DBAttribute.AllowUserToAddRows = false;
            this.DBAttribute.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBAttribute.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBAttribute.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBAttribute.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBAttribute.Location = new System.Drawing.Point(3, 3);
            this.DBAttribute.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBAttribute.Name = "DBAttribute";
            this.DBAttribute.RowHeadersWidth = 30;
            this.DBAttribute.RowTemplate.Height = 30;
            this.DBAttribute.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBAttribute.Size = new System.Drawing.Size(780, 595);
            this.DBAttribute.TabIndex = 4;
            // 
            // tabDBElementSkillType
            // 
            this.tabDBElementSkillType.Controls.Add(this.DBSkillType);
            this.tabDBElementSkillType.Location = new System.Drawing.Point(4, 27);
            this.tabDBElementSkillType.Name = "tabDBElementSkillType";
            this.tabDBElementSkillType.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBElementSkillType.Size = new System.Drawing.Size(786, 601);
            this.tabDBElementSkillType.TabIndex = 2;
            this.tabDBElementSkillType.Text = "スキル種別";
            this.tabDBElementSkillType.UseVisualStyleBackColor = true;
            // 
            // DBSkillType
            // 
            this.DBSkillType.AllowUserToAddRows = false;
            this.DBSkillType.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBSkillType.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBSkillType.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBSkillType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBSkillType.Location = new System.Drawing.Point(3, 3);
            this.DBSkillType.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBSkillType.Name = "DBSkillType";
            this.DBSkillType.RowHeadersWidth = 30;
            this.DBSkillType.RowTemplate.Height = 30;
            this.DBSkillType.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBSkillType.Size = new System.Drawing.Size(780, 595);
            this.DBSkillType.TabIndex = 6;
            // 
            // tabDBElementItemMainType
            // 
            this.tabDBElementItemMainType.Controls.Add(this.DBItemMainType);
            this.tabDBElementItemMainType.Location = new System.Drawing.Point(4, 27);
            this.tabDBElementItemMainType.Name = "tabDBElementItemMainType";
            this.tabDBElementItemMainType.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBElementItemMainType.Size = new System.Drawing.Size(786, 601);
            this.tabDBElementItemMainType.TabIndex = 3;
            this.tabDBElementItemMainType.Text = "アイテム大種別";
            this.tabDBElementItemMainType.UseVisualStyleBackColor = true;
            // 
            // DBItemMainType
            // 
            this.DBItemMainType.AllowUserToAddRows = false;
            this.DBItemMainType.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBItemMainType.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBItemMainType.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBItemMainType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBItemMainType.Location = new System.Drawing.Point(3, 3);
            this.DBItemMainType.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBItemMainType.Name = "DBItemMainType";
            this.DBItemMainType.RowHeadersWidth = 30;
            this.DBItemMainType.RowTemplate.Height = 30;
            this.DBItemMainType.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBItemMainType.Size = new System.Drawing.Size(780, 595);
            this.DBItemMainType.TabIndex = 8;
            // 
            // tabDBElementItemSubType
            // 
            this.tabDBElementItemSubType.Controls.Add(this.DBItemSubType);
            this.tabDBElementItemSubType.Location = new System.Drawing.Point(4, 27);
            this.tabDBElementItemSubType.Name = "tabDBElementItemSubType";
            this.tabDBElementItemSubType.Padding = new System.Windows.Forms.Padding(3);
            this.tabDBElementItemSubType.Size = new System.Drawing.Size(786, 601);
            this.tabDBElementItemSubType.TabIndex = 4;
            this.tabDBElementItemSubType.Text = "アイテム小種別";
            this.tabDBElementItemSubType.UseVisualStyleBackColor = true;
            // 
            // DBItemSubType
            // 
            this.DBItemSubType.AllowUserToAddRows = false;
            this.DBItemSubType.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.DBItemSubType.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.DBItemSubType.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DBItemSubType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DBItemSubType.Location = new System.Drawing.Point(3, 3);
            this.DBItemSubType.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.DBItemSubType.Name = "DBItemSubType";
            this.DBItemSubType.RowHeadersWidth = 30;
            this.DBItemSubType.RowTemplate.Height = 30;
            this.DBItemSubType.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.DBItemSubType.Size = new System.Drawing.Size(780, 595);
            this.DBItemSubType.TabIndex = 10;
            // 
            // ctlDBElementDB
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.TableLayoutPanel1);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ctlDBElementDB";
            this.Size = new System.Drawing.Size(800, 638);
            this.TableLayoutPanel1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabDBElementParameter.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBParameter)).EndInit();
            this.tabDBElementAttribute.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBAttribute)).EndInit();
            this.tabDBElementSkillType.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBSkillType)).EndInit();
            this.tabDBElementItemMainType.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBItemMainType)).EndInit();
            this.tabDBElementItemSubType.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DBItemSubType)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		internal System.Windows.Forms.TableLayoutPanel TableLayoutPanel1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabDBElementParameter;
        internal System.Windows.Forms.DataGridView DBParameter;
        private System.Windows.Forms.TabPage tabDBElementAttribute;
        private System.Windows.Forms.DataGridView DBAttribute;
        private System.Windows.Forms.TabPage tabDBElementSkillType;
        private System.Windows.Forms.DataGridView DBSkillType;
        private System.Windows.Forms.TabPage tabDBElementItemMainType;
        private System.Windows.Forms.DataGridView DBItemMainType;
        private System.Windows.Forms.TabPage tabDBElementItemSubType;
        private System.Windows.Forms.DataGridView DBItemSubType;
    }
}
