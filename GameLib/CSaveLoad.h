//###################################################################
//		セーブデータ管理クラス
//###################################################################

class CSaveLoad {
public:
	//SQ_NOBIND_BEGIN
	//ヘッダー単一データ
	class CSaveHeaderData {
	public:
		bool Enabled;		//有効なセーブデータであるかどうか
		string DateTime;	//セーブ日時
		string Header;		//バージョンを含まないヘッダーテキスト

		//コンストラクター
		CSaveHeaderData(bool enabled, string date, string header) {
			this->Enabled = enabled;
			this->DateTime = date;
			this->Header = header;
		}
	};
	//SQ_NOBIND_END

	//SQ_BEGINMEMBER
	//デストラクター
	SQ_NOBIND ~CSaveLoad() {
		vector<CSaveHeaderData>().swap(this->saveHeaderList);
	}

	//メンバーメソッド
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
	//メンバー変数
	vector<CSaveHeaderData> saveHeaderList;			//ヘッダーのみ格納する
	string temporarySaveData;						//一時的に保存できる内部セーブデータ領域

	//プライベートメソッド
	string generateRawData();
	bool loadRawData(const string& textBuffer);
	bool encodeStringToFile(const string& plain, string fileName);
	bool decodeFileToString(string& plain, string fileName);
};
