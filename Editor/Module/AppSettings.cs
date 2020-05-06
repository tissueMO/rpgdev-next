namespace Editor.Module {
    using Properties;
    using System;
    using System.ComponentModel;
    using System.Drawing;
    using System.IO;
    using System.Windows.Forms;
    using System.Xml.Serialization;

    /// <summary>
    /// アプリケーションの設定項目をGUI上で編集するためのクラスです
    /// </summary>
    [Serializable()]
    public class AppSettings {

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("テキストエディターの背景色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Text_BackColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Text_BackColor {
            get {
                return this.Text_BackColor.ToArgb();
            }
            set {
                this.Text_BackColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("カーソル行にアンダーラインを表示します。")]
        public bool Text_UnderBar {
            get; set;
        }

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("カーソル行アンダーラインの色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Text_UnderBarColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Text_UnderBarColor {
            get {
                return this.Text_UnderBarColor.ToArgb();
            }
            set {
                this.Text_UnderBarColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("[/*コメント部分*/] および [//コメント行] の色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Text_CommentColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Text_CommentColor {
            get {
                return this.Text_CommentColor.ToArgb();
            }
            set {
                this.Text_CommentColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("テキストエディター（スクリプトを除く）のフォントを設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Font Text_Font {
            get; set;
        }

        /// <summary>
        /// Fontクラスはシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public string _Text_Font {
            get {
                return AppSettings.FontToString(this.Text_Font);
            }
            set {
                this.Text_Font = AppSettings.StringToFont(value);
            }
        }

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("テキストエディターの文字色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Text_TextColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Text_TextColor {
            get {
                return this.Text_TextColor.ToArgb();
            }
            set {
                this.Text_TextColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-1. テキストエディター")]
        [System.ComponentModel.Description("ジャンプリストに該当させる行頭文字列を設定します。")]
        public string Text_JumpChar {
            get; set;
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプトを開いたとき、自動でコード解析を実行するかどうかを設定します。")]
        public bool Script_AutoAnalyze {
            get; set;
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプトエディター（英文フォントのみ有効）のフォントを設定します。\r\n日本語フォントはテキストエディターのフォントが使われます。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Font Script_Font {
            get; set;
        }

        /// <summary>
        /// Fontクラスはシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public string _Script_Font {
            get {
                return AppSettings.FontToString(this.Script_Font);
            }
            set {
                this.Script_Font = AppSettings.StringToFont(value);
            }
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプトの予約語の色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Script_KeywordColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Script_KeywordColor {
            get {
                return this.Script_KeywordColor.ToArgb();
            }
            set {
                this.Script_KeywordColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプトの識別子（クラス・列挙体・プロパティ）の色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Script_NameColor1 {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Script_NameColor1 {
            get {
                return this.Script_NameColor1.ToArgb();
            }
            set {
                this.Script_NameColor1 = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプトの識別子（変数・関数・定数・列挙体メンバー）の色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Script_NameColor2 {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Script_NameColor2 {
            get {
                return this.Script_NameColor2.ToArgb();
            }
            set {
                this.Script_NameColor2 = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプト上の数値の色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Script_NumberColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Script_NumberColor {
            get {
                return this.Script_NumberColor.ToArgb();
            }
            set {
                this.Script_NumberColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("スクリプト上の \"文字列\" の色を設定します。")]
        [System.Xml.Serialization.XmlIgnore]
        public System.Drawing.Color Script_StringColor {
            get; set;
        }

        /// <summary>
        /// Color構造体はシリアル化できないため、このプロパティを経由して保存を行う
        /// </summary>
        [Browsable(false)]
        public int _Script_StringColor {
            get {
                return this.Script_StringColor.ToArgb();
            }
            set {
                this.Script_StringColor = Color.FromArgb(value);
            }
        }

        [System.ComponentModel.Category("1-2. スクリプトエディター")]
        [System.ComponentModel.Description("データベース要素のFixedIDをスクリプトに挿入するとき、他の数値と区別するために16進数で表記するかどうかを設定します。\r\nデータベースでFixedID列を表示しない設定にしている場合は有効にすることを推奨します。")]
        [System.ComponentModel.ReadOnly(true)]
        public bool Script_FixedIDHexMode {
            get; set;
        } = true;

        [System.ComponentModel.Category("2. データベースエディター")]
        [System.ComponentModel.Description("一意のIDとして設定されるFixedID列を非表示にするかどうかを設定します。\r\nこれは次回起動時から反映されます。")]
        public bool DB_VisibleFixedID {
            get; set;
        }

        [System.ComponentModel.Category("3. マップエディター")]
        [System.ComponentModel.Description("非アクティブレイヤーの透明度を設定します。")]
        public int Map_NonActiveLayerTransparency {
            get; set;
        }

        [System.ComponentModel.Category("3. マップエディター")]
        [System.ComponentModel.Description("イベントのスクリプト編集スタイルを簡易編集モードに切り替えます。")]
        [System.ComponentModel.ReadOnly(true)]
        public bool Map_SQEasyMode {
            get; set;
        }

        [System.ComponentModel.Category("3. マップエディター")]
        [System.ComponentModel.Description("マップテスターを起動している間、エディターの動作を停止するかどうかを設定します。\r\nスペックが低く、処理落ちする場合は有効にしてください。")]
        public bool Map_WaitForTest {
            get; set;
        } = false;

        //このクラスが扱うダイアログ
        private static readonly SaveFileDialog dlgExport = new SaveFileDialog();
        private static readonly OpenFileDialog dlgImport = new OpenFileDialog();

        /// <summary>
        /// コンストラクター
        /// 設定情報から読み込みます。
        /// </summary>
        public AppSettings() {
            this.Text_BackColor = Settings.Default.Text_BackColor;
            this.Text_CommentColor = Settings.Default.Text_CommentColor;
            this.Text_Font = Settings.Default.Text_Font;
            this.Text_TextColor = Settings.Default.Text_TextColor;
            this.Text_UnderBar = Settings.Default.Text_UnderBar;
            this.Text_UnderBarColor = Settings.Default.Text_UnderBarColor;
            this.Text_JumpChar = Settings.Default.Text_JumpChar;
            this.Script_AutoAnalyze = Settings.Default.Script_AutoAnalyze;
            this.Script_Font = Settings.Default.Script_Font;
            this.Script_KeywordColor = Settings.Default.Script_KeywordColor;
            this.Script_NameColor1 = Settings.Default.Script_NameColor1;
            this.Script_NameColor2 = Settings.Default.Script_NameColor2;
            this.Script_NumberColor = Settings.Default.Script_NumberColor;
            this.Script_StringColor = Settings.Default.Script_StringColor;
            this.Script_FixedIDHexMode = Settings.Default.Script_FixedIDHexMode;
            this.DB_VisibleFixedID = Settings.Default.DB_VisibleFixedID;
            this.Map_NonActiveLayerTransparency = Settings.Default.Map_NonActiveLayerTransparency;
            this.Map_SQEasyMode = Settings.Default.Map_SQEasyMode;
            this.Map_WaitForTest = Settings.Default.Map_WaitForTest;

            //ダイアログ初期化
            AppSettings.dlgExport.AddExtension = AppSettings.dlgExport.AddExtension = true;
            AppSettings.dlgExport.CheckFileExists = false;
            AppSettings.dlgImport.CheckFileExists = true;
            AppSettings.dlgExport.DefaultExt = AppSettings.dlgImport.DefaultExt = "xml";
            AppSettings.dlgExport.Filter = AppSettings.dlgImport.Filter = "設定情報ファイル (*.xml)|*.xml";
            AppSettings.dlgExport.Title = "設定情報のエクスポート";
            AppSettings.dlgImport.Title = "設定情報のインポート";
        }

        /// <summary>
        /// 設定情報に書き込みます。
        /// </summary>
        public void Apply() {
            Settings.Default.Text_BackColor = this.Text_BackColor;
            Settings.Default.Text_CommentColor = this.Text_CommentColor;
            Settings.Default.Text_Font = this.Text_Font;
            Settings.Default.Text_UnderBar = this.Text_UnderBar;
            Settings.Default.Text_UnderBarColor = this.Text_UnderBarColor;
            Settings.Default.Text_TextColor = this.Text_TextColor;
            Settings.Default.Text_JumpChar = this.Text_JumpChar;
            Settings.Default.Script_Font = this.Script_Font;
            Settings.Default.Script_AutoAnalyze = this.Script_AutoAnalyze;
            Settings.Default.Script_KeywordColor = this.Script_KeywordColor;
            Settings.Default.Script_NameColor1 = this.Script_NameColor1;
            Settings.Default.Script_NameColor2 = this.Script_NameColor2;
            Settings.Default.Script_NumberColor = this.Script_NumberColor;
            Settings.Default.Script_StringColor = this.Script_StringColor;
            Settings.Default.Script_FixedIDHexMode = this.Script_FixedIDHexMode;
            Settings.Default.DB_VisibleFixedID = this.DB_VisibleFixedID;
            Settings.Default.Map_NonActiveLayerTransparency = this.Map_NonActiveLayerTransparency;
            Settings.Default.Map_SQEasyMode = this.Map_SQEasyMode;
            Settings.Default.Map_WaitForTest = this.Map_WaitForTest;
            Settings.Default.Save();
        }

        /// <summary>
        /// 現在の設定情報を外部にエクスポートします。
        /// </summary>
        public void Export() {
            if (AppSettings.dlgExport.ShowDialog() != DialogResult.OK) {
                return;
            }

            try {
                var serializer = new XmlSerializer(typeof(AppSettings));
                using (var W = new StreamWriter(Common.OpenFileWriteOnly(AppSettings.dlgExport.FileName), Common.SJIS)) {
                    serializer.Serialize(W, this);
                }
                MessageBox.Show("設定情報をエクスポートしました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } catch {
                MessageBox.Show("設定情報のエクスポートに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// 外部から設定情報をインポートします。
        /// </summary>
        public static void Import(ref AppSettings obj) {
            if (AppSettings.dlgImport.ShowDialog() != DialogResult.OK) {
                return;
            }

            try {
                var serializer = new XmlSerializer(typeof(AppSettings));
                using (var R = new StreamReader(Common.OpenFileReadOnly(AppSettings.dlgImport.FileName), Common.SJIS)) {
                    obj = serializer.Deserialize(R) as AppSettings;
                }
                MessageBox.Show("設定情報をインポートしました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            } catch {
                MessageBox.Show("設定情報のインポートに失敗しました。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// フォントを文字列にします。
        /// </summary>
        private static string FontToString(Font val) {
            var cvt = new FontConverter();
            return cvt.ConvertToString(val);
        }

        /// <summary>
        /// 文字列からフォントにします。
        /// </summary>
        private static Font StringToFont(string val) {
            var cvt = new FontConverter();
            return cvt.ConvertFromString(val) as Font;
        }
    }
}
