//###################################################################
//			イベントに関するクラス定義
//###################################################################
#include "Header.h"
bool EVExtraMove(TS_MT);


//####################  イベントページのリソースを読み込む  ####################
void CSceneMap::CMapData::CMapEventData::CMapEventPageData::LoadGraphics(string fileName) {
	//既にグラフィックが読まれている場合は破棄する
	if(!this->Graphics.empty() && !this->IsTilesetGraphics) {
		this->DisposeGraphics();
	}

	this->Graphics = fileName;
	if(fileName.find(":") == string::npos) {
		//タイルグラフィックでない場合のみ読み込む
		this->GraphHandle = Game.Material.getGraphics()->Add(fileName);
		this->IsTilesetGraphics = false;
		CSize graphSize;
		DxLib::GetGraphSize(this->GraphHandle, &graphSize.Width, &graphSize.Height);
		this->OneCharSize.Set(
			graphSize.Width / static_cast<int>(TS::Map::EventAnimation::Count),
			graphSize.Height / static_cast<int>(TS::Map::Direction4::None)
			);
	} else {
		this->GraphHandle = -1;
		this->IsTilesetGraphics = true;
		this->TilePalletPosition.Set(this->Graphics);
	}
}


//####################  イベントページのリソースを破棄する  ####################
void CSceneMap::CMapData::CMapEventData::CMapEventPageData::DisposeGraphics() {
	Game.Material.getGraphics()->Delete(this->GraphHandle);
	this->GraphHandle = -1;
	this->Graphics.clear();
}


//####################  有効なページのインデックスを設定する  ####################
// -1 を設定すると非表示状態となる
void CSceneMap::CMapData::CMapEventData::SetPageIndex(int val) {
	TS_CHECKINDEX(val, -1, this->GetPageCount()) {
		//変更されるときのみ処理する
		if(this->pageIndex != val) {
			this->pageIndex = val;
			this->moveTask.clear();
			this->MoveTaskNowIndex = -1;
			this->routeLock = false;
			this->TargetCharPTR = this->getCurrentPage();

			//日常移動のスクリプトを読み込む（ダミーイベントを除く）
			if(val != -1 && !this->IsDummy) {
				Game.getSceneMap()->MapData.RunEV(this->ID, static_cast<int>(TS::Map::EventScriptType::Move), false);
			}
		}
	}
}


//####################  有効なページのインデックスを取得する  ####################
int CSceneMap::CMapData::CMapEventData::GetPageIndex() {
	return this->pageIndex;
}


//####################  ページ数を取得する  ####################
int CSceneMap::CMapData::CMapEventData::GetPageCount() {
	return this->pages.size();
}


//####################  ページを追加する  ####################
void CSceneMap::CMapData::CMapEventData::AddPage(CMapEventPageData pg) {
	this->pages.push_back(pg);
}


//####################  日常移動のタスクを追加する  ####################
CSceneMap::CMapData::CMapEventData* CSceneMap::CMapData::CMapEventData::AddMoveTask(int type, int par1, string par2) {
	this->moveTask.push_back(CEventMoveTask(type, par1, par2));
	return this;
}


//####################  イベントの向きを設定する  ####################
void CSceneMap::CMapData::CMapEventData::SetDirection4(int dir4) {
	if(this->getCurrentPage() != nullptr && !this->getCurrentPage()->FixedDirection) {
		this->getCurrentPage()->setDirection(dir4);
	}
}


//####################  主人公の方を向かせる  ####################
void CSceneMap::CMapData::CMapEventData::SetDirection4ToPlayer() {
	this->getCurrentPage()->setDirection(this->GetDirection4ToPlayer());
}


//####################  主人公に背を向けさせる  ####################
void CSceneMap::CMapData::CMapEventData::SetDirection4AgainstPlayer() {
	this->getCurrentPage()->setDirection(CSceneMap::TurnHalf4(this->GetDirection4ToPlayer()));
}


//####################  主人公への向きを取得する  ####################
int CSceneMap::CMapData::CMapEventData::GetDirection4ToPlayer() {
	int dir = static_cast<int>(TS::Map::Direction4::None);
	if(this->getCurrentPage() != nullptr && !this->getCurrentPage()->FixedDirection) {
		//主人公との距離を算出
		CPoint distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
		if(distance.X < 0 && abs(distance.X) > abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::West);
		} else if(distance.X > 0 && abs(distance.X) > abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::East);
		} else if(distance.Y < 0 && abs(distance.X) < abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::North);
		} else if(distance.Y > 0 && abs(distance.X) < abs(distance.Y)) {
			dir = static_cast<int>(TS::Map::Direction4::South);
		}
	}
	return dir;
}


//####################  指定した方向に進むために必要なMoveCounterの値をセットする  ####################
void CSceneMap::CMapData::CMapObjectData::AutoSetMoveCounter(int direction4) {
	CPoint preMove = CSceneMap::Direction4ToPoint(direction4);
	this->MoveCounter.X = (preMove.X < 0) ? +Game.getSceneMap()->MapData.TileSize : (preMove.X > 0) ? -Game.getSceneMap()->MapData.TileSize : 0;
	this->MoveCounter.Y = (preMove.Y < 0) ? +Game.getSceneMap()->MapData.TileSize : (preMove.Y > 0) ? -Game.getSceneMap()->MapData.TileSize : 0;
}


//####################  現在有効なページを取得する  ####################
CSceneMap::CMapData::CMapEventData::CMapEventPageData* CSceneMap::CMapData::CMapEventData::getCurrentPage() {
	if(this->pageIndex == -1) {
		return nullptr;
	} else {
		return &this->pages[this->pageIndex];
	}
}


//####################  イベント方向移動の指示を行う  ####################
//返値で自分のイベントへの参照を返すため、連続的に呼び出してセットできるようにする
CSceneMap::CMapData::CMapObjectData* CSceneMap::CMapData::CMapObjectData::SetExtraMove(int type, int par1, string par2) {
	this->moveExtraTask.push_back(CEventMoveTask(type, par1, par2));
	return this;
}


//####################  マップの範囲外にいたらマップ上のループ設定に従って補正する  ####################
void CSceneMap::CMapData::CMapObjectData::ApplyMapLoopPosition() {
	if(Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Horizontal)
	   || Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
		//X軸方向のループ
		if(this->Position.X < 0) {
			this->Position.X = Game.getSceneMap()->MapData.MapSize.Width - 1;
		} else if(Game.getSceneMap()->MapData.MapSize.Width <= this->Position.X) {
			this->Position.X = 0;
		}
	}
	if(Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Vertical)
	   || Game.getSceneMap()->MapData.MapLoopOption == static_cast<int>(TS::Map::MapLoopType::Both)) {
		//Y軸方向のループ
		if(this->Position.Y < 0) {
			this->Position.Y = Game.getSceneMap()->MapData.MapSize.Height - 1;
		} else if(Game.getSceneMap()->MapData.MapSize.Height <= this->Position.Y) {
			this->Position.Y = 0;
		}
	}
}


//####################  イベント方向移動を実行する  ####################
void CSceneMap::CMapData::CMapObjectData::DoExtraMove(bool waitMode) {
	if(this->moveExtraTask.size() == 0) {
		return;		//タスクがない場合は処理しない
	}

	this->MoveWaitTimer = 0;		//移動待機タイマーを初期化する
	if(waitMode) {
		//移動が終わるまで待つ場合
		for(int i = 0; i < this->moveExtraTask.size();) {
			//現在のインデックスのタスクを実行する
			i = this->moveScript(this->moveExtraTask[i], i, false);

			//一度画面を更新し、単一移動が終わるまで待つ
			do {
				Game.DoSceneDraw();
				TS_EVERYLOOP;
			} while(this->getIsMoving());
		}
		this->moveExtraTask.clear();
	} else {
		Game.MT.AddThread("EVExtraMove", ::EVExtraMove, TS::System::Scene::Map, false)
			.AddInteger("ID", this->ID)
			.AddInteger("idx", 0);
	}
}


//####################  イベント方向移動を並列的に実行する  ####################
//Vint["ID"] = イベントID
//Vint["idx"] = 現在実行中のタスクインデックス
bool EVExtraMove(TS_MT) {
	CSceneMap::CMapData::CMapObjectData* ev = nullptr;
	if(*Data.GetInteger("ID") == -1) {
		ev = &Game.getSceneMap()->MapData.Player.MapObject;
	} else {
		ev = Game.getSceneMap()->MapData.EVs.GetFromID(*Data.GetInteger("ID"));
	}
	if(ev == nullptr) {
		return true;		//イベントが存在しないので終了
	}

	if(!ev->getIsMoving()) {
		//現在の移動タスクを実行する
		*Data.GetInteger("idx") = ev->moveScript(ev->getExtraMoveTask(*Data.GetInteger("idx")), *Data.GetInteger("idx"), false);
	}
	if(*Data.GetInteger("idx") <= ev->getExtraMoveTaskCount()) {
		return false;		//まだ終わっていない
	}

	//完了
	ev->clearExtraMoveTask();
	return true;
}


//####################  すべてのイベントページのリソースを破棄する  ####################
void CSceneMap::CMapData::CMapEventData::Dispose() {
	for(auto& pg : this->pages) {
		pg.DisposeGraphics();
	}
	vector<CMapEventPageData>().swap(this->pages);
	this->TargetCharPTR = nullptr;
}


//####################  セーブデータからイベント情報を読み込んでインスタンスを生成する  ####################
CSceneMap::CMapData::CMapEventData::CMapEventData(vector<string>& buf) {
	string temp;
	vector<string> spl;
	int pgCount = 0;

	//イベント情報：数値部
	temp = Game.PopHead(buf);
	Game.SplitString(spl, temp, ":");
	this->ID = atoi(Game.PopHead(spl).c_str());
	this->VisibleID = atoi(Game.PopHead(spl).c_str());
	pgCount = atoi(Game.PopHead(spl).c_str());
	this->pageIndex = atoi(Game.PopHead(spl).c_str());
	this->Position.X = atoi(Game.PopHead(spl).c_str());
	this->Position.Y = atoi(Game.PopHead(spl).c_str());
	this->Hide = atoi(Game.PopHead(spl).c_str());
	this->MoveTaskNowIndex = atoi(Game.PopHead(spl).c_str());

	//イベント情報：文字列部
	temp = Game.PopHead(buf);
	this->Name = temp;

	//ページ情報
	TS_COUNTLOOP(i, pgCount) {
		CMapEventPageData page;

		//ページ情報：数値部
		temp = Game.PopHead(buf);
		Game.SplitString(spl, temp, ":");
		page.TriggerType = atoi(Game.PopHead(spl).c_str());
		page.MoveType = atoi(Game.PopHead(spl).c_str());
		page.Direction = atoi(Game.PopHead(spl).c_str());
		page.MoveSpeed = atoi(Game.PopHead(spl).c_str());
		page.MoveFrequency = atoi(Game.PopHead(spl).c_str());
		page.AnimID = atoi(Game.PopHead(spl).c_str());
		page.DrawPriority = atoi(Game.PopHead(spl).c_str());
		page.HitRect.Width = atoi(Game.PopHead(spl).c_str());
		page.HitRect.Height = atoi(Game.PopHead(spl).c_str());
		page.EnabledStopAnim = atoi(Game.PopHead(spl).c_str());
		page.EnabledWalkAnim = atoi(Game.PopHead(spl).c_str());
		page.NoHit = atoi(Game.PopHead(spl).c_str());
		page.FixedDirection = atoi(Game.PopHead(spl).c_str());

		//ページ情報：文字列部
		page.LoadGraphics(Game.PopHead(buf));

		this->pages.push_back(page);
	}
}


//####################  イベント情報をセーブデータに書き込む  ####################
void CSceneMap::CMapData::CMapEventData::toString(string& buf) {
	//イベント情報
	buf += Game.Format("%d:%d:%d:%d:%d:%d:%d:%d\n",
					   this->ID,
					   this->VisibleID,
					   this->GetPageCount(),
					   this->pageIndex,
					   this->Position.X,
					   this->Position.Y,
					   this->Hide,
					   this->MoveTaskNowIndex
					   );
	buf += Game.Format("%s\n", this->Name.c_str());

	//ページ情報
	for(auto& page : this->pages) {
		buf += Game.Format("%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\n",
						   page.TriggerType,
						   page.MoveType,
						   page.Direction,
						   page.MoveSpeed,
						   page.MoveFrequency,
						   page.AnimID,
						   page.DrawPriority,
						   page.HitRect.Width,
						   page.HitRect.Height,
						   page.EnabledStopAnim,
						   page.EnabledWalkAnim,
						   page.NoHit,
						   page.FixedDirection
						   );
		buf += Game.Format("%s\n", page.Graphics.c_str());
	}
}


//####################  イベント情報をログ出力用のデータにして返す  ####################
string CSceneMap::CMapData::CMapEventData::toString() {
	string buf;

	buf = Game.Format("%d (%s)\n\tPageIdx:%d\tPos(%d, %d)\t向き:%d\n",
					  this->ID,
					  this->Name.c_str(),
					  this->pageIndex,
					  this->Position.X,
					  this->Position.Y,
					  this->Hide
					  );

	//アクティブなページの情報だけを使う
	if(this->pageIndex != -1) {
		auto page = this->getCurrentPage();
		buf += Game.Format("\t起動条件:%d\t日常移動:%d\t速度:%d\t頻度:%d\n\tアニメID:%d\t描画優先度:%d\t接触範囲:%dx%d\t静止アニメ:%d\t歩行アニメ:%d\tすり抜け:%d\t向き固定:%d\n\tグラフィック:\"%s\"\n",
						   page->TriggerType,
						   page->MoveType,
						   page->MoveSpeed,
						   page->MoveFrequency,
						   page->AnimID,
						   page->DrawPriority,
						   page->HitRect.Width,
						   page->HitRect.Height,
						   page->EnabledStopAnim,
						   page->EnabledWalkAnim,
						   page->NoHit,
						   page->FixedDirection,
						   page->Graphics.c_str()
						   );
	}

	//個別変数情報
	buf += "\t個別変数: ";
	for(auto value : this->SelfValues.data.Items) {
		buf += Game.Format("ID:%d(%s)=%d  ",
						   value.second.ID,
						   Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfEvent))->GetRowFromID(value.second.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
						   value.second.Value
						   );
	}
	buf += "\n";
	return buf;
}


//####################  イベント日常移動の次のタスクを取り出す  ####################
CSceneMap::CMapData::CEventMoveTask* CSceneMap::CMapData::CMapEventData::getNextMoveTask() {
	TS_CHECKINDEX(this->MoveTaskNowIndex + 1, 0, this->moveTask.size()) {
		return &this->moveTask[this->MoveTaskNowIndex + 1];
	} else {
		return nullptr;
	}
}


//####################  イベント方向移動の指定したインデックスのタスクを取得する  ####################
CSceneMap::CMapData::CEventMoveTask CSceneMap::CMapData::CMapObjectData::getExtraMoveTask(int index) {
	TS_CHECKINDEX(index, 0, this->moveExtraTask.size()) {
		return this->moveExtraTask[index];
	}
	return CEventMoveTask();
}


//####################  ランダム移動を開始する  ####################
void CSceneMap::CMapData::CMapEventData::moveRandom() {
	CPoint preMove;

	//初期位置から10x10の範囲内でランダム移動する
	do {
		preMove.Set(0, 0);
		preMove = CSceneMap::Direction4ToPoint(Game.NextRand(0, static_cast<int>(TS::Map::Direction4::None) - 1));
	} while(
		this->relativePosition.X + preMove.X < -TS::Map::EVRandomMoveRange
		|| this->relativePosition.Y + preMove.Y < -TS::Map::EVRandomMoveRange
		|| this->relativePosition.X + preMove.X > TS::Map::EVRandomMoveRange
		|| this->relativePosition.Y + preMove.Y > TS::Map::EVRandomMoveRange
		);

	//実際に移動させる: ここで同時に接触判定を行う
	if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), true)) {
		int dir4 = CSceneMap::PointToDirection4(preMove);
		this->Position = this->Position + preMove;
		this->relativePosition = this->relativePosition + preMove;
		this->AutoSetMoveCounter(dir4);
		this->SetDirection4(dir4);
	}
}


//####################  主人公に近づく移動を開始する  ####################
void CSceneMap::CMapData::CMapEventData::moveNear() {
	CPoint preMove, distance;

	//主人公との距離を算出
	distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;

	if(distance.X != 0 && distance.Y != 0) {
		//縦・横どちらでも移動できる: 上下/左右はランダムに決める
		switch(Game.NextRand(0, 1)) {
			case 0:		//横移動
				preMove.X = (distance.X < 0) ? -1 : +1;
				break;
			case 1:		//縦移動
				preMove.Y = (distance.Y < 0) ? -1 : +1;
				break;
		}
	} else if(distance.X != 0) {
		//横方向でのみ移動
		preMove.X = (distance.X < 0) ? -1 : +1;
	} else if(distance.Y != 0) {
		//縦方向でのみ移動
		preMove.Y = (distance.Y < 0) ? -1 : +1;
	}

	//実際に移動させる: ここで同時に接触判定を行う
	if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), true)) {
		int dir4 = CSceneMap::PointToDirection4(preMove);
		this->Position = this->Position + preMove;
		this->AutoSetMoveCounter(dir4);
		this->SetDirection4(dir4);
	}
}


//####################  主人公から遠ざかる移動を開始する  ####################
void CSceneMap::CMapData::CMapEventData::moveAway() {
	CPoint preMove, distance;
	int Xmove = (Game.NextRand(1, 100) <= 50);

	//主人公との距離を算出
	distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;

	//自分を基点として主人公がいる象限に行かないようにする
	if(distance.X == 0 && abs(distance.Y) != 0) {
		//縦方向で並んでいる状態: Yで逃げてXはランダム移動
		if(Xmove) {
			preMove.X = (Game.NextRand(1, 100) <= 50) ? -1 : +1;
		} else {
			preMove.Y = (distance.Y > 0) ? -1 : +1;
		}
	} else if(abs(distance.X) != 0 && distance.Y == 0) {
		//横方向で並んでいる状態: Xで逃げてYはランダム移動
		if(Xmove) {
			preMove.X = (distance.X > 0) ? -1 : +1;
		} else {
			preMove.Y = (Game.NextRand(1, 100) <= 50) ? -1 : +1;
		}
	} else if(distance.X > 0 && distance.Y > 0) {
		//第１象限
		if(Xmove) {
			preMove.X = -1;
		} else {
			preMove.Y = -1;
		}
	} else if(distance.X < 0 && distance.Y > 0) {
		//第２象限
		if(Xmove) {
			preMove.X = +1;
		} else {
			preMove.Y = -1;
		}
	} else if(distance.X < 0 && distance.Y < 0) {
		//第３象限
		if(Xmove) {
			preMove.X = +1;
		} else {
			preMove.Y = +1;
		}
	} else if(distance.X > 0 && distance.Y < 0) {
		//第４象限
		if(Xmove) {
			preMove.X = -1;
		} else {
			preMove.Y = +1;
		}
	}

	//実際に移動させる: ここで同時に接触判定を行う
	if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), true)) {
		int dir4 = CSceneMap::PointToDirection4(preMove);
		this->Position = this->Position + preMove;
		this->AutoSetMoveCounter(dir4);
		this->SetDirection4(dir4);
	}
}


//####################  スクリプト指定の移動を開始する  ####################
//返値は次に実行すべきタスクインデックス
int CSceneMap::CMapData::CMapObjectData::moveScript(CEventMoveTask task, int tasknowIndex, bool EVTrigger) {
	int temp;
	CPoint distance, preMove;
	auto charObj = this->TargetCharPTR;
	if(charObj == nullptr) {
		return -1;
	}
	auto EVchar = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(charObj);

	switch((TS::Map::EventMoveTask)task.TaskType) {
		case TS::Map::EventMoveTask::Wait:
			//オプションの単位はミリ秒
			if(this->MoveWaitTimer <= 0) {
				this->MoveWaitTimer = DxLib::GetNowCount();		//ウェイトのスタート時刻をセット
			}
			if(::GetNowCount() - this->MoveWaitTimer < task.OptionInteger) {
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			this->MoveWaitTimer = 0;			//タイマー初期化して次へ進む
			break;

		case TS::Map::EventMoveTask::Repeat:
			return -1;		//最初のインデックスへ移動する

		case TS::Map::EventMoveTask::Forward:		//一歩前進
			preMove = CSceneMap::Direction4ToPoint(charObj->getDirection());
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(charObj->getDirection());
			} else if(this->routeLock) {
				//障害物に当たったとき、経路固定の場合は次のフレームで再度移動を試みる
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			break;
		case TS::Map::EventMoveTask::Backward:		//一歩後退
			temp = CSceneMap::TurnHalf4(charObj->getDirection());
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);		//向き固定だと変更されないこともある
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(charObj->getDirection());
			} else if(this->routeLock) {
				//障害物に当たったとき、経路固定の場合は次のフレームで再度移動を試みる
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			break;
		case TS::Map::EventMoveTask::Move:			//指定方向へ一歩進む
			preMove = CSceneMap::Direction4ToPoint(task.OptionInteger);
			charObj->setDirection(task.OptionInteger);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(task.OptionInteger);
			} else if(this->routeLock) {
				//障害物に当たったとき、経路固定の場合は次のフレームで再度移動を試みる
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			break;
		case TS::Map::EventMoveTask::MoveRandom:	//ランダム方向へ一歩進む
			temp = Game.NextRand(0, static_cast<int>(TS::Map::Direction4::None) - 1);
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(temp);
			} else if(this->routeLock) {
				//障害物に当たったとき、経路固定の場合は次のフレームで再度移動を試みる
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			break;
		case TS::Map::EventMoveTask::MoveToPlayer:			//主人公側へ一歩進む
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			temp = CSceneMap::PointToDirection4(distance);
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(temp);
			} else if(this->routeLock) {
				//障害物に当たったとき、経路固定の場合は次のフレームで再度移動を試みる
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			break;
		case TS::Map::EventMoveTask::MoveAgainstPlayer:	//反・主人公側へ一歩進む
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			distance.X = -distance.X;
			distance.Y = -distance.Y;		//主人公とは反対方向にする
			temp = CSceneMap::PointToDirection4(distance);
			preMove = CSceneMap::Direction4ToPoint(temp);
			charObj->setDirection(temp);
			if(Game.getSceneMap()->MapData.CheckBeforeMove(this->ID, this->Position.X, this->Position.Y, CSceneMap::PointToDirection4(preMove), EVTrigger)) {
				this->Position = this->Position + preMove;
				this->AutoSetMoveCounter(temp);
			} else if(this->routeLock) {
				//障害物に当たったとき、経路固定の場合は次のフレームで再度移動を試みる
				return tasknowIndex;			//次のタスクに移らずに次のフレームまで待つ
			}
			break;

		case TS::Map::EventMoveTask::Direction:
			charObj->setDirection(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::Turn180:
			charObj->setDirection(CSceneMap::TurnHalf4(charObj->getDirection()));
			break;
		case TS::Map::EventMoveTask::TurnLeft:
			charObj->setDirection(CSceneMap::TurnLeft4(charObj->getDirection()));
			break;
		case TS::Map::EventMoveTask::TurnRight:
			charObj->setDirection(CSceneMap::TurnRight4(charObj->getDirection()));
			break;
		case TS::Map::EventMoveTask::TurnRandom:
			charObj->setDirection(Game.NextRand(0, static_cast<int>(TS::Map::Direction4::None) - 1));
			break;

		case TS::Map::EventMoveTask::TurnToPlayer:
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			charObj->setDirection(CSceneMap::PointToDirection4(distance));
			break;

		case TS::Map::EventMoveTask::TurnAgainstPlayer:
			distance = Game.getSceneMap()->MapData.Player.MapObject.Position - this->Position;
			charObj->setDirection(CSceneMap::TurnHalf4(CSceneMap::PointToDirection4(distance)));
			break;

		case TS::Map::EventMoveTask::EnabledStopAnim:
			charObj->EnabledStopAnim = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::EnabledWalkAnim:
			charObj->EnabledWalkAnim = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::FixedDirection:
			charObj->FixedDirection = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::NoHit:
			charObj->NoHit = Game.IntToBool(task.OptionInteger);
			break;

		case TS::Map::EventMoveTask::Hide:
			this->Hide = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::LockRoute:
			this->routeLock = Game.IntToBool(task.OptionInteger);
			break;
		case TS::Map::EventMoveTask::SetMoveFrequency:
			if(EVchar != nullptr) {
				//イベントページが操作対象であるときのみ実行
				EVchar->MoveFrequency = task.OptionInteger;
			}
			break;
		case TS::Map::EventMoveTask::SetMoveSpeed:
			if(EVchar != nullptr) {
				//イベントページが操作対象であるときのみ実行
				EVchar->MoveSpeed = task.OptionInteger;
			}
			break;

		case TS::Map::EventMoveTask::ChangeGraphics:
			if(EVchar != nullptr) {
				//イベントページが操作対象であるときのみ実行
				EVchar->LoadGraphics(task.OptionString);
			}
			break;

		case TS::Map::EventMoveTask::SQ:
			//イベントハンドラーとしてスクリプトを呼び出す
			bool result = Game.SQ.DoFunc<bool>(TS::SQ::SQEvent_EVMove, this->ID, task.OptionInteger);
			if(!result) {
				return tasknowIndex;		//falseが返った場合は次のフレームで再試行する
			}
			break;
	}
	this->ApplyMapLoopPosition();		//マップの範囲外に出たらマップのループ設定に従って補正する
	return tasknowIndex + 1;
}
