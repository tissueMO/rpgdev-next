//###################################################################
//			�G�t�F�N�g �f�[�^�x�[�X
//###################################################################
#include "Header.h"

bool ShowEffect(TS_MT);


//####################  �j������  ####################
void CDatabase::CDBEffect::Dispose() {
	vector<CDBEffectData>().swap(this->PlayingList);
}


//####################  �G�t�F�N�g�Đ�: �o�b�N�O���E���h  ####################
int CDatabase::CDBEffect::Show(int ID, int destX, int destY) {
	if(this->GetSubDB(0)->GetRowFromID(ID) == nullptr) {
		return -1;		//���݂��Ȃ��G�t�F�N�g�͏������Ȃ�
	}

	//���ɓ�����W�œ���G�t�F�N�g���Đ�����Ă���ꍇ�͏������Ȃ�
	for(auto& effect : this->PlayingList) {
		if(effect.ID == ID && effect.TargetPosition.X == destX && effect.TargetPosition.Y == destY) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Exists, "���ɓ���G�t�F�N�g [%d] ��������W [%d, %d]�ōĐ�����Ă��܂��B", ID, destX, destY);
			return -1;
		}
	}

	//�Đ������X�g�֓o�^
	CDBEffectData NewItem;
	NewItem.ID = ID;
	NewItem.Handle = Game.NextRand(0, INT_MAX);
	NewItem.Name = *this->GetSubDB(0)->GetCell(ID, this->GetSubDB(0)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).c_str())->GetStringPTR();
	NewItem.SQHandle = -1;
	NewItem.Frame = -1;
	NewItem.TargetType = *this->GetSubDB(0)->GetCell(ID, this->GetSubDB(0)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBEffectColumnIndices::DrawDest)).c_str())->GetIntegerPTR();
	NewItem.TargetPosition.Set(destX, destY);
	this->PlayingList.push_back(NewItem);

	//����G�t�F�N�g��\�����Ă��Ȃ��Ƃ���MT�֐����Z�b�g
	if(Game.MT.CountThread("ShowEffect") == 0) {
		Game.MT.AddThread("ShowEffect", ::ShowEffect, TS::System::Scene::All)
			.AddInteger("EffectAsyncLoadMode", Game.BoolToInt(Game.DB.getFDBSystem()->GetBoolean(TS::DB::System::EffectAsyncLoadMode)))
			.AddInteger("EffectFPS", Game.DB.getFDBSystem()->GetInteger(TS::DB::System::EffectFPS));
	}
	return NewItem.Handle;
}


//####################  �G�t�F�N�g�Đ�: �I���܂ő҂�  ####################
void CDatabase::CDBEffect::ShowForWait(int id, int destX, int destY) {
	const int handle = this->Show(id, destX, destY);
	while(true) {
		const auto effect = this->GetEffectData(handle);
		if(effect == nullptr || effect->WaitBreakFlag) {
			return;		//�Đ��I�����邩�A�ҋ@���f���}������Ƒҋ@�I������
		}
		Game.Wait(0.1, true, false);
	}
}


//####################  �Đ����G�t�F�N�g�̃n���h������G�t�F�N�g�����擾  ####################
CDatabase::CDBEffect::CDBEffectData* CDatabase::CDBEffect::GetEffectData(int handle) {
	for(auto& data : this->PlayingList) {
		if(data.Handle == handle) {
			return &data;
		}
	}
	return nullptr;
}


//####################  ���C���[�O���t�B�b�N�ǂݍ���  ####################
void CDatabase::CDBEffect::CDBEffectData::LoadGraph(int layerIndex, string fileName, int secSizeWidth, int secSizeHeight) {
	CDBEffectLayerData NewItem;
	NewItem.Name = fileName;
	NewItem.ID = Game.Material.getGraphics()->Add(fileName);
	NewItem.SectionSize.Set(secSizeWidth, secSizeHeight);
	this->Graphics.Add(NewItem);
}


//####################  ���C���[�O���t�B�b�N�`��  ####################
void CDatabase::CDBEffect::CDBEffectData::Draw(int layerIndex, int destX, int destY, int secX, int secY, int blendMode, int trans, int ext, int angle) {
	DxLib::SetDrawBlendMode(Game.DXLib.ConvertToDXBlend(blendMode), 255 - trans);

	//�����œn���`���̍��W�͒��S���W
	auto layer = this->Graphics.GetFromIndex(layerIndex);
	Game.DXLib.DrawRectRotaGraph(
		this->Graphics.GetFromIndex(layerIndex)->ID,
		this->TargetPosition.X + destX, this->TargetPosition.Y + destY,
		secX * layer->SectionSize.Width, secY * layer->SectionSize.Height,
		layer->SectionSize.Width, layer->SectionSize.Height, (double)angle * PI / 180.0, (double)ext / 100, true, false);
}


//####################  ���߁F���ʉ��Đ�  ####################
void CDatabase::CDBEffect::CDBEffectData::PlaySE(int id, int volume, int pitch, int pan, int sameCounter) {
	//���̃t���[�����ŏ��ɌĂяo���ꂽ�Ƃ��Ɏ��s�����
	if(sameCounter == 0) {
		//�t�@�C�������f�[�^�x�[�X�����������
		auto data = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Sound))->GetRowFromID(id);
		string fileName = *data->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR();
		if(fileName.find(".mid") != string::npos) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Except, "�G�t�F�N�g�̌��ʉ���MIDI�t�@�C���͑Ή����Ă��܂���B\n>%s", fileName.c_str());
			return;
		}

		//NOTE: ���ʁE�s�b�`�E�p����ݒ肷�邽�߂ɐV���ɓǂݍ���
		CSoundData temp;
		temp.Load(fileName, volume, pitch, pan);
		temp.Play(static_cast<int>(TS::Material::PlayType::Back), true);			//�g���̂Ẵn���h���Ƃ��čĐ����A�Đ���͎����Ŕj�������
	}
}


//####################  ���߁F�t���b�V��  ####################
void CDatabase::CDBEffect::CDBEffectData::Flash(int color, int strength, int count, int sameCounter) {
	//���̃t���[�����ŏ��ɌĂяo���ꂽ�Ƃ��Ɏ��s�����
	if(sameCounter == 0) {
		Game.DXLib.Flash(color, strength, static_cast<int>(TS::Map::SquareSpeed::Moment), count);
	}
}


//####################  ���߁F��ʐU��  ####################
void CDatabase::CDBEffect::CDBEffectData::Shake(int strength, int sameCounter) {
	//���̃t���[�����ŏ��ɌĂяo���ꂽ�Ƃ��Ɏ��s�����
	if(sameCounter == 0) {
		Game.DXLib.ShakeOne(strength);
	}
}


//####################  ���߁F�ҋ@���f  ####################
void CDatabase::CDBEffect::CDBEffectData::WaitBreak() {
	this->WaitBreakFlag = true;
}


//####################  �l�s���s�F�G�t�F�N�g��\������ ####################
//���ӁF�G�t�F�N�g�̐ݒ�Łu��ʒ��S�v�Ƃ���Ă���ꍇ�͏�ɉ�ʒ��S�ŕ\�������
bool ShowEffect(TS_MT) {
	auto db = Game.DB.getFDBEffect();

	//�Đ����̃G�t�F�N�g�����ׂď�������
	TS_COUNTLOOP(i, db->PlayingList.size()) {
		int FrameLength = -1;				//�G�t�F�N�g�X�N���v�g�ɂ���Ď������S�t���[����
		CDatabase::CDBEffect::CDBEffectData& CurrentEffect = db->PlayingList[i];

		if(CurrentEffect.Frame == -1) {
			//�����ݒ�E�X�N���v�g�ǂݍ���
			CurrentEffect.SQHandle = Game.SQ.Add(Game.Format(TS::FileNames::SQDBEffect, CurrentEffect.ID));
			if(*db->GetSubDB(0)->GetCell(CurrentEffect.ID, TS::DB::ColumnNameEffectTargetType)->GetIntegerPTR() == static_cast<int>(TS::DB::DBEffectViewPosition::CenterScreen)) {
				//��ʒ��S�ɍ��W���Z�b�g����
				CurrentEffect.TargetPosition.X = Game.WindowSize.Width / 2;
				CurrentEffect.TargetPosition.Y = Game.WindowSize.Height / 2;
			}

			//�G�t�F�N�g��񓯊��œǂݍ��ސݒ�̏ꍇ
			if(*Data.GetInteger("EffectAsyncLoadMode") == TRUE) {
				DxLib::SetUseASyncLoadFlag(true);		//�񓯊��ǂݍ��݂��J�n
			}
		}

		//�G�t�F�N�g�̓ǂݍ��݂��n�܂��Ă��Ȃ�or�������Ă���ꍇ�̂݁A�X�N���v�g���Ăяo��
		if(CurrentEffect.Frame >= -1) {
			//�t���[�����̓f�[�^�x�[�X�����������E���̃Z���͐��l�ł͂Ȃ�������ł��邱�Ƃɒ��ӂ���
			FrameLength = *db->GetSubDB(0)->GetCell(CurrentEffect.ID, db->GetSubDB(0)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + static_cast<int>(TS::DB::DBEffectColumnIndices::FrameLength)).c_str())->GetIntegerPTR();
			//�X�N���v�g���Ăяo���ăG�t�F�N�g��`�悷��E�t���[��-1�̂Ƃ��̓O���t�B�b�N�ǂݍ��݂��s��
			int sameLength = TS::System::FixedFPS / *Data.GetInteger("EffectFPS");		//�G�t�F�N�g�̓���t���[����������s����邩������
			if(sameLength == 0) {
				sameLength = 1;
			}
			Game.SQ.DoFunc<int>(
				TS::SQ::SQEvent_PlayEffect,
				CurrentEffect.Handle,
				(CurrentEffect.Frame == -1) ? -1 : CurrentEffect.Frame / sameLength,	//�G�t�F�N�g�̂ǂ̃t���[�����Đ����邩
				CurrentEffect.Frame % sameLength										//����t���[���̉���ڂ̍Đ���
				);

			DxLib::SetUseASyncLoadFlag(false);		//�񓯊��ǂݍ��݂�����
			TS_NOBLEND;

			//�G�t�F�N�g�̏I�[�̎��̃t���[���Ō�n�����s��
			if(FrameLength + 1 <= CurrentEffect.Frame / sameLength) {
				TS_COUNTLOOP(l, db->PlayingList[i].Graphics.GetCount()) {
					Game.Material.getGraphics()->Delete(db->PlayingList[i].Graphics.GetFromIndex(l)->ID);
				}
				db->PlayingList[i].Graphics.Clear();
//				Game.SQ.Delete(CurrentEffect.SQHandle);			//�X�N���v�g�j��
				db->PlayingList.erase(db->PlayingList.begin() + i);
				i--;
				continue;		//���̃��[�v��
			}
		}

		//�񓯊��̏ꍇ�͓ǂݍ��݂��I���܂ő҂�
		if(CurrentEffect.Frame < 0 && *Data.GetInteger("EffectAsyncLoadMode") == TRUE) {
			CurrentEffect.Frame--;

			bool isOK = true;
			for(auto& graph : CurrentEffect.Graphics.Items) {
				//�ǂݍ��݂ɃG���[���������Ă���ꍇ�͖�������i�����������Ƃɂ���j
				if(::CheckHandleASyncLoad(graph.ID) == TRUE) {
					isOK = false;
					break;
				}
			}

			//�ǂݍ��݂�����������Đ��t���[���ֈڂ�
			if(isOK) {
				CurrentEffect.Frame = 0;
			}
		} else {
			CurrentEffect.Frame++;
		}
	}

	//�Đ�����G�t�F�N�g���Ȃ��Ȃ�����I������
	if(Game.DB.getFDBEffect()->PlayingList.size() == 0) {
		return true;
	}
	return false;
}
