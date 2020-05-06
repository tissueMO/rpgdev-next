using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace Editor.Dialog.Common {
    public partial class dlgVersion : Form {
        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgVersion() {
            this.InitializeComponent();
            this.Text = $"{this.AssemblyProduct} のバージョン情報";
            this.labelProductName.Text = this.AssemblyProduct;
            this.labelVersion.Text = $"バージョン {this.AssemblyVersion}";
            this.labelCopyright.Text = this.AssemblyCopyright;
            this.labelCompanyName.Text = this.AssemblyCompany;
            this.textBoxDescription.Text = Properties.Resources.License;
        }

        #region <アセンブリ情報へのアクセサー>
        public string AssemblyTitle {
            get {
                var attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyTitleAttribute), false);
                if (attributes.Length > 0) {
                    var titleAttribute = (AssemblyTitleAttribute) attributes[0];
                    if (titleAttribute.Title != "") {
                        return titleAttribute.Title;
                    }
                }
                return Path.GetFileNameWithoutExtension(Assembly.GetExecutingAssembly().CodeBase);
            }
        }

        public string AssemblyVersion {
            get {
                return Assembly.GetExecutingAssembly().GetName().Version.ToString();
            }
        }

        public string AssemblyDescription {
            get {
                var attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyDescriptionAttribute), false);
                return (attributes.Length > 0) ? ((AssemblyDescriptionAttribute) attributes[0]).Description : "";
            }
        }

        public string AssemblyProduct {
            get {
                var attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyProductAttribute), false);
                return (attributes.Length > 0) ? ((AssemblyProductAttribute) attributes[0]).Product : "";
            }
        }

        public string AssemblyCopyright {
            get {
                var attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyCopyrightAttribute), false);
                return (attributes.Length > 0) ? ((AssemblyCopyrightAttribute) attributes[0]).Copyright : "";
            }
        }

        public string AssemblyCompany {
            get {
                var attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyCompanyAttribute), false);
                return (attributes.Length > 0) ? ((AssemblyCompanyAttribute) attributes[0]).Company : "";
            }
        }
        #endregion
    }
}
