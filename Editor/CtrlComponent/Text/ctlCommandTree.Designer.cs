namespace Editor.CtrlComponent.Text {
	partial class ctlCommandTree {
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
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("スクリプト標準命令");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("スクリプト共通定数");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("スクリプト共通変数");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("スクリプト共通関数");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("変数");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("関数");
            System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("クラス");
            System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("定数");
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("列挙体");
            System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("イベントハンドラー");
            System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode("組み込みオブジェクト", new System.Windows.Forms.TreeNode[] {
            treeNode5,
            treeNode6,
            treeNode7,
            treeNode8,
            treeNode9,
            treeNode10});
            System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode("メッセージ表示");
            System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode("マップを移動");
            System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode("ファイル名/タイルパレット位置を挿入");
            System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode("色を挿入");
            System.Windows.Forms.TreeNode treeNode16 = new System.Windows.Forms.TreeNode("イベントIDを挿入");
            System.Windows.Forms.TreeNode treeNode17 = new System.Windows.Forms.TreeNode("データベースIDを挿入");
            System.Windows.Forms.TreeNode treeNode18 = new System.Windows.Forms.TreeNode("データベース列を挿入");
            System.Windows.Forms.TreeNode treeNode19 = new System.Windows.Forms.TreeNode("移動経路を挿入(イベント日常移動)");
            System.Windows.Forms.TreeNode treeNode20 = new System.Windows.Forms.TreeNode("移動経路を挿入(イベント臨時移動)");
            System.Windows.Forms.TreeNode treeNode21 = new System.Windows.Forms.TreeNode("移動経路を挿入(プレイヤー臨時移動)");
            System.Windows.Forms.TreeNode treeNode22 = new System.Windows.Forms.TreeNode("コマンド名");
            System.Windows.Forms.TreeNode treeNode23 = new System.Windows.Forms.TreeNode("中間ノード");
            System.Windows.Forms.TreeNode treeNode24 = new System.Windows.Forms.TreeNode("引数");
            System.Windows.Forms.TreeNode treeNode25 = new System.Windows.Forms.TreeNode("戻り値");
            System.Windows.Forms.TreeNode treeNode26 = new System.Windows.Forms.TreeNode("コメント");
            System.Windows.Forms.TreeNode treeNode27 = new System.Windows.Forms.TreeNode("スクリプト関数タグを挿入", new System.Windows.Forms.TreeNode[] {
            treeNode22,
            treeNode23,
            treeNode24,
            treeNode25,
            treeNode26});
            System.Windows.Forms.TreeNode treeNode28 = new System.Windows.Forms.TreeNode("ツール", new System.Windows.Forms.TreeNode[] {
            treeNode12,
            treeNode13,
            treeNode14,
            treeNode15,
            treeNode16,
            treeNode17,
            treeNode18,
            treeNode19,
            treeNode20,
            treeNode21,
            treeNode27});
            System.Windows.Forms.TreeNode treeNode29 = new System.Windows.Forms.TreeNode("コードを直接入力して挿入");
            System.Windows.Forms.TreeNode treeNode30 = new System.Windows.Forms.TreeNode("行コメントを挿入");
            System.Windows.Forms.TreeNode treeNode31 = new System.Windows.Forms.TreeNode("空行を挿入");
            System.Windows.Forms.TreeNode treeNode32 = new System.Windows.Forms.TreeNode("Easyエディター", new System.Windows.Forms.TreeNode[] {
            treeNode29,
            treeNode30,
            treeNode31});
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ctlCommandTree));
            this.trvTree = new System.Windows.Forms.TreeView();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.rmnReloadSQNodes = new System.Windows.Forms.ToolStripMenuItem();
            this.CommandIcons = new System.Windows.Forms.ImageList(this.components);
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // trvTree
            // 
            this.trvTree.ContextMenuStrip = this.contextMenuStrip1;
            this.trvTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trvTree.FullRowSelect = true;
            this.trvTree.HideSelection = false;
            this.trvTree.HotTracking = true;
            this.trvTree.ImageKey = "ListItem(LI)_5796.png";
            this.trvTree.ImageList = this.CommandIcons;
            this.trvTree.Location = new System.Drawing.Point(0, 0);
            this.trvTree.Name = "trvTree";
            treeNode1.ImageKey = "brackets_Square_16xLG.png";
            treeNode1.Name = "ノード0";
            treeNode1.SelectedImageKey = "brackets_Square_16xLG.png";
            treeNode1.Text = "スクリプト標準命令";
            treeNode1.ToolTipText = "Squirrel言語でサポートされている命令が含まれます。";
            treeNode2.ImageKey = "Constant_495_128x.png";
            treeNode2.Name = "ノード1";
            treeNode2.SelectedImageKey = "Constant_495_128x.png";
            treeNode2.Text = "スクリプト共通定数";
            treeNode2.ToolTipText = "定数定義スクリプトに含まれる定数です。すべてのスクリプトで使用できます。";
            treeNode3.ImageKey = "FieldIcon.png";
            treeNode3.Name = "ノード3";
            treeNode3.SelectedImageKey = "FieldIcon.png";
            treeNode3.Text = "スクリプト共通変数";
            treeNode3.ToolTipText = "2文字のサフィックスとFixedIDで構成される、スクリプトのグローバル変数です。これらの値はセーブ対象となります。";
            treeNode4.ImageKey = "method_16xLG.png";
            treeNode4.Name = "ノード0";
            treeNode4.SelectedImageKey = "method_16xLG.png";
            treeNode4.Text = "スクリプト共通関数";
            treeNode4.ToolTipText = "共通関数定義スクリプトに含まれる関数です。すべてのスクリプトで使用できます。";
            treeNode5.ImageKey = "FieldIcon.png";
            treeNode5.Name = "cppvalue";
            treeNode5.SelectedImageKey = "FieldIcon.png";
            treeNode5.Text = "変数";
            treeNode5.ToolTipText = "クラスインスタンスを含むグローバル領域の変数です。";
            treeNode6.ImageKey = "method_16xLG.png";
            treeNode6.Name = "cppfunc";
            treeNode6.SelectedImageKey = "method_16xLG.png";
            treeNode6.Text = "関数";
            treeNode6.ToolTipText = "汎用的な機能を提供します。";
            treeNode7.ImageKey = "Class_489.png";
            treeNode7.Name = "cppclass";
            treeNode7.SelectedImageKey = "Class_489.png";
            treeNode7.Tag = "";
            treeNode7.Text = "クラス";
            treeNode7.ToolTipText = "一定の役割を持つ機能群です。インスタンスを利用するには「組み込みオブジェクト：変数」から参照して下さい。";
            treeNode8.ImageKey = "Constant_495.png";
            treeNode8.Name = "cppconst";
            treeNode8.SelectedImageKey = "Constant_495.png";
            treeNode8.Text = "定数";
            treeNode8.ToolTipText = "値に名前を付けたものです。";
            treeNode9.ImageKey = "Enum_582.png";
            treeNode9.Name = "cppenum";
            treeNode9.SelectedImageKey = "Enum_582.png";
            treeNode9.Text = "列挙体";
            treeNode9.ToolTipText = "定数をグループにしたものです。";
            treeNode10.ImageKey = "Event_594.png";
            treeNode10.Name = "sqeventhandler";
            treeNode10.SelectedImageKey = "Event_594.png";
            treeNode10.Text = "イベントハンドラー";
            treeNode10.ToolTipText = "ゲームプログラム側がスクリプトに処理を委譲するものを請け負うのに必要な関数です。";
            treeNode11.ImageKey = "Schema_16xLG.png";
            treeNode11.Name = "cppobject";
            treeNode11.SelectedImageKey = "Schema_16xLG.png";
            treeNode11.Text = "組み込みオブジェクト";
            treeNode11.ToolTipText = "ゲームプログラムがサポートしているコンポーネントが含まれます。";
            treeNode12.ImageKey = "Bubble_16xLG.png";
            treeNode12.Name = "ノード0";
            treeNode12.SelectedImageKey = "Bubble_16xLG.png";
            treeNode12.Tag = "msg";
            treeNode12.Text = "メッセージ表示";
            treeNode12.ToolTipText = "会話ウィンドウを表示するコマンドを挿入します。";
            treeNode13.ImageKey = "PathEditorLinkMultiplicityOne.png";
            treeNode13.Name = "ノード8";
            treeNode13.SelectedImageKey = "PathEditorLinkMultiplicityOne.png";
            treeNode13.Tag = "map";
            treeNode13.Text = "マップを移動";
            treeNode13.ToolTipText = "マップを移動させるコマンドを挿入します。";
            treeNode14.ImageKey = "Textfile_818_16x.png";
            treeNode14.Name = "ノード5";
            treeNode14.SelectedImageKey = "Textfile_818_16x.png";
            treeNode14.Tag = "filename";
            treeNode14.Text = "ファイル名/タイルパレット位置を挿入";
            treeNode14.ToolTipText = "両端にダブルクォートが付加されます。";
            treeNode15.ImageKey = "applications-graphics-2.png";
            treeNode15.Name = "ノード9";
            treeNode15.SelectedImageKey = "applications-graphics-2.png";
            treeNode15.Tag = "color";
            treeNode15.Text = "色を挿入";
            treeNode16.ImageKey = "Event_594.png";
            treeNode16.Name = "ノード0";
            treeNode16.SelectedImageKey = "Event_594.png";
            treeNode16.Tag = "evid";
            treeNode16.Text = "イベントIDを挿入";
            treeNode16.ToolTipText = "ここで挿入されるのはFixedIDです。これはマップ/イベントエディターでのみ有効です。";
            treeNode17.ImageKey = "DatabaseReferenceNode_7598.png";
            treeNode17.Name = "ノード6";
            treeNode17.SelectedImageKey = "DatabaseReferenceNode_7598.png";
            treeNode17.Tag = "dbid";
            treeNode17.Text = "データベースIDを挿入";
            treeNode17.ToolTipText = "ここで挿入されるのはFixedIDです。";
            treeNode18.ImageKey = "DatabaseReferenceNode_7598.png";
            treeNode18.Name = "ノード7";
            treeNode18.SelectedImageKey = "DatabaseReferenceNode_7598.png";
            treeNode18.Tag = "dbcolumn";
            treeNode18.Text = "データベース列を挿入";
            treeNode18.ToolTipText = "列インデックス、列名のどちらかを選択して挿入します。";
            treeNode19.ImageKey = "Branch_13220.png";
            treeNode19.Name = "ノード10";
            treeNode19.SelectedImageKey = "Branch_13220.png";
            treeNode19.Tag = "pathnormalmoveev";
            treeNode19.Text = "移動経路を挿入(イベント日常移動)";
            treeNode19.ToolTipText = "イベントの日常移動に使われる移動パターンを一括でセットします。";
            treeNode20.ImageKey = "Branch_13220.png";
            treeNode20.Name = "ノード0";
            treeNode20.SelectedImageKey = "Branch_13220.png";
            treeNode20.Tag = "pathextramoveev";
            treeNode20.Text = "移動経路を挿入(イベント臨時移動)";
            treeNode20.ToolTipText = "イベントの臨時移動に使われる移動パターンを一括でセットします。";
            treeNode21.Name = "ノード0";
            treeNode21.Tag = "pathextramoveplayer";
            treeNode21.Text = "移動経路を挿入(プレイヤー臨時移動)";
            treeNode21.ToolTipText = "プレイヤーの臨時移動に使われる移動パターンを一括でセットします。";
            treeNode22.Name = "ノード1";
            treeNode22.Tag = "summary";
            treeNode22.Text = "コマンド名";
            treeNode22.ToolTipText = "コマンドツリー上に表示される表示名です。";
            treeNode23.Name = "ノード2";
            treeNode23.Tag = "node";
            treeNode23.Text = "中間ノード";
            treeNode23.ToolTipText = "コマンドツリー上での途中の分類です。\\で区切ることで階層を深めることができます。";
            treeNode24.Name = "ノード6";
            treeNode24.Tag = "par";
            treeNode24.Text = "引数";
            treeNode24.ToolTipText = "上から順に1番目、2番目...と認識されます。";
            treeNode25.Name = "ノード5";
            treeNode25.Tag = "return";
            treeNode25.Text = "戻り値";
            treeNode26.Name = "ノード4";
            treeNode26.Tag = "comment";
            treeNode26.Text = "コメント";
            treeNode27.ImageKey = "ParametersInfo_2423.png";
            treeNode27.Name = "ノード0";
            treeNode27.SelectedImageKey = "ParametersInfo_2423.png";
            treeNode27.Tag = "scripttag";
            treeNode27.Text = "スクリプト関数タグを挿入";
            treeNode27.ToolTipText = "コマンドツリーに関数を登録するときに必要なタグを挿入します。なお、これらのタグは所定のファイル内でのみ有効です。";
            treeNode28.ImageKey = "PickAxe_16xLG.png";
            treeNode28.Name = "ノード4";
            treeNode28.SelectedImageKey = "PickAxe_16xLG.png";
            treeNode28.Tag = "tools";
            treeNode28.Text = "ツール";
            treeNode29.Name = "ノード0";
            treeNode29.Tag = "rawcode";
            treeNode29.Text = "コードを直接入力して挿入";
            treeNode30.Name = "ノード2";
            treeNode30.Tag = "comment";
            treeNode30.Text = "行コメントを挿入";
            treeNode31.Name = "ノード3";
            treeNode31.Tag = "emptyline";
            treeNode31.Text = "空行を挿入";
            treeNode32.ImageKey = "CustomActionsEditor_5850.png";
            treeNode32.Name = "ノード0";
            treeNode32.SelectedImageKey = "CustomActionsEditor_5850.png";
            treeNode32.Tag = "easytools";
            treeNode32.Text = "Easyエディター";
            treeNode32.ToolTipText = "イベント編集方法を「簡易編集モード」にしている場合に使える機能です。イベント編集以外では無効となります。";
            this.trvTree.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3,
            treeNode4,
            treeNode11,
            treeNode28,
            treeNode32});
            this.trvTree.SelectedImageIndex = 0;
            this.trvTree.ShowNodeToolTips = true;
            this.trvTree.Size = new System.Drawing.Size(281, 464);
            this.trvTree.TabIndex = 0;
            this.trvTree.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.trvTree_NodeMouseClick);
            this.trvTree.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.trvTree_MouseDoubleClick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.rmnReloadSQNodes});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(184, 26);
            // 
            // rmnReloadSQNodes
            // 
            this.rmnReloadSQNodes.Image = ((System.Drawing.Image)(resources.GetObject("rmnReloadSQNodes.Image")));
            this.rmnReloadSQNodes.Name = "rmnReloadSQNodes";
            this.rmnReloadSQNodes.Size = new System.Drawing.Size(183, 22);
            this.rmnReloadSQNodes.Text = "最新の状態に更新(&R)";
            this.rmnReloadSQNodes.Click += new System.EventHandler(this.rmnReloadSQNodes_Click);
            // 
            // CommandIcons
            // 
            this.CommandIcons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("CommandIcons.ImageStream")));
            this.CommandIcons.TransparentColor = System.Drawing.Color.Transparent;
            this.CommandIcons.Images.SetKeyName(0, "ListItem(LI)_5796.png");
            this.CommandIcons.Images.SetKeyName(1, "Constant_495.png");
            this.CommandIcons.Images.SetKeyName(2, "Enum_582.png");
            this.CommandIcons.Images.SetKeyName(3, "EnumItem_588.png");
            this.CommandIcons.Images.SetKeyName(4, "Class_489.png");
            this.CommandIcons.Images.SetKeyName(5, "Field-Protected_544.png");
            this.CommandIcons.Images.SetKeyName(6, "Property_501.png");
            this.CommandIcons.Images.SetKeyName(7, "Method_636.png");
            this.CommandIcons.Images.SetKeyName(8, "Event_594.png");
            this.CommandIcons.Images.SetKeyName(9, "brackets_Square_16xLG.png");
            this.CommandIcons.Images.SetKeyName(10, "Constant_495_128x.png");
            this.CommandIcons.Images.SetKeyName(11, "Schema_16xLG.png");
            this.CommandIcons.Images.SetKeyName(12, "FieldIcon.png");
            this.CommandIcons.Images.SetKeyName(13, "method_16xLG.png");
            this.CommandIcons.Images.SetKeyName(14, "flag_16xLG.png");
            this.CommandIcons.Images.SetKeyName(15, "domaintypeicon.png");
            this.CommandIcons.Images.SetKeyName(16, "Strings_7959_16x.png");
            this.CommandIcons.Images.SetKeyName(17, "PickAxe_16xLG.png");
            this.CommandIcons.Images.SetKeyName(18, "Textfile_818_16x.png");
            this.CommandIcons.Images.SetKeyName(19, "applications-graphics-2.png");
            this.CommandIcons.Images.SetKeyName(20, "DatabaseReferenceNode_7598.png");
            this.CommandIcons.Images.SetKeyName(21, "Branch_13220.png");
            this.CommandIcons.Images.SetKeyName(22, "PathEditorLinkMultiplicityOne.png");
            this.CommandIcons.Images.SetKeyName(23, "WindowsForm_817_16x.png");
            this.CommandIcons.Images.SetKeyName(24, "enum_16xLG.png");
            this.CommandIcons.Images.SetKeyName(25, "class_16xLG.png");
            this.CommandIcons.Images.SetKeyName(26, "Bubble_16xLG.png");
            this.CommandIcons.Images.SetKeyName(27, "ParametersInfo_2423.png");
            this.CommandIcons.Images.SetKeyName(28, "CustomActionsEditor_5850.png");
            // 
            // ctlCommandTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.trvTree);
            this.Font = new System.Drawing.Font("Meiryo UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ctlCommandTree";
            this.Size = new System.Drawing.Size(281, 464);
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.ImageList CommandIcons;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem rmnReloadSQNodes;
		public System.Windows.Forms.TreeView trvTree;
	}
}
