//###################################################################
//			�ėp�f�� �f�[�^�x�[�X
//###################################################################
#include "Header.h"


//####################  �ǂݍ��ݏ���  ####################
bool CDatabase::CDBMaterial::Load(string fileName) {
	//�f�t�H���g�̓ǂݍ��ݏ�������s����
	if(!CDBBase::Load(fileName)) {
		return false;		//�ǂݍ��݂Ɏ��s�����ꍇ�̓��\�[�X���ǂݍ��܂��ɒ��f����
	}

	//�E�[��Ƀn���h�����ǉ�����: �����ł͂܂����\�[�X�̓ǂݍ��݂��s��Ȃ�
	TS_COUNTLOOP(i, static_cast<int>(TS::DB::DBMaterialIndices::Count)) {
		this->GetSubDB(i)->AddColumn(TS::DB::ColumnNameMaterialHandle);
		TS_COUNTLOOP(row, this->GetSubDB(i)->GetRowCount()) {
			auto newCell = CDBCell(static_cast<int>(TS::DB::ColumnType::OneNumber), std::to_string(-1));
			this->GetSubDB(i)->AddCell(row, &newCell);
		}
	}

	//���Ƀ��\�[�X��ǂݍ���ł���ꍇ�́A����ꂽ�n���h���𕜌�����
	if(this->loaded) {
		TS_COUNTLOOP(type, static_cast<int>(TS::DB::DBMaterialIndices::Count)) {
			TS_COUNTLOOP(row, Game.DB.getFDBMaterial()->GetSubDB(type)->GetRowCount()) {
				*Game.DB.getFDBMaterial()->GetSubDB(type)->GetCellFromIndex(row, Game.DB.getFDBMaterial()->GetSubDB(type)->GetColumnIndex(TS::DB::ColumnNameMaterialHandle))->GetIntegerPTR()
					= Game.DB.getFDBMaterial()->GetMaterialObject(type)->Restore(
						type,
						*Game.DB.getFDBMaterial()->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
			}
		}
	}

	this->loaded = true;
	return true;
}


//####################  �f�ރn���h�����擾  ####################
int CDatabase::CDBMaterial::GetHandle(int type, int id) {
	if(type < 0) {
		return -1;
	} else if(static_cast<int>(TS::DB::DBMaterialIndices::Count) <= type) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "�͈͊O�̑f�ގ�� [%d - ID: %d] ���Q�Ƃ���܂����B", type, id);
		return -1;
	}

	auto cell = this->GetSubDB(type)->GetCell(id, TS::DB::ColumnNameMaterialHandle);
	if(cell != nullptr) {
		return *cell->GetIntegerPTR();
	} else {
		return -1;
	}
}


//####################  �w�肵���T�uDB�ɑΉ�����f�ރf�[�^�x�[�X��Ԃ�  ####################
CMaterialBase* CDatabase::CDBMaterial::GetMaterialObject(int subDBIndex) {
	switch((TS::DB::DBMaterialIndices)subDBIndex) {
		case TS::DB::DBMaterialIndices::Face:
		case TS::DB::DBMaterialIndices::Graphics:
			return Game.Material.getGraphics();

		case TS::DB::DBMaterialIndices::Sound:
			return Game.Material.getSounds();

		case TS::DB::DBMaterialIndices::Font:
			return Game.Material.getFonts();
	}
	return nullptr;
}


//####################  �T�E���h�f�[�^��f�ފǗ��N���X���o�R���čĐ�����: �o�b�N�O���E���h���[�h  ####################
void CDatabase::CDBMaterial::Play(int soundID, int volume, int pitch, int pan) {
	if(soundID != -1) {
		Game.Material.getSounds()->Play(this->GetHandle(static_cast<int>(TS::DB::DBMaterialIndices::Sound), soundID), static_cast<int>(TS::Material::PlayType::Back), volume, pitch, pan);
	}
}


//####################  �T�E���h�f�[�^��f�ފǗ��N���X���o�R���čĐ�����: �ҋ@���[�h  ####################
void CDatabase::CDBMaterial::PlayForWait(int soundID, int volume, int pitch, int pan) {
	if(soundID != -1) {
		Game.Material.getSounds()->Play(this->GetHandle(static_cast<int>(TS::DB::DBMaterialIndices::Sound), soundID), static_cast<int>(TS::Material::PlayType::Wait), volume, pitch, pan);
	}
}


//####################  �T�E���h�f�[�^��f�ފǗ��N���X���o�R���Ē�~����  ####################
void CDatabase::CDBMaterial::Stop(int soundID) {
	if(soundID != -1) {
		Game.Material.getSounds()->Stop(this->GetHandle(static_cast<int>(TS::DB::DBMaterialIndices::Sound), soundID));
	}
}
