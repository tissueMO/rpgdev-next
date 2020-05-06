using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;
    using EVData = CtrlComponent.Map.mgrMapObject.MapOneData.EventOneData;

    public partial class ctlEVPage : UserControl {
        private int beforeTabIndex = -1;
        private EVData.EventOnePage EVPage;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlEVPage() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //環境設定に応じたエディターを設置する
            if (Settings.Default.Map_SQEasyMode) {
                this.uctlEVCommandEditor.Visible = true;
            } else {
                this.uctlTextEditor.Visible = true;
                this.uctlTextEditor.IsReadOnly = true;           //保存ボタンを押せないようにするため
                this.uctlTextEditor.IsScriptMode = true;
                this.uctlTextEditor.CanSQTest = false;
                this.uctlTextEditor.ApplySettings();
            }

            //各種リストを初期化する
            this.cmbStartIf.Items.Clear();
            this.cmbStartIf.Items.AddRange(Map.GetEventTriggerTypeList());
            this.cmbStartIf.SelectedIndex = (int) Map.EventTriggerType.None;
            this.cmbMoveType.Items.Clear();
            this.cmbMoveType.Items.AddRange(Map.GetEventMoveTypeList());
            this.cmbMoveType.SelectedIndex = (int) Map.EventMoveType.None;

            this.cmbDirection.Items.Clear();
            this.cmbDirection.Items.AddRange(Map.GetDirection4List(false));
            this.cmbDirection.SelectedIndex = (int) Map.Direction4.South;
            this.cmbFrequency.Items.Clear();
            this.cmbFrequency.Items.AddRange(Map.GetSpeedList());
            this.cmbFrequency.SelectedIndex = (int) Map.Speed.Low;
            this.cmbSpeed.Items.Clear();
            this.cmbSpeed.Items.AddRange(Map.GetSpeedList());
            this.cmbSpeed.SelectedIndex = (int) Map.Speed.Low;

            this.cmbAnimNum.Items.Clear();
            this.cmbAnimNum.Items.AddRange(new string[] { "動作１", "静止", "動作２" });
            this.cmbAnimNum.SelectedIndex = (int) Map.EventAnimation.Stop;
            this.cmbDrawPriority.Items.Clear();
            this.cmbDrawPriority.Items.AddRange(Map.GetEventDrawPriorityList());
            this.cmbDrawPriority.SelectedIndex = (int) Map.EventDrawPriority.Middle;
            this.uctlHitRange.Maximum = new Point(Map.MaxMapSize);
            this.chkIsStopAnim.Checked = true;
            this.chkIsWalkAnim.Checked = true;

            this.setCommentScriptTypeComment((int) Map.EventScriptType.Page);
        }

        /// <summary>
        /// コントロールが読み込まれるときにスクリプトエディターをセットします。
        /// </summary>
        private void ctlEVPage_Load(object sender, EventArgs e) {
            this.tbcEVScripts.SelectedIndex = (int) Map.EventScriptType.Run;
        }

        /// <summary>
        /// イベントページ情報をコントロールに反映します。
        /// </summary>
        public void SetEVPageData(EVData.EventOnePage evPage) {
            this.EVPage = evPage;

            //NOTE: イベントページ情報 IN
            this.cmbStartIf.SelectedIndex = (int) evPage.Trigger;
            this.cmbMoveType.SelectedIndex = (int) evPage.MoveType;

            this.cmbDirection.SelectedIndex = (int) evPage.Direction;
            this.cmbSpeed.SelectedIndex = (int) evPage.MoveSpeed;
            this.cmbFrequency.SelectedIndex = (int) evPage.MoveFrequency;

            this.uctlGraphics.FileName = evPage.Graphic;
            this.cmbAnimNum.SelectedIndex = (int) evPage.AnimationPattern;
            this.cmbDrawPriority.SelectedIndex = (int) evPage.DrawPriority;
            this.uctlHitRange.Result = new Point(evPage.HitRect);
            this.chkIsStopAnim.Checked = evPage.StoppingAnimation;
            this.chkIsWalkAnim.Checked = evPage.MovingAnimation;
            this.chkNoHit.Checked = evPage.NoHit;
            this.chkFixedDirection.Checked = evPage.FixedDirection;

            //スクリプトをロードする
            if (!Settings.Default.Map_SQEasyMode) {
                this.uctlTextEditor.EdittingText = evPage.SQ[this.tbcEVScripts.SelectedIndex];
            } else {
                this.uctlEVCommandEditor.SetTextAll(evPage.SQ[this.tbcEVScripts.SelectedIndex]);
            }
        }

        /// <summary>
        /// コントロールからイベントページ情報を生成して返します。
        /// </summary>
        public EVData.EventOnePage GetEVPageData() {
            var evPage = new EVData.EventOnePage {
                //NOTE: イベントページ情報 OUT
                Trigger = (Map.EventTriggerType) this.cmbStartIf.SelectedIndex,
                MoveType = (Map.EventMoveType) this.cmbMoveType.SelectedIndex,

                Direction = (Map.Direction4) this.cmbDirection.SelectedIndex,
                MoveSpeed = (Map.Speed) this.cmbSpeed.SelectedIndex,
                MoveFrequency = (Map.Speed) this.cmbFrequency.SelectedIndex,

                Graphic = this.uctlGraphics.FileName,
                AnimationPattern = (Map.EventAnimation) this.cmbAnimNum.SelectedIndex,
                DrawPriority = (Map.EventDrawPriority) this.cmbDrawPriority.SelectedIndex,
                HitRect = new Size(this.uctlHitRange.Result),
                StoppingAnimation = this.chkIsStopAnim.Checked,
                MovingAnimation = this.chkIsWalkAnim.Checked,
                NoHit = this.chkNoHit.Checked,
                FixedDirection = this.chkFixedDirection.Checked
            };

            //スクリプトをコピーする
            for (var i = 0; i < Common.GetEnumCount<Map.EventScriptType>(); i++) {
                evPage.SQ[i] = this.EVPage.SQ[i];
            }

            return evPage;
        }

        /// <summary>
        /// 現在のコードを現在の種別もしくは指定した種別の中に格納します。
        /// </summary>
        public void ApplyCurrentScript(int index = -1) {
            if (!Settings.Default.Map_SQEasyMode) {
                this.EVPage.SQ[index != -1 ? index : this.tbcEVScripts.SelectedIndex] = this.uctlTextEditor.EdittingText;
                this.uctlTextEditor.IsDirty = false;
            } else {
                this.EVPage.SQ[index != -1 ? index : this.tbcEVScripts.SelectedIndex] = this.uctlEVCommandEditor.GetTextAll();
                this.uctlEVCommandEditor.IsDirty = false;
            }
        }

        /// <summary>
        /// 指定したコードを現在の入力欄に挿入します。
        /// </summary>
        public void InsertCommand(string code, bool forbiddenCRLF) {
            if (!Settings.Default.Map_SQEasyMode) {
                Common.InsertCommand(this.uctlTextEditor, code, forbiddenCRLF);
            } else {
                this.uctlEVCommandEditor.InsertLine(this.uctlEVCommandEditor.CurrentIndex, code, true, forbiddenCRLF);
            }
        }

        /// <summary>
        /// タブ切り替え時にコードを格納し、エディターコントロールが移動先まで追いかける
        /// </summary>
        private void tbcEVScripts_Selected(object sender, TabControlEventArgs e) {
            if (this.beforeTabIndex != -1) {
                //移動前での編集結果を適用する
                this.ApplyCurrentScript(this.beforeTabIndex);
            }

            //移動先のコードをエディターに格納する
            this.setCommentScriptTypeComment(e.TabPageIndex);
            if (!Settings.Default.Map_SQEasyMode) {
                this.uctlTextEditor.EdittingText = (this.EVPage?.SQ != null) ? this.EVPage.SQ[e.TabPageIndex] : "";
            } else {
                this.uctlEVCommandEditor.SetTextAll((this.EVPage?.SQ != null) ? this.EVPage.SQ[e.TabPageIndex] : "");
            }

            this.SuspendLayout();
            e.TabPage.Controls.Add(this.pnlEditor);
            this.ResumeLayout();
            this.beforeTabIndex = e.TabPageIndex;
        }

        /// <summary>
        /// スクリプト種別の説明を表示します。
        /// </summary>
        private void setCommentScriptTypeComment(int index) {
            switch ((Map.EventScriptType) index) {
                case Map.EventScriptType.Page:
                    this.lblComment.Text = "[return (条件式);] を用いることでページ条件を定義します。\r\nページ番号の降順に呼び出され、定義しない場合はそのページが常に条件を満たすようになります。";
                    break;
                case Map.EventScriptType.Trigger:
                    this.lblComment.Text = "[return (条件式);] を用いることで起動条件の詳細を定義します。\r\nこのイベントページが実行される直前に呼び出され、定義しない場合は常に条件を満たすようになります。";
                    break;
                case Map.EventScriptType.Move:
                    this.lblComment.Text = "このページが有効であるときに行われる日常移動のパターンを定義します。これはマップが読み込まれるときに呼び出されます。\r\n応用的な使い方として、自動的に始まるイベントの事前準備の処理等を記述することもできます。";
                    break;
                case Map.EventScriptType.Run:
                    this.lblComment.Text = "このイベントページが起動したときの処理内容を定義します。\r\n「並列実行」でない場合は他のすべての処理が停止します。";
                    break;
                default:
                    this.lblComment.Text = "";
                    break;
            }
        }
    }
}
