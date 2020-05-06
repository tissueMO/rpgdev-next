using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace GenerateGlueCode {
    /// <summary>
    /// C++のヘッダーをもとに、スクリプトに公開するためのグルーコードを生成する。
    /// </summary>
    public static class GenerateSQGlueCode {
        private const string SQ_NOBIND_BEGIN = "SQ_NOBIND_BEGIN";
        private const string SQ_NOBIND_END = "SQ_NOBIND_END";
        private const string SQ_NOBIND = "SQ_NOBIND";
        private const string SQ_BEGINMEMBER = "SQ_BEGINMEMBER";
        private const string SQ_CLASS = "class ";
        private const string SQ_SUPERCLASS = ": public ";
        private const string SQ_ABSTRACTCLASS = "SQ_ABSTRACT class ";
        private const string SQ_CLASSVALUE = "SQ_CLASSVALUE";
        private const string SQ_CLASSFUNC = "SQ_CLASSFUNC";
        private const string SQ_INSTANCEVALUE = "SQ_INSTANCEVALUE";
        private const string SQ_INSTANCEFUNC = "SQ_INSTANCEFUNC";
        private const string SQ_CONST = "SQ_CONST";
        private const string SQ_ENUM = "SQ_ENUM enum class ";
        private const string SQ_PROPGET = "SQ_PROPGET";
        private const string SQ_PROPSET = "SQ_PROPSET";
        private const string SQ_NAMESPACE = "namespace ";

        public const string SQ_EXCEPTNAMESPACE = "TS_";         //NOTE: TS名前空間は定数名に含めない。

        //NOTE: パス設定
        public const string destFile = @"..\..\..\GameLib\SQGlue.cpp";
        public const string strConstantFile = @"..\..\..\GameLib\ConstantStrings.h";

        /// <summary>
        /// グルーコードを生成します。
        /// </summary>
        public static void Generate() {
            //NOTE: スクリプトに教える材料が含まれるヘッダーファイルを集める
            Program.CurrentDir = @"..\..\..\GameLib\";
            var preSrcFiles = Directory.GetFiles(Program.CurrentDir, "*.h", SearchOption.AllDirectories);
            var srcFiles = new List<string>();
            string[] exceptFiles = {
				//検索対象外にするファイルのフルパス
				@"GameLib\resource.h",
                @"GameLib\DataType.h",
                @"GameLib\DXArchive.h",
                @"GameLib\DXArchiveVer5.h",
                @"GameLib\FileLib.h",
                @"GameLib\GuruGuruSMF4_Cpp.h",
            };
            string[] castFuncs = {
				//キャストが必要なオーバーロードした関数
				"CGameObject::PrintLog",
                "CDXLibWrapper::SetDrawArea",
                "CDatabase::CDBBase::Load",
                "CDatabase::CDBParty::Load",
                "CDatabase::CDBValue::Load",
                "CDatabase::CDBUser::Load",
                "CMaterial::CMaterialGraphics::Add",
                "CMaterial::CMaterialSound::Add",
                "CMaterial::CMaterialFont::Add",
                "CMultiThread::AddThread",
                "CRectangle::SetByCorner",
                "CRectangle::PointContains",
                "CPoint::Set",
                "CSize::Set",
                "CRectangle::Set",
                "CRGBColor::Set",
                "CSoundData::Load",
                "CSceneMap::CMapData::RunEV",
            };

            var superClassNames = new Stack<string>();
            var abstractClasses = new Stack<bool>();
            string buf, temp, className = "", enumName = "";
            var nowPath = "";        //現在見ているクラスの位置
            int lineCount;              //デバッグ用：現在の行数
            var isNoBindArea = false;
            var privateArea = false;

            //NOTE: 処理するヘッダーファイルの順序を調整する
            foreach (var file in preSrcFiles) {
                //後回しにする必要があるファイルだけ除外する
                switch (Path.GetFileName(file)) {
                    case "CGameObject.h":
                    case "General.h":
                    case "Header.h":
                        break;

                    default:
                        srcFiles.Add(file);
                        break;
                }
            }
            srcFiles.Insert(0, Program.CurrentDir + "General.h");
            srcFiles.Add(Program.CurrentDir + "CGameObject.h");
            srcFiles.Add(Program.CurrentDir + "Header.h");

            //読み込みつつ書き込む：インデントなし
            Console.WriteLine("対象ファイル群の読み込みを開始します...");

            using (var w = new StreamWriter(destFile, false, Program.SJIS)) {
                w.WriteLine("//###################################################################");
                w.WriteLine("//			グルーコード");
                w.WriteLine("//###################################################################");
                w.WriteLine("#include \"Header.h\"");
                w.WriteLine("");
                w.WriteLine("void CScript::SQGlue() {");

                foreach (var src in srcFiles) {
                    className = "";
                    enumName = "";
                    nowPath = "";

                    var isExcepted = false;
                    foreach (var exceptFile in exceptFiles) {
                        if (src.Contains(exceptFile)) {
                            Console.WriteLine("除外ファイル -> " + src.Substring(Program.CurrentDir.Length));
                            isExcepted = true;
                            break;
                        }
                    }
                    if (isExcepted) {
                        //除外ファイルのためスキップ
                        continue;
                    }

                    //Console.WriteLine("ファイル読み込み開始 -> " + src.Substring(Program.CurrentDir.Length));
                    lineCount = 0;
                    using (var r = new StreamReader(src, Program.SJIS)) {
                        do {
                            lineCount++;
                            buf = r.ReadLine().Replace("\t", "").Trim();       //タブ文字はすべてカットする

                            //無視区間である場合は抜ける
                            if (buf.IndexOf(SQ_NOBIND_BEGIN) != -1) {
                                isNoBindArea = true;
                            } else if (buf.IndexOf(SQ_NOBIND_END) != -1) {
                                isNoBindArea = false;
                            } else if (buf.Length >= 2 && buf.Substring(0, 2) == "//" && buf.IndexOf(SQ_BEGINMEMBER) == -1) {
                                continue;       //コメント行・かつSQマクロが含まれない
                            } else if (buf.Length >= 2 && buf.Substring(0, 2) == "/*") {
                                isNoBindArea = true;        //コメント範囲・始まり
                            } else if (buf.IndexOf("*/") != -1) {
                                isNoBindArea = false;       //コメント範囲・終わり
                            } else if (buf.Length == 0) {
                                continue;       //空行
                            } else if (buf.IndexOf(SQ_NOBIND) != -1) {
                                continue;       //バインド対象外の行
                            } else if (buf.IndexOf("#") == 0) {
                                continue;       //プリプロセッサ行
                            }
                            if (isNoBindArea) {
                                continue;
                            }

                            //種別に応じた処理
                            if (buf.IndexOf(SQ_NAMESPACE) == 0) {
                                //名前空間開始
                                //NOTE: 名前空間は紐づけられない

                                var tempNamespace = buf.Substring(SQ_NAMESPACE.Length, (buf.IndexOf(" {")) - SQ_NAMESPACE.Length);
                                GenerateSQGlueCode.IntoBlock(ref nowPath, tempNamespace);

                            } else if (buf.IndexOf(SQ_ENUM) != -1) {
                                //列挙体定義開始

                                enumName = buf.Substring(SQ_ENUM.Length, (buf.IndexOf(" {")) - SQ_ENUM.Length);
                                GenerateSQGlueCode.IntoBlock(ref nowPath, enumName);
                                w.WriteLine("Sqrat::ConstTable()");
                                w.WriteLine(".Enum(\"" + enumName + "\", Sqrat::Enumeration()\t\t\t//" + nowPath);

                            } else if (buf.IndexOf(SQ_CLASS) != -1) {
                                //クラス定義開始

                                className = GenerateSQGlueCode.GetClassName(buf);

                                //継承元クラスがあるか？
                                if (buf.IndexOf(":") != -1) {
                                    superClassNames.Push(GenerateSQGlueCode.GetSuperClassName(buf));
                                } else {
                                    superClassNames.Push("");
                                }

                                //抽象クラスか？
                                if (buf.IndexOf(SQ_ABSTRACTCLASS) != -1) {
                                    abstractClasses.Push(true);
                                } else {
                                    abstractClasses.Push(false);
                                }

                                GenerateSQGlueCode.IntoBlock(ref nowPath, className);
                                //NOTE: SQ_BEGINMEMBER が出てくるまで定義を開始しない

                            } else if (buf.IndexOf(SQ_CONST) != -1) {
                                //定数定義

                                temp = GenerateSQGlueCode.GetValueName(buf);
                                if (GenerateSQGlueCode.GetValueType(buf) == "string") {
                                    //文字列型の定数はchar型の配列に変換する必要がある
                                    w.WriteLine("Sqrat::ConstTable().Const(\"" + nowPath.Replace("::", "_").Replace(SQ_EXCEPTNAMESPACE, "") + "_" + temp + "\", " + nowPath + "::" + temp + ".c_str());\t\t\t//" + GenerateSQGlueCode.GetValueType(buf) + "\r\n");
                                } else {
                                    w.WriteLine("Sqrat::ConstTable().Const(\"" + nowPath.Replace("::", "_").Replace(SQ_EXCEPTNAMESPACE, "") + "_" + temp + "\", " + nowPath + "::" + temp + ");\t\t\t//" + GenerateSQGlueCode.GetValueType(buf) + "\r\n");
                                }

                            } else if (string.IsNullOrEmpty(nowPath) && buf.IndexOf(SQ_INSTANCEVALUE) != -1) {
                                //グローバル変数を紐づける

                                temp = GenerateSQGlueCode.GetValueName(buf);
                                w.WriteLine("Sqrat::RootTable().SetInstance(\"" + temp + "\", &" + temp + ");\t\t\t//" + GenerateSQGlueCode.GetValueType(buf));

                            } else if (!string.IsNullOrEmpty(enumName)) {
                                //列挙体定義中のみ

                                if (buf.IndexOf("}") == 0) {
                                    //列挙体を閉じる
                                    GenerateSQGlueCode.OutBlock(ref nowPath);
                                    w.WriteLine(");");
                                    enumName = "";
                                    w.WriteLine("");
                                } else {
                                    // [,]or[ =]の部分で切る
                                    temp = buf;
                                    var equal = buf.IndexOf(" =");
                                    var comma = buf.IndexOf(",");
                                    if (equal != -1) {
                                        temp = temp.Substring(0, equal);
                                    } else if (comma != -1) {
                                        temp = temp.Substring(0, comma);
                                    } else {
                                        continue;       //構文エラー
                                    }
                                    w.WriteLine(".Const(\"" + temp + "\", (int)" + nowPath + "::" + temp + ")");
                                }

                            } else if (!string.IsNullOrEmpty(className)) {
                                //クラス定義中のみ

                                if (buf.IndexOf("};") == 0) {
                                    //クラスを閉じる
                                    GenerateSQGlueCode.OutBlock(ref nowPath);
                                    w.WriteLine(");");
                                    className = GenerateSQGlueCode.GetNowClassName(nowPath);
                                    privateArea = false;
                                    w.WriteLine("");
                                } else if (privateArea) {
                                    //クラスが閉じられるまでスルー
                                    continue;
                                } else if (buf.IndexOf(SQ_BEGINMEMBER) != -1) {
                                    //クラス定義を開始する
                                    var superClassName = superClassNames.Pop();
                                    if (string.IsNullOrEmpty(superClassName)) {
                                        //単独クラス
                                        if (!abstractClasses.Pop()) {
                                            w.WriteLine("Sqrat::RootTable().Bind(\"" + className + "\", Sqrat::Class<" + nowPath + ">()");
                                        } else {
                                            w.WriteLine("Sqrat::RootTable().Bind(\"" + className + "\", Sqrat::Class<" + nowPath + ", Sqrat::NoConstructor>()");
                                        }
                                    } else {
                                        //派生クラス
                                        if (!abstractClasses.Pop()) {
                                            w.WriteLine("Sqrat::RootTable().Bind(\"" + className + "\", Sqrat::DerivedClass<" + nowPath + ", " + superClassName + ">()");
                                        } else {
                                            w.WriteLine("Sqrat::RootTable().Bind(\"" + className + "\", Sqrat::DerivedClass<" + nowPath + ", " + superClassName + ", Sqrat::NoConstructor>()");
                                        }
                                    }
                                } else if (buf.IndexOf("private:") != -1 || buf.IndexOf("protected:") != -1) {
                                    //紐づけないグループはスルー
                                    privateArea = true;
                                } else if (buf.IndexOf(SQ_INSTANCEVALUE) != -1) {
                                    //メンバー変数
                                    temp = GenerateSQGlueCode.GetValueName(buf);
                                    w.WriteLine(".Var(\"" + temp + "\", &" + nowPath + "::" + temp + ")\t\t\t//" + GenerateSQGlueCode.GetValueType(buf));
                                } else if (buf.IndexOf(SQ_INSTANCEFUNC) != -1) {
                                    //メンバーメソッド
                                    temp = GenerateSQGlueCode.GetFuncName(buf);

                                    var funcInfo = GenerateSQGlueCode.GetFuncInfo(buf);
                                    GenerateSQGlueCode.GetArgTypeChars(funcInfo, out var argLength, out var typeChars);

                                    if (!castFuncs.Contains(nowPath + "::" + temp)) {
                                        //キャスト不要
                                        w.WriteLine(".Func(\"" + temp + "\", (&" + nowPath + "::" + temp + $"), {argLength}, {typeChars})\t\t\t//" + funcInfo);
                                    } else {
                                        //要キャスト
                                        w.WriteLine(".Func(\"" + temp + "\", (" + GenerateSQGlueCode.GetFuncCast(buf, nowPath, false) + "&" + nowPath + "::" + temp + $"), {argLength}, {typeChars})\t\t\t//" + funcInfo);
                                    }
                                } else if (buf.IndexOf(SQ_CLASSVALUE) != -1) {
                                    //クラス変数
                                    temp = GenerateSQGlueCode.GetValueName(buf);
                                    w.WriteLine(".SetStaticValue(\"" + temp + "\", &" + nowPath + "::" + temp + ")\t\t\t//" + GenerateSQGlueCode.GetValueType(buf));
                                } else if (buf.IndexOf(SQ_CLASSFUNC) != -1) {
                                    //クラスメソッド
                                    temp = GenerateSQGlueCode.GetFuncName(buf);

                                    var funcInfo = GenerateSQGlueCode.GetFuncInfo(buf);
                                    GenerateSQGlueCode.GetArgTypeChars(funcInfo, out var argLength, out var typeChars);

                                    if (!castFuncs.Contains(nowPath + "::" + temp)) {
                                        //キャスト不要
                                        w.WriteLine(".StaticFunc(\"" + temp + "\", (&" + nowPath + "::" + temp + $"), {argLength}, {typeChars})\t\t\t//" + funcInfo);
                                    } else {
                                        //要キャスト
                                        w.WriteLine(".StaticFunc(\"" + temp + "\", (" + GenerateSQGlueCode.GetFuncCast(buf, nowPath, true) + "&" + nowPath + "::" + temp + $"), {argLength}, {typeChars})\t\t\t//" + funcInfo);
                                    }
                                } else if (buf.IndexOf(SQ_PROPGET) != -1) {
                                    //プロパティメソッド
                                    temp = GenerateSQGlueCode.GetPropName(buf);
                                    var setter = buf.IndexOf(SQ_PROPSET);
                                    if (setter == -1) {
                                        //取得専用プロパティ
                                        w.WriteLine(".Prop(\"" + temp + "\", &" + nowPath + "::get" + temp + ")\t\t\t//" + GenerateSQGlueCode.GetPropType(buf));
                                    } else {
                                        //取得/設定が可能なプロパティ
                                        w.WriteLine(".Prop(\"" + temp + "\", &" + nowPath + "::get" + temp + ", &" + nowPath + "::set" + temp + ")\t\t\t//" + GenerateSQGlueCode.GetPropType(buf));
                                    }
                                } else if (buf.IndexOf(SQ_CLASS) != -1) {
                                    //入れ子クラス開始
                                    className = GenerateSQGlueCode.GetClassName(buf);
                                    GenerateSQGlueCode.IntoBlock(ref nowPath, className);
                                }

                            } else if (buf.IndexOf("}") == 0 && buf.IndexOf("};") == -1) {
                                //名前空間を閉じる
                                GenerateSQGlueCode.OutBlock(ref nowPath);
                            }
                        } while (!r.EndOfStream);
                    }
                }
                w.WriteLine("}");
            }
            Console.WriteLine($"\nグルーコードの生成が完了しました -> {destFile.Substring(Program.CurrentDir.Length)}\n");
        }

        /// <summary>
        /// 指定したブロックの中に入り、アドレスを更新する
        /// </summary>
        public static void IntoBlock(ref string address, string blockName) {
            if (string.IsNullOrEmpty(address)) {
                address = blockName;
            } else {
                address += "::" + blockName;
            }
        }

        /// <summary>
        /// 現在のブロックから抜けて、アドレスを更新する
        /// </summary>
        /// <param name="address"></param>
        public static void OutBlock(ref string address) {
            if (string.IsNullOrEmpty(address)) {
                throw new Exception("グローバル名前空間のクラスを取得しようとしました。");
            } else if (address.IndexOf("::") == -1) {
                address = "";
            } else {
                address = address.Substring(0, address.LastIndexOf("::"));
            }
        }

        /// <summary>
        /// 現在のクラス名を取得する
        /// </summary>
        public static string GetNowClassName(string address) {
            if (address.IndexOf("::") == -1) {
                return address;
            } else {
                return address.Substring(address.LastIndexOf("::") + 2);
            }
        }

        /// <summary>
        /// 一行文字列からクラス名を抜き出す
        /// </summary>
        public static string GetClassName(string buf) {
            var index = buf.IndexOf(SQ_CLASS);
            return buf.Substring(index + SQ_CLASS.Length, buf.IndexOf(" ", index + SQ_CLASS.Length) - (index + SQ_CLASS.Length));
        }

        /// <summary>
        /// 一行文字列から継承元クラス名を抜き出す
        /// </summary>
        public static string GetSuperClassName(string buf) {
            var index = buf.IndexOf(SQ_SUPERCLASS);
            if (index == -1) {
                return "";
            } else {
                return buf.Substring(index + SQ_SUPERCLASS.Length, buf.IndexOf(" ", index + SQ_SUPERCLASS.Length) - (index + SQ_SUPERCLASS.Length));
            }
        }

        /// <summary>
        /// 一行文字列から変数名を抜き出す
        /// </summary>
        public static string GetValueName(string buf) {
            var index = buf.IndexOf(";");
            var equal = buf.IndexOf(" =");
            var count = 1;
            if (equal != -1) {
                index = equal;      //初期化式がある場合はその直前を終点とする
            }
            while (buf.Substring(index - count, 1) != " ") {
                count++;
            }
            return buf.Substring(index - count + 1, count - 1);
        }

        /// <summary>
        /// 一行文字列からプロパティ名を抜き出す
        /// </summary>
        public static string GetPropName(string buf) {
            var index = buf.IndexOf("(");
            var count = 1;
            while (buf.Substring(index - count, 1) != " ") {
                count++;
            }

            //頭のgetを抜いた識別子にする
            var temp = buf.Substring(index - count + 1, count - 1);
            temp = temp.Substring(3);
            return temp;
        }

        /// <summary>
        /// 一行文字列から関数名を抜き出す
        /// </summary>
        public static string GetFuncName(string buf) {
            var index = buf.IndexOf("(");
            var count = 1;
            while (buf.Substring(index - count, 1) != " ") {
                count++;
            }
            return buf.Substring(index - count + 1, count - 1);
        }

        /// <summary>
        /// 一行文字列から変数の型を抜き出す
        /// </summary>
        public static string GetValueType(string buf) {
            //初期化式がある場合はその直前までにトリミングして末尾にセミコロンを付ける
            if (buf.IndexOf(" =") != -1) {
                buf = buf.Substring(0, buf.IndexOf(" =")) + ";";
            }
            var valName = GenerateSQGlueCode.GetValueName(buf);
            var temp = buf.Substring(0, buf.LastIndexOf(valName) - 1);
            return temp.Substring(temp.LastIndexOf(" ") + 1);
        }

        /// <summary>
        /// 一行文字列からプロパティの型を抜き出す
        /// </summary>
        public static string GetPropType(string buf) {
            var index = buf.IndexOf("*");
            if (index == -1) {
                index = buf.IndexOf(" get");
            }
            var count = 1;
            while (buf.Substring(index - count, 1) != " ") {
                count++;
            }
            return buf.Substring(index - count + 1, count - 1);
        }

        /// <summary>
        /// 一行文字列から関数情報（戻り値の型、引数リスト）を抜き出す
        /// </summary>
        public static string GetFuncInfo(string buf) {
            var start = buf.IndexOf("(");
            int end;

            end = buf.LastIndexOf(") {");       //関数宣言の直後に定義が続く場合
            if (end == -1) {
                end = buf.LastIndexOf(")");     //関数宣言のみの場合
            }

            var funcName = GenerateSQGlueCode.GetFuncName(buf);
            string temp;
            var list = new List<string>();

            //戻り値の型を取り出す
            temp = buf.Substring(0, buf.IndexOf(funcName) - 1);
            list.Add(temp.Substring(temp.LastIndexOf(" ") + 1));

            //引数リストを取り出す
            temp = buf.Substring(start + 1, end - start - 1);
            var args = temp.Split(',');
            for (var i = 0; i < args.Length; i++) {
                args[i] = args[i].Trim();                   //両端の余分なスペースをカット
            }
            list.AddRange(args);

            //Console.WriteLine($"\t{funcName}関数 -> 引数 {args.Length} 個");

            //書き出し用文字列に整形する
            temp = list[0] + "|";
            for (var i = 1; i < list.Count; i++) {
                temp += list[i];
                if (i < list.Count - 1) {
                    temp += ",";
                }
            }
            return temp;
        }

        /// <summary>
        /// 関数情報（戻り値の型、引数リスト）から関数のバインドに必要な型情報の文字列と引数の数を返す
        /// </summary>
        public static void GetArgTypeChars(string funcInfo, out int argLength, out string typeStr) {
            var spl = funcInfo.Substring(funcInfo.IndexOf("|") + 1).Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);

            //引数がない場合は NULL 指定にして終了
            if (spl == null || spl.Length == 0) {
                argLength = 0;      //引数の個数をチェックしない
                typeStr = "NULL";
                return;
            }

            // NOTE: 引数の個数はバインド時のみルートテーブルを含めるので一つ増えることに注意する。
            //引数の型を示す文字列をセットする
            argLength = spl.Length + 1;
            typeStr = "\"";
            typeStr += ".";
            for (var i = 0; i < spl.Length; i++) {
                //定数キーワードを除去
                int idx;
                idx = spl[i].IndexOf("const ");
                if (idx == 0) {
                    spl[i] = spl[i].Substring(idx + 6);
                }

                //末尾の参照・ポインタ演算子を除去
                idx = spl[i].IndexOf("*");
                if (idx != -1) {
                    spl[i] = spl[i].Substring(0, idx);
                }
                idx = spl[i].IndexOf("&");
                if (idx != -1) {
                    spl[i] = spl[i].Substring(0, idx);
                }

                //引数の名前を除去
                idx = spl[i].IndexOf(" ");
                if (idx != -1) {
                    spl[i] = spl[i].Substring(0, idx);
                }

                switch (spl[i]) {
                    case "int":
                    case "double":
                    case "float":
                        typeStr += "n";     //スクリプトでは整数も小数も許可する
                        break;
                    case "bool":
                        typeStr += "b|i";
                        break;
                    case "string":
                    case "TCHAR":
                        typeStr += "s";
                        break;
                    default:
                        typeStr += "x";
                        break;
                }
            }
            typeStr += "\"";
        }

        /// <summary>
        /// 一行文字列からオーバーロードした関数のキャスト構文を生成する
        /// </summary>
        public static string GetFuncCast(string buf, string address, bool isStatic) {
            var funcInfo = GenerateSQGlueCode.GetFuncInfo(buf);         //関数情報を取得
            var spl1 = funcInfo.Split('|');                  //戻り値と引数リストに分解
            var spl2 = spl1[1].Split(',');                   //引数リストを分解
            var temp = "(";
            var upAddress = address;                      //現在のクラスの階層
            temp += spl1[0];
            temp += "(";

            if (!isStatic) {
                //メンバー関数の場合はクラス階層まで示す必要がある
                temp += address;
                temp += "::";
            }
            temp += "*)(";

            for (var i = 0; i < spl2.Length; i++) {
                temp += spl2[i].Substring(0, spl2[i].LastIndexOf(" "));
                if (i < spl2.Length - 1) {
                    temp += ", ";
                }
            }
            temp += "))";
            return temp;
        }
    }
}
