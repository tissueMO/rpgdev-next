using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Editor.Dialog.Database {
    using Module;
    using Module.DB;
    using Properties;
    using DBListType = KeyValuePair<string, int>;

    public partial class dlgEditCurve : Form {
        private readonly List<Database.CurveData> data;
        private readonly int maxLevel;

        /// <summary>
        /// コンストラクター
        /// </summary>
        /// <param name="srcDB">設定リストのソース。nullを設定すると単体設定画面になります。</param>
        public dlgEditCurve(DataGridView srcDB, int maxLevel, string defaultData) {
            this.InitializeComponent();

            //初期設定
            this.maxLevel = maxLevel;
            this.data = new List<Database.CurveData>();
            this.cmbTargetDB.Items.Clear();
            this.cmbTargetDB.Enabled = (srcDB != null);

            if (srcDB == null) {
                //設定対象には唯一の項目を選択して固定する
                this.cmbTargetDB.Items.Add("");
                this.data.Add(new Database.CurveData());
            } else {
                //ソースDBのリストを生成する
                Database.CreateComboBoxListFromSrcDB(this.cmbTargetDB, srcDB);
                for (var i = 0; i < this.cmbTargetDB.Items.Count; i++) {
                    this.data.Add(new Database.CurveData());     //リストの数だけ曲線を生成する
                }

                //設定対象が存在しない場合はダイアログを開かない
                if (this.cmbTargetDB.Items.Count == 0) {
                    MessageBox.Show("設定項目が一つもありません。", Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    this.DialogResult = DialogResult.Cancel;
                    this.Close();
                    return;
                }
            }

            //既定値を設定
            if (!string.IsNullOrEmpty(defaultData)) {
                //設定対象ごとに処理
                var split = defaultData.Split(Resources.Split_LongData.ToCharArray());

                foreach (var target in split) {
                    var subsplit = target.Split(Resources.Split_SimpleList.ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

                    //設定対象のインデックスを探す
                    for (var k = 0; subsplit == null || k < this.cmbTargetDB.Items.Count; k++) {
                        if (srcDB != null && subsplit[(int) Database.DBCurveDataIndices.IDName] != ((DBListType) this.cmbTargetDB.Items[k]).Key) {
                            continue;
                        }

                        //データを一括設定
                        for (var i = 0; i < (int) Database.DBCurveDataIndices.Count; i++) {
                            switch (i) {
                                case (int) Database.DBCurveDataIndices.Base:
                                    this.data[k].Base = int.Parse(subsplit[i]);
                                    break;
                                case (int) Database.DBCurveDataIndices.Coefficient:
                                    this.data[k].Coefficient = int.Parse(subsplit[i]);
                                    break;
                                case (int) Database.DBCurveDataIndices.Index:
                                    this.data[k].Index = int.Parse(subsplit[i]);
                                    break;
                            }
                        }
                        break;
                    }
                }
            }

            //コントロールに反映
            this.numBase.Value = this.data[0].Base;
            this.numIndex.Value = this.data[0].Index;
            this.numCoefficient.Value = this.data[0].Coefficient;
            this.cmbTargetDB.SelectedIndex = 0;
        }

        /// <summary>
        /// 結果を返します。
        /// ソースDBが存在しない場合は、KeyとValueに同一の複数値、
        /// 存在する場合は、Keyには項目名を含む表示用のテキスト、Valueには [可視ID:名前] をFixedIDに置き換えたテキストが格納されます。
        /// </summary>
        public KeyValuePair<string, string> Result {
            get {
                //現在のモードに適したデータ形式で返す
                var viewbuf = "";
                var tagbuf = "";
                var subbuf = "";

                if (!this.cmbTargetDB.Enabled) {
                    //複数値として返す
                    subbuf = $"{Database.NotFoundItemID}" + Resources.Split_SimpleList + this.CreateOneData(-1);
                    viewbuf = subbuf;
                    tagbuf = subbuf;
                } else {
                    //IDValuesとして返す
                    for (var k = 0; k < this.cmbTargetDB.Items.Count; k++) {
                        var tgt = (DBListType) this.cmbTargetDB.Items[k];
                        subbuf = this.CreateOneData(k) + Resources.Split_LongData;
                        viewbuf += tgt.Key + Resources.Split_SimpleList + subbuf;
                        tagbuf += tgt.Value + Resources.Split_SimpleList + subbuf;
                    }
                    viewbuf = Common.CutLastChar(viewbuf, Resources.Split_LongData.ToCharArray()[0]);
                    tagbuf = Common.CutLastChar(tagbuf, Resources.Split_LongData.ToCharArray()[0]);
                }

                return new KeyValuePair<string, string>(viewbuf, tagbuf);
            }
        }

        /// <summary>
        /// 作業結果の単項目あたり基本データを生成する
        /// </summary>
        private string CreateOneData(int dataIndex) {
            var buf = "";

            for (var i = (int) Database.DBCurveDataIndices.Base; i < (int) Database.DBCurveDataIndices.Count; i++) {
                switch (i) {
                    case (int) Database.DBCurveDataIndices.Base:
                        buf += this.data[dataIndex == -1 ? 0 : dataIndex].Base;
                        break;
                    case (int) Database.DBCurveDataIndices.Coefficient:
                        buf += this.data[dataIndex == -1 ? 0 : dataIndex].Coefficient;
                        break;
                    case (int) Database.DBCurveDataIndices.Index:
                        buf += this.data[dataIndex == -1 ? 0 : dataIndex].Index;
                        break;
                }
                buf += Resources.Split_SimpleList;
            }

            buf = Common.CutLastChar(buf, Resources.Split_SimpleList.ToCharArray()[0]);
            return buf;
        }

        /// <summary>
        /// 設定対象が変更されたらコントロールに反映する
        /// </summary>
        private void cmbTargetDB_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.cmbTargetDB.SelectedIndex == -1) {
                return;
            }

            this.numBase.Value = this.data[this.cmbTargetDB.SelectedIndex].Base;
            this.numIndex.Value = this.data[this.cmbTargetDB.SelectedIndex].Index;
            this.numCoefficient.Value = this.data[this.cmbTargetDB.SelectedIndex].Coefficient;
            this.applyData(sender, e);
        }

        /// <summary>
        /// ユーザーによって値が変更されたら内部データに反映する
        /// </summary>
        private void numBase_ValueChanged(object sender, EventArgs e) {
            if (sender != null && this.cmbTargetDB.SelectedIndex != -1) {
                this.data[this.cmbTargetDB.SelectedIndex].Base = (int) this.numBase.Value;
                this.applyData(sender, e);
            }
        }

        /// <summary>
        /// ユーザーによって値が変更されたら内部データに反映する
        /// </summary>
        private void numCoefficient_ValueChanged(object sender, EventArgs e) {
            if (sender != null && this.cmbTargetDB.SelectedIndex != -1) {
                this.data[this.cmbTargetDB.SelectedIndex].Coefficient = (int) this.numCoefficient.Value;
                this.applyData(sender, e);
            }
        }

        /// <summary>
        /// ユーザーによって値が変更されたら内部データに反映する
        /// </summary>
        private void numIndex_ValueChanged(object sender, EventArgs e) {
            if (sender != null && this.cmbTargetDB.SelectedIndex != -1) {
                this.data[this.cmbTargetDB.SelectedIndex].Index = (int) this.numIndex.Value;
                this.applyData(sender, e);
            }
        }

        /// <summary>
        /// 変更を反映してチャートとテーブルを描画し直す
        /// </summary>
        private void applyData(object sender, EventArgs e) {
            if (this.cmbTargetDB.SelectedIndex == -1) {
                return;
            }

            //チャート更新
            this.crtPreview.Series[0].Points.Clear();
            this.crtPreview.ChartAreas[0].AxisX.Maximum = this.maxLevel;
            this.crtPreview.ChartAreas[0].AxisX.Interval = this.maxLevel / 2;

            for (var lv = 1; lv <= this.maxLevel; lv++) {
                checked {
                    try {
                        var val = (double) this.numBase.Value + ((double) this.numCoefficient.Value / 1000) * Math.Pow(lv, (double) this.numIndex.Value / 1000);

                        if (val <= int.MaxValue) {
                            this.crtPreview.Series[0].Points.AddY((int) val);
                        } else {        //オーバーフローしなくてもdecimalの最大を超えていたら最大値でセットする
                            this.crtPreview.Series[0].Points.AddY(int.MaxValue);
                        }
                    } catch {
                        //オーバーフローした場合は最大値でセットする
                        this.crtPreview.Series[0].Points.AddY(int.MaxValue);
                    }
                }
            }

            //テーブル更新
            this.ltvTable.BeginUpdate();
            this.ltvTable.Items.Clear();
            while (this.ltvTable.Columns.Count > 1) {
                // Lv 以外の列をすべて削除
                this.ltvTable.Columns.RemoveAt(1);
            }
            if (this.cmbTargetDB.Enabled) {
                // すべてのパラメーターの変動を一覧する
                foreach (var item in this.cmbTargetDB.Items) {
                    var text = ((DBListType) item).Key;
                    this.ltvTable.Columns.Add(Database.GetNameFromIDName(text), 70, HorizontalAlignment.Right);
                }
            } else {
                // 単一系列値の変動を一覧する
                this.ltvTable.Columns.Add("値", 70, HorizontalAlignment.Right);
                this.ltvTable.Columns.Add("差分値", 70, HorizontalAlignment.Right);
            }
            for (var lv = 1; lv <= this.maxLevel; lv++) {
                var newItem = new ListViewItem(lv.ToString());

                //各パラメーターごとに現在のレベル時の値を計算
                foreach (var curve in this.data) {
                    int value = 0;

                    checked {
                        try {
                            var temp = curve.Base + ((double) curve.Coefficient / 1000) * Math.Pow(lv, (double) curve.Index / 1000);

                            if (temp <= int.MaxValue) {
                                value = (int) temp;
                            } else {
                                //オーバーフローしなくてもdecimalの最大を超えていたら最大値でセットする
                                value = int.MaxValue;
                            }
                        } catch {
                            //オーバーフローした場合は最大値でセットする
                            value = int.MaxValue;
                        }
                    }

                    newItem.SubItems.Add(value.ToString());
                    if (!this.cmbTargetDB.Enabled) {
                        // 単一系列値の場合は差分値を添える
                        if (lv > 1) {
                            var previousValue = int.Parse(this.ltvTable.Items[lv - 1 - 1].SubItems[1].Text);
                            var delta = value - previousValue;
                            newItem.SubItems.Add(delta.ToString());
                        } else {
                            newItem.SubItems.Add("---");
                        }
                    }
                }

                this.ltvTable.Items.Add(newItem);
            }
            this.ltvTable.EndUpdate();
        }
    }
}
