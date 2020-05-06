//###################################################################
//		メニューシーン: グローバル変数
//###################################################################

//メニュー固定
DepthCount <- 13;					//メニュー階層数
TargetMemberDepth <- 10;			//対象キャラ選択の階層番号
PlaceDepth <- 11;					//場所選択の階層番号
ChoiceDepth <- 12;					//選択肢の階層番号
MaxStatusSubListCount <- 11;		//ステータスメニューのサブリスト欄に表示できる項目数
SaveAreaMax <- 8;					//セーブリスト領域に表示できる最大数


//メニュー汎用
Cursor <- array(DepthCount, 0);		//各階層のカーソル位置
TopIndex <- array(DepthCount, 0);	//各階層のウィンドウ領域にある先頭要素のインデックス
Depth <- 0;							//メニュー階層
ErrorMsg <- "";						//エラーメッセージ内容
AreaMax <- 17;						//リスト領域に表示できる項目数
WindowTransparent <- 0;				//フェードするウィンドウの透明度


//メニュー独自
TargetMemberSelecting <- false;		//対象キャラ選択中かどうか
PlaceSelecting <- false;			//場所選択中かどうか
PlaceList <- null;					//選択可能な場所リスト (CIDNameData)
OneMenuMode <- false;				//単体メニューを有効にしてサイドのコマンド一覧を非表示にするかどうか


//描画情報
LeftOffset <- 350;					//コマンドメニュー分のオフセット
MeterOffset <- 110;					//メーター用のオフセット
MeterWidth <- 150;					//メーター幅
MeterHeight <- 10;					//メーター高さ


//装備メニューで必要なもの
EquipTypeList <- array(0);			//装備大種別のIDリスト
EquipList <- array(0);				//装備大種別で絞り込まれた装備品リスト (CIDValueData)


//対象メンバー選択の種別
SelectMemberType <- {
	One = 0,		//味方一人
	OneDead = 1,	//味方一人・死者含む
	All = 2,		//味方全員
};


//メニュー項目
MenuItem <- {
	Status = 0,		//ステータス
	Item = 1,		//アイテム
	Equip = 2,		//装備
	Skill = 3,		//スキル
	Member = 4,		//並び替え
	Save = 5,		//セーブ
	Option = 6,		//環境設定
};


//オプション項目
OptionMenu <- {
	MapMessageSpeed = 0,		//会話メッセージ表示速度
	BattleMessageSpeed = 1,		//戦闘メッセージ表示速度
	SoundVolume = 2,			//サウンド音量
};
