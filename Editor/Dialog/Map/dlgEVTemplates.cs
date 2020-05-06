using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace Editor.Dialog.Map {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, int>;
    using EVData = CtrlComponent.Map.mgrMapObject.MapOneData.EventOneData;

    public partial class dlgEVTemplates : Form {
        private Point tilePosition;
        private readonly string MapGUID;
        private readonly CtrlComponent.Text.ctlCommandTree comTree;
        private readonly CtrlComponent.Database.ctlDatabaseEditor dbCtl;
        private readonly AllDB dbList;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgEVTemplates(EVData ev, string mapGUID, CtrlComponent.Text.ctlCommandTree comTree, CtrlComponent.Database.ctlDatabaseEditor dbCtl) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.ev = ev;
            this.tilePosition = ev.TilePosition;
            this.MapGUID = mapGUID;
            this.comTree = comTree;
            this.dbCtl = dbCtl;
            this.dbList = dbCtl.DBList;

            // デフォルトの可視ID
            this.numEVVisibleID.Value = ev.VisibleID;

            //初期設定
            //リンク
            this.uctlSQChangeMapLink.DBList = this.dbList;

            //ドア
            Database.CreateComboBoxListFromSrcDB(this.cmbKeyItemDoor, this.dbList[Database.DBIndices.Item].DBs[0], true);
            Database.CreateComboBoxListFromSrcDB(this.cmbSEDoor, this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], true);
            Database.SelectComboBoxItemFromIDName(this.cmbSEDoor, Settings.Default.Last_EVTemplate_DoorSE);
            if (this.cmbSEDoor.SelectedIndex == -1) {
                this.cmbSEDoor.SelectedIndex = 0;    //「なし」を選択
            }

            //ドア＋リンク
            Database.CreateComboBoxListFromSrcDB(this.cmbKeyItemDL, this.dbList[Database.DBIndices.Item].DBs[0], true);
            Database.CreateComboBoxListFromSrcDB(this.cmbSEDL, this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], true);
            Database.SelectComboBoxItemFromIDName(this.cmbSEDL, Settings.Default.Last_EVTemplate_DoorSE);
            this.uctlSQChangeMapDL.DBList = this.dbList;
            if (this.cmbSEDL.SelectedIndex == -1) {
                this.cmbSEDL.SelectedIndex = 0;    //「なし」を選択
            }

            //村人
            this.uctlSQMessageHuman.SetDBList(this.dbList);

            //財宝
            Database.CreateComboBoxListFromSrcDB(this.cmbOpenSETreasure, this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], true);
            Database.CreateComboBoxListFromSrcDB(this.cmbItemTreasure, this.dbList[Database.DBIndices.Item].DBs[0], false);
            Database.CreateComboBoxListFromSrcDB(this.cmbKeyItemTreasure, this.dbList[Database.DBIndices.Item].DBs[0], true);
            Database.CreateComboBoxListFromSrcDB(this.cmbSelfValueTreasure, this.dbList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.SelfEvent], false);
            Database.SelectComboBoxItemFromIDName(this.cmbOpenSETreasure, Settings.Default.Last_EVTemplate_TreasureSE);
            this.rdbMoneyTreasure.Checked = true;
            if (this.cmbOpenSETreasure.SelectedIndex == -1) {
                this.cmbOpenSETreasure.SelectedIndex = 0;    //「なし」を選択
            }

            //お店
            Database.CreateComboBoxListFromSrcDB(this.cmbFaceShop, this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Face], true);
            Database.CreateComboBoxListFromSrcDB(this.cmbItemDBShop, this.dbList[Database.DBIndices.Item].DBs[0]);
            this.uctlShopItemList.SetupList(new string[] { "商品名" }, null, true);

            //ユーザー定義
            this.reloadEVTemplates();
        }

        /// <summary>
        /// テンプレートタブの順序
        /// </summary>
        public enum TemplateIndices {
            Plain,
            Link,
            Door,
            DoorLink,
            Human,
            Treasure,
            Shop,
            UserEV,
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public EVData Result {
            get {
                return this.ev;
            }
        }
        private EVData ev;

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            switch ((TemplateIndices) this.tbcEVTemplates.SelectedIndex) {
                case TemplateIndices.Plain:
                    //コード生成を行わない
                    break;

                case TemplateIndices.Link:
                    if (this.uctlSQChangeMapLink.CheckError()) {
                        //不備がある場合は却下
                        this.DialogResult = DialogResult.None;
                    } else {
                        //コード生成を行う
                        this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.Link
                            .Replace("$1", this.uctlSQChangeMapLink.ResultMapFileName)
                            .Replace("$2", this.uctlSQChangeMapLink.ResultPosition.X.ToString())
                            .Replace("$3", this.uctlSQChangeMapLink.ResultPosition.Y.ToString())
                            .Replace("$4", this.uctlSQChangeMapLink.ResultDirection);

                        //ページ設定
                        this.ev.Pages[0].Trigger = Map.EventTriggerType.Push;
                        this.ev.Pages[0].MovingAnimation = false;
                        this.ev.Pages[0].StoppingAnimation = false;
                        this.ev.Pages[0].NoHit = true;
                    }
                    break;

                case TemplateIndices.Door:
                    //コード生成を行う
                    Settings.Default.Last_EVTemplate_DoorSE = ((DBListType) this.cmbSEDoor.SelectedItem).Key;
                    if (this.cmbKeyItemDoor.SelectedIndex > 0) {
                        //鍵あり
                        this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.DoorWithKey
                            .Replace("$1", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Item].DBs[0], ((DBListType) this.cmbKeyItemDoor.SelectedItem).Key))
                            .Replace("$2", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((DBListType) this.cmbSEDoor.SelectedItem).Key));
                    } else {
                        //鍵なし
                        this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.Door
                            .Replace("$2", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((DBListType) this.cmbSEDoor.SelectedItem).Key));
                    }

                    //ページ設定
                    this.ev.Pages[0].Trigger = Map.EventTriggerType.Push;
                    this.ev.Pages[0].Direction = Map.Direction4.East;
                    this.ev.Pages[0].Graphic = this.uctlGraphicsDoor.FileName;
                    this.ev.Pages[0].MovingAnimation = false;
                    this.ev.Pages[0].StoppingAnimation = false;
                    break;

                case TemplateIndices.DoorLink:
                    if (this.uctlSQChangeMapDL.CheckError()) {
                        //不備がある場合は却下
                        this.DialogResult = DialogResult.None;
                    } else {
                        //コード生成を行う
                        Settings.Default.Last_EVTemplate_DoorSE = ((DBListType) this.cmbSEDL.SelectedItem).Key;
                        if (this.cmbKeyItemDL.SelectedIndex > 0) {
                            //鍵あり
                            this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.DoorLinkWithKey
                                .Replace("$1", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Item].DBs[0], ((DBListType) this.cmbKeyItemDL.SelectedItem).Key))
                                .Replace("$2", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((DBListType) this.cmbSEDL.SelectedItem).Key))
                                .Replace("$3", this.uctlSQChangeMapDL.ResultMapFileName)
                                .Replace("$4", this.uctlSQChangeMapDL.ResultPosition.X.ToString())
                                .Replace("$5", this.uctlSQChangeMapDL.ResultPosition.Y.ToString());
                        } else {
                            //鍵なし
                            this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.DoorLink
                                .Replace("$2", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((DBListType) this.cmbSEDL.SelectedItem).Key))
                                .Replace("$3", this.uctlSQChangeMapDL.ResultMapFileName)
                                .Replace("$4", this.uctlSQChangeMapDL.ResultPosition.X.ToString())
                                .Replace("$5", this.uctlSQChangeMapDL.ResultPosition.Y.ToString());
                        }

                        //ページ設定
                        this.ev.Pages[0].Trigger = Map.EventTriggerType.Push;
                        this.ev.Pages[0].Direction = Map.Direction4.East;
                        this.ev.Pages[0].Graphic = this.uctlGraphicsDL.FileName;
                        this.ev.Pages[0].MovingAnimation = false;
                        this.ev.Pages[0].StoppingAnimation = false;
                    }
                    break;

                case TemplateIndices.Human:
                    //コード生成を行う
                    this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.Person
                        .Replace("$1", this.uctlSQMessageHuman.ResultFaceID)
                        .Replace("$2", this.uctlSQMessageHuman.ResultSpeakerName)
                        .Replace("$3", this.uctlSQMessageHuman.ResultMessage);

                    //ページ設定
                    this.ev.Pages[0].Trigger = Map.EventTriggerType.Push;
                    this.ev.Pages[0].MoveType = Map.EventMoveType.Random;
                    this.ev.Pages[0].Graphic = this.uctlGraphicsWalkMsg.FileName;
                    break;

                case TemplateIndices.Treasure:
                    if (this.rdbItemTreasure.Checked && this.cmbItemTreasure.SelectedIndex == -1) {
                        //中身のアイテムが選択されていない
                        MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "中身のアイテム"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        this.DialogResult = DialogResult.None;
                    } else if (this.cmbSelfValueTreasure.SelectedIndex == -1) {
                        //個別変数が選択されていない
                        MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "ページ切替を行うためのイベント個別変数"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        this.DialogResult = DialogResult.None;
                    } else {
                        //コード生成を行う
                        Settings.Default.Last_EVTemplate_TreasureSE = ((DBListType) this.cmbOpenSETreasure.SelectedItem).Key;
                        var treasureName = "";

                        if (this.cmbKeyItemTreasure.SelectedIndex > 0) {
                            //鍵あり
                            this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.TreasureWithKey
                                .Replace("$1", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Item].DBs[0], ((DBListType) this.cmbKeyItemTreasure.SelectedItem).Key))
                                .Replace("$2", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((DBListType) this.cmbOpenSETreasure.SelectedItem).Key))
                                .Replace("$3", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.SelfEvent], ((DBListType) this.cmbSelfValueTreasure.SelectedItem).Key));
                        } else {
                            //鍵なし
                            this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.Treasure
                                .Replace("$2", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((DBListType) this.cmbOpenSETreasure.SelectedItem).Key))
                                .Replace("$3", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.SelfEvent], ((DBListType) this.cmbSelfValueTreasure.SelectedItem).Key));
                        }

                        if (this.rdbItemTreasure.Checked) {
                            //財宝がアイテムの場合
                            treasureName = Database.GetNameFromIDName(((DBListType) this.cmbItemTreasure.SelectedItem).Key);
                            this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run]
                                .Replace("//ITEM: ", "")
                                .Replace("$4A", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Item].DBs[0], ((DBListType) this.cmbItemTreasure.SelectedItem).Key));
                        } else if (this.rdbMoneyTreasure.Checked) {
                            //財宝がお金の場合
                            treasureName = $"{(int) this.numMoneyTreasure.Value} G";
                            this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run]
                                .Replace("//MONEY: ", "")
                                .Replace("$4B", ((int) this.numMoneyTreasure.Value).ToString());
                        }

                        //拾得物をメッセージの中に入れる
                        this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run]
                            .Replace("$5", "\"" + Resources.SQ_GotTreasureMessage.Replace("$", treasureName) + "\"");

                        //１ページ目の設定
                        this.ev.Pages[0].Trigger = Map.EventTriggerType.PressEnterKey;
                        this.ev.Pages[0].Direction = Map.Direction4.South;
                        this.ev.Pages[0].Graphic = this.uctlGraphicsBeforeTreasure.FileName;
                        this.ev.Pages[0].MovingAnimation = false;
                        this.ev.Pages[0].StoppingAnimation = false;

                        //２ページ目の設定
                        this.ev.Pages.Add(new EVData.EventOnePage());
                        this.ev.Pages[1].Direction = Map.Direction4.South;
                        this.ev.Pages[1].Graphic = this.uctlGraphicsAfterTreasure.FileName;
                        this.ev.Pages[1].MovingAnimation = false;
                        this.ev.Pages[1].StoppingAnimation = false;
                        this.ev.Pages[1].SQ[(int) Map.EventScriptType.Run] = "// このページは取得後の状態（２ページ目）です。\r\n// 取得後の財宝イベントは起動しないようにします。";
                        this.ev.Pages[1].SQ[(int) Map.EventScriptType.Page] = Resources.Treasure2ndPage
                            .Replace("$1", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Value].DBs[(int) Database.DBValueIndices.SelfEvent], ((DBListType) this.cmbSelfValueTreasure.SelectedItem).Key));
                    }
                    break;

                case TemplateIndices.Shop:
                    if (this.uctlShopItemList.Result.Count == 0) {
                        MessageBox.Show("商品が一つもありません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        this.DialogResult = DialogResult.None;
                    } else {
                        //コード生成を行う
                        var beforeMsg = this.txtBeforeMessageShop.Text
                            .Replace("\r", "")          //CRは削除
                            .Replace("\\f\n", "\\f")    //改頁して改行している部分は改頁だけにする
                            .Replace("\n", "\\n");      //改行記号は文字列として含める
                        var afterMsg = this.txtAfterMessageShop.Text
                            .Replace("\r", "")          //CRは削除
                            .Replace("\\f\n", "\\f")    //改頁して改行している部分は改頁だけにする
                            .Replace("\n", "\\n");      //改行記号は文字列として含める

                        this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = Resources.Shop
                            .Replace("$1", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Face], ((DBListType) this.cmbFaceShop.SelectedItem).Key))
                            .Replace("$2", "\"" + this.txtSpeakerShop.Text + "\"")
                            .Replace("$3", "\"" + beforeMsg + "\"")
                            .Replace("$4", "\"" + afterMsg + "\"");

                        //商品リスト追加コードを挿入する
                        var buf = "";
                        foreach (var item in this.uctlShopItemList.Result) {
                            buf += Resources.SQ_ShopAddItem.Replace("$", Database.GetFixedIDWithCommentName(this.dbList[Database.DBIndices.Item].DBs[0], item.SubItems[1].Text)) + ";";
                            buf += "\n";
                        }
                        this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run] = this.ev.Pages[0].SQ[(int) Map.EventScriptType.Run]
                            .Replace("//ADDITEMS", buf);

                        //ページ設定
                        this.ev.Pages[0].Trigger = Map.EventTriggerType.PressEnterKey;
                        this.ev.Pages[0].MovingAnimation = false;
                        this.ev.Pages[0].StoppingAnimation = false;
                    }
                    break;

                case TemplateIndices.UserEV:
                    if (this.ltvUserTemplates.SelectedIndices.Count == 0) {
                        //テンプレートが選択されていない
                        MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "イベントテンプレート"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        this.DialogResult = DialogResult.None;
                    } else {
                        //コード生成・各種イベントページ設定を行う
                        using (var R = new StreamReader(Common.OpenFileReadOnly(ProjectManager.ProjectPath + Resources.Path_PrjSys_EVTemplate + this.ltvUserTemplates.SelectedItems[0].Text + Resources.Extension_SQTemplate), Common.SJIS)) {
                            var templateEV = EVData.DoLoadEVSetFunction(R);
                            if (templateEV.Count > 0) {
                                this.ev = templateEV[0];
                                this.ev.DoLoadEVFunction(R);
                            }
                        }
                    }
                    break;
            }

            //却下された場合は中断する
            if (this.DialogResult != DialogResult.OK) {
                return;
            }

            //イベント編集画面へ移行する
            this.ev.VisibleID = (int) this.numEVVisibleID.Value;
            this.ev.Name = this.txtEVName.Text;
            this.ev.TilePosition = this.tilePosition;
            var Dlg = new dlgEVSettings(this.ev, this.MapGUID, this.comTree, this.dbCtl) {
                Tag = this
            };

            //呼び出し先のダイアログ側が、このウィンドウを非表示にする
            if (Dlg.ShowDialog(this.Owner) != DialogResult.OK) {
                this.DialogResult = DialogResult.Cancel;
            }
        }

        /// <summary>
        /// テンプレート切替時にイベント名の雛形を自動生成する
        /// </summary>
        private void tbcEVTemplates_SelectedIndexChanged(object sender, EventArgs e) {
            this.applyAutoGeneratedEVName();
        }

        /// <summary>
        /// イベント名の雛形を自動生成して適用します。
        /// 自動生成に相当しないイベント名が入力された場合は適用しません。
        /// ユーザー定義テンプレートが選択されている場合は、テンプレート名を雛形とします。
        /// </summary>
        private void applyAutoGeneratedEVName() {
            if (this.tbcEVTemplates.SelectedIndex == -1) {
                return;
            }

            // イベント名がデフォルトから変更されているかどうかをチェック
            var tabTexts = this.tbcEVTemplates.TabPages
                .OfType<TabPage>()
                .Select(tab => tab.Text)
                .ToArray();
            var userTemplateNames = this.ltvUserTemplates.Items
                .OfType<ListViewItem>()
                .Select(item => item.Text)
                .ToArray();
            if (!string.IsNullOrEmpty(this.txtEVName.Text)
                    && !tabTexts.Contains(this.txtEVName.Text)
                    && !userTemplateNames.Contains(this.txtEVName.Text)) {
                // デフォルトから変更が入っているため自動生成を抑制する
                return;
            }

            // 自動生成する
            if (this.tbcEVTemplates.SelectedIndex == (int) TemplateIndices.Plain) {
                this.txtEVName.Text = "";
            } else if (this.tbcEVTemplates.SelectedIndex != (int) TemplateIndices.UserEV) {
                this.txtEVName.Text = this.tbcEVTemplates.SelectedTab.Text;
            } else {
                if (this.ltvUserTemplates.SelectedIndices.Count != 0) {
                    // ユーザー定義テンプレートの場合は、テンプレート名を使用する
                    this.txtEVName.Text = this.ltvUserTemplates.SelectedItems[0].Text;
                } else {
                    this.txtEVName.Text = "";
                }
            }
        }

        /// <summary>
        /// 財宝：種別変更
        /// </summary>
        private void rdbTreasure_CheckedChanged(object sender, EventArgs e) {
            this.numMoneyTreasure.Enabled = (this.rdbMoneyTreasure.Checked);
            this.cmbItemTreasure.Enabled = (this.rdbItemTreasure.Checked);
        }

        /// <summary>
        /// 財宝：取得前のグラフィックを決定したら取得後にもそれを自動で反映させる
        /// </summary>
        private void uctlGraphicsBeforeTreasure_GraphicsChanged(object sender, EventArgs e) {
            this.uctlGraphicsAfterTreasure.FileName = this.uctlGraphicsBeforeTreasure.FileName;
        }

        /// <summary>
        /// お店：陳列商品選択時
        /// </summary>
        private void uctlShopItemList_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            Database.SelectComboBoxItemFromIDName(this.cmbItemDBShop, e.Item.SubItems[1].Text);
        }

        /// <summary>
        /// お店：陳列前の設定
        /// </summary>
        private void uctlShopItemList_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            e.Item.Text = ((DBListType) this.cmbItemDBShop.SelectedItem).Value.ToString();
            e.Item.SubItems.Add(((DBListType) this.cmbItemDBShop.SelectedItem).Key);
        }

        /// <summary>
        /// お店：入力欄をクリア
        /// </summary>
        private void uctlShopItemList_RequestClearListEditorSource(object sender, EventArgs e) {
            this.cmbItemDBShop.SelectedIndex = -1;
        }

        /// <summary>
        /// お店：コントロール有効無効
        /// </summary>
        private void uctlShopItemList_RequestWatchControls(object sender, EventArgs e) {
            this.uctlShopItemList.SetCanApply(this.cmbItemDBShop.SelectedIndex != -1);
        }

        /// <summary>
        /// ユーザー定義：イベントテンプレートのリストを読み込みます。
        /// </summary>
        private void reloadEVTemplates() {
            this.ltvUserTemplates.SelectedIndices.Clear();
            this.ltvUserTemplates.Items.Clear();
            this.ltvUserTemplates.BeginUpdate();

            var files = Directory.GetFiles(ProjectManager.ProjectPath + Common.CutLastChar(Resources.Path_PrjSys_EVTemplate, '\\'));
            foreach (var file in files) {
                if (Path.GetExtension(file) == Resources.Extension_SQTemplate) {
                    this.ltvUserTemplates.Items.Add(Path.GetFileNameWithoutExtension(file));
                }
            }

            this.ltvUserTemplates.EndUpdate();
        }

        /// <summary>
        /// ユーザー定義：テンプレート選択変更
        /// </summary>
        private void ltvUserTemplates_SelectedIndexChanged(object sender, EventArgs e) {
            var isEnabled = (this.ltvUserTemplates.SelectedIndices.Count > 0);
            this.btnExportTemplate.Enabled = isEnabled;
            this.btnDeleteTemplate.Enabled = isEnabled;

            // イベント名の雛形を再生成
            this.applyAutoGeneratedEVName();
        }

        /// <summary>
        /// ユーザー定義：インポート
        /// </summary>
        private void btnImportTemplate_Click(object sender, EventArgs e) {
            //インポート元選択
            if (this.dlgImporter.ShowDialog() != DialogResult.OK) {
                return;
            }

            //上書き確認
            if (File.Exists(ProjectManager.ProjectPath + Resources.Path_PrjSys_EVTemplate + Path.GetFileName(this.dlgImporter.FileName))
            && MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "同名のテンプレートに存在しますが、上書き"), Resources.AppName, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) != DialogResult.Yes) {
                return;
            }

            //テンプレートファイルをプロジェクトフォルダーへコピーする
            try {
                File.Copy(
                    this.dlgImporter.FileName,
                    ProjectManager.ProjectPath + Resources.Path_PrjSys_EVTemplate + Path.GetFileName(this.dlgImporter.FileName),
                    true
                );
                this.reloadEVTemplates();
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "イベントテンプレートのインポート"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// ユーザー定義：エクスポート
        /// </summary>
        private void btnExportTemplate_Click(object sender, EventArgs e) {
            if (this.ltvUserTemplates.SelectedIndices.Count == 0) {
                return;
            }

            //インポート先選択
            if (this.dlgExporter.ShowDialog() != DialogResult.OK) {
                return;
            }

            //テンプレートファイルをエクスポート先へコピーする
            try {
                File.Copy(
                    ProjectManager.ProjectPath + Resources.Path_PrjSys_EVTemplate + this.ltvUserTemplates.SelectedItems[0].Text + Resources.Extension_SQTemplate,
                    this.dlgExporter.FileName,
                    true
                );
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "イベントテンプレートのエクスポート"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        /// <summary>
        /// ユーザー定義：削除
        /// </summary>
        private void btnDeleteTemplate_Click(object sender, EventArgs e) {
            if (this.ltvUserTemplates.SelectedIndices.Count == 0
            || MessageBox.Show(Resources.MsgQ_Confirm.Replace("$", "選択されたイベントテンプレートを削除"), Resources.AppName, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button2) != DialogResult.Yes) {
                return;
            }

            //削除を実行する
            try {
                File.Delete(ProjectManager.ProjectPath + Resources.Path_PrjSys_EVTemplate + this.ltvUserTemplates.SelectedItems[0].Text + Resources.Extension_SQ);
                this.reloadEVTemplates();
            } catch {
                MessageBox.Show(Resources.MsgE_Failed.Replace("$", "イベントテンプレートの削除"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
