using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace GenerateGlueCode {
    using SQObject = GenerateCPPDocument.SQObject;

    /// <summary>
    /// スクリプトをもとに、リファレンス情報を生成する。
    /// </summary>
    public static class GenerateSQDocument {
        public static List<SQObject> SQConstants;
        public static List<SQObject> SQFunctions;

        //定数用キーワード
        private const string SQ_ENUM = "enum ";
        private const string SQ_CONST = "const ";

        //関数用キーワード
        private const string SQ_FUNC = "function";
        public const string SQ_TAG = "///";
        public const string DOC_PATH = "[node]";
        public const string DOC_SUMMARY = "[summary]";
        public const string DOC_RETURN = "[return]";
        public const string DOC_PARAM = "[par]";
        public const string DOC_COMMENT = "[comment]";

        /// <summary>
        /// 与えられたストリームで定数定義のソーススクリプトを読み込みます。
        /// </summary>
        public static void LoadConstants(TextReader r) {
            SQConstants = new List<SQObject>();

            SQObject obj;
            SQObject tempEnum = null;
            int start, end;
            var line = 0;
            string buf = "", pbuf = "", temp;

            //スクリプトを行単位で読み込む
            while (r.Peek() > -1) {
                line++;
                pbuf = buf;             //前行のバッファ
                buf = r.ReadLine();

                if (tempEnum != null && buf.IndexOf("}") != -1) {
                    //列挙体終端
                    tempEnum = null;
                } else if (buf.IndexOf(SQ_ENUM) != -1) {
                    //列挙体開始
                    obj = CreateSQObjectForConst(GenerateCPPDocument.ObjectSubType.SQEnum);

                    //識別子を抽出
                    temp = buf.Substring(buf.IndexOf(SQ_ENUM) + SQ_ENUM.Length).Trim().Replace("\t", "").Replace(" ", "");
                    end = temp.IndexOf("{");
                    if (end == -1) {
                        //列挙体が開始できない構文エラー
                        Debug.WriteLine($"{line}行目: 列挙体が開始できないためスキップします。");
                        continue;
                    }
                    temp = temp.Substring(0, end);
                    obj.Name = temp;

                    //情報をセットする
                    if (pbuf.IndexOf("//") == 0) {
                        //前行にコメントがあればそれをsummaryとする
                        obj.Summary = pbuf.Substring(2);
                    }
                    SQConstants.Add(obj);
                    tempEnum = obj;
                } else if (tempEnum != null) {
                    //列挙体メンバー
                    obj = CreateSQObjectForConst(GenerateCPPDocument.ObjectSubType.SQEnumMember);
                    obj.ClassPath = tempEnum.Name;

                    //識別子を抽出
                    temp = buf;
                    while (temp.Length > 0 && temp.Substring(0, 1) == "\t") {
                        temp = temp.Substring(1);     //先頭のタブをカット
                    }
                    end = temp.IndexOf(",");
                    if (end == -1) {
                        //区切り記号がない場合はコメント部分まで読み込む
                        end = temp.IndexOf("/");
                        if (end == -1) {
                            //コメントもない場合は末尾まで読み込む
                            end = temp.Length;
                        }
                    }
                    temp = temp.Substring(0, end).Replace("\t", "").Trim();
                    obj.Name = temp;

                    //情報をセットする
                    start = buf.IndexOf("//");
                    if (start != -1) {
                        //同じ行にコメントがあればそれをsummaryとする
                        obj.Summary = buf.Substring(start + 2);
                    }
                    SQConstants.Add(obj);
                } else if (buf.IndexOf(SQ_CONST) != -1) {
                    //定数定義
                    obj = CreateSQObjectForConst(GenerateCPPDocument.ObjectSubType.SQConst);

                    //識別子を抽出
                    temp = buf.Substring(buf.IndexOf(SQ_CONST) + SQ_CONST.Length).Replace("\t", "").Replace(" ", "");
                    end = temp.IndexOf("=");
                    if (end == -1) {
                        //定数が定義できない構文エラー
                        Debug.WriteLine($"{line}行目: 定数定義の構文エラーにつきスキップします。");
                        continue;
                    }
                    obj.Name = temp.Substring(0, end);

                    //情報をノードにセットする
                    start = buf.IndexOf("//");
                    if (start != -1) {
                        //同じ行にコメントがあればそれをsummaryとする
                        obj.Summary = buf.Substring(start + 2);
                    }
                    SQConstants.Add(obj);
                }
            }
        }

        /// <summary>
        /// 与えられたストリームで関数定義のソーススクリプトを読み込みます。
        /// </summary>
        public static void LoadFunctions(TextReader r) {
            SQFunctions = new List<SQObject>();

            var line = 0;
            var obj = CreateSQObjectForFunc();
            while (r.Peek() > -1) {
                line++;
                var buf = r.ReadLine().Trim();

                if (buf.IndexOf(SQ_FUNC) != -1) {
                    //関数定義

                    //識別子を抽出
                    var temp = buf.Substring(buf.IndexOf(SQ_FUNC) + SQ_FUNC.Length);
                    var end = temp.IndexOf("(");
                    if (end == -1) {
                        //構文エラー
                        Debug.WriteLine($"関数定義スクリプト {line}行目: 構文エラーにつき関数を追加できません。");
                        obj = new SQObject();
                        continue;
                    }
                    obj.Name = temp.Substring(0, end).Trim();

                    //引数リスト
                    var start = end + 1;
                    end = temp.IndexOf(")");
                    if (end == -1) {
                        //構文エラー
                        Debug.WriteLine($"関数定義スクリプト {line}行目: 構文エラーにつき関数を追加できません。");
                        obj = new SQObject();
                        continue;
                    }
                    temp = temp.Substring(start, end - start);
                    if (!string.IsNullOrEmpty(temp)) {
                        obj.ArgumentList.AddRange(temp.Split(new char[] { ',' }).ToList());
                    }

                    //引数リストの空白トリミング
                    for (var i = 0; i < obj.ArgumentList.Count; i++) {
                        obj.ArgumentList[i] = obj.ArgumentList[i].Trim();
                    }

                    SQFunctions.Add(obj);
                    obj = CreateSQObjectForFunc();      //次のオブジェクトを先行して生成する
                } else {
                    var text = buf;
                    if (text.IndexOf(SQ_TAG) != -1 && text.IndexOf("]") != -1) {
                        //タグ型である場合は値部分を取り出す
                        text = text.Substring(text.IndexOf("]") + 1);
                    } else {
                        //タグではない行は無視する
                        continue;
                    }

                    if (buf.IndexOf(DOC_PATH) != -1) {
                        //ノード階層指定
                        //NOTE: ファイルパスと同様に [\] で区切るとノードを深めることができる。
                        obj.ClassPath = text;
                    } else if (buf.IndexOf(DOC_SUMMARY) != -1) {
                        //概要
                        obj.Summary = text;
                    } else if (buf.IndexOf(DOC_COMMENT) != -1) {
                        //解説
                        obj.Comment = text;
                    } else if (buf.IndexOf(DOC_RETURN) != -1) {
                        //戻り値
                        obj.ReturnComment = text;
                    } else if (buf.IndexOf(DOC_PARAM) != -1) {
                        //引数
                        obj.ArgComments.Add(text);
                    }
                }
            }
        }

        /// <summary>
        /// 新しいスクリプトオブジェクト情報を生成します。
        /// </summary>
        private static SQObject CreateSQObjectForConst(GenerateCPPDocument.ObjectSubType subType) {
            var tempobj = new SQObject {
                IsFunc = false,
                NodeID = GenerateCPPDocument.ObjectType.Const,
                SubType = subType,
                ReturnType = "不明"
            };
            return tempobj;
        }

        /// <summary>
        /// 新しいスクリプト関数情報を生成します。
        /// </summary>
        private static SQObject CreateSQObjectForFunc() {
            var tempobj = new SQObject {
                IsFunc = true,
                NodeID = GenerateCPPDocument.ObjectType.Function,
                SubType = GenerateCPPDocument.ObjectSubType.SQFunc
            };
            return tempobj;
        }

        /// <summary>
        /// 識別子からsummaryを取得します。
        /// </summary>
        public static string GetSummaryFromConstName(string objName) {
            foreach (var obj in SQConstants) {
                if (obj.Name == objName) {
                    return obj.Summary;
                }
            }
            return "";
        }
    }
}
