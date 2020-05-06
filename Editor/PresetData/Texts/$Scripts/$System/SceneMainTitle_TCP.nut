//###################################################################
//		�^�C�g���V�[��: TCP�ڑ��̃T���v���X�N���v�g
//		���̃X�N���v�g�͓��쒆�Ƀ����[�h���Ȃ��ŉ������B
//###################################################################

PortNum <- 8900;	//�|�[�g�ԍ�
Mode <- -1;			//�ڑ����[�h
IP <- null;			//�ڑ����IP�A�h���X
HdlNet <- -1;		//�ڑ����Ă���n���h��
HdlLost <- -1;		//�ؒf���ꂽ�n���h��

//�ڑ����[�h
enum ConnectMode {
	Server,			//�ڑ����󂯂鑤
	Client,			//�ڑ���\�����ޑ�
};


//####################  �ڑ��J�n����  ####################
function SceneMainSetup() {
	ShowMessage(-1, "�V�X�e��", "TCP�ڑ��̃e�X�g���s���܂��B\n�����ł͊ȒP�ȃ`���b�g�����Ă݂܂��傤�B\n�\�߃|�[�g {PortNum} �Ԃ��������Ă���K�v������܂��B\a");
	Mode = Choice("���肩��̐ڑ���҂�|�ڑ�����w�肷��", false);
	IP = CIPv4Address();
	switch(Mode) {
		case ConnectMode.Server:
			//�ڑ�������܂ő҂�
			HdlNet = -1;
			print("�ڑ��ҋ@���ł�...");
			Game.DXLib.PreparationListenNetWork(PortNum);
			while(Game.Wait(0, true, false) && HdlNet == -1) {
				HdlNet = Game.DXLib.GetNewAcceptNetWork();
			}
			
			if(HdlNet == -1) {
				print("�ڑ��Ɏ��s���܂����B");
				Game.DB.FDBMaterial.PlayForWait(0x293AF6A5/*�G���[��*/, 255, 0, 0);
				Game.ApplicationExit(ExitCode.Success);
				return;
			}

			//�ڑ��m��
			Game.DXLib.StopListenNetWork();
			IP = Game.DXLib.GetNetWorkIP(HdlNet);
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			print("�ڑ����m�����܂����B -> �ڑ���: " + IP.ToString());
			break;
			
		case ConnectMode.Client:
			do {
				print("�ڑ����IP�A�h���X�� [xxx.xxx.xxx.xxx] �̌`���œ��͂��ĉ�����...");
				local addrText = Game.ScanConsole();
				local spl = split(strip(addrText), ".");
				if(spl.len() != 4) {
					print("������IP�A�h���X�ł��B");
					continue;
				}
				IP.Set(spl[0].tointeger(), spl[1].tointeger(), spl[2].tointeger(), spl[3].tointeger());
				break;
			} while(true);
			
			HdlNet = -1;
			for(local i = 0; i < 3 && HdlNet == -1; i++) {
				HdlNet = Game.DXLib.ConnectNetWork(IP, PortNum);
				if(HdlNet == -1) {
					print(format("�ڑ��Ɏ��s���܂����B [%d ���]", i + 1));
					Game.DB.FDBMaterial.PlayForWait(0x293AF6A5/*�G���[��*/, 255, 0, 0);
					Game.Wait(3.0, false, false);
				}
			}
			if(HdlNet == -1) {
				print("�ڑ��ł��܂���ł����B");
				Game.DB.FDBMaterial.PlayForWait(0x293AF6A5/*�G���[��*/, 255, 0, 0);
				Game.ApplicationExit(ExitCode.Success);
				return;
			}

			//�ڑ��m��
			Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
			print("�ڑ����m�����܂����B -> �ڑ���: " + IP.ToString());
			break;
	}
	
	print("Enter�L�[�������ƕ�����̓��͂��n�܂�A�f�[�^�𑗐M���܂��B");
}


//####################  ���M����  ####################
function SceneMainInput() {
	//�ڑ��ؒf�̃`�F�b�N
	HdlLost = Game.DXLib.GetLostNetWork();
	if(HdlLost == HdlNet) {
		print("�ڑ��ؒf");
		Game.ApplicationExit(ExitCode.Success);
		return;
	}

	if(CheckDecideKeyInput() == 1) {
		//���𑗂�
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		print("���M���镶�������͂��ĉ�����...");
		local data = Game.DXLib.KeyInputString(0, 0, 1000, true);
		if(Game.DXLib.KeyInputResult != TRUE) {
			print("���f����܂����B");
			return;
		}
		
		//�f�[�^���M
		print("�f�[�^���M: [" + data + "]");
		Game.DXLib.NetWorkSend(HdlNet, data);
	}
}


//####################  ��M����  ####################
function SceneMainDraw() {
	//�����󂯂�
	if(Game.DXLib.GetNetWorkDataLength(HdlNet) == 0) {
		return;		//�󂯂Ă��Ȃ�
	}
	
	//�f�[�^��M
	local length = Game.DXLib.GetNetWorkDataLength(HdlNet);
	if(Game.DXLib.NetWorkRecv(HdlNet, length) == 0) {
		Game.DB.FDBMaterial.Play(0x61301713/*���艹*/, 255, 0, 0);
		print("�f�[�^��M: [" + Game.DXLib.RecvData + "]");
	}
}


//####################  �j������  ####################
function SceneMainDispose() {
	print("�ڑ��𐳏�ɐؒf���܂��B");
	Game.DXLib.CloseNetWork(HdlNet);
}
