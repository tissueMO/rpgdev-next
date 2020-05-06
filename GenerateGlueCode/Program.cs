using System;
using System.Text;

namespace GenerateGlueCode {
    public static class Program {
        public static Encoding SJIS = Encoding.GetEncoding("Shift-JIS");
        public static string CurrentDir {
            get {
                return currentDir;
            }
            set {
                currentDir = value;
                Console.WriteLine("------------------------------------------------------------------");
                Console.WriteLine($"カレントディレクトリ\n>{currentDir}\n");
            }
        }
        private static string currentDir = "";


        /// <summary>
        /// エントリーポイント
        /// </summary>
        private static void Main(string[] args) {
#if !DEBUG
            //リリースモード時は起動させないようにする
            return;
#else
            //C#の定数群をC++に移植する
            if (QuestionYesNo("\n1. C#の定数定義をC++へ移植しますか？")) {
                GenerateCPPConstants.Generate();
            }

            //C++用のグルーコードを生成する
            if (QuestionYesNo("\n\n2. SQグルーコードを生成しますか？")) {
                GenerateSQGlueCode.Generate();
            }

            //生成したコードからエディターのツリー用ドキュメントを生成する
            if (QuestionYesNo("\n\n3. エディター用のドキュメントを生成しますか？")) {
                GenerateCPPDocument.Generate();
            }

            //ツリー用ドキュメントから未設定の項目を列挙する
            if (Program.QuestionYesNo("\n\n4. 未設定のリファレンス項目を検索しますか？")) {
                GenerateCPPDocument.LoadDocument();

                var cnt = 0;
                foreach (var item in GenerateCPPDocument.SQObjects) {
                    if (string.IsNullOrEmpty(item.Summary)) {
                        cnt++;
                        Console.WriteLine($"\t{item.ClassPath}: Summary");
                    }
                    for (var i = 0; i < item.ArgumentList.Count; i++) {
                        if (string.IsNullOrEmpty(item.ArgComments[i])) {
                            cnt++;
                            Console.WriteLine($"\t{item.ClassPath}: par{i + 1}");
                        }
                    }
                }
                if (cnt == 0) {
                    Console.WriteLine($"\tすべて設定されています。");

                    //ツリー用ドキュメントからHTMLマニュアル用のJavaScriptコードを生成する
                    if (QuestionYesNo("\n\n5. HTMLマニュアル用のJavaScriptコードを生成しますか？")) {
                        GenerateJSForHTML.Generate();
                    }
                }
            }

            Console.WriteLine("\n終了するには何かキーを押して下さい...");
            Console.ReadKey();
#endif
        }

        /// <summary>
        /// はい/いいえ を問う
        /// </summary>
        public static bool QuestionYesNo(string msg) {
            Console.WriteLine(msg + "(y/n)");
            var result = (Console.ReadKey().Key == ConsoleKey.Y);
            Console.WriteLine("\n");
            return result;
        }
    }
}
