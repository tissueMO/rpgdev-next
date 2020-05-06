using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Schedule {
    using Module;
    using Properties;

    /// <summary>
    /// メインクラスの定義
    /// </summary>
    public partial class ctlScheduleEditor : CtrlComponent.Common.ctlEditorParent {
        private readonly mgrSchedule mgr = new mgrSchedule();
        private ScheduleDrawComponent mgrDraw;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlScheduleEditor() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.tolSortType.SelectedIndex = Settings.Default.Form_ScheduleSortType;
        }

        /// <summary>
        /// タスクリストのソート方法
        /// </summary>
        public enum TaskSortType {
            Group,          //部門 -> 担当者 -> 開始日 の順
            DateTime,       //開始日順
        }

        /// <summary>
        /// 変更されたかどうか
        /// </summary>
        public override bool IsDirty {
            get;
            protected set;
        }

        /// <summary>
        /// エディタータブを表示するかどうか
        /// </summary>
        public bool EnabledEditMode {
            get {
                return this.enabledEditMode;
            }
            set {
                this.enabledEditMode = value;
                this.tbpEditor.Text = "エディター";

                //コントロール有効無効
                this.tolSave.Enabled = this.enabledEditMode;
                this.tolSection.Enabled = this.enabledEditMode;
                this.tolAddTask.Enabled = this.enabledEditMode;
                this.tolDeleteTask.Enabled = this.enabledEditMode;
            }
        }
        private bool enabledEditMode = true;

        /// <summary>
        /// 現在のタスクリストを画面に反映します。
        /// </summary>
        private void ApplyToForm() {
            this.ltvData.BeginUpdate();
            this.ltvData.SelectedIndices.Clear();
            this.ltvData.Items.Clear();
            this.mgr.SortTaskList();     //反映前に並び順を整理する

            foreach (var item in this.mgr.Tasks) {
                var add = new ListViewItem {
                    UseItemStyleForSubItems = false        //統一フォント設定を解除し、サブアイテム個別のフォント設定を有効にする
                };

                //部門名と担当者名を得る
                if (item.SectionID == -1) {
                    add.Text = Resources.Sche_MilestoneName;
                    add.SubItems.Add(Resources.Sche_SectionAllWorkers);
                } else {
                    add.Text = "不明な部門";
                    add.ForeColor = Color.Red;
                    add.SubItems.Add("不明な担当者");
                    add.SubItems[(int) mgrSchedule.TaskListColumns.WorkerName].ForeColor = Color.Red;

                    //部門を検索する
                    foreach (var sub in this.mgr.Sections) {
                        if (sub.ID != item.SectionID) {
                            //部門IDが一致しない場合はスルー
                            continue;
                        }

                        add.Text = sub.Name;
                        add.ForeColor = Color.Black;

                        if (item.WorkerID == -1) {
                            //担当者が全員のとき
                            add.SubItems[(int) mgrSchedule.TaskListColumns.WorkerName].Text = Resources.Sche_SectionAllWorkers;
                            add.SubItems[(int) mgrSchedule.TaskListColumns.WorkerName].ForeColor = Color.Black;
                        } else {
                            //担当者を検索する
                            foreach (var sub2 in sub.Workers) {
                                if (sub2.ID != item.WorkerID) {
                                    //担当者IDが一致しない場合はスルー
                                    continue;
                                }

                                add.SubItems[(int) mgrSchedule.TaskListColumns.WorkerName].Text = sub2.Name;
                                add.SubItems[(int) mgrSchedule.TaskListColumns.WorkerName].ForeColor = Color.Black;
                                break;
                            }
                        }
                        break;
                    }
                }
                add.SubItems.Add(item.TaskName);

                //依存タスクである場合は依存先を示す
                if (item.DependTaskID != -1) {
                    add.SubItems.Add("不明な依存先");
                    add.SubItems[(int) mgrSchedule.TaskListColumns.StartDate].ForeColor = Color.Red;

                    foreach (var sub in this.mgr.Tasks) {
                        if (item.DependTaskID == sub.TaskID) {
                            add.SubItems[(int) mgrSchedule.TaskListColumns.StartDate].Text = mgrSchedule.GetTaskFullName(this.mgr.Sections, sub);
                            add.SubItems[(int) mgrSchedule.TaskListColumns.StartDate].ForeColor = Color.Black;
                            break;
                        }
                    }
                } else {
                    add.SubItems.Add(item.StartDate.ToShortDateString());
                }

                add.SubItems.Add(item.Span.ToString() + " 日間");
                if (item.SectionID != -1) {
                    if (!item.NotApplyedProgress) {
                        add.SubItems.Add(item.Progress.ToString() + " %");
                    } else {
                        add.SubItems.Add("* " + item.Progress.ToString() + " %");
                    }
                } else {
                    add.SubItems.Add("-----");
                }
                add.SubItems.Add("", Color.Black, item.ViewColor, this.Font);

                this.ltvData.Items.Add(add);
            }

            this.ltvData.EndUpdate();
            this.DrawSchedule();
        }

        /// <summary>
        /// 読み込み
        /// </summary>
        public void DoLoad() {
            this.mgr.Sections.Clear();
            this.mgr.Tasks.Clear();

            FileStream fs;
            try {
                //読み取り専用で開く
                fs = Common.OpenFileReadOnly(ProjectManager.ProjectPath + Resources.Path_PrjSys_ScheduleFile);
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "スケジュール情報の読み込み"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            using (var R = new StreamReader(fs, Common.SJIS)) {
                if (R.EndOfStream) {
                    //ファイルが空の場合は処理しない
                    goto AfterLoad;
                }

                //一行目は部門数
                var secCount = int.Parse(R.ReadLine());

                //部門情報
                string buf;
                for (var i = 0; i < secCount; i++) {
                    //部門情報読み込み
                    var newSec = new mgrSchedule.SectionData();
                    buf = R.ReadLine();
                    Common.SetIDName(buf, out newSec.ID, out newSec.Name);
                    newSec.Workers = new List<mgrSchedule.SectionData.WorkerData>();

                    //担当者情報読み込み
                    buf = R.ReadLine();
                    var split2 = buf.Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                    if (split2 != null) {
                        foreach (var sub in split2) {
                            var newWorker = new mgrSchedule.SectionData.WorkerData();
                            Common.SetIDName(sub, out newWorker.ID, out newWorker.Name);
                            newSec.Workers.Add(newWorker);
                        }
                    }
                    this.mgr.Sections.Add(newSec);
                }

                //タスク情報
                while (!R.EndOfStream) {
                    buf = R.ReadLine();
                    var split = buf.Split('\t');
                    var newTask = new mgrSchedule.TaskData {
                        TaskID = int.Parse(split[(int) mgrSchedule.TaskDataIndices.TaskID]),
                        TaskName = split[(int) mgrSchedule.TaskDataIndices.TaskName],
                        SectionID = int.Parse(split[(int) mgrSchedule.TaskDataIndices.SectionID]),
                        WorkerID = int.Parse(split[(int) mgrSchedule.TaskDataIndices.WorkerID]),
                        DependTaskID = int.Parse(split[(int) mgrSchedule.TaskDataIndices.DependTaskID]),
                        StartDate = DateTime.ParseExact(split[(int) mgrSchedule.TaskDataIndices.StartDate], Common.DateFormat, null),
                        Span = int.Parse(split[(int) mgrSchedule.TaskDataIndices.Span]),
                        Progress = int.Parse(split[(int) mgrSchedule.TaskDataIndices.Progress]),
                        ViewColor = Common.StringToColor(split[(int) mgrSchedule.TaskDataIndices.Color])
                    };

                    this.mgr.Tasks.Add(newTask);
                }
            }

            AfterLoad:
            this.ApplyToForm();     //mgrをltvに反映
        }

        public override bool DoSave() {
            if (!this.IsDirty) {
                //変更がないときは保存しない
                return false;
            }

            using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + Resources.Path_PrjSys_ScheduleFile), Common.SJIS)) {
                //セクション情報
                W.WriteLine(this.mgr.Sections.Count);        //先頭に部門数を入れておく

                foreach (var sec in this.mgr.Sections) {
                    //部門情報が先行
                    W.WriteLine(sec.ID + Resources.Split_IDNamePair + sec.Name);

                    //部門内の担当者情報を次の行に書き込む
                    var buf = "";
                    foreach (var worker in sec.Workers) {
                        buf += worker.ID + Resources.Split_IDNamePair + worker.Name + "\t";
                    }
                    W.WriteLine(buf);
                }

                //タスク情報
                foreach (var task in this.mgr.Tasks) {
                    var buf = "";

                    for (var i = 0; i < Common.GetEnumCount<mgrSchedule.TaskDataIndices>(); i++) {
                        switch (i) {
                            case (int) mgrSchedule.TaskDataIndices.TaskID:
                                buf += task.TaskID + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.TaskName:
                                buf += task.TaskName + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.SectionID:
                                buf += task.SectionID + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.WorkerID:
                                buf += task.WorkerID + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.DependTaskID:
                                buf += task.DependTaskID + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.StartDate:
                                buf += task.StartDate.ToString(Common.DateFormat) + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.Span:
                                buf += task.Span.ToString() + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.Progress:
                                buf += task.Progress.ToString() + "\t";
                                break;
                            case (int) mgrSchedule.TaskDataIndices.Color:
                                buf += Common.ColorToString(task.ViewColor);
                                break;
                        }
                    }
                    W.WriteLine(buf);
                }
            }

            this.IsDirty = false;
            return true;
        }

        public override bool Jump(string rPath) {
            if (rPath == Resources.Path_PrjSys_ScheduleFile) {
                this.tbcScheduleTabs.SelectedIndex = 0;
                return true;
            }
            return false;
        }

        public override bool Conflict(string rPath, ProjectManager project) {
            return true;
        }

        /// <summary>
        /// 描画イメージを保存
        /// </summary>
        private void tolSaveImage_Click(object sender, EventArgs e) {
            if (this.dlgSave.ShowDialog(this) != DialogResult.OK) {
                return;
            }

            //再描画イベントの発生を待つとイメージを保存できなくなるため、ここで手動描画する
            this.DrawSchedule(true);        //キャンバスサイズ確定
            this.drawViewerTitle(this.picTitle.Image, Graphics.FromImage(this.picTitle.Image));
            this.drawViewerDate(this.picDate.Image, Graphics.FromImage(this.picDate.Image));
            this.drawViewerSection(this.picSection.Image, Graphics.FromImage(this.picSection.Image));
            this.drawViewerTask(this.picTask.Image, Graphics.FromImage(this.picTask.Image));

            //出力用イメージへ描画
            var img = new Bitmap(this.picTitle.Image.Width + this.picDate.Image.Width, this.picTitle.Image.Height + this.picSection.Image.Height);
            var g = Graphics.FromImage(img);
            g.DrawImage(this.picTitle.Image, 0, 0);
            g.DrawImage(this.picDate.Image, this.picTitle.Image.Width, 0);
            g.DrawImage(this.picSection.Image, 0, this.picTitle.Image.Height);
            g.DrawImage(this.picTask.Image, this.picTitle.Image.Width, this.picTitle.Image.Height);
            img.Save(this.dlgSave.FileName);

            MessageBox.Show("スケジュールのイメージを保存しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            g.Dispose();
            img.Dispose();
        }

        /// <summary>
        /// 実際にガントチャートを描画します。
        /// </summary>
        /// <param name="Reset">スクロール設定も含めてすべてリセットするかどうか</param>
        private void DrawSchedule(bool skipRedraw = false) {
            this.mgrDraw = new ScheduleDrawComponent(this.mgr);

            //描画先のキャンバスを生成
            var bmpTitle = new Bitmap(this.pnlSection.Width, ScheduleDrawComponent.DrawHeaderHeight);
            var bmpDate = new Bitmap((this.mgrDraw.WidthCount + 1) * ScheduleDrawComponent.DrawDayWidth + ScheduleDrawComponent.DrawRightMargin, ScheduleDrawComponent.DrawHeaderHeight);
            var bmpSec = new Bitmap(this.pnlSection.Width, this.mgrDraw.HeightCount * ScheduleDrawComponent.DrawLineHeight + 1);
            var bmpTask = new Bitmap((this.mgrDraw.WidthCount + 1) * ScheduleDrawComponent.DrawDayWidth + ScheduleDrawComponent.DrawRightMargin, this.mgrDraw.HeightCount * ScheduleDrawComponent.DrawLineHeight + 1);

            //リサイズしたらキャンバスを更新
            Image temp;
            temp = this.picTitle.Image;
            if (this.picTitle.Image == null || this.picTitle.Image.Size != bmpTitle.Size) {
                this.picTitle.Image = bmpTitle;
                temp?.Dispose();
            }

            temp = this.picDate.Image;
            if (this.picDate.Image == null || this.picDate.Image.Size != bmpDate.Size) {
                this.picDate.Image = bmpDate;
                temp?.Dispose();
            }

            temp = this.picSection.Image;
            if (this.picSection.Image == null || this.picSection.Image.Size != bmpSec.Size) {
                this.picSection.Image = bmpSec;
                temp?.Dispose();
            }

            temp = this.picTask.Image;
            if (this.picTask.Image == null || this.picTask.Image.Size != bmpTask.Size) {
                this.picTask.Image = bmpTask;
                this.pnlTask.AutoScrollPosition = Point.Empty;
                this.picDate.Left = 0;
                this.picSection.Top = 0;
                temp?.Dispose();
            }

            //再描画を指示
            if (!skipRedraw) {
                this.picTitle.Refresh();
                this.picDate.Refresh();
                this.picSection.Refresh();
                this.picTask.Refresh();
            }
        }

        /// <summary>
        /// 保存を指示
        /// </summary>
        private void tolSave_Click(object sender, EventArgs e) {
            if (this.DoSave()) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// セクション編集
        /// </summary>
        private void tolSection_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Schedule.dlgEditSection(this.mgr.Sections);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.IsDirty = true;
                this.ApplyToForm();
            }
        }

        /// <summary>
        /// タスクを追加
        /// </summary>
        private void tolAddTask_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Schedule.dlgEditTask(this.mgr, new mgrSchedule.TaskData());
            Dlg.Text = Dlg.Text.Replace("編集", "追加");
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.IsDirty = true;
                this.mgr.Tasks.Add(Dlg.Result);
                this.ApplyToForm();
            }
        }

        /// <summary>
        /// タスクを削除
        /// </summary>
        private void tolDeleteTask_Click(object sender, EventArgs e) {
            if (this.ltvData.SelectedIndices.Count == 0
            || MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "選択されたタスクを削除"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) != DialogResult.Yes) {
                return;
            }

            //選択項目すべてを削除
            for (var i = this.ltvData.SelectedIndices.Count - 1; i >= 0; i--) {
                this.mgr.Tasks.RemoveAt(this.ltvData.SelectedIndices[i]);
            }
            this.IsDirty = true;
            this.ApplyToForm();
            this.ltvData_SelectedIndexChanged(null, null);      //再度選択状況を反映する
        }

        /// <summary>
        /// タスクを編集
        /// </summary>
        private void ltvData_DoubleClick(object sender, EventArgs e) {
            if (this.ltvData.SelectedIndices.Count > 0) {
                var Dlg = new Dialog.Schedule.dlgEditTask(this.mgr, this.mgr.Tasks[this.ltvData.SelectedIndices[0]]);
                if (Dlg.ShowDialog() == DialogResult.OK) {
                    //ダイアログの中で現在のタスク情報が変更されている
                    this.IsDirty = true;
                    this.ApplyToForm();     //mgrをltvに反映
                }
            }
        }

        /// <summary>
        /// 項目選択
        /// </summary>
        private void ltvData_SelectedIndexChanged(object sender, EventArgs e) {
            var isEnabled = (this.ltvData.SelectedIndices.Count > 0);
            this.tolDeleteTask.Enabled = isEnabled;
        }

        /// <summary>
        /// ソート方法変更
        /// </summary>
        private void tolSortType_SelectedIndexChanged(object sender, EventArgs e) {
            Settings.Default.Form_ScheduleSortType = this.tolSortType.SelectedIndex;
            this.mgr.TaskSortType = (TaskSortType) this.tolSortType.SelectedIndex;
            this.ApplyToForm();
        }

        /// <summary>
        /// タスク部分のスクロールに合わせてカレンダー・セクション部分も一緒に動かす
        /// </summary>
        private void pnlTask_Paint(object sender, PaintEventArgs e) {
            this.pnlTask_Scroll(sender, null);
        }
        private void pnlTask_Scroll(object sender, ScrollEventArgs e) {
            //実際にはスクロールではなく、左上座標を動かしている
            this.picDate.Left = this.pnlTask.AutoScrollPosition.X;
            this.picSection.Top = this.pnlTask.AutoScrollPosition.Y;
        }

        /// <summary>
        /// タイトル部分の再描画
        /// </summary>
        private void picTitle_Paint(object sender, PaintEventArgs e) {
            this.drawViewerTitle(this.picTitle.Image, e.Graphics);
        }

        /// <summary>
        /// タイトル部分の描画処理
        /// </summary>
        private void drawViewerTitle(Image img, Graphics g) {
            if (img == null) {
                //描画領域が存在しない場合は処理しない
                return;
            }

            this.mgrDraw = new ScheduleDrawComponent(this.mgr, g, img.Size);

            //マイルストーン
            g.DrawString(
                Resources.Sche_MilestoneName,
                this.Font,
                Brushes.Black,
                0,
                ScheduleDrawComponent.DrawHeaderHeight + (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * -1 + ScheduleDrawComponent.OffsetHeaderLine - 1
            );

            //年月日凡例
            g.DrawString("年", this.Font, Brushes.Black, img.Width - 25, (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 0 + 1);
            g.DrawString("月", this.Font, Brushes.Black, img.Width - 25, (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 1 + 1);
            g.DrawString("日", this.Font, Brushes.Black, img.Width - 25, (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 2 + 1);

            //横罫線
            //年
            g.DrawLine(
                Pens.Silver,
                0,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 1 - 2,
                img.Width - 1,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 1 - 2
            );

            //月
            g.DrawLine(
                Pens.Silver,
                0,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 2 - 2,
                img.Width - 1,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 2 - 2
            );

            //日
            g.DrawLine(
                ScheduleDrawComponent.TaskFramePen,
                0,
                ScheduleDrawComponent.DrawHeaderHeight - (ScheduleDrawComponent.DrawLineHeight - ScheduleDrawComponent.OffsetHeaderLine) - 2,
                img.Width - 2,
                ScheduleDrawComponent.DrawHeaderHeight - (ScheduleDrawComponent.DrawLineHeight - ScheduleDrawComponent.OffsetHeaderLine) - 2
            );

            //マイルストーン
            g.DrawLine(
                ScheduleDrawComponent.TaskFramePen,
                0,
                ScheduleDrawComponent.DrawHeaderHeight,
                img.Width - ScheduleDrawComponent.DrawRightMargin, ScheduleDrawComponent.DrawHeaderHeight
            );
        }

        /// <summary>
        /// カレンダー部分の再描画
        /// </summary>
        private void picDate_Paint(object sender, PaintEventArgs e) {
            this.drawViewerDate(this.picDate.Image, e.Graphics);
        }

        /// <summary>
        /// カレンダー部分の描画処理
        /// </summary>
        private void drawViewerDate(Image img, Graphics g) {
            if (img == null) {
                //描画領域が存在しない場合は処理しない
                return;
            }

            this.mgrDraw = new ScheduleDrawComponent(this.mgr, g, img.Size);

            //マイルストーン
            foreach (var item in this.mgr.Tasks) {
                if (item.SectionID == -1) {
                    g.FillRectangle(
                        new SolidBrush(Color.FromArgb(ScheduleDrawComponent.DayTrans, item.ViewColor)),
                        (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                        ScheduleDrawComponent.DrawHeaderHeight - ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine - 2,
                        item.Span * ScheduleDrawComponent.DrawDayWidth, this.picTask.Image.Height
                    );
                    g.DrawRectangle(
                        ScheduleDrawComponent.TaskFramePen,
                        (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                        ScheduleDrawComponent.DrawHeaderHeight - ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine - 2,
                        item.Span * ScheduleDrawComponent.DrawDayWidth, this.picTask.Image.Height
                    );
                    g.DrawString(
                        item.TaskName,
                        ScheduleDrawComponent.TaskFont,
                        Brushes.Black,
                        (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth + ScheduleDrawComponent.OffsetSection,
                        ScheduleDrawComponent.DrawHeaderHeight - ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine + ScheduleDrawComponent.OffsetSection
                    );
                }
            }

            //ヘッダー部
            for (var i = 0; i <= this.mgrDraw.WidthCount; i++) {
                var currentDate = this.mgrDraw.StartDate.AddDays(i);

                //本日ハイライト
                if (currentDate == DateTime.Today) {
                    g.FillRectangle(
                        Brushes.Yellow,
                        i * ScheduleDrawComponent.DrawDayWidth,
                        2,
                        ScheduleDrawComponent.DrawDayWidth,
                        (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 3
                    );
                }

                if (i == 0 || currentDate.Day == 1) {
                    //月の初日に年を描画する
                    g.DrawString(
                        currentDate.Year.ToString(),
                        this.Font,
                        Brushes.Black,
                        i * ScheduleDrawComponent.DrawDayWidth,
                        (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 0
                    );
                }

                if (i == 0 || currentDate.Day == 1 || currentDate.Day == 15) {
                    //月の初日と15日に月を描画する
                    g.DrawString(
                        $"{currentDate.Month,2}",
                        this.Font,
                        Brushes.Black,
                        ScheduleDrawComponent.OffsetHeaderDay + i * ScheduleDrawComponent.DrawDayWidth,
                        (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 1
                    );
                }

                //日付は曜日で色分けする
                g.DrawString(
                    $"{currentDate.Day,2}",
                    this.Font,
                    (currentDate.DayOfWeek == DayOfWeek.Sunday) ? Brushes.Red : (currentDate.DayOfWeek == DayOfWeek.Saturday) ? Brushes.Blue : Brushes.Black,
                    ScheduleDrawComponent.OffsetHeaderDay + i * ScheduleDrawComponent.DrawDayWidth,
                    (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 2 + 1
                );
            }

            //縦罫線
            for (var i = 1; i <= this.mgrDraw.WidthCount + 1; i++) {
                g.DrawLine(
                    ScheduleDrawComponent.TransLinePen,
                    i * ScheduleDrawComponent.DrawDayWidth,
                    0,
                    i * ScheduleDrawComponent.DrawDayWidth,
                    img.Height
                );
            }

            //横罫線
            //年
            g.DrawLine(
                Pens.Silver,
                0,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 1 - 2,
                img.Width - ScheduleDrawComponent.DrawRightMargin,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 1 - 2
            );

            //月
            g.DrawLine(
                Pens.Silver,
                0,
                (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 2 - 2,
                img.Width - ScheduleDrawComponent.DrawRightMargin, (ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetHeaderLine) * 2 - 2
            );

            //日
            g.DrawLine(
                ScheduleDrawComponent.TaskFramePen,
                0,
                ScheduleDrawComponent.DrawHeaderHeight - (ScheduleDrawComponent.DrawLineHeight - ScheduleDrawComponent.OffsetHeaderLine) - 2,
                img.Width - ScheduleDrawComponent.DrawRightMargin,
                ScheduleDrawComponent.DrawHeaderHeight - (ScheduleDrawComponent.DrawLineHeight - ScheduleDrawComponent.OffsetHeaderLine) - 2
            );

            //マイルストーン
            g.DrawLine(
                ScheduleDrawComponent.TaskFramePen,
                0,
                ScheduleDrawComponent.DrawHeaderHeight,
                img.Width - ScheduleDrawComponent.DrawRightMargin,
                ScheduleDrawComponent.DrawHeaderHeight
            );
        }

        /// <summary>
        /// セクション部分の再描画
        /// </summary>
        private void picSection_Paint(object sender, PaintEventArgs e) {
            this.drawViewerSection(this.picSection.Image, e.Graphics);
        }

        /// <summary>
        /// セクション部分の描画処理
        /// </summary>
        private void drawViewerSection(Image img, Graphics g) {
            if (img == null) {
                //描画領域が存在しない場合は処理しない
                return;
            }

            this.mgrDraw = new ScheduleDrawComponent(this.mgr, g, img.Size);

            //個別タスク
            for (int i = 0, cnt = 0; i < this.mgr.Sections.Count; i++, cnt++) {
                //部門名
                g.DrawString(
                    this.mgr.Sections[i].Name,
                    ScheduleDrawComponent.TaskFont,
                    Brushes.Black,
                    5,
                    ScheduleDrawComponent.DrawLineHeight * cnt + ScheduleDrawComponent.OffsetSection
                );

                //担当者とそのタスク
                for (var j = -1/*部門全体から始める*/; j < this.mgr.Sections[i].Workers.Count; j++) {
                    if (j != -1) {
                        cnt++;
                        g.DrawString(
                            this.mgr.Sections[i].Workers[j].Name,
                            this.Font,
                            Brushes.Black,
                            30,
                            ScheduleDrawComponent.DrawLineHeight * cnt + ScheduleDrawComponent.OffsetSection
                        );
                    }
                }
            }

            //縦罫線
            g.DrawLine(
                Pens.Black,
                img.Width - 1,
                0,
                img.Width - 1,
                img.Height
            );

            //横罫線
            for (var i = 1; i <= this.mgrDraw.HeightCount; i++) {
                g.DrawLine(
                    ScheduleDrawComponent.TransLinePen,
                    0,
                    i * ScheduleDrawComponent.DrawLineHeight,
                    img.Width,
                    i * ScheduleDrawComponent.DrawLineHeight
                );
            }
        }

        /// <summary>
        /// 日程部分の再描画
        /// </summary>
        private void picTask_Paint(object sender, PaintEventArgs e) {
            this.drawViewerTask(this.picTask.Image, e.Graphics);
        }

        /// <summary>
        /// 日程部分の描画処理
        /// </summary>
        private void drawViewerTask(Image img, Graphics g) {
            if (img == null) {
                //描画領域が存在しない場合は処理しない
                return;
            }

            this.mgrDraw = new ScheduleDrawComponent(this.mgr, g, img.Size);

            //マイルストーン
            foreach (var item in this.mgr.Tasks) {
                if (item.SectionID == -1) {
                    g.FillRectangle(
                        new SolidBrush(Color.FromArgb(ScheduleDrawComponent.DayTrans, item.ViewColor)),
                        (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                        0,
                        item.Span * ScheduleDrawComponent.DrawDayWidth,
                        img.Height - 1
                    );
                    g.DrawRectangle(
                        ScheduleDrawComponent.TaskFramePen,
                        (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                        0,
                        item.Span * ScheduleDrawComponent.DrawDayWidth,
                        img.Height - 1
                    );
                }
            }

            //個別タスク
            var cnt = 0;
            for (var sec = 0; sec < this.mgr.Sections.Count; sec++) {
                for (var worker = -1/*部門全体から始める*/; worker < this.mgr.Sections[sec].Workers.Count; worker++) {
                    if (worker != -1) {
                        cnt++;
                    }
                    //該当タスクがあれば描画する
                    foreach (var item in this.mgr.Tasks) {
                        if ((worker == -1 && item.SectionID == this.mgr.Sections[sec].ID && item.WorkerID == -1)
                        || (worker >= 0 && item.SectionID == this.mgr.Sections[sec].ID && item.WorkerID == this.mgr.Sections[sec].Workers[worker].ID)) {
                            if (item.DependTaskID == -1) {
                                //独立タスクは通常通り描画
                                g.FillRectangle(
                                    new SolidBrush(Color.FromArgb(ScheduleDrawComponent.DayTrans, item.ViewColor)),
                                    (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                                    cnt * ScheduleDrawComponent.DrawLineHeight,
                                    item.Span * ScheduleDrawComponent.DrawDayWidth,
                                    ScheduleDrawComponent.DrawLineHeight
                                );
                                g.DrawRectangle(
                                    ScheduleDrawComponent.TaskFramePen,
                                    (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                                    cnt * ScheduleDrawComponent.DrawLineHeight,
                                    item.Span * ScheduleDrawComponent.DrawDayWidth,
                                    ScheduleDrawComponent.DrawLineHeight
                                );
                                g.DrawString(
                                    $"{item.TaskName} [{item.Progress}%]",
                                    ScheduleDrawComponent.TaskFont,
                                    Brushes.Black,
                                    (item.StartDate.Date - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth + ScheduleDrawComponent.OffsetSection,
                                    cnt * ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetSection
                                );
                            } else {
                                //依存タスクは開始日を算出してセットする
                                g.FillRectangle(
                                    new SolidBrush(Color.FromArgb(ScheduleDrawComponent.DayTrans, item.ViewColor)),
                                    (this.mgr.GetStartDateFromDependTaskID(item.DependTaskID) - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                                    cnt * ScheduleDrawComponent.DrawLineHeight,
                                    item.Span * ScheduleDrawComponent.DrawDayWidth,
                                    ScheduleDrawComponent.DrawLineHeight
                                );
                                g.DrawRectangle(
                                    ScheduleDrawComponent.TaskFramePen,
                                    (this.mgr.GetStartDateFromDependTaskID(item.DependTaskID) - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth,
                                    cnt * ScheduleDrawComponent.DrawLineHeight,
                                    item.Span * ScheduleDrawComponent.DrawDayWidth,
                                    ScheduleDrawComponent.DrawLineHeight
                                );
                                g.DrawString(
                                    $"{item.TaskName} [{item.Progress}%]",
                                    ScheduleDrawComponent.TaskFont,
                                    Brushes.Black,
                                    (this.mgr.GetStartDateFromDependTaskID(item.DependTaskID) - this.mgrDraw.StartDate).Days * ScheduleDrawComponent.DrawDayWidth + ScheduleDrawComponent.OffsetSection,
                                    cnt * ScheduleDrawComponent.DrawLineHeight + ScheduleDrawComponent.OffsetSection
                                );
                            }
                        }
                    }
                }
                cnt++;
            }

            //縦罫線
            for (var i = 1; i <= this.mgrDraw.WidthCount + 1; i++) {
                g.DrawLine(
                    ScheduleDrawComponent.TransLinePen,
                    i * ScheduleDrawComponent.DrawDayWidth,
                    0,
                    i * ScheduleDrawComponent.DrawDayWidth,
                    img.Height
                );
            }

            //横罫線
            //セクション
            for (var i = 1; i <= this.mgrDraw.HeightCount; i++) {
                g.DrawLine(
                    ScheduleDrawComponent.TransLinePen,
                    0,
                    i * ScheduleDrawComponent.DrawLineHeight,
                    img.Width - ScheduleDrawComponent.DrawRightMargin,
                    i * ScheduleDrawComponent.DrawLineHeight
                );
            }
        }
    }
}
