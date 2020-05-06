using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace Editor.Module {
    using Properties;
    using SQObject = GenerateGlueCode.GenerateCPPDocument.SQObject;

    public static class SQ {

        /// <summary>
        /// 変数リストの列順序
        /// </summary>
        public enum ValueListColumns {
            Scope,
            Name,
            Row,
        }

        /// <summary>
        /// 関数リストの列順序
        /// </summary>
        public enum FuncListColumns {
            Name,
            Row,
            StatementCount,
            CyclomaticComplexity,
        }

        /// <summary>
        /// 共通変数リストの列順序
        /// </summary>
        public enum DBValueListColumns {
            TypeName,
            RawName,
            Name,
            Count,
        }

        /// <summary>
        /// 共通変数の使用場所リストの列順序
        /// </summary>
        public enum DBValueReferenceListColumns {
            Name,
            Row,
        }

        /// <summary>
        /// 両端のダブルクォートを削除します。
        /// </summary>
        public static string CutDoubleQuotation(string str) {
            var buf = str;

            //先頭
            if (buf.Length > 0 && buf.Substring(0, 1) == "\"") {
                buf = buf.Substring(1);
            }

            //末尾
            if (buf.Length > 0 && buf.Substring(buf.Length - 1, 1) == "\"") {
                buf = buf.Substring(0, buf.Length - 1);
            }

            return buf;
        }

        /// <summary>
        /// 指定した列挙体メンバーをスクリプト用の列挙体表記に変換します。
        /// </summary>
        public static string ToSQEnum(Type enumType, int member) {
            return enumType.Name + "." + Enum.ToObject(enumType, member).ToString();
        }

        /// <summary>
        /// 指定したスクリプト用の列挙体表記を基に対応する数値を取得します。
        /// </summary>
        public static int FromSQEnum(Type enumType, string member) {
            try {
                if (int.TryParse(member, out var val)) {
                    //整数に変換できる場合はそのまま通過
                    return (int) Enum.Parse(enumType, member);
                } else {
                    //文字列として入力されている場合、列挙体名であると想定し、メンバー部分を抜き出して変換する
                    if (member.IndexOf(".") != -1) {
                        member = member.Substring(member.IndexOf(".") + 1);
                    }
                    return (int) Enum.Parse(enumType, member);
                }
            } catch {
                return -1;
            }
        }

        /// <summary>
        /// 指定したノードの下にスクリプト標準命令を登録します。
        /// </summary>
        public static void SetSQStatementsTree(TreeNode root) {
            string buf;
            TreeNode tempNode = null;
            root.Nodes.Clear();

            using (var R = new StringReader(Resources.SquirrelKeywords)) {
                while (R.Peek() > -1) {
                    var node = new TreeNode();
                    buf = R.ReadLine();
                    var spl = buf.Split(new char[] { ';' });
                    var idx = 0;

                    //１番目は必ず識別子
                    node.Tag = spl[idx];
                    idx++;

                    //２番目は [末尾に追加する文字列]
                    if (spl.Length >= 5) {
                        node.Tag += spl[1];
                        idx++;

                        //３番目は [末尾にセミコロンを追加するかどうか]
                        if (spl.Length >= 6) {
                            node.Tag += ";";
                            idx++;
                        }
                    }

                    //４番目は分類
                    var found = false;
                    foreach (TreeNode g in root.Nodes) {
                        //分類ノードが既に作成されているか調べる
                        if (g.Text == spl[idx]) {
                            found = true;
                            tempNode = g;
                        }
                    }
                    if (!found) {
                        //現在の分類がまだ作成されていない場合は追加する
                        var gNode = new TreeNode {
                            Text = spl[idx]
                        };
                        root.Nodes.Add(gNode);
                        tempNode = gNode;
                    }

                    //５番目はsummary、６番目はcomment
                    if (string.IsNullOrEmpty(spl[idx + 1])) {
                        //summaryがない場合は識別子を代替させる
                        node.Text = node.Tag.ToString();
                    } else {
                        node.Text = spl[idx + 1];
                    }

                    //commentは句点で改行するやさしさ
                    node.ToolTipText = spl[idx + 2].Replace("。", "。\r\n");

                    tempNode.Nodes.Add(node);
                }
            }
        }

        /// <summary>
        /// 指定したノードの下にスクリプト定数を登録します。
        /// </summary>
        public static void SetSQConstantsTree(TreeNode root, string rPath) {
            root.Nodes.Clear();
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                System.Diagnostics.Debug.WriteLine($"{rPath}: 与えられたファイルが見つかりません。");
                return;
            }

            //スクリプトを読み込む
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath), Common.SJIS)) {
                GenerateGlueCode.GenerateSQDocument.LoadConstants(R);
            }

            //ツリーを生成する
            var currentNode = root;
            foreach (var obj in GenerateGlueCode.GenerateSQDocument.SQConstants) {
                var node = new TreeNode {
                    ToolTipText = obj.Comment,
                    Tag = obj         //詳細なオブジェクト情報はTagに格納する
                };

                //summaryがない場合は識別子で登録する
                if (!string.IsNullOrEmpty(obj.Summary)) {
                    node.Text = obj.Summary;
                } else if (!string.IsNullOrEmpty(obj.Name)) {
                    node.Text = obj.Name;
                } else {
                    //無名のノードは作成しない
                    continue;
                }

                //アイコンを設定して、ノードに追加する
                currentNode = Common.CreateNodePath(root, GenerateGlueCode.GenerateSQDocument.GetSummaryFromConstName(obj.ClassPath));
                switch (obj.SubType) {
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQConst:
                        Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPConst);
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnum:
                        Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPEnum);
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnumMember:
                        Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPEnumMember);
                        break;
                }

                currentNode.Nodes.Add(node);
            }
        }

        /// <summary>
        /// 指定したノードの下にスクリプト関数を登録します。
        /// </summary>
        public static void SetSQFunctionsTree(TreeNode root, string rPath) {
            root.Nodes.Clear();
            if (!File.Exists(ProjectManager.ProjectPath + rPath)) {
                System.Diagnostics.Debug.WriteLine($"{rPath}: 与えられたファイルが見つかりません。");
                return;
            }

            //スクリプトを読み込む
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath), Common.SJIS)) {
                GenerateGlueCode.GenerateSQDocument.LoadFunctions(R);
            }

            //ツリーを生成する
            var currentNode = root;
            foreach (var func in GenerateGlueCode.GenerateSQDocument.SQFunctions) {
                var node = new TreeNode();
                var parentPath = func.ClassPath;
                node.ToolTipText = func.Comment?.Replace("。", "。\r\n");
                node.Tag = func;         //詳細なオブジェクト情報はTagに格納する

                //summaryがない場合は識別子で登録する
                if (!string.IsNullOrEmpty(func.Summary)) {
                    node.Text = func.Summary;
                } else if (!string.IsNullOrEmpty(func.Name)) {
                    node.Text = func.Name;
                } else {
                    //無名のノードは作成しない
                    continue;
                }

                //オブジェクトによって指定された階層のノードに追加する
                currentNode = Common.CreateNodePath(root, parentPath);
                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPFunc);
                currentNode.Nodes.Add(node);
            }
        }

        /// <summary>
        /// 指定したノードの下にゲームプログラムがサポートするオブジェクト群を登録します。
        /// </summary>
        public static void SetCPPObjectsTree(TreeNode root) {
            //直下のノードのToolTipをやさしくする
            foreach (TreeNode node in root.Nodes) {
                node.ToolTipText = node.ToolTipText.Replace("。", "。\r\n");
            }

            //リソースから読み込む
            using (var R = new StringReader(Resources.SQCodeTree)) {
                GenerateGlueCode.GenerateCPPDocument.LoadDocumentByStream(R);
            }

            //仮名称（識別子）でツリーを生成する
            var currentNode = root;
            TreeNode parentNode = null;
            TreeNode tempNode = null;
            var parentPath = "";

            foreach (var obj in GenerateGlueCode.GenerateCPPDocument.SQObjects) {
                var node = new TreeNode {
                    Text = obj.Name,       //暫定的に識別子で登録する
                    Tag = obj             //詳細なオブジェクト情報はTagに格納する
                };
                obj.ClassPath = obj.ClassPath
                        .Replace("TS::", "")
                        .Replace("SQEventHandler::", "")
                        .Replace("::", "\\");      //区切り記号をTreeView用にする
                parentPath = Path.GetDirectoryName(obj.ClassPath);

                //種別に応じて振り分ける
                switch (obj.NodeID) {
                    case GenerateGlueCode.GenerateCPPDocument.ObjectType.Const:
                        //定数: 途中のノードは機械的なものなので自動生成にする
                        switch (obj.SubType) {
                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Enum:
                                currentNode = Common.CreateNodePath(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjEnum], parentPath);
                                tempNode = Common.GetExistsNode(currentNode, node.Text);
                                if (tempNode != null) {
                                    //生成しようとしているノードが既に存在する（メンバーが先に登録される等で先行的に仮生成された）場合はそれに置き換える
                                    node = tempNode;
                                    tempNode.Tag = obj;
                                } else {
                                    //存在しない場合は新規追加する
                                    currentNode.Nodes.Add(node);
                                }
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPEnum);
                                parentNode = node;
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.EnumMember:
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPEnumMember);
                                parentNode.Nodes.Add(node);
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Const:
                                currentNode = Common.CreateNodePath(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjConst], parentPath);
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPConst);
                                currentNode.Nodes.Add(node);
                                break;
                        }
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectType.Value:
                        //ノード直下に並べる
                        currentNode = Common.CreateNodePath(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjValue], parentPath);
                        Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPValue);
                        currentNode.Nodes.Add(node);
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectType.Function:
                        //ノード直下に並べる
                        currentNode = Common.CreateNodePath(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjFunc], parentPath);
                        Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPFunc);
                        currentNode.Nodes.Add(node);
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectType.GameObject:
                        //途中のノードもクラス階層を成すが、まずは識別子の階層で生成する
                        currentNode = Common.CreateNodePath(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjClass], parentPath);
                        switch (obj.SubType) {
                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class:
                                tempNode = Common.GetExistsNode(currentNode, node.Text);
                                if (tempNode != null) {
                                    //生成しようとしているノードが既に存在する（入れ子クラス等により先行的に仮生成された）場合はそれに置き換える
                                    node = tempNode;
                                    tempNode.Tag = obj;
                                } else {
                                    //存在しない場合は新規追加する
                                    currentNode.Nodes.Add(node);
                                }
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPClass);
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassFunc:
                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberFunc:
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPFunc);
                                currentNode.Nodes.Add(node);
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassValue:
                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberValue:
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPValue);
                                currentNode.Nodes.Add(node);
                                break;

                            case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberProperty:
                                Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.CPPProperty);
                                currentNode.Nodes.Add(node);
                                break;
                        }
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectType.SQEventHandler:
                        currentNode = Common.CreateNodePath(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjSQEventHandler], null);
                        Common.SetTreeNodeImageIndex(node, (int) CtrlComponent.Text.ctlCommandTree.CPPObjectIconIndices.SQEventHandler);
                        currentNode.Nodes.Add(node);
                        break;
                }
            }

            //変数/プロパティは、そのクラス型に対応するメンバーをコピーする
            //クラスは、派生元クラスがあるならそれらのメンバーをコピーする
            var nodes = new List<TreeNode>();
            nodes.AddRange(Common.GetNodeListToFlat(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjClass]));
            nodes.AddRange(Common.GetNodeListToFlat(root.Nodes[CtrlComponent.Text.ctlCommandTree.NodeNameCppObjValue]));
            foreach (var node in nodes) {
                var obj = node.Tag as SQObject;
                if (obj == null) {
                    continue;
                }

                if (obj.NodeID == GenerateGlueCode.GenerateCPPDocument.ObjectType.Value
                || obj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberProperty) {
                    //変数/プロパティ: 該当クラス型を検索

                    if (string.IsNullOrEmpty(obj.ReturnType) || obj.ReturnType.Substring(0, 1) != "C") {
                        //クラスでない型は処理しない
                        continue;
                    }

                    // [読み取り専用] の文字列を無視する
                    var idx = obj.ReturnType.IndexOf(" ");
                    var typeName = (idx == -1) ? obj.ReturnType : obj.ReturnType.Substring(0, idx);

                    foreach (var sub in nodes) {
                        var subobj = sub.Tag as SQObject;
                        if (subobj == null) {
                            continue;
                        } else if (subobj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class
                        && typeName == subobj.Name) {
                            //一致するクラスを発見: クラス以外のノードをコピーする
                            SQ.addNodeExceptClass(node, sub);
                            break;
                        }
                    }
                } else if (obj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class
                && !string.IsNullOrEmpty(obj.SuperClassName)) {
                    //派生元クラスがあるクラス

                    //派生元クラス名を抽出
                    var superClass = obj.SuperClassName;
                    int idx;
                    idx = superClass.IndexOf(",");
                    if (idx != -1) {
                        superClass = superClass.Substring(0, idx);
                    }
                    idx = superClass.LastIndexOf("::");
                    if (idx != -1) {
                        superClass = superClass.Substring(idx + 2);
                    }

                    //派生元クラスを検索
                    var found = false;
                    foreach (var sub in nodes) {
                        var subobj = sub.Tag as SQObject;
                        if (subobj == null) {
                            continue;
                        } else if (subobj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class && subobj.Name == superClass) {
                            //一致するクラスを発見
                            foreach (TreeNode subnode in sub.Nodes) {
                                node.Nodes.Add(subnode.Clone() as TreeNode);
                            }
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        System.Diagnostics.Debug.WriteLine("見つからない: " + superClass);
                    }
                }
            }

            //表示名とツールチップをセットする
            nodes = Common.GetNodeListToFlat(root);
            foreach (var node in nodes) {
                var obj = node.Tag as SQObject;
                if (obj == null) {
                    continue;
                }

                if (!string.IsNullOrEmpty(obj.Summary)) {
                    node.Text = obj.Summary;
                }
                switch (obj.SubType) {
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassFunc:
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.ClassValue:
                        //staticなメンバーには名称の頭に接頭辞を付ける
                        node.Text = CtrlComponent.Text.ctlCommandTree.ClassStaticMemberHeader + node.Text;
                        break;
                }

                //ツールチップのテキストをやさしくする
                node.ToolTipText = obj.Comment.Replace("。", "。\r\n");

                //先頭行に実際の識別子と型名（クラス以外）を追加する
                switch (obj.SubType) {
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Const:
                        node.ToolTipText = obj.Name + " [" + (obj.ReturnType ?? "定数") + "]\r\n" + node.ToolTipText;
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnum:
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Enum:
                        node.ToolTipText = obj.Name + " [列挙体]\r\n" + node.ToolTipText;
                        break;

                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnumMember:
                    case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.EnumMember:
                        node.ToolTipText = obj.Name + " [列挙体メンバー]\r\n" + node.ToolTipText;
                        break;

                    default:
                        if (obj.SubType != GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class) {
                            //プロパティは型名も追加する
                            node.ToolTipText = obj.Name + " [" + obj.ReturnType + "]\r\n" + node.ToolTipText;
                        } else {
                            node.ToolTipText = obj.Name + "\r\n" + node.ToolTipText;
                        }
                        break;
                }
            }
        }

        /// <summary>
        /// コマンドツリー：クラス以外のノードを対象ノードに追加します。
        /// </summary>
        /// <param name="root">追加先ノード</param>
        /// <param name="src">追加元ノード</param>
        private static void addNodeExceptClass(TreeNode dest, TreeNode src) {
            foreach (TreeNode node in src.Nodes) {
                var obj = node.Tag as SQObject;
                if (obj == null) {
                    continue;
                } else if (obj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class) {
                    continue;
                }

                //このノードを追加する
                var newNode = node.Clone() as TreeNode;
                dest.Nodes.Add(newNode);

                //再帰的に処理しない
                //if(node.Nodes.Count > 0) {
                //	//サブノードを処理する
                //	SQ.addNodeExceptClass(dest.Nodes[dest.Nodes.Count], node);
                //}
            }
        }

        /// <summary>
        /// 関数の呼び出しの文を基にして、関数名を返します。
        /// </summary>
        /// <param name="statement">関数の呼び出し処理のコード</param>
        /// <returns>関数名</returns>
        public static string GetFunctionNameByCall(string statement) {
            //余分な空白はすべて削除する
            statement = statement.Replace("\t", "").Trim();

            if (statement.IndexOf("(") == -1) {
                //不正なコード
                return "";
            }

            return statement.Substring(0, statement.IndexOf("(")).Trim();
        }

        /// <summary>
        /// 関数の呼び出しの文を基にして、引数リストを作成します。
        /// </summary>
        /// <param name="statement">関数の呼び出し処理のコード</param>
        /// <returns>引数リスト</returns>
        public static string[] GetArgumentListByCall(string statement) {
            var list = new List<string>();
            var buf = "";

            //丸括弧の中身だけを取り出す
            if (statement.IndexOf("(") == -1 || statement.IndexOf(")") == -1) {
                //不正なコード
                return null;
            }
            statement = Common.CutHeadTabs(statement);      //先頭のタブ文字を削除する
            buf = statement.Substring(statement.IndexOf("(") + 1);
            buf = buf.Substring(0, buf.LastIndexOf(")"));

            if (string.IsNullOrEmpty(buf)) {
                //引数がない場合は空配列で返す
                return new string[0];
            }

            var depth = 0;
            var marge = "";

            //全文字を走査
            for (var i = 0; i < buf.Length; i++) {
                var ch = buf.Substring(i, 1);
                if (ch == "(") {
                    //入れ子呼び出し開始点
                    depth++;
                    marge += ch;
                    continue;
                }
                if (ch == ")") {
                    depth--;
                    marge += ch;
                    continue;
                }
                if (depth == 0 && ch == ",") {
                    //ここまでの引数を確定
                    list.Add(marge.Trim());
                    marge = "";
                    continue;
                }
                marge += ch;
            }

            //最後に残った引数を追加して終わり
            if (!string.IsNullOrEmpty(marge)) {
                list.Add(marge.Trim());
            }

            return list.ToArray();
        }

        /// <summary>
        /// スクリプト内の関数リストをListView形式にして返します。
        /// １列目：行数、２列目：関数名
        /// </summary>
        /// <returns></returns>
        public static List<ListViewItem> GetFunctionList(string rPath) {
            var textlines = new List<string>();
            var list = new List<ListViewItem>();

            //スクリプトを行単位で読み込む
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath), Common.SJIS)) {
                while (!R.EndOfStream) {
                    textlines.Add(R.ReadLine());
                }
            }

            //全行の行頭を走査
            var i = 0;
            foreach (var line in textlines) {
                int index;

                //関数定義を検索
                var buf = Common.CutHeadTabs(line);
                var funcKeyword = Resources.SQ_Function.Trim();     //関数のキーワードだが、ここでは空白部分をカットする
                index = buf.IndexOf(funcKeyword, StringComparison.InvariantCulture);
                if (index != -1 && buf.IndexOf("(") != -1) {
                    //見つかった場合、関数名を抜き出す
                    buf = buf.Substring(index + funcKeyword.Length);
                    index = buf.IndexOf("(");
                    if (index == -1) {
                        //構文エラー
                        continue;
                    }

                    buf = buf.Substring(0, index).Trim();
                    if (string.IsNullOrEmpty(buf)) {
                        //無名関数
                        buf = "(無名関数)";
                    }

                    var item = new ListViewItem {
                        Text = buf
                    };
                    item.SubItems.Add((i + 1).ToString());        //行番号
                    item.SubItems.Add("?");     //ステートメント数
                    item.SubItems.Add("?");     //サイクロマティック複雑度

                    //引数リストをTagに入れる
                    item.Tag = SQ.GetArgumentListByCall(line);

                    list.Add(item);
                }
                i++;
            }
            return list;
        }

        /// <summary>
        /// スクリプト内の変数をListViewItemのリストにして返します。
        /// １列目：種別、２列目：変数名と代入式
        /// </summary>
        public static List<ListViewItem> GetValueList(string rPath) {
            var textlines = new List<string>();
            var list = new List<ListViewItem>();

            //スクリプトを行単位で読み込む
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath), Common.SJIS)) {
                while (!R.EndOfStream) {
                    textlines.Add(R.ReadLine());
                }
            }

            //全行の行頭を走査
            var i = 0;
            foreach (var line in textlines) {
                var buf = Common.CutHeadTabs(line);
                string temp;
                int index, sub;

                //定数を検索
                index = buf.IndexOf(Resources.SQ_ValueConstant, StringComparison.Ordinal);
                if (index != -1) {
                    //見つかった場合、識別子を抜き出す
                    temp = buf.Substring(index + Resources.SQ_ValueConstant.Length);
                    if (temp.IndexOf("=") != -1) {
                        temp = temp.Substring(0, temp.IndexOf("=")).Trim();
                    }

                    var item = new ListViewItem {
                        Text = "定数"
                    };
                    item.SubItems.Add(temp);
                    item.SubItems.Add((i + 1).ToString());        //行番号
                    list.Add(item);
                }

                //グローバル変数を検索
                index = buf.IndexOf(Resources.SQ_ValueGlobal, StringComparison.Ordinal);
                if (index != -1) {
                    //見つかった場合、識別子を抜き出す
                    var item = new ListViewItem {
                        Text = "外部"
                    };
                    item.SubItems.Add(buf.Substring(0, index).Trim());
                    item.SubItems.Add((i + 1).ToString());        //行番号
                    list.Add(item);
                }

                //ローカル変数を検索
                index = buf.IndexOf(Resources.SQ_ValueLocal, StringComparison.InvariantCulture);
                sub = buf.IndexOf(Resources.SQ_For, StringComparison.InvariantCulture);     //同じ行にfor文がある場合、カウンター変数とみなしてスキップする
                if (index != -1 && sub == -1) {
                    //見つかった場合、識別子を抜き出す
                    temp = buf.Substring(index + Resources.SQ_ValueLocal.Length);
                    if (temp.IndexOf("=") != -1) {
                        temp = temp.Substring(0, temp.IndexOf("=")).Trim();
                    }

                    var item = new ListViewItem {
                        Text = "自動"
                    };
                    item.SubItems.Add(temp);
                    item.SubItems.Add((i + 1).ToString());        //行番号
                    list.Add(item);
                }

                i++;
            }
            return list;
        }

        /// <summary>
        /// 指定したスクリプトファイルをもとに、コードメトリックスを計算します。
        /// </summary>
        public static void CalcCodeMetrics(string rPath, List<ListViewItem> funcList) {
            var list = new List<ListViewItem>();

            //行ごとにテキストを管理する
            var txt = new TextBox();
            using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + rPath), Common.SJIS)) {
                txt.Text = R.ReadToEnd();
            }

            //関数リストをジャンプリストとして関数ごとに計算する
            for (var i = 0; i < funcList.Count; i++) {
                var lineIdx = int.Parse(funcList[i].SubItems[(int) FuncListColumns.Row].Text) - 1;
                if (txt.Lines.Length <= lineIdx                                              //行番号が範囲外になっていて、関数リストが古いものである可能性がある
                || txt.Lines[lineIdx].IndexOf(Resources.SQ_Function.Trim()) == -1) {        //ジャンプ先が関数定義になっていないため、関数リストが古いものである可能性がある
                    MessageBox.Show("計算中にエラーが発生しました。\r\n関数リストを更新して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                //始点は関数定義の次の行に設定する
                lineIdx++;

                //終点を設定する
                var endLineIdx = lineIdx;
                if (i + 1 < funcList.Count) {
                    //次のジャンプ先まで読み進める
                    endLineIdx = int.Parse(funcList[i + 1].SubItems[(int) FuncListColumns.Row].Text) - 1 - 1;
                } else {
                    //ファイル末尾まで読み進める
                    endLineIdx = txt.Lines.Length - 1;
                }

                //計算処理
                var statementCount = 0;
                var cycCount = 1;
                for (var line = lineIdx; line <= endLineIdx; line++) {
                    //コメント行は無視する
                    if (Common.CutHeadTabs(txt.Lines[line]).IndexOf("//") == 0) {
                        continue;
                    }

                    //行コメントは削除する: 範囲コメントは対応せずコード扱いとなる
                    var idx = txt.Lines[line].IndexOf("//");
                    if (idx != -1) {
                        txt.Lines[line] = txt.Lines[line].Substring(0, idx);
                    }

                    //ステートメント数: セミコロン数 + 制御文キーワード数
                    var ctrlStatementCount = 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "switch") ? 1 : 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "while") ? 1 : 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "for") ? 1 : 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "case") ? 1 : 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "default") ? 1 : 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "continue") ? 1 : 0;
                    ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "catch") ? 1 : 0;
                    if (txt.Lines[line].IndexOf("else") != -1) {
                        // else if を 1 として数える
                        ctrlStatementCount++;
                    } else {
                        ctrlStatementCount += Common.StringFindHead(txt.Lines[line], "if") ? 1 : 0;
                    }
                    if (!Common.StringFindHead(txt.Lines[line], "for")) {
                        //for文の中のセミコロンは除外する
                        statementCount += Common.StringFindCount(txt.Lines[line], ";");
                    }
                    statementCount += ctrlStatementCount;

                    //サイクロマティック複雑度: 1 + 制御文キーワード数 + 論理演算子数 + 三項演算子数
                    cycCount += ctrlStatementCount;
                    cycCount += Common.StringFindCount(txt.Lines[line], "&&");
                    cycCount += Common.StringFindCount(txt.Lines[line], "||");
                    cycCount += (Common.StringFindCount(txt.Lines[line], "?") > 0 && Common.StringFindCount(txt.Lines[line], ":") > 0) ? Common.StringFindCount(txt.Lines[line], "?") : 0;
                }

                //関数リストの行を更新する
                funcList[i].SubItems[(int) FuncListColumns.StatementCount].Text = statementCount.ToString();
                funcList[i].SubItems[(int) FuncListColumns.CyclomaticComplexity].Text = cycCount.ToString();
            }
        }
    }
}
