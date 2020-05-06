//###################################################################
//			フォント素材管理クラス定義
//###################################################################
#include "Header.h"


//####################  フォントを追加  ####################
int CMaterial::CMaterialFont::Add(int dbType, int fixedID) {
	auto row = (Game.DB.getFDBMaterial()->GetSubDB(dbType)->GetRowFromID(fixedID));
	string fontName, fileName;
	int size, thick, lineSpace, space, type, edgeColor;

	//NOTE: データベースの行データからフォント情報を取得する	
	fontName = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 0).GetStringPTR();
	fileName = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 1).GetStringPTR();
	//SetButton = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 2).GetStringPTR();
	size = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 3).GetIntegerPTR();
	thick = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 4).GetIntegerPTR();
	lineSpace = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 5).GetIntegerPTR();
	space = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 6).GetIntegerPTR();
	type = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 7).GetIntegerPTR();

	//縁色を設定する
	auto edgeRGB = *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count) + 8).GetIntegers();
	if(edgeRGB.Count() == 3) {
		auto edgeRGBColor = CRGBColor(edgeRGB.Get(0), edgeRGB.Get(1), edgeRGB.Get(2));
		edgeColor = edgeRGBColor.ToCode();
	} else {
		edgeColor = BLACK;
	}

	return this->Add(fixedID, fontName, fileName, size, thick, lineSpace, space, type, edgeColor);
}

int CMaterial::CMaterialFont::Add(int dbID, string fontName, string fileName, int size, int thick, int lineSpace, int space, int type, int edgeColor) {
	CFontData NewItem;
	NewItem.DBID = dbID;
	NewItem.Name = fileName;
	NewItem.Size = size;
	NewItem.Thick = thick;
	NewItem.Type = type;
	NewItem.LineSpace = lineSpace;
	NewItem.Space = space;
	NewItem.IsOriginalFont = !fileName.empty();
	NewItem.EdgeColor = edgeColor;
	NewItem.FontName = fontName;

	//外部フォントの場合はロードを行う
	if(!fileName.empty()) {
		//DXアーカイブ後は使用できないので無効
		//if(!PathFileExists(fileName.c_str())) {
		//	Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "存在しないフォントファイルを使ってフォントを追加しようとしました。\n>%s", fileName.c_str());
		//} else {
#ifdef _DEBUG
			//デバッグ時: ファイルから直接リソースを読み込む
			::AddFontResourceEx(fileName.c_str(), FR_PRIVATE, nullptr);
#else
			//リリース時: DXアーカイブからバイナリを取り込んでからリソースを読み込む
			void* bin;
			int hdl = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
			if (hdl == 0) {
				Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::NotFound, "フォントファイルの取り出しに失敗しました。\n>%s", fileName.c_str());
				return -1;
			}
			int size = DxLib::FileRead_size((TS::DirectoryNames::PrePath + fileName).c_str());
			bin = malloc(size + 1);
			DxLib::FileRead_read(bin, size, hdl);

			DWORD font_num = 0;
			NewItem.OriginalFontHandle = ::AddFontMemResourceEx(bin, size, nullptr, &font_num);

			DxLib::FileRead_close(hdl);
			::free(bin);
#endif
		//}
	}

	//フォントタイプをDXライブラリ用に変換する
	int fontType = -1;
	switch((TS::DBLiteral::FontDrawing)type) {
		case TS::DBLiteral::FontDrawing::Normal:
			fontType = DX_FONTTYPE_NORMAL;
			break;
		case TS::DBLiteral::FontDrawing::AS:
			fontType = DX_FONTTYPE_ANTIALIASING_4X4;
			break;
		case TS::DBLiteral::FontDrawing::Edge:
			fontType = DX_FONTTYPE_EDGE;
			break;
		case TS::DBLiteral::FontDrawing::EdgeAS:
			fontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4;
			break;
	}

	NewItem.ID = DxLib::CreateFontToHandle(fontName.c_str(), size, thick, fontType);
	DxLib::SetFontSpaceToHandle(space, NewItem.ID);
	DxLib::SetFontLineSpaceToHandle(lineSpace, NewItem.ID);
	this->Data.Add(NewItem);
	return NewItem.ID;
}


//####################  フォントハンドルを復元  ####################
int CMaterial::CMaterialFont::Restore(int dbType, int dbID) {
	//既に読み込まれているフォントデータからデータベース上でのIDが一致するものを探す
	for(auto& item : this->Data.Items) {
		if(item.second.DBID == dbID) {
			return item.second.ID;
		}
	}
	return -1;
}


//####################  フォントを削除  ####################
void CMaterial::CMaterialFont::Delete(int ID) {
	if(this->Data.Get(ID)->IsOriginalFont) {
#ifdef _DEBUG
		::RemoveFontResourceEx(this->Data.Get(ID)->Name.c_str(), FR_PRIVATE, nullptr);
#else
		if(this->Data.Get(ID)->OriginalFontHandle != nullptr) {
			::RemoveFontMemResourceEx(this->Data.Get(ID)->OriginalFontHandle);
		}
#endif
	}
	DxLib::DeleteFontToHandle(this->Data.Get(ID)->ID);
	this->Data.Delete(ID);
}


//####################  フォントをすべて破棄し、標準フォントを追加する  ####################
void CMaterial::CMaterialFont::Clear() {
	//リソースを破棄
	for(auto& item : this->Data.Items) {
		if(item.second.IsOriginalFont) {
#ifdef _DEBUG
			::RemoveFontResourceEx(item.second.Name.c_str(), FR_PRIVATE, nullptr);
#else
			if(item.second.OriginalFontHandle != nullptr) {
				::RemoveFontMemResourceEx(item.second.OriginalFontHandle);
			}
#endif
		}
		DxLib::DeleteFontToHandle(item.second.ID);
	}

	//すべて破棄して標準フォントを追加する
	this->Data.Clear();
	this->AddDefaultFont();
}


//####################  既定のフォントを取得  ####################
CMaterial::CFontData CMaterial::CFontData::GetDefaultFont() {
	CFontData font;
	font.ID = -1;
	font.FontName = "ＭＳ ゴシック";
	font.Size = TS::Material::FontNormalSize;
	font.Thick = TS::Material::FontNormalThick;
	font.Type = TS::Material::FontNormalType;
	font.LineSpace = TS::Material::FontNormalLineSpace;
	font.Space = 0;
	font.IsOriginalFont = false;
	font.EdgeColor = WHITE;
	return font;
}


//####################  既定のフォントを追加する  ####################
void CMaterial::CMaterialFont::AddDefaultFont() {
	auto font = CFontData::GetDefaultFont();
	this->DefaultFontHandle = DxLib::CreateFontToHandle(font.FontName.c_str(), font.Size, font.Thick, font.Type);
	DxLib::SetFontSpaceToHandle(font.Space, this->DefaultFontHandle);
	DxLib::SetFontLineSpaceToHandle(font.LineSpace, this->DefaultFontHandle);
	this->Data.Add(font);
}
