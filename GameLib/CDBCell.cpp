//###################################################################
//			データベースの単一セル
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  コンストラクター  ####################
CDatabase::CDBCell::CDBCell(int cellType, string rawData) {
	this->cellType = cellType;
	this->columnType = "Unknown";
	this->SetData(rawData);
}
CDatabase::CDBCell::CDBCell(string columnType, string rawData) {
	this->setCellType(columnType);
	this->SetData(rawData);
}


//####################  初期値に戻す  ####################
void CDatabase::CDBCell::Init() {
	this->SetData(this->rawData);
}


//####################  文字列を指定した種別の値としてセット  ####################
void CDatabase::CDBCell::SetData(string rawData, int cellType) {
	vector<string> split, subspl;
	this->rawData = rawData;

	//セル種別を変更する場合
	if(cellType != -1) {
		this->cellType = cellType;
	}

	//現在のセル種別に応じた値を代入
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
				//セル値が空の時は０を一つだけセットする
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


//####################  このセルの情報を文字列にして返す  ####################
string CDatabase::CDBCell::ToString() {
	string strbuf;

	//セル種別に応じて文字列データを整形
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


//####################  セル種別を取得  ####################
int CDatabase::CDBCell::GetCellType() {
	return this->cellType;
}


//####################  セル種別に対応する値を返す：該当種別以外はダミー扱いとなる  ####################
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
		return CPoint();		//不正な値
	}
	return CPoint(atoi(spl[0].c_str()), atoi(spl[1].c_str()));
}
vector<CDatabase::CCurveData> CDatabase::CDBCell::ConvertToCurves() {
	//NOTE: 曲線データ列であるかどうかを列名以外で判定する手段がないため、手動で変換する。
	vector<CCurveData> result;
	vector<string> vectorbuf;

	if(this->rawData.empty()) {
		//データが存在しない場合
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Invalid, "データベース曲線情報が無効です。");
		return result;
	}

	Game.SplitString(vectorbuf, this->rawData, "|");
	for(auto& cv : vectorbuf) {
		//曲線情報を分ける
		CCurveData data;
		vector<string> spl;
		Game.SplitString(spl, cv, ";");
		if(spl.size() == static_cast<int>(TS::DB::DBCurveDataIndices::Count)) {
			data.ID = atoi(spl[static_cast<int>(TS::DB::DBCurveDataIndices::IDName)].c_str());
			if(data.ID != -1) {
				//NOTE: この時点でのIDはパラメーターDBのVisibleIDなので、FixedIDに変換する。
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
	//NOTE: アドオン列であるかどうかを列名以外で判定する手段がないため、手動で変換する。
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


//####################  セル種別を設定する  ####################
void CDatabase::CDBCell::setCellType(string columnType) {
	//列名からセル種別を決定する
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
