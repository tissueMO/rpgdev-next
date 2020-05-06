//###################################################################
//		エディター・ゲームプログラム間で共通する定数群
//###################################################################
using System;
using System.Collections.Generic;


namespace Editor.Module {
    /// <summary>
    /// DXライブラリ関連
    /// </summary>
    public static partial class Media {
        /// <summary>
        /// DXライブラリが対応する補間方法
        /// </summary>
        public enum DXInterpolation {
            Nearest,
            Bilinear,
        }

        /// <summary>
        /// 合成方法の種類
        /// </summary>
        public enum BlendMode {
            Alpha,
            Add,
            Subtract,
            Multiply,
            Invsrc,
        }

        /// <summary>
        /// サウンド情報を文字列の形式にしたときのデータの並び順
        /// </summary>
        private enum DataOrder {
            FileName,
            Volume,
            Pitch,
            Panpot,
        }
    }
}


namespace Editor.Module.DB {
    /// <summary>
    /// データベース関連
    /// </summary>
    public static partial class Database {
        /// <summary>
        /// 標準データベースの基本列のインデックス
        /// </summary>
        public enum DBDefaultColumnIndices {
            FixedID,
            VisibleID,
            Name,
            Count
        }

        /// <summary>
        /// 曲線データの順序
        /// </summary>
        public enum DBCurveDataIndices {
            IDName,
            Base,
            Coefficient,
            Index,
            Count,
        }

        /// <summary>
        /// 基本要素DBの並び順
        /// </summary>
        public enum DBElementIndices {
            Parameter,
            Attribute,
            SkillType,
            ItemType,
            ItemSubType,
            Count
        }

        /// <summary>
        /// 変数DBの並び順
        /// </summary>
        public enum DBValueIndices {
            CommonFlag,
            CommonInteger,
            CommonString,
            SelfActor,
            SelfClass,
            SelfUse,
            SelfState,
            SelfUnit,
            SelfEvent,
            Count
        }

        /// <summary>
        /// 初期設定DBの並び順
        /// </summary>
        public enum DBInitIndices {
            Party,
            PartyButton,
            Item,
            ItemButton,
            Money,
        }

        /// <summary>
        /// 素材DBの並び順
        /// </summary>
        public enum DBMaterialIndices {
            Face,
            Graphics,
            Sound,
            Font,
            Count
        }

        /// <summary>
        /// ユニットDBの各エネミー情報の並び順
        /// </summary>
        public enum DBUnitEnemyIndices {
            FixedID,
            Position,
        }

        /// <summary>
        /// 標準列の後ろに続くエフェクトDBの列順序
        /// </summary>
        public enum DBEffectColumnIndices {
            FrameLength,
            DrawDest,
            Layers,
        }

        /// <summary>
        /// 標準列の後ろに続くタイルセットDBの列順序
        /// </summary>
        public enum DBTilesetColumnIndices {
            Comment,
            AutoTiles,
            ObjTiles,
        }

        /// <summary>
        /// エフェクト描画対象の範囲
        /// </summary>
        public enum DBEffectViewPosition {
            Each,           //個別
            OnlyOne,        //一括
            CenterScreen,   //画面中心
        }
    }
}


namespace Editor.Module.DB {
    /// <summary>
    /// データベースに関するべた書きせざるを得ない部分
    /// </summary>
    public static partial class DBLiteral {
        /// <summary>
        /// 直接効果の典型リストを返します。
        /// </summary>
        public static List<string> GetListUseType() {
            var list = new List<string> {
                "スクリプト指定",    //Script
                "物理攻撃",           //Physic
                "魔法攻撃",           //Magic
                "回復",         //Heal
                "補助"         //Utility
            };
            return list;
        }

        /// <summary>
        /// 場面制限のリストを返します。
        /// </summary>
        public static List<string> GetListSceneLimit() {
            var list = new List<string> {
                "常にＮＧ",           //AlwaysNG
                "移動中のみＯＫ",    //OnlyMap
                "戦闘中のみＯＫ",    //OnlyBattle
                "常にＯＫ"           //AlwaysOK
            };
            return list;
        }

        /// <summary>
        /// 対象種別のリストを返します。
        /// </summary>
        public static List<string> GetListTargetType() {
            var list = new List<string> {
                "なし",                 //None
                "味方単体",                   //OneParty
                "味方単体（戦闘不能含む）",   //OnePartyWithDead
                "味方全体",                   //AllParty
                "味方全体（戦闘不能含む）",   //AllPartyWithDead
                "敵単体",                    //OneEnemy
                "敵全体",                    //AllEnemy
                "場所"                 //Place
            };
            return list;
        }

        /// <summary>
        /// 間接効果の制約リストを返します。
        /// </summary>
        public static List<string> GetListStateLimit() {
            var list = new List<string> {
                "なし",                 //None
                "戦闘不能",                   //KO
                "行動不能"                   //ForbiddenAction
            };
            return list;
        }

        /// <summary>
        /// 間接効果の解除条件のリストを返します。
        /// </summary>
        public static List<string> GetListStateSolve() {
            var list = new List<string> {
                "解除しない",              //None
                "ダメージを受けたら",      //Damamge
                "ターン経過",              //Turn
                "マップ上の移動"           //Walk
            };
            return list;
        }

        /// <summary>
        /// フォントの描画方法のリストを返します。
        /// </summary>
        public static List<string> GetListFontDrawing() {
            var list = new List<string> {
                "通常",                 //Normal
                "縁付き",                    //Edge
                "アンチエイリアス",           //AS
                "縁付き＋アンチエイリアス"   //EdgeAS
            };
            return list;
        }

        /// <summary>
        /// アドオン情報のリストを返します。
        /// </summary>
        public static List<Database.DBAddonType> GetListAddon() {
            var list = new List<Database.DBAddonType> {
                new Database.DBAddonType("アクター個別変数 [＋]", new Database.DBAddress(Database.DBIndices.Value, (int) Database.DBValueIndices.SelfActor), true, false),        //SelfValuePlus
                new Database.DBAddonType("パラメーター [＋]", new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Parameter), true, false),            //ParameterPlus
                new Database.DBAddonType("属性耐性を設定 [＋％]", new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.Attribute), true, false),         //AttributeResistanceSet
                new Database.DBAddonType("間接効果耐性を設定 [＋％]", new Database.DBAddress(Database.DBIndices.State), true, false),                                             //StateResistanceSet
                new Database.DBAddonType("装備大種別を固定", new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemType), true, true),          //LockEquipType
                new Database.DBAddonType("装備大種別を切替", new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemType), true, true),          //SetEquipType
                new Database.DBAddonType("装備小種別を切替", new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.ItemSubType), true, true),       //SetEquipSubType
                new Database.DBAddonType("スキル使用可否", new Database.DBAddress(Database.DBIndices.Skill), true, true),                                                    //SetSkillUse
                new Database.DBAddonType("スキル種別の使用可否", new Database.DBAddress(Database.DBIndices.Element, (int) Database.DBElementIndices.SkillType), true, true)       //SetSkillType
            };
            return list;
        }
    }
}


namespace Editor.Module {

    /// <summary>
    /// マップ関連
    /// </summary>
    public static partial class Map {
        public const int TileColumnCount = 8;       //タイルセットの横に並べるタイル数
        public const int AutoTileRowLength = 16;     //タイルセットに並べられるオートタイルの行数
        public const int MapSizeMaxLength = 500;    //マップの一辺あたりの最大サイズ

        /// <summary>
        /// レイヤー順序
        /// </summary>
        public enum Layer {
            Low,
            Middle,
            High,
            Shadow,
            Event,
        }

        /// <summary>
        /// １タイルを 1/4 単位にしたときの位置
        /// </summary>
        public enum QuarterTile {
            LeftTop,
            RightTop,
            LeftBottom,
            RightBottom,
            Count
        }

        /// <summary>
        /// ４方角
        /// </summary>
        public enum Direction4 {
            South,
            West,
            East,
            North,
            None,
        }

        /// <summary>
        /// ８方角
        /// </summary>
        public enum Direction8 {
            SouthWest,
            South,
            SouthEast,
            West,
            None,
            East,
            NorthWest,
            North,
            NorthEast,
        }

        /// <summary>
        /// 段階的な速度
        /// </summary>
        public enum Speed {
            VeryLow,
            Low,
            Normal,
            High,
            VeryHigh,
            Moment,
        }

        /// <summary>
        /// タイル情報の種別
        /// </summary>
        public enum TileDataType {
            Hit,
            InOut,
            Priority,
            GroupNumber,
        }

        /// <summary>
        /// 当たり判定の種別
        /// </summary>
        public enum HitType {
            OK,
            NG,
            Depend,
        }

        /// <summary>
        /// 通行設定の種別：ビット演算を利用
        /// </summary>
        [Flags]
        public enum InOutType {
            None = 0,
            OKBottom = 2,
            OKLeft = 4,
            OKRight = 8,
            OKTop = 16,
        }

        /// <summary>
        /// 描画優先度
        /// </summary>
        public enum TileDrawPriority {
            Normal,
            SemiTransparent,    //下半身が半透明になる
            SemiTopMost,        //下半身が隠れる
            TopMost,            //最前面表示
        }

        /// <summary>
        /// オートタイルの接続方法
        /// </summary>
        public enum AutoTilePattern {
            Isolate,
            Vertical,
            Horizontal,
            FourSide,
            AllSide,
        }

        /// <summary>
        /// マップのループ方向
        /// </summary>
        public enum MapLoopType {
            None,
            Vertical,
            Horizontal,
            Both,
        }

        /// <summary>
        /// 遠景・フォグのスクロール方法
        /// </summary>
        public enum ScrollType {
            None,
            FollowChar,
            LeftDown,
            Down,
            RightDown,
            Left,
            Right,
            LeftUp,
            Up,
            RightUp,
        }

        /// <summary>
        /// イベントスクリプトの種別
        /// </summary>
        public enum EventScriptType {
            Page,           //ページ条件
            Trigger,        //起動条件
            Move,           //日常移動
            Run,            //実行内容
        }

        /// <summary>
        /// イベント起動条件の種別
        /// </summary>
        public enum EventTriggerType {
            None,
            PressEnterKey,
            Push,
            Pushed,
            Auto,
            AfterMoveMap,
            Parallel,
        }

        /// <summary>
        /// イベント日常移動の種別
        /// </summary>
        public enum EventMoveType {
            None,
            Random,
            Near,
            Away,
            Script,
        }

        /// <summary>
        /// イベントの描画優先度
        /// </summary>
        public enum EventDrawPriority {
            Low,
            Middle,
            High,
        }

        /// <summary>
        /// イベントのアニメーションパターン
        /// </summary>
        public enum EventAnimation {
            Move1,
            Stop,
            Move2,
            Count,
        }

        /// <summary>
        /// イベントの日常移動の指示内容
        /// </summary>
        public enum EventMoveTask {
            Direction,          //向き変更
            Forward,            //前へ１歩進む
            Backward,           //後ろへ１歩下がる
            Move,               //指定した方向へ１歩進む
            MoveRandom,         //ランダムな方向へ１歩進む
            MoveToPlayer,       //主人公に１歩近づく
            MoveAgainstPlayer,  //主人公から１歩遠ざかる
            TurnLeft,           //左を向く
            TurnRight,          //右を向く
            Turn180,            //反対側を向く
            TurnRandom,         //ランダムな方向を向く
            TurnToPlayer,       //主人公を向く
            TurnAgainstPlayer,  //主人公の反対を向く
            Wait,               //ウェイトＮミリ秒
            SetMoveSpeed,       //移動速度変更
            SetMoveFrequency,   //移動頻度変更
            EnabledWalkAnim,    //歩行アニメーション
            EnabledStopAnim,    //足踏みアニメーション
            NoHit,              //すり抜け
            FixedDirection,     //向き固定
            Hide,               //非表示
            ChangeGraphics,     //グラフィック変更
            SQ,                 //外部スクリプト呼び出し
            LockRoute,          //ルート固定（移動できない場合、スキップせずに待つようにする）
            Repeat,             //以上繰り返し
        };
    }
}
