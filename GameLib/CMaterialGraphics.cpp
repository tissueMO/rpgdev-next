//###################################################################
//			グラフィック素材管理クラス定義
//###################################################################
#include "Header.h"


//####################  グラフィックを追加  ####################
int CMaterial::CMaterialGraphics::Add(int dbType, int dbID) {
	auto row = Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(dbID);
	return this->Add(*row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR());
}
int CMaterial::CMaterialGraphics::Add(string fileName) {
	CIDValueData NewItem;
	NewItem.Name = fileName;

	//ファイルチェック
	if(fileName.empty()) {
		//ファイル名が空の場合は読み込まない
		return -1;

		//DXアーカイブ後は使用できないので無効
		//} else if(!PathFileExists(fileName.c_str())) {
		//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないグラフィックを追加しようとしました。\n>%s", fileName.c_str());
	}

	//既に同じグラフィックが読み込まれていないか調べる
	for(auto& it : this->Data.Items) {
		if(it.second.Name == fileName) {
			it.second.Value++;		//参照カウンター加算
			return it.second.ID;
		}
	}

	//存在しなければ新たに読み込む
	NewItem.ID = DxLib::LoadGraph((TS::DirectoryNames::PrePath + fileName).c_str());
	NewItem.Value = 1;			//参照カウンター初期化
	this->Data.Add(NewItem);
	return NewItem.ID;
}


//####################  グラフィックハンドルを復元  ####################
int CMaterial::CMaterialGraphics::Restore(int dbType, int dbID) {
	string fileName = *Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(dbID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).GetStringPTR();

	//既に読み込まれているグラフィックデータからファイル名の一致するものを探す
	for(auto& item : this->Data.Items) {
		if(item.second.Name == fileName) {
			return item.second.ID;
		}
	}
	return -1;
}


//####################  グラフィックを削除  ####################
void CMaterial::CMaterialGraphics::Delete(int hdl) {
	if(!this->Data.Exists(hdl)) {
		return;		//存在しない場合は処理しない
	}

	//他に同一のグラフィックが残っている場合はまだリソースを削除しない
	for(auto& item : this->Data.Items) {
		if(item.second.ID == hdl) {
			item.second.Value--;
			if(item.second.Value > 0) {
				return;		//まだ残っている
			} else {
				break;		//もう残っていない
			}
		}
	}

	//残っていなければ実際に削除する
	DxLib::DeleteGraph(hdl);
	this->Data.Delete(hdl);
}


//####################  グラフィックをすべて削除  ####################
void CMaterial::CMaterialGraphics::Clear() {
	for(auto& it : this->Data.Items) {
		DxLib::DeleteGraph(it.second.ID);		//ハンドルをすべて削除
	}
	this->Data.Clear();
}


//####################  アイコングラフィックを読み込み  ####################
void CMaterial::CMaterialGraphics::LoadIconGraphic() {
	this->iconGraphicHandle = this->Add(Game.DB.getFDBSystem()->GetString(TS::DB::System::IconFileName));
	this->IconSize = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::IconSize);
}
