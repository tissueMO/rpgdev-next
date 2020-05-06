//###################################################################
//		���j�b�g �X�N���v�g
//###################################################################

///[summary]�o�g���C�x���g�����s����
///[par]���j�b�gID
///[par]�o�g���C�x���g�̎�� (BattleEventType)
///[par]�s�����e (�s���Ɋւ��Ȃ��C�x���g�� null ���^�����܂�)
function UnitEvent(intUnitID, eventType, actData) {
	switch(intUnitID) {
		case 0x25206B4F/*�`���[�g���A��*/:
			switch(eventType) {
				case BattleEventType.StartBattle:
					Game.SceneMap.ShowMessage(-1/*�Ȃ�*/, "", "�퓬�̃`���[�g���A���ł��B\f�܂��̓L�����̍s�������肵�܂��B\n�����ł́u�U���v��I��ł݂܂��傤�B");
					break;

				case BattleEventType.StartTurn:
					if(UnitSelfValues.Get(0xC46E1C6/*�ėpA*/).Value == 0) {
						Game.SceneMap.ShowMessage(-1/*�Ȃ�*/, "", "�R�}���h��I�����I����ƁA���ۂɍs�����n�܂�܂��B\n�p�[�e�B/�G�l�~�[���킸�f�����̑����L�������ɍs�����܂��B");
						UnitSelfValues.Get(0xC46E1C6/*�ėpA*/).Value = 1;
					}
					break;

				case BattleEventType.AfterAction:
					if(GetAttackSide(actData.TargetIndex) == AttackSide.Party
					&& UnitSelfValues.Get(0xC46E1C6/*�ėpA*/).Value == 1) {
						Game.SceneMap.ShowMessage(-1/*�Ȃ�*/, "", "�_���[�W���󂯂Ă��܂��܂����B\f" + HPName + "���[���ɂȂ�Ǝ��S�����ƂȂ�A\n�s��������񕜂������肷�邱�Ƃ��ł��Ȃ��Ȃ�܂��B");
						UnitSelfValues.Get(0xC46E1C6/*�ėpA*/).Value = 2;
					}
					break;

				case BattleEventType.EndingBattleWin:
					Game.SceneMap.ShowMessage(-1/*�Ȃ�*/, "", "���߂łƂ��������܂��I\n�퓬�ɏ������܂����B\f�|�����G�l�~�[����o���l�Ƃ����𓾂܂��B\n���x���A�b�v��X�L���K���A�E���A�C�e���̔�����s���܂��B");
					break;

				case BattleEventType.EndingBattleLose:
					Game.SceneMap.ShowMessage(-1/*�Ȃ�*/, "", "�c�O�c�B�퓬�ɔs�k���Ă��܂��܂����B\n�ʏ�A�Q�[���I�[�o�[�ɂȂ�ƃ^�C�g���V�[���ɖ߂�܂��B\f�C�x���g�퓬�ł͐ݒ�ɂ����\n�����I�ɃV�i���I��i�s�����邱�Ƃ��ł��܂��B\f�v���Z�b�g�X�N���v�g�̉�������ł�\nHP=1 �ŏ�Ƀp�[�e�B�𕜊������邱�Ƃ��ł��܂��B");
					break;

				case BattleEventType.EndingBattleEscape:
					Game.SceneMap.ShowMessage(-1/*�Ȃ�*/, "", "�퓬���瓦���o�����Ƃɐ������܂����B\n�r���܂œ|�����G�l�~�[�̌o���l�₨���͈�ؓ���ł��܂���B");
					break;
			}
			break;
	}
}
