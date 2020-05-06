using System;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;

    public partial class ctlOKCancelApply : UserControl {
        public event EventHandler ClickOK;
        public event EventHandler ClickCancel;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlOKCancelApply() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
        }

        /// <summary>
        /// OKボタンの表示名
        /// </summary>
        public string OKTitle {
            get {
                return this.btnOK.Text;
            }
            set {
                this.btnOK.Text = value;
            }
        }

        /// <summary>
        /// Enterキーでダイアログが閉じないようにするかどうか: これは配置前もしくはデザイナーでのみ設定可能です。
        /// </summary>
        public bool DisenabledAcceptButton {
            get; set;
        } = false;

        /// <summary>
        /// ESCキーでダイアログが閉じないようにするかどうか: これは配置前もしくはデザイナーでのみ設定可能です。
        /// </summary>
        public bool DisenabledCancelButton {
            get; set;
        } = false;

        /// <summary>
        /// コントロールが配置されるときに既定のボタンを設定する
        /// </summary>
        private void ctlOKCancelApply_Load(object sender, EventArgs e) {
            if (!this.DisenabledAcceptButton) {
                this.ParentForm.AcceptButton = this.btnOK;
            }
            if (!this.DisenabledCancelButton) {
                this.ParentForm.CancelButton = this.btnCancel;
            }
        }

        private void btnOK_Click(object sender, EventArgs e) {
            this.ParentForm.DialogResult = DialogResult.OK;
            ClickOK?.Invoke(sender, e);
            if (this.ParentForm.DialogResult == DialogResult.OK) {
                this.ParentForm.Close();
            }
        }

        private void btnCancel_Click(object sender, EventArgs e) {
            this.ParentForm.DialogResult = DialogResult.Cancel;
            ClickCancel?.Invoke(sender, e);
            if (this.ParentForm.DialogResult == DialogResult.Cancel) {
                this.ParentForm.Close();
            }
        }

        private void btnApply_Click(object sender, EventArgs e) {
            ClickOK?.Invoke(sender, e);
        }
    }
}
