using System.IO;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;

    public partial class ctlDBSystem : ctlDBBase {
        private DBProperty property;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBSystem() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //内部データベースの初期化
            this.property = new DBProperty();
            this.initInnerDB();
            this.SupportedRowTools = false;
            this.DB.SelectedObject = this.property;
        }

        /// <summary>
        /// 内部データベースの初期化を行います。
        /// </summary>
        public void initInnerDB() {
            var obj = new DataGridView();
            for (var i = 0; i < Common.GetEnumCount<Database.DBSystemIndices>(); i++) {
                obj.Columns.Add(((Database.DBSystemIndices) i).ToString(), ((Database.DBSystemIndices) i).ToString());
            }
            obj.AllowUserToAddRows = false;

            this.DBs = new DataGridView[] { obj };
            this.property.DB = obj;
            this.property.SetDB();
        }

        public override bool DoSave() {
            if (!this.SaveXML(ProjectManager.ProjectPath + this.FileName)) {
                return false;
            }
            this.IsDirty = false;
            return true;
        }

        public override void DoLoad() {
            ///読み込みが正常に行われなかった場合は保存フラグを立てる
            this.NewCreated = !this.LoadXML(ProjectManager.ProjectPath + this.FileName);
        }

        public override DialogResult Import() {
            var result = false;
            this.dlgImporter.Filter = $"|{Path.GetFileName(this.FileName)}";

            if (this.dlgImporter.ShowDialog() == DialogResult.OK) {
                result = this.LoadXML(this.dlgImporter.FileName);
            } else {
                return DialogResult.Cancel;
            }

            return result ? DialogResult.OK : DialogResult.No;
        }

        public override DialogResult Export() {
            var result = false;
            this.dlgExporter.Filter = $"|{Path.GetFileName(this.FileName)}";
            this.dlgExporter.FileName = Path.GetFileName(this.FileName);
            this.dlgExporter.DefaultExt = "xml";

            if (this.dlgExporter.ShowDialog() == DialogResult.OK) {
                result = this.SaveXML(this.dlgExporter.FileName);
            } else {
                return DialogResult.Cancel;
            }

            return result ? DialogResult.OK : DialogResult.No;
        }

        /// <summary>
        /// XMLファイルからデータを読み込みます。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        private bool LoadXML(string fullPath) {
            if (File.Exists(fullPath)) {
                try {
                    var serializer = new XmlSerializer(typeof(DBProperty));
                    using (var R = new StreamReader(Common.OpenFileReadOnly(fullPath), Common.SJIS)) {
                        this.property = serializer.Deserialize(R) as DBProperty;
                    }
                    this.initInnerDB();
                } catch {
                    return false;
                }
                this.DB.SelectedObject = this.property;
                return true;
            } else {
                this.DB.SelectedObject = new DBProperty();
                return false;
            }
        }

        /// <summary>
        /// 現在のデータをXMLファイルに書き出します。
        /// </summary>
        /// <returns>成功したかどうか</returns>
        private bool SaveXML(string fullPath) {
            try {
                var serializer = new XmlSerializer(typeof(DBProperty));
                using (var W = new StreamWriter(Common.OpenFileWriteOnly(fullPath), Common.SJIS)) {
                    serializer.Serialize(W, this.property);
                }
            } catch {
                return false;
            }
            this.DB.SelectedObject = this.property;
            return true;
        }

        /// <summary>
        /// プロパティが変更されたら、内部DBに変更を反映します。
        /// </summary>
        private void DB_PropertyValueChanged(object s, PropertyValueChangedEventArgs e) {
            this.IsDirty = true;
            this.property.SetDB();
        }
    }
}
