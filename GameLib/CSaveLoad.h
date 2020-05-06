//###################################################################
//		�Z�[�u�f�[�^�Ǘ��N���X
//###################################################################

class CSaveLoad {
public:
	//SQ_NOBIND_BEGIN
	//�w�b�_�[�P��f�[�^
	class CSaveHeaderData {
	public:
		bool Enabled;		//�L���ȃZ�[�u�f�[�^�ł��邩�ǂ���
		string DateTime;	//�Z�[�u����
		string Header;		//�o�[�W�������܂܂Ȃ��w�b�_�[�e�L�X�g

		//�R���X�g���N�^�[
		CSaveHeaderData(bool enabled, string date, string header) {
			this->Enabled = enabled;
			this->DateTime = date;
			this->Header = header;
		}
	};
	//SQ_NOBIND_END

	//SQ_BEGINMEMBER
	//�f�X�g���N�^�[
	SQ_NOBIND ~CSaveLoad() {
		vector<CSaveHeaderData>().swap(this->saveHeaderList);
	}

	//�����o�[���\�b�h
	SQ_INSTANCEFUNC void GetAllSaveHeader();
	SQ_INSTANCEFUNC bool CopyData(int fromIndex, int toIndex);
	SQ_INSTANCEFUNC bool DeleteData(int targetIndex);
	SQ_INSTANCEFUNC bool LoadData(int targetIndex);
	SQ_INSTANCEFUNC bool SaveData(int targetIndex);
	SQ_INSTANCEFUNC bool QuickSave();
	SQ_INSTANCEFUNC bool QuickLoad();
	SQ_INSTANCEFUNC bool IsEnabled(int index);
	SQ_INSTANCEFUNC string GetSaveHeader(int index);
	SQ_INSTANCEFUNC string GetSaveDateTime(int index);
	SQ_NOBIND bool GetPlainData(int index, string& plain);

private:
	//�����o�[�ϐ�
	vector<CSaveHeaderData> saveHeaderList;			//�w�b�_�[�̂݊i�[����
	string temporarySaveData;						//�ꎞ�I�ɕۑ��ł�������Z�[�u�f�[�^�̈�

	//�v���C�x�[�g���\�b�h
	string generateRawData();
	bool loadRawData(const string& textBuffer);
	bool encodeStringToFile(const string& plain, string fileName);
	bool decodeFileToString(string& plain, string fileName);
};
