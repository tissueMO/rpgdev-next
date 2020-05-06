namespace Editor.CtrlComponent.Map {
	partial class ctlSQMessage {
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
			this.txtMessageMsg = new System.Windows.Forms.TextBox();
			this.txtSpeakerMsg = new System.Windows.Forms.TextBox();
			this.btnNewPageMsg = new System.Windows.Forms.Button();
			this.btnInsertValueMsg = new System.Windows.Forms.Button();
			this.label21 = new System.Windows.Forms.Label();
			this.label20 = new System.Windows.Forms.Label();
			this.label15 = new System.Windows.Forms.Label();
			this.label22 = new System.Windows.Forms.Label();
			this.btnBreakMsg = new System.Windows.Forms.Button();
			this.cmbFaceMsg = new System.Windows.Forms.ComboBox();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.SuspendLayout();
			// 
			// txtMessageMsg
			// 
			this.txtMessageMsg.Font = new System.Drawing.Font("ＭＳ ゴシック", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.txtMessageMsg.Location = new System.Drawing.Point(84, 59);
			this.txtMessageMsg.Multiline = true;
			this.txtMessageMsg.Name = "txtMessageMsg";
			this.txtMessageMsg.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.txtMessageMsg.Size = new System.Drawing.Size(365, 182);
			this.txtMessageMsg.TabIndex = 6;
			// 
			// txtSpeakerMsg
			// 
			this.txtSpeakerMsg.Location = new System.Drawing.Point(84, 30);
			this.txtSpeakerMsg.Name = "txtSpeakerMsg";
			this.txtSpeakerMsg.Size = new System.Drawing.Size(180, 23);
			this.txtSpeakerMsg.TabIndex = 3;
			// 
			// btnNewPageMsg
			// 
			this.btnNewPageMsg.Location = new System.Drawing.Point(4, 184);
			this.btnNewPageMsg.Name = "btnNewPageMsg";
			this.btnNewPageMsg.Size = new System.Drawing.Size(75, 27);
			this.btnNewPageMsg.TabIndex = 8;
			this.btnNewPageMsg.Text = "改ページ";
			this.btnNewPageMsg.UseVisualStyleBackColor = true;
			this.btnNewPageMsg.Click += new System.EventHandler(this.btnNewPageMsg_Click);
			// 
			// btnInsertValueMsg
			// 
			this.btnInsertValueMsg.Location = new System.Drawing.Point(4, 157);
			this.btnInsertValueMsg.Name = "btnInsertValueMsg";
			this.btnInsertValueMsg.Size = new System.Drawing.Size(75, 27);
			this.btnInsertValueMsg.TabIndex = 7;
			this.btnInsertValueMsg.Text = "変数挿入";
			this.btnInsertValueMsg.UseVisualStyleBackColor = true;
			this.btnInsertValueMsg.Click += new System.EventHandler(this.btnInsertValueMsg_Click);
			// 
			// label21
			// 
			this.label21.AutoSize = true;
			this.label21.Location = new System.Drawing.Point(3, 62);
			this.label21.Name = "label21";
			this.label21.Size = new System.Drawing.Size(37, 15);
			this.label21.TabIndex = 5;
			this.label21.Text = "セリフ:";
			// 
			// label20
			// 
			this.label20.AutoSize = true;
			this.label20.Location = new System.Drawing.Point(3, 34);
			this.label20.Name = "label20";
			this.label20.Size = new System.Drawing.Size(48, 15);
			this.label20.TabIndex = 2;
			this.label20.Text = "発話者:";
			// 
			// label15
			// 
			this.label15.AutoSize = true;
			this.label15.Location = new System.Drawing.Point(3, 6);
			this.label15.Name = "label15";
			this.label15.Size = new System.Drawing.Size(45, 15);
			this.label15.TabIndex = 0;
			this.label15.Text = "フェイス:";
			// 
			// label22
			// 
			this.label22.AutoSize = true;
			this.label22.Font = new System.Drawing.Font("Meiryo UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.label22.Location = new System.Drawing.Point(294, 41);
			this.label22.Name = "label22";
			this.label22.Size = new System.Drawing.Size(121, 20);
			this.label22.TabIndex = 4;
			this.label22.Text = "｜                ｜";
			// 
			// btnBreakMsg
			// 
			this.btnBreakMsg.Location = new System.Drawing.Point(4, 211);
			this.btnBreakMsg.Name = "btnBreakMsg";
			this.btnBreakMsg.Size = new System.Drawing.Size(75, 27);
			this.btnBreakMsg.TabIndex = 9;
			this.btnBreakMsg.Text = "中断";
			this.toolTip1.SetToolTip(this.btnBreakMsg, "メッセージウィンドウを閉じずに処理を中断します。");
			this.btnBreakMsg.UseVisualStyleBackColor = true;
			this.btnBreakMsg.Click += new System.EventHandler(this.btnBreakMsg_Click);
			// 
			// cmbFaceMsg
			// 
			this.cmbFaceMsg.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbFaceMsg.FormattingEnabled = true;
			this.cmbFaceMsg.Location = new System.Drawing.Point(84, 2);
			this.cmbFaceMsg.Name = "cmbFaceMsg";
			this.cmbFaceMsg.Size = new System.Drawing.Size(180, 23);
			this.cmbFaceMsg.TabIndex = 1;
			// 
			// ctlSQMessage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.cmbFaceMsg);
			this.Controls.Add(this.btnBreakMsg);
			this.Controls.Add(this.txtMessageMsg);
			this.Controls.Add(this.txtSpeakerMsg);
			this.Controls.Add(this.btnNewPageMsg);
			this.Controls.Add(this.btnInsertValueMsg);
			this.Controls.Add(this.label21);
			this.Controls.Add(this.label20);
			this.Controls.Add(this.label15);
			this.Controls.Add(this.label22);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "ctlSQMessage";
			this.Size = new System.Drawing.Size(454, 244);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		private System.Windows.Forms.TextBox txtMessageMsg;
		private System.Windows.Forms.TextBox txtSpeakerMsg;
		private System.Windows.Forms.Button btnNewPageMsg;
		private System.Windows.Forms.Button btnInsertValueMsg;
		private System.Windows.Forms.Label label21;
		private System.Windows.Forms.Label label20;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.Label label22;
		private System.Windows.Forms.Button btnBreakMsg;
		private System.Windows.Forms.ComboBox cmbFaceMsg;
		private System.Windows.Forms.ToolTip toolTip1;
	}
}
