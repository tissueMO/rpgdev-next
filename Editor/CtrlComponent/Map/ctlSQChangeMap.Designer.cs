namespace Editor.CtrlComponent.Map {
	partial class ctlSQChangeMap {
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
			this.components = new System.ComponentModel.Container();
			this.btnMapPosSetter = new System.Windows.Forms.Button();
			this.cmbDirection = new System.Windows.Forms.ComboBox();
			this.lblDirection = new System.Windows.Forms.Label();
			this.label10 = new System.Windows.Forms.Label();
			this.label11 = new System.Windows.Forms.Label();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.uctlPosition = new Editor.CtrlComponent.Common.ctlPosition();
			this.uctlFileTree = new Editor.CtrlComponent.Common.ctlFileTree();
			this.SuspendLayout();
			// 
			// btnMapPosSetter
			// 
			this.btnMapPosSetter.Location = new System.Drawing.Point(366, 19);
			this.btnMapPosSetter.Name = "btnMapPosSetter";
			this.btnMapPosSetter.Size = new System.Drawing.Size(34, 25);
			this.btnMapPosSetter.TabIndex = 11;
			this.btnMapPosSetter.Text = "...";
			this.btnMapPosSetter.UseVisualStyleBackColor = true;
			this.btnMapPosSetter.Click += new System.EventHandler(this.btnMapPosSetter_Click);
			// 
			// cmbDirection
			// 
			this.cmbDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbDirection.FormattingEnabled = true;
			this.cmbDirection.Location = new System.Drawing.Point(229, 77);
			this.cmbDirection.Name = "cmbDirection";
			this.cmbDirection.Size = new System.Drawing.Size(136, 23);
			this.cmbDirection.TabIndex = 13;
			this.toolTip1.SetToolTip(this.cmbDirection, "「中立」を選択すると、移動後の向きが維持されるようになります。");
			// 
			// lblDirection
			// 
			this.lblDirection.AutoSize = true;
			this.lblDirection.Location = new System.Drawing.Point(221, 57);
			this.lblDirection.Name = "lblDirection";
			this.lblDirection.Size = new System.Drawing.Size(36, 15);
			this.lblDirection.TabIndex = 12;
			this.lblDirection.Text = "方向:";
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(221, 0);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(36, 15);
			this.label10.TabIndex = 9;
			this.label10.Text = "座標:";
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Location = new System.Drawing.Point(0, 0);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(48, 15);
			this.label11.TabIndex = 7;
			this.label11.Text = "移動先:";
			// 
			// uctlPosition
			// 
			this.uctlPosition.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlPosition.Location = new System.Drawing.Point(229, 20);
			this.uctlPosition.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlPosition.Maximum = new System.Drawing.Point(100, 100);
			this.uctlPosition.MaximumSize = new System.Drawing.Size(500, 23);
			this.uctlPosition.Minimum = new System.Drawing.Point(0, 0);
			this.uctlPosition.MinimumSize = new System.Drawing.Size(100, 23);
			this.uctlPosition.Name = "uctlPosition";
			this.uctlPosition.Result = new System.Drawing.Point(0, 0);
			this.uctlPosition.Size = new System.Drawing.Size(137, 23);
			this.uctlPosition.TabIndex = 10;
			this.uctlPosition.Type = Editor.CtrlComponent.Common.ctlPosition.InputType.Point;
			// 
			// uctlFileTree
			// 
			this.uctlFileTree.AutoReload = true;
			this.uctlFileTree.CanDelete = false;
			this.uctlFileTree.CanRename = false;
			this.uctlFileTree.DefaultExnpandDepth = 2;
			this.uctlFileTree.FileFilter = "";
			this.uctlFileTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlFileTree.IncludeFiles = true;
			this.uctlFileTree.Location = new System.Drawing.Point(5, 20);
			this.uctlFileTree.Name = "uctlFileTree";
			this.uctlFileTree.RootPath = "";
			this.uctlFileTree.Size = new System.Drawing.Size(199, 283);
			this.uctlFileTree.TabIndex = 8;
			this.uctlFileTree.VisibleCheckBoxes = false;
			// 
			// ctlSQChangeMap
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.btnMapPosSetter);
			this.Controls.Add(this.cmbDirection);
			this.Controls.Add(this.uctlPosition);
			this.Controls.Add(this.lblDirection);
			this.Controls.Add(this.label10);
			this.Controls.Add(this.label11);
			this.Controls.Add(this.uctlFileTree);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.MaximumSize = new System.Drawing.Size(399, 303);
			this.MinimumSize = new System.Drawing.Size(399, 303);
			this.Name = "ctlSQChangeMap";
			this.Size = new System.Drawing.Size(399, 303);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnMapPosSetter;
		private System.Windows.Forms.ComboBox cmbDirection;
		private Common.ctlPosition uctlPosition;
		private System.Windows.Forms.Label lblDirection;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label11;
		private Common.ctlFileTree uctlFileTree;
		private System.Windows.Forms.ToolTip toolTip1;
	}
}
