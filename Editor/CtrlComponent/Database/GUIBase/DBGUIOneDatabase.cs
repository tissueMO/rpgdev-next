using System;

namespace Editor.CtrlComponent.Database {
    using Module;

    public partial class ctlDBGUIBase {

        /// <summary>
        /// 内部システム機構が保持する単一データのインターフェース
        /// </summary>
        public abstract class DBGUIOneDatabase : IDirtyable {
            public event EventHandler DataChanged;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public DBGUIOneDatabase(int fixedID, int visibleID, string name) {
                this.FixedID = fixedID;
                this.VisibleID = visibleID;
                this.Name = name;
            }

            /// <summary>
            /// このデータが変更されているかどうか
            /// </summary>
            public bool IsDirty {
                get {
                    return this.isDirty;
                }
                set {
                    this.isDirty = value;
                    this.OnDataChanged();
                }
            }
            private bool isDirty = false;

            public int FixedID {
                get;
                protected set;
            } = -1;

            public virtual int VisibleID {
                get {
                    return this.visibleID;
                }
                set {
                    this.visibleID = value;
                    this.IsDirty = true;
                }
            }
            protected int visibleID = 0;

            public virtual string Name {
                get {
                    return this.name;
                }
                set {
                    this.name = value;
                    this.IsDirty = true;
                }
            }
            protected string name;

            /// <summary>
            /// 当該データをファイルに書き出します。
            /// </summary>
            public virtual bool SaveFile(string rPath = null) {
                return true;
            }

            /// <summary>
            /// ファイルから当該データを読み込みます。
            /// </summary>
            public virtual void LoadFile() {
            }

            /// <summary>
            /// このデータによるDataChangedイベントを発生させます。
            /// ただし、IsDirtyのフラグが立てられている場合にのみ実行します。
            /// </summary>
            protected void OnDataChanged() {
                if (this.IsDirty) {
                    this.DataChanged?.Invoke(this, null);
                }
            }
        }

    }
}
