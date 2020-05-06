namespace Editor.Dialog.Text.Script {
	partial class dlgEVcmdMessage {
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
			this.uctlSQMessage = new Editor.CtrlComponent.Map.ctlSQMessage();
			this.ctlOKCancel1 = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.SuspendLayout();
			// 
			// uctlSQMessage
			// 
			this.uctlSQMessage.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlSQMessage.Location = new System.Drawing.Point(8, 10);
			this.uctlSQMessage.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlSQMessage.Name = "uctlSQMessage";
			this.uctlSQMessage.Size = new System.Drawing.Size(454, 248);
			this.uctlSQMessage.TabIndex = 0;
			// 
			// ctlOKCancel1
			// 
			this.ctlOKCancel1.DisenabledAcceptButton = true;
			this.ctlOKCancel1.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.ctlOKCancel1.Location = new System.Drawing.Point(253, 267);
			this.ctlOKCancel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.ctlOKCancel1.Name = "ctlOKCancel1";
			this.ctlOKCancel1.Size = new System.Drawing.Size(212, 34);
			this.ctlOKCancel1.TabIndex = 1;
			// 
			// dlgEVcmdMessage
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(473, 307);
			this.Controls.Add(this.ctlOKCancel1);
			this.Controls.Add(this.uctlSQMessage);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgEVcmdMessage";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "メッセージを挿入";
			this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Map.ctlSQMessage uctlSQMessage;
		private CtrlComponent.Common.ctlOKCancel ctlOKCancel1;
	}
}