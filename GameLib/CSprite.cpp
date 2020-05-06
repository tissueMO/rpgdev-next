//###################################################################
//			スプライトクラス定義
//###################################################################
#include "Header.h"


//####################  スプライトグラフィックを描画  ####################
void CSprite::CSpriteData::CSpriteGraphicArea::Draw(CPoint pos) {
	Game.DXLib.DrawRectExtendGraph(this->ID, pos.X + this->Location.X, pos.Y + this->Location.Y, this->VisibleRange.Width * this->ExtendRateH / 100, this->VisibleRange.Height * this->ExtendRateV / 100, this->VisibleRange.Left, this->VisibleRange.Top, this->VisibleRange.Width, this->VisibleRange.Height);
}


//####################  スプライトテキストを描画  ####################
void CSprite::CSpriteData::CSpriteTextArea::Draw(CPoint pos) {
	Game.DXLib.SetDrawArea(this->DrawArea);
	Game.DXLib.DrawStringAlign(pos.X + this->Location.X, pos.Y + this->Location.Y, this->Align, this->FontID, this->Color, this->Text.c_str());
	Game.DXLib.ResetDrawArea();
}


//####################  実視範囲を解除  ####################
void CSprite::CSpriteData::CSpriteGraphicArea::InitVisibleRange() {
	this->VisibleRange.Set(0, 0, Size.Width, Size.Height);
}


//####################  このスプライトにグラフィックを追加  ####################
// baseHandle が -1 以外である場合、新規に読み込まずにそのハンドルをコピーする
int CSprite::CSpriteData::AddGraphics(string path, int x, int y, int baseHandle) {
	CSpriteGraphicArea NewItem;

	NewItem.IsNewGraph = (baseHandle == -1);
	if(baseHandle == -1) {
		//素材を読み込む
		NewItem.ID = Game.Material.getGraphics()->Add(path);
		NewItem.Name = path;
	} else {
		NewItem.ID = baseHandle;
		NewItem.Name = "(BaseGraphics)";
	}

	NewItem.ExtendRateH = 100;
	NewItem.ExtendRateV = 100;
	NewItem.Location.Set(x, y);
	DxLib::GetGraphSize(NewItem.ID, &NewItem.Size.Width, &NewItem.Size.Height);
	NewItem.VisibleRange.Set(0, 0, NewItem.Size.Width, NewItem.Size.Height);

	this->Graphics.Add(NewItem);
	return this->Graphics.GetCount() - 1;		//新しく生成されたグラフィックのインデックスを返す
}


//####################  このスプライトの指定グラフィックを変更  ####################
void CSprite::CSpriteData::ChangeGraphics(int index, string path, int x, int y, int baseHandle) {
	CSpriteGraphicArea *CurrentItem = this->Graphics.GetFromIndex(index);

	//以前のグラフィックを削除
	if(CurrentItem->IsNewGraph) {
		Game.Material.getGraphics()->Delete(CurrentItem->ID);
	}

	//素材を読み込む
	CurrentItem->IsNewGraph = (baseHandle == -1);
	if(baseHandle == -1) {
		CurrentItem->ID = Game.Material.getGraphics()->Add(path);
		CurrentItem->Name = path;
	} else {
		CurrentItem->ID = baseHandle;
		CurrentItem->Name = "(BaseGraphics)";
	}
}


//####################  このスプライトの指定グラフィックを削除  ####################
void CSprite::CSpriteData::DeleteGraphics(int index) {
	if(this->Graphics.GetFromIndex(index)->IsNewGraph) {
		Game.Material.getGraphics()->Delete(this->Graphics.GetFromIndex(index)->ID);
	}
	this->Graphics.DeleteFromIndex(index);
}


//####################  このスプライトの指定グラフィックデータを取得  ####################
CSprite::CSpriteData::CSpriteGraphicArea* CSprite::CSpriteData::GetGraphics(int index) {
	return this->Graphics.GetFromIndex(index);
}


//####################  このスプライトにテキストを追加  ####################
int CSprite::CSpriteData::AddText(string text, int x, int y, int color, int FontID, int align) {
	CSpriteTextArea NewItem;
	NewItem.Text = text;
	NewItem.Location.Set(x, y);
	NewItem.Color = color;
	NewItem.FontID = FontID;
	NewItem.Align = align;
	NewItem.DrawArea.Set(0, 0, Game.WindowSize.Width, Game.WindowSize.Height);
	this->Texts.Add(NewItem);
	return this->Texts.GetCount() - 1;		//新しく生成されたテキストのインデックスを返す
}


//####################  このスプライトの指定テキストを変更  ####################
void CSprite::CSpriteData::ChangeText(int index, string text) {
	this->Texts.GetFromIndex(index)->Text = text;
}


//####################  このスプライトの指定テキストを削除  ####################
void CSprite::CSpriteData::DeleteText(int index) {
	this->Texts.DeleteFromIndex(index);
}


//####################  このスプライトの指定グラフィックデータを取得  ####################
CSprite::CSpriteData::CSpriteTextArea* CSprite::CSpriteData::GetText(int index) {
	return this->Texts.GetFromIndex(index);
}


//####################  このスプライトを通常描画  ####################
void CSprite::CSpriteData::Draw() {
	//グラフィック部
	for(auto& g : this->Graphics.Items) {
		g.Draw(this->Location);
	}

	//テキスト部
	for(auto& t : this->Texts.Items) {
		t.Draw(this->Location);
	}
}


//####################  このスプライトをワイプイン  ####################
void CSprite::CSpriteData::WipeIn(int type, int wipeSpeed) {
	//速度が０またはマイナスの場合は標準速度にする
	if(wipeSpeed <= 0) {
		wipeSpeed = static_cast<int>(TS::Map::Speed::High);
	}

	this->WipeType = type;
	this->WipeCounter = 0;
	this->WipeSpeed = wipeSpeed;
	this->WipeState = static_cast<int>(TS::Sprite::WipeState::WipeIn);
	this->DeleteAfterWipe = false;
}


//####################  このスプライトをワイプアウト  ####################
void CSprite::CSpriteData::WipeOut(int type, int wipeSpeed, bool afterDelete) {
	//速度が０またはマイナスの場合は標準速度にする
	if(wipeSpeed <= 0) {
		wipeSpeed = static_cast<int>(TS::Map::Speed::High);
	}

	this->WipeType = type;
	this->WipeCounter = 255;
	this->WipeSpeed = wipeSpeed;
	this->WipeState = static_cast<int>(TS::Sprite::WipeState::WipeOut);
	this->DeleteAfterWipe = afterDelete;
}


//####################  スプライトを追加  ####################
void CSprite::Add(int ID) {
	auto newItem = CSpriteData();
	newItem.Hide = false;
	newItem.ID = ID;
	newItem.WipeState = static_cast<int>(TS::Sprite::WipeState::None);
	newItem.Trans = 0;
	this->Data.Add(newItem);
}


//####################  スプライトを削除  ####################
void CSprite::Delete(int ID) {
	this->releaseResource(this->Data.GetFromID(ID));
	this->Data.DeleteFromID(ID);
}


//####################  スプライトが保持するリソースを解放する  ####################
void CSprite::releaseResource(CSpriteData* data) {
	//グラフィックハンドルを頭から順にすべて破棄
	while(this->Data.GetFromID(data->ID)->Graphics.GetCount() > 0) {
		this->Data.GetFromID(data->ID)->DeleteGraphics(0);
	}
}


//####################  スプライトをクリア  ####################
//wipeSpeed=255にすると瞬間削除になる
void CSprite::Clear(int wipeType, int wipeSpeed) {
	for(auto& Item : this->Data.Items) {
		this->Delete(Item.ID);
	}
}


//####################  スプライトデータを取得  ####################
CSprite::CSpriteData* CSprite::Get(int ID) {
	return this->Data.GetFromID(ID);
}


//####################  １枚の新規画像を持ったスプライトを追加  ####################
void CSprite::AddNewGraphics(int ID, string path, int x, int y) {
	this->Add(ID);
	this->Data.GetFromID(ID)->AddGraphics(path, x, y);
}


//####################  １枚の既存画像を持ったスプライトを追加  ####################
void CSprite::AddLoadedGraphics(int ID, int graphID, int x, int y) {
	this->Add(ID);
	this->Data.GetFromID(ID)->AddGraphics("", x, y, graphID);
}


//####################  １枚の新規画像を持ったスプライトを追加してワイプイン  ####################
void CSprite::AddWipeNewGraphics(int ID, string path, int x, int y, int wipeType, int wipeSpeed) {
	this->AddNewGraphics(ID, path, x, y);
	this->Data.GetFromID(ID)->WipeIn(wipeType, wipeSpeed);
}


//####################  １枚の既存画像を持ったスプライトを追加してワイプイン  ####################
void CSprite::AddWipeLoadedGraphics(int ID, int graphID, int x, int y, int wipeType, int wipeSpeed) {
	this->AddLoadedGraphics(ID, graphID, x, y);
	this->Data.GetFromID(ID)->WipeIn(wipeType, wipeSpeed);
}


//####################  スプライトをコピー  ####################
void CSprite::Copy(int newID, int baseID, int x, int y) {
	if(newID == baseID) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "スプライトのコピー元とコピー先のIDが同一です。");
		return;
	}
	auto newItem = CSpriteData(*this->Data.GetFromID(baseID));
	newItem.ID = newID;
	this->Data.Add(newItem);
}


//####################  スプライトをコピーしてワイプイン  ####################
void CSprite::CopyWipe(int newID, int baseID, int x, int y, int wipeType, int wipeSpeed) {
	this->Copy(newID, baseID, x, y);
	this->Data.GetFromID(newID)->WipeIn(wipeType, wipeSpeed);
}


//####################  指定範囲のIDを持つスプライトを一斉にワイプイン  ####################
void CSprite::WipeInRange(int startID, int endID, int wipeType, int wipeSpeed) {
	for(int ID = startID; ID <= endID; ID++) {
		if(!this->Data.Exists(ID)) {
			continue;
		}
		this->Data.GetFromID(ID)->WipeIn(wipeType, wipeSpeed);
	}
}


//####################  指定範囲のIDを持つスプライトを一斉にワイプアウト  ####################
void CSprite::WipeOutRange(int startID, int endID, int wipeType, int wipeSpeed, bool afterDelete) {
	for(int ID = startID; ID <= endID; ID++) {
		if(!this->Data.Exists(ID)) {
			continue;
		}
		this->Data.GetFromID(ID)->WipeOut(wipeType, wipeSpeed, afterDelete);
	}
}


//####################  すべてのスプライトを描画する  ####################
void CSprite::DrawAll() {
	if(this->ForbidLayerDraw) {
		return;
	}

	//全スプライトを走査
	for(auto& sprite : this->Data.Items) {
		//ワイプ中である場合はワイプ処理を進める
		if(sprite.WipeState != static_cast<int>(TS::Sprite::WipeState::None)) {
			sprite.WipeCounter += (sprite.WipeState == static_cast<int>(TS::Sprite::WipeState::WipeIn)) ? +sprite.WipeSpeed : -sprite.WipeSpeed;

			//カウンターが端点に達した場合はワイプ完了
			if(sprite.WipeCounter <= 0) {
				sprite.WipeState = static_cast<int>(TS::Sprite::WipeState::None);
				sprite.Hide = true;
			} else if(255 <= sprite.WipeCounter) {
				sprite.WipeState = static_cast<int>(TS::Sprite::WipeState::None);
			}
		}

		//非表示でない場合のみ描画
		if(!sprite.Hide) {
			if(sprite.WipeState == static_cast<int>(TS::Sprite::WipeState::None)) {
				//通常描画
				TS_ALPHABLEND(255 - sprite.Trans);
				sprite.Draw();
				TS_NOBLEND;
			} else {
				//ワイプ中の場合はスクリプト呼び出し
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_WipeSprite, &sprite);
			}
		}
	}

	//ワイプが完了している削除待ちのスプライトを削除する
	this->Data.Items.erase(std::remove_if(TS_CONTAINERALL(this->Data.Items), [&](auto& item) {
		if(item.WipeState == static_cast<int>(TS::Sprite::WipeState::None) && item.DeleteAfterWipe) {
			this->releaseResource(&item);		//vectorから削除する前に保持しているリソースを破棄する
			return true;
		}
		return false;
	}), this->Data.Items.end());
}
