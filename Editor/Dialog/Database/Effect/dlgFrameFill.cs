using System;
using System.Windows.Forms;

namespace Editor.Dialog.Database.Effect {
    public partial class dlgFrameFill : Form {
        /// <summary>
        /// デザイナー用のコンストラクター
        /// </summary>
        public dlgFrameFill() {
            this.InitializeComponent();
            this.Setup();
        }

        /// <summary>
        /// 実際に呼び出されるコンストラクター
        /// </summary>
        public dlgFrameFill(int frameLength) {
            this.InitializeComponent();
            this.numStart.Maximum = frameLength - 1;
            this.numEnd.Maximum = frameLength - 1;
            this.Setup();
        }

        /// <summary>
        /// 初期設定を行います。
        /// </summary>
        public virtual void Setup() {
            this.Text = "エフェクトフレーム一括";
            this.lblText.Text = "始点フレームの設定が指定範囲に適用されます。";
        }

        /// <summary>
        /// 操作結果を返します。
        /// </summary>
        public void GetResult(out int start, out int end, out CtrlComponent.Database.mgrDBEffect.EffectOneData.EffectLayer.EffectFrame.EffectFrameActionEnable param) {
            start = (int) this.numStart.Value;
            end = (int) this.numEnd.Value;

            param = new CtrlComponent.Database.mgrDBEffect.EffectOneData.EffectLayer.EffectFrame.EffectFrameActionEnable {
                ViewPosition = this.chkViewPos.Checked,
                SectionPosition = this.chkSectionPos.Checked,
                Transparent = this.chkTrans.Checked,
                Extend = this.chkExt.Checked,
                Angle = this.chkAngle.Checked,
                Blend = this.chkBlendMode.Checked
            };
        }

        /// <summary>
        /// 終点が始点の前にならないようにする
        /// </summary>
        private void numStart_ValueChanged(object sender, EventArgs e) {
            this.numEnd.Minimum = this.numStart.Value;
        }
    }
}
