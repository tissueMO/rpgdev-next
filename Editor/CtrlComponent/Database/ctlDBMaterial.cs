using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;

    public partial class ctlDBMaterial : ctlDBBase {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBMaterial() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //各データベースのバインド
            var list = new List<DataGridView>();
            for (var i = 0; i < Common.GetEnumCount<Database.DBMaterialIndices>(); i++) {
                switch (i) {
                    case (int) Database.DBMaterialIndices.Face:
                        list.Add(this.DBFaces);
                        break;
                    case (int) Database.DBMaterialIndices.Font:
                        list.Add(this.DBFonts);
                        break;
                    case (int) Database.DBMaterialIndices.Graphics:
                        list.Add(this.DBGraphics);
                        break;
                    case (int) Database.DBMaterialIndices.Sound:
                        list.Add(this.DBSounds);
                        break;
                }
            }
            this.DBs = list.ToArray();
        }

        public override DataGridView CurrentSubDB {
            get {
                if (this.ActiveControl is DataGridView) {
                    return this.ActiveControl as DataGridView;
                }
                return null;
            }
        }
    }
}
