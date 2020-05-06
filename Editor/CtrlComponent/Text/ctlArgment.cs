using System;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Text {
    public partial class ctlArgment : UserControl {
        private ContextMenuStrip argTools;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlArgment() {
            this.InitializeComponent();
        }

        /// <summary>
        /// 入力内容
        /// </summary>
        public string Result {
            get {
                return this.txtArgument.Text;
            }
        }

        /// <summary>
        /// コントロールをセットアップ
        /// </summary>
        public void Setup(ContextMenuStrip argTools, int numbering, string argType, string comment, string value = "") {
            this.argTools = argTools;

            //numberingをマイナスにすると [N:] 部分が表記無しにする
            if (numbering < 0) {
                this.lblArgType.Text = "";
            } else {
                this.lblArgType.Text = $"{numbering}: {argType}";
            }

            //引数の説明
            if (!string.IsNullOrEmpty(comment)) {
                this.lblArgComment.Text = comment;
                this.toolHint.SetToolTip(this.lblArgComment, comment.Replace("。", "。\r\n"));
            } else {
                this.lblArgComment.Text = "説明はありません。";
            }

            this.txtArgument.Text = value;
        }


        /// <summary>
        /// 引数入力補助メニューを表示
        /// </summary>
        private void btnTools_Click(object sender, EventArgs e) {
            this.argTools.Tag = this.txtArgument;         //開く前に宛先となるテキストボックスをセットしておく
            this.argTools.Show(MousePosition);
        }
    }
}
