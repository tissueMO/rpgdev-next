using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;

    public partial class ctlDBValues : ctlDBBase {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlDBValues() {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);

            //各データベースのバインド
            var list = new List<DataGridView>();
            for (var i = 0; i < Common.GetEnumCount<Database.DBValueIndices>(); i++) {
                switch (i) {
                    case (int) Database.DBValueIndices.CommonFlag:
                        list.Add(this.DBFlag);
                        break;
                    case (int) Database.DBValueIndices.CommonInteger:
                        list.Add(this.DBInteger);
                        break;
                    case (int) Database.DBValueIndices.CommonString:
                        list.Add(this.DBString);
                        break;
                    case (int) Database.DBValueIndices.SelfActor:
                        list.Add(this.DBActorSelf);
                        break;
                    case (int) Database.DBValueIndices.SelfClass:
                        list.Add(this.DBClassSelf);
                        break;
                    case (int) Database.DBValueIndices.SelfUse:
                        list.Add(this.DBUseSelf);
                        break;
                    case (int) Database.DBValueIndices.SelfState:
                        list.Add(this.DBStateSelf);
                        break;
                    case (int) Database.DBValueIndices.SelfUnit:
                        list.Add(this.DBUnitSelf);
                        break;
                    case (int) Database.DBValueIndices.SelfEvent:
                        list.Add(this.DBEVSelf);
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
