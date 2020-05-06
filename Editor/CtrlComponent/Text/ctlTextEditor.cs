using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Text {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class ctlTextEditor : CtrlComponent.Common.ctlEditorParent {
        public List<Tuple<string, int>> lastLineHistory = new List<Tuple<string, int>>();           //最後に編集していた行をファイルごとに記録するためのプロセス内でのみ有効なコンテナー

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlTextEditor() {
            this.InitializeComponent();

            //並び替え可能なListViewの設定
            this.ltvTextSearchResult.ListViewItemSorter = new ListViewItemComparer(new ListViewItemComparer.ComparerMode[] {
                ListViewItemComparer.ComparerMode.Integer,
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.Integer,
                ListViewItemComparer.ComparerMode.Integer,
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.String
            });

            this.ltvValueCountList.ListViewItemSorter = new ListViewItemComparer(new ListViewItemComparer.ComparerMode[] {
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.Integer
            });

            this.ltvValueReferencesList.ListViewItemSorter = new ListViewItemComparer(new ListViewItemComparer.ComparerMode[] {
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.Integer
            });

            this.ltvScriptFuncList.ListViewItemSorter = new ListViewItemComparer(new ListViewItemComparer.ComparerMode[] {
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.Integer,
                ListViewItemComparer.ComparerMode.Integer,
                ListViewItemComparer.ComparerMode.Integer
            });

            this.ltvScriptValueList.ListViewItemSorter = new ListViewItemComparer(new ListViewItemComparer.ComparerMode[] {
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.String,
                ListViewItemComparer.ComparerMode.Integer
            });
        }

        /// <summary>
        /// テキスト検索の対象範囲
        /// </summary>
        private enum TextSearchListColumns {
            WordLength,
            FileName,
            Row,
            Column,
            IsReplaced,
            SerachWord,
        }

        /// <summary>
        /// ドキュメント検索の範囲
        /// </summary>
        public enum TextFindScope {
            OneDocument,
            OnlyText,
            OnlyScript,
            All,
        }

        /// <summary>
        /// データベースへの参照
        /// </summary>
        public AllDB DBList {
            get; set;
        }

        /// <summary>
        /// 作業中のデータが変更されているかどうか
        /// </summary>
        public override bool IsDirty {
            get {
                if (!this.uctlTextFiles.IsFileSelected) {
                    return false;
                } else {
                    return this.uctlOneText.IsDirty;
                }
            }
            protected set {
                this.uctlOneText.IsDirty = value;
            }
        }

        public override bool DoSave() {
            return this.uctlOneText.DoSave();
        }

        public override void DoClose() {
            var newData = new Tuple<string, int>(this.uctlOneText.FileName, this.uctlOneText.azkEditor.Document.GetLineIndexFromCharIndex(this.uctlOneText.azkEditor.Document.AnchorIndex));

            //閉じる
            this.uctlOneText.DoClose();

            //最後に編集していたカーソル行を保存する
            if (string.IsNullOrEmpty(newData.Item1)) {
                return;
            }

            for (var i = 0; i < this.lastLineHistory.Count; i++) {
                if (this.lastLineHistory[i].Item1 == newData.Item1) {
                    //既存のカーソル行情報を更新
                    this.lastLineHistory[i] = newData;
                    return;
                }
            }

            //新規にカーソル行情報を記録する
            this.lastLineHistory.Add(newData);
        }

        public override bool Jump(string rPath) {
            if (rPath.IndexOf(Resources.Path_Texts) != -1) {
                this.uctlTextFiles.ReloadTree(rPath);
                return true;
            }
            return false;
        }

        public override bool Conflict(string rPath, ProjectManager project) {
            if (rPath == this.uctlOneText.FileName && this.uctlOneText.IsReadOnly) {
                this.ReloadContent(this.uctlTextFiles);
                return true;
            }
            return false;
        }

        /// <summary>
        /// サブコントロールのセットアップを中継する
        /// </summary>
        public void SetupSubControls(CtrlComponent.Database.ctlDatabaseEditor dbCtl, CtrlComponent.Map.mgrMapObject mgrMap) {
            //データベースへの参照をサブコントロールに渡す
            this.uctlComTree.Setup(dbCtl, mgrMap);
            this.uctlOneText.Setup(dbCtl);
        }

        /// <summary>
        /// コマンドツリーからのコマンド挿入を受け付ける
        /// </summary>
        private void uctlComTree_RequestInsertCode(object sender, ctlCommandTree.RequestInsertCodeEventArgs e) {
            if (this.uctlComTree.TopLevelControl as MainWindow == null) {
                //コマンドツリーが出張中（イベント編集画面など）の場合は処理しない
                return;
            }
            Common.InsertCommand(this.uctlOneText, e.Code, e.ForbiddenCRLF);
            this.uctlOneText.azkEditor.Focus();
        }

        /// <summary>
        /// 要求を受けたコマンドツリーを選択する
        /// </summary>
        private void uctlOneText_FoundCommandNode(object sender, ctlOneTextEditor.FoundCommandNodeEventArgs e) {
            Common.FoundCommandNode(this.uctlOneText, this.uctlComTree, this.DBList, e);
            this.uctlOneText.azkEditor.Focus();
        }

        /// <summary>
        /// ファイルツリー選択前
        /// </summary>
        private void uctlTextFiles_NodeBeforeSelect(object sender, TreeViewCancelEventArgs e) {
            //システムによる操作を含めてしまうと、無限確認を引き起こすためスルーする
            if (e.Action != TreeViewAction.Unknown) {
                if (this.confirmSave()) {
                    e.Cancel = true;
                    return;
                }
            }

            //ファイルを閉じる
            this.DoClose();
            this.ctlSwitchTextTree(false);      //選択が解除されると想定してツールバーを無効にする
        }

        /// <summary>
        /// ファイルが変更される前に保存を行う
        /// </summary>
        /// <returns>操作が中断されたかどうか</returns>
        private bool confirmSave() {
            if (Settings.Default.Common_NoConfirmSave) {
                //確認せずに必ず保存する設定の場合
                this.DoSave();
            } else if (this.uctlOneText.FileName != null && this.uctlOneText.IsDirty && !this.uctlOneText.IsReadOnly) {
                switch (MessageBox.Show(Resources.MsgQ_Save.Replace("$", "テキスト"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question)) {
                    case DialogResult.Yes:
                        this.DoSave();
                        break;
                    case DialogResult.Cancel:
                        return true;
                }
            }
            return false;
        }

        /// <summary>
        /// ファイルツリー選択後
        /// </summary>
        private void uctlTextFiles_NodeAfterSelect(object sender, TreeViewEventArgs e) {
            //ツールバーを有効にする
            this.ctlSwitchTextTree(true);

            //コード解析結果をクリアする
            this.ltvScriptFuncList.Items.Clear();
            this.ltvScriptValueList.Items.Clear();

            //選択ファイルを読み込み
            if (this.uctlTextFiles.IsFileSelected) {
                this.uctlOneText.DoLoad(e.Node.FullPath);

                //スクリプトの場合は自動でコード解析を実行する
                if (this.uctlOneText.IsScriptMode && Settings.Default.Script_AutoAnalyze) {
                    this.doCodeAnalyze();
                }

                //前回のカーソル行情報を復元する
                foreach (var data in this.lastLineHistory) {
                    if (data.Item1 == e.Node.FullPath) {
                        this.uctlOneText.Focus();
                        this.uctlOneText.azkEditor.Document.SetCaretIndex((data.Item2 < this.uctlOneText.azkEditor.Document.LineCount) ? data.Item2 : this.uctlOneText.azkEditor.Document.LineCount - 1, 0);
                        this.uctlOneText.azkEditor.ScrollToCaret();
                        break;
                    }
                }
                this.uctlOneText.azkEditor.Focus();
            }
        }

        /// <summary>
        /// 新しいファイルを作成完了したとき
        /// </summary>
        private void uctlTextFiles_AfterCreateNode(object sender, CtrlComponent.Common.FileTreeNodeEventArgs e) {
            if (!e.IsFolder && !e.Failed) {
                //var args = new TreeViewEventArgs(e.SelectedNode, TreeViewAction.ByKeyboard);
                //uctlTextFiles_NodeAfterSelect(sender, args);
            }
        }

        /// <summary>
        /// ファイルの名前を変更したら開き直す
        /// </summary>
        private void uctlTextFiles_RenamedNode(object sender, CtrlComponent.Common.FileTreeNodeEventArgs e) {
            if (!e.IsFolder && !e.Failed) {
                var args = new TreeViewEventArgs(e.SelectedNode, TreeViewAction.ByKeyboard);
                this.uctlTextFiles_NodeAfterSelect(sender, args);
            }
        }

        /// <summary>
        /// ファイルツリーの削除完了
        /// </summary>
        private void uctlTextFiles_AfterDeleteNode(object sender, CtrlComponent.Common.FileTreeNodeEventArgs e) {
            //削除後に自動選択された項目を反映する
            this.uctlTextFiles.FocusToTree();
        }

        /// <summary>
        /// ショートカットキーで保存実行
        /// </summary>
        private void mnuTextSave_Click(object sender, EventArgs e) {
            if (this.uctlOneText.Visible && this.IsDirty && this.uctlOneText.IsReadOnly == false && this.DoSave()) {
                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        /// <summary>
        /// 新しいファイル
        /// </summary>
        private void tolTextNewFile_Click(object sender, EventArgs e) {
            this.uctlTextFiles.CreateFile();
        }

        /// <summary>
        /// 新しいフォルダー
        /// </summary>
        private void tolTextNewFolder_Click(object sender, EventArgs e) {
            this.uctlTextFiles.CreateFolder();
        }

        /// <summary>
        /// ファイル/フォルダーをコピー
        /// </summary>
        private void tolTextCopyFile_Click(object sender, EventArgs e) {
            this.uctlTextFiles.CopySelectedNode();
            this.tolTextPasteFile.Enabled = (Clipboard.GetFileDropList() != null && Clipboard.GetFileDropList().Count > 0);
        }

        /// <summary>
        /// ファイル/フォルダーを貼り付け
        /// </summary>
        private void tolTextPasteFile_Click(object sender, EventArgs e) {
            this.uctlTextFiles.PasteNode();
        }

        /// <summary>
        /// ファイル/フォルダーを削除
        /// </summary>
        private void tolTextDeleteFile_Click(object sender, EventArgs e) {
            //削除前に実行する作業を登録した上で実行する
            this.uctlTextFiles.DeleteSelectedNode(() => {
                //保存ダイアログを表示させずにファイルを閉じる
                this.uctlOneText.IsDirty = false;
                this.uctlOneText.DoClose();
            });
        }

        /// <summary>
        /// ファイルツリーを再読込
        /// </summary>
        private void tolTextReloadFiles_Click(object sender, EventArgs e) {
            this.uctlTextFiles.ReloadTree(this.uctlTextFiles.SelectedNodeFullPath);
        }

        /// <summary>
        /// エクスプローラーでフォルダーを開く
        /// </summary>
        private void tolOpenFolder_Click(object sender, EventArgs e) {
            if (this.uctlTextFiles.IsFolderSelected) {
                System.Diagnostics.Process.Start(this.uctlTextFiles.SelectedNodeFullPathOnFileSystem);
            }
        }

        /// <summary>
        /// ListViewの列ヘッダーをクリックして並び替える
        /// </summary>
        private void ltv_ColumnClick(object sender, ColumnClickEventArgs e) {
            ((sender as ListView).ListViewItemSorter as ListViewItemComparer).ColumnIndex = e.Column;
            (sender as ListView).Sort();
        }

        /// <summary>
        /// 検索範囲を保存する
        /// </summary>
        private void cmbTextSearchRange_SelectedIndexChanged(object sender, EventArgs e) {
            Settings.Default.Last_TextSearchRange = this.cmbTextSearchRange.SelectedIndex;
        }

        /// <summary>
        /// 大文字小文字区別の切り替えを保存
        /// </summary>
        private void chkTextCapital_CheckedChanged(object sender, EventArgs e) {
            Settings.Default.Last_TextSearchCase = this.chkTextCapital.Checked;
        }

        /// <summary>
        /// 正規表現の切り替えを保存
        /// </summary>
        private void chkTextRegex_CheckedChanged(object sender, EventArgs e) {
            Settings.Default.Last_TextSearchRegux = this.chkTextRegex.Checked;
        }

        /// <summary>
        /// 一括検索【非同期】
        /// </summary>
        private void btnTextSearchAll_Click_1(object sender, EventArgs e) {
            this.doSearchAll();
        }

        /// <summary>
        /// 単一置換
        /// </summary>
        private void btnTextReplaceOne_Click(object sender, EventArgs e) {
            if (this.ltvTextSearchResult.SelectedItems.Count > 0) {
                var line = int.Parse(this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.Row].Text) - 1;
                var col = int.Parse(this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.Column].Text) - 1;
                var length = int.Parse(this.ltvTextSearchResult.SelectedItems[0].Text);

                if (this.uctlOneText.azkEditor.GetSelectedText() == this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.SerachWord].Text) {
                    //選択部分がヒットした単語と一致する場合のみ置換する
                    this.uctlOneText.azkEditor.Document.Replace(this.txtTextReplace.Text);
                    this.uctlOneText.azkEditor.Document.SetSelection(this.uctlOneText.azkEditor.Document.CaretIndex - this.txtTextReplace.Text.Length, this.uctlOneText.azkEditor.Document.CaretIndex);
                    this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.IsReplaced].Text = Resources.Text_ReplacedMark;
                    System.Media.SystemSounds.Asterisk.Play();
                } else if (this.uctlOneText.azkEditor.GetSelectedText() == this.txtTextReplace.Text) {
                    //選択部分が置換後の文字列である場合は次に移動する
                    if (this.ltvTextSearchResult.SelectedIndices[0] + 1 < this.ltvTextSearchResult.Items.Count) {
                        var oldindex = this.ltvTextSearchResult.SelectedIndices[0];
                        this.ltvTextSearchResult.SelectedIndices.Clear();
                        this.ltvTextSearchResult.SelectedIndices.Add(oldindex + 1);
                        this.ltvTextSearchResult_DoubleClick(null, null);
                    } else {
                        MessageBox.Show(Resources.MsgI_CompleteTextReplace, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    }
                }
            } else if (this.ltvTextSearchResult.Items.Count > 0) {
                //選択がない場合は先頭を選択
                this.ltvTextSearchResult.SelectedIndices.Add(0);
                this.ltvTextSearchResult_DoubleClick(null, null);
            }
        }

        /// <summary>
        /// 一括置換【非同期】
        /// </summary>
        private void btnTextReplaceAll_Click(object sender, EventArgs e) {
            this.doReplaceAll();
        }

        /// <summary>
        /// 指定した単語にジャンプ
        /// </summary>
        private void ltvTextSearchResult_DoubleClick(object sender, EventArgs e) {
            if (this.ltvTextSearchResult.SelectedItems.Count > 0) {
                //現在のファイルを保存して、次のファイルを開く
                if (this.confirmSave()) {
                    return;
                }

                this.uctlTextFiles.SelectTreeNode(this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.FileName].Text);

                var line = int.Parse(this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.Row].Text) - 1;
                if (this.uctlOneText.azkEditor.Document.LineCount <= line) {
                    MessageBox.Show("無効なジャンプ先です。\r\n検索し直してください。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                var col = int.Parse(this.ltvTextSearchResult.SelectedItems[0].SubItems[(int) TextSearchListColumns.Column].Text) - 1;
                var length = int.Parse(this.ltvTextSearchResult.SelectedItems[0].Text);

                this.uctlOneText.azkEditor.SetSelection(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line) + col, this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line) + col + length);
                this.uctlOneText.azkEditor.ScrollToCaret();
                this.uctlOneText.azkEditor.Focus();
            }
        }

        /// <summary>
        /// 関数・変数リスト生成【非同期】
        /// </summary>
        private void tolTextDoCodeAnalyzer_Click(object sender, EventArgs e) {
            if (this.uctlTextFiles.IsFileSelected == false || Path.GetExtension(this.uctlOneText.FileName) != Resources.Extension_SQ) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "スクリプトファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            this.doCodeAnalyze();
        }

        /// <summary>
        /// コードメトリックス計算【非同期】
        /// </summary>
        private void tolTextDoCodeMetrics_Click(object sender, EventArgs e) {
            if (this.uctlTextFiles.IsFileSelected == false || Path.GetExtension(this.uctlOneText.FileName) != Resources.Extension_SQ) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "スクリプトファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            //行番号の昇順に並び替えないと処理ができない仕様
            var sorter = (this.ltvScriptFuncList.ListViewItemSorter as ListViewItemComparer);
            if (sorter == null) {
                return;
            }
            sorter.ColumnIndex = (int) SQ.FuncListColumns.Row;
            sorter.Order = SortOrder.Ascending;
            this.ltvScriptFuncList.Sort();

            //メトリックス計算実行
            this.doCodeMetrics();
        }

        /// <summary>
        /// 指定した関数にジャンプ
        /// </summary>
        private void ltvScriptFuncList_DoubleClick(object sender, EventArgs e) {
            if (this.ltvScriptFuncList.SelectedItems.Count > 0) {
                var line = int.Parse(this.ltvScriptFuncList.SelectedItems[0].SubItems[(int) SQ.FuncListColumns.Row].Text) - 1;
                if (this.uctlOneText.azkEditor.Document.LineCount <= line) {
                    MessageBox.Show("無効なジャンプ先です。\r\n関数リストを更新して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                this.uctlOneText.azkEditor.SetSelection(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line), this.uctlOneText.azkEditor.Document.GetLineEndIndexFromCharIndex(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line)));
                this.uctlOneText.azkEditor.ScrollToCaret();
                this.uctlOneText.azkEditor.Focus();
            }
        }

        /// <summary>
        /// 指定した変数にジャンプ
        /// </summary>
        private void ltvScriptValueList_DoubleClick(object sender, EventArgs e) {
            if (this.ltvScriptValueList.SelectedItems.Count > 0) {
                var line = int.Parse(this.ltvScriptValueList.SelectedItems[0].SubItems[(int) SQ.ValueListColumns.Row].Text) - 1;
                if (this.uctlOneText.azkEditor.Document.LineCount <= line) {
                    MessageBox.Show("無効なジャンプ先です。\r\n変数リストを更新して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                this.uctlOneText.azkEditor.SetSelection(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line), this.uctlOneText.azkEditor.Document.GetLineEndIndexFromCharIndex(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line)));
                this.uctlOneText.azkEditor.ScrollToCaret();
                this.uctlOneText.azkEditor.Focus();
            }
        }

        /// <summary>
        /// 共通変数の使用回数を調査【非同期】
        /// </summary>
        private void tolScriptValueListCount_Click(object sender, EventArgs e) {
            this.doSearchValues();
        }

        /// <summary>
        /// 指定した変数の使用場所リストを表示
        /// </summary>
        private void tolScriptValueFind_Click(object sender, EventArgs e) {
            this.ltvValueReferencesList.BeginUpdate();
            this.ltvValueReferencesList.Items.Clear();

            if (this.ltvValueCountList.SelectedItems.Count > 0 && this.ltvValueCountList.SelectedItems[0].Tag != null) {
                //変数リスト項目のTagにその使用場所が格納されている	
                var list = this.ltvValueCountList.SelectedItems[0].Tag as List<ListViewItem>;
                for (var i = 0; i < list.Count; i++) {
                    this.ltvValueReferencesList.Items.Add(list[i]);
                }
            }

            this.ltvValueReferencesList.EndUpdate();
        }

        /// <summary>
        /// 指定した変数の使用場所にジャンプ
        /// </summary>
        private void ltvValueSearchResult_DoubleClick(object sender, EventArgs e) {
            if (this.ltvValueReferencesList.SelectedItems.Count > 0) {
                //現在のファイルを保存して、次のファイルを開く
                if (this.confirmSave()) {
                    return;
                }
                this.uctlTextFiles.SelectTreeNode(this.ltvValueReferencesList.SelectedItems[0].SubItems[(int) SQ.DBValueReferenceListColumns.Name].Text);

                var line = int.Parse(this.ltvValueReferencesList.SelectedItems[0].SubItems[(int) SQ.DBValueReferenceListColumns.Row].Text) - 1;
                if (this.uctlOneText.azkEditor.Document.LineCount <= line) {
                    MessageBox.Show("無効なジャンプ先です。\r\nリストを更新して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                this.uctlOneText.azkEditor.SetSelection(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line), this.uctlOneText.azkEditor.Document.GetLineEndIndexFromCharIndex(this.uctlOneText.azkEditor.Document.GetLineHeadIndex(line)));
                this.uctlOneText.azkEditor.ScrollToCaret();
                this.uctlOneText.azkEditor.Focus();
            }
        }

        /// <summary>
        /// テキストファイルの選択に関わるコントロール
        /// </summary>
        private void ctlSwitchTextTree(bool isEnabled) {
            this.tolTextNewFile.Enabled = isEnabled;
            this.tolTextNewFolder.Enabled = isEnabled;
            this.tolTextDeleteFile.Enabled = isEnabled;
            this.tolTextCopyFile.Enabled = isEnabled;
            //tolTextCopyFile.Enabled = isEnabled ? uctlTextFiles.IsFileSelected : false;
            this.tolTextPasteFile.Enabled = isEnabled ? (Clipboard.GetFileDropList() != null && Clipboard.GetFileDropList().Count > 0) : false;
            this.tolOpenFolder.Enabled = this.uctlTextFiles.IsFolderSelected;
        }

        /// <summary>
        /// テキストファイルの一括検索に関わるコントロール
        /// </summary>
        private void ctlSwitchTextSearchReplace(bool isEnabled, bool listClear = true) {
            this.prgTextFindMarqee.Visible = !isEnabled;
            this.tbcTextTools.Enabled = isEnabled;
            this.txtTextSearch.Enabled = isEnabled;
            this.txtTextReplace.Enabled = isEnabled;
            this.cmbTextSearchRange.Enabled = isEnabled;
            this.uctlTextFiles.Enabled = isEnabled;

            if (!isEnabled) {
                this.ltvTextSearchResult.BeginUpdate();
                if (listClear) {
                    this.ltvTextSearchResult.SelectedIndices.Clear();
                    this.ltvTextSearchResult.Items.Clear();
                }
            } else {
                this.ltvTextSearchResult.EndUpdate();
            }
        }

        /// <summary>
        /// スクリプトの関数・変数リスト生成に関わるコントロール
        /// </summary>
        private void ctlSwitchScriptAnalyze(bool isEnabled) {
            this.prgTextFindMarqee.Visible = !isEnabled;
            this.tbcTextTools.Enabled = isEnabled;
            this.tolTextDoCodeAnalyzerList.Enabled = isEnabled;
            this.tolTextDoCodeMetrics.Enabled = isEnabled;
            this.ltvScriptFuncList.Enabled = isEnabled;
            this.ltvScriptValueList.Enabled = isEnabled;

            if (!isEnabled) {
                this.ltvScriptFuncList.BeginUpdate();
                this.ltvScriptFuncList.SelectedIndices.Clear();
                this.ltvScriptFuncList.Items.Clear();

                this.ltvScriptValueList.BeginUpdate();
                this.ltvScriptValueList.SelectedIndices.Clear();
                this.ltvScriptValueList.Items.Clear();
            } else {
                this.ltvScriptFuncList.EndUpdate();
                this.ltvScriptValueList.EndUpdate();
            }
        }

        /// <summary>
        /// スクリプトのコードメトリックスに関わるコントロール
        /// </summary>
        private void ctlSwitchScriptCodeMetrics(bool isEnabled) {
            this.prgTextFindMarqee.Visible = !isEnabled;
            this.tbcTextTools.Enabled = isEnabled;
            this.tolTextDoCodeAnalyzerList.Enabled = isEnabled;
            this.tolTextDoCodeMetrics.Enabled = isEnabled;
            this.ltvScriptFuncList.Enabled = isEnabled;
            this.ltvScriptValueList.Enabled = isEnabled;

            if (!isEnabled) {
                this.ltvScriptFuncList.BeginUpdate();
                this.ltvScriptFuncList.SelectedIndices.Clear();
                this.ltvScriptFuncList.Items.Clear();
            } else {
                this.ltvScriptFuncList.EndUpdate();
            }
        }

        /// <summary>
        /// スクリプトの共通変数検索に関わるコントロール
        /// </summary>
        private void ctlSwitchSearchValue(bool isEnabled) {
            this.prgTextFindMarqee.Visible = !isEnabled;
            this.tbcTextTools.Enabled = isEnabled;
            this.ltvValueCountList.Enabled = isEnabled;
            this.ltvValueReferencesList.Enabled = isEnabled;
            this.tolValueCount.Enabled = isEnabled;

            if (!isEnabled) {
                this.ltvValueCountList.BeginUpdate();
                this.ltvValueCountList.SelectedIndices.Clear();
                this.ltvValueCountList.Items.Clear();

                this.ltvValueReferencesList.BeginUpdate();
                this.ltvValueReferencesList.Items.Clear();
            } else {
                this.ltvValueCountList.EndUpdate();
                this.ltvValueReferencesList.EndUpdate();
            }
        }

        /// <summary>
        /// 【非同期】一括検索を実行します。
        /// </summary>
        private async void doSearchAll() {
            if (string.IsNullOrEmpty(this.txtTextSearch.Text)) {
                return;
            } else if (this.uctlOneText.IsDirty && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "現在のファイルを保存") + "\r\n保存しない場合は一括検索できません。", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                return;
            }

            //処理中に一旦閉じる必要があるため、最初に保存しておく
            this.uctlOneText.DoSave();

            var files = new List<string>();
            var flatNodes = new List<TreeNode>();
            var findText = this.txtTextSearch.Text;
            var IsRegux = this.chkTextRegex.Checked;
            var IsStrictCase = this.chkTextCapital.Checked;

            //まずは全ノードをフラットにする
            flatNodes.AddRange(Common.GetNodeListToFlat(this.uctlTextFiles.RootNode));

            //検索範囲に従ってファイルリストを作成する
            switch (this.cmbTextSearchRange.SelectedIndex) {
                case (int) TextFindScope.OneDocument:
                    if (!this.uctlTextFiles.IsFileSelected) {
                        return;
                    }
                    files.Add(this.uctlTextFiles.SelectedNodeFullPath);
                    break;

                case (int) TextFindScope.OnlyText:
                    foreach (var node in flatNodes) {
                        if (Path.GetExtension(node.Text) != Resources.Extension_SQ && node.Tag?.ToString() == CtrlComponent.Common.ctlFileTree.TagFile) {
                            files.Add(node.FullPath);
                        }
                    }
                    break;

                case (int) TextFindScope.OnlyScript:
                    foreach (var node in flatNodes) {
                        if (Path.GetExtension(node.Text) == Resources.Extension_SQ && node.Tag?.ToString() == CtrlComponent.Common.ctlFileTree.TagFile) {
                            files.Add(node.FullPath);
                        }
                    }
                    break;

                case (int) TextFindScope.All:
                    foreach (var node in flatNodes) {
                        if (node.Tag?.ToString() == CtrlComponent.Common.ctlFileTree.TagFile) {
                            files.Add(node.FullPath);
                        }
                    }
                    break;
            }

            //ファイルロックを解除
            this.ctlSwitchTextSearchReplace(false);
            this.uctlOneText.DoClose();

            //検索処理開始
            var wordFinder = await Task.Run(() => {
                var ctl = new ListView();
                System.Text.RegularExpressions.Regex reg = null;
                if (IsRegux) {
                    try {
                        reg = new System.Text.RegularExpressions.Regex(findText, (IsStrictCase ? System.Text.RegularExpressions.RegexOptions.None : System.Text.RegularExpressions.RegexOptions.IgnoreCase));
                    } catch {
                        return ctl;
                    }
                }

                //登録された全ファイルを走査
                foreach (var file in files) {
                    FileStream fs = null;
                    fs = Common.OpenFileReadOnly(ProjectManager.ProjectPath + file);

                    using (var R = new StreamReader(fs, Common.SJIS)) {
                        var line = 1;
                        while (!R.EndOfStream) {
                            try {
                                //同一行内における正規表現ループ開始
                                var buf = R.ReadLine();
                                var col = -1;
                                var length = findText.Length;
                                var hitWord = findText;

                                if (!IsRegux) {
                                    //通常検索
                                    col = buf.IndexOf(findText, (IsStrictCase ? StringComparison.InvariantCulture : StringComparison.InvariantCultureIgnoreCase));
                                } else {
                                    //正規表現検索
                                    System.Text.RegularExpressions.MatchCollection regMatch;
                                    regMatch = reg.Matches(buf);
                                    if (regMatch.Count > 0) {
                                        col = regMatch[0].Groups[0].Index;
                                        length = regMatch[0].Groups[0].Length;
                                        hitWord = regMatch[0].Groups[0].Value;
                                    }
                                }

                                while (col != -1) {
                                    var item = new ListViewItem {
                                        Text = length.ToString()          //先頭に隠し列があることに注意
                                    };
                                    item.SubItems.Add(file);
                                    item.SubItems.Add(line.ToString());
                                    item.SubItems.Add((col + 1).ToString());
                                    item.SubItems.Add("");
                                    item.SubItems.Add(hitWord);             //末尾にも隠し列があることに注意
                                    ctl.Items.Add(item);

                                    //同一行内で次を検索
                                    if (!IsRegux) {
                                        //通常検索
                                        col = buf.IndexOf(findText, col + length, (IsStrictCase ? StringComparison.InvariantCulture : StringComparison.InvariantCultureIgnoreCase));
                                    } else {
                                        //正規表現検索
                                        var regMatch = reg.Matches(buf, col + length);
                                        col = -1;
                                        if (regMatch.Count > 0) {
                                            col = regMatch[0].Groups[0].Index;
                                            length = regMatch[0].Groups[0].Length;
                                            hitWord = regMatch[0].Groups[0].Value;
                                        }
                                    }
                                }
                            } catch {
                                Common.EndFileAccessLock(ref fs);
                                return ctl;     //正規表現に問題がある場合は操作全体を中断させる
                            }

                            line++;
                        }
                    }
                }

                //Common.EndFileAccessLock(ref fs);    //usingを抜けた時点で解放されているため処理しない
                return ctl;
            });

            //検索結果を画面に反映
            foreach (ListViewItem item in wordFinder.Items) {
                var add = new ListViewItem {
                    Text = item.Text
                };
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.FileName].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.Row].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.Column].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.IsReplaced].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.SerachWord].Text);
                this.ltvTextSearchResult.Items.Add(add);
            }

            //System.Media.SystemSounds.Asterisk.Play();
            this.ctlSwitchTextSearchReplace(true);

            if (this.uctlTextFiles.IsFileSelected) {
                //ファイルロック再開
                this.uctlOneText.DoLoad(this.uctlTextFiles.SelectedNodeFullPath);
            }
        }

        /// <summary>
        /// 【非同期】一括置換を実行します。
        /// </summary>
        private async void doReplaceAll() {
            if (this.ltvTextSearchResult.Items.Count == 0) {
                return;
            } else if (this.uctlOneText.IsDirty && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "現在のファイルを保存") + "\r\nなお、この操作は元に戻せませんのでご注意下さい。", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.No) {
                return;
            } else if (this.uctlOneText.IsDirty == false && MessageBox.Show(Resources.MsgW_ImportantWork.Replace("$", "この操作は元に戻せません"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.No) {
                return;
            }

            //処理中に一旦閉じる必要があるため、最初に保存しておく
            this.uctlOneText.DoSave();

            //ファイルロックを解除
            this.ctlSwitchTextSearchReplace(false, false);
            this.uctlOneText.DoClose();

            //別スレッドで実行するために指示リストを作る
            var ReplaceText = this.txtTextReplace.Text;
            var taskList = new ListView();
            foreach (ListViewItem item in this.ltvTextSearchResult.Items) {
                var add = new ListViewItem {
                    Text = item.Text
                };
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.FileName].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.Row].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.Column].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.IsReplaced].Text);
                add.SubItems.Add(item.SubItems[(int) TextSearchListColumns.SerachWord].Text);
                taskList.Items.Add(add);
            }

            //処理開始：置換できなかったインデックスのリストを戻り値にする
            var wordReplacer = await Task.Run(() => {
                var NGList = new List<int>();
                var ctl = new Sgry.Azuki.WinForms.AzukiControl();
                FileStream fs = null;

                //全項目を走査
                for (var i = 0; i < taskList.Items.Count; i++) {
                    if (taskList.Items[i].SubItems[(int) TextSearchListColumns.IsReplaced].Text == Resources.Text_ReplacedMark) {
                        //既に置換されているものは処理しない
                        continue;
                    }

                    var line = int.Parse(taskList.Items[i].SubItems[(int) TextSearchListColumns.Row].Text) - 1;
                    var col = int.Parse(taskList.Items[i].SubItems[(int) TextSearchListColumns.Column].Text) - 1;
                    var length = int.Parse(taskList.Items[i].Text);

                    //新規の場合 or 次のファイルへ移る場合のみテキストを読み込む
                    if (ctl.Tag == null || ctl.Tag?.ToString() != taskList.Items[i].SubItems[(int) TextSearchListColumns.FileName].Text) {
                        Common.EndFileAccessLock(ref fs);
                        fs = Common.OpenFileEditMode(ProjectManager.ProjectPath + taskList.Items[i].SubItems[(int) TextSearchListColumns.FileName].Text);
                        if (fs == null) {
                            //編集権が得られなければスキップする
                            NGList.Add(i);
                            continue;
                        }
                        ctl.Text = Common.ReadFileAll(fs);
                        ctl.Tag = taskList.Items[i].SubItems[(int) TextSearchListColumns.FileName].Text;
                    }

                    ctl.Document.SetCaretIndex(
                        int.Parse(taskList.Items[i].SubItems[(int) TextSearchListColumns.Row].Text) - 1,
                        int.Parse(taskList.Items[i].SubItems[(int) TextSearchListColumns.Column].Text) - 1
                    );
                    ctl.Document.SetSelection(
                        ctl.Document.AnchorIndex,
                        ctl.Document.AnchorIndex + int.Parse(taskList.Items[i].Text)
                    );

                    //検索文字列を正確に選択できているときのみ置換を実行する
                    if (ctl.GetSelectedText() == taskList.Items[i].SubItems[(int) TextSearchListColumns.SerachWord].Text) {
                        //選択地点を置換
                        ctl.Document.Replace(ReplaceText);
                    } else {
                        NGList.Add(i);
                    }

                    //次のファイルへ移る場合は保存
                    if ((i + 1 >= taskList.Items.Count)
                    || (i + 1 < taskList.Items.Count && taskList.Items[i + 1].SubItems[(int) TextSearchListColumns.FileName].Text != ctl.Tag?.ToString())) {
                        Common.WriteFileAll(ref fs, ctl.Text);
                    }
                }

                Common.EndFileAccessLock(ref fs);
                return NGList;
            });

            //結果を画面に反映
            if (this.uctlTextFiles.IsFileSelected) {
                //現在開いているファイルを読み直す
                this.uctlTextFiles.ReloadTree(this.uctlTextFiles.SelectedNodeFullPath);
            }

            for (var i = 0; i < this.ltvTextSearchResult.Items.Count; i++) {
                if (!wordReplacer.Contains(i)) {
                    //正常終了
                    this.ltvTextSearchResult.Items[i].Text = this.txtTextReplace.Text.Length.ToString();
                    this.ltvTextSearchResult.Items[i].SubItems[(int) TextSearchListColumns.IsReplaced].Text = Resources.Text_ReplacedMark;
                } else {
                    //失敗
                    this.ltvTextSearchResult.Items[i].SubItems[(int) TextSearchListColumns.IsReplaced].Text = "";
                }
            }

            //System.Media.SystemSounds.Asterisk.Play();
            this.ctlSwitchTextSearchReplace(true);
            if (this.uctlTextFiles.IsFileSelected) {
                //ファイルロック再開
                this.uctlOneText.DoLoad(this.uctlTextFiles.SelectedNodeFullPath);
            }

            if (wordReplacer.Count == 0) {
                MessageBox.Show(Resources.MsgI_CompleteTextReplace, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } else {
                MessageBox.Show(Resources.MsgI_CompleteTextReplace.Replace("。", $"が、{wordReplacer.Count} 件 失敗しています。"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        /// <summary>
        /// 【非同期】共通変数の使用回数とその参照箇所を検索します。
        /// </summary>
        private async void doSearchValues() {
            if (this.uctlOneText.IsDirty && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "現在のファイルを保存") + "\r\n保存しない場合は一括検索できません。", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                return;
            }

            var files = new List<string>();
            var flatNodes = new List<TreeNode>();

            //まずは全ノードをフラットにする
            flatNodes.AddRange(Common.GetNodeListToFlat(this.uctlTextFiles.RootNode));

            //処理中に一旦閉じる必要があるため、最初に保存しておく
            this.uctlOneText.DoSave();
            this.ctlSwitchSearchValue(false);
            this.uctlOneText.DoClose();

            //全スクリプトを列挙
            foreach (var node in flatNodes) {
                if (Path.GetExtension(node.Text) == Resources.Extension_SQ && node.Tag?.ToString() == CtrlComponent.Common.ctlFileTree.TagFile) {
                    files.Add(node.FullPath);
                }
            }

            //事前に変数リストを作成する
            var list = new List<ListViewItem>();
            list.AddRange(Database.CreateValuesListViewItemList("フラグ", Resources.SQ_UserFlag + "$", this.DBList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.CommonFlag]));
            list.AddRange(Database.CreateValuesListViewItemList("整数", Resources.SQ_UserInteger + "$", this.DBList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.CommonInteger]));
            list.AddRange(Database.CreateValuesListViewItemList("文字列", Resources.SQ_UserString + "$", this.DBList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.CommonString]));

            //検索を実行
            var funcFinder = await Task.Run(() => {
                //各種共通変数を全スクリプトから調べ上げて個数を算出する
                for (var i = 0; i < list.Count; i++) {
                    var cnt = 0;
                    var refList = new List<ListViewItem>();

                    foreach (var file in files) {
                        FileStream fs = null;
                        fs = Common.OpenFileReadOnly(ProjectManager.ProjectPath + file);
                        using (var R = new StreamReader(fs, Common.SJIS)) {
                            var line = 1;

                            while (!R.EndOfStream) {
                                //同一行内における検索ループ開始
                                var buf = R.ReadLine();
                                var col = -1;
                                var length = list[i].SubItems[(int) SQ.DBValueListColumns.RawName].Text.Length;
                                col = buf.IndexOf(list[i].SubItems[(int) SQ.DBValueListColumns.RawName].Text, StringComparison.InvariantCulture);

                                while (col != -1) {
                                    var refItem = new ListViewItem {
                                        Text = file
                                    };
                                    refItem.SubItems.Add(line.ToString());
                                    refList.Add(refItem);
                                    cnt++;

                                    //同一行内で次を検索
                                    col = buf.IndexOf(list[i].SubItems[1].Text, col + length, StringComparison.InvariantCulture);
                                }

                                line++;
                            }
                        }
                    }

                    list[i].SubItems[(int) SQ.DBValueListColumns.Count].Text = cnt.ToString();
                    list[i].Tag = refList;     //個数リストの項目内のタグに参照箇所のリストを格納する
                }

                return list;
            });

            //処理結果を画面に反映
            foreach (var item in funcFinder) {
                this.ltvValueCountList.Items.Add(item);
            }

            //System.Media.SystemSounds.Asterisk.Play();
            this.ctlSwitchSearchValue(true);
        }

        /// <summary>
        /// 【非同期】コード解析を実行します。
        /// </summary>
        private async void doCodeAnalyze() {
            if (this.uctlOneText.IsDirty && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "現在のファイルを保存") + "\r\n保存しない場合は計算できません。", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                return;
            }

            //最新のファイルを読み込むため、解析前に一旦保存する
            this.uctlOneText.DoSave();
            this.ctlSwitchScriptAnalyze(false);

            //関数探索を実行
            var funcFinder = await Task.Run(() => {
                return SQ.GetFunctionList(this.uctlOneText.FileName);
            });

            //変数探索を実行
            var valFinder = await Task.Run(() => {
                return SQ.GetValueList(this.uctlOneText.FileName);
            });

            //処理結果を画面に反映
            this.ltvScriptFuncList.Items.AddRange(funcFinder.ToArray());
            this.ltvScriptValueList.Items.AddRange(valFinder.ToArray());

            //System.Media.SystemSounds.Asterisk.Play();
            this.ctlSwitchScriptAnalyze(true);
        }

        /// <summary>
        /// 【非同期】コードメトリックス計算を実行します。
        /// </summary>
        private async void doCodeMetrics() {
            if (this.uctlOneText.IsDirty && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "現在のファイルを保存") + "\r\n保存しない場合は計算できません。", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No) {
                return;
            }

            //最新のファイルを読み込むため、解析前に一旦保存する
            this.uctlOneText.DoSave();

            //現在の関数リストを一時的に記憶する
            var funcFinder = new List<ListViewItem>();
            foreach (ListViewItem item in this.ltvScriptFuncList.Items) {
                funcFinder.Add(item);
            }

            this.ctlSwitchScriptCodeMetrics(false);

            //コードメトリックス計算を実行
            await Task.Run(() => {
                SQ.CalcCodeMetrics(this.uctlOneText.FileName, funcFinder);
            });

            //処理結果を画面に反映
            this.ltvScriptFuncList.Items.AddRange(funcFinder.ToArray());

            System.Media.SystemSounds.Asterisk.Play();
            this.ctlSwitchScriptCodeMetrics(true);

            //結果が見える程度にスプリッター境界線を広げる
            this.splText1.SplitterDistance = 350;
        }
    }
}
