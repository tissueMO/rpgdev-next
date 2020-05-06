using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Text.Script {
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class dlgEVcmdMessage : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEVcmdMessage(AllDB DBList, int faceID = -1, string speaker = "", string msg = "") {
            this.InitializeComponent();
            this.uctlSQMessage.SetDBList(DBList);
            this.uctlSQMessage.Setup(faceID, speaker, msg);
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public string Result {
            get {
                return Resources.SQ_EVMessage
                    .Replace("$1", this.uctlSQMessage.ResultFaceID)
                    .Replace("$2", this.uctlSQMessage.ResultSpeakerName)
                    .Replace("$3", this.uctlSQMessage.ResultMessage);
            }
        }
    }
}
