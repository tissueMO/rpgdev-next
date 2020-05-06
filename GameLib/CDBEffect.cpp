//###################################################################
//			エフェクト データベース
//###################################################################
#include "Header.h"

bool ShowEffect(TS_MT);


//####################  破棄処理  ####################
void CDatabase::CDBEffect::Dispose() {
	vector<CDBEffectData>().swap(this->PlayingList);
}


//####################  エフェクト再生: バックグラウンド  ####################
int CDatabase::CDBEffect::Show(int ID, int destX, int destY) {
	if(this->GetSubDB(0)->GetRowFromID(ID) == nullptr) {
		return -1;		//存在しないエフェクトは処理しない
	}

	//既に同一座標で同一エフェクトが再生されている場合は処理しない
	for(auto& effect : this->PlayingList) {
		if(effect.ID == ID && effect.TargetPosition.X == destX && effect.TargetPosition.Y == destY) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "既に同一エフェクト [%d] が同一座標 [%d, %d]で再生されています。", ID, destX, destY);
			return -1;
		}
	}

	//再生中リストへ登録
	CDBEffectData NewItem;
	NewItem.ID = ID;
	NewItem.Handle = Game.NextRand(0, INT_MAX);
	NewItem.Name = *this->GetSubDB(0)->GetCell(ID, this->GetSubDB(0)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).c_str())->GetStringPTR();
	NewItem.SQHandle = -1;
	NewItem.Frame = -1;
	NewItem.TargetType = *this->GetSubDB(0)->GetCell(ID, this->GetSubDB(0)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBEffectColumnIndices::DrawDest)).c_str())->GetIntegerPTR();
	NewItem.TargetPosition.Set(destX, destY);
	this->PlayingList.push_back(NewItem);

	//一つもエフェクトを表示していないときはMT関数をセット
	if(Game.MT.CountThread("ShowEffect") == 0) {
		Game.MT.AddThread("ShowEffect", ::ShowEffect, TS::System::Scene::All)
			.AddInteger("EffectAsyncLoadMode", Game.BoolToInt(Game.DB.getFDBSystem()->GetBoolean(TS::DB::System::EffectAsyncLoadMode)))
			.AddInteger("EffectFPS", Game.DB.getFDBSystem()->GetInteger(TS::DB::System::EffectFPS));
	}
	return NewItem.Handle;
}


//####################  エフェクト再生: 終了まで待つ  ####################
void CDatabase::CDBEffect::ShowForWait(int id, int destX, int destY) {
	const int handle = this->Show(id, destX, destY);
	while(true) {
		const auto effect = this->GetEffectData(handle);
		if(effect == nullptr || effect->WaitBreakFlag) {
			return;		//再生終了するか、待機中断合図があると待機終了する
		}
		Game.Wait(0.1, true, false);
	}
}


//####################  再生中エフェクトのハンドルからエフェクト情報を取得  ####################
CDatabase::CDBEffect::CDBEffectData* CDatabase::CDBEffect::GetEffectData(int handle) {
	for(auto& data : this->PlayingList) {
		if(data.Handle == handle) {
			return &data;
		}
	}
	return nullptr;
}


//####################  レイヤーグラフィック読み込み  ####################
void CDatabase::CDBEffect::CDBEffectData::LoadGraph(int layerIndex, string fileName, int secSizeWidth, int secSizeHeight) {
	CDBEffectLayerData NewItem;
	NewItem.Name = fileName;
	NewItem.ID = Game.Material.getGraphics()->Add(fileName);
	NewItem.SectionSize.Set(secSizeWidth, secSizeHeight);
	this->Graphics.Add(NewItem);
}


//####################  レイヤーグラフィック描画  ####################
void CDatabase::CDBEffect::CDBEffectData::Draw(int layerIndex, int destX, int destY, int secX, int secY, int blendMode, int trans, int ext, int angle) {
	DxLib::SetDrawBlendMode(Game.DXLib.ConvertToDXBlend(blendMode), 255 - trans);

	//ここで渡す描画先の座標は中心座標
	auto layer = this->Graphics.GetFromIndex(layerIndex);
	Game.DXLib.DrawRectRotaGraph(
		this->Graphics.GetFromIndex(layerIndex)->ID,
		this->TargetPosition.X + destX, this->TargetPosition.Y + destY,
		secX * layer->SectionSize.Width, secY * layer->SectionSize.Height,
		layer->SectionSize.Width, layer->SectionSize.Height, (double)angle * PI / 180.0, (double)ext / 100, true, false);
}


//####################  命令：効果音再生  ####################
void CDatabase::CDBEffect::CDBEffectData::PlaySE(int id, int volume, int pitch, int pan, int sameCounter) {
	//そのフレームが最初に呼び出されたときに実行される
	if(sameCounter == 0) {
		//ファイル名をデータベースから引っ張る
		auto data = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Sound))->GetRowFromID(id);
		string fileName = *data->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR();
		if(fileName.find(".mid") != string::npos) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "エフェクトの効果音にMIDIファイルは対応していません。\n>%s", fileName.c_str());
			return;
		}

		//NOTE: 音量・ピッチ・パンを設定するために新たに読み込む
		CSoundData temp;
		temp.Load(fileName, volume, pitch, pan);
		temp.Play(static_cast<int>(TS::Material::PlayType::Back), true);			//使い捨てのハンドルとして再生し、再生後は自動で破棄される
	}
}


//####################  命令：フラッシュ  ####################
void CDatabase::CDBEffect::CDBEffectData::Flash(int color, int strength, int count, int sameCounter) {
	//そのフレームが最初に呼び出されたときに実行される
	if(sameCounter == 0) {
		Game.DXLib.Flash(color, strength, static_cast<int>(TS::Map::SquareSpeed::Moment), count);
	}
}


//####################  命令：画面振動  ####################
void CDatabase::CDBEffect::CDBEffectData::Shake(int strength, int sameCounter) {
	//そのフレームが最初に呼び出されたときに実行される
	if(sameCounter == 0) {
		Game.DXLib.ShakeOne(strength);
	}
}


//####################  命令：待機中断  ####################
void CDatabase::CDBEffect::CDBEffectData::WaitBreak() {
	this->WaitBreakFlag = true;
}


//####################  ＭＴ実行：エフェクトを表示する ####################
//注意：エフェクトの設定で「画面中心」とされている場合は常に画面中心で表示される
bool ShowEffect(TS_MT) {
	auto db = Game.DB.getFDBEffect();

	//再生中のエフェクトをすべて処理する
	TS_COUNTLOOP(i, db->PlayingList.size()) {
		int FrameLength = -1;				//エフェクトスクリプトによって示される全フレーム数
		CDatabase::CDBEffect::CDBEffectData& CurrentEffect = db->PlayingList[i];

		if(CurrentEffect.Frame == -1) {
			//初期設定・スクリプト読み込み
			CurrentEffect.SQHandle = Game.SQ.Add(Game.Format(TS::FileNames::SQDBEffect, CurrentEffect.ID));
			if(*db->GetSubDB(0)->GetCell(CurrentEffect.ID, TS::DB::ColumnNameEffectTargetType)->GetIntegerPTR() == static_cast<int>(TS::DB::DBEffectViewPosition::CenterScreen)) {
				//画面中心に座標をセットする
				CurrentEffect.TargetPosition.X = Game.WindowSize.Width / 2;
				CurrentEffect.TargetPosition.Y = Game.WindowSize.Height / 2;
			}

			//エフェクトを非同期で読み込む設定の場合
			if(*Data.GetInteger("EffectAsyncLoadMode") == TRUE) {
				DxLib::SetUseASyncLoadFlag(true);		//非同期読み込みを開始
			}
		}

		//エフェクトの読み込みが始まっていないor完了している場合のみ、スクリプトを呼び出す
		if(CurrentEffect.Frame >= -1) {
			//フレーム数はデータベースから引っ張る・このセルは数値ではなく文字列であることに注意せよ
			FrameLength = *db->GetSubDB(0)->GetCell(CurrentEffect.ID, db->GetSubDB(0)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBEffectColumnIndices::FrameLength)).c_str())->GetIntegerPTR();
			//スクリプトを呼び出してエフェクトを描画する・フレーム-1のときはグラフィック読み込みを行う
			int sameLength = TS::System::FixedFPS / *Data.GetInteger("EffectFPS");		//エフェクトの同一フレームが何回実行されるかを示す
			if(sameLength == 0) {
				sameLength = 1;
			}
			Game.SQ.DoFunc<int>(
				TS::SQ::SQEvent_PlayEffect,
				CurrentEffect.Handle,
				(CurrentEffect.Frame == -1) ? -1 : CurrentEffect.Frame / sameLength,	//エフェクトのどのフレームを再生するか
				CurrentEffect.Frame % sameLength										//同一フレームの何回目の再生か
				);

			DxLib::SetUseASyncLoadFlag(false);		//非同期読み込みを解除
			TS_NOBLEND;

			//エフェクトの終端の次のフレームで後始末を行う
			if(FrameLength + 1 <= CurrentEffect.Frame / sameLength) {
				TS_COUNTLOOP(l, db->PlayingList[i].Graphics.GetCount()) {
					Game.Material.getGraphics()->Delete(db->PlayingList[i].Graphics.GetFromIndex(l)->ID);
				}
				db->PlayingList[i].Graphics.Clear();
//				Game.SQ.Delete(CurrentEffect.SQHandle);			//スクリプト破棄
				db->PlayingList.erase(db->PlayingList.begin() + i);
				i--;
				continue;		//次のループへ
			}
		}

		//非同期の場合は読み込みが終わるまで待つ
		if(CurrentEffect.Frame < 0 && *Data.GetInteger("EffectAsyncLoadMode") == TRUE) {
			CurrentEffect.Frame--;

			bool isOK = true;
			for(auto& graph : CurrentEffect.Graphics.Items) {
				//読み込みにエラーが発生している場合は無視する（成功したことにする）
				if(::CheckHandleASyncLoad(graph.ID) == TRUE) {
					isOK = false;
					break;
				}
			}

			//読み込みが完了したら再生フレームへ移る
			if(isOK) {
				CurrentEffect.Frame = 0;
			}
		} else {
			CurrentEffect.Frame++;
		}
	}

	//再生するエフェクトがなくなったら終了する
	if(Game.DB.getFDBEffect()->PlayingList.size() == 0) {
		return true;
	}
	return false;
}
