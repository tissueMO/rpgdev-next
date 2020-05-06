using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Database.Effect {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using EffectOrder = CtrlComponent.Database.mgrDBEffect.EffectOneData.EffectOrder;
    using SoundItem = KeyValuePair<string, int>;

    public partial class dlgOrder : Form {
        private readonly AllDB DBList;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgOrder(AllDB DBList, EffectOrder baseOrder, int frameLength, int targetFrame, bool enabledEditFrameNum, List<string> SQList) {
            this.InitializeComponent();
            this.numTargetFrame.Enabled = enabledEditFrameNum;
            this.numTargetFrame.Maximum = frameLength - 1;
            this.DBList = DBList;

            //効果音リストの生成
            Database.CreateComboBoxListFromSrcDB(this.cmbSEList, DBList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], false);

            //独自コマンドの関数リストを生成
            this.cmbCommand.Items.Clear();
            foreach (var sqFile in SQList) {
                var funcs = SQ.GetFunctionList(sqFile);
                foreach (var func in funcs) {
                    this.cmbCommand.Items.Add(func.SubItems[(int) SQ.FuncListColumns.Name].Text);
                }
            }
            this.tips.SetToolTip(this.cmbCommand, $"同一フレームの実行カウンターは {Resources.SQ_EffectSameCounter} です。\r\nこれは特定のフレームが最初に実行されるときだけ処理したい場合などに使います。");

            //既定値のセット
            this.numTargetFrame.Value = (this.numTargetFrame.Maximum >= targetFrame) ? targetFrame : this.numTargetFrame.Maximum;
            if (baseOrder == null) {
                return;
            }

            if (baseOrder.Frame < frameLength) {
                //有効なフレーム番号のときだけ既定値からセットする
                this.numTargetFrame.Value = baseOrder.Frame;
            }
            switch (baseOrder.Type) {
                case EffectOrder.OrderType.Sound:
                    //一致する効果音ファイルを探す
                    for (var i = 0; i < this.cmbSEList.Items.Count; i++) {
                        if (((SoundItem) this.cmbSEList.Items[i]).Value.ToString() == baseOrder.Options[(int) EffectOrder.SoundOptionIndices.FixedID]) {
                            this.cmbSEList.SelectedIndex = i;
                            this.uctlSEEditor.SoundData = new Media.SoundObject(
                                DBList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound].Rows[i].Cells[(int) Database.DBDefaultColumnIndices.Count].Value?.ToString(),
                                int.Parse(baseOrder.Options[(int) EffectOrder.SoundOptionIndices.Volume]),
                                int.Parse(baseOrder.Options[(int) EffectOrder.SoundOptionIndices.Pitch]),
                                int.Parse(baseOrder.Options[(int) EffectOrder.SoundOptionIndices.Pan])
                            );
                            break;
                        }
                    }
                    this.rdbSE.Checked = true;
                    break;

                case EffectOrder.OrderType.Shake:
                    this.numShakeStrength.Value = int.Parse(baseOrder.Options[0]);
                    this.rdbShake.Checked = true;
                    break;

                case EffectOrder.OrderType.Flash:
                    this.pnlFlashColor.BackColor = Common.StringToColor(baseOrder.Options[(int) EffectOrder.FlashOptionIndices.Color]);
                    this.numFlashStrength.Value = int.Parse(baseOrder.Options[(int) EffectOrder.FlashOptionIndices.Strength]);
                    this.numFlashCount.Value = int.Parse(baseOrder.Options[(int) EffectOrder.FlashOptionIndices.Count]);
                    this.rdbFlash.Checked = true;
                    break;

                case EffectOrder.OrderType.WaitBreak:
                    this.rdbWaitBreak.Checked = true;
                    break;

                case EffectOrder.OrderType.User:
                    this.cmbCommand.Text = baseOrder.Options[0];
                    this.rdbScript.Checked = true;
                    break;
            }
        }

        /// <summary>
        /// ウィンドウ破棄前にサウンドを破棄する
        /// </summary>
        private void dlgOrder_FormClosing(object sender, FormClosingEventArgs e) {
            this.uctlSEEditor.UnloadMedia();
        }

        /// <summary>
        /// 操作結果
        /// </summary>
        public EffectOrder Result {
            get {
                //この時点で作成される命令情報は仮作成の状態で、実際に登録されるときに正式にセットされる
                if (this.rdbSE.Checked) {
                    var pars = new string[Common.GetEnumCount<EffectOrder.SoundOptionIndices>()];
                    for (var i = 0; i < pars.Length; i++) {
                        switch (i) {
                            case (int) EffectOrder.SoundOptionIndices.FixedID:
                                pars[i] = ((SoundItem) this.cmbSEList.SelectedItem).Value.ToString();
                                break;
                            case (int) EffectOrder.SoundOptionIndices.IDName:
                                pars[i] = ((SoundItem) this.cmbSEList.SelectedItem).Key;
                                break;
                            case (int) EffectOrder.SoundOptionIndices.Volume:
                                pars[i] = this.uctlSEEditor.SoundData.Volume.ToString();
                                break;
                            case (int) EffectOrder.SoundOptionIndices.Pitch:
                                pars[i] = this.uctlSEEditor.SoundData.Pitch.ToString().ToString();
                                break;
                            case (int) EffectOrder.SoundOptionIndices.Pan:
                                pars[i] = this.uctlSEEditor.SoundData.Panpot.ToString();
                                break;
                        }
                    }
                    return new EffectOrder(
                        null,
                        (int) this.numTargetFrame.Value,
                        EffectOrder.OrderType.Sound,
                        pars
                    );
                } else if (this.rdbShake.Checked) {
                    return new EffectOrder(
                        null,
                        (int) this.numTargetFrame.Value,
                        EffectOrder.OrderType.Shake,
                        new string[] {
                            ((int)this.numShakeStrength.Value).ToString()
                        }
                    );
                } else if (this.rdbFlash.Checked) {
                    var pars = new string[Common.GetEnumCount<EffectOrder.FlashOptionIndices>()];
                    for (var i = 0; i < pars.Length; i++) {
                        switch (i) {
                            case (int) EffectOrder.FlashOptionIndices.Color:
                                pars[i] = Common.ColorToString(this.pnlFlashColor.BackColor);
                                break;
                            case (int) EffectOrder.FlashOptionIndices.Strength:
                                pars[i] = ((int) this.numFlashStrength.Value).ToString();
                                break;
                            case (int) EffectOrder.FlashOptionIndices.Count:
                                pars[i] = ((int) this.numFlashCount.Value).ToString();
                                break;
                        }
                    }
                    return new EffectOrder(
                        null,
                        (int) this.numTargetFrame.Value,
                        EffectOrder.OrderType.Flash,
                        pars
                    );
                } else if (this.rdbWaitBreak.Checked) {
                    return new EffectOrder(
                        null,
                        (int) this.numTargetFrame.Value,
                        EffectOrder.OrderType.WaitBreak,
                        new string[] { "" }
                    );
                } else if (this.rdbScript.Checked) {
                    return new EffectOrder(
                        null,
                        (int) this.numTargetFrame.Value,
                        EffectOrder.OrderType.User,
                        new string[] {
                            this.cmbCommand.Text
                        }
                    );
                }
                return null;
            }
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (this.rdbSE.Checked) {
                if (this.cmbSEList.SelectedIndex == -1) {
                    //効果音が選択されていない
                    MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "再生する効果音"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.DialogResult = DialogResult.None;
                    return;
                }
            }
            if (this.rdbScript.Checked) {
                if (string.IsNullOrEmpty(this.cmbCommand.Text)) {
                    //スクリプトが入力されていない
                    MessageBox.Show(Resources.MsgE_NothingInput.Replace("$", "スクリプトコード"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.DialogResult = DialogResult.None;
                    return;
                } else if (this.cmbCommand.Text.IndexOf(";") != -1) {
                    //セミコロンはエディター側で自動付加するのでユーザーによって入れてはいけない
                    MessageBox.Show("コード欄にセミコロンを含めることはできません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.DialogResult = DialogResult.None;
                    return;
                }
            }
        }

        /// <summary>
        /// サウンドの再生準備
        /// </summary>
        private void cmbSEList_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.cmbSEList.SelectedIndex == -1) {
                return;
            }

            //FixedIDからファイル名を取得して、プレイヤーにセットする
            this.uctlSEEditor.SoundData = new Media.SoundObject(Database.GetFileNameFromFixedID(this.DBList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], ((SoundItem) this.cmbSEList.SelectedItem).Value));
        }

        /// <summary>
        /// 色の選択
        /// </summary>
        private void P_FlashColor_MouseClick(object sender, MouseEventArgs e) {
            this.dlgSetColor.Color = this.pnlFlashColor.BackColor;
            if (this.dlgSetColor.ShowDialog() == DialogResult.OK) {
                this.pnlFlashColor.BackColor = this.dlgSetColor.Color;
            }
        }

        /// <summary>
        /// チェックが変更されたときにパネルの有効無効
        /// </summary>
        private void rdb_CheckedChanged(object sender, EventArgs e) {
            this.pnlSE.Enabled = this.rdbSE.Checked;
            this.pnlFlash.Enabled = this.rdbFlash.Checked;
            this.pnlShake.Enabled = this.rdbShake.Checked;
            this.pnlScript.Enabled = this.rdbScript.Checked;
        }
    }
}
