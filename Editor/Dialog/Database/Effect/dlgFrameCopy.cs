using System;
using System.Windows.Forms;

namespace Editor.Dialog.Database.Effect {
    public partial class dlgFrameCopy : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgFrameCopy(int frameLength, int currentFrame) {
            this.InitializeComponent();
            this.numBase.Value = currentFrame;
            this.numDestStart.Maximum = frameLength - 1;
            this.numDestEnd.Maximum = frameLength - 1;
        }

        /// <summary>
        /// 操作結果を返します。
        /// </summary>
        public void GetResult(out int start, out int end) {
            start = (int) this.numDestStart.Value;
            end = (int) this.numDestEnd.Value;
        }

        /// <summary>
        /// 終点が始点の前にならないようにする
        /// </summary>
        private void numDestStart_ValueChanged(object sender, EventArgs e) {
            this.numDestEnd.Minimum = this.numDestStart.Value;
        }
    }
}
