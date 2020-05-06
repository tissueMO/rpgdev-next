using System;
using System.Drawing;
using System.IO;

namespace Editor.CtrlComponent.Database {
    public partial class mgrDBEffect {
        public partial class EffectOneData {

            /// <summary>
            /// レイヤーの単体情報
            /// </summary>
            public partial class EffectLayer {
                public event DataChangedEventHandler DataChanged;
                private bool fileChanged = true;

                /// <summary>
                /// コンストラクター
                /// </summary>
                public EffectLayer(int frameLength, int fixedID, string rPath, Size secSize) {
                    this.SetFrameLength(frameLength);
                    this.FixedID = fixedID;
                    this.FileName = rPath;
                    this.SectionSize = secSize;

                    //フレーム情報初期化
                    this.Frames = new EffectFrame[frameLength];
                    for (var i = 0; i < this.Frames.Length; i++) {
                        this.Frames[i] = new EffectFrame();
                    }
                }

                /// <summary>
                /// デストラクター
                /// </summary>
                ~EffectLayer() {
                    this.LayerImage?.Dispose();
                }

                public int FixedID {
                    get {
                        return this.fixedID;
                    }
                    set {
                        this.fixedID = value;
                        this.fileChanged = true;
                    }
                }
                private int fixedID;

                /// <summary>
                /// レイヤーが持つグラフィックのファイル名です。
                /// </summary>
                public string FileName {
                    get {
                        return this.fileName;
                    }
                    set {
                        this.fileName = value;
                        this.fileChanged = true;
                        if (File.Exists(ProjectManager.ProjectPath + this.fileName)) {
                            //グラフィックを読み込む
                            this.LayerImage = Image.FromFile(ProjectManager.ProjectPath + this.fileName);
                        }
                    }
                }
                private string fileName;

                /// <summary>
                /// １区画当たりのサイズです。
                /// </summary>
                public Size SectionSize {
                    get {
                        return this.sectionSize;
                    }
                    set {
                        this.sectionSize = value;
                        this.fileChanged = true;
                    }
                }
                private Size sectionSize;

                /// <summary>
                /// 一番最後のセクション位置を示します。
                /// </summary>
                public Point LastSectionPosition {
                    get {
                        if (this.fileChanged) {
                            //変更がある場合は計算し直す
                            this.fileChanged = false;
                            if (!File.Exists(ProjectManager.ProjectPath + this.FileName)) {
                                return Point.Empty;
                            }

                            var img = Image.FromFile(ProjectManager.ProjectPath + this.FileName);
                            this.tileWidth.X = (int) Math.Ceiling((double) img.Size.Width / this.SectionSize.Width);
                            this.tileWidth.Y = (int) Math.Ceiling((double) img.Size.Height / this.SectionSize.Height);
                            img.Dispose();
                        }
                        return this.tileWidth;
                    }
                }
                private Point tileWidth = Point.Empty;

                /// <summary>
                /// フレームリストを返します。
                /// この配列のサイズを変える権限は自身のクラスにあります。
                /// </summary>
                public EffectFrame[] Frames {
                    get {
                        return this.frames;
                    }
                    private set {
                        this.frames = value;
                    }
                }
                //フレームリストはリサイズするためにフィールドとして定義しておく必要がある
                private EffectFrame[] frames;

                /// <summary>
                /// DXライブラリ用のグラフィックハンドルです。
                /// </summary>
                public int dxGraphHdl {
                    get; set;
                } = -1;

                /// <summary>
                /// パレット用のイメージです。
                /// </summary>
                public Image LayerImage {
                    get {
                        return this.layerImage;
                    }
                    private set {
                        this.layerImage = value;
                    }
                }
                private Image layerImage;

                /// <summary>
                /// フレームの長さを変更します。
                /// </summary>
                public void SetFrameLength(int frameLength) {
                    var oldLength = this.Frames?.Length ?? 0;
                    //こっちはすべての要素がnullになってしまうので使わないこと this.Frames = new EffectFrame[frameLength];
                    Array.Resize(ref this.frames, frameLength);

                    //以前より長くなる場合はその分を初期化する
                    for (var i = oldLength; i < this.Frames.Length; i++) {
                        this.Frames[i] = new EffectFrame();
                    }
                }

                /// <summary>
                /// 全フレームの描画位置を指定分だけずらします。
                /// </summary>
                /// <param name="offset"></param>
                public void Shift(Point offset) {
                    foreach (var frame in this.Frames) {
                        frame.ViewPosition.Offset(offset);
                    }
                    this.DataChanged?.Invoke(this, new DataChangedEventArgs(false, false, false, false));
                }

                /// <summary>
                /// 指定したフレームを指定範囲にコピーします。
                /// </summary>
                public void FrameCopy(int start, int end, int currentFrame) {
                    for (var i = start; i <= end; i++) {
                        this.Frames[i] = this.Frames[currentFrame].GenerateClone();
                    }
                    this.DataChanged?.Invoke(this, new DataChangedEventArgs(false, false, false, false));
                }

                /// <summary>
                /// フレーム一括：始点フレームの値に統一します。
                /// </summary>
                public void FrameFill(int start, int end, EffectFrame.EffectFrameActionEnable param) {
                    for (var i = start + 1; i <= end; i++) {
                        if (i < 0 || this.Frames.Length <= i) {
                            //フレームが範囲外の場合は処理しない
                            continue;
                        }

                        if (param.ViewPosition) {
                            this.Frames[i].ViewPosition = this.Frames[start].ViewPosition;
                        }

                        if (param.SectionPosition) {
                            this.Frames[i].SectionPosition = this.Frames[start].SectionPosition;
                        }

                        if (param.Transparent) {
                            this.Frames[i].Transparent = this.Frames[start].Transparent;
                        }

                        if (param.Extend) {
                            this.Frames[i].Extend = this.Frames[start].Extend;
                        }

                        if (param.Angle) {
                            this.Frames[i].Angle = this.Frames[start].Angle;
                        }

                        if (param.Blend) {
                            this.Frames[i].Blend = this.Frames[start].Blend;
                        }
                    }

                    this.DataChanged?.Invoke(this, new DataChangedEventArgs(false, false, false, false));
                }

                /// <summary>
                /// フレーム補間：始点フレームから終点フレームにかけて線形的（一次関数的）に変化させます。
                /// </summary>
                public void FrameComplete(int start, int end, EffectFrame.EffectFrameActionEnable param) {
                    var frameCount = end - start;
                    for (int now = 1, i = start + 1; i <= end; now++, i++) {
                        if (i < 0 || this.Frames.Length <= i) {
                            //フレームが範囲外の場合は処理しない
                            continue;
                        }

                        if (param.ViewPosition) {
                            this.Frames[i].ViewPosition.X = this.calcComplete(this.Frames[start].ViewPosition.X, this.Frames[end].ViewPosition.X, now, frameCount);
                            this.Frames[i].ViewPosition.Y = this.calcComplete(this.Frames[start].ViewPosition.Y, this.Frames[end].ViewPosition.Y, now, frameCount);
                        }

                        if (param.SectionPosition) {
                            //２次元座標を１次元座標に変換した上で、現在フレームに設定すべき位置を算出する
                            var startSecIndex = this.Frames[start].SectionPosition.X + this.Frames[start].SectionPosition.Y * this.LastSectionPosition.X;
                            var endSecIndex = this.Frames[end].SectionPosition.X + this.Frames[end].SectionPosition.Y * this.LastSectionPosition.X;
                            var distance = this.calcComplete(startSecIndex, endSecIndex, now, frameCount);
                            this.Frames[i].SectionPosition = new Point(distance % this.LastSectionPosition.X, (int) Math.Floor((double) distance / this.LastSectionPosition.X));

                            //セクション範囲外にならないように調整する
                            if (this.Frames[i].SectionPosition.Y >= this.LastSectionPosition.Y) {
                                this.Frames[i].SectionPosition.Y = this.LastSectionPosition.Y - 1;
                            } else if (this.Frames[i].SectionPosition.Y < 0) {
                                this.Frames[i].SectionPosition = Point.Empty;
                            }
                        }

                        if (param.Transparent) {
                            this.Frames[i].Transparent = (byte) this.calcComplete(this.Frames[start].Transparent, this.Frames[end].Transparent, now, frameCount);
                        }

                        if (param.Extend) {
                            this.Frames[i].Extend = this.calcComplete(this.Frames[start].Extend, this.Frames[end].Extend, now, frameCount);
                        }

                        if (param.Angle) {
                            this.Frames[i].Angle = this.calcComplete(this.Frames[start].Angle, this.Frames[end].Angle, now, frameCount);
                        }
                    }

                    this.DataChanged?.Invoke(this, new DataChangedEventArgs(false, false, false, false));
                }

                /// <summary>
                /// フレーム補間の計算を行います。
                /// </summary>
                private int calcComplete(int startVal, int endVal, int counter, int frameCount) {
                    return startVal + (endVal - startVal) * counter / frameCount;
                }

                /// <summary>
                /// 指定位置にあるフレームの情報を置き換えます。
                /// </summary>
                public void ReplaceFrame(int index, EffectFrame newFrame) {
                    this.Frames[index] = newFrame;
                    this.DataChanged?.Invoke(this, new DataChangedEventArgs(false, false, false, false));
                }
            }

        }
    }
}
