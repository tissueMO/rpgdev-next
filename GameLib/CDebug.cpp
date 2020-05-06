//###################################################################
//		デバッグ用コード
//###################################################################
#include "Header.h"
#include <conio.h>
extern char buf[TS::System::BufferLengthMax];


//####################  スペースキーでゲームを停止させた直後に行う処理  ####################
//返値はスクリプトのステップ実行に移行するかどうか
bool CDebug::StartDebugConsole() {
	//::clsConsole();
	CDebug::ActivateConsole();
	TS_LOG("スクリプトデバッガーが起動しました。\n\tデバッグコンソールの閉じるボタンを押すと、ゲーム自体が終了するので注意して下さい。\n\tデバッグコンソール上でコマンドが入力できます。コマンドのヘルプは h で表示できます。");
	Game.NoTimeStamp = true;

	while(!Game.SQ.KillDebuggerFlag) {
		//コマンド入力を受け付ける
		::printf(">");
		::fflush(stdin);
		::fgets(::buf, sizeof(::buf) - 1, stdin);
		::buf[strlen(::buf) - 1] = '\0';

		//前方空白を除去
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

		//パラメーター分割
		vector<string> spl;
		Game.SplitString(spl, temp, " ");

		//####################  コマンド処理  ####################
		if(spl.size() == 0 || spl[0] == "end" || spl[0] == "exit") {
			//デバッグを終了してゲームに戻る
			DxLib::WaitTimer(500);		//Enterキーがゲーム上で反映されないために少し待つ
			break;
		} else if(spl[0] == "step") {
			//ステップ実行: 次のスクリプトコードを実行したときからデバッガーが動作する
			Game.NoTimeStamp = false;
			return true;
		} else if(spl[0] == "cls") {
			CDebug::ClsConsole();
			continue;
		} else if(spl[0] == "frs") {
			if(spl.size() == 2) {
				Game.FPSKeeper.setSkipFrameCount(atoi(spl[1].c_str()));
			} else {
				::puts("cmdError: 引数の個数が不正です。");
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
			//定義されないコマンドはスクリプトに流す
			if(Game.SQ.DoFunc<bool>(TS::SQ::SQEvent_DebugCommand, temp)) {
				continue;
			}
		}

		//定義されていないコマンド
		::printf("cmdError: 定義されていないデバッグコマンドです。\n");
		//::pauseConsole();
		//::clsConsole();
	}

	*Game.GetKeyInputCounterPTR(TS::System::SQDebugKey) = 2;		//連続してデバッガーが起動するのを回避する
	CDebug::ActivateGameWindow();
	Game.NoTimeStamp = false;
	TS_LOG("スクリプトデバッガーが終了しました。");
	return false;
}


//####################  コマンド処理：ヘルプ  ####################
bool CDebug::cmdHelp(CMDARGS) {
	if(spl[0] == "h" || spl[0] == "help") {
		::printf(
			"入力方法:\n"
			"	・最初にコマンド名を入力し、パラメーターが必要な場合はスペースで区切って続けます。\n"
			"	・ファイル名以外に全角文字を混ぜないようご注意下さい。\n"
			"	・ファイル名にはプロジェクトフォルダーを基点とした相対パスで指定しますが、この中にスペースを含めてはいけません。\n"
			"	・ON/OFFを示す値は 1/0 で入力します。\n"
			"注意事項:\n"
			"	・閉じるボタンを押すと、ゲームプログラム自体が終了します。\n"
			"	・[ID]とある箇所は、ここではユーザーが自由に割り当てられる重複可能な可視IDのことを指します。\n"
			"	　スクリプトで使われるシステム固有のIDとは異なりますのでご注意下さい。\n"
			"\n"
			"＊＊＊ デバッグコンソール操作 ＊＊＊\n"
			"	デバッグを終了:			exit / end / (空白) のいずれか\n"
			"	バッファをクリア		cls\n"
			"\n"
			"＊＊＊ システム操作 ＊＊＊\n"
			"	固定フレームレート変更		frs [値]\n"		//Frame Skip
			"\n"
			"＊＊＊ システム情報列挙 ＊＊＊\n"
			"	グラフィック素材:		lgrh\n"		//List Graphics
			"	サウンド素材:			lsnd\n"		//List Sounds
			"	フォント素材:			lfnt\n"		//List Fonts
			"	固定データベース:		lfdb\n"		//List Fixed Database
			"	可変データベース:		lvdb\n"		//List Variable Database
			"	ユーザーデータベース:		ludb\n"		//List User Database
			"	スプライト:			lsp\n"		//List Sprite
			"	スクリプト:			lsq\n"		//List SQscript
			"	疑似スレッド:			ltd\n"		//List Thread
			"	マップイベント:			lev\n"		//List Event
			"	マップイベント個別変数:		levsv\n"	//List Event SelfValue
			"\n"
			"＊＊＊ 可変/ユーザー データベース操作 ＊＊＊\n"
			"	すべて再読込:			rdb\n"		//Reload Database
			"	固定DB再読込:			rfdb\n"		//Reload Fixed-Database
			"	可変DB再読込:			rvdb\n"		//Reload Variable-Database
			"	ユーザーDB再読込:		rudb\n"		//Reload User-Database
			"	項目名でIDを検索:		fid [項目名]\n"		//Find fixedDB ID
			"		＊固定DBからFixedID/VisibleIDを部分一致で検索します。\n"
			"	可変DB-セル値設定:		svdb [DBインデックス] [DBサブインデックス] [行番号] [列番号] [値]\n"		//Set Variable-Database-Cell
			"	可変DB-セル初期化:		ivdb [DBインデックス] [DBサブインデックス] [行番号] [列番号]\n"			//Init Variable-Database-Cell
			"		＊可変DBインデックス:		0:共通変数  1:パーティ\n"
			"		＊DBサブインデックス:		エディター上の表について、左上を０として右方向に数えたインデックス\n"
			"		＊行番号、列番号:		一番最初を０として数えます。\n"
			"	ユーザーDB-セル値設定:		sudb [UserDBID] [行番号] [列番号] [値]\n"		//Set User-Database-Cell
			"	ユーザーDB-セル初期化:		iudb [UserDBID] [行番号] [列番号]\n"			//Init User-Database-Cell
			"		＊行番号、列番号:		一番最初を０として数えます。\n"
			"	共通変数の値 列挙:		lvl\n"		//List Value
			"	共通変数の値 変更:		svl [f/i/s] [ID] [値]\n"		//Set Value
			"		＊f=フラグ  i=整数  s=文字列\n"
			"	所持金変更:			mny [加算値]\n"		//Money
			"		＊減らしたいときは [加算値] マイナスの値を入れます。\n"
			"	アイテム所持:			itm [アイテムID] [個数]\n"		//Item
			"		＊減らしたいときは [個数] にマイナスの値を入れます。\n"
			"	パーティキャラ-情報列挙:	lchr ([パーティキャラID])\n"		//List Char
			"	パーティキャラ-装備品 変更:	seqp [パーティキャラID] [装備大種別ID] [アイテムID]\n"		//Set Equip
			"		＊装備を外したいときは [アイテムID] に -1 を入れます。\n"
			"	パーティキャラ-クラス 設定:	scls [パーティキャラID] [クラスID]\n"		//Set Class
			"	パーティキャラ-クラス 解除:	rcls [パーティキャラID] [クラスID]\n"		//Release Class
			"	パーティキャラ-スキル 設定:	sskl [パーティキャラID] [スキルID]\n"		//Set Skill
			"	パーティキャラ-スキル 解除:	rskl [パーティキャラID] [スキルID]\n"		//Release Skill
			"	パーティキャラ-間接効果 設定:	sstt [パーティキャラID] [間接効果ID]\n"		//Set State
			"	パーティキャラ-間接効果 解除:	rstt [パーティキャラID] [間接効果ID]\n"		//Release State
			"	パーティキャラ-間接効果 全解除:			rsttall [パーティキャラID]\n"		//Release State All
			"	パーティキャラ-間接効果 バステ全解除:	rsttbad [パーティキャラID]\n"		//Release State only Bad
			"	パーティキャラ-パラメーター 設定:		spar [パーティキャラID] [パラメーターID] [値]\n"	//Set Parameter
			"	パーティキャラ-個別変数 設定:			sval [パーティキャラID] [アクター個別変数ID] [値]\n"	//Set selfValue
			"	パーティ情報列挙:		lpt\n"	//List Party
			"	パーティに追加:			apt [パーティキャラID]\n"		//Add To Party
			"	パーティから離脱(ID):		dptid [パーティキャラID]\n"
			"	パーティから離脱(Index):	dptidx [パーティ並び位置インデックス]\n"
			"		＊並び位置インデックス: 先頭を０として数えます。\n"
			"\n"
			"＊＊＊ マップシーン操作 ＊＊＊\n"
			"	マップ変更:			map [ファイル名] [X座標] [Y座標]\n"
			"	イベント位置移動:		pos [イベントID] [X座標] [Y座標]\n"		//Position
			"		＊[イベントID] に -1 を指定すると、主人公が対象になります。\n"
			"	イベント操作:			sev [イベントID] [パラメーター番号] [値]\n"		//Set Event
			"		＊[イベントID] に -1 を指定すると、主人公が対象になります。\n"
			"		＊パラメーター番号: 0:起動条件  1:日常移動  2:方向  3:速度  4:頻度  5:静止アニメ  6:歩行アニメ  7:すり抜け  8:向き固定\n"
			"		＊値: エディター上の選択リストの項目を上から０として数えた値に対応します。\n"
			"	イベント実行:			dev [イベントID]\n"		//Do Event
			"\n"
			"＊＊＊ スクリプト操作 ＊＊＊\n"
			"	ステップ実行:			step\n"
			"	スクリプトファイル実行:		sqf [ファイル名]\n"
			"	スクリプトコード即席実行:	sqc [コード]\n"
			"		＊ここで実行するコードで不具合があった場合、\n"
			"		　デバッグ終了後に正常動作しなくなる危険性がありますのでご注意下さい。\n"
			"\n"
			"＊＊＊ スクリプトによる拡張コマンド ＊＊＊\n"
			);
		Game.SQ.DoFunc<int>(TS::SQ::SQEvent_DebugCommand, "h");
		::printf("\n");
		return true;
	}
	return false;
}


//####################  コマンド処理：システム情報列挙  ####################
bool CDebug::cmdSystemList(CMDARGS) {
	if(spl[0] == "lgrh") {
		//グラフィック素材
		for(auto item : Game.Material.getGraphics()->Data.Items) {
			TS_LOG("%d: %s (参照数=%d)", item.second.ID, item.second.Name.c_str(), item.second.Value);
		}
		return true;
	} else if(spl[0] == "lsnd") {
		//サウンド素材
		for(auto item : Game.Material.getSounds()->Data.Items) {
			TS_LOG("%d: %s (%s, Loop=%d~%d)", item.second.ID, item.second.Name.c_str(), item.second.IsMIDI ? "MIDI" : "Wave", item.second.StartLoopPos, item.second.EndLoopPos);
		}
		return true;
	} else if(spl[0] == "lfnt") {
		//フォント素材
		for(auto item : Game.Material.getFonts()->Data.Items) {
			TS_LOG("%d: %s (サイズ=%d, 太さ=%d, 行間=%d, 文字間=%d, 描画=%d, 縁色=%d)", item.second.ID, item.second.FontName.c_str(), item.second.Size, item.second.Thick, item.second.LineSpace, item.second.Space, item.second.Type, item.second.EdgeColor);
		}
		return true;
	} else if(spl[0] == "lfdb") {
		//固定DB
		for(auto db : Game.DB.FDB) {
			db->PrintData();
		}
		return true;
	} else if(spl[0] == "lvdb") {
		//可変DB
		for(auto db : Game.DB.VDB) {
			db->PrintData();
		}
		return true;
	} else if(spl[0] == "ludb") {
		//ユーザーDB
		Game.DB.UDB.PrintData();
		return true;
	} else if(spl[0] == "lsp") {
		//スプライト
		TS_LOG("スプライト一覧:");
		for(auto& sprite : Game.Sprite.Data.Items) {
			buf.clear();
			buf = std::to_string(sprite.ID) + ": " + sprite.Name;
			buf += Game.Format("    (%d, %d), Hide=%d, Wipe=%d", sprite.Location.X, sprite.Location.Y, sprite.Hide, static_cast<int>(sprite.WipeType));
			TS_LOG(buf);

			//グラフィック情報
			for(auto& g : sprite.Graphics.Items) {
				TS_LOG("\t" + std::to_string(g.ID) + ": " + g.Name);
			}

			//テキスト情報
			for(auto& t : sprite.Texts.Items) {
				TS_LOG("\t" + std::to_string(t.ID) + ": " + Game.Replace(t.Text, "\n", "[改行]"));
			}
		}
		return true;
	} else if(spl[0] == "lsq") {
		//スクリプト
		Game.SQ.PrintLog();
		return true;
	} else if(spl[0] == "ltd") {
		//MT関数
		Game.MT.PrintLog();
		return true;
	} else if(spl[0] == "lev") {
		//マップイベント
		if(Game.GetCurrentMainSceneID() == static_cast<int>(TS::System::Scene::Map)) {
			Game.getSceneMap()->PrintEVs();
		} else {
			::puts("現在マップシーンではありません。");
		}
		return true;
	} else if(spl[0] == "levsv") {
		//マップイベント個別変数
		if(Game.GetCurrentMainSceneID() == static_cast<int>(TS::System::Scene::Map)) {
			Game.getSceneMap()->PrintSelfValueData();
		} else {
			::puts("現在マップシーンではありません。");
		}
		return true;
	}
	return false;
}


//####################  コマンド処理：データベース  ####################
bool CDebug::cmdDB(CMDARGS) {
	//パラメーターを整数値に変換したリストを作る
	string temp;
	vector<int> ints;
	for(auto& arg : spl) {
		ints.push_back(atoi(arg.c_str()));
	}

	if(spl[0] == "rdb") {
		//すべて再読込
		Game.DB.LoadAll();
		return true;
	} else if(spl[0] == "rfdb") {
		//固定DB再読込
		Game.DB.ReloadDB(static_cast<int>(TS::DB::DBType::FDB), true);
		return true;
	} else if(spl[0] == "rvdb") {
		//可変DB再読込
		Game.DB.ReloadDB(static_cast<int>(TS::DB::DBType::VDB), true);
		return true;
	} else if(spl[0] == "rudb") {
		//ユーザーDB再読込
		Game.DB.ReloadDB(static_cast<int>(TS::DB::DBType::UDB), true);
		return true;
	} else if(spl[0] == "fid") {
		//固定データベースの項目名からFixedID/VisibleIDを部分一致で検索
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
				::puts("cmdError: 該当する項目が見つかりませんでした。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "svdb") {
		//VDB-セル値をセット
		if(spl.size() == 6) {
			TS_CHECKINDEX(ints[1], 0, Game.DB.VDB.size()) {
				TS_CHECKINDEX(ints[2], 0, Game.DB.VDB[ints[1]]->GetSubDBCount()) {
					TS_CHECKINDEX(ints[3], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetRowCount()) {
						TS_CHECKINDEX(ints[4], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetColumnCount()) {
							temp = Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString();
							Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->SetData(spl[5]);
							::printf("\t%s -> %s\n", temp.c_str(), Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString().c_str());
						} else {
							::puts("cmdError: [列番号] が範囲外です。");
						}
					} else {
						::puts("cmdError: [行番号] が範囲外です。");
					}
				} else {
					::puts("cmdError: [DBサブインデックス] が範囲外です。");
				}
			} else {
				::puts("cmdError: [DBインデックス] が範囲外です。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "ivdb") {
		//VDB-セル値を初期化
		if(spl.size() == 5) {
			TS_CHECKINDEX(ints[1], 0, Game.DB.VDB.size()) {
				TS_CHECKINDEX(ints[2], 0, Game.DB.VDB[ints[1]]->GetSubDBCount()) {
					TS_CHECKINDEX(ints[3], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetRowCount()) {
						TS_CHECKINDEX(ints[4], 0, Game.DB.VDB[ints[1]]->GetSubDB(ints[2])->GetColumnCount()) {
							temp = Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString();
							Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->InitCellFromIndex(ints[3], ints[4]);
							::printf("\t%s -> %s\n", temp.c_str(), Game.DB.FDB[ints[1]]->GetSubDB(ints[2])->GetCellFromIndex(ints[3], ints[4])->ToString().c_str());
						} else {
							::puts("cmdError: [列番号] が範囲外です。");
						}
					} else {
						::puts("cmdError: [行番号] が範囲外です。");
					}
				} else {
					::puts("cmdError: [DBサブインデックス] が範囲外です。");
				}
			} else {
				::puts("cmdError: [DBインデックス] が範囲外です。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "sudb") {
		//UDB-セル値をセット
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
						::puts("cmdError: [列番号] が範囲外です。");
					}
				} else {
					::puts("cmdError: [行番号] が範囲外です。");
				}
			} else {
				::puts("cmdError: [UserDBID] が無効です。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "iudb") {
		//UDB-セル値を初期化
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
						::puts("cmdError: [列番号] が範囲外です。");
					}
				} else {
					::puts("cmdError: [行番号] が範囲外です。");
				}
			} else {
				::puts("cmdError: [UserDBID] が無効です。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "lvl") {
		//共通変数の値を列挙
		Game.DB.getVDBValue()->PrintData();
		return true;
	} else if(spl[0] == "svl") {
		//共通変数の値を変更
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
					::puts("cmdError: 無効な共通フラグのIDです。");
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
					::puts("cmdError: 無効な共通整数のIDです。");
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
					::puts("cmdError: 無効な共通文字変数のIDです。");
				}
			} else {
				::puts("cmdError: 無効な共通変数の種別記号です。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "mny") {
		//所持金変更
		if(spl.size() == 2) {
			int bval = Game.DB.getVDBParty()->Money;
			Game.DB.getVDBParty()->setMoney(Game.DB.getVDBParty()->getMoney() + ints[1]);
			::printf("\t%d -> %d\n", bval, Game.DB.getVDBParty()->Money);
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "itm") {
		//アイテム所持
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Item), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				while(ints[2] != 0) {
					if(ints[2] > 0) {
						//増やす
						Game.DB.getVDBParty()->AddItem(ints[1], 1);
						ints[2]--;
					} else if(ints[2] < 0) {
						//減らす
						Game.DB.getVDBParty()->DeleteItem(ints[1], 1);
						ints[2]++;
					}
				}
			} else {
				::puts("cmdError: 無効なアイテムIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "lchr") {
		//アクター情報列挙
		if(spl.size() == 1) {
			Game.DB.getVDBActor()->PrintData();
		} else if(spl.size() == 2) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				Game.DB.getVDBActor()->DB.GetFromID(ints[1])->PrintData();
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "seqp") {
		//装備品変更
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
									 (beforeID == -1) ? "(なし)" : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(beforeID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
									 "(なし)"
									 );
						} else {
							::puts("処理失敗");
						}
					} else if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ints[3]) != nullptr) {
						if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetEquip(ints[3])) {
							::printf("%s -> %s\n",
									 (beforeID == -1) ? "(なし)" : Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(beforeID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
									 Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Item)]->GetSubDB(0)->GetRowFromID(ints[3])->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str()
									 );
						}
					} else {
						::puts("cmdError: 無効なアイテムIDです。");
					}
				} else {
					::puts("cmdError: 無効な装備大種別IDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "scls") {
		//クラス設定
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Class), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetClass(ints[2]);
				} else {
					::puts("cmdError: 無効なクラスIDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "rcls") {
		//クラス解除
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Class), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Class)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseClass(ints[2])) {
						::puts("処理成功");
					} else {
						::puts("処理失敗");
					}
				} else {
					::puts("cmdError: 無効なクラスIDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "sskl") {
		//スキル設定
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Skill), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetSkill(ints[2]);
				} else {
					::puts("cmdError: 無効なスキルIDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "rskl") {
		//スキル解除
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Skill), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Skill)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseSkill(ints[2])) {
						::puts("処理成功");
					} else {
						::puts("処理失敗");
					}
				} else {
					::puts("cmdError: 無効なスキルIDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "sstt") {
		//間接効果設定
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::State), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SetState(ints[2], true);
				} else {
					::puts("cmdError: 無効な間接効果IDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "rstt") {
		//間接効果解除
		if(spl.size() == 3) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::State), 0, ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::State)]->GetSubDB(0)->GetRowFromID(ints[2]) != nullptr) {
					if(Game.DB.getVDBActor()->DB.GetFromID(ints[1])->ReleaseState(ints[2])) {
						::puts("処理成功");
					} else {
						::puts("処理失敗");
					}
				} else {
					::puts("cmdError: 無効な間接効果IDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "spar") {
		//パラメーター設定
		if(spl.size() == 4) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Element), static_cast<int>(TS::DB::DBElementIndices::Parameter), ints[2]);
				if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Element)]->GetSubDB(static_cast<int>(TS::DB::DBElementIndices::Parameter))->GetRowFromID(ints[2]) != nullptr) {
					::printf("%d -> %d\n", Game.DB.getVDBActor()->DB.GetFromID(ints[1])->Parameters.GetFromID(ints[2])->Value, ints[3]);
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->Parameters.GetFromID(ints[2])->Value = ints[3];
				} else {
					::puts("cmdError: 無効なパラメーターIDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "sval") {
		//アクター個別変数設定
		if(spl.size() == 4) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				ints[2] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::VDB), static_cast<int>(TS::DB::VDBIndices::Value), static_cast<int>(TS::DB::DBValueIndices::SelfActor), ints[2]);
				if(Game.DB.VDB[static_cast<int>(TS::DB::VDBIndices::Value)]->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfActor))->GetRowFromID(ints[2]) != nullptr) {
					::printf("%d -> %d\n", Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SelfValues.Get(ints[2])->Value, ints[3]);
					Game.DB.getVDBActor()->DB.GetFromID(ints[1])->SelfValues.Set(ints[2], ints[3]);
				} else {
					::puts("cmdError: 無効なアクター個別変数IDです。");
				}
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "lpt") {
		//パーティメンバー列挙
		Game.DB.getVDBParty()->PrintData();
		return true;
	} else if(spl[0] == "apt") {
		//パーティへ追加
		if(spl.size() == 2) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				Game.DB.getVDBParty()->AddMember(ints[1], -1);
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "dptid") {
		//パーティから離脱/ID
		if(spl.size() == 2) {
			ints[1] = Game.DB.GetFixedID(static_cast<int>(TS::DB::DBType::FDB), static_cast<int>(TS::DB::FDBIndices::Char), 0, ints[1]);
			if(Game.DB.FDB[static_cast<int>(TS::DB::FDBIndices::Char)]->GetSubDB(0)->GetRowFromID(ints[1]) != nullptr) {
				Game.DB.getVDBParty()->DeleteMemberFromID(ints[1]);
			} else {
				::puts("cmdError: 無効なパーティキャラIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "dptidx") {
		//パーティから離脱/Index
		if(spl.size() == 2) {
			Game.DB.getVDBParty()->DeleteMemberFromIndex(ints[1]);
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	}
	return false;
}


//####################  コマンド処理：マップシーン  ####################
bool CDebug::cmdMap(CMDARGS) {
	bool notMapScene = false;
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		notMapScene = true;
	}

	//パラメーターを整数値に変換したリストを作る
	string temp;
	vector<int> ints;
	for(auto& arg : spl) {
		ints.push_back(atoi(arg.c_str()));
	}

	if(spl[0] == "map") {
		//マップ変更
		if(notMapScene) {
			::puts("cmdError: 現在マップシーンではありません。");
		} else if(spl.size() == 4) {
			Game.getSceneMap()->MapData.LoadMap(spl[1], false);
			Game.getSceneMap()->MapData.Player.MapObject.Position.Set(ints[2], ints[3]);
			Game.getSceneMap()->MapData.AfterLoadMap();
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "pos") {
		//イベント位置移動
		if(notMapScene) {
			::puts("cmdError: 現在マップシーンではありません。");
		} else if(spl.size() == 4) {
			ints[1] = Game.getSceneMap()->MapData.GetEVFixedID(ints[1]);
			if(ints[1] == -1) {
				//主人公対象
				Game.getSceneMap()->MapData.Player.MapObject.Position.Set(ints[2], ints[3]);
			} else if(Game.getSceneMap()->MapData.EVExists(ints[1])) {
				//イベント対象
				Game.getSceneMap()->MapData.EVGetData(ints[1])->Position.Set(ints[2], ints[3]);
			} else {
				::puts("cmdError: 無効なイベントIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "sev") {
		//イベント操作
		if(notMapScene) {
			::puts("cmdError: 現在マップシーンではありません。");
		} else if(spl.size() == 4) {
			CSceneMap::CMapData::CMapCharObjectData* data = nullptr;
			CSceneMap::CMapData::CMapEventData::CMapEventPageData* ev = nullptr;
			ints[1] = Game.getSceneMap()->MapData.GetEVFixedID(ints[1]);
			if(ints[1] == -1) {
				//主人公対象
				data = &Game.getSceneMap()->MapData.Player.CharData;
			} else if(Game.getSceneMap()->MapData.EVExists(ints[1])) {
				//イベント対象
				data = Game.getSceneMap()->MapData.EVGetData(ints[1])->getCurrentPage();
				if(data == nullptr) {
					::puts("cmdError: 指定されたイベントは非出現状態です。");
				}
			} else {
				::puts("cmdError: 無効なイベントIDです。");
			}
			if(data != nullptr) {
				switch(ints[2]) {
					case 0:		//起動条件
						ev = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(data);
						if(ev != nullptr) {
							ev->TriggerType = ints[3];
						}
						break;
					case 1:		//日常移動
						ev = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(data);
						if(ev != nullptr) {
							ev->MoveType = ints[3];
						}
						break;
					case 2:		//方向
						data->Direction = ints[3];
						break;
					case 3:		//速度
						data->MoveSpeed = ints[3];
						break;
					case 4:		//頻度
						ev = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(data);
						if(ev != nullptr) {
							ev->MoveFrequency = ints[3];
						}
						break;
					case 5:		//静止アニメ
						data->EnabledStopAnim = ints[3];
						break;
					case 6:		//歩行アニメ
						data->EnabledWalkAnim = ints[3];
						break;
					case 7:		//すり抜け
						data->NoHit = ints[3];
						break;
					case 8:		//向き固定
						data->FixedDirection = ints[3];
						break;
					default:
						::puts("cmdError: 無効なパラメーター番号です。");
				}
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "dev") {
		//イベント実行
		if(notMapScene) {
			::puts("cmdError: 現在マップシーンではありません。");
		} else if(spl.size() == 2) {
			ints[1] = Game.getSceneMap()->MapData.GetEVFixedID(ints[1]);
			if(Game.getSceneMap()->MapData.EVExists(ints[1])) {
				Game.getSceneMap()->MapData.RunEV(ints[1], static_cast<int>(TS::Map::EventScriptType::Run), true);
			} else {
				::puts("cmdError: 無効なイベントIDです。");
			}
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	}
	return false;
}


//####################  コマンド処理：スクリプト  ####################
bool CDebug::cmdSQ(CMDARGS) {
	if(spl[0] == "sqf") {
		//スクリプトファイル実行
		if(spl.size() == 2) {
			Game.SQ.Do(spl[1], false);
		} else {
			::puts("cmdError: 引数の個数が不正です。");
		}
		return true;
	} else if(spl[0] == "sqc") {
		//即席コード実行
		auto pos = buf.find(" ");
		if(pos != string::npos) {
			string code = buf.substr(pos);
			if(code.size() > 0 && code.substr(code.size() - 1, 1) == ";") {
				Sqrat::Script tester;
				try {
					::puts("");		//空行を作る
					tester.CompileString(code);
					tester.Run();
					::puts("即席スクリプトコードの実行が完了しました。");
				} catch(...) {
					::puts("\ncmdError: 与えられたコードを実行できません。\n");
				}
			} else {
				::puts("cmdError: コードの末尾にはセミコロンが必要です。");
			}
		} else {
			::puts("cmdError: コードが含まれていません。");
		}
		CDebug::ActivateConsole();
		return true;
	}
	return false;
}


//####################  コンソールをアクティブにする  ####################
void CDebug::ActivateConsole() {
	::SetForegroundWindow(::GetConsoleWindow());
}


//####################  ゲームウィンドウをアクティブにする  ####################
void CDebug::ActivateGameWindow() {
	::SetForegroundWindow(::GetMainWindowHandle());
}


//####################  コンソールをクリアする  ####################
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


//####################  何かキーが押されるまで処理を止める  ####################
//返値は押されたキーの文字コード
int CDebug::PauseConsole() {
	::fflush(stdin);						//入力バッファのゴミを掃除する
	//return ::getchar();

	//一文字入力を受け付ける
	int ret = ::_getch();
	::putc('>', stdin);
	::putc(ret, stdin);
	::printf("\n");
	return ret;
}
