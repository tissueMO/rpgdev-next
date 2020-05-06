namespace Editor.Dialog.Text.Script {
	partial class dlgInsertCommand {
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
			this.lblSummary = new System.Windows.Forms.Label();
			this.panel1 = new System.Windows.Forms.Panel();
			this.lblComment = new System.Windows.Forms.Label();
			this.pnlArguments = new System.Windows.Forms.FlowLayoutPanel();
			this.lblReturnType = new System.Windows.Forms.Label();
			this.lblReturnComment = new System.Windows.Forms.Label();
			this.argTools = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// lblSummary
			// 
			this.lblSummary.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lblSummary.AutoEllipsis = true;
			this.lblSummary.Font = new System.Drawing.Font("Meiryo UI", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.lblSummary.Location = new System.Drawing.Point(0, 0);
			this.lblSummary.Name = "lblSummary";
			this.lblSummary.Padding = new System.Windows.Forms.Padding(5);
			this.lblSummary.Size = new System.Drawing.Size(447, 34);
			this.lblSummary.TabIndex = 0;
			this.lblSummary.Text = "コマンド概要";
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BackColor = System.Drawing.SystemColors.Window;
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel1.Controls.Add(this.lblSummary);
			this.panel1.Location = new System.Drawing.Point(12, 12);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(449, 38);
			this.panel1.TabIndex = 0;
			// 
			// lblComment
			// 
			this.lblComment.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lblComment.AutoEllipsis = true;
			this.lblComment.Location = new System.Drawing.Point(12, 63);
			this.lblComment.Name = "lblComment";
			this.lblComment.Size = new System.Drawing.Size(449, 49);
			this.lblComment.TabIndex = 1;
			this.lblComment.Text = "詳細な説明";
			this.lblComment.UseMnemonic = false;
			// 
			// pnlArguments
			// 
			this.pnlArguments.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.pnlArguments.AutoScroll = true;
			this.pnlArguments.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pnlArguments.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.pnlArguments.Location = new System.Drawing.Point(12, 121);
			this.pnlArguments.Name = "pnlArguments";
			this.pnlArguments.Size = new System.Drawing.Size(451, 214);
			this.pnlArguments.TabIndex = 2;
			this.pnlArguments.WrapContents = false;
			// 
			// lblReturnType
			// 
			this.lblReturnType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lblReturnType.AutoEllipsis = true;
			this.lblReturnType.Location = new System.Drawing.Point(12, 343);
			this.lblReturnType.Name = "lblReturnType";
			this.lblReturnType.Size = new System.Drawing.Size(449, 19);
			this.lblReturnType.TabIndex = 3;
			this.lblReturnType.Text = "戻り値:";
			this.lblReturnType.UseMnemonic = false;
			// 
			// lblReturnComment
			// 
			this.lblReturnComment.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lblReturnComment.AutoEllipsis = true;
			this.lblReturnComment.Location = new System.Drawing.Point(27, 362);
			this.lblReturnComment.Name = "lblReturnComment";
			this.lblReturnComment.Size = new System.Drawing.Size(434, 31);
			this.lblReturnComment.TabIndex = 5;
			this.lblReturnComment.UseMnemonic = false;
			// 
			// argTools
			// 
			this.argTools.Name = "argTools";
			this.argTools.ShowImageMargin = false;
			this.argTools.Size = new System.Drawing.Size(36, 4);
			this.argTools.Text = "コマンドツリー";
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.ctlOKCancel1.DisenabledAcceptButton = false;
			this.ctlOKCancel1.DisenabledCancelButton = false;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(257, 412);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.OKTitle = "OK";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 4;
			this.ctlOKCancel1.ClickOK += new System.EventHandler(this.ctlOKCancel1_ClickOK);
			// 
			// dlgInsertCommand
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(476, 452);
			this.Controls.Add(this.lblReturnComment);
			this.Controls.Add(this.lblReturnType);
			this.Controls.Add(this.pnlArguments);
			this.Controls.Add(this.lblComment);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgInsertCommand";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "コマンドの挿入";
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label lblSummary;
		private System.Windows.Forms.Panel panel1;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
		private System.Windows.Forms.Label lblComment;
		private System.Windows.Forms.FlowLayoutPanel pnlArguments;
		private System.Windows.Forms.Label lblReturnType;
		private System.Windows.Forms.Label lblReturnComment;
		private System.Windows.Forms.ContextMenuStrip argTools;
	}
}