//###################################################################
//			�����萔��`
//###################################################################
#include "ConstantKeys.h"

namespace TS {

	//####################  �V�X�e��  ####################
	namespace System {
		const string DXArchiveExt = "arc";
	}

	//####################  �t�H���_�[��  ####################
	namespace DirectoryNames {
		//�t�H���_�[�Q
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


	//####################  �t�@�C����  ####################
	namespace FileNames {
		const string SQCompileLogFileName = "RPGDeveloperNEXT_SQCompileResult.log";			//�X�N���v�g�R���p�C���̌��ʃt�@�C��

		//�풓�X�N���v�g
		const string SQEventHandler = "Texts\\$Scripts\\$System\\$EventHandler.nut";		//�C�x���g�h���u������
		const string SQConstants = "Texts\\$Scripts\\$System\\$Constants.nut";				//�萔��`���܂Ƃ߂��X�N���v�g
		const string SQFunctions = "Texts\\$Scripts\\$System\\$Functions.nut";				//�ėp�֐��̒�`���܂Ƃ߂��X�N���v�g
		const string SQMapEvents = "Texts\\$Scripts\\$MapEvents\\%s.nut";					//�}�b�v�̃X�N���v�g

		//�V�[���X�N���v�g
		const string SQScenes[] = {
			"Texts\\$Scripts\\$System\\$SceneMainTitle.nut",
			"Texts\\$Scripts\\$System\\$SceneMainMap.nut",
			"Texts\\$Scripts\\$System\\$SceneSubMenu.nut",
			"Texts\\$Scripts\\$System\\$SceneSubShop.nut",
			"Texts\\$Scripts\\$System\\$SceneSubBattle.nut",
		};

		//�f�[�^�x�[�X�X�N���v�g
		const string SQDBEffect = "Databases\\$Effects\\%d.nut";					//�G�t�F�N�g�̍Đ���� [ID.nut]

		//�Œ�f�[�^�x�[�X
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

		//�σf�[�^�x�[�X
		const string VDBs[] = {
			"Databases\\Value.tsv",
			"Databases\\Init.tsv",
		};

		//���[�U�[�f�[�^�x�[�X
		const string UserDB = "Databases\\UserDB.tsv";

		//�^�C���Z�b�g�f�[�^
		const string TilesetData = "Databases\\$Tilesets\\%d.tile";

		//���[�U�[DB
		const string UserDBData = "Databases\\$UserDatabases\\%d.tsv";
	}

	namespace DB {
		//NOTE: �G�f�B�^�[�̗񖼂�ύX����Ƃ��́A�ȉ��ɊY�����Ȃ����m�F���邱�ƁB
		const string ColumnNameAttribute = "����";
		const string ColumnNameParameter = "�p�����[�^�[";
		const string ColumnNameParameterRate = "�p�����[�^�[�ϓ���";
		const string ColumnNameSelfValue = "�ʕϐ�";
		const string ColumnNameAddon = "�A�h�I��";
		const string ColumnNameGraphics = "�O���t�B�b�N";
		const string ColumnNameSkill = "�K���X�L��";

		const string ColumnNameCharTitle = "������";
		const string ColumnNameCharLevel = "���x��";
		const string ColumnNameCharIsNPC = "�����퓬";
		const string ColumnNameCharEquip = "�����i";
		const string ColumnNameCharClass = "�N���X";
		const string ColumnNameCharEXPCurve = "�o���l�Ȑ�";

		const string ColumnNameSkillType = "���";

		const string ColumnNameItemType = "�A�C�e������";
		const string ColumnNameItemSubType = "�A�C�e�������";
		const string ColumnNameItemPrice = "���i";

		const string ColumnNameStateLimit = "����";
		const string ColumnNameStateUpper = "��ʌ݊�";
		const string ColumnNameStateBad = "�ُ�";

		const string ColumnNameEffectTargetType = "�`��Ώ�";

		SQ_CONST const string ColumnNameMaterialHandle = "Handle";

		//�V�X�e���f�[�^�x�[�X�̐ݒ荀�ږ�
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


	//####################  �Z�[�u�f�[�^  ####################
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


	//####################  �X�N���v�g  ####################
	namespace SQ {
		const string UserFlagName = "UF%d";			//���ʕϐ����̏���
		const string UserIntegerName = "UI%d";
		const string UserStringName = "US%d";

		//�C�x���g�n���h���[�X�N���v�g
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

		//�V�[���X�N���v�g
		SQ_EVENTHANDLER(SQEvent_SceneMainDraw, "SceneMainDraw", void);
		SQ_EVENTHANDLER(SQEvent_SceneMainInput, "SceneMainInput", void);
		SQ_EVENTHANDLER(SQEvent_SceneMainSetup, "SceneMainSetup", void);
		SQ_EVENTHANDLER(SQEvent_SceneMainDispose, "SceneMainDispose", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubDraw, "SceneSubDraw", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubInput, "SceneSubInput", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubSetup, "SceneSubSetup", void);
		SQ_EVENTHANDLER(SQEvent_SceneSubDispose, "SceneSubDispose", void);

		//�G�t�F�N�g�X�N���v�g
		SQ_EVENTHANDLER_NOBIND(SQEvent_PlayEffect, "PlayEffect", void, intEffectHandle, intFrameIndex, intFrameTimes);

		//�}�b�v�C�x���g�X�N���v�g
		SQ_EVENTHANDLER_NOBIND(SQEvent_SetupMapEvents, "SetupMapEvents", void);
		SQ_EVENTHANDLER_NOBIND(SQEvent_DoMapEvent, "EV%d", bool, intType, intPageIndex);
	}
}
