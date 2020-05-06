//###################################################################
//		鬼ごっこアルゴリズム
//###################################################################

//####################  ノードタイル情報  ####################
TileType <- {
	Null = 0,		//無効
	None = 1,		//歩行OK
	Wall = 2,		//歩行NG
};


//####################  ノード情報  ####################
class SQNode {
	//メンバー変数
	Fstar = INT_MAX;
	Gstar = INT_MAX;
	prev = -1;				//そのタイルから見て、一つ前のタイルがどの方向にあるかを示す (始点は -1 になる)
	done = false;
	pos = CPoint();
	type = -1;

	//コンストラクター
	constructor(_type = TileType.Null) {
		this.type = _type;
	}
}


//####################  アルゴリズム本体  ####################
Astar <- {

	MoveTasks = array(0),		//進行すべき方向を示したリスト
	MoveTaskIndex = 0,			//現在の進行タスクの位置

	maze = array(0),			//ノードとしてのマップタイル情報
	open = array(0),			//未確定のノードリスト
	start = CPoint(),
	end = CPoint(),


	//####################  アルゴリズム初期化  ####################
	InitAstar = function() {
		//２次元配列を生成
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
					//物イベントの当たり判定を考慮する
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


	//####################  経路探索を開始するために必要な準備を行う  ####################
	AstarStart = function(startPos, endPos) {
		//始点と終点をセット
		start.Set(startPos.X, startPos.Y);
		end.Set(endPos.X, endPos.Y);

		//スタート地点のみ gs を 0 とし、open に加える
		maze[start.X][start.Y].Gstar = 0;
		maze[start.X][start.Y].Fstar = maze[start.X][start.Y].Gstar + getHstar(maze[start.X][start.Y]);
		open = array(0);
		open.append(maze[start.X][start.Y]);
	},


	//####################  Hstar取得  ####################
	getHstar = function(node) {
		return abs(node.pos.X - end.X) + abs(node.pos.Y - end.Y);
	},


	//####################  二つのイベントの距離を計算する  ####################
	GetDistance = function(aID, bID) {
		if(aID == -1 && bID == -1) {
			//主人公の再帰比較
			return 0;
		} else if(aID == -1) {
			//片方が主人公
			return	abs(Game.SceneMap.MapData.Player.MapObject.Position.X - Game.SceneMap.MapData.EVGetData(bID).Position.X) +
					abs(Game.SceneMap.MapData.Player.MapObject.Position.Y - Game.SceneMap.MapData.EVGetData(bID).Position.Y);
		} else if(bID == -1) {
			//片方が主人公
			return	abs(Game.SceneMap.MapData.Player.MapObject.Position.X - Game.SceneMap.MapData.EVGetData(aID).Position.X) +
					abs(Game.SceneMap.MapData.Player.MapObject.Position.Y - Game.SceneMap.MapData.EVGetData(aID).Position.Y);
		} else {
			//イベント同士の比較
			return	abs(Game.SceneMap.MapData.EVGetData(aID).Position.X - Game.SceneMap.MapData.EVGetData(bID).Position.X) +
					abs(Game.SceneMap.MapData.EVGetData(aID).Position.Y - Game.SceneMap.MapData.EVGetData(bID).Position.Y);
		}
	},


	//####################  指定したイベントから、指定した目標点への最短ルートをセットする  ####################
	ResetRouteByAstar = function(intEVID, endPos, intLength) {
		if(intLength < 10) {
			//対象との最短距離が非常に接近している場合は、１タイルずつ指示していくように修正する
			intLength = 1;
		}

		//前回のデータを初期化する
		MoveTasks = array(0);
		MoveTaskIndex = 0;
		for(local x = 0; x < Game.SceneMap.MapData.MapSize.Width; x++) {
			for(local y = 0; y < Game.SceneMap.MapData.MapSize.Height; y++) {
				maze[x][y].done = false;
				maze[x][y].prev = -1;
				maze[x][y].Fstar = maze[x][y].Gstar = INT_MAX;
			}
		}
		//探索準備
		AstarStart(
			Game.SceneMap.MapData.EVGetData(intEVID).Position,
			endPos
		);
		AstarSearch();			//実際の探索を行う

		//探索が終了したら、遡って現在の場所からどの方向へ進むべきかを返す
		local prevnodes = array(0);
		local temppos = CPoint();			//現在フォーカスとなっている座標（始点がゴール、終点がスタート）
		temppos.Set(maze[end.X][end.Y].pos.X, maze[end.X][end.Y].pos.Y);
		if(maze[end.X][end.Y].done == true) {
			while(true) {
				if(maze[end.X][end.Y].prev == -1) {
					return;		//終点が始点であるということは、一致しているということなので、向きはそのままにする
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
				prevnodes.insert(0, maze[temppos.X][temppos.Y]);	//リストの先頭に、今遡っている最先端のノードを追加する

				if(prevnodes[0].pos.X == start.X && prevnodes[0].pos.Y == start.Y) {
					if(prevnodes.len() <= 1) {		//終点の一つ前がすぐ始点であれば、終点の prev を反転（始点からの方向に）したものを確定させて返す
						MoveTasks.append(Game.SceneMap.TurnHalf4(maze[end.X][end.Y].prev));
						break;
					} else {						//始点に辿り着いたら、その一つ前の prev を反転（始点からの方向に）したものを確定させて返す
						for(local i = 1; i <= intLength && i < prevnodes.len(); i++) {
							MoveTasks.append(Game.SceneMap.TurnHalf4(prevnodes[i].prev));
						}
						break;
					}
				}
			}
		}
	},


	//####################  ダイクストラ法の１ステップを実行する  ####################
	AstarSearch = function() {
		while(true) {
			//未確定ノードの中から、スコアが最小となるノード u を決定する
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

			//未確定ノードがなければ終了とする
			if(u == null) {
				return true;
			}

			//ノード u を確定（確認済み）ノードとして、未確定ノードから削除する
			open.remove(minIndex)
			maze[u.pos.X][u.pos.Y].done = true;

			//ゴールに到達したらすぐに探索を終える
			if(maze[u.pos.X][u.pos.Y].pos.X == end.X && maze[u.pos.X][u.pos.Y].pos.Y == end.Y) {
				return true;
			}

			//ノード u の周りのノードのスコアを更新する
			for(local i = 0; i < 4/*四方を見る*/; i++) {
				//範囲外でないかどうかをチェックして、範囲内であれば、周りにあるノードのうちの一つである、ノード v を取得する
				local out = false;
				local v = SQNode();

				switch(i) {		// i は u から見た方向を示す
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

				//確定（確認済み）ノード あるいは 壁である場合はスキップする
				if(v.done == true || v.type == TileType.Wall) {
					continue;
				}

				//既存のスコアよりも小さいときのみ更新する
				if(u.Gstar + 1 + getHstar(v) < v.Fstar) {
					v.Gstar = u.Gstar + 1;
					v.Fstar = v.Gstar + getHstar(v);
					v.prev = Game.SceneMap.TurnHalf4(i);		//prevを設定する（v から見ると、u からの視点に対して逆になることを利用する）

					local added = false;
					foreach(n, node in open) {
						if(node.pos.X == v.pos.X && node.pos.Y == v.pos.Y) {
							added = true;
							break;
						}
					}
					if(added == false) {
						//既に追加されている場合を除き、open リストに追加する
						maze[v.pos.X][v.pos.Y].prev = v.prev;
						open.append(clone v);
					}
				}
			}
		}
		return false;
	},
};
