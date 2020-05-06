//###################################################################
//		�o�g���V�[��: �O���[�o���ϐ�
//###################################################################

//���j���[�Œ�
DepthCount <- 13;				//���j���[�K�w��
TargetPTDepth <- 10;			//�Ώۃp�[�e�B�L�����I���̊K�w�ԍ�
TargetUTDepth <- 11;			//�ΏۃG�l�~�[�I���̊K�w�ԍ�
ChoiceDepth <- 12;				//�I�����̊K�w�ԍ�


//���j���[�ėp
Cursor <- array(DepthCount, 0);		//�e�K�w�̃J�[�\���ʒu
TopIndex <- array(DepthCount, 0);	//�e�K�w�̃E�B���h�E�̈�ɂ���擪�v�f�̃C���f�b�N�X
Depth <- 0;							//���j���[�K�w
ErrorMsg <- "";						//�G���[���b�Z�[�W���e
AreaMax <- 8;						//���X�g�̈�ɕ\���ł��鍀�ڐ�
WindowTransparent <- 0;				//�t�F�[�h����E�B���h�E�̓����x


//���j���[�Ǝ�
TargetPTSelecting <- false;			//�Ώۃp�[�e�B�L�����I�𒆂��ǂ���
TargetPTAllowDead <- false;			//�Ώۃp�[�e�B�L�����̑I���Ɏ��҂��܂߂��邩�ǂ���
TargetUTSelecting <- false;			//�ΏۃG�l�~�[�I�𒆂��ǂ���


//��{���
UnitID <- -1;					//���j�b�gID
UnitSelfValues <- null;			//���j�b�g�ʕϐ�

DisabledEscape <- false;		//�������Ȃ��퓬��
DisabledGameOver <- false;		//�Q�[���I�[�o�[�𖳌��ɂ��邩

FileBack <- "";					//�퓬�w�i�̃t�@�C����
HdlBack <- -1;					//�퓬�w�i�̃n���h��
FileBGM <- "";					//�퓬BGM�̃t�@�C����
HdlBGM <- -1;					//�퓬BGM�̃n���h��

TurnCount <- 1;					//�o�߃^�[����
Phase <- -1;					//���݂̐퓬�t�F�[�Y
Result <- -1;					//�퓬����


//�`����
PartyPanelHeight <- 100;		//�p�[�e�B�p�l���̍���
CommentPanelHeight <- 32;		//���b�Z�[�W�p�l���̍���
LineHeight <- 27;				//�R�}���h���X�g�̃E�B���h�E��s���̍���
CommandPanelWidth <- 120;		//�R�}���h�p�l���̕�
MeterHeight <- 8;				//���[�^�[�̍���
OffsetCorner <- 16;				//�E�B���h�E���̕�����̍���I�t�Z�b�g


//�퓬�t�F�[�Y
BattlePhase <- {
	Setup = 0,			//����ʁE������
	Start = 1,			//�퓬�J�n�t�F�[�Y
	Command = 2,		//�R�}���h�I���t�F�[�Y
	Action = 3,			//�s�����t�F�[�Y
	Ending = 4,			//�퓬�I���t�F�[�Y
};


//�R�}���h
MenuItem <- {
	Attack = 0,			//�U��
	Defense = 1,		//�h��
	Skill = 2,			//�X�L��
	Item = 3,			//�A�C�e��
	Escape = 4,			//������
};


//�A�C�e���̑I�����
CommandItemType <- {
	Having = 0,			//�����i
	Equip = 1,			//�����i
};


//�퓬�������̊K�w
EndingDepth <- {
	EXPMoney = 0,		//�o���l�A�����l��
	LevelUp = 1,		//���x���A�b�v
	LearnSkill = 2,		//�X�L���K��
	DropItem = 3,		//�h���b�v�A�C�e��
};


//�A�N�^�[���X�g
Enemies <- array(0);			//���j�b�g���̃A�N�^�[���X�g (CDBActorData)


//�R�}���h�I���t�F�[�Y�ɕK�v�Ȃ���
SkillTypeList <- array(0);		//�X�L����ʃ��X�g (CIDNameData)
SkillList <- array(0);			//�X�L�����X�g (CIDValueData)

EquipTypeList <- array(0);		//�������ʂ�ID���X�g
EquipList <- array(0);			//�������ʂōi�荞�܂ꂽ�����i���X�g (CIDValueData)


//�s���t�F�[�Y�ɕK�v�Ȃ���
Actions <- array(0);			//�s�����X�g (SQActionData)
TurnEndingTasks <- array(0);	//�^�[���I�����̏������X�g (function)
TurnEndingTaskArgs <- array(0);	//�^�[���I�����̏����̈��� (table)

DamageEffectTasks <- array(0);	//�_���[�W���l�\�����X�g (SQDamageEffectData)
PTPanelShakes <- array(0);		//�p�[�e�B�p�l���\���ʒu�������p (SQShakeData)
DamageMotionUT <- array(0);		//�G�l�~�[�̃_���[�W���[�V�����p (SQShakeData)

ActionMsg <- "";				//�s�����b�Z�[�W�̓��e
ActingIndex <- -1;				//�s�����̃A�N�^�[�̃C���f�b�N�X

CriticalHitFlag <- false;		//�N���e�B�J���q�b�g���N�����Ƃ��ɗ��Ă���t���O
PTEscaped <- false;				//�p�[�e�B�������ɐ��������Ƃ��ɗ��Ă���t���O


//�퓬���ʃt�F�[�Y�ɕK�v�Ȃ���
Results <- {
	Money = 0,					//�l�����z
	EXP = 0,					//�l���o���l

	LevelUpName = "",			//���x���A�b�v�����L�����̖��O
	BeforeLevel = -1,			//���x���A�b�v����O�̃��x��
	AfterLevel = -1,			//���x���A�b�v������̃��x��
	LevelUpParameters = null,	//���x���A�b�v�ɂ��p�����[�^�[�ϓ����X�g (CIDValueData)
	LevelUpSkills = null,		//���x���A�b�v�ɂ��K���X�L�����X�g (CIDNameData)

	DropItems = null,			//�h���b�v�A�C�e�����X�g (CIDNameData)
};
