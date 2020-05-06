//###################################################################
//		�^�C�g���V�[��: UDP�ڑ��̃T���v���X�N���v�g
//		���̃X�N���v�g�͓��쒆�Ƀ����[�h���Ȃ��ŉ������B
//###################################################################

PortNum <- 8900;		//�|�[�g�ԍ�
Mode <- -1;				//�ڑ����[�h
IP <- null;				//�ڑ����IP�A�h���X
HdlUDPSend <- -1;		//UDP���M�\�P�b�g
HdlUDPRecv <- -1;		//UDP��M�\�P�b�g
MyName <- "";			//���[�U�[��
MyPos <- CPoint();		//�����̕\���ʒu
YourName <- "";			//����̃��[�U�[��
YourPos <- CPoint();	//����̕\���ʒu
MoveSpeed <- 5;			//�ړ����x
StartTime <- -1;		//�J�n����


//####################  �ڑ��J�n����  ####################
function SceneMainSetup() {
	ShowMessage(-1, "�V�X�e��", "UDP�ڑ��̃e�X�g���s���܂��B\n�����ł͐ڑ���̉�ʂ����L���Ă݂܂��傤�B\n�\�߃|�[�g {PortNum} �Ԃ��������Ă���K�v������܂��B");

	print("���Ȃ��̕\��������͂��ĉ�����...");
	MyName = Game.ScanConsole();

	IP = CIPv4Address();
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

	//UDP���M�\�P�b�g�쐬
	HdlUDPSend = Game.DXLib.MakeUDPSocket(-1);
	HdlUDPRecv = Game.DXLib.MakeUDPSocket(PortNum);

	//������
	StartTime = Game.DXLib.GetNowCount();
	MyPos.Set(Game.WindowSize.Width / 2, Game.WindowSize.Height / 2);
	YourPos.Set(Game.WindowSize.Width / 2, Game.WindowSize.Height / 2);
}


//####################  ���M����  ####################
function SceneMainInput() {
	//�\���ʒu�𓮂���
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

	//���t���[���Ń��[�U�[���ƈʒu���𑗂�
	Game.DXLib.NetWorkSendUDP(HdlUDPSend, IP, PortNum, MyName + "|" + MyPos.X + "|" + MyPos.Y);
	StartTime = Game.DXLib.GetNowCount();
}


//####################  ��M����  ####################
function SceneMainDraw() {
	//��M�`�F�b�N
	if(Game.DXLib.CheckNetWorkRecvUDP(HdlUDPRecv) == TRUE) {
		//�f�[�^��M
		Game.DXLib.NetWorkRecvUDP(HdlUDPRecv, false);
		local spl = split(Game.DXLib.RecvData, "|");
		if(spl.len() == 3) {
			//�󂯎�����f�[�^�𔽉f
			YourName = spl[0];
			YourPos.Set(spl[1].tointeger(), spl[2].tointeger());
		}
	}

	//�����̃��[�U�[����`�悷��
	Game.DXLib.DrawStringC(MyPos.X, MyPos.Y, 0x1B6F049F/*�W���t�H���g*/, CYAN, MyName);

	//������̃��[�U�[����`�悷��
	Game.DXLib.DrawStringC(YourPos.X, YourPos.Y, 0x1B6F049F/*�W���t�H���g*/, PINK, YourName);
}


//####################  �j������  ####################
function SceneMainDispose() {
	print("UDP�\�P�b�g��������܂��B");
	Game.DXLib.DeleteUDPSocket(HdlUDPRecv);
	Game.DXLib.DeleteUDPSocket(HdlUDPSend);
}
