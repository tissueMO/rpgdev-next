// ============================================================================
//
//		�L�����N�^�R�[�h�֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

#ifndef CHARCODE_H
#define CHARCODE_H
#pragma warning(disable:4996)

// include --------------------------------------

#include <tchar.h>
#include "DataType.h"


// define ---------------------------------------

#define CHARCODEFORMAT_SHIFTJIS				(932)
#define CHARCODEFORMAT_GB2312				(936)
#define CHARCODEFORMAT_UHC					(949)
#define CHARCODEFORMAT_BIG5					(950)
#define CHARCODEFORMAT_UTF16LE				(1200)
#define CHARCODEFORMAT_UTF16BE				(1201)
#define CHARCODEFORMAT_WINDOWS_1252			(1252)			// ����(���e�������̕����R�[�h)
#define CHARCODEFORMAT_ISO_IEC_8859_15		(32764)			// ����(���e�������̕����R�[�h)
#define CHARCODEFORMAT_UTF8					(65001)
#define CHARCODEFORMAT_ASCII				(32765)			// �A�X�L�[�����R�[�h
#define CHARCODEFORMAT_UTF32LE				(32766)			// �����ȃR�[�h�����������̂ŏ���ɒ�`
#define CHARCODEFORMAT_UTF32BE				(32767)			// �����ȃR�[�h�����������̂ŏ���ɒ�`

// �V�t�gJIS�Q�o�C�g��������
#define CHECK_SHIFTJIS_2BYTE( x )			( ( u8 )( ( ( ( u8 )(x) ) ^ 0x20) - ( u8 )0xa1 ) < 0x3c )

// UTF16LE�T���Q�[�g�y�A����( ���g���G���f�B�A�����p )
#define CHECK_CPU_LE_UTF16LE_4BYTE( x )		( ( ( x ) & 0xfc00 ) == 0xd800 )

// UTF16LE�T���Q�[�g�y�A����( ���g���G���f�B�A�����p )
#define CHECK_CPU_LE_UTF16LE_4BYTE( x )		( ( ( x ) & 0xfc00 ) == 0xd800 )

// UTF16LE�T���Q�[�g�y�A����( �r�b�O�G���f�B�A�����p )
#define CHECK_CPU_BE_UTF16LE_4BYTE( x )		( ( ( ( ( ( ( u16 )( x ) ) >> 8 ) & 0xff ) | ( ( ( u16 )( x ) << 8 ) & 0xff00 ) ) & 0xfc00 ) == 0xd800 )

// UTF16BE�T���Q�[�g�y�A����( ���g���G���f�B�A�����p )
#define CHECK_CPU_LE_UTF16BE_4BYTE( x )		CHECK_CPU_BE_UTF16LE_4BYTE( x )

// UTF16BE�T���Q�[�g�y�A����( �r�b�O�G���f�B�A�����p )
#define CHECK_CPU_BE_UTF16BE_4BYTE( x )		CHECK_CPU_LE_UTF16LE_4BYTE( x )

#define WCHAR_T_CODEPAGE				( sizeof( wchar_t ) > 2 ? DX_CHARCODEFORMAT_UTF32LE : DX_CHARCODEFORMAT_UTF16LE )

// wchar_t �T���Q�[�g�y�A����( UTF-32 or UTF-16 �z�� )
#ifdef WCHAR_T_BE
	#define CHECK_WCHAR_T_DOUBLE( x )	( sizeof( wchar_t ) == 2 && ( ( ( u16 )( x ) & 0x00fc ) == 0x00d8 ) )
#else
	#define CHECK_WCHAR_T_DOUBLE( x )	( sizeof( wchar_t ) == 2 && ( ( ( u16 )( x ) & 0xfc00 ) == 0xd800 ) )
#endif

// function proto type --------------------------

extern	int				InitCharCode( void ) ;																	// �����R�[�h���C�u�����̏�����

extern	int				GetCharCodeFormatUnitSize(	int CharCodeFormat ) ;													// �w��̃R�[�h�y�[�W�̏��ŏ��T�C�Y���擾����( �߂�l�F�o�C�g�� )
extern	int				GetCharBytes(			const char *CharCode, int CharCodeFormat ) ;									// �P�����̃o�C�g�����擾����( �߂�l�F�P�����̃o�C�g�� )
extern	u32				GetCharCode(			const char *CharCode, int CharCodeFormat, int *CharBytes ) ;					// �P�����̕����R�[�h�ƕ����̃o�C�g�����擾����
extern	int				PutCharCode(			u32 CharCode, int CharCodeFormat, char *Dest ) ;								// �����R�[�h��ʏ�̕�����ɕϊ�����A�I�[�Ƀk�������͏������܂Ȃ�( �߂�l�F�������񂾃o�C�g�� )
extern	u32				ConvCharCode(			u32 SrcCharCode, int SrcCharCodeFormat, int DestCharCodeFormat ) ;				// �����R�[�h���w��̃R�[�h�y�[�W�̕����ɕϊ�����
extern	int				ConvCharCodeString(		const u32 *Src, int SrcCharCodeFormat, u32 *Dest, int DestCharCodeFormat ) ;	// �P�����S�o�C�g�̔z����A�ʃR�[�h�y�[�W�̂P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				StringToCharCodeString( const char  *Src, int SrcCharCodeFormat, u32  *Dest ) ;						// ��������P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				CharCodeStringToString( const u32 *Src, char *Dest, int DestCharCodeFormat ) ;						// �P�����S�o�C�g�̔z��𕶎���ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				ConvString(				const char *Src, int SrcCharCodeFormat, char *Dest, int DestCharCodeFormat ) ;	// ��������w��̃R�[�h�y�[�W�̕�����ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				GetStringCharNum(		const char *String, int CharCodeFormat ) ;									// ������Ɋ܂܂�镶�������擾����
extern	const char *	GetStringCharAddress(	const char *String, int CharCodeFormat, int Index ) ;							// �w��ԍ��̕����̃A�h���X���擾����
extern	u32				GetStringCharCode(		const char *String, int CharCodeFormat, int Index ) ;							// �w��ԍ��̕����̃R�[�h���擾����

extern	void			CL_strcpy(            int CharCodeFormat, char *Dest,                     const char *Src ) ;
extern	void			CL_strcpy_s(          int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src ) ;
extern	void			CL_strncpy(           int CharCodeFormat, char *Dest,                     const char *Src, int Num ) ;
extern	void			CL_strncpy_s(         int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Num ) ;
extern	void			CL_strcat(            int CharCodeFormat, char *Dest,                     const char *Src ) ;
extern	void			CL_strcat_s(          int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src ) ;
extern	const char *	CL_strstr(            int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strlen(            int CharCodeFormat, const char *Str ) ;
extern	int				CL_strcmp(            int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_stricmp(           int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strcmp_str2_ascii( int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strncmp(           int CharCodeFormat, const char *Str1, const char *Str2, int Size ) ;
extern	const char *	CL_strchr(            int CharCodeFormat, const char *Str, u32 CharCode ) ;
extern	const char *	CL_strrchr(           int CharCodeFormat, const char *Str, u32 CharCode ) ;
extern	char *			CL_strupr(            int CharCodeFormat, char *Str ) ;
extern	int				CL_vsprintf(          int CharCodeFormat, char *Buffer, const char *FormatString, va_list Arg ) ;
extern	int				CL_sprintf(           int CharCodeFormat, char *Buffer, const char *FormatString, ... ) ;
extern	char *			CL_itoa(              int CharCodeFormat, int Value, char *Buffer, int Radix ) ;
extern	int				CL_atoi(              int CharCodeFormat, const char *Str ) ;
extern	double			CL_atof(              int CharCodeFormat, const char *Str ) ;
extern	int				CL_vsscanf(           int CharCodeFormat, const char *String, const char *FormatString, va_list Arg ) ;
extern	int				CL_sscanf(            int CharCodeFormat, const char *String, const char *FormatString, ... ) ;

#endif // CHARCODE_H

