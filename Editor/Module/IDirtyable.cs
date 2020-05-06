namespace Editor.Module {

    /// <summary>
    /// 編集したかどうかを示せるようにします。
    /// </summary>
    public interface IDirtyable {
        /// <summary>
        /// このオブジェクトが変更されたかどうかを示します。
        /// 具体的な処理が必要な場合はオーバーライドして下さい。
        /// </summary>
        bool IsDirty {
            get; set;
        }
    }
}
