using System;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    public partial class dlgEditSettings : Form {
        public event EventHandler Exporting;
        public event EventHandler Importing;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEditSettings(object targetInctance, bool enableIO) {
            this.InitializeComponent();
            this.SetTargetObject(targetInctance);
            this.btnImport.Visible = enableIO;
            this.btnExport.Visible = enableIO;
        }

        /// <summary>
        /// 設定対象のオブジェクトを設定する
        /// </summary>
        public void SetTargetObject(object targetInctance) {
            this.SettingsCtrl.SelectedObject = targetInctance;
        }

        /// <summary>
        /// プロパティ変更時
        /// </summary>
        private void settingsCtrl_PropertyValueChanged(object s, PropertyValueChangedEventArgs e) {
            this.SettingsCtrl.Refresh();
        }

        /// <summary>
        /// OKボタン
        /// </summary>
        private void btnOK_Click(object sender, EventArgs e) {
            this.DialogResult = DialogResult.OK;
        }

        /// <summary>
        /// インポート
        /// ここで呼ばれるイベントハンドラーの中で、設定対象のオブジェクトを再設定して下さい。
        /// </summary>
        private void btnImport_Click(object sender, EventArgs e) {
            this.Importing?.Invoke(this.SettingsCtrl.SelectedObject, null);
        }

        /// <summary>
        /// エクスポート
        /// </summary>
        private void btnExport_Click(object sender, EventArgs e) {
            this.Exporting?.Invoke(this.SettingsCtrl.SelectedObject, null);
        }
    }
}
