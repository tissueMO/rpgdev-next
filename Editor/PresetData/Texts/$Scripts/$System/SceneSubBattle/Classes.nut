//###################################################################
//		バトルシーン: クラス定義
//###################################################################

//アクター行動内容
class SQActionData {
	ActorIndex = -1;		//行動者インデックス
	TargetIndex = -1;		//対象者インデックス
	Command = -1;			//コマンド
	Option = 0;				//コマンドに付随するパラメーター
	EffectID = -1;			//エフェクトID
	Disabled = false;		//無効化された行動かどうか

	//コンストラクター
	constructor(actor, target = -1, command = -1, opt = -1, effectID = EmptyEquipEffectID) {
		this.ActorIndex = actor;
		this.TargetIndex = target;
		this.Command = command;
		this.Option = opt;
		this.EffectID = effectID;
		this.Disabled = false;
	}

	///[summary]見える形で出力する
	function Print() {
		print("\t" + this.ActorIndex
				 + " -> " + this.TargetIndex
				 + " / " + this.Command
				 + ":" + this.Option
				 + " / EffectID: " + this.EffectID
		);
	}
}

//ダメージ数値表示
class SQDamageEffectData {
	ActorIndex = -1;		//対象アクターのインデックス
	Value = 0;				//ダメージ量
	Type = -1;				//表示種別
	StartTimeMS = 0;		//開始時刻ミリ秒
	PositionY = 0;			//相対Y座標

	//コンストラクター
	constructor(idx, val, effectType) {
		this.ActorIndex = idx;
		this.Value = val;
		this.Type = effectType;
		this.StartTimeMS = Game.DXLib.GetNowCount();
	}

	//表示種別
	DamageEffectType = {
		HPDamage = 0,		//HPダメージ
		HPHeal = 1,			//HP回復
		MPDamage = 2,		//MPダメージ
		MPHeal = 3,			//MP回復
		Critical = 4,		//クリティカルヒット
		Miss = 5,			//ミス
		Reflect = 6,		//リフレクト
	};
}

//振動情報
class SQShakeData {
	Doing = false;			//実行中かどうか
	PositionX = 0;			//相対X座標
	PositionY = 0;			//相対Y座標
	Strength = 0;			//強さ
	FrameCounter = 0;		//フレームカウンター

	///[summary]振動を開始する
	///[par]振動の強さ
	function StartShake(strength) {
		this.Strength = strength;
		this.FrameCounter = 0;
		this.Doing = true;
	}

	///[summary]振動を処理する
	function DoShake() {
		this.FrameCounter++;
		this.PositionX = this.Strength * sin((PI / 180) * this.FrameCounter * 90/*振動定数*/);
		this.PositionY = this.Strength * cos((PI / 180) * this.FrameCounter * 90/*振動定数*/);
		if(this.FrameCounter % 4 == 0) {
			this.Strength--;		//減衰させる
			if(this.Strength <= 1) {
				this.PositionX = 0;
				this.PositionY = 0;
				this.Doing = false;		//終了
			}
		}
	}
}
