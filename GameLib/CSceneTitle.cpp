//###################################################################
//			�^�C�g���V�[����`
//###################################################################
#include "Header.h"
extern void RestoreTilePallet();


//####################  �V�[��������  ####################
void CSceneTitle::Setup() {
	//�Q�[�����̂̏��������s��
	Game.MT.Clear(false);
	Game.DB.InitDBType(static_cast<int>(TS::DB::DBType::VDB));
	Game.DB.InitDBType(static_cast<int>(TS::DB::DBType::UDB));
	Game.DrawBright.Set(255, 255, 255);
	Game.Pausing = false;
	Game.BGMKeep = false;
	Game.BGSKeep = false;
	Game.ForbidSceneInput = false;
	Game.ForbidSceneDraw = false;

	//�T�E���h�͂��ׂĒ�~����
	Game.Material.getSounds()->StopAll();

	CSceneBase::Setup();
}
