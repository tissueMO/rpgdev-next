using Sgry.Azuki.WinForms;
using System;
using System.Windows.Forms;

namespace Editor.Dialog.Text {
    using Properties;

    public partial class dlgLocalSearchReplace : Form {
        private readonly AzukiControl ctl = null;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgLocalSearchReplace(AzukiControl ctl) {
            this.InitializeComponent();
            this.txtFind.Focus();
            this.ctl = ctl;
            this.ResetFindText();

            //設定を復元
            this.chkCase.Checked = Settings.Default.Last_TextSearchCase;
            this.chkRegux.Checked = Settings.Default.Last_TextSearchRegux;
        }

        /// <summary>
        /// ウィンドウ消去時
        /// </summary>
        private void dlgLocalSearchReplace_FormClosing(object sender, FormClosingEventArgs e) {
            this.unmark();

            //設定を保存
            Settings.Default.Last_TextSearchCase = this.chkCase.Checked;
            Settings.Default.Last_TextSearchRegux = this.chkRegux.Checked;
        }

        /// <summary>
        /// 既定の検索文字列をセットする
        /// </summary>
        public void ResetFindText() {
            var selectedText = this.ctl.Document.GetTextInRange(this.ctl.Document.AnchorIndex, this.ctl.Document.CaretIndex);

            if (!string.IsNullOrEmpty(selectedText)) {
                //現在の選択文字列を取り出す
                this.txtFind.Text = selectedText;
            } else if (Clipboard.GetText() != null) {
                //クリップボード文字列を取り出す
                this.txtFind.Text = Clipboard.GetText();
            } else {
                //何も情報がなければ空白にする
                this.txtFind.Text = "";
            }

            this.txtFind.SelectAll();
        }

        /// <summary>
        /// 閉じるボタン（画面外）
        /// </summary>
        private void btnClose_Click(object sender, EventArgs e) {
            this.Close();
        }

        /// <summary>
        /// 次を検索
        /// </summary>
        private void btnFindNext_Click(object sender, EventArgs e) {
            //ハイライト初期化
            this.ctl.Document.Unmark(0, this.ctl.Document.Length, CtrlComponent.Text.ctlOneTextEditor.FindMarkID);
            this.ctl.Document.WatchPatterns.Unregister(CtrlComponent.Text.ctlOneTextEditor.FindMarkID);
            this.ctl.Refresh();

            Sgry.Azuki.SearchResult result;
            try {
                if (!this.chkRegux.Checked) {
                    //通常検索
                    result = this.ctl.Document.FindNext(this.txtFind.Text, this.ctl.Document.CaretIndex);
                } else {
                    //正規表現
                    result = this.ctl.Document.FindNext(
                        new System.Text.RegularExpressions.Regex(
                            this.txtFind.Text,
                            this.chkCase.Checked ? System.Text.RegularExpressions.RegexOptions.None : System.Text.RegularExpressions.RegexOptions.IgnoreCase),
                        this.ctl.Document.CaretIndex
                    );
                }
            } catch {
                //正規表現に問題があるなどして検索できなかった
                System.Media.SystemSounds.Exclamation.Play();
                return;
            }

            //検索結果を画面に反映
            if (!string.IsNullOrEmpty(this.txtFind.Text)) {
                try {
                    var pattern = this.txtFind.Text;
                    if (!this.chkRegux.Checked) {
                        //通常検索: 正規表現におけるメタ文字になってしまう文字をエスケープする
                        for (var i = 0; i < pattern.Length; i++) {
                            if (pattern.Substring(i, 1).IndexOfAny(new char[] { '$', '(', ')', '*', '+', '.', '[', ']', '?', '\\', '^', '{', '}', '|' }) != -1) {
                                pattern = pattern.Insert(i, @"\");
                                i++;
                            }
                        }
                    }
                    this.ctl.Document.WatchPatterns.Register(new Sgry.Azuki.WatchPattern(CtrlComponent.Text.ctlOneTextEditor.FindMarkID, new System.Text.RegularExpressions.Regex(pattern, (this.chkCase.Checked ? System.Text.RegularExpressions.RegexOptions.None : System.Text.RegularExpressions.RegexOptions.IgnoreCase))));
                } catch {
                    //正規表現に問題があるなどして検索できなかった
                    System.Media.SystemSounds.Exclamation.Play();
                    return;
                }
            }

            //ハイライトを反映
            this.ctl.Refresh();

            //ヒット位置に移動
            if (result != null) {
                this.ctl.SetSelection(result.Begin, result.End);
                this.ctl.ScrollToCaret();
            } else {
                //検索終端に達したらカーソルを先頭に戻す
                System.Media.SystemSounds.Asterisk.Play();
                Sgry.Azuki.Actions.MoveToFileHead(this.ctl);
            }
        }

        /// <summary>
        /// 置換して次へ
        /// </summary>
        private void btnReplaceOne_Click(object sender, EventArgs e) {
            //現在の選択テキストに対して実行、検索テキストであるかどうかは検証しない
            var selectedText = this.ctl.Document.GetTextInRange(this.ctl.Document.AnchorIndex, this.ctl.Document.CaretIndex);
            if (selectedText.Length > 0) {
                this.ctl.Document.Replace(this.txtReplace.Text);
            }

            //次を検索
            this.btnFindNext_Click(null, null);
        }

        /// <summary>
        /// すべて置換
        /// </summary>
        private void btnReplaceAll_Click(object sender, EventArgs e) {
            //カーソルを先頭に戻す
            Sgry.Azuki.Actions.MoveToFileHead(this.ctl);

            //次を検索
            var index = this.ctl.Document.CaretIndex;
            this.btnFindNext_Click(null, null);

            //カーソル位置が上に行くまで置換を繰り返す
            while (index < this.ctl.Document.CaretIndex) {
                index = this.ctl.Document.CaretIndex;
                this.btnReplaceOne_Click(null, null);
            }
            this.unmark();
        }

        /// <summary>
        /// ハイライトを解除
        /// </summary>
        private void unmark() {
            this.ctl.Document.Unmark(0, this.ctl.Document.Length, CtrlComponent.Text.ctlOneTextEditor.FindMarkID);
            this.ctl.Document.WatchPatterns.Unregister(CtrlComponent.Text.ctlOneTextEditor.FindMarkID);
            this.ctl.Refresh();      //ハイライトを反映
        }
    }
}
