//###################################################################
//			マップデータの中身に関するクラス定義
//###################################################################
#include "Header.h"


//####################  イベントの起動予約を行う  ####################
void CSceneMap::CMapData::ReserveEV(int EVID) {
	this->reservedEVs.push_back(EVID);
}


//####################  指定したイベントのアクティブなページの指定種別のスクリプトを呼び出す  ####################
bool CSceneMap::CMapData::RunEV(int EVID, int type, bool pushRunningStack) {
	return this->RunEV(EVID, this->EVs.GetFromID(EVID)->GetPageIndex(), type, pushRunningStack);
}


//####################  指定したイベントの指定したページの指定種別のスクリプトを呼び出す  ####################
bool CSceneMap::CMapData::RunEV(int EVID, int pageIndex, int type, bool pushRunningStack) {
	bool result = false;

	//既に同一イベントの実行内容が実行中である場合は処理しない
	if(type == static_cast<int>(TS::Map::EventScriptType::Run)) {
		for(auto& ev : this->runningEVNest_system) {
			if(EVID == ev->ID) {
				return false;
			}
		}
	}

	if(pageIndex != -1) {
		//実行中のイベントIDを呼び出し履歴に登録する
		this->runningEVNest_system.insert(this->runningEVNest_system.begin(), this->EVs.GetFromID(EVID));
		if(pushRunningStack) {
			this->runningEVNest.insert(this->runningEVNest.begin(), EVID);
		}

		result = Game.SQ.DoFunc<bool>(
			Game.Format(TS::SQ::SQEvent_DoMapEvent, EVID),
			type,
			pageIndex
			);

		//シーンが変更されて現在のインスタンスが無効になった場合は中断する
		if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
			return false;
		}

		//実行完了したら呼び出し履歴から削除する
		this->runningEVNest_system.erase(this->runningEVNest_system.begin());
		if(pushRunningStack) {
			this->runningEVNest.erase(this->runningEVNest.begin());
		}
	} else {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定されたID [%d] のイベントは現在マップ上で出現状態にありません。", EVID);
	}
	return result;
}


//####################  イベントを追加する  ####################
void CSceneMap::CMapData::SetEV(int EVID, int visibleID, int x, int y, string name) {
	CMapEventData ev;
	ev.ID = EVID;
	ev.VisibleID = visibleID;
	ev.Position.Set(x, y);
	ev.Name = name;
	ev.IsDummy = false;
	ev.SetPageIndex(-1);
	this->EVs.Add(ev);
	this->refreshAllEVTargetChar();
}


//####################  指定したイベントのページを追加する  ####################
void CSceneMap::CMapData::AddEVPage(int EVID, string param, string graph) {
	CMapEventData::CMapEventPageData pg;
	vector<string> buf;
	Game.SplitString(buf, param, ":");

	//文字列で示された数値パラメーターを分解して代入する
	pg.TriggerType = atoi(Game.PopHead(buf).c_str());
	pg.MoveType = atoi(Game.PopHead(buf).c_str());
	pg.Direction = atoi(Game.PopHead(buf).c_str());
	pg.MoveSpeed = atoi(Game.PopHead(buf).c_str());
	pg.MoveFrequency = atoi(Game.PopHead(buf).c_str());
	pg.AnimID = atoi(Game.PopHead(buf).c_str());
	pg.DrawPriority = atoi(Game.PopHead(buf).c_str());
	pg.HitRect.Width = atoi(Game.PopHead(buf).c_str());
	pg.HitRect.Height = atoi(Game.PopHead(buf).c_str());
	pg.EnabledStopAnim = atoi(Game.PopHead(buf).c_str());
	pg.EnabledWalkAnim = atoi(Game.PopHead(buf).c_str());
	pg.NoHit = atoi(Game.PopHead(buf).c_str());
	pg.FixedDirection = atoi(Game.PopHead(buf).c_str());
	pg.LoadGraphics(graph);

	this->EVs.GetFromID(EVID)->AddPage(pg);
}


//####################  ダミーイベントを生成する  ####################
//返値はイベントのFixedID
int CSceneMap::CMapData::SetDummyEV(int x, int y, string graph, int direction4, int animID, int priority, bool enabledStopAnim, bool enabledWalkAnim, bool fixedDirection) {
	//イベントのFixedIDをランダム生成する
	int id;
	do {
		id = Game.NextRand(1, INT_MAX);
	} while(this->EVs.Exists(id));
	this->SetEV(id, -1, x, y, "ダミーイベント");
	auto ev = this->EVGetData(id);

	//既定のページを追加
	CMapEventData::CMapEventPageData pg;
	pg.TriggerType = static_cast<int>(TS::Map::EventTriggerType::None);
	pg.MoveType = static_cast<int>(TS::Map::EventMoveType::None);
	pg.Direction = direction4;
	pg.MoveSpeed = static_cast<int>(TS::Map::Speed::Normal);
	pg.MoveFrequency = static_cast<int>(TS::Map::Speed::Normal);
	pg.AnimID = animID;
	pg.DrawPriority = priority;
	pg.HitRect.Set(0, 0);
	pg.EnabledStopAnim = enabledStopAnim;
	pg.EnabledWalkAnim = enabledWalkAnim;
	pg.NoHit = true;
	pg.FixedDirection = fixedDirection;
	pg.LoadGraphics(graph);
	ev->AddPage(pg);
	ev->SetPageIndex(0);
	ev->IsDummy = true;

	this->refreshAllEVTargetChar();
	return id;
}


//####################  イベントの可視IDからFixedIDを取得する  ####################
int CSceneMap::CMapData::GetEVFixedID(int visibleID) {
	for(auto& ev : this->EVs.Items) {
		if(ev.VisibleID == visibleID) {
			return ev.ID;
		}
	}
	return -1;
}


//####################  イベントをすべて削除する  ####################
void CSceneMap::CMapData::EVClear() {
	while(this->EVs.GetCount() > 0) {
		this->EVs.GetFromIndex(0)->Dispose();
		this->EVs.DeleteFromIndex(0);
	}
	this->EVs.Clear();
}


//####################  イベントを削除する  ####################
void CSceneMap::CMapData::EVDeleteFromID(int id) {
	this->EVs.GetFromID(id)->Dispose();
	this->EVs.DeleteFromID(id);
	this->refreshAllEVTargetChar();
}


//####################  指定キャラを指定した方向に一つ進めるかどうかを検証する  ####################
//EVID=-1 は主人公を示す
//EVTriggerをtrueにすると、主人公とイベントとの間にぶつかり判定が起こったときにトリガーが発生した場合にイベント起動を予約する
bool CSceneMap::CMapData::CheckBeforeMove(int EVID, int x, int y, int direction4, bool EVTrigger) {
	CPoint temp;
	CPoint beforePos(x, y);
	CPoint afterPos;
	this->GetOneMovePosition(beforePos, afterPos, direction4);
	CRectangle range;

	//移動先の座標がマップの外になる場合（ループ設定が行われていない場合）は移動できない
	if(afterPos.X < 0 || afterPos.Y < 0) {
		return false;
	}

	//その他・事前に断定できるもの
	if(EVID != -1 && !this->EVs.GetFromID(EVID)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないイベントID [%d] が参照されました。", EVID);
		return false;
	} else if(EVID != -1 && this->EVs.GetFromID(EVID)->GetPageIndex() == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "指定されたイベント [ID: %d] は現在非表示状態のイベントです。", EVID);
		return false;
	} else if(EVID == -1 && this->Player.CharData.NoHit) {
		return true;		//主人公が当たり判定を持たないときの主人公移動は、常にどこでも移動可能
	} else if(EVID != -1 && this->EVs.GetFromID(EVID)->getCurrentPage() != nullptr && this->EVs.GetFromID(EVID)->getCurrentPage()->NoHit) {
		return true;		//イベントが当たり判定を持たないときは常にどこでも移動可能
	}

	//移動先と移動元のタイルパレット情報を取得しておく
	CDatabase::CDBTileset::CTileData* tile;

	//1. 移動元の通行設定
	for(int l = static_cast<int>(TS::Map::Layer::High); l >= static_cast<int>(TS::Map::Layer::Low); l--) {
		temp = this->Data->at(l)[beforePos.X][beforePos.Y].PalletPosition;
		if(temp.IsEmpty()) {
			//空白タイルは次のループへ
			continue;
		}
		tile = Game.DB.getFDBTileset()->GetTilePallet(temp.X, temp.Y);

		//オートタイルのみ: 同じレイヤーについて、移動先が同じタイルの場合は移動できるとみなす
		if(temp.Y < TS::Map::AutoTileRowLength) {
			if(temp == this->Data->at(l)[afterPos.X][afterPos.Y].PalletPosition) {
				break;
			}
		}

		//それ以外は通行設定を判定する
		if((tile->InOut & (int)pow(2.0, direction4 + 1)) == FALSE) {
			return false;
		} else {
			break;
		}
	}

	//2. 移動先の通行設定
	for(int l = static_cast<int>(TS::Map::Layer::High); l >= static_cast<int>(TS::Map::Layer::Low); l--) {
		temp = this->Data->at(l)[afterPos.X][afterPos.Y].PalletPosition;
		if(temp.IsEmpty()) {
			//空白タイルは次のループへ
			continue;
		}
		tile = Game.DB.getFDBTileset()->GetTilePallet(temp.X, temp.Y);

		//オートタイルのみ: 同じレイヤーについて、移動元と同じタイルの場合は移動できるとみなす
		if(temp.Y < TS::Map::AutoTileRowLength) {
			if(temp == this->Data->at(l)[beforePos.X][beforePos.Y].PalletPosition) {
				break;
			}
		}

		//それ以外は通行設定を判定する
		if((tile->InOut & (int)pow(2.0, Game.getSceneMap()->TurnHalf4(direction4) + 1)) == FALSE) {
			return false;
		} else {
			break;
		}
	}

	//3. 移動先座標にいるイベントとの接触判定: 主人公のみ
	if(EVID == -1) {
		for(auto& ev : this->EVs.Items) {
			if(ev.GetPageIndex() == -1 || ev.getCurrentPage()->NoHit) {
				continue;		//当たり判定のないイベントは無視
			} else if(ev.Position == afterPos) {
				if(ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Push) || ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed)) {
					Game.getSceneMap()->MapData.ReserveEV(ev.ID);
				}
				return false;
			}
		}
	}

	//4. 移動先座標にいる主人公との接触判定: イベントのみ
	if(EVID != -1 && this->Player.CharData.NoHit == false && this->EVs.GetFromID(EVID)->getCurrentPage() != nullptr) {
		if(this->Player.MapObject.Position == afterPos) {
			if(this->EVs.GetFromID(EVID)->getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed)) {
				Game.getSceneMap()->MapData.ReserveEV(EVID);
			}
			return false;
		}
	}

	//5. マップタイルの当たり判定
	return this->IsWalkableTile(afterPos.X, afterPos.Y);
}


//####################  指定キャラの現在位置でイベントが起動するかどうかを検証する  ####################
//EVID=-1 は主人公を示す
//当たり判定の有無を問わず、接触起動するイベントが主人公と一致していたらイベント起動を予約する
bool CSceneMap::CMapData::CheckOverlapTriggerEV(int EVID, int x, int y) {
	CPoint afterPos(x, y);
	CRectangle range;
	bool reserved = false;

	//1. 現在地座標にいるイベントとの一致判定: 主人公のみ
	if(EVID == -1) {
		for(auto& ev : this->EVs.Items) {
			if(ev.GetPageIndex() != -1) {
				auto pg = ev.getCurrentPage();
				range.Set(ev.Position.X - pg->HitRect.Width, ev.Position.Y - pg->HitRect.Height, pg->HitRect.Width * 2 + 1, pg->HitRect.Height * 2 + 1);
				if(range.PointContains(afterPos) && (ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Push) || ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed))) {
					//主人公がイベントに接触したときにイベント起動予約を行う
					this->ReserveEV(ev.ID);
					reserved |= true;
				}
			}
		}
		return reserved;
	}

	//2. 主体がイベントのとき、主人公との一致判定
	if(EVID != -1 && this->EVs.GetFromID(EVID)->getCurrentPage() != nullptr) {
		auto& ev = *this->EVs.GetFromID(EVID);
		auto pg = ev.getCurrentPage();
		range.Set(ev.Position.X - pg->HitRect.Width, ev.Position.Y - pg->HitRect.Height, pg->HitRect.Width * 2 + 1, pg->HitRect.Height * 2 + 1);
		if(range.PointContains(this->Player.MapObject.Position)) {
			if(this->EVs.GetFromID(EVID)->getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Pushed)) {
				//イベントが主人公に接触したときにイベント起動予約を行う
				this->ReserveEV(EVID);
				reserved |= true;
			}
		}
		return reserved;
	}

	return reserved;
}


//####################  現在の主人公の位置で決定キーを押したら起動するイベントがある場合はその起動予約を行う  ####################
bool CSceneMap::CMapData::CheckPressEnterTriggerEV() {
	//主人公の一歩先の座標がマップの外になる場合（ループ設定が行われていない場合）は無効
	CPoint beforePos = this->Player.MapObject.Position;
	CPoint afterPos = CPoint();
	this->GetOneMovePosition(beforePos, afterPos, this->Player.CharData.getDirection());
	if(afterPos.X < 0 || afterPos.Y < 0) {
		return false;
	}

	//当たり判定を持つイベントは隣り合う形で、持たないイベントは一致した状態のときに起動する
	CRectangle range;
	for(auto& ev : this->EVs.Items) {
		auto pg = ev.getCurrentPage();
		if(pg == nullptr || pg->TriggerType != static_cast<int>(TS::Map::EventTriggerType::PressEnterKey)) {
			continue;		//対象外のイベント
		}

		//当たり判定の有無によって分ける
		range.Set(ev.Position.X - pg->HitRect.Width, ev.Position.Y - pg->HitRect.Height, pg->HitRect.Width * 2 + 1, pg->HitRect.Height * 2 + 1);
		if(range.PointContains(this->Player.MapObject.Position)) {
			//接触範囲考慮: 一致する状態だった
			this->ReserveEV(ev.ID);
			return true;
		} else if(pg->NoHit == false && ev.Position == afterPos) {
			//隣り合う状態だった
			this->ReserveEV(ev.ID);
			return true;
		}
	}
	return false;
}


//####################  イベントリストが変更されたときに各イベントのマップオブジェクトに関連付けるキャラクターオブジェクトを再設定する  ####################
//この処理が必要なのは、イベントリストの要素を増減したとき
//vector要素の個数が変動した時点でその要素以降のメンバーに関するポインタ情報がすべて狂うためである
void CSceneMap::CMapData::refreshAllEVTargetChar() {
	for(auto &ev : this->EVs.Items) {
		ev.TargetCharPTR = ev.getCurrentPage();
	}
}
