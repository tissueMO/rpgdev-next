using System;
using System.Drawing;

namespace Editor.CtrlComponent.Database {
    using Module;

    public partial class mgrDBEffect {
        public partial class EffectOneData {
            public partial class EffectLayer {

                /// <summary>
                /// フレームの単体情報
                /// </summary>
                public class EffectFrame : ICloneable {
                    public Point ViewPosition;
                    public Point SectionPosition;
                    public Media.BlendMode Blend;
                    public int Transparent;
                    public int Extend;
                    public int Angle;

                    /// <summary>
                    /// フレームに対する操作で有効とするフィールド
                    /// </summary>
                    public class EffectFrameActionEnable {
                        public bool ViewPosition;
                        public bool SectionPosition;
                        public bool Transparent;
                        public bool Extend;
                        public bool Angle;
                        public bool Blend;
                    }

                    public EffectFrame() {
                        this.ViewPosition = Point.Empty;
                        this.SectionPosition = Point.Empty;
                        this.Blend = Media.BlendMode.Alpha;
                        this.Transparent = 0;
                        this.Extend = 100;
                        this.Angle = 0;
                    }

                    public EffectFrame(Point viewPos, Point secPos, Media.BlendMode blend, int trans, int ext, int angle) {
                        this.ViewPosition = viewPos;
                        this.SectionPosition = secPos;
                        this.Blend = blend;
                        this.Transparent = trans;
                        this.Extend = ext;
                        this.Angle = angle;
                    }

                    /// <summary>
                    /// オブジェクトの簡易コピー（インスタンスは新しく、なおかつメンバーの値がコピーされたもの）を生成する。
                    /// この関数はIClonableインターフェースの実装に必要なためpublic設定にしていますが、フレームの複製にはGenerateCloneメソッドを使うようにして下さい。
                    /// </summary>
                    /// <returns></returns>
                    public object Clone() {
                        return this.MemberwiseClone();
                    }

                    /// <summary>
                    /// このフレームのコピーを生成して返します。
                    /// </summary>
                    public EffectFrame GenerateClone() {
                        return this.Clone() as EffectFrame;
                    }
                }

            }
        }
    }
}
