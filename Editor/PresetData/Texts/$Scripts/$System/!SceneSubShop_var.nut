//###################################################################
//		�V���b�v�V�[��: �O���[�o���ϐ�
//###################################################################

//���j���[�Œ�
DepthCount <- 13;					//���j���[�K�w��
ChoiceDepth <- 12;					//�I�����̊K�w�ԍ�


//���j���[�ėp
Cursor <- array(DepthCount, 0);		//�e�K�w�̃J�[�\���ʒu
TopIndex <- array(DepthCount, 0);	//�e�K�w�̃E�B���h�E�̈�ɂ���擪�v�f�̃C���f�b�N�X
Depth <- 0;							//���j���[�K�w
ErrorMsg <- "";						//�G���[���b�Z�[�W���e
AreaMax <- 15;						//���X�g�̈�ɕ\���ł��鍀�ڐ�
WindowTransparent <- 0;				//�t�F�[�h����E�B���h�E�̓����x


//���X�����ɕK�v�Ȃ���
ShowAtSign <- true;					//�P���L����\�����邩�ǂ���
BuySellSEID <- 0x61301713/*���艹*/;		//�������̌��ʉ�ID


//���j���[����
MenuItem <- {
	Buy = 0,		//�w��
	Sell = 1,		//���p
	Exit = 2,		//�I��
};
