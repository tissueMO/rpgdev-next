概要
　ＤＸライブラリを VisualC# で使うための DLL
　( 一応C#以外で使用する場合の為に DxDLL.h を同梱してあります、DxDLL.dll の中に含まれている
　　関数の一覧が載っています )

使い方

　＜準備＞
　　　DxLibDotNet.dll を使用する場合

　　　　１．C# のプロジェクトのフォルダの、実行ファイルが作成されるフォルダ
　　　　　　(通常は 「プロジェクトのフォルダ\bin\Debug」 か 「プロジェクトのフォルダ\bin\Release」)
　　　　　　に DxLib.dll と DxLib_x64.dll と DxLibDotNet.dll をコピー

　　　　２．C# のプロジェクトを開いて
　　　　　　「プロジェクト」→「参照の追加」→「参照タブ」で、１でコピーした DxLibDotNet.dll を選択して「OK」ボタン。


　　　DxDLL.cs を使用する場合

　　　　１．C# のプロジェクトのフォルダの、実行ファイルが作成されるフォルダ
　　　　　　(通常は 「プロジェクトのフォルダ\bin\Debug」 か 「プロジェクトのフォルダ\bin\Release」)
　　　　　　に DxLib.dll と DxLib_x64.dll をコピー

　　　　２．C# のプロジェクトを開いて
　　　　　　「プロジェクト」→「既存項目の追加」で、DxDLL.cs を「追加」。



　＜プログラムで実際に使う＞
　　　ＤＸライブラリを使用するソースに using DxLibDLL; を書く。
　　　後は、基本的にＣ言語版のＤＸライブラリの関数名やマクロ定義の前に DX. を付けたものとなります。
　　　(DX_BLENDMODE_ALPHA 等も DX が２回続いて変ですが、DX.DX_BLENDMODE_ALPHA となります)

　　例：
　　　　　int GrHandle = DX.LoadGraph( "test1.bmp" );
　　　　　DX.SetDrawBlendMode( DX.DX_BLENDMODE_ALPHA, 128 );
　　　　　DX.DrawGraph( 0, 0, GrHandle, DX.TRUE );


　　　LoadDivGraph 等の int 型のポインタを渡すものは、out int HandleBuf のように定義されています。
　　　というか、文字列以外のポインタは全て out で渡すようになっています。

　　例：
　　　　　int[] Handle = new int[25];
　　　　　DX.LoadDivGraph( "test2.bmp", 25, 5, 5, 10, 10, out Handle[0] );

　　　上記以外の文法上の違いはありませんので、基本的にはＣ言語版のＤＸライブラリのリファレンスを
　　　ご参照ください。



　＜Unicode版について＞
　　　UnicodeVerフォルダの中には DxDLL.cs や DxLib.dll 等のファイルの名前に W が付いた
　　　DxDLLW.cs や DxLibW.dll がありますが、こちらは文字列の文字コードに Unicode を使用した
　　　バージョンとなります。
　　　通常版と全く同じ手順で使うことができ、対応する文字コードも通常版より多いので、
　　　不都合が無ければUnicode版を使ってください。
　　　( 使い方については、「＜準備＞」の説明の DxLib.dll, DxLib_x64.dll, DxLibDotNet.dll, DxDLL.cs を
　　　　DxLibW.dll, DxLibW_x64.dll, DxLibWDotNet.dll, DxDLLW.cs に置き換えて読んでください )



　＜使えない関数＞
　　　基本的に「可変引数の関数」「void 型のポインタを引数で取る関数」「コールバック関数を
　　　引数で取る関数」が使えません。(なので通信機能は使えません・・・)
　　　また、ポインタを返す関数やポインタが含まれている構造体を扱う関数も使えませんので、
　　　非公開関数はかなりの数が使えなくなっています。

　　　(公開関数で使用できないもの)
　　　モデルを扱う関数の一部
　　　DrawFormatString
　　　GetDrawFormatStringWidth
　　　DrawFormatStringToHandle
　　　GetDrawFormatStringWidthToHandle
　　　SetDataToMask
　　　DrawMaskToDirectData
　　　DrawFillMaskToDirectData
　　　printfDx
　　　NetWorkSend
　　　NetWorkRecv
　　　NetWorkRecvToPeek
　　　CreateGraphFromMem
　　　LoadSoundMemByMemImage
　　　GraphLock
　　　GraphUnLock

　　　<余談>
　　　　上記「使用できない関数」の内、プロジェクトの「条件付コンパイルシンボル」に「DX_USE_UNSAFE」を
　　　　定義して、且つ「アンセーフコードの許可」を有効にすると幾つか使える関数が増えます。
　　　　ご興味がありましたら DxDLL.cs の中を「unsafe」や「DX_USE_UNSAFE」で検索してみてください。


使用の許諾
　基本的にＣ言語版ＤＸライブラリと同じです。


公開サイト
　http://homepage2.nifty.com/natupaji/DxLib/index.html




