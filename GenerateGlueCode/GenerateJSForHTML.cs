using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace GenerateGlueCode {
    /// <summary>
    /// 組み込みオブジェクトのツリー構造とリファレンスドキュメントをHTMLマニュアル用に変換します。
    /// </summary>
    public class GenerateJSForHTML {

        /// <summary>
        /// ルートノード群
        /// </summary>
        private enum RootNode {
            Const,
            Enum,
            Function,
            Class,
            Value,
            EventHandler,
        }

        /// <summary>
        /// ツリーの出力準備に使う内部ノード
        /// </summary>
        private class JSNode : ICloneable {
            public string Text;
            public GenerateCPPDocument.SQObject obj = null;
            public List<JSNode> SubNodes = new List<JSNode>();

            /// <summary>
            /// コンストラクター
            /// </summary>
            /// <param name="txt">表示名</param>
            public JSNode(string txt) {
                this.Text = txt;
            }

            /// <summary>
            /// サブノードをソートします。
            /// </summary>
            public void Sort() {
                this.SubNodes.Sort((x, y) => {
                    //種別で比較する
                    if (x.obj != null && y.obj != null) {
                        if (x.obj.SubType != y.obj.SubType) {
                            //クラスが一番上に来るようにする
                            var xType = (x.obj.SubType != GenerateCPPDocument.ObjectSubType.Class) ? (int) x.obj.SubType : -1;
                            var yType = (y.obj.SubType != GenerateCPPDocument.ObjectSubType.Class) ? (int) y.obj.SubType : -1;
                            return -(x.obj.SubType - y.obj.SubType);        //種別順とは逆順にする
                        } else {
                            //表示名で比較する
                            return string.Compare(x.obj.Summary, y.obj.Summary);
                        }
                    }

                    //識別子で比較する
                    return string.Compare(x.Text, y.Text);
                });

                //サブノードの中もソートする
                foreach (var subNode in this.SubNodes) {
                    subNode.Sort();
                }
            }

            /// <summary>
            /// このノードをコピーします。
            /// </summary>
            public object Clone() {
                var newInstance = new JSNode(this.Text) {
                    obj = this.obj     //SQObjectは参照をコピーする
                };
                foreach (var subNode in this.SubNodes) {
                    newInstance.SubNodes.Add(subNode.Clone() as JSNode);
                }
                return newInstance;
            }
        }

        /// <summary>
        /// 内部ノードをフラットにします。
        /// </summary>
        private static List<JSNode> GetFlatJSNodes(JSNode root) {
            var nodes = new List<JSNode>();
            foreach (var subNode in root.SubNodes) {
                nodes.Add(subNode);
                if (subNode.SubNodes.Count > 0) {
                    nodes.AddRange(GetFlatJSNodes(subNode));
                }
            }
            return nodes;
        }

        /// <summary>
        /// 指定した相対パスが存在する場合は該当するノードを、存在しない場合はnullを返します。
        /// </summary>
        /// <param name="root">基点となるノード</param>
        /// <param name="path">基点となるノードからのパス</param>
        /// <returns>取得された末端のノード</returns>
        private static JSNode GetExistsNode(JSNode root, string path) {
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
            foreach (var node in root.SubNodes) {
                if (node.Text == nodeName) {
                    if (string.IsNullOrEmpty(laterPath)) {
                        //既存の末端ノード
                        return node;
                    } else {
                        //既存の中間ノード
                        return GetExistsNode(node, laterPath);
                    }
                }
            }

            //見つからなかった
            return null;
        }

        /// <summary>
        /// 指定したパスに基づいてツリー構造を生成します。
        /// </summary>
        /// <param name="root">基点となるノード</param>
        /// <param name="path">基点となるノードからのパス</param>
        /// <returns>生成または取得された末端のノード</returns>
        private static JSNode CreateNodePath(JSNode root, string path) {
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
            foreach (var node in root.SubNodes) {
                if (node.Text == nodeName) {
                    if (string.IsNullOrEmpty(laterPath)) {
                        //既存の末端ノード
                        return node;
                    } else {
                        //既存の中間ノード
                        return CreateNodePath(node, laterPath);
                    }
                }
            }

            //見つからなかったら新規作成する
            var newNode = new JSNode(nodeName);
            root.SubNodes.Add(newNode);

            if (string.IsNullOrEmpty(laterPath)) {
                //新規の末端ノード
                return newNode;
            } else {
                //新規の中間ノード
                return CreateNodePath(newNode, laterPath);
            }
        }

        /// <summary>
        /// 内部ノードを生成します。
        /// </summary>
        private static void CreateJSNodes(JSNode root) {
            var currentNode = root;
            JSNode parentNode = null;
            JSNode tempNode = null;
            var parentPath = "";

            foreach (var obj in GenerateCPPDocument.SQObjects) {
                var node = new JSNode(obj.Name) {
                    obj = obj
                };    //暫定的に識別子で登録する
                obj.ClassPath = obj.ClassPath
                        .Replace("TS::", "")
                        .Replace("SQEventHandler::", "")
                        .Replace("::", "\\");      //区切り記号をTreeView用にする
                parentPath = Path.GetDirectoryName(obj.ClassPath);

                //種別に応じて振り分ける
                switch (obj.NodeID) {
                    case GenerateCPPDocument.ObjectType.Const:
                        //定数: 途中のノードは機械的なものなので自動生成にする
                        switch (obj.SubType) {
                            case GenerateCPPDocument.ObjectSubType.Enum:
                                currentNode = CreateNodePath(root.SubNodes[(int) RootNode.Enum], parentPath);
                                tempNode = GetExistsNode(currentNode, node.Text);
                                if (tempNode != null) {
                                    //生成しようとしているノードが既に存在する（メンバーが先に登録される等で先行的に仮生成された）場合はそれに置き換える
                                    node = tempNode;
                                    tempNode.obj = obj;
                                } else {
                                    //存在しない場合は新規追加する
                                    currentNode.SubNodes.Add(node);
                                }
                                parentNode = node;
                                break;

                            case GenerateCPPDocument.ObjectSubType.EnumMember:
                                parentNode.SubNodes.Add(node);
                                break;

                            case GenerateCPPDocument.ObjectSubType.Const:
                                currentNode = CreateNodePath(root.SubNodes[(int) RootNode.Const], parentPath);
                                currentNode.SubNodes.Add(node);
                                break;
                        }
                        break;

                    case GenerateCPPDocument.ObjectType.Value:
                        //ノード直下に並べる
                        currentNode = CreateNodePath(root.SubNodes[(int) RootNode.Value], parentPath);
                        currentNode.SubNodes.Add(node);
                        break;

                    case GenerateCPPDocument.ObjectType.Function:
                        //ノード直下に並べる
                        currentNode = CreateNodePath(root.SubNodes[(int) RootNode.Function], parentPath);
                        currentNode.SubNodes.Add(node);
                        break;

                    case GenerateCPPDocument.ObjectType.GameObject:
                        //途中のノードもクラス階層を成すが、まずは識別子の階層で生成する
                        currentNode = CreateNodePath(root.SubNodes[(int) RootNode.Class], parentPath);
                        switch (obj.SubType) {
                            case GenerateCPPDocument.ObjectSubType.Class:
                                tempNode = GetExistsNode(currentNode, node.Text);
                                if (tempNode != null) {
                                    //生成しようとしているノードが既に存在する（入れ子クラス等により先行的に仮生成された）場合はそれに置き換える
                                    node = tempNode;
                                    tempNode.obj = obj;
                                } else {
                                    //存在しない場合は新規追加する
                                    currentNode.SubNodes.Add(node);
                                }
                                break;

                            case GenerateCPPDocument.ObjectSubType.ClassFunc:
                            case GenerateCPPDocument.ObjectSubType.MemberFunc:
                                currentNode.SubNodes.Add(node);
                                break;

                            case GenerateCPPDocument.ObjectSubType.ClassValue:
                            case GenerateCPPDocument.ObjectSubType.MemberValue:
                                currentNode.SubNodes.Add(node);
                                break;

                            case GenerateCPPDocument.ObjectSubType.MemberProperty:
                                currentNode.SubNodes.Add(node);
                                break;
                        }
                        break;

                    case GenerateCPPDocument.ObjectType.SQEventHandler:
                        currentNode = CreateNodePath(root.SubNodes[(int) RootNode.EventHandler], null);
                        currentNode.SubNodes.Add(node);
                        break;
                }
            }

            //派生元クラスがある場合、それらのメンバーをコピーする
            List<JSNode> nodes;
            nodes = GetFlatJSNodes(root.SubNodes[(int) RootNode.Class]);
            foreach (var node in nodes) {
                if (node.obj == null) {
                    continue;
                }

                if (!string.IsNullOrEmpty(node.obj.SuperClassName)) {
                    //派生元クラスがあるクラス
                    var superClass = node.obj.SuperClassName;
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
                        if (sub.obj == null) {
                            continue;
                        } else if (sub.obj.SubType == GenerateCPPDocument.ObjectSubType.Class && sub.obj.Name == superClass) {
                            //一致するクラスを発見
                            foreach (var subnode in sub.SubNodes) {
                                node.SubNodes.Add(subnode.Clone() as JSNode);
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
        }

        /// <summary>
        /// 指定した深さのインデントを作ります。
        /// </summary>
        private static string GetIndent(int depth) {
            var buf = "";
            for (var i = 0; i < depth; i++) {
                buf += "\t";
            }
            return buf;
        }

        /// <summary>
        /// 展開可能なノードを出力します。
        /// </summary>
        private static void WriteExtendableNode(JSNode node, StreamWriter W, int depth) {
            if (node.obj == null && node.SubNodes.Count == 0) {
                //サブノードもなく、このノードもリンクが生成できない場合はそもそも出力しない
                return;
            }

            var buf = "";
            buf += $"+ '{GetIndent(depth)}<div class=\"section\">'" + "\r\n";
            depth++;
            if (node.obj == null) {
                //このノードにリンクを作らない場合
                buf += $"+ '{GetIndent(depth)}<div class=\"title\"><span class=\"arrow\"></span>{node.Text}</div>'" + "\r\n";
            } else {
                //このノードに説明がついている場合
                buf += $"+ '{GetIndent(depth)}<div class=\"title\"><span class=\"arrow\"></span><a href=\"#\" onclick=\"GenerateAutoDocument(\\'{node.obj.ClassPath.Replace("\\", ":")}\\');\">{node.obj.Summary}</a></div>'" + "\r\n";
            }
            buf += $"+ '{GetIndent(depth)}<div class=\"content\">'";
            W.WriteLine(buf);

            buf = "";
            depth++;

            //一番上から順に走査し、一番深いサブノードがあるノードから先に記述する
            var ulFlag = false;
            foreach (var subNode in node.SubNodes) {
                if (subNode.SubNodes.Count > 0) {
                    //このサブノードが展開できるものである場合は再帰呼び出しへ
                    WriteExtendableNode(subNode, W, depth);
                } else {
                    //このサブノードが展開できないものである場合は箇条書きへ
                    if (!ulFlag) {
                        //サブノードが存在しないノードが出現したら、以降すべてにおいてサブノードは存在しない
                        buf += $"+ '{GetIndent(depth)}<ul>'" + "\r\n";
                        ulFlag = true;
                        depth++;
                    }
                    buf += $"+ '{GetIndent(depth)}<li><a href=\"#\" onclick=\"GenerateAutoDocument(\\'{subNode.obj.ClassPath.Replace("\\", ":")}\\');\">{subNode.obj.Summary}</a></li>'" + "\r\n";
                }
            }
            if (ulFlag) {
                depth--;
                buf += $"+ '{GetIndent(depth)}</ul>'" + "\r\n";
            }

            depth--;
            buf += $"+ '{GetIndent(depth)}</div>'" + "\r\n";
            depth--;
            buf += $"+ '{GetIndent(depth)}</div>'";
            W.WriteLine(buf);
        }

        /// <summary>
        /// 既に読み込まれているSQObjectsから生成します。
        /// </summary>
        public static void Generate() {
            Program.CurrentDir = @"..\Editor\Manual\js\";
            var DestTreeFile = "auto-tree.js";
            var DestTreeDocument = "auto-document.js";

            #region <内部ツリー生成>
            Console.WriteLine("内部ツリーを生成しています...");
            var root = new JSNode("組み込みオブジェクト");
            for (var i = 0; i < Enum.GetValues(typeof(RootNode)).Length; i++) {
                switch ((RootNode) i) {
                    case RootNode.Const:
                        root.SubNodes.Add(new JSNode("定数"));
                        break;
                    case RootNode.Enum:
                        root.SubNodes.Add(new JSNode("列挙体"));
                        break;
                    case RootNode.Function:
                        root.SubNodes.Add(new JSNode("グローバル関数"));
                        break;
                    case RootNode.Value:
                        root.SubNodes.Add(new JSNode("グローバル変数"));
                        break;
                    case RootNode.Class:
                        root.SubNodes.Add(new JSNode("クラス"));
                        break;
                    case RootNode.EventHandler:
                        root.SubNodes.Add(new JSNode("イベントハンドラー"));
                        break;
                }
            }
            CreateJSNodes(root);
            root.Sort();
            #endregion

            #region <ツリーJS生成>
            Console.WriteLine("ツリー生成用のJavaScriptを作成します...");
            using (var W = new StreamWriter(Program.CurrentDir + DestTreeFile, false, Encoding.UTF8)) {
                //固定部分
                W.WriteLine("// JavaScript Document");
                W.WriteLine("// 「組み込みオブジェクト」ツリーと同様の構造を出力します。");
                W.WriteLine("document.open();");
                W.WriteLine("");
                W.WriteLine("var text = \"\"");
                W.WriteLine("");

                W.WriteLine("+ '<div class=\"section\">'");
                W.WriteLine("+ '<div class=\"title\"><span class=\"arrow\"></span>組み込みオブジェクト</div>'");
                W.WriteLine("+ '	<div class=\"content\">'");

                //可変部分
                foreach (var node in root.SubNodes) {
                    WriteExtendableNode(node, W, 2);
                }

                //固定部分
                W.WriteLine("+ '	</div>'");
                W.WriteLine("+ '</div>'");
                W.WriteLine(";");
                W.WriteLine("");
                W.WriteLine("document.write(text);");
                W.WriteLine("document.close();");
            }
            #endregion

            #region <ドキュメントJS生成>
            Console.WriteLine("ドキュメント用のJavaScriptを作成します...");
            using (var W = new StreamWriter(Program.CurrentDir + DestTreeDocument, false, Encoding.UTF8)) {
                //固定部分
                W.WriteLine("// JavaScript Document");
                W.WriteLine("// 「組み込みオブジェクト」ツリーに対応するドキュメントを出力します。");
                W.WriteLine("function GenerateAutoDocument(strClassPath) {");
                W.WriteLine("	var div = document.getElementById(\"mainContainer\");");
                W.WriteLine("	div.innerHTML = \"<div id =\\\"breadcrumb\\\"><a href=\\\"index.html\\\">HOME</a> &gt; 組み込みオブジェクト</div>\";");
                W.WriteLine("	div.innerHTML += generator(strClassPath);");
                //W.WriteLine("	div.innerHTML += generator[strClassPath]();");
                W.WriteLine("}");
                W.WriteLine("");
                W.WriteLine("function generator(strClassPath) {");
                //W.WriteLine("var generator = {");
                W.WriteLine("	switch(strClassPath) {");

                //可変部分
                foreach (var obj in GenerateCPPDocument.SQObjects) {
                    var buf = $"		case \"{obj.ClassPath.Replace("\\", ":")}\": return \"";
                    //string buf = $"[\"{obj.ClassPath.Replace("\\", ":")}\"]() {{ return \"";

                    //タイトル見出し
                    buf += $"<h2>{obj.Summary}</h2>";
                    if (!string.IsNullOrEmpty(obj.Comment)) {
                        //DXライブラリのリファレンスマニュアルにはリンクを挿入する
                        if (obj.Comment.IndexOf("DXライブラリのリファレンスマニュアル") != -1) {
                            obj.Comment = obj.Comment.Replace("DXライブラリのリファレンスマニュアル", "DXライブラリの<a href=\\\"http://dxlib.o.oo7.jp/dxfunc.html\\\" target=\\\"_blank\\\">リファレンスマニュアル</a>");
                        }

                        //末尾以外の句点には改行を挿入する
                        buf += $"<div class=\\\"text\\\">{obj.Comment.TrimEnd(new char[] { '。' }).Replace("。", "。<br>")}。<hr>";
                    } else {
                        buf += $"<div class=\\\"text\\\">特に解説はありません。<hr>";
                    }

                    //種別に応じて表記内容を変える
                    switch (obj.NodeID) {
                        case GenerateCPPDocument.ObjectType.Const:
                            switch (obj.SubType) {
                                case GenerateCPPDocument.ObjectSubType.Const:
                                    buf += "<h3>定義位置</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ClassPath.Replace("\\", "::")}</div>";
                                    buf += "<h3>オブジェクト種別</h3>";
                                    buf += "<div class=\\\"text\\\">定数</div>";
                                    break;

                                case GenerateCPPDocument.ObjectSubType.Enum:
                                    buf += "<h3>定義位置</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ClassPath.Replace("\\", "::")}</div>";
                                    buf += "<h3>オブジェクト種別</h3>";
                                    buf += "<div class=\\\"text\\\">列挙体</div>";
                                    break;

                                case GenerateCPPDocument.ObjectSubType.EnumMember:
                                    buf += "<h3>定義位置</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ClassPath.Replace("\\", "::")}</div>";
                                    buf += "<h3>オブジェクト種別</h3>";
                                    buf += "<div class=\\\"text\\\">列挙体メンバー</div>";
                                    break;
                            }
                            break;

                        case GenerateCPPDocument.ObjectType.Value:
                        case GenerateCPPDocument.ObjectType.GameObject:
                            switch (obj.SubType) {
                                case GenerateCPPDocument.ObjectSubType.Class:
                                    buf += "<h3>定義位置</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ClassPath.Replace("\\", "::")}</div>";
                                    buf += "<h3>オブジェクト種別</h3>";
                                    buf += "<div class=\\\"text\\\">クラス</div>";
                                    buf += "<h3>継承元クラス</h3>";
                                    if (!string.IsNullOrEmpty(obj.SuperClassName)) {
                                        buf += $"<div class=\\\"text\\\">{obj.SuperClassName}</div>";
                                    } else {
                                        buf += $"<div class=\\\"text\\\">継承しているクラスはありません。</div>";
                                    }
                                    break;

                                case GenerateCPPDocument.ObjectSubType.ClassValue:
                                case GenerateCPPDocument.ObjectSubType.GlobalValue:
                                case GenerateCPPDocument.ObjectSubType.MemberValue:
                                case GenerateCPPDocument.ObjectSubType.MemberProperty:
                                    buf += "<h3>定義位置</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ClassPath.Replace("\\", "::")}</div>";
                                    buf += "<h3>オブジェクト種別</h3>";
                                    switch (obj.SubType) {
                                        case GenerateCPPDocument.ObjectSubType.ClassValue:
                                            buf += "<div class=\\\"text\\\">静的変数</div>";
                                            break;
                                        case GenerateCPPDocument.ObjectSubType.GlobalValue:
                                            buf += "<div class=\\\"text\\\">グローバル変数</div>";
                                            break;
                                        case GenerateCPPDocument.ObjectSubType.MemberValue:
                                            buf += "<div class=\\\"text\\\">メンバー変数</div>";
                                            break;
                                        case GenerateCPPDocument.ObjectSubType.MemberProperty:
                                            buf += "<div class=\\\"text\\\">メンバープロパティ</div>";
                                            break;
                                    }
                                    buf += "<h3>型</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ReturnType}</div>";
                                    break;

                                case GenerateCPPDocument.ObjectSubType.ClassFunc:
                                case GenerateCPPDocument.ObjectSubType.GlobalFunc:
                                case GenerateCPPDocument.ObjectSubType.MemberFunc:
                                    buf += "<h3>定義位置</h3>";
                                    buf += $"<div class=\\\"text\\\">{obj.ClassPath.Replace("\\", "::")}</div>";
                                    buf += "<h3>オブジェクト種別</h3>";
                                    switch (obj.SubType) {
                                        case GenerateCPPDocument.ObjectSubType.ClassFunc:
                                            buf += "<div class=\\\"text\\\">静的関数</div>";
                                            break;
                                        case GenerateCPPDocument.ObjectSubType.GlobalFunc:
                                            buf += "<div class=\\\"text\\\">グローバル関数</div>";
                                            break;
                                        case GenerateCPPDocument.ObjectSubType.MemberFunc:
                                            buf += "<div class=\\\"text\\\">メンバー関数</div>";
                                            break;
                                    }

                                    //引数リスト
                                    buf += "<h3>引数リスト</h3><div class=\\\"text\\\">";
                                    if (obj.ArgumentList.Count > 0) {
                                        buf += "<ul>";
                                        for (var i = 0; i < obj.ArgumentList.Count; i++) {
                                            buf += $"<li>第{i + 1}引数 [{obj.ArgumentList[i]}]: {obj.ArgComments[i]}</li>";
                                        }
                                        buf += "</ul>";
                                    } else {
                                        buf += "引数はありません。";
                                    }
                                    buf += "</div>";

                                    //戻り値
                                    if (obj.ReturnType != "void") {
                                        buf += $"<h3>戻り値</h3><div class=\\\"text\\\">{obj.ReturnType}: {obj.ReturnComment.Replace("。", "。<br>")}</div>";
                                    } else {
                                        buf += $"<h3>戻り値</h3><div class=\\\"text\\\">{obj.ReturnType}: 戻り値はありません。</div>";
                                    }
                                    break;
                            }
                            break;

                        case GenerateCPPDocument.ObjectType.SQEventHandler:
                            buf += "<h3>関数名</h3>";
                            buf += $"<div class=\\\"text\\\">{obj.Name}</div>";
                            buf += "<h3>オブジェクト種別</h3>";
                            buf += "<div class=\\\"text\\\">スクリプト関数定義/イベントハンドラー</div>";

                            //引数リスト
                            buf += "<h3>引数リスト</h3><div class=\\\"text\\\">";
                            if (obj.ArgumentList.Count > 0) {
                                buf += "<ul>";
                                for (var i = 0; i < obj.ArgumentList.Count; i++) {
                                    buf += $"<li>第{i + 1}引数 [{obj.ArgumentList[i]}]: {obj.ArgComments[i]}</li>";
                                }
                                buf += "</ul>";
                            } else {
                                buf += "引数はありません。";
                            }
                            buf += "</div>";

                            //戻り値
                            if (obj.ReturnType != "void") {
                                buf += $"<h3>戻り値</h3><div class=\\\"text\\\">{obj.ReturnType}: {obj.ReturnComment.Replace("。", "。<br>")}</div>";
                            } else {
                                buf += $"<h3>戻り値</h3><div class=\\\"text\\\">{obj.ReturnType}: 戻り値はありません。</div>";
                            }
                            break;
                    }

                    buf += "</div>";
                    //buf += "\"; },";
                    buf += "\";";
                    W.WriteLine(buf);
                }

                //固定部分
                W.WriteLine("	}");
                W.WriteLine("}");
                //W.WriteLine("};");
            }
            #endregion

            Console.WriteLine($"\nHTML用のJavaScriptの生成が完了しました ->\n\t{DestTreeFile}\n\t{DestTreeDocument}\n");
        }
    }
}
