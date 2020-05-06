#include "TextAnalyse.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifdef USE_DXLIB
#include "DxLib.h"

#define FILEPOINTER					int
#define FOPEN( name )				FileRead_open( name )
#define FSIZE( name, filep, dst )	dst = FileRead_size( name )
#define FREAD( buf, size, filep )	FileRead_read( buf, size, filep )
#define FSEEK( filep, pos, type )	FileRead_seek( filep, pos, type )
#define FCLOSE( filep )				FileRead_close( filep )
#else
#include <stdio.h>
#define FILEPOINTER					FILE *
#define FOPEN( name )				fopen( name, "rb" )
#define FSIZE( name, filep, dst )	fseek( filep, 0L, SEEK_END ); dst = ftell( filep ); fseek( filep, 0L, SEEK_SET )
#define FREAD( buf, size, filep )	fread( buf, size, 1, filep )
#define FSEEK( filep, pos, type )	fseek( filep, pos, type )
#define FCLOSE( filep )				fclose( filep )
#endif

TextAnalyse::TextAnalyse()
{
	enable  = false;
	mem     = false;
	buffer  = 0;
	pos     = 0;
	last    = 0;
	strsize = 0;
	commentNoSkip = false;
	addskip[0] = '\0';
	notskip_space_tab = false;
}

TextAnalyse::~TextAnalyse()
{
	release();
}

//テキストファイルの読み込み
bool TextAnalyse::load( const char *path )
{
	FILEPOINTER fp;

	if( enable ) release();

	fp = FOPEN( path );
	if( fp == 0 ) return false;

	FSIZE( path, fp, strsize );

	buffer = new char[(unsigned int )( strsize + 1 )];
	FREAD( buffer, ( size_t )strsize, fp );
	FCLOSE( fp );
	buffer[strsize] = '\0';

	enable = true;
	mem    = false;
	pos    = buffer;
	last   = buffer + strsize;

	return true;
}

//メモリ上のテキストの解析を設定
bool TextAnalyse::setmem( const char *string, int size )
{
	if( size < 0 ) size = ( int )strlen( string );
	if( enable ) release();

	buffer  = (char *)string;
	strsize = size;
	enable  = true;
	mem     = true;
	pos     = buffer;
	last    = buffer + strsize;

	return true;
}

//読み込んだテキストファイルを開放
bool TextAnalyse::release( void )
{
	if( enable == false ) return true;

	if( mem == false ) delete []buffer;
	buffer = 0;
	pos    = 0;
	last   = 0;
	enable = false;
	mem    = false;

	return true;
}

//解析位置を先頭に戻す
void TextAnalyse::reset( void )
{
	pos = buffer;
}

//指定の座標に移動する
bool TextAnalyse::move( const char *pos )
{
	if( pos < buffer || pos >= last ) return false;
	this->pos = (char *)pos;
	return true;
}

//現在の座標を取得する
char *TextAnalyse::getpos( void )
{
	return pos;
}

//指定の文字列がある位置の１バイト先まで移動する(無い場合は移動しない)
bool TextAnalyse::search( const char *str )
{
	char *p;
	char buffer[1024];

	p = pos;

	for(;;)
	{
		if( getstr( buffer ) == false ) break;
		if( strcmp( buffer, str ) == 0 ) break;
	}

	if( pos >= last )
	{
		pos = p;
		return false;
	}

	return true;
}

//指定の文字列がある位置の１バイト先の座標を取得する(無い場合は0)
char *TextAnalyse::check( const char *str )
{
	char *p, *res;
	char buffer[1024];

	p = pos;

	for(;;)
	{
		if( getstr( buffer ) == false ) break;
		if( strcmp( buffer, str ) == 0 ) break;
	}
	res = pos >= last ? 0 : pos;

	pos = p;
	return res;
}

//次の数値を取得
bool TextAnalyse::getint( int *buf )
{
	char buffer[1024];

	if( getstr(buffer) == false ) return false;
	*buf = atoi( buffer );

	return true;
}

//次の数値を１６進数として取得
bool TextAnalyse::getint16( int *buf )
{
	char buffer[1024];
	unsigned char c;
	int keta, i, j, num;

	if( getstr(buffer) == false ) return false;

	keta = ( int )strlen( buffer );
	j = 1;
	num = 0;
	for( i = 0; i < keta; i ++, j <<= 4 )
	{
		c = (unsigned char)buffer[keta-1-i];
		     if( c >= '0' && c <= '9' ) num += (c - '0') * j;
		else if( c >= 'a' && c <= 'z' ) num += ( (c - 'a') + 10) * j;
		else if( c >= 'A' && c <= 'Z' ) num += ( (c - 'A') + 10) * j;
		else
		{
			*buf = -1;
			return false;
		}
	}
	*buf = num;

	return true;
}

//次の数値を浮動小数点数として取得
bool TextAnalyse::getfloat( float *buf )
{
	char buffer[1024];

	if( getstr(buffer) == false ) return false;
	*buf = ( float )atof( buffer );

	return true;
}

//次の文字列を取得
bool TextAnalyse::getstr( char *buf, bool nomove )
{
	int i, j;
	char *temp;

	next();
	temp = pos;
	if( pos >= last ) return false;

	//最初の文字が " か、若しくは最初の文字が L でその後 " どうかで処理を分岐
	if( pos[0] == '\"' ||
		( pos[0] == 'L' && pos[1] == '\"' ) )
	{
		// " があるところまでコピー
		if( pos[0] == '\"' )
		{
			pos++;
		}
		else
		{
			pos += 2;
		}
		for( j = 0, i = 0; pos + i < last && pos[i] != '\"'; )
		{
			if( checksize( pos + i ) == 2 )
			{
				buf[j  ] = pos[i  ];
				buf[j+1] = pos[i+1];
				i += 2;
				j += 2;
			}
			else
			{
				//￥の後に”がある場合は”を文字列の一部としてみなす
				if( pos + i + 1 < last && pos[i] == '\\' && pos[i+1] == '\"' )
				{
					buf[j] = '\"';
					j++;
					i += 2;
				}
				else
				//改行、タブは無視する
				if( pos[i] != '\r' && pos[i] != '\n' && pos[i] != '\t' )
				{
					buf[j] = pos[i];
					j++;
					i++;
				}
				else
				{
					i++;
				}
			}
		}
		buf[j] = '\0';
		pos += i;
		if( *pos == '\"' ) pos++;
	}
	else
	{
		//意味が無い文字があるところまでコピー
		for( i = 0; pos + i < last && checkC( pos[i] ) == true; )
		{
			if( checksize( pos + i ) == 2 )
			{
				buf[i  ] = pos[i  ];
				buf[i+1] = pos[i+1];
				i += 2;
			}
			else
			{
				buf[i] = pos[i];
				i ++;
			}
		}
		buf[i] = '\0';
		pos += i;
	}

	if( nomove ) pos = temp;

	return true;
}

//文字のバイト数を得る
int TextAnalyse::checksize( const char *str )
{
	return ( (unsigned char)*str >= 0x81 && (unsigned char)*str <= 0x9F ) || ( (unsigned char)*str >= 0xE0 && (unsigned char)*str <= 0xFC ) ? 2 : 1 ;
}

//意味のある単語まで飛ぶ
bool TextAnalyse::next( void )
{
	for(;;)
	{
		if( pos >= last ) return false;

		//コメントを飛ばす
		if( commentNoSkip == false && pos[0] == '/' && pos[1] == '/' )
		{
			pos += 2;
			for(; pos < last && pos[0] != '\n'; pos += checksize( pos ) ){}
			if( pos >= last ) return false;
			pos ++;
		}
		else
		if( commentNoSkip == false && pos[0] == '/' && pos[1] == '*' )
		{
			pos += 2;
			for(; pos < last && ( pos[0] != '*' || pos[1] != '/' ) ; pos += checksize( pos ) ){}
			if( pos >= last ) return false;
			pos += 2;
		}
		else
		//空白を飛ばす
		if( checkC( *pos ) == false )
		{
			for(; pos < last && checkC( *pos ) == false; pos ++ ){}
			if( pos >= last ) return false;
		}
		else break ;
	}

	return true;
}

//次の行に移動する
bool TextAnalyse::nextline( void )
{
	for(;;)
	{
		if( pos >= last ) return false;

		if( pos[0] == '\n' )
		{
			pos++;
			break;
		}

		pos += checksize( pos );
	}

	return true;
}

//グループをスキップする
bool TextAnalyse::skipgroup( const char *groupPre, const char *groupPost )
{
	char str[256];
	int count;

	if( search( groupPre ) == false ) return false;

	count = 1;
	for(;;)
	{
		if( getstr( str ) == false ) return false;

		if( strcmp( groupPre, str ) == 0 )
		{
			count ++ ;
		}
		else
		if( strcmp( groupPost, str ) == 0 )
		{
			count -- ;
			if( count == 0 ) return true;
		}
	}
}

//コメントをスキップするかどうかを設定する(true:スキップする false:スキップしない)
void TextAnalyse::comment_skip_enable( bool flag )
{
	commentNoSkip = !flag;
}

//追加意味なし文字を設定する
void TextAnalyse::setaddskip( const char *str )
{
	strcpy( addskip, str );
}

//スペースとタブをスキップしないかどうか(true:スキップしない false:スキップする)
void TextAnalyse::setnotskip_space_tab( bool flag )
{
	notskip_space_tab = flag;
}

//意味のある文字か調べる
bool TextAnalyse::checkC( char str )
{
	int i;
	for( i = 0; addskip[i] != '\0' && addskip[i] != str ; i++ ){}
	if( addskip[i] != '\0' ) return false;

	if( notskip_space_tab )
	{
		return str != '\0' &&                              str != '\n' && str != '\r';
	}
	else
	{
		return str != '\0' && str != ' ' && str != '\t' && str != '\n' && str != '\r';
	}
}

//次に意味のある単語のポインタを取得する
char *TextAnalyse::nextpos( void )
{
	if( next() == false ) return 0;
	return pos;
}

//次に意味のある単語の最初の文字を取得する( -1:ファイルの終端 )
char TextAnalyse::nextchar( void )
{
	if( next() == false ) return -1;
	return *pos;
}

//次に意味のある単語(ファイルの終端に来ている場合はヌル文字のみの文字列が返る)
const char *TextAnalyse::nextstr()
{
	static char buffer[1024];

	next();
	buffer[0] = '\0';
	getstr( buffer, true );
	return buffer;
}





















TextAnalyseW::TextAnalyseW()
{
	enable  = false;
	mem     = false;
	buffer  = 0;
	pos     = 0;
	last    = 0;
	strsize = 0;
	commentNoSkip = false;
}

TextAnalyseW::~TextAnalyseW()
{
	release();
}

//指定のテキストファイルがユニコードかどうかを判定する(true:ユニコード  false:マルチバイト)
bool TextAnalyseW::checkUnicode( const char *path )
{
	FILEPOINTER fp;
	unsigned char temp[2];

	fp = FOPEN( path );
	if( fp == 0 ) return false;
	FREAD( temp, 2, fp );
	FCLOSE( fp );

	return temp[0] == 0xff && temp[1] == 0xfe;
}

//指定のメモリ上のテキストがユニコードかどうかを判定する(true:ユニコード  false:マルチバイト)
bool TextAnalyseW::checkUnicodeMem( void *string )
{
	return ((unsigned char *)string)[0] == 0xff && ((unsigned char *)string)[1] == 0xfe;
}

//テキストファイルの読み込み
bool TextAnalyseW::load( const char *path )
{
	FILEPOINTER fp;
	unsigned char temp[2];

	if( enable ) release();

	fp = FOPEN( path );
	if( fp == 0 ) return false;

	FREAD( temp, 2, fp );
	if( temp[0] != 0xff || temp[1] != 0xfe )
	{
		FCLOSE( fp );
		return false;
	}

	FSIZE( path, fp, strsize );
	strsize = ( int )( ( strsize - 2 ) / sizeof( wchar_t ) );
	buffer = new wchar_t[( unsigned int )( strsize + 1 )];
	FREAD( buffer, strsize * sizeof( wchar_t ), fp );
	FSEEK( fp, 2, SEEK_SET );
	FCLOSE( fp );
	buffer[strsize] = L'\0';

	enable = true;
	mem    = false;
	pos    = buffer;
	last   = buffer + strsize;

	return true;
}

//メモリ上のテキストの解析を設定(size はバイト数)
bool TextAnalyseW::setmem( const wchar_t *string, int size )
{
	unsigned char *p;
	p = (unsigned char *)string;
	if( p[0] == 0xff && p[1] == 0xfe )
	{
		string ++ ;
		if( size >= 0 ) size -= 2 ;
	}

	size = ( int )( size < 0 ? wcslen( string ) : size / sizeof( wchar_t ) ) ;
	if( enable ) release();

	buffer  = (wchar_t *)string;
	strsize = size;
	enable  = true;
	mem     = true;
	pos     = buffer;
	last    = buffer + strsize;

	return true;
}

//読み込んだテキストファイルを開放
bool TextAnalyseW::release( void )
{
	if( enable == false ) return true;

	if( mem == false ) delete []buffer;
	buffer = 0;
	pos    = 0;
	last   = 0;
	enable = false;
	mem    = false;

	return true;
}

//解析位置を先頭に戻す
void TextAnalyseW::reset( void )
{
	pos = buffer;
}

//指定の座標に移動する
bool TextAnalyseW::move( const wchar_t *pos )
{
	if( pos < buffer || pos >= last ) return false;
	this->pos = (wchar_t *)pos;
	return true;
}

//現在の座標を取得する
wchar_t *TextAnalyseW::getpos( void )
{
	return pos;
}

//指定の文字列がある位置の１バイト先まで移動する(無い場合は移動しない)
bool TextAnalyseW::search( const wchar_t *str )
{
	wchar_t *p;
	wchar_t buffer[1024];

	p = pos;

	for(;;)
	{
		if( getstr( buffer ) == false ) break;
		if( wcscmp( buffer, str ) == 0 ) break;
	}

	if( pos >= last )
	{
		pos = p;
		return false;
	}

	return true;
}

//指定の文字列がある位置の１バイト先の座標を取得する(無い場合は0)
wchar_t *TextAnalyseW::check( const wchar_t *str )
{
	wchar_t *p, *res;
	wchar_t buffer[1024];

	p = pos;

	for(;;)
	{
		if( getstr( buffer ) == false ) break;
		if( wcscmp( buffer, str ) == 0 ) break;
	}
	res = pos >= last ? 0 : pos;

	pos = p;
	return res;
}

//次の数値を取得
bool TextAnalyseW::getint( int *buf )
{
	wchar_t buffer[1024];
	wchar_t *temp;

	if( getstr(buffer) == false ) return false;
	*buf = (int)wcstol( buffer, &temp, 10 );

	return true;
}

//次の数値を１６進数として取得
bool TextAnalyseW::getint16( int *buf )
{
	wchar_t buffer[1024];
	unsigned char c;
	int keta, i, j, num;

	if( getstr(buffer) == false ) return false;

	keta = ( int )wcslen( buffer );
	j = 1;
	num = 0;
	for( i = 0; i < keta; i ++, j <<= 4 )
	{
		c = (unsigned char)buffer[keta-1-i];
		     if( c >= '0' && c <= '9' ) num +=   (c - '0')       * j;
		else if( c >= 'a' && c <= 'z' ) num += ( (c - 'a') + 10) * j;
		else if( c >= 'A' && c <= 'Z' ) num += ( (c - 'A') + 10) * j;
		else
		{
			*buf = -1;
			return false;
		}
	}
	*buf = num;

	return true;
}

//次の文字列を取得
bool TextAnalyseW::getstr( wchar_t *buf, bool nomove )
{
	int i, j;
	wchar_t *temp;

	next();
	temp = pos;
	if( pos >= last ) return false;

	//最初の文字が " かどうかで処理を分岐
	if( pos[0] == L'\"' )
	{
		// " があるところまでコピー
		pos++;
		for( j = 0, i = 0; pos + i < last && pos[i] != L'\"'; i++ )
		{
			//改行、タブは無視する
			if( pos[i] == L'\r' || pos[i] == L'\n' || pos[i] == L'\t' ) continue;
			buf[j] = pos[i];
			j++;
		}
		buf[j] = L'\0';
		pos += i;
		if( *pos == L'\"' ) pos++;
	}
	else
	{
		//意味が無い文字があるところまでコピー
		for( i = 0; pos + i < last && checkC( pos[i] ) == true; i++ )
		{
			buf[i] = pos[i];
		}
		buf[i] = L'\0';
		pos += i;
	}

	if( nomove ) pos = temp;

	return true;
}

//意味のある単語まで飛ぶ
bool TextAnalyseW::next( void )
{
	for(;;)
	{
		if( pos >= last ) return false;

		//コメントを飛ばす
		if( commentNoSkip == false && pos[0] == L'/' && pos[1] == L'/' )
		{
			pos += 2;
			for(; pos < last && pos[0] != '\n'; pos ++ ){}
			if( pos >= last ) return false;
			pos ++;
		}
		else
		if( commentNoSkip == false && pos[0] == L'/' && pos[1] == L'*' )
		{
			pos += 2;
			for(; pos < last && ( pos[0] != L'*' || pos[1] != L'/' ) ; pos ++ ){}
			if( pos >= last ) return false;
			pos += 2;
		}
		else
		//空白を飛ばす
		if( checkC( *pos ) == false )
		{
			for(; pos < last && checkC( *pos ) == false; pos ++ ){}
			if( pos >= last ) return false;
		}
		else break ;
	}

	return true;
}

//次の行に移動する
bool TextAnalyseW::nextline( void )
{
	for(;;)
	{
		if( pos >= last ) return false;

		if( pos[0] == L'\n' )
		{
			pos++;
			break;
		}

		pos ++;
	}

	return true;
}

//グループをスキップする
bool TextAnalyseW::skipgroup( const wchar_t *groupPre, const wchar_t *groupPost )
{
	wchar_t str[256];
	int count;

	if( search( groupPre ) == false ) return false;

	count = 1;
	for(;;)
	{
		if( getstr( str ) == false ) return false;

		if( wcscmp( groupPre, str ) == 0 )
		{
			count ++ ;
		}
		else
		if( wcscmp( groupPost, str ) == 0 )
		{
			count -- ;
			if( count == 0 ) return true;
		}
	}
}

//コメントをスキップするかどうかを設定する(true:スキップする false:スキップしない)
void TextAnalyseW::comment_skip_enable( bool flag )
{
	commentNoSkip = !flag;
}

//追加意味なし文字を設定する
void TextAnalyseW::setaddskip( const wchar_t *str )
{
	wcscpy( addskip, str );
}

//スペースとタブをスキップしないかどうか(true:スキップしない false:スキップする)
void TextAnalyseW::setnotskip_space_tab( bool flag )
{
	notskip_space_tab = flag;
}

//意味のある文字か調べる
bool TextAnalyseW::checkC( wchar_t str )
{
	int i;
	for( i = 0; addskip[i] != L'\0' && addskip[i] != str ; i++ ){}
	if( addskip[i] != L'\0' ) return false;

	if( notskip_space_tab )
	{
		return str != L'\0' &&                                str != L'\n' && str != L'\r';
	}
	else
	{
		return str != L'\0' && str != L' ' && str != L'\t' && str != L'\n' && str != L'\r';
	}
}

//次に意味のある単語のポインタを取得する
wchar_t *TextAnalyseW::nextpos( void )
{
	if( next() == false ) return 0;
	return pos;
}

//次に意味のある単語の最初の文字を取得する
wchar_t TextAnalyseW::nextchar( void )
{
	next();
	return *pos;
}

//次に意味のある単語(ファイルの終端に来ている場合はヌル文字のみの文字列が返る)
const wchar_t *TextAnalyseW::nextstr()
{
	static wchar_t buffer[1024];

	next();
	buffer[0] = L'\0';
	getstr( buffer, true );
	return buffer;
}
