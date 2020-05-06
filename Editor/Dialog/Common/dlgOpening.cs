using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;

    public partial class dlgOpening : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgOpening() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.lblAppTitle.Text = Properties.Resources.AppName;
            this.Activate();
        }
    }
}
