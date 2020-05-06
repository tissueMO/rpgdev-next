using DxLibDLL;
using System;
using System.IO;
using System.Windows.Forms;

namespace Editor {
    using Module;
    using Properties;
    using System.Text;

    public class Program {
        private static MainWindow wnd;

        [STAThread]
        private static void Main(string[] args) {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);

            //先行処理
            DX.SetOutApplicationLogValidFlag(DX.FALSE);

            //メインウィンドウ起動準備
            wnd = new MainWindow();
            Media.GGS = GuruGuruSmf.Ggs4Dll.GetInstance();
            Media.GGS.OpenDevice(GuruGuruSmf.Device.DirectMusic, wnd.Handle);

            //実際には描画しないコントロールをセットした状態でDXライブラリを初期化しないと色々不具合が起きる
            //	1. 描画先を変更したときに２回目だけ反映されない（ただし、DXライブラリ側によって解決済み）
            //	2. 最初にセットしたコントロールのサイズが変わると、描画結果が元のサイズから拡大縮小されたものになってしまう
            Media.Init(wnd.pnlDefaultDrawTarget.Handle);

            wnd.SplashCloseFlag = true;

            //コマンドライン引数にプロジェクトファイルが指定されている場合はそれを開く
            ProjectManager project = null;
            if (args.Length > 0 && File.Exists(args[0])) {
                var pPath = Path.GetDirectoryName(Path.GetDirectoryName(args[0]));
                project = new ProjectManager(wnd, pPath);
                project = ProjectManager.LoadProjectFile(project);
            } else {
                //前に開いたプロジェクトを開く？
                if (string.IsNullOrEmpty(Settings.Default.Last_ProjectPath) == false
                && ProjectManager.CheckProjectInfo(Settings.Default.Last_ProjectPath)) {
                    if (MessageBox.Show("前回開いたプロジェクトを開きますか？", Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1, MessageBoxOptions.ServiceNotification) == DialogResult.Yes) {
                        project = new ProjectManager(wnd, Settings.Default.Last_ProjectPath);
                        project = ProjectManager.LoadProjectFile(project);
                    }
                }
            }
            if (project == null) {
                //プロジェクトオープンできず
                Application.Exit();
            }
            wnd.Project = project;

            //内部ログを初期化
            var errorLogBuffer = new StringBuilder();

            //ハンドルされない例外が発生したときの処理: 再開可能
            Application.ThreadException += (sender, e) => {
                //内部ログへスタックトレースと例外メッセージを書き込む
                addErrorLog(errorLogBuffer, e.Exception);

                MessageBox.Show("予期せぬエラーが発生しました。\r\nこのエラーが何度も発生するときは、プログラム終了後に出力されるログを作者までご送付下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            };

            //ハンドルされない例外が発生したときの処理: 再開不可
            AppDomain.CurrentDomain.UnhandledException += (sender, e) => {
                //内部ログへスタックトレースと例外メッセージを書き込む
                errorLogBuffer.Append("\r\n" + DateTime.Now.ToString(Common.DateFormat) + "\r\n");

                var ex = (e.ExceptionObject as Exception);
                if (ex != null) {
                    addErrorLog(errorLogBuffer, ex);
                } else {
                    errorLogBuffer.Append("(原因不明のエラーが発生しました)\r\n\r\n");
                    errorLogBuffer.Append("---------------------------------------------------------\r\n");
                }

                MessageBox.Show("予期せぬエラーが発生したため、" + Resources.AppName + "を終了します。\r\nこのエラーが何度も発生するときは、プログラム終了後に出力されるログを作者までご送付下さい。", Resources.AppName, MessageBoxButtons.OK, e.IsTerminating ? MessageBoxIcon.Error : MessageBoxIcon.Warning);
            };

            //メインウィンドウ立ち上げ
            Application.Run(wnd);

            if (errorLogBuffer.Length == 0) {
                //ログがないときはそのまま終了
                return;
            }

            //ログが出力されているときは書き出す
            try {
                File.WriteAllText(
                    Application.StartupPath + "\\" + DateTime.Now.ToString("yy-MM-dd HH-mm-ss") + " - " + Environment.UserName + ".log",
                    errorLogBuffer.ToString(),
                    Common.SJIS
                );
            } catch (Exception e) {
                MessageBox.Show("ログの書き出しに失敗しました。\r\n\r\n" + e.Message, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// ログバッファにログを追記します。
        /// </summary>
        private static void addErrorLog(StringBuilder errorLogBuffer, Exception e) {
            errorLogBuffer.Append("\r\n" + DateTime.Now.ToString(Common.DateFormat) + "\r\n");
            errorLogBuffer.Append(" " + e.Message + "\r\n\r\n");
            errorLogBuffer.Append(e.StackTrace + "\r\n\r\n");

            // InnerException がある場合はすべて表示する
            var subEx = e;
            for (var i = 1; true; i++) {
                e = e.InnerException;
                if (e == null) {
                    //次の例外がなくなったら終了
                    break;
                }

                errorLogBuffer.Append(" InnerException[" + i.ToString() + "]: " + e.Message + "\r\n");
                errorLogBuffer.Append(e.StackTrace + "\r\n\r\n");
            }

            errorLogBuffer.Append("---------------------------------------------------------\r\n");
        }

    }
}
