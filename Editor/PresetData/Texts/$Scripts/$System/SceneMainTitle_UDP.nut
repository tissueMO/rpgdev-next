//###################################################################
//		タイトルシーン: UDP接続のサンプルスクリプト
//		このスクリプトは動作中にリロードしないで下さい。
//###################################################################

PortNum <- 8900;		//ポート番号
Mode <- -1;				//接続モード
IP <- null;				//接続先のIPアドレス
HdlUDPSend <- -1;		//UDP送信ソケット
HdlUDPRecv <- -1;		//UDP受信ソケット
MyName <- "";			//ユーザー名
MyPos <- CPoint();		//自分の表示位置
YourName <- "";			//相手のユーザー名
YourPos <- CPoint();	//相手の表示位置
MoveSpeed <- 5;			//移動速度
StartTime <- -1;		//開始時刻


//####################  接続開始準備  ####################
function SceneMainSetup() {
	ShowMessage(-1, "システム", "UDP接続のテストを行います。\nここでは接続先の画面を共有してみましょう。\n予めポート {PortNum} 番が解放されている必要があります。");

	print("あなたの表示名を入力して下さい...");
	MyName = Game.ScanConsole();

	IP = CIPv4Address();
	do {
		print("接続先のIPアドレスを [xxx.xxx.xxx.xxx] の形式で入力して下さい...");
		local addrText = Game.ScanConsole();
		local spl = split(strip(addrText), ".");
		if(spl.len() != 4) {
			print("無効なIPアドレスです。");
			continue;
		}
		IP.Set(spl[0].tointeger(), spl[1].tointeger(), spl[2].tointeger(), spl[3].tointeger());
		break;
	} while(true);

	//UDP送信ソケット作成
	HdlUDPSend = Game.DXLib.MakeUDPSocket(-1);
	HdlUDPRecv = Game.DXLib.MakeUDPSocket(PortNum);

	//初期化
	StartTime = Game.DXLib.GetNowCount();
	MyPos.Set(Game.WindowSize.Width / 2, Game.WindowSize.Height / 2);
	YourPos.Set(Game.WindowSize.Width / 2, Game.WindowSize.Height / 2);
}


//####################  送信処理  ####################
function SceneMainInput() {
	//表示位置を動かす
	if(Game.GetKeyInputCounter(KeyID.Right) > 0) {
		MyPos.X += MoveSpeed;
	}
	if(Game.GetKeyInputCounter(KeyID.Left) > 0) {
		MyPos.X -= MoveSpeed;
	}
	if(Game.GetKeyInputCounter(KeyID.Down) > 0) {
		MyPos.Y += MoveSpeed;
	}
	if(Game.GetKeyInputCounter(KeyID.Up) > 0) {
		MyPos.Y -= MoveSpeed;
	}

	//毎フレームでユーザー名と位置情報を送る
	Game.DXLib.NetWorkSendUDP(HdlUDPSend, IP, PortNum, MyName + "|" + MyPos.X + "|" + MyPos.Y);
	StartTime = Game.DXLib.GetNowCount();
}


//####################  受信処理  ####################
function SceneMainDraw() {
	//受信チェック
	if(Game.DXLib.CheckNetWorkRecvUDP(HdlUDPRecv) == TRUE) {
		//データ受信
		Game.DXLib.NetWorkRecvUDP(HdlUDPRecv, false);
		local spl = split(Game.DXLib.RecvData, "|");
		if(spl.len() == 3) {
			//受け取ったデータを反映
			YourName = spl[0];
			YourPos.Set(spl[1].tointeger(), spl[2].tointeger());
		}
	}

	//自分のユーザー名を描画する
	Game.DXLib.DrawStringC(MyPos.X, MyPos.Y, 0x1B6F049F/*標準フォント*/, CYAN, MyName);

	//相手方のユーザー名を描画する
	Game.DXLib.DrawStringC(YourPos.X, YourPos.Y, 0x1B6F049F/*標準フォント*/, PINK, YourName);
}


//####################  破棄処理  ####################
function SceneMainDispose() {
	print("UDPソケットを解放します。");
	Game.DXLib.DeleteUDPSocket(HdlUDPRecv);
	Game.DXLib.DeleteUDPSocket(HdlUDPSend);
}
