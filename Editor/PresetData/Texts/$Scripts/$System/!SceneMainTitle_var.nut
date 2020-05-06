//###################################################################
//		タイトルシーン: グローバル変数
//###################################################################

//メニュー固定
DepthCount <- 13;					//メニュー階層数


//メニュー汎用
Cursor <- array(DepthCount, 0);		//各階層のカーソル位置
TopIndex <- array(DepthCount, 0);	//各階層のウィンドウ領域にある先頭要素のインデックス
Depth <- 0;							//メニュー階層
AreaMax <- 4;						//セーブデータ領域に表示できる項目数
WindowTransparent <- 0;				//フェードするウィンドウの透明度


//タイトル画面に必要なもの
Graphs <- array(10, -1);			//グラフィックハンドル
SaveCount <- Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount);		//セーブデータ数


//項目リスト
MenuItem <- {
	Start = 0,			//はじめから
	Load = 1,			//つづきから
	Exit = 2,			//ゲーム終了
};

//「はじめから」の情報
TitleStart <- {
	MapFileName = "Maps\\01 レオナルドの里\\村外観.map",		//マップ
	PositionX = 22,							//X座標
	PositionY = 6,							//Y座標
	Direction = Direction4.South,			//向き
};
