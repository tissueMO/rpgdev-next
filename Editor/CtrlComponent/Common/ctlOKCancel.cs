using System;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;

    public partial class ctlOKCancel : UserControl {
        public event EventHandler ClickOK;
        public event EventHandler ClickCancel;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlOKCancel() {
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
        private void ctlOKCancel_Load(object sender, EventArgs e) {
            if (!this.DisenabledAcceptButton) {
                this.ParentForm.AcceptButton = this.btnOK;
            }
            if (!this.DisenabledCancelButton) {
                this.ParentForm.CancelButton = this.btnCancel;
            }
        }

        /// <summary>
        /// OKボタンを押下したときの動作を行います。
        /// </summary>
        public void ApplyOK(object sender, EventArgs e) {
            this.btnOK_Click(null, null);
        }

        private void btnOK_Click(object sender, EventArgs e) {
            this.ParentForm.DialogResult = DialogResult.OK;
            ClickOK?.Invoke(sender, e);
            if (this.ParentForm.DialogResult == DialogResult.OK) {
                this.ParentForm.Close();
            }
        }

        /// <summary>
        /// キャンセルボタンを押下したときの動作を行います。
        /// </summary>
        public void ApplyCancel(object sender, EventArgs e) {
            this.btnCancel_Click(null, null);
        }

        private void btnCancel_Click(object sender, EventArgs e) {
            this.ParentForm.DialogResult = DialogResult.Cancel;
            ClickCancel?.Invoke(sender, e);
            if (this.ParentForm.DialogResult == DialogResult.Cancel) {
                this.ParentForm.Close();
            }
        }

    }
}
