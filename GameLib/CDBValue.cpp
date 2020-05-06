//###################################################################
//			フラグ・変数 データベース
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  読み込み処理  ####################
bool CDatabase::CDBValue::Load(string fileName) {
	//デフォルトの読み込み処理が先行する
	if(!CDBBase::Load(fileName)) {
		return false;		//読み込みに失敗した場合は、この先の処理を続行できない
	}

	//スクリプトに共通変数を公開する
	this->bindValues();
	return true;
}


//####################  セーブデータからのロード処理  ####################
void CDatabase::CDBValue::Load(vector<string>& vectorbuf) {
	//共通変数群の読み込み
	int id, intVal;
	string str;
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		while(true) {
			str = Game.PopHead(vectorbuf);
			if(str == TS::SaveLoad::ValueSplit) {
				break;		//区切り行が出たら切り替える
			}
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					::sscanf(str.c_str(), (TS::SQ::UserFlagName + "=%d\n").c_str(), &id, &intVal);
					*this->GetSubDB(type)->GetCell(id, this->GetSubDB(type)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).c_str())->GetBooleanPTR() = Game.IntToBool(intVal);
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					::sscanf(str.c_str(), (TS::SQ::UserIntegerName + "=%d\n").c_str(), &id, &intVal);
					*this->GetSubDB(type)->GetCell(id, this->GetSubDB(type)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).c_str())->GetIntegerPTR() = intVal;
					break;

				case TS::DB::DBValueIndices::CommonString:
					::sscanf(str.c_str(), (TS::SQ::UserStringName + "=").c_str(), &id);
					*this->GetSubDB(type)->GetCell(id, this->GetSubDB(type)->GetColumnName(static_cast<int>(TS::DB::DBDefaultColumnIndices::Count)).c_str())->GetStringPTR() = Game.PopHead(vectorbuf);
					break;
			}
		}
	}

	//スクリプトに値を反映させる
	this->bindValues();
}


//####################  セーブデータへのセーブ処理  ####################
void CDatabase::CDBValue::Save(string& strbuf) {
	//共通変数群の書き出し
	string valName;
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					valName = Game.Format(TS::SQ::UserFlagName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					strbuf += Game.Format(valName + "=%d\n", Game.BoolToInt(Game.SQ.GetSQBool(valName)));
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					valName = Game.Format(TS::SQ::UserIntegerName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					strbuf += Game.Format(valName + "=%d\n", Game.SQ.GetSQInteger(valName));
					break;

				case TS::DB::DBValueIndices::CommonString:
					valName = Game.Format(TS::SQ::UserStringName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					strbuf += Game.Format(valName + "=\n%s\n", Game.SQ.GetSQString(valName).c_str());
					break;
			}
		}
		strbuf += TS::SaveLoad::ValueSplit + "\n";		//種別ごとの区切り
	}
}


//####################  データベースの中身をログとして出力する  ####################
void CDatabase::CDBValue::PrintData() {
	TS_LOG("\nDB: " + this->fileName);
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		//複数のサブDBを持っている場合はインデックスを示す
		if(this->DBs.GetCount() > 1) {
			TS_LOG("\nSubDB[" + std::to_string(type) + "]:");
		}

		//最新値で出力する
		string valID, valName;
		TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
			valName = *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR();
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					valID = Game.Format(TS::SQ::UserFlagName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					TS_LOG(valID + " (%s) = %s", valName.c_str(), TS_BOOLSTR(Game.SQ.GetSQBool(valID)));
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					valID = Game.Format(TS::SQ::UserIntegerName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					TS_LOG(valID + " (%s) = %d", valName.c_str(), Game.SQ.GetSQInteger(valID));
					break;

				case TS::DB::DBValueIndices::CommonString:
					valID = Game.Format(TS::SQ::UserStringName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
					TS_LOG(valID + " (%s) = \"%s\"", valName.c_str(), Game.SQ.GetSQString(valID).c_str());
					break;
			}
		}
	}
}


//####################  指定した種別の変数リストを生成して返す  ####################
void CDatabase::CDBValue::GetSelfValueList(int type, CSelfValueData* dest) {
	dest->data.Clear();

	if(type <= static_cast<int>(TS::DB::DBValueIndices::CommonString) || static_cast<int>(TS::DB::DBValueIndices::Count) <= type) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::OutOfRange, "範囲外の個別変数種別 [%d] です。", type);
		return;
	}

	//全行実行
	TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
		dest->data.Add(CIDValueData(
			*this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR(),
			*this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR(),
			*this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetIntegerPTR()			//既定値
			));
	}
}


//####################  指定した種別の個別変数リストをスクリプト用に生成して返す  ####################
CSelfValueData CDatabase::CDBValue::GetSelfValues(int type) {
	CSelfValueData selfValues;
	this->GetSelfValueList(type, &selfValues);
	return selfValues;
}


//####################  スクリプトに共通変数を公開・値をセットする  ####################
void CDatabase::CDBValue::bindValues() {
	for(int type = 0; type <= static_cast<int>(TS::DB::DBValueIndices::CommonString); type++) {
		TS_COUNTLOOP(row, this->GetSubDB(type)->GetRowCount()) {
			switch((TS::DB::DBValueIndices)type) {
				case TS::DB::DBValueIndices::CommonFlag:
					Game.SQ.SetSQValueByVal(
						this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetBooleanPTR(),
						Game.Format(TS::SQ::UserFlagName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR())
						);
					break;

				case TS::DB::DBValueIndices::CommonInteger:
					Game.SQ.SetSQValueByVal(
						this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetIntegerPTR(),
						Game.Format(TS::SQ::UserIntegerName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR())
						);
					break;

				case TS::DB::DBValueIndices::CommonString:
					Game.SQ.SetSQValueByVal(
						this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Count))->GetStringPTR(),
						Game.Format(TS::SQ::UserStringName, *this->GetSubDB(type)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR())
						);
					break;
			}
		}
	}
}
