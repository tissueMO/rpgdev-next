using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;

    /// <summary>
    /// 設定用のプロパティ群
    /// </summary>
    [Serializable()]
    public class DBProperty {
        [System.Xml.Serialization.XmlIgnore]
        public DataGridView DB = null;        //親クラスが持つオブジェクトへの参照

        /// <summary>
        /// 既定のコンストラクター：シリアライズに必要
        /// </summary>
        public DBProperty() {
        }

        /// <summary>
        /// 現在の情報を関連付けたDataGridViewに反映します。
        /// </summary>
        public void SetDB() {
            //システムDBの項目を定義する
            this.DB.Rows.Clear();
            this.DB.Rows.Add();

            for (var i = 0; i < Common.GetEnumCount<Database.DBSystemIndices>(); i++) {
                var row = this.DB.Rows[this.DB.RowCount - 1];

                switch (i) {
                    case (int) Database.DBSystemIndices.GameTitle:
                        row.Cells[i].Value = this.strGameTitle;
                        break;
                    case (int) Database.DBSystemIndices.GameVersion:
                        row.Cells[i].Value = this.strGameVersion;
                        break;
                    case (int) Database.DBSystemIndices.WindowSize:
                        row.Cells[i].Value = this.sizeWindowSize;
                        break;
                    case (int) Database.DBSystemIndices.AllowFullScreen:
                        row.Cells[i].Value = this.boolAllowFullScreen;
                        break;
                    case (int) Database.DBSystemIndices.NowLoadingFileName:
                        row.Cells[i].Value = this.strNowLoadingFileName;
                        break;
                    case (int) Database.DBSystemIndices.IconFileName:
                        row.Cells[i].Value = this.strIconFileName;
                        break;
                    case (int) Database.DBSystemIndices.StandardTransparentColor:
                        row.Cells[i].Value = this.StandardTransparentColor;
                        break;
                    case (int) Database.DBSystemIndices.DrawInterpolation:
                        row.Cells[i].Value = this.DrawInterpolation;
                        break;
                    case (int) Database.DBSystemIndices.EffectAsyncLoadMode:
                        row.Cells[i].Value = this.boolEffectAsyncLoadMode;
                        break;
                    case (int) Database.DBSystemIndices.TileSize:
                        row.Cells[i].Value = this.intTileSize;
                        break;
                    case (int) Database.DBSystemIndices.IconSize:
                        row.Cells[i].Value = this.intIconSize;
                        break;
                    case (int) Database.DBSystemIndices.MaxPartyCount:
                        row.Cells[i].Value = this.intMaxPartyCount;
                        break;
                    case (int) Database.DBSystemIndices.MaxLevel:
                        row.Cells[i].Value = this.intMaxLevel;
                        break;
                    case (int) Database.DBSystemIndices.MaxSaveDataCount:
                        row.Cells[i].Value = this.intMaxSaveDataCount;
                        break;
                    case (int) Database.DBSystemIndices.DBCharGraphicsViewRange:
                        row.Cells[i].Value = this.edtDBCharGraphicsViewRange;
                        break;
                    case (int) Database.DBSystemIndices.EnemyLevelExpression:
                        row.Cells[i].Value = this.edtEnemyLevelExpression;
                        break;
                    case (int) Database.DBSystemIndices.EffectFPS:
                        row.Cells[i].Value = this.intEffectFPS;
                        break;
                    case (int) Database.DBSystemIndices.ShadowTransparent:
                        row.Cells[i].Value = this.intShadowTransparent;
                        break;
                    case (int) Database.DBSystemIndices.FogTransparent:
                        row.Cells[i].Value = this.intFogTransparent;
                        break;
                    case (int) Database.DBSystemIndices.EnabledSystemResource:
                        row.Cells[i].Value = this.boolEnabledSystemResource;
                        break;
                    case (int) Database.DBSystemIndices.EnabledLimitedDebug:
                        row.Cells[i].Value = this.boolEnabledLimitedDebug;
                        break;
                    default:
                        throw new Exception($"{i}: 無効なシステムDBの項目です。");
                }
            }
        }

        [System.ComponentModel.Category("アプリケーション情報")]
        [System.ComponentModel.Description("タイトルバーに表示されるゲームのタイトルです。")]
        public string strGameTitle {
            get; set;
        } = "無題のゲーム";

        [System.ComponentModel.Category("アプリケーション情報")]
        [System.ComponentModel.Description("ゲームの内部バージョン番号です。\r\nこのバージョンに合致するセーブデータのみ有効とみなされるため、変更すると以前のセーブデータは読み込めなくなります。")]
        public string strGameVersion {
            get; set;
        } = "1.00";

        [System.ComponentModel.Category("アプリケーション情報")]
        [System.ComponentModel.Description("ゲームウィンドウのサイズです。")]
        public Size sizeWindowSize {
            get {
                return this.windowSize;
            }
            set {
                if (value.Width > 1080 * 2 || value.Height >= 1080 * 2) {
                    //４Ｋより大きいものはDXライブラリ的にも危ないので禁止する
                    throw new Exception("ウィンドウサイズが大きすぎます。");
                } else if (value.Width < 100 || value.Height < 100) {
                    //小さすぎるものも禁止する
                    throw new Exception("ウィンドウサイズが小さすぎます。");
                }
                this.windowSize = value;
            }
        }
        private Size windowSize = new Size(640, 480);

        [System.ComponentModel.Category("アプリケーション情報")]
        [System.ComponentModel.Description("フルスクリーンモードを許可するかどうかを設定します。")]
        public bool boolAllowFullScreen {
            get; set;
        } = true;

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("ゲームが起動している間に表示される、NowLoadingの画像です。")]
        [Editor(typeof(System.Windows.Forms.Design.FileNameEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public string strNowLoadingFileName {
            get {
                return this.nowLoadingFileName;
            }
            set {
                if (string.IsNullOrEmpty(value)) {
                    this.nowLoadingFileName = "";
                } else if (value.IndexOf(ProjectManager.ProjectPath) == -1) {
                    this.nowLoadingFileName = value;     //プロジェクトフォルダーのパスが含まれない場合はそのまま登録する
                } else {
                    this.nowLoadingFileName = value.Substring(ProjectManager.ProjectPath.Length);        //プロジェクトフォルダー内のファイルであれば短縮する
                }
            }
        }
        private string nowLoadingFileName = "";

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("データベースで使われるアイコンをすべて格納した画像ファイルです。")]
        [Editor(typeof(System.Windows.Forms.Design.FileNameEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public string strIconFileName {
            get {
                return this.iconFileName;
            }
            set {
                if (string.IsNullOrEmpty(value)) {
                    this.iconFileName = "";
                } else if (value.IndexOf(ProjectManager.ProjectPath) == -1) {
                    this.iconFileName = value;     //プロジェクトフォルダーのパスが含まれない場合はそのまま登録する
                } else {
                    this.iconFileName = value.Substring(ProjectManager.ProjectPath.Length);        //プロジェクトフォルダー内のファイルであれば短縮する
                }
            }
        }
        private string iconFileName = "";

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("ビットマップやJPEGイメージなど、透過情報がない画像に対する標準の透明色を指定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public Color StandardTransparentColor {
            get; set;
        } = Color.White;

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int intStandardTransparentColor {
            get {
                return this.StandardTransparentColor.ToArgb();
            }
            set {
                this.StandardTransparentColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("画像を拡大/縮小して描画するときの補間方法です。\r\nニアレストネイバー法(最近傍補間)はドットが荒くなりますがくっきり見え、バイリニア法(線形補間)は滑らかになりますがぼやけて見えます。")]
        [System.Xml.Serialization.XmlIgnore]
        public Media.DXInterpolation DrawInterpolation {
            get; set;
        } = Media.DXInterpolation.Nearest;

        /// <summary>
        /// 列挙体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int intDrawInterpolation {
            get {
                return (int) this.DrawInterpolation;
            }
            set {
                this.DrawInterpolation = (Media.DXInterpolation) value;
            }
        }

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("エフェクトの素材読み込みを非同期で行うかどうかを設定します。\r\n無効にすると、表示前に若干のタイムラグが生じますが、コマ落ちすることなく再生できます。")]
        public bool boolEffectAsyncLoadMode {
            get; set;
        } = false;

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("テストプレイ中、画面上にリソース情報を描画するかどうかを設定します。ゲーム発行後は常に無効となります。")]
        public bool boolEnabledSystemResource {
            get; set;
        } = true;

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("テストプレイ中にスクリプトのランタイムエラーが発生したとき、無理に処理を継続して起こる異常終了を避けるために、\r\nその場でゲームを終了するかどうかを設定します。ゲーム発行後は常に有効となります。")]
        public bool boolEnabledLimitedDebug {
            get; set;
        } = true;

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("エフェクトのフレームレートを設定します。\r\n60の約数のみ有効です。")]
        public int intEffectFPS {
            get {
                return this.effectFPS;
            }
            set {
                if (value <= 1) {
                    throw new Exception("値を１以下にすることはできません。");
                } else if (60 % value == 0) {
                    this.effectFPS = value;
                    return;
                }
                throw new Exception("値は60の約数のみ有効です。");
            }
        }
        private int effectFPS = 60;

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("マップ影層の描画透明度です。0=完全不透明、255=完全透明")]
        public int intShadowTransparent {
            get {
                return this.shadowTransparent;
            }
            set {
                if (value < 0 || 255 < value) {
                    throw new Exception("透明度は 0～255 の間でなければなりません。");
                } else {
                    this.shadowTransparent = value;
                }
            }
        }
        private int shadowTransparent = 80;

        [System.ComponentModel.Category("ゲームシステム")]
        [System.ComponentModel.Description("マップフォグの描画透明度です。0=完全不透明、255=完全透明")]
        public int intFogTransparent {
            get {
                return this.fogTransparent;
            }
            set {
                if (value < 0 || 255 < value) {
                    throw new Exception("透明度は 0～255 の間でなければなりません。");
                } else {
                    this.fogTransparent = value;
                }
            }
        }
        private int fogTransparent = 50;

        [System.ComponentModel.Category("サイズ")]
        [System.ComponentModel.Description("マップタイルの１辺あたりの長さです。")]
        public int intTileSize {
            get {
                return this.tileSize;
            }
            set {
                if (value < 1) {
                    throw new Exception("サイズは 1 以上でなければなりません。");
                } else {
                    this.tileSize = value;
                }
            }
        }
        private int tileSize = 32;

        [System.ComponentModel.Category("サイズ")]
        [System.ComponentModel.Description("アイコンの１辺あたりの長さです。")]
        public int intIconSize {
            get {
                return this.iconSize;
            }
            set {
                if (value < 1) {
                    throw new Exception("サイズは 1 以上でなければなりません。");
                } else {
                    this.iconSize = value;
                }
            }
        }
        private int iconSize = 24;

        [System.ComponentModel.Category("上限値")]
        [System.ComponentModel.Description("パーティ編成の最大人数です。")]
        public int intMaxPartyCount {
            get {
                return this.maxPartyCount;
            }
            set {
                if (value < 1) {
                    throw new Exception("上限値は 1 以上でなければなりません。");
                } else {
                    this.maxPartyCount = value;
                }
            }
        }
        private int maxPartyCount = 4;

        [System.ComponentModel.Category("上限値")]
        [System.ComponentModel.Description("レベルの最大値です。")]
        public int intMaxLevel {
            get {
                return this.maxLevel;
            }
            set {
                if (value < 1) {
                    throw new Exception("上限値は 1 以上でなければなりません。");
                } else {
                    this.maxLevel = value;
                }
            }
        }
        private int maxLevel = 99;

        [System.ComponentModel.Category("上限値")]
        [System.ComponentModel.Description("セーブデータの最大数です。")]
        public int intMaxSaveDataCount {
            get {
                return this.maxSaveDataCount;
            }
            set {
                if (value < 1) {
                    throw new Exception("上限値は 1 以上でなければなりません。");
                } else {
                    this.maxSaveDataCount = value;
                }
            }
        }
        private int maxSaveDataCount = 10;

        [System.ComponentModel.Category("エディター設定")]
        [System.ComponentModel.Description("ユニットデータベースにおける各エネミーの強さを示す指標の計算式です。\r\n[$ID]を含めると、そこにエネミーのパラメーターが代入されます。")]
        public string edtEnemyLevelExpression {
            get; set;
        } = "";

        [System.ComponentModel.Category("エディター設定")]
        [System.ComponentModel.Description("パーティキャラデータベースにおける歩行グラフィックの表示範囲です。")]
        public Rectangle edtDBCharGraphicsViewRange {
            get {
                return this.dBCharGraphicsViewRange;
            }
            set {
                if (value.Size.Width == 0 || value.Size.Height == 0) {
                    throw new Exception("設定値は 幅、高さともに 1 以上でなければなりません。");
                }
                this.dBCharGraphicsViewRange = value;
            }
        }
        private Rectangle dBCharGraphicsViewRange = new Rectangle(0, 0, 64, 64);
    }
}
