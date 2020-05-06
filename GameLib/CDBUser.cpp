//###################################################################
//			���[�U�[��`�̃f�[�^�x�[�X
//###################################################################
#include "Header.h"
extern char buf[TS::System::BufferLengthMax];


//####################  �ǂݍ��ݏ���  ####################
bool CDatabase::CDBUser::Load(string fileName) {
	//�f�t�H���g�̓ǂݍ��ݏ�������s����
	CDBBase::Load(fileName);

	//���[�U�[DB�̒��g�����ׂă��[�h����
	this->UserDBs.Clear();
	TS_COUNTLOOP(row, this->GetSubDB(0)->GetRowCount()) {
		if(!this->openFile(Game.Format(TS::FileNames::UserDBData, *this->GetSubDB(0)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR()))) {
			continue;		//�ǂݍ��݂Ɏ��s�����ꍇ�̓X�L�b�v
		}

		//�f�[�^�x�[�X�����Z�b�g
		auto db = CDatabase::CDBBaseOne();
		db.ID = *this->GetSubDB(0)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR();
		db.Name = *this->GetSubDB(0)->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR();
		this->UserDBs.Add(db);
		auto addedDB = this->UserDBs.GetFromIndex(this->UserDBs.GetCount() - 1);

		//�P�s�ځF��
		this->readNextRow();
		int colCount = atoi(this->stringBuffer.c_str());

		//�Q�s�ځ`�F�P�s�ɂP��̗��񂪊i�[����Ă��邪�A�G�f�B�^�[�p�Ȃ̂œǂݎ̂Ă�
		vector<string> names;
		TS_COUNTLOOP(i, colCount) {
			this->readNextRow();
		}

		//��^�����^�u��؂�
		this->readNextRow();
		addedDB->SetColumnTypes(&this->splitBuffer);

		//��\�������^�u��؂�
		this->readNextRow();
		addedDB->SetColumnNames(&this->splitBuffer);

		//�ȍ~�A�s���
		while(this->readNextRow() >= 1) {
			addedDB->LoadRow(&this->splitBuffer);
		}

		this->closeFile();
	}
	return true;
}


//####################  �ǂݍ��ݏ���  ####################
//���[�U�[DB�̒��g�A�s�f�[�^�݂̂�ǂݏo��
void CDatabase::CDBUser::Load(vector<string>& vectorbuf) {
	int id, count;
	string temp;
	vector<string> split;

	//�擪�̃f�[�^�x�[�X�����m�F����
	count = atoi(Game.PopHead(vectorbuf).c_str());
	TS_COUNTLOOP(i, count) {
		id = atoi(Game.PopHead(vectorbuf).c_str());
		this->UserDBs.GetFromID(id)->ClearRow();
		while(true) {
			temp = Game.PopHead(vectorbuf);
			if(temp.empty()) {
				break;		//��s�܂ł���̃f�[�^�x�[�X�ƔF������
			}
			Game.SplitString(split, temp, "\t");
			this->UserDBs.GetFromID(id)->LoadRow(&split);
		}
	}
}


//####################  �������ݏ���  ####################
//���[�U�[DB�̒��g�A�s�f�[�^�݂̂������o��
void CDatabase::CDBUser::Save(string& strbuf) {
	//�擪�Ƀf�[�^�x�[�X��������
	strbuf += Game.Format("%d\n", this->UserDBs.GetCount());
	TS_COUNTLOOP(i, this->UserDBs.GetCount()) {
		//�eDB�̐擪�Ƀf�[�^�x�[�XID������
		strbuf += Game.Format("%d\n", *this->GetSubDB(0)->GetCellFromIndex(i, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR());
		TS_COUNTLOOP(row, this->UserDBs.GetFromIndex(i)->GetRowCount()) {
			this->stringBuffer = "";
			TS_COUNTLOOP(col, this->UserDBs.GetFromIndex(i)->GetColumnCount()) {
				this->stringBuffer += this->UserDBs.GetFromIndex(i)->GetCellFromIndex(row, col)->ToString();
				if(col < this->UserDBs.GetFromIndex(i)->GetColumnCount() - 1) {
					this->stringBuffer += "\t";
				}
			}
			strbuf += this->stringBuffer + "\n";
		}
		strbuf += "\n";
	}
}


//####################  ���[�U�[DB�ւ̃A�N�Z�T�[  ####################
CDatabase::CDBBaseOne* CDatabase::CDBUser::GetUserDB(int id) {
	return this->UserDBs.GetFromID(id);
}


//####################  �f�[�^�x�[�X�̒��g�����O�Ƃ��ďo�͂���  ####################
void CDatabase::CDBUser::PrintData() {
	TS_LOG("\nDB: " + this->fileName);
	for(auto& db : this->UserDBs.Items) {
		TS_LOG("\n[" + db.Name + "]");

		//�v�f���o�͂���
		string strbuf;
		for(int row = -1; row < db.GetRowCount(); row++) {
			strbuf = "";
			TS_COUNTLOOP(col, db.GetColumnCount()) {
				if(row == -1) {
					//��
					strbuf += db.GetColumnName(col) + "(" + db.GetColumnType(col) + ")";
				} else {
					//�v�f�̒l
					strbuf += db.GetCellFromIndex(row, col)->ToString();
				}
				strbuf += "\t";
			}
			TS_LOG(strbuf);
		}
	}
}


//####################  �j������  ####################
void CDatabase::CDBUser::Dispose() {
	this->UserDBs.Clear();
}
