using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Text.Script {
    using Properties;
    using SQObject = GenerateGlueCode.GenerateCPPDocument.SQObject;

    public partial class dlgInsertCommand : Form {
        private readonly SQObject obj;
        private CtrlComponent.Text.ctlCommandTree comTree;

        /// <summary>
        /// コンストラクター：関数入力モード
        /// </summary>
        public dlgInsertCommand(CtrlComponent.Text.ctlCommandTree comTree, SQObject obj, string[] args = null) {
            this.InitializeComponent();
            this.obj = obj;

            //与えられたオブジェクトに応じてセットアップする
            //概要
            if (!string.IsNullOrEmpty(obj.Summary)) {
                this.lblSummary.Text = obj.Summary;
            } else {
                //概要がない場合はオブジェクトのフルパスをそのまま表示する
                this.lblSummary.Text = (obj.ClassPath ?? "不明な関数: " + obj.Name).Replace("\\", ".");
            }

            //詳細説明
            if (!string.IsNullOrEmpty(obj.Comment)) {
                this.lblComment.Text = obj.Comment;
            } else {
                this.lblComment.Text = "説明はありません。";
            }

            //戻り値の型
            if (!string.IsNullOrEmpty(obj.ReturnType)) {
                this.lblReturnType.Text = $"戻り値 ({obj.ReturnType}):\r\n";
            } else {
                //戻り値の型が不明な場合
                this.lblReturnType.Text = $"戻り値 (不明):\r\n";
            }

            //戻り値の説明: 句点で改行するやさしさ
            if (!string.IsNullOrEmpty(obj.ReturnComment)) {
                this.lblReturnComment.Text = obj.ReturnComment.Replace("。", "。\r\n");
            } else if (obj.ReturnType == "void") {
                this.lblReturnComment.Text = "戻り値はありません。";
            } else {
                this.lblReturnComment.Text = "説明はありません。";
            }

            //引数欄を作る
            this.SuspendLayout();
            for (var i = 0; i < obj.ArgumentList.Count; i++) {
                var newArg = new CtrlComponent.Text.ctlArgment();
                newArg.Setup(this.argTools, i + 1, obj.ArgumentList[i], (i < obj.ArgComments.Count) ? obj.ArgComments[i] : "", (args != null && i < args.Length) ? args[i] : "");
                newArg.Size = new Size(this.pnlArguments.Width * 90 / 100, newArg.Height);
                this.pnlArguments.Controls.Add(newArg);
            }

            this.ResumeLayout(false);
            this.SetCommandTree(comTree);
        }

        /// <summary>
        /// コンストラクター：直接入力モード
        /// </summary>
        public dlgInsertCommand(CtrlComponent.Text.ctlCommandTree comTree, string summary, string comment, string footer, string defaultText) {
            this.InitializeComponent();
            this.obj = null;

            //与えられたオブジェクトに応じてセットアップする
            this.lblSummary.Text = summary;
            this.lblComment.Text = comment;
            this.lblReturnType.Text = "";
            this.lblReturnComment.Text = footer;

            //単一入力欄を作る
            this.SuspendLayout();
            var newArg = new CtrlComponent.Text.ctlArgment();
            newArg.Setup(this.argTools, -1, "", "", defaultText);
            newArg.Size = new Size(this.pnlArguments.Width * 90 / 100, newArg.Height);
            this.pnlArguments.Controls.Add(newArg);

            this.ResumeLayout(false);
            this.SetCommandTree(comTree);
        }

        /// <summary>
        /// コマンドツリーから引数入力補助メニューの中身を生成する
        /// </summary>
        private void SetCommandTree(CtrlComponent.Text.ctlCommandTree comTree) {
            this.comTree = comTree;
            this.argTools.Items.Clear();

            foreach (TreeNode rootNode in comTree.trvTree.Nodes) {
                var newItem = new ToolStripMenuItem();
                this.SetToolStripMenuItemSettings(newItem, rootNode);
                this.GetSubNodes(newItem, rootNode);
                this.argTools.Items.Add(newItem);
            }
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                if (this.obj == null) {
                    //直接入力: 引数をそのまま返す
                    return (this.pnlArguments.Controls[0] as CtrlComponent.Text.ctlArgment).Result;
                } else {
                    //コマンド挿入: コードに直して返す
                    var buf = this.obj.Name + "(";

                    //引数リスト
                    for (var i = 0; i < this.obj.ArgumentList.Count && i < this.pnlArguments.Controls.Count; i++) {
                        var arg = this.pnlArguments.Controls[i] as CtrlComponent.Text.ctlArgment;

                        if (arg == null) {
                            //不具合発生時は中断する
                            return buf;
                        }

                        buf += arg.Result;
                        if (i < this.obj.ArgumentList.Count - 1) {
                            buf += ", ";
                        }
                    }

                    return buf + ")";
                }
            }
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void ctlOKCancel1_ClickOK(object sender, EventArgs e) {
            if (this.obj == null) {
                //直接入力モードのときは検証しない
                return;
            }

            //引数リストを検証する
            for (var i = 0; i < this.obj.ArgumentList.Count && i < this.pnlArguments.Controls.Count; i++) {
                var arg = this.pnlArguments.Controls[i] as CtrlComponent.Text.ctlArgment;

                if (arg == null) {
                    //不具合発生時は処理しない
                    continue;
                } else if (string.IsNullOrEmpty(arg.Result)) {
                    //（入力を後回しにしたいときもあるので）引数チェックを一切行わないようにする場合は、以下をコメント解除すること
                    //continue;

                    //未入力の引数がある場合は100％構文エラーになるぞ、の警告を出す
                    if (MessageBox.Show(Resources.MsgE_NothingInput.Replace("$", $"第 {i + 1} 引数").Replace("。", "が、続行してもよろしいですか？"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning) != DialogResult.Yes) {
                        this.DialogResult = DialogResult.None;
                        return;
                    } else {
                        break;
                    }

                    //不備がある場合は確定できないようにするときは、以下をコメント解除すること
                    //MessageBox.Show(Resources.MsgE_NothingInput.Replace("$", $"第 {i + 1} 引数"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    //this.DialogResult = DialogResult.None;
                    //return;
                }
            }
        }

        /// <summary>
        /// TreeViewを右クリックメニューに置き換えます。
        /// </summary>
        private void GetSubNodes(ToolStripMenuItem rootItem, TreeNode rootNode) {
            foreach (TreeNode node in rootNode.Nodes) {
                var newItem = new ToolStripMenuItem();
                this.SetToolStripMenuItemSettings(newItem, node);

                if (node.Nodes.Count > 0) {
                    //さらに下のノードも作る
                    this.GetSubNodes(newItem, node);
                }

                rootItem.DropDownItems.Add(newItem);
            }
        }

        /// <summary>
        /// 右クリックメニューのアイテムに既定の設定を施します。
        /// </summary>
        private void SetToolStripMenuItemSettings(ToolStripMenuItem item, TreeNode node) {
            item.Text = node.Text;
            item.ToolTipText = node.ToolTipText;
            item.AutoToolTip = true;
            item.CheckOnClick = true;       //連続的な操作ができるようになる
            item.CheckedChanged += (sender, e) => {
                //連続的な操作ができるようになる代わりにチェックが入ってしまうので、押されても変化が起きないようにする
                (sender as ToolStripMenuItem).Checked = false;
            };
            item.Tag = node;        //右クリックメニューのTagに元となったノード情報を格納する
            item.Click += this.insertCode;
        }

        /// <summary>
        /// このテキストボックスにコードを挿入する
        /// </summary>
        private void insertCode(object sender, EventArgs e) {
            var destTextBox = this.argTools.Tag as TextBox;
            var toolStripMenu = sender as ToolStripMenuItem;
            var node = toolStripMenu?.Tag as TreeNode;

            //無効なノードを選択した場合は無反応とする
            if (destTextBox == null || node == null) {
                return;
            }

            //コードを挿入する
            destTextBox.SelectedText = this.comTree.GetNodeCode(node, out var forbiddenCRLF).Replace("\r\n", "").Replace(";", "");
        }
    }
}
