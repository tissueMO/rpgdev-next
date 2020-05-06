//###################################################################
//		ショップシーン: グローバル変数
//###################################################################

//メニュー固定
DepthCount <- 13;					//メニュー階層数
ChoiceDepth <- 12;					//選択肢の階層番号


//メニュー汎用
Cursor <- array(DepthCount, 0);		//各階層のカーソル位置
TopIndex <- array(DepthCount, 0);	//各階層のウィンドウ領域にある先頭要素のインデックス
Depth <- 0;							//メニュー階層
ErrorMsg <- "";						//エラーメッセージ内容
AreaMax <- 15;						//リスト領域に表示できる項目数
WindowTransparent <- 0;				//フェードするウィンドウの透明度


//お店処理に必要なもの
ShowAtSign <- true;					//単価記号を表示するかどうか
BuySellSEID <- 0x61301713/*決定音*/;		//売買時の効果音ID


//メニュー項目
MenuItem <- {
	Buy = 0,		//購入
	Sell = 1,		//売却
	Exit = 2,		//終了
};
