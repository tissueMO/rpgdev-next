//###################################################################
//		�G�l�~�[ �X�N���v�g
//###################################################################

///[summary]�G�l�~�[�̍s���I��
///[par]�G�l�~�[ID
///[par]�s�����e(SQActionData)�̔z��
///[comment]�s�����e���X�g�ɂ͊���ň�����������Ă��܂��B�z��actions �̕ύX���ʂ��s�����e�Ƃ��ēo�^����܂��B�����s���ɂ���ꍇ��append���A�s�����Ȃ��ꍇ�͗v�f�����[���ɂ��܂��B
function SelectCommandEnemy(intID, actions) {
	local rand = Game.NextRand(1, 100);

	switch(intID) {
		case 0x1B5682FD/*�`���[�g���A����p�X���C��*/:
			actions[0].Command = MenuItem.Attack;
			actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			actions[0].EffectID = EmptyEquipEffectID;
			break;

		case 0x409C361F/*�X���C��*/:
			if(rand <= 15) {
				actions[0].Command = MenuItem.Escape;
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x1DC9F561/*�R�E����*/:
			actions[0].Command = MenuItem.Attack;
			actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			actions[0].EffectID = EmptyEquipEffectID;
			break;

		case 0x37B4A5F7/*���΂��L�m�R*/:
			if(rand <= 20) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x22A28BE5/*�X���[�v*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x50918D5E/*���g�����g*/:
			if(rand <= 20) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x365496DB/*�p�j�b�N*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x57B1F7CB/*���_��Y*/:
			if(rand <= 50 && TurnCount <= 3
			&& GetActorData(actions[0].ActorIndex).States.Exists(0x4C3A1FDA/*�p���[�A�b�v*/) == false) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x23C6D92A/*�p���[�v���X*/;
				actions[0].TargetIndex = actions[0].ActorIndex/*����*/;
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x286772AD/*�ЂƂڏ��m*/:
			if(TurnCount == 1) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4EB43EA/*���t�D��*/;
				//����m���p�[�e�B�ɂ���ꍇ�͏���m�D��
				local womanSoldierIndex = Game.DB.VDBParty.CheckCharInParty(0x1DA3A3F7/*����m*/);
				actions[0].TargetIndex = (womanSoldierIndex != -1) ? womanSoldierIndex : 0;
			} else if(rand <= 35) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*�t�@�C�A*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x2089C2F1/*�O���t�H��*/:
			if(rand <= 35) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x754A7AE6/*�Ή��u���X*/;
				actions[0].TargetIndex = -1;
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x10847557/*�l�N���}���T�[*/:
			if(rand <= 50 && TurnCount <= 3
			&& GetActorData(actions[0].ActorIndex).States.Exists(0x69CE82E8/*�f�B�t�F���X�A�b�v*/) == false) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4160663C/*�K�[�h�v���X*/;
				actions[0].TargetIndex = actions[0].ActorIndex/*����*/;
			} else if(rand <= 40) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*�t�@�C�A*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else if(rand <= 55) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x56D945DC/*�|�C�Y��*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else if(rand <= 70) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x2335FAE0/*�X���[�N*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else {
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x569A84D6/*�l�H����*/:
			if(rand <= 50 && TurnCount <= 3
			&& GetActorData(actions[0].ActorIndex).States.Exists(0x5D4CF03C/*�X�s�[�h�A�b�v*/) == false) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x510301FE/*�X�s�[�h�v���X*/;
				actions[0].TargetIndex = actions[0].ActorIndex/*����*/;
			} else {
				//���S2��s��
				//�s��1
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;

				//�s��2
				actions.append(SQActionData(actions[0].ActorIndex))
				actions[1].Command = MenuItem.Attack;
				actions[1].TargetIndex = SelectRandom(AttackSide.Party);
				actions[1].EffectID = EmptyEquipEffectID;
			}
			break;

		case 0x4F74D7F0/*�_�[�N���I�i���h*/:
			if(rand <= 20) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x754A7AE6/*�Ή��u���X*/;
				actions[0].TargetIndex = -1;
			} else if(rand <= 40) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*�t�@�C�A*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
			} else if(rand <= 55) {
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x141F1FBB/*�j�ł̗ӌ�*/;
				actions[0].TargetIndex = -1;
			} else {
				//�s��1
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Party);
				actions[0].EffectID = EmptyEquipEffectID;

				//�s��2
				actions.append(SQActionData(actions[0].ActorIndex))
				actions[1].Command = MenuItem.Attack;
				actions[1].TargetIndex = SelectRandom(AttackSide.Party);
				actions[1].EffectID = EmptyEquipEffectID;
			}
			break;
	}
}
