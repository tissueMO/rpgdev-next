//###################################################################
//			�T�E���h�f�ފǗ��N���X��`
//###################################################################
#include "Header.h"


//####################  �T�E���h��ǉ�  ####################
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

	//�t�@�C���`�F�b�N
	if(fileName.empty()) {
		//�t�@�C��������̏ꍇ�͓ǂݍ��܂Ȃ�
		return -1;

		//DX�A�[�J�C�u��͎g�p�ł��Ȃ��̂Ŗ���
		//} else if(!PathFileExists(fileName.c_str())) {
		//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��T�E���h��ǉ����悤�Ƃ��܂����B\n>%s", fileName.c_str());
	}

	//NOTE: �T�E���h�͉��ʁE�����E�p���̒��������邽�߁A�����T�E���h�ł��قȂ�n���h���𐶐�����K�v������B
	//���ɓ����T�E���h���ǂݍ��܂�Ă��Ȃ������ׂ�
	//for(auto& it : this->Data.Items) {
	//	if(it.second.Name == fileName) {
	//		it.second.Value++;		//�Q�ƃJ�E���^�[���Z
	//		return it.second.ID;
	//	}
	//}

	//���݂��Ȃ���ΐV���ɓǂݍ���
	//NewItem.Value = 1;			//�Q�ƃJ�E���^�[������
	if(::strcmp(PathFindExtension(fileName.c_str()), ".mid") == 0) {
		//MIDI�̓ǂݍ��݂͍Đ����ɍs����
		NewItem.ID = Game.NextRand(INT_MIN, -2);	//MIDI�̓n���h���𓾂Ȃ��̂� -1 �ȊO�̕����Ƃ���
		NewItem.IsMIDI = true;						//MIDI�t���O
	} else {
		//MIDI�ȊO�̃T�E���h�͂����œǂݍ���
		NewItem.ID = DxLib::LoadSoundMem((TS::DirectoryNames::PrePath + fileName).c_str());

#ifdef _DEBUG
		//���[�v���̓ǂݍ���: DX�A�[�J�C�u��͎g�p�ł��Ȃ��̂Ŗ����ƂȂ�
		FILE *fp;
		char temp[MAX_PATH];
		::strcpy(temp, fileName.c_str());
		PathRemoveExtension(temp);
		::strcat(temp, "_loop.txt");		//�t�@�C������DXLib�w��`��
		if(PathFileExists(temp)) {			//�Ή����郋�[�v��`�t�@�C��������΁A��������[�v���Ƃ��ēǂݍ���
			if((fp = fopen(temp, "r")) == nullptr) {
				Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�T�E���h���[�v���̓ǂݍ��݂Ɏ��s���܂����B\n>%s", temp);
			} else {
				// NOTE: �f�[�^�Ƃ��ēǂݍ��ނ��̂́A���ۂ̔��f��DX���C�u�����������ł���Ă����B
				::fgets(temp, MAX_PATH, fp);
				::sscanf(temp, "LOOPSTART=%d", &NewItem.StartLoopPos);
				::fgets(temp, MAX_PATH, fp);
				::sscanf(temp, "LOOPEND=%d", &NewItem.EndLoopPos);
				::fclose(fp);

				//���[�v�ݒ��K�p
				//DxLib::SetLoopPosSoundMem(NewItem.StartLoopPos, NewItem.ID);
				//DxLib::SetLoopStartTimePosSoundMem(NewItem.EndLoopPos, NewItem.ID);
			}
		}
#endif
	}
	this->Data.Add(NewItem);
	return NewItem.ID;
}


//####################  �T�E���h�n���h���𕜌�  ####################
int CMaterial::CMaterialSound::Restore(int dbType, int dbID) {
	string fileName = *Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(dbID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR();

	//���ɓǂݍ��܂�Ă���T�E���h�f�[�^����t�@�C�����̈�v������̂�T��
	for(auto& item : this->Data.Items) {
		if(item.second.Name == fileName) {
			return item.second.ID;
		}
	}
	return -1;
}


//####################  �T�E���h���폜  ####################
void CMaterial::CMaterialSound::Delete(int hdl) {
	if(!this->Data.Exists(hdl)) {
		return;		//���݂��Ȃ��ꍇ�͏������Ȃ�
	}

	//���ɓ���̃T�E���h���c���Ă���ꍇ�͂܂����\�[�X���폜���Ȃ�
	//NOTE: �T�E���h�͉��ʁE�����E�p���̒��������邽�߁A�����T�E���h�ł��قȂ�n���h���𐶐�����K�v������B
	//for(auto& item : this->Data.Items) {
	//	if(item.second.ID == hdl) {
	//		item.second.Value--;
	//		if(item.second.Value > 0) {
	//			return;		//�܂��c���Ă���
	//		} else {
	//			break;		//�����c���Ă��Ȃ�
	//		}
	//	}
	//}

	//�c���Ă��Ȃ���Ύ��ۂɍ폜����
	DxLib::DeleteSoundMem(hdl);
	this->Data.Delete(hdl);
}


//####################  �T�E���h�����ׂč폜  ####################
void CMaterial::CMaterialSound::Clear() {
	for(auto& it : this->Data.Items) {
		DxLib::DeleteSoundMem(it.second.ID);		//�n���h�������ׂč폜
	}
	this->Data.Clear();
}


//####################  �l�s�֐��F�t�F�[�h�Đ�  ####################
//Vint["Handle"] : �n���h��
//Vint["MaxVolume"] : �ŏI����
//Vdec["PlusPerLoop"] : �P���[�v������̉��Z�ʁi���悻1/60�b�ɂP��j
//Vdec["CurrentVolume"] : ���݂̉���
bool FadePlay(TS_MT) {
	*Data.GetDecimal("CurrentVolume") += *Data.GetDecimal("PlusPerLoop");
	if(*Data.GetDecimal("CurrentVolume") > *Data.GetInteger("MaxVolume")) {
		*Data.GetDecimal("CurrentVolume") = *Data.GetInteger("MaxVolume");
	}
	DxLib::ChangeVolumeSoundMem(*Data.GetDecimal("CurrentVolume"), *Data.GetInteger("Handle"));

	if((int)*Data.GetDecimal("CurrentVolume") >= *Data.GetInteger("MaxVolume")) {
		return true;		//�t�F�[�h����
	}
	return false;
}
//####################  �Đ�  ####################
//Volume���}�C�i�X�ɂ���ƕW�����ʂ��K�p�����
//�����ł�PitchRate��MIDI�̂ݗL���ŁA50 �̂Ƃ��P�I�N�^�[�u���A200 �̂Ƃ��P�I�N�^�[�u��ƂȂ�
//��ʃT�E���h��PitchRate��ݒ肷��ꍇ�́A�ėp�N���X��CSoundData�N���X�𗘗p����K�v������
//Pan�� MIDI�ł̓T�|�[�g���Ȃ�
void CMaterial::CMaterialSound::Play(int hdl, int playType, int volume, int pitchRate, int panPot, double fadeTime) {
	if(hdl < 0) {
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��T�E���h [ID: %d] ���Đ����悤�Ƃ��܂����B", hdl);
		return;
	}

	if(volume < TS::Material::MinVolume) {
		//�W�����ʂ��Z�b�g����
		volume = this->StandardVolume;
	} else {
		//�ő剹�ʂ͕W������
		volume = Game.WithIn(volume, TS::Material::MinVolume, this->StandardVolume);
	}
	panPot = Game.WithIn(panPot, TS::Material::MinPan, TS::Material::MaxPan);

	auto &it = *this->Data.Get(hdl);
	it.Volume = volume;
	if(it.IsMIDI) {
		// NOTE : MIDI�̓p���ɑΉ����Ȃ�
		//MIDI�͂����œǂݍ���ōĐ�����
		int option = GuruGuruSmf::PlayOption::SkipBeginning;
		pitchRate = Game.WithIn(pitchRate, TS::Material::Common_MinPitch, TS::Material::Common_MaxPitch);

		GGS->ClearList();
		GGS->SetMasterVolume(TS::Material::MIDI_MinVolume + it.Volume / (TS::Material::MaxVolume / (TS::Material::MIDI_MaxVolume - TS::Material::MIDI_MinVolume)));		//MIDI�� -127�`0 �̊Ԃŉ��ʂ��w�肷��
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
		//����ȊO�͓ǂݍ��񂾂��̂��Đ�����
		if(fadeTime > 0.0F) {
			//MT�֐����o�R����
			Game.MT.AddThread("FadePlay", ::FadePlay, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", 0.0F);
			DxLib::PlaySoundMem(it.ID, (playType != static_cast<int>(TS::Material::PlayType::Loop)) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(0, it.ID);
		} else {
			//�����ɏ�������
			DxLib::PlaySoundMem(it.ID, (playType != static_cast<int>(TS::Material::PlayType::Loop)) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(it.Volume, it.ID);
		}
		DxLib::ChangePanSoundMem(panPot, it.ID);			//�p���ύX
	}

	//�Đ����I���܂ő҂ꍇ
	if(playType == static_cast<int>(TS::Material::PlayType::Wait)) {
		do {
			TS_EVERYLOOP;
		} while(this->CheckPlayStatus(hdl) == static_cast<int>(TS::Material::PlayStatus::Playing));
	}
}


//####################  �w�肵���T�E���h���Đ����A�Đ���Ɏ����Ńn���h����j������  ####################
//�g���̂Ă�SE�ȂǂɎg�����Ƃ�z�肵�Ă���B���X�g��̍��ڂƃn���h���̍폜�^�C�~���O�ɃY����������̂��́B
void CMaterial::CMaterialSound::PlayOnce(int hdl, int playType, int volume, int pitchRate, int panPot) {
	DxLib::SetPlayFinishDeleteSoundMem(true, hdl);
	this->Play(hdl, playType, volume, pitchRate, panPot);
	this->Data.Delete(hdl);		//�n���h���͎c�����܂܃��X�g����͐�ɍ폜���Ă���
}


//####################  �l�s�֐��F�t�F�[�h��~  ####################
//Data.Vint["Handle"] : �n���h�� (�T�E���hID)
//Data.Vint["IsPause"] : ���݂̍Đ��ʒu��ۑ����邩
//Data.Vint["Dispose"] : ����������n���h����j�����邩
//Data.Vdec["PlusPerLoop"] : �P���[�v������̌��Z�ʁi���悻1/60�b�ɂP��j
//Data.Vdec["CurrentVolume"] : ���݂̉��� (0-255)
bool FadeStop(TS_MT) {
	*Data.GetDecimal("CurrentVolume") -= *Data.GetDecimal("PlusPerLoop");
	if(*Data.GetDecimal("CurrentVolume") > 0.0) {
		ChangeVolumeSoundMem(*Data.GetDecimal("CurrentVolume"), *Data.GetInteger("Handle"));
	} else if(*Data.GetDecimal("CurrentVolume") <= 0.0) {
		StopSoundMem(*Data.GetInteger("Handle"));			//�����ɒ�~����
		if(*Data.GetInteger("IsPause")) {				//�|�[�Y�̏ꍇ
			//���݂̍Đ��ʒu��ۑ�����
			Game.Material.getSounds()->Data.Get(*Data.GetInteger("Handle"))->CurrentPos = DxLib::GetCurrentPositionSoundMem(*Data.GetInteger("Handle"));
		} else if(*Data.GetInteger("Dispose")) {		//�j������ꍇ
			Game.Material.getSounds()->Delete(*Data.GetInteger("Handle"));
		}
		return true;	//�t�F�[�h����
	}
	return false;
}
//####################  ��~  ####################
//ID�� -1 ���w�肷��ƁA���ׂĒ�~����
void CMaterial::CMaterialSound::Stop(int hdl, double fadeTime, bool afterDelete) {
	if(hdl == -1) {
		//���ׂĒ�~
		for(auto& it : this->Data.Items) {
			if(it.second.ID != -1) {
				this->Stop(it.second.ID, fadeTime, afterDelete);
			}
		}
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��T�E���h�T�E���h [ID: %d] ���~���悤�Ƃ��܂����B", hdl);
		return;
	}

	//�P���~
	auto it = *this->Data.Get(hdl);
	if(it.IsMIDI) {
		GGS->Stop(fadeTime * 1000);
	} else {
		if(fadeTime > 0.0) {
			//MT�֐����o�R����
			Game.MT.AddThread("FadeStop", ::FadeStop, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", DxLib::GetVolumeSoundMem2(it.ID))
				.AddInteger("IsPause", false)
				.AddInteger("Dispose", afterDelete);
		} else {
			//�����ɏ�������
			DxLib::StopSoundMem(it.ID);
			if(afterDelete) {
				this->Delete(hdl);
			}
		}
	}
}


//####################  ���ׂĒ�~  ####################
void CMaterial::CMaterialSound::StopAll(double fadeTime, bool afterDelete) {
	for(auto& snd : this->Data.Items) {
		this->Stop(snd.second.ID, fadeTime, afterDelete);
	}
}


//####################  �ꎞ��~  ####################
//ID�� -1 ���w�肷��ƁA���ׂĈꎞ��~����
void CMaterial::CMaterialSound::Pause(int hdl, double fadeTime) {
	if(hdl == -1) {
		//���ׂĈꎞ��~
		for(auto& it : this->Data.Items) {
			this->Pause(it.second.ID, fadeTime);
		}
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��T�E���h [ID: %d] ���ꎞ��~���悤�Ƃ��܂����B", hdl);
		return;
	}

	//�P��̈ꎞ��~
	auto it = *this->Data.Get(hdl);
	if(it.IsMIDI) {
		GGS->Stop(fadeTime * 1000);
	} else {
		it.CurrentPos = DxLib::GetCurrentPositionSoundMem(it.ID);
		if(fadeTime > 0.0F) {
			//MT�֐����o�R����
			Game.MT.AddThread("FadeStop", ::FadeStop, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", DxLib::GetVolumeSoundMem2(it.ID))
				.AddInteger("IsPause", true)
				.AddInteger("Dispose", false);
		} else {
			//�����ɏ�������
			DxLib::StopSoundMem(it.ID);
		}
	}
}


//####################  �ꎞ��~�����n�_����ĊJ  ####################
//ID�� -1 ���w�肷��ƁA���ׂčĊJ����
void CMaterial::CMaterialSound::Resume(int hdl, bool loopMode, double fadeTime) {
	if(hdl == -1) {
		//���ׂčĊJ
		for(auto& it : this->Data.Items) {
			this->Resume(it.second.ID, loopMode, fadeTime);
		}
		return;
	} else if(!this->Data.Exists(hdl)) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "���݂��Ȃ��T�E���h [ID: %d] ���ĊJ���悤�Ƃ��܂����B", hdl);
		return;
	}

	//�P��ĊJ
	auto it = *this->Data.Get(hdl);
	if(it.IsMIDI) {
		GGS->Pause();
	} else {
		DxLib::SetCurrentPositionSoundMem(it.CurrentPos, it.ID);		//�Đ��ʒu�𕜌����Ă���Đ�����
		if(fadeTime > 0.0F) {
			//MT�֐����o�R����
			Game.MT.AddThread("FadePlay", ::FadePlay, TS::System::Scene::All)
				.AddInteger("Handle", it.ID)
				.AddInteger("MaxVolume", it.Volume)
				.AddDecimal("PlusPerLoop", it.Volume / fadeTime / TS::System::FixedFPS)
				.AddDecimal("CurrentVolume", 0.0F);
			DxLib::PlaySoundMem(it.ID, (!loopMode) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(0, it.ID);
		} else {
			//�����ɏ�������
			DxLib::PlaySoundMem(it.ID, (!loopMode) ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP);
			DxLib::ChangeVolumeSoundMem(it.Volume, it.ID);
		}
	}
}


//####################  �w�肵���T�E���h�̍Đ���Ԃ��擾  ####################
int CMaterial::CMaterialSound::CheckPlayStatus(int hdl) {
	if(!this->Data.Exists(hdl)) {
		return static_cast<int>(TS::Material::PlayStatus::Error);
	}

	//MIDI�͎擾���@���قȂ�
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


//####################  ���ʂ�ύX  ####################
void CMaterial::CMaterialSound::setStandardVolume(int value) {
	this->StandardVolume = Game.WithIn(value, TS::Material::MinVolume, TS::Material::MaxVolume);
}
