using System;
using System.IO;

namespace GenerateGlueCode {
    /// <summary>
    /// C#の共通定数定義ファイルから、C++用にコードを書き換える。
    /// </summary>
    public static class GenerateCPPConstants {

        //NOTE: パス設定
        public const string srcFile = @"..\..\..\Editor\Module\CommonConstants.cs";
        public const string destFile = @"..\..\..\GameLib\CommonConstants.h";
        private const string CSStringList = "> GetList";
        private const string CSStringListBegin = "var list = ";
        private const string CSStringListEnd = "return list;";
        private const string CSEnum = "public enum ";
        private const string CSConst = "public const ";
        private const string CSNamespace = "public static partial class ";
        private const string CPPEnum = "SQ_ENUM enum class";
        private const string CPPConst = "SQ_CONST const";
        private const string CPPNamespace = "namespace";

        /// <summary>
        /// C++用の定数定義コードを生成します。
        /// </summary>
        public static void Generate() {
            string buf, temp, enumName = "", nsName = "";
            var strList = false;
            int lineCount;              //デバッグ用：現在の行数

            //読み込みつつ書き込む：インデントなし
            Program.CurrentDir = @"..\";
            using (var w = new StreamWriter(destFile, false, Program.SJIS)) {
                w.WriteLine("//###################################################################");
                w.WriteLine("//		エディター・ゲームプログラム間で共通する定数群");
                w.WriteLine("//###################################################################");
                w.WriteLine("");
                w.WriteLine("namespace TS {\r\n");

                nsName = "";
                enumName = "";
                lineCount = 0;
                Console.WriteLine("ファイル読み込み開始 -> " + srcFile.Substring(Program.CurrentDir.Length));
                using (var r = new StreamReader(srcFile, Program.SJIS)) {
                    do {
                        lineCount++;
                        buf = r.ReadLine().Replace("\t", "").Trim();       //タブ文字はすべてカットする

                        //除外条件
                        if (strList && buf.IndexOf(CSStringListBegin) == 0) {
                            //文字列リスト定義開始点：読み捨て
                            continue;
                        } else if (strList && buf.IndexOf(CSStringListEnd) == 0) {
                            //文字列リスト定義終端：読み捨て
                            continue;
                        }

                        if (string.IsNullOrEmpty(enumName) == false && strList && buf.IndexOf("//") != -1) {
                            //文字列リストメンバー定義
                            temp = buf.Substring(buf.IndexOf("//") + 2);
                            w.WriteLine($"\t\t\t{temp},");
                        } else if (string.IsNullOrEmpty(enumName) == false && buf.IndexOf("}") == -1) {
                            //列挙体メンバー定義
                            w.WriteLine($"\t\t\t{buf}");
                        } else if (buf.IndexOf(CSNamespace) == 0) {
                            //C#でのクラス定義は、C++では名前空間定義にする
                            temp = buf.Substring(CSNamespace.Length, (buf.IndexOf(" {")) - CSNamespace.Length);

                            //NOTE: 例外設定
                            switch (temp) {
                                case "Database":
                                    temp = "DB";        //C++側では略称を使う
                                    break;
                                case "Media":
                                    temp = "System";    //C++側では所属を変更する
                                    break;
                            }

                            nsName = temp;
                            w.WriteLine($"\t{CPPNamespace} {temp} {{");
                        } else if (buf.IndexOf(CSEnum) == 0) {
                            //列挙体定義
                            temp = buf.Substring(CSEnum.Length, (buf.IndexOf(" {")) - CSEnum.Length);
                            enumName = temp;
                            strList = false;
                            w.WriteLine($"\t\t{CPPEnum} {temp} {{");
                        } else if (buf.IndexOf(CSConst) == 0) {
                            //定数定義
                            temp = buf.Substring(CSConst.Length);
                            w.WriteLine($"\t\t{CPPConst} {temp}");
                        } else if (buf.IndexOf(CSStringList) != -1) {
                            //C#での文字列リスト定義は、C++では列挙体定義にして、行末のコメント部分を識別子として取り込む
                            temp = buf.Substring(buf.IndexOf(CSStringList) + CSStringList.Length);
                            temp = temp.Substring(0, temp.IndexOf("()"));
                            enumName = temp;
                            strList = true;
                            w.WriteLine($"\t\t{CPPEnum} {temp} {{");
                        } else if (buf.IndexOf("}") == 0) {
                            //ブロック終端
                            if (!string.IsNullOrEmpty(enumName)) {
                                //列挙体終端
                                w.WriteLine("\t\t};\r\n");
                                enumName = "";
                                strList = false;
                            } else if (!string.IsNullOrEmpty(nsName)) {
                                //C#クラス定義/C++名前空間定義終端
                                w.WriteLine("\t}\r\n\r\n");
                                nsName = "";
                            }
                        }
                    } while (!r.EndOfStream);
                }
                w.WriteLine("}");
            }
            Console.WriteLine($"\nC++用の定数定義コードの生成が完了しました -> {destFile.Substring(Program.CurrentDir.Length)}");
        }
    }
}
