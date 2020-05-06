using System;
using System.Windows.Forms;

namespace Editor.Dialog.Schedule {
    using Editor.CtrlComponent.Schedule;
    using Properties;

    public partial class dlgEditTask : Form {
        private readonly CtrlComponent.Schedule.mgrSchedule.TaskData targetTask;
        private readonly CtrlComponent.Schedule.mgrSchedule mgr;
        private readonly int srcSectionIndex = -1;           //部門リストの既定インデックス

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEditTask(CtrlComponent.Schedule.mgrSchedule src, CtrlComponent.Schedule.mgrSchedule.TaskData task) {
            this.InitializeComponent();

            //ダイアログをセットアップする
            //参照設定
            this.targetTask = task;
            this.mgr = src;

            //部門情報（担当者情報はコントロールの有効無効をチェックする処理の中で設定される）
            this.cmbSection.Items.Clear();
            foreach (var item in this.mgr.Sections) {
                this.cmbSection.Items.Add(item.Name);
                if (item.ID == this.targetTask.SectionID) {
                    //既定選択
                    this.cmbSection.SelectedIndex = this.cmbSection.Items.Count - 1;
                }
            }
            //末尾に追加
            this.cmbSection.Items.Add(Resources.Sche_MilestoneName);

            if (this.cmbSection.SelectedIndex == -1) {
                //既定部門がない場合はマイルストーンを選択
                this.cmbSection.SelectedIndex = this.cmbSection.Items.Count - 1;
            }

            //最初に選択された部門インデックスを記憶する
            this.srcSectionIndex = this.cmbSection.SelectedIndex;

            //依存先情報
            this.cmbLink.Items.Insert(0, Resources.Sche_NoneItem);
            this.cmbLink.SelectedIndex = 0;          //既定は独立タスク
            var hasUnknownSectionOrWorker = false;
            foreach (var item in this.mgr.Tasks) {
                var caption = CtrlComponent.Schedule.mgrSchedule.GetTaskFullName(this.mgr.Sections, item);
                if (string.IsNullOrEmpty(caption)) {
                    hasUnknownSectionOrWorker = true;
                    continue;
                }
                this.cmbLink.Items.Add(caption);
                if (item.TaskID == this.targetTask.DependTaskID) {
                    //既定選択
                    this.cmbLink.SelectedIndex = this.cmbLink.Items.Count - 1;
                }
            }
            if (hasUnknownSectionOrWorker) {
                MessageBox.Show("無効な部門もしくは無効な担当者が設定されているタスクは\r\n依存先タスクのリストから除外されます。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            //その他情報
            this.pnlColor.BackColor = this.targetTask.ViewColor;
            this.txtName.Text = this.targetTask.TaskName;
            if (this.dtmStart.MinDate <= this.targetTask.StartDate && this.targetTask.StartDate <= this.dtmStart.MaxDate) {
                this.dtmStart.Value = this.targetTask.StartDate;
            } else {
                // 依存先が正しくない等の経緯により、開始日時が範囲内に収まっていない場合は現在日時を格納する
                this.dtmStart.Value = DateTime.Today;
                MessageBox.Show("依存先タスクが正しく設定されていません。\r\n依存先は解除され、開始日時は暫定的に今日の日付へ修正されます。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            this.numSpan.Value = this.targetTask.Span;
            this.numProgress.Value = this.targetTask.Progress;

            //コントロールの有効無効、選択部門に対応する担当者情報の更新
            this.cmbSection_SelectedIndexChanged(null, null);
            this.cmbLink_SelectedIndexChanged(null, null);
        }

        /// <summary>
        /// ダイアログの編集結果
        /// </summary>
        public CtrlComponent.Schedule.mgrSchedule.TaskData Result {
            get {
                return this.targetTask;
            }
        }

        /// <summary>
        /// 一意のタスクIDを生成します。
        /// </summary>
        /// <returns>一意のタスクID</returns>
        private int generateTaskID() {
            var ID = -1;
            var NG = false;

            do {
                ID++;
                NG = false;
                foreach (var item in this.mgr.Tasks) {
                    if (item.TaskID == ID) {
                        NG = true;
                        break;
                    }
                }
            } while (NG);

            return ID;
        }

        /// <summary>
        /// 入力内容を検証
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            //無名チェック
            if (string.IsNullOrEmpty(this.txtName.Text)) {
                MessageBox.Show(Resources.MsgE_NothingInput.Replace("$", "タスク名"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            } else {
                //Resultを作る
                var newSecID = -1;          //既定は マイルストーン
                var newWorkerID = -1;       //既定は 全員
                var newDtaskID = -1;        //既定は 独立タスク

                //選択項目から部門ID、担当者IDを得る
                if (this.cmbSection.SelectedIndex != this.cmbSection.Items.Count - 1) {
                    newSecID = this.mgr.Sections[this.cmbSection.SelectedIndex].ID;
                    if (this.cmbWorker.SelectedIndex == 0) {
                        //部門全体
                        newWorkerID = -1;
                    } else {
                        newWorkerID = this.mgr.Sections[this.cmbSection.SelectedIndex].Workers[this.cmbWorker.SelectedIndex - 1].ID;
                    }
                }

                //同一部門、同一担当者に同名のタスクが存在しないか検査する
                foreach (var item in this.mgr.Tasks) {
                    if (newSecID == item.SectionID && newWorkerID == item.WorkerID) {
                        if (item.TaskName == this.txtName.Text && item != this.targetTask) {
                            MessageBox.Show(Resources.MsgE_ForbidOverlap.Replace("$", "同一部門もしくは同一担当者の中でタスク名"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                            this.DialogResult = DialogResult.None;
                            return;
                        }
                    }
                }

                //自分自身を依存先にしていないか検査する
                if (this.cmbLink.SelectedIndex != 0) {
                    newDtaskID = this.mgr.Tasks[this.cmbLink.SelectedIndex - 1].TaskID;
                    if (newDtaskID == this.targetTask.TaskID) {
                        MessageBox.Show("依存先が循環参照になっています。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        this.DialogResult = DialogResult.None;
                        return;
                    }
                }

                //既にある同じ担当者のタスクと日程がバッティングしないか検査する
                if (this.mgr.CheckOverlapedTask(newSecID, newWorkerID, this.targetTask.TaskID, newDtaskID, this.dtmStart.Value.Date, (int) this.numSpan.Value)) {
                    if (MessageBox.Show("同一担当者の中で日程が重複するタスクがあります。\r\nこのまま登録してもよろしいですか？", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                        this.DialogResult = DialogResult.None;
                        return;
                    }
                }

                //プロジェクト全体の期間が最大値を超えないかどうかを検証
                if (!this.mgr.CheckTaskInProjectMaxPeriod(new mgrSchedule.TaskData() {
                    TaskID = this.targetTask.TaskID,
                    StartDate = this.dtmStart.Value,
                    Span = (int) this.numSpan.Value,
                    DependTaskID = newDtaskID,
                })) {
                    MessageBox.Show($"プロジェクト全体の期間が長すぎます。\r\nプロジェクトのスケジュールは全体で最大{mgrSchedule.MaxPeriodDays}日までに収めて下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.DialogResult = DialogResult.None;
                    return;
                }

                //チェックが完了してから、targetTaskを変更する
                if (this.targetTask.TaskID == -1) {
                    //IDが未設定（新規作成）の場合は生成する
                    this.targetTask.TaskID = this.generateTaskID();
                }

                this.targetTask.SectionID = newSecID;
                this.targetTask.WorkerID = newWorkerID;
                this.targetTask.DependTaskID = newDtaskID;
                this.targetTask.TaskName = this.txtName.Text;
                this.targetTask.StartDate = this.dtmStart.Value;
                this.targetTask.Span = (int) this.numSpan.Value;
                this.targetTask.ViewColor = this.pnlColor.BackColor;
                this.targetTask.Progress = (int) this.numProgress.Value;
            }
        }

        /// <summary>
        /// 部門変更: コントロールの有効無効を含む
        /// </summary>
        private void cmbSection_SelectedIndexChanged(object sender, EventArgs e) {
            this.cmbWorker.Enabled = (this.cmbSection.SelectedIndex != this.cmbSection.Items.Count - 1);
            this.cmbLink.Enabled = this.cmbWorker.Enabled;
            this.numProgress.Enabled = this.cmbWorker.Enabled;

            //担当者リストを更新：マイルストーンのときは無効
            this.cmbWorker.Items.Clear();
            this.cmbWorker.Items.Add(Resources.Sche_NoneItem);
            if (this.cmbWorker.Enabled) {
                foreach (var item in this.mgr.Sections[this.cmbSection.SelectedIndex].Workers) {
                    this.cmbWorker.Items.Add(item.Name);
                    if (item.ID == this.targetTask.WorkerID && this.srcSectionIndex == this.cmbSection.SelectedIndex) {
                        //既定選択
                        this.cmbWorker.SelectedIndex = this.cmbWorker.Items.Count - 1;
                    }
                }
            }
            if (this.cmbWorker.SelectedIndex == -1) {
                //既定選択
                this.cmbWorker.SelectedIndex = 0;
            }
        }

        /// <summary>
        /// 依存先変更
        /// </summary>
        private void cmbLink_SelectedIndexChanged(object sender, EventArgs e) {
            this.dtmStart.Enabled = (this.cmbLink.SelectedIndex == 0);
        }

        /// <summary>
        /// 色変更
        /// </summary>
        private void pnlColor_MouseClick(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                this.dlgColor.Color = this.pnlColor.BackColor;
                if (this.dlgColor.ShowDialog(this) == DialogResult.OK) {
                    this.pnlColor.BackColor = this.dlgColor.Color;
                }
            }
        }
    }
}
