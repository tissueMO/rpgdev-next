//###################################################################
//		DXライブラリラッパークラス定義
//###################################################################
//文字列描画は複数行文字列にも対応
#include "Header.h"
#include "DXArchive.h"
#include "DXArchiveVer5.h"
#include "FileLib.h"

bool ScreenFlash(TS_MT);
bool ScreenWindowShake(TS_MT);
extern char buf[TS::System::BufferLengthMax];


//####################  画面フラッシュ  ####################
void CDXLibWrapper::Flash(int color, int strength, int speed, int count) {
	Game.MT.DeleteThread("画面フラッシュ");
	Game.MT.AddThread("画面フラッシュ", ::ScreenFlash, TS::System::Scene::All)
		.AddInteger("Color", color)
		.AddInteger("Strength", strength)
		.AddInteger("Speed", speed)
		.AddInteger("Count", count)
		.AddInteger("NowCount", 0)
		.AddInteger("NowStrength", 0);
}
//Data.Vint["Color"] = 色
//Data.Vint["Strength"] = 強さ
//Data.Vint["Speed"] = 速さ
//Data.Vint["Count"] = 回数
//Data.Vint["NowStrength"] = 現在の強さ（内部）
//Data.Vint["NowCount"] = 現在の回数（内部）
bool ScreenFlash(TS_MT) {
	TS_ALPHABLEND(*Data.GetInteger("Strength") - *Data.GetInteger("NowStrength"));
	DxLib::DrawBox(0, 0, Game.WindowSize.Width, Game.WindowSize.Height, *Data.GetInteger("Color"), true);
	TS_NOBLEND;

	*Data.GetInteger("NowStrength") += *Data.GetInteger("Speed");
	if(*Data.GetInteger("NowStrength") >= *Data.GetInteger("Strength")) {
		*Data.GetInteger("NowCount") += 1;
		*Data.GetInteger("NowStrength") = 0;
		if(*Data.GetInteger("NowCount") >= *Data.GetInteger("Count")) {
			return true;
		}
	}
	return false;
}


//####################  画面振動：単発  ####################
void CDXLibWrapper::ShakeOne(int strength) {
	Game.MT.DeleteThread("画面振動：単発");
	Game.MT.AddThread("画面振動：単発", ::ScreenWindowShake, TS::System::Scene::All)
		.AddInteger("Strength", strength)
		.AddInteger("TimeCounter", 0)
		.AddInteger("PosX", 0)
		.AddInteger("PosY", 0)
		.AddInteger("KeepMode", FALSE);
}
//####################  画面振動：維持 を開始  ####################
void CDXLibWrapper::StartShake(int strength) {
	Game.MT.DeleteThread("画面振動：維持");
	Game.MT.AddThread("画面振動：維持", ::ScreenWindowShake, TS::System::Scene::All)
		.AddInteger("Strength", strength)
		.AddInteger("TimeCounter", 0)
		.AddInteger("PosX", 0)
		.AddInteger("PosY", 0)
		.AddInteger("KeepMode", TRUE);
}
//####################  画面振動：維持 を停止  ####################
void CDXLibWrapper::StopShake() {
	Game.MT.DeleteThread("画面振動：維持");
}
//Data.Vint["Strength"] = 振動の強度
//Data.Vint["TimeCounter"] = 振動経過時間
//Data.Vint["PosX"] = 振動X
//Data.Vint["PosY"] = 振動Y
//Data.Vint["KeepMode"] = 減衰せずに維持するかどうか
bool ScreenWindowShake(TS_MT) {
	*Data.GetInteger("TimeCounter") += 1;
	*Data.GetInteger("PosX") = sin(static_cast<double>(PI / 180) * (*Data.GetInteger("TimeCounter") * 90/*振動定数*/)) * *Data.GetInteger("Strength");
	*Data.GetInteger("PosY") = cos(static_cast<double>(PI / 180) * (*Data.GetInteger("TimeCounter") * 90/*振動定数*/)) * *Data.GetInteger("Strength");
	if(*Data.GetInteger("TimeCounter") % 4 == 0) {
		*Data.GetInteger("Strength") -= 1;		//減衰させる
	}

	//画面基準位置の微調整を行う
	Game.DXLib.Offset.Set(*Data.GetInteger("PosX"), *Data.GetInteger("PosY"));

	//元の状態に戻ったら終了
	if(*Data.GetInteger("Strength") <= 1) {
		Game.DXLib.Offset.Set(0, 0);
		return true;
	}
	return false;
}


//####################  指定領域にウィンドウを描画  ####################
void CDXLibWrapper::DrawWindow(int hdl, int intX, int intY, int intWidth, int intHeight) {
	//背景色: 拡大縮小 (ただし四隅 2pix は丸いウィンドウのために除外される)
	this->DrawRectExtendGraph(hdl, intX + 2, intY + 2, intWidth - 2 * 2, intHeight - 2 * 2, 2, 2, 64 - 2 * 2, 64 - 2 * 2, true);

	//背景パターン: タイリング
	for(int x = intX + 2; x < intX + intWidth - 2; x += 64) {
		//端に来たら切り取り幅を調整する
		int sizeX = 64;
		if(x + 64 > intX + intWidth - 2) {
			sizeX = intX + intWidth - 2 - x;
		}

		for(int y = intY + 2; y < intY + intHeight - 2; y += 64) {
			//端に来たら切り取り高さを調整する
			int sizeY = 64;
			if(y + 64 > intY + intHeight - 2) {
				sizeY = intY + intHeight - 2 - y;
			}

			//タイリング描画
			this->DrawRectGraph(hdl, x, y, 0, 64, sizeX, sizeY, true, false);
		}
	}

	//枠: 四隅は 左上、右上、左下、右下 の順、それ以外の辺はタイリング
	this->DrawRectGraph(hdl, intX, intY, 64 + 0, 0, 16, 16, true, false);
	this->DrawRectGraph(hdl, intX + intWidth - 16, intY, 64 + 48, 0, 16, 16, true, false);
	this->DrawRectGraph(hdl, intX, intY + intHeight - 16, 64 + 0, 48, 16, 16, true, false);
	this->DrawRectGraph(hdl, intX + intWidth - 16, intY + intHeight - 16, 64 + 48, 48, 16, 16, true, false);
	for(int x = intX + 16; x < intX + intWidth - 16; x += 16) {
		//横方向タイリング: 端に来たら切り取り幅を調整する
		int sizeX = 16;
		if(x + 16 > intX + intWidth - 16) {
			sizeX = intX + intWidth - 16 - x;
		}
		this->DrawRectGraph(hdl, x, intY, 64 + 16, 0, sizeX, 16, true, false);
		this->DrawRectGraph(hdl, x, intY + intHeight - 16, 64 + 16, 64 - 16, sizeX, 16, true, false);
	}
	for(int y = intY + 16; y < intY + intHeight - 16; y += 16) {
		//縦方向タイリング: 端に来たら切り取り高さを調整する
		int sizeY = 16;
		if(y + 16 > intY + intHeight - 16) {
			sizeY = intY + intHeight - 16 - y;
		}
		this->DrawRectGraph(hdl, intX, y, 64, 16, 16, sizeY, true, false);
		this->DrawRectGraph(hdl, intX + intWidth - 16, y, 64 + 64 - 16, 16, 16, sizeY, true, false);
	}
}


//####################  指定領域にパネル型のカーソルを描画  ####################
void CDXLibWrapper::DrawCursorPanel(int hdl, int intX, int intY, int intWidth, int intHeight) {
	//四隅は 左上、右上、左下、右下 の順
	this->DrawRectExtendGraph(hdl, intX, intY, 2, 2, 64, 64, 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX + intWidth - 2, intY, 2, 2, 64 + 32 - 2, 64, 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX, intY + intHeight - 2, 2, 2, 64, 64 + 32 - 2, 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX + intWidth - 2, intY + intHeight - 2, 2, 2, 64 + 32 - 2, 64 + 32 - 2, 2, 2, true);

	//四辺は 左、右、上、下 の順
	this->DrawRectExtendGraph(hdl, intX, intY + 2, 2, intHeight - 2 * 2, 64, 64 + 2, 2, 32 - 2 * 2, true);
	this->DrawRectExtendGraph(hdl, intX + intWidth - 2, intY + 2, 2, intHeight - 2 * 2, 64 + 32 - 2, 64 + 2, 2, 32 - 2 * 2, true);
	this->DrawRectExtendGraph(hdl, intX + 2, intY, intWidth - 2 * 2, 2, 64 + 2, 64, 32 - 2 * 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX + 2, intY + intHeight - 2, intWidth - 2 * 2, 2, 64 + 2, 64 + 32 - 2, 32 - 2 * 2, 2, true);

	//領域内部は拡大縮小
	this->DrawRectExtendGraph(hdl, intX + 2, intY + 2, intWidth - 2 * 2, intHeight - 2 * 2, 64 + 2, 64 + 2, 32 - 2 * 2, 32 - 2 * 2, true);
}


//####################  指定座標に矢印型のカーソルを描画  ####################
void CDXLibWrapper::DrawCursorArrow(int hdl, int intX, int intY, int intDir4) {
	switch(static_cast<TS::Map::Direction4>(intDir4)) {
		case TS::Map::Direction4::East:
			Game.DXLib.DrawRectGraph(hdl, intX, intY, 64 + 64 - 16 - 8, 16, 8, 32, true, false);
			break;
		case TS::Map::Direction4::West:
			Game.DXLib.DrawRectGraph(hdl, intX, intY, 64 + 16, 16, 8, 32, true, false);
			break;
		case TS::Map::Direction4::South:
			Game.DXLib.DrawRectGraph(hdl, intX, intY, 64 + 16, 64 - 16 - 8, 32, 8, true, false);
			break;
		case TS::Map::Direction4::North:
			Game.DXLib.DrawRectGraph(hdl, intX, intY, 64 + 16, 16, 32, 8, true, false);
			break;
	}
}


//####################  点描画  ####################
void CDXLibWrapper::DrawPixel(int x, int y, int color) {
	DxLib::DrawPixel(this->Offset.X + x, y, color);
}


//####################  線描画  ####################
void CDXLibWrapper::DrawLine(int sx, int sy, int ex, int ey, int color, int thickness) {
	DxLib::DrawLine(this->Offset.X + sx, this->Offset.Y + sy, this->Offset.X + ex, this->Offset.Y + ey, color, thickness);
}


//####################  三角描画  ####################
//三角形の頂点a, b, cの座標に従って描画する
void CDXLibWrapper::DrawTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color, bool fill) {
	DxLib::DrawTriangle(this->Offset.X + ax, this->Offset.Y + ay, this->Offset.X + bx, this->Offset.Y + by, this->Offset.X + cx, cy, color, fill);
}


//####################  矩形描画（絶対位置指定）  ####################
void CDXLibWrapper::DrawBox(int left, int top, int right, int bottom, int color, bool fill, int thickness) {
	TS_COUNTLOOP(i, thickness) {
		DxLib::DrawBox(this->Offset.X + left + i, this->Offset.Y + top + i, this->Offset.X + right - i, this->Offset.Y + bottom - i, color, fill);
	}
}


//####################  矩形描画（相対位置指定）  ####################
void CDXLibWrapper::DrawRect(int left, int top, int width, int height, int color, bool fill, int thickness) {
	TS_COUNTLOOP(i, thickness) {
		DxLib::DrawBox(this->Offset.X + left + i, this->Offset.Y + top + i, this->Offset.X + left + width - i, this->Offset.Y + top + height - i, color, fill);
	}
}


//####################  円描画  ####################
void CDXLibWrapper::DrawCircle(int x, int y, int r, int color, bool fill, int thickness) {
	DxLib::DrawCircle(this->Offset.X + x, this->Offset.Y + y, r, color, fill, thickness);
}


//####################  楕円描画  ####################
void CDXLibWrapper::DrawOval(int x, int y, int rx, int ry, int color, bool fill, int thickness) {
	DxLib::DrawOval(this->Offset.X + x, this->Offset.Y + y, rx, ry, color, fill, thickness);
}


//####################  描画領域を解除する  ####################
void CDXLibWrapper::ResetDrawArea() {
	DxLib::SetDrawArea(0, 0, Game.WindowSize.Width, Game.WindowSize.Height);
}


//####################  描画領域を設定する  ####################
void CDXLibWrapper::SetDrawArea(int left, int top, int right, int bottom) {
	DxLib::SetDrawArea(left, top, right, bottom);
}
void CDXLibWrapper::SetDrawArea(CRectangle& range) {
	this->SetDrawArea(range.Left, range.Top, range.Right(), range.Bottom());
}


//####################  指定したフォントで指定した文字列を描画したときの横幅を取得  ####################
CSize CDXLibWrapper::GetDrawStringSize(int fontID, const TCHAR* text) {
	CSize temp;
	DxLib::GetDrawStringSizeToHandle(
		&temp.Width, &temp.Height, nullptr, text, ::strlen(text),
		*Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font))->GetCell(fontID, TS::DB::ColumnNameMaterialHandle)->GetIntegerPTR()
	);
	return temp;
}


//####################  色コードを取得  ####################
int CDXLibWrapper::GetColor(int r, int g, int b) {
	return DxLib::GetColor(r, g, b);
}


//####################  合成方法の情報をDXライブラリ定数に変換する  ####################
int CDXLibWrapper::ConvertToDXBlend(int blendMode) {
	switch(static_cast<TS::System::BlendMode>(blendMode)) {
		case TS::System::BlendMode::Alpha:		return DX_BLENDMODE_ALPHA;
		case TS::System::BlendMode::Add:		return DX_BLENDMODE_ADD;
		case TS::System::BlendMode::Subtract:	return DX_BLENDMODE_SUB;
		case TS::System::BlendMode::Multiply:	return DX_BLENDMODE_MULA;
		case TS::System::BlendMode::Invsrc:		return DX_BLENDMODE_INVSRC;
	}
	return DX_BLENDMODE_NOBLEND;
}


//####################  グラフィックを描画  ####################
void CDXLibWrapper::DrawGraph(int Handle, int x, int y, bool trans, bool turn) {
	if(!turn) {
		DxLib::DrawGraph(this->Offset.X + x, this->Offset.Y + y, Handle, trans);
	} else {
		DxLib::DrawTurnGraph(this->Offset.X + x, this->Offset.Y + y, Handle, trans);
	}
}


//####################  グラフィックのサイズを変化させて描画  ####################
void CDXLibWrapper::DrawExtendGraph(int Handle, int x, int y, double xrate, double yrate, bool trans) {
	CSize size;
	DxLib::GetGraphSize(Handle, &size.Width, &size.Height);
	if(yrate == 0.0F) {		//倍率を同じにする
		yrate = xrate;
	}
	DxLib::DrawExtendGraph(this->Offset.X + x, this->Offset.Y + y, x + (size.Width * xrate), y + (size.Height * yrate), Handle, trans);
}


//####################  グラフィックを拡大縮小した上で回転させて描画  ####################
//注意：描画先は中心座標
//angleは回転角度で、＋側で右に回転する。PIで180度回転。回転させたくない場合は０にする
void CDXLibWrapper::DrawRotaGraph(int Handle, int cx, int cy, double angle, double exRate, bool trans, bool turn) {
	DxLib::DrawRotaGraph(this->Offset.X + cx, this->Offset.Y + cy, exRate, angle, Handle, trans, turn);
}


//####################  グラフィックの一部を描画  ####################
void CDXLibWrapper::DrawRectGraph(int Handle, int sx, int sy, int gx, int gy, int width, int height, bool trans, bool turn) {
	DxLib::DrawRectGraph(this->Offset.X + sx, this->Offset.Y + sy, gx, gy, width, height, Handle, trans, turn);
}


//####################  グラフィックの一部をサイズを変化させて描画  ####################
void CDXLibWrapper::DrawRectExtendGraph(int Handle, int left, int top, int width, int height, int gx, int gy, int rawWidth, int rawHeight, bool trans) {
	DxLib::DrawRectExtendGraph(this->Offset.X + left, this->Offset.Y + top, this->Offset.X + left + width, this->Offset.Y + top + height, gx, gy, rawWidth, rawHeight, Handle, trans);
}


//####################  グラフィックの一部を拡大縮小した上でサイズを回転させて描画  ####################
//注意：描画先は中心座標
//angleは回転角度で、＋側で右に回転する。PIで180度回転。回転させたくない場合は０にする
void CDXLibWrapper::DrawRectRotaGraph(int Handle, int cx, int cy, int gx, int gy, int rawWidth, int rawHeight, double angle, double exRate, bool trans, bool turn) {
	DxLib::DrawRectRotaGraph(this->Offset.X + cx, this->Offset.Y + cy, gx, gy, rawWidth, rawHeight, exRate, angle, Handle, trans, turn);
}


//####################  画像を自由変形描画  ####################
void CDXLibWrapper::DrawModifiedGraph(int Handle, int leftTopX, int leftTopY, int rightTopX, int rightTopY, int rightBottomX, int rightBottomY, int leftBottomX, int leftBottomY, bool trans) {
	DxLib::DrawModiGraph(leftTopX, leftTopY, rightTopX, rightTopY, rightBottomX, rightBottomY, leftBottomX, leftBottomY, Handle, trans);
}


//####################  画像とブレンド用画像を合成して描画  ####################
void CDXLibWrapper::DrawBlendGraph(int x, int y, int Handle, bool trans, string blendFileName, int borderParam) {
	int blendHandle = DxLib::LoadBlendGraph((TS::DirectoryNames::PrePath + blendFileName).c_str());
	DxLib::DrawBlendGraph(x, y, Handle, trans, blendHandle, TS_WITHIN(borderParam, 0, 255), TS::System::DXBlendRange);
	DxLib::DeleteGraph(blendHandle);
}


//####################  文字列を描画  ####################
void CDXLibWrapper::DrawString(int x, int y, int fontID, int color, const TCHAR *text) {
	auto db = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font));
	auto fontRow = db->GetRowFromID(fontID);
	auto cell = (fontRow == nullptr) ? nullptr : &fontRow->at(db->GetColumnIndex(TS::DB::ColumnNameMaterialHandle));
	auto font = (cell == nullptr) ? Game.Material.getFonts()->Data.Get(-1) : Game.Material.getFonts()->Data.Get(*cell->GetIntegerPTR());
	const int hendle = (cell == nullptr) ? Game.Material.getFonts()->DefaultFontHandle : font->ID;
	string str = text;
	vector<string> Texts;
	Game.SplitString(Texts, str, "\n");	//改行文字ごとに分割する

	TS_COUNTLOOP(Counter, Texts.size()) {
		DxLib::DrawStringToHandle(
			this->Offset.X + x,
			this->Offset.Y + y + font->LineSpace * Counter,
			Texts[Counter].c_str(),
			color,
			hendle,
			font->EdgeColor);
	}
}


//####################  文字列を中央揃えで描画  ####################
void CDXLibWrapper::DrawStringC(int centerX, int y, int fontID, int color, const TCHAR *text) {
	auto db = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font));
	auto fontRow = db->GetRowFromID(fontID);
	auto cell = (fontRow == nullptr) ? nullptr : &fontRow->at(db->GetColumnIndex(TS::DB::ColumnNameMaterialHandle));
	auto font = (cell == nullptr) ? Game.Material.getFonts()->Data.Get(-1) : Game.Material.getFonts()->Data.Get(*cell->GetIntegerPTR());
	const int hendle = (cell == nullptr) ? Game.Material.getFonts()->DefaultFontHandle : font->ID;
	string str = text;
	vector<string> Texts;
	Game.SplitString(Texts, str, "\n");		//改行文字ごとに分割する

	TS_COUNTLOOP(Counter, Texts.size()) {
		DxLib::DrawStringToHandle(
			this->Offset.X + centerX - DxLib::GetDrawStringWidthToHandle(Texts[Counter].c_str(), strlen(Texts[Counter].c_str()), hendle) / 2,
			this->Offset.Y + y + font->LineSpace * Counter,
			Texts[Counter].c_str(),
			color,
			hendle,
			font->EdgeColor);
	}
}


//####################  文字列を右揃えで描画  ####################
void CDXLibWrapper::DrawStringR(int right, int y, int fontID, int color, const TCHAR *text) {
	auto db = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font));
	auto fontRow = db->GetRowFromID(fontID);
	auto cell = (fontRow == nullptr) ? nullptr : &fontRow->at(db->GetColumnIndex(TS::DB::ColumnNameMaterialHandle));
	auto font = (cell == nullptr) ? Game.Material.getFonts()->Data.Get(-1) : Game.Material.getFonts()->Data.Get(*cell->GetIntegerPTR());
	const int hendle = (cell == nullptr) ? Game.Material.getFonts()->DefaultFontHandle : font->ID;
	string str = text;
	vector<string> Texts;
	Game.SplitString(Texts, str, "\n");		//改行文字ごとに分割する

	TS_COUNTLOOP(Counter, Texts.size()) {
		DxLib::DrawStringToHandle(
			this->Offset.X + right - DxLib::GetDrawStringWidthToHandle(Texts[Counter].c_str(), strlen(Texts[Counter].c_str()), hendle),
			this->Offset.Y + y + font->LineSpace * Counter,
			Texts[Counter].c_str(),
			color,
			hendle,
			font->EdgeColor);
	}
}


//####################  文字列を指定揃えで描画  ####################
void CDXLibWrapper::DrawStringAlign(int x, int y, int align, int fontID, int color, const TCHAR * text) {
	switch(align) {
		case static_cast<int>(TS::Sprite::TextAlign::Left):
			this->DrawString(x, y, fontID, color, text);
			return;

		case static_cast<int>(TS::Sprite::TextAlign::Right):
			this->DrawStringR(x, y, fontID, color, text);
			return;

		case static_cast<int>(TS::Sprite::TextAlign::Center):
			this->DrawStringC(x, y, fontID, color, text);
			return;

		default:
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "無効な文字列揃え位置番号 [%d] です。", align);
			return;
	}
}


//####################  指定したグラフィックのサイズを取得  ####################
CSize CDXLibWrapper::GetGraphSize(int Handle) {
	CSize temp;
	DxLib::GetGraphSize(Handle, &temp.Width, &temp.Height);
	return temp;
}


//####################  グラフィックの合成モードを変更  ####################
void CDXLibWrapper::SetDrawBlendMode(int mode, int param) {
	this->beforeBlendMode = this->currentBlendMode;
	this->beforeBlendParam = this->currentBlendParam;
	this->currentBlendMode = mode;
	this->currentBlendParam = param;
	DxLib::SetDrawBlendMode(CDXLibWrapper::ConvertToDXBlend(mode), Game.WithIn(param, 0, 255));
}


//####################  グラフィックの合成モードを復元する  ####################
//一つ前までしか戻れず、この関数を２度実行すると元に戻る
void CDXLibWrapper::RestoreDrawBlendMode() {
	this->SetDrawBlendMode(this->beforeBlendMode, this->beforeBlendParam);
}


//####################  描画輝度を変更  ####################
void CDXLibWrapper::SetDrawBright(int r, int g, int b) {
	DxLib::SetDrawBright(Game.WithIn(r, 0, 255), Game.WithIn(g, 0, 255), Game.WithIn(b, 0, 255));
}


//####################  動画を再生する  ####################
//第２引数は何倍にするかを整数で指定する（等倍＝１）
void CDXLibWrapper::PlayVideo(string fileName, int exRate, bool canCancel) {
	if(::PlayMovie(fileName.c_str(), exRate, (canCancel) ? DX_MOVIEPLAYTYPE_BCANCEL : DX_MOVIEPLAYTYPE_NORMAL) == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "動画の再生に失敗しました。必要なコーデックがインストールされていない可能性があります。\n>%s", fileName.c_str());
	}
}


//####################  動画像を再生  ####################
void CDXLibWrapper::PlayMovieToGraph(int Handle) {
	DxLib::PlayMovieToGraph(Handle);
}


//####################  動画像を停止  ####################
void CDXLibWrapper::PauseMovieToGraph(int Handle) {
	DxLib::PauseMovieToGraph(Handle);
}


//####################  動画像の再生位置を変更  ####################
void CDXLibWrapper::SeekMovieToGraph(int Handle, int timeMS) {
	DxLib::SeekMovieToGraph(Handle, timeMS);
}


//####################  動画像の再生位置を取得  ####################
int CDXLibWrapper::TellMovieToGraph(int Handle) {
	return DxLib::TellMovieToGraph(Handle);
}


//####################  動画像の再生状態を取得  ####################
int CDXLibWrapper::GetMovieStateToGraph(int Handle) {
	return DxLib::GetMovieStateToGraph(Handle);
}


//####################  接続されているジョイパッドの数を取得  ####################
int CDXLibWrapper::GetJoypadNum() {
	DxLib::ReSetupJoypad();			//最新の状態に更新
	return DxLib::GetJoypadNum();
}


//####################  指定したパッドの入力ボタン情報を取得  ####################
int CDXLibWrapper::GetJoypadInputState(int playerNum) {
	return DxLib::GetJoypadInputState(this->getPadInputType(playerNum));
}


//####################  指定したパッドのアナログスティックの値を取得  ####################
CPoint CDXLibWrapper::GetJoypadAnalogInput(int playerNum) {
	CSize temp;
	DxLib::GetJoypadAnalogInput(&temp.Width, &temp.Height, this->getPadInputType(playerNum));
	return temp;
}


//####################  指定したパッドの振動を開始する  ####################
void CDXLibWrapper::StartJoypadVibration(int playerNum, int power, int time) {
	DxLib::StartJoypadVibration(this->getPadInputType(playerNum), Game.WithIn(power, 0, 1000), time);
}


//####################  指定したパッドの振動を停止する  ####################
void CDXLibWrapper::StopJoypadVibration(int playerNum) {
	DxLib::StopJoypadVibration(this->getPadInputType(playerNum));
}


//####################  マウスカーソルの表示有無をセットする  ####################
void CDXLibWrapper::SetMouseDispFlag(bool flag) {
	DxLib::SetMouseDispFlag(flag);
}


//####################  マウスカーソルの位置を取得  ####################
CPoint CDXLibWrapper::GetMousePoint() {
	CPoint temp;
	DxLib::GetMousePoint(&temp.X, &temp.Y);
	return temp;
}


//####################  マウスカーソルの位置をセット  ####################
void CDXLibWrapper::SetMousePoint(int x, int y) {
	DxLib::SetMousePoint(x, y);
}


//####################  マウスボタンの入力情報を取得  ####################
int CDXLibWrapper::GetMouseInput() {
	return DxLib::GetMouseInput();
}


//####################  マウスホイールの量を取得  ####################
int CDXLibWrapper::GetMouseWheelRotVol() {
	return DxLib::GetMouseWheelRotVol();
}


//####################  任意の文字列を入力させる  ####################
string CDXLibWrapper::KeyInputString(int x, int y, int maxCharLength, bool allowCancel) {
	maxCharLength = Game.WithIn(maxCharLength, 0, sizeof(::buf) - 1);
	this->KeyInputResult = DxLib::KeyInputString(x, y, maxCharLength, ::buf, allowCancel);
	return ::buf;
}


//####################  任意の半角文字列を入力させる  ####################
string CDXLibWrapper::KeyInputSingleCharString(int x, int y, int maxCharLength, bool allowCancel) {
	maxCharLength = Game.WithIn(maxCharLength, 0, sizeof(::buf) - 1);
	this->KeyInputResult = DxLib::KeyInputSingleCharString(x, y, maxCharLength, ::buf, allowCancel);
	return ::buf;
}


//####################  任意の数値を入力させる  ####################
int CDXLibWrapper::KeyInputNumber(int x, int y, int min, int max, bool allowCancel) {
	return DxLib::KeyInputNumber(x, y, max, min, allowCancel);
}


//####################  ユーザー入力系メソッドの各種表示色を変更する  ####################
//数値を ; で区切って17個渡す
void CDXLibWrapper::SetKeyInputStringColor(const TCHAR* param) {
	int NmlStr, NmlCur, IMEStrBack, IMECur, IMELine;
	int IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE;
	int IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor;
	int IMEStr, IMEStrE;
	vector<string> spl;

	Game.SplitString(spl, param, ";");
	if(spl.size() != 17) {
		TS_LOG("ユーザー入力の表示色は 17個 必要です。与えられたパラメーター数は %d 個です。", spl.size());
		return;
	}

	//変数へ代入する
	NmlStr = atoi(Game.PopHead(spl).c_str());
	NmlCur = atoi(Game.PopHead(spl).c_str());
	IMEStrBack = atoi(Game.PopHead(spl).c_str());
	IMECur = atoi(Game.PopHead(spl).c_str());
	IMELine = atoi(Game.PopHead(spl).c_str());

	IMESelectStr = atoi(Game.PopHead(spl).c_str());
	IMEModeStr = atoi(Game.PopHead(spl).c_str());
	NmlStrE = atoi(Game.PopHead(spl).c_str());
	IMESelectStrE = atoi(Game.PopHead(spl).c_str());
	IMEModeStrE = atoi(Game.PopHead(spl).c_str());

	IMESelectWinE = atoi(Game.PopHead(spl).c_str());
	IMESelectWinF = atoi(Game.PopHead(spl).c_str());
	SelectStrBackColor = atoi(Game.PopHead(spl).c_str());
	SelectStrColor = atoi(Game.PopHead(spl).c_str());
	SelectStrEdgeColor = atoi(Game.PopHead(spl).c_str());

	IMEStr = atoi(Game.PopHead(spl).c_str());
	IMEStrE = atoi(Game.PopHead(spl).c_str());

	DxLib::SetKeyInputStringColor(NmlStr, NmlCur, IMEStrBack, IMECur, IMELine, IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE, IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor, IMEStr, IMEStrE);
}


//####################  コンピューターが起動してから経過したミリ秒数を取得  ####################
int CDXLibWrapper::GetNowCount() {
	return DxLib::GetNowCount();
}


//####################  現在の画面をスクリーンショットにして保存  ####################
void CDXLibWrapper::SaveDrawScreenToPNG(int leftTopX, int leftTopY, int rightBottomX, int rightBottomY, string fileName) {
	DxLib::SaveDrawScreenToPNG(leftTopX, leftTopY, rightBottomX, rightBottomY, fileName.c_str());
}


//####################  ファイルを開く  ####################
int CDXLibWrapper::FileRead_open(string fileName) {
	return DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
}


//####################  ファイルを閉じる  ####################
void CDXLibWrapper::FileRead_close(int handle) {
	DxLib::FileRead_close(handle);
}


//####################  ファイルから１行読み込む  ####################
string CDXLibWrapper::FileRead_gets(int handle) {
	DxLib::FileRead_gets(::buf, sizeof(::buf) - 1, handle);
	return ::buf;
}


//####################  ファイルから１文字読み込む  ####################
int CDXLibWrapper::FileRead_getc(int handle) {
	return DxLib::FileRead_getc(handle);
}


//####################  ファイルの末尾に到達しているかどうかを調べる  ####################
bool CDXLibWrapper::FileRead_eof(int handle) {
	return Game.IntToBool(::FileRead_eof(handle));
}


//####################  パッドのプレイヤー番号を取得する  ####################
int CDXLibWrapper::getPadInputType(int playerNum) {
	playerNum = Game.WithIn(playerNum, 0, 4);
	switch(playerNum) {
		case 0:
			return DX_INPUT_KEY;
			break;
		case 1:
			return DX_INPUT_KEY_PAD1;
			break;
		case 2:
			return DX_INPUT_PAD2;
			break;
		case 3:
			return DX_INPUT_PAD3;
			break;
		case 4:
			return DX_INPUT_PAD4;
			break;
	}
	return -1;
}


//####################  他のマシンに接続する  ####################
int CDXLibWrapper::ConnectNetWork(CDXLibWrapper::CIPv4Address ip, int portNum) {
	return DxLib::ConnectNetWork(ip.ToIPDATA(), portNum);
}


//####################  接続を終了する  ####################
int CDXLibWrapper::CloseNetWork(int handle) {
	return DxLib::CloseNetWork(handle);
}


//####################  接続を受けられる状態にする  ####################
int CDXLibWrapper::PreparationListenNetWork(int portNum) {
	return DxLib::PreparationListenNetWork(portNum);
}


//####################  接続を受け付けている状態を解除する  ####################
int CDXLibWrapper::StopListenNetWork() {
	return DxLib::StopListenNetWork();
}


//####################  データを送信する  ####################
int CDXLibWrapper::NetWorkSend(int handle, const TCHAR* data) {
	::sprintf_s(this->netWorkBuf, "%s", data);
	return DxLib::NetWorkSend(handle, this->netWorkBuf, ::strlen(this->netWorkBuf) + 1);
}


//####################  受信データ一時記憶バッファに溜まっているデータの量を得る  ####################
int CDXLibWrapper::GetNetWorkDataLength(int handle) {
	return DxLib::GetNetWorkDataLength(handle);
}


//####################  未送信のデータの量を得る  ####################
int CDXLibWrapper::GetNetWorkSendDataLength(int handle) {
	return DxLib::GetNetWorkSendDataLength(handle);
}


//####################  受信データ一時記憶バッファに溜まっているデータを取得する  ####################
//取得したデータは getRecvData() で取得できる
int CDXLibWrapper::NetWorkRecv(int handle, int length) {
	return DxLib::NetWorkRecv(handle, this->netWorkBuf, length);
}


//####################  受信したデータを読み込む、読み込んだデータはバッファから削除されない  ####################
int CDXLibWrapper::NetWorkRecvToPeek(int handle, int length) {
	return DxLib::NetWorkRecvToPeek(handle, this->netWorkBuf, length);
}


//####################  新たに確立した接続を示すネットワークハンドルを得る  ####################
int CDXLibWrapper::GetNewAcceptNetWork() {
	return DxLib::GetNewAcceptNetWork();
}


//####################  新たに破棄された接続を示すネットワークハンドルを得る  ####################
int CDXLibWrapper::GetLostNetWork() {
	return DxLib::GetLostNetWork();
}


//####################  接続状態を取得する  ####################
int CDXLibWrapper::GetNetWorkAcceptState(int handle) {
	return DxLib::GetNetWorkAcceptState(handle);
}


//####################  接続先のIPアドレスを得る  ####################
CDXLibWrapper::CIPv4Address CDXLibWrapper::GetNetWorkIP(int handle) {
	IPDATA temp;
	const int result = DxLib::GetNetWorkIP(handle, &temp);
	if(result == -1) {
		return CIPv4Address();
	} else {
		return CIPv4Address(&temp);
	}
}


//####################  ＵＤＰを使用して通信するためのソケットを作成する  ####################
int CDXLibWrapper::MakeUDPSocket(int recvPortNum) {
	return DxLib::MakeUDPSocket(recvPortNum);
}


//####################  ＵＤＰを使用して通信するためのソケットを削除する  ####################
int CDXLibWrapper::DeleteUDPSocket(int UDPHandle) {
	return DxLib::DeleteUDPSocket(UDPHandle);
}


//####################  ＵＤＰを使用して他のマシンにデータを送信する  ####################
int CDXLibWrapper::NetWorkSendUDP(int UDPHandle, CDXLibWrapper::CIPv4Address sendIP, int sendPortNum, const TCHAR* data) {
	::sprintf_s(this->netWorkBuf, "%s", data);
	return DxLib::NetWorkSendUDP(UDPHandle, sendIP.ToIPDATA(), sendPortNum, this->netWorkBuf, ::strlen(this->netWorkBuf) + 1);
}


//####################  ＵＤＰを使用して他のマシンからのデータを受信する  ####################
//送信元のアドレスは getUDPRecvIP() で取得できる
//受信ポート番号は getUDPRecvPortNum() で取得できる
int CDXLibWrapper::NetWorkRecvUDP(int UDPHandle, bool peekMode) {
	return DxLib::NetWorkRecvUDP(UDPHandle, &this->UDPRecvIP, &this->UDPRecvPortNum, this->netWorkBuf, sizeof(this->netWorkBuf) - 1, peekMode);
}


//####################  ＵＤＰを使用した他のマシンから受信データがあるかどうかを取得する  ####################
int CDXLibWrapper::CheckNetWorkRecvUDP(int UDPHandle) {
	return DxLib::CheckNetWorkRecvUDP(UDPHandle);
}


//####################  DXアーカイブを作成  ####################
//返値は成功したかどうか
//引数はいずれもフォルダーのフルパス指定
bool CDXLibWrapper::EncodeArchive(string srcFullPath, string destFullPath) {
	char src[MAX_PATH], dest[MAX_PATH];

	if(!PathFileExists(srcFullPath.c_str())) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "アーカイブの対象フォルダーが存在しません。\n>%s", srcFullPath.c_str());
		return false;
	}

	//ソースパスのフォルダー名部分を抜き出し、拡張子を付加して出力ファイル名とする
	string destFileName;
	const auto pos = srcFullPath.find_last_of("\\");
	if(pos != string::npos) {
		//階層がある場合、最後の階層を抜き出す
		destFileName = srcFullPath.substr(pos + 1);
	} else {
		//階層がない場合 (フルパス指定なのに階層がないのはおかしいが...)
		destFileName = srcFullPath;
	}
	destFileName += "." + TS::System::DXArchiveExt;

	//出力先のフルパスを更新する: アーカイブ元のフォルダー名をアーカイブファイル名として追加する
	if(destFullPath.substr(destFullPath.size() - 1, 1) != "\\") {
		//末尾に区切り記号を付加する
		destFullPath += "\\";
	}
	destFullPath += destFileName;

	//char型の配列に格納する
	::sprintf_s(src, "%s", srcFullPath.c_str());
	::sprintf_s(dest, "%s", destFullPath.c_str());

	//アーカイブ作成処理実行
	if(DXArchive::EncodeArchiveOneDirectory(dest, src, true, true, (unsigned char) 255, TS::System::DXArchivePass.c_str(), false, true) < 0) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "アーカイブの作成中にエラーが発生しました。\n>作成元: %s\n>出力先: %s", srcFullPath.c_str(), destFullPath.c_str());
		return false;
	}
	return true;
}


//####################  DXアーカイブを展開  ####################
//返値は成功したかどうか
//アーカイブ元はファイル名のフルパス、出力先はフォルダーのフルパス指定
bool CDXLibWrapper::DecodeArchive(string srcFullPath, string destFullPath) {
	char src[MAX_PATH], dest[MAX_PATH];

	if(!PathFileExists(srcFullPath.c_str())) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "対象のアーカイブファイルが存在しません。\n>%s", srcFullPath.c_str());
		return false;
	} else if(!PathFileExists(destFullPath.c_str())) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "出力先フォルダーが存在しません。\n>%s", destFullPath.c_str());
		return false;
	}

	//ソースパスのファイル名部分を抜き出し、拡張子を抜いて出力フォルダー名とする
	string destDirName;
	const auto pos = srcFullPath.find_last_of("\\");
	if(pos != string::npos) {
		//階層がある場合、最後の階層を抜き出す
		destDirName = srcFullPath.substr(pos + 1);
	} else {
		//階層がない場合 (フルパス指定なのに階層がないのはおかしいが...)
		destDirName = srcFullPath;
	}
	destDirName = Game.Replace(destDirName, "." + TS::System::DXArchiveExt, "");

	//出力先のフルパスを更新する: アーカイブ元のファイル名から拡張子を抜いたフォルダー名を追加する
	if(destFullPath.substr(destFullPath.size() - 1, 1) != "\\") {
		//末尾に区切り記号を付加する
		destFullPath += "\\";
	}
	destFullPath += destDirName;

	//char型の配列に格納する
	::sprintf_s(src, "%s", srcFullPath.c_str());
	::sprintf_s(dest, "%s", destFullPath.c_str());

	//アーカイブ展開処理実行
	if(DXArchive::DecodeArchive(src, dest, TS::System::DXArchivePass.c_str()) < 0) {
		if(DXArchive_VER5::DecodeArchive(src, dest, TS::System::DXArchivePass.c_str()) < 0) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "アーカイブの展開中にエラーが発生しました。\n>展開元: %s\n>出力先: %s", srcFullPath.c_str(), destFullPath.c_str());
			return false;
		}
	}
	return true;
}
