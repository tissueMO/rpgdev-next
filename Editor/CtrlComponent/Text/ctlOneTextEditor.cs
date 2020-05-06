using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Text {
    using Editor.Dialog.Common;
    using Module;
    using Properties;
    using SQObject = GenerateGlueCode.GenerateCPPDocument.SQObject;

    public partial class ctlOneTextEditor : UserControl, IDirtyable {
        public const int FindMarkID = 1;                        //検索時におけるマーキング機能のID
        public const string FindMarkName = "find";              //検索時におけるマーキング機能の名前
        public const int ConflictMarkID = 2;                    //マージにおけるマーキング機能のID
        public const string ConflictMarkName = "conflict";      //マージにおけるマーキング機能の名前

        private static readonly Color dirtyBarColor = Color.FromArgb(255, 255, 170, 50);
        private static readonly Color cleanedLineBar = Color.FromArgb(255, 0, 255, 0);

        private FileStream fs = null;                           //ファイルアクセス制御
        private Dialog.Text.dlgLocalSearchReplace Dlg;
        private bool forbiddenAutoFindCode = false;             //入力補完欄のテキスト変更時に自動で絞り込むのを禁止するフラグ
        private bool continueAutoComplete = false;              //再度入力補完を開き直すフラグ

        public CtrlComponent.Database.ctlDatabaseEditor DBCtrl;
        public delegate void FoundCommandNodeEventHandler(object sender, FoundCommandNodeEventArgs e);
        public event FoundCommandNodeEventHandler FoundCommandNode;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlOneTextEditor() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.FileName = null;

            //Azukiエディターコントロールの初期化
            Sgry.Azuki.Marking.Register(new Sgry.Azuki.MarkingInfo(FindMarkID, FindMarkName));
            this.ApplySettings();
            this.azkEditor.AutoIndentHook = Sgry.Azuki.AutoIndentHooks.GenericHook;      //デフォルトのオートインデント
            this.azkEditor.ConvertsFullWidthSpaceToSpace = true;     //全角スペース問題防止
            this.azkEditor.TabWidth = 4;
            this.azkEditor.MarksUri = true;
            this.azkEditor.ShowsDirtBar = true;
            this.azkEditor.ShowsHRuler = true;
            this.azkEditor.ShowsHScrollBar = true;
            this.azkEditor.ShowsLineNumber = true;
            this.azkEditor.ShowsVScrollBar = true;
            this.azkEditor.UnindentsWithBackspace = true;
            this.azkEditor.UsesTabForIndent = true;
            this.azkEditor.ColorScheme.DirtyLineBar = ctlOneTextEditor.dirtyBarColor;
            this.azkEditor.ColorScheme.CleanedLineBar = ctlOneTextEditor.cleanedLineBar;
            this.azkEditor.ColorScheme.SelectionBack = SystemColors.Highlight;
            this.azkEditor.ColorScheme.SelectionFore = SystemColors.HighlightText;
            this.azkEditor.ColorScheme.SetMarkingDecoration(FindMarkID, new Sgry.Azuki.BgColorTextDecoration(Color.Yellow));
            this.ApplyColorSettings();

            //恒常的なイベントハンドラーをセット
            this.txtAutoCompleteInserter.Enter += this.autoCompleteTextBox_Enter;
            this.txtAutoCompleteInserter.KeyDown += this.autoCompleteTextBox_KeyDown;
            this.txtAutoCompleteInserter.TextChanged += this.autoCompleteTextBox_TextChanged;
            this.azkEditor.TextChanged += (sender, e) => {
                //テキストが変更されるたびにコントロール監視する
                this.watchCtrl();
            };
        }

        /// <summary>
        /// コマンドツリーの該当箇所を発見したときのイベントの引数
        /// </summary>
        public class FoundCommandNodeEventArgs : EventArgs {
            public TreeNode Node;
            public string[] Arguments;
            public string NodeTag;
            public string Option;

            public FoundCommandNodeEventArgs(TreeNode node, string[] args = null) {
                this.Node = node;
                this.Arguments = args;
            }
            public FoundCommandNodeEventArgs(string tag, string option) {
                this.NodeTag = tag;
                this.Option = option;
            }
        }

        /// <summary>
        /// 編集中のファイル名 (rPath)
        /// </summary>
        public string FileName {
            get {
                return this.fileName;
            }
            set {
                if (string.IsNullOrEmpty(value)) {
                    this.Visible = false;
                    this.fileName = null;
                    this.IsScriptMode = false;
                    Common.EndFileAccessLock(ref this.fs);
                } else {
                    this.Visible = true;
                    this.fileName = value;
                    this.IsScriptMode = (Path.GetExtension(value) == Resources.Extension_SQ);
                    this.IsReadOnly = !Common.StartFileAccessLock(ref this.fs, this.FileName);        //アクセスロックor閲覧モード
                }
            }
        }
        private string fileName;

        /// <summary>
        /// 読み取り専用であるかどうか
        /// </summary>
        public bool IsReadOnly {
            get {
                return this.isReadOnly;
            }
            set {
                this.isReadOnly = value;
                this.tolSave.Enabled = !this.isReadOnly;
            }
        }
        private bool isReadOnly = false;

        /// <summary>
        /// スクリプトモードであるかどうか
        /// </summary>
        public bool IsScriptMode {
            get {
                return this.isScriptMode;
            }
            set {
                this.isScriptMode = value;
                this.CanSQTest = value;
                this.tolSplitSQCode.Visible = value;
                this.tolInsertCode.Visible = value;
                this.tolInsertReserchCode.Visible = value;
                this.ctmInsertCode.Visible = value;
                this.ctmInsertReserchCode.Visible = value;
            }
        }
        private bool isScriptMode = false;

        /// <summary>
        /// スクリプトの単体テストを有効にするかどうか
        /// </summary>
        public bool CanSQTest {
            get {
                return this.canSQTest;
            }
            set {
                this.tolSplitSQTest.Visible = value;
                this.tolTestCommand.Visible = value;
                this.tolSQTest.Visible = value;
                if (value && !this.IsScriptMode) {
                    //単体テストを有効にする場合は自動的にスクリプトモードにする
                    this.IsScriptMode = true;
                }
            }
        }
        private readonly bool canSQTest = false;

        /// <summary>
        /// 変更されたかどうか
        /// </summary>
        public bool IsDirty {
            get {
                return this.azkEditor.Document.IsDirty;
            }
            set {
                this.azkEditor.Document.IsDirty = value;
            }
        }

        /// <summary>
        /// 編集中のテキスト
        /// </summary>
        public string EdittingText {
            get {
                return this.azkEditor.Text;
            }
            set {
                this.azkEditor.Text = value;
                this.azkEditor.Document.IsDirty = false;
                this.azkEditor.Document.ClearHistory();
                this.watchCtrl();
            }
        }

        /// <summary>
        /// 保存処理
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public bool DoSave() {
            if (this.IsReadOnly) {
                MessageBox.Show(Resources.MsgE_ReadOnly, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            } else if (this.IsDirty && !this.IsReadOnly) {
                //末尾の空白を除去する
                //全行を走査
                for (var i = 0; i < this.azkEditor.Document.LineCount; i++) {
                    var buf = this.azkEditor.Document.GetLineContent(i);
                    var start = this.azkEditor.Document.GetLineHeadIndex(i);
                    var end = start + buf.Length;

                    //末尾の空白を除去
                    var newLine = buf.TrimEnd(new char[] { ' ', '　', '\t' });
                    if (buf != newLine) {
                        //変更がある場合のみ反映
                        this.azkEditor.Document.Replace(newLine, start, end);
                    }
                }

                //保存処理
                Common.WriteFileAll(ref this.fs, this.azkEditor.Text);
                this.IsDirty = false;
            }
            return true;
        }

        /// <summary>
        /// 読み込み処理
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public bool DoLoad(string rPath) {
            this.FileName = rPath;      //ファイルロックor読み取り専用を適用
            this.setFontForMode();      //ファイル種別に従ったフォントを適用
            this.azkEditor.Text = Common.ReadFileAll(this.fs);
            this.azkEditor.SetSelection(0, 0);
            this.azkEditor.ClearHistory();
            this.IsDirty = false;
            this.watchCtrl();
            return true;
        }

        /// <summary>
        /// ファイルを閉じる
        /// </summary>
        public void DoClose() {
            this.FileName = null;
        }

        /// <summary>
        /// テキストエディターをセットアップします。
        /// これはプロジェクトが読み込まれた後に呼び出して下さい。
        /// </summary>
        public void Setup(CtrlComponent.Database.ctlDatabaseEditor dbCtl) {
            this.DBCtrl = dbCtl;
        }

        /// <summary>
        /// Settingsの設定をエディターに反映します。
        /// </summary>
        public void ApplySettings() {
            this.setFontForMode();
            this.azkEditor.HighlightsCurrentLine = Settings.Default.Text_UnderBar;
            this.azkEditor.ForeColor = Settings.Default.Text_TextColor;
            this.azkEditor.BackColor = Settings.Default.Text_BackColor;
            this.azkEditor.ColorScheme.LineNumberBack = this.azkEditor.BackColor;
            this.azkEditor.ColorScheme.LineNumberFore = Settings.Default.Text_CommentColor;
            this.azkEditor.ColorScheme.HighlightColor = Settings.Default.Text_UnderBarColor;
            this.azkEditor.ColorScheme.SetColor(Sgry.Azuki.CharClass.Keyword, Settings.Default.Script_KeywordColor, this.azkEditor.BackColor);
            this.azkEditor.ColorScheme.SetColor(Sgry.Azuki.CharClass.Keyword2, Settings.Default.Script_NameColor1, this.azkEditor.BackColor);
            this.azkEditor.ColorScheme.SetColor(Sgry.Azuki.CharClass.Keyword3, Settings.Default.Script_NameColor2, this.azkEditor.BackColor);
            this.azkEditor.ColorScheme.SetColor(Sgry.Azuki.CharClass.Number, Settings.Default.Script_NumberColor, this.azkEditor.BackColor);
            this.azkEditor.ColorScheme.SetColor(Sgry.Azuki.CharClass.String, Settings.Default.Script_StringColor, this.azkEditor.BackColor);
            this.azkEditor.ColorScheme.SetColor(Sgry.Azuki.CharClass.Comment, Settings.Default.Text_CommentColor, this.azkEditor.BackColor);
        }

        /// <summary>
        /// 標準の色分け設定、コマンド種別の色分けを適用します。
        /// </summary>
        public void ApplyColorSettings() {
            var Highlighter = new Sgry.Azuki.Highlighter.KeywordHighlighter();
            var SQkeywords = Resources.SquirrelKeywords.Replace("\r", "").Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);     //リソースからスクリプトのキーワード一覧を読み込む

            //スクリプトのキーワード情報をコメント等余計な部分を落とす
            for (var i = 0; i < SQkeywords.Length; i++) {
                SQkeywords[i] = SQkeywords[i].Substring(0, SQkeywords[i].IndexOf(";"));
            }

            //キーワード色分け設定
            Highlighter.AddKeywordSet(SQkeywords, Sgry.Azuki.CharClass.Keyword);
            Highlighter.AddKeywordSet(CtrlComponent.Text.ctlCommandTree.CodeContainers.ToArray(), Sgry.Azuki.CharClass.Keyword2);
            Highlighter.AddKeywordSet(CtrlComponent.Text.ctlCommandTree.CodeMembers.ToArray(), Sgry.Azuki.CharClass.Keyword3);

            //コメント色分け設定
            Highlighter.AddEnclosure("\"", "\"", Sgry.Azuki.CharClass.String, false);
            Highlighter.AddEnclosure(Resources.Code_CommentStart, Resources.Code_CommentEnd, Sgry.Azuki.CharClass.Comment, true);
            Highlighter.AddLineHighlight(Resources.Code_Comment1, Sgry.Azuki.CharClass.Comment);

            this.azkEditor.Highlighter = Highlighter;
        }

        /// <summary>
        /// フォントモードに合わせたフォントを反映します。
        /// </summary>
        private void setFontForMode() {
            if (this.IsScriptMode) {
                //スクリプトフォントを適用
                this.azkEditor.Font = Settings.Default.Script_Font;
            } else {
                //テキストフォントを適用
                this.azkEditor.Font = Settings.Default.Text_Font;
            }
        }

        /// <summary>
        /// 保存処理
        /// </summary>
        private void tolSave_Click(object sender, EventArgs e) {
            if (this.DoSave()) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// 元に戻す
        /// </summary>
        private void tolUndo_Click(object sender, EventArgs e) {
            if (this.azkEditor.CanUndo) {
                this.azkEditor.Undo();
            }
        }

        /// <summary>
        /// やり直し
        /// </summary>
        private void tolRedo_Click(object sender, EventArgs e) {
            if (this.azkEditor.CanRedo) {
                this.azkEditor.Redo();
            }
        }

        /// <summary>
        /// 切り取り
        /// </summary>
        private void tolCut_Click(object sender, EventArgs e) {
            if (this.azkEditor.CanCut) {
                this.azkEditor.Cut();
            }
        }

        /// <summary>
        /// コピー
        /// </summary>
        private void tolCopy_Click(object sender, EventArgs e) {
            if (this.azkEditor.CanCopy) {
                this.azkEditor.Copy();
            }
        }

        /// <summary>
        /// 貼り付け
        /// </summary>
        private void tolPaste_Click(object sender, EventArgs e) {
            if (this.azkEditor.CanPaste) {
                this.azkEditor.Paste();
            }
        }

        /// <summary>
        /// ジャンプリスト
        /// </summary>
        private void tolJumpList_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Common.dlgSelectInList("ジャンプリスト");

            //全行の行頭を走査
            for (var i = 0; i < this.azkEditor.Document.LineCount; i++) {
                var buf = this.azkEditor.Document.GetLineContent(i);
                if (buf.Length >= Settings.Default.Text_JumpChar.Length && buf.Substring(0, Settings.Default.Text_JumpChar.Length) == Settings.Default.Text_JumpChar) {
                    Dlg.AddItem(i + 1, buf);
                }
            }

            if (Dlg.ItemCount == 0) {
                //ジャンプリストがない場合は中断
                MessageBox.Show(Resources.MsgE_NothingJumpList, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            } else if (Dlg.ShowDialog() == DialogResult.OK) {
                var line = Dlg.GetResultID();
                this.azkEditor.SetSelection(
                    this.azkEditor.Document.GetLineHeadIndex(line - 1),
                    this.azkEditor.Document.GetLineEndIndexFromCharIndex(this.azkEditor.Document.GetLineHeadIndex(line - 1))
                );
                this.azkEditor.ScrollToCaret();
            }
        }

        /// <summary>
        /// コード補完
        /// </summary>
        private void tolInsertCode_Click(object sender, EventArgs e) {
            if (this.autoComplete.Visible && !this.continueAutoComplete) {
                //既に補完窓が立ち上がっている場合は処理しない
                return;
            }
            this.continueAutoComplete = false;

            var pos = this.azkEditor.PointToScreen(this.azkEditor.GetPositionFromIndex(this.azkEditor.CaretIndex));
            this.azkEditor.Document.GetSelection(out var x, out var y);
            pos.Y += this.azkEditor.LineHeight;      //現在の行の一個下にウィンドウを表示する

            //フォントをエディターに一致させる
            this.txtAutoCompleteInserter.Font = this.azkEditor.Font;
            this.txtAutoCompleteInserter.Text = "";

            //左辺のオブジェクトを認識して候補を絞る
            var addDotFlag = false;
            var word = "";
            TreeNode startNode = null;      //絞り込みの根幹ノード

            if (x > 1 && this.azkEditor.Document.GetTextInRange(x - 1, x) == ".") {
                // [.] の左側の単語を抽出する
                word = this.azkEditor.Document.GetTextInRange(
                    this.azkEditor.Document.WordProc.PrevWordStart(this.azkEditor.Document, x - 2), x - 1)
                    .Replace("\r\n", "");
            } else if (x > 0) {
                //カーソル部分の単語を抽出する
                word = this.azkEditor.Document.GetTextInRange(
                    this.azkEditor.Document.WordProc.PrevWordStart(this.azkEditor.Document, x - 1),
                    this.azkEditor.Document.WordProc.NextWordStart(this.azkEditor.Document, x))
                    .Replace("\r\n", "");
                if (!string.IsNullOrEmpty(word)) {
                    //カーソルを単語の末尾に移動して [.] を付加する
                    var index = this.azkEditor.Document.WordProc.NextWordStart(this.azkEditor.Document, x);
                    this.azkEditor.Document.SetSelection(index, index);
                    addDotFlag = true;
                }
            }
            if (!string.IsNullOrEmpty(word)) {
                //単語を抽出できたら検索する
                foreach (var node in ctlCommandTree.AutoCompleteSource) {
                    var buf = ctlCommandTree.GetCode(node);
                    if (buf != word) {
                        continue;
                    }

                    //色分けされる対象で、かつコンテナー型のときは絞り込みができる
                    var obj = (node.Tag as SQObject);
                    if (obj == null) {
                        continue;
                    }
                    switch (obj.SubType) {
                        case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Enum:
                        case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberProperty:
                        case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.GlobalValue:
                            startNode = node;
                            break;
                    }
                    break;
                }
            }

            //オートコンプリートのソースをセットする
            this.txtAutoCompleteInserter.AutoCompleteMode = AutoCompleteMode.None;       //フリーズ不具合防止のため、セット直前で一旦Noneにする必要がある
            if (startNode == null) {
                //全体検索: 予め生成された全リスト（の参照）に差し替える
                this.txtAutoCompleteInserter.AutoCompleteCustomSource = ctlCommandTree.AutoCompleteSourceAll;
            } else {
                //絞り込み検索: ここでリストを生成して、その参照に差し替える
                var tempList = new AutoCompleteStringCollection();
                foreach (TreeNode node in startNode.Nodes) {
                    tempList.Add(ctlCommandTree.GetCode(node));
                }
                this.txtAutoCompleteInserter.AutoCompleteCustomSource = tempList;

                //現在の単語に続けて入力する場合は [.] をソースコードに自動付加する
                if (addDotFlag) {
                    this.azkEditor.Document.Replace(".");
                }
            }
            this.txtAutoCompleteInserter.AutoCompleteMode = AutoCompleteMode.Suggest;

            //補完ウィンドウを開く
            this.autoComplete.Show(pos);
            if (x != y) {
                //テキストが選択状態のときはそれを入力した状態で開始する
                this.forbiddenAutoFindCode = true;
                this.txtAutoCompleteInserter.Text = this.azkEditor.Document.GetTextInRange(x, y);
                this.forbiddenAutoFindCode = false;
            }
            this.txtAutoCompleteInserter.Focus();
        }

        /// <summary>
        /// コード補完：入力欄に入ったらカーソルを末尾に移動させる
        /// </summary>
        private void autoCompleteTextBox_Enter(object sender, EventArgs e) {
            var ctl = (sender as ToolStripTextBox);
            if (ctl != null) {
                ctl.SelectionStart = ctl.Text.Length;
                ctl.SelectionLength = 0;
            }
        }

        /// <summary>
        /// コード補完：閉じる
        /// </summary>
        private void autoCompleteTextBox_KeyDown(object sender, KeyEventArgs e) {
            switch (e.KeyCode) {
                case Keys.Escape:
                    //キャンセル
                    this.txtAutoCompleteInserter.Text = "";
                    this.autoComplete.Close();
                    break;

                case Keys.Enter:
                    //確定して挿入する
                    this.azkEditor.Document.Replace(this.txtAutoCompleteInserter.Text);
                    this.txtAutoCompleteInserter.Text = "";
                    this.autoComplete.Close();
                    break;
            }
        }

        /// <summary>
        /// コード補完：挿入欄で入力があったときに解説を表示する
        /// </summary>
        private void autoCompleteTextBox_TextChanged(object sender, EventArgs e) {
            this.txtCodeTypeName.Text = "";
            this.txtCodeComment.Text = "";
            this.txtCodeComment.ToolTipText = "";
            if (string.IsNullOrEmpty(this.txtAutoCompleteInserter.Text) || this.forbiddenAutoFindCode) {
                //入力されていない場合 or 入力時の自動検索が禁止されているとき は中断
                return;
            }

            if (this.txtAutoCompleteInserter.Text.Substring(this.txtAutoCompleteInserter.Text.Length - 1, 1) == ".") {
                //末尾に . を打つとそこまでを確定して続行する
                this.azkEditor.Document.Replace(this.txtAutoCompleteInserter.Text);
                this.txtAutoCompleteInserter.Text = "";
                this.continueAutoComplete = true;
                this.tolInsertCode_Click(sender, e);
                return;
            }

            foreach (var item in ctlCommandTree.AutoCompleteSource) {
                if (this.txtAutoCompleteInserter.Text == ctlCommandTree.GetCode(item)) {
                    //識別子で一致したらそのオブジェクトの解説を表示する

                    //種別表示
                    this.txtCodeComment.Text = "";
                    var obj = (item.Tag as SQObject);
                    if (obj != null) {
                        switch (obj.NodeID) {
                            case GenerateGlueCode.GenerateCPPDocument.ObjectType.SQEventHandler:
                                this.txtCodeTypeName.Text += "【イベントハンドラー】";
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectType.Const:
                                this.txtCodeTypeName.Text += "【定数: " + obj.ReturnType + "】";
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectType.Value:
                                this.txtCodeTypeName.Text += "【変数: " + obj.ReturnType + "】";
                                break;

                            default:
                                switch (obj.SubType) {
                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class:
                                        this.txtCodeTypeName.Text += "【クラス】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassFunc:
                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.GlobalFunc:
                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberFunc:
                                        this.txtCodeTypeName.Text += "【関数: " + obj.ReturnType + "】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassValue:
                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.GlobalValue:
                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberValue:
                                        this.txtCodeTypeName.Text += "【変数: " + obj.ReturnType + "】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberProperty:
                                        this.txtCodeTypeName.Text += "【プロパティ: " + obj.ReturnType + "】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Enum:
                                        this.txtCodeTypeName.Text += "【列挙体】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnumMember:
                                        this.txtCodeTypeName.Text += "【列挙体メンバー】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQFunc:
                                        this.txtCodeTypeName.Text += "【スクリプト関数】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnum:
                                        this.txtCodeTypeName.Text += "【スクリプト列挙体】";
                                        break;

                                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.EnumMember:
                                        this.txtCodeTypeName.Text += "【スクリプト列挙体メンバー】";
                                        break;
                                }
                                break;
                        }
                    }

                    //種別表示のツールヒントにコマンドツリー上のフルパス
                    this.txtCodeTypeName.ToolTipText = item.FullPath;

                    //コマンドツリーでも表示されている内容を下段に表示する
                    this.txtCodeComment.Text += ctlCommandTree.GetCodeSummary(item);
                    this.txtCodeComment.ToolTipText = ctlCommandTree.GetCodeComment(item);
                    return;
                }
            }
        }

        /// <summary>
        /// コード逆探索
        /// </summary>
        private void tolInsertReserchCode_Click(object sender, EventArgs e) {
            //カーソル部分の単語を抽出する
            var doc = this.azkEditor.Document;
            this.azkEditor.Document.GetSelection(out var x, out var y);
            string word, line;

            try {
                line = Common.CutHeadTabs(this.azkEditor.Document.GetLineContent(
                    this.azkEditor.Document.GetLineIndexFromCharIndex(this.azkEditor.Document.AnchorIndex)))
                    .Trim();

                if (x == y && x > 0) {
                    //カーソルにある単語を選択状態にする
                    word = this.azkEditor.Document.GetTextInRange(
                        this.azkEditor.Document.WordProc.PrevWordStart(this.azkEditor.Document, x - 1),
                        this.azkEditor.Document.WordProc.NextWordStart(this.azkEditor.Document, x))
                        .Replace("\r\n", "");
                    this.azkEditor.Document.SetSelection(
                        this.azkEditor.Document.WordProc.PrevWordStart(this.azkEditor.Document, x - 1),
                        this.azkEditor.Document.WordProc.NextWordStart(this.azkEditor.Document, x)
                    );
                } else {
                    word = this.azkEditor.Document.GetTextInRange(x, y);
                }

                if (string.IsNullOrEmpty(word)) {
                    //見つからなかった
                    throw new Exception();
                } else if (word.Length > 2 && word.Substring(0, 2) == "0x") {
                    //16進数と見られる文字列は接頭辞を削除する
                    word = word.Substring(2);
                }

                if (int.TryParse(word, System.Globalization.NumberStyles.AllowHexSpecifier, null, out var id)
                || int.TryParse(word, out id)) {

                    //数値の場合はデータベースのIDとして検索する
                    var list = this.DBCtrl.FindDBRow(id);
                    if (list.Count > 0) {
                        var index = 0;
                        if (list.Count > 1) {
                            //複数見つかった場合は重複解決ダイアログを開く
                            var Dlg = new Dialog.Common.dlgSelectInList("データベースの選択");
                            for (var i = 0; i < list.Count; i++) {
                                Dlg.AddItem(i + 1, list[i].Item2.Tag?.ToString());
                            }
                            if (Dlg.ShowDialog() != DialogResult.OK) {
                                //キャンセル中断
                                return;
                            }
                            index = Dlg.GetResultID();
                        }
                        MessageBox.Show("次のデータベース項目に該当しました:\r\n" + list[index].Item2.Tag?.ToString() + "\r\n" + list[index].Item1, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Information);
                    } else {
                        //見つからなかった
                        throw new Exception();
                    }

                } else if (line.IndexOf(Resources.SQ_EVMessage.Substring(0, Resources.SQ_EVMessage.IndexOf("("))) != -1) {

                    //メッセージ表示のコマンドがある場合
                    this.azkEditor.Document.SetSelection(
                        this.azkEditor.Document.GetLineHeadIndexFromCharIndex(x),
                        this.azkEditor.Document.GetLineEndIndexFromCharIndex(
                            this.azkEditor.Document.GetLineHeadIndex(this.azkEditor.Document.GetLineIndexFromCharIndex(x))
                        ) - 2
                    );
                    this.FoundCommandNode?.Invoke(this, new FoundCommandNodeEventArgs(CtrlComponent.Text.ctlCommandTree.TagToolMessage, line));

                } else if (line.IndexOf(Resources.SQ_ChangeMap.Substring(0, Resources.SQ_ChangeMap.IndexOf("("))) != -1) {

                    //場所移動のコマンドがある場合
                    this.azkEditor.Document.SetSelection(
                        this.azkEditor.Document.GetLineHeadIndexFromCharIndex(x),
                        this.azkEditor.Document.GetLineEndIndexFromCharIndex(
                            this.azkEditor.Document.GetLineHeadIndex(this.azkEditor.Document.GetLineIndexFromCharIndex(x))
                        ) - 2
                    );
                    this.FoundCommandNode?.Invoke(this, new FoundCommandNodeEventArgs(CtrlComponent.Text.ctlCommandTree.TagToolChangeMap, line));

                } else {

                    //文字列の場合はコードとみなしてコマンドツリーで検索にかける
                    var foundNodes = ctlCommandTree.FindNodes(word);
                    var targetIndex = 0;

                    if (foundNodes == null || foundNodes.Count == 0) {
                        //見つからなかった
                        throw new Exception();
                    }

                    if (foundNodes.Count > 1) {
                        // 複数見つかったときはユーザーに選ばせる
                        var Dlg = new dlgSelectInList("複数の候補 - コード逆探索");
                        var count = 1;
                        foreach (var node in foundNodes) {
                            Dlg.AddItem(count, node.FullPath);
                            count++;
                        }
                        if (Dlg.ShowDialog() != DialogResult.OK) {
                            return;
                        }
                        targetIndex = Dlg.GetResultIndex();
                    }

                    //引数リストもすべて選択する
                    var foundNode = foundNodes[targetIndex];
                    var start = line.IndexOf(word) + word.Length;
                    var depth = 0;
                    string[] args = null;

                    if (line.IndexOf("(", start) == start) {
                        //すぐ後ろに引数リストがある場合は選択範囲を伸ばす
                        //ただし、閉じ括弧がないときは伸ばさない
                        for (var index = start + 1; index < line.Length; index++) {
                            var temp = line.Substring(index, 1);
                            if (temp == "(") {
                                depth++;
                                continue;
                            }
                            if (temp == ")") {
                                depth--;
                            }

                            if (depth < 0) {
                                //関数呼び出しの括弧が閉じられたらここで終わる
                                word += line.Substring(start, index - start + 1);

                                //引数リストを取得
                                args = SQ.GetArgumentListByCall(word);

                                //選択範囲を伸ばす
                                this.azkEditor.Document.SetSelection(
                                    this.azkEditor.Document.AnchorIndex,
                                    this.azkEditor.Document.AnchorIndex + word.Length
                                    );
                                break;
                            }
                        }
                    }
                    this.FoundCommandNode?.Invoke(this, new FoundCommandNodeEventArgs(foundNode, args));
                }
            } catch {
                //見つからなかったらビープ音を鳴らす
                System.Media.SystemSounds.Beep.Play();
            }
        }

        /// <summary>
        /// コメントアウト
        /// </summary>
        private void tolDoComment_Click(object sender, EventArgs e) {
            var doc = this.azkEditor.Document;
            this.azkEditor.Document.GetSelection(out var start, out var end);

            if (doc.GetLineIndexFromCharIndex(start) == doc.GetLineIndexFromCharIndex(end)
            && doc.GetLineEndIndexFromCharIndex(end) != end
            && start != end) {
                //単一行内ですべて選択されていない場合は範囲コメントにする
                var buf = doc.GetTextInRange(start, end);
                buf = Resources.Code_CommentStart + buf + Resources.Code_CommentEnd;
                doc.Replace(buf, start, end);
                doc.SetSelection(start, end + (Resources.Code_CommentStart.Length + Resources.Code_CommentEnd.Length));
            } else {
                //行コメント
                if (start < end && end == doc.GetLineHeadIndexFromCharIndex(end)) {
                    //末尾が \r で終わっていたら（＝行全体選択）、前の行末（一文字分前）まで巻き戻す
                    end = doc.GetLineHeadIndex(
                        doc.GetLineIndexFromCharIndex(end) - 1/*行数*/) + doc.GetLineLength(doc.GetLineIndexFromCharIndex(end) - 1/*行数*/) - 1/*\r分のみ*/;
                }

                if (end > 0 && start <= end && doc.GetLineEndIndexFromCharIndex(end) - 2/*末尾の\r\n*/ > 0) {
                    //単一行のとき 0 をとる
                    var lines = doc.GetLineIndexFromCharIndex(end) - doc.GetLineIndexFromCharIndex(start);

                    var buf = doc.GetTextInRange(
                        doc.GetLineHeadIndexFromCharIndex(start),
                        doc.GetLineEndIndexFromCharIndex(end) - 2/*末尾の\r\n*/
                    );

                    buf = Resources.Code_Comment1 + buf;                         //先頭行もコメントアウトする
                    buf = buf.Replace("\r\n", "\r\n" + Resources.Code_Comment1).Replace(Resources.Code_Comment1 + "\r\n", "\r\n");     //改行直後にコメント符号を入れるが、空行はコメントアウトしない
                    doc.Replace(
                        buf,
                        doc.GetLineHeadIndexFromCharIndex(start),
                        doc.GetLineEndIndexFromCharIndex(end) - 2/*末尾の\r\n*/
                    );
                    doc.SetSelection(
                        doc.GetLineHeadIndexFromCharIndex(start),
                        doc.GetLineEndIndexFromCharIndex(doc.GetLineHeadIndex(doc.GetLineIndexFromCharIndex(start) + lines))
                    );
                }
            }
        }

        /// <summary>
        /// コメントアウトを解除
        /// </summary>
        private void tolUndoComment_Click(object sender, EventArgs e) {
            var doc = this.azkEditor.Document;
            var start = (doc.AnchorIndex <= doc.CaretIndex) ? doc.AnchorIndex : doc.CaretIndex;
            var end = (doc.AnchorIndex <= doc.CaretIndex) ? doc.CaretIndex : doc.AnchorIndex;

            if (doc.GetLineIndexFromCharIndex(start) == doc.GetLineIndexFromCharIndex(end)
            && doc.GetLineEndIndexFromCharIndex(end) != end
            && start != end) {
                //単一行で選択範囲があり、かつすべて選択されていない場合は範囲コメントとみなす
                var buf = doc.GetTextInRange(start, end);
                if (buf.Length >= Resources.Code_CommentStart.Length + Resources.Code_CommentEnd.Length
                && buf.Substring(0, Resources.Code_CommentStart.Length) == Resources.Code_CommentStart
                && buf.Substring(buf.Length - Resources.Code_CommentEnd.Length, Resources.Code_CommentEnd.Length) == Resources.Code_CommentEnd) {
                    //コメント部分であることを確認したら、その部分を除去する
                    buf = buf.Substring(Resources.Code_CommentStart.Length);
                    buf = buf.Substring(0, buf.Length - Resources.Code_CommentEnd.Length);
                    doc.Replace(buf, start, end);
                    doc.SetSelection(start, end - (Resources.Code_CommentStart.Length + Resources.Code_CommentEnd.Length));
                }
            } else {
                //行コメント
                if (start < end && end == doc.GetLineHeadIndexFromCharIndex(end)) {
                    //末尾が \r で終わっていたら（＝行全体選択）、前の行末（一文字分前）まで巻き戻す
                    end = doc.GetLineHeadIndex(doc.GetLineIndexFromCharIndex(end) - 1) + doc.GetLineLength(doc.GetLineIndexFromCharIndex(end) - 1) - 1/*\r分のみ*/;
                } else if (start == end) {
                    //選択されていなければ、その行全体を対象とする
                    start = doc.GetLineHeadIndex(doc.GetLineIndexFromCharIndex(start));
                    end = doc.GetLineEndIndexFromCharIndex(start) - 1/*末尾の\r*/;
                }

                if (end > 0 && start < end) {
                    var lines = doc.GetLineIndexFromCharIndex(end) - doc.GetLineIndexFromCharIndex(start);        //単一行のとき０をとる
                    var buf = "";

                    for (var i = 0; i <= lines; i++) {
                        var linestr = doc.GetLineContent(doc.GetLineIndexFromCharIndex(start) + i);

                        if (Common.CutHeadTabs(linestr).Length >= Resources.Code_Comment1.Length
                        && Common.CutHeadTabs(linestr).Substring(0, Resources.Code_Comment1.Length) == Resources.Code_Comment1) {
                            //コメント部分であることを確認したら、その部分を除去する
                            linestr = linestr.Substring(
                                0,
                                linestr.IndexOf(Resources.Code_Comment1)) + linestr.Substring(linestr.IndexOf(Resources.Code_Comment1) + Resources.Code_Comment1.Length
                            );
                        }

                        if (i != lines) {
                            linestr += "\r\n";
                        }
                        buf += linestr;
                    }

                    doc.Replace(
                        buf,
                        doc.GetLineHeadIndexFromCharIndex(start),
                        doc.GetLineEndIndexFromCharIndex(end) - 2/*末尾の\r\n*/
                    );
                    doc.SetSelection(
                        doc.GetLineHeadIndexFromCharIndex(start),
                        doc.GetLineEndIndexFromCharIndex(doc.GetLineHeadIndex(doc.GetLineIndexFromCharIndex(start) + lines))
                    );
                }
            }
        }

        /// <summary>
        /// テストコマンドをドロップダウンしたときに関数リストを更新する
        /// </summary>
        private void tolTestCommand_DropDown(object sender, EventArgs e) {
            //現在のスクリプトの関数リストを生成する
            this.tolTestCommand.BeginUpdate();
            this.tolTestCommand.Items.Clear();

            var list = SQ.GetFunctionList(this.FileName);
            foreach (var item in list) {
                if (item.Text.Length > 0 && item.Text.Substring(0, 1) == "(") {
                    //無名関数はスキップ
                    continue;
                }

                var buf = item.Text + "(";
                var args = item.Tag as string[];

                for (var i = 0; i < args.Length; i++) {
                    buf += "/*" + args[i] + "*/";
                    if (i < args.Length - 1) {
                        buf += ", ";
                    }
                }

                buf += ");";
                this.tolTestCommand.Items.Add(buf);

            }

            this.tolTestCommand.EndUpdate();
        }

        /// <summary>
        /// テストコマンド入力欄をエンターキーでテスト開始
        /// </summary>
        private void tolTestCommand_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyData == Keys.Enter) {
                this.tolSQTest_Click(sender, e);
            }
        }

        /// <summary>
        /// 即席テスト
        /// </summary>
        private void tolSQTest_Click(object sender, EventArgs e) {
            var buf = $"st \"{this.FileName}\" \"{this.tolTestCommand.Text.Replace("\"", "\\\"")}\"";
            Settings.Default.Last_SQTestParameter = this.tolTestCommand.Text;

            //テスト実行
            Common.RunTester(buf, false);
        }

        /// <summary>
        /// 検索と置換
        /// </summary>
        private void ctmFindReplace_Click(object sender, EventArgs e) {
            if (this.Dlg != null && this.Dlg.Visible) {
                this.Dlg.Activate();
                this.Dlg.ResetFindText();
            } else {
                this.Dlg = new Dialog.Text.dlgLocalSearchReplace(this.azkEditor);
                this.Dlg.Show(this);
            }
        }

        /// <summary>
        /// すべて選択
        /// </summary>
        private void ctmSelctAll_Click(object sender, EventArgs e) {
            this.azkEditor.SelectAll();
        }

        /// <summary>
        /// テキストをダブルクリックしたときの処理
        /// </summary>
        private void azkEditor_MouseDoubleClick(object sender, MouseEventArgs e) {
            var data = e as Sgry.Azuki.IMouseEventArgs;
            if (data == null) {
                return;
            }

            var url = "";
            var IDs = this.azkEditor.Document.GetMarkingsAt(data.Index);
            foreach (var id in IDs) {
                switch (id) {
                    default:
                        //システム定義のID
                        if (id == Sgry.Azuki.Marking.Uri) {
                            //URLジャンプ
                            data.Handled = true;
                            url = this.azkEditor.Document.GetMarkedText(data.Index, id);
                            System.Diagnostics.Process.Start(url);
                        }
                        break;
                }
            }
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void watchCtrl() {
            this.tolUndo.Enabled = this.ctmUndo.Enabled = this.azkEditor.CanUndo;
            this.tolRedo.Enabled = this.ctmRedo.Enabled = this.azkEditor.CanRedo;
            //tolCut.Enabled = ctmCut.Enabled = azkEditor.CanCut;
            //tolCopy.Enabled = ctmCopy.Enabled = azkEditor.CanCopy;
            //tolPaste.Enabled = ctmPaste.Enabled = azkEditor.CanPaste;
        }
    }
}
