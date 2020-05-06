namespace Editor.Dialog.Common {
	partial class dlgFileSelect {
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
			this.ScrollPanel = new System.Windows.Forms.Panel();
			this.uctlTileset = new Editor.CtrlComponent.Common.ctlSelectTile();
			this.picViewer = new System.Windows.Forms.PictureBox();
			this.btnNothing = new System.Windows.Forms.Button();
			this.uctlMusicPlayer = new Editor.CtrlComponent.Common.ctlSoundPlayer();
			this.uctlOKCancel = new Editor.CtrlComponent.Common.ctlOKCancel();
			this.uctlFileTree = new Editor.CtrlComponent.Common.ctlFileTree();
			this.ScrollPanel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.picViewer)).BeginInit();
			this.SuspendLayout();
			// 
			// ScrollPanel
			// 
			this.ScrollPanel.AutoScroll = true;
			this.ScrollPanel.Controls.Add(this.uctlTileset);
			this.ScrollPanel.Controls.Add(this.picViewer);
			this.ScrollPanel.Location = new System.Drawing.Point(292, 12);
			this.ScrollPanel.Name = "ScrollPanel";
			this.ScrollPanel.Size = new System.Drawing.Size(410, 275);
			this.ScrollPanel.TabIndex = 1;
			// 
			// uctlTileset
			// 
			this.uctlTileset.AutoScroll = true;
			this.uctlTileset.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.uctlTileset.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.uctlTileset.Location = new System.Drawing.Point(0, 0);
			this.uctlTileset.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlTileset.Name = "uctlTileset";
			this.uctlTileset.Position = new System.Drawing.Point(0, 0);
			this.uctlTileset.Size = new System.Drawing.Size(410, 275);
			this.uctlTileset.TabIndex = 2;
			this.uctlTileset.TileSize = new System.Drawing.Size(0, 0);
			this.uctlTileset.Visible = false;
			this.uctlTileset.TileDoubleClicked += new System.EventHandler(this.uctlTileset_DoubleClick);
			this.uctlTileset.MouseEnter += new System.EventHandler(this.uctlTileset_MouseEnter);
			// 
			// picViewer
			// 
			this.picViewer.BackColor = System.Drawing.SystemColors.AppWorkspace;
			this.picViewer.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.picViewer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.picViewer.ErrorImage = null;
			this.picViewer.InitialImage = null;
			this.picViewer.Location = new System.Drawing.Point(0, 0);
			this.picViewer.Name = "picViewer";
			this.picViewer.Size = new System.Drawing.Size(410, 275);
			this.picViewer.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.picViewer.TabIndex = 1;
			this.picViewer.TabStop = false;
			// 
			// btnNothing
			// 
			this.btnNothing.Location = new System.Drawing.Point(387, 341);
			this.btnNothing.Name = "btnNothing";
			this.btnNothing.Size = new System.Drawing.Size(100, 28);
			this.btnNothing.TabIndex = 3;
			this.btnNothing.Text = "選択しない";
			this.btnNothing.UseVisualStyleBackColor = true;
			this.btnNothing.Click += new System.EventHandler(this.btnNothing_Click);
			// 
			// uctlMusicPlayer
			// 
			this.uctlMusicPlayer.Enabled = false;
			this.uctlMusicPlayer.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlMusicPlayer.Location = new System.Drawing.Point(602, 294);
			this.uctlMusicPlayer.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlMusicPlayer.Name = "uctlMusicPlayer";
			this.uctlMusicPlayer.SelfLoadMode = true;
			this.uctlMusicPlayer.Size = new System.Drawing.Size(97, 30);
			this.uctlMusicPlayer.SoundFileName = null;
			this.uctlMusicPlayer.TabIndex = 2;
			this.uctlMusicPlayer.BeforePlaySound += new System.EventHandler(this.uctlMusicPlayer_BeforePlaySound);
			// 
			// uctlOKCancel
			// 
			this.uctlOKCancel.DisenabledAcceptButton = false;
			this.uctlOKCancel.DisenabledCancelButton = false;
			this.uctlOKCancel.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlOKCancel.Location = new System.Drawing.Point(490, 338);
			this.uctlOKCancel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlOKCancel.Name = "uctlOKCancel";
			this.uctlOKCancel.OKTitle = "OK";
			this.uctlOKCancel.Size = new System.Drawing.Size(212, 34);
			this.uctlOKCancel.TabIndex = 4;
			this.uctlOKCancel.ClickOK += new System.EventHandler(this.uctlOKCancel_ClickOK);
			// 
			// uctlFileTree
			// 
			this.uctlFileTree.AutoReload = true;
			this.uctlFileTree.CanDelete = false;
			this.uctlFileTree.CanRename = false;
			this.uctlFileTree.DefaultExnpandDepth = 2;
			this.uctlFileTree.Dock = System.Windows.Forms.DockStyle.Left;
			this.uctlFileTree.FileFilter = "";
			this.uctlFileTree.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.uctlFileTree.IncludeFiles = true;
			this.uctlFileTree.Location = new System.Drawing.Point(0, 0);
			this.uctlFileTree.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.uctlFileTree.Name = "uctlFileTree";
			this.uctlFileTree.RootPath = "";
			this.uctlFileTree.Size = new System.Drawing.Size(282, 380);
			this.uctlFileTree.TabIndex = 0;
			this.uctlFileTree.VisibleCheckBoxes = false;
			this.uctlFileTree.AfterSelectNode += new System.Windows.Forms.TreeViewEventHandler(this.uctlFileTree_NodeAfterSelect);
			this.uctlFileTree.FileDoubleClicked += new System.EventHandler(this.uctlFileTree_FileDoubleClicked);
			// 
			// dlgFileSelect
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(711, 380);
			this.Controls.Add(this.btnNothing);
			this.Controls.Add(this.uctlMusicPlayer);
			this.Controls.Add(this.ScrollPanel);
			this.Controls.Add(this.uctlOKCancel);
			this.Controls.Add(this.uctlFileTree);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "dlgFileSelect";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "素材選択";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.dlgFileSelect_FormClosing);
			this.Load += new System.EventHandler(this.dlgFileSelect_Load);
			this.ScrollPanel.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.picViewer)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private CtrlComponent.Common.ctlFileTree uctlFileTree;
		private CtrlComponent.Common.ctlSoundPlayer uctlMusicPlayer;
		private CtrlComponent.Common.ctlOKCancel uctlOKCancel;
		private System.Windows.Forms.Panel ScrollPanel;
		private System.Windows.Forms.PictureBox picViewer;
		private System.Windows.Forms.Button btnNothing;
		private CtrlComponent.Common.ctlSelectTile uctlTileset;
	}
}