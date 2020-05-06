namespace Editor.Dialog.Database.Effect {
    public partial class dlgFrameComplete : Editor.Dialog.Database.Effect.dlgFrameFill {
        public dlgFrameComplete() : base() {
            this.InitializeComponent();
            this.Setup();
        }

        public dlgFrameComplete(int frameLength) : base(frameLength) {
            this.InitializeComponent();
            this.Setup();
        }

        public override void Setup() {
            this.Text = "エフェクトフレーム補間";
            this.lblText.Text = "始点から終点にかけて線形的に補間します。";
        }
    }
}
