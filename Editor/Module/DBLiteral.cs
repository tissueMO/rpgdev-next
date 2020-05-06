using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Module.DB {
    using Properties;
    using AllDB = Dictionary<Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    /// <summary>
    /// データベース関連でベタ書きせざるを得ない箇所はここにまとめます。
    /// </summary>
    public static partial class DBLiteral {
        private const int btnColumnWidth = 30;              //列タイトルのないボタンの幅
        private const int textMaxLength = 65536;            //テキスト列の最大文字数

        /// <summary>
        /// 全データベースをフォーム上に生成する
        /// </summary>
        public static void SetupDatabases(TabControl tbc, AllDB DBList) {
            tbc.TabPages.Clear();
            DBList.Clear();

            //この時点ではまだFixedIDの列が存在しないことに注意
            for (var mainTabIndex = 0; mainTabIndex < Common.GetEnumCount<Database.DBIndices>(); mainTabIndex++) {
                CtrlComponent.Database.ctlDBBase db;
                DataGridView[] dbsubs = null;
                switch (mainTabIndex) {
                    case (int) Database.DBIndices.Char:
                        tbc.TabPages.Add("パーティキャラ");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Char, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnPictureFile(DBList, "グラフィック", 100, (Rectangle) (DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.DBCharGraphicsViewRange, 0].Value), true));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "肩書き", 150, false));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "属性", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "クラス", 150, new Database.DBAddress(Database.DBIndices.Class)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Class)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "アイテム小種別", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemSubType)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemSubType)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "装備品", 150, new Database.DBAddress(Database.DBIndices.Item)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Item)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "パラメーター", 150, true));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputCurve(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "経験値曲線", 150, true));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputCurve(DBList, dbsubs[0].Columns.Count, null));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "レベル", 70, 1, int.MaxValue, 1));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "習得スキル", 150, new Database.DBAddress(Database.DBIndices.Skill)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Skill)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnBool(DBList, "自律戦闘", 80));
                        DBLiteral.AddSelfValueAndAddonColumns(DBList, dbsubs[0], Database.DBValueIndices.SelfActor);
                        DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_Char);
                        break;

                    case (int) Database.DBIndices.Class:
                        tbc.TabPages.Add("クラス");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Class, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "属性", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "習得スキル", 150, new Database.DBAddress(Database.DBIndices.Skill)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Skill)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "パラメーター変動率", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                        DBLiteral.AddSelfValueAndAddonColumns(DBList, dbsubs[0], Database.DBValueIndices.SelfClass);
                        DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_Class);
                        break;

                    case (int) Database.DBIndices.Item:
                    case (int) Database.DBIndices.Skill:
                        tbc.TabPages.Add("");       //名前は後で設定する
                        if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Skill) {
                            //スキルDB
                            DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Skill, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        } else if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Item) {
                            //アイテムDB
                            DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Item, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        }
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnPictureIcon(DBList, "アイコン", 80,
                            new Rectangle(
                                    0, 0,
                                    int.Parse((DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconSize, 0].Value.ToString())),
                                    int.Parse((DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconSize, 0].Value.ToString())))
                            )
                        );

                        if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Skill) {
                            tbc.TabPages[tbc.TabCount - 1].Text = "スキル";
                            Database.AddDBColumn(dbsubs[0], new DBColumnListDB(DBList, "種別", 180, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.SkillType)));
                            Database.AddDBColumn(dbsubs[0], new DBColumnTextNumbers(DBList, "コスト", 70, int.MinValue, int.MaxValue, null));
                        } else if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Item) {
                            tbc.TabPages[tbc.TabCount - 1].Text = "アイテム";
                            Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "価格", 70, 0, int.MaxValue, 0));
                            Database.AddDBColumn(dbsubs[0], new DBColumnBool(DBList, "廃棄可", 70, true));
                            Database.AddDBColumn(dbsubs[0], new DBColumnBool(DBList, "消耗品", 70));
                            Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "パラメーター", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                            Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                        }
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        Database.AddDBColumn(dbsubs[0], new DBColumnListUser(DBList, "対象種別", 180, DBLiteral.GetListTargetType()));
                        Database.AddDBColumn(dbsubs[0], new DBColumnListUser(DBList, "場面制限", 180, DBLiteral.GetListSceneLimit()));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "属性", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));

                        if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Item) {
                            Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "アイテム大種別", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemType)));
                            Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemType)));
                        }

                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "アイテム小種別", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemSubType)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemSubType)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnListDB(DBList, "エフェクト", 180, new Database.DBAddress(Database.DBIndices.Effect)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "分散度", 70, int.MinValue, int.MaxValue, 20));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "速度補正", 80, int.MinValue, int.MaxValue, 100));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextNumbers(DBList, "効果量", 80, int.MinValue, int.MaxValue, null));
                        Database.AddDBColumn(dbsubs[0], new DBColumnListDB(DBList, "間接効果", 180, new Database.DBAddress(Database.DBIndices.State)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnListUser(DBList, "直接効果", 180, DBLiteral.GetListUseType()));

                        if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Skill) {
                            //スキルDB
                            DBLiteral.AddSelfValueColumn(DBList, dbsubs[0], Database.DBValueIndices.SelfUse);
                            DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_Skill);
                        } else if ((Database.DBIndices) mainTabIndex == Database.DBIndices.Item) {
                            //アイテムDB
                            DBLiteral.AddSelfValueAndAddonColumns(DBList, dbsubs[0], Database.DBValueIndices.SelfUse);
                            DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_Item);
                        }
                        break;

                    case (int) Database.DBIndices.Enemy:
                        tbc.TabPages.Add("エネミー");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Enemy, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnPictureFile(DBList, "グラフィック", 100, Rectangle.Empty));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "属性", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "パラメーター", 150, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "経験値", 100, 0, int.MaxValue, 0));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "所持金", 100, 0, int.MaxValue, 0));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "ドロップアイテム", 150, new Database.DBAddress(Database.DBIndices.Item)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count, new Database.DBAddress(Database.DBIndices.Item)));
                        DBLiteral.AddSelfValueAndAddonColumns(DBList, dbsubs[0], Database.DBValueIndices.SelfActor);
                        DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_Enemy);
                        break;

                    case (int) Database.DBIndices.State:
                        tbc.TabPages.Add("間接効果");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_State, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnPictureIcon(DBList, "アイコン", 80,
                            new Rectangle(
                                    0, 0,
                                    int.Parse((DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconSize, 0].Value.ToString())),
                                    int.Parse((DBList[Database.DBIndices.System].DBs[0][(int) Database.DBSystemIndices.IconSize, 0].Value.ToString())))
                            )
                        );
                        Database.AddDBColumn(dbsubs[0], new DBColumnListUser(DBList, "制約", 180, DBLiteral.GetListStateLimit()));
                        Database.AddDBColumn(dbsubs[0], new DBColumnListUser(DBList, "解除条件", 180, DBLiteral.GetListStateSolve()));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "解除条件値", 90, int.MinValue, int.MaxValue, 0));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "解除確率", 90, 0, int.MaxValue, 100));
                        Database.AddDBColumn(dbsubs[0], new DBColumnBool(DBList, "移動中も有効", 100));
                        Database.AddDBColumn(dbsubs[0], new DBColumnBool(DBList, "異常", 70));
                        Database.AddDBColumn(dbsubs[0], new DBColumnListDB(DBList, "上位互換", 180, new Database.DBAddress(Database.DBIndices.State)));
                        DBLiteral.AddSelfValueAndAddonColumns(DBList, dbsubs[0], Database.DBValueIndices.SelfState);
                        DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_State);
                        break;

                    case (int) Database.DBIndices.Init:
                        tbc.TabPages.Add("初期設定");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Init, new CtrlComponent.Database.ctlDBGeneral(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        db.SupportedRowTools = false;               //標準の行の操作を許可しない
                        db.SupportedNameSearch = false;             //名前検索を許可しない
                        dbsubs = db.DBs;
                        dbsubs[0].AllowUserToAddRows = false;
                        dbsubs[0].AllowUserToDeleteRows = false;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDNames(DBList, "パーティ編成", 300, new Database.DBAddress(Database.DBIndices.Char)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDNames(DBList, dbsubs[0].Columns.Count - 1, new Database.DBAddress(Database.DBIndices.Char)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextIDValues(DBList, "所持アイテム", 300, new Database.DBAddress(Database.DBIndices.Item)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnButtonInputIDValues(DBList, dbsubs[0].Columns.Count - 1, new Database.DBAddress(Database.DBIndices.Item)));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "所持金", 150, 0, int.MaxValue, 0));

                        //唯一の行を生成する
                        dbsubs[0].Rows.Add();
                        dbsubs[0][4, 0].Value = 0;          //所持金のセルは既定値を入れないとエラーになる
                        break;

                    case (int) Database.DBIndices.Material:
                        tbc.TabPages.Add("汎用素材");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Material, new CtrlComponent.Database.ctlDBMaterial(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;

                        //列設定
                        for (var i = 0; i < dbsubs.Length; i++) {
                            dbsubs[i].Tag = tbc.TabPages[tbc.TabCount - 1].Text + Resources.Split_IDNamePair + ((Database.DBMaterialIndices) i).ToString();
                            DBLiteral.AddStandardHeadColumns(DBList, dbsubs[i]);
                            switch (i) {
                                case (int) Database.DBMaterialIndices.Face:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnPictureFile(DBList, "グラフィック", 100, Rectangle.Empty));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnBool(DBList, "反対側", 70));
                                    break;

                                case (int) Database.DBMaterialIndices.Graphics:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnPictureFile(DBList, "グラフィック", 100, Rectangle.Empty));
                                    break;

                                case (int) Database.DBMaterialIndices.Sound:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnText(DBList, "ファイル名", 150, true));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnButtonSelectFile(DBList, dbsubs[i].Columns.Count, Resources.Extension_Sounds, Resources.Path_Materials));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnButtonPlaySound(DBList, dbsubs[i].Columns.Count - 1));
                                    break;

                                case (int) Database.DBMaterialIndices.Font:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnText(DBList, "フォント正式名称", 150, false));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnText(DBList, "フォントファイル名", 150, true));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnButtonSelectFile(DBList, dbsubs[i].Columns.Count, Resources.Extension_Font, Resources.Path_Fonts));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnTextOneNumber(DBList, "大きさ", 70, 1, byte.MaxValue, 20));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnTextOneNumber(DBList, "太さ", 70, 1, byte.MaxValue, 6));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnTextOneNumber(DBList, "行間", 70, 0, byte.MaxValue, 0));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnTextOneNumber(DBList, "文字間", 70, 0, byte.MaxValue, 0));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnListUser(DBList, "描画方法", 180, DBLiteral.GetListFontDrawing()));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnTextNumbers(DBList, "縁色", 150, 0, byte.MaxValue, new int[] { byte.MaxValue, byte.MaxValue, byte.MaxValue }));
                                    Database.AddDBColumn(dbsubs[i], new DBColumnButtonInputColor(DBList, dbsubs[i].Columns.Count));
                                    break;
                            }
                        }
                        break;

                    case (int) Database.DBIndices.Element:
                        tbc.TabPages.Add("基本要素");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Element, new CtrlComponent.Database.ctlDBElementDB(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;

                        //列設定
                        for (var i = 0; i < dbsubs.Length; i++) {
                            dbsubs[i].Tag = tbc.TabPages[tbc.TabCount - 1].Text + Resources.Split_IDNamePair + ((Database.DBElementIndices) i).ToString();
                            DBLiteral.AddStandardHeadColumns(DBList, dbsubs[i]);
                            switch (i) {
                                case (int) Database.DBElementIndices.ItemType:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnBool(DBList, "装備品", 80));
                                    break;
                            }
                        }
                        break;

                    case (int) Database.DBIndices.Value:
                        tbc.TabPages.Add("変数/フラグ");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Value, new CtrlComponent.Database.ctlDBValues(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;

                        //列設定
                        for (var i = 0; i < dbsubs.Length; i++) {
                            dbsubs[i].Tag = tbc.TabPages[tbc.TabCount - 1].Text + Resources.Split_IDNamePair + ((Database.DBValueIndices) i).ToString();
                            DBLiteral.AddStandardHeadColumns(DBList, dbsubs[i]);
                            switch (i) {
                                case (int) Database.DBValueIndices.CommonFlag:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnBool(DBList, "既定値", 70));
                                    break;

                                case (int) Database.DBValueIndices.CommonString:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnText(DBList, "既定値", 90, false));
                                    break;

                                default:
                                    Database.AddDBColumn(dbsubs[i], new DBColumnTextOneNumber(DBList, "既定値", 80, int.MinValue, int.MaxValue, 0));
                                    break;
                            }
                            DBLiteral.AddCommentColumn(DBList, dbsubs[i]);
                        }
                        break;

                    case (int) Database.DBIndices.System:
                        tbc.TabPages.Add("システム");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_System, new CtrlComponent.Database.ctlDBSystem(), tbc) as CtrlComponent.Database.ctlDBBase);
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        db.SupportedNameSearch = false;         //名前検索を許可しない
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;
                        break;

                    case (int) Database.DBIndices.Unit:
                        tbc.TabPages.Add("ユニット");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Unit, new CtrlComponent.Database.ctlDBGUIUnit(), tbc) as CtrlComponent.Database.ctlDBBase);
                        ((CtrlComponent.Database.ctlDBGUIUnit) DBList[Database.DBIndices.Unit]).DBList = DBList;
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        //Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "指標", 500, false));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "配置情報", 500, false));
                        DBLiteral.AddStandardFootColumns(DBList, dbsubs[0], Resources.Path_Scripts_Unit);
                        break;

                    case (int) Database.DBIndices.Effect:
                        tbc.TabPages.Add("エフェクト");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Effect, new CtrlComponent.Database.ctlDBGUIEffect(), tbc) as CtrlComponent.Database.ctlDBBase);
                        ((CtrlComponent.Database.ctlDBGUIEffect) DBList[Database.DBIndices.Effect]).DBList = DBList;
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "フレーム数", 100, int.MinValue, int.MaxValue, 0));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "描画対象", 100, int.MinValue, int.MaxValue, 0));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "レイヤー情報", 100, false));
                        break;

                    case (int) Database.DBIndices.Tileset:
                        tbc.TabPages.Add("タイルセット");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_Tileset, new CtrlComponent.Database.ctlDBGUITileset(), tbc) as CtrlComponent.Database.ctlDBBase);
                        ((CtrlComponent.Database.ctlDBGUITileset) DBList[Database.DBIndices.Tileset]).DBList = DBList;
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "メモ", 100, false));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "オートタイルリスト", 100, false));
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "オブジェクトタイルリスト", 100, false));
                        break;

                    case (int) Database.DBIndices.UserDB:
                        tbc.TabPages.Add("ユーザー定義");
                        DBList.Add((Database.DBIndices) mainTabIndex, addDBTab(Resources.Path_DBs_UserDB, new CtrlComponent.Database.ctlDBGUIUserDB(), tbc) as CtrlComponent.Database.ctlDBBase);
                        ((CtrlComponent.Database.ctlDBGUIUserDB) DBList[Database.DBIndices.UserDB]).DBList = DBList;
                        db = DBList[(Database.DBIndices) mainTabIndex];
                        dbsubs = db.DBs;
                        dbsubs[0].Tag = tbc.TabPages[tbc.TabCount - 1].Text;

                        //列設定
                        DBLiteral.AddStandardHeadColumns(DBList, dbsubs[0]);
                        Database.AddDBColumn(dbsubs[0], new DBColumnText(DBList, "メモ", 100, false));
                        Database.AddDBColumn(dbsubs[0], new DBColumnTextOneNumber(DBList, "列数", 100, int.MinValue, int.MaxValue, 0));
                        break;
                }
            }
        }

        /// <summary>
        /// 新規データベースタブを生成します。
        /// </summary>
        private static CtrlComponent.Database.ctlDBBase addDBTab(string fileName, CtrlComponent.Database.ctlDBBase ctl, TabControl tbc) {
            tbc.TabPages[tbc.TabCount - 1].Controls.Add(ctl);
            ctl.Dock = DockStyle.Fill;
            ctl.FileName = fileName;

            foreach (var db in ctl.DBs) {
                Database.InitDataGridView(db);
            }

            ctl.IsDirtyChanged += (sender, e) => {
                tbc.Refresh();
            };

            return ctl;
        }

        /// <summary>
        /// 指定したデータベースに可変IDと名前の列を追加します。
        /// </summary>
        /// <param name="db">対象オブジェクト</param>
        public static void AddStandardHeadColumns(AllDB DBList, DataGridView db) {
            DBColumn temp;

            temp = new DBColumnTextOneNumber(DBList, "ID", 60, int.MinValue, int.MaxValue, 0) {
                Frozen = true
            };
            Database.AddDBColumn(db, temp);

            temp = new DBColumnText(DBList, "名前", 150, false) {
                Frozen = true
            };
            Database.AddDBColumn(db, temp);
        }

        /// <summary>
        /// 指定したデータベースに説明欄の列を追加します。
        /// </summary>
        /// <param name="db">対象オブジェクト</param>
        public static void AddCommentColumn(AllDB DBList, DataGridView db) {
            Database.AddDBColumn(db, new DBColumnText(DBList, "注釈", 200, false));
        }

        /// <summary>
        /// 指定したデータベースにスクリプト移行ボタンと説明欄の列を追加します。
        /// </summary>
        /// <param name="db">対象オブジェクト</param>
        public static void AddStandardFootColumns(AllDB DBList, DataGridView db, string scriptFileName) {
            DBLiteral.AddCommentColumn(DBList, db);
            Database.AddDBColumn(db, new DBColumnButtonToScript(DBList, scriptFileName));
        }

        /// <summary>
        /// 指定したデータベースに個別変数の列を追加します。
        /// </summary>
        /// <param name="db">対象オブジェクト</param>
        public static void AddSelfValueColumn(AllDB DBList, DataGridView db, Database.DBValueIndices valueType) {
            Database.AddDBColumn(db, new DBColumnTextIDValues(DBList, "個別変数", 150, new Database.DBAddress(Database.DBIndices.Value, (int) valueType)));
            Database.AddDBColumn(db, new DBColumnButtonInputIDValues(DBList, db.Columns.Count, new Database.DBAddress(Database.DBIndices.Value, (int) valueType)));
        }

        /// <summary>
        /// 指定したデータベースに個別変数、Add-onの列を追加します。
        /// </summary>
        /// <param name="db">対象オブジェクト</param>
        public static void AddSelfValueAndAddonColumns(AllDB DBList, DataGridView db, Database.DBValueIndices valueType) {
            DBLiteral.AddSelfValueColumn(DBList, db, valueType);
            Database.AddDBColumn(db, new DBColumnText(DBList, "アドオン", 150, true));
            Database.AddDBColumn(db, new DBColumnButtonInputAddon(DBList, db.Columns.Count));
        }
    }
}
