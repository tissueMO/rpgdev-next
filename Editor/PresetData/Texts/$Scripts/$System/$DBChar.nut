//###################################################################
//		�p�[�e�B�L���� �X�N���v�g
//###################################################################

///[summary]�����퓬���L���ȃL�����̍s���I��
///[par]�p�[�e�B�L����ID
///[par]�s�����e(SQActionData)�̔z��
///[comment]�s�����e���X�g�ɂ͊���ň�����������Ă��܂��B�z��actions �̕ύX���ʂ��s�����e�Ƃ��ēo�^����܂��B�����s���ɂ���ꍇ��append���A�s�����Ȃ��ꍇ�͗v�f�����[���ɂ��܂��B
function SelectCommandChar(intID, actions) {
	local me = Game.DB.VDBActor.GetData(intID);
	local rand = Game.NextRand(1, 100);

	switch(intID) {
		case 0x1DA3A3F7/*����m*/:
			local leoActorIndex = Game.DB.VDBParty.CheckCharInParty(0x27264F10/*���I�i���h*/);
			local containsLeoInParty = (leoActorIndex != -1);
			local leo = Game.DB.VDBActor.GetData(0x27264F10/*���I�i���h*/);
			local skillDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
			local itemDB = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
			local isBossMode = (Enemies.len() == 1 && GetActorData(EnemyIndexOrigin).GetParameter(0x2019EC10/*�ő�g�o*/).Value >= 150);

			if(containsLeoInParty
			&& IsDead(leoActorIndex) == false
			&& leo.SelfValues.Get(0x7BAC0D68/*HP*/).Value <= leo.GetParameter(0x2019EC10/*�ő�g�o*/).Value * 0.5
			&& skillDB.GetCell(0x4D07D04B/*�q�[��*/, "�R�X�g").Integers.Get(0) <= me.SelfValues.Get(0x46A20EE9/*MP*/).Value
			&& rand <= 100) {
				// ���I�i���h��
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4D07D04B/*�q�[��*/;
				actions[0].TargetIndex = leoActorIndex;
			} else if(me.SelfValues.Get(0x7BAC0D68/*HP*/).Value <= me.GetParameter(0x2019EC10/*�ő�g�o*/).Value * 0.5
			&& skillDB.GetCell(0x4D07D04B/*�q�[��*/, "�R�X�g").Integers.Get(0) <= me.SelfValues.Get(0x46A20EE9/*MP*/).Value
			&& rand <= 75) {
				// ����m��
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4D07D04B/*�q�[��*/;
				actions[0].TargetIndex = Game.DB.VDBParty.CheckCharInParty(intID);
			} else if(containsLeoInParty
			&& IsDead(leoActorIndex) == false
			&& isBossMode
			&& TurnCount <= 5
			&& leo.States.Exists(0x69CE82E8/*�f�B�t�F���X�A�b�v*/) == false
			&& rand <= 50) {
				// ���I�i���h�K�[�h�v���X
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x4160663C/*�K�[�h�v���X*/;
				actions[0].TargetIndex = leoActorIndex;
			} else if(containsLeoInParty
			&& IsDead(leoActorIndex) == false
			&& isBossMode
			&& TurnCount <= 5
			&& leo.States.Exists(0x4C3A1FDA/*�p���[�A�b�v*/) == false
			&& rand <= 50) {
				// ���I�i���h�p���[�v���X
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x23C6D92A/*�p���[�v���X*/;
				actions[0].TargetIndex = leoActorIndex;
			} else if(rand <= 25
			&& skillDB.GetCell(0x7E5A58BF/*�t�@�C�A*/, "�R�X�g").Integers.Get(0) <= me.SelfValues.Get(0x46A20EE9/*MP*/).Value) {
				// �U�����@
				actions[0].Command = MenuItem.Skill;
				actions[0].Option = 0x7E5A58BF/*�t�@�C�A*/;
				actions[0].TargetIndex = SelectRandom(AttackSide.Unit);
			} else {
				// �ʏ�U��
				actions[0].Command = MenuItem.Attack;
				actions[0].TargetIndex = SelectRandom(AttackSide.Unit);
				if(me.GetEquipIDFromItemTypeID(0x12414D21/*����*/) != -1) {
					actions[0].EffectID = itemDB.GetCell(me.GetEquipIDFromItemTypeID(0x12414D21/*����*/), "�G�t�F�N�g").Integer;
				} else {
					actions[0].EffectID = EmptyEquipEffectID;
				}
			}

			break;
	}
}
