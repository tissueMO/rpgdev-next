//###################################################################
//			汎用素材 データベース
//###################################################################
#include "Header.h"


//####################  読み込み処理  ####################
bool CDatabase::CDBMaterial::Load(string fileName) {
	//デフォルトの読み込み処理が先行する
	if(!CDBBase::Load(fileName)) {
		return false;		//読み込みに失敗した場合はリソースも読み込まずに中断する
	}

	//右端列にハンドル列を追加する: ここではまだリソースの読み込みを行わない
	TS_COUNTLOOP(i, static_cast<int>(TS::DB::DBMaterialIndices::Count)) {
		this->GetSubDB(i)->AddColumn(TS::DB::ColumnNameMaterialHandle);
		TS_COUNTLOOP(row, this->GetSubDB(i)->GetRowCount()) {
			auto newCell = CDBCell(static_cast<int>(TS::DB::ColumnType::OneNumber), std::to_string(-1));
			this->GetSubDB(i)->AddCell(row, &newCell);
		}
	}

	//既にリソースを読み込んでいる場合は、失われたハンドルを復元する
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


//####################  素材ハンドルを取得  ####################
int CDatabase::CDBMaterial::GetHandle(int type, int id) {
	if(type < 0) {
		return -1;
	} else if(static_cast<int>(TS::DB::DBMaterialIndices::Count) <= type) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外の素材種別 [%d - ID: %d] が参照されました。", type, id);
		return -1;
	}

	auto cell = this->GetSubDB(type)->GetCell(id, TS::DB::ColumnNameMaterialHandle);
	if(cell != nullptr) {
		return *cell->GetIntegerPTR();
	} else {
		return -1;
	}
}


//####################  指定したサブDBに対応する素材データベースを返す  ####################
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


//####################  サウンドデータを素材管理クラスを経由して再生する: バックグラウンドモード  ####################
void CDatabase::CDBMaterial::Play(int soundID, int volume, int pitch, int pan) {
	if(soundID != -1) {
		Game.Material.getSounds()->Play(this->GetHandle(static_cast<int>(TS::DB::DBMaterialIndices::Sound), soundID), static_cast<int>(TS::Material::PlayType::Back), volume, pitch, pan);
	}
}


//####################  サウンドデータを素材管理クラスを経由して再生する: 待機モード  ####################
void CDatabase::CDBMaterial::PlayForWait(int soundID, int volume, int pitch, int pan) {
	if(soundID != -1) {
		Game.Material.getSounds()->Play(this->GetHandle(static_cast<int>(TS::DB::DBMaterialIndices::Sound), soundID), static_cast<int>(TS::Material::PlayType::Wait), volume, pitch, pan);
	}
}


//####################  サウンドデータを素材管理クラスを経由して停止する  ####################
void CDatabase::CDBMaterial::Stop(int soundID) {
	if(soundID != -1) {
		Game.Material.getSounds()->Stop(this->GetHandle(static_cast<int>(TS::DB::DBMaterialIndices::Sound), soundID));
	}
}
