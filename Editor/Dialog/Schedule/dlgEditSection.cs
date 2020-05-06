using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Schedule {
    using Module;
    using Properties;

    public partial class dlgEditSection : Form {
        private const int ImgIndexSection = 0;
        private const int ImgIndexWorker = 1;
        private readonly List<CtrlComponent.Schedule.mgrSchedule.SectionData> srcSections;      //参照先の部門リスト

        /// <summary>
        /// コンストラクター
        /// </summary>
        /// <param name="sections">セクションリストへの参照</param>
        public dlgEditSection(List<CtrlComponent.Schedule.mgrSchedule.SectionData> sections) {
            this.InitializeComponent();

            //引数に従ってセットアップする
            this.trvSection.Nodes.Clear();
            this.srcSections = sections;

            foreach (var sec in this.srcSections) {
                var node = new TreeNode();
                Common.SetTreeNodeImageIndex(node, ImgIndexSection);
                node.Text = sec.Name;
                node.Tag = sec.ID;

                foreach (var worker in sec.Workers) {
                    var sub = new TreeNode();
                    Common.SetTreeNodeImageIndex(sub, ImgIndexWorker);
                    sub.Text = worker.Name;
                    sub.Tag = worker.ID;
                    node.Nodes.Add(sub);
                }

                this.trvSection.Nodes.Add(node);
            }

            this.trvSection.ExpandAll();
        }

        /// <summary>
        /// ダイアログの編集結果
        /// </summary>
        public List<CtrlComponent.Schedule.mgrSchedule.SectionData> Result {
            get {
                return this.srcSections;
            }
        }

        /// <summary>
        /// 一意のIDを生成します。
        /// </summary>
        private int generateID(TreeNodeCollection nodes) {
            var ID = -1;
            bool NG;

            do {
                ID++;
                NG = false;
                foreach (TreeNode item in nodes) {
                    if (int.Parse(item.Tag.ToString()) == ID) {
                        NG = true;
                        break;
                    }
                }
            } while (NG);

            return ID;
        }

        /// <summary>
        /// 部門を追加
        /// </summary>
        private void tolSection_Click(object sender, EventArgs e) {
            var temp = new TreeNode();
            Common.SetTreeNodeImageIndex(temp, ImgIndexSection);
            temp.Text = "新しい部門";
            temp.Tag = this.generateID(this.trvSection.Nodes);
            this.trvSection.Nodes.Add(temp);
            this.trvSection.SelectedNode = temp;
            this.trvSection.SelectedNode.BeginEdit();
        }

        /// <summary>
        /// 担当者を追加
        /// </summary>
        private void tolWorker_Click(object sender, EventArgs e) {
            if (this.trvSection.SelectedNode != null) {
                //既に担当者を選択している場合は部門にさかのぼる
                if (this.trvSection.SelectedNode.Parent != null) {
                    this.trvSection.SelectedNode = this.trvSection.SelectedNode.Parent;
                }
                var temp = new TreeNode();
                Common.SetTreeNodeImageIndex(temp, ImgIndexWorker);
                temp.Text = "新しい担当者";
                temp.Tag = this.generateID(this.trvSection.SelectedNode.Nodes);
                this.trvSection.SelectedNode.Nodes.Add(temp);
                this.trvSection.SelectedNode = temp;
                this.trvSection.SelectedNode.BeginEdit();
            }
        }

        /// <summary>
        /// 削除
        /// </summary>
        private void tolDeleteTask_Click(object sender, EventArgs e) {
            if (this.trvSection.SelectedNode != null) {
                if (MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "選択された項目を削除"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) == DialogResult.No) {
                    return;
                }
                this.trvSection.SelectedNode.Remove();
                this.trvSection.Focus();

                if (this.trvSection.Nodes.Count == 0) {
                    this.trvSection_AfterSelect(sender, null);
                }
            }
        }

        /// <summary>
        /// 上へ
        /// </summary>
        private void tolUp_Click(object sender, EventArgs e) {
            if (this.trvSection.SelectedNode != null && this.trvSection.SelectedNode.PrevNode != null) {
                //前後でIDを入れ替える
                var parent = (this.trvSection.SelectedNode.Parent == null) ? this.trvSection.Nodes : this.trvSection.SelectedNode.Parent.Nodes;
                parent.Insert(this.trvSection.SelectedNode.Index + 1, this.trvSection.SelectedNode.PrevNode.Clone() as TreeNode);
                this.trvSection.SelectedNode.PrevNode.Remove();
                this.trvSection.ExpandAll();
                this.checkEnabledUpDown(this.trvSection.SelectedNode);
            }
        }

        /// <summary>
        /// 下へ
        /// </summary>
        private void tolDown_Click(object sender, EventArgs e) {
            if (this.trvSection.SelectedNode != null && this.trvSection.SelectedNode.NextNode != null) {
                //前後でIDを入れ替える
                var parent = (this.trvSection.SelectedNode.Parent == null) ? this.trvSection.Nodes : this.trvSection.SelectedNode.Parent.Nodes;
                parent.Insert(this.trvSection.SelectedNode.Index, this.trvSection.SelectedNode.NextNode.Clone() as TreeNode);
                this.trvSection.SelectedNode.NextNode.Remove();
                this.trvSection.ExpandAll();
                this.checkEnabledUpDown(this.trvSection.SelectedNode);
            }
        }

        /// <summary>
        /// セクション選択
        /// </summary>
        private void trvSection_AfterSelect(object sender, TreeViewEventArgs e) {
            this.tolDelete.Enabled = (e?.Node != null);
            this.tolWorker.Enabled = this.tolDelete.Enabled;
            this.checkEnabledUpDown(e?.Node);
        }

        /// <summary>
        /// 上・下のコントロールの有効無効を確認します。
        /// </summary>
        /// <param name="target">選択されているノード</param>
        private void checkEnabledUpDown(TreeNode target) {
            this.tolUp.Enabled = (this.tolDelete.Enabled && target?.PrevNode != null);
            this.tolDown.Enabled = (this.tolDelete.Enabled && target?.NextNode != null);
        }

        /// <summary>
        /// セクション項目編集前
        /// </summary>
        private void trvSection_BeforeLabelEdit(object sender, NodeLabelEditEventArgs e) {
            this.tlbToolbar.Enabled = false;
        }

        /// <summary>
        /// セクション項目編集後
        /// </summary>
        private void trvSection_AfterLabelEdit(object sender, NodeLabelEditEventArgs e) {
            //無名チェック
            this.tlbToolbar.Enabled = true;
            if (e.Node == null) {
                //ESCなどで強制解除された場合はサイレントスルー
                return;
            } else if (string.IsNullOrEmpty(e.Label)) {
                e.CancelEdit = true;
                return;
            }

            //重複チェック
            if (e.Node.Parent == null) {
                //部門同士の重複
                foreach (TreeNode node in this.trvSection.Nodes) {
                    if (node != e.Node && node.Text == e.Label) {
                        e.CancelEdit = true;
                        MessageBox.Show(Resources.MsgE_ForbidOverlap.Replace("$", "部門名"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        break;
                    }
                }
            } else {
                //同一部門内での担当者同士の重複
                foreach (TreeNode node in e.Node.Parent.Nodes) {
                    if (node != e.Node && node.Text == e.Label) {
                        e.CancelEdit = true;
                        MessageBox.Show(Resources.MsgE_ForbidOverlap.Replace("$", "同一部門内で担当者名"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        break;
                    }
                }
            }
        }

        /// <summary>
        /// 結果を返す
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            //セクションリストに反映させるが、このときの並び順はTreeView上の表示に従い、この順番がソートにおける基準となる
            this.srcSections.Clear();

            foreach (TreeNode node in this.trvSection.Nodes) {
                var sec = new CtrlComponent.Schedule.mgrSchedule.SectionData {
                    Name = node.Text,
                    ID = int.Parse(node.Tag.ToString()),
                    Workers = new List<CtrlComponent.Schedule.mgrSchedule.SectionData.WorkerData>()
                };

                foreach (TreeNode sub in node.Nodes) {
                    var add = new CtrlComponent.Schedule.mgrSchedule.SectionData.WorkerData {
                        ID = int.Parse(sub.Tag.ToString()),
                        Name = sub.Text
                    };
                    sec.Workers.Add(add);
                }

                this.srcSections.Add(sec);
            }
        }
    }
}
