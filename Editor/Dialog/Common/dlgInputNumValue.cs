using System.Windows.Forms;

namespace Editor.Dialog.Common {

    public partial class dlgInputNumValue : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        /// <param name="title">ダイアログのタイトル</param>
        public dlgInputNumValue(int min, int max, int defaultval, string title = "") {
            this.InitializeComponent();

            //初期設定
            this.numInput.Minimum = min;
            this.numInput.Maximum = max;
            if (min <= defaultval && defaultval <= max) {
                this.numInput.Value = defaultval;
            }
            if (!string.IsNullOrEmpty(title)) {
                this.Text = title;
            }
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public int Result {
            get {
                return (int) this.numInput.Value;
            }
        }
    }
}
