//###################################################################
//		タイトルシーン: TCP接続のサンプルスクリプト
//		このスクリプトは動作中にリロードしないで下さい。
//###################################################################

PortNum <- 8900;	//ポート番号
Mode <- -1;			//接続モード
IP <- null;			//接続先のIPアドレス
HdlNet <- -1;		//接続しているハンドル
HdlLost <- -1;		//切断されたハンドル

//接続モード
enum ConnectMode {
	Server,			//接続を受ける側
	Client,			//接続を申し込む側
};


//####################  接続開始準備  ####################
function SceneMainSetup() {
	ShowMessage(-1, "システム", "TCP接続のテストを行います。\nここでは簡単なチャットをしてみましょう。\n予めポート {PortNum} 番が解放されている必要があります。\a");
	Mode = Choice("相手からの接続を待つ|接続先を指定する", false);
	IP = CIPv4Address();
	switch(Mode) {
		case ConnectMode.Server:
			//接続があるまで待つ
			HdlNet = -1;
			print("接続待機中です...");
			Game.DXLib.PreparationListenNetWork(PortNum);
			while(Game.Wait(0, true, false) && HdlNet == -1) {
				HdlNet = Game.DXLib.GetNewAcceptNetWork();
			}
			
			if(HdlNet == -1) {
				print("接続に失敗しました。");
				Game.DB.FDBMaterial.PlayForWait(0x293AF6A5/*エラー音*/, 255, 0, 0);
				Game.ApplicationExit(ExitCode.Success);
				return;
			}

			//接続確立
			Game.DXLib.StopListenNetWork();
			IP = Game.DXLib.GetNetWorkIP(HdlNet);
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			print("接続が確立しました。 -> 接続先: " + IP.ToString());
			break;
			
		case ConnectMode.Client:
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
			
			HdlNet = -1;
			for(local i = 0; i < 3 && HdlNet == -1; i++) {
				HdlNet = Game.DXLib.ConnectNetWork(IP, PortNum);
				if(HdlNet == -1) {
					print(format("接続に失敗しました。 [%d 回目]", i + 1));
					Game.DB.FDBMaterial.PlayForWait(0x293AF6A5/*エラー音*/, 255, 0, 0);
					Game.Wait(3.0, false, false);
				}
			}
			if(HdlNet == -1) {
				print("接続できませんでした。");
				Game.DB.FDBMaterial.PlayForWait(0x293AF6A5/*エラー音*/, 255, 0, 0);
				Game.ApplicationExit(ExitCode.Success);
				return;
			}

			//接続確立
			Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
			print("接続が確立しました。 -> 接続先: " + IP.ToString());
			break;
	}
	
	print("Enterキーを押すと文字列の入力が始まり、データを送信します。");
}


//####################  送信処理  ####################
function SceneMainInput() {
	//接続切断のチェック
	HdlLost = Game.DXLib.GetLostNetWork();
	if(HdlLost == HdlNet) {
		print("接続切断");
		Game.ApplicationExit(ExitCode.Success);
		return;
	}

	if(CheckDecideKeyInput() == 1) {
		//情報を送る
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		print("送信する文字列を入力して下さい...");
		local data = Game.DXLib.KeyInputString(0, 0, 1000, true);
		if(Game.DXLib.KeyInputResult != TRUE) {
			print("中断されました。");
			return;
		}
		
		//データ送信
		print("データ送信: [" + data + "]");
		Game.DXLib.NetWorkSend(HdlNet, data);
	}
}


//####################  受信処理  ####################
function SceneMainDraw() {
	//情報を受ける
	if(Game.DXLib.GetNetWorkDataLength(HdlNet) == 0) {
		return;		//受けていない
	}
	
	//データ受信
	local length = Game.DXLib.GetNetWorkDataLength(HdlNet);
	if(Game.DXLib.NetWorkRecv(HdlNet, length) == 0) {
		Game.DB.FDBMaterial.Play(0x61301713/*決定音*/, 255, 0, 0);
		print("データ受信: [" + Game.DXLib.RecvData + "]");
	}
}


//####################  破棄処理  ####################
function SceneMainDispose() {
	print("接続を正常に切断します。");
	Game.DXLib.CloseNetWork(HdlNet);
}
