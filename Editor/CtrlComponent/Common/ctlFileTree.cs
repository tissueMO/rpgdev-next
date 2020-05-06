using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Common {
    using Module;
    using Properties;

    /// <summary>
    /// ノード関連のイベントハンドラー
    /// </summary>
    public delegate void FileTreeNodeEventHandler(object sender, FileTreeNodeEventArgs e);

    /// <summary>
    /// メインクラスの定義
    /// </summary>
    public partial class ctlFileTree : UserControl {
        public const string TagFolder = "folder";
        public const string TagFile = "file";
        public const string TagTempFolder = "tempfolder";     //一時的に生成したフォルダーに付けるタグ
        public const string TagTempFile = "tempfile";         //一時的に生成したファイルに付けるタグ
        public const int ImgIndexFolder = 0;
        public const int ImgIndexFile = 1;
        public event FileTreeNodeEventHandler CreatedNode;              //ファイル・フォルダーの作成が完了したときに発生させるイベント
        public event FileTreeNodeEventHandler RenamedNode;              //ファイル・フォルダーのリネームが完了したときに発生させるイベント
        public event FileTreeNodeEventHandler DeletedNode;              //ファイル・フォルダーの削除が完了したときに発生させるイベント
        public event TreeViewCancelEventHandler BeforeSelectNode;       //ノードが選択される前に発生させるイベント
        public event TreeViewEventHandler AfterSelectNode;              //ノードが選択された後に発生させるイベント
        public event EventHandler FileDoubleClicked;                    //ファイルノードがダブルクリックされたときに発生させるイベント

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlFileTree() {
            this.InitializeComponent();
        }

        /// <summary>
        /// デフォルトで展開するノードの深さ
        /// </summary>
        public int DefaultExnpandDepth {
            get; set;
        } = 2;

        /// <summary>
        /// 基準フォルダー：空白の場合はプロジェクトフォルダー直下です。
        /// </summary>
        public string RootPath {
            get {
                return this.rootPath;
            }
            set {
                this.rootPath = ProjectManager.ProjectPath + (!string.IsNullOrEmpty(value) ? Common.AddToPathSplit(value) : "");
                this.ReloadTree();      //自動でツリーに反映する
            }
        }

        private string rootPath;

        /// <summary>
        /// ファイル名を変更できるかどうか
        /// </summary>
        public bool CanRename {
            get {
                return this.trvTree.LabelEdit;
            }
            set {
                this.trvTree.LabelEdit = value;
            }
        }

        /// <summary>
        /// ファイル・フォルダーを削除できるかどうか
        /// </summary>
        public bool CanDelete {
            get; set;
        }

        /// <summary>
        /// ファイルを表示するかどうか
        /// </summary>
        public bool IncludeFiles {
            get; set;
        } = true;

        /// <summary>
        /// 表示するファイルの拡張子フィルター
        /// 複数登録する場合は ; で区切ります
        /// </summary>
        public string FileFilter {
            get; set;
        } = "";

        /// <summary>
        /// ノードが選択されているかどうか
        /// </summary>
        public bool IsSelected {
            get {
                return (this.trvTree.SelectedNode != null);
            }
        }

        /// <summary>
        /// ファイルが選択されているかどうか
        /// </summary>
        public bool IsFileSelected {
            get {
                return (this.trvTree.SelectedNode?.Tag?.ToString() == TagFile);
            }
        }

        /// <summary>
        /// フォルダーが選択されているかどうか
        /// </summary>
        public bool IsFolderSelected {
            get {
                return (this.trvTree.SelectedNode?.Tag?.ToString() == TagFolder);
            }
        }

        /// <summary>
        /// 選択ノードへのフルパス
        /// </summary>
        public string SelectedNodeFullPath {
            get {
                return this.trvTree.SelectedNode?.FullPath;
            }
        }

        /// <summary>
        /// 選択ノードへのファイルシステム上のフルパス
        /// </summary>
        public string SelectedNodeFullPathOnFileSystem {
            get {
                var temp = Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'));
                return Common.AddToPathSplit(temp) + this.SelectedNodeFullPath;
            }
        }

        /// <summary>
        /// 選択ノードのタグ
        /// </summary>
        public string SelectedNodeTag {
            get {
                return this.trvTree.SelectedNode?.Tag?.ToString();
            }
        }

        /// <summary>
        /// 選択ノードのテキスト
        /// </summary>
        public string SelectedNodeText {
            get {
                return this.trvTree.SelectedNode?.Text;
            }
        }

        /// <summary>
        /// ルートノード
        /// </summary>
        public TreeNode RootNode {
            get {
                return this.trvTree.Nodes[0];
            }
        }

        /// <summary>
        /// 選択したファイルが見つからなかったとき、自動でリロードを行うかどうか
        /// </summary>
        public bool AutoReload {
            get; set;
        } = true;

        /// <summary>
        /// ファイルツリーにチェックボックスを付けるかどうか
        /// </summary>
        public bool VisibleCheckBoxes {
            get {
                return this.trvTree.CheckBoxes;
            }
            set {
                this.trvTree.CheckBoxes = value;
            }
        }

        /// <summary>
        /// チェックされたノードのフルパスリスト
        /// </summary>
        public List<string> CheckedNodePaths {
            get;
            private set;
        } = new List<string>();

        /// <summary>
        /// TreeViewをアクティブにします。
        /// </summary>
        public void FocusToTree() {
            this.trvTree.Focus();
        }

        /// <summary>
        /// すべてのノードを削除します。
        /// </summary>
        public void ClearTreeNodesAll() {
            this.trvTree.Nodes.Clear();
        }

        /// <summary>
        /// ルートに独自のノードを追加します。
        /// リロードを行うと消えてしまいますのでご注意下さい。
        /// </summary>
        public void InsertRoot(TreeNode node) {
            this.trvTree.Nodes.Insert(0, node);
        }

        /// <summary>
        /// ファイルが選択されている場合はその親フォルダーを、フォルダーが選択されている場合はそのまま返します。
        /// </summary>
        public TreeNode GetFolder() {
            if (this.IsFileSelected) {
                return this.trvTree.SelectedNode.Parent;
            } else if (this.IsFolderSelected) {
                return this.trvTree.SelectedNode;
            } else {
                return null;
            }
        }

        /// <summary>
        /// パスを指定してノードを選択します。
        /// </summary>
        public void SelectTreeNode(string path) {
            if (this.SelectedNodeFullPath == path) {
                return;     //既に選択されている場合は処理しない
            } else if (string.IsNullOrEmpty(path)) {
                return;     //無効なパスも処理しない
            }

            //選択解除
            this.trvTree.SelectedNode = null;

            //パス文字列を階層ごとに分解
            path = Common.CutLastChar(path, '\\');      //末尾の区切り記号を除去する
            var split = path?.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);
            if (split == null || split.Length == 0) {
                return;
            }

            //親ノードを探して初動を作る
            foreach (TreeNode node in this.trvTree.Nodes) {
                if (node.Text == split[0]) {
                    this.trvTree.SelectedNode = node;
                    break;
                }
            }

            if (this.trvTree.SelectedNode == null) {
                //親ノードすら見つからない場合はここで終了
                return;
            }

            //子ノードを順次選択していく
            for (var i = 1; i < split.Length; i++) {
                var found = false;
                foreach (TreeNode node in this.trvTree.SelectedNode.Nodes) {
                    if (node.Text == split[i]) {
                        this.trvTree.SelectedNode = node;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    //見つからなかった時点で打ち切る
                    return;
                }
            }
        }

        /// <summary>
        /// パスを指定してノードにチェックを入れます。
        /// </summary>
        public void CheckTreeNode(string path, bool isChecked, bool includeSubNodes) {
            if (string.IsNullOrEmpty(path)) {
                //無効なパスは処理しない
                return;
            }

            //パス文字列を階層ごとに分解
            path = Common.CutLastChar(path, '\\');      //末尾の区切り記号を除去する
            var split = path.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries);

            //親ノードを探して初動を作る
            var currentNode = this.trvTree.Nodes[0];
            for (var i = 0; i < split.Length; i++) {
                var found = false;
                foreach (TreeNode node in currentNode.Nodes) {
                    if (node.Text == split[i]) {
                        currentNode = node;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    //見つからなかった場合は中断
                    return;
                }
            }

            //たどり着いたノードのチェック状態をセットする
            if (currentNode != null) {
                currentNode.Checked = isChecked;

                //サブノードの状態も影響させる場合は再帰的に実行する
                if (includeSubNodes) {
                    foreach (TreeNode node in currentNode.Nodes) {
                        this.CheckTreeNode(path + "\\" + node.Text, isChecked, true);
                    }
                }
            }
        }

        /// <summary>
        /// システムによるノードの選択変更をユーザー操作によるものに改竄します。
        /// システム操作での変更時は保存処理を行わないため、保存が必要なシステム操作の前に実行する必要があります。
        /// </summary>
        /// <returns>操作全体をキャンセルすべきか</returns>
        private bool raiseSelfNodeChange() {
            //システムが選択解除するとイベントが適切に処理できないため、事前に対策を打つ
            var args = new TreeViewCancelEventArgs(this.trvTree.SelectedNode, false, TreeViewAction.Collapse);
            this.BeforeSelectNode?.Invoke(null, args);
            return args.Cancel;     //イベント内でノードの選択解除が拒否された場合は中断
        }

        /// <summary>
        /// ツリーを再読込します。
        /// </summary>
        /// <param name="selectPath">再読込後に選択するパス（未指定の場合は選択状態の現状を維持する）</param>
        public void ReloadTree(string selectPath = null) {
            if (this.raiseSelfNodeChange()) {
                //操作がキャンセルされた
                return;
            }

            this.trvTree.BeginUpdate();
            this.trvTree.SelectedNode = null;
            this.trvTree.Nodes.Clear();

            //選択状態を復元する
            if (this.RootPath != null && Directory.Exists(this.RootPath)) {
                this.trvTree.Nodes.Add(this.makeFileTree(this.RootPath));       //フォルダー階層を再帰的に読み込む
                Common.ExpandTreeNodeToDepth(this.trvTree.Nodes[0], this.DefaultExnpandDepth);
                this.SelectTreeNode(selectPath);
            }

            this.trvTree.EndUpdate();
            this.Focus();           //再選択を適用
        }

        /// <summary>
        /// 基準フォルダーが親ノードになる形で、フォルダー階層を再帰的に読み込みます。
        /// </summary>
        /// <param name="root">基準フォルダー</param>
        /// <returns>フォルダー階層をもったノード</returns>
        private TreeNode makeFileTree(string root) {
            var node = new TreeNode();
            Common.SetTreeNodeImageIndex(node, ImgIndexFolder);
            node.Tag = TagFolder;
            node.Text = Path.GetFileName(Common.CutLastChar(root, '\\'));

            //基準フォルダー直下のフォルダー以下に対して階層構造生成
            foreach (var folder in Directory.GetDirectories(root)) {
                node.Nodes.Add(this.makeFileTree(folder));
            }

            //基準フォルダー直下のファイルを追加
            if (this.IncludeFiles) {
                foreach (var file in Directory.GetFiles(root)) {
                    //除外リストのフィルターと、許可リストのフィルター両方を満たすファイルを表示する
                    if (!Common.CheckFileExtensionFilter(file, Resources.NG_FileExtension)
                    && (string.IsNullOrEmpty(this.FileFilter) || Common.CheckFileExtensionFilter(file, this.FileFilter))) {
                        var filenode = new TreeNode();
                        Common.SetTreeNodeImageIndex(filenode, ImgIndexFile);
                        filenode.Tag = TagFile;
                        filenode.Text = Path.GetFileName(file);
                        node.Nodes.Add(filenode);
                    }
                }
            }
            return node;
        }

        /// <summary>
        /// 引数に指定したノード内にフォルダー階層構造を生成します。
        /// </summary>
        /// <param name="root">基準フォルダー</param>
        /// <param name="node">生成先のノード（基準フォルダーに相当するもの）</param>
        private void makeFileTree(string root, TreeNode node) {
            foreach (var folder in Directory.GetDirectories(root)) {
                node.Nodes.Add(this.makeFileTree(root));
            }
        }

        /// <summary>
        /// 現在のノードの下にファイルを新規作成します。
        /// ファイルが選択されている場合は同一フォルダー内に作成します。
        /// 実際の作成処理はラベル編集後に行います。
        /// </summary>
        public void CreateFile(string defaultName = "新しいファイル") {
            if (this.raiseSelfNodeChange()) {
                return;     //操作がキャンセルされた
            }

            //ファイルを選択している場合は同一フォルダー内に作成する
            if (this.IsFileSelected) {
                this.trvTree.SelectedNode = this.trvTree.SelectedNode.Parent;
            }

            //名前を決めさせる
            var Dlg = new Dialog.Common.dlgInputTextValue(this.SelectedNodeFullPath, false, false, defaultName, this.FileFilter);
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                //ファイルを生成する
                var newNode = new TreeNode {
                    Text = Dlg.ResultText
                };
                this.trvTree.SelectedNode.Nodes.Add(newNode);
                var eArgs = new FileTreeNodeEventArgs {
                    IsFolder = false,
                    rPath = Dlg.rPath,
                    SelectedNode = newNode
                };
                this.applyCreate(eArgs);
            }
        }

        /// <summary>
        /// 現在のノードの下にフォルダーを新規作成します。
        /// ファイルが選択されている場合は同一フォルダー内に作成します。
        /// 実際の作成処理はラベル編集後に行います。
        /// </summary>
        public void CreateFolder() {
            if (this.raiseSelfNodeChange()) {
                return;     //操作がキャンセルされた
            }

            //ファイルを選択している場合は同一フォルダー内に作成する
            if (this.IsFileSelected) {
                this.trvTree.SelectedNode = this.trvTree.SelectedNode.Parent;
            }

            //名前を決めさせる
            var Dlg = new Dialog.Common.dlgInputTextValue(this.SelectedNodeFullPath, true, false, "新しいフォルダー");
            if (Dlg.ShowDialog(this) == DialogResult.OK) {
                //フォルダーを生成する
                var newNode = new TreeNode {
                    Text = Dlg.ResultText
                };
                this.trvTree.SelectedNode.Nodes.Add(newNode);

                var eArgs = new FileTreeNodeEventArgs {
                    IsFolder = true,
                    rPath = Dlg.rPath,
                    SelectedNode = newNode
                };
                this.applyCreate(eArgs);
            }
        }

        /// <summary>
        /// 選択ノードの名前を変更する前に除外チェックを行う
        /// </summary>
        private void trvTree_BeforeLabelEdit(object sender, NodeLabelEditEventArgs e) {
            if (e.Node.Parent == null) {
                MessageBox.Show(Resources.MsgE_NGDeleteRoot.Replace("$", "名前を変更"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.CancelEdit = true;
            } else if (e.Node.Text.Length > 0 && e.Node.Text.Substring(0, 1) == Resources.NG_SystemFolderSymbol) {
                MessageBox.Show(Resources.MsgE_NGDeleteRoot.Replace("$", "名前を変更"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.CancelEdit = true;
            } else {
                var args = new TreeViewCancelEventArgs(e.Node, false, TreeViewAction.ByMouse);
                this.BeforeSelectNode?.Invoke(this, args);
                e.CancelEdit = args.Cancel;
            }
        }

        /// <summary>
        /// 選択ノードの名前を変更した後
        /// </summary>
        private void trvTree_AfterLabelEdit(object sender, NodeLabelEditEventArgs e) {
            if (e.Node == null) {
                return;     //入力中にリロードが起こるなどして処理できない状態になった
            }

            var eArgs = new FileTreeNodeEventArgs();
            var oldFile = Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + e.Node?.FullPath;
            var newFile = Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + Common.AddToPathSplit(Path.GetDirectoryName(e.Node?.FullPath)) + e.Label;
            var isFolder = (e.Node.Tag?.ToString() == TagFolder || e.Node.Tag?.ToString() == TagTempFolder);

            //除外設定
            if (e.Label == null) {
                //変更しなかった場合はサイレントスルー
                e.CancelEdit = true;
            } else if (!Common.CheckEnabledPath(e.Label)) {
                //ファイル・フォルダー名に使えない文字が含まれる
                MessageBox.Show(Resources.MsgE_FileNameInvarid.Replace("$", "ファイル/フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.CancelEdit = true;
            } else if (File.Exists(newFile)) {
                //ファイル名と重複しているフォルダー名
                MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "ファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.CancelEdit = true;
            } else if (Directory.Exists(newFile)) {
                //フォルダー名と重複しているファイル名
                MessageBox.Show(Resources.MsgE_FileExists.Replace("$", "フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.CancelEdit = true;
            } else if (!isFolder && this.FileFilter != "" && !Common.CheckFileExtensionFilter(newFile, this.FileFilter)) {
                //該当拡張子が付いていないと、作成してもリストに表示されないので無効にする
                MessageBox.Show("無効な拡張子です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                e.CancelEdit = true;
            }

            //発生させるイベントのパラメーター設定
            eArgs.rPath = Path.GetDirectoryName(e.Node.FullPath) + this.trvTree.PathSeparator + e.Label;
            eArgs.SelectedNode = e.Node;
            eArgs.IsFolder = isFolder;

            //リネーム実行
            this.applyRename(e, eArgs, e.Node.Tag?.ToString(), oldFile, newFile);
        }

        /// <summary>
        /// ファイル/フォルダー作成を確定させます。
        /// </summary>
        /// <param name="e">呼び出し元のイベント引数</param>
        private void applyCreate(FileTreeNodeEventArgs eArgs) {
            //空のファイル・フォルダーを生成する
            try {
                if (!eArgs.IsFolder) {
                    File.WriteAllText(ProjectManager.ProjectPath + eArgs.rPath, "", Common.SJIS);
                } else {
                    Directory.CreateDirectory(ProjectManager.ProjectPath + eArgs.rPath);
                }
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "ファイルの操作"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;     //失敗した場合は中断する
            }

            eArgs.SelectedNode.Text = Path.GetFileName(eArgs.rPath);        //ここで新しいノード名を確定させる
            this.CreatedNode?.Invoke(this, eArgs);
            this.ReloadTree(eArgs.SelectedNode.FullPath);
        }

        /// <summary>
        /// ファイル/フォルダーのリネームを確定させます。
        /// </summary>
        /// <param name="e">呼び出し元のイベント引数</param>
        private void applyRename(NodeLabelEditEventArgs e, FileTreeNodeEventArgs eArgs, string typeTag, string oldFile, string newFile) {
            if (!e.CancelEdit) {
                try {
                    if (typeTag == TagFile) {
                        File.Move(oldFile, newFile);
                    } else {
                        Directory.Move(oldFile, newFile);
                    }
                    eArgs.SelectedNode.Text = Path.GetFileName(eArgs.rPath);        //ここで新しいノード名を確定させる
                } catch (UnauthorizedAccessException) {      //アクセス権限がロックされていた場合
                    e.CancelEdit = true;
                    eArgs.Failed = true;
                    MessageBox.Show(Resources.MsgE_AcccessError, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                } catch {
                    e.CancelEdit = true;
                    eArgs.Failed = true;
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "ファイルの操作"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            this.RenamedNode?.Invoke(this, eArgs);       //失敗しても発生させる
        }

        /// <summary>
        /// 現在選択されているファイルやフォルダーをエクスプローラーで貼り付けできる形でクリップボードに送ります。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public bool CopySelectedNode() {
            if (!this.IsSelected) {
                return false;
            }

            var srcFile = Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + this.SelectedNodeFullPath;
            var nodes = new System.Collections.Specialized.StringCollection {
                srcFile
            };
            Clipboard.SetFileDropList(nodes);
            return true;
        }

        /// <summary>
        /// クリップボードにあるファイルを取り出して現在のフォルダーに貼り付けます。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        public bool PasteNode() {
            if (Clipboard.GetFileDropList() == null || Clipboard.GetFileDropList().Count == 0 || !this.IsSelected) {
                return false;
            }

            var nodes = Clipboard.GetFileDropList();
            var folder = "";
            var newFile = "";
            folder += Common.AddToPathSplit(this.GetFolder().FullPath);

            foreach (var node in nodes) {
                try {
                    if (File.Exists(node)) {
                        //ファイルである場合
                        newFile = Common.GenerateCopyFileName(folder + Path.GetFileName(node));
                        File.Copy(node, Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + newFile);
                    } else if (Directory.Exists(node)) {
                        //フォルダーである場合
                        newFile = Common.GenerateCopyFolderName(folder + Path.GetFileName(node));
                        Common.CopyDirectory(node, Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + newFile, false);
                    }
                } catch {
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "ファイルのコピー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }

            this.ReloadTree(newFile);
            return true;
        }

        /// <summary>
        /// 現在選択されているファイルまたはフォルダーを削除します。
        /// </summary>
        /// <param name="editorCloseMethod">関連エディターを閉じるメソッドへの参照</param>
        /// <returns>成功したかどうか</returns>
        public bool DeleteSelectedNode(Action editorCloseMethod) {
            if (!this.IsSelected || !this.CanDelete) {
                return false;
            }

            var fileName = Path.GetFileName(this.SelectedNodeFullPath);
            var BeforeSelectedNode = this.trvTree.SelectedNode;

            //除外設定
            if (this.trvTree.SelectedNode.Parent == null) {       //ルートフォルダーは削除できない
                MessageBox.Show(Resources.MsgE_NGDeleteRoot.Replace("$", "削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            } else if (fileName.Substring(0, 1) == Resources.NG_SystemFolderSymbol) {
                MessageBox.Show(Resources.MsgE_NGDeleteRoot.Replace("$", "削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            //発生させるイベントの準備
            var eArgs = new FileTreeNodeEventArgs {
                rPath = this.SelectedNodeFullPath,
                SelectedNode = this.trvTree.SelectedNode,
                IsFolder = this.IsFolderSelected
            };

            if (MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", fileName + " を削除"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.Yes) {
                //関連エディターを閉じる処理
                editorCloseMethod?.Invoke();

                try {
                    //削除を実行
                    if (this.IsFolderSelected) {
                        Common.DeleteDirectory(Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + this.GetFolder().FullPath);
                    } else {
                        File.Delete(Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.RootPath, '\\'))) + Common.AddToPathSplit(this.GetFolder().FullPath) + fileName);
                    }
                    BeforeSelectedNode.Remove();
                } catch {
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "削除操作"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            } else {
                return false;
            }

            this.DeletedNode?.Invoke(this, eArgs);
            return true;
        }

        /// <summary>
        /// ノード選択前イベント発生
        /// </summary>
        private void trvTree_BeforeSelect(object sender, TreeViewCancelEventArgs e) {
            this.BeforeSelectNode?.Invoke(sender, e);
        }

        /// <summary>
        /// ノード選択後イベント発生
        /// </summary>
        private void trvTree_AfterSelect(object sender, TreeViewEventArgs e) {
            if (File.Exists(this.SelectedNodeFullPathOnFileSystem)
            || Directory.Exists(this.SelectedNodeFullPathOnFileSystem)) {
                this.AfterSelectNode?.Invoke(sender, e);
            } else if (e.Node?.Tag.ToString() != TagFile
            && e.Node?.Tag.ToString() != TagFolder
            && e.Node?.Tag.ToString() != TagTempFile
            && e.Node?.Tag.ToString() != TagTempFolder) {
                //不明なタグが付けられたノードを選択した場合はイベントを発生させる
                this.AfterSelectNode?.Invoke(sender, e);
            } else if (this.AutoReload) {
                //無効なノードを選択したら再読込
                this.ReloadTree();
            }
        }

        /// <summary>
        /// ノード折り畳み前イベント発生
        /// </summary>
        private void trvTree_BeforeCollapse(object sender, TreeViewCancelEventArgs e) {
            //this.BeforeSelectNode?.Invoke(sender, e);
        }

        /// <summary>
        /// ノード折り畳み後イベント発生
        /// </summary>
        private void trvTree_AfterCollapse(object sender, TreeViewEventArgs e) {
            //this.AfterSelectNode?.Invoke(sender, e);
        }

        /// <summary>
        /// ファイルノードをダブルクリックでイベント発生
        /// </summary>
        private void trvTree_MouseDoubleClick(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left && this.IsFileSelected) {
                this.FileDoubleClicked?.Invoke(sender, e);
            }
        }

        /// <summary>
        /// ノードのチェック状態が変更されたとき
        /// </summary>
        private void trvTree_AfterCheck(object sender, TreeViewEventArgs e) {
            //チェックリストを更新する
            if (e.Node.Checked) {
                this.CheckedNodePaths.Add(e.Node.FullPath);
            } else {
                this.CheckedNodePaths.Remove(e.Node.FullPath);
            }

            //サブノードも反映する
            foreach (TreeNode node in e.Node.Nodes) {
                node.Checked = e.Node.Checked;
            }
        }
    }

    /// <summary>
    /// メインクラスで発生させる独自イベントが返すデータ
    /// </summary>
    public class FileTreeNodeEventArgs : EventArgs {
        public string rPath;                    //対象ノードへのパス（ルートフォルダー基準）
        public bool IsFolder;
        public bool Failed;
        public TreeNode SelectedNode;
    }
}
