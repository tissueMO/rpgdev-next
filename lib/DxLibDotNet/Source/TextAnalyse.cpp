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

//�e�L�X�g�t�@�C���̓ǂݍ���
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

//��������̃e�L�X�g�̉�͂�ݒ�
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

//�ǂݍ��񂾃e�L�X�g�t�@�C�����J��
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

//��͈ʒu��擪�ɖ߂�
void TextAnalyse::reset( void )
{
	pos = buffer;
}

//�w��̍��W�Ɉړ�����
bool TextAnalyse::move( const char *pos )
{
	if( pos < buffer || pos >= last ) return false;
	this->pos = (char *)pos;
	return true;
}

//���݂̍��W���擾����
char *TextAnalyse::getpos( void )
{
	return pos;
}

//�w��̕����񂪂���ʒu�̂P�o�C�g��܂ňړ�����(�����ꍇ�͈ړ����Ȃ�)
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

//�w��̕����񂪂���ʒu�̂P�o�C�g��̍��W���擾����(�����ꍇ��0)
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

//���̐��l���擾
bool TextAnalyse::getint( int *buf )
{
	char buffer[1024];

	if( getstr(buffer) == false ) return false;
	*buf = atoi( buffer );

	return true;
}

//���̐��l���P�U�i���Ƃ��Ď擾
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

//���̐��l�𕂓������_���Ƃ��Ď擾
bool TextAnalyse::getfloat( float *buf )
{
	char buffer[1024];

	if( getstr(buffer) == false ) return false;
	*buf = ( float )atof( buffer );

	return true;
}

//���̕�������擾
bool TextAnalyse::getstr( char *buf, bool nomove )
{
	int i, j;
	char *temp;

	next();
	temp = pos;
	if( pos >= last ) return false;

	//�ŏ��̕����� " ���A�Ⴕ���͍ŏ��̕����� L �ł��̌� " �ǂ����ŏ����𕪊�
	if( pos[0] == '\"' ||
		( pos[0] == 'L' && pos[1] == '\"' ) )
	{
		// " ������Ƃ���܂ŃR�s�[
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
				//���̌�Ɂh������ꍇ�́h�𕶎���̈ꕔ�Ƃ��Ă݂Ȃ�
				if( pos + i + 1 < last && pos[i] == '\\' && pos[i+1] == '\"' )
				{
					buf[j] = '\"';
					j++;
					i += 2;
				}
				else
				//���s�A�^�u�͖�������
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
		//�Ӗ�����������������Ƃ���܂ŃR�s�[
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

//�����̃o�C�g���𓾂�
int TextAnalyse::checksize( const char *str )
{
	return ( (unsigned char)*str >= 0x81 && (unsigned char)*str <= 0x9F ) || ( (unsigned char)*str >= 0xE0 && (unsigned char)*str <= 0xFC ) ? 2 : 1 ;
}

//�Ӗ��̂���P��܂Ŕ��
bool TextAnalyse::next( void )
{
	for(;;)
	{
		if( pos >= last ) return false;

		//�R�����g���΂�
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
		//�󔒂��΂�
		if( checkC( *pos ) == false )
		{
			for(; pos < last && checkC( *pos ) == false; pos ++ ){}
			if( pos >= last ) return false;
		}
		else break ;
	}

	return true;
}

//���̍s�Ɉړ�����
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

//�O���[�v���X�L�b�v����
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

//�R�����g���X�L�b�v���邩�ǂ�����ݒ肷��(true:�X�L�b�v���� false:�X�L�b�v���Ȃ�)
void TextAnalyse::comment_skip_enable( bool flag )
{
	commentNoSkip = !flag;
}

//�ǉ��Ӗ��Ȃ�������ݒ肷��
void TextAnalyse::setaddskip( const char *str )
{
	strcpy( addskip, str );
}

//�X�y�[�X�ƃ^�u���X�L�b�v���Ȃ����ǂ���(true:�X�L�b�v���Ȃ� false:�X�L�b�v����)
void TextAnalyse::setnotskip_space_tab( bool flag )
{
	notskip_space_tab = flag;
}

//�Ӗ��̂��镶�������ׂ�
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

//���ɈӖ��̂���P��̃|�C���^���擾����
char *TextAnalyse::nextpos( void )
{
	if( next() == false ) return 0;
	return pos;
}

//���ɈӖ��̂���P��̍ŏ��̕������擾����( -1:�t�@�C���̏I�[ )
char TextAnalyse::nextchar( void )
{
	if( next() == false ) return -1;
	return *pos;
}

//���ɈӖ��̂���P��(�t�@�C���̏I�[�ɗ��Ă���ꍇ�̓k�������݂̂̕����񂪕Ԃ�)
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

//�w��̃e�L�X�g�t�@�C�������j�R�[�h���ǂ����𔻒肷��(true:���j�R�[�h  false:�}���`�o�C�g)
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

//�w��̃�������̃e�L�X�g�����j�R�[�h���ǂ����𔻒肷��(true:���j�R�[�h  false:�}���`�o�C�g)
bool TextAnalyseW::checkUnicodeMem( void *string )
{
	return ((unsigned char *)string)[0] == 0xff && ((unsigned char *)string)[1] == 0xfe;
}

//�e�L�X�g�t�@�C���̓ǂݍ���
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

//��������̃e�L�X�g�̉�͂�ݒ�(size �̓o�C�g��)
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

//�ǂݍ��񂾃e�L�X�g�t�@�C�����J��
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

//��͈ʒu��擪�ɖ߂�
void TextAnalyseW::reset( void )
{
	pos = buffer;
}

//�w��̍��W�Ɉړ�����
bool TextAnalyseW::move( const wchar_t *pos )
{
	if( pos < buffer || pos >= last ) return false;
	this->pos = (wchar_t *)pos;
	return true;
}

//���݂̍��W���擾����
wchar_t *TextAnalyseW::getpos( void )
{
	return pos;
}

//�w��̕����񂪂���ʒu�̂P�o�C�g��܂ňړ�����(�����ꍇ�͈ړ����Ȃ�)
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

//�w��̕����񂪂���ʒu�̂P�o�C�g��̍��W���擾����(�����ꍇ��0)
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

//���̐��l���擾
bool TextAnalyseW::getint( int *buf )
{
	wchar_t buffer[1024];
	wchar_t *temp;

	if( getstr(buffer) == false ) return false;
	*buf = (int)wcstol( buffer, &temp, 10 );

	return true;
}

//���̐��l���P�U�i���Ƃ��Ď擾
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

//���̕�������擾
bool TextAnalyseW::getstr( wchar_t *buf, bool nomove )
{
	int i, j;
	wchar_t *temp;

	next();
	temp = pos;
	if( pos >= last ) return false;

	//�ŏ��̕����� " ���ǂ����ŏ����𕪊�
	if( pos[0] == L'\"' )
	{
		// " ������Ƃ���܂ŃR�s�[
		pos++;
		for( j = 0, i = 0; pos + i < last && pos[i] != L'\"'; i++ )
		{
			//���s�A�^�u�͖�������
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
		//�Ӗ�����������������Ƃ���܂ŃR�s�[
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

//�Ӗ��̂���P��܂Ŕ��
bool TextAnalyseW::next( void )
{
	for(;;)
	{
		if( pos >= last ) return false;

		//�R�����g���΂�
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
		//�󔒂��΂�
		if( checkC( *pos ) == false )
		{
			for(; pos < last && checkC( *pos ) == false; pos ++ ){}
			if( pos >= last ) return false;
		}
		else break ;
	}

	return true;
}

//���̍s�Ɉړ�����
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

//�O���[�v���X�L�b�v����
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

//�R�����g���X�L�b�v���邩�ǂ�����ݒ肷��(true:�X�L�b�v���� false:�X�L�b�v���Ȃ�)
void TextAnalyseW::comment_skip_enable( bool flag )
{
	commentNoSkip = !flag;
}

//�ǉ��Ӗ��Ȃ�������ݒ肷��
void TextAnalyseW::setaddskip( const wchar_t *str )
{
	wcscpy( addskip, str );
}

//�X�y�[�X�ƃ^�u���X�L�b�v���Ȃ����ǂ���(true:�X�L�b�v���Ȃ� false:�X�L�b�v����)
void TextAnalyseW::setnotskip_space_tab( bool flag )
{
	notskip_space_tab = flag;
}

//�Ӗ��̂��镶�������ׂ�
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

//���ɈӖ��̂���P��̃|�C���^���擾����
wchar_t *TextAnalyseW::nextpos( void )
{
	if( next() == false ) return 0;
	return pos;
}

//���ɈӖ��̂���P��̍ŏ��̕������擾����
wchar_t TextAnalyseW::nextchar( void )
{
	next();
	return *pos;
}

//���ɈӖ��̂���P��(�t�@�C���̏I�[�ɗ��Ă���ꍇ�̓k�������݂̂̕����񂪕Ԃ�)
const wchar_t *TextAnalyseW::nextstr()
{
	static wchar_t buffer[1024];

	next();
	buffer[0] = L'\0';
	getstr( buffer, true );
	return buffer;
}
