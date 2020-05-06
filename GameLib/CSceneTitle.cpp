//###################################################################
//			タイトルシーン定義
//###################################################################
#include "Header.h"
extern void RestoreTilePallet();


//####################  シーン生成時  ####################
void CSceneTitle::Setup() {
	//ゲーム自体の初期化を行う
	Game.MT.Clear(false);
	Game.DB.InitDBType(static_cast<int>(TS::DB::DBType::VDB));
	Game.DB.InitDBType(static_cast<int>(TS::DB::DBType::UDB));
	Game.DrawBright.Set(255, 255, 255);
	Game.Pausing = false;
	Game.BGMKeep = false;
	Game.BGSKeep = false;
	Game.ForbidSceneInput = false;
	Game.ForbidSceneDraw = false;

	//サウンドはすべて停止する
	Game.Material.getSounds()->StopAll();

	CSceneBase::Setup();
}
