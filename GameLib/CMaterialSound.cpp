//###################################################################
//			サウンド素材管理クラス定義
//###################################################################
#include "Header.h"


//####################  サウンドを追加  ####################
int CMaterial::CMaterialSound::Add(int dbType, int fixedID) {
	auto row = Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(fixedID);
	return this->Add(*row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR());
}
int CMaterial::CMaterialSound::Add(string fileName) {
	CSoundData NewItem;
	NewItem.Name = fileName;
	NewItem.CurrentPos = 0;
	NewItem.StartLoopPos = 0;
	NewItem.EndLoopPos = 0;
	NewItem.IsMIDI = false;

	//ファイルチェック
	if(fileName.empty()) {
		//ファイル名が空の場合は読み込まない
		return -1;

		//DXアーカイブ後は使用できないので無効
		//} else if(!PathFileExists(fileName.c_str())) {
		//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないサウンドを追加しようとしました。\n>%s", fileName.c_str());
	}

	//NOTE: サウンドは音量・音程・パンの調整があるため、同じサウンドでも異なるハンドルを生成する必要がある。
	//既に同じサウンドが読み込まれていないか調べる
	//for(auto& it : this->Data.Items) {
	//	if(it.second.Name == fileName) {
	//		it.second.Value++;		//参照カウンター加算
	//		return it.second.ID;
	//	}
	//}

	//存在しなければ新たに読み込む
	//NewItem.Value = 1;			//参照カウンター初期化
	if(::strcmp(PathFindExtension(fileName.c_str()), ".mid") == 0) {
		//MIDIの読み込みは再生時に行われる
		NewItem.ID = Game.NextRand(INT_MIN, -2);	//MIDIはハンドルを得ないので -1 以外の負数とする
		NewItem.IsMIDI = true;						//MIDIフラグ
	} else {
		//MIDI以外のサウンドはここで読み込む
		NewItem.ID = DxLib::LoadSoundMem((TS::DirectoryNames::PrePath + fileName).c_str());

#ifdef _DEBUG
		//ループ情報の読み込み: DXアーカイブ後は使用できないので無効となる
		FILE *fp;
		char temp[MAX_PATH];
		::strcpy(temp, fileName.c_str());
		PathRemoveExtension(temp);
		::strcat(temp, "_loop.txt");		//ファイル名はDXLib指定形式
		if(PathFileExists(temp)) {			//対応するループ定義ファイルがあれば、それをループ情報として読み込む
			if((fp = fopen(temp, "r")) == nullptr) {
				Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "サウンドループ情報の読み込みに失敗しました。\n>%s", temp);
			} else {
				// NOTE: データとして読み込むものの、実際の反映はDXライブラリが自動でやってくれる。
				::fgets(temp, MAX_PATH, fp);
				::sscanf(temp, "LOOPSTART=%d", &NewItem.StartLoopPos);
				::fgets(temp, MAX_PATH, fp);
				::sscanf(temp, "LOOPEND=%d", &NewItem.EndLoopPos);
				::fclose(fp);

				//ループ設定を適用
				//DxLib::SetLoopPosSoundMem(NewItem.StartLoopPos, NewItem.ID);
				//DxLib::SetLoopStartTimePosSoundMem(NewItem.EndLoopPos, NewItem.ID);
			}
		}
#endif
	}
	this->Data.Add(NewItem);
	return NewItem.ID;
}


//####################  サウンドハンドルを復元  ####################
int CMaterial::CMaterialSound::Restore(int dbType, int dbID) {
	string fileName = *Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(dbID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR();

	//既に読み込まれているサウンドデータからファイル名の一致するものを探す
	for(auto& item : this->Data.Items) {
		if(item.second.Name == fileName) {
			return item.second.ID;
		}
	}
	return -1;
}


//####################  サウンドを削除  ####################
void CMaterial::CMaterialSound::Delete(int hdl) {
	if(!this->Data.Exists(hdl)) {
		return;		//存在しない場合は処理しない
	}

	//他に同一のサウンドが残っている場合はまだリソースを削除しない
	//NOTE: サウンドは音量・音程・パンの調整があるため、同じサウンドでも異なるハンドルを生成する必要がある。
	//for(auto& item : this->Data.Items) {
	//	if(item.second.ID == hdl) {
	//		item.second.Value--;
	//		if(item.second.Value > 0) {
	//			return;		//まだ残っている
	//		} else {
	//			break;		//もう残っていない
	//		}
	//	}
	//}

	//残っていなければ実際に削除する
	DxLib::DeleteSoundMem(hdl);
	this->Data.Delete(hdl);
}


//####################  サウンドをすべて削除  ####################
void CMaterial::CMaterialSound::Clear() {
	for(auto& it : this->Data.Items) {
		DxLib::DeleteSoundMem(it.second.ID);		//ハンドルをすべて削除
	}
	this->Data.Clear();
}


//####################  ＭＴ関数：フェード再生  ####################
//Vint["Handle"] : ハンドル
//Vint["MaxVolume"] : 最終音量
//Vdec["PlusPerLoop"] : １ループあたりの加算量（およそ1/60秒に１回）
//Vdec["CurrentVolume"] : 現在の音量
bool FadePlay(TS_MT) {
	*Data.GetDecimal("CurrentVolume") += *Data.GetDecimal("PlusPerLoop");
	if(*Data.GetDecimal("CurrentVolume") > *Data.GetInteger("MaxVolume")) {
		*Data.GetDecimal("CurrentVolume") = *Data.GetInteger("MaxVolume");
	}
	DxLib::ChangeVolumeSoundMem(*Data.GetDecimal("CurrentVolume"), *Data.GetInteger("Handle"));

	if((int)*Data.GetDecimal("CurrentVolume") >= *Data.GetInteger("MaxVolume")) {
		return true;		//フェード完了
	}
	return false;
}
//####################  再生  ####################
//Volumeをマイナスにすると標準音量が適用される
//ここでのPitchRateはMIDIのみ有効で、50 のとき１オクターブ下、200 のとき１オクターブ上となる
//一般サウンドでPitchRateを設定する場合は、汎用クラスのCSoundDataクラスを利用する必要がある
//Panは MIDIではサポートしない
void CMaterial::CMaterialSound::Play(int hdl, int playType, int volume, int pitchRate, int panPot, double fadeTime) {
	if(hdl < 0) {
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないサウンド [ID: %d] を再生しようとしました。", hdl);
		return;
	}

	if(volume < TS::Material::MinVolume) {
		//標準音量をセットする
		volume = this->StandardVolume;
	} else {
		//最大音量は標準音量
		volume = Game.WithIn(volume, TS::Material::MinVolume, this->StandardVolume);
	}
	panPot = Game.WithIn(panPot, TS::Material::MinPan, TS::Material::MaxPan);

	auto &it = *this->Data.Get(hdl);
	it.Volume = volume;
	if(it.IsMIDI) {
		// NOTE : MIDIはパンに対応しない
		//MIDIはここで読み込んで再生する
		int option = GuruGuruSmf::PlayOption::SkipBeginning;
		pitchRate = Game.WithIn(pitchRate, TS::Material::Common_MinPitch, TS::Material::Common_MaxPitch);

		GGS->ClearList();
		GGS->SetMasterVolume(TS::Material::MIDI_MinVolume + it.Volume / (TS::Material::MaxVolume / (TS::Material::MIDI_MaxVolume - TS::Material::MIDI_MinVolume)));		//MIDIは -127～0 の間で音量を指定する
		GGS->AddListFromFileA(it.Name.c_str(), 0, it.ID);

		if(playType == static_cast<int>(TS::Material::PlayType::Loop)) {
			if(pitchRate == 0) {
				option = GuruGuruSmf::PlayOption::SkipBeginning | GuruGuruSmf::PlayOption::Loop;
			} else {
				option = GuruGuruSmf::PlayOption::SkipBeginning | GuruGuruSmf::PlayOption::Loop | GuruGuruSmf::PlayOption::MasterPitch;
			}
		} else {
			if(pitchRate == 0) {
				option = GuruGuruSmf::PlayOption::SkipBeginning;
			} else {
				option = GuruGuruSmf::PlayOption::SkipBeginning | GuruGuruSmf::PlayOption::MasterPitch;
			}
		}

		GGS->Play(option, hdl, 0, fadeTime * 1000, 0);
		GGS->SetMasterPitch(Game.WithIn(34.134 * pitchRate, TS::Material::MIDI_MinPitch, TS::Material::MIDI_MaxPitch));
	} else {
		//それ以外は読み込んだものを再生する
		if(fadeTime > 0.0F) {
			//MT関数を経由する
			Game.MT.AddThread("FadePlay", ::FadePlay, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", 0.0F);
			DxLib::PlaySoundMem(it.ID, (playType != static_cast<int>(TS::Material::PlayType::Loop)) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(0, it.ID);
		} else {
			//即座に処理する
			DxLib::PlaySoundMem(it.ID, (playType != static_cast<int>(TS::Material::PlayType::Loop)) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(it.Volume, it.ID);
		}
		DxLib::ChangePanSoundMem(panPot, it.ID);			//パン変更
	}

	//再生が終わるまで待つ場合
	if(playType == static_cast<int>(TS::Material::PlayType::Wait)) {
		do {
			TS_EVERYLOOP;
		} while(this->CheckPlayStatus(hdl) == static_cast<int>(TS::Material::PlayStatus::Playing));
	}
}


//####################  指定したサウンドを再生し、再生後に自動でハンドルを破棄する  ####################
//使い捨てのSEなどに使うことを想定している。リスト上の項目とハンドルの削除タイミングにズレが生じるのが肝。
void CMaterial::CMaterialSound::PlayOnce(int hdl, int playType, int volume, int pitchRate, int panPot) {
	DxLib::SetPlayFinishDeleteSoundMem(true, hdl);
	this->Play(hdl, playType, volume, pitchRate, panPot);
	this->Data.Delete(hdl);		//ハンドルは残したままリストからは先に削除しておく
}


//####################  ＭＴ関数：フェード停止  ####################
//Data.Vint["Handle"] : ハンドル (サウンドID)
//Data.Vint["IsPause"] : 現在の再生位置を保存するか
//Data.Vint["Dispose"] : 完了したらハンドルを破棄するか
//Data.Vdec["PlusPerLoop"] : １ループあたりの減算量（およそ1/60秒に１回）
//Data.Vdec["CurrentVolume"] : 現在の音量 (0-255)
bool FadeStop(TS_MT) {
	*Data.GetDecimal("CurrentVolume") -= *Data.GetDecimal("PlusPerLoop");
	if(*Data.GetDecimal("CurrentVolume") > 0.0) {
		ChangeVolumeSoundMem(*Data.GetDecimal("CurrentVolume"), *Data.GetInteger("Handle"));
	} else if(*Data.GetDecimal("CurrentVolume") <= 0.0) {
		StopSoundMem(*Data.GetInteger("Handle"));			//正式に停止する
		if(*Data.GetInteger("IsPause")) {				//ポーズの場合
			//現在の再生位置を保存する
			Game.Material.getSounds()->Data.Get(*Data.GetInteger("Handle"))->CurrentPos = DxLib::GetCurrentPositionSoundMem(*Data.GetInteger("Handle"));
		} else if(*Data.GetInteger("Dispose")) {		//破棄する場合
			Game.Material.getSounds()->Delete(*Data.GetInteger("Handle"));
		}
		return true;	//フェード完了
	}
	return false;
}
//####################  停止  ####################
//IDに -1 を指定すると、すべて停止する
void CMaterial::CMaterialSound::Stop(int hdl, double fadeTime, bool afterDelete) {
	if(hdl == -1) {
		//すべて停止
		for(auto& it : this->Data.Items) {
			if(it.second.ID != -1) {
				this->Stop(it.second.ID, fadeTime, afterDelete);
			}
		}
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないサウンドサウンド [ID: %d] を停止しようとしました。", hdl);
		return;
	}

	//単一停止
	auto it = *this->Data.Get(hdl);
	if(it.IsMIDI) {
		GGS->Stop(fadeTime * 1000);
	} else {
		if(fadeTime > 0.0) {
			//MT関数を経由する
			Game.MT.AddThread("FadeStop", ::FadeStop, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", DxLib::GetVolumeSoundMem2(it.ID))
				.AddInteger("IsPause", false)
				.AddInteger("Dispose", afterDelete);
		} else {
			//即座に処理する
			DxLib::StopSoundMem(it.ID);
			if(afterDelete) {
				this->Delete(hdl);
			}
		}
	}
}


//####################  すべて停止  ####################
void CMaterial::CMaterialSound::StopAll(double fadeTime, bool afterDelete) {
	for(auto& snd : this->Data.Items) {
		this->Stop(snd.second.ID, fadeTime, afterDelete);
	}
}


//####################  一時停止  ####################
//IDに -1 を指定すると、すべて一時停止する
void CMaterial::CMaterialSound::Pause(int hdl, double fadeTime) {
	if(hdl == -1) {
		//すべて一時停止
		for(auto& it : this->Data.Items) {
			this->Pause(it.second.ID, fadeTime);
		}
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないサウンド [ID: %d] を一時停止しようとしました。", hdl);
		return;
	}

	//単一の一時停止
	auto it = *this->Data.Get(hdl);
	if(it.IsMIDI) {
		GGS->Stop(fadeTime * 1000);
	} else {
		it.CurrentPos = DxLib::GetCurrentPositionSoundMem(it.ID);
		if(fadeTime > 0.0F) {
			//MT関数を経由する
			Game.MT.AddThread("FadeStop", ::FadeStop, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", DxLib::GetVolumeSoundMem2(it.ID))
				.AddInteger("IsPause", true)
				.AddInteger("Dispose", false);
		} else {
			//即座に処理する
			DxLib::StopSoundMem(it.ID);
		}
	}
}


//####################  一時停止した地点から再開  ####################
//IDに -1 を指定すると、すべて再開する
void CMaterial::CMaterialSound::Resume(int hdl, bool loopMode, double fadeTime) {
	if(hdl == -1) {
		//すべて再開
		for(auto& it : this->Data.Items) {
			this->Resume(it.second.ID, loopMode, fadeTime);
		}
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないサウンド [ID: %d] を再開しようとしました。", hdl);
		return;
	}

	//単一再開
	auto it = *this->Data.Get(hdl);
	if(it.IsMIDI) {
		GGS->Pause();
	} else {
		DxLib::SetCurrentPositionSoundMem(it.CurrentPos, it.ID);		//再生位置を復元してから再生する
		if(fadeTime > 0.0F) {
			//MT関数を経由する
			Game.MT.AddThread("FadePlay", ::FadePlay, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", 0.0F);
			DxLib::PlaySoundMem(it.ID, (!loopMode) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(0, it.ID);
		} else {
			//即座に処理する
			DxLib::PlaySoundMem(it.ID, (!loopMode) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(it.Volume, it.ID);
		}
	}
}


//####################  指定したサウンドの再生状態を取得  ####################
int CMaterial::CMaterialSound::CheckPlayStatus(int hdl) {
	if(!this->Data.Exists(hdl)) {
		return static_cast<int>(TS::Material::PlayStatus::Error);
	}

	//MIDIは取得方法が異なる
	if(this->Data.Get(hdl)->IsMIDI) {
		GuruGuruSmf::PlayerStatus status;
		GGS->GetPlayerStatus(&status);

		switch(status.State) {
			case GuruGuruSmf::PlayerState::Stop:	return static_cast<int>(TS::Material::PlayStatus::Stop);
			case GuruGuruSmf::PlayerState::Pause:	return static_cast<int>(TS::Material::PlayStatus::Stop);
			case GuruGuruSmf::PlayerState::Play:	return static_cast<int>(TS::Material::PlayStatus::Playing);
			default: 								return static_cast<int>(TS::Material::PlayStatus::Error);
		}
	}
	return ::CheckSoundMem(this->Data.Get(hdl)->ID);
}


//####################  音量を変更  ####################
void CMaterial::CMaterialSound::setStandardVolume(int value) {
	this->StandardVolume = Game.WithIn(value, TS::Material::MinVolume, TS::Material::MaxVolume);
}
