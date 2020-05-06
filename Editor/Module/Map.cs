using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Editor.Module {
    public static partial class Map {
        public const int MaxEVPageCount = 10;       //イベントページの最大数
        public const int ShadowTransparent = 120;   //影の透明度
        public const int WheelScroolValue = 5;      //マウスホイールでの移動量

        /// <summary>
        /// マップ上でマウスホイールされたときの共通処理
        /// </summary>
        public static void MapEditor_MouseWheel(MouseEventArgs e, HScrollBar hsc, VScrollBar vsc, Control container, CtrlComponent.Map.mgrMapObject mgr, ToolStripComboBox tolScale) {
            var value = -e.Delta;

            if ((Control.ModifierKeys & Keys.Control) == Keys.Control) {
                //Ctrlキーが押されていたらスケール変更する
                if (value < 0) {
                    tolScale.SelectedIndex = (tolScale.SelectedIndex == 0) ? 0 : tolScale.SelectedIndex - 1;
                } else {
                    tolScale.SelectedIndex = (tolScale.SelectedIndex == tolScale.Items.Count - 1) ? tolScale.SelectedIndex = tolScale.Items.Count - 1 : tolScale.SelectedIndex + 1;
                }
            } else {
                //スクロールを行う
                if (!Control.ModifierKeys.HasFlag(Keys.Shift)) {
                    //縦スクロール
                    if (vsc.Value + value > vsc.Maximum) {
                        vsc.Value = vsc.Maximum;
                    } else if (vsc.Value + value < 0) {
                        vsc.Value = 0;
                    } else {
                        vsc.Value = vsc.Value + value;
                    }
                } else {
                    //横スクロール (Shiftキー押下状態)
                    if (hsc.Value + value > hsc.Maximum) {
                        hsc.Value = hsc.Maximum;
                    } else if (hsc.Value + value < 0) {
                        hsc.Value = 0;
                    } else {
                        hsc.Value = hsc.Value + value;
                    }
                }
                Map.MapEditor_Resize(hsc, vsc, container, mgr);
            }
        }

        /// <summary>
        /// マップの指定したタイル座標が見えるところまでスクロールします。
        /// </summary>
        public static void MapEditor_ScrollToCursor(HScrollBar hsc, VScrollBar vsc, Control container, CtrlComponent.Map.mgrMapObject mgr) {
            var pos = new Point(mgr.Cursor.X * mgr.ScaledTileSize, mgr.Cursor.Y * mgr.ScaledTileSize);
            pos.Offset(-container.Width / 2, -container.Height / 2);

            if (pos.X < 0) {
                hsc.Value = 0;
            } else if (hsc.Maximum < pos.X) {
                hsc.Value = hsc.Maximum;
            } else {
                hsc.Value = pos.X;
            }

            if (pos.Y < 0) {
                vsc.Value = 0;
            } else if (vsc.Maximum < pos.Y) {
                vsc.Value = vsc.Maximum;
            } else {
                vsc.Value = pos.Y;
            }

            Map.MapEditor_Resize(hsc, vsc, container, mgr);
        }

        /// <summary>
        /// マップ表示領域が変更されたときの共通処理
        /// </summary>
        public static void MapEditor_Resize(HScrollBar hsc, VScrollBar vsc, Control container, CtrlComponent.Map.mgrMapObject mgr) {
            if (mgr == null) {
                return;
            }

            if (!mgr.GetMapPreviewScrollSize(container.Size, out var x, out var y)) {
                //処理できなかった場合は中断する
                return;
            } else {
                hsc.Maximum = x;
                hsc.LargeChange = mgr.ScaledTileSize;
                vsc.Maximum = y;
                vsc.LargeChange = mgr.ScaledTileSize;
            }

            mgr.ViewRange = new Rectangle(
                hsc.Value,
                vsc.Value,
                (mgr.MapData.MapSize.Width * mgr.ScaledTileSize < container.Width) ? mgr.MapData.MapSize.Width * mgr.ScaledTileSize : container.Width,
                (mgr.MapData.MapSize.Height * mgr.ScaledTileSize < container.Height) ? mgr.MapData.MapSize.Height * mgr.ScaledTileSize : container.Height
            );
        }

        /// <summary>
        /// マップの最大サイズ
        /// </summary>
        public static Size MaxMapSize {
            get {
                return new Size(Map.MapSizeMaxLength, Map.MapSizeMaxLength);
            }
        }

        /// <summary>
        /// マップレイヤーをテキストにして返します。
        /// </summary>
        public static string[] GetLayerList() {
            var list = new string[Common.GetEnumCount<Layer>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((Layer) i) {
                    case Layer.Low:
                        list[i] = "下層";
                        break;
                    case Layer.Middle:
                        list[i] = "中層";
                        break;
                    case Layer.High:
                        list[i] = "上層";
                        break;
                    case Layer.Shadow:
                        list[i] = "影層";
                        break;
                    case Layer.Event:
                        list[i] = "イベント";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// ４方角をテキストにして返します。
        /// 「中立」を含めるかどうかを指定することができます。
        /// </summary>
        public static string[] GetDirection4List(bool containNone) {
            var list = new List<string>();
            for (var i = 0; i < Common.GetEnumCount<Direction4>(); i++) {
                switch ((Direction4) i) {
                    case Direction4.East:
                        list.Add("東");
                        break;
                    case Direction4.North:
                        list.Add("北");
                        break;
                    case Direction4.South:
                        list.Add("南");
                        break;
                    case Direction4.West:
                        list.Add("西");
                        break;
                    case Direction4.None:
                        if (containNone) {
                            list.Add("中立");
                        }
                        break;
                }
            }
            return list.ToArray();
        }

        /// <summary>
        /// ８方角をテキストにして返します。
        /// </summary>
        public static string[] GetDirection8List(bool containNone) {
            var list = new string[Common.GetEnumCount<Direction8>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((Direction8) i) {
                    case Direction8.East:
                        list[i] = "東";
                        break;
                    case Direction8.North:
                        list[i] = "北";
                        break;
                    case Direction8.NorthEast:
                        list[i] = "北東";
                        break;
                    case Direction8.NorthWest:
                        list[i] = "北西";
                        break;
                    case Direction8.South:
                        list[i] = "南";
                        break;
                    case Direction8.SouthEast:
                        list[i] = "南東";
                        break;
                    case Direction8.SouthWest:
                        list[i] = "南西";
                        break;
                    case Direction8.West:
                        list[i] = "西";
                        break;
                    case Direction8.None:
                        list[i] = "(中立)";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// 段階的な速度をテキストにして返します。
        /// </summary>
        public static string[] GetSpeedList() {
            var list = new string[Common.GetEnumCount<Speed>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((Speed) i) {
                    case Speed.VeryLow:
                        list[i] = "すごく遅い";
                        break;
                    case Speed.Low:
                        list[i] = "遅い";
                        break;
                    case Speed.Normal:
                        list[i] = "標準";
                        break;
                    case Speed.High:
                        list[i] = "速い";
                        break;
                    case Speed.VeryHigh:
                        list[i] = "すごく速い";
                        break;
                    case Speed.Moment:
                        list[i] = "瞬間";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// マップのループ方向をテキストにして返します。
        /// </summary>
        public static string[] GetMapLoopTypeList() {
            var list = new string[Common.GetEnumCount<MapLoopType>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((MapLoopType) i) {
                    case MapLoopType.None:
                        list[i] = "無効";
                        break;
                    case MapLoopType.Vertical:
                        list[i] = "縦方向";
                        break;
                    case MapLoopType.Horizontal:
                        list[i] = "横方向";
                        break;
                    case MapLoopType.Both:
                        list[i] = "縦・横方向";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// 遠景・フォグのスクロール方法をテキストにして返します。
        /// </summary>
        public static string[] GetScrollTypeList() {
            var list = new string[Common.GetEnumCount<ScrollType>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((ScrollType) i) {
                    case ScrollType.None:
                        list[i] = "無効";
                        break;
                    case ScrollType.FollowChar:
                        list[i] = "キャラクター移動に伴う";
                        break;
                    case ScrollType.LeftDown:
                        list[i] = "左下方向";
                        break;
                    case ScrollType.Down:
                        list[i] = "上方向";
                        break;
                    case ScrollType.RightDown:
                        list[i] = "右下方向";
                        break;
                    case ScrollType.Left:
                        list[i] = "左方向";
                        break;
                    case ScrollType.Right:
                        list[i] = "右方向";
                        break;
                    case ScrollType.LeftUp:
                        list[i] = "左上方向";
                        break;
                    case ScrollType.Up:
                        list[i] = "上方向";
                        break;
                    case ScrollType.RightUp:
                        list[i] = "右上方向";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// イベント起動条件の種別をテキストにして返します。
        /// </summary>
        public static string[] GetEventTriggerTypeList() {
            var list = new string[Common.GetEnumCount<EventTriggerType>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((EventTriggerType) i) {
                    case EventTriggerType.None:
                        list[i] = "起動しない";
                        break;
                    case EventTriggerType.PressEnterKey:
                        list[i] = "決定キー押下";
                        break;
                    case EventTriggerType.Push:
                        list[i] = "主人公から接触";
                        break;
                    case EventTriggerType.Pushed:
                        list[i] = "イベントから接触";
                        break;
                    case EventTriggerType.Auto:
                        list[i] = "自動";
                        break;
                    case EventTriggerType.AfterMoveMap:
                        list[i] = "マップ移動直後";
                        break;
                    case EventTriggerType.Parallel:
                        list[i] = "並列実行";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// イベント日常移動の種別をテキストにして返します。
        /// </summary>
        public static string[] GetEventMoveTypeList() {
            var list = new string[Common.GetEnumCount<EventMoveType>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((EventMoveType) i) {
                    case EventMoveType.None:
                        list[i] = "移動しない";
                        break;
                    case EventMoveType.Random:
                        list[i] = "ランダム";
                        break;
                    case EventMoveType.Near:
                        list[i] = "主人公に近づく";
                        break;
                    case EventMoveType.Away:
                        list[i] = "主人公から遠ざかる";
                        break;
                    case EventMoveType.Script:
                        list[i] = "ユーザー定義";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// イベントの描画優先度をテキストにして返します。
        /// </summary>
        public static string[] GetEventDrawPriorityList() {
            var list = new string[Common.GetEnumCount<EventDrawPriority>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((EventDrawPriority) i) {
                    case EventDrawPriority.Low:
                        list[i] = "主人公の下";
                        break;
                    case EventDrawPriority.Middle:
                        list[i] = "主人公と同じ";
                        break;
                    case EventDrawPriority.High:
                        list[i] = "主人公の上";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// 当たり判定の種別をテキストにして返します。
        /// </summary>
        public static string[] GetTileHitTypeList() {
            var list = new string[Common.GetEnumCount<HitType>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((HitType) i) {
                    case HitType.OK:
                        list[i] = "○";
                        break;
                    case HitType.NG:
                        list[i] = "×";
                        break;
                    case HitType.Depend:
                        list[i] = "↓";
                        break;
                }
            }
            return list;
        }

        /// <summary>
        /// 描画優先度の種別をテキストにして返します。
        /// </summary>
        public static string[] GetTileDrawPriorityList() {
            var list = new string[Common.GetEnumCount<TileDrawPriority>()];
            for (var i = 0; i < list.Length; i++) {
                switch ((TileDrawPriority) i) {
                    case TileDrawPriority.Normal:
                        list[i] = "";
                        break;
                    case TileDrawPriority.SemiTopMost:
                        list[i] = "▲";
                        break;
                    case TileDrawPriority.SemiTransparent:
                        list[i] = "□";
                        break;
                    case TileDrawPriority.TopMost:
                        list[i] = "★";
                        break;
                }
            }
            return list;
        }
    }
}
