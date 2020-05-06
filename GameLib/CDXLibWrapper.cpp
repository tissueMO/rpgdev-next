//###################################################################
//		DX���C�u�������b�p�[�N���X��`
//###################################################################
//������`��͕����s������ɂ��Ή�
#include "Header.h"
#include "DXArchive.h"
#include "DXArchiveVer5.h"
#include "FileLib.h"

bool ScreenFlash(TS_MT);
bool ScreenWindowShake(TS_MT);
extern char buf[TS::System::BufferLengthMax];


//####################  ��ʃt���b�V��  ####################
void CDXLibWrapper::Flash(int color, int strength, int speed, int count) {
	Game.MT.DeleteThread("��ʃt���b�V��");
	Game.MT.AddThread("��ʃt���b�V��", ::ScreenFlash, TS::System::Scene::All)
		.AddInteger("Color", color)
		.AddInteger("Strength", strength)
		.AddInteger("Speed", speed)
		.AddInteger("Count", count)
		.AddInteger("NowCount", 0)
		.AddInteger("NowStrength", 0);
}
//Data.Vint["Color"] = �F
//Data.Vint["Strength"] = ����
//Data.Vint["Speed"] = ����
//Data.Vint["Count"] = ��
//Data.Vint["NowStrength"] = ���݂̋����i�����j
//Data.Vint["NowCount"] = ���݂̉񐔁i�����j
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


//####################  ��ʐU���F�P��  ####################
void CDXLibWrapper::ShakeOne(int strength) {
	Game.MT.DeleteThread("��ʐU���F�P��");
	Game.MT.AddThread("��ʐU���F�P��", ::ScreenWindowShake, TS::System::Scene::All)
		.AddInteger("Strength", strength)
		.AddInteger("TimeCounter", 0)
		.AddInteger("PosX", 0)
		.AddInteger("PosY", 0)
		.AddInteger("KeepMode", FALSE);
}
//####################  ��ʐU���F�ێ� ���J�n  ####################
void CDXLibWrapper::StartShake(int strength) {
	Game.MT.DeleteThread("��ʐU���F�ێ�");
	Game.MT.AddThread("��ʐU���F�ێ�", ::ScreenWindowShake, TS::System::Scene::All)
		.AddInteger("Strength", strength)
		.AddInteger("TimeCounter", 0)
		.AddInteger("PosX", 0)
		.AddInteger("PosY", 0)
		.AddInteger("KeepMode", TRUE);
}
//####################  ��ʐU���F�ێ� ���~  ####################
void CDXLibWrapper::StopShake() {
	Game.MT.DeleteThread("��ʐU���F�ێ�");
}
//Data.Vint["Strength"] = �U���̋��x
//Data.Vint["TimeCounter"] = �U���o�ߎ���
//Data.Vint["PosX"] = �U��X
//Data.Vint["PosY"] = �U��Y
//Data.Vint["KeepMode"] = ���������Ɉێ����邩�ǂ���
bool ScreenWindowShake(TS_MT) {
	*Data.GetInteger("TimeCounter") += 1;
	*Data.GetInteger("PosX") = sin(static_cast<double>(PI / 180) * (*Data.GetInteger("TimeCounter") * 90/*�U���萔*/)) * *Data.GetInteger("Strength");
	*Data.GetInteger("PosY") = cos(static_cast<double>(PI / 180) * (*Data.GetInteger("TimeCounter") * 90/*�U���萔*/)) * *Data.GetInteger("Strength");
	if(*Data.GetInteger("TimeCounter") % 4 == 0) {
		*Data.GetInteger("Strength") -= 1;		//����������
	}

	//��ʊ�ʒu�̔��������s��
	Game.DXLib.Offset.Set(*Data.GetInteger("PosX"), *Data.GetInteger("PosY"));

	//���̏�Ԃɖ߂�����I��
	if(*Data.GetInteger("Strength") <= 1) {
		Game.DXLib.Offset.Set(0, 0);
		return true;
	}
	return false;
}


//####################  �w��̈�ɃE�B���h�E��`��  ####################
void CDXLibWrapper::DrawWindow(int hdl, int intX, int intY, int intWidth, int intHeight) {
	//�w�i�F: �g��k�� (�������l�� 2pix �͊ۂ��E�B���h�E�̂��߂ɏ��O�����)
	this->DrawRectExtendGraph(hdl, intX + 2, intY + 2, intWidth - 2 * 2, intHeight - 2 * 2, 2, 2, 64 - 2 * 2, 64 - 2 * 2, true);

	//�w�i�p�^�[��: �^�C�����O
	for(int x = intX + 2; x < intX + intWidth - 2; x += 64) {
		//�[�ɗ�����؂��蕝�𒲐�����
		int sizeX = 64;
		if(x + 64 > intX + intWidth - 2) {
			sizeX = intX + intWidth - 2 - x;
		}

		for(int y = intY + 2; y < intY + intHeight - 2; y += 64) {
			//�[�ɗ�����؂��荂���𒲐�����
			int sizeY = 64;
			if(y + 64 > intY + intHeight - 2) {
				sizeY = intY + intHeight - 2 - y;
			}

			//�^�C�����O�`��
			this->DrawRectGraph(hdl, x, y, 0, 64, sizeX, sizeY, true, false);
		}
	}

	//�g: �l���� ����A�E��A�����A�E�� �̏��A����ȊO�̕ӂ̓^�C�����O
	this->DrawRectGraph(hdl, intX, intY, 64 + 0, 0, 16, 16, true, false);
	this->DrawRectGraph(hdl, intX + intWidth - 16, intY, 64 + 48, 0, 16, 16, true, false);
	this->DrawRectGraph(hdl, intX, intY + intHeight - 16, 64 + 0, 48, 16, 16, true, false);
	this->DrawRectGraph(hdl, intX + intWidth - 16, intY + intHeight - 16, 64 + 48, 48, 16, 16, true, false);
	for(int x = intX + 16; x < intX + intWidth - 16; x += 16) {
		//�������^�C�����O: �[�ɗ�����؂��蕝�𒲐�����
		int sizeX = 16;
		if(x + 16 > intX + intWidth - 16) {
			sizeX = intX + intWidth - 16 - x;
		}
		this->DrawRectGraph(hdl, x, intY, 64 + 16, 0, sizeX, 16, true, false);
		this->DrawRectGraph(hdl, x, intY + intHeight - 16, 64 + 16, 64 - 16, sizeX, 16, true, false);
	}
	for(int y = intY + 16; y < intY + intHeight - 16; y += 16) {
		//�c�����^�C�����O: �[�ɗ�����؂��荂���𒲐�����
		int sizeY = 16;
		if(y + 16 > intY + intHeight - 16) {
			sizeY = intY + intHeight - 16 - y;
		}
		this->DrawRectGraph(hdl, intX, y, 64, 16, 16, sizeY, true, false);
		this->DrawRectGraph(hdl, intX + intWidth - 16, y, 64 + 64 - 16, 16, 16, sizeY, true, false);
	}
}


//####################  �w��̈�Ƀp�l���^�̃J�[�\����`��  ####################
void CDXLibWrapper::DrawCursorPanel(int hdl, int intX, int intY, int intWidth, int intHeight) {
	//�l���� ����A�E��A�����A�E�� �̏�
	this->DrawRectExtendGraph(hdl, intX, intY, 2, 2, 64, 64, 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX + intWidth - 2, intY, 2, 2, 64 + 32 - 2, 64, 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX, intY + intHeight - 2, 2, 2, 64, 64 + 32 - 2, 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX + intWidth - 2, intY + intHeight - 2, 2, 2, 64 + 32 - 2, 64 + 32 - 2, 2, 2, true);

	//�l�ӂ� ���A�E�A��A�� �̏�
	this->DrawRectExtendGraph(hdl, intX, intY + 2, 2, intHeight - 2 * 2, 64, 64 + 2, 2, 32 - 2 * 2, true);
	this->DrawRectExtendGraph(hdl, intX + intWidth - 2, intY + 2, 2, intHeight - 2 * 2, 64 + 32 - 2, 64 + 2, 2, 32 - 2 * 2, true);
	this->DrawRectExtendGraph(hdl, intX + 2, intY, intWidth - 2 * 2, 2, 64 + 2, 64, 32 - 2 * 2, 2, true);
	this->DrawRectExtendGraph(hdl, intX + 2, intY + intHeight - 2, intWidth - 2 * 2, 2, 64 + 2, 64 + 32 - 2, 32 - 2 * 2, 2, true);

	//�̈�����͊g��k��
	this->DrawRectExtendGraph(hdl, intX + 2, intY + 2, intWidth - 2 * 2, intHeight - 2 * 2, 64 + 2, 64 + 2, 32 - 2 * 2, 32 - 2 * 2, true);
}


//####################  �w����W�ɖ��^�̃J�[�\����`��  ####################
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


//####################  �_�`��  ####################
void CDXLibWrapper::DrawPixel(int x, int y, int color) {
	DxLib::DrawPixel(this->Offset.X + x, y, color);
}


//####################  ���`��  ####################
void CDXLibWrapper::DrawLine(int sx, int sy, int ex, int ey, int color, int thickness) {
	DxLib::DrawLine(this->Offset.X + sx, this->Offset.Y + sy, this->Offset.X + ex, this->Offset.Y + ey, color, thickness);
}


//####################  �O�p�`��  ####################
//�O�p�`�̒��_a, b, c�̍��W�ɏ]���ĕ`�悷��
void CDXLibWrapper::DrawTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color, bool fill) {
	DxLib::DrawTriangle(this->Offset.X + ax, this->Offset.Y + ay, this->Offset.X + bx, this->Offset.Y + by, this->Offset.X + cx, cy, color, fill);
}


//####################  ��`�`��i��Έʒu�w��j  ####################
void CDXLibWrapper::DrawBox(int left, int top, int right, int bottom, int color, bool fill, int thickness) {
	TS_COUNTLOOP(i, thickness) {
		DxLib::DrawBox(this->Offset.X + left + i, this->Offset.Y + top + i, this->Offset.X + right - i, this->Offset.Y + bottom - i, color, fill);
	}
}


//####################  ��`�`��i���Έʒu�w��j  ####################
void CDXLibWrapper::DrawRect(int left, int top, int width, int height, int color, bool fill, int thickness) {
	TS_COUNTLOOP(i, thickness) {
		DxLib::DrawBox(this->Offset.X + left + i, this->Offset.Y + top + i, this->Offset.X + left + width - i, this->Offset.Y + top + height - i, color, fill);
	}
}


//####################  �~�`��  ####################
void CDXLibWrapper::DrawCircle(int x, int y, int r, int color, bool fill, int thickness) {
	DxLib::DrawCircle(this->Offset.X + x, this->Offset.Y + y, r, color, fill, thickness);
}


//####################  �ȉ~�`��  ####################
void CDXLibWrapper::DrawOval(int x, int y, int rx, int ry, int color, bool fill, int thickness) {
	DxLib::DrawOval(this->Offset.X + x, this->Offset.Y + y, rx, ry, color, fill, thickness);
}


//####################  �`��̈����������  ####################
void CDXLibWrapper::ResetDrawArea() {
	DxLib::SetDrawArea(0, 0, Game.WindowSize.Width, Game.WindowSize.Height);
}


//####################  �`��̈��ݒ肷��  ####################
void CDXLibWrapper::SetDrawArea(int left, int top, int right, int bottom) {
	DxLib::SetDrawArea(left, top, right, bottom);
}
void CDXLibWrapper::SetDrawArea(CRectangle& range) {
	this->SetDrawArea(range.Left, range.Top, range.Right(), range.Bottom());
}


//####################  �w�肵���t�H���g�Ŏw�肵���������`�悵���Ƃ��̉������擾  ####################
CSize CDXLibWrapper::GetDrawStringSize(int fontID, const TCHAR* text) {
	CSize temp;
	DxLib::GetDrawStringSizeToHandle(
		&temp.Width, &temp.Height, nullptr, text, ::strlen(text),
		*Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font))->GetCell(fontID, TS::DB::ColumnNameMaterialHandle)->GetIntegerPTR()
	);
	return temp;
}


//####################  �F�R�[�h���擾  ####################
int CDXLibWrapper::GetColor(int r, int g, int b) {
	return DxLib::GetColor(r, g, b);
}


//####################  �������@�̏���DX���C�u�����萔�ɕϊ�����  ####################
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


//####################  �O���t�B�b�N��`��  ####################
void CDXLibWrapper::DrawGraph(int Handle, int x, int y, bool trans, bool turn) {
	if(!turn) {
		DxLib::DrawGraph(this->Offset.X + x, this->Offset.Y + y, Handle, trans);
	} else {
		DxLib::DrawTurnGraph(this->Offset.X + x, this->Offset.Y + y, Handle, trans);
	}
}


//####################  �O���t�B�b�N�̃T�C�Y��ω������ĕ`��  ####################
void CDXLibWrapper::DrawExtendGraph(int Handle, int x, int y, double xrate, double yrate, bool trans) {
	CSize size;
	DxLib::GetGraphSize(Handle, &size.Width, &size.Height);
	if(yrate == 0.0F) {		//�{���𓯂��ɂ���
		yrate = xrate;
	}
	DxLib::DrawExtendGraph(this->Offset.X + x, this->Offset.Y + y, x + (size.Width * xrate), y + (size.Height * yrate), Handle, trans);
}


//####################  �O���t�B�b�N���g��k��������ŉ�]�����ĕ`��  ####################
//���ӁF�`���͒��S���W
//angle�͉�]�p�x�ŁA�{���ŉE�ɉ�]����BPI��180�x��]�B��]���������Ȃ��ꍇ�͂O�ɂ���
void CDXLibWrapper::DrawRotaGraph(int Handle, int cx, int cy, double angle, double exRate, bool trans, bool turn) {
	DxLib::DrawRotaGraph(this->Offset.X + cx, this->Offset.Y + cy, exRate, angle, Handle, trans, turn);
}


//####################  �O���t�B�b�N�̈ꕔ��`��  ####################
void CDXLibWrapper::DrawRectGraph(int Handle, int sx, int sy, int gx, int gy, int width, int height, bool trans, bool turn) {
	DxLib::DrawRectGraph(this->Offset.X + sx, this->Offset.Y + sy, gx, gy, width, height, Handle, trans, turn);
}


//####################  �O���t�B�b�N�̈ꕔ���T�C�Y��ω������ĕ`��  ####################
void CDXLibWrapper::DrawRectExtendGraph(int Handle, int left, int top, int width, int height, int gx, int gy, int rawWidth, int rawHeight, bool trans) {
	DxLib::DrawRectExtendGraph(this->Offset.X + left, this->Offset.Y + top, this->Offset.X + left + width, this->Offset.Y + top + height, gx, gy, rawWidth, rawHeight, Handle, trans);
}


//####################  �O���t�B�b�N�̈ꕔ���g��k��������ŃT�C�Y����]�����ĕ`��  ####################
//���ӁF�`���͒��S���W
//angle�͉�]�p�x�ŁA�{���ŉE�ɉ�]����BPI��180�x��]�B��]���������Ȃ��ꍇ�͂O�ɂ���
void CDXLibWrapper::DrawRectRotaGraph(int Handle, int cx, int cy, int gx, int gy, int rawWidth, int rawHeight, double angle, double exRate, bool trans, bool turn) {
	DxLib::DrawRectRotaGraph(this->Offset.X + cx, this->Offset.Y + cy, gx, gy, rawWidth, rawHeight, exRate, angle, Handle, trans, turn);
}


//####################  �摜�����R�ό`�`��  ####################
void CDXLibWrapper::DrawModifiedGraph(int Handle, int leftTopX, int leftTopY, int rightTopX, int rightTopY, int rightBottomX, int rightBottomY, int leftBottomX, int leftBottomY, bool trans) {
	DxLib::DrawModiGraph(leftTopX, leftTopY, rightTopX, rightTopY, rightBottomX, rightBottomY, leftBottomX, leftBottomY, Handle, trans);
}


//####################  �摜�ƃu�����h�p�摜���������ĕ`��  ####################
void CDXLibWrapper::DrawBlendGraph(int x, int y, int Handle, bool trans, string blendFileName, int borderParam) {
	int blendHandle = DxLib::LoadBlendGraph((TS::DirectoryNames::PrePath + blendFileName).c_str());
	DxLib::DrawBlendGraph(x, y, Handle, trans, blendHandle, TS_WITHIN(borderParam, 0, 255), TS::System::DXBlendRange);
	DxLib::DeleteGraph(blendHandle);
}


//####################  �������`��  ####################
void CDXLibWrapper::DrawString(int x, int y, int fontID, int color, const TCHAR *text) {
	auto db = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font));
	auto fontRow = db->GetRowFromID(fontID);
	auto cell = (fontRow == nullptr) ? nullptr : &fontRow->at(db->GetColumnIndex(TS::DB::ColumnNameMaterialHandle));
	auto font = (cell == nullptr) ? Game.Material.getFonts()->Data.Get(-1) : Game.Material.getFonts()->Data.Get(*cell->GetIntegerPTR());
	const int hendle = (cell == nullptr) ? Game.Material.getFonts()->DefaultFontHandle : font->ID;
	string str = text;
	vector<string> Texts;
	Game.SplitString(Texts, str, "\n");	//���s�������Ƃɕ�������

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


//####################  ������𒆉������ŕ`��  ####################
void CDXLibWrapper::DrawStringC(int centerX, int y, int fontID, int color, const TCHAR *text) {
	auto db = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font));
	auto fontRow = db->GetRowFromID(fontID);
	auto cell = (fontRow == nullptr) ? nullptr : &fontRow->at(db->GetColumnIndex(TS::DB::ColumnNameMaterialHandle));
	auto font = (cell == nullptr) ? Game.Material.getFonts()->Data.Get(-1) : Game.Material.getFonts()->Data.Get(*cell->GetIntegerPTR());
	const int hendle = (cell == nullptr) ? Game.Material.getFonts()->DefaultFontHandle : font->ID;
	string str = text;
	vector<string> Texts;
	Game.SplitString(Texts, str, "\n");		//���s�������Ƃɕ�������

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


//####################  ��������E�����ŕ`��  ####################
void CDXLibWrapper::DrawStringR(int right, int y, int fontID, int color, const TCHAR *text) {
	auto db = Game.DB.getFDBMaterial()->GetSubDB(static_cast<int>(TS::DB::DBMaterialIndices::Font));
	auto fontRow = db->GetRowFromID(fontID);
	auto cell = (fontRow == nullptr) ? nullptr : &fontRow->at(db->GetColumnIndex(TS::DB::ColumnNameMaterialHandle));
	auto font = (cell == nullptr) ? Game.Material.getFonts()->Data.Get(-1) : Game.Material.getFonts()->Data.Get(*cell->GetIntegerPTR());
	const int hendle = (cell == nullptr) ? Game.Material.getFonts()->DefaultFontHandle : font->ID;
	string str = text;
	vector<string> Texts;
	Game.SplitString(Texts, str, "\n");		//���s�������Ƃɕ�������

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


//####################  ��������w�葵���ŕ`��  ####################
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
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�����ȕ����񑵂��ʒu�ԍ� [%d] �ł��B", align);
			return;
	}
}


//####################  �w�肵���O���t�B�b�N�̃T�C�Y���擾  ####################
CSize CDXLibWrapper::GetGraphSize(int Handle) {
	CSize temp;
	DxLib::GetGraphSize(Handle, &temp.Width, &temp.Height);
	return temp;
}


//####################  �O���t�B�b�N�̍������[�h��ύX  ####################
void CDXLibWrapper::SetDrawBlendMode(int mode, int param) {
	this->beforeBlendMode = this->currentBlendMode;
	this->beforeBlendParam = this->currentBlendParam;
	this->currentBlendMode = mode;
	this->currentBlendParam = param;
	DxLib::SetDrawBlendMode(CDXLibWrapper::ConvertToDXBlend(mode), Game.WithIn(param, 0, 255));
}


//####################  �O���t�B�b�N�̍������[�h�𕜌�����  ####################
//��O�܂ł����߂ꂸ�A���̊֐����Q�x���s����ƌ��ɖ߂�
void CDXLibWrapper::RestoreDrawBlendMode() {
	this->SetDrawBlendMode(this->beforeBlendMode, this->beforeBlendParam);
}


//####################  �`��P�x��ύX  ####################
void CDXLibWrapper::SetDrawBright(int r, int g, int b) {
	DxLib::SetDrawBright(Game.WithIn(r, 0, 255), Game.WithIn(g, 0, 255), Game.WithIn(b, 0, 255));
}


//####################  ������Đ�����  ####################
//��Q�����͉��{�ɂ��邩�𐮐��Ŏw�肷��i���{���P�j
void CDXLibWrapper::PlayVideo(string fileName, int exRate, bool canCancel) {
	if(::PlayMovie(fileName.c_str(), exRate, (canCancel) ? DX_MOVIEPLAYTYPE_BCANCEL : DX_MOVIEPLAYTYPE_NORMAL) == -1) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "����̍Đ��Ɏ��s���܂����B�K�v�ȃR�[�f�b�N���C���X�g�[������Ă��Ȃ��\��������܂��B\n>%s", fileName.c_str());
	}
}


//####################  ���摜���Đ�  ####################
void CDXLibWrapper::PlayMovieToGraph(int Handle) {
	DxLib::PlayMovieToGraph(Handle);
}


//####################  ���摜���~  ####################
void CDXLibWrapper::PauseMovieToGraph(int Handle) {
	DxLib::PauseMovieToGraph(Handle);
}


//####################  ���摜�̍Đ��ʒu��ύX  ####################
void CDXLibWrapper::SeekMovieToGraph(int Handle, int timeMS) {
	DxLib::SeekMovieToGraph(Handle, timeMS);
}


//####################  ���摜�̍Đ��ʒu���擾  ####################
int CDXLibWrapper::TellMovieToGraph(int Handle) {
	return DxLib::TellMovieToGraph(Handle);
}


//####################  ���摜�̍Đ���Ԃ��擾  ####################
int CDXLibWrapper::GetMovieStateToGraph(int Handle) {
	return DxLib::GetMovieStateToGraph(Handle);
}


//####################  �ڑ�����Ă���W���C�p�b�h�̐����擾  ####################
int CDXLibWrapper::GetJoypadNum() {
	DxLib::ReSetupJoypad();			//�ŐV�̏�ԂɍX�V
	return DxLib::GetJoypadNum();
}


//####################  �w�肵���p�b�h�̓��̓{�^�������擾  ####################
int CDXLibWrapper::GetJoypadInputState(int playerNum) {
	return DxLib::GetJoypadInputState(this->getPadInputType(playerNum));
}


//####################  �w�肵���p�b�h�̃A�i���O�X�e�B�b�N�̒l���擾  ####################
CPoint CDXLibWrapper::GetJoypadAnalogInput(int playerNum) {
	CSize temp;
	DxLib::GetJoypadAnalogInput(&temp.Width, &temp.Height, this->getPadInputType(playerNum));
	return temp;
}


//####################  �w�肵���p�b�h�̐U�����J�n����  ####################
void CDXLibWrapper::StartJoypadVibration(int playerNum, int power, int time) {
	DxLib::StartJoypadVibration(this->getPadInputType(playerNum), Game.WithIn(power, 0, 1000), time);
}


//####################  �w�肵���p�b�h�̐U�����~����  ####################
void CDXLibWrapper::StopJoypadVibration(int playerNum) {
	DxLib::StopJoypadVibration(this->getPadInputType(playerNum));
}


//####################  �}�E�X�J�[�\���̕\���L�����Z�b�g����  ####################
void CDXLibWrapper::SetMouseDispFlag(bool flag) {
	DxLib::SetMouseDispFlag(flag);
}


//####################  �}�E�X�J�[�\���̈ʒu���擾  ####################
CPoint CDXLibWrapper::GetMousePoint() {
	CPoint temp;
	DxLib::GetMousePoint(&temp.X, &temp.Y);
	return temp;
}


//####################  �}�E�X�J�[�\���̈ʒu���Z�b�g  ####################
void CDXLibWrapper::SetMousePoint(int x, int y) {
	DxLib::SetMousePoint(x, y);
}


//####################  �}�E�X�{�^���̓��͏����擾  ####################
int CDXLibWrapper::GetMouseInput() {
	return DxLib::GetMouseInput();
}


//####################  �}�E�X�z�C�[���̗ʂ��擾  ####################
int CDXLibWrapper::GetMouseWheelRotVol() {
	return DxLib::GetMouseWheelRotVol();
}


//####################  �C�ӂ̕��������͂�����  ####################
string CDXLibWrapper::KeyInputString(int x, int y, int maxCharLength, bool allowCancel) {
	maxCharLength = Game.WithIn(maxCharLength, 0, sizeof(::buf) - 1);
	this->KeyInputResult = DxLib::KeyInputString(x, y, maxCharLength, ::buf, allowCancel);
	return ::buf;
}


//####################  �C�ӂ̔��p���������͂�����  ####################
string CDXLibWrapper::KeyInputSingleCharString(int x, int y, int maxCharLength, bool allowCancel) {
	maxCharLength = Game.WithIn(maxCharLength, 0, sizeof(::buf) - 1);
	this->KeyInputResult = DxLib::KeyInputSingleCharString(x, y, maxCharLength, ::buf, allowCancel);
	return ::buf;
}


//####################  �C�ӂ̐��l����͂�����  ####################
int CDXLibWrapper::KeyInputNumber(int x, int y, int min, int max, bool allowCancel) {
	return DxLib::KeyInputNumber(x, y, max, min, allowCancel);
}


//####################  ���[�U�[���͌n���\�b�h�̊e��\���F��ύX����  ####################
//���l�� ; �ŋ�؂���17�n��
void CDXLibWrapper::SetKeyInputStringColor(const TCHAR* param) {
	int NmlStr, NmlCur, IMEStrBack, IMECur, IMELine;
	int IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE;
	int IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor;
	int IMEStr, IMEStrE;
	vector<string> spl;

	Game.SplitString(spl, param, ";");
	if(spl.size() != 17) {
		TS_LOG("���[�U�[���͂̕\���F�� 17�� �K�v�ł��B�^����ꂽ�p�����[�^�[���� %d �ł��B", spl.size());
		return;
	}

	//�ϐ��֑������
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


//####################  �R���s���[�^�[���N�����Ă���o�߂����~���b�����擾  ####################
int CDXLibWrapper::GetNowCount() {
	return DxLib::GetNowCount();
}


//####################  ���݂̉�ʂ��X�N���[���V���b�g�ɂ��ĕۑ�  ####################
void CDXLibWrapper::SaveDrawScreenToPNG(int leftTopX, int leftTopY, int rightBottomX, int rightBottomY, string fileName) {
	DxLib::SaveDrawScreenToPNG(leftTopX, leftTopY, rightBottomX, rightBottomY, fileName.c_str());
}


//####################  �t�@�C�����J��  ####################
int CDXLibWrapper::FileRead_open(string fileName) {
	return DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
}


//####################  �t�@�C�������  ####################
void CDXLibWrapper::FileRead_close(int handle) {
	DxLib::FileRead_close(handle);
}


//####################  �t�@�C������P�s�ǂݍ���  ####################
string CDXLibWrapper::FileRead_gets(int handle) {
	DxLib::FileRead_gets(::buf, sizeof(::buf) - 1, handle);
	return ::buf;
}


//####################  �t�@�C������P�����ǂݍ���  ####################
int CDXLibWrapper::FileRead_getc(int handle) {
	return DxLib::FileRead_getc(handle);
}


//####################  �t�@�C���̖����ɓ��B���Ă��邩�ǂ����𒲂ׂ�  ####################
bool CDXLibWrapper::FileRead_eof(int handle) {
	return Game.IntToBool(::FileRead_eof(handle));
}


//####################  �p�b�h�̃v���C���[�ԍ����擾����  ####################
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


//####################  ���̃}�V���ɐڑ�����  ####################
int CDXLibWrapper::ConnectNetWork(CDXLibWrapper::CIPv4Address ip, int portNum) {
	return DxLib::ConnectNetWork(ip.ToIPDATA(), portNum);
}


//####################  �ڑ����I������  ####################
int CDXLibWrapper::CloseNetWork(int handle) {
	return DxLib::CloseNetWork(handle);
}


//####################  �ڑ����󂯂����Ԃɂ���  ####################
int CDXLibWrapper::PreparationListenNetWork(int portNum) {
	return DxLib::PreparationListenNetWork(portNum);
}


//####################  �ڑ����󂯕t���Ă����Ԃ���������  ####################
int CDXLibWrapper::StopListenNetWork() {
	return DxLib::StopListenNetWork();
}


//####################  �f�[�^�𑗐M����  ####################
int CDXLibWrapper::NetWorkSend(int handle, const TCHAR* data) {
	::sprintf_s(this->netWorkBuf, "%s", data);
	return DxLib::NetWorkSend(handle, this->netWorkBuf, ::strlen(this->netWorkBuf) + 1);
}


//####################  ��M�f�[�^�ꎞ�L���o�b�t�@�ɗ��܂��Ă���f�[�^�̗ʂ𓾂�  ####################
int CDXLibWrapper::GetNetWorkDataLength(int handle) {
	return DxLib::GetNetWorkDataLength(handle);
}


//####################  �����M�̃f�[�^�̗ʂ𓾂�  ####################
int CDXLibWrapper::GetNetWorkSendDataLength(int handle) {
	return DxLib::GetNetWorkSendDataLength(handle);
}


//####################  ��M�f�[�^�ꎞ�L���o�b�t�@�ɗ��܂��Ă���f�[�^���擾����  ####################
//�擾�����f�[�^�� getRecvData() �Ŏ擾�ł���
int CDXLibWrapper::NetWorkRecv(int handle, int length) {
	return DxLib::NetWorkRecv(handle, this->netWorkBuf, length);
}


//####################  ��M�����f�[�^��ǂݍ��ށA�ǂݍ��񂾃f�[�^�̓o�b�t�@����폜����Ȃ�  ####################
int CDXLibWrapper::NetWorkRecvToPeek(int handle, int length) {
	return DxLib::NetWorkRecvToPeek(handle, this->netWorkBuf, length);
}


//####################  �V���Ɋm�������ڑ��������l�b�g���[�N�n���h���𓾂�  ####################
int CDXLibWrapper::GetNewAcceptNetWork() {
	return DxLib::GetNewAcceptNetWork();
}


//####################  �V���ɔj�����ꂽ�ڑ��������l�b�g���[�N�n���h���𓾂�  ####################
int CDXLibWrapper::GetLostNetWork() {
	return DxLib::GetLostNetWork();
}


//####################  �ڑ���Ԃ��擾����  ####################
int CDXLibWrapper::GetNetWorkAcceptState(int handle) {
	return DxLib::GetNetWorkAcceptState(handle);
}


//####################  �ڑ����IP�A�h���X�𓾂�  ####################
CDXLibWrapper::CIPv4Address CDXLibWrapper::GetNetWorkIP(int handle) {
	IPDATA temp;
	const int result = DxLib::GetNetWorkIP(handle, &temp);
	if(result == -1) {
		return CIPv4Address();
	} else {
		return CIPv4Address(&temp);
	}
}


//####################  �t�c�o���g�p���ĒʐM���邽�߂̃\�P�b�g���쐬����  ####################
int CDXLibWrapper::MakeUDPSocket(int recvPortNum) {
	return DxLib::MakeUDPSocket(recvPortNum);
}


//####################  �t�c�o���g�p���ĒʐM���邽�߂̃\�P�b�g���폜����  ####################
int CDXLibWrapper::DeleteUDPSocket(int UDPHandle) {
	return DxLib::DeleteUDPSocket(UDPHandle);
}


//####################  �t�c�o���g�p���đ��̃}�V���Ƀf�[�^�𑗐M����  ####################
int CDXLibWrapper::NetWorkSendUDP(int UDPHandle, CDXLibWrapper::CIPv4Address sendIP, int sendPortNum, const TCHAR* data) {
	::sprintf_s(this->netWorkBuf, "%s", data);
	return DxLib::NetWorkSendUDP(UDPHandle, sendIP.ToIPDATA(), sendPortNum, this->netWorkBuf, ::strlen(this->netWorkBuf) + 1);
}


//####################  �t�c�o���g�p���đ��̃}�V������̃f�[�^����M����  ####################
//���M���̃A�h���X�� getUDPRecvIP() �Ŏ擾�ł���
//��M�|�[�g�ԍ��� getUDPRecvPortNum() �Ŏ擾�ł���
int CDXLibWrapper::NetWorkRecvUDP(int UDPHandle, bool peekMode) {
	return DxLib::NetWorkRecvUDP(UDPHandle, &this->UDPRecvIP, &this->UDPRecvPortNum, this->netWorkBuf, sizeof(this->netWorkBuf) - 1, peekMode);
}


//####################  �t�c�o���g�p�������̃}�V�������M�f�[�^�����邩�ǂ������擾����  ####################
int CDXLibWrapper::CheckNetWorkRecvUDP(int UDPHandle) {
	return DxLib::CheckNetWorkRecvUDP(UDPHandle);
}


//####################  DX�A�[�J�C�u���쐬  ####################
//�Ԓl�͐����������ǂ���
//�����͂�������t�H���_�[�̃t���p�X�w��
bool CDXLibWrapper::EncodeArchive(string srcFullPath, string destFullPath) {
	char src[MAX_PATH], dest[MAX_PATH];

	if(!PathFileExists(srcFullPath.c_str())) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�A�[�J�C�u�̑Ώۃt�H���_�[�����݂��܂���B\n>%s", srcFullPath.c_str());
		return false;
	}

	//�\�[�X�p�X�̃t�H���_�[�������𔲂��o���A�g���q��t�����ďo�̓t�@�C�����Ƃ���
	string destFileName;
	const auto pos = srcFullPath.find_last_of("\\");
	if(pos != string::npos) {
		//�K�w������ꍇ�A�Ō�̊K�w�𔲂��o��
		destFileName = srcFullPath.substr(pos + 1);
	} else {
		//�K�w���Ȃ��ꍇ (�t���p�X�w��Ȃ̂ɊK�w���Ȃ��̂͂���������...)
		destFileName = srcFullPath;
	}
	destFileName += "." + TS::System::DXArchiveExt;

	//�o�͐�̃t���p�X���X�V����: �A�[�J�C�u���̃t�H���_�[�����A�[�J�C�u�t�@�C�����Ƃ��Ēǉ�����
	if(destFullPath.substr(destFullPath.size() - 1, 1) != "\\") {
		//�����ɋ�؂�L����t������
		destFullPath += "\\";
	}
	destFullPath += destFileName;

	//char�^�̔z��Ɋi�[����
	::sprintf_s(src, "%s", srcFullPath.c_str());
	::sprintf_s(dest, "%s", destFullPath.c_str());

	//�A�[�J�C�u�쐬�������s
	if(DXArchive::EncodeArchiveOneDirectory(dest, src, true, true, (unsigned char) 255, TS::System::DXArchivePass.c_str(), false, true) < 0) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�A�[�J�C�u�̍쐬���ɃG���[���������܂����B\n>�쐬��: %s\n>�o�͐�: %s", srcFullPath.c_str(), destFullPath.c_str());
		return false;
	}
	return true;
}


//####################  DX�A�[�J�C�u��W�J  ####################
//�Ԓl�͐����������ǂ���
//�A�[�J�C�u���̓t�@�C�����̃t���p�X�A�o�͐�̓t�H���_�[�̃t���p�X�w��
bool CDXLibWrapper::DecodeArchive(string srcFullPath, string destFullPath) {
	char src[MAX_PATH], dest[MAX_PATH];

	if(!PathFileExists(srcFullPath.c_str())) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�Ώۂ̃A�[�J�C�u�t�@�C�������݂��܂���B\n>%s", srcFullPath.c_str());
		return false;
	} else if(!PathFileExists(destFullPath.c_str())) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�o�͐�t�H���_�[�����݂��܂���B\n>%s", destFullPath.c_str());
		return false;
	}

	//�\�[�X�p�X�̃t�@�C���������𔲂��o���A�g���q�𔲂��ďo�̓t�H���_�[���Ƃ���
	string destDirName;
	const auto pos = srcFullPath.find_last_of("\\");
	if(pos != string::npos) {
		//�K�w������ꍇ�A�Ō�̊K�w�𔲂��o��
		destDirName = srcFullPath.substr(pos + 1);
	} else {
		//�K�w���Ȃ��ꍇ (�t���p�X�w��Ȃ̂ɊK�w���Ȃ��̂͂���������...)
		destDirName = srcFullPath;
	}
	destDirName = Game.Replace(destDirName, "." + TS::System::DXArchiveExt, "");

	//�o�͐�̃t���p�X���X�V����: �A�[�J�C�u���̃t�@�C��������g���q�𔲂����t�H���_�[����ǉ�����
	if(destFullPath.substr(destFullPath.size() - 1, 1) != "\\") {
		//�����ɋ�؂�L����t������
		destFullPath += "\\";
	}
	destFullPath += destDirName;

	//char�^�̔z��Ɋi�[����
	::sprintf_s(src, "%s", srcFullPath.c_str());
	::sprintf_s(dest, "%s", destFullPath.c_str());

	//�A�[�J�C�u�W�J�������s
	if(DXArchive::DecodeArchive(src, dest, TS::System::DXArchivePass.c_str()) < 0) {
		if(DXArchive_VER5::DecodeArchive(src, dest, TS::System::DXArchivePass.c_str()) < 0) {
			Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�A�[�J�C�u�̓W�J���ɃG���[���������܂����B\n>�W�J��: %s\n>�o�͐�: %s", srcFullPath.c_str(), destFullPath.c_str());
			return false;
		}
	}
	return true;
}
