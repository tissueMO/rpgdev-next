namespace Editor.CtrlComponent.Text {
	partial class ctlArgment {
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
			this.txtArgument = new System.Windows.Forms.TextBox();
			this.btnTools = new System.Windows.Forms.Button();
			this.lblArgType = new System.Windows.Forms.Label();
			this.toolHint = new System.Windows.Forms.ToolTip(this.components);
			this.lblArgComment = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// txtArgument
			// 
			this.txtArgument.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtArgument.Location = new System.Drawing.Point(22, 41);
			this.txtArgument.Name = "txtArgument";
			this.txtArgument.Size = new System.Drawing.Size(291, 23);
			this.txtArgument.TabIndex = 2;
			// 
			// btnTools
			// 
			this.btnTools.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnTools.Location = new System.Drawing.Point(313, 39);
			this.btnTools.Name = "btnTools";
			this.btnTools.Size = new System.Drawing.Size(35, 27);
			this.btnTools.TabIndex = 3;
			this.btnTools.Text = "...";
			this.btnTools.UseVisualStyleBackColor = true;
			this.btnTools.Click += new System.EventHandler(this.btnTools_Click);
			// 
			// lblArgType
			// 
			this.lblArgType.Dock = System.Windows.Forms.DockStyle.Top;
			this.lblArgType.Location = new System.Drawing.Point(0, 0);
			this.lblArgType.Margin = new System.Windows.Forms.Padding(3, 5, 3, 0);
			this.lblArgType.Name = "lblArgType";
			this.lblArgType.Size = new System.Drawing.Size(350, 19);
			this.lblArgType.TabIndex = 1;
			this.lblArgType.Text = "第 N 引数 (型名):";
			// 
			// lblArgComment
			// 
			this.lblArgComment.AutoEllipsis = true;
			this.lblArgComment.Dock = System.Windows.Forms.DockStyle.Top;
			this.lblArgComment.Location = new System.Drawing.Point(0, 19);
			this.lblArgComment.Name = "lblArgComment";
			this.lblArgComment.Padding = new System.Windows.Forms.Padding(15, 0, 0, 0);
			this.lblArgComment.Size = new System.Drawing.Size(350, 17);
			this.lblArgComment.TabIndex = 4;
			this.lblArgComment.Text = "説明";
			// 
			// ctlArgment
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.lblArgComment);
			this.Controls.Add(this.lblArgType);
			this.Controls.Add(this.btnTools);
			this.Controls.Add(this.txtArgument);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(2);
			this.MinimumSize = new System.Drawing.Size(0, 25);
			this.Name = "ctlArgment";
			this.Size = new System.Drawing.Size(350, 71);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.TextBox txtArgument;
		private System.Windows.Forms.Button btnTools;
		private System.Windows.Forms.Label lblArgType;
		private System.Windows.Forms.ToolTip toolHint;
		private System.Windows.Forms.Label lblArgComment;
	}
}
