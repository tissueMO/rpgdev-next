//###################################################################
//		�X�L�� �X�N���v�g
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Common.nut");

///[summary]�X�L���g�p���̌��ʓ��e
///[par]�g�p����X�L����ID
///[par]�g�p�҃C���f�b�N�X
///[par]�Ώێ҃C���f�b�N�X
///[return]��������ǂ���
function UseSkill(intID, intActorIdx, intTargetIdx) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	local stateID = db.GetCell(intID, "�Ԑڌ���").Integer;
	local value = db.GetCell(intID, "���ʗ�").Integers.Get(0);
	local variance = db.GetCell(intID, "���U�x").Integer;
	local successRate = db.GetCell(intID, "�ʕϐ�").IDValues.GetValue(0x48CA9CB8/*������*/);
	local attrList = db.GetCell(intID, "����").Integers;
	local targetType = db.GetCell(intID, "�Ώێ��").Integer;
	local used = true;

	//���ڌ��ʂɊ�Â��ď�������
	local useType = db.GetCell(intID, "���ڌ���").Integer;
	switch(useType) {
		case UseType.Script:
			return UseSkillOthers(intID, intActorIdx, intTargetIdx);
		default:
			used = TypicalUse(intID, intActorIdx, intTargetIdx, targetType, useType, stateID, value, variance, successRate, attrList);
			break;
	}
	return used;
}


///[summary]�ʓI�Ȍ��ʓ��e
///[par]�g�p����X�L����ID
///[par]�g�p�҃C���f�b�N�X
///[par]�Ώێ҃C���f�b�N�X
///[return]��������ǂ���
function UseSkillOthers(intID, intActorIdx, intTargetIdx) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Skill).GetSubDB(0);
	local value = db.GetCell(intID, "���ʗ�").Integers.Get(0);
	local used = true;

	//ID���Ƃɏ������`����
	switch(intID) {
		// NOTE: �f�[�^�x�[�X�Œ��ڌ��ʂ��u�X�N���v�g�w��v�ɂ����ꍇ�͏������e�������ɒ�`���܂��B

	}

	return used;
}
