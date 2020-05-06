//###################################################################
//		�o�g���V�[��: �N���X��`
//###################################################################

//�A�N�^�[�s�����e
class SQActionData {
	ActorIndex = -1;		//�s���҃C���f�b�N�X
	TargetIndex = -1;		//�Ώێ҃C���f�b�N�X
	Command = -1;			//�R�}���h
	Option = 0;				//�R�}���h�ɕt������p�����[�^�[
	EffectID = -1;			//�G�t�F�N�gID
	Disabled = false;		//���������ꂽ�s�����ǂ���

	//�R���X�g���N�^�[
	constructor(actor, target = -1, command = -1, opt = -1, effectID = EmptyEquipEffectID) {
		this.ActorIndex = actor;
		this.TargetIndex = target;
		this.Command = command;
		this.Option = opt;
		this.EffectID = effectID;
		this.Disabled = false;
	}

	///[summary]������`�ŏo�͂���
	function Print() {
		print("\t" + this.ActorIndex
				 + " -> " + this.TargetIndex
				 + " / " + this.Command
				 + ":" + this.Option
				 + " / EffectID: " + this.EffectID
		);
	}
}

//�_���[�W���l�\��
class SQDamageEffectData {
	ActorIndex = -1;		//�ΏۃA�N�^�[�̃C���f�b�N�X
	Value = 0;				//�_���[�W��
	Type = -1;				//�\�����
	StartTimeMS = 0;		//�J�n�����~���b
	PositionY = 0;			//����Y���W

	//�R���X�g���N�^�[
	constructor(idx, val, effectType) {
		this.ActorIndex = idx;
		this.Value = val;
		this.Type = effectType;
		this.StartTimeMS = Game.DXLib.GetNowCount();
	}

	//�\�����
	DamageEffectType = {
		HPDamage = 0,		//HP�_���[�W
		HPHeal = 1,			//HP��
		MPDamage = 2,		//MP�_���[�W
		MPHeal = 3,			//MP��
		Critical = 4,		//�N���e�B�J���q�b�g
		Miss = 5,			//�~�X
		Reflect = 6,		//���t���N�g
	};
}

//�U�����
class SQShakeData {
	Doing = false;			//���s�����ǂ���
	PositionX = 0;			//����X���W
	PositionY = 0;			//����Y���W
	Strength = 0;			//����
	FrameCounter = 0;		//�t���[���J�E���^�[

	///[summary]�U�����J�n����
	///[par]�U���̋���
	function StartShake(strength) {
		this.Strength = strength;
		this.FrameCounter = 0;
		this.Doing = true;
	}

	///[summary]�U������������
	function DoShake() {
		this.FrameCounter++;
		this.PositionX = this.Strength * sin((PI / 180) * this.FrameCounter * 90/*�U���萔*/);
		this.PositionY = this.Strength * cos((PI / 180) * this.FrameCounter * 90/*�U���萔*/);
		if(this.FrameCounter % 4 == 0) {
			this.Strength--;		//����������
			if(this.Strength <= 1) {
				this.PositionX = 0;
				this.PositionY = 0;
				this.Doing = false;		//�I��
			}
		}
	}
}
