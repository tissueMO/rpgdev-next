using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Dialog.Map {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;
    using DBListType = KeyValuePair<string, int>;

    public partial class dlgMapSettings : Form {
        private readonly CtrlComponent.Map.mgrMapObject.MapOneData result;

        /// <summary>
        /// コンストラクター
        /// </summary>
        public dlgMapSettings(AllDB DBList, string title, CtrlComponent.Map.mgrMapObject.MapOneData data) {
            this.InitializeComponent();
            Common.EnableDoubleBuffering(this);
            this.Text = title;

            //設定対象が存在しない場合はダイアログを開かない
            this.result = data;
            if (data == null) {
                this.DialogResult = DialogResult.Cancel;
                this.Close();
                return;
            }

            //NOTE: マップ情報 IN
            //初期設定
            this.uctlMapSize.Maximum = new Point(Map.MaxMapSize);
            this.cmbMapLoopType.Items.AddRange(Map.GetMapLoopTypeList());
            this.cmbPTMoveSpeed.Items.AddRange(Map.GetSpeedList());
            Common.SetListControlKeyValuePairMode(this.cmbTileset);
            Database.CreateComboBoxListFromSrcDB(this.cmbTileset, DBList[Database.DBIndices.Tileset].DBs[0]);
            this.uctlPathBaseMap.FileFilter = Resources.Extension_Map;
            this.uctlPathBaseMap.RootPath = Resources.Path_Maps;

            this.uctlPathBGM.FileFilter = Resources.Extension_Sounds;
            this.uctlPathBGM.RootPath = Resources.Path_Materials;
            this.uctlPathBGS.FileFilter = Resources.Extension_LoopSounds;
            this.uctlPathBGS.RootPath = Resources.Path_Materials;

            this.uctlPathBack.FileFilter = Resources.Extension_Graphics;
            this.uctlPathBack.RootPath = Resources.Path_Materials;
            this.cmbBackScrollType.Items.AddRange(Map.GetScrollTypeList());
            this.cmbBackScrollType.SelectedIndex = (int) Map.ScrollType.None;

            this.uctlPathFog.FileFilter = Resources.Extension_Graphics;
            this.uctlPathFog.RootPath = Resources.Path_Materials;
            this.cmbFogScrollType.Items.AddRange(Map.GetScrollTypeList());
            this.cmbFogScrollType.SelectedIndex = (int) Map.ScrollType.None;
            this.cmbFogBlend.Items.AddRange(Media.GetBlendModeList());
            this.cmbFogBlend.SelectedIndex = (int) Media.BlendMode.Alpha;

            this.uctlPathBattleBack.FileFilter = Resources.Extension_Graphics;
            this.uctlPathBattleBack.RootPath = Resources.Path_Materials;
            Common.SetListControlKeyValuePairMode(this.cmbUnitDB);
            Database.CreateComboBoxListFromSrcDB(this.cmbUnitDB, DBList[Database.DBIndices.Unit].DBs[0]);

            //マップ設定を復元する
            this.txtName.Text = data.Name;
            this.uctlMapSize.Result = new Point(data.MapSize);
            this.cmbMapLoopType.SelectedIndex = (int) data.MapLoopOption;
            Database.SelectComboBoxItemFromIDName(this.cmbTileset, Database.GetIDNameFromFixedID(DBList[Database.DBIndices.Tileset].DBs[0], data.TilesetFixedID));
            this.cmbPTMoveSpeed.SelectedIndex = (int) data.MoveSpeed;
            this.uctlPathBaseMap.Result = data.BaseMapFileName;

            this.uctlPathBGM.Result = data.BGM.FileName;
            this.uctlBGMEditor.SoundData = data.BGM;
            this.uctlPathBGS.Result = data.BGS.FileName;
            this.uctlBGSEditor.SoundData = data.BGS;

            this.uctlPathBack.Result = data.BackPictureFileName;
            this.cmbBackScrollType.SelectedIndex = (int) data.BackPictureScrollType;
            this.numBackSpeed.Value = data.BackPictureScrollSpeed;

            this.uctlPathFog.Result = data.FogFileName;
            this.cmbFogScrollType.SelectedIndex = (int) data.FogScrollType;
            this.numFogSpeed.Value = data.FogScrollSpeed;
            this.cmbFogBlend.SelectedIndex = (int) data.FogBlendMode;

            this.numEncounterRate.Value = data.EncounterRate;
            this.uctlPathBattleBack.Result = data.BattleBackFileName;
            var units = new List<ListViewItem>();
            foreach (var unit in data.EncounterUnits) {
                var item = new ListViewItem {
                    Text = unit.ToString()
                };

                var IDName = Database.GetIDNameFromFixedID(DBList[Database.DBIndices.Unit].DBs[0], unit);
                if (string.IsNullOrEmpty(IDName)) {
                    //存在しないFixedIDは除外する
                    continue;
                }
                item.SubItems.Add(IDName);

                units.Add(item);
            }
            this.uctlUnitList.SetupList(new string[] { "ユニット" }, units, true);
        }

        /// <summary>
        /// 確定前に検証する
        /// </summary>
        private void uctlOKCancel_ClickOK(object sender, EventArgs e) {
            if (this.cmbTileset.SelectedIndex == -1) {
                MessageBox.Show(Resources.MsgE_NothingSelect.Replace("$", "タイルセット"), Resources.AppName, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.DialogResult = DialogResult.None;
                return;
            }

            //有効であればここで情報を反映する
            //NOTE: マップ情報 OUT
            this.result.Name = this.txtName.Text;
            this.result.MapSize = new Size(this.uctlMapSize.Result);
            this.result.MapLoopOption = (Map.MapLoopType) this.cmbMapLoopType.SelectedIndex;
            this.result.TilesetFixedID = ((DBListType) this.cmbTileset.SelectedItem).Value;
            this.result.MoveSpeed = (Map.Speed) this.cmbPTMoveSpeed.SelectedIndex;
            this.result.BaseMapFileName = this.uctlPathBaseMap.Result;

            this.result.BGM = this.uctlBGMEditor.SoundData;
            this.result.BGS = this.uctlBGSEditor.SoundData;

            this.result.BackPictureFileName = this.uctlPathBack.Result;
            this.result.BackPictureScrollType = (Map.ScrollType) this.cmbBackScrollType.SelectedIndex;
            this.result.BackPictureScrollSpeed = (int) this.numBackSpeed.Value;

            this.result.FogFileName = this.uctlPathFog.Result;
            this.result.FogScrollType = (Map.ScrollType) this.cmbFogScrollType.SelectedIndex;
            this.result.FogScrollSpeed = (int) this.numFogSpeed.Value;
            this.result.FogBlendMode = (Media.BlendMode) this.cmbFogBlend.SelectedIndex;

            this.result.EncounterUnits.Clear();
            foreach (var item in this.uctlUnitList.Result) {
                this.result.EncounterUnits.Add(int.Parse(item.Text));
            }
            this.result.EncounterRate = (int) this.numEncounterRate.Value;
            this.result.BattleBackFileName = this.uctlPathBattleBack.Result;
        }

        /// <summary>
        /// リストを選択すると入力ソース欄に反映する
        /// </summary>
        private void uctlUnitList_ListItemSelected(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            Database.SelectComboBoxItemFromIDName(this.cmbUnitDB, e.Item.SubItems[1].Text);
        }

        /// <summary>
        /// ユニット情報をListViewItemにセット
        /// </summary>
        private void uctlUnitList_RequestAddItem(object sender, CtrlComponent.Common.ctlListEditor.RequestAddItemEventArgs e) {
            if (this.cmbUnitDB.SelectedIndex == -1) {
                e.Canceled = true;
                return;
            }
            e.Item.Text = ((DBListType) this.cmbUnitDB.SelectedItem).Value.ToString();
            e.Item.SubItems.Add(((DBListType) this.cmbUnitDB.SelectedItem).Key);
        }

        /// <summary>
        /// 入力ソース欄を初期化
        /// </summary>
        private void uctlUnitList_RequestClearListEditorSource(object sender, EventArgs e) {
            this.cmbUnitDB.SelectedIndex = -1;
        }

        /// <summary>
        /// ウィンドウ破棄前にサウンドを破棄する
        /// </summary>
        private void dlgMapSettings_FormClosing(object sender, FormClosingEventArgs e) {
            this.uctlBGMEditor.UnloadMedia();
            this.uctlBGSEditor.UnloadMedia();
        }

        /// <summary>
        /// コントロール監視
        /// </summary>
        private void uctlUnitList_RequestWatchControls(object sender, EventArgs e) {
            var OK = true;
            OK &= (this.cmbUnitDB.SelectedIndex != -1);
            this.uctlUnitList.SetCanApply(OK);
        }

        /// <summary>
        /// BGM再生準備
        /// </summary>
        private void uctlPathBGM_PathChanged(object sender, EventArgs e) {
            this.uctlBGMEditor.SoundData = new Media.SoundObject(this.uctlPathBGM.Result, Media.SoundVolumeDefault);
        }

        /// <summary>
        /// BGS再生準備
        /// </summary>
        private void uctlPathBGS_PathChanged(object sender, EventArgs e) {
            this.uctlBGSEditor.SoundData = new Media.SoundObject(this.uctlPathBGS.Result, Media.SoundVolumeDefault);
        }
    }
}
