using System;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    using Properties;

    public partial class dlgManual : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgManual() {
            this.InitializeComponent();
            this.tolHome_Click(null, null);     //既定でホームへ移動する
        }

        /// <summary>
        /// ウィンドウ表示時
        /// </summary>
        private void dlgManual_Load(object sender, EventArgs e) {
            //ウィンドウ設定復元
            this.Location = Settings.Default.Help_WindowLocation;
            this.Size = Settings.Default.Help_WindowSize;
            this.WindowState = Settings.Default.Help_WindowState;
        }

        /// <summary>
        /// ウィンドウ消去時
        /// </summary>
        private void dlgManual_FormClosing(object sender, FormClosingEventArgs e) {
            //ウィンドウ設定保存
            if (this.WindowState != FormWindowState.Minimized) {
                Settings.Default.Help_WindowLocation = this.Location;
                Settings.Default.Help_WindowSize = this.Size;
                Settings.Default.Help_WindowState = this.WindowState;
            }
        }

        /// <summary>
        /// 前のページに戻る
        /// </summary>
        private void tolPreviousPage_Click(object sender, EventArgs e) {
            this.browser.GoBack();
        }

        /// <summary>
        /// 次のページに進む
        /// </summary>
        private void tolNextPage_Click(object sender, EventArgs e) {
            this.browser.GoForward();
        }

        /// <summary>
        /// ホームに移動する
        /// </summary>
        private void tolHome_Click(object sender, EventArgs e) {
            //Windows形式のパスからURIに変換する
            var manualURI = new Uri(Application.StartupPath + "\\" + Resources.Path_Manual);
            this.browser.Navigate(manualURI);

            //ブラウザバージョンを検証するモードにするには、以下をコメント解除すること
            //browser.Navigate("http://www.useragentstring.com/");
        }

        /// <summary>
        /// ボタンの有効/無効を更新
        /// </summary>
        private void browser_Navigated(object sender, WebBrowserNavigatedEventArgs e) {
            this.tolPreviousPage.Enabled = this.browser.CanGoBack;
            this.tolNextPage.Enabled = this.browser.CanGoForward;
        }
    }
}
