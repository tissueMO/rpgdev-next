using System;
using System.Collections.Generic;

namespace Editor.Module {

    /// <summary>
    /// 前後操作の復元を定義するインターフェースです。
    /// </summary>
    public interface ICommand {

        /// <summary>
        /// 操作が既にユーザーによって行われたかどうかを示します。
        /// </summary>
        bool DoneByUser {
            get; set;
        }

        /// <summary>
        /// 操作を実行します。
        /// </summary>
        void Do();

        /// <summary>
        /// 元に戻します。
        /// </summary>
        void Undo();

        /// <summary>
        /// やり直します。
        /// </summary>
        void Redo();
    }

    /// <summary>
    /// 各種エディターが装備すべき、前後操作の復元を管理するクラスです。
    /// </summary>
    public class UndoRedoManager {
        private readonly Stack<ICommand> undoBuffer = new Stack<ICommand>();
        private readonly Stack<ICommand> redoBuffer = new Stack<ICommand>();
        private bool canUndo = false;
        private bool canRedo = false;
        public event EventHandler BufferChanged;        //元に戻すorやり直す コマンド可否が変化したときに発生するイベント

        /// <summary>
        /// 引数を取るコマンドの定義
        /// </summary>
        protected class CommandEx : ICommand {
            private readonly Delegate doMethod;
            private readonly Delegate undoMethod;
            private readonly object[] doParameter;
            private readonly object[] undoParameter;

            /// <summary>
            /// コンストラクター
            /// </summary>
            public CommandEx(Delegate doMethod, object[] doParameter, Delegate undoMethod, object[] undoParameter, bool doneByUser) {
                this.doMethod = doMethod;
                this.doParameter = doParameter;
                this.undoMethod = undoMethod;
                this.undoParameter = undoParameter;
                this.DoneByUser = doneByUser;
            }

            public bool DoneByUser {
                get; set;
            }

            public void Do() {
                this.doMethod?.DynamicInvoke(this.doParameter);
            }

            public void Undo() {
                this.undoMethod?.DynamicInvoke(this.undoParameter);
            }

            public void Redo() {
                this.doMethod?.DynamicInvoke(this.doParameter);
            }
        }

        /// <summary>
        /// 元に戻せるかどうか
        /// </summary>
        public bool CanUndo {
            private set {
                if (this.canUndo != value) {
                    //状態が変化したときはイベントを発生させる
                    this.canUndo = value;
                    this.BufferChanged?.Invoke(this, null);
                }
            }
            get {
                return this.canUndo;
            }
        }

        /// <summary>
        /// やり直せるかどうか
        /// </summary>
        public bool CanRedo {
            private set {
                if (this.canRedo != value) {
                    //状態が変化したときはイベントを発生させる
                    this.canRedo = value;
                    this.BufferChanged?.Invoke(this, null);
                }
            }
            get {
                return this.canRedo;
            }
        }

        /// <summary>
        /// バッファーを初期化します。
        /// </summary>
        public void InitBuffer() {
            this.undoBuffer.Clear();
            this.redoBuffer.Clear();
            this.CanUndo = false;
            this.CanRedo = false;
            this.BufferChanged?.Invoke(this, null);
        }

        /// <summary>
        /// 操作を実行してバッファーに追加します。
        /// </summary>
        public void Do(ICommand command) {
            this.undoBuffer.Push(command);
            this.CanUndo = this.undoBuffer.Count > 0;
            if (!command.DoneByUser) {
                //まだ操作が行われていなければ、操作実行する
                command.Do();
            }

            //新しい操作をすると、その先のバッファは初期化される
            this.redoBuffer.Clear();
            this.CanRedo = this.redoBuffer.Count > 0;
        }

        /// <summary>
        /// 操作を実行してバッファーに追加します。
        /// </summary>
        /// <param name="doMethod">実際に行う操作</param>
        /// <param name="undoMethod">元に戻すための操作</param>
        public void Do(Delegate doMethod, object[] doParameter, Delegate undoMethod, object[] undoParameter, bool doneByUser) {
            this.Do(new CommandEx(doMethod, doParameter, undoMethod, undoParameter, doneByUser));
        }

        /// <summary>
        /// 元に戻します。
        /// </summary>
        public void Undo() {
            if (!this.CanUndo) {
                return;
            }

            var command = this.undoBuffer.Pop();
            this.CanUndo = this.undoBuffer.Count > 0;
            command.Undo();     //操作実行
            this.redoBuffer.Push(command);
            this.CanRedo = this.redoBuffer.Count > 0;
        }

        /// <summary>
        /// やり直します。
        /// </summary>
        public void Redo() {
            if (!this.CanRedo) {
                return;
            }

            var command = this.redoBuffer.Pop();
            this.CanRedo = this.redoBuffer.Count > 0;
            command.Redo();     //操作実行
            this.undoBuffer.Push(command);
            this.CanUndo = this.undoBuffer.Count > 0;
        }
    }
}
