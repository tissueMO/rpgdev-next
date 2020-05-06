using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;
    using Properties;

    public partial class ctlPosition : UserControl {
        public event EventHandler ValueChanged;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlPosition() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
        }

        /// <summary>
        /// このコントロールで入力できるデータの想定型
        /// </summary>
        public enum InputType {
            Point,
            Size,
        }

        /// <summary>
        /// このコントロールが想定する型
        /// </summary>
        public InputType Type {
            get {
                return this.type;
            }
            set {
                this.type = value;

                //設定された型に合わせて区切り記号を変える
                switch (value) {
                    case InputType.Point:
                        this.lblSplit.Text = Resources.Split_Argument;
                        break;
                    case InputType.Size:
                        this.lblSplit.Text = Resources.Split_Size;
                        break;
                }
            }
        }
        private InputType type = InputType.Point;

        /// <summary>
        /// 座標情報
        /// </summary>
        public Point Result {
            get {
                return new Point((int) this.numX.Value, (int) this.numY.Value);
            }
            set {
                this.numX.Value = value.X;
                this.numY.Value = value.Y;
            }
        }

        /// <summary>
        /// 最大値
        /// </summary>
        public Point Maximum {
            get {
                return new Point((int) this.numX.Maximum, (int) this.numY.Maximum);
            }
            set {
                this.numX.Maximum = value.X;
                this.numY.Maximum = value.Y;
            }
        }

        /// <summary>
        /// 最小値
        /// </summary>
        public Point Minimum {
            get {
                return new Point((int) this.numX.Minimum, (int) this.numY.Minimum);
            }
            set {
                this.numX.Minimum = value.X;
                this.numY.Minimum = value.Y;
            }
        }

        /// <summary>
        /// 値が変更されたことを知らせます。
        /// </summary>
        private void num_ValueChanged(object sender, EventArgs e) {
            this.ValueChanged?.Invoke(this, null);
        }
    }
}
