//###################################################################
//			文字定数定義
//###################################################################
#include "ConstantKeys.h"

namespace TS {

	//####################  システム  ####################
	namespace System {
		const string DXArchiveExt = "arc";
	}

	//####################  フォルダー名  ####################
	namespace DirectoryNames {
		//フォルダー群
#ifdef _DEBUG
		const string PrePath = "";
#else
		const string PrePath = "Data\\";
#endif

		const string ScreenShot = "ScreenShots\\";
		const string SaveData = "SaveFiles\\";
		const string Log = "Logs\\";
		const string Database = "Databases\\$UserDatabases\\";
	}


	//####################  ファイル名  ####################
	namespace FileNames {
		const string SQCompileLogFileName = "RPGDeveloperNEXT_SQCompileResult.log";			//スクリプトコンパイルの結果ファイル

		//常駐スクリプト
		const string SQEventHandler = "Texts\\$Scripts\\$System\\$EventHandler.nut";		//イベントドリブン処理
		const string SQConstants = "Texts\\$Scripts\\$System\\$Constants.nut";				//定数定義をまとめたスクリプト
		const string SQFunctions = "Texts\\$Scripts\\$System\\$Functions.nut";				//汎用関数の定義をまとめたスクリプト
		const string SQMapEvents = "Texts\\$Scripts\\$MapEvents\\%s.nut";					//マップのスクリプト

		//シーンスクリプト
		const string SQScenes[] = {
			"Texts\\$Scripts\\$System\\$SceneMainTitle.nut",
			"Texts\\$Scripts\\$System\\$SceneMainMap.nut",
			"Texts\\$Scripts\\$System\\$SceneSubMenu.nut",
			"Texts\\$Scripts\\$System\\$SceneSubShop.nut",
			"Texts\\$Scripts\\$System\\$SceneSubBattle.nut",
		};

		//データベーススクリプト
		const string SQDBEffect = "Databases\\$Effects\\%d.nut";					//エフェクトの再生情報 [ID.nut]

		//固定データベース
		const string FDBs[] = {
			"Databases\\System.xml",
			"Databases\\Material.tsv",
			"Databases\\Element.tsv",
			"Databases\\Class.tsv",
			"Databases\\Skill.tsv",
			"Databases\\Item.tsv",
			"Databases\\State.tsv",
			"Databases\\Char.tsv",
			"Databases\\Enemy.tsv",
			"Databases\\Unit.tsv",
			"Databases\\Effect.tsv",
			"Databases\\Tileset.tsv",
			"Databases\\Init.tsv",
		};

		//可変データベース
		const string VDBs[] = {
			"Databases\\Value.tsv",
			"Databases\\Init.tsv",
		};

		//ユーザーデータベース
		const string UserDB = "Databases\\UserDB.tsv";

		//タイルセットデータ
		const string TilesetData = "Databases\\$Tilesets\\%d.tile";

		//ユーザーDB
		const string UserDBData = "Databases\\$UserDatabases\\%d.tsv";
	}

	namespace DB {
		//NOTE: エディターの列名を変更するときは、以下に該当しないか確認すること。
		const string ColumnNameAttribute = "属性";
		const string ColumnNameParameter = "パラメーター";
		const string ColumnNameParameterRate = "パラメーター変動率";
		const string ColumnNameSelfValue = "個別変数";
		const string ColumnNameAddon = "アドオン";
		const string ColumnNameGraphics = "グラフィック";
		const string ColumnNameSkill = "習得スキル";

		const string ColumnNameCharTitle = "肩書き";
		const string ColumnNameCharLevel = "レベル";
		const string ColumnNameCharIsNPC = "自律戦闘";
		const string ColumnNameCharEquip = "装備品";
		const string ColumnNameCharClass = "クラス";
		const string ColumnNameCharEXPCurve = "経験値曲線";

		const string ColumnNameSkillType = "種別";

		const string ColumnNameItemType = "アイテム大種別";
		const string ColumnNameItemSubType = "アイテム小種別";
		const string ColumnNameItemPrice = "価格";

		const string ColumnNameStateLimit = "制約";
		const string ColumnNameStateUpper = "上位互換";
		const string ColumnNameStateBad = "異常";

		const string ColumnNameEffectTargetType = "描画対象";

		SQ_CONST const string ColumnNameMaterialHandle = "Handle";

		//システムデータベースの設定項目名
		namespace System {
			SQ_CONST const string GameTitle = "strGameTitle";
			SQ_CONST const string GameVersion = "strGameVersion";
			SQ_CONST const string WindowSize = "sizeWindowSize";
			SQ_CONST const string AllowFullScreen = "boolAllowFullScreen";
			SQ_CONST const string IconFileName = "strIconFileName";
			SQ_CONST const string NowLoadingFileName = "strNowLoadingFileName";
			SQ_CONST const string StandardTransparentColor = "intStandardTransparentColor";
			SQ_CONST const string DrawInterpolation = "intDrawInterpolation";
			SQ_CONST const string EffectAsyncLoadMode = "boolEffectAsyncLoadMode";
			SQ_CONST const string EffectFPS = "intEffectFPS";
			SQ_CONST const string TileSize = "intTileSize";
			SQ_CONST const string IconSize = "intIconSize";
			SQ_CONST const string MaxPartyCount = "intMaxPartyCount";
			SQ_CONST const string MaxLevel = "intMaxLevel";
			SQ_CONST const string MaxSaveDataCount = "intMaxSaveDataCount";
			SQ_CONST const string ShadowTransparent = "intShadowTransparent";
			SQ_CONST const string FogTransparent = "intFogTransparent";
			SQ_CONST const string EnabledSystemResource = "boolEnabledSystemResource";
			SQ_CONST const string EnabledLimitedDebug = "boolEnabledLimitedDebug";
		}
	}


	//####################  セーブデータ  ####################
	namespace SaveLoad {
		const string VDB = "##### VDB #####";
		const string ValueSplit = "##### ValueSplit #####";
		const string ActorSplit = "##### ActorSplit #####";
		const string VDBSplit = "##### VDBSplit #####";
		const string UDB = "##### UDB #####";
		const string EV = "##### EV #####";
		const string NoMap = "##### NOMAP #####";
		const string MapData = "##### MAPDATA #####";
		const string User = "##### USERDATA #####";
	}


	//####################  スクリプト  ####################
	namespace SQ {
		const string UserFlagName = "UF%d";			//共通変数名の書式
		const string UserIntegerName = "UI%d";
		const string UserStringName = "US%d";

		//イベントハンドラースクリプト
		SQ_EVENTHANDLER(SQEvent_NowLoading, "EventHandler_NowLoading", void, intGraphHandle);
		SQ_EVENTHANDLER(SQEvent_StartMapTester, "EventHandler_StartMapTest", void, strMapFileName, intX, intY);
		SQ_EVENTHANDLER(SQEvent_StartBattleTester, "EventHandler_StartBattleTest", void, intUnitID);
		SQ_EVENTHANDLER(SQEvent_WipeSprite, "EventHandler_WipeSprite", void, spriteData);
		SQ_EVENTHANDLER(SQEvent_RequiredSaveHeader, "EventHandler_RequiredSaveHeader", string);
		SQ_EVENTHANDLER(SQEvent_LoadUserData, "EventHandler_LoadUserData", void, strSaveData);
		SQ_EVENTHANDLER(SQEvent_SaveUserData, "EventHandler_SaveUserData", string);
		SQ_EVENTHANDLER(SQEvent_ShowMessage, "EventHandler_ShowMessage", void, intFaceID, strSpeaker, strMsg);
		SQ_EVENTHANDLER(SQEvent_ChangeMap, "EventHandler_ChangeMap", void, strMapFileName, intX, intY, intDirection4);
		SQ_EVENTHANDLER(SQEvent_EVMove, "EventHandler_EVMove", bool, intEVID, intOption);
		SQ_EVENTHANDLER(SQEvent_DebugCommand, "EventHandler_DebugCommand", bool, strCommandArg);

		SQ_EVENTHANDLER(SQEvent_DBStateStart, "StateStart", void, intStateID, actorData);
		SQ_EVENTHANDLER(SQEvent_DBStateEnd, "StateEnd", void, intStateID, actorData, hasUpper);

		//シーンスクリプト
		SQ_EVENTHANDLER(SQEvent_SceneMainDraw, "SceneMainDraw", void);
		SQ_EVENTHANDLER(SQEvent_SceneMainInput, "SceneMainInput", void);
		SQ_EVENTHANDLER(SQEvent_SceneMainSetup, "SceneMainSetup", void);
		SQ_EVENTHANDLER(SQEvent_SceneMainDispose, "SceneMainDispose", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubDraw, "SceneSubDraw", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubInput, "SceneSubInput", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubSetup, "SceneSubSetup", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubDispose, "SceneSubDispose", void);

		//エフェクトスクリプト
		SQ_EVENTHANDLER_NOBIND(SQEvent_PlayEffect, "PlayEffect", void, intEffectHandle, intFrameIndex, intFrameTimes);

		//マップイベントスクリプト
		SQ_EVENTHANDLER_NOBIND(SQEvent_SetupMapEvents, "SetupMapEvents", void);
		SQ_EVENTHANDLER_NOBIND(SQEvent_DoMapEvent, "EV%d", bool, intType, intPageIndex);
	}
}
