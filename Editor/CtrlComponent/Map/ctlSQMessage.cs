using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Map {
    using Module.DB;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBItem = KeyValuePair<string, int>;

    public partial class ctlSQMessage : UserControl {
        private AllDB dbList;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public ctlSQMessage() {
            this.InitializeComponent();
        }

        /// <summary>
        /// データベースのインスタンスをセットする
        /// </summary>
        public void SetDBList(AllDB DBList) {
            this.dbList = DBList;
            Database.CreateComboBoxListFromSrcDB(this.cmbFaceMsg, this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Face], true);
        }

        /// <summary>
        /// 入力情報をセットします。
        /// </summary>
        public void Setup(int faceID, string speaker, string msg) {
            //フェイスID
            var idName = Database.GetIDNameFromFixedID(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Face], faceID);
            foreach (DBItem item in this.cmbFaceMsg.Items) {
                if (idName == item.Key) {
                    this.cmbFaceMsg.SelectedItem = item;
                }
            }

            //話者名
            this.txtSpeakerMsg.Text = speaker;

            //文章
            this.txtMessageMsg.Text = msg
                .Replace("\\f", "\\f\r\n")      //改頁しているところは編集上では改行する
                .Replace("\\n", "\r\n");        //改行はCRLFへ
        }

        /// <summary>
        /// 操作結果：フェイスID
        /// </summary>
        public string ResultFaceID {
            get {
                return Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Face], ((DBItem) this.cmbFaceMsg.SelectedItem).Key);
            }
        }

        /// <summary>
        /// 操作結果：話者名
        /// </summary>
        public string ResultSpeakerName {
            get {
                return "\"" + this.txtSpeakerMsg.Text + "\"";
            }
        }

        /// <summary>
        /// 操作結果：文章
        /// </summary>
        public string ResultMessage {
            get {
                //末尾の改行記号を削除
                while (this.txtMessageMsg.Text.Length > 0 && this.txtMessageMsg.Text.Substring(this.txtMessageMsg.Text.Length - 2, 2) == "\r\n") {
                    this.txtMessageMsg.Text = this.txtMessageMsg.Text.Substring(0, this.txtMessageMsg.Text.Length - 2);
                }

                return "\"" + this.txtMessageMsg.Text
                    .Replace("\r", "")          //CRは削除
                    .Replace("\\f\n", "\\f")    //改頁して改行している部分は改頁だけにする
                    .Replace("\n", "\\n")       //改行記号は文字列として含める
                    + "\"";
            }
        }

        /// <summary>
        /// 村人：変数挿入
        /// </summary>
        private void btnInsertValueMsg_Click(object sender, EventArgs e) {
            var Dlg = new Dialog.Text.Script.dlgInsertDB(this.dbList, false, true);
            if (Dlg.ShowDialog() == DialogResult.OK) {
                this.txtMessageMsg.SelectedText = "{" + Dlg.Result + "}";
            }
        }

        /// <summary>
        /// 村人：改頁
        /// </summary>
        private void btnNewPageMsg_Click(object sender, EventArgs e) {
            this.txtMessageMsg.SelectedText = "\\f";
        }

        /// <summary>
        /// 村人：中断
        /// </summary>
        private void btnBreakMsg_Click(object sender, EventArgs e) {
            this.txtMessageMsg.SelectedText = "\\a";
        }
    }
}
