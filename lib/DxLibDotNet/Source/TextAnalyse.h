#ifndef TEXTANALYSE_H
#define TEXTANALYSE_H

//#define USE_DXLIB
#include "stdlib.h"

//文字列解析用クラス
class TextAnalyse
{
public:
	TextAnalyse();
	~TextAnalyse();

	bool load( const char *path );						//テキストファイルの読み込み
	bool setmem( const char *string, int size = -1 );	//メモリ上のテキストの解析を設定

	bool release( void );							//読み込んだテキストファイルを開放
	void reset( void );								//解析位置を先頭に戻す
	bool move( const char *pos );					//指定の座標に移動する
	char *getpos( void );							//現在の座標を取得する
	bool search( const char *str );					//指定の文字列がある位置の１バイト先まで移動する(無い場合は移動しない)
	char *check( const char *str );					//指定の文字列がある位置の１バイト先の座標を取得する(無い場合は0)
	bool getint( int *buf );						//次の数値を取得
	bool getint16( int *buf );						//次の数値を１６進数として取得
	bool getfloat( float *buf );					//次の数値を浮動小数点数として取得
	bool getstr( char *buf, bool nomove = false );	//次の文字列を取得

	bool next( void );								//意味のある単語まで飛ぶ( true:意味のある単語があった  false:意味のある単語が無かった )
	char *nextpos( void );							//次に意味のある単語のポインタを取得する(座標は意味のある単語の最初の文字に移動する)
	char nextchar( void );							//次に意味のある単語の最初の文字を取得する(座標は意味のある単語の最初の文字に移動する)( -1:ファイルの終端 )
	const char *nextstr();							//次に意味のある単語(ファイルの終端に来ている場合はヌル文字のみの文字列が返る)
	bool nextline( void );							//次の行に移動する
	bool skipgroup( const char *groupPre, const char *groupPost );	//グループをスキップする
	void comment_skip_enable( bool flag );			//コメントをスキップするかどうかを設定する(true:スキップする false:スキップしない)
	void setaddskip( const char *str );				//追加意味なし文字を設定する
	void setnotskip_space_tab( bool flag );			//スペースとタブをスキップしないかどうか(true:スキップしない false:スキップする)

	static int checksize( const char *str );		//文字のバイト数を得る

protected:
	bool enable;			//有効なデータがある状態か
	bool mem;				//メモリ上のテキストを解析しているか
	bool commentNoSkip;		//コメントをスキップしないかどうか( true:しない false:する )
	char *buffer;			//文字列バッファの先頭
	char *last;				//文字列バッファの終端の１バイト先
	int strsize;			//文字列のサイズ
	char *pos;				//現在の位置
	char addskip[16];		//追加意味なし文字
	bool notskip_space_tab;	//スペースとタブをスキップしないかどうか(true:スキップしない false:スキップする)

	bool checkC( char str );		//意味のある文字か調べる(true:意味がある文字)
};

//Unicode文字列解析用クラス
class TextAnalyseW
{
public:
	TextAnalyseW();
	~TextAnalyseW();

	static bool checkUnicode( const char *path );			//指定のテキストファイルがユニコードかどうかを判定する(true:ユニコード  false:マルチバイト)
	static bool checkUnicodeMem( void *string );			//指定のメモリ上のテキストがユニコードかどうかを判定する(true:ユニコード  false:マルチバイト)

	bool load( const char *path );							//テキストファイルの読み込み
	bool setmem( const wchar_t *string, int size = -1 );	//メモリ上のテキストの解析を設定(size はバイト数)

	bool release( void );									//読み込んだテキストファイルを開放
	void reset( void );										//解析位置を先頭に戻す
	bool move( const wchar_t *pos );						//指定の座標に移動する
	wchar_t *getpos( void );								//現在の座標を取得する
	bool search( const wchar_t *str );						//指定の文字列がある位置の１バイト先まで移動する(無い場合は移動しない)
	wchar_t *check( const wchar_t *str );					//指定の文字列がある位置の１バイト先の座標を取得する(無い場合は0)
	bool getint( int *buf );								//次の数値を取得
	bool getint16( int *buf );								//次の数値を１６進数として取得
	bool getstr( wchar_t *buf, bool nomove = false );		//次の文字列を取得

	bool next( void );										//意味のある単語まで飛ぶ
	wchar_t *nextpos( void );								//次に意味のある単語のポインタを取得する(座標は意味のある単語の最初の文字に移動する)
	wchar_t nextchar( void );								//次に意味のある単語の最初の文字を取得する(座標は意味のある単語の最初の文字に移動する)
	const wchar_t *nextstr();								//次に意味のある単語(ファイルの終端に来ている場合はヌル文字のみの文字列が返る)
	bool nextline( void );									//次の行に移動する
	bool skipgroup( const wchar_t *groupPre, const wchar_t *groupPost );	//グループをスキップする
	void comment_skip_enable( bool flag );					//コメントをスキップするかどうかを設定する
	void setaddskip( const wchar_t *str );					//追加意味なし文字を設定する
	void setnotskip_space_tab( bool flag );					//スペースとタブをスキップしないかどうか(true:スキップしない false:スキップする)

protected:
	bool enable;		//有効なデータがある状態か
	bool mem;			//メモリ上のテキストを解析しているか
	bool commentNoSkip;	//コメントをスキップしないかどうか( true:しない false:する )
	wchar_t *buffer;	//文字列バッファの先頭
	wchar_t *last;		//文字列バッファの終端の１バイト先
	int strsize;		//文字列のサイズ
	wchar_t *pos;		//現在の位置
	wchar_t addskip[16];		//追加意味なし文字
	bool notskip_space_tab;	//スペースとタブをスキップしないかどうか(true:スキップしない false:スキップする)

	bool checkC( wchar_t str );		//意味のある文字か調べる(true:意味がある文字)
};

#endif
