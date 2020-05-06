//###################################################################
//		�o�g���V�[��: �퓬�v�Z��
//###################################################################

///[summary]�A�N�^�[�̐��������擾
///[par]�A�N�^�[�̃C���f�b�N�X
///[return]������
function GetActorSuccessRate(idx) {
	//�A�N�^�[�̌ʕϐ������o��
	return GetActorData(idx).SelfValues.Get(0x3D4228D9/*������*/).Value;
}


///[summary]�����U���̌v�Z
///[par]�s���� (CDBActorData)
///[par]�Ώێ� (CDBActorData)
///[par]�U���������X�g (CIntegerArray)
///[par]��{�l
///[par]���U�x
///[par]������
///[par]�N���e�B�J���q�b�g�𔻒肷�邩�ǂ���
///[return]�_���[�W��
function CalcPhysicalAttack(actor, target, attrList, base, variance, successRate, enabledCritical) {
	local value = base;
	value += actor.GetParameter(0x48EB70FA/*�����U����*/).Value / 2;

	//�N���e�B�J������
	CriticalHitFlag = enabledCritical ? JudgeCritical() : false;
	if(CriticalHitFlag == true) {
		//�_���[�W�ʂ� 2�{ �ɂ��āA����̎���́E�h��𖳎�����
		value *= CriticalDamageRate;
	} else {
		value -= target.GetParameter(0x469C1AF5/*���������*/).Value / 4;

		//�����ϐ����l��
		local attrDefense = 100;
		for(local i = 0; i < attrList.Count(); i++) {
			attrDefense = attrDefense * target.GetAttributeResistance(attrList.Get(i)) / 100;
		}
		value = (value / attrDefense.tofloat() * 100.0).tointeger();

		//�h�䔻��
		if(IsDefensing(target) == true) {
			value /= 2;
		}
	}

	//���U
	value += Game.NextRand(0, value * variance / 100);

	// 0 �ȉ��̏ꍇ�� 0�`1 �ɏC������
	if(value <= 0) {
		value = Game.NextRand(0, 1);
	}

	//��������
	if(JudgeHit(actor, successRate) == false) {
		//��������
		value = 0;
	}
	return value;
}


///[summary]���@�U���̌v�Z
///[par]�s���� (CDBActorData)
///[par]�Ώێ� (CDBActorData)
///[par]�U���������X�g (CIntegerArray)
///[par]��{�l
///[par]���U�x
///[par]������
///[par]�N���e�B�J���q�b�g�𔻒肷�邩�ǂ���
///[return]�_���[�W��
function CalcMagicalAttack(actor, target, attrList, base, variance, successRate, enabledCritical) {
	local value = base;
	value += actor.GetParameter(0x236EB043/*���@�U����*/).Value / 2;

	//�N���e�B�J������
	CriticalHitFlag = enabledCritical ? JudgeCritical() : false;
	if(CriticalHitFlag == true) {
		//�_���[�W�ʂ� 2�{ �ɂ��āA����̎���́E�h��𖳎�����
		value *= CriticalDamageRate;
	} else {
		value -= target.GetParameter(0x99D0AC5/*���@�����*/).Value / 4;

		//�����ϐ����l��
		local attrDefense = 100;
		for(local i = 0; i < attrList.Count(); i++) {
			attrDefense = attrDefense * target.GetAttributeResistance(attrList.Get(i)) / 100;
		}
		value = (value / attrDefense.tofloat() * 100.0).tointeger();

		//�h�䔻��
		if(IsDefensing(target) == true) {
			value /= 2;
		}
	}

	//���U
	value += Game.NextRand(0, value * variance / 100);

	// 0 �ȉ��̏ꍇ�� 0�`1 �ɏC������
	if(value <= 0) {
		value = Game.NextRand(0, 1);
	}

	//��������
	if(JudgeHit(actor, successRate) == false) {
		//��������
		value = 0;
	}
	return value;
}


///[summary]�񕜂̌v�Z
///[par]�s���� (CDBActorData)
///[par]�Ώێ� (CDBActorData)
///[par]��{�l
///[par]���U�x
///[return]�񕜗�
function CalcHeal(actor, target, base, variance) {
	local value = base;
	value += Game.NextRand(0, value * variance / 100);
	return value;
}


///[summary]�h�䔻��
///[par]�ΏۃA�N�^�[ (CDBActorData)
///[return]�h�䂵�Ă��邩�ǂ���
function IsDefensing(actor) {
	return (actor.SelfValues.Get(0x7BD2A434/*�h�䒆���ǂ���*/).Value == TRUE);
}


///[summary]����������s��
///[comment]�w�肵���A�N�^�[�̍s���������������ǂ����𔻒肵�܂��B
///[par]�A�N�^�[�C���X�^���X
///[par]������
///[return]�����������ǂ���
function JudgeHit(actor, successRate) {
	//��������: �A�N�^�[���g�̐������ƁA���̍U���̐����������킹��
	if(Game.NextRand(1, 100) <= (100 - actor.SelfValues.Get(0x3D4228D9/*������*/).Value * successRate / 100)) {
		//��������
		return false;
	}
	return true;
}


///[summary]�N���e�B�J������
///[comment]�����I�ɃO���[�o���ϐ��̃t���O�ɂ����f����܂��B
///[return]�N���e�B�J���q�b�g�������������ǂ���
function JudgeCritical() {
	CriticalHitFlag = (Game.NextRand(1, 100) <= CriticalRate);
	return CriticalHitFlag;
}


///[summary]��������
///[par]������l��
///[return]�����ɐ����������ǂ���
function JudgeEscape(count) {
	local maxCount = 0;		//�����Ă��郁���o�[�̑���
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		if(IsDead(i) == false) {
			maxCount++;
		}
	}
	local oneRate = 100.0 / maxCount;
	return (Game.NextRand(1, 100) <= oneRate * count - EscapeMissRate / (maxCount - count + 1));
}

