//###################################################################
//		�S�������A���S���Y��
//###################################################################

//####################  �m�[�h�^�C�����  ####################
TileType <- {
	Null = 0,		//����
	None = 1,		//���sOK
	Wall = 2,		//���sNG
};


//####################  �m�[�h���  ####################
class SQNode {
	//�����o�[�ϐ�
	Fstar = INT_MAX;
	Gstar = INT_MAX;
	prev = -1;				//���̃^�C�����猩�āA��O�̃^�C�����ǂ̕����ɂ��邩������ (�n�_�� -1 �ɂȂ�)
	done = false;
	pos = CPoint();
	type = -1;

	//�R���X�g���N�^�[
	constructor(_type = TileType.Null) {
		this.type = _type;
	}
}


//####################  �A���S���Y���{��  ####################
Astar <- {

	MoveTasks = array(0),		//�i�s���ׂ����������������X�g
	MoveTaskIndex = 0,			//���݂̐i�s�^�X�N�̈ʒu

	maze = array(0),			//�m�[�h�Ƃ��Ẵ}�b�v�^�C�����
	open = array(0),			//���m��̃m�[�h���X�g
	start = CPoint(),
	end = CPoint(),


	//####################  �A���S���Y��������  ####################
	InitAstar = function() {
		//�Q�����z��𐶐�
		maze = array(Game.SceneMap.MapData.MapSize.Width);
		foreach(x, submaze in maze) {
			maze[x] = array(Game.SceneMap.MapData.MapSize.Height);
			foreach(y, node in maze[x]) {
				maze[x][y] = SQNode(TileType.None);
				maze[x][y].pos = CPoint();
				maze[x][y].pos.Set(x, y);
				if(Game.SceneMap.MapData.IsWalkableTile(x, y) == false) {
					maze[x][y].type = TileType.Wall;
				} else {
					//���C�x���g�̓����蔻����l������
					for(local i = 0; i < Game.SceneMap.MapData.EVCount(); i++) {
						if(Game.SceneMap.MapData.EVGetDataFromIndex(i).Position.X == x
						&& Game.SceneMap.MapData.EVGetDataFromIndex(i).Position.Y == y) {
							if(Game.SceneMap.MapData.EVGetDataFromIndex(i).Hide == false
							&& !Game.SceneMap.MapData.EVGetDataFromIndex(i).CurrentPage.IsNull()
							&& Game.SceneMap.MapData.EVGetDataFromIndex(i).CurrentPage.NoHit == false) {
								maze[x][y].type = TileType.Wall;
								break;
							}
						}
					}
				}
			}
		}
	},


	//####################  �o�H�T�����J�n���邽�߂ɕK�v�ȏ������s��  ####################
	AstarStart = function(startPos, endPos) {
		//�n�_�ƏI�_���Z�b�g
		start.Set(startPos.X, startPos.Y);
		end.Set(endPos.X, endPos.Y);

		//�X�^�[�g�n�_�̂� gs �� 0 �Ƃ��Aopen �ɉ�����
		maze[start.X][start.Y].Gstar = 0;
		maze[start.X][start.Y].Fstar = maze[start.X][start.Y].Gstar + getHstar(maze[start.X][start.Y]);
		open = array(0);
		open.append(maze[start.X][start.Y]);
	},


	//####################  Hstar�擾  ####################
	getHstar = function(node) {
		return abs(node.pos.X - end.X) + abs(node.pos.Y - end.Y);
	},


	//####################  ��̃C�x���g�̋������v�Z����  ####################
	GetDistance = function(aID, bID) {
		if(aID == -1 && bID == -1) {
			//��l���̍ċA��r
			return 0;
		} else if(aID == -1) {
			//�Е�����l��
			return	abs(Game.SceneMap.MapData.Player.MapObject.Position.X - Game.SceneMap.MapData.EVGetData(bID).Position.X) +
					abs(Game.SceneMap.MapData.Player.MapObject.Position.Y - Game.SceneMap.MapData.EVGetData(bID).Position.Y);
		} else if(bID == -1) {
			//�Е�����l��
			return	abs(Game.SceneMap.MapData.Player.MapObject.Position.X - Game.SceneMap.MapData.EVGetData(aID).Position.X) +
					abs(Game.SceneMap.MapData.Player.MapObject.Position.Y - Game.SceneMap.MapData.EVGetData(aID).Position.Y);
		} else {
			//�C�x���g���m�̔�r
			return	abs(Game.SceneMap.MapData.EVGetData(aID).Position.X - Game.SceneMap.MapData.EVGetData(bID).Position.X) +
					abs(Game.SceneMap.MapData.EVGetData(aID).Position.Y - Game.SceneMap.MapData.EVGetData(bID).Position.Y);
		}
	},


	//####################  �w�肵���C�x���g����A�w�肵���ڕW�_�ւ̍ŒZ���[�g���Z�b�g����  ####################
	ResetRouteByAstar = function(intEVID, endPos, intLength) {
		if(intLength < 10) {
			//�ΏۂƂ̍ŒZ���������ɐڋ߂��Ă���ꍇ�́A�P�^�C�����w�����Ă����悤�ɏC������
			intLength = 1;
		}

		//�O��̃f�[�^������������
		MoveTasks = array(0);
		MoveTaskIndex = 0;
		for(local x = 0; x < Game.SceneMap.MapData.MapSize.Width; x++) {
			for(local y = 0; y < Game.SceneMap.MapData.MapSize.Height; y++) {
				maze[x][y].done = false;
				maze[x][y].prev = -1;
				maze[x][y].Fstar = maze[x][y].Gstar = INT_MAX;
			}
		}
		//�T������
		AstarStart(
			Game.SceneMap.MapData.EVGetData(intEVID).Position,
			endPos
		);
		AstarSearch();			//���ۂ̒T�����s��

		//�T�����I��������A�k���Č��݂̏ꏊ����ǂ̕����֐i�ނׂ�����Ԃ�
		local prevnodes = array(0);
		local temppos = CPoint();			//���݃t�H�[�J�X�ƂȂ��Ă�����W�i�n�_���S�[���A�I�_���X�^�[�g�j
		temppos.Set(maze[end.X][end.Y].pos.X, maze[end.X][end.Y].pos.Y);
		if(maze[end.X][end.Y].done == true) {
			while(true) {
				if(maze[end.X][end.Y].prev == -1) {
					return;		//�I�_���n�_�ł���Ƃ������Ƃ́A��v���Ă���Ƃ������ƂȂ̂ŁA�����͂��̂܂܂ɂ���
				}

				switch(maze[temppos.X][temppos.Y].prev) {
					case Direction4.East:
						temppos.X++;
						break;
					case Direction4.South:
						temppos.Y++;
						break;
					case Direction4.West:
						temppos.X--;
						break;
					case Direction4.North:
						temppos.Y--;
						break;
				}
				prevnodes.insert(0, maze[temppos.X][temppos.Y]);	//���X�g�̐擪�ɁA���k���Ă���Ő�[�̃m�[�h��ǉ�����

				if(prevnodes[0].pos.X == start.X && prevnodes[0].pos.Y == start.Y) {
					if(prevnodes.len() <= 1) {		//�I�_�̈�O�������n�_�ł���΁A�I�_�� prev �𔽓]�i�n�_����̕����Ɂj�������̂��m�肳���ĕԂ�
						MoveTasks.append(Game.SceneMap.TurnHalf4(maze[end.X][end.Y].prev));
						break;
					} else {						//�n�_�ɒH�蒅������A���̈�O�� prev �𔽓]�i�n�_����̕����Ɂj�������̂��m�肳���ĕԂ�
						for(local i = 1; i <= intLength && i < prevnodes.len(); i++) {
							MoveTasks.append(Game.SceneMap.TurnHalf4(prevnodes[i].prev));
						}
						break;
					}
				}
			}
		}
	},


	//####################  �_�C�N�X�g���@�̂P�X�e�b�v�����s����  ####################
	AstarSearch = function() {
		while(true) {
			//���m��m�[�h�̒�����A�X�R�A���ŏ��ƂȂ�m�[�h u �����肷��
			local minScore = INT_MAX;
			local minIndex = -1;
			local u = null;

			foreach(idx, node in open) {
				if(node.done == true) {
					continue;
				} else if(node.Fstar < minScore) {
					minScore = node.Fstar;
					minIndex = idx;
					u = clone open[idx];
				}
			}

			//���m��m�[�h���Ȃ���ΏI���Ƃ���
			if(u == null) {
				return true;
			}

			//�m�[�h u ���m��i�m�F�ς݁j�m�[�h�Ƃ��āA���m��m�[�h����폜����
			open.remove(minIndex)
			maze[u.pos.X][u.pos.Y].done = true;

			//�S�[���ɓ��B�����炷���ɒT�����I����
			if(maze[u.pos.X][u.pos.Y].pos.X == end.X && maze[u.pos.X][u.pos.Y].pos.Y == end.Y) {
				return true;
			}

			//�m�[�h u �̎���̃m�[�h�̃X�R�A���X�V����
			for(local i = 0; i < 4/*�l��������*/; i++) {
				//�͈͊O�łȂ����ǂ������`�F�b�N���āA�͈͓��ł���΁A����ɂ���m�[�h�̂����̈�ł���A�m�[�h v ���擾����
				local out = false;
				local v = SQNode();

				switch(i) {		// i �� u ���猩������������
					case Direction4.East:
						if(u.pos.X + 1 >= Game.SceneMap.MapData.MapSize.Width) {
							out = true;
						} else {
							v = maze[u.pos.X + 1][u.pos.Y];
						}
						break;

					case Direction4.South:
						if(u.pos.Y + 1 >= Game.SceneMap.MapData.MapSize.Height) {
							out = true;
						} else {
							v = maze[u.pos.X][u.pos.Y + 1];
						}
						break;

					case Direction4.West:
						if(u.pos.X - 1 < 0) {
							out = true;
						} else {
							v = maze[u.pos.X - 1][u.pos.Y];
						}
						break;

					case Direction4.North:
						if(u.pos.Y - 1 < 0) {
							out = true;
						} else {
							v = maze[u.pos.X][u.pos.Y - 1];
						}
						break;
				}
				if(out == true) {
					continue;
				}

				//�m��i�m�F�ς݁j�m�[�h ���邢�� �ǂł���ꍇ�̓X�L�b�v����
				if(v.done == true || v.type == TileType.Wall) {
					continue;
				}

				//�����̃X�R�A�����������Ƃ��̂ݍX�V����
				if(u.Gstar + 1 + getHstar(v) < v.Fstar) {
					v.Gstar = u.Gstar + 1;
					v.Fstar = v.Gstar + getHstar(v);
					v.prev = Game.SceneMap.TurnHalf4(i);		//prev��ݒ肷��iv ���猩��ƁAu ����̎��_�ɑ΂��ċt�ɂȂ邱�Ƃ𗘗p����j

					local added = false;
					foreach(n, node in open) {
						if(node.pos.X == v.pos.X && node.pos.Y == v.pos.Y) {
							added = true;
							break;
						}
					}
					if(added == false) {
						//���ɒǉ�����Ă���ꍇ�������Aopen ���X�g�ɒǉ�����
						maze[v.pos.X][v.pos.Y].prev = v.prev;
						open.append(clone v);
					}
				}
			}
		}
		return false;
	},
};
