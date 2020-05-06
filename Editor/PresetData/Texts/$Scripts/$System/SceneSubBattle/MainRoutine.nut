//###################################################################
//		�o�g���V�[��: ���C�����[�`��
//###################################################################
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\PhaseAction.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\PhaseCommand.nut");
Game.SQInclude("Texts\\$Scripts\\$System\\SceneSubBattle\\PhaseEnding.nut");

///[comment]���͏�������Ă΂�郁�C�����[�`��
function BattleInput() {
	//�C�x���g����Ăяo�����퓬���l�����邽�߁A���Z�b�g�͋֎~����
	//if(Game.GetKeyInputCounter(KeyID.F12) == 1) {
	//	GameReset();
	//	return;
	//}

	switch(Phase) {
		case BattlePhase.Start:
			// NOTE: �搧�U���E�s�ӑł����̏����͂����ɑ}�����܂��B
			MoveToCommandPhase();
			break;

		case BattlePhase.Command:
			Routine_PhaseCommand();
			break;

		case BattlePhase.Action:
			Routine_PhaseAction();
			break;
	}
}


///[summary]�R�}���h�I���t�F�[�Y�Ɉڂ�
function MoveToCommandPhase() {
	Actions.resize(0);
	Cursor = array(DepthCount, 0);
	TopIndex = array(DepthCount, 0);
	Phase = BattlePhase.Command;

	//�ŏ��̃L�����̃R�}���h�I���Ɉڂ�
	TopIndex[0] = -1;
	PhaseCommandSubRoutine.MoveToNextActorCommand(null);
}


///[summary]�s���t�F�[�Y�Ɉڂ�
function MoveToActionPhase() {
	//�����퓬�̃p�[�e�B�L������AI���Ăяo���čs�����e�����肷��
	print("�p�[�e�BNPC�̍s�����e:");
	for(local i = 0; i < Game.DB.VDBParty.GetCharCount(); i++) {
		local actor = Game.DB.VDBActor.GetData(Game.DB.VDBParty.GetCharIDFromPartyIndex(i));
		if(IsForbiddenAction(i) == true) {
			continue;		//�s���ł��Ȃ����
		} else if(actor.IsNPC == false) {
			continue;		//�v���C���[�ɂ���ăR�}���h�I������Ă���
		}
		local actions = array(0);
		actions.append(SQActionData(i, -1, -1, -1, -1));
		SelectCommandChar(actor.ID, actions);
		if(actions.len() == 0) {
			continue;		//�s�����Ȃ����ƂɂȂ���
		}

		//�s������
		foreach(act in actions) {
			act.Print();
			Actions.append(act);
		}
	}

	//���j�b�g��AI���Ăяo���čs�����e�����肷��
	print("�G�l�~�[�̍s�����e:");
	foreach(idx, enemy in Enemies) {
		if(IsForbiddenAction(EnemyIndexOrigin + idx) == true) {
			continue;		//�s���ł��Ȃ����
		}

		local actions = array(0);
		actions.append(SQActionData(EnemyIndexOrigin + idx, -1, -1, -1, -1));
		SelectCommandEnemy(enemy.ID, actions);
		if(actions.len() == 0) {
			continue;		//�s�����Ȃ����ƂɂȂ���
		}

		//�s������
		foreach(act in actions) {
			act.Print();
			Actions.append(act);
		}
	}

	SortActions();				//�s�����Ԃ����肷��
	Phase = BattlePhase.Action;
	DoTurnStartingTasks();		//�^�[���J�n���̏�������s����
}


///[comment]�퓬�I���t�F�[�Y�Ɉڂ�
///[par]�퓬���� (BattleResult)
function MoveToEndingPhase(result) {
	Result = result;
	Game.Material.Sounds.Stop(HdlBGM, 0.5, false);
	Game.Wait(0.5, true, false);
	Game.Material.Sounds.Delete(HdlBGM);

	//�퓬���ʂɊ�Â��ăT�u���[�`���Ɉڍs����
	PhaseEndingSubRoutine[result]();
	if(result == BattleResult.Lose && DisabledGameOver == false) {
		return;		//�Q�[���I�[�o�[�Ȃ̂Ń^�C�g����ʂֈڍs����
	}

	//�퓬���̂ݗL���ȊԐڌ��ʂ����ׂĉ�������
	StateReleaseOnlyBattle();

	//�T�u�V�[�����I�����ă}�b�v�ɕ��A����
	switch(result) {
		case BattleResult.Win:
			print("�퓬����: ����");
			break;
		case BattleResult.Lose:
			print("�퓬����: �s�k");
			break;
		case BattleResult.Escape:
			print("�퓬����: ����");
			break;
		case BattleResult.Break:
			print("�퓬����: ���f");
			break;
	}
	Game.FadeOut(SquareSpeed.High);
	Game.EndSubScene();

	//����ł��܂����p�[�e�B�����o�[�����ׂđh������
	RecoverPartyAll();

	//�퓬�̌Ăяo�����Ńt�F�[�h�C������̂ł����ł͏������Ȃ�
	//Game.FadeIn(SquareSpeed.High);
}
