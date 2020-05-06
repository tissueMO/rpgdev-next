//###################################################################
//		DXライブラリラッパークラス
//###################################################################

class CDXLibWrapper {
public:
	//####################  通信をするのに必要となるIPアドレス情報のラッパークラス  ####################
	class CIPv4Address {
	public:
		//SQ_BEGINMEMBER
		//コンストラクター
		SQ_NOBIND CIPv4Address() = default;
		SQ_NOBIND CIPv4Address(const IPDATA* ip) {
			this->Set(ip->d1, ip->d2, ip->d3, ip->d4);
		}

		//メンバー変数
		SQ_NOBIND array<int, 4> IP;

		//メンバー関数
		//アドレスをセットする
		SQ_INSTANCEFUNC void Set(int d1, int d2, int d3, int d4) {
			this->IP.at(0) = d1;
			this->IP.at(1) = d2;
			this->IP.at(2) = d3;
			this->IP.at(3) = d4;
		}

		//指定したバイト位置の値を取得する
		SQ_INSTANCEFUNC int Get(int d) {
			TS_CHECKINDEX(d, 0, 4) {
				return this->IP.at(d);
			} else {
				return -1;
			}
		}

		//IPアドレスを文字列として返す
		SQ_INSTANCEFUNC string ToString() {
			return to_string(this->IP.at(0)) + "." + to_string(this->IP.at(1)) + "." + to_string(this->IP.at(2)) + "." + to_string(this->IP.at(3));
		}

		//DXライブラリで使われるIPアドレス型に変換する
		SQ_NOBIND IPDATA ToIPDATA() {
			IPDATA ip;
			ip.d1 = this->IP.at(0);
			ip.d2 = this->IP.at(1);
			ip.d3 = this->IP.at(2);
			ip.d4 = this->IP.at(3);
			return ip;
		}
	};

	//SQ_BEGINMEMBER
	//メンバー変数
	SQ_NOBIND CPoint Offset;
	SQ_PROPGET CPoint* getOffset() { return &this->Offset; }

	SQ_NOBIND int KeyInputResult = -1;
	SQ_PROPGET int getKeyInputResult() { return this->KeyInputResult; }

	SQ_PROPGET string getRecvData() { return string(this->netWorkBuf); }

	SQ_PROPGET CIPv4Address getUDPRecvIP() { return CIPv4Address(&this->UDPRecvIP); }

	SQ_PROPGET int getUDPRecvPortNum() { return this->UDPRecvPortNum; }


	//メンバー関数
	//応用処理
	SQ_INSTANCEFUNC void Flash(int color, int strength, int speed, int count);
	SQ_INSTANCEFUNC void ShakeOne(int strength);
	SQ_INSTANCEFUNC void StartShake(int strength);
	SQ_INSTANCEFUNC void StopShake();
	SQ_INSTANCEFUNC void DrawWindow(int hdl, int intX, int intY, int intWidth, int intHeight);
	SQ_INSTANCEFUNC void DrawCursorPanel(int hdl, int intX, int intY, int intWidth, int intHeight);
	SQ_INSTANCEFUNC void DrawCursorArrow(int hdl, int intX, int intY, int intDir4);

	//基本図形描画
	SQ_INSTANCEFUNC void DrawPixel(int x, int y, int color);
	SQ_INSTANCEFUNC void DrawLine(int sx, int sy, int ex, int ey, int color, int thickness = 1);
	SQ_INSTANCEFUNC void DrawTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color, bool fill = true);
	SQ_INSTANCEFUNC void DrawBox(int left, int top, int right, int bottom, int color, bool fill = true, int thickness = 1);
	SQ_INSTANCEFUNC void DrawRect(int left, int top, int width, int height, int color, bool fill = true, int thickness = 1);
	SQ_INSTANCEFUNC void DrawCircle(int x, int y, int r, int color, bool fill = true, int thickness = 1);
	SQ_INSTANCEFUNC void DrawOval(int x, int y, int rx, int ry, int color, bool fill = true, int thickness = 1);

	//基本グラフィックス描画
	SQ_INSTANCEFUNC void DrawGraph(int Handle, int x, int y, bool trans = true, bool turn = false);
	SQ_INSTANCEFUNC void DrawExtendGraph(int Handle, int x, int y, double xRate, double yRate = 0.0F, bool trans = true);
	SQ_INSTANCEFUNC void DrawRotaGraph(int Handle, int cx, int cy, double angle, double exRate = 1.0F, bool trans = true, bool turn = false);
	SQ_INSTANCEFUNC void DrawRectGraph(int Handle, int sx, int sy, int gx, int gy, int width, int height, bool trans = true, bool turn = false);
	SQ_INSTANCEFUNC void DrawRectExtendGraph(int Handle, int left, int top, int width, int height, int gx, int gy, int rawWidth, int rawHeight, bool trans = true);
	SQ_INSTANCEFUNC void DrawRectRotaGraph(int Handle, int cx, int cy, int gx, int gy, int rawWidth, int rawHeight, double angle, double exRate = 1.0F, bool trans = true, bool turn = false);
	SQ_INSTANCEFUNC void DrawModifiedGraph(int Handle, int leftTopX, int leftTopY, int rightTopX, int rightTopY, int rightBottomX, int rightBottomY, int leftBottomX, int leftBottomY, bool trans = true);

	//応用グラフィックス描画
	SQ_INSTANCEFUNC void DrawBlendGraph(int x, int y, int Handle, bool trans, string blendFileName, int borderParam);

	//文字列描画
	SQ_INSTANCEFUNC void DrawString(int x, int y, int fontID, int color, const TCHAR* text);
	SQ_INSTANCEFUNC void DrawStringC(int centerX, int y, int fontID, int color, const TCHAR* text);
	SQ_INSTANCEFUNC void DrawStringR(int right, int y, int fontID, int color, const TCHAR* text);
	SQ_INSTANCEFUNC void DrawStringAlign(int x, int y, int align, int fontID, int color, const TCHAR* text);

	//描画系ユーティリティ
	SQ_INSTANCEFUNC CSize GetGraphSize(int Handle);
	SQ_INSTANCEFUNC void SetDrawBlendMode(int mode, int param);
	SQ_INSTANCEFUNC void RestoreDrawBlendMode();
	SQ_INSTANCEFUNC void SetDrawBright(int r, int g, int b);
	SQ_INSTANCEFUNC void ResetDrawArea();
	SQ_INSTANCEFUNC void SetDrawArea(int left, int top, int right, int bottom);
	SQ_NOBIND void SetDrawArea(CRectangle& range);
	SQ_INSTANCEFUNC CSize GetDrawStringSize(int fontID, const TCHAR* text);
	SQ_INSTANCEFUNC int GetColor(int r, int g, int b);
	SQ_NOBIND static int ConvertToDXBlend(int blendMode);

	//動画再生
	SQ_INSTANCEFUNC void PlayVideo(string fileName, int exRate, bool canCancel);
	SQ_INSTANCEFUNC void PlayMovieToGraph(int Handle);
	SQ_INSTANCEFUNC void PauseMovieToGraph(int Handle);
	SQ_INSTANCEFUNC void SeekMovieToGraph(int Handle, int timeMS);
	SQ_INSTANCEFUNC int TellMovieToGraph(int Handle);
	SQ_INSTANCEFUNC int GetMovieStateToGraph(int Handle);		//戻り値は TS::Material::PlayStatus に対応

	//入力関連
	SQ_INSTANCEFUNC int GetJoypadNum();
	SQ_INSTANCEFUNC int GetJoypadInputState(int playerNum);
	SQ_INSTANCEFUNC CPoint GetJoypadAnalogInput(int playerNum);
	SQ_INSTANCEFUNC void StartJoypadVibration(int playerNum, int power, int time);
	SQ_INSTANCEFUNC void StopJoypadVibration(int playerNum);
	SQ_INSTANCEFUNC void SetMouseDispFlag(bool flag);
	SQ_INSTANCEFUNC CPoint GetMousePoint();
	SQ_INSTANCEFUNC void SetMousePoint(int x, int y);
	SQ_INSTANCEFUNC int GetMouseInput();
	SQ_INSTANCEFUNC int GetMouseWheelRotVol();
	SQ_INSTANCEFUNC string KeyInputString(int x, int y, int maxCharLength, bool allowCancel);
	SQ_INSTANCEFUNC string KeyInputSingleCharString(int x, int y, int maxCharLength, bool allowCancel);
	SQ_INSTANCEFUNC int KeyInputNumber(int x, int y, int min, int max, bool allowCancel);
	SQ_INSTANCEFUNC void SetKeyInputStringColor(const TCHAR* param);

	//ユーティリティ
	SQ_INSTANCEFUNC int GetNowCount();
	SQ_INSTANCEFUNC void SaveDrawScreenToPNG(int leftTopX, int leftTopY, int rightBottomX, int rightBottomY, string fileName);

	//非同期読み込み
	SQ_INSTANCEFUNC int CheckHandleAsyncLoad(int handle) { return ::CheckHandleASyncLoad(handle); }
	SQ_INSTANCEFUNC int GetASyncLoadNum() { return ::GetASyncLoadNum(); }

	//ファイル読み込み
	SQ_INSTANCEFUNC int FileRead_open(string fileName);
	SQ_INSTANCEFUNC void FileRead_close(int handle);
	SQ_INSTANCEFUNC string FileRead_gets(int handle);
	SQ_INSTANCEFUNC int FileRead_getc(int handle);
	SQ_INSTANCEFUNC bool FileRead_eof(int handle);

	//通信関連
	SQ_INSTANCEFUNC int ConnectNetWork(CIPv4Address ip, int portNum);
	SQ_INSTANCEFUNC int CloseNetWork(int handle);
	SQ_INSTANCEFUNC int PreparationListenNetWork(int portNum);
	SQ_INSTANCEFUNC int StopListenNetWork();
	SQ_INSTANCEFUNC int NetWorkSend(int handle, const TCHAR* data);
	SQ_INSTANCEFUNC int GetNetWorkDataLength(int handle);
	SQ_INSTANCEFUNC int GetNetWorkSendDataLength(int handle);
	SQ_INSTANCEFUNC int NetWorkRecv(int handle, int length);
	SQ_INSTANCEFUNC int NetWorkRecvToPeek(int handle, int length);
	SQ_INSTANCEFUNC int GetNewAcceptNetWork();
	SQ_INSTANCEFUNC int GetLostNetWork();
	SQ_INSTANCEFUNC int GetNetWorkAcceptState(int handle);
	SQ_INSTANCEFUNC CIPv4Address GetNetWorkIP(int handle);

	SQ_INSTANCEFUNC int MakeUDPSocket(int recvPortNum);
	SQ_INSTANCEFUNC int DeleteUDPSocket(int UDPHandle);
	SQ_INSTANCEFUNC int NetWorkSendUDP(int UDPHandle, CIPv4Address sendIP, int sendPortNum, const TCHAR* data);
	SQ_INSTANCEFUNC int NetWorkRecvUDP(int UDPHandle, bool peekMode);
	SQ_INSTANCEFUNC int CheckNetWorkRecvUDP(int UDPHandle);

	//DXアーカイブ
	SQ_NOBIND bool EncodeArchive(string srcFullPath, string destFullPath);
	SQ_NOBIND bool DecodeArchive(string srcFullPath, string destFullPath);


private:
	int beforeBlendMode = -1;
	int beforeBlendParam = 0;
	int currentBlendMode = -1;
	int currentBlendParam = 0;
	int getPadInputType(int playerNum);

	//通信関連
	char netWorkBuf[1024];		//送受信に使うバッファ
	IPDATA UDPRecvIP;			//UDPで受信したデータの送信元アドレス
	int UDPRecvPortNum = -1;	//UDPで受信したデータの受信ポート番号
};
