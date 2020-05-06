using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Editor.Module {
    using Properties;
    using System.Diagnostics;
    using System.Security;
    using System.Security.Cryptography;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using SQObject = GenerateGlueCode.GenerateCPPDocument.SQObject;

    /// <summary>
    /// コード全域で使える汎用機能を提供します。
    /// </summary>
    public static class Common {
        public static readonly Encoding SJIS = Encoding.GetEncoding("Shift-JIS");
        public static readonly string DateFormat = "yy/MM/dd HH:mm:ss";

        /// <summary>
        /// テスタープログラムのプロセス
        /// </summary>
        private static Process testerProcess; 

        /// <summary>
        /// メインウィンドウに指定したスクリプトを２ペインで開くように指示するイベントの型
        /// </summary>
        public delegate void RequestOpenFileEventHandler(object sender, RequestOpenFileEventArgs e);

        /// <summary>
        /// SHA-256準拠のハッシュ値を返します。
        /// </summary>
        /// <param name="salt">ハッシュ ソルト</param>
        /// <param name="text">ハッシュ化する文字列</param>
        /// <returns>SHA-256準拠のハッシュ化されたBase64文字列</returns>
        public static string CreateHashSHA256(string salt, string text) {
            var bytes = Common.SJIS.GetBytes(salt + text);
            using (var csp = new SHA256CryptoServiceProvider()) {
                var hash = csp.ComputeHash(bytes);
                return Convert.ToBase64String(hash);
            }
        }

        /// <summary>
        /// 平文をSecureStringに変換します。
        /// </summary>
        /// <param name="plainString">平文</param>
        /// <returns>SecuredString</returns>
        public static SecureString ConvertSecureString(string plainString) {
            var secured = new SecureString();
            foreach (var c in plainString) {
                secured.AppendChar(c);
            }
            return secured;
        }

        /// <summary>
        /// テスターを起動します。
        /// </summary>
        /// <param name="args">テスターに渡すコマンドライン引数</param>
        public static void RunTester(string args, bool waitMode) {
            if(Common.testerProcess != null) {
                //既にテスターが起動中の場合は重複させない
                MessageBox.Show(Resources.MsgE_Cannot.Replace("$", "既にテスタープログラムが起動中です。\r\nテスターを多重起動すること"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            //テスターを最新版に更新
            try {
                Common.CopyTester();
            } catch (FileNotFoundException e) {
                //テスター自体がない場合は起動できない
                MessageBox.Show(e.Message, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            } catch (Exception e) {
                //コピー処理に失敗した
                MessageBox.Show(e.Message, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            //コマンドライン引数として渡す文字列はイミディエイトウィンドウにも出力する
            Debug.WriteLine(args);

            if (waitMode) {
                //待機モード：プログラムが終了したとみなされるまでに数秒～十数秒のタイムラグが起きる
                Process.Start(ProjectManager.ProjectPath + Resources.Path_GameProgram, args).WaitForExit();
            } else {
                Common.testerProcess = Process.Start(ProjectManager.ProjectPath + Resources.Path_GameProgram, args);
                Common.testerProcess.EnableRaisingEvents = true;
                Common.testerProcess.Exited += (sender, e) => {
                    Common.testerProcess = null;
                };
            }
        }

        /// <summary>
        /// 最新版のテスターをプロジェクトフォルダーにコピーします。
        /// </summary>
        public static void CopyTester() {
            //必要なDLLが存在しない場合はコピーする
            if (!File.Exists(ProjectManager.ProjectPath + Resources.Path_GGS)) {
                try {
                    File.Copy(Application.StartupPath + "\\" + Resources.Path_GGS, ProjectManager.ProjectPath + Resources.Path_GGS, true);
                } catch {
                    throw new Exception(Resources.MsgE_Failed.Replace("$", "ゲームプログラムの起動に必要なライブラリのコピー"));
                }
            }

            //デバッグ用のゲームプログラムをコピーする
            try {
                File.Copy(Application.StartupPath + "\\" + Resources.Path_GameProgram, ProjectManager.ProjectPath + Resources.Path_GameProgram, true);
            } catch {
                //コピーできなかった場合は宛先ファイルの存在有無を確認する
                if (!File.Exists(ProjectManager.ProjectPath + Resources.Path_GameProgram)) {
                    throw new FileNotFoundException(Resources.MsgE_Failed.Replace("$", "I/Oエラーのため、ゲームプログラムの起動"));
                }
                throw new Exception("I/Oエラーのため、最新のゲームプログラムに更新できません。");
            }
        }

        /// <summary>
        /// テキストエディターに指定したコマンドを挿入します。
        /// </summary>
        public static void InsertCommand(CtrlComponent.Text.ctlOneTextEditor uctlOneText, string code, bool forbiddenCRLF) {
            var indent = Common.GetIndentDepth(
                                uctlOneText.azkEditor.Document.GetLineContent(
                                        uctlOneText.azkEditor.Document.GetLineIndexFromCharIndex(uctlOneText.azkEditor.Document.AnchorIndex)));

            //先頭のインデントを覚えておく
            var tabs = "";
            for (var i = 0; i < indent; i++) {
                tabs += "\t";
            }

            //改行加工が許可されている場合のみ
            if (!forbiddenCRLF) {
                if (code.IndexOf(":") != -1) {
                    //ラベルには改行と空行を挟み、自動インデントする
                    code = code.Replace(":", ":\r\n" + tabs + "\t" + "\r\n" + tabs + "\tbreak;");
                } else if (code.IndexOf("{") != -1) {
                    //ブロック開始点には改行と空行を挟み、自動インデントする
                    code = code.Replace("{", "{\r\n" + tabs + "\t" + "\r\n" + tabs);
                }
                if (code.IndexOf(";") != -1) {
                    //文が終わっている場合は自動で改行する
                    code = code.Replace(";", ";\r\n" + tabs);
                }
            }

            uctlOneText.azkEditor.Document.Replace(code);
            uctlOneText.azkEditor.Focus();
        }

        /// <summary>
        /// コマンドツリーの逆探索の結果における特殊なノードを処理し、置き換え用の文字列を返します。
        /// </summary>
        public static string EditCommandByTool(AllDB DBList, CtrlComponent.Text.ctlOneTextEditor.FoundCommandNodeEventArgs e) {
            //引数を解析する
            var arg = e.Option.Substring(e.Option.IndexOf("(") + 1);
            if (arg.LastIndexOf(")") != -1) {
                arg = arg.Substring(0, arg.LastIndexOf(")"));
            }

            var args = arg.Split(',');
            for (var i = 0; i < args.Length; i++) {
                args[i] = args[i].Trim();
            }

            switch (e.NodeTag) {
                case CtrlComponent.Text.ctlCommandTree.TagToolMessage:
                    //会話メッセージ
                    if (args.Length >= 3) {
                        var isHex = false;
                        var faceID = -1;

                        if (args[0].Length > 2 && args[0].Substring(0, 2) == "0x") {
                            //16進数のIDは接頭辞を削除する
                            args[0] = args[0].Substring(2);
                            isHex = true;
                        }
                        if (args[0].IndexOf("/") != -1) {
                            //コメントがくっついている場合
                            args[0] = args[0].Substring(0, args[0].IndexOf("/"));
                        }

                        //顔グラIDを10進数に変換
                        if (isHex) {
                            int.TryParse(args[0], System.Globalization.NumberStyles.AllowHexSpecifier, null, out faceID);
                        } else {
                            int.TryParse(args[0], out faceID);
                        }

                        var Dlg = new Dialog.Text.Script.dlgEVcmdMessage(
                            DBList,
                            faceID,
                            Common.CutDoubleQuotation(args[1]),
                            Common.CutDoubleQuotation(args[2])
                        );

                        if (Dlg.ShowDialog() == DialogResult.OK) {
                            return Dlg.Result;
                        }
                    }
                    break;

                case CtrlComponent.Text.ctlCommandTree.TagToolChangeMap:
                    //場所移動
                    if (args.Length >= 4) {
                        if (args[3].IndexOf("/") != -1) {
                            //コメントがくっついている場合
                            args[3] = args[3].Substring(0, args[3].IndexOf("/"));
                        }

                        var Dlg = new Dialog.Text.Script.dlgEVcmdChangeMap(
                            DBList,
                            Common.CutDoubleQuotation(args[0]),
                            int.Parse(args[1]),
                            int.Parse(args[2]),
                            SQ.FromSQEnum(typeof(Map.Direction4), args[3])
                        );

                        if (Dlg.ShowDialog() == DialogResult.OK) {
                            return Dlg.Result;
                        }
                    }
                    break;
            }

            return "";
        }

        /// <summary>
        /// コマンドを逆探索した結果をコマンドツリーに反映させ、さらに関数の場合は再編集します。
        /// </summary>
        public static void FoundCommandNode(CtrlComponent.Text.ctlOneTextEditor uctlOneText, CtrlComponent.Text.ctlCommandTree uctlComTree, AllDB DBList, CtrlComponent.Text.ctlOneTextEditor.FoundCommandNodeEventArgs e) {
            //特殊なノードは別の処理を行う
            if (e.Node == null) {
                var text = EditCommandByTool(DBList, e);
                if (!string.IsNullOrEmpty(text)) {
                    Common.InsertCommand(uctlOneText, text, true);
                }
                return;
            }

            uctlComTree.trvTree.SelectedNode = e.Node;
            uctlComTree.Focus();

            if (e.Node.Tag is SQObject obj) {
                if (obj.NodeID == GenerateGlueCode.GenerateCPPDocument.ObjectType.SQEventHandler) {
                    //イベントハンドラーは「関数定義」なのでダイアログによる再編集はできない
                    return;
                }

                switch (obj.SubType) {
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassFunc:
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberFunc:
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQFunc:
                        if (obj.ArgumentList.Count > 0) {
                            //引数がある関数は、ダイアログを開いて再編集する
                            var Dlg = new Dialog.Text.Script.dlgInsertCommand(uctlComTree, obj, e.Arguments);
                            if (Dlg.ShowDialog() != DialogResult.OK) {
                                return;
                            }

                            //編集箇所を特定して置き換える
                            Common.InsertCommand(uctlOneText, Dlg.Result, true);
                        }
                        break;
                }
            }
        }

        /// <summary>
        /// TreeViewにおいて、指定したパスに基づいてツリー構造を生成します。
        /// </summary>
        /// <param name="root">基点となるノード</param>
        /// <param name="path">基点となるノードからのパス</param>
        /// <returns>生成または取得された末端のノード</returns>
        public static TreeNode CreateNodePath(TreeNode root, string path) {
            string nodeName, laterPath;

            if (string.IsNullOrEmpty(path)) {
                //パスが無効な場合は基点ノードをそのまま返す
                return root;
            } else if (path.IndexOf('\\') == -1) {
                //末端のノードである場合
                nodeName = path;
                laterPath = "";
            } else {
                //途中のノードである場合
                nodeName = path.Substring(0, path.IndexOf('\\'));
                laterPath = path.Substring(path.IndexOf('\\') + 1);
            }

            //現在のノードを探索する
            foreach (TreeNode node in root.Nodes) {
                if (node.Text == nodeName) {
                    if (string.IsNullOrEmpty(laterPath)) {
                        //既存の末端ノード
                        return node;
                    } else {
                        //既存の中間ノード
                        return Common.CreateNodePath(node, laterPath);
                    }
                }
            }

            //見つからなかったら新規作成する
            var newNode = new TreeNode();
            Common.SetTreeNodeImageIndex(newNode, 0);
            newNode.Text = nodeName;
            root.Nodes.Add(newNode);

            if (string.IsNullOrEmpty(laterPath)) {
                //新規の末端ノード
                return newNode;
            } else {
                //新規の中間ノード
                return Common.CreateNodePath(newNode, laterPath);
            }
        }

        /// <summary>
        /// TreeViewにおいて、指定した相対パスが存在する場合は該当するノードを、存在しない場合はnullを返します。
        /// </summary>
        /// <param name="root">基点となるノード</param>
        /// <param name="path">基点となるノードからのパス</param>
        /// <returns>取得された末端のノード</returns>
        public static TreeNode GetExistsNode(TreeNode root, string path) {
            string nodeName, laterPath;

            if (string.IsNullOrEmpty(path)) {
                //パスが無効なときは中断
                return null;
            } else if (path.IndexOf('\\') == -1) {
                //末端のノードである場合
                nodeName = path;
                laterPath = "";
            } else {
                //途中のノードである場合
                nodeName = path.Substring(0, path.IndexOf('\\'));
                laterPath = path.Substring(path.IndexOf('\\') + 1);
            }

            //現在のノードを探索する
            foreach (TreeNode node in root.Nodes) {
                if (node.Text == nodeName) {
                    if (string.IsNullOrEmpty(laterPath)) {
                        //既存の末端ノード
                        return node;
                    } else {
                        //既存の中間ノード
                        return Common.GetExistsNode(node, laterPath);
                    }
                }
            }

            //見つからなかった
            return null;
        }

        /// <summary>
        /// 指定したノードにImageIndexを設定します。
        /// </summary>
        /// <param name="targetNode">対象ノード</param>
        /// <param name="imgIndex">ImageIndex</param>
        public static void SetTreeNodeImageIndex(TreeNode targetNode, int imgIndex) {
            targetNode.ImageIndex = imgIndex;
            targetNode.SelectedImageIndex = imgIndex;
        }

        /// <summary>
        /// 指定したノードの中身をフラット（階層がすべて等しい状態）にしたノードを返します。
        /// </summary>
        /// <param name="node">トップレベルのノード</param>
        /// <returns>トップレベル内に含まれるサブノードのリスト</returns>
        public static List<TreeNode> GetNodeListToFlat(TreeNode node) {
            var result = new List<TreeNode>();

            foreach (TreeNode sub in node.Nodes) {
                result.Add(sub);
                if (sub.GetNodeCount(false) > 0) {
                    result.AddRange(GetNodeListToFlat(sub));
                }
            }

            return result;
        }

        /// <summary>
        /// 指定したノードの中身すべてのフルパスをリストにして返します。
        /// </summary>
        /// <param name="node">トップレベルのノード</param>
        /// <returns>トップレベル内に含まれるサブノードへのフルパスのリスト</returns>
        public static List<string> GetNodePathList(TreeNode node) {
            var nodeList = GetNodeListToFlat(node);
            var pathList = new List<string>();

            foreach (var sub in nodeList) {
                pathList.Add(sub.FullPath);
            }

            return pathList;
        }

        /// <summary>
        /// 指定したフォルダーをコピーします。
        /// </summary>
        /// <param name="stSourcePath">コピー元のフォルダーへのフルパス</param>
        /// <param name="stDestPath">コピー先のフォルダーへのフルパス</param>
        /// <param name="bOverwrite">既にファイルが存在する場合に上書きするかどうか</param>
        public static void CopyDirectory(string stSourcePath, string stDestPath, bool bOverwrite = true) {
            //コピー先のディレクトリがなければ作成する
            if (!Directory.Exists(stDestPath)) {
                try {
                    Directory.CreateDirectory(stDestPath);
                    File.SetAttributes(stDestPath, File.GetAttributes(stSourcePath));
                } catch {
                    MessageBox.Show("フォルダーの作成に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                //新規作成するときは既にファイルが存在することはない
                bOverwrite = true;
            }

            //ファイルを先にコピーする
            foreach (var stCopyFrom in Directory.GetFiles(stSourcePath)) {
                var stCopyTo = Path.Combine(stDestPath, Path.GetFileName(stCopyFrom));

                if (File.Exists(stCopyTo) && !bOverwrite) {
                    //既に存在していて上書きできない
                    MessageBox.Show("ファイル [" + Path.GetDirectoryName(stCopyFrom) + "] は既に存在しています。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    continue;
                }

                try {
                    File.Copy(stCopyFrom, stCopyTo, bOverwrite);
                } catch {
                    MessageBox.Show("ファイル [" + Path.GetDirectoryName(stCopyFrom) + "] のコピーに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

            //コピー元のディレクトリをすべてコピーする
            foreach (var stCopyFrom in Directory.GetDirectories(stSourcePath)) {
                var stCopyTo = Path.Combine(stDestPath, Path.GetFileName(stCopyFrom));
                Common.CopyDirectory(stCopyFrom, stCopyTo, bOverwrite);
            }
        }

        /// <summary>
        /// 指定したフォルダーをすべて削除します。
        /// </summary>
        /// <param name="dirPath">削除対象のフォルダーへのフルパス</param>
        public static void DeleteDirectory(string dirPath) {
            var dirInfo = new DirectoryInfo(dirPath);
            Common.removeReadonlyAttribute(dirInfo);

            try {
                dirInfo.Delete(true);
            } catch {
                MessageBox.Show("フォルダーの削除に失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 指定したフォルダー以下のファイル属性をすべて削除します。
        /// </summary>
        private static void removeReadonlyAttribute(DirectoryInfo dirInfo) {
            //元のフォルダーの属性を変更
            if ((dirInfo.Attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly) {
                dirInfo.Attributes = FileAttributes.Normal;
            }

            //フォルダー内のすべてのファイルの属性を適用
            foreach (var fi in dirInfo.GetFiles()) {
                if ((fi.Attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly) {
                    fi.Attributes = FileAttributes.Normal;
                }
            }

            //サブフォルダーの属性にも適用
            foreach (var di in dirInfo.GetDirectories()) {
                Common.removeReadonlyAttribute(di);
            }
        }

        /// <summary>
        /// 両端のダブルクォートをカットした文字列を返します。
        /// </summary>
        public static string CutDoubleQuotation(string text) {
            var buf = text;

            if (buf.Length > 0 && buf.Substring(0, 1) == "\"") {
                buf = buf.Substring(1);
            }

            if (buf.Length > 0 && buf.Substring(buf.Length - 1, 1) == "\"") {
                buf = buf.Substring(0, buf.Length - 1);
            }

            return buf;
        }

        /// <summary>
        /// 始点から目的のオブジェクトまで辿ります。見つからなければnullを返します。
        /// </summary>
        public static Control GetHigherControl(Control start, Type dest) {
            var parent = start.Parent;

            while (parent != null) {
                if (parent.GetType() == dest) {
                    return parent;
                }
                parent = parent.Parent;
            }

            return null;
        }

        /// <summary>
        /// 値を一つ足します。このとき、指定値の次が制限値に達する場合は、頭に回します。
        /// 有効範囲の中に最大値を含みません。
        /// </summary>
        public static int LoopNext(int val, int start, int limit, bool reverse) {
            //範囲外の時は修正する
            if (val < start || limit <= val) {
                return Common.ApplyLimit(val, start, limit - 1);
            }

            //通常処理
            var next = val + (reverse ? -1 : +1);
            if (limit <= next) {
                next = start;
            } else if (next < start) {
                next = limit - 1;
            }
            return next;
        }

        /// <summary>
        /// 値を指定した範囲の中に収めて返します。
        /// 有効範囲の中に最大値を含みます。
        /// </summary>
        public static int ApplyLimit(int val, int min, int max) {
            if (val < min) {
                return min;
            } else if (max < val) {
                return max;
            }
            return val;
        }
        public static Point ApplyLimit(Point val, Rectangle limit) {
            // limit の右端/下端は含まないことに注意する
            if (val.X < limit.Left) {
                val.X = limit.Left;
            }
            if (limit.Right <= val.X) {
                val.X = limit.Right - 1;
            }
            if (val.Y < limit.Top) {
                val.Y = limit.Top;
            }
            if (limit.Bottom <= val.Y) {
                val.Y = limit.Bottom - 1;
            }
            return val;
        }
        public static Rectangle ApplyLimit(Rectangle val, Rectangle limit) {
            // limit の右端/下端は含まないことに注意する
            // はみ出ている４端点をそれぞれ修正する
            if (limit.Contains(val)) {
                return val;
            }

            //左上座標を修正する
            val.Location = Common.ApplyLimit(val.Location, limit);

            //サイズを修正する
            if (limit.Right <= val.Right) {
                //右端が右に行きすぎ
                val.Width = limit.Right - val.X;
                if (val.Width < 0) {
                    val.Width = 0;
                }
            }
            if (limit.Bottom <= val.Bottom) {
                //下端が下に行きすぎ
                val.Height = limit.Bottom - val.Y;
                if (val.Height < 0) {
                    val.Height = 0;
                }
            }

            return val;
        }

        /// <summary>
        /// 文字列の配列を、区切り記号を用いた一連の文字列にします。
        /// </summary>
        public static string LinkStringArray(string[] array, char splitChar) {
            //配列が無効な場合は空白で返す
            if (array == null) {
                return "";
            }

            var buf = "";
            foreach (var item in array) {
                buf += item;
                buf += splitChar;
            }
            buf = Common.CutLastChar(buf, splitChar);
            return buf;
        }

        /// <summary>
        /// Color型を [R,G,B] の形式で示される文字列情報に変換します。
        /// </summary>
        public static string ColorToString(Color col) {
            return col.R + Resources.Split_ColorRGB + col.G + Resources.Split_ColorRGB + col.B;
        }

        /// <summary>
        /// [R,G,B] の形式で示される座標情報をColor型に変換します。
        /// </summary>
        public static Color StringToColor(string par) {
            //無効な形式のときは黒にする
            if (string.IsNullOrEmpty(par)) {
                return Color.Black;
            }

            var spl = par.Split(Resources.Split_ColorRGB.ToCharArray());
            if (spl.Length == Common.GetEnumCount<ColorDataIndices>()
            && int.TryParse(spl[(int) ColorDataIndices.Red], out var r)
            && int.TryParse(spl[(int) ColorDataIndices.Green], out var g)
            && int.TryParse(spl[(int) ColorDataIndices.Blue], out var b)) {
                return Color.FromArgb(r, g, b);
            }
            return Color.Black;
        }

        /// <summary>
        /// Point型を [X:Y] の形式で示される座標情報に変換します。
        /// </summary>
        public static string PointToString(Point pos) {
            return pos.X + Resources.Split_IDNamePair + pos.Y;
        }

        /// <summary>
        /// [X:Y] の形式で示される座標情報をPoint型に変換します。
        /// </summary>
        public static Point StringToPoint(string par) {
            //無効な形式の時は原点にする
            if (string.IsNullOrEmpty(par)) {
                return Point.Empty;
            }

            var spl = par.Split(Resources.Split_IDNamePair.ToCharArray());
            if (spl.Length == Common.GetEnumCount<XYPointIndices>() && int.TryParse(spl[(int) XYPointIndices.X], out var x) && int.TryParse(spl[(int) XYPointIndices.Y], out var y)) {
                return new Point(x, y);
            }
            return Point.Empty;
        }

        /// <summary>
        /// Size型を [幅x高さ] の形式で示される座標情報に変換します。
        /// </summary>
        public static string SizeToString(Size pos) {
            return pos.Width + Resources.Split_Size + pos.Height;
        }

        /// <summary>
        /// [幅x高さ] の形式で示される座標情報をSize型に変換します。
        /// </summary>
        public static Size StringToSize(string par) {
            //無効な形式の時はゼロサイズにする
            if (string.IsNullOrEmpty(par)) {
                return Size.Empty;
            }

            var spl = par.Split(Resources.Split_Size.ToCharArray());
            if (spl.Length == Common.GetEnumCount<XYPointIndices>() && int.TryParse(spl[(int) XYPointIndices.X], out var x) && int.TryParse(spl[(int) XYPointIndices.Y], out var y)) {
                return new Size(x, y);
            }
            return Size.Empty;
        }

        /// <summary>
        /// 指定された区切り記号に従って一連の文字列を生成します。
        /// </summary>
        /// <param name="splitChar">区切り記号</param>
        /// <param name="pars">区切る文字列の集合</param>
        /// <returns>区切り記号を含む一連の文字列</returns>
        public static string CreateSplittedString(char splitChar, params object[] pars) {
            //配列が無効な場合は空白で返す
            if (pars == null) {
                return "";
            }

            var buf = "";
            for (var idx = 0; idx < pars.Length; idx++) {
                buf += pars[idx]?.ToString();
                if (idx < pars.Length - 1) {
                    buf += splitChar;
                }
            }
            return buf;
        }

        /// <summary>
        /// ListBox/ComboBoxのリストをキーと値のペアの形式として初期化します。
        /// </summary>
        public static void SetListControlKeyValuePairMode(ListControl lst) {
            //項目を初期化する
            if (lst is ListBox ctl1) {
                ctl1.Items.Clear();
            } else if (lst is ComboBox ctl2) {
                ctl2.Items.Clear();
            }

            lst.DisplayMember = "Key";
            lst.ValueMember = "Value";
        }

        /// <summary>
        /// ListBoxの該当キーのインデックスを求めます。
        /// </summary>
        /// <returns>見つからなかった場合は -1</returns>
        public static int GetComboBoxIndexFromText<T>(ListBox lst, string key) {
            for (var i = 0; i < lst.Items.Count; i++) {
                if (((KeyValuePair<string, T>) lst.Items[i]).Key == key) {
                    return i;
                }
            }
            return -1;
        }

        /// <summary>
        /// ComboBoxの該当キーのインデックスを求めます。
        /// </summary>
        /// <returns>見つからなかった場合は -1</returns>
        public static int GetComboBoxIndexFromText<T>(ComboBox cmb, string key) {
            for (var i = 0; i < cmb.Items.Count; i++) {
                if (((KeyValuePair<string, T>) cmb.Items[i]).Key == key) {
                    return i;
                }
            }
            return -1;
        }

        /// <summary>
        /// 指定したコンテナーの中にある指定した型に一致するコントロールのリストを返します。
        /// これはサブコンテナーの中も探索対象です。
        /// </summary>
        public static List<Control> GetControlsOfType(Control ctl, Type ctlType) {
            var result = new List<Control>();
            foreach (Control subCtrl in ctl.Controls) {
                //対象コントロールを判定
                if (subCtrl.GetType() == ctlType) {
                    result.Add(subCtrl);
                }

                //サブコントロールを探索
                if (subCtrl.Controls.Count > 0) {
                    var subResult = GetControlsOfType(subCtrl, ctlType);
                    if (subResult.Count > 0) {
                        result.AddRange(subResult);
                    }
                }
            }
            return result;
        }

        /// <summary>
        /// 補間方法を適用します。
        /// </summary>
        /// <param name="g">Graphicsオブジェクト</param>
        public static void ApplyInterpolation(System.Drawing.Graphics g, PictureInterPolation ip) {
            switch (ip) {
                case PictureInterPolation.None:
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                    g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.Default;
                    g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
                    g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;      //ドットがずれないようにする
                    break;

                case PictureInterPolation.Bilinear:
                    Common.ApplyHighQualityDrawing(g);
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBilinear;
                    break;

                case PictureInterPolation.Bicubic:
                    Common.ApplyHighQualityDrawing(g);
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                    break;
            }
        }

        /// <summary>
        /// ハイクオリティな描画設定を適用します。
        /// </summary>
        /// <param name="g">Graphicsオブジェクト</param>
        public static void ApplyHighQualityDrawing(System.Drawing.Graphics g) {
            g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
        }

        /// <summary>
        /// 列挙体の項目数を取得します。
        /// </summary>
        /// <typeparam name="T">列挙体</typeparam>
        /// <returns>列挙体の項目数</returns>
        public static int GetEnumCount<T>() {
            var type = typeof(T);

            if (!type.IsEnum) {
                throw new Exception("指定されたオブジェクトが列挙体ではありません。");
            }

            return Enum.GetNames(type).Length;
        }

        /// <summary>
        /// IDと名前がセットになった文字列をそれぞれ分解してセットします。
        /// </summary>
        /// <param name="rawText">元の文字列</param>
        /// <param name="id">代入先</param>
        /// <param name="str">代入先</param>
        public static void SetIDName(string rawText, out int id, out string name) {
            var spl = rawText?.Split(Resources.Split_IDNamePair.ToArray());

            //不正な形式の場合はIDを既定値にして返す
            if (spl == null || spl.Length <= 1 || !int.TryParse(spl[(int) IDNameDataIndices.ID], out var tempid)) {
                id = 0;
                name = (spl != null) ? spl[0] : "";
                return;
            }

            id = tempid;
            name = spl[(int) IDNameDataIndices.Name];
        }

        /// <summary>
        /// バイト数を適切な単位に直します。
        /// </summary>
        /// <param name="bytes">バイト数</param>
        /// <returns>Byte, KB, MB, GB</returns>
        public static string GetFileSizeWithUnit(long bytes) {
            double rSize = bytes;
            if (rSize < 1024.0F) {
                return $"{rSize:0.00} Bytes";
            }
            rSize /= 1024.0F;
            if (rSize < 1024.0F) {
                return $"{rSize:0.00} KB";
            }
            rSize /= 1024.0F;
            if (rSize < 1024.0F) {
                return $"{rSize:0.00} MB";
            }
            rSize /= 1024.0F;
            return $"{rSize:0.00} GB";
        }

        /// <summary>
        /// 元のファイル名から競合ファイルを生成します。
        /// </summary>
        /// <param name="srcfileName">元のファイル名</param>
        /// <param name="userName">競合を発生させたユーザー名</param>
        /// <returns>生成された競合ファイル名（プロジェクトフォルダーを基準としたパス）</returns>
        public static string GenerateConflictFile(string srcfileName, string userName) {
            var newFileName = Common.GenerateConflictFileName(srcfileName, userName);

            //空の競合ファイル生成
            using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + newFileName))) {
            }

            return newFileName;
        }

        /// <summary>
        /// 元のファイル名から競合ファイル名を生成します。
        /// </summary>
        /// <param name="srcfileName">元のファイル名</param>
        /// <param name="userName">競合を発生させたユーザー名</param>
        /// <returns>有効な競合ファイル名（プロジェクトフォルダーを基準としたパス）</returns>
        public static string GenerateConflictFileName(string srcfileName, string userName) {
            var file = Path.GetFileNameWithoutExtension(srcfileName);       //ファイル名部分のみ抽出
            var dir = Path.GetDirectoryName(srcfileName);       //ディレクトリー部分は固定
            var ext = Path.GetExtension(srcfileName);           //拡張子も固定
            var cnt = 0;
            var newFileName = "";

            //存在しないファイル名を生成する
            do {
                cnt++;
                newFileName = $"{dir}\\{file} ({userName} 競合 {cnt}){ext}";
            } while (File.Exists(ProjectManager.ProjectPath + newFileName));

            return newFileName;
        }

        /// <summary>
        /// 指定した深さまでのノードを展開します。
        /// </summary>
        /// <param name="parentNode">親となるノード</param>
        /// <param name="depth">0のとき展開しない、それ以上は展開する階層の深さ</param>
        public static void ExpandTreeNodeToDepth(TreeNode parentNode, int depth) {
            if (depth == 0) {
                //展開すべき終端まで達した
                return;
            } else if (parentNode.Text.Length > 0 && parentNode.Text.Substring(0, 1) == Resources.NG_SystemFolderSymbol) {
                // $ から始まるシステムフォルダーは展開しない
                return;
            }

            parentNode.Expand();
            if (depth > 1) {
                //次の階層も展開する
                foreach (TreeNode node in parentNode.Nodes) {
                    Common.ExpandTreeNodeToDepth(node, depth - 1);
                }
            }
        }

        /// <summary>
        /// 指定したコントロール内の全コントロールのタブオーダーを自動設定します。
        /// Top値 -&gt; Left値 の優先順位です。
        /// </summary>
        /// <param name="ctl">親コンテナー</param>
        public static void SetAutoTabIndices(Control ctl) {
            if (ctl.Controls.Count == 0) {
                return;
            }

            var ctls = new List<Control>();
            var index = 0;
            foreach (Control item in ctl.Controls) {
                ctls.Add(item);
                if (item.Controls.Count > 0) {
                    //コンテナーである場合はそのサブコントロールをその内部で設定する
                    Common.SetAutoTabIndices(item);
                }
            }

            //コントロールの順番を決定する
            ctls.Sort((x, y) => {
                if (ctl is TableLayoutPanel) {
                    //テーブルレイアウト内のコントロールのときは 行->列 の順番に従う
                    var subx = (ctl as TableLayoutPanel).GetPositionFromControl(x);
                    var suby = (ctl as TableLayoutPanel).GetPositionFromControl(y);
                    if (subx.Row != suby.Row) {
                        return subx.Row - suby.Row;
                    } else {
                        return subx.Column - suby.Column;
                    }
                } else {
                    //通常比較
                    if (x.Top != y.Top) {
                        //まず上端位置を比較材料にする
                        return x.Top - y.Top;
                    } else {
                        //次に左端位置を比較材料にする
                        return x.Left - y.Left;
                    }
                }
            });

            //並び替えられた順番に従ってTabIndexを設定する
            foreach (var item in ctls) {
                item.TabIndex = index;
                index++;
            }
        }

        /// <summary>
        /// 現在のファイルアクセス権を使ってテキストをファイルに書き込みます。
        /// </summary>
        /// <param name="fs">FileStreamオブジェクト</param>
        /// <param name="text">書き込むテキスト</param>
        /// <returns>成功したかどうか</returns>
        public static bool WriteFileAll(ref FileStream fs, string text) {
            if (fs == null) {
                return false;
            } else if (!fs.CanWrite) {
                //読み取り専用のときは保存できない
                MessageBox.Show(Resources.MsgE_ReadOnly, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            //編集権がある場合は書き込みモードで開き直す
            var fileName = fs.Name;
            Common.EndFileAccessLock(ref fs);
            try {
                fs = new FileStream(fileName, FileMode.Create, FileAccess.ReadWrite, FileShare.Read);
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "テキストの保存"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            var W = new StreamWriter(fs, Common.SJIS);
            W.Write(text);
            W.Close();

            //再度読み込みモードで編集権を持ち直す
            return Common.StartFileAccessLock(ref fs, fs.Name.Substring(ProjectManager.ProjectPath.Length));
        }

        /// <summary>
        /// 現在のファイルアクセス権を使ってファイルを読み込んで返します。
        /// </summary>
        /// <param name="fs">FileStreamオブジェクト</param>
        /// <returns>読み込んだすべてのテキスト</returns>
        public static string ReadFileAll(FileStream fs) {
            if (fs == null) {
                return "";
            }
            var R = new StreamReader(fs, Common.SJIS);
            return R.ReadToEnd();
        }

        /// <summary>
        /// ファイルアクセス権の制御を開始します。
        /// </summary>
        /// <param name="fs">FileStreamオブジェクト</param>
        /// <param name="rPath">プロジェクトフォルダーを基準とした相対パス</param>
        /// <returns>編集権を得たかどうか</returns>
        public static bool StartFileAccessLock(ref FileStream fs, string rPath) {
            //一旦アクセス権を解除
            EndFileAccessLock(ref fs);
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                return false;       //対象ファイルが存在しない
            }

            //最初は編集権を得ようとする
            //注意：OpenOrCreateでないと読めないが、これだと書き込めないので書き込み時に変更する
            fs = Common.OpenFileEditMode(ProjectManager.ProjectPath + rPath);
            if (fs != null) {
                return true;        //編集権を獲得できた
            }

            //ダメだったら読み取り専用で開く
            fs = Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath);
            MessageBox.Show(Resources.MsgW_ReadOnly, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            return false;
        }

        /// <summary>
        /// 編集権を持った状態でファイルを開き、生成したFileStreamオブジェクトを返します。
        /// </summary>
        /// <param name="fullPath">対象ファイルへのフルパス</param>
        /// <returns>FileStreamオブジェクト。編集権を獲得できなかった場合はnullを返します。</returns>
        public static FileStream OpenFileEditMode(string fullPath) {
            try {
                return new FileStream(fullPath, FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.Read);
            } catch {
                return null;
            }
        }

        /// <summary>
        /// 書き込み専用でファイルを開き、生成したFileStreamオブジェクトを返します。
        /// </summary>
        /// <param name="fullPath">対象ファイルへのフルパス</param>
        /// <returns>FileStreamオブジェクト。失敗したらnullを返します。</returns>
        public static FileStream OpenFileWriteOnly(string fullPath) {
            //注意：FileShare.ReadWriteにするのは、最初に編集権を獲得したユーザーがFileShare.Readに設定しており、ここでさらにFileShare.Readを指定すると読み込みすらできなくなってしまうため
            try {
                return new FileStream(fullPath, FileMode.Create, FileAccess.ReadWrite, FileShare.Read);
            } catch {
                return null;
            }
        }

        /// <summary>
        /// 読み取り専用でファイルを開き、生成したFileStreamオブジェクトを返します。
        /// </summary>
        /// <param name="fullPath">対象ファイルへのフルパス</param>
        /// <returns>FileStreamオブジェクト。失敗したらnullを返します。</returns>
        public static FileStream OpenFileReadOnly(string fullPath) {
            //注意：FileShare.ReadWriteにするのは、最初に編集権を獲得したユーザーがFileShare.Readに設定しており、ここでさらにFileShare.Readを指定すると読み込みすらできなくなってしまうため
            try {
                return new FileStream(fullPath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            } catch {
                return null;
            }
        }

        /// <summary>
        /// ファイルアクセス権の制御を解放します。
        /// </summary>
        /// <param name="fs">FileStreamオブジェクト</param>
        public static void EndFileAccessLock(ref FileStream fs) {
            if (fs == null) {
                return;
            }

            fs.Close();

            try {
                fs.Dispose();
            } catch {
                //多重に破棄されてもスルー
            }
            fs = null;
        }

        /// <summary>
        /// 指定したパスが有効であるかを調べます。
        /// 無効である場合はそのパス名でファイル/フォルダーを生成できません。
        /// </summary>
        /// <param name="path">フルパス</param>
        /// <returns>有効であるかどうか</returns>
        public static bool CheckEnabledPath(string path) {
            if (string.IsNullOrEmpty(path)) {
                //空白の場合は問答無用でNG
                return false;
            }

            var OK = true;
            OK &= (path.IndexOfAny(Path.GetInvalidFileNameChars()) == -1);              //システムによって使用できない文字を含んでいるか
            OK &= (path.Substring(0, 1) != Resources.NG_SystemFolderSymbol);            //システムフォルダーとして定義された頭文字ではないか

            //追加的に許可しない文字についても検査する
            for (var i = 0; i < Resources.NG_String.Length; i++) {
                OK &= (path.IndexOf(Resources.NG_String.Substring(i, 1)) == -1);
            }

            return OK;
        }

        /// <summary>
        /// 与えられた文字列に指定した文字列が何回出現するかを調べます。
        /// </summary>
        /// <param name="src">対象文字列</param>
        /// <param name="findText">検索文字列</param>
        /// <returns>出現回数</returns>
        public static int StringFindCount(string src, string findText) {
            var cnt = 0;
            var idx = -findText.Length;

            while (true) {
                idx = src.IndexOf(findText, idx + findText.Length);
                if (idx == -1) {
                    break;
                }
                cnt++;
            }

            return cnt;
        }

        /// <summary>
        /// 与えられた文字列の先頭が指定した文字列であるかを調べます。先頭の空白は無視されます。
        /// </summary>
        /// <param name="src">対象文字列</param>
        /// <param name="findText">検索文字列</param>
        /// <returns>検索文字列が先頭にあるかどうか</returns>
        public static bool StringFindHead(string src, string findText) {
            src = Common.CutHeadTabs(src).TrimStart();
            return (src.IndexOf(findText) == 0);
        }

        /// <summary>
        /// パスの末尾に区切り記号が不足していたら追加します。
        /// ただし、空白文字列のときはそのまま返します。
        /// </summary>
        /// <param name="path">フォルダーへのパス</param>
        /// <returns>末尾に区切り記号が付いたパス</returns>
        public static string AddToPathSplit(string path) {
            if (string.IsNullOrEmpty(path)) {
                //空白のときは加工しない
                return "";
            } else if (path.Length > 0 && path.Substring(path.Length - 1, 1) == "\\") {
                //既に末尾に付いている場合は加工しない
                return path;
            } else {
                return path + "\\";
            }
        }

        /// <summary>
        /// 末尾に指定した文字がある場合は取り除きます。
        /// </summary>
        public static string CutLastChar(string text, char cutTargetChar) {
            return text.TrimEnd(new char[] { cutTargetChar });
        }

        /// <summary>
        /// 指定フォルダーを基点とするフォルダー構造を生成します。
        /// タブインデントで階層を表現したテキストを用います。このテキストの１行目は基点フォルダーです。
        /// [.] を含むものはファイルとみなし、空のテキストファイルを生成します。
        /// [$] を含むものは特殊ファイルとみなし、スキップされます。
        /// </summary>
        /// <param name="rootPath">基点パス</param>
        /// <param name="folderTreeText">フォルダー構造が記述されたテキスト</param>
        public static void CreateFolders(string rootPath, string folderTreeText) {
            //rootPathの末尾には必ず区切り記号を入れる
            rootPath = Common.AddToPathSplit(rootPath);

            using (var sf = new StringReader(folderTreeText)) {
                var Depth = 0;      //ルートフォルダー直下を 0 としたディレクトリーの深さ
                var buf = "";
                var directoryName = "";
                var fileName = "";

                //１行目はコメント扱い
                sf.ReadLine();

                while (sf.Peek() != -1) {
                    buf = sf.ReadLine();

                    var newDepth = Common.GetIndentDepth(buf);
                    if (newDepth > Depth) {
                        //次のフォルダーへ入る
                        Depth++;
                        directoryName += Common.AddToPathSplit(fileName);
                    } else if (newDepth < Depth) {
                        //前のフォルダーに下がる
                        for (; Depth > newDepth; Depth--) {
                            directoryName = Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(directoryName, '\\')));
                        }
                    }

                    fileName = buf.Replace("\t", "");
                    try {
                        if (fileName.IndexOf(".") == -1) {
                            Directory.CreateDirectory(rootPath + directoryName + fileName);
                        } else if (!File.Exists(rootPath + directoryName + fileName)) {
                            //ドット入りの名前はファイルとみなし、存在しなければ空のテキストファイルを生成する
                            File.WriteAllText(rootPath + directoryName + fileName, "", Common.SJIS);
                        }
                    } catch {
                        MessageBox.Show(Resources.MsgE_Failed.Replace("$", "ディレクトリー構造の生成"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }
            }
        }

        /// <summary>
        /// インデントの深さを取得します。
        /// </summary>
        /// <param name="lineText">一行文字列</param>
        /// <returns>先頭に連続するタブ文字の数</returns>
        public static int GetIndentDepth(string lineText) {
            if (lineText.Length > 0 && lineText.Substring(0, 1) == "\t") {
                var Depth = 1;

                while (lineText.Length > Depth && lineText.Substring(Depth, 1) == "\t") {
                    Depth++;
                }

                return Depth;
            }
            return 0;       //インデントなし
        }

        /// <summary>
        /// 先頭のタブ文字を除去します。
        /// </summary>
        /// <param name="str">元の文字列</param>
        /// <returns>先頭のタブ文字がカットされた文字列</returns>
        public static string CutHeadTabs(string str) {
            return str.TrimStart(new char[] { ' ', '\t' });         //先頭の空白カット
        }

        /// <summary>
        /// 基点となっているフォルダー名を除去した、２段目以降のファイルパスを返します。
        /// </summary>
        public static string CutRootFolderName(string rPath) {
            var temp = rPath;
            if (temp.IndexOf("\\") != -1) {
                temp = temp.Substring(temp.IndexOf("\\") + 1);
            }
            return temp;
        }

        /// <summary>
        /// コピー先のファイル名を自動で生成します。
        /// </summary>
        /// <param name="rPath">プロジェクトフォルダーを基準とした元のファイル名</param>
        /// <returns>コピー先のファイル名として使用できる文字列</returns>
        public static string GenerateCopyFileName(string rPath) {
            var buf = Path.GetFileName(rPath);

            for (var i = 1; File.Exists(ProjectManager.ProjectPath + Common.AddToPathSplit(Path.GetDirectoryName(rPath)) + buf); i++) {
                buf = Path.GetFileNameWithoutExtension(rPath) + $" ({i})" + Path.GetExtension(rPath);
            }

            return Common.AddToPathSplit(Path.GetDirectoryName(rPath)) + buf;
        }

        /// <summary>
        /// コピー先のフォルダー名を自動で生成します。
        /// </summary>
        /// <param name="rPath">プロジェクトフォルダーを基準とした元のフォルダー名</param>
        /// <returns>コピー先のフォルダー名として使用できる文字列</returns>
        public static string GenerateCopyFolderName(string rPath) {
            var buf = Path.GetFileName(rPath);
            var parent = Path.GetDirectoryName(rPath);

            //フォルダー名の部分を走査する
            for (var i = 1; Directory.Exists(ProjectManager.ProjectPath + Common.AddToPathSplit(parent) + buf); i++) {
                buf = Path.GetFileName(rPath) + $" ({i})";
            }

            return Common.AddToPathSplit(parent) + buf;
        }

        /// <summary>
        /// ImageIndexを正規の並び順としてタブページを昇順に並び替えます。
        /// </summary>
        /// <param name="tabs">対象とするタブコレクション</param>
        public static void SortTabByImageIndex(TabControl.TabPageCollection tabs) {
            var pages = new List<TabPage>();       //並び替えるために用意するコレクション
            foreach (TabPage item in tabs) {
                pages.Add(item);
            }

            //並び替えて反映
            pages.Sort((x, y) => (x as TabPage).ImageIndex - (y as TabPage).ImageIndex);
            tabs.Clear();
            tabs.AddRange(pages.ToArray());
        }

        /// <summary>
        /// 指定ファイルの拡張子がフィルターに該当するかどうかを調べます。
        /// </summary>
        /// <param name="fileNameWithExtension">ファイル名（フルパス、ファイル名のみ両方可）</param>
        /// <param name="filterExtension">拡張子の該当リスト [.jpg;.bmp;...]</param>
        /// <returns>ヒットした場合はtrue</returns>
        public static bool CheckFileExtensionFilter(string fileNameWithExtension, string filterExtension) {
            var filters = filterExtension.Split(Resources.Split_SimpleList.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            if (filters == null) {
                return false;
            }

            foreach (var filter in filters) {
                if (Path.GetExtension(fileNameWithExtension).ToLower() == filter) {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// コントロールのDoubleBufferedプロパティを有効にします。
        /// </summary>
        /// <param name="control">対象のコントロール</param>
        public static void EnableDoubleBuffering(Control control, Control[] excepts = null) {
            //除外リストに該当するかチェック
            if (excepts != null) {
                foreach (var except in excepts) {
                    if (except == control) {
                        return;
                    }
                }
            }

            control.GetType().InvokeMember(
                "DoubleBuffered",
                System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.SetProperty,
                null,
                control,
                new object[] { true }
            );

            //さらに下の階層のコントロールにも適用する
            foreach (Control sub in control.Controls) {
                Common.EnableDoubleBuffering(sub, excepts);
            }
        }
    }

    /// <summary>
    /// IDと名前がペアのデータ順序
    /// </summary>
    public enum IDNameDataIndices {
        ID,
        Name,
    }

    /// <summary>
    /// IDと値がペアのデータ順序
    /// </summary>
    public enum IDValueDataIndices {
        IDName,
        Value,
    }

    /// <summary>
    /// 座標情報の順序
    /// </summary>
    public enum XYPointIndices {
        X,
        Y,
    }

    /// <summary>
    /// 色のデータ順序
    /// </summary>
    public enum ColorDataIndices {
        Red,
        Green,
        Blue,
    }

    /// <summary>
    /// GDIが対応する補間方法
    /// </summary>
    public enum PictureInterPolation {
        None,
        Bilinear,
        Bicubic,
    }

    /// <summary>
    /// 拡大率
    /// </summary>
    public static class ViewScale {

        /// <summary>
        /// 有効な拡大率
        /// </summary>
        public enum ScaleRate {
            Normal,             // 等倍
            ThreeQuarters,      // 75%
            Half,               // 50%
            Quarter,            // 25%
        }

        /// <summary>
        /// 列挙体の要素から実際の倍率値を返します。
        /// </summary>
        public static double GetRate(ScaleRate scale) {
            switch (scale) {
                case ScaleRate.Normal:
                    return 1.00;
                case ScaleRate.ThreeQuarters:
                    return 0.75;
                case ScaleRate.Half:
                    return 0.50;
                case ScaleRate.Quarter:
                    return 0.25;
            }
            return 0.0;
        }

        /// <summary>
        /// 指定した拡大率を表記法にして返します。
        /// </summary>
        public static string GetName(ScaleRate scale) {
            return $"{(int) (GetRate(scale) * 100)}％";
        }

        /// <summary>
        /// すべての拡大率を表記法にして返します。
        /// これをコレクションにAddRangeで登録すれば、そのコレクションのインデックスと拡大率が対応関係になります。
        /// </summary>
        public static string[] GetNames() {
            var result = new List<string>();
            for (var i = 0; i < Common.GetEnumCount<ScaleRate>(); i++) {
                result.Add(GetName((ScaleRate) i));
            }
            return result.ToArray();
        }

        /// <summary>
        /// サイズに対して拡大率を適用します。
        /// </summary>
        public static void ApplyRateToSize(ref Size size, ScaleRate scale) {
            size.Width = (int) (size.Width * GetRate(scale));
            size.Height = (int) (size.Height * GetRate(scale));
        }

        /// <summary>
        /// 元のサイズに戻します。
        /// </summary>
        public static void RestoreOriginalSize(ref Size size, ScaleRate scale) {
            size.Width = (int) (size.Width / GetRate(scale));
            size.Height = (int) (size.Height / GetRate(scale));
        }

        /// <summary>
        /// 座標に対して拡大率を適用します。
        /// </summary>
        public static void ApplyRateToPoint(ref Point pos, ScaleRate scale) {
            pos.X = (int) (pos.X * GetRate(scale));
            pos.Y = (int) (pos.Y * GetRate(scale));
        }

        /// <summary>
        /// 元の座標に戻します。
        /// </summary>
        public static void RestoreOriginalPoint(ref Point pos, ScaleRate scale) {
            pos.X = (int) (pos.X / GetRate(scale));
            pos.Y = (int) (pos.Y / GetRate(scale));
        }
    }

    /// <summary>
    /// ファイルを開く指示を要求するイベントの引数
    /// </summary>
    public class RequestOpenFileEventArgs : EventArgs {
        public string FileName;
        public string UserName;
        public MainTabIndices TabIndex;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public RequestOpenFileEventArgs(string rPath, MainTabIndices index, string userName = "") {
            this.FileName = rPath;
            this.UserName = userName;
            this.TabIndex = index;
        }
    }

    /// <summary>
    /// D&D操作の対象オブジェクト
    /// </summary>
    public class DrugObject {
        private Point beforePoint;
        private Point afterPoint;
        public bool Drugging = false;
        public int TargetIndex = -1;

        //コンストラクター
        public DrugObject(int index) {
            this.Drugging = true;
            this.TargetIndex = index;
        }
        public DrugObject(int index, Point pos) {
            this.Drugging = true;
            this.TargetIndex = index;
            this.afterPoint = pos;
        }
        public DrugObject(int index, int x, int y) {
            this.Drugging = true;
            this.TargetIndex = index;
            this.afterPoint = new Point(x, y);
        }

        /// <summary>
        /// 操作距離を取得
        /// </summary>
        public Point GetDistance(Point pos) {
            this.beforePoint = this.afterPoint;
            this.afterPoint = pos;
            return new Point(this.afterPoint.X - this.beforePoint.X, this.afterPoint.Y - this.beforePoint.Y);
        }

        /// <summary>
        /// 操作距離を取得
        /// </summary>
        public Point GetDistance(int x, int y) {
            return this.GetDistance(new Point(x, y));
        }
    }
}
