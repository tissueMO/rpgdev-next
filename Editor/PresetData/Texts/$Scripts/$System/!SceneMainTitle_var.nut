//###################################################################
//		�^�C�g���V�[��: �O���[�o���ϐ�
//###################################################################

//���j���[�Œ�
DepthCount <- 13;					//���j���[�K�w��


//���j���[�ėp
Cursor <- array(DepthCount, 0);		//�e�K�w�̃J�[�\���ʒu
TopIndex <- array(DepthCount, 0);	//�e�K�w�̃E�B���h�E�̈�ɂ���擪�v�f�̃C���f�b�N�X
Depth <- 0;							//���j���[�K�w
AreaMax <- 4;						//�Z�[�u�f�[�^�̈�ɕ\���ł��鍀�ڐ�
WindowTransparent <- 0;				//�t�F�[�h����E�B���h�E�̓����x


//�^�C�g����ʂɕK�v�Ȃ���
Graphs <- array(10, -1);			//�O���t�B�b�N�n���h��
SaveCount <- Game.DB.FDBSystem.GetInteger(DB_System_MaxSaveDataCount);		//�Z�[�u�f�[�^��


//���ڃ��X�g
MenuItem <- {
	Start = 0,			//�͂��߂���
	Load = 1,			//�Â�����
	Exit = 2,			//�Q�[���I��
};

//�u�͂��߂���v�̏��
TitleStart <- {
	MapFileName = "Maps\\01 ���I�i���h�̗�\\���O��.map",		//�}�b�v
	PositionX = 22,							//X���W
	PositionY = 6,							//Y���W
	Direction = Direction4.South,			//����
};
