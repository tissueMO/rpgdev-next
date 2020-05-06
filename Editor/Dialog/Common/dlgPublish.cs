using System;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;
    using System.Threading.Tasks;

    public partial class dlgPublish : Form {
        private readonly ProjectManager prj;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgPublish(ProjectManager prj) {
            this.InitializeComponent();
            this.prj = prj;

            //アーカイブすべき既定のフォルダーにチェックを入れる
            this.uctlArchiveTree.CheckTreeNode(Resources.Path_DBs, true, true);
            this.uctlArchiveTree.CheckTreeNode(Resources.Path_Materials, true, true);
            this.uctlArchiveTree.CheckTreeNode(Resources.Path_Fonts, true, true);
            this.uctlArchiveTree.CheckTreeNode(Resources.Path_Scripts, true, true);
            this.uctlArchiveTree.CheckTreeNode(Resources.Path_Maps, true, true);

            //コピーすべき既定のファイルにチェックを入れる
            //以下は別途コピー処理を行うので必要なし
            //uctlCopyTree.CheckTreeNode(Resources.Path_GameProgram, true, false);
            //uctlCopyTree.CheckTreeNode(Resources.Path_GGS, true, false);
        }

        /// <summary>
        /// 確定して発行する
        /// </summary>
        private async void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (string.IsNullOrEmpty(this.uctlDestPath.Result)) {
                //発行先フォルダーが選択されていない
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "発行先のフォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return;
            }

            //非同期的に実行する
            this.DialogResult = DialogResult.None;
            this.pnlProcess.Visible = true;
            this.uctlOKCancel.Enabled = false;
            var succeeded = await Task.Run(() => this.doPublish());
            this.pnlProcess.Visible = false;
            if (succeeded) {
                // 完了後の処理
                MessageBox.Show("製品版の発行が完了しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Information);

                //発行先フォルダーを開く
                try {
                    if (Directory.Exists(Common.CutLastChar(this.uctlDestPath.Result, '\\'))) {
                        System.Diagnostics.Process.Start(Common.CutLastChar(this.uctlDestPath.Result, '\\'));
                    }
                } catch {
                    MessageBox.Show(Resources.MsgE_Failed.Replace("$", "発行先フォルダーのオープン"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                this.Close();
            }

            this.uctlOKCancel.Enabled = true;
        }

        /// <summary>
        /// 実際に発行処理を行う
        /// </summary>
        /// <returns>正常に発行処理を完了できたかどうか</returns>
        private bool doPublish() {
            //処理中に一回でもエラーが起きたら中断する
            var procName = "";
            try {
                procName = "初期処理";
                var destDir = Common.AddToPathSplit(this.uctlDestPath.Result) + this.prj.ProjectName + "\\";
                var srcDir = Path.GetDirectoryName(Common.CutLastChar(ProjectManager.ProjectPath, '\\')) + "\\";
                var destZip = Path.GetDirectoryName(Common.CutLastChar(destDir, '\\')) + "\\" + this.prj.ProjectName + ".zip";

                if (this.chkEnabledZip.Checked) {
                    //発行先のzipファイルを確認
                    if(File.Exists(destZip)) {
                        if(MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "発行先のZIPファイルが既に存在します。\r\n発行前に削除"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning) != DialogResult.Yes) {
                            throw new Exception("発行先のZIPファイルが既に存在しています。");
                        }

                        procName = "発行先に存在しているZIPファイルの削除";
                        File.Delete(destZip);
                    }
                }

                //発行先のフォルダーを作成する
                procName = "発行先フォルダーの作成";
                if (Directory.Exists(Common.CutLastChar(destDir, '\\'))) {
                    throw new Exception("既に発行先フォルダーが存在します。");
                }
                Directory.CreateDirectory(Common.CutLastChar(destDir, '\\'));

                //プロジェクト内のゴミを除去する
                procName = "プロジェクト内の不要なファイルの除去";
                this.prj.CleanGarbage();

                //有効時：autorun.infを生成する
                if (this.chkAutorun.Checked) {
                    procName = "「autorun.inf」ファイルの作成";

                    //ゲームプログラムのアイコンファイルを発行先フォルダーにコピーする
                    using (var W = new FileStream(destDir + "icon.ico", FileMode.Create, FileAccess.Write)) {
                        Resources.icon_gameprogram.Save(W);
                    }

                    //autorun.infを生成する
                    File.WriteAllText(destDir + "autorun.inf", Resources.autorun);
                }

                //単純コピーするフォルダーやファイルのコピーを行う
                procName = "単純コピー処理";
                foreach (var rPath in this.uctlCopyTree.CheckedNodePaths) {
                    if (File.Exists(srcDir + rPath)) {
                        if (File.Exists(destDir + Common.CutRootFolderName(rPath))) {
                            //コピー先に既にファイルが存在する場合は先に削除しておく
                            File.Delete(destDir + Common.CutRootFolderName(rPath));
                        }
                        File.Copy(srcDir + rPath, destDir + Common.CutRootFolderName(rPath));
                    } else if (Directory.Exists(srcDir + rPath)) {
                        Common.CopyDirectory(srcDir + rPath, destDir + Common.CutRootFolderName(rPath));
                    }
                }

                //GuruGuruSMF.dllを発行先へコピーする
                procName = "「GuruGuruSMF.dll」のコピー";
                File.Copy(Application.StartupPath + "\\" + Resources.Path_GGS, destDir + Resources.Path_GGS, true);

                //リリース用のゲームプログラムを発行先へコピーする
                procName = "リリース用ゲームプログラムのコピー";
                File.Copy(Application.StartupPath + "\\" + Resources.Path_GameProgramRelease, destDir + Resources.Path_GameProgram, true);

                //Dataフォルダーを作成する
                procName = "「Data」フォルダーの作成";
                Directory.CreateDirectory(destDir + Resources.Path_ArchiveData);

                //アーカイブするフォルダーをすべてDataフォルダーにコピーする
                procName = "アーカイブ対象フォルダーのコピー";
                foreach (var rPath in this.uctlArchiveTree.CheckedNodePaths) {
                    Common.CopyDirectory(srcDir + rPath, destDir + Resources.Path_ArchiveData + "\\" + Common.CutRootFolderName(rPath));
                }
                var mapDestDir = Path.GetFileName(Path.GetDirectoryName(ProjectManager.ProjectPath)) + "\\" + Path.GetDirectoryName(Resources.Path_Maps);
                if (this.uctlArchiveTree.CheckedNodePaths.Contains(mapDestDir)) {
                    // マップデータのみgzip圧縮したファイルとして書き出す
                    procName = "マップファイルの圧縮";
                    var mapFiles = Directory.GetFiles(destDir + Resources.Path_ArchiveData + "\\" + Resources.Path_Maps, "*.map", SearchOption.AllDirectories);

                    foreach (var mapFile in mapFiles) {
                        string mapData;
                        using (var R = new StreamReader(Common.OpenFileReadOnly(mapFile), Common.SJIS)) {
                            // 非圧縮のマップデータを読み込む
                            mapData = R.ReadToEnd();
                        }
                        using (var gz = new System.IO.Compression.GZipStream(Common.OpenFileWriteOnly(mapFile), System.IO.Compression.CompressionLevel.Fastest))
                        using (var W = new StreamWriter(gz, Common.SJIS)) {
                            // gzip圧縮して書き出す
                            W.Write(mapData);
                        }
                    }
                }

                //スクリプトをバイトコードにするとゲームプログラム側で実行できなくなる・エンディアン依存になる・定数に関する問題などが絡むため、コンパイルしないことにした
                //スクリプトを一斉にコンパイルする
                //procName = "スクリプトのコンパイル";
                //var sqcpl = System.Diagnostics.Process.Start(
                //	ProjectManager.ProjectPath + Resources.Path_GameProgram,
                //	$"sc \"{destDir + Resources.Path_ArchiveData}\""
                //);
                //sqcpl.WaitForExit();
                //if(sqcpl.ExitCode != 0) {
                //	throw new Exception("いくつかのスクリプトでコンパイルエラーが発生しています。");
                //}

                //DataフォルダーをDXアーカイブする: アーカイバーは絶対パスでファイル名を扱うため、必ず絶対パスで与える
                procName = "「Data」フォルダーのアーカイブ処理";
                var dxarc = System.Diagnostics.Process.Start(
                    ProjectManager.ProjectPath + Resources.Path_GameProgram,
                    $"ac \"{destDir}{Resources.Path_ArchiveData}\" \"{Common.CutLastChar(destDir, '\\')}\""
                );
                dxarc.WaitForExit();
                if (dxarc.ExitCode != 0) {
                    throw new Exception("考えられる原因はI/Oエラーもしくはアーカイブの内部処理です。");
                }

                //アーカイブ元のフォルダーを削除する
                procName = "アーカイブ元フォルダーの削除";
                Common.DeleteDirectory(destDir + Resources.Path_ArchiveData);

                //有効時：発行先フォルダーのZIP圧縮を行い、成功したら圧縮元フォルダーを削除する
                if (this.chkEnabledZip.Checked) {
                    procName = "発行先フォルダーのZIP圧縮処理";

                    //ZIPの出力先は出力先フォルダーの一つ上の階層となる
                    System.IO.Compression.ZipFile.CreateFromDirectory(Common.CutLastChar(destDir, '\\'), destZip, System.IO.Compression.CompressionLevel.Fastest, true);

                    //圧縮元のフォルダーを削除する
                    Common.DeleteDirectory(Common.CutLastChar(destDir, '\\'));
                }

                this.DialogResult = DialogResult.OK;
                return true;
            } catch (Exception ex) {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", procName) + "\r\n\r\n" + ex.Message, Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return false;
            }
        }
    }
}
