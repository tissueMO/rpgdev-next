using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Text.Script {
    using Module;
    using Properties;
    using TaskType = KeyValuePair<string, dlgEVcmdMoveTask.MoveTask>;

    public partial class dlgEVcmdMoveTask : Form {
        private readonly bool extraMove;
        private readonly bool isPlayer;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEVcmdMoveTask(bool extraMove, bool isPlayer) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.uctlGraphSetter.FileFilter = Resources.Extension_Graphics;

            //指示リストを初期化
            Common.SetListControlKeyValuePairMode(this.lstTasks);

            //速度リストを生成
            this.cmbSpeed.Items.Clear();
            this.cmbSpeed.Items.AddRange(Map.GetSpeedList());
            this.cmbSpeed.SelectedIndex = (int) Map.Speed.Normal;

            //臨時移動・日常移動の切り替えを行う
            this.extraMove = extraMove;
            this.chkRepeat.Enabled = !extraMove;
            this.chkRouteLock.Enabled = !extraMove;
            this.chkWaitMode.Enabled = extraMove;

            //操作対象の切り替えを行う
            this.isPlayer = isPlayer;
            this.btnDirAgainstPlayer.Enabled = !isPlayer;
            this.btnDirToPlayer.Enabled = !isPlayer;
            this.btnMoveToPlayer.Enabled = !isPlayer;
            this.btnMoveAgainstPlayer.Enabled = !isPlayer;
        }

        /// <summary>
        /// 移動指示の内容
        /// </summary>
        public class MoveTask {
            public Map.EventMoveTask Type;
            private readonly int intOption;
            private readonly string altIntOption;
            private readonly string strOption;
            private readonly bool isFlagOption;
            private readonly string intComment;

            /// <summary>
            /// コンストラクター
            /// </summary>
            /// <param name="type">指示内容</param>
            /// <param name="intArg">オプション整数値</param>
            /// <param name="altIntArg">オプション整数値の置き換え名（列挙体名・定数など）</param>
            /// <param name="comment">整数値に付けるコメント</param>
            /// <param name="strArg">オプション文字列値</param>
            public MoveTask(Map.EventMoveTask type, int intArg, string altIntArg, string comment, string strArg) {
                this.Type = type;
                this.intOption = intArg;
                this.altIntOption = altIntArg;
                this.intComment = comment;
                this.strOption = strArg;
                this.isFlagOption = false;
            }

            /// <summary>
            /// コンストラクター
            /// </summary>
            public MoveTask(Map.EventMoveTask type, bool intArg, string strArg) {
                this.Type = type;
                this.intOption = Convert.ToInt32(intArg);
                this.strOption = strArg;
                this.isFlagOption = true;
            }

            /// <summary>
            /// フラグ表記に対応した整数オプション値を文字列として返す
            /// </summary>
            public string IntOption {
                get {
                    if (this.isFlagOption) {
                        //フラグ用
                        return (this.intOption == 0) ? "FALSE" : "TRUE";
                    } else if (!string.IsNullOrEmpty(this.altIntOption)) {
                        //列挙体表示用
                        return this.altIntOption;
                    } else if (!string.IsNullOrEmpty(this.intComment)) {
                        //整数値にコメントを付ける
                        return this.intOption.ToString() + "/*" + this.intComment + "*/";
                    } else {
                        return this.intOption.ToString();
                    }
                }
            }

            /// <summary>
            /// 文字列オプション値をダブルクォート文字列にして返す
            /// </summary>
            public string StrOption {
                get {
                    return "\"" + this.strOption + "\"";
                }
            }

            /// <summary>
            /// スクリプト用の列挙体として指示内容を取得する
            /// </summary>
            public static string GetMoveTypeCode(Map.EventMoveTask taskType) {
                return SQ.ToSQEnum(typeof(Map.EventMoveTask), (int) taskType);
            }
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                var buf = this.isPlayer ? Resources.SQ_PlayerMapObject : Resources.SQ_ThisEVData;

                if (this.extraMove) {
                    //臨時移動用の出力
                    foreach (TaskType item in this.lstTasks.Items) {
                        //指示リスト
                        buf += "\r\n";
                        buf += "\t." + Resources.SQ_EVExtraMoveSet
                            .Replace("$1", MoveTask.GetMoveTypeCode(item.Value.Type))
                            .Replace("$2", item.Value.IntOption)
                            .Replace("$3", item.Value.StrOption);
                    }

                    //移動実行処理・移動完了まで待つかどうかの指定
                    buf += "\r\n";
                    buf += "\t." + Resources.SQ_EVExtraMoveDo.Replace("$", this.chkWaitMode.Checked ? "true" : "false");
                } else {
                    //日常移動用の出力
                    if (this.chkRouteLock.Checked) {
                        //経路固定へ
                        buf += "\r\n";
                        buf += "\t." + Resources.SQ_EVMoveSet
                            .Replace("$1", MoveTask.GetMoveTypeCode(Map.EventMoveTask.LockRoute))
                            .Replace("$2", "TRUE")
                            .Replace("$3", "\"\"");
                    }

                    foreach (TaskType item in this.lstTasks.Items) {
                        //指示リスト
                        buf += "\r\n";
                        buf += "\t." + Resources.SQ_EVMoveSet
                            .Replace("$1", MoveTask.GetMoveTypeCode(item.Value.Type))
                            .Replace("$2", item.Value.IntOption)
                            .Replace("$3", item.Value.StrOption);
                    }

                    if (this.chkRepeat.Checked) {
                        //以上、繰り返し
                        buf += "\r\n";
                        buf += "\t." + Resources.SQ_EVMoveSet
                            .Replace("$1", MoveTask.GetMoveTypeCode(Map.EventMoveTask.Repeat))
                            .Replace("$2", "TRUE")
                            .Replace("$3", "\"\"");
                    }
                }
                buf += ";";
                buf += "\r\n";
                return buf;
            }
        }

        /// <summary>
        /// 指示を追加する
        /// </summary>
        private void AddTask(string text, Map.EventMoveTask type, int arg = 0, string altArg = "", string argComment = "", string str = "") {
            this.lstTasks.Items.Add(new TaskType(text, new MoveTask(type, arg, altArg, argComment, str)));
            this.lstTasks.SelectedIndex = this.lstTasks.Items.Count - 1;
        }
        private void AddTask(string text, Map.EventMoveTask type, bool arg, string str = "") {
            text += " " + (arg ? "ON" : "OFF");
            this.lstTasks.Items.Add(new TaskType(text, new MoveTask(type, arg, str)));
            this.lstTasks.SelectedIndex = this.lstTasks.Items.Count - 1;
        }

        /// <summary>
        /// 指示リストの項目操作：上・下・削除
        /// </summary>
        private void mnuUp_Click(object sender, EventArgs e) {
            if (this.lstTasks.SelectedIndices.Count > 0 && 0 < this.lstTasks.SelectedIndices[0]) {
                var temp = new KeyValuePair<int, TaskType>(this.lstTasks.SelectedIndices[0], (TaskType) this.lstTasks.SelectedItems[0]);
                this.lstTasks.Items.RemoveAt(temp.Key);
                this.lstTasks.Items.Insert(temp.Key - 1, temp.Value);
                this.lstTasks.SelectedIndices.Add(temp.Key - 1);
            }
        }
        private void mnuDown_Click(object sender, EventArgs e) {
            if (this.lstTasks.SelectedIndices.Count > 0 && this.lstTasks.SelectedIndices[0] < this.lstTasks.Items.Count - 1) {
                var temp = new KeyValuePair<int, TaskType>(this.lstTasks.SelectedIndices[0], (TaskType) this.lstTasks.SelectedItems[0]);
                this.lstTasks.Items.RemoveAt(temp.Key);
                this.lstTasks.Items.Insert(temp.Key + 1, temp.Value);
                this.lstTasks.SelectedIndices.Clear();
                this.lstTasks.SelectedIndices.Add(temp.Key + 1);
            }
        }
        private void mnuDelete_Click(object sender, EventArgs e) {
            if (this.lstTasks.SelectedIndices.Count > 0) {
                this.lstTasks.Items.RemoveAt(this.lstTasks.SelectedIndex);
            }
        }

        /// <summary>
        /// 指定した方向へ移動
        /// </summary>
        private void btnMoveNorth_Click(object sender, EventArgs e) {
            this.AddTask("移動: [↑]", Map.EventMoveTask.Move, (int) Map.Direction4.North, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.North));
        }
        private void btnMoveWest_Click(object sender, EventArgs e) {
            this.AddTask("移動: [←]", Map.EventMoveTask.Move, (int) Map.Direction4.West, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.West));
        }
        private void btnMoveEast_Click(object sender, EventArgs e) {
            this.AddTask("移動: [→]", Map.EventMoveTask.Move, (int) Map.Direction4.East, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.East));
        }
        private void btnMoveSouth_Click(object sender, EventArgs e) {
            this.AddTask("移動: [↓]", Map.EventMoveTask.Move, (int) Map.Direction4.South, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.South));
        }

        /// <summary>
        /// 前進・後退
        /// </summary>
        private void btnMoveForward_Click(object sender, EventArgs e) {
            this.AddTask("移動: 前進", Map.EventMoveTask.Forward);
        }
        private void btnMoveBack_Click(object sender, EventArgs e) {
            this.AddTask("移動: 後退", Map.EventMoveTask.Backward);
        }

        /// <summary>
        /// 主人公の方へ近づく・遠ざかる
        /// </summary>
        private void btnMoveToPlayer_Click(object sender, EventArgs e) {
            this.AddTask("移動: 主人公へ近づく", Map.EventMoveTask.MoveToPlayer);
        }
        private void btnMoveAgainstPlayer_Click(object sender, EventArgs e) {
            this.AddTask("移動: 主人公から離れる", Map.EventMoveTask.MoveAgainstPlayer);
        }

        /// <summary>
        /// ランダム一歩
        /// </summary>
        private void btnMoveRandom_Click(object sender, EventArgs e) {
            this.AddTask("移動: ランダム", Map.EventMoveTask.MoveRandom);
        }

        /// <summary>
        /// 指定した方向を向く
        /// </summary>
        private void btnDirNorth_Click(object sender, EventArgs e) {
            this.AddTask("方向: [↑]", Map.EventMoveTask.Direction, (int) Map.Direction4.North, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.North));
        }
        private void btnDirWest_Click(object sender, EventArgs e) {
            this.AddTask("方向: [←]", Map.EventMoveTask.Direction, (int) Map.Direction4.West, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.West));
        }
        private void btnDirEast_Click(object sender, EventArgs e) {
            this.AddTask("方向: [→]", Map.EventMoveTask.Direction, (int) Map.Direction4.East, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.East));
        }
        private void btnDirSouth_Click(object sender, EventArgs e) {
            this.AddTask("方向: [↓]", Map.EventMoveTask.Direction, (int) Map.Direction4.South, SQ.ToSQEnum(typeof(Map.Direction4), (int) Map.Direction4.South));
        }

        /// <summary>
        /// 左・右を向く
        /// </summary>
        private void btnDirLeft_Click(object sender, EventArgs e) {
            this.AddTask("方向: 左向け左", Map.EventMoveTask.TurnLeft);
        }
        private void btnDirRight_Click(object sender, EventArgs e) {
            this.AddTask("方向: 右向け右", Map.EventMoveTask.TurnRight);
        }

        /// <summary>
        /// 主人公の方を向く・背ける
        /// </summary>
        private void btnDirToPlayer_Click(object sender, EventArgs e) {
            this.AddTask("方向: 主人公を向く", Map.EventMoveTask.TurnToPlayer);
        }
        private void btnDirAgainstPlayer_Click(object sender, EventArgs e) {
            this.AddTask("方向: 主人公から背ける", Map.EventMoveTask.TurnAgainstPlayer);
        }

        /// <summary>
        /// ランダム方向を向く
        /// </summary>
        private void btnDirRandom_Click(object sender, EventArgs e) {
            this.AddTask("方向: ランダム", Map.EventMoveTask.TurnRandom);
        }

        /// <summary>
        /// 回れ右
        /// </summary>
        private void btnDirBack_Click(object sender, EventArgs e) {
            this.AddTask("方向: 後ろを向く", Map.EventMoveTask.Turn180);
        }

        /// <summary>
        /// 速度変更
        /// </summary>
        private void btnMoveSpeed_Click(object sender, EventArgs e) {
            this.AddTask(
                $"速さ: 移動速度を変更 [{Map.GetSpeedList()[this.cmbSpeed.SelectedIndex]}]",
                Map.EventMoveTask.SetMoveSpeed, this.cmbSpeed.SelectedIndex,
                SQ.ToSQEnum(typeof(Map.Speed), this.cmbSpeed.SelectedIndex)
            );
        }
        private void btnMoveFrequency_Click(object sender, EventArgs e) {
            this.AddTask(
                $"速さ: 移動頻度を変更 [{Map.GetSpeedList()[this.cmbSpeed.SelectedIndex]}]",
                Map.EventMoveTask.SetMoveFrequency,
                this.cmbSpeed.SelectedIndex,
                SQ.ToSQEnum(typeof(Map.Speed),
                this.cmbSpeed.SelectedIndex)
            );
        }

        /// <summary>
        /// ON/OFF 切替
        /// </summary>
        private void btnStopAnim_Click(object sender, EventArgs e) {
            this.AddTask("切替: 静止アニメ", Map.EventMoveTask.EnabledStopAnim, this.chkValue.Checked);
        }
        private void btnWalkAnim_Click(object sender, EventArgs e) {
            this.AddTask("切替: 歩行アニメ", Map.EventMoveTask.EnabledWalkAnim, this.chkValue.Checked);
        }
        private void btnNoHit_Click(object sender, EventArgs e) {
            this.AddTask("切替: すり抜け", Map.EventMoveTask.NoHit, this.chkValue.Checked);
        }
        private void btnFixedDirection_Click(object sender, EventArgs e) {
            this.AddTask("切替: 向き固定", Map.EventMoveTask.FixedDirection, this.chkValue.Checked);
        }
        private void btnHide_Click(object sender, EventArgs e) {
            this.AddTask("切替: 非表示", Map.EventMoveTask.Hide, this.chkValue.Checked);
        }

        /// <summary>
        /// スクリプトのイベントハンドラー呼び出し
        /// </summary>
        private void btnSQ_Click(object sender, EventArgs e) {
            this.AddTask($"その他: スクリプト呼出 [引数: {(int) this.numArg.Value}]", Map.EventMoveTask.SQ, (int) this.numArg.Value);
        }

        /// <summary>
        /// グラフィック変更
        /// </summary>
        private void btnChangeGraph_Click(object sender, EventArgs e) {
            this.AddTask($"その他: グラフィック変更 [{this.uctlGraphSetter.Result}]", Map.EventMoveTask.ChangeGraphics, 0, this.uctlGraphSetter.Result);
        }

        /// <summary>
        /// ミリ秒でウェイト
        /// </summary>
        private void btnWait_Click(object sender, EventArgs e) {
            this.AddTask($"その他: ウェイト [{(int) this.numWaitTime.Value} ミリ秒]", Map.EventMoveTask.Wait, (int) this.numWaitTime.Value, "", "ミリ秒");
        }
    }
}
