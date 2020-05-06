//###################################################################
//		�A�C�e�� �X�N���v�g
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\Common.nut");

///[summary]�A�C�e���g�p���̌��ʓ��e
///[par]�g�p����A�C�e����ID
///[par]�g�p�҃C���f�b�N�X
///[par]�Ώێ҃C���f�b�N�X
///[par]����Ȃ��悤�ɂ��邩�ǂ���
///[return]��������ǂ���
function UseItem(intID, intActorIdx, intTargetIdx, boolIsInfinity) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local stateID = db.GetCell(intID, "�Ԑڌ���").Integer;
	local value = db.GetCell(intID, "���ʗ�").Integers.Get(0);
	local variance = db.GetCell(intID, "���U�x").Integer;
	local successRate = db.GetCell(intID, "�ʕϐ�").IDValues.GetValue(0x48CA9CB8/*������*/);
	local attrList = db.GetCell(intID, "����").Integers;
	local targetType = db.GetCell(intID, "�Ώێ��").Integer;
	local used = true;
	if(db.GetCell(intID, "���Օi").Boolean == false) {
		//���Օi�łȂ��ꍇ�͖����Ɏg����
		boolIsInfinity = true;
	}

	//���ڌ��ʂɊ�Â��ď�������
	local useType = db.GetCell(intID, "���ڌ���").Integer;
	switch(useType) {
		case UseType.Script:
			return UseItemOthers(intID, intActorIdx, intTargetIdx, boolIsInfinity);
		default:
			used = TypicalUse(intID, intActorIdx, intTargetIdx, targetType, useType, stateID, value, variance, successRate, attrList);
			break;
	}
	return (boolIsInfinity ? false : used);
}


///[summary]�ʓI�Ȍ��ʓ��e
///[par]�g�p����A�C�e����ID
///[par]�g�p�҃C���f�b�N�X
///[par]�Ώێ҃C���f�b�N�X
///[par]����Ȃ��悤�ɂ��邩�ǂ���
///[return]��������ǂ���
function UseItemOthers(intID, intActorIdx, intTargetIdx, boolIsInfinity) {
	local db = Game.DB.GetDB(DBType.FDB, FDBIndices.Item).GetSubDB(0);
	local value = db.GetCell(intID, "���ʗ�").Integers.Get(0);
	local used = true;

	//ID���Ƃɏ������`����
	switch(intID) {
		// NOTE: �f�[�^�x�[�X�Œ��ڌ��ʂ��u�X�N���v�g�w��v�ɂ����ꍇ�͏������e�������ɒ�`���܂��B

		case 0x6F0C5D44/*���Ă̋P��*/:
			CloseMenuScene();
			local db = Game.DB.UDB.GetUserDB(0x2C80399D/*�ꏊ���X�g*/);
			Game.SceneMap.ChangeMap(
				db.GetCellFromIndex(intTargetIdx, db.GetColumnIndex("�}�b�v�t�@�C����")).String,
				db.GetCellFromIndex(intTargetIdx, db.GetColumnIndex("X")).Integer,
				db.GetCellFromIndex(intTargetIdx, db.GetColumnIndex("Y")).Integer,
				Direction4.South
			);
			break;

	}

	return (boolIsInfinity ? false : used);
}
