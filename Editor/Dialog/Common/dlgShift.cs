using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    public partial class dlgShift : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgShift() {
            this.InitializeComponent();
            this.uctlPos.Minimum = new Point(short.MinValue, short.MinValue);
            this.uctlPos.Maximum = new Point(short.MaxValue, short.MaxValue);
        }

        /// <summary>
        /// 座標情報
        /// </summary>
        public Point Result {
            get {
                return this.uctlPos.Result;
            }
        }

        /// <summary>
        /// 最小値と最大値を同時にセットします。
        /// </summary>
        public void SetLimit(Point min, Point max) {
            this.SetMinimum(min);
            this.SetMaximum(max);
        }

        /// <summary>
        /// 最大値をセットします。
        /// </summary>
        public void SetMaximum(Point pos) {
            if (this.uctlPos.Minimum.X < pos.X) {
                this.uctlPos.Maximum = new Point(pos.X, this.uctlPos.Maximum.Y);
            }
            if (this.uctlPos.Minimum.Y < pos.Y) {
                this.uctlPos.Maximum = new Point(this.uctlPos.Maximum.X, pos.Y);
            }
        }

        /// <summary>
        /// 最小値をセットします。
        /// </summary>
        public void SetMinimum(Point pos) {
            if (this.uctlPos.Maximum.X > pos.X) {
                this.uctlPos.Minimum = new Point(pos.X, this.uctlPos.Minimum.Y);
            }
            if (this.uctlPos.Maximum.Y > pos.Y) {
                this.uctlPos.Minimum = new Point(this.uctlPos.Minimum.X, pos.Y);
            }
        }

        /// <summary>
        /// 左へ
        /// </summary>
        private void btnLeft_Click(object sender, EventArgs e) {
            if (this.uctlPos.Minimum.X < this.uctlPos.Result.X) {
                this.uctlPos.Result = new Point(this.uctlPos.Result.X - 1, this.uctlPos.Result.Y);
            }
        }

        /// <summary>
        /// 右へ
        /// </summary>
        private void btnRight_Click(object sender, EventArgs e) {
            if (this.uctlPos.Maximum.X > this.uctlPos.Result.X) {
                this.uctlPos.Result = new Point(this.uctlPos.Result.X + 1, this.uctlPos.Result.Y);
            }
        }

        /// <summary>
        /// 上へ
        /// </summary>
        private void btnUp_Click(object sender, EventArgs e) {
            if (this.uctlPos.Minimum.Y < this.uctlPos.Result.Y) {
                this.uctlPos.Result = new Point(this.uctlPos.Result.X, this.uctlPos.Result.Y - 1);
            }
        }

        /// <summary>
        /// 下へ
        /// </summary>
        private void btnDown_Click(object sender, EventArgs e) {
            if (this.uctlPos.Maximum.Y > this.uctlPos.Result.Y) {
                this.uctlPos.Result = new Point(this.uctlPos.Result.X, this.uctlPos.Result.Y + 1);
            }
        }
    }
}
