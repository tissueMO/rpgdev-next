using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module;
    using Properties;
    using ItemType = KeyValuePair<string, Tuple<int, string>>;      //コードとインデントの深さを保持する

    public partial class ctlEVEasyEditor : UserControl, IDirtyable {
        private static List<string> Clipboard = null;
        private CtrlComponent.Text.ctlCommandTree comTree;
        private const string MarkBlockStart = "◆";
        private const string MarkBlockEnd = "：";
        private const string MarkUnknown = "？";
        private const string MarkNormal = "■";
        private const string MarkComment = "※";
        private static readonly char[] codeSplitMarks = new char[] { '\t', ' ', '.', ',', ':', '(', ')', '{', '}', ';', '=', '<', '>', '!' };

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlEVEasyEditor() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            Common.SetListControlKeyValuePairMode(this.lstEditor);       //エディターに裏の顔（実際の生コード）を持たせる
        }

        /// <summary>
        /// このコントロールを使える状態にします。
        /// </summary>
        public void Setup(CtrlComponent.Text.ctlCommandTree comTree) {
            this.comTree = comTree;
        }

        /// <summary>
        /// 現在の編集行番号
        /// </summary>
        public int CurrentIndex {
            get {
                if (this.lstEditor.SelectedIndex == -1) {
                    //選択されていないときは先頭行にする
                    return 0;
                } else {
                    return this.lstEditor.SelectedIndex;
                }
            }
            set {
                this.lstEditor.SelectedIndices.Clear();
                if (value < 0) {
                    this.lstEditor.SelectedIndex = 0;
                } else if (this.lstEditor.Items.Count <= value) {
                    this.lstEditor.SelectedIndex = this.lstEditor.Items.Count - 1;
                } else {
                    this.lstEditor.SelectedIndex = value;
                }
            }
        }

        /// <summary>
        /// 変更されているかどうか
        /// </summary>
        public bool IsDirty {
            get; set;
        }

        /// <summary>
        /// スクリプトの編集を開始します。
        /// </summary>
        public void SetTextAll(string str) {
            this.lstEditor.Items.Clear();
            if (str == null) {
                //文字列がnullのときは明示的に空白文字列にする
                str = "";
            }

            //改行で区切り、各行を逐語訳してエディターに登録する
            var spl = str.Replace("\r", "").Split('\n');
            foreach (var line in spl) {
                this.InsertLine(-1, line, false, false);
            }

            //末尾には必ず空行を作る
            if (!string.IsNullOrEmpty(spl[spl.Length - 1])) {
                this.InsertLine(-1, "", false, false);
            }

            this.RefreshIndentAll();
            this.IsDirty = false;
        }

        /// <summary>
        /// 編集したスクリプトをテキストにして返します。
        /// </summary>
        public string GetTextAll() {
            var buf = "";

            //末尾の空行は含めない
            for (var i = 0; i < this.lstEditor.Items.Count - 1; i++) {
                var item = ((ItemType) this.lstEditor.Items[i]).Value;

                //インデント用のタブを追加する
                var tabs = "";
                for (var n = 0; n < item.Item1; n++) {
                    tabs += "\t";
                }

                //一行コードが完成する
                buf += tabs;
                buf += item.Item2;
                buf += "\r\n";
            }
            return buf;
        }

        /// <summary>
        /// 指定行にコードを挿入します。
        /// </summary>
        /// <param name="index">-1 にすると末尾の空行を無視して強制的に末尾に追加します。</param>
        /// <param name="code">生コード</param>
        /// <param name="enabledFormat">自動でブロックの始端・終端を認識して全行をフォーマットするかどうか</param>
        /// <param name="forbiddenCRLF">コマンドツリーからの指示で改行加工を禁止するかどうか</param>
        public void InsertLine(int index, string code, bool enabledFormat, bool forbiddenCRLF) {
            var lines = new List<string>();
            if (enabledFormat && forbiddenCRLF == false && code.IndexOf("{") != -1) {
                //挿入時のみ：コードに始端・終端が含まれる場合は複数行にする
                lines.Add(code.Substring(0, code.IndexOf("{") + 1));
                lines.Add("");      //空行を挟む
                lines.Add(code.Substring(code.IndexOf("}")));
            } else {
                //加工しないが、改行コードが含まれる場合は分割する
                var spl = code.Replace("\r", "").Split(new char[] { '\n' });
                lines.AddRange(spl.ToList());
            }

            //挿入する行ごとに見ていく
            for (var i = 0; i < lines.Count; i++) {
                //インデントは仮の値を入れて挿入する
                var buf = this.ApplySQFormat(lines[i]);
                var text = this.ConvertEasyText(buf);
                var newItem = new ItemType(text, new Tuple<int, string>(0, buf));

                if (index == -1) {
                    //末尾追加
                    this.lstEditor.Items.Add(newItem);
                } else {
                    //指定位置に挿入
                    this.lstEditor.Items.Insert(index + i, newItem);
                }
            }

            if (enabledFormat) {
                //自動フォーマットモードのときはここでインデントを更新する
                this.RefreshIndentAll();
            }

            if (index != -1) {
                //ユーザーによる変更のときは変更フラグを立てる
                this.IsDirty = true;
            }

            this.lstEditor.Focus();
        }

        /// <summary>
        /// 指定行のコードを置換します。
        /// </summary>
        /// <param name="code">生コード</param>
        public void ReplaceLine(int index, string code) {
            //インデントを検討
            var indent = this.GetIndent(index, code);

            //置換処理
            code = this.ApplySQFormat(code);
            var text = this.ConvertEasyText(code);
            var newItem = new ItemType(text, new Tuple<int, string>(indent, code));
            this.lstEditor.Items[index] = newItem;

            this.IsDirty = true;
        }

        /// <summary>
        /// 指定位置のインデントを返します。
        /// </summary>
        private int GetIndent(int index, string code) {
            var indent = 0;
            var destidx = (index == -1) ? this.lstEditor.Items.Count : index;

            if (destidx > 0) {
                //一つ前の行を見る
                var prevLine = ((ItemType) this.lstEditor.Items[destidx - 1]).Value;

                //行内完結ブロック {...} は最初の判定で引っかかる
                if (code.IndexOf("}") != -1 && prevLine.Item2.IndexOf("{") == -1) {
                    //一つ前の行に始端記号がある場合、最後に始まっていることを確認する（ただ単に {...} で囲われているブロックはインデントの対象としない）
                    if (prevLine.Item2.IndexOf("}") != -1 && prevLine.Item2.IndexOf("{") <= prevLine.Item2.IndexOf("}")) {
                        //一つ前の行は単なる行内完結ブロックなのでインデントを引き継ぐ
                        indent = prevLine.Item1;
                    } else if (code.IndexOf("{") != -1 && code.IndexOf("{") <= code.IndexOf("}")) {
                        //この行は単なる行内完結ブロックなのでインデントを引き継ぐ
                        indent = prevLine.Item1;
                    } else {
                        //この行が終端記号でインデントを戻す場合（ブロック内に一行もない場合を除く）
                        indent = prevLine.Item1 - 1;
                    }
                } else if (code.IndexOf("}") == -1 && prevLine.Item2.IndexOf("{") != -1) {
                    //一つ前の行に始端記号がある場合、最後に始まっていることを確認する（ただ単に {...} で囲われているブロックはインデントの対象としない）
                    if (prevLine.Item2.IndexOf("}") != -1 && prevLine.Item2.IndexOf("{") <= prevLine.Item2.IndexOf("}")) {
                        //一つ前の行は単なる行内完結ブロックなのでインデントを引き継ぐ
                        indent = prevLine.Item1;
                    } else {
                        //一つ前の行が始端記号でインデントが深まる場合（ブロック内に一行もない場合を除く）
                        indent = prevLine.Item1 + 1;
                    }
                } else {
                    //現在のインデントを引き継ぐ
                    indent = prevLine.Item1;
                }
            }

            return indent;
        }

        /// <summary>
        /// すべての行のインデントを更新します。
        /// </summary>
        private void RefreshIndentAll() {
            for (var i = 0; i < this.lstEditor.Items.Count; i++) {
                var rawCode = this.GetLineRawCode(i);
                this.lstEditor.Items[i] = new ItemType(((ItemType) this.lstEditor.Items[i]).Key, new Tuple<int, string>(this.GetIndent(i, rawCode), rawCode));
            }
        }

        /// <summary>
        /// 指定したスクリプト行を編集用のコードに整形します。
        /// </summary>
        public string ApplySQFormat(string line) {
            var buf = line;

            //インデント削除
            buf = Common.CutHeadTabs(buf).TrimStart();

            return buf;
        }

        /// <summary>
        /// 指定したスクリプト行をEasyエディターの表示用テキストにして返します。
        /// ここではインデントを考慮しません。
        /// </summary>
        public string ConvertEasyText(string line) {
            var unknownFlag = false;
            var commentFlag = false;
            var blockStartFlag = false;
            var blockEndFlag = false;
            var emptyFlag = string.IsNullOrEmpty(line);
            var buf = "";
            var temp = "";

            if (line.IndexOf("//") == 0) {
                //コメント行はそのまま表示する
                buf = line.Substring(2);
                commentFlag = true;
            } else {
                //コード行は逐語訳する: CodeをSummaryに置き換える
                unknownFlag = true;

                //Codeを識別子ごとに分割する
                while (!string.IsNullOrEmpty(line)) {
                    //同一行内の次のコマンドを抜き取る
                    temp = this.GetSubCode(ref line);

                    if (temp.Length <= 1) {
                        //１文字の識別子・演算子は逐語訳せずにそのまま扱う
                        //ブロック起点・終端のチェック
                        if (temp == "{") {
                            buf += "始端";
                            blockStartFlag = true;
                        } else if (temp == "}") {
                            buf += "終端";
                            blockEndFlag = true;
                        } else {
                            buf += temp;
                        }
                        continue;
                    } else if (temp.Substring(0, 2) == "/*") {
                        //コメント部分はそのまま扱う
                        buf += temp;
                        continue;
                    }

                    //リテラル値：数値・文字列を検証する
                    if (int.TryParse(temp, out var val)) {
                        //整数値リテラル
                        buf += temp;
                        continue;
                    } else if (temp.IndexOf("\"") == 0 && temp.IndexOf("\"", 1) == temp.Length - 1) {
                        //文字列リテラル
                        buf += temp;
                        continue;
                    }

                    //単一識別子をオートコンプリートソースから検索する
                    var found = false;
                    foreach (var acword in CtrlComponent.Text.ctlCommandTree.AutoCompleteSource) {
                        var code = CtrlComponent.Text.ctlCommandTree.GetCode(acword);
                        var summary = CtrlComponent.Text.ctlCommandTree.GetCodeSummary(acword);

                        //識別子以外の文字列を含むコードはその文字列が現れる手前までを実質的なコードとする
                        var idx = code.IndexOfAny(ctlEVEasyEditor.codeSplitMarks);
                        if (idx != -1) {
                            code = code.Substring(0, idx);
                        }

                        if (code.Length > 1 && temp == code) {
                            //２文字以上の単語を見つけたら置換する
                            unknownFlag = false;
                            if (!string.IsNullOrEmpty(summary)) {
                                buf += summary;
                            } else {
                                //Summaryが未定義の場合は識別子に[]を付加するだけにする
                                buf += "[" + code + "]";
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        //該当しなかった場合、そのまま付加する
                        buf += temp;
                    }
                }
            }

            //先頭記号を付ける
            if (emptyFlag) {
                //空行はノーマーク
                buf = "　";
            } else if (commentFlag) {
                //コメント行
                buf = MarkComment + buf;
            } else if (blockEndFlag) {
                //ブロック終端
                buf = MarkBlockEnd + buf;
            } else if (blockStartFlag) {
                //ブロック起点
                buf = MarkBlockStart + buf;
            } else if (unknownFlag) {
                //不明なコード
                buf = MarkUnknown + buf;
            } else {
                //通常の行
                buf = MarkNormal + buf;
            }

            return buf;
        }

        /// <summary>
        /// スクリプト行から逐次的に次のコマンド（単語単位）を抜き取ります。
        /// </summary>
        private string GetSubCode(ref string line) {
            var buf = "";
            int start;

            //先頭が行コメントである場合は末尾まで一挙に抜き取る
            start = line.IndexOf("//");
            if (start == 0) {
                buf = line;
                line = "";
                return buf;
            }

            //先頭が範囲コメントである場合はコメント部分を一挙に抜き取る
            start = line.IndexOf("/*");
            if (start == 0) {
                var end = line.IndexOf("*/");
                if (end == -1) {
                    //範囲コメントが複数行にまたがる場合は先頭行のコメント部分が認識され、以降はコード行に戻される
                    buf = line;
                    line = "";
                } else {
                    buf = line.Substring(start, end - start + 2);
                    line = line.Remove(start, end - start + 2);
                }
                return buf;
            }

            //先頭がダブルクォートである場合は、文字列部分を一挙に抜き取る
            start = line.IndexOf("\"");
            if (start == 0) {
                var end = line.IndexOf("\"", start + 1);
                if (end == -1) {
                    //文字列が閉じられていない場合は行末までを文字列とみなす
                    buf = line;
                    line = "";
                } else {
                    buf = line.Substring(start, end - start + 2);
                    line = line.Remove(start, end - start + 2);
                }
                return buf;
            }

            var idx = line.IndexOfAny(ctlEVEasyEditor.codeSplitMarks);
            if (idx == -1) {
                //すべて抜き取る
                buf = line;
                line = "";
            } else if (idx == 0) {
                //先頭に区切り記号がある場合はその区切り記号を抜き取る
                buf = line.Substring(0, 1);
                line = line.Remove(0, 1);
            } else {
                //次の区切り記号の直前までを抜き取る
                buf = line.Substring(0, idx);
                line = line.Remove(0, idx);
            }

            return buf;
        }

        /// <summary>
        /// 指定行の生コードを取得します。
        /// </summary>
        private string GetLineRawCode(int index) {
            return ((ItemType) this.lstEditor.Items[index]).Value.Item2;
        }

        /// <summary>
        /// 行をダブルクリックで編集する
        /// </summary>
        private void lstEditor_DoubleClick(object sender, EventArgs e) {
            if ((Control.ModifierKeys & Keys.Shift) == Keys.Shift) {
                //Shiftキー押下状態で挿入モード
                this.ctmInsert_Click(sender, e);
            } else {
                //それ以外は置換モード
                this.ctmEdit_Click(sender, e);
            }
        }

        /// <summary>
        /// 対象行を直接編集する
        /// </summary>
        private void ctmEdit_Click(object sender, EventArgs e) {
            if (this.CurrentIndex == this.lstEditor.Items.Count - 1) {
                //末尾の項目は削除対象にできないので挿入モードにする
                this.ctmInsert_Click(sender, e);
                return;
            }

            //特殊なコードは専門のダイアログで編集できるようにする
            var code = this.GetLineRawCode(this.CurrentIndex);
            CtrlComponent.Text.ctlOneTextEditor.FoundCommandNodeEventArgs argData;
            string newCode;

            if (code.IndexOf(Resources.SQ_EVMessage.Substring(0, Resources.SQ_EVMessage.IndexOf("("))) != -1) {
                //メッセージ表示のコマンド
                argData = new CtrlComponent.Text.ctlOneTextEditor.FoundCommandNodeEventArgs(CtrlComponent.Text.ctlCommandTree.TagToolMessage, code);
                newCode = Common.EditCommandByTool(this.comTree.dbCtl.DBList, argData);
                if (!string.IsNullOrEmpty(newCode)) {
                    this.ReplaceLine(this.CurrentIndex, newCode);
                }
            } else if (code.IndexOf(Resources.SQ_ChangeMap.Substring(0, Resources.SQ_ChangeMap.IndexOf("("))) != -1) {
                //場所移動のコマンド
                argData = new CtrlComponent.Text.ctlOneTextEditor.FoundCommandNodeEventArgs(CtrlComponent.Text.ctlCommandTree.TagToolChangeMap, code);
                newCode = Common.EditCommandByTool(this.comTree.dbCtl.DBList, argData);
                if (!string.IsNullOrEmpty(newCode)) {
                    this.ReplaceLine(this.CurrentIndex, newCode);
                }
            } else {
                //選択先頭行を直接入力で編集する
                var dlg = new Dialog.Text.Script.dlgInsertCommand(
                        this.comTree,
                        "直接編集モード：置換",
                        "タイプミスには十分注意して下さい。\r\n入力欄はスクリプト表記になっています。",
                        "",
                        code);
                if (dlg.ShowDialog() == DialogResult.OK) {
                    this.ReplaceLine(this.CurrentIndex, dlg.Result);
                    this.RefreshIndentAll();
                }
            }
        }

        /// <summary>
        /// 対象行に挿入する
        /// </summary>
        private void ctmInsert_Click(object sender, EventArgs e) {
            //選択先頭行で挿入する
            var dlg = new Dialog.Text.Script.dlgInsertCommand(
                        this.comTree,
                        "直接編集モード：挿入",
                        "タイプミスには十分注意して下さい。\r\n入力欄はスクリプト表記になっています。",
                        "", "");
            if (dlg.ShowDialog() == DialogResult.OK) {
                this.InsertLine(this.CurrentIndex, dlg.Result, true, true);
                this.RefreshIndentAll();
            }
        }

        /// <summary>
        /// 切り取り
        /// </summary>
        private void ctmCut_Click(object sender, EventArgs e) {
            this.ctmCopy_Click(sender, e);
            this.ctmDelete_Click(sender, e);
        }

        /// <summary>
        /// コピー
        /// </summary>
        private void ctmCopy_Click(object sender, EventArgs e) {
            if (this.lstEditor.SelectedIndices.Count > 0) {
                Clipboard = new List<string>();
                foreach (ItemType line in this.lstEditor.SelectedItems) {
                    //生コードをコピーする
                    Clipboard.Add(line.Value.Item2);
                }
            }
        }

        /// <summary>
        /// 貼り付け
        /// </summary>
        private void ctmPaste_Click(object sender, EventArgs e) {
            if (this.lstEditor.SelectedIndices.Count == 0 || Clipboard == null) {
                return;
            }

            //生コードを変換して貼り付ける
            var index = this.lstEditor.SelectedIndices[0];
            for (var i = 0; i < Clipboard.Count; i++) {
                this.InsertLine(index + i, Clipboard[i], false, false);
            }

            //貼り付けたコードを選択状態にする
            this.lstEditor.SelectedIndices.Clear();
            for (var i = index; i < index + Clipboard.Count; i++) {
                this.lstEditor.SelectedIndices.Add(i);
            }
            this.RefreshIndentAll();
        }

        /// <summary>
        /// 削除
        /// </summary>
        private void ctmDelete_Click(object sender, EventArgs e) {
            //現在のインデックスを記憶する
            var idx = this.CurrentIndex;

            //削除処理
            while (this.lstEditor.SelectedIndices.Count > 0) {
                if (this.lstEditor.SelectedIndices[0] == this.lstEditor.Items.Count - 1) {
                    //末尾の項目は削除対象にできない
                    this.lstEditor.SelectedIndices.Remove(this.lstEditor.SelectedIndices[0]);
                } else {
                    this.lstEditor.Items.RemoveAt(this.lstEditor.SelectedIndices[0]);
                }
            }

            //現在の行を再選択
            this.CurrentIndex = (idx < this.lstEditor.Items.Count) ? idx : this.lstEditor.Items.Count - 1;
            this.RefreshIndentAll();
            this.IsDirty = true;
        }

        /// <summary>
        /// すべて選択
        /// </summary>
        private void ctmSelctAll_Click(object sender, EventArgs e) {
            this.lstEditor.SelectedIndices.Clear();
            for (var i = 0; i < this.lstEditor.Items.Count; i++) {
                this.lstEditor.SelectedIndices.Add(i);
            }
        }

        /// <summary>
        /// エディターを描画する
        /// </summary>
        private void lstEditor_DrawItem(object sender, DrawItemEventArgs e) {
            Common.ApplyHighQualityDrawing(e.Graphics);
            e.DrawBackground();     //既定の背景描画

            if ((e.State & DrawItemState.Selected) != DrawItemState.Selected) {
                //選択されていない場合は既定の背景色: しましま模様
                e.Graphics.FillRectangle((e.Index % 2 == 1) ? Brushes.White : new SolidBrush(Settings.Default.Text_BackColor), e.Bounds);
            }

            //インデントを適用したテキストにする
            var item = (ItemType) this.lstEditor.Items[e.Index];
            var text = item.Key;
            for (var i = 0; i < item.Value.Item1; i++) {
                text = "　　" + text;
            }

            //文字列描画
            if (item.Value.Item1 < 0) {
                //インデントエラー行
                e.Graphics.DrawString(text, e.Font, Brushes.Red, e.Bounds);
            } else {
                switch (item.Key.Substring(0, 1)) {
                    case MarkBlockStart:
                    case MarkBlockEnd:
                        //文字色をキーワード色にする
                        e.Graphics.DrawString(text, e.Font, new SolidBrush(Settings.Default.Script_KeywordColor), e.Bounds);
                        break;

                    case MarkComment:
                        //文字色をコメント色にする
                        e.Graphics.DrawString(text, e.Font, new SolidBrush(Settings.Default.Text_CommentColor), e.Bounds);
                        break;

                    case MarkUnknown:
                    case MarkNormal:
                        e.Graphics.DrawString(text, e.Font, Brushes.Black, e.Bounds);
                        break;
                }
            }

            //既定の選択描画
            e.DrawFocusRectangle();
        }
    }
}
