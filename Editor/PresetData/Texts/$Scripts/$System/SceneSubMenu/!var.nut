//###################################################################
//		���j���[�V�[��: �O���[�o���ϐ�
//###################################################################

//���j���[�Œ�
DepthCount <- 13;					//���j���[�K�w��
TargetMemberDepth <- 10;			//�ΏۃL�����I���̊K�w�ԍ�
PlaceDepth <- 11;					//�ꏊ�I���̊K�w�ԍ�
ChoiceDepth <- 12;					//�I�����̊K�w�ԍ�
MaxStatusSubListCount <- 11;		//�X�e�[�^�X���j���[�̃T�u���X�g���ɕ\���ł��鍀�ڐ�
SaveAreaMax <- 8;					//�Z�[�u���X�g�̈�ɕ\���ł���ő吔


//���j���[�ėp
Cursor <- array(DepthCount, 0);		//�e�K�w�̃J�[�\���ʒu
TopIndex <- array(DepthCount, 0);	//�e�K�w�̃E�B���h�E�̈�ɂ���擪�v�f�̃C���f�b�N�X
Depth <- 0;							//���j���[�K�w
ErrorMsg <- "";						//�G���[���b�Z�[�W���e
AreaMax <- 17;						//���X�g�̈�ɕ\���ł��鍀�ڐ�
WindowTransparent <- 0;				//�t�F�[�h����E�B���h�E�̓����x


//���j���[�Ǝ�
TargetMemberSelecting <- false;		//�ΏۃL�����I�𒆂��ǂ���
PlaceSelecting <- false;			//�ꏊ�I�𒆂��ǂ���
PlaceList <- null;					//�I���\�ȏꏊ���X�g (CIDNameData)
OneMenuMode <- false;				//�P�̃��j���[��L���ɂ��ăT�C�h�̃R�}���h�ꗗ���\���ɂ��邩�ǂ���


//�`����
LeftOffset <- 350;					//�R�}���h���j���[���̃I�t�Z�b�g
MeterOffset <- 110;					//���[�^�[�p�̃I�t�Z�b�g
MeterWidth <- 150;					//���[�^�[��
MeterHeight <- 10;					//���[�^�[����


//�������j���[�ŕK�v�Ȃ���
EquipTypeList <- array(0);			//�������ʂ�ID���X�g
EquipList <- array(0);				//�������ʂōi�荞�܂ꂽ�����i���X�g (CIDValueData)


//�Ώۃ����o�[�I���̎��
SelectMemberType <- {
	One = 0,		//������l
	OneDead = 1,	//������l�E���Ҋ܂�
	All = 2,		//�����S��
};


//���j���[����
MenuItem <- {
	Status = 0,		//�X�e�[�^�X
	Item = 1,		//�A�C�e��
	Equip = 2,		//����
	Skill = 3,		//�X�L��
	Member = 4,		//���ёւ�
	Save = 5,		//�Z�[�u
	Option = 6,		//���ݒ�
};


//�I�v�V��������
OptionMenu <- {
	MapMessageSpeed = 0,		//��b���b�Z�[�W�\�����x
	BattleMessageSpeed = 1,		//�퓬���b�Z�[�W�\�����x
	SoundVolume = 2,			//�T�E���h����
};
