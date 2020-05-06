using System.IO;
using System.Windows.Forms;

namespace Editor {
    using Module;
    using Properties;

    /// <summary>
    /// プロジェクトに関する情報です
    /// </summary>
    [System.Runtime.Serialization.DataContract]
    public class ProjectManager {
        private MainWindow mainWnd = null;

        [System.Runtime.Serialization.IgnoreDataMember]
        [System.ComponentModel.Description("このプロジェクトフォルダーのパスです。\r\nこのプロパティは変更できません。")]
        public static string ProjectPath {
            get {
                return projectPath;
            }
            set {
                //末尾に必ず区切り記号を入れる
                projectPath = Common.AddToPathSplit(value);
            }
        }
        private static string projectPath;

        [System.Runtime.Serialization.DataMember(Name = "projectName")]
        [System.ComponentModel.Description("プロジェクト名です。\r\nコードネーム等でも構いませんが、ゲームを発行するときの名前として使われます。")]
        public string ProjectName {
            get {
                return this.projectName;
            }
            set {
                if (!Common.CheckEnabledPath(value)) {
                    MessageBox.Show("ファイル名に使用できない文字が含まれています。\r\nプロジェクト名を修正して下さい。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                this.projectName = value;
            }
        }
        private string projectName;

        /// <summary>
        /// 新規プロジェクトを立ち上げます。
        /// </summary>
        /// <param name="wnd">メインウィンドウのインスタンス参照</param>
        /// <param name="rootPath">プロジェクトのルートフォルダー</param>
        /// <param name="projectName">プロジェクト名</param>
        /// <param name="loadPreset">プリセットデータを読み込むかどうか</param>
        public ProjectManager(MainWindow wnd, string rootPath, string projectName, bool loadPreset) {
            rootPath = Common.AddToPathSplit(rootPath);

            //プロジェクトのルートフォルダーを作成
            try {
                Directory.CreateDirectory(rootPath + projectName);
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "プロジェクトフォルダーの作成"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            //プロジェクト設定
            ProjectManager.ProjectPath = Common.AddToPathSplit(rootPath) + projectName;

            this.ProjectName = projectName;
            this.createSystemComponents();          //プロジェクトに必要なファイル群を生成
            ProjectManager.SaveProjectFile(this);

            if (loadPreset) {
                //プリセットデータを取り込む
                Common.CopyDirectory(Application.StartupPath + "\\" + Resources.Path_PresetData, Common.CutLastChar(ProjectManager.ProjectPath, '\\'), true);
            }

            this.mainWnd = wnd;
        }

        /// <summary>
        /// 既存プロジェクトを開きます。
        /// 別途サインインもしくはサインアップする必要があります。
        /// </summary>
        /// <param name="wnd">メインウィンドウのインスタンス参照</param>
        /// <param name="rootPath">プロジェクトのルートフォルダー</param>
        public ProjectManager(MainWindow wnd, string rootPath) {
            //プロジェクト設定
            ProjectManager.ProjectPath = rootPath;
            this.createSystemComponents();      //プロジェクトに必要なファイル群を生成
            this.mainWnd = wnd;
        }

        /// <summary>
        /// プロジェクトを保存します。
        /// </summary>
        public ConfirmResult Save(bool confirm) {
            var result = this.mainWnd.AllSave(confirm);
            if (result != ConfirmResult.Completed) {
                MessageBox.Show(Resources.MsgW_ResultError, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            return result;
        }

        /// <summary>
        /// プロジェクトファイルを書き出します。
        /// プロジェクトファイルはJSONで書き出すため、UTF-8で扱います。
        /// </summary>
        /// <param name="prj">保存対象のプロジェクト</param>
        public static void SaveProjectFile(ProjectManager prj) {
            using (var M = new MemoryStream()) {
                using (var R = new StreamReader(M)) {
                    using (var W = new StreamWriter(Common.OpenFileWriteOnly(ProjectManager.ProjectPath + Resources.Path_PrjSys_PrjFile), System.Text.Encoding.UTF8)) {
                        using (var writer = System.Runtime.Serialization.Json.JsonReaderWriterFactory.CreateJsonWriter(M, System.Text.Encoding.UTF8, true, true, "    ")) {
                            var serializer = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(ProjectManager));
                            serializer.WriteObject(writer, prj);
                            writer.Flush();
                            var json = System.Text.Encoding.UTF8.GetString(M.ToArray());
                            W.Write(json);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// プロジェクトファイルを読み込みます。
        /// プロジェクトファイルはJSONで書き出すため、UTF-8で扱います。
        /// </summary>
        /// <param name="prj">ロード前のプロジェクト</param>
        /// <returns>ロード済みのプロジェクト</returns>
        public static ProjectManager LoadProjectFile(ProjectManager prj) {
            // 全般設定
            using (var R = new StreamReader(ProjectManager.ProjectPath + Resources.Path_PrjSys_PrjFile, System.Text.Encoding.UTF8)) {
                using (var M = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(R.ReadToEnd()))) {
                    var serializer = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(ProjectManager));
                    var mainWnd = prj.mainWnd;
                    prj = (ProjectManager) serializer.ReadObject(M);
                    prj.mainWnd = mainWnd;
                }
            }
            return prj;
        }

        /// <summary>
        /// 正規のプロジェクトかどうかを検査します。
        /// </summary>
        public static bool CheckProjectInfo(string rootPath) {
            //プロジェクトファイルが指定場所にあることを確認する
            return File.Exists(Common.AddToPathSplit(rootPath) + Resources.Path_PrjSys_PrjFile);
        }

        /// <summary>
        /// プロジェクトに最低限必要なファイルが不足している場合は自動生成します。
        /// </summary>
        private void createSystemComponents() {
            //リソースから読み込んでフォルダー構造を自動生成
            Common.CreateFolders(ProjectManager.ProjectPath, Resources.ProjectFolderDefault);
        }

        /// <summary>
        /// プロジェクト内に存在する不要なファイルを除去します。
        /// </summary>
        public void CleanGarbage() {
            //各種エディターのゴミを除去する
            this.mainWnd.AllCleanup();
        }

    }
}
