//###################################################################
//			�f�[�^�x�[�X�̒P��Z��
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �R���X�g���N�^�[  ####################
CDatabase::CDBCell::CDBCell(int cellType, string rawData) {
	this->cellType = cellType;
	this->columnType = "Unknown";
	this->SetData(rawData);
}
CDatabase::CDBCell::CDBCell(string columnType, string rawData) {
	this->setCellType(columnType);
	this->SetData(rawData);
}


//####################  �����l�ɖ߂�  ####################
void CDatabase::CDBCell::Init() {
	this->SetData(this->rawData);
}


//####################  ��������w�肵����ʂ̒l�Ƃ��ăZ�b�g  ####################
void CDatabase::CDBCell::SetData(string rawData, int cellType) {
	vector<string> split, subspl;
	this->rawData = rawData;

	//�Z����ʂ�ύX����ꍇ
	if(cellType != -1) {
		this->cellType = cellType;
	}

	//���݂̃Z����ʂɉ������l����
	switch((TS::DB::ColumnType)this->cellType) {
		case TS::DB::ColumnType::OneNumber:
			this->intValue = atoi(rawData.c_str());
			break;

		case TS::DB::ColumnType::Numbers:
			this->intValues.Clear();
			Game.SplitString(split, rawData, ";");
			for(auto& val : split) {
				this->intValues.Add(atoi(val.c_str()));
			}
			if(split.empty()) {
				//�Z���l����̎��͂O��������Z�b�g����
				this->intValues.Add(0);
			}
			break;

		case TS::DB::ColumnType::String:
			this->strValue = rawData;
			break;

		case TS::DB::ColumnType::Boolean:
			if(atoi(rawData.c_str()) == FALSE) {
				this->boolValue = false;
			} else {
				this->boolValue = true;
			}
			break;

		case TS::DB::ColumnType::IDNames:
			this->intValues.Clear();
			Game.SplitString(split, rawData, "|");
			for(auto& val : split) {
				this->intValues.Add(atoi(val.c_str()));
			}
			break;

		case TS::DB::ColumnType::IDValues:
			this->IDValues.Clear();
			Game.SplitString(split, rawData, "|");
			for(auto& idValue : split) {
				Game.SplitString(subspl, idValue, ";");
				if(subspl.size() == 2) {
					this->IDValues.Add(atoi(subspl[0].c_str()), atoi(subspl[1].c_str()));
				}
			}
			break;
	}
}


//####################  ���̃Z���̏��𕶎���ɂ��ĕԂ�  ####################
string CDatabase::CDBCell::ToString() {
	string strbuf;

	//�Z����ʂɉ����ĕ�����f�[�^�𐮌`
	switch((TS::DB::ColumnType)this->cellType) {
		case TS::DB::ColumnType::OneNumber:
			sprintf_s(::buf, "%d", this->intValue);
			strbuf = ::buf;
			break;

		case TS::DB::ColumnType::Numbers:
		case TS::DB::ColumnType::IDNames:
			TS_COUNTLOOP(i, this->intValues.Count()) {
				if(i > 0) {
					strbuf += ";";
				}
				sprintf_s(::buf, "%d", this->intValues.Get(i));
				strbuf += ::buf;
			}
			break;

		case TS::DB::ColumnType::String:
			strbuf = this->strValue;
			break;

		case TS::DB::ColumnType::Boolean:
			sprintf_s(::buf, "%d", this->boolValue ? TRUE : FALSE);
			strbuf = ::buf;
			break;

		case TS::DB::ColumnType::IDValues:
			TS_COUNTLOOP(i, this->IDValues.Count()) {
				if(i > 0) {
					strbuf += "|";
				}
				sprintf_s(::buf, "%d;%d",
						  this->IDValues.Get(i)->ID,
						  this->IDValues.Get(i)->Value
						  );
				strbuf += ::buf;
			}
			break;
	}
	return strbuf;
}


//####################  �Z����ʂ��擾  ####################
int CDatabase::CDBCell::GetCellType() {
	return this->cellType;
}


//####################  �Z����ʂɑΉ�����l��Ԃ��F�Y����ʈȊO�̓_�~�[�����ƂȂ�  ####################
int* CDatabase::CDBCell::GetIntegerPTR() {
	return &this->intValue;
}
bool* CDatabase::CDBCell::GetBooleanPTR() {
	return &this->boolValue;
}
string* CDatabase::CDBCell::GetStringPTR() {
	return &this->strValue;
}
CIntegerArray* CDatabase::CDBCell::GetIntegers() {
	return &this->intValues;
}
CDatabase::CDBCell::CDBIDValues* CDatabase::CDBCell::GetIDValues() {
	return &this->IDValues;
}
int CDatabase::CDBCell::ConvertToColorCode() {
	if(this->IsNull()) {
		return 0;
	}
	vector<string> vectorbuf;
	Game.SplitString(vectorbuf, this->rawData, ";");
	CRGBColor color;
	TS_COUNTLOOP(i, 3) {
		if(vectorbuf.size() > i) {
			color.Set(
				(vectorbuf.size() >= 1) ? atoi(vectorbuf[0].c_str()) : 0,
				(vectorbuf.size() >= 2) ? atoi(vectorbuf[1].c_str()) : 0,
				(vectorbuf.size() >= 3) ? atoi(vectorbuf[2].c_str()) : 0
				);
		}
	}
	return color.ToCode();
}
CPoint CDatabase::CDBCell::ConvertToPoint() {
	vector<string> spl;
	Game.SplitString(spl, *this->GetStringPTR(), ":");
	if(spl.size() != 2) {
		return CPoint();		//�s���Ȓl
	}
	return CPoint(atoi(spl[0].c_str()), atoi(spl[1].c_str()));
}
vector<CDatabase::CCurveData> CDatabase::CDBCell::ConvertToCurves() {
	//NOTE: �Ȑ��f�[�^��ł��邩�ǂ�����񖼈ȊO�Ŕ��肷���i���Ȃ����߁A�蓮�ŕϊ�����B
	vector<CCurveData> result;
	vector<string> vectorbuf;

	if(this->rawData.empty()) {
		//�f�[�^�����݂��Ȃ��ꍇ
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "�f�[�^�x�[�X�Ȑ���񂪖����ł��B");
		return result;
	}

	Game.SplitString(vectorbuf, this->rawData, "|");
	for(auto& cv : vectorbuf) {
		//�Ȑ����𕪂���
		CCurveData data;
		vector<string> spl;
		Game.SplitString(spl, cv, ";");
		if(spl.size() == static_cast<int>(TS::DB::DBCurveDataIndices::Count)) {
			data.ID = atoi(spl[static_cast<int>(TS::DB::DBCurveDataIndices::IDName)].c_str());
			if(data.ID != -1) {
				//NOTE: ���̎��_�ł�ID�̓p�����[�^�[DB��VisibleID�Ȃ̂ŁAFixedID�ɕϊ�����B
				data.ID = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Element), static_cast<int>(TS::DB::DBElementIndices::Parameter), data.ID);
			}
			data.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::IDName)] = data.ID;
			data.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Base)] = atoi(spl[static_cast<int>(TS::DB::DBCurveDataIndices::Base)].c_str());
			data.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Coefficient)] = atoi(spl[static_cast<int>(TS::DB::DBCurveDataIndices::Coefficient)].c_str());
			data.Curve[static_cast<int>(TS::DB::DBCurveDataIndices::Index)] = atoi(spl[static_cast<int>(TS::DB::DBCurveDataIndices::Index)].c_str());
			result.push_back(data);
		}
	}
	return result;
}
CDatabase::CDBCell::CDBAddons CDatabase::CDBCell::ConvertToAddons() {
	//NOTE: �A�h�I����ł��邩�ǂ�����񖼈ȊO�Ŕ��肷���i���Ȃ����߁A�蓮�ŕϊ�����B
	vector<string> split, subspl;
	CDBAddons addons;
	Game.SplitString(split, rawData, "|");
	for(auto& addon : split) {
		Game.SplitString(subspl, addon, ";");
		if(subspl.size() == TS::DB::AddonArgLength) {
			addons.Add(atoi(subspl[0].c_str()), atoi(subspl[1].c_str()), atoi(subspl[2].c_str()));
		}
	}
	return addons;
}


//####################  �Z����ʂ�ݒ肷��  ####################
void CDatabase::CDBCell::setCellType(string columnType) {
	//�񖼂���Z����ʂ����肷��
	this->columnType = columnType;
	if(columnType.find("FixedID") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::OneNumber);
	} else if(columnType.find("DBColumnList") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::OneNumber);
	} else if(columnType.find("OneNumber") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::OneNumber);
	} else if(columnType.find("Numbers") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::Numbers);
	} else if(columnType.find("IDNames") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::IDNames);
	} else if(columnType.find("IDValues") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::IDValues);
	} else if(columnType.find("DBColumnBool") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::Boolean);
	} else if(columnType.find("DBColumnPicture") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::String);
	} else if(columnType.find("DBColumnText") != string::npos) {
		this->cellType = static_cast<int>(TS::DB::ColumnType::String);
	} else {
		this->cellType = static_cast<int>(TS::DB::ColumnType::None);
	}
}
