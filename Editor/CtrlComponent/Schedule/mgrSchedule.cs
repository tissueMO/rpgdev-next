using System;
using System.Collections.Generic;
using System.Drawing;

namespace Editor.CtrlComponent.Schedule {
    using Module;
    using Properties;

    /// <summary>
    /// スケジュールを管理するクラス
    /// </summary>
    public class mgrSchedule {
        public List<SectionData> Sections = new List<SectionData>();
        public List<TaskData> Tasks = new List<TaskData>();
        public ctlScheduleEditor.TaskSortType TaskSortType = ctlScheduleEditor.TaskSortType.Group;

        /// <summary>
        /// このクラスでスケジュール管理下における最大日数
        /// </summary>
        public const int MaxPeriodDays = 365 * 2;

        /// <summary>
        /// 部門データ
        /// </summary>
        public class SectionData {

            /// <summary>
            /// 担当者データ
            /// </summary>
            public class WorkerData {
                public int ID;
                public string Name;
            }

            public int ID;
            public string Name;
            public List<WorkerData> Workers;     //部門に属する担当者のIDと名前がペアになったリスト
        }

        /// <summary>
        /// タスクエディター列順序
        /// </summary>
        public enum TaskListColumns {
            SectionName,
            WorkerName,
            TaskName,
            StartDate,
            Span,
            Progress,
            Color,
        }

        /// <summary>
        /// タスク情報のデータ順序
        /// </summary>
        public enum TaskDataIndices {
            TaskID,
            TaskName,
            SectionID,
            WorkerID,
            DependTaskID,
            StartDate,
            Span,
            Progress,
            Color,
        }

        /// <summary>
        /// タスクデータ
        /// </summary>
        public class TaskData {
            public int SectionID = -1;      //部門ID : 既定でマイルストーン
            public int WorkerID = -1;       //担当者ID
            public int TaskID = -1;         //タスクID
            public string TaskName;         //タスク名
            public int Span = 1;            //日数
            public int DependTaskID = -1;   //依存先タスクID
            public int Progress = 0;        //進捗率
            public DateTime StartDate = DateTime.Now;      //開始日
            public Color ViewColor = Color.Blue;           //表示色
            public bool NotApplyedProgress = false;        //進捗報告が反映されていない状態であるか
        }

        /// <summary>
        /// 指定タスクに対する対象担当者のタスクの重複をチェックします
        /// </summary>
        /// <param name="secID">対象担当者の部門ID</param>
        /// <param name="workerID">対象担当者ID</param>
        /// <param name="taskID">チェックから除外される対象のタスクID</param>
        /// <returns>重複しているかどうか</returns>
        public bool CheckOverlapedTask(int secID, int workerID, int taskID, int dependID, DateTime start, int span) {
            //対象タスクが占めるカレンダー領域を生成
            DateTime sDate, eDate;
            if (dependID == -1) {
                sDate = start;
            } else {
                sDate = this.GetStartDateFromDependTaskID(dependID).Date;
            }
            eDate = sDate.AddDays(span).Date;

            //現在のタスクリストから調査
            foreach (var item in this.Tasks) {
                if (item.SectionID == secID && item.WorkerID == workerID && item.TaskID != taskID) {
                    //現在見ているタスクが占めるカレンダー領域を生成
                    DateTime StartDate, EndDate;
                    if (item.DependTaskID == -1) {
                        StartDate = item.StartDate.Date;
                    } else if (item.DependTaskID == taskID) {
                        //チェック対象のタスクを依存先としているタスクは検索対象外とする
                        continue;
                    } else {
                        StartDate = this.GetStartDateFromDependTaskID(item.DependTaskID).Date;
                    }
                    EndDate = StartDate.AddDays(item.Span).Date;

                    //重複判定
                    if ((StartDate <= sDate && sDate < EndDate) || (StartDate < eDate && eDate <= EndDate)) {
                        return true;
                    }
                }
            }

            return false;
        }

        /// <summary>
        /// 依存先タスクIDから元のタスクの開始日を割り出します
        /// </summary>
        /// <param name="dTaskID">依存先タスクのID</param>
        /// <returns>タスクが開始できると見込まれる日付</returns>
        public DateTime GetStartDateFromDependTaskID(int dTaskID) {
            //IDからタスク情報を得る
            TaskData dTask = null;
            foreach (var task in this.Tasks) {
                if (task.TaskID == dTaskID && dTaskID != -1) {
                    dTask = task;
                    break;
                }
            }

            if (dTask == null) {
                //依存先タスクが見つからなかった場合は最速のタスクにする
                return DateTime.MinValue;
            } else if (dTask.DependTaskID != -1) {
                //依存先がさらに依存している場合は、そのタスクの終了日まで遡って確認する
                return this.GetStartDateFromDependTaskID(dTask.DependTaskID).AddDays(dTask.Span).Date;
            } else {
                //依存先が独立タスクのときはそのタスクの終了日を返す
                return dTask.StartDate.Date.AddDays(dTask.Span).Date;
            }
        }

        /// <summary>
        /// スケジュールの開始日を取得します
        /// </summary>
        /// <param name="tempData">暫定データとして差し替えるタスク</param>
        /// <returns>プロジェクトの中で一番最初に始まるタスクの開始日</returns>
        public DateTime GetFirstStartTaskDate(TaskData tempData = null) {
            var result = DateTime.MaxValue;
            var tempTasks = new List<TaskData>(this.Tasks);
            if (tempData?.TaskID == -1) {
                // 暫定タスクを加える
                tempTasks.Add(tempData);
            }

            foreach (var item in tempTasks) {
                var startDate = new DateTime(item.StartDate.Ticks);
                var dependID = item.DependTaskID;

                if (item.TaskID == tempData?.TaskID) {
                    // 暫定タスクに差し替える
                    startDate = new DateTime(tempData.StartDate.Ticks);
                    dependID = tempData.DependTaskID;
                }

                if (result > startDate && dependID == -1) {
                    result = startDate;
                }
            }

            return result.Date;
        }

        /// <summary>
        /// スケジュールの終了日を取得します
        /// </summary>
        /// <param name="tempData">暫定データとして差し替えるタスク</param>
        /// <returns>プロジェクトの中で一番最後に終わるタスクの終了日</returns>
        public DateTime GetLastEndTaskDate(TaskData tempData = null) {
            var result = DateTime.MinValue;
            var isAddMode = tempData?.TaskID == -1;
            const int newID = int.MaxValue;
            TaskData oldTaskData = null;

            try {
                // 前処理
                if (tempData != null) {
                    if (isAddMode) {
                        // 新規タスクの場合はこの関数内のみ暫定的に追加したことにする
                        tempData.TaskID = newID;
                        this.Tasks.Add(tempData);
                    } else {
                        // 既存タスクを変更した場合はこの関数内でのみ暫定的に変更したことにする
                        var targetTaskIndex = this.Tasks.FindIndex(task => task.TaskID == tempData.TaskID);
                        if (targetTaskIndex != -1) {
                            oldTaskData = this.Tasks[targetTaskIndex];
                            this.Tasks[targetTaskIndex] = tempData;
                        }
                    }
                }

                // 全タスクを走査
                foreach (var item in this.Tasks) {
                    var startDate = new DateTime(item.StartDate.Ticks);
                    var dependID = item.DependTaskID;
                    var span = item.Span;

                    if (item.TaskID == tempData?.TaskID) {
                        // 暫定タスクに差し替える
                        startDate = new DateTime(tempData.StartDate.Ticks);
                        dependID = tempData.DependTaskID;
                        span = tempData.Span;
                    }

                    startDate = (dependID == -1) ? startDate : this.GetStartDateFromDependTaskID(dependID);
                    if (result < startDate.Date.AddDays(span - 1)) {
                        result = startDate.Date.AddDays(span - 1);
                    }
                }
            } finally {
                // 復元処理
                if (tempData != null) {
                    if (isAddMode) {
                        // 新規タスクとして暫定的に追加したデータを削除
                        this.Tasks.Remove(this.Tasks.Find(task => task.TaskID == newID));
                        tempData.TaskID = -1;
                    } else {
                        // 既存タスクとして暫定的に変更したデータを復元
                        var targetTaskIndex = this.Tasks.FindIndex(task => task.TaskID == tempData.TaskID);
                        if (targetTaskIndex != -1 && oldTaskData != null) {
                            this.Tasks[targetTaskIndex] = oldTaskData;
                        }
                    }
                }
            }

            return result.Date;
        }

        /// <summary>
        /// このプロジェクトにおけるスケジュール上の期間を返します。
        /// </summary>
        /// <returns>プロジェクト期間</returns>
        public TimeSpan GetProjectPeriod(TaskData tempData = null) {
            var start = this.GetFirstStartTaskDate(tempData);
            var end = this.GetLastEndTaskDate(tempData);
            var delta = end - start;
            return delta;
        }

        /// <summary>
        /// 指定したタスクを考慮して、このプロジェクトにおけるスケジュール上の最大期間を超過しないかどうかを検査します。
        /// </summary>
        /// <param name="task">検査対象のタスク</param>
        /// <returns>このプロジェクトにおけるスケジュール上の最大期間を超過しないかどうか</returns>
        public bool CheckTaskInProjectMaxPeriod(TaskData task) {
            var period = this.GetProjectPeriod(task);
            return period.TotalDays <= mgrSchedule.MaxPeriodDays;
        }

        /// <summary>
        /// 現在のタスク一覧をソートします
        /// </summary>
        public void SortTaskList() {
            //ソート前に依存タスクの日程を確定させる
            foreach (var item in this.Tasks) {
                if (item.DependTaskID != -1) {
                    item.StartDate = this.GetStartDateFromDependTaskID(item.DependTaskID).Date;
                }
            }

            //ソートを実行
            switch (this.TaskSortType) {
                case ctlScheduleEditor.TaskSortType.DateTime:
                    this.Tasks.Sort((x, y) => {
                        //開始日順で見る
                        return DateTime.Compare(x.StartDate.Date, y.StartDate.Date);
                    });
                    return;

                case ctlScheduleEditor.TaskSortType.Group:
                    this.Tasks.Sort((x, y) => {
                        //部門登録順で見る
                        int sec_x = -1, sec_y = -1;
                        for (var i = 0; i < this.Sections.Count && sec_x == -1 && sec_y == -1; i++) {
                            if (this.Sections[i].ID == x.SectionID) {
                                sec_x = i;
                            }
                            if (this.Sections[i].ID == y.SectionID) {
                                sec_y = i;
                            }
                        }
                        if (sec_x != sec_y) {
                            return -(sec_x - sec_y);
                        }

                        //担当者登録順で見る: マイルストーンの場合は担当者名の判定を行わない
                        int worker_x = -1, worker_y = -1;
                        if (sec_x != -1) {
                            for (var i = 0; i < this.Sections[sec_x].Workers.Count && worker_x == -1 && worker_y == -1; i++) {
                                if (this.Sections[sec_x].Workers[i].ID == x.WorkerID) {
                                    worker_x = i;
                                }
                                if (this.Sections[sec_x].Workers[i].ID == y.WorkerID) {
                                    worker_y = i;
                                }
                            }
                        }
                        if (worker_x != worker_y) {
                            return -(worker_x - worker_y);
                        }

                        //開始日順で見る
                        if (x.StartDate.Date != y.StartDate.Date) {
                            return DateTime.Compare(x.StartDate.Date, y.StartDate.Date);
                        }

                        //タスクIDで見る
                        return -(x.TaskID - y.TaskID);
                    });
                    return;
            }
        }

        /// <summary>
        /// 指定したタスクの所属を含めた正式名称を返します
        /// </summary>
        /// <param name="secs">部門リスト</param>
        /// <param name="targetTask">対象タスク</param>
        /// <returns>対象タスクの所属部門、担当者名を含めたタスク名</returns>
        public static string GetTaskFullName(List<SectionData> secs, TaskData targetTask) {
            //マイルストーンは担当者部分を省略する
            if (targetTask.SectionID == -1) {
                return Resources.Sche_MilestoneName + Resources.Split_IDNamePair + targetTask.TaskName;
            }

            //部門を探す
            foreach (var sec in secs) {
                if (sec.ID != targetTask.SectionID) {
                    continue;
                }

                //担当者を探す
                foreach (var worker in sec.Workers) {
                    if (worker.ID != targetTask.WorkerID) {
                        continue;
                    }
                    return sec.Name + Resources.Split_IDNamePair + worker.Name + Resources.Split_IDNamePair + targetTask.TaskName;
                }

                if (targetTask.WorkerID == -1) {
                    //担当者全員の場合
                    return sec.Name + Resources.Split_IDNamePair + Resources.Sche_SectionAllWorkers + Resources.Split_IDNamePair + targetTask.TaskName;
                }

                // 担当者不明の場合
                return "";
            }

            //見つからなかった場合
            return "";
        }
    }

    /// <summary>
    /// 描画に必要な情報を管理するクラス
    /// </summary>
    public class ScheduleDrawComponent {
        public static readonly int DrawDayWidth = 30;           //一日当たりの幅
        public static readonly int DrawHeaderHeight = 85;       //ヘッダーの高さ
        public static readonly int DrawLineHeight = 22;         //一行当たりの高さ
        public static readonly int DrawRightMargin = 300;       //右端の余白
        public static readonly int OffsetHeaderLine = -3;       //日付の各行のオフセット
        public static readonly int OffsetHeaderDay = 5;         //日付の各列のオフセット
        public static readonly int OffsetSection = 3;           //セクションの各行のオフセット
        public static readonly int DayTrans = 125;              //日全体を塗りつぶすときの不透明度
        public static readonly Pen TransLinePen = new Pen(Color.FromArgb(150, Color.Silver));
        public static readonly Pen TaskFramePen = new Pen(Color.Black, 2);
        public static readonly Font TaskFont = new Font(Settings.Default.Text_Font.FontFamily, 9, FontStyle.Bold, GraphicsUnit.Point);

        public int HeightCount = 0;             //マイルストーンを含まない、部門全体およびその中の担当者をすべて一覧にしたときの項目数（縦軸）
        public int WidthCount = -1;             //開始日から終了日までの日数
        public DateTime StartDate;              //プロジェクトの開始日
        public DateTime EndDate;                //プロジェクトの終了日

        /// <summary>
        /// 情報取得用コンストラクター
        /// </summary>
        /// <param name="mgrObject">ScheduleManagerオブジェクト</param>
        public ScheduleDrawComponent(mgrSchedule mgrObject) {
            this.CalcData(mgrObject);
        }

        /// <summary>
        /// 描画用コンストラクター
        /// </summary>
        /// <param name="mgrObject">ScheduleManagerオブジェクト</param>
        /// <param name="Graphics">Graphicsオブジェクト</param>
        public ScheduleDrawComponent(mgrSchedule mgrObject, Graphics g, Size imgSize) {
            this.CalcData(mgrObject);

            //描画の初期設定
            Common.ApplyHighQualityDrawing(g);

            //背景と外枠を描画
            g.FillRectangle(Brushes.White, 0, 0, imgSize.Width, imgSize.Height);
            g.DrawRectangle(ScheduleDrawComponent.TaskFramePen, 1, 1, imgSize.Width - 2, imgSize.Height - 2);
        }

        /// <summary>
        /// 必要情報を算出します。
        /// </summary>
        /// <param name="mgrObject">ScheduleManagerオブジェクト</param>
        private void CalcData(mgrSchedule mgrObject) {
            this.StartDate = mgrObject.GetFirstStartTaskDate();
            this.EndDate = mgrObject.GetLastEndTaskDate();

            //縦軸を算出
            foreach (var item in mgrObject.Sections) {
                this.HeightCount++;
                foreach (var sub in item.Workers) {
                    this.HeightCount++;
                }
            }

            //横軸を算出
            if (mgrObject.Tasks.Count > 0) {
                this.WidthCount = (this.EndDate - this.StartDate).Days;
            }
        }
    }
}
