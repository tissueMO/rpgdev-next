using System;
using System.Drawing;

namespace Editor.CtrlComponent.Map {
    using Module;

    public partial class mgrMapObject {
        public partial class MapOneData {
            public partial class EventOneData {

                /// <summary>
                /// イベントページの単一データ
                /// </summary>
                public class EventOnePage : ICloneable {
                    /// <summary>
                    /// イベントスクリプト
                    /// </summary>
                    public string[] SQ {
                        get;
                        private set;
                    } = new string[Common.GetEnumCount<Map.EventScriptType>()];

                    /// <summary>
                    /// 起動条件
                    /// </summary>
                    public Map.EventTriggerType Trigger {
                        get; set;
                    } = Map.EventTriggerType.None;

                    /// <summary>
                    /// 日常移動
                    /// </summary>
                    public Map.EventMoveType MoveType {
                        get; set;
                    } = Map.EventMoveType.None;

                    /// <summary>
                    /// 方向
                    /// </summary>
                    public Map.Direction4 Direction {
                        get; set;
                    } = Map.Direction4.South;

                    /// <summary>
                    /// 移動速度
                    /// </summary>
                    public Map.Speed MoveSpeed {
                        get; set;
                    } = Map.Speed.Low;

                    /// <summary>
                    /// 移動頻度
                    /// </summary>
                    public Map.Speed MoveFrequency {
                        get; set;
                    } = Map.Speed.Low;

                    /// <summary>
                    /// グラフィック
                    /// </summary>
                    public string Graphic {
                        get; set;
                    } = "";

                    /// <summary>
                    /// 既定アニメ状態
                    /// </summary>
                    public Map.EventAnimation AnimationPattern {
                        get; set;
                    } = Map.EventAnimation.Stop;

                    /// <summary>
                    /// 描画優先度
                    /// </summary>
                    public Map.EventDrawPriority DrawPriority {
                        get; set;
                    } = Map.EventDrawPriority.Middle;

                    /// <summary>
                    /// イベント座標を中心とした接触範囲
                    /// </summary>
                    public Size HitRect {
                        get; set;
                    } = Size.Empty;

                    /// <summary>
                    /// 静止時のアニメーション有無
                    /// </summary>
                    public bool StoppingAnimation {
                        get; set;
                    } = true;

                    /// <summary>
                    /// 移動時のアニメーション有無
                    /// </summary>
                    public bool MovingAnimation {
                        get; set;
                    } = true;

                    /// <summary>
                    /// すり抜けるかどうか
                    /// </summary>
                    public bool NoHit {
                        get; set;
                    } = false;

                    /// <summary>
                    /// 方向を固定するかどうか
                    /// </summary>
                    public bool FixedDirection {
                        get; set;
                    } = false;

                    /// <summary>
                    /// ページのコピーを返します。
                    /// </summary>
                    public object Clone() {
                        var newObject = this.MemberwiseClone() as EventOnePage;

                        //スクリプトをコピーする
                        newObject.SQ = new string[Common.GetEnumCount<Map.EventScriptType>()];
                        for (var i = 0; i < this.SQ.Length; i++) {
                            newObject.SQ[i] = this.SQ[i];
                        }
                        return newObject;
                    }
                }

            }
        }
    }
}
