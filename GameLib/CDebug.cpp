//###################################################################
//		�f�o�b�O�p�R�[�h
//###################################################################
#include "Header.h"
#include <conio.h>
extern char buf[TS::System::BufferLengthMax];


//####################  �X�y�[�X�L�[�ŃQ�[�����~����������ɍs������  ####################
//�Ԓl�̓X�N���v�g�̃X�e�b�v���s�Ɉڍs���邩�ǂ���
bool CDebug::StartDebugConsole() {
	//::clsConsole();
	CDebug::ActivateConsole();
	TS_LOG("�X�N���v�g�f�o�b�K�[���N�����܂����B\n\t�f�o�b�O�R���\�[���̕���{�^���������ƁA�Q�[�����̂��I������̂Œ��ӂ��ĉ������B\n\t�f�o�b�O�R���\�[����ŃR�}���h�����͂ł��܂��B�R�}���h�̃w���v�� h �ŕ\���ł��܂��B");
	Game.NoTimeStamp = true;

	while(!Game.SQ.KillDebuggerFlag) {
		//�R�}���h���͂��󂯕t����
		::printf(">");
		::fflush(stdin);
		::fgets(::buf, sizeof(::buf) - 1, stdin);
		::buf[strlen(::buf) - 1] = '\0';

		//�O���󔒂�����
		int start = 0;
		string temp = ::buf;
		if(!temp.empty()) {
			TS_COUNTLOOP(i, temp.size()) {
				if(temp.substr(i, 1) != "") {
					start = i;
					break;
				}
			}
			temp = temp.substr(start);
		}

		//�p�����[�^�[����
		vector<string> spl;
		Game.SplitString(spl, temp, " ");

		//####################  �R�}���h����  ####################
		if(spl.size() == 0 || spl[0] == "end" || spl[0] == "exit") {
			//�f�o�b�O���I�����ăQ�[���ɖ߂�
			DxLib::WaitTimer(500);		//Enter�L�[���Q�[����Ŕ��f����Ȃ����߂ɏ����҂�
			break;
		} else if(spl[0] == "step") {
			//�X�e�b�v���s: ���̃X�N���v�g�R�[�h�����s�����Ƃ�����f�o�b�K�[�����삷��
			Game.NoTimeStamp = false;
			return true;
		} else if(spl[0] == "cls") {
			CDebug::ClsConsole();
			continue;
		} else if(spl[0] == "frs") {
			if(spl.size() == 2) {
				Game.FPSKeeper.setSkipFrameCount(atoi(spl[1].c_str()));
			} else {
				::puts("cmdError: �����̌����s���ł��B");
			}
			continue;
		} else if(CDebug::cmdHelp(temp, spl)) {
			continue;
		} else if(CDebug::cmdSystemList(temp, spl)) {
			continue;
		} else if(CDebug::cmdDB(temp, spl)) {
			continue;
		} else if(CDebug::cmdMap(temp, spl)) {
			continue;
		} else if(CDebug::cmdSQ(temp, spl)) {
			continue;
		} else {
			//��`����Ȃ��R�}���h�̓X�N���v�g�ɗ���
			if(Game.SQ.DoFunc<bool>(TS::SQ::SQEvent_DebugCommand, temp)) {
				continue;
			}
		}

		//��`����Ă��Ȃ��R�}���h
		::printf("cmdError: ��`����Ă��Ȃ��f�o�b�O�R�}���h�ł��B\n");
		//::pauseConsole();
		//::clsConsole();
	}

	*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) = 2;		//�A�����ăf�o�b�K�[���N������̂��������
	CDebug::ActivateGameWindow();
	Game.NoTimeStamp = false;
	TS_LOG("�X�N���v�g�f�o�b�K�[���I�����܂����B");
	return false;
}


//####################  �R�}���h�����F�w���v  ####################
bool CDebug::cmdHelp(CMDARGS) {
	if(spl[0] == "h" || spl[0] == "help") {
		::printf(
			"���͕��@:\n"
			"	�E�ŏ��ɃR�}���h������͂��A�p�����[�^�[���K�v�ȏꍇ�̓X�y�[�X�ŋ�؂��đ����܂��B\n"
			"	�E�t�@�C�����ȊO�ɑS�p�����������Ȃ��悤�����Ӊ������B\n"
			"	�E�t�@�C�����ɂ̓v���W�F�N�g�t�H���_�[����_�Ƃ������΃p�X�Ŏw�肵�܂����A���̒��ɃX�y�[�X���܂߂Ă͂����܂���B\n"
			"	�EON/OFF�������l�� 1/0 �œ��͂��܂��B\n"
			"���ӎ���:\n"
			"	�E����{�^���������ƁA�Q�[���v���O�������̂��I�����܂��B\n"
			"	�E[ID]�Ƃ���ӏ��́A�����ł̓��[�U�[�����R�Ɋ��蓖�Ă���d���\�ȉ�ID�̂��Ƃ��w���܂��B\n"
			"	�@�X�N���v�g�Ŏg����V�X�e���ŗL��ID�Ƃ͈قȂ�܂��̂ł����Ӊ������B\n"
			"\n"
			"������ �f�o�b�O�R���\�[������ ������\n"
			"	�f�o�b�O���I��:			exit / end / (��) �̂����ꂩ\n"
			"	�o�b�t�@���N���A		cls\n"
			"\n"
			"������ �V�X�e������ ������\n"
			"	�Œ�t���[�����[�g�ύX		frs [�l]\n"		//Frame Skip
			"\n"
			"������ �V�X�e������ ������\n"
			"	�O���t�B�b�N�f��:		lgrh\n"		//List Graphics
			"	�T�E���h�f��:			lsnd\n"		//List Sounds
			"	�t�H���g�f��:			lfnt\n"		//List Fonts
			"	�Œ�f�[�^�x�[�X:		lfdb\n"		//List Fixed Database
			"	�σf�[�^�x�[�X:		lvdb\n"		//List Variable Database
			"	���[�U�[�f�[�^�x�[�X:		ludb\n"		//List User Database
			"	�X�v���C�g:			lsp\n"		//List Sprite
			"	�X�N���v�g:			lsq\n"		//List SQscript
			"	�^���X���b�h:			ltd\n"		//List Thread
			"	�}�b�v�C�x���g:			lev\n"		//List Event
			"	�}�b�v�C�x���g�ʕϐ�:		levsv\n"	//List Event SelfValue
			"\n"
			"������ ��/���[�U�[ �f�[�^�x�[�X���� ������\n"
			"	���ׂčēǍ�:			rdb\n"		//Reload Database
			"	�Œ�DB�ēǍ�:			rfdb\n"		//Reload Fixed-Database
			"	��DB�ēǍ�:			rvdb\n"		//Reload Variable-Database
			"	���[�U�[DB�ēǍ�:		rudb\n"		//Reload User-Database
			"	���ږ���ID������:		fid [���ږ�]\n"		//Find fixedDB ID
			"		���Œ�DB����FixedID/VisibleID�𕔕���v�Ō������܂��B\n"
			"	��DB-�Z���l�ݒ�:		svdb [DB�C���f�b�N�X] [DB�T�u�C���f�b�N�X] [�s�ԍ�] [��ԍ�] [�l]\n"		//Set Variable-Database-Cell
			"	��DB-�Z��������:		ivdb [DB�C���f�b�N�X] [DB�T�u�C���f�b�N�X] [�s�ԍ�] [��ԍ�]\n"			//Init Variable-Database-Cell
			"		����DB�C���f�b�N�X:		0:���ʕϐ�  1:�p�[�e�B\n"
			"		��DB�T�u�C���f�b�N�X:		�G�f�B�^�[��̕\�ɂ��āA������O�Ƃ��ĉE�����ɐ������C���f�b�N�X\n"
			"		���s�ԍ��A��ԍ�:		��ԍŏ����O�Ƃ��Đ����܂��B\n"
			"	���[�U�[DB-�Z���l�ݒ�:		sudb [UserDBID] [�s�ԍ�] [��ԍ�] [�l]\n"		//Set User-Database-Cell
			"	���[�U�[DB-�Z��������:		iudb [UserDBID] [�s�ԍ�] [��ԍ�]\n"			//Init User-Database-Cell
			"		���s�ԍ��A��ԍ�:		��ԍŏ����O�Ƃ��Đ����܂��B\n"
			"	���ʕϐ��̒l ��:		lvl\n"		//List Value
			"	���ʕϐ��̒l �ύX:		svl [f/i/s] [ID] [�l]\n"		//Set Value
			"		��f=�t���O  i=����  s=������\n"
			"	�������ύX:			mny [���Z�l]\n"		//Money
			"		�����炵�����Ƃ��� [���Z�l] �}�C�i�X�̒l�����܂��B\n"
			"	�A�C�e������:			itm [�A�C�e��ID] [��]\n"		//Item
			"		�����炵�����Ƃ��� [��] �Ƀ}�C�i�X�̒l�����܂��B\n"
			"	�p�[�e�B�L����-����:	lchr ([�p�[�e�B�L����ID])\n"		//List Char
			"	�p�[�e�B�L����-�����i �ύX:	seqp [�p�[�e�B�L����ID] [��������ID] [�A�C�e��ID]\n"		//Set Equip
			"		���������O�������Ƃ��� [�A�C�e��ID] �� -1 �����܂��B\n"
			"	�p�[�e�B�L����-�N���X �ݒ�:	scls [�p�[�e�B�L����ID] [�N���XID]\n"		//Set Class
			"	�p�[�e�B�L����-�N���X ����:	rcls [�p�[�e�B�L����ID] [�N���XID]\n"		//Release Class
			"	�p�[�e�B�L����-�X�L�� �ݒ�:	sskl [�p�[�e�B�L����ID] [�X�L��ID]\n"		//Set Skill
			"	�p�[�e�B�L����-�X�L�� ����:	rskl [�p�[�e�B�L����ID] [�X�L��ID]\n"		//Release Skill
			"	�p�[�e�B�L����-�Ԑڌ��� �ݒ�:	sstt [�p�[�e�B�L����ID] [�Ԑڌ���ID]\n"		//Set State
			"	�p�[�e�B�L����-�Ԑڌ��� ����:	rstt [�p�[�e�B�L����ID] [�Ԑڌ���ID]\n"		//Release State
			"	�p�[�e�B�L����-�Ԑڌ��� �S����:			rsttall [�p�[�e�B�L����ID]\n"		//Release State All
			"	�p�[�e�B�L����-�Ԑڌ��� �o�X�e�S����:	rsttbad [�p�[�e�B�L����ID]\n"		//Release State only Bad
			"	�p�[�e�B�L����-�p�����[�^�[ �ݒ�:		spar [�p�[�e�B�L����ID] [�p�����[�^�[ID] [�l]\n"	//Set Parameter
			"	�p�[�e�B�L����-�ʕϐ� �ݒ�:			sval [�p�[�e�B�L����ID] [�A�N�^�[�ʕϐ�ID] [�l]\n"	//Set selfValue
			"	�p�[�e�B����:		lpt\n"	//List Party
			"	�p�[�e�B�ɒǉ�:			apt [�p�[�e�B�L����ID]\n"		//Add To Party
			"	�p�[�e�B���痣�E(ID):		dptid [�p�[�e�B�L����ID]\n"
			"	�p�[�e�B���痣�E(Index):	dptidx [�p�[�e�B���шʒu�C���f�b�N�X]\n"
			"		�����шʒu�C���f�b�N�X: �擪���O�Ƃ��Đ����܂��B\n"
			"\n"
			"������ �}�b�v�V�[������ ������\n"
			"	�}�b�v�ύX:			map [�t�@�C����] [X���W] [Y���W]\n"
			"	�C�x���g�ʒu�ړ�:		pos [�C�x���gID] [X���W] [Y���W]\n"		//Position
			"		��[�C�x���gID] �� -1 ���w�肷��ƁA��l�����ΏۂɂȂ�܂��B\n"
			"	�C�x���g����:			sev [�C�x���gID] [�p�����[�^�[�ԍ�] [�l]\n"		//Set Event
			"		��[�C�x���gID] �� -1 ���w�肷��ƁA��l�����ΏۂɂȂ�܂��B\n"
			"		���p�����[�^�[�ԍ�: 0:�N������  1:����ړ�  2:����  3:���x  4:�p�x  5:�Î~�A�j��  6:���s�A�j��  7:���蔲��  8:�����Œ�\n"
			"		���l: �G�f�B�^�[��̑I�����X�g�̍��ڂ��ォ��O�Ƃ��Đ������l�ɑΉ����܂��B\n"
			"	�C�x���g���s:			dev [�C�x���gID]\n"		//Do Event
			"\n"
			"������ �X�N���v�g���� ������\n"
			"	�X�e�b�v���s:			step\n"
			"	�X�N���v�g�t�@�C�����s:		sqf [�t�@�C����]\n"
			"	�X�N���v�g�R�[�h���Ȏ��s:	sqc [�R�[�h]\n"
			"		�������Ŏ��s����R�[�h�ŕs����������ꍇ�A\n"
			"		�@�f�o�b�O�I����ɐ��퓮�삵�Ȃ��Ȃ�댯��������܂��̂ł����Ӊ������B\n"
			"\n"
			"������ �X�N���v�g�ɂ��g���R�}���h ������\n"
			);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DebugCommand, "h");
		::printf("\n");
		return true;
	}
	return false;
}


//####################  �R�}���h�����F�V�X�e������  ####################
bool CDebug::cmdSystemList(CMDARGS) {
	if(spl[0] == "lgrh") {
		//�O���t�B�b�N�f��
		for(auto item : Game.Material.getGraphics()->Data.Items) {
			TS_LOG("%d: %s (�Q�Ɛ�=%d)", item.second.ID, item.second.Name.c_str(), item.second.Value);
		}
		return true;
	} else if(spl[0] == "lsnd") {
		//�T�E���h�f��
		for(auto item : Game.Material.getSounds()->Data.Items) {
			TS_LOG("%d: %s (%s, Loop=%d~%d)", item.second.ID, item.second.Name.c_str(), item.second.IsMIDI ? "MIDI" : "Wave", item.second.StartLoopPos, item.second.EndLoopPos);
		}
		return true;
	} else if(spl[0] == "lfnt") {
		//�t�H���g�f��
		for(auto item : Game.Material.getFonts()->Data.Items) {
			TS_LOG("%d: %s (�T�C�Y=%d, ����=%d, �s��=%d, ������=%d, �`��=%d, ���F=%d)", item.second.ID, item.second.FontName.c_str(), item.second.Size, item.second.Thick, item.second.LineSpace, item.second.Space, item.second.Type, item.second.EdgeColor);
		}
		return true;
	} else if(spl[0] == "lfdb") {
		//�Œ�DB
		for(auto db : Game.DB.FDB) {
			db->PrintData();
		}
		return true;
	} else if(spl[0] == "lvdb") {
		//��DB
		for(auto db : Game.DB.VDB) {
			db->PrintData();
		}
		return true;
	} else if(spl[0] == "ludb") {
		//���[�U�[DB
		Game.DB.UDB.PrintData();
		return true;
	} else if(spl[0] == "lsp") {
		//�X�v���C�g
		TS_LOG("�X�v���C�g�ꗗ:");
		for(auto& sprite : Game.Sprite.Data.Items) {
			buf.clear();
			buf = std::to_string(sprite.ID) + ": " + sprite.Name;
			buf += Game.Format("    (%d, %d), Hide=%d, Wipe=%d", sprite.Location.X, sprite.Location.Y, sprite.Hide, static_cast<int>(sprite.WipeType));
			TS_LOG(buf);

			//�O���t�B�b�N���
			for(auto& g : sprite.Graphics.Items) {
				TS_LOG("\t" + std::to_string(g.ID) + ": " + g.Name);
			}

			//�e�L�X�g���
			for(auto& t : sprite.Texts.Items) {
				TS_LOG("\t" + std::to_string(t.ID) + ": " + Game.Replace(t.Text, "\n", "[���s]"));
			}
		}
		return true;
	} else if(spl[0] == "lsq") {
		//�X�N���v�g
		Game.SQ.PrintLog();
		return true;
	} else if(spl[0] == "ltd") {
		//MT�֐�
		Game.MT.PrintLog();
		return true;
	} else if(spl[0] == "lev") {
		//�}�b�v�C�x���g
		if(Game.GetCurrentMainSceneID() == static_cast<int>(TS::System::Scene::Map)) {
			Game.getSceneMap()->PrintEVs();
		} else {
			::puts("���݃}�b�v�V�[���ł͂���܂���B");
		}
		return true;
	} else if(spl[0] == "levsv") {
		//�}�b�v�C�x���g�ʕϐ�
		if(Game.GetCurrentMainSceneID() == static_cast<int>(TS::System::Scene::Map)) {
			Game.getSceneMap()->PrintSelfValueData();
		} else {
			::puts("���݃}�b�v�V�[���ł͂���܂���B");
		}
		return true;
	}
	return false;
}


//####################  �R�}���h�����F�f�[�^�x�[�X  ####################
bool CDebug::cmdDB(CMDARGS) {
	//�p�����[�^�[�𐮐��l�ɕϊ��������X�g�����
	string temp;
	vector<int> ints;
	for(auto& arg : spl) {
		ints.push_back(atoi(arg.c_str()));
	}

	if(spl[0] == "rdb") {
		//���ׂčēǍ�
		Game.DB.LoadAll();
		return true;
	} else if(spl[0] == "rfdb") {
		//�Œ�DB�ēǍ�
		Game.DB.ReloadDB(static_cast<int>(TS::DB::DBType::FDB), true);
		return true;
	} else if(spl[0] == "rvdb") {
		//��DB�ēǍ�
		Game.DB.ReloadDB(static_cast<int>(TS::DB::DBType::VDB), true);
		return true;
	} else if(spl[0] == "rudb") {
		//���[�U�[DB�ēǍ�
		Game.DB.ReloadDB(static_cast<int>(TS::DB::DBType::UDB), true);
		return true;
	} else if(spl[0] == "fid") {
		//�Œ�f�[�^�x�[�X�̍��ږ�����FixedID/VisibleID�𕔕���v�Ō���
		bool found = false;
		if(spl.size() == 2) {
			for(auto& fdb : Game.DB.FDB) {
				TS_COUNTLOOP(i, fdb->GetSubDBCount()) {
					auto subdb = fdb->GetSubDB(i);
					TS_COUNTLOOP(row, subdb->GetRowCount()) {
						if(subdb->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR()->find(spl[1]) != string::npos) {
							found = true;
							::printf("\tFDB[\"%s\"].SubDB[%d] : %d / %d [%s]\n",
									 fdb->GetDBFileName().c_str(),
									 i,
									 *subdb->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::FixedID))->GetIntegerPTR(),
									 *subdb->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::VisibleID))->GetIntegerPTR(),
									 subdb->GetCellFromIndex(row, static_cast<int>(TS::DB::DBDefaultColumnIndices::Name))->GetStringPTR()->c_str()
									 );
						}
					}
				}
			}
			if(!found) {
				::puts("cmdError: �Y�����鍀�ڂ�������܂���ł����B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "svdb") {
		//VDB-�Z���l���Z�b�g
		if(spl.size() == 6) {
			TS_CHECKINDEX(ints[1], 0, Game.DB.VDB.size()) {
				TS_CHECKINDEX(ints[2], 0, Game.DB.VDB[ints[1]]->GetSubDBCount()) {
					TS_CHECKINDEX(ints[3], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetRowCount()) {
						TS_CHECKINDEX(ints[4], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetColumnCount()) {
							temp = Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString();
							Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->SetData(spl[5]);
							::printf("\t%s -> %s\n", temp.c_str(), Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString().c_str());
						} else {
							::puts("cmdError: [��ԍ�] ���͈͊O�ł��B");
						}
					} else {
						::puts("cmdError: [�s�ԍ�] ���͈͊O�ł��B");
					}
				} else {
					::puts("cmdError: [DB�T�u�C���f�b�N�X] ���͈͊O�ł��B");
				}
			} else {
				::puts("cmdError: [DB�C���f�b�N�X] ���͈͊O�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "ivdb") {
		//VDB-�Z���l��������
		if(spl.size() == 5) {
			TS_CHECKINDEX(ints[1], 0, Game.DB.VDB.size()) {
				TS_CHECKINDEX(ints[2], 0, Game.DB.VDB[ints[1]]->GetSubDBCount()) {
					TS_CHECKINDEX(ints[3], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetRowCount()) {
						TS_CHECKINDEX(ints[4], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetColumnCount()) {
							temp = Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString();
							Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->InitCellFromIndex(ints[3], ints[4]);
							::printf("\t%s -> %s\n", temp.c_str(), Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString().c_str());
						} else {
							::puts("cmdError: [��ԍ�] ���͈͊O�ł��B");
						}
					} else {
						::puts("cmdError: [�s�ԍ�] ���͈͊O�ł��B");
					}
				} else {
					::puts("cmdError: [DB�T�u�C���f�b�N�X] ���͈͊O�ł��B");
				}
			} else {
				::puts("cmdError: [DB�C���f�b�N�X] ���͈͊O�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "sudb") {
		//UDB-�Z���l���Z�b�g
		if(spl.size() == 5) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::UDB), 0, 0, ints[1]);
			auto db = Game.DB.UDB.GetUserDB(ints[1]);
			if(db != nullptr) {
				TS_CHECKINDEX(ints[2], 0, db->GetRowCount()) {
					TS_CHECKINDEX(ints[3], 0, db->GetColumnCount()) {
						temp = db->GetCellFromIndex(ints[2], ints[3])->ToString();
						db->GetCellFromIndex(ints[2], ints[3])->SetData(spl[4]);
						::printf("\t%s -> %s\n", temp.c_str(), db->GetCellFromIndex(ints[2], ints[3])->ToString().c_str());
					} else {
						::puts("cmdError: [��ԍ�] ���͈͊O�ł��B");
					}
				} else {
					::puts("cmdError: [�s�ԍ�] ���͈͊O�ł��B");
				}
			} else {
				::puts("cmdError: [UserDBID] �������ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "iudb") {
		//UDB-�Z���l��������
		if(spl.size() == 4) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::UDB), 0, 0, ints[1]);
			auto db = Game.DB.UDB.GetUserDB(ints[1]);
			if(db != nullptr) {
				TS_CHECKINDEX(ints[2], 0, db->GetRowCount()) {
					TS_CHECKINDEX(ints[3], 0, db->GetColumnCount()) {
						temp = db->GetCellFromIndex(ints[2], ints[3])->ToString();
						db->InitCellFromIndex(ints[2], ints[3]);
						::printf("\t%s -> %s\n", temp.c_str(), db->GetCellFromIndex(ints[2], ints[3])->ToString().c_str());
					} else {
						::puts("cmdError: [��ԍ�] ���͈͊O�ł��B");
					}
				} else {
					::puts("cmdError: [�s�ԍ�] ���͈͊O�ł��B");
				}
			} else {
				::puts("cmdError: [UserDBID] �������ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "lvl") {
		//���ʕϐ��̒l���
		Game.DB.getVDBValue()->PrintData();
		return true;
	} else if(spl[0] == "svl") {
		//���ʕϐ��̒l��ύX
		if(spl.size() == 4) {
			if(spl[1] == "f") {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::VDB), static_cast<int>(TS::DB::VDBIndices::Value), static_cast<int>(TS::DB::DBValueIndices::CommonFlag), ints[2]);
				if(Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::CommonFlag))->GetRowFromID(ints[2]) != nullptr) {
					bool bval = Game.SQ.GetSQBool(Game.Format(TS::SQ::UserFlagName, ints[2]));
					bool aval = ints[3] == 0 ? false : true;
					Game.SQ.SetSQValueByVal(&aval, Game.Format(TS::SQ::UserFlagName, ints[2]));
					::printf("\t%s -> %s (%s)\n", TS_BOOLSTR(bval), TS_BOOLSTR(aval),
							 Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::CommonFlag))->GetRowFromID(ints[2])->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
							 );
				} else {
					::puts("cmdError: �����ȋ��ʃt���O��ID�ł��B");
				}
			} else if(spl[1] == "i") {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::VDB), static_cast<int>(TS::DB::VDBIndices::Value), static_cast<int>(TS::DB::DBValueIndices::CommonInteger), ints[2]);
				if(Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::CommonInteger))->GetRowFromID(ints[2]) != nullptr) {
					int bval = Game.SQ.GetSQInteger(Game.Format(TS::SQ::UserIntegerName, ints[2]));
					int aval = ints[3];
					Game.SQ.SetSQValueByVal(&aval, Game.Format(TS::SQ::UserIntegerName, ints[2]));
					::printf("\t%d -> %d (%s)\n", bval, aval,
							 Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::CommonInteger))->GetRowFromID(ints[2])->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
							 );
				} else {
					::puts("cmdError: �����ȋ��ʐ�����ID�ł��B");
				}
			} else if(spl[1] == "s") {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::VDB), static_cast<int>(TS::DB::VDBIndices::Value), static_cast<int>(TS::DB::DBValueIndices::CommonString), ints[2]);
				if(Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::CommonString))->GetRowFromID(ints[2]) != nullptr) {
					string bval = Game.SQ.GetSQString(Game.Format(TS::SQ::UserStringName, ints[2]));
					string aval = spl[3];
					Game.SQ.SetSQValueByVal(&aval, Game.Format(TS::SQ::UserStringName, ints[2]));
					::printf("\t\"%s\" -> \"%s\" (%s)\n", bval.c_str(), aval.c_str(),
							 Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::CommonString))->GetRowFromID(ints[2])->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
							 );
				} else {
					::puts("cmdError: �����ȋ��ʕ����ϐ���ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȋ��ʕϐ��̎�ʋL���ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "mny") {
		//�������ύX
		if(spl.size() == 2) {
			int bval = Game.DB.getVDBParty()->Money;
			Game.DB.getVDBParty()->setMoney(Game.DB.getVDBParty()->getMoney() + ints[1]);
			::printf("\t%d -> %d\n", bval, Game.DB.getVDBParty()->Money);
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "itm") {
		//�A�C�e������
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				while(ints[2] != 0) {
					if(ints[2] > 0) {
						//���₷
						Game.DB.getVDBParty()->AddItem(ints[1], 1);
						ints[2]--;
					} else if(ints[2] < 0) {
						//���炷
						Game.DB.getVDBParty()->DeleteItem(ints[1], 1);
						ints[2]++;
					}
				}
			} else {
				::puts("cmdError: �����ȃA�C�e��ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "lchr") {
		//�A�N�^�[����
		if(spl.size() == 1) {
			Game.DB.getVDBActor()->PrintData();
		} else if(spl.size() == 2) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				Game.DB.getVDBActor()->DB.GetFromID(ints[1])->PrintData();
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "seqp") {
		//�����i�ύX
		if(spl.size() == 4) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Element), static_cast<int>(TS::DB::DBElementIndices::ItemType), ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::ItemType))->GetRowFromID(ints[2]) != nullptr) {
					int beforeID = Game.DB.getVDBActor()->DB.GetFromID(ints[1])->GetEquipIDFromItemTypeID(ints[2]);
					ints[3] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item), 0, ints[3]);
					if(ints[3] == -1) {
						if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseEquipType(ints[2])) {
							::printf("%s -> %s\n",
									 (beforeID == -1) ? "(�Ȃ�)" : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(beforeID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
									 "(�Ȃ�)"
									 );
						} else {
							::puts("�������s");
						}
					} else if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ints[3]) != nullptr) {
						if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetEquip(ints[3])) {
							::printf("%s -> %s\n",
									 (beforeID == -1) ? "(�Ȃ�)" : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(beforeID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
									 Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ints[3])->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
									 );
						}
					} else {
						::puts("cmdError: �����ȃA�C�e��ID�ł��B");
					}
				} else {
					::puts("cmdError: �����ȑ�������ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "scls") {
		//�N���X�ݒ�
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Class), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetClass(ints[2]);
				} else {
					::puts("cmdError: �����ȃN���XID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "rcls") {
		//�N���X����
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Class), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseClass(ints[2])) {
						::puts("��������");
					} else {
						::puts("�������s");
					}
				} else {
					::puts("cmdError: �����ȃN���XID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "sskl") {
		//�X�L���ݒ�
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Skill), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetSkill(ints[2]);
				} else {
					::puts("cmdError: �����ȃX�L��ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "rskl") {
		//�X�L������
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Skill), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseSkill(ints[2])) {
						::puts("��������");
					} else {
						::puts("�������s");
					}
				} else {
					::puts("cmdError: �����ȃX�L��ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "sstt") {
		//�Ԑڌ��ʐݒ�
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::State), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetState(ints[2], true);
				} else {
					::puts("cmdError: �����ȊԐڌ���ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "rstt") {
		//�Ԑڌ��ʉ���
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::State), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseState(ints[2])) {
						::puts("��������");
					} else {
						::puts("�������s");
					}
				} else {
					::puts("cmdError: �����ȊԐڌ���ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "spar") {
		//�p�����[�^�[�ݒ�
		if(spl.size() == 4) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Element), static_cast<int>(TS::DB::DBElementIndices::Parameter), ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowFromID(ints[2]) != nullptr) {
					::printf("%d -> %d\n", Game.DB.getVDBActor()->DB.GetFromID(ints[1])->Parameters.GetFromID(ints[2])->Value, ints[3]);
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->Parameters.GetFromID(ints[2])->Value = ints[3];
				} else {
					::puts("cmdError: �����ȃp�����[�^�[ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "sval") {
		//�A�N�^�[�ʕϐ��ݒ�
		if(spl.size() == 4) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::VDB), static_cast<int>(TS::DB::VDBIndices::Value), static_cast<int>(TS::DB::DBValueIndices::SelfActor), ints[2]);
				if(Game.DB.VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfActor))->GetRowFromID(ints[2]) != nullptr) {
					::printf("%d -> %d\n", Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SelfValues.Get(ints[2])->Value, ints[3]);
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SelfValues.Set(ints[2], ints[3]);
				} else {
					::puts("cmdError: �����ȃA�N�^�[�ʕϐ�ID�ł��B");
				}
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "lpt") {
		//�p�[�e�B�����o�[��
		Game.DB.getVDBParty()->PrintData();
		return true;
	} else if(spl[0] == "apt") {
		//�p�[�e�B�֒ǉ�
		if(spl.size() == 2) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				Game.DB.getVDBParty()->AddMember(ints[1], -1);
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "dptid") {
		//�p�[�e�B���痣�E/ID
		if(spl.size() == 2) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				Game.DB.getVDBParty()->DeleteMemberFromID(ints[1]);
			} else {
				::puts("cmdError: �����ȃp�[�e�B�L����ID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "dptidx") {
		//�p�[�e�B���痣�E/Index
		if(spl.size() == 2) {
			Game.DB.getVDBParty()->DeleteMemberFromIndex(ints[1]);
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	}
	return false;
}


//####################  �R�}���h�����F�}�b�v�V�[��  ####################
bool CDebug::cmdMap(CMDARGS) {
	bool notMapScene = false;
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		notMapScene = true;
	}

	//�p�����[�^�[�𐮐��l�ɕϊ��������X�g�����
	string temp;
	vector<int> ints;
	for(auto& arg : spl) {
		ints.push_back(atoi(arg.c_str()));
	}

	if(spl[0] == "map") {
		//�}�b�v�ύX
		if(notMapScene) {
			::puts("cmdError: ���݃}�b�v�V�[���ł͂���܂���B");
		} else if(spl.size() == 4) {
			Game.getSceneMap()->MapData.LoadMap(spl[1], false);
			Game.getSceneMap()->MapData.Player.MapObject.Position.Set(ints[2], ints[3]);
			Game.getSceneMap()->MapData.AfterLoadMap();
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "pos") {
		//�C�x���g�ʒu�ړ�
		if(notMapScene) {
			::puts("cmdError: ���݃}�b�v�V�[���ł͂���܂���B");
		} else if(spl.size() == 4) {
			ints[1] = Game.getSceneMap()->MapData.GetEVFixedID(ints[1]);
			if(ints[1] == -1) {
				//��l���Ώ�
				Game.getSceneMap()->MapData.Player.MapObject.Position.Set(ints[2], ints[3]);
			} else if(Game.getSceneMap()->MapData.EVExists(ints[1])) {
				//�C�x���g�Ώ�
				Game.getSceneMap()->MapData.EVGetData(ints[1])->Position.Set(ints[2], ints[3]);
			} else {
				::puts("cmdError: �����ȃC�x���gID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "sev") {
		//�C�x���g����
		if(notMapScene) {
			::puts("cmdError: ���݃}�b�v�V�[���ł͂���܂���B");
		} else if(spl.size() == 4) {
			CSceneMap::CMapData::CMapCharObjectData* data = nullptr;
			CSceneMap::CMapData::CMapEventData::CMapEventPageData* ev = nullptr;
			ints[1] = Game.getSceneMap()->MapData.GetEVFixedID(ints[1]);
			if(ints[1] == -1) {
				//��l���Ώ�
				data = &Game.getSceneMap()->MapData.Player.CharData;
			} else if(Game.getSceneMap()->MapData.EVExists(ints[1])) {
				//�C�x���g�Ώ�
				data = Game.getSceneMap()->MapData.EVGetData(ints[1])->getCurrentPage();
				if(data == nullptr) {
					::puts("cmdError: �w�肳�ꂽ�C�x���g�͔�o����Ԃł��B");
				}
			} else {
				::puts("cmdError: �����ȃC�x���gID�ł��B");
			}
			if(data != nullptr) {
				switch(ints[2]) {
					case 0:		//�N������
						ev = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(data);
						if(ev != nullptr) {
							ev->TriggerType = ints[3];
						}
						break;
					case 1:		//����ړ�
						ev = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(data);
						if(ev != nullptr) {
							ev->MoveType = ints[3];
						}
						break;
					case 2:		//����
						data->Direction = ints[3];
						break;
					case 3:		//���x
						data->MoveSpeed = ints[3];
						break;
					case 4:		//�p�x
						ev = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(data);
						if(ev != nullptr) {
							ev->MoveFrequency = ints[3];
						}
						break;
					case 5:		//�Î~�A�j��
						data->EnabledStopAnim = ints[3];
						break;
					case 6:		//���s�A�j��
						data->EnabledWalkAnim = ints[3];
						break;
					case 7:		//���蔲��
						data->NoHit = ints[3];
						break;
					case 8:		//�����Œ�
						data->FixedDirection = ints[3];
						break;
					default:
						::puts("cmdError: �����ȃp�����[�^�[�ԍ��ł��B");
				}
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "dev") {
		//�C�x���g���s
		if(notMapScene) {
			::puts("cmdError: ���݃}�b�v�V�[���ł͂���܂���B");
		} else if(spl.size() == 2) {
			ints[1] = Game.getSceneMap()->MapData.GetEVFixedID(ints[1]);
			if(Game.getSceneMap()->MapData.EVExists(ints[1])) {
				Game.getSceneMap()->MapData.RunEV(ints[1], static_cast<int>(TS::Map::EventScriptType::Run), true);
			} else {
				::puts("cmdError: �����ȃC�x���gID�ł��B");
			}
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	}
	return false;
}


//####################  �R�}���h�����F�X�N���v�g  ####################
bool CDebug::cmdSQ(CMDARGS) {
	if(spl[0] == "sqf") {
		//�X�N���v�g�t�@�C�����s
		if(spl.size() == 2) {
			Game.SQ.Do(spl[1], false);
		} else {
			::puts("cmdError: �����̌����s���ł��B");
		}
		return true;
	} else if(spl[0] == "sqc") {
		//���ȃR�[�h���s
		auto pos = buf.find(" ");
		if(pos != string::npos) {
			string code = buf.substr(pos);
			if(code.size() > 0 && code.substr(code.size() - 1, 1) == ";") {
				Sqrat::Script tester;
				try {
					::puts("");		//��s�����
					tester.CompileString(code);
					tester.Run();
					::puts("���ȃX�N���v�g�R�[�h�̎��s���������܂����B");
				} catch(...) {
					::puts("\ncmdError: �^����ꂽ�R�[�h�����s�ł��܂���B\n");
				}
			} else {
				::puts("cmdError: �R�[�h�̖����ɂ̓Z�~�R�������K�v�ł��B");
			}
		} else {
			::puts("cmdError: �R�[�h���܂܂�Ă��܂���B");
		}
		CDebug::ActivateConsole();
		return true;
	}
	return false;
}


//####################  �R���\�[�����A�N�e�B�u�ɂ���  ####################
void CDebug::ActivateConsole() {
	::SetForegroundWindow(::GetConsoleWindow());
}


//####################  �Q�[���E�B���h�E���A�N�e�B�u�ɂ���  ####################
void CDebug::ActivateGameWindow() {
	::SetForegroundWindow(::GetMainWindowHandle());
}


//####################  �R���\�[�����N���A����  ####################
void CDebug::ClsConsole() {
	COORD coordScreen = {0, 0};    /* here's where we'll home the cursor */
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;                 /* number of character cells in the current buffer */
	auto hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

	/* get the number of character cells in the current buffer */
	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	//PERR(bSuccess, "GetConsoleScreenBufferInfo");
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */
	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
										  dwConSize, coordScreen, &cCharsWritten);
	//PERR(bSuccess, "FillConsoleOutputCharacter");

	/* get the current Text attribute */
	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	//PERR(bSuccess, "ConsoleScreenBufferInfo");

	/* now set the buffer's attributes accordingly */
	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
										  dwConSize, coordScreen, &cCharsWritten);
	//PERR(bSuccess, "FillConsoleOutputAttribute");

	/* put the cursor at (0, 0) */
	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
	//PERR(bSuccess, "SetConsoleCursorPosition");
	return;
}


//####################  �����L�[���������܂ŏ������~�߂�  ####################
//�Ԓl�͉����ꂽ�L�[�̕����R�[�h
int CDebug::PauseConsole() {
	::fflush(stdin);						//���̓o�b�t�@�̃S�~��|������
	//return ::getchar();

	//�ꕶ�����͂��󂯕t����
	int ret = ::_getch();
	::putc('>', stdin);
	::putc(ret, stdin);
	::printf("\n");
	return ret;
}
