using DxLibDLL;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace Editor.CtrlComponent.Database {
    using Module;
    using Module.DB;
    using Properties;
    using AllDB = Dictionary<Module.DB.Database.DBIndices, CtrlComponent.Database.ctlDBBase>;

    public partial class mgrDBEffect {

        /// <summary>
        /// DXライブラリの描画対象オブジェクト
        /// </summary>
        public class EffectDrawObject : Media.DrawTarget {
            private const int FlashSpeed = 16;
            private readonly AllDB dbList;
            private Point screenOffset = Point.Empty;
            private bool editorMode = true;
            private readonly mgrDBEffect parent;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public EffectDrawObject(AllDB dbList, Control ctl, mgrDBEffect parent) : base(ctl, Media.DXInterpolation.Bilinear) {
                this.dbList = dbList;
                this.parent = parent;
            }

            /// <summary>
            /// 現在表示しているフレーム
            /// </summary>
            public int FrameNumber {
                get; set;
            } = -1;

            /// <summary>
            /// 現在対象となっているエフェクトデータ
            /// </summary>
            public EffectOneData TargetEffectData {
                get; set;
            }

            /// <summary>
            /// エフェクトを描画します。
            /// </summary>
            public override void Draw() {
                var scaledScreenSize = new Size(mgrDBEffect.ScreenSize.Width, mgrDBEffect.ScreenSize.Height);
                ViewScale.ApplyRateToSize(ref scaledScreenSize, this.parent.Scale);
                DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);

                //背景の格子
                if (this.editorMode) {
                    this.DrawGridPattern();
                }

                //ターゲットグラフィック
                DX.GetGraphSize(dxTargetHdl, out var x, out var y);
                var tgtSize = new Size(x, y);
                ViewScale.ApplyRateToSize(ref tgtSize, this.parent.Scale);
                DX.DrawExtendGraph(
                    scaledScreenSize.Width / 2 - tgtSize.Width / 2,
                    scaledScreenSize.Height / 2 - tgtSize.Height / 2,
                    scaledScreenSize.Width / 2 + tgtSize.Width / 2,
                    scaledScreenSize.Height / 2 + tgtSize.Height / 2,
                    dxTargetHdl, DX.TRUE
                );

                //対象フレームの各レイヤー
                if (this.TargetEffectData != null && this.FrameNumber != -1) {
                    foreach (var layer in this.TargetEffectData.Layers) {
                        var trans = 255 - layer.Frames[this.FrameNumber].Transparent;
                        if (this.editorMode && this.TargetEffectData.CurrentLayer != layer) {
                            trans -= 150;       //非アクティブレイヤーはさらに透明度を落とす
                        }

                        //ブレンド設定適用
                        switch (layer.Frames[this.FrameNumber].Blend) {
                            case Media.BlendMode.Alpha:
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, trans);
                                break;
                            case Media.BlendMode.Add:
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_ADD, trans);
                                break;
                            case Media.BlendMode.Subtract:
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_SUB, trans);
                                break;
                            case Media.BlendMode.Multiply:
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_MULA, trans);
                                break;
                            case Media.BlendMode.Invsrc:
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_INVSRC, trans);
                                break;
                        }

                        //描画処理
                        DX.DrawRectRotaGraph(
                            scaledScreenSize.Width / 2 + layer.Frames[this.FrameNumber].ViewPosition.X,
                            scaledScreenSize.Height / 2 + layer.Frames[this.FrameNumber].ViewPosition.Y,
                            layer.Frames[this.FrameNumber].SectionPosition.X * layer.SectionSize.Width,
                            layer.Frames[this.FrameNumber].SectionPosition.Y * layer.SectionSize.Height,
                            layer.SectionSize.Width,
                            layer.SectionSize.Height,
                            layer.Frames[this.FrameNumber].Extend / 100.0 * ViewScale.GetRate(this.parent.Scale),
                            Math.PI / 180 * layer.Frames[this.FrameNumber].Angle,
                            layer.dxGraphHdl,
                            DX.TRUE,
                            DX.FALSE
                        );

                        //レイヤー枠とファイル名を描画
                        if (this.editorMode) {
                            if (this.TargetEffectData.CurrentLayer != layer) {
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, 100);
                            } else {
                                DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);
                            }
                            DX.DrawBox(
                                (int) (scaledScreenSize.Width / 2 + layer.Frames[this.FrameNumber].ViewPosition.X - layer.SectionSize.Width / 2 * layer.Frames[this.FrameNumber].Extend * ViewScale.GetRate(this.parent.Scale) / 100),
                                (int) (scaledScreenSize.Height / 2 + layer.Frames[this.FrameNumber].ViewPosition.Y - layer.SectionSize.Height / 2 * layer.Frames[this.FrameNumber].Extend * ViewScale.GetRate(this.parent.Scale) / 100),
                                (int) (scaledScreenSize.Width / 2 + layer.Frames[this.FrameNumber].ViewPosition.X + layer.SectionSize.Width / 2 * layer.Frames[this.FrameNumber].Extend * ViewScale.GetRate(this.parent.Scale) / 100),
                                (int) (scaledScreenSize.Height / 2 + layer.Frames[this.FrameNumber].ViewPosition.Y + layer.SectionSize.Height / 2 * layer.Frames[this.FrameNumber].Extend * ViewScale.GetRate(this.parent.Scale) / 100),
                                DX.GetColor(0, 255, 0),
                                DX.FALSE
                            );
                            DX.DrawString(
                                (int) (scaledScreenSize.Width / 2 + layer.Frames[this.FrameNumber].ViewPosition.X - layer.SectionSize.Width / 2 * layer.Frames[this.FrameNumber].Extend * ViewScale.GetRate(this.parent.Scale) / 100),
                                (int) (scaledScreenSize.Height / 2 + layer.Frames[this.FrameNumber].ViewPosition.Y - layer.SectionSize.Height / 2 * layer.Frames[this.FrameNumber].Extend / 100 * ViewScale.GetRate(this.parent.Scale)) - 20,
                                Path.GetFileName(layer.FileName),
                                DX.GetColor(255, 255, 255)
                            );
                        }
                    }
                }
                DX.SetDrawBlendMode(DX.DX_BLENDMODE_NOBLEND, -1);

                //中心線
                if (this.editorMode) {
                    DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, 150);
                    DX.DrawLine(0, scaledScreenSize.Height / 2, scaledScreenSize.Width, scaledScreenSize.Height / 2, DX.GetColor(200, 200, 200));
                    DX.DrawLine(scaledScreenSize.Width / 2, 0, scaledScreenSize.Width / 2, scaledScreenSize.Height, DX.GetColor(200, 200, 200));
                }

                //画面枠
                DX.DrawBox(this.screenOffset.X, this.screenOffset.Y, scaledScreenSize.Width, scaledScreenSize.Height, DX.GetColor(255, 255, 255), DX.FALSE);
            }

            public override void LoadMaterial() {
                base.LoadMaterial();

                //グラフィック
                dxTargetHdl = DX.LoadGraph(ProjectManager.ProjectPath + Settings.Default.DB_EffectTargetPicture);
                if (this.TargetEffectData != null) {
                    foreach (var layer in this.TargetEffectData.Layers) {
                        layer.dxGraphHdl = DX.LoadGraph(ProjectManager.ProjectPath + layer.FileName);
                    }
                }
            }

            public void LoadMaterialSounds() {
                //サウンド
                foreach (var order in this.TargetEffectData.Orders) {
                    if (order.Type == EffectOneData.EffectOrder.OrderType.Sound) {
                        //サウンドファイルは元のDBを参照する
                        //Media.SetSoundData(
                        //	order.SoundObj,
                        //	Database.GetFileNameFromFixedID(this.DBList[Database.DBIndices.Material].DBs[(int)Database.DBMaterialIndices.Sound], int.Parse(order.Options[(int)EffectOneData.EffectOrder.SoundOptionIndices.FixedID])),
                        //	int.Parse(order.Options[(int)EffectOneData.EffectOrder.SoundOptionIndices.Volume]),
                        //	int.Parse(order.Options[(int)EffectOneData.EffectOrder.SoundOptionIndices.Pitch]),
                        //	int.Parse(order.Options[(int)EffectOneData.EffectOrder.SoundOptionIndices.Pan])
                        //);
                        Media.SetSoundData(
                            ref order.dxHandle,
                            Database.GetFileNameFromFixedID(this.dbList[Database.DBIndices.Material].DBs[(int) Database.DBMaterialIndices.Sound], int.Parse(order.Options[(int) EffectOneData.EffectOrder.SoundOptionIndices.FixedID])),
                            int.Parse(order.Options[(int) EffectOneData.EffectOrder.SoundOptionIndices.Volume]),
                            int.Parse(order.Options[(int) EffectOneData.EffectOrder.SoundOptionIndices.Pitch]),
                            int.Parse(order.Options[(int) EffectOneData.EffectOrder.SoundOptionIndices.Pan])
                        );
                    }
                }
            }

            /// <summary>
            /// エフェクトを再生します。
            /// </summary>
            public void Play() {
                var flashStrength = 0;
                var flashStartStrength = 0;
                var flashColor = Color.Black;
                var flashCnt = 0;
                var shakeStrength = 0;
                var shakeCnt = 0;
                var shakePos = Point.Empty;

                Media.StopDraw = true;          //手動描画に切り替える
                this.editorMode = false;        //エディター用の描画を無効にする

                //全フレーム実行
                for (var i = 0; i < this.TargetEffectData?.FrameLength; i++) {
                    //HPSに合わせて重複フレーム実行
                    for (var times = 0; times < NormalFPS / mgrDBEffect.FPS; times++) {
                        this.FrameNumber = i;
                        this.ClsScreen();
                        this.Draw();

                        //フラッシュ継続
                        if (flashCnt > 0) {
                            DX.SetDrawBlendMode(DX.DX_BLENDMODE_ALPHA, flashStrength);
                            DX.DrawBox(0, 0, mgrDBEffect.ScreenSize.Width, mgrDBEffect.ScreenSize.Height, DX.GetColor(flashColor.R, flashColor.G, flashColor.B), DX.TRUE);

                            flashStrength -= FlashSpeed;
                            if (flashStrength < 0) {
                                flashStrength = flashStartStrength;
                                flashCnt--;
                            }
                        }

                        //画面振動継続
                        if (shakeStrength > 0) {
                            shakeCnt++;
                            shakePos.X = (int) (Math.Sin((Math.PI / 180) * (shakeCnt * 90)) * shakeStrength);
                            shakePos.Y = (int) (Math.Cos((Math.PI / 180) * (shakeCnt * 90)) * shakeStrength);
                            this.screenOffset = shakePos;

                            if (shakeCnt % 4 == 0) {
                                //減衰させる
                                shakeStrength--;
                                if (shakeStrength == 0) {
                                    //振動終了
                                    this.screenOffset = Point.Empty;
                                }
                            }
                        }

                        //各種命令を処理する
                        if (times == 0) {
                            foreach (var order in this.TargetEffectData.Orders) {
                                if (order.Frame == i) {
                                    switch (order.Type) {
                                        case EffectOneData.EffectOrder.OrderType.Sound:
                                            DX.PlaySoundMem(order.dxHandle, DX.DX_PLAYTYPE_BACK);
                                            break;

                                        case EffectOneData.EffectOrder.OrderType.Flash:
                                            flashStrength = int.Parse(order.Options[(int) EffectOneData.EffectOrder.FlashOptionIndices.Strength]);
                                            flashStartStrength = flashStrength;
                                            flashColor = Common.StringToColor(order.Options[(int) EffectOneData.EffectOrder.FlashOptionIndices.Color]);
                                            flashCnt = int.Parse(order.Options[(int) EffectOneData.EffectOrder.FlashOptionIndices.Count]);
                                            break;

                                        case EffectOneData.EffectOrder.OrderType.Shake:
                                            shakeStrength = int.Parse(order.Options[0]);
                                            shakeCnt = 0;
                                            shakePos = Point.Empty;
                                            break;
                                    }
                                }
                            }
                        }
                        DX.ScreenFlip();
                        DX.ProcessMessage();
                    }
                }

                //再生後、静止状態で少し待機する
                this.ClsScreen();
                this.FrameNumber = -1;
                this.Draw();
                DX.ScreenFlip();
                DX.WaitTimer(500);

                this.editorMode = true;
                Media.StopDraw = false;
            }
        }

    }
}
