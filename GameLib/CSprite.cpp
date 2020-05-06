//###################################################################
//			�X�v���C�g�N���X��`
//###################################################################
#include "Header.h"


//####################  �X�v���C�g�O���t�B�b�N��`��  ####################
void CSprite::CSpriteData::CSpriteGraphicArea::Draw(CPoint pos) {
	Game.DXLib.DrawRectExtendGraph(this->ID, pos.X + this->Location.X, pos.Y + this->Location.Y, this->VisibleRange.Width * this->ExtendRateH / 100, this->VisibleRange.Height * this->ExtendRateV / 100, this->VisibleRange.Left, this->VisibleRange.Top, this->VisibleRange.Width, this->VisibleRange.Height);
}


//####################  �X�v���C�g�e�L�X�g��`��  ####################
void CSprite::CSpriteData::CSpriteTextArea::Draw(CPoint pos) {
	Game.DXLib.SetDrawArea(this->DrawArea);
	Game.DXLib.DrawStringAlign(pos.X + this->Location.X, pos.Y + this->Location.Y, this->Align, this->FontID, this->Color, this->Text.c_str());
	Game.DXLib.ResetDrawArea();
}


//####################  �����͈͂�����  ####################
void CSprite::CSpriteData::CSpriteGraphicArea::InitVisibleRange() {
	this->VisibleRange.Set(0, 0, Size.Width, Size.Height);
}


//####################  ���̃X�v���C�g�ɃO���t�B�b�N��ǉ�  ####################
// baseHandle �� -1 �ȊO�ł���ꍇ�A�V�K�ɓǂݍ��܂��ɂ��̃n���h�����R�s�[����
int CSprite::CSpriteData::AddGraphics(string path, int x, int y, int baseHandle) {
	CSpriteGraphicArea NewItem;

	NewItem.IsNewGraph = (baseHandle == -1);
	if(baseHandle == -1) {
		//�f�ނ�ǂݍ���
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
	return this->Graphics.GetCount() - 1;		//�V�����������ꂽ�O���t�B�b�N�̃C���f�b�N�X��Ԃ�
}


//####################  ���̃X�v���C�g�̎w��O���t�B�b�N��ύX  ####################
void CSprite::CSpriteData::ChangeGraphics(int index, string path, int x, int y, int baseHandle) {
	CSpriteGraphicArea *CurrentItem = this->Graphics.GetFromIndex(index);

	//�ȑO�̃O���t�B�b�N���폜
	if(CurrentItem->IsNewGraph) {
		Game.Material.getGraphics()->Delete(CurrentItem->ID);
	}

	//�f�ނ�ǂݍ���
	CurrentItem->IsNewGraph = (baseHandle == -1);
	if(baseHandle == -1) {
		CurrentItem->ID = Game.Material.getGraphics()->Add(path);
		CurrentItem->Name = path;
	} else {
		CurrentItem->ID = baseHandle;
		CurrentItem->Name = "(BaseGraphics)";
	}
}


//####################  ���̃X�v���C�g�̎w��O���t�B�b�N���폜  ####################
void CSprite::CSpriteData::DeleteGraphics(int index) {
	if(this->Graphics.GetFromIndex(index)->IsNewGraph) {
		Game.Material.getGraphics()->Delete(this->Graphics.GetFromIndex(index)->ID);
	}
	this->Graphics.DeleteFromIndex(index);
}


//####################  ���̃X�v���C�g�̎w��O���t�B�b�N�f�[�^���擾  ####################
CSprite::CSpriteData::CSpriteGraphicArea* CSprite::CSpriteData::GetGraphics(int index) {
	return this->Graphics.GetFromIndex(index);
}


//####################  ���̃X�v���C�g�Ƀe�L�X�g��ǉ�  ####################
int CSprite::CSpriteData::AddText(string text, int x, int y, int color, int FontID, int align) {
	CSpriteTextArea NewItem;
	NewItem.Text = text;
	NewItem.Location.Set(x, y);
	NewItem.Color = color;
	NewItem.FontID = FontID;
	NewItem.Align = align;
	NewItem.DrawArea.Set(0, 0, Game.WindowSize.Width, Game.WindowSize.Height);
	this->Texts.Add(NewItem);
	return this->Texts.GetCount() - 1;		//�V�����������ꂽ�e�L�X�g�̃C���f�b�N�X��Ԃ�
}


//####################  ���̃X�v���C�g�̎w��e�L�X�g��ύX  ####################
void CSprite::CSpriteData::ChangeText(int index, string text) {
	this->Texts.GetFromIndex(index)->Text = text;
}


//####################  ���̃X�v���C�g�̎w��e�L�X�g���폜  ####################
void CSprite::CSpriteData::DeleteText(int index) {
	this->Texts.DeleteFromIndex(index);
}


//####################  ���̃X�v���C�g�̎w��O���t�B�b�N�f�[�^���擾  ####################
CSprite::CSpriteData::CSpriteTextArea* CSprite::CSpriteData::GetText(int index) {
	return this->Texts.GetFromIndex(index);
}


//####################  ���̃X�v���C�g��ʏ�`��  ####################
void CSprite::CSpriteData::Draw() {
	//�O���t�B�b�N��
	for(auto& g : this->Graphics.Items) {
		g.Draw(this->Location);
	}

	//�e�L�X�g��
	for(auto& t : this->Texts.Items) {
		t.Draw(this->Location);
	}
}


//####################  ���̃X�v���C�g�����C�v�C��  ####################
void CSprite::CSpriteData::WipeIn(int type, int wipeSpeed) {
	//���x���O�܂��̓}�C�i�X�̏ꍇ�͕W�����x�ɂ���
	if(wipeSpeed <= 0) {
		wipeSpeed = static_cast<int>(TS::Map::Speed::High);
	}

	this->WipeType = type;
	this->WipeCounter = 0;
	this->WipeSpeed = wipeSpeed;
	this->WipeState = static_cast<int>(TS::Sprite::WipeState::WipeIn);
	this->DeleteAfterWipe = false;
}


//####################  ���̃X�v���C�g�����C�v�A�E�g  ####################
void CSprite::CSpriteData::WipeOut(int type, int wipeSpeed, bool afterDelete) {
	//���x���O�܂��̓}�C�i�X�̏ꍇ�͕W�����x�ɂ���
	if(wipeSpeed <= 0) {
		wipeSpeed = static_cast<int>(TS::Map::Speed::High);
	}

	this->WipeType = type;
	this->WipeCounter = 255;
	this->WipeSpeed = wipeSpeed;
	this->WipeState = static_cast<int>(TS::Sprite::WipeState::WipeOut);
	this->DeleteAfterWipe = afterDelete;
}


//####################  �X�v���C�g��ǉ�  ####################
void CSprite::Add(int ID) {
	auto newItem = CSpriteData();
	newItem.Hide = false;
	newItem.ID = ID;
	newItem.WipeState = static_cast<int>(TS::Sprite::WipeState::None);
	newItem.Trans = 0;
	this->Data.Add(newItem);
}


//####################  �X�v���C�g���폜  ####################
void CSprite::Delete(int ID) {
	this->releaseResource(this->Data.GetFromID(ID));
	this->Data.DeleteFromID(ID);
}


//####################  �X�v���C�g���ێ����郊�\�[�X���������  ####################
void CSprite::releaseResource(CSpriteData* data) {
	//�O���t�B�b�N�n���h���𓪂��珇�ɂ��ׂĔj��
	while(this->Data.GetFromID(data->ID)->Graphics.GetCount() > 0) {
		this->Data.GetFromID(data->ID)->DeleteGraphics(0);
	}
}


//####################  �X�v���C�g���N���A  ####################
//wipeSpeed=255�ɂ���Əu�ԍ폜�ɂȂ�
void CSprite::Clear(int wipeType, int wipeSpeed) {
	for(auto& Item : this->Data.Items) {
		this->Delete(Item.ID);
	}
}


//####################  �X�v���C�g�f�[�^���擾  ####################
CSprite::CSpriteData* CSprite::Get(int ID) {
	return this->Data.GetFromID(ID);
}


//####################  �P���̐V�K�摜���������X�v���C�g��ǉ�  ####################
void CSprite::AddNewGraphics(int ID, string path, int x, int y) {
	this->Add(ID);
	this->Data.GetFromID(ID)->AddGraphics(path, x, y);
}


//####################  �P���̊����摜���������X�v���C�g��ǉ�  ####################
void CSprite::AddLoadedGraphics(int ID, int graphID, int x, int y) {
	this->Add(ID);
	this->Data.GetFromID(ID)->AddGraphics("", x, y, graphID);
}


//####################  �P���̐V�K�摜���������X�v���C�g��ǉ����ă��C�v�C��  ####################
void CSprite::AddWipeNewGraphics(int ID, string path, int x, int y, int wipeType, int wipeSpeed) {
	this->AddNewGraphics(ID, path, x, y);
	this->Data.GetFromID(ID)->WipeIn(wipeType, wipeSpeed);
}


//####################  �P���̊����摜���������X�v���C�g��ǉ����ă��C�v�C��  ####################
void CSprite::AddWipeLoadedGraphics(int ID, int graphID, int x, int y, int wipeType, int wipeSpeed) {
	this->AddLoadedGraphics(ID, graphID, x, y);
	this->Data.GetFromID(ID)->WipeIn(wipeType, wipeSpeed);
}


//####################  �X�v���C�g���R�s�[  ####################
void CSprite::Copy(int newID, int baseID, int x, int y) {
	if(newID == baseID) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�X�v���C�g�̃R�s�[���ƃR�s�[���ID������ł��B");
		return;
	}
	auto newItem = CSpriteData(*this->Data.GetFromID(baseID));
	newItem.ID = newID;
	this->Data.Add(newItem);
}


//####################  �X�v���C�g���R�s�[���ă��C�v�C��  ####################
void CSprite::CopyWipe(int newID, int baseID, int x, int y, int wipeType, int wipeSpeed) {
	this->Copy(newID, baseID, x, y);
	this->Data.GetFromID(newID)->WipeIn(wipeType, wipeSpeed);
}


//####################  �w��͈͂�ID�����X�v���C�g����ĂɃ��C�v�C��  ####################
void CSprite::WipeInRange(int startID, int endID, int wipeType, int wipeSpeed) {
	for(int ID = startID; ID <= endID; ID++) {
		if(!this->Data.Exists(ID)) {
			continue;
		}
		this->Data.GetFromID(ID)->WipeIn(wipeType, wipeSpeed);
	}
}


//####################  �w��͈͂�ID�����X�v���C�g����ĂɃ��C�v�A�E�g  ####################
void CSprite::WipeOutRange(int startID, int endID, int wipeType, int wipeSpeed, bool afterDelete) {
	for(int ID = startID; ID <= endID; ID++) {
		if(!this->Data.Exists(ID)) {
			continue;
		}
		this->Data.GetFromID(ID)->WipeOut(wipeType, wipeSpeed, afterDelete);
	}
}


//####################  ���ׂẴX�v���C�g��`�悷��  ####################
void CSprite::DrawAll() {
	if(this->ForbidLayerDraw) {
		return;
	}

	//�S�X�v���C�g�𑖍�
	for(auto& sprite : this->Data.Items) {
		//���C�v���ł���ꍇ�̓��C�v������i�߂�
		if(sprite.WipeState != static_cast<int>(TS::Sprite::WipeState::None)) {
			sprite.WipeCounter += (sprite.WipeState == static_cast<int>(TS::Sprite::WipeState::WipeIn)) ? +sprite.WipeSpeed : -sprite.WipeSpeed;

			//�J�E���^�[���[�_�ɒB�����ꍇ�̓��C�v����
			if(sprite.WipeCounter <= 0) {
				sprite.WipeState = static_cast<int>(TS::Sprite::WipeState::None);
				sprite.Hide = true;
			} else if(255 <= sprite.WipeCounter) {
				sprite.WipeState = static_cast<int>(TS::Sprite::WipeState::None);
			}
		}

		//��\���łȂ��ꍇ�̂ݕ`��
		if(!sprite.Hide) {
			if(sprite.WipeState == static_cast<int>(TS::Sprite::WipeState::None)) {
				//�ʏ�`��
				TS_ALPHABLEND(255 - sprite.Trans);
				sprite.Draw();
				TS_NOBLEND;
			} else {
				//���C�v���̏ꍇ�̓X�N���v�g�Ăяo��
				Game.SQ.DoFunc<int>(TS::SQ::SQEvent_WipeSprite, &sprite);
			}
		}
	}

	//���C�v���������Ă���폜�҂��̃X�v���C�g���폜����
	this->Data.Items.erase(std::remove_if(TS_CONTAINERALL(this->Data.Items), [&](auto& item) {
		if(item.WipeState == static_cast<int>(TS::Sprite::WipeState::None) && item.DeleteAfterWipe) {
			this->releaseResource(&item);		//vector����폜����O�ɕێ����Ă��郊�\�[�X��j������
			return true;
		}
		return false;
	}), this->Data.Items.end());
}
