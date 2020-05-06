//###################################################################
//		�X�N���v�g���ʒ萔���`���܂��B
//###################################################################

//��{�V�X�e��
const MsgFontID = 0x2ABC7058/*���b�Z�[�W�t�H���g*/;		//���b�Z�[�W�t�H���g
const SubtitleFontID = 0x7F6CF4B3/*�e���b�v�t�H���g*/;		//�e���b�v�t�H���g
const MarqueeMinimum = -160;						//NowLoading: �}�[�L�[�ŏ��l
const MarqueeMaximum = 400;							//NowLoading: �}�[�L�[�ő�l

const IsFixedSizedFace = 0;				//���b�Z�[�W: �t�F�C�X�O���t�B�b�N���o�X�g�A�b�v�ł͂Ȃ��A�E�B���h�E���Ɏ��߂�`���̏ꍇ�� 1 �ɂ��ĉ������B
const MsgFaceStandardWidth = 190;		//���b�Z�[�W: �t�F�C�X�O���t�B�b�N�̑z�蕝: ���̕��������b�Z�[�W���������ɂ���܂��B
const MsgTextOffset = 2;				//���b�Z�[�W: �e�L�X�g�̌Œ�I�t�Z�b�g: ���̕��������b�Z�[�W���������ɂ���܂��B
const MsgWndHeight = 128;				//���b�Z�[�W: ���b�Z�[�W�E�B���h�E�̍���
const MsgNameWidth = 200;				//���b�Z�[�W: ���O�E�B���h�E�̕�
const MsgNameHeight = 36;				//���b�Z�[�W: ���O�E�B���h�E�̍���

const ChoiceWndMargin = 100;			//�I����: �E�B���h�E�̍��E�]��

const SubtitleLineHeight = 25;			//�e���b�v: �P�s������̍���


//�f�o�b�O�p
const ItemCountForTester = 5;		//�e�X�^�[��p: ����������A�C�e����


//�}�b�v�֘A
const IntoMenuSEID = 0x61301713/*���艹*/;	//���j���[��ʂ��J���Ƃ��̉�
const NormalEncounterStepCount = 40;		//�G���J�E���g�W��=100 �̂Ƃ��̖ڕW����
const MapNamePanelTime = 5000;				//�}�b�v���\���p�l���̕\�����ԃ~���b
const MapNameFadeTime = 500;				//�}�b�v���\���p�l���̃t�F�[�h���ԃ~���b
const LadderTileGroupNum = 99;				//��q�����̃^�C���O���[�v�ԍ�


//���j���[�֘A
const EnabledIcons = 1;				//�A�C�e���ƃX�L���ɃA�C�R����\�������邩�ǂ���


//�V���b�v�֘A
const SellRate = 0.5;				//���p���̉��i������


//�o�g���֘A
const EnemyIndexOrigin = 100;		//���j�b�g���̃A�N�^�[�C���f�b�N�X�̊J�n�ԍ�
const CriticalDamageRate = 2.0;		//�N���e�B�J���q�b�g���̃_���[�W�{��
const CriticalRate = 3;				//�N���e�B�J���q�b�g������
const SpeedVarianceRate = 20;		//�s�����Ԃōl�������q�����̕��U�x
const EscapeMissRate = 5;			//�S���œ������Ƃ��̎��s�m��
const EmptyEquipEffectID = 0x2C31DE4D/*�f��U��*/;		//�f��U���̃G�t�F�N�gID
const PluralString = "����";			//�擪�Җ��ɑ��������`


//�A�N�^�[�֘A
const MaxBaseParameter = 999;		//��{�p�����[�^�[(�p�����[�^�[DB)�̍ő�l
const MaxExParameter = 9999;		//�g���p�����[�^�[(�ʕϐ�)�̍ő�l
const HPMeterPatternStart = 4;		//HP���[�^�[�̃p�^�[���J�n�C���f�b�N�X
const HPMeterPatternCount = 10;		//HP���[�^�[�̃p�^�[����
const MPMeterPattern = 3;			//MP���[�^�[�̃p�^�[���C���f�b�N�X
const StateIconMaxCount = 3;		//�Ԑڌ��ʃA�C�R���̕\�������
const StateIconSize = 16;			//�Ԑڌ��ʃA�C�R���̎��ۂ̕\���T�C�Y
const UseIconSize = 16;				//�A�C�e���E�X�L���A�C�R���̎��ۂ̕\���T�C�Y


//�g���p�����[�^�[
enum ExParameter {
	HP,
	MP,
};


//�C�x���g�g���ړ��̎��
enum EVMoveExType {
	Jump,				//�W�����v
	Oni,				//�S�������S���[�h
};


//�X�v���C�g���C�v���
enum SpriteWipeType {
	Fade,				//�t�F�[�h
	SlideRight,			//�X���C�h�E�E
	SlideLeft,			//�X���C�h�E��
	SlideDown,			//�X���C�h�E��
	SlideUp,			//�X���C�h�E��
	Transition,			//�����摜�𗘗p
};


//�o�g���C�x���g���
enum BattleEventType {
	StartBattle,		//�퓬�J�n��
	StartTurn,			//�^�[���J�n��
	BeforeAction,		//�s���O
	AfterAction,		//�s����
	EndingTurn,			//�^�[���I����
	EndingBattleWin,	//�퓬������
	EndingBattleLose,	//�퓬�s�k��
	EndingBattleEscape,	//�퓬������
};


//�퓬����
enum BattleResult  {
	Win,				//����
	Lose,				//�s�k
	Escape,				//����
	Break,				//���f
};


//�I��������
enum YesNoResult {
	Yes,				//�u�͂��v
	No,					//�u�������v
	Cancel = -1,		//�u�L�����Z���v
};


//����f��DB��ID
enum DefaultMaterialID {
	WipeBlendGraph,		//���C�v�p�����摜
	TitleGraph,			//�^�C�g����ʃO���t�B�b�N
	TitleBGM,			//�^�C�g�����BGM
	BattleTestGraph,	//�o�g���e�X�g�p�w�i�O���t�B�b�N
	BattleBGM,			//�ʏ�퓬BGM
	BattleStartWipeGraph,		//�퓬�J�n���̃��C�v�摜
	BattleEndingWipeGraph,		//�퓬�������̃��C�v�摜
	GameOverBGM,		//�Q�[���I�[�o�[BGM
};


//�U���T�C�h
enum AttackSide {
	Party,				//�p�[�e�B
	Unit,				//���j�b�g
};
