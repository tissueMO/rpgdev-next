using System;
using System.Collections.Generic;
using System.IO;

namespace GenerateGlueCode {
    /// <summary>
    /// C++のグルーコードをもとに、エディター用のリファレンスドキュメントを生成する。
    /// </summary>
    public static class GenerateCPPDocument {
        public static List<SQObject> SQObjects;
        public const string destFile = @"..\..\..\Editor\PlainText\SQCodeTree.txt";
        private static string[] srcFiles;

        private const int MetaLineLength = 6;               //読み捨てる冒頭の行数

        private const string CPP_SQEVENT = "SQ_EVENTHANDLER(";

        private const string SQ_VALUE = "Sqrat::RootTable().SetInstance(";
        private const string SQ_FUNC = "Sqrat::RootTable().Func(";
        private const string SQ_CLASS = "Sqrat::RootTable().Bind(";
        private const string SQ_INSTANCEVALUE = ".Var(";
        private const string SQ_INSTANCEFUNC = ".Func(";
        private const string SQ_CLASSVALUE = ".SetStaticValue(";
        private const string SQ_CLASSFUNC = ".StaticFunc(";
        private const string SQ_PROP = ".Prop(";
        private const string SQ_ENUM = ".Enum(";
        private const string SQ_ENUMMEMBER = ".Const(";
        private const string SQ_CONST = "Sqrat::ConstTable().Const(";

        private const string DOC_NODETYPE = "<nodeType>";
        private const string DOC_PARENT = "<parent>";
        private const string DOC_TYPE = "<type>";
        private const string DOC_SUMMARY = "<summary>";
        private const string DOC_RETURN = "<return:$>";
        private const string DOC_PARAM = "<p$1:$2>";
        private const string DOC_COMMENT = "<comment>";
        private const string DOC_OBJECT_END = "----------------------------------------";


        /// <summary>
        /// スクリプトが標準サポートする型リスト
        /// </summary>
        private static readonly string[] DefaultTypes = new string[] {
            "int",
            "bool",
            "double",
            "string",
        };

        /// <summary>
        /// 要素の分類大種別
        /// </summary>
        public enum ObjectType {
            Const,                          //定数・列挙体グループ
            Value,                          //グローバル変数グループ
            Function,                       //グローバル関数グループ
            GameObject,                     //ゲームオブジェクト関連のグループ
            SQEventHandler,                 //スクリプトが処理すべきイベントハンドラー
        }

        /// <summary>
        /// 要素の分類小種別
        /// </summary>
        public enum ObjectSubType {
            Const,
            Enum,
            EnumMember,
            GlobalValue,
            GlobalFunc,
            Class,
            ClassValue,
            ClassFunc,
            MemberValue,
            MemberFunc,
            MemberProperty,
            SQConst,
            SQEnum,
            SQEnumMember,
            SQFunc,
        }

        /// <summary>
        /// オブジェクトの単一データ
        /// </summary>
        public class SQObject {
            public ObjectType NodeID;           //そのオブジェクトの大分類
            public ObjectSubType SubType;       //そのオブジェクトの小分類
            public string ClassPath;            //そのオブジェクトを保有するクラス/名前空間のフルネーム
            public string Name;                 //識別子
            public string SuperClassName;       //親クラスの名前
            public string ReturnType;           //関数の戻り値または変数/定数の型
            public bool IsFunc;                 //関数かどうか
            public List<string> ArgumentList = new List<string>();   //関数のみ: 引数リスト

            public string Summary;
            public string ReturnComment;
            public List<string> ArgComments = new List<string>();
            public string Comment;


            /// <summary>
            /// コンストラクター: 既定
            /// </summary>
            public SQObject() {
            }


            /// <summary>
            /// コンストラクター: 一行文字列からデータを生成する
            /// </summary>
            public SQObject(ObjectSubType subType, bool isFunc, string buf) {
                int idx;
                string temp;
                string[] list, subList;

                //与えられたままに決定するもの
                this.SubType = subType;
                this.IsFunc = isFunc;

                //大種別を決定する
                switch (subType) {
                    case ObjectSubType.Const:
                    case ObjectSubType.Enum:
                    case ObjectSubType.EnumMember:
                        this.NodeID = ObjectType.Const;
                        break;

                    case ObjectSubType.GlobalValue:
                        this.NodeID = ObjectType.Value;
                        break;

                    case ObjectSubType.GlobalFunc:
                        this.NodeID = ObjectType.Function;
                        break;

                    case ObjectSubType.SQFunc:
                        this.NodeID = ObjectType.SQEventHandler;
                        break;

                    default:
                        this.NodeID = ObjectType.GameObject;
                        break;
                }

                //小種別ごとの処理
                switch (subType) {
                    case ObjectSubType.Const:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = this.get2ndArg(buf).Replace("(int)", "").Replace("(float)", "").Replace(".c_str()", "");       //整数・小数・文字列の変換部分を除去
                        if (this.ClassPath.IndexOf("::") == -1) {
                            //グローバル定数の例外措置
                            this.ClassPath = this.Name;
                        }
                        this.ReturnType = this.getCommentArea(buf);
                        break;

                    case ObjectSubType.Enum:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = this.getCommentArea(buf);
                        break;

                    case ObjectSubType.EnumMember:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = this.get2ndArg(buf).Replace("(int)", "");          //整数への変換部分を除去
                        break;

                    case ObjectSubType.GlobalValue:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = this.Name;
                        this.ReturnType = this.getCommentArea(buf);
                        break;

                    case ObjectSubType.GlobalFunc:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = this.Name;

                        //戻り値・引数情報
                        list = this.getCommentArea(buf).Split('|');     //戻り値と引数リストに分ける
                        this.ReturnType = list[0];
                        this.ArgumentList.Clear();
                        this.ArgComments.Clear();
                        if (!string.IsNullOrEmpty(list[1])) {
                            subList = list[1].Split(',');
                            for (var i = 0; i < subList.Length; i++) {
                                this.ArgumentList.Add(subList[i]);
                                this.ArgComments.Add("");
                            }
                        }
                        break;

                    case ObjectSubType.Class:
                        this.Name = this.getDQArea(buf);
                        temp = this.getBracketArea(buf).Replace(", ", ",");
                        this.ClassPath = temp;
                        idx = temp.IndexOf(",");
                        if (idx != -1) {
                            //継承元クラスがある場合は分解する
                            this.ClassPath = temp.Substring(0, idx);
                            this.SuperClassName = temp.Substring(idx + 1);
                        }
                        break;

                    case ObjectSubType.MemberValue:
                    case ObjectSubType.ClassValue:
                        this.Name = this.getDQArea(buf);
                        temp = this.get2ndArg(buf);
                        this.ClassPath = temp.Substring(temp.IndexOf("&") + 1);
                        this.ReturnType = this.getCommentArea(buf);
                        break;

                    case ObjectSubType.MemberProperty:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = this.getPropertyClassPath(buf);
                        this.ReturnType = this.getCommentArea(buf);
                        if (!this.checkPropertySetter(buf)) {
                            //setterを含まない場合は型名に ReadOnly を付加する
                            this.ReturnType += " (読み取り専用)";
                        }
                        break;

                    case ObjectSubType.ClassFunc:
                    case ObjectSubType.MemberFunc:
                        this.Name = this.getDQArea(buf);
                        temp = this.get2ndArg(buf);
                        this.ClassPath = temp.Substring(temp.IndexOf("&") + 1);

                        //戻り値・引数情報
                        list = this.getCommentArea(buf).Split('|');     //戻り値と引数リストに分ける
                        this.ReturnType = list[0];
                        this.ArgumentList.Clear();
                        this.ArgComments.Clear();
                        if (!string.IsNullOrEmpty(list[1])) {
                            subList = list[1].Split(',');
                            for (var i = 0; i < subList.Length; i++) {
                                this.ArgumentList.Add(subList[i]);
                                this.ArgComments.Add("");
                            }
                        }
                        break;

                    case ObjectSubType.SQFunc:
                        this.Name = this.getDQArea(buf);
                        this.ClassPath = "SQEventHandler::" + this.Name;

                        //戻り値・引数情報
                        temp = buf.Substring(buf.LastIndexOf("\",") + 3);
                        temp = temp.Substring(0, temp.IndexOf(");"));
                        temp = temp.Trim();
                        list = temp.Split(',');
                        if (list.Length > 0) {
                            this.ReturnType = list[0];
                            this.ArgumentList.Clear();
                            this.ArgComments.Clear();
                            for (var i = 1; i < list.Length; i++) {
                                this.ArgumentList.Add(list[i].Trim());
                                this.ArgComments.Add("");
                            }
                        }
                        break;
                }
            }


            /// <summary>
            /// 与えられたストリームにこのオブジェクトの情報を書き込む
            /// </summary>
            public void Write(StreamWriter w) {
                w.WriteLine($"[{this.ClassPath}]");
                w.WriteLine($"{DOC_NODETYPE}{this.NodeID.ToString()}/{this.SubType.ToString()}");         //付加情報：オブジェクトの分類詳細

                //派生クラスは親クラスのタグを追加
                if (!string.IsNullOrEmpty(this.SuperClassName)) {
                    w.WriteLine($"{DOC_PARENT}{this.SuperClassName}");
                }

                //変数は型名タグを追加
                if (!string.IsNullOrEmpty(this.ReturnType) && !this.IsFunc) {
                    w.WriteLine($"{DOC_TYPE}{this.ReturnType}");
                }
                w.WriteLine($"{DOC_SUMMARY}{this.Summary}");

                //関数のみの項目
                if (this.IsFunc) {
                    w.WriteLine($"{DOC_RETURN.Replace("$", this.ReturnType)}{this.ReturnComment}");
                    for (var i = 0; i < this.ArgumentList.Count; i++) {
                        w.WriteLine($"{DOC_PARAM.Replace("$1", (i + 1).ToString()).Replace("$2", this.ArgumentList[i])}{this.ArgComments[i]}");
                    }
                }

                //コメント欄のみ複数行を許可する
                w.WriteLine($"{DOC_COMMENT}{this.Comment}");
                w.WriteLine($"{DOC_OBJECT_END}");
            }


            /// <summary>
            /// 一行文字列からコメント部分を抜き出す
            /// </summary>
            private string getCommentArea(string buf) {
                var index = buf.IndexOf("//");
                if (index == -1) {
                    return "";
                } else {
                    return buf.Substring(index + 2);
                }
            }


            /// <summary>
            /// 一行文字列からダブルクォートで囲われた部分を抜き出す
            /// </summary>
            private string getDQArea(string buf) {
                var index = buf.IndexOf("\"");
                if (index == -1) {
                    return "";
                }

                var temp = buf.Substring(index + 1);
                var subindex = temp.IndexOf("\"");
                if (subindex == -1) {
                    return "";
                }

                return temp.Substring(0, subindex);
            }


            /// <summary>
            /// 一行文字列から第２引数を抜き出す
            /// </summary>
            private string get2ndArg(string buf) {
                var temp = "";

                //半角スペースを除去
                buf = buf.Replace(" ", "");

                //コメント部分が末尾にある場合は除去
                if (buf.IndexOf("//") != -1) {
                    buf = buf.Substring(0, buf.IndexOf("//"));
                }

                var index = buf.IndexOf(",");

                //第２引数がない？
                if (index == -1) {
                    return "";
                }

                //第３引数以降がある？
                var regux = new System.Text.RegularExpressions.Regex(@",\(.*?\),");
                var match = regux.Match(buf);
                if (match.Success) {
                    //第２引数は (...) で括られている前提で抜き出す
                    temp = match.Value.Substring(3);
                    temp = temp.Substring(0, temp.Length - 2);
                    return temp;
                }

                temp = buf.Substring(index + 1);
                var subindex = temp.LastIndexOfAny(new char[] { ')' });
                if (subindex == -1) {
                    return "";
                }
                return temp.Substring(0, subindex);
            }


            /// <summary>
            /// 一行文字列から第２引数を抜き出す
            /// </summary>
            private string getPropertyClassPath(string buf) {
                var temp = this.get2ndArg(buf);
                temp = temp.Substring(temp.IndexOf("&") + 1);
                temp = temp.Substring(0, temp.IndexOf("::get"));
                return temp + $"::{this.getDQArea(buf)}";
            }


            /// <summary>
            /// 一行文字列から <...> で囲われた部分を抜き出す
            /// </summary>
            private string getBracketArea(string buf) {
                var index = buf.IndexOf("<");
                if (index == -1) {
                    return "";
                }

                var temp = buf.Substring(index + 1);
                var subindex = temp.IndexOf(">");
                if (subindex == -1) {
                    return "";
                }
                return temp.Substring(0, subindex);
            }


            /// <summary>
            /// 一行文字列にプロパティのsetterを含んでいるかどうかを調べる
            /// </summary>
            private bool checkPropertySetter(string buf) {
                var index = buf.IndexOf("&");
                if (index == -1 || buf.IndexOf("&", index + 1) == -1) {
                    return false;
                }
                return true;
            }
        }


        /// <summary>
        /// エディター用のリファレンスドキュメントを生成します。
        /// </summary>
        public static void Generate() {
            //NOTE: リファレンスドキュメントの生成元ファイルリスト
            srcFiles = new string[] {
                GenerateSQGlueCode.destFile,
                @"..\..\..\GameLib\CScript.cpp",
            };
            Program.CurrentDir = @"..\Editor\PlainText\";
            string buf;
            var data = new List<SQObject>();

            //グルーコード読み込み
            Console.WriteLine("生成したグルーコードを読み込みます...");
            LoadCPPGlueCode(data);

            //イベントハンドラー読み込み
            Console.WriteLine("スクリプトのイベントハンドラーを読み込みます...");
            using (var r = new StreamReader(GenerateSQGlueCode.strConstantFile, Program.SJIS)) {
                while (!r.EndOfStream) {
                    buf = r.ReadLine();
                    if (buf.IndexOf(CPP_SQEVENT) != -1) {
                        //該当箇所を発見
                        data.Add(new SQObject(ObjectSubType.SQFunc, true, buf));
                    }
                }
            }

            //既存のドキュメントを読み込む
            Console.WriteLine("既存のリファレンスドキュメントを読み込みます...");
            using (var r = new StreamReader(destFile, Program.SJIS)) {
                LoadDocumentByStream(r);
            }

            //NOTE: ソートはコマンドツリーに読み込まれてから行われる。
            //取り込んだデータを整理する
            //data.Sort((x, y) => {
            //	//大分類ごとに整理
            //	if(x.NodeID != y.NodeID) {
            //		return (int)x.NodeID - (int)y.NodeID;
            //	}

            //	//ClassPathごとに整理
            //	if(x.ClassPath != y.ClassPath) {
            //		return string.Compare(x.ClassPath, y.ClassPath);
            //	}

            //	//変数・関数ごとに整理
            //	if(x.IsFunc != y.IsFunc) {
            //		return Convert.ToInt32(x.IsFunc) - Convert.ToInt32(y.IsFunc);
            //	}

            //	//最後に名称アルファベット順でソート
            //	return string.Compare(x.Name, y.Name);
            //});

            //既存データと今回読み込んだデータを比較して、変更がないものは既存データのものに置き換える
            CompareAndReplaceData(data);

            //最後にドキュメントとして書き出す
            using (var w = new StreamWriter(destFile, false, Program.SJIS)) {
                foreach (var item in data) {
                    item.Write(w);
                }
            }

            Console.WriteLine($"\nリファレンスドキュメントの生成が完了しました -> {destFile.Substring(Program.CurrentDir.Length)}\n");
        }


        /// <summary>
        /// C++グルーコードからデータを読み込みます。
        /// </summary>
        public static void LoadCPPGlueCode(List<SQObject> data) {
            var buf = "";
            foreach (var srcFile in GenerateCPPDocument.srcFiles) {
                Console.WriteLine(srcFile);
                using (var r = new StreamReader(srcFile, Program.SJIS)) {
                    //不要な部分は読み捨てる
                    for (var i = 0; i < MetaLineLength; i++) {
                        r.ReadLine();
                    }

                    //以降、終端まで読み込む
                    while (!r.EndOfStream) {
                        buf = r.ReadLine().Replace("\t", "").Trim();        //余分な空白はカット

                        if (string.IsNullOrEmpty(buf)) {
                            //空行は無視
                            continue;
                        } else if (buf.IndexOf(SQ_CONST) == 0) {
                            //定数: DEBUG定数が多重に定義されているので、片方を落とす必要がある
                            var NG = false;
                            var newData = new SQObject(ObjectSubType.Const, false, buf);
                            foreach (var item in data) {
                                if (item.Name == newData.Name && item.ClassPath == newData.ClassPath) {
                                    NG = true;
                                    break;
                                }
                            }
                            if (!NG) {
                                data.Add(newData);
                            }
                        } else if (buf.IndexOf(SQ_ENUM) == 0) {
                            //列挙体定義開始
                            data.Add(new SQObject(ObjectSubType.Enum, false, buf));
                        } else if (buf.IndexOf(SQ_ENUMMEMBER) == 0) {
                            //列挙体メンバー
                            data.Add(new SQObject(ObjectSubType.EnumMember, false, buf));
                        } else if (buf.IndexOf(SQ_VALUE) == 0) {
                            //グローバル変数
                            data.Add(new SQObject(ObjectSubType.GlobalValue, false, buf));
                        } else if (buf.IndexOf(SQ_FUNC) == 0) {
                            //グローバル関数
                            data.Add(new SQObject(ObjectSubType.GlobalFunc, true, buf));
                        } else if (buf.IndexOf(SQ_CLASS) == 0) {
                            //クラス定義開始
                            data.Add(new SQObject(ObjectSubType.Class, false, buf));
                        } else if (buf.IndexOf(SQ_CLASSVALUE) == 0) {
                            //クラス変数
                            data.Add(new SQObject(ObjectSubType.ClassValue, false, buf));
                        } else if (buf.IndexOf(SQ_CLASSFUNC) == 0) {
                            //クラスメソッド
                            data.Add(new SQObject(ObjectSubType.ClassFunc, true, buf));
                        } else if (buf.IndexOf(SQ_INSTANCEVALUE) == 0) {
                            //メンバー変数
                            data.Add(new SQObject(ObjectSubType.MemberValue, false, buf));
                        } else if (buf.IndexOf(SQ_INSTANCEFUNC) == 0) {
                            //メンバーメソッド
                            data.Add(new SQObject(ObjectSubType.MemberFunc, true, buf));
                        } else if (buf.IndexOf(SQ_PROP) == 0) {
                            //メンバープロパティ
                            data.Add(new SQObject(ObjectSubType.MemberProperty, false, buf));
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 既存のリファレンスドキュメントを読み込みます。
        /// </summary>
        public static void LoadDocument() {
            using (var r = new StreamReader(destFile, Program.SJIS)) {
                LoadDocumentByStream(r);
            }
        }

        /// <summary>
        /// 既存のリファレンスドキュメントをストリーム経由で読み込みます。
        /// </summary>
        public static void LoadDocumentByStream(TextReader r) {
            SQObjects = new List<SQObject>();

            var obj = new SQObject();
            while (r.Peek() > -1) {
                var buf = r.ReadLine();
                var text = buf;
                if (text.IndexOf(">") != -1) {
                    //タグ型である場合は値部分を取り出す
                    text = text.Substring(text.IndexOf(">") + 1);
                }

                if (buf.IndexOf("[") == 0) {
                    obj.ClassPath = buf.Substring(1, buf.Length - 2);
                    if (obj.ClassPath.IndexOf("::") == -1) {
                        //ルートレベルのオブジェクト
                        obj.Name = obj.ClassPath;
                    } else {
                        //階層のあるオブジェクト
                        obj.Name = obj.ClassPath.Substring(obj.ClassPath.LastIndexOf("::") + 2);
                    }
                } else if (buf.IndexOf(DOC_NODETYPE) != -1) {
                    //nodeType
                    var spl = text.Split(new char[] { '/' });
                    obj.NodeID = (ObjectType) Enum.Parse(typeof(ObjectType), spl[0]);
                    obj.SubType = (ObjectSubType) Enum.Parse(typeof(ObjectSubType), spl[1]);

                    if (obj.NodeID == ObjectType.Const && obj.SubType == ObjectSubType.Const) {
                        //NOTE: 定数の識別子は階層が作れない都合で、例外的にClassPathも含める必要がある。
                        obj.Name = obj.ClassPath.Replace("::", "_").Replace(GenerateSQGlueCode.SQ_EXCEPTNAMESPACE, "");
                    }
                } else if (buf.IndexOf(DOC_PARENT) != -1) {
                    //parent
                    obj.SuperClassName = text;
                } else if (buf.IndexOf(DOC_TYPE) != -1) {
                    //type
                    obj.ReturnType = text;
                } else if (buf.IndexOf(DOC_SUMMARY) != -1) {
                    //summary
                    obj.Summary = text;
                } else if (buf.IndexOf(DOC_RETURN.Substring(0, DOC_RETURN.IndexOf("$"))) != -1) {
                    //return
                    var tag = buf.Substring(buf.IndexOf(":") + 1);
                    tag = tag.Substring(0, tag.IndexOf(">"));
                    obj.IsFunc = true;
                    obj.ReturnType = tag;
                    obj.ReturnComment = text;
                } else if (buf.IndexOf(DOC_PARAM.Substring(0, DOC_PARAM.IndexOf("$"))) != -1) {
                    //par
                    var tag = buf.Substring(buf.IndexOf(":") + 1).Trim();
                    tag = tag.Substring(0, tag.IndexOf(">"));
                    obj.ArgumentList.Add(tag);
                    obj.ArgComments.Add(text);
                } else if (buf.IndexOf(DOC_COMMENT) != -1) {
                    //comment
                    obj.Comment = text;
                } else if (buf.IndexOf(DOC_OBJECT_END) != -1) {
                    //オブジェクト終端
                    SQObjects.Add(obj);
                    obj = new SQObject();
                }
            }
        }


        /// <summary>
        /// 既存のリファレンスドキュメントと最新のデータを比較して、概要とコメントを引き継ぎながら内容を更新します。
        /// </summary>
        public static void CompareAndReplaceData(List<SQObject> data) {
            var edittedObjects = new List<string>();         //変更があったためにコメントが削除されたオブジェクト
            var edittedArgObjects = new List<string>();
            foreach (var item in data) {
                //既存データの中から探し出す
                var OK = false;
                foreach (var loadeddata in SQObjects) {
                    if (item.ClassPath == loadeddata.ClassPath
                    && item.IsFunc == loadeddata.IsFunc
                    && item.NodeID == loadeddata.NodeID
                    && item.SubType == loadeddata.SubType
                    && item.SuperClassName == loadeddata.SuperClassName) {
                        //概要情報が一致していれば、概要とコメントは引き継ぐ
                        item.Summary = loadeddata.Summary;
                        item.Comment = loadeddata.Comment;
                        OK = true;

                        //戻り値を比較する
                        if (item.IsFunc) {
                            if (item.ReturnType == loadeddata.ReturnType) {
                                //戻り値の型が一致していればコメントを引き継ぐ
                                item.ReturnComment = loadeddata.ReturnComment;
                            }
                        }

                        //引数リストを比較する
                        var argOK = true;
                        for (var i = 0; i < item.ArgumentList.Count; i++) {
                            if (i >= loadeddata.ArgumentList.Count) {
                                //既存の関数よりも引数が多くなっている場合
                                argOK = false;
                                break;
                            } else if (item.ArgumentList[i] == loadeddata.ArgumentList[i]) {
                                //この引数は一致している
                                item.ArgComments[i] = loadeddata.ArgComments[i];
                            } else {
                                //この引数は一致していないが続行する
                                argOK = false;
                            }
                        }

                        //引数リストが不一致のときはその旨を最後に示すようにする
                        if (!argOK) {
                            edittedArgObjects.Add(item.ClassPath);
                        }
                    }
                }
                if (!OK) {
                    //新しく追加されたオブジェクトの場合
                    edittedObjects.Add(item.ClassPath);
                }
            }

            if (edittedObjects.Count > 0) {
                Console.WriteLine($"新規に追加されたオブジェクト:");
                foreach (var item in edittedObjects) {
                    Console.WriteLine("\t" + item);
                }
            }

            if (edittedArgObjects.Count > 0) {
                Console.WriteLine($"引数に変更があったオブジェクト:");
                foreach (var item in edittedArgObjects) {
                    Console.WriteLine("\t" + item);
                }
            }
        }
    }
}
