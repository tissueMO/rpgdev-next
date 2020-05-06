using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Module;
    using Properties;

    public partial class dlgFileSelect : Form {
        private const string TagTileSet = "tileset";      //ファイルノードにおけるタイルセットのタグ

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgFileSelect(string rootPath, string fileFilter, bool visibleFiles, bool enabledTileset, bool onlyTileset, string defaultPath = "") {
            this.InitializeComponent();
            this.FileFilter = fileFilter;
            this.EnabledTileset = enabledTileset;
            this.OnlyTileset = onlyTileset;
            this.IsFileVisible = visibleFiles;
            this.FileName = defaultPath;
            this.RootPath = rootPath;

            this.uctlFileTree.IncludeFiles = visibleFiles;
            this.uctlFileTree.ReloadTree(defaultPath);

            //既存選択がタイルの場合はそのタイルを選択する
            if (this.IsTilesetSelected) {
                this.uctlTileset.Position = Common.StringToPoint(defaultPath);
            }
        }

        /// <summary>
        /// タイルセットを有効にするか
        /// </summary>
        public bool EnabledTileset {
            get {
                return this.enabledTileset;
            }
            set {
                this.enabledTileset = value;
                if (!value) {
                    this.OnlyTileset = false;
                }
            }
        }
        private bool enabledTileset = false;

        /// <summary>
        /// タイルセットだけを選択するモードにするか
        /// </summary>
        public bool OnlyTileset {
            get {
                return this.onlyTileset;
            }
            set {
                this.onlyTileset = value;
                if (value) {
                    this.EnabledTileset = true;
                }
            }
        }
        private bool onlyTileset = false;

        /// <summary>
        /// 基準となるフォルダーへのパス（プロジェクトフォルダーからの相対パス）
        /// </summary>
        private string RootPath {
            get {
                return this.uctlFileTree.RootPath;
            }
            set {
                this.uctlFileTree.RootPath = value;
            }
        }

        /// <summary>
        /// 選択されたファイルへの基準フォルダーを含むパス
        /// </summary>
        public string FileName {
            get;
            private set;
        } = "";

        /// <summary>
        /// タイルセットが選択されているか
        /// </summary>
        public bool IsTilesetSelected {
            get {
                return (this.FileName?.IndexOf(Resources.Split_IDNamePair) != -1);
            }
        }

        /// <summary>
        /// ファイルを表示させるか
        /// </summary>
        public bool IsFileVisible {
            get;
            private set;
        } = true;

        /// <summary>
        /// 有効な拡張子
        /// </summary>
        private string FileFilter {
            get {
                return this.uctlFileTree.FileFilter;
            }
            set {
                this.uctlFileTree.FileFilter = value;
            }
        }

        /// <summary>
        /// 初期化
        /// </summary>
        private void dlgFileSelect_Load(object sender, EventArgs e) {
            //タイルセットを表示する場合は特殊なノードを追加する
            if (this.EnabledTileset) {
                var node = new TreeNode("*タイルセット") {
                    Tag = TagTileSet,
                };
                this.uctlFileTree.AutoReload = false;        //独自に追加したノードが消えないようにする

                if (this.OnlyTileset) {
                    //タイルセットだけを選択するモードのときは通常ノードをすべて削除する
                    this.uctlFileTree.ClearTreeNodesAll();
                }
                this.uctlFileTree.InsertRoot(node);
            }
        }

        /// <summary>
        /// 破棄
        /// </summary>
        private void dlgFileSelect_FormClosing(object sender, FormClosingEventArgs e) {
            this.uctlMusicPlayer.UnloadMedia();
        }

        /// <summary>
        /// ファイルを選択しないで確定する
        /// </summary>
        private void btnNothing_Click(object sender, EventArgs e) {
            this.FileName = "";
            this.DialogResult = DialogResult.OK;
        }

        /// <summary>
        /// 選択情報を検証
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (this.uctlFileTree.SelectedNodeTag == TagTileSet) {
                //タイル位置としてファイル名をセットする
                this.FileName = this.uctlTileset.Position.X + Resources.Split_IDNamePair + this.uctlTileset.Position.Y;
            } else if (this.uctlFileTree.IsFileSelected == false && this.IsFileVisible) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "ファイル"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            } else if (this.uctlFileTree.IsSelected == false && !this.IsFileVisible) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "フォルダー"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
            } else {
                this.FileName = (Common.AddToPathSplit(Path.GetDirectoryName(Common.CutLastChar(this.uctlFileTree.RootPath, '\\'))) + this.uctlFileTree.SelectedNodeFullPath).Substring(ProjectManager.ProjectPath.Length);
            }
        }

        /// <summary>
        /// 選択したファイルをプレビュー
        /// </summary>
        private void uctlFileTree_NodeAfterSelect(object sender, TreeViewEventArgs e) {
            this.uctlMusicPlayer.Enabled = false;
            this.picViewer.Image = null;
            this.uctlTileset.Visible = false;
            if (this.uctlFileTree.IsFileSelected) {
                if (Common.CheckFileExtensionFilter(this.uctlFileTree.SelectedNodeText, Resources.Extension_Graphics)) {
                    //グラフィックファイルを選択した場合
                    this.picViewer.Visible = true;
                    this.picViewer.SizeMode = PictureBoxSizeMode.Zoom;
                    this.picViewer.LoadAsync(this.uctlFileTree.SelectedNodeFullPathOnFileSystem);
                } else if (Common.CheckFileExtensionFilter(this.uctlFileTree.SelectedNodeText, Resources.Extension_Sounds)) {
                    //サウンドファイルを選択した場合
                    this.uctlTileset.Visible = false;
                    this.picViewer.Visible = false;
                    this.uctlMusicPlayer.Enabled = true;
                }
            } else if (this.uctlFileTree.IsSelected) {
                if (this.uctlFileTree.SelectedNodeTag == TagTileSet) {
                    //タイルセットを選択した場合
                    this.uctlTileset.Visible = true;
                    this.picViewer.Visible = false;
                    var bmp = CtrlComponent.Map.ctlTilePallet.PalletGraphics;
                    if (bmp != null) {
                        this.uctlTileset.Setup(bmp, new Size(bmp.Width / Map.TileColumnCount, bmp.Width / Map.TileColumnCount), Common.StringToPoint(this.FileName));
                    }
                }
            }
        }

        /// <summary>
        /// 音楽プレビュー直前に読み込む
        /// </summary>
        private void uctlMusicPlayer_BeforePlaySound(object sender, EventArgs e) {
            this.uctlMusicPlayer.LoadMedia(this.uctlFileTree.SelectedNodeFullPathOnFileSystem.Replace(ProjectManager.ProjectPath, ""));
        }

        /// <summary>
        /// タイルをダブルクリックで確定する
        /// </summary>
        private void uctlTileset_DoubleClick(object sender, EventArgs e) {
            this.DialogResult = DialogResult.OK;
            this.uctlOKCancel_ClickOK(sender, e);
        }

        /// <summary>
        /// ファイルをダブルクリックで確定する
        /// </summary>
        private void uctlFileTree_FileDoubleClicked(object sender, EventArgs e) {
            this.uctlTileset_DoubleClick(sender, e);
        }

        /// <summary>
        /// マウスカーソルがコントロールに入ると自動でタイル一覧にフォーカスを当てる
        /// </summary>
        private void uctlTileset_MouseEnter(object sender, EventArgs e) {
            this.uctlTileset.Focus();
        }
    }
}
