using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;

namespace Editor.CtrlComponent.Text {
    using Module;
    using Properties;
    using SQObject = GenerateGlueCode.GenerateCPPDocument.SQObject;

    public partial class ctlCommandTree : UserControl {
        public CtrlComponent.Database.ctlDatabaseEditor dbCtl;
        public CtrlComponent.Map.mgrMapObject mgrMap;

        public delegate void RequestInsertCodeEventHandler(object sender, RequestInsertCodeEventArgs e);
        public event RequestInsertCodeEventHandler RequestInsertCode;               //関連付けられたテキストエディターにコード挿入を要求するイベント
        public static List<string> CodeContainers = new List<string>();             //コンテナー系のオブジェクト
        public static List<string> CodeMembers = new List<string>();                //メンバー系のオブジェクト
        public static List<TreeNode> AutoCompleteSource = new List<TreeNode>();     //入力補完のツリー情報を探すのに使うリスト
        public static AutoCompleteStringCollection AutoCompleteSourceAll = new AutoCompleteStringCollection();      //入力補完のサジェスト候補に使われる全リスト

        public const string ClassStaticMemberHeader = "[S]";        //静的メンバー表示の接頭辞

        //組み込みオブジェクトのサブノード名
        public const string NodeNameCppObject = "cppobject";
        public const string NodeNameCppObjValue = "cppvalue";
        public const string NodeNameCppObjFunc = "cppfunc";
        public const string NodeNameCppObjClass = "cppclass";
        public const string NodeNameCppObjConst = "cppconst";
        public const string NodeNameCppObjEnum = "cppenum";
        public const string NodeNameCppObjSQEventHandler = "sqeventhandler";

        //ルートノードの表示名
        public const string NodeSQStatement = "スクリプト標準命令";
        public const string NodeSQConst = "スクリプト共通定数";
        public const string NodeSQValue = "スクリプト共通変数";
        public const string NodeSQFunc = "スクリプト共通関数";
        public const string NodeCPPObj = "組み込みオブジェクト";
        public const string NodeTool = "ツール";
        public const string NodeEasyTool = "Easyエディター";

        //ツールノードのタグ
        public const string TagToolMessage = "msg";
        public const string TagToolFileName = "filename";
        public const string TagToolEVID = "evid";
        public const string TagToolDBID = "dbid";
        public const string TagToolDBColumn = "dbcolumn";
        public const string TagToolChangeMap = "map";
        public const string TagToolColor = "color";
        public const string TagToolPathNormalEV = "pathnormalmoveev";
        public const string TagToolPathExtraEV = "pathextramoveev";
        public const string TagToolPathExtraPlayer = "pathextramoveplayer";
        public const string TagToolSQTag = "scripttag";

        //スクリプトタグのノードタグ
        public const string TagSQTagSummary = "summary";
        public const string TagSQTagNode = "node";
        public const string TagSQTagParameter = "par";
        public const string TagSQTagReturn = "return";
        public const string TagSQTagComment = "comment";

        //Easyツールノードのタグ
        public const string TagEasyComment = "comment";
        public const string TagEasyEmptyLine = "emptyline";
        public const string TagEasyRawCode = "rawcode";

        /// <summary>
        /// 組み込みオブジェクトのアイコンインデックス
        /// </summary>
        public enum CPPObjectIconIndices {
            None,
            CPPConst,
            CPPEnum,
            CPPEnumMember,
            CPPClass,
            CPPValue,
            CPPProperty,
            CPPFunc,
            SQEventHandler,
        }

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlCommandTree() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //リソースから読み込んでコマンドツリーを生成する
            //ただし、定数定義スクリプトのノードはプロジェクトが読み込まれた後に手動で生成する
            CodeContainers.Clear();
            CodeMembers.Clear();
            this.trvTree.BeginUpdate();

            foreach (TreeNode node in this.trvTree.Nodes) {
                switch (node.Text) {
                    case NodeSQStatement:
                        SQ.SetSQStatementsTree(node);
                        break;

                    case NodeCPPObj:
                        SQ.SetCPPObjectsTree(node);
                        node.Expand();

                        //ノードをコンテナー系とメンバー系に振り分ける・重複を許可しない
                        var subNodes = Common.GetNodeListToFlat(node);
                        foreach (var subNode in subNodes) {
                            var obj = subNode.Tag as SQObject;

                            if (subNode.Tag != null && obj == null) {
                                //文字列で格納されているものはコンテナーに分類する
                                CodeContainers.Add(subNode.Tag.ToString());
                                continue;
                            } else if (obj == null) {
                                //対象外のノード
                                continue;
                            }

                            switch (obj.SubType) {
                                case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Class:
                                case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Enum:
                                case GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberProperty:
                                    //コンテナー系
                                    if (!CodeContainers.Contains(obj.Name)) {
                                        CodeContainers.Add(obj.Name);
                                    }
                                    break;

                                default:
                                    //メンバー系
                                    if (!CodeMembers.Contains(obj.Name)) {
                                        CodeMembers.Add(obj.Name);
                                    }
                                    break;
                            }
                        }
                        CodeContainers.Sort();
                        CodeMembers.Sort();
                        break;

                    case NodeTool:
                        //ツール群はよく使うので展開しておく
                        node.Expand();
                        break;
                }
            }

            //「組み込みオブジェクト：変数」は最もよく使うので展開しておく
            this.trvTree.Nodes[NodeNameCppObject].Nodes[NodeNameCppObjValue].Expand();

            this.trvTree.EndUpdate();
        }

        /// <summary>
        /// コード挿入を要求するイベントの引数
        /// </summary>
        public class RequestInsertCodeEventArgs : EventArgs {
            public string Code;
            public bool ForbiddenCRLF;
            public RequestInsertCodeEventArgs(string code, bool forbiddenCRLF) {
                this.Code = code;
                this.ForbiddenCRLF = forbiddenCRLF;
            }
        }

        /// <summary>
        /// コマンドツリーのソート方法
        /// </summary>
        private class CommandTreeSorter : System.Collections.IComparer {
            public int Compare(object x, object y) {
                var tx = x as TreeNode;
                var ty = y as TreeNode;

                //トップレベルのノードは並び替えない
                if (tx.Parent == null) {
                    return 1;
                }

                //アイコンインデックスで比較する
                if (tx.ImageIndex != ty.ImageIndex) {
                    return tx.ImageIndex - ty.ImageIndex;
                }

                //テキストで比較する
                return string.Compare(tx.Text, ty.Text);
            }
        }

        /// <summary>
        /// Easyエディターツール群を有効にするかどうか
        /// </summary>
        public bool EnabledEasyEditorTools {
            get; set;
        }

        /// <summary>
        /// コマンドツリーをセットアップします。
        /// これはプロジェクトが読み込まれた後に呼び出して下さい。
        /// </summary>
        public void Setup(CtrlComponent.Database.ctlDatabaseEditor dbCtl, CtrlComponent.Map.mgrMapObject mgrMap) {
            this.dbCtl = dbCtl;
            this.mgrMap = mgrMap;
            var subNodes = new List<TreeNode>();

            //オートコンプリートのソースを更新

            //組み込みオブジェクトは 定数・列挙体・変数 ノード以下に絞る
            AutoCompleteSource.Clear();
            subNodes.AddRange(Common.GetNodeListToFlat(this.trvTree.Nodes[NodeNameCppObject].Nodes[NodeNameCppObjConst]));
            subNodes.AddRange(Common.GetNodeListToFlat(this.trvTree.Nodes[NodeNameCppObject].Nodes[NodeNameCppObjEnum]));
            subNodes.AddRange(Common.GetNodeListToFlat(this.trvTree.Nodes[NodeNameCppObject].Nodes[NodeNameCppObjValue]));
            AutoCompleteSource.AddRange(subNodes);
            foreach (var node1 in subNodes) {
                AutoCompleteSourceAll.Add(ctlCommandTree.GetCode(node1));
            }

            //プロジェクト関連のスクリプトからノード生成する
            this.trvTree.BeginUpdate();
            foreach (TreeNode node in this.trvTree.Nodes) {
                switch (node.Text) {
                    case NodeSQConst:
                        //定数定義スクリプトを読み込む
                        SQ.SetSQConstantsTree(node, Resources.Path_Scripts_Constants);
                        break;

                    case NodeSQFunc:
                        //関数定義スクリプトを読み込む
                        SQ.SetSQFunctionsTree(node, Resources.Path_Scripts_Functions);
                        break;
                }

                //オートコンプリートのソースに追加
                subNodes = Common.GetNodeListToFlat(node);
                AutoCompleteSource.AddRange(subNodes);
                foreach (var node2 in subNodes) {
                    AutoCompleteSourceAll.Add(ctlCommandTree.GetCode(node2));
                }

            }

            //コマンドツリーの並び替え実行
            this.trvTree.TreeViewNodeSorter = new CommandTreeSorter();
            this.trvTree.EndUpdate();

            ////オートコンプリートのノード情報リストの並び替え実行
            //AutoCompleteSource.Sort((x, y) => {
            //	//コードの文字列逆順にする
            //	var xcode = GetCode(x);
            //	var ycode = GetCode(y);
            //	return -string.Compare(xcode, ycode);
            //});
        }

        /// <summary>
        /// スクリプトがソースになっているノードを再読込してコマンドツリーを再セットアップします。
        /// </summary>
        public void ReloadTree() {
            this.Setup(this.dbCtl, this.mgrMap);
        }

        /// <summary>
        /// ノードに対応するコードを取得します。
        /// </summary>
        public static string GetCode(TreeNode node) {
            if (node.Tag == null) {
                //挿入に対応していないノード
                return "";
            } else if ((node.Tag as SQObject) == null) {
                //直接入力方式のタグ
                return node.Tag?.ToString();
            } else {
                //詳細情報を持つコマンド
                return (node.Tag as SQObject).Name;
            }
        }

        /// <summary>
        /// ノードに対応する概要を取得します。
        /// </summary>
        public static string GetCodeSummary(TreeNode node) {
            if (node.Tag == null) {
                //挿入に対応していないノード
                return "";
            } else if ((node.Tag as SQObject) == null) {
                //直接入力方式のタグ
                return node.Text;
            } else {
                //詳細情報を持つコマンド
                return (node.Tag as SQObject).Summary;
            }
        }

        /// <summary>
        /// ノードに対応するコメントを取得します。
        /// </summary>
        public static string GetCodeComment(TreeNode node) {
            if (node.Tag == null) {
                //挿入に対応していないノード
                return "";
            } else if ((node.Tag as SQObject) == null) {
                //直接入力方式のタグ
                return node.ToolTipText;
            } else {
                //詳細情報を持つコマンド
                return (node.Tag as SQObject).Comment;
            }
        }

        /// <summary>
        /// コードに一致するノードを取得します。
        /// </summary>
        public static List<TreeNode> FindNodes(string code) {
            var idx = code.IndexOf(" ");
            if (idx != -1) {
                //空白部分の手前で切る
                code = code.Substring(0, idx);
            }

            var resultList = new Dictionary<string, TreeNode>();
            foreach (var node in AutoCompleteSource) {
                if (!resultList.ContainsKey(node.FullPath) && GetCode(node) == code) {
                    resultList.Add(node.FullPath, node);
                }
            }
            return resultList.Values.ToList();
        }

        /// <summary>
        /// 任意のタイミングでツリーを再読込する
        /// </summary>
        private void rmnReloadSQNodes_Click(object sender, EventArgs e) {
            this.ReloadTree();
        }

        /// <summary>
        /// ノードを選択すると 展開/折り畳み
        /// </summary>
        private void trvTree_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e) {
            if (e.Node != null && e.Button == MouseButtons.Left) {
                if (e.Node.IsExpanded) {
                    e.Node.Collapse();
                } else {
                    e.Node.Expand();
                }
            }
        }

        /// <summary>
        /// ノードをダブルクリックすると挿入
        /// </summary>
        private void trvTree_MouseDoubleClick(object sender, MouseEventArgs e) {
            if (this.trvTree.SelectedNode == null) {
                return;
            }

            //選択ノードのコードを挿入する
            var code = this.GetNodeCode(this.trvTree.SelectedNode, out var forbiddenCRLF);
            if (!string.IsNullOrEmpty(code)) {
                this.RequestInsertCode?.Invoke(this, new RequestInsertCodeEventArgs(code, forbiddenCRLF));
            }
        }

        /// <summary>
        /// ノードから挿入コードを返します。
        /// </summary>
        public string GetNodeCode(TreeNode node, out bool forbiddenCRLF) {
            var code = "";
            var nodeTag = node.Tag?.ToString();
            var nodes = node.FullPath.Split(new char[] { '\\' });
            forbiddenCRLF = false;

            //ルートノードで判別
            switch (nodes[0]) {
                case NodeSQValue:
                    //スクリプト共通変数
                    var DlgDB1 = new Dialog.Text.Script.dlgInsertDB(this.dbCtl.DBList, false, true);
                    if (DlgDB1.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    code = DlgDB1.Result;
                    break;

                case NodeTool:
                    //各種ツール
                    code = this.getCodeTool(node);
                    forbiddenCRLF = true;
                    break;

                case NodeEasyTool:
                    //イベントコマンド簡易編集ツール
                    if (!this.EnabledEasyEditorTools) {
                        MessageBox.Show("Easyエディターはイベント編集時のみ有効です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        break;
                    }
                    code = this.getCodeEasyTool(node);
                    break;

                default:
                    //一般的なコード挿入
                    code = this.getCodeObject(node);
                    break;
            }
            return code;
        }

        /// <summary>
        /// Easyエディターツールによって挿入コードを生成します。
        /// </summary>
        private string getCodeEasyTool(TreeNode node) {
            var nodeTag = node.Tag?.ToString();
            switch (nodeTag) {
                case TagEasyComment:
                    //コメント行
                    var DlgComment = new Dialog.Common.dlgInputTextValue("コメント文を入力して下さい。", "", null);
                    if (DlgComment.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    return "//" + DlgComment.ResultText;

                case TagEasyEmptyLine:
                    //空行
                    return "\t";        //空白だと挿入されないので意図的にTabを一文字分入れる

                case TagEasyRawCode:
                    //生コード行
                    var Dlg = new Dialog.Text.Script.dlgInsertCommand(
                        this,
                        "直接編集モード：挿入",
                        "入力欄はスクリプト表記になっています。\r\nタイプミスには十分注意して下さい。",
                        "", ""
                    );
                    if (Dlg.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    return Dlg.Result;
            }
            return "";
        }

        /// <summary>
        /// ツールによって挿入コードを生成します。
        /// </summary>
        private string getCodeTool(TreeNode node) {
            var nodeTag = node.Tag?.ToString();
            switch (nodeTag) {
                case TagToolMessage:
                    var DlgMsg = new Dialog.Text.Script.dlgEVcmdMessage(this.dbCtl.DBList);
                    if (DlgMsg.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    return DlgMsg.Result;

                case TagToolFileName:
                    var DlgFN = new Dialog.Common.dlgFileSelect(null, "", true, true, false);
                    if (DlgFN.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    //両端のダブルクォートとパス区切り記号のエスケープシーケンス化
                    return "\"" + DlgFN.FileName.Replace("\\", "\\\\") + "\"";

                case TagToolEVID:
                    if (this.ParentForm is Dialog.Map.dlgEVSettings) {
                        //イベント編集画面のときに限る
                        if (this.mgrMap.MapData == null) {
                            //編集中のマップが存在しない
                            return "";
                        }

                        var DlgEVID = new Dialog.Common.dlgSelectInList("イベントIDの挿入");
                        for (var i = 0; i < this.mgrMap.MapData.EVCount; i++) {
                            DlgEVID.AddItem(this.mgrMap.MapData[i].VisibleID, this.mgrMap.MapData[i].Name);
                        }

                        if (DlgEVID.ShowDialog() != DialogResult.OK) {
                            //ダイアログ中断
                            return "";
                        }

                        if (!Settings.Default.Script_FixedIDHexMode) {
                            //10進数のID表記
                            return this.mgrMap.MapData[DlgEVID.GetResultIndex()].FixedID.ToString() + "/*" + DlgEVID.GetResultTitle() + "*/";
                        } else {
                            //16進数のID表記
                            return "0x" + this.mgrMap.MapData[DlgEVID.GetResultIndex()].FixedID.ToString("X") + "/*" + DlgEVID.GetResultTitle() + "*/";
                        }
                    } else {
                        MessageBox.Show("イベントIDの挿入はマップエディター上のイベント編集画面でのみ有効です。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return "";
                    }

                case TagToolDBID:
                case TagToolDBColumn:
                    var DlgDB2 = new Dialog.Text.Script.dlgInsertDB(this.dbCtl.DBList, (nodeTag == TagToolDBColumn) ? true : false, false);
                    if (DlgDB2.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    return DlgDB2.Result;

                case TagToolChangeMap:
                    var DlgMap = new Dialog.Text.Script.dlgEVcmdChangeMap(this.dbCtl.DBList);
                    if (DlgMap.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    return DlgMap.Result;

                case TagToolColor:
                    var DlgCLR = new ColorDialog {
                        AllowFullOpen = true,
                        FullOpen = true
                    };
                    if (DlgCLR.ShowDialog() != DialogResult.OK) {
                        return "";
                    }

                    //RGBの順で３つの引数を作り、カラーコードを取得する関数をセットにする
                    return Resources.SQ_GetColor.Replace("$", $"{DlgCLR.Color.R}, {DlgCLR.Color.G}, {DlgCLR.Color.B}");

                case TagToolPathNormalEV:
                case TagToolPathExtraEV:
                case TagToolPathExtraPlayer:
                    var DlgPath = new Dialog.Text.Script.dlgEVcmdMoveTask(
                        nodeTag != TagToolPathNormalEV,
                        nodeTag == TagToolPathExtraPlayer
                    );
                    if (DlgPath.ShowDialog() != DialogResult.OK) {
                        return "";
                    }
                    return DlgPath.Result;

                case TagSQTagSummary:
                    return GenerateGlueCode.GenerateSQDocument.SQ_TAG + GenerateGlueCode.GenerateSQDocument.DOC_SUMMARY + "\r\n";
                case TagSQTagNode:
                    return GenerateGlueCode.GenerateSQDocument.SQ_TAG + GenerateGlueCode.GenerateSQDocument.DOC_PATH + "\r\n";
                case TagSQTagParameter:
                    return GenerateGlueCode.GenerateSQDocument.SQ_TAG + GenerateGlueCode.GenerateSQDocument.DOC_PARAM + "\r\n";
                case TagSQTagReturn:
                    return GenerateGlueCode.GenerateSQDocument.SQ_TAG + GenerateGlueCode.GenerateSQDocument.DOC_RETURN + "\r\n";
                case TagSQTagComment:
                    return GenerateGlueCode.GenerateSQDocument.SQ_TAG + GenerateGlueCode.GenerateSQDocument.DOC_COMMENT + "\r\n";
            }
            return "";
        }

        /// <summary>
        /// 一般的な方法によって挿入コードを生成します。
        /// </summary>
        private string getCodeObject(TreeNode node) {
            if (node.Tag == null) {
                //挿入に対応していないノード
                return "";
            } else if ((node.Tag as SQObject) == null) {
                //直接入力方式のタグ
                return node.Tag?.ToString();
            }


            //それ以外は、詳細情報を持つコマンド
            var obj = node.Tag as SQObject;
            var head = "";

            //Shiftキーが押されている場合: そのまま挿入
            if ((Form.ModifierKeys & Keys.Shift) == Keys.Shift) {
                return obj.Name;
            }

            //グローバル変数の中のノード: 起点の変数からのフルパスにする
            if (node.FullPath.IndexOf(this.trvTree.Nodes[NodeNameCppObject].Nodes[NodeNameCppObjValue].FullPath) == 0) {
                head = "";
                var subnode = node.Parent;
                while ((subnode.Tag as SQObject) != null) {
                    var subobj = (subnode.Tag as SQObject);
                    head = subobj.Name + "." + head;
                    subnode = subnode.Parent;
                }
            }

            //イベントハンドラーとして分類されたスクリプトの関数: 定義形式で挿入する
            if (obj.NodeID == GenerateGlueCode.GenerateCPPDocument.ObjectType.SQEventHandler) {
                var buf = Resources.SQ_Function + obj.Name + "(";
                for (var i = 0; i < obj.ArgumentList.Count; i++) {
                    buf += obj.ArgumentList[i];
                    if (i < obj.ArgumentList.Count - 1) {
                        buf += ", ";
                    }
                }
                buf += ") {}";
                return buf;
            }


            if (obj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.SQEnum
            || obj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.Enum) {
                //列挙体: 末尾に [.] を付加して挿入する
                return head + obj.Name + ".";

            } else if (obj.SubType == GenerateGlueCode.GenerateCPPDocument.ObjectSubType.MemberProperty
            && string.IsNullOrEmpty(obj.ReturnType) == false && obj.ReturnType.Substring(0, 1) == "C") {
                //組み込み型でないプロパティ: 末尾に [.] を付加して挿入する
                return head + obj.Name + ".";

            } else if (!obj.IsFunc) {
                //それ以外の関数でないオブジェクトはそのまま挿入
                return head + obj.Name;

            } else if ((string.IsNullOrEmpty(obj.ReturnType) || obj.ReturnType == "void")
            && obj.ArgumentList.Count == 0) {
                //戻り値も引数もない関数: セミコロンを付けて直接挿入
                return head + obj.Name + "();";

            } else {
                //それ以外はダイアログを挟む
                var DlgInsert = new Dialog.Text.Script.dlgInsertCommand(this, obj);
                if (DlgInsert.ShowDialog() != DialogResult.OK) {
                    return "";
                }

                //戻り値がない場合はセミコロンを付けて挿入する
                if (string.IsNullOrEmpty(obj.ReturnType) || obj.ReturnType == "void") {
                    return head + DlgInsert.Result + ";";
                } else {
                    return head + DlgInsert.Result;
                }
            }
        }
    }
}
