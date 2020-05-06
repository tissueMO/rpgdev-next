namespace Editor.CtrlComponent.Common {
	partial class ctlFileTree {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlFileTree));
			this.imlIcons = new System.Windows.Forms.ImageList(this.components);
			this.trvTree = new System.Windows.Forms.TreeView();
			this.SuspendLayout();
			// 
			// imlIcons
			// 
			this.imlIcons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imlIcons.ImageStream")));
			this.imlIcons.TransparentColor = System.Drawing.Color.Transparent;
			this.imlIcons.Images.SetKeyName(0, "Showbottompanel_10299.png");
			this.imlIcons.Images.SetKeyName(1, "FormatDocument_382.png");
			// 
			// trvTree
			// 
			this.trvTree.Dock = System.Windows.Forms.DockStyle.Fill;
			this.trvTree.FullRowSelect = true;
			this.trvTree.HideSelection = false;
			this.trvTree.HotTracking = true;
			this.trvTree.ImageIndex = 0;
			this.trvTree.ImageList = this.imlIcons;
			this.trvTree.Location = new System.Drawing.Point(0, 0);
			this.trvTree.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
			this.trvTree.Name = "trvTree";
			this.trvTree.SelectedImageIndex = 0;
			this.trvTree.Size = new System.Drawing.Size(213, 311);
			this.trvTree.TabIndex = 0;
			this.trvTree.BeforeLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.trvTree_BeforeLabelEdit);
			this.trvTree.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.trvTree_AfterLabelEdit);
			this.trvTree.AfterCheck += new System.Windows.Forms.TreeViewEventHandler(this.trvTree_AfterCheck);
			this.trvTree.BeforeCollapse += new System.Windows.Forms.TreeViewCancelEventHandler(this.trvTree_BeforeCollapse);
			this.trvTree.AfterCollapse += new System.Windows.Forms.TreeViewEventHandler(this.trvTree_AfterCollapse);
			this.trvTree.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.trvTree_BeforeSelect);
			this.trvTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.trvTree_AfterSelect);
			this.trvTree.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.trvTree_MouseDoubleClick);
			// 
			// ctlFileTree
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.trvTree);
			this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Name = "ctlFileTree";
			this.Size = new System.Drawing.Size(213, 311);
			this.ResumeLayout(false);

		}

		#endregion

		internal System.Windows.Forms.ImageList imlIcons;
		private System.Windows.Forms.TreeView trvTree;
	}
}
