// ============================================================================
//
//		�L�����N�^�R�[�h�֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

// include --------------------------------------

#include "DataType.h"
#include "CharCode.h"
#include "FileLib.h"
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>

#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#endif

// define ---------------------------------------

// ����������̃T�C�Y�w��v���t�B�b�N�X
#define PRINTF_SIZE_PREFIX_h		(0)
#define PRINTF_SIZE_PREFIX_l		(1)
#define PRINTF_SIZE_PREFIX_ll		(2)
#define PRINTF_SIZE_PREFIX_w		(3)
#define PRINTF_SIZE_PREFIX_I		(4)
#define PRINTF_SIZE_PREFIX_I32		(5)
#define PRINTF_SIZE_PREFIX_I64		(6)

// ����������̌^�w��
#define PRINTF_TYPE_c				(0)
#define PRINTF_TYPE_C				(1)
#define PRINTF_TYPE_d				(2)
#define PRINTF_TYPE_i				(3)
#define PRINTF_TYPE_o				(4)
#define PRINTF_TYPE_u				(5)
#define PRINTF_TYPE_x				(6)
#define PRINTF_TYPE_X				(7)
#define PRINTF_TYPE_e				(8)
#define PRINTF_TYPE_E				(9)
#define PRINTF_TYPE_f				(10)
#define PRINTF_TYPE_g				(11)
#define PRINTF_TYPE_G				(12)
#define PRINTF_TYPE_a				(13)
#define PRINTF_TYPE_A				(14)
#define PRINTF_TYPE_n				(15)
#define PRINTF_TYPE_p				(16)
#define PRINTF_TYPE_s				(17)
#define PRINTF_TYPE_S				(18)
#define PRINTF_TYPE_Z				(19)
#define PRINTF_TYPE_NUM				(20)

#define MIN_COMPRESS		(4)						// �Œሳ�k�o�C�g��
#define MAX_SEARCHLISTNUM	(64)					// �ő��v����T���ׂ̃��X�g��H��ő吔
#define MAX_SUBLISTNUM		(65536)					// ���k���ԒZ�k�̂��߂̃T�u���X�g�̍ő吔
#define MAX_COPYSIZE 		(0x1fff + MIN_COMPRESS)	// �Q�ƃA�h���X����R�s�[�o�؂�ő�T�C�Y( ���k�R�[�h���\���ł���R�s�[�T�C�Y�̍ő�l + �Œሳ�k�o�C�g�� )
#define MAX_ADDRESSLISTNUM	(1024 * 1024 * 1)		// �X���C�h�����̍ő�T�C�Y
#define MAX_POSITION		(1 << 24)				// �Q�Ɖ\�ȍő告�΃A�h���X( 16MB )

// �������`�F�b�N
#define CHARCODETABLE_INITCHECK( CharCodeFormat )		\
	switch( (CharCodeFormat) )\
	{\
	case CHARCODEFORMAT_SHIFTJIS	:\
		if( g_CharCodeSystem.InitializeCharCodeCP932InfoFlag == FALSE )\
		{\
			SetupCharCodeCP932TableInfo() ;\
		}\
		break ;\
\
	case CHARCODEFORMAT_GB2312 :\
		if( g_CharCodeSystem.InitializeCharCodeCP936InfoFlag == FALSE )\
		{\
			SetupCharCodeCP936TableInfo() ;\
		}\
		break ;\
\
	case CHARCODEFORMAT_UHC :\
		if( g_CharCodeSystem.InitializeCharCodeCP949InfoFlag == FALSE )\
		{\
			SetupCharCodeCP949TableInfo() ;\
		}\
		break ;\
\
	case CHARCODEFORMAT_BIG5 :\
		if( g_CharCodeSystem.InitializeCharCodeCP950InfoFlag == FALSE )\
		{\
			SetupCharCodeCP950TableInfo() ;\
		}\
		break ;\
\
	case CHARCODEFORMAT_WINDOWS_1252 :\
		if( g_CharCodeSystem.InitializeCharCodeCP1252InfoFlag == FALSE )\
		{\
			SetupCharCodeCP1252TableInfo() ;\
		}\
		break ;\
\
	case CHARCODEFORMAT_ISO_IEC_8859_15 :\
		if( g_CharCodeSystem.InitializeCharCodeISO_IEC_8859_15InfoFlag == FALSE )\
		{\
			SetupCharCodeISO_IEC_8859_15TableInfo() ;\
		}\
		break ;\
	}

// data type ------------------------------------

// UTF-16�Ɗe�����R�[�h�̑Ή��\�̏��
struct CHARCODETABLEINFO
{
	u16					MultiByteToUTF16[ 0x10000 ] ;		// �e�����R�[�h����UTF-16�ɕϊ����邽�߂̃e�[�u��
	u16					UTF16ToMultiByte[ 0x10000 ] ;		// UTF-16����e�����R�[�h�ɕϊ����邽�߂̃e�[�u��
} ;

// �W���֐��̌݊��֐��Ŏg�p������
struct CHARCODESYSTEM
{
	int					InitializeFlag ;							// �������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )

	int					InitializeCharCodeCP932InfoFlag ;			// Shift-JIS�̕����R�[�h���̏������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )
	CHARCODETABLEINFO	CharCodeCP932Info ;							// Shift-JIS�̕����R�[�h���

	int					InitializeCharCodeCP936InfoFlag ;			// GB2312�̕����R�[�h���̏������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )
	CHARCODETABLEINFO	CharCodeCP936Info ;							// GB2312�̕����R�[�h���

	int					InitializeCharCodeCP949InfoFlag ;			// UHC�̕����R�[�h���̏������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )
	CHARCODETABLEINFO	CharCodeCP949Info ;							// UHC�̕����R�[�h���

	int					InitializeCharCodeCP950InfoFlag ;			// BIG5�̕����R�[�h���̏������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )
	CHARCODETABLEINFO	CharCodeCP950Info ;							// BIG5�̕����R�[�h���

	int					InitializeCharCodeCP1252InfoFlag ;			// ����(���e�������̕����R�[�h)�̕����R�[�h���̏������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )
	CHARCODETABLEINFO	CharCodeCP1252Info ;						// ����(���e�������̕����R�[�h)�̕����R�[�h���

	int					InitializeCharCodeISO_IEC_8859_15InfoFlag ;	// ����(���e�������̕����R�[�h)�̕����R�[�h���̏������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )
	CHARCODETABLEINFO	CharCodeISO_IEC_8859_15Info ;				// ����(���e�������̕����R�[�h)�̕����R�[�h���
} ;

// data -----------------------------------------

static u8 NumberToCharTable[ 2 ][ 16 ] =
{
	{
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
	},

	{
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	}
} ;

static u8 FloatErrorStr_QNAN[ 7 ][ 7 ] =
{
	{ '1',   0,   0,   0,   0,   0,   0 },
	{ '1', '.',   0,   0,   0,   0,   0 },
	{ '1', '.', '$',   0,   0,   0,   0 },
	{ '1', '.', '#', 'R',   0,   0,   0 },
	{ '1', '.', '#', 'Q', 'O',   0,   0 },
	{ '1', '.', '#', 'Q', 'N', 'B',   0 },
	{ '1', '.', '#', 'Q', 'N', 'A', 'N' },
} ;

static u8 FloatErrorStr_INF[ 6 ][ 7 ] =
{
	{ '1',   0,   0,   0,   0,   0,   0 },
	{ '1', '.',   0,   0,   0,   0,   0 },
	{ '1', '.', '$',   0,   0,   0,   0 },
	{ '1', '.', '#', 'J',   0,   0,   0 },
	{ '1', '.', '#', 'I', 'O',   0,   0 },
	{ '1', '.', '#', 'I', 'N', 'F',   0 },
} ;

static u8 FloatErrorStr_IND[ 6 ][ 7 ] =
{
	{ '1',   0,   0,   0,   0,   0,   0 },
	{ '1', '.',   0,   0,   0,   0,   0 },
	{ '1', '.', '$',   0,   0,   0,   0 },
	{ '1', '.', '#', 'J',   0,   0,   0 },
	{ '1', '.', '#', 'I', 'O',   0,   0 },
	{ '1', '.', '#', 'I', 'N', 'D',   0 },
} ;

#if !( defined(WIN32) || defined(WIN64) )

extern u8 CP932ToUTF16Table[] ;
extern u8 CP936ToUTF16Table[] ;
extern u8 CP949ToUTF16Table[] ;
extern u8 CP950ToUTF16Table[] ;
extern u8 CP1252ToUTF16Table[] ;
extern u8 ISO_IEC_8859_15ToUTF16Table[] ;

#endif

CHARCODESYSTEM g_CharCodeSystem ;

// function proto type --------------------------

#if !( defined(WIN32) || defined(WIN64) )

// UTF-16�Ɗe�����R�[�h�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeTableInfo( CHARCODETABLEINFO *TableInfo, u8 *PressTable, int IsSingleCharType = FALSE ) ;

#else

static void SetupCharCodeCP932TableInfo( void ) ;				// UTF-16��Shift-JIS�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP936TableInfo( void ) ;				// UTF-16��GB2312�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP949TableInfo( void ) ;				// UTF-16��UHC�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP950TableInfo( void ) ;				// UTF-16��BIG5�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP1252TableInfo( void ) ;				// UTF-16�Ɖ���(���e�������̕����R�[�h)�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeISO_IEC_8859_15TableInfo( void ) ;		// UTF-16�Ɖ���(���e�������̕����R�[�h)�̑Ή��\�̃Z�b�g�A�b�v���s��

#endif

// �f�R�[�h( �߂�l:�𓀌�̃T�C�Y  -1 �̓G���[  Dest �� NULL �����邱�Ƃ��\ )
static int LzDecode( void *Src, void *Dest ) ;

// function code --------------------------------

// �f�R�[�h( �߂�l:�𓀌�̃T�C�Y  -1 �̓G���[  Dest �� NULL �����邱�Ƃ��\ )
int LzDecode( void *Src, void *Dest )
{
	u32 srcsize, destsize, code, indexsize, keycode, conbo, index ;
	u8 *srcp, *destp, *dp, *sp ;

	destp = (u8 *)Dest ;
	srcp  = (u8 *)Src ;
	
	// �𓀌�̃f�[�^�T�C�Y�𓾂�
	destsize = *((u32 *)&srcp[0]) ;

	// ���k�f�[�^�̃T�C�Y�𓾂�
	srcsize = *((u32 *)&srcp[4]) - 9 ;

	// �L�[�R�[�h
	keycode = srcp[8] ;
	
	// �o�͐悪�Ȃ��ꍇ�̓T�C�Y�����Ԃ�
	if( Dest == NULL )
		return destsize ;
	
	// �W�J�J�n
	sp  = srcp + 9 ;
	dp  = destp ;
	while( srcsize )
	{
		// �L�[�R�[�h�������ŏ����𕪊�
		if( sp[0] != keycode )
		{
			// �񈳏k�R�[�h�̏ꍇ�͂��̂܂܏o��
			*dp = *sp ;
			dp      ++ ;
			sp      ++ ;
			srcsize -- ;
			continue ;
		}
	
		// �L�[�R�[�h���A�����Ă����ꍇ�̓L�[�R�[�h���̂��o��
		if( sp[1] == keycode )
		{
			*dp = (u8)keycode ;
			dp      ++ ;
			sp      += 2 ;
			srcsize -= 2 ;
			
			continue ;
		}

		// ���o�C�g�𓾂�
		code = sp[1] ;

		// �����L�[�R�[�h�����傫�Ȓl�������ꍇ�̓L�[�R�[�h
		// �Ƃ̃o�b�e�B���O�h�~�ׂ̈Ɂ{�P���Ă���̂Ł|�P����
		if( code > keycode ) code -- ;

		sp      += 2 ;
		srcsize -= 2 ;

		// �A�������擾����
		conbo = code >> 3 ;
		if( code & ( 0x1 << 2 ) )
		{
			conbo |= *sp << 5 ;
			sp      ++ ;
			srcsize -- ;
		}
		conbo += MIN_COMPRESS ;	// �ۑ����Ɍ��Z�����ŏ����k�o�C�g���𑫂�

		// �Q�Ƒ��΃A�h���X���擾����
		indexsize = code & 0x3 ;
		switch( indexsize )
		{
		case 0 :
			index = *sp ;
			sp      ++ ;
			srcsize -- ;
			break ;
			
		case 1 :
			index = *((u16 *)sp) ;
			sp      += 2 ;
			srcsize -= 2 ;
			break ;
			
		case 2 :
			index = *((u16 *)sp) | ( sp[2] << 16 ) ;
			sp      += 3 ;
			srcsize -= 3 ;
			break ;
		}
		index ++ ;		// �ۑ����Ɂ|�P���Ă���̂Ł{�P����

		// �W�J
		if( index < conbo )
		{
			u32 num ;

			num  = index ;
			while( conbo > num )
			{
				memcpy( dp, dp - num, num ) ;
				dp    += num ;
				conbo -= num ;
				num   += num ;
			}
			if( conbo != 0 )
			{
				memcpy( dp, dp - num, conbo ) ;
				dp += conbo ;
			}
		}
		else
		{
			memcpy( dp, dp - index, conbo ) ;
			dp += conbo ;
		}
	}

	// �𓀌�̃T�C�Y��Ԃ�
	return (int)destsize ;
}

#if !( defined(WIN32) || defined(WIN64) )

// UTF-16�Ɗe�����R�[�h�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeTableInfo( CHARCODETABLEINFO *TableInfo, u8 *PressTable, int IsSingleCharType )
{
	u32 i ;

	Char128ToBin( PressTable, PressTable ) ;
	LzDecode( PressTable, TableInfo->MultiByteToUTF16 ) ;

	if( IsSingleCharType )
	{
		for( i = 0 ; i < 0x100 ; i ++ )
		{
			TableInfo->UTF16ToMultiByte[ TableInfo->MultiByteToUTF16[ i ] ] = ( u16 )i ;
		}
	}
	else
	{
		for( i = 0 ; i < 0x10000 ; i ++ )
		{
			TableInfo->UTF16ToMultiByte[ TableInfo->MultiByteToUTF16[ i ] ] = ( u16 )i ;
		}
	}
}

#else

// UTF-16��Shift-JIS�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP932TableInfo( void )
{
	wchar_t Dest[ 8 ] ;
	char Src[ 8 ] ;
	unsigned int i ;
	unsigned int Code ;
	CHARCODETABLEINFO *Info = &g_CharCodeSystem.CharCodeCP932Info ;

	if( g_CharCodeSystem.InitializeCharCodeCP932InfoFlag )
	{
		return ;
	}

	for( i = 0 ; i < 0x10000 ; i ++ )
	{
		u32 result ;

		Code = i ;
		if( i < 0x100 )
		{
			Src[ 0 ] = ( char )i ;
			Src[ 1 ] = 0 ;
		}
		else
		{
			Src[ 0 ] = ( char )( ( i & 0xff00 ) >> 8 ) ;
			Src[ 1 ] = ( char )( i & 0xff ) ;
			Src[ 2 ] = 0 ;
			if( ( ( u8 )( ( ( ( u8 )Src[ 0 ] ) ^ 0x20) - ( u8 )0xa1 ) < 0x3c ) == false )
			{
				continue ;
			}
		}

		result = MultiByteToWideChar( 932, MB_ERR_INVALID_CHARS, Src, -1, Dest, 8 ) ;
		if( result != 0 )
		{
			Info->MultiByteToUTF16[ i ] = ( WORD )Dest[ 0 ] ;
			Info->UTF16ToMultiByte[ Dest[ 0 ] ] = ( WORD )i ;
		}
	}

	g_CharCodeSystem.InitializeCharCodeCP932InfoFlag = TRUE ;
}

// UTF-16��GB2312�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP936TableInfo( void )
{
	wchar_t Dest[ 8 ] ;
	char Src[ 8 ] ;
	unsigned int i ;
	unsigned int Code ;
	CHARCODETABLEINFO *Info = &g_CharCodeSystem.CharCodeCP936Info ;

	if( g_CharCodeSystem.InitializeCharCodeCP936InfoFlag )
	{
		return ;
	}

	for( i = 0 ; i < 0x10000 ; i ++ )
	{
		u32 result ;

		Code = i ;
		if( i < 0x100 )
		{
			Src[ 0 ] = ( char )i ;
			Src[ 1 ] = 0 ;
		}
		else
		{
			Src[ 0 ] = ( char )( ( i & 0xff00 ) >> 8 ) ;
			Src[ 1 ] = ( char )( i & 0xff ) ;
			Src[ 2 ] = 0 ;
			if( ( ( u8 )Src[ 0 ] & 80 ) == 0 )
			{
				continue ;
			}
		}

		result = MultiByteToWideChar( 936, MB_ERR_INVALID_CHARS, Src, -1, Dest, 8 ) ;
		if( result != 0 )
		{
			Info->MultiByteToUTF16[ i ] = ( WORD )Dest[ 0 ] ;
			Info->UTF16ToMultiByte[ Dest[ 0 ] ] = ( WORD )i ;
		}
	}

	g_CharCodeSystem.InitializeCharCodeCP936InfoFlag = TRUE ;
}

// UTF-16��UHC�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP949TableInfo( void )
{
	wchar_t Dest[ 8 ] ;
	char Src[ 8 ] ;
	unsigned int i ;
	unsigned int Code ;
	CHARCODETABLEINFO *Info = &g_CharCodeSystem.CharCodeCP949Info ;

	if( g_CharCodeSystem.InitializeCharCodeCP949InfoFlag )
	{
		return ;
	}

	for( i = 0 ; i < 0x10000 ; i ++ )
	{
		u32 result ;

		Code = i ;
		if( i < 0x100 )
		{
			Src[ 0 ] = ( char )i ;
			Src[ 1 ] = 0 ;
		}
		else
		{
			Src[ 0 ] = ( char )( ( i & 0xff00 ) >> 8 ) ;
			Src[ 1 ] = ( char )( i & 0xff ) ;
			Src[ 2 ] = 0 ;
			if( ( ( u8 )Src[ 0 ] & 80 ) == 0 )
			{
				continue ;
			}
		}

		result = MultiByteToWideChar( 949, MB_ERR_INVALID_CHARS, Src, -1, Dest, 8 ) ;
		if( result != 0 )
		{
			Info->MultiByteToUTF16[ i ] = ( WORD )Dest[ 0 ] ;
			Info->UTF16ToMultiByte[ Dest[ 0 ] ] = ( WORD )i ;
		}
	}

	g_CharCodeSystem.InitializeCharCodeCP949InfoFlag = TRUE ;
}

// UTF-16��BIG5�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP950TableInfo( void )
{
	wchar_t Dest[ 8 ] ;
	char Src[ 8 ] ;
	unsigned int i ;
	unsigned int Code ;
	CHARCODETABLEINFO *Info = &g_CharCodeSystem.CharCodeCP950Info ;

	if( g_CharCodeSystem.InitializeCharCodeCP950InfoFlag )
	{
		return ;
	}

	for( i = 0 ; i < 0x10000 ; i ++ )
	{
		u32 result ;

		Code = i ;
		if( i < 0x100 )
		{
			Src[ 0 ] = ( char )i ;
			Src[ 1 ] = 0 ;
		}
		else
		{
			Src[ 0 ] = ( char )( ( i & 0xff00 ) >> 8 ) ;
			Src[ 1 ] = ( char )( i & 0xff ) ;
			Src[ 2 ] = 0 ;
			if( ( ( u8 )Src[ 0 ] & 80 ) == 0 )
			{
				continue ;
			}
		}

		result = MultiByteToWideChar( 950, MB_ERR_INVALID_CHARS, Src, -1, Dest, 8 ) ;
		if( result != 0 )
		{
			Info->MultiByteToUTF16[ i ] = ( WORD )Dest[ 0 ] ;
			Info->UTF16ToMultiByte[ Dest[ 0 ] ] = ( WORD )i ;
		}
	}

	g_CharCodeSystem.InitializeCharCodeCP950InfoFlag = TRUE ;
}

// UTF-16�Ɖ���(���e�������̕����R�[�h)�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeCP1252TableInfo( void )
{
	wchar_t Dest[ 8 ] ;
	char Src[ 8 ] ;
	unsigned int i ;
	unsigned int Code ;
	CHARCODETABLEINFO *Info = &g_CharCodeSystem.CharCodeCP1252Info ;

	if( g_CharCodeSystem.InitializeCharCodeCP1252InfoFlag )
	{
		return ;
	}

	for( i = 0 ; i < 0x100 ; i ++ )
	{
		u32 result ;

		Code = i ;
		Src[ 0 ] = ( char )i ;
		Src[ 1 ] = 0 ;

		result = MultiByteToWideChar( 1252, MB_ERR_INVALID_CHARS, Src, -1, Dest, 8 ) ;
		if( result != 0 )
		{
			Info->MultiByteToUTF16[ i ] = ( WORD )Dest[ 0 ] ;
			Info->UTF16ToMultiByte[ Dest[ 0 ] ] = ( WORD )i ;
		}
	}

	g_CharCodeSystem.InitializeCharCodeCP1252InfoFlag = TRUE ;
}

// UTF-16�Ɖ���(���e�������̕����R�[�h)�̑Ή��\�̃Z�b�g�A�b�v���s��
static void SetupCharCodeISO_IEC_8859_15TableInfo( void )
{
	wchar_t Dest[ 8 ] ;
	char Src[ 8 ] ;
	unsigned int i ;
	unsigned int Code ;
	CHARCODETABLEINFO *Info = &g_CharCodeSystem.CharCodeISO_IEC_8859_15Info ;

	if( g_CharCodeSystem.InitializeCharCodeISO_IEC_8859_15InfoFlag )
	{
		return ;
	}

	for( i = 0 ; i < 0x100 ; i ++ )
	{
		u32 result ;

		Code = i ;
		Src[ 0 ] = ( char )i ;
		Src[ 1 ] = 0 ;

		result = MultiByteToWideChar( 1252, MB_ERR_INVALID_CHARS, Src, -1, Dest, 8 ) ;
		if( result != 0 )
		{
			Info->MultiByteToUTF16[ i ] = ( WORD )Dest[ 0 ] ;
			Info->UTF16ToMultiByte[ Dest[ 0 ] ] = ( WORD )i ;
		}
	}
	Info->MultiByteToUTF16[ 0xA4 ] = 0x20AC ;
	Info->UTF16ToMultiByte[ 0x20AC ] = 0xA4 ;

	Info->MultiByteToUTF16[ 0xA6 ] = 0x0160 ;
	Info->UTF16ToMultiByte[ 0x0160 ] = 0xA6 ;

	Info->MultiByteToUTF16[ 0xA8 ] = 0x0161 ;
	Info->UTF16ToMultiByte[ 0x0161 ] = 0xA8 ;

	Info->MultiByteToUTF16[ 0xB4 ] = 0x017D ;
	Info->UTF16ToMultiByte[ 0x017D ] = 0xB4 ;

	Info->MultiByteToUTF16[ 0xB8 ] = 0x017E ;
	Info->UTF16ToMultiByte[ 0x017E ] = 0xB8 ;

	Info->MultiByteToUTF16[ 0xBC ] = 0x0152 ;
	Info->UTF16ToMultiByte[ 0x0152 ] = 0xBC ;

	Info->MultiByteToUTF16[ 0xBD ] = 0x0153 ;
	Info->UTF16ToMultiByte[ 0x0153 ] = 0xBD ;

	Info->MultiByteToUTF16[ 0xBE ] = 0x0178 ;
	Info->UTF16ToMultiByte[ 0x0178 ] = 0xBE ;

	g_CharCodeSystem.InitializeCharCodeISO_IEC_8859_15InfoFlag = TRUE ;
}

#endif

// �����R�[�h���C�u�����̏�����
extern int InitCharCode( void )
{
	// ���ɏ������ς݂̏ꍇ�͉��������I��
	if( g_CharCodeSystem.InitializeFlag )
	{
		return 0 ;
	}

#if !( defined(WIN32) || defined(WIN64) )

	// �L�����N�^�R�[�h�Ή��\�Z�b�g�A�b�v
	SetupCharCodeTableInfo( &g_CharCodeSystem.CharCodeCP932Info,           CP932ToUTF16Table ) ;
	SetupCharCodeTableInfo( &g_CharCodeSystem.CharCodeCP936Info,           CP936ToUTF16Table ) ;
	SetupCharCodeTableInfo( &g_CharCodeSystem.CharCodeCP949Info,           CP949ToUTF16Table ) ;
	SetupCharCodeTableInfo( &g_CharCodeSystem.CharCodeCP950Info,           CP950ToUTF16Table ) ;
	SetupCharCodeTableInfo( &g_CharCodeSystem.CharCodeCP1252Info,          CP1252ToUTF16Table, TRUE ) ;
	SetupCharCodeTableInfo( &g_CharCodeSystem.CharCodeISO_IEC_8859_15Info, ISO_IEC_8859_15ToUTF16Table, TRUE ) ;

#endif

	// �������t���O�𗧂Ă�
	g_CharCodeSystem.InitializeFlag = TRUE ;

	// ����I��
	return 0 ;
}

// �w��̃R�[�h�y�[�W�̏��ŏ��T�C�Y���擾����( �߂�l�F�o�C�g�� )
__inline int GetCharCodeFormatUnitSize_inline( int CharCodeFormat )
{
	// �Ή����Ă��Ȃ��R�[�h�y�[�W�̏ꍇ�͉��������I��
	switch( CharCodeFormat )
	{
	case CHARCODEFORMAT_SHIFTJIS :
	case CHARCODEFORMAT_GB2312 :
	case CHARCODEFORMAT_WINDOWS_1252 :
	case CHARCODEFORMAT_ISO_IEC_8859_15 :
	case CHARCODEFORMAT_UHC :
	case CHARCODEFORMAT_BIG5 :
	case CHARCODEFORMAT_ASCII :
		return 1 ;

	case CHARCODEFORMAT_UTF16LE :
	case CHARCODEFORMAT_UTF16BE :
		return 2 ;

	case CHARCODEFORMAT_UTF8 :
		return 1 ;

	case CHARCODEFORMAT_UTF32LE :
	case CHARCODEFORMAT_UTF32BE :
		return 4 ;

	default :
		return -1 ;
	}
}

// �w��̃R�[�h�y�[�W�̏��ŏ��T�C�Y���擾����( �߂�l�F�o�C�g�� )
extern int GetCharCodeFormatUnitSize( int CharCodeFormat )
{
	return GetCharCodeFormatUnitSize_inline( CharCodeFormat ) ;
}

// �P�����̃o�C�g�����擾����( �߂�l�F�P�����̃o�C�g�� )
__inline int GetCharBytes_inline( const char *CharCode, int CharCodeFormat )
{
	switch( CharCodeFormat )
	{
	case CHARCODEFORMAT_SHIFTJIS :
		return CHECK_SHIFTJIS_2BYTE( ( ( u8 * )CharCode )[ 0 ] ) ? 2 : 1 ;
	//	return ( ( ( u8 * )CharCode )[ 0 ] >= 0x81 && ( ( u8 * )CharCode )[ 0 ] <= 0x9F ) || ( ( ( u8 * )CharCode )[ 0 ] >= 0xE0 && ( ( u8 * )CharCode )[ 0 ] <= 0xFC ) ;
		break ;

	case CHARCODEFORMAT_ASCII :
	case CHARCODEFORMAT_WINDOWS_1252 :
	case CHARCODEFORMAT_ISO_IEC_8859_15 :
		return 1 ;

	case CHARCODEFORMAT_GB2312 :
	case CHARCODEFORMAT_UHC :
	case CHARCODEFORMAT_BIG5 :
		return ( ( ( ( u8 * )CharCode )[ 0 ] & 0x80 ) != 0 ) ? 2 : 1 ;

	case CHARCODEFORMAT_UTF16LE :
		return ( ( ( ( u8 * )CharCode )[ 0 ] | ( ( ( u8 * )CharCode )[ 1 ] << 8 ) ) & 0xfc00 ) == 0xd800 ? 4 : 2 ;

	case CHARCODEFORMAT_UTF16BE :
		return ( ( ( ( ( u8 * )CharCode )[ 0 ] << 8 ) | ( ( u8 * )CharCode )[ 1 ] ) & 0xfc00 ) == 0xd800 ? 4 : 2 ;

	case CHARCODEFORMAT_UTF8 :
		if( ( ( ( u8 * )CharCode )[ 0 ] & 0x80 ) == 0x00 )
		{
			return 1 ;
		}

		if( ( ( ( u8 * )CharCode )[ 0 ] & 0xe0 ) == 0xc0 )
		{
			return 2 ;
		}

		if( ( ( ( u8 * )CharCode )[ 0 ] & 0xf0 ) == 0xe0 )
		{
			return 3 ;
		}

		if( ( ( ( u8 * )CharCode )[ 0 ] & 0xf8 ) == 0xf0 )
		{
			return 4 ;
		}

		if( ( ( ( u8 * )CharCode )[ 0 ] & 0xfc ) == 0xf8 )
		{
			return 5 ;
		}

		if( ( ( ( u8 * )CharCode )[ 0 ] & 0xfe ) == 0xfc )
		{
			return 6 ;
		}
		break ;

	case CHARCODEFORMAT_UTF32LE :
	case CHARCODEFORMAT_UTF32BE :
		return 4 ;
	}

	return -1 ;
}

// �P�����̃o�C�g�����擾����( �߂�l�F�P�����̃o�C�g�� )
extern int GetCharBytes( const char *CharCode, int CharCodeFormat )
{
	return GetCharBytes_inline( CharCode, CharCodeFormat ) ;
}

// �P�����̕����R�[�h�ƕ����̃o�C�g�����擾����
__inline u32 GetCharCode_inline( const char *CharCode, int CharCodeFormat, int *CharBytes )
{
	int UseSrcSize ;
	u32 DestCode ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		if( ( ( u8 * )CharCode )[ 0 ] == 0 )
		{
			if( CharBytes != NULL )
			{
				*CharBytes = 1 ;
			}
			return 0 ;
		}
		break ;

	case 2 :
		if( *( ( u16 * )CharCode ) == 0 )
		{
			if( CharBytes != NULL )
			{
				*CharBytes = 2 ;
			}
			return 0 ;
		}
		break ;

	case 4 :
		if( *( ( u32 * )CharCode ) == 0 )
		{
			if( CharBytes != NULL )
			{
				*CharBytes = 4 ;
			}
			return 0 ;
		}
		break ;
	}

	UseSrcSize = GetCharBytes_inline( CharCode, CharCodeFormat ) ;

	switch( CharCodeFormat )
	{
	case CHARCODEFORMAT_SHIFTJIS :
	case CHARCODEFORMAT_GB2312 :
	case CHARCODEFORMAT_UHC :
	case CHARCODEFORMAT_BIG5 :
		if( UseSrcSize == 2 )
		{
			DestCode = ( ( ( u8 * )CharCode )[ 0 ] << 8 ) | ( ( u8 * )CharCode )[ 1 ] ;
		}
		else
		{
			DestCode = ( ( u8 * )CharCode )[ 0 ] ;
		}
		break ;

	case CHARCODEFORMAT_ASCII :
	case CHARCODEFORMAT_WINDOWS_1252 :
	case CHARCODEFORMAT_ISO_IEC_8859_15 :
		DestCode = ( ( u8 * )CharCode )[ 0 ] ;
		break ;

	case CHARCODEFORMAT_UTF16LE :
	case CHARCODEFORMAT_UTF16BE :
		{
			u32 SrcCode1 ;
			u32 SrcCode2 ;

			if( CharCodeFormat == CHARCODEFORMAT_UTF16BE )
			{
				SrcCode1 = ( ( ( u8 * )CharCode )[ 0 ] << 8 ) | ( ( u8 * )CharCode )[ 1 ] ;
			}
			else
			{
				SrcCode1 = ( ( u8 * )CharCode )[ 0 ] | ( ( ( u8 * )CharCode )[ 1 ] << 8 ) ;
			}

			if( UseSrcSize == 4 )
			{
				if( CharCodeFormat == CHARCODEFORMAT_UTF16BE )
				{
					SrcCode2 = ( ( ( u8 * )CharCode )[ 2 ] << 8 ) | ( ( u8 * )CharCode )[ 3 ] ;
				}
				else
				{
					SrcCode2 = ( ( u8 * )CharCode )[ 2 ] | ( ( ( u8 * )CharCode )[ 3 ] << 8 ) ;
				}

				DestCode = ( ( ( SrcCode1 & 0x3ff ) << 10 ) | ( SrcCode2 & 0x3ff ) ) + 0x10000 ;
			}
			else
			{
				DestCode = SrcCode1 ;
			}
		}
		break ;

	case CHARCODEFORMAT_UTF8 :
		switch( UseSrcSize )
		{
		case 1 :
			DestCode = ( ( u8 * )CharCode )[ 0 ] ;
			break ;

		case 2 :
			DestCode = ( ( ( ( u8 * )CharCode )[ 0 ] & 0x1f ) << 6 ) | ( ( ( u8 * )CharCode )[ 1 ] & 0x3f ) ;
			break ;

		case 3 :
			DestCode = ( ( ( ( u8 * )CharCode )[ 0 ] & 0x0f ) << 12 ) | ( ( ( ( u8 * )CharCode )[ 1 ] & 0x3f ) << 6 ) | ( ( ( u8 * )CharCode )[ 2 ] & 0x3f ) ;
			break ;

		case 4 :
			DestCode = ( ( ( ( u8 * )CharCode )[ 0 ] & 0x07 ) << 18 ) | ( ( ( ( u8 * )CharCode )[ 1 ] & 0x3f ) << 12 ) | ( ( ( ( u8 * )CharCode )[ 2 ] & 0x3f ) << 6 ) | ( ( ( u8 * )CharCode )[ 3 ] & 0x3f ) ;
			break ;

		case 5 :
			DestCode = ( ( ( ( u8 * )CharCode )[ 0 ] & 0x03 ) << 24 ) | ( ( ( ( u8 * )CharCode )[ 1 ] & 0x3f ) << 18 ) | ( ( ( ( u8 * )CharCode )[ 2 ] & 0x3f ) << 12 ) | ( ( ( ( u8 * )CharCode )[ 3 ] & 0x3f ) << 6 ) | ( ( ( u8 * )CharCode )[ 4 ] & 0x3f ) ;
			break ;

		case 6 :
			DestCode = ( ( ( ( u8 * )CharCode )[ 0 ] & 0x01 ) << 30 ) | ( ( ( ( u8 * )CharCode )[ 1 ] & 0x3f ) << 24 ) | ( ( ( ( u8 * )CharCode )[ 2 ] & 0x3f ) << 18 ) | ( ( ( ( u8 * )CharCode )[ 3 ] & 0x3f ) << 12 ) | ( ( ( ( u8 * )CharCode )[ 4 ] & 0x3f ) << 6 ) | ( ( ( u8 * )CharCode )[ 5 ] & 0x3f ) ;
			break ;
		}
		break ;

	case CHARCODEFORMAT_UTF32LE :
		DestCode = ( ( u8 * )CharCode )[ 0 ] | ( ( ( u8 * )CharCode )[ 1 ] << 8 ) | ( ( ( u8 * )CharCode )[ 2 ] << 16 ) | ( ( ( u8 * )CharCode )[ 3 ] << 24 ) ;
		break ;

	case CHARCODEFORMAT_UTF32BE :
		DestCode = ( ( ( u8 * )CharCode )[ 0 ] << 24 ) | ( ( ( u8 * )CharCode )[ 1 ] << 16 ) | ( ( ( u8 * )CharCode )[ 2 ] << 8 ) | ( ( u8 * )CharCode )[ 3 ] ;
		break ;

	default :
		return 0 ;
	}

	if( CharBytes != NULL )
	{
		*CharBytes = UseSrcSize ;
	}

	return DestCode ;
}

// �P�����̕����R�[�h�ƕ����̃o�C�g�����擾����
extern u32 GetCharCode( const char *CharCode, int CharCodeFormat, int *CharBytes )
{
	return GetCharCode_inline( CharCode, CharCodeFormat, CharBytes ) ;
}

// �����R�[�h��ʏ�̕�����ɕϊ�����A�I�[�Ƀk�������͏������܂Ȃ�( �߂�l�F�������񂾃o�C�g�� )
__inline int PutCharCode_inline( u32 CharCode, int CharCodeFormat, char *Dest )
{
	switch( CharCodeFormat )
	{
	case CHARCODEFORMAT_SHIFTJIS :
	case CHARCODEFORMAT_GB2312 :
	case CHARCODEFORMAT_UHC :
	case CHARCODEFORMAT_BIG5 :
		if( CharCode >= 0x100 )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( CharCode >> 8 ) ;
				( ( u8 * )Dest )[ 1 ] = ( u8 )( CharCode ) ;
			}
			return 2;
		}
		else
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )CharCode ;
			}
			return 1 ;
		}

	case CHARCODEFORMAT_ASCII :
	case CHARCODEFORMAT_WINDOWS_1252 :
	case CHARCODEFORMAT_ISO_IEC_8859_15 :
		if( Dest != NULL )
		{
			( ( u8 * )Dest )[ 0 ] = ( u8 )CharCode ;
		}
		return 1 ;

	case CHARCODEFORMAT_UTF16LE :
	case CHARCODEFORMAT_UTF16BE :
		{
			u32 DestCode1 ;
			u32 DestCode2 ;
			u32 DestSize ;

			// UTF-16 �ŕ\���ł��Ȃ��l�̏ꍇ�̓L�����Z��
			if( CharCode > 0x10ffff )
			{
				return 0 ;
			}

			if( CharCode > 0xffff )
			{
				DestCode1 = 0xd800 | ( ( ( CharCode - 0x10000 ) >> 10 ) & 0x3ff ) ;
				DestCode2 = 0xdc00 | (   ( CharCode - 0x10000 )         & 0x3ff ) ;

				DestSize = 4 ;
			}
			else
			{
				DestCode1 = CharCode ;
				DestCode2 = 0 ;

				DestSize = 2 ;
			}

			if( Dest != NULL )
			{
				if( CharCodeFormat == CHARCODEFORMAT_UTF16BE )
				{
					( ( u8 * )Dest )[ 0 ] = ( u8 )( DestCode1 >> 8 ) ;
					( ( u8 * )Dest )[ 1 ] = ( u8 )( DestCode1 ) ;

					if( DestCode2 != 0 )
					{
						( ( u8 * )Dest )[ 2 ] = ( u8 )( DestCode2 >> 8 ) ;
						( ( u8 * )Dest )[ 3 ] = ( u8 )( DestCode2 ) ;
					}
				}
				else
				{
					( ( u8 * )Dest )[ 0 ] = ( u8 )( DestCode1 ) ;
					( ( u8 * )Dest )[ 1 ] = ( u8 )( DestCode1 >> 8 ) ;

					if( DestCode2 != 0 )
					{
						( ( u8 * )Dest )[ 2 ] = ( u8 )( DestCode2 ) ;
						( ( u8 * )Dest )[ 3 ] = ( u8 )( DestCode2 >> 8 ) ;
					}
				}
			}

			return DestSize ;
		}

	case CHARCODEFORMAT_UTF8 :
		if( CharCode <= 0x7f )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( CharCode ) ;
			}

			return 1 ;
		}
		else
		if( CharCode <= 0x7ff )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( 0xc0 | ( ( CharCode >> 6 ) & 0x1f ) ) ;
				( ( u8 * )Dest )[ 1 ] = ( u8 )( 0x80 | ( ( CharCode      ) & 0x3f ) ) ;
			}

			return 2 ;
		}
		else
		if( CharCode <= 0xffff )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( 0xe0 | ( ( CharCode >> 12 ) & 0x0f ) ) ;
				( ( u8 * )Dest )[ 1 ] = ( u8 )( 0x80 | ( ( CharCode >>  6 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 2 ] = ( u8 )( 0x80 | ( ( CharCode       ) & 0x3f ) ) ;
			}

			return 3 ;
		}
		else
		if( CharCode <= 0x1fffff )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( 0xf0 | ( ( CharCode >> 18 ) & 0x07 ) ) ;
				( ( u8 * )Dest )[ 1 ] = ( u8 )( 0x80 | ( ( CharCode >> 12 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 2 ] = ( u8 )( 0x80 | ( ( CharCode >>  6 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 3 ] = ( u8 )( 0x80 | ( ( CharCode       ) & 0x3f ) ) ;
			}

			return 4 ;
		}
		else
		if( CharCode <= 0x3ffffff )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( 0xf8 | ( ( CharCode >> 24 ) & 0x03 ) ) ;
				( ( u8 * )Dest )[ 1 ] = ( u8 )( 0x80 | ( ( CharCode >> 18 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 2 ] = ( u8 )( 0x80 | ( ( CharCode >> 12 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 3 ] = ( u8 )( 0x80 | ( ( CharCode >>  6 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 4 ] = ( u8 )( 0x80 | ( ( CharCode       ) & 0x3f ) ) ;
			}

			return 5 ;
		}
		else
		if( CharCode <= 0x7fffffff )
		{
			if( Dest != NULL )
			{
				( ( u8 * )Dest )[ 0 ] = ( u8 )( 0xfc | ( ( CharCode >> 30 ) & 0x01 ) ) ;
				( ( u8 * )Dest )[ 1 ] = ( u8 )( 0x80 | ( ( CharCode >> 24 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 2 ] = ( u8 )( 0x80 | ( ( CharCode >> 18 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 3 ] = ( u8 )( 0x80 | ( ( CharCode >> 12 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 4 ] = ( u8 )( 0x80 | ( ( CharCode >>  6 ) & 0x3f ) ) ;
				( ( u8 * )Dest )[ 5 ] = ( u8 )( 0x80 | ( ( CharCode       ) & 0x3f ) ) ;
			}

			return 6 ;
		}
		else
		{
			return 0 ;
		}

	case CHARCODEFORMAT_UTF32LE :
		if( Dest != NULL )
		{
			( ( u8 * )Dest )[ 0 ] = ( u8 )( CharCode       ) ;
			( ( u8 * )Dest )[ 1 ] = ( u8 )( CharCode >>  8 ) ;
			( ( u8 * )Dest )[ 2 ] = ( u8 )( CharCode >> 16 ) ;
			( ( u8 * )Dest )[ 3 ] = ( u8 )( CharCode >> 24 ) ;
		}
		return 4 ;

	case CHARCODEFORMAT_UTF32BE :
		if( Dest != NULL )
		{
			( ( u8 * )Dest )[ 0 ] = ( u8 )( CharCode >> 24 ) ;
			( ( u8 * )Dest )[ 1 ] = ( u8 )( CharCode >> 16 ) ;
			( ( u8 * )Dest )[ 2 ] = ( u8 )( CharCode >>  8 ) ;
			( ( u8 * )Dest )[ 3 ] = ( u8 )( CharCode       ) ;
		}
		return 4 ;

	default :
		return 0 ;
	}

	return 0 ;
}

// �����R�[�h��ʏ�̕�����ɕϊ�����A�I�[�Ƀk�������͏������܂Ȃ�( �߂�l�F�������񂾃o�C�g�� )
extern int PutCharCode( u32 CharCode, int CharCodeFormat, char *Dest )
{
	return PutCharCode_inline( CharCode, CharCodeFormat, Dest ) ;
}

// �����R�[�h���w��̃R�[�h�y�[�W�̕����ɕϊ�����
__inline u32 ConvCharCode_inline( u32 SrcCharCode, int SrcCharCodeFormat, int DestCharCodeFormat )
{
	// �L�����N�^�[�R�[�h�e�[�u��������������Ă��Ȃ������珉����
	if( g_CharCodeSystem.InitializeFlag == FALSE )
	{
		InitCharCode() ;
	}

	if( SrcCharCodeFormat == DestCharCodeFormat )
	{
		return SrcCharCode ;
	}

	switch( SrcCharCodeFormat )
	{
	case CHARCODEFORMAT_SHIFTJIS :
	case CHARCODEFORMAT_GB2312 :
	case CHARCODEFORMAT_WINDOWS_1252 :
	case CHARCODEFORMAT_ISO_IEC_8859_15 :
	case CHARCODEFORMAT_UHC :
	case CHARCODEFORMAT_BIG5 :
		{
			u32 Unicode ;

			if( DestCharCodeFormat == CHARCODEFORMAT_ASCII )
			{
				if( SrcCharCode > 0xff )
				{
					return 0 ;
				}
				return SrcCharCode ;
			}

			switch( SrcCharCodeFormat )
			{
			case CHARCODEFORMAT_SHIFTJIS :
				Unicode = g_CharCodeSystem.CharCodeCP932Info.MultiByteToUTF16[ SrcCharCode ] ;
				break ;

			case CHARCODEFORMAT_GB2312 :
				Unicode = g_CharCodeSystem.CharCodeCP936Info.MultiByteToUTF16[ SrcCharCode ] ;
				break ;

			case CHARCODEFORMAT_UHC :
				Unicode = g_CharCodeSystem.CharCodeCP949Info.MultiByteToUTF16[ SrcCharCode ] ;
				break ;

			case CHARCODEFORMAT_BIG5 :
				Unicode = g_CharCodeSystem.CharCodeCP950Info.MultiByteToUTF16[ SrcCharCode ] ;
				break ;

			case CHARCODEFORMAT_WINDOWS_1252 :
				Unicode = g_CharCodeSystem.CharCodeCP1252Info.MultiByteToUTF16[ SrcCharCode ] ;
				break ;

			case CHARCODEFORMAT_ISO_IEC_8859_15 :
				Unicode = g_CharCodeSystem.CharCodeISO_IEC_8859_15Info.MultiByteToUTF16[ SrcCharCode ] ;
				break ;

			default :
				return 0 ;
			}

			switch( DestCharCodeFormat )
			{
			case CHARCODEFORMAT_SHIFTJIS :
				return g_CharCodeSystem.CharCodeCP932Info.UTF16ToMultiByte[ Unicode ] ;

			case CHARCODEFORMAT_GB2312 :
				return g_CharCodeSystem.CharCodeCP936Info.UTF16ToMultiByte[ Unicode ] ;

			case CHARCODEFORMAT_UHC :
				return g_CharCodeSystem.CharCodeCP949Info.UTF16ToMultiByte[ Unicode ] ;

			case CHARCODEFORMAT_BIG5 :
				return g_CharCodeSystem.CharCodeCP950Info.UTF16ToMultiByte[ Unicode ] ;

			case CHARCODEFORMAT_WINDOWS_1252 :
				return g_CharCodeSystem.CharCodeCP1252Info.UTF16ToMultiByte[ Unicode ] ;

			case CHARCODEFORMAT_ISO_IEC_8859_15 :
				return g_CharCodeSystem.CharCodeISO_IEC_8859_15Info.UTF16ToMultiByte[ Unicode ] ;

			case CHARCODEFORMAT_UTF16LE :
			case CHARCODEFORMAT_UTF16BE :
			case CHARCODEFORMAT_UTF8 :
			case CHARCODEFORMAT_UTF32LE :
			case CHARCODEFORMAT_UTF32BE :
				return Unicode ;

			default :
				return 0 ;
			}
		}
		return 0;

	case CHARCODEFORMAT_ASCII :
		return SrcCharCode ;

	case CHARCODEFORMAT_UTF16LE :
	case CHARCODEFORMAT_UTF16BE :
	case CHARCODEFORMAT_UTF8 :
	case CHARCODEFORMAT_UTF32LE :
	case CHARCODEFORMAT_UTF32BE :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_SHIFTJIS :
			if( SrcCharCode > 0xffff )
			{
				return 0 ;
			}
			return g_CharCodeSystem.CharCodeCP932Info.UTF16ToMultiByte[ SrcCharCode ] ;

		case CHARCODEFORMAT_GB2312 :
			if( SrcCharCode > 0xffff )
			{
				return 0 ;
			}
			return g_CharCodeSystem.CharCodeCP936Info.UTF16ToMultiByte[ SrcCharCode ] ;

		case CHARCODEFORMAT_UHC :
			if( SrcCharCode > 0xffff )
			{
				return 0 ;
			}
			return g_CharCodeSystem.CharCodeCP949Info.UTF16ToMultiByte[ SrcCharCode ] ;

		case CHARCODEFORMAT_BIG5 :
			if( SrcCharCode > 0xffff )
			{
				return 0 ;
			}
			return g_CharCodeSystem.CharCodeCP950Info.UTF16ToMultiByte[ SrcCharCode ] ;

		case CHARCODEFORMAT_WINDOWS_1252 :
			if( SrcCharCode > 0xffff )
			{
				return 0 ;
			}
			return g_CharCodeSystem.CharCodeCP1252Info.UTF16ToMultiByte[ SrcCharCode ] ;

		case CHARCODEFORMAT_ISO_IEC_8859_15 :
			if( SrcCharCode > 0xffff )
			{
				return 0 ;
			}
			return g_CharCodeSystem.CharCodeISO_IEC_8859_15Info.UTF16ToMultiByte[ SrcCharCode ] ;

		case CHARCODEFORMAT_UTF16LE :
		case CHARCODEFORMAT_UTF16BE :
		case CHARCODEFORMAT_UTF8 :
		case CHARCODEFORMAT_UTF32LE :
		case CHARCODEFORMAT_UTF32BE :
			return SrcCharCode ;

		default :
			return 0 ;
		}
		return 0 ;

	default :
		return 0 ;
	}

	return 0 ;
}

// �����R�[�h���w��̃R�[�h�y�[�W�̕����ɕϊ�����
extern u32 ConvCharCode( u32 SrcCharCode, int SrcCharCodeFormat, int DestCharCodeFormat )
{
	CHARCODETABLE_INITCHECK( SrcCharCodeFormat )
	CHARCODETABLE_INITCHECK( DestCharCodeFormat )

	return ConvCharCode_inline( SrcCharCode, SrcCharCodeFormat, DestCharCodeFormat ) ;
}

// �P�����S�o�C�g�̔z����A�ʃR�[�h�y�[�W�̂P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern int ConvCharCodeString( const u32 *Src, int SrcCharCodeFormat, u32 *Dest, int DestCharCodeFormat )
{
	int DestSize ;
	u32 DestCode ;

	CHARCODETABLE_INITCHECK( SrcCharCodeFormat )
	CHARCODETABLE_INITCHECK( DestCharCodeFormat )

	DestSize = 0 ;
	for(;;)
	{
		if( *Src == 0 )
		{
			break ;
		}

		DestCode = ConvCharCode_inline( *Src, SrcCharCodeFormat, DestCharCodeFormat ) ;
		Src ++ ;

		if( DestCode != 0 )
		{
			if( Dest != NULL )
			{
				*Dest = DestCode ;
				Dest ++ ;
			}

			DestSize += 4 ;
		}
	}

	if( Dest != NULL )
	{
		*Dest = 0 ;
		Dest ++ ;
	}
	DestSize += 4 ;

	return DestSize ;
}

// ��������P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
__inline int StringToCharCodeString_inline( const char *Src, int SrcCharCodeFormat, u32 *Dest )
{
	int DestSize ;
	u32 SrcCode ;
	const u8 *SrcStr ;
	int UseSrcSize ;

	SrcStr = ( u8 * )Src ;
	DestSize = 0 ;
	for(;;)
	{
		SrcCode = GetCharCode_inline( ( const char * )SrcStr, SrcCharCodeFormat, &UseSrcSize ) ;
		SrcStr += UseSrcSize ;
		if( SrcCode == 0 )
		{
			break ;
		}

		if( Dest != NULL )
		{
			*Dest = SrcCode ;
			Dest ++ ;
		}
		DestSize += 4 ;
	}

	if( Dest != NULL )
	{
		*Dest = 0 ;
		Dest ++ ;
	}
	DestSize += 4 ;

	return DestSize ;
}

// ��������P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern int StringToCharCodeString( const char *Src, int SrcCharCodeFormat, u32 *Dest )
{
	return StringToCharCodeString_inline( Src, SrcCharCodeFormat, Dest ) ;
}

// �P�����S�o�C�g�̔z��𕶎���ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern int CharCodeStringToString( const u32 *Src, char *Dest, int DestCharCodeFormat )
{
	int DestSize ;
	u8 *DestStr ;
	int WriteSize ;

	DestStr = ( u8 * )Dest ;
	DestSize = 0 ;
	for(;;)
	{
		if( *Src == 0 )
		{
			break ;
		}

		WriteSize = PutCharCode_inline( *Src, DestCharCodeFormat, ( char * )DestStr ) ;
		if( DestStr != NULL )
		{
			DestStr += WriteSize ;
		}
		DestSize += WriteSize ;
		Src ++ ;
	}

	switch( DestCharCodeFormat )
	{
	case 1 :
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = 0 ;
			DestStr += 1;
		}
		DestSize += 1 ;
		break ;

	case 2 :
		if( DestStr != NULL )
		{
			( ( u16 * )DestStr )[ 0 ] = 0 ;
			DestStr += 2;
		}
		DestSize += 2 ;
		break ;

	case 4 :
		if( DestStr != NULL )
		{
			( ( u32 * )DestStr )[ 0 ] = 0 ;
			DestStr += 4 ;
		}
		DestSize += 4 ;
		break ;
	}

	return DestSize ;
}

// ConvString �̖`�������̃}�N��
#define CONVSTRING_BEGIN				\
	int				DestSize ;			\
	u32			Unicode ;			\
	const u8 *	SrcStr ;			\
	u8 *			DestStr ;			\
										\
	SrcStr   = ( u8 * )Src ;			\
	DestStr  = ( u8 * )Dest ;			\
	DestSize = 0 ;


// ConvString �� ShiftJIS����Unicode���擾����܂ł̏���
__inline bool ConvString_SrcCode_SHIFTJIS( const u8 *( &SrcStr ), u32 &Unicode )
{
	u32 SrcCode ;

	if( CHECK_SHIFTJIS_2BYTE( ( ( u8 * )SrcStr )[ 0 ] ) )
	{
		SrcCode = ( ( ( u8 * )SrcStr )[ 0 ] << 8 ) | ( ( u8 * )SrcStr )[ 1 ] ;
		SrcStr += 2 ;
	}
	else
	{
		SrcCode = ( ( u8 * )SrcStr )[ 0 ] ;
		if( SrcCode == 0 )
		{
			return false;
		}
		SrcStr ++ ;
	}

	Unicode = g_CharCodeSystem.CharCodeCP932Info.MultiByteToUTF16[ SrcCode ] ;

	return true;
}

// ConvString �� UTF16LE����Unicode���擾����܂ł̏���
__inline bool ConvString_SrcCode_UTF16LE( const u8 *( &SrcStr ), u32 &Unicode )
{
	if( ( ( ( ( u8 * )SrcStr )[ 0 ] | ( ( ( u8 * )SrcStr )[ 1 ] << 8 ) ) & 0xfc00 ) == 0xd800 )
	{
		u32 SrcCode1 ;
		u32 SrcCode2 ;

		SrcCode1 = ( ( u8 * )SrcStr )[ 0 ] | ( ( ( u8 * )SrcStr )[ 1 ] << 8 ) ;
		SrcCode2 = ( ( u8 * )SrcStr )[ 2 ] | ( ( ( u8 * )SrcStr )[ 3 ] << 8 ) ;

		Unicode = ( ( ( SrcCode1 & 0x3ff ) << 10 ) | ( SrcCode2 & 0x3ff ) ) + 0x10000 ;
		SrcStr += 4 ;
	}
	else
	{
		Unicode = ( ( u8 * )SrcStr )[ 0 ] | ( ( ( u8 * )SrcStr )[ 1 ] << 8 ) ;
		if( Unicode == 0 )
		{
			return false ;
		}
		SrcStr += 2 ;
	}

	return true ;
}

// ConvString �� UTF16BE����Unicode���擾����܂ł̏���
__inline bool ConvString_SrcCode_UTF16BE( const u8 *( &SrcStr ), u32 &Unicode )
{
	if( ( ( ( ( ( u8 * )SrcStr )[ 0 ] << 8 ) | ( ( u8 * )SrcStr )[ 1 ] ) & 0xfc00 ) == 0xd800 )
	{
		u32 SrcCode1 ;
		u32 SrcCode2 ;

		SrcCode1 = ( ( ( u8 * )SrcStr )[ 0 ] << 8 ) | ( ( u8 * )SrcStr )[ 1 ] ;
		SrcCode2 = ( ( ( u8 * )SrcStr )[ 2 ] << 8 ) | ( ( u8 * )SrcStr )[ 3 ] ;

		Unicode = ( ( ( SrcCode1 & 0x3ff ) << 10 ) | ( SrcCode2 & 0x3ff ) ) + 0x10000 ;
		SrcStr += 4 ;
	}
	else
	{
		Unicode = ( ( ( u8 * )SrcStr )[ 0 ] << 8 ) | ( ( u8 * )SrcStr )[ 1 ] ;
		if( Unicode == 0 )
		{
			return false ;
		}
		SrcStr += 2 ;
	}

	return true ;
}

// ConvString �� UTF8����Unicode���擾����܂ł̏���
__inline bool ConvString_SrcCode_UTF8( const u8 *( &SrcStr ), u32 &Unicode )
{
	if( ( ( ( u8 * )SrcStr )[ 0 ] & 0x80 ) == 0x00 )
	{
		Unicode = ( ( u8 * )SrcStr )[ 0 ] ;
		if( Unicode == 0 )
		{
			return false ;
		}
		SrcStr += 1 ;
	}
	else
	if( ( ( ( u8 * )SrcStr )[ 0 ] & 0xe0 ) == 0xc0 )
	{
		Unicode = ( ( ( ( u8 * )SrcStr )[ 0 ] & 0x1f ) << 6 ) | ( ( ( u8 * )SrcStr )[ 1 ] & 0x3f ) ;
		SrcStr += 2 ;
	}
	else
	if( ( ( ( u8 * )SrcStr )[ 0 ] & 0xf0 ) == 0xe0 )
	{
		Unicode = ( ( ( ( u8 * )SrcStr )[ 0 ] & 0x0f ) << 12 ) | ( ( ( ( u8 * )SrcStr )[ 1 ] & 0x3f ) << 6 ) | ( ( ( u8 * )SrcStr )[ 2 ] & 0x3f ) ;
		SrcStr += 3 ;
	}
	else
	if( ( ( ( u8 * )SrcStr )[ 0 ] & 0xf8 ) == 0xf0 )
	{
		Unicode = ( ( ( ( u8 * )SrcStr )[ 0 ] & 0x07 ) << 18 ) | ( ( ( ( u8 * )SrcStr )[ 1 ] & 0x3f ) << 12 ) | ( ( ( ( u8 * )SrcStr )[ 2 ] & 0x3f ) << 6 ) | ( ( ( u8 * )SrcStr )[ 3 ] & 0x3f ) ;
		SrcStr += 4 ;
	}
	else
	if( ( ( ( u8 * )SrcStr )[ 0 ] & 0xfc ) == 0xf8 )
	{
		Unicode = ( ( ( ( u8 * )SrcStr )[ 0 ] & 0x03 ) << 24 ) | ( ( ( ( u8 * )SrcStr )[ 1 ] & 0x3f ) << 18 ) | ( ( ( ( u8 * )SrcStr )[ 2 ] & 0x3f ) << 12 ) | ( ( ( ( u8 * )SrcStr )[ 3 ] & 0x3f ) << 6 ) | ( ( ( u8 * )SrcStr )[ 4 ] & 0x3f ) ;
		SrcStr += 5 ;
	}
	else
	if( ( ( ( u8 * )SrcStr )[ 0 ] & 0xfe ) == 0xfc )
	{
		Unicode = ( ( ( ( u8 * )SrcStr )[ 0 ] & 0x01 ) << 30 ) | ( ( ( ( u8 * )SrcStr )[ 1 ] & 0x3f ) << 24 ) | ( ( ( ( u8 * )SrcStr )[ 2 ] & 0x3f ) << 18 ) | ( ( ( ( u8 * )SrcStr )[ 3 ] & 0x3f ) << 12 ) | ( ( ( ( u8 * )SrcStr )[ 4 ] & 0x3f ) << 6 ) | ( ( ( u8 * )SrcStr )[ 5 ] & 0x3f ) ;
		SrcStr += 6 ;
	}
	else
	{
		return false ;
	}

	return true ;
}

// ConvString �� UTF32LE����Unicode���擾����܂ł̏���
__inline bool ConvString_SrcCode_UTF32LE( const u8 *( &SrcStr ), u32 &Unicode )
{
	Unicode = ( ( u8 * )SrcStr )[ 0 ] | ( ( ( u8 * )SrcStr )[ 1 ] << 8 ) | ( ( ( u8 * )SrcStr )[ 2 ] << 16 ) | ( ( ( u8 * )SrcStr )[ 3 ] << 24 ) ;
	return Unicode != 0 ;
}

// ConvString �� UTF32BE����Unicode���擾����܂ł̏���
__inline bool ConvString_SrcCode_UTF32BE( const u8 *( &SrcStr ), u32 &Unicode )
{
	Unicode = ( ( ( u8 * )SrcStr )[ 0 ] << 24 ) | ( ( ( u8 * )SrcStr )[ 1 ] << 16 ) | ( ( ( u8 * )SrcStr )[ 2 ] << 8 ) | ( ( u8 * )SrcStr )[ 3 ] ;
	return Unicode != 0 ;
}

// ConvString �� Unicode���� ShiftJIS�Ƃ��ď������ނ܂ł̏���
__inline void ConvString_DestCode_SHIFTJIS( u8 *&DestStr, u32 &DestCode, int &DestSize )
{
	u32 CharCode ;

	// �e�[�u���l �ŕ\���ł��Ȃ��l�̏ꍇ�̓L�����Z��
	if( DestCode > 0xffff )
	{
		return ;
	}

	CharCode = g_CharCodeSystem.CharCodeCP932Info.UTF16ToMultiByte[ DestCode ] ;

	if( CharCode >= 0x100 )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( CharCode >> 8 ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( CharCode ) ;
			DestStr += 2 ;
		}
		DestSize += 2 ;
	}
	else
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )CharCode ;
			DestStr += 1 ;
		}
		DestSize += 1 ;
	}
}

// ConvString �� Unicode����UTF16LE�Ƃ��ď������ނ܂ł̏���
__inline void ConvString_DestCode_UTF16LE( u8 *&DestStr, u32 &DestCode, int &DestSize )
{
	// UTF-16 �ŕ\���ł��Ȃ��l�̏ꍇ�̓L�����Z��
	if( DestCode > 0x10ffff )
	{
		return ;
	}

	if( DestCode > 0xffff )
	{
		if( DestStr != NULL )
		{
			u32 DestCode1 ;
			u32 DestCode2 ;

			DestCode1 = 0xd800 | ( ( ( DestCode - 0x10000 ) >> 10 ) & 0x3ff ) ;
			DestCode2 = 0xdc00 | (   ( DestCode - 0x10000 )         & 0x3ff ) ;

			( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode1      ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( DestCode1 >> 8 ) ;

			( ( u8 * )DestStr )[ 2 ] = ( u8 )( DestCode2      ) ;
			( ( u8 * )DestStr )[ 3 ] = ( u8 )( DestCode2 >> 8 ) ;

			DestStr += 4 ;
		}

		DestSize += 4 ;
	}
	else
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode      ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( DestCode >> 8 ) ;

			DestStr += 2 ;
		}

		DestSize += 2 ;
	}
}

// ConvString �� Unicode����UTF16BE�Ƃ��ď������ނ܂ł̏���
__inline void ConvString_DestCode_UTF16BE( u8 *&DestStr, u32 &DestCode, int &DestSize )
{
	// UTF-16 �ŕ\���ł��Ȃ��l�̏ꍇ�̓L�����Z��
	if( DestCode > 0x10ffff )
	{
		return ;
	}

	if( DestCode > 0xffff )
	{
		if( DestStr != NULL )
		{
			u32 DestCode1 ;
			u32 DestCode2 ;

			DestCode1 = 0xd800 | ( ( ( DestCode - 0x10000 ) >> 10 ) & 0x3ff ) ;
			DestCode2 = 0xdc00 | (   ( DestCode - 0x10000 )         & 0x3ff ) ;

			( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode1 >> 8 ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( DestCode1      ) ;

			( ( u8 * )DestStr )[ 2 ] = ( u8 )( DestCode2 >> 8 ) ;
			( ( u8 * )DestStr )[ 3 ] = ( u8 )( DestCode2      ) ;

			DestStr += 4 ;
		}

		DestSize += 4 ;
	}
	else
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode >> 8 ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( DestCode      ) ;

			DestStr += 2 ;
		}

		DestSize += 2 ;
	}
}

// ConvString �� Unicode����UTF8�Ƃ��ď������ނ܂ł̏���
__inline void ConvString_DestCode_UTF8( u8 *&DestStr, u32 &DestCode, int &DestSize )
{
	if( DestCode <= 0x7f )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode ) ;
			DestStr ++ ;
		}

		DestSize ++ ;
	}
	else
	if( DestCode <= 0x7ff )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( 0xc0 | ( ( DestCode >> 6 ) & 0x1f ) ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( 0x80 | ( ( DestCode      ) & 0x3f ) ) ;
			DestStr += 2 ;
		}

		DestSize += 2 ;
	}
	else
	if( DestCode <= 0xffff )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( 0xe0 | ( ( DestCode >> 12 ) & 0x0f ) ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( 0x80 | ( ( DestCode >>  6 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 2 ] = ( u8 )( 0x80 | ( ( DestCode       ) & 0x3f ) ) ;
			DestStr += 3 ;
		}

		DestSize += 3 ;
	}
	else
	if( DestCode <= 0x1fffff )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( 0xf0 | ( ( DestCode >> 18 ) & 0x07 ) ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( 0x80 | ( ( DestCode >> 12 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 2 ] = ( u8 )( 0x80 | ( ( DestCode >>  6 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 3 ] = ( u8 )( 0x80 | ( ( DestCode       ) & 0x3f ) ) ;
			DestStr += 4 ;
		}

		DestSize += 4 ;
	}
	else
	if( DestCode <= 0x3ffffff )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( 0xf8 | ( ( DestCode >> 24 ) & 0x03 ) ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( 0x80 | ( ( DestCode >> 18 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 2 ] = ( u8 )( 0x80 | ( ( DestCode >> 12 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 3 ] = ( u8 )( 0x80 | ( ( DestCode >>  6 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 4 ] = ( u8 )( 0x80 | ( ( DestCode       ) & 0x3f ) ) ;
			DestStr += 5 ;
		}

		DestSize += 5 ;
	}
	else
	if( DestCode <= 0x7fffffff )
	{
		if( DestStr != NULL )
		{
			( ( u8 * )DestStr )[ 0 ] = ( u8 )( 0xfc | ( ( DestCode >> 30 ) & 0x01 ) ) ;
			( ( u8 * )DestStr )[ 1 ] = ( u8 )( 0x80 | ( ( DestCode >> 24 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 2 ] = ( u8 )( 0x80 | ( ( DestCode >> 18 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 3 ] = ( u8 )( 0x80 | ( ( DestCode >> 12 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 4 ] = ( u8 )( 0x80 | ( ( DestCode >>  6 ) & 0x3f ) ) ;
			( ( u8 * )DestStr )[ 5 ] = ( u8 )( 0x80 | ( ( DestCode       ) & 0x3f ) ) ;
			DestStr += 6 ;
		}

		DestSize += 6 ;
	}
	else
	{
		DestSize += 0 ;
	}
}

// ConvString �� Unicode����UTF32LE�Ƃ��ď������ނ܂ł̏���
__inline void ConvString_DestCode_UTF32LE( u8 *&DestStr, u32 &DestCode, int &DestSize )
{
	if( DestStr != NULL )
	{
		( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode       ) ;
		( ( u8 * )DestStr )[ 1 ] = ( u8 )( DestCode >>  8 ) ;
		( ( u8 * )DestStr )[ 2 ] = ( u8 )( DestCode >> 16 ) ;
		( ( u8 * )DestStr )[ 3 ] = ( u8 )( DestCode >> 24 ) ;
		DestStr += 4 ;
	}

	DestSize += 4 ;
}

// ConvString �� Unicode����UTF32BE�Ƃ��ď������ނ܂ł̏���
__inline void ConvString_DestCode_UTF32BE( u8 *&DestStr, u32 &DestCode, int &DestSize )
{
	if( DestStr != NULL )
	{
		( ( u8 * )DestStr )[ 0 ] = ( u8 )( DestCode >> 24 ) ;
		( ( u8 * )DestStr )[ 1 ] = ( u8 )( DestCode >> 16 ) ;
		( ( u8 * )DestStr )[ 2 ] = ( u8 )( DestCode >>  8 ) ;
		( ( u8 * )DestStr )[ 3 ] = ( u8 )( DestCode       ) ;
		DestStr += 4 ;
	}

	DestSize += 4 ;
}

// ConvString �� 1�o�C�g�P�ʃR�[�h�̏I�[�������������ޏ���
__inline void ConvString_1BYTE_NULL_CHAR( u8 *&DestStr, int &DestSize )
{
	if( DestStr != NULL )
	{
		( ( u8 * )DestStr )[ 0 ] = 0 ;
		DestStr += 1 ;
	}
	DestSize += 1 ;
}

// ConvString �� 2�o�C�g�P�ʃR�[�h�̏I�[�������������ޏ���
__inline void ConvString_2BYTE_NULL_CHAR( u8 *&DestStr, int &DestSize )
{
	if( DestStr != NULL )
	{
		( ( u16 * )DestStr )[ 0 ] = 0 ;
		DestStr += 2 ;
	}
	DestSize += 2 ;
}

// ConvString �� 4�o�C�g�P�ʃR�[�h�̏I�[�������������ޏ���
__inline void ConvString_4BYTE_NULL_CHAR( u8 *&DestStr, int &DestSize )
{
	if( DestStr != NULL )
	{
		( ( u32 * )DestStr )[ 0 ] = 0 ;
		DestStr += 4 ;
	}
	DestSize += 4 ;
}


// ConvString �� UTF16LE �� UTF16BE ���������p
__inline int ConvString_UTF16LE_TO_UTF16BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16BE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16LE �� UTF8 ���������p
__inline int ConvString_UTF16LE_TO_UTF8( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF8( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16LE �� UTF32LE ���������p
__inline int ConvString_UTF16LE_TO_UTF32LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32LE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16LE �� UTF32BE ���������p
__inline int ConvString_UTF16LE_TO_UTF32BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32BE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}


// ConvString �� UTF16BE �� UTF16LE ���������p
__inline int ConvString_UTF16BE_TO_UTF16LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16LE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16BE �� UTF8 ���������p
__inline int ConvString_UTF16BE_TO_UTF8( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF8( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16BE �� UTF32LE ���������p
__inline int ConvString_UTF16BE_TO_UTF32LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32LE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16BE �� UTF32BE ���������p
__inline int ConvString_UTF16BE_TO_UTF32BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32BE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}


// ConvString �� UTF8 �� UTF16LE ���������p
__inline int ConvString_UTF8_TO_UTF16LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF8( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16LE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF8 �� UTF16BE ���������p
__inline int ConvString_UTF8_TO_UTF16BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF8( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16BE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF8 �� UTF32LE ���������p
__inline int ConvString_UTF8_TO_UTF32LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF8( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32LE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF8 �� UTF32BE ���������p
__inline int ConvString_UTF8_TO_UTF32BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF8( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32BE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}


// ConvString �� UTF32LE �� UTF16LE ���������p
__inline int ConvString_UTF32LE_TO_UTF16LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16LE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32LE �� UTF16BE ���������p
__inline int ConvString_UTF32LE_TO_UTF16BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16BE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32LE �� UTF8 ���������p
__inline int ConvString_UTF32LE_TO_UTF8( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF8( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32LE �� UTF32BE ���������p
__inline int ConvString_UTF32LE_TO_UTF32BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32LE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32BE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}


// ConvString �� UTF32BE �� UTF16LE ���������p
__inline int ConvString_UTF32BE_TO_UTF16LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16LE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32BE �� UTF16BE ���������p
__inline int ConvString_UTF32BE_TO_UTF16BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16BE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32BE �� UTF8 ���������p
__inline int ConvString_UTF32BE_TO_UTF8( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF8( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32BE �� UTF32LE ���������p
__inline int ConvString_UTF32BE_TO_UTF32LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32BE( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32LE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}


// ConvString �� ShiftJIS �� UTF16LE ���������p
__inline int ConvString_SHIFTJIS_TO_UTF16LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_SHIFTJIS( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16LE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� ShiftJIS �� UTF16BE ���������p
__inline int ConvString_SHIFTJIS_TO_UTF16BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_SHIFTJIS( SrcStr, Unicode ) )
		ConvString_DestCode_UTF16BE( DestStr, Unicode, DestSize ) ;
	ConvString_2BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� ShiftJIS �� UTF8 ���������p
__inline int ConvString_SHIFTJIS_TO_UTF8( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_SHIFTJIS( SrcStr, Unicode ) )
		ConvString_DestCode_UTF8( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� ShiftJIS �� UTF32LE ���������p
__inline int ConvString_SHIFTJIS_TO_UTF32LE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_SHIFTJIS( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32LE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� ShiftJIS �� UTF32BE ���������p
__inline int ConvString_SHIFTJIS_TO_UTF32BE( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_SHIFTJIS( SrcStr, Unicode ) )
		ConvString_DestCode_UTF32BE( DestStr, Unicode, DestSize ) ;
	ConvString_4BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}


// ConvString �� UTF16LE �� ShiftJIS ���������p
__inline int ConvString_UTF16LE_TO_SHIFTJIS( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16LE( SrcStr, Unicode ) )
		ConvString_DestCode_SHIFTJIS( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF16BE �� ShiftJIS ���������p
__inline int ConvString_UTF16BE_TO_SHIFTJIS( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF16BE( SrcStr, Unicode ) )
		ConvString_DestCode_SHIFTJIS( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF8 �� ShiftJIS ���������p
__inline int ConvString_UTF8_TO_SHIFTJIS( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF8( SrcStr, Unicode ) )
		ConvString_DestCode_SHIFTJIS( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32LE �� ShiftJIS ���������p
__inline int ConvString_UTF32LE_TO_SHIFTJIS( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32LE( SrcStr, Unicode ) )
		ConvString_DestCode_SHIFTJIS( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ConvString �� UTF32BE �� ShiftJIS ���������p
__inline int ConvString_UTF32BE_TO_SHIFTJIS( const char *Src, char *Dest )
{
	CONVSTRING_BEGIN
	while( ConvString_SrcCode_UTF32BE( SrcStr, Unicode ) )
		ConvString_DestCode_SHIFTJIS( DestStr, Unicode, DestSize ) ;
	ConvString_1BYTE_NULL_CHAR( DestStr, DestSize ) ;
	return DestSize ;
}

// ��������w��̃R�[�h�y�[�W�̕�����ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern int ConvString( const char *Src, int SrcCharCodeFormat, char *Dest, int DestCharCodeFormat )
{
	// �L�����N�^�[�R�[�h�e�[�u��������������Ă��Ȃ������珉����
	if( g_CharCodeSystem.InitializeFlag == FALSE )
	{
		InitCharCode() ;
	}

	CHARCODETABLE_INITCHECK( SrcCharCodeFormat )
	CHARCODETABLE_INITCHECK( DestCharCodeFormat )

	// ���������p�̊֐�������ꍇ�͂�������g�p����
	switch( SrcCharCodeFormat )
	{
	case CHARCODEFORMAT_SHIFTJIS :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_UTF16LE : return ConvString_SHIFTJIS_TO_UTF16LE( Src, Dest ) ;
		case CHARCODEFORMAT_UTF16BE : return ConvString_SHIFTJIS_TO_UTF16BE( Src, Dest ) ;
		case CHARCODEFORMAT_UTF8    : return ConvString_SHIFTJIS_TO_UTF8(    Src, Dest ) ;
		case CHARCODEFORMAT_UTF32LE : return ConvString_SHIFTJIS_TO_UTF32LE( Src, Dest ) ;
		case CHARCODEFORMAT_UTF32BE : return ConvString_SHIFTJIS_TO_UTF32BE( Src, Dest ) ;
		}
		break ;

	case CHARCODEFORMAT_UTF16LE :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_SHIFTJIS : return ConvString_UTF16LE_TO_SHIFTJIS( Src, Dest ) ;
		case CHARCODEFORMAT_UTF16BE  : return ConvString_UTF16LE_TO_UTF16BE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF8     : return ConvString_UTF16LE_TO_UTF8(     Src, Dest ) ;
		case CHARCODEFORMAT_UTF32LE  : return ConvString_UTF16LE_TO_UTF32LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF32BE  : return ConvString_UTF16LE_TO_UTF32BE(  Src, Dest ) ;
		}
		break ;

	case CHARCODEFORMAT_UTF16BE :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_SHIFTJIS : return ConvString_UTF16BE_TO_SHIFTJIS( Src, Dest ) ;
		case CHARCODEFORMAT_UTF16LE  : return ConvString_UTF16BE_TO_UTF16LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF8     : return ConvString_UTF16BE_TO_UTF8(     Src, Dest ) ;
		case CHARCODEFORMAT_UTF32LE  : return ConvString_UTF16BE_TO_UTF32LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF32BE  : return ConvString_UTF16BE_TO_UTF32BE(  Src, Dest ) ;
		}
		break ;

	case CHARCODEFORMAT_UTF8 :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_SHIFTJIS : return ConvString_UTF8_TO_SHIFTJIS( Src, Dest ) ;
		case CHARCODEFORMAT_UTF16LE  : return ConvString_UTF8_TO_UTF16LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF16BE  : return ConvString_UTF8_TO_UTF16BE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF32LE  : return ConvString_UTF8_TO_UTF32LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF32BE  : return ConvString_UTF8_TO_UTF32BE(  Src, Dest ) ;
		}
		break ;

	case CHARCODEFORMAT_UTF32LE :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_SHIFTJIS : return ConvString_UTF32LE_TO_SHIFTJIS( Src, Dest ) ;
		case CHARCODEFORMAT_UTF16LE  : return ConvString_UTF32LE_TO_UTF16LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF16BE  : return ConvString_UTF32LE_TO_UTF16BE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF8     : return ConvString_UTF32LE_TO_UTF8(     Src, Dest ) ;
		case CHARCODEFORMAT_UTF32BE  : return ConvString_UTF32LE_TO_UTF32BE(  Src, Dest ) ;
		}
		break ;

	case CHARCODEFORMAT_UTF32BE :
		switch( DestCharCodeFormat )
		{
		case CHARCODEFORMAT_SHIFTJIS : return ConvString_UTF32BE_TO_SHIFTJIS( Src, Dest ) ;
		case CHARCODEFORMAT_UTF16LE  : return ConvString_UTF32BE_TO_UTF16LE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF16BE  : return ConvString_UTF32BE_TO_UTF16BE(  Src, Dest ) ;
		case CHARCODEFORMAT_UTF8     : return ConvString_UTF32BE_TO_UTF8(     Src, Dest ) ;
		case CHARCODEFORMAT_UTF32LE  : return ConvString_UTF32BE_TO_UTF32LE(  Src, Dest ) ;
		}
		break ;
	}

	// ���������ꍇ�͋��ʂ̏������s��
	{
		int DestSize ;
		u32 SrcCode ;
		u32 DestCode ;
		u8 *DestStr ;
		const u8 *SrcStr ;
		int UseSrcSize ;
		int WriteSize ;

		SrcStr  = ( u8 * )Src ;
		DestStr = ( u8 * )Dest ;
		DestSize = 0 ;
		for(;;)
		{
			SrcCode = GetCharCode_inline( ( const char * )SrcStr, SrcCharCodeFormat, &UseSrcSize ) ;
			if( SrcCode == 0 )
			{
				break ;
			}
			SrcStr += UseSrcSize ;

			DestCode = ConvCharCode_inline( SrcCode, SrcCharCodeFormat, DestCharCodeFormat ) ;

			WriteSize = PutCharCode_inline( DestCode, DestCharCodeFormat, ( char * )DestStr ) ;
			if( DestStr != NULL )
			{
				DestStr += WriteSize ;
			}
			DestSize += WriteSize ;
		}

		switch( GetCharCodeFormatUnitSize_inline( DestCharCodeFormat ) )
		{
		case 1 :
			if( DestStr != NULL )
			{
				( ( u8 * )DestStr )[ 0 ] = 0 ;
				DestStr += 1;
			}
			DestSize += 1 ;
			break ;

		case 2 :
			if( DestStr != NULL )
			{
				( ( u16 * )DestStr )[ 0 ] = 0 ;
				DestStr += 2;
			}
			DestSize += 2 ;
			break ;

		case 4 :
			if( DestStr != NULL )
			{
				( ( u32 * )DestStr )[ 0 ] = 0 ;
				DestStr += 4 ;
			}
			DestSize += 4 ;
			break ;
		}

		return DestSize ;
	}
}

// ������Ɋ܂܂�镶�������擾����
extern int GetStringCharNum( const char *String, int CharCodeFormat )
{
	u32 CharCode ;
	int CharBytes ;
	int Count ;
	int Address ;

	Address = 0 ;
	Count = 0 ;
	for(;;)
	{
		CharCode = GetCharCode_inline( ( const char * )&( ( u8 * )String )[ Address ], CharCodeFormat, &CharBytes ) ;
		if( CharCode == 0 )
		{
			break ;
		}

		Address += CharBytes ;
		Count ++ ;
	}

	return Count ;
}

// �w��ԍ��̕����̃A�h���X���擾����
extern const char *GetStringCharAddress( const char *String, int CharCodeFormat, int Index )
{
	u32 CharCode ;
	int CharBytes ;
	int Count ;
	int Address ;

	Address = 0 ;
	Count = 0 ;
	for(;;)
	{
		if( Count == Index )
		{
			return ( const char * )&( ( u8 * )String )[ Address ] ;
		}

		CharCode = GetCharCode_inline( ( const char * )&( ( u8 * )String )[ Address ], CharCodeFormat, &CharBytes ) ;
		if( CharCode == 0 )
		{
			break ;
		}

		Address += CharBytes ;
		Count ++ ;
	}

	return NULL ;
}

// �w��ԍ��̕����̃R�[�h���擾����
extern u32 GetStringCharCode( const char *String, int CharCodeFormat, int Index )
{
	u32 CharCode ;
	int CharBytes ;
	int Count ;
	int Address ;

	Address = 0 ;
	Count = 0 ;
	for(;;)
	{
		CharCode = GetCharCode_inline( ( const char * )&( ( u8 * )String )[ Address ], CharCodeFormat, &CharBytes ) ;
		if( CharCode == 0 || Count == Index )
		{
			break ;
		}

		Address += CharBytes ;
		Count ++ ;
	}

	return CharCode ;
}






















extern void CL_strcpy( int CharCodeFormat, char *Dest, const char *Src )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u8 * )Dest )[ i ] = ( ( u8 * )Src )[ i ] ;
		}
		( ( u8 * )Dest )[ i ] = 0 ;
		break ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u16 * )Dest )[ i ] = ( ( u16 * )Src )[ i ] ;
		}
		( ( u16 * )Dest )[ i ] = 0 ;
		break ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u32 * )Dest )[ i ] = ( ( u32 * )Src )[ i ] ;
		}
		( ( u32 * )Dest )[ i ] = 0 ;
		break ;
	}
}

extern void CL_strcpy_s( int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src )
{
	size_t i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		if( BufferBytes < 1 )
		{
			return ;
		}
		for( i = 0 ; i * 1 < BufferBytes - 1 && ( ( u8 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u8 * )Dest )[ i ] = ( ( u8 * )Src )[ i ] ;
		}
		( ( u8 * )Dest )[ i ] = 0 ;
		break ;

	case 2 :
		if( BufferBytes < 2 )
		{
			return ;
		}
		for( i = 0 ; i * 2 < BufferBytes - 2 && ( ( u16 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u16 * )Dest )[ i ] = ( ( u16 * )Src )[ i ] ;
		}
		( ( u16 * )Dest )[ i ] = 0 ;
		break ;

	case 4 :
		if( BufferBytes < 4 )
		{
			return ;
		}
		for( i = 0 ; i * 4 < BufferBytes - 4 && ( ( u32 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u32 * )Dest )[ i ] = ( ( u32 * )Src )[ i ] ;
		}
		( ( u32 * )Dest )[ i ] = 0 ;
		break ;
	}
}

extern void CL_strncpy( int CharCodeFormat, char *Dest, const char *Src, int Num )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; i < Num && ( ( u8 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u8 * )Dest )[ i ] = ( ( u8 * )Src )[ i ] ;
		}
		for( ; i < Num ; i ++ )
		{
			( ( u8 * )Dest )[ i ] = 0 ;
		}
		break ;

	case 2 :
		for( i = 0 ; i < Num && ( ( u16 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u16 * )Dest )[ i ] = ( ( u16 * )Src )[ i ] ;
		}
		for( ; i < Num ; i ++ )
		{
			( ( u16 * )Dest )[ i ] = 0 ;
		}
		break ;

	case 4 :
		for( i = 0 ; i < Num && ( ( u32 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u32 * )Dest )[ i ] = ( ( u32 * )Src )[ i ] ;
		}
		for( ; i < Num ; i ++ )
		{
			( ( u32 * )Dest )[ i ] = 0 ;
		}
		break ;
	}
}

extern void CL_strncpy_s( int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Num )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		if( BufferBytes < 1 )
		{
			return ;
		}
		if( Num > ( int )BufferBytes )
		{
			Num = ( int )BufferBytes ;
		}
		for( i = 0 ; i < Num && ( ( u8 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u8 * )Dest )[ i ] = ( ( u8 * )Src )[ i ] ;
		}
		for( ; i < Num ; i ++ )
		{
			( ( u8 * )Dest )[ i ] = 0 ;
		}
		break ;

	case 2 :
		if( BufferBytes < 2 )
		{
			return ;
		}
		if( Num > ( int )( BufferBytes / 2 ) )
		{
			Num = ( int )( BufferBytes / 2 ) ;
		}
		for( i = 0 ; i < Num && ( ( u16 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u16 * )Dest )[ i ] = ( ( u16 * )Src )[ i ] ;
		}
		for( ; i < Num ; i ++ )
		{
			( ( u16 * )Dest )[ i ] = 0 ;
		}
		break ;

	case 4 :
		if( BufferBytes < 4 )
		{
			return ;
		}
		if( Num > ( int )( BufferBytes / 4 ) )
		{
			Num = ( int )( BufferBytes / 4 ) ;
		}
		for( i = 0 ; i < Num && ( ( u32 * )Src )[ i ] != 0 ; i ++ )
		{
			( ( u32 * )Dest )[ i ] = ( ( u32 * )Src )[ i ] ;
		}
		for( ; i < Num ; i ++ )
		{
			( ( u32 * )Dest )[ i ] = 0 ;
		}
		break ;
	}
}

extern void CL_strcat( int CharCodeFormat, char *Dest, const char *Src )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Dest )[ i ] != 0 ; i ++ ){}
		CL_strcpy( CharCodeFormat, ( char * )&( ( u8 * )Dest )[ i ], Src ) ;
		break ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Dest )[ i ] != 0 ; i ++ ){}
		CL_strcpy( CharCodeFormat, ( char * )&( ( u16 * )Dest )[ i ], Src ) ;
		break ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Dest )[ i ] != 0 ; i ++ ){}
		CL_strcpy( CharCodeFormat, ( char * )&( ( u32 * )Dest )[ i ], Src ) ;
		break ;
	}
}

extern void CL_strcat_s( int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src )
{
	size_t i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Dest )[ i ] != 0 ; i ++ ){}
		if( i >= BufferBytes - 1 )
		{
			return ;
		}
		CL_strcpy_s( CharCodeFormat, ( char * )&( ( u8 * )Dest )[ i ], BufferBytes - i, Src ) ;
		break ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Dest )[ i ] != 0 ; i ++ ){}
		if( i * 2 >= BufferBytes - 2 )
		{
			return ;
		}
		CL_strcpy_s( CharCodeFormat, ( char * )&( ( u16 * )Dest )[ i ], BufferBytes - i * 2, Src ) ;
		break ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Dest )[ i ] != 0 ; i ++ ){}
		if( i * 4 >= BufferBytes - 4 )
		{
			return ;
		}
		CL_strcpy_s( CharCodeFormat, ( char * )&( ( u32 * )Dest )[ i ], BufferBytes - i * 4, Src ) ;
		break ;
	}
}

extern const char *CL_strstr( int CharCodeFormat, const char *Str1, const char *Str2 )
{
	int i, j ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Str1 )[ i ] != 0 ; i ++ )
		{
			for( j = 0 ; ( ( u8 * )Str2 )[ j     ] != 0 &&
				         ( ( u8 * )Str1 )[ i + j ] != 0 &&
						 ( ( u8 * )Str1 )[ i + j ] == ( ( u8 * )Str2 )[ j ] ; j ++ ){}

			if( ( ( u8 * )Str2 )[ j ] == 0 )
			{
				return ( const char * )&( ( u8 * )Str1 )[ i ] ;
			}

			if( ( ( u8 * )Str1 )[ i + j ] == 0 )
			{
				return NULL ;
			}
		}
		break ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str1 )[ i ] != 0 ; i ++ )
		{
			for( j = 0 ; ( ( u16 * )Str2 )[ j     ] != 0 &&
				         ( ( u16 * )Str1 )[ i + j ] != 0 &&
						 ( ( u16 * )Str1 )[ i + j ] == ( ( u16 * )Str2 )[ j ] ; j ++ ){}

			if( ( ( u16 * )Str2 )[ j ] == 0 )
			{
				return ( const char * )&( ( u16 * )Str1 )[ i ] ;
			}

			if( ( ( u16 * )Str1 )[ i + j ] == 0 )
			{
				return NULL ;
			}
		}
		break ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str1 )[ i ] != 0 ; i ++ )
		{
			for( j = 0 ; ( ( u32 * )Str2 )[ j     ] != 0 &&
				         ( ( u32 * )Str1 )[ i + j ] != 0 &&
						 ( ( u32 * )Str1 )[ i + j ] == ( ( u32 * )Str2 )[ j ] ; j ++ ){}

			if( ( ( u32 * )Str2 )[ j ] == 0 )
			{
				return ( const char * )&( ( u32 * )Str1 )[ i ] ;
			}

			if( ( ( u32 * )Str1 )[ i + j ] == 0 )
			{
				return NULL ;
			}
		}
		break ;
	}

	return NULL ;
}

extern int CL_strlen( int CharCodeFormat, const char *Str )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8  * )Str )[ i ] != 0 ; i ++ ){}
		return i ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str )[ i ] != 0 ; i ++ ){}
		return i ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str )[ i ] != 0 ; i ++ ){}
		return i ;
	}

	return 0 ;
}

extern int CL_strcmp( int CharCodeFormat, const char *Str1, const char *Str2 )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Str1 )[ i ] != 0 &&
			         ( ( u8 * )Str2 )[ i ] != 0 &&
					 ( ( u8 * )Str1 )[ i ] == ( ( u8 * )Str2 )[ i ] ; i ++ ){}
		if( ( ( u8 * )Str1 )[ i ] == 0 &&
			( ( u8 * )Str2 )[ i ] == 0 )
		{
			return 0 ;
		}
		return ( ( u8 * )Str1 )[ i ] < ( ( u8 * )Str2 )[ i ] ? -1 : 1 ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str1 )[ i ] != 0 &&
			         ( ( u16 * )Str2 )[ i ] != 0 &&
					 ( ( u16 * )Str1 )[ i ] == ( ( u16 * )Str2 )[ i ] ; i ++ ){}
		if( ( ( u16 * )Str1 )[ i ] == 0 &&
			( ( u16 * )Str2 )[ i ] == 0 )
		{
			return 0 ;
		}
		return ( ( u16 * )Str1 )[ i ] < ( ( u16 * )Str2 )[ i ] ? -1 : 1 ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str1 )[ i ] != 0 &&
			         ( ( u32 * )Str2 )[ i ] != 0 &&
					 ( ( u32 * )Str1 )[ i ] == ( ( u32 * )Str2 )[ i ] ; i ++ ){}
		if( ( ( u32 * )Str1 )[ i ] == 0 &&
			( ( u32 * )Str2 )[ i ] == 0 )
		{
			return 0 ;
		}
		return ( ( u32 * )Str1 )[ i ] < ( ( u32 * )Str2 )[ i ] ? -1 : 1 ;
	}

	return 0 ;
}

extern int CL_strcmp_str2_ascii( int CharCodeFormat, const char *Str1, const char *Str2 )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Str1 )[ i ] != 0 &&
			         ( ( u8 * )Str2 )[ i ] != 0 &&
					 ( ( u8 * )Str1 )[ i ] == ( ( u8 * )Str2 )[ i ] ; i ++ ){}
		if( ( ( u8 * )Str1 )[ i ] == 0 &&
			( ( u8 * )Str2 )[ i ] == 0 )
		{
			return 0 ;
		}
		return ( ( u8 * )Str1 )[ i ] < ( ( u8 * )Str2 )[ i ] ? -1 : 1 ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str1 )[ i ] != 0 &&
			         ( ( u8 * )Str2 )[ i ] != 0 &&
					 ( ( u16 * )Str1 )[ i ] == ( ( u8 * )Str2 )[ i ] ; i ++ ){}
		if( ( ( u16 * )Str1 )[ i ] == 0 &&
			( ( u8 * )Str2 )[ i ] == 0 )
		{
			return 0 ;
		}
		return ( ( u16 * )Str1 )[ i ] < ( ( u8 * )Str2 )[ i ] ? -1 : 1 ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str1 )[ i ] != 0 &&
			         ( ( u8  * )Str2 )[ i ] != 0 &&
					 ( ( u32 * )Str1 )[ i ] == ( ( u8 * )Str2 )[ i ] ; i ++ ){}
		if( ( ( u32 * )Str1 )[ i ] == 0 &&
			( ( u8  * )Str2 )[ i ] == 0 )
		{
			return 0 ;
		}
		return ( ( u32 * )Str1 )[ i ] < ( ( u8 * )Str2 )[ i ] ? -1 : 1 ;
	}

	return 0 ;
}

extern int CL_stricmp( int CharCodeFormat, const char *Str1, const char *Str2 )
{
	int i ;
	u32 Str1Code ;
	u32 Str2Code ;
	int Str1CodeBytes ;
	int Str2CodeBytes ;

	for( i = 0 ;; i += Str1CodeBytes )
	{
		Str1Code = GetCharCode_inline( ( const char * )&( ( u8 * )Str1 )[ i ], CharCodeFormat, &Str1CodeBytes ) ;
		Str2Code = GetCharCode_inline( ( const char * )&( ( u8 * )Str2 )[ i ], CharCodeFormat, &Str2CodeBytes ) ;

		if( Str1Code >= 0x61 && Str1Code <= 0x7a )
		{
			Str1Code = Str1Code - 0x61 + 0x41 ;
		}

		if( Str2Code >= 0x61 && Str2Code <= 0x7a )
		{
			Str2Code = Str2Code - 0x61 + 0x41 ;
		}

		if( Str1Code      != Str2Code      ||
			Str1CodeBytes != Str2CodeBytes ||
			Str1Code == 0                  ||
			Str2Code == 0                  )
		{
			break ;
		}
	}

	if( Str1CodeBytes != Str2CodeBytes )
	{
		return Str1CodeBytes < Str2CodeBytes ? -1 : 1 ;
	}

	return Str1Code == Str2Code ? 0 : ( Str1Code < Str2Code ? -1 : 1 ) ;
}

extern int CL_strncmp( int CharCodeFormat, const char *Str1, const char *Str2, int Size )
{
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; i < Size && ( ( u8 * )Str1 )[ i ] == ( ( u8 * )Str2 )[ i ] ; i ++ ){}
		return i == Size ? 0 : ( ( ( u8 * )Str1 )[ i ] < ( ( u8 * )Str2 )[ i ] ? -1 : 1 ) ;

	case 2 :
		for( i = 0 ; i < Size && ( ( u16 * )Str1 )[ i ] == ( ( u16 * )Str2 )[ i ] ; i ++ ){}
		return i == Size ? 0 : ( ( ( u16 * )Str1 )[ i ] < ( ( u16 * )Str2 )[ i ] ? -1 : 1 ) ;

	case 3 :
		for( i = 0 ; i < Size && ( ( u32 * )Str1 )[ i ] == ( ( u32 * )Str2 )[ i ] ; i ++ ){}
		return i == Size ? 0 : ( ( ( u32 * )Str1 )[ i ] < ( ( u32 * )Str2 )[ i ] ? -1 : 1 ) ;
	}

	return 0 ;
}

extern const char *CL_strchr( int CharCodeFormat, const char *Str, u32 CharCode )
{
	int i ;
	u32 StrCharCode ;
	int CodeBytes ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u8 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode == CharCode )
			{
				return ( const char * )&( ( u8 * )Str )[ i ] ;
			}

			if( CodeBytes > 1 )
			{
				i ++ ;
			}
		}
		return NULL ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u16 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode == CharCode )
			{
				return ( const char * )&( ( u16 * )Str )[ i ] ;
			}

			if( CodeBytes > 2 )
			{
				i ++ ;
			}
		}
		return NULL ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u32 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode == CharCode )
			{
				return ( const char * )&( ( u32 * )Str )[ i ] ;
			}
		}
		return NULL ;
	}

	return NULL ;
}

extern const char *CL_strrchr( int CharCodeFormat, const char *Str, u32 CharCode )
{
	const char *lastp ;
	u32 StrCharCode ;
	int CodeBytes ;
	int i ;

	lastp = NULL;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u8 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode == CharCode )
			{
				lastp = ( const char * )&( ( u8 * )Str )[ i ] ;
			}

			if( CodeBytes > 1 )
			{
				i ++ ;
			}
		}
		break ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u16 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode == CharCode )
			{
				lastp = ( const char * )&( ( u16 * )Str )[ i ] ;
			}

			if( CodeBytes > 2 )
			{
				i ++ ;
			}
		}
		break ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u32 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode == CharCode )
			{
				lastp = ( const char * )&( ( u32 * )Str )[ i ] ;
			}
		}
		break ;
	}

	return lastp ;
}

extern char * CL_strupr( int CharCodeFormat, char *Str )
{
	u32 StrCharCode ;
	int CodeBytes ;
	int i ;

	switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
	{
	case 1 :
		for( i = 0 ; ( ( u8 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u8 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode >= 0x61 && StrCharCode <= 0x7a )
			{
				StrCharCode = StrCharCode - 0x61 + 0x41 ;
				PutCharCode_inline( StrCharCode, CharCodeFormat, ( char * )&( ( u8 * )Str )[ i ] ) ;
			}

			if( CodeBytes > 1 )
			{
				i ++ ;
			}
		}
		break ;

	case 2 :
		for( i = 0 ; ( ( u16 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u16 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode >= 0x61 && StrCharCode <= 0x7a )
			{
				StrCharCode = StrCharCode - 0x61 + 0x41 ;
				PutCharCode_inline( StrCharCode, CharCodeFormat, ( char * )&( ( u16 * )Str )[ i ] ) ;
			}

			if( CodeBytes > 2 )
			{
				i ++ ;
			}
		}
		break ;

	case 4 :
		for( i = 0 ; ( ( u32 * )Str )[ i ] != 0 ; i ++ )
		{
			StrCharCode = GetCharCode_inline( ( const char * )&( ( u32 * )Str )[ i ], CharCodeFormat, &CodeBytes ) ;
			if( StrCharCode >= 0x61 && StrCharCode <= 0x7a )
			{
				StrCharCode = StrCharCode - 0x61 + 0x41 ;
				PutCharCode_inline( StrCharCode, CharCodeFormat, ( char * )&( ( u32 * )Str )[ i ] ) ;
			}
		}
		break ;
	}

	return Str ;
}

static u32 CL_vsprintf_help_getnumber( const u32 *CharCode, int *UseCharNum )
{
	u32 Result ;
	u32 AddNum ;
	int i ;
	u32 NumberNum ;
	u32 NumberStr[ 256 ] ;

	for( NumberNum = 0 ; CharCode[ NumberNum ] >= '0' && CharCode[ NumberNum ] <= '9' ; NumberNum ++ )
	{
		NumberStr[ NumberNum ] = CharCode[ NumberNum ] ;
	}

	Result = 0 ;
	AddNum = 1 ;
	for( i = ( int )( NumberNum - 1 ) ; i >= 0 ; i -- )
	{
		Result += AddNum * ( NumberStr[ i ] - '0' ) ;
		AddNum *= 10 ;
	}

	if( UseCharNum != NULL )
	{
		*UseCharNum = ( int )NumberNum ;
	}

	return Result ;
}

// �w��̕������w�萔�������ށA�߂�l�͏������񂾃o�C�g��
static int CL_vsprintf_help_set_char( u64 Num, u32 CharCode, char *Dest, int DestCharCodeFormat )
{
	u32 i ;
	int DestSize ;

	DestSize = 0 ;
	for( i = 0 ; i < Num ; i ++ )
	{
		DestSize += PutCharCode_inline( CharCode, DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
	}

	return DestSize ;
}

// f �^�C�v�p�� NaN �����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_NaN_f(
	int FPClass,
	int Flag_Sharp,
	int ZeroAdd,
	int Precision,
	u8 *NumberStrBuffer
)
{
	int DestSize ;

	// ���x���}�C�i�X�̏ꍇ�͊���l���Z�b�g
	if( Precision < 0 )
	{
		Precision = 6 ;
	}

	DestSize = 0 ;
	// ���x�� 0 �̏ꍇ�� Flag_Sharp ������� . ��t����
	if( Precision == 0 )
	{
		NumberStrBuffer[ DestSize ] = '1' ;
		DestSize ++ ;

		if( Flag_Sharp )
		{
			NumberStrBuffer[ DestSize ] = '.' ;
			DestSize ++ ;
		}

		Precision = 0 ;
	}
	else
	{
		int SetNum ;
		int i ;
		u8 ( *Table )[ 7 ] = NULL ;
		int MaxLength = 0 ;

		// �g�p����e�[�u���ƍő�g�p���x�����Z�b�g
		switch( FPClass )
		{
		case _FPCLASS_SNAN :
		case _FPCLASS_QNAN :
			Table     = FloatErrorStr_QNAN ;
			MaxLength = 5 ;
			break ;

		case _FPCLASS_NINF :
		case _FPCLASS_PINF :
			Table     = FloatErrorStr_INF ;
			MaxLength = 4 ;
			break ;
		}

		// ���x��������̍ő咷�ȏ�̏ꍇ�͕������S�ē]��
		if( Precision >= MaxLength )
		{
			SetNum = MaxLength + 2 ;
		}
		else
		// ����ȊO�̏ꍇ�͐��x + 2 �̕������]��
		{
			SetNum = Precision + 2 ;
		}

		// ��������
		for( i = 0 ; i < SetNum ; i ++ )
		{
			NumberStrBuffer[ DestSize ] = Table[ SetNum - 1 ][ i ] ;
			DestSize ++ ;
		}

		// �g�p�������̐��x������
		Precision -= SetNum - 2 ;

		// ���x�Ɏc�肪�������� 0 �Ŗ��߂�
		if( ZeroAdd )
		{
			for( i = 0 ; i < Precision ; i ++ )
			{
				NumberStrBuffer[ DestSize ] = '0' ;
				DestSize ++ ;
			}
		}
	}

	return DestSize ;
}

// e �^�C�v�p�� NaN �����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_NaN_e(
	int FPClass,
	int Flag_Sharp,
	int Precision,
	int Big,
	u8 *NumberStrBuffer
)
{
	int DestSize ;

	// ��� e+000 ���t���ȊO�� CL_vsprintf_help_ftoa_NaN_f �Ə����͓���
	DestSize = CL_vsprintf_help_ftoa_NaN_f( FPClass, Flag_Sharp, 1, Precision, NumberStrBuffer ) ;

	// ���e+000 ��t����
	NumberStrBuffer[ DestSize + 0 ] = Big ? 'E' : 'e' ;
	NumberStrBuffer[ DestSize + 1 ] = '+' ;
	NumberStrBuffer[ DestSize + 2 ] = '0' ;
	NumberStrBuffer[ DestSize + 3 ] = '0' ;
	NumberStrBuffer[ DestSize + 4 ] = '0' ;
	DestSize += 5 ;

	return DestSize ;
}

// g �^�C�v�p�� NaN �����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_NaN_g(
	int FPClass,
	int Flag_Sharp,
	int Precision,
	u8 *NumberStrBuffer
)
{
	// ���x���}�C�i�X�̏ꍇ�͊���l�� 6 ���Z�b�g
	if( Precision < 0 )
	{
		Precision = 6 ;
	}

	// f �`���̈����ł� Precision �� 1 ���Ȃ���ԂɂȂ�
	if( Precision > 0 )
	{
		Precision -- ;
	}

	// ���x�̐��l�̈����� 1 ���Ȃ��̂ƁA���x���]���ɂ����Ă� 0 ���t���Ȃ��ȊO�� CL_vsprintf_help_ftoa_NaN_f �Ə����͓���
	return CL_vsprintf_help_ftoa_NaN_f( FPClass, Flag_Sharp, 0, Precision, NumberStrBuffer ) ;
}

// a �^�C�v�p�� NaN �����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_NaN_a(
	int FPClass,
	int Flag_Sharp,
	int Precision,
	int Big,
	u8 *NumberStrBuffer
)
{
	int DestSize = 0 ;

	// �擪�� 0x�A��� p+0 ���t���ȊO�� CL_vsprintf_help_ftoa_NaN_f �Ə����͓���
	NumberStrBuffer[ DestSize + 0 ] = '0' ;
	NumberStrBuffer[ DestSize + 1 ] = '0' ;
	DestSize += 2 ;

	DestSize += CL_vsprintf_help_ftoa_NaN_f( FPClass, Flag_Sharp, 1, Precision, &NumberStrBuffer[ DestSize ] ) ;

	NumberStrBuffer[ DestSize + 0 ] = Big ? 'P' : 'p' ;
	NumberStrBuffer[ DestSize + 1 ] = '+' ;
	NumberStrBuffer[ DestSize + 2 ] = '0' ;
	DestSize += 3 ;

	return DestSize ;
}

// a �^�C�v�̕��������_�l�̕����񉻂��s���A�߂�l�͏������񂾃o�C�g��
static int CL_vsprintf_help_ftoa_a(
	double Number,
	int Flag_Sharp,
	int Precision,
	int Big,
	u8 *NumberStrBuffer
)
{
	double TopFigureNumber ;
	double TempNumber ;
	double TargetFigureOne ;
	int LoopFlag ;
	int TempNumberI ;
	u8 NumberCharTemp ;
	int TopFigure ;
	int IntNumberNum ;
	int FloatNumberNum ;
	int i ;
	int DestSize = 0 ;
	int Zero = 0 ;
	u8 NumberStrTemp[ 1024 ] ;

	// ���x�����܂��Ă��Ȃ��ꍇ�͊���l�� 6 ���Z�b�g
	if( Precision < 0 )
	{
		Precision = 6 ;
	}

	// �}�C�i�X�l�̏ꍇ�̓v���X�l�ɂ���
	if( Number < 0.0 )
	{
		Number = -Number ;
	}

	// ���[�v�������ǂ����̃t���O��|��
	LoopFlag = 0 ;

	// �����񉻂�蒼��������ꍇ�ɔ�Ԉʒu
LOOPLABEL :
	DestSize = 0 ;

	// 16�i����\�� 0x ���o�͂���
	NumberStrBuffer[ DestSize + 0 ] = '0' ;
	NumberStrBuffer[ DestSize + 1 ] = 'x' ;
	DestSize += 2 ;

	// �ŏ�ʂ̌���T��

	// 0 �̏ꍇ�͓��ʏ���
	TempNumber = Number ;
	TargetFigureOne = 1.0 ;
	if( Number == 0.0 )
	{
		Zero = 1 ;
		TopFigure = 0 ;
	}
	else
	if( Number < 1.0 )
	{
		for( TopFigure = 0 ; TempNumber <  1.0 ; TempNumber *= 2.0, TopFigure --, TargetFigureOne /= 2.0 ){}
	}
	else
	{
		for( TopFigure = 0 ; TempNumber >= 2.0 ; TempNumber /= 2.0, TopFigure ++, TargetFigureOne *= 2.0 ){}
	}
	TopFigureNumber = TempNumber ;

	// �ŏ�ʌ��̏o��
	NumberCharTemp = ( u8 )TempNumber ;
	NumberStrBuffer[ DestSize ] = '0' + NumberCharTemp ;
	DestSize ++ ;

	// ��������񋓂���
	TempNumber = fmod( TopFigureNumber, 1.0 ) ;
	FloatNumberNum = 0 ;
	for( i = 0 ; i < Precision ; i ++ )
	{
		TargetFigureOne /= 16.0 ;
		TempNumber *= 16.0 ;
		NumberStrTemp[ FloatNumberNum ] = ( u8 )fmod( TempNumber, 16.0 ) ;
		TempNumber -= ( double )NumberStrTemp[ FloatNumberNum ] ;
		FloatNumberNum++ ;
	}

	// ���̌��̒l�� 0.8 �𒴂���ꍇ�͍Ō�̌��̒l�ɂP�𑫂��ĕ����񉻏������ŏ������蒼��
	if( LoopFlag == 0 )
	{
		TempNumber *= 16.0 ;
		if( fmod( TempNumber, 16.0 ) > 8.0 )
		{
			Number += TargetFigureOne ;
			LoopFlag = 1 ;
			goto LOOPLABEL ;
		}
	}

	// ���x�� 0 �ł� Flag_Sharp ���L���ȏꍇ�� . ���Z�b�g����
	if( Precision > 0 || Flag_Sharp )
	{
		NumberStrBuffer[ DestSize ] = '.' ;
		DestSize ++ ;
	}

	// �ŏ�ʌ��ȉ����o�͂���
	for( i = 0 ; i < Precision ; i ++ )
	{
		NumberStrBuffer[ DestSize ] = NumberToCharTable[ Big ][ NumberStrTemp[ i ] ] ;
		DestSize ++ ;
	}

	// �w������ p ���� P ���Z�b�g����
	NumberStrBuffer[ DestSize ] = Big ? 'P' : 'p' ;
	DestSize ++ ;

	// �w������ + ���� - ���Z�b�g����
	if( TopFigure < 0 )
	{
		TopFigure = -TopFigure ;
		NumberStrBuffer[ DestSize ] = '-' ;
	}
	else
	{
		NumberStrBuffer[ DestSize ] = '+' ;
	}
	DestSize ++ ;

	// �w�����̐��l��񋓂���
	if( TopFigure == 0 )
	{
		IntNumberNum = 1 ;
		NumberStrTemp[ 0 ] = 0 ;
	}
	else
	{
		TempNumberI = TopFigure ;
		for( IntNumberNum = 0 ; TempNumberI != 0 ; TempNumberI /= 10, IntNumberNum ++ )
		{
			NumberStrTemp[ IntNumberNum ] = TempNumberI % 10 ;
		}
	}

	// �w�����̏o��
	for( i = IntNumberNum - 1 ; i >= 0 ; i -- )
	{
		NumberStrBuffer[ DestSize ] = '0' + NumberStrTemp[ i ] ;
		DestSize ++ ;
	}

	// �I��
	return DestSize ;
}

// e �^�C�v�̕��������_�l�̕����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_e(
	double Number,
	int Flag_Sharp,
	int Precision,
	int Big,
	u8 *NumberStrBuffer
)
{
	double TopFigureNumber ;
	double TargetFigureOne ;
	double TempNumber ;
	u8 NumberCharTemp ;
	int TopFigure ;
	int FloatNumberNum ;
	int LoopFlag ;
	int i ;
	int DestSize ;
	int Zero = 0 ;
	u8 NumberStrTemp[ 1024 ] ;

	// ���x�����܂��Ă��Ȃ��ꍇ�͊���l�� 6 ���Z�b�g
	if( Precision < 0 )
	{
		Precision = 6 ;
	}

	// �}�C�i�X�l�̏ꍇ�̓v���X�l�ɂ���
	if( Number < 0.0 )
	{
		Number = -Number ;
	}

	// ���[�v�������ǂ����̃t���O��|��
	LoopFlag = 0 ;

	// �����񉻂�蒼��������ꍇ�ɔ�Ԉʒu
LOOPLABEL :
	DestSize = 0 ;

	// �ŏ�ʂ̌���T��

	// 0 �̏ꍇ�͓��ʏ���
	TempNumber = Number ;
	TargetFigureOne = 1.0 ;
	if( Number == 0.0 )
	{
		Zero = 1 ;
		TopFigure = 0 ;
	}
	else
	if( Number < 1.0 )
	{
		for( TopFigure = 0 ; TempNumber <   1.0 ; TempNumber *= 10.0, TopFigure --, TargetFigureOne /= 10.0 ){}
	}
	else
	{
		for( TopFigure = 0 ; TempNumber >= 10.0 ; TempNumber /= 10.0, TopFigure ++, TargetFigureOne *= 10.0 ){}
	}
	TopFigureNumber = TempNumber ;

	// ��������񋓂���
	TempNumber = fmod( TopFigureNumber, 1.0 ) ;
	FloatNumberNum = 0 ;
	for( i = 0 ; i < Precision ; i ++ )
	{
		TargetFigureOne /= 10.0 ;
		TempNumber *= 10.0 ;
		NumberStrTemp[ FloatNumberNum ] = ( u8 )fmod( TempNumber, 10.0 ) ;
		TempNumber -= ( double )NumberStrTemp[ FloatNumberNum ] ;
		FloatNumberNum++ ;
	}

	// ���̌��̒l�� 0.5 �𒴂���ꍇ�͍Ō�̌��̒l�ɂP�𑫂��ĕ����񉻏������ŏ������蒼��
	if( LoopFlag == 0 )
	{
		TempNumber *= 10.0 ;
		if( fmod( TempNumber, 10.0 ) > 5.0 )
		{
			Number += TargetFigureOne ;
			LoopFlag = 1 ;
			goto LOOPLABEL ;
		}
	}

	// �ŏ�ʌ��̏o��
	NumberCharTemp = ( u8 )TopFigureNumber ;
	NumberStrBuffer[ DestSize ] = '0' + NumberCharTemp ;
	DestSize ++ ;

	// ���x�� 0 �ł� Flag_Sharp ���L���ȏꍇ�� . ���Z�b�g����
	if( Precision > 0 || Flag_Sharp )
	{
		NumberStrBuffer[ DestSize ] = '.' ;
		DestSize ++ ;
	}

	// �ŏ�ʌ��ȉ����o�͂���
	for( i = 0 ; i < Precision ; i ++ )
	{
		NumberStrBuffer[ DestSize ] = '0' + NumberStrTemp[ i ] ;
		DestSize ++ ;
	}

	// �w������ e ���� E ���Z�b�g����
	NumberStrBuffer[ DestSize ] = Big ? 'E' : 'e' ;
	DestSize ++ ;

	// �w������ + ���� - ���Z�b�g����
	if( TopFigure < 0 )
	{
		TopFigure = -TopFigure ;
		NumberStrBuffer[ DestSize ] = '-' ;
	}
	else
	{
		NumberStrBuffer[ DestSize ] = '+' ;
	}
	DestSize ++ ;

	// �w�����̂R�����o��
	NumberStrBuffer[ DestSize + 0 ] = '0' + ( TopFigure / 100 ) % 10 ;
	NumberStrBuffer[ DestSize + 1 ] = '0' + ( TopFigure /  10 ) % 10 ;
	NumberStrBuffer[ DestSize + 2 ] = '0' +   TopFigure         % 10 ;
	DestSize += 3 ;

	// �I��
	return DestSize ;
}

// f �^�C�v�̕��������_�l�̕����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_f(
	double Number,
	int Flag_Sharp,
	int Precision,
	u8 *NumberStrBuffer
)
{
	double TempNumber ;
	double TargetFigureOne ;
	int LoopFlag ;
	int IntNumberNum ;
	int FloatNumberNum ;
	int i ;
	int DestSize ;
	u8 NumberStrTemp[ 1024 ] ;

	// ���x�����܂��Ă��Ȃ��ꍇ�͊���l�� 6 ���Z�b�g
	if( Precision < 0 )
	{
		Precision = 6 ;
	}

	// �}�C�i�X�l�̏ꍇ�̓v���X�l�ɂ���
	if( Number < 0.0 )
	{
		Number = -Number ;
	}

	// ���[�v�������ǂ����̃t���O��|��
	LoopFlag = 0 ;

	// �����񉻂�蒼��������ꍇ�ɔ�Ԉʒu
LOOPLABEL :
	DestSize = 0 ;

	IntNumberNum = 0 ;
	TempNumber = Number - fmod( Number, 1.0 ) ;
	if( Number < 1.0 )
	{
		// �[���ȉ��̏ꍇ�͐������̐��l�� 0 �̂ݏo��
		NumberStrBuffer[ DestSize ] = '0' ;
		DestSize ++ ;
		IntNumberNum ++ ;
	}
	else
	{
		// �������̐��l��񋓂���
		do
		{
			NumberStrTemp[ IntNumberNum ] = ( u8 )fmod( TempNumber, 10.0 ) ; 
			TempNumber -= ( double )NumberStrTemp[ IntNumberNum ] ;
			IntNumberNum ++ ;
			TempNumber /= 10.0 ;
		}while( TempNumber >= 1.0 ) ;

		// �������̐��l���Z�b�g����
		for( i = IntNumberNum - 1 ; i >= 0 ; i -- )
		{
			NumberStrBuffer[ DestSize ] = '0' + NumberStrTemp[ i ] ;
			DestSize ++ ;
		}
	}

	// ��������񋓂���
	TempNumber = fmod( Number, 1.0 ) ;
	FloatNumberNum = 0 ;
	TargetFigureOne = 1.0 ;
	for( i = 0 ; i < Precision ; i ++ )
	{
		TargetFigureOne /= 10.0 ;
		TempNumber *= 10.0 ;
		NumberStrTemp[ FloatNumberNum ] = ( u8 )fmod( TempNumber, 10.0 ) ;
		TempNumber -= ( double )NumberStrTemp[ FloatNumberNum ] ;
		FloatNumberNum++ ;
	}

	// ���̌��̒l�� 0.5 �𒴂���ꍇ�͍Ō�̌��̒l�ɂP�𑫂��ĕ����񉻏������ŏ������蒼��
	if( LoopFlag == 0 )
	{
		TempNumber *= 10.0 ;
		if( fmod( TempNumber, 10.0 ) > 5.0 )
		{
			Number += TargetFigureOne ;
			LoopFlag = 1 ;
			goto LOOPLABEL ;
		}
	}

	// ���x�� 0 �� # ���Ȃ��ꍇ�͂����ŏI��
	if( Precision == 0 && Flag_Sharp == 0 )
	{
		return DestSize ;
	}

	// . ���Z�b�g
	NumberStrBuffer[ DestSize ] = '.' ;
	DestSize ++ ;

	// ���������o��
	for( i = 0 ; i < FloatNumberNum ; i ++ )
	{
		NumberStrBuffer[ DestSize ] = '0' + NumberStrTemp[ i ] ;
		DestSize ++ ;
	}

	// �I��
	return DestSize ;
}

// g �^�C�v�̕��������_�l�̕����񉻂��s���A�߂�l�͕�����
static int CL_vsprintf_help_ftoa_g(
	double Number,
	int Flag_Sharp,
	int Precision,
	int Big,
	u8 *NumberStrBuffer
)
{
	double TopFigureNumber ;
	double TempNumber ;
	double TargetFigureOne ;
	int LoopFlag ;
	u8 NumberCharTemp ;
	int TopFigure ;
	int LastNonZeroFigure ;
	int FloatNumberNum ;
	int i ;
	int PrecisionF ;
	int DestSize = 0 ;
	int Zero = 0 ;
	u8 NumberStrTemp[ 1024 ] ;

	// ���x�����܂��Ă��Ȃ��ꍇ�͊���l�� 6 ���Z�b�g
	if( Precision < 0 )
	{
		Precision = 6 ;
	}
	else
	// g �ł� Precision �� 0 �Ƃ��������͖����̂ŁA0 �̏ꍇ�� 1 �ɂ���
	if( Precision == 0 )
	{
		Precision = 1 ;
	}

	// �}�C�i�X�l�̏ꍇ�̓v���X�l�ɂ���
	if( Number < 0.0 )
	{
		Number = -Number ;
	}

	// ���[�v�������ǂ����̃t���O��|��
	LoopFlag = 0 ;

	// �����񉻂�蒼��������ꍇ�ɔ�Ԉʒu
LOOPLABEL :
	DestSize = 0 ;

	// �ŏ�ʂ̌���T��

	// 0 �̏ꍇ�͓��ʏ���
	TempNumber = Number ;
	TargetFigureOne = 1.0 ;
	if( Number == 0.0 )
	{
		Zero = 1 ;
		TopFigure = 0 ;
	}
	else
	if( Number < 1.0 )
	{
		for( TopFigure = 0 ; TempNumber <   1.0 ; TempNumber *= 10.0, TopFigure --, TargetFigureOne /= 10.0 ){}
	}
	else
	{
		for( TopFigure = 0 ; TempNumber >= 10.0 ; TempNumber /= 10.0, TopFigure ++, TargetFigureOne *= 10.0 ){}
	}
	TopFigureNumber = TempNumber ;

	// ��������񋓂���
	LastNonZeroFigure = -1 ;
	TempNumber = fmod( TopFigureNumber, 1.0 ) ;
	FloatNumberNum = 0 ;
	for( i = 0 ; i < Precision - 1 ; i ++ )
	{
		TargetFigureOne /= 10.0 ;
		TempNumber *= 10.0 ;
		NumberStrTemp[ FloatNumberNum ] = ( u8 )fmod( TempNumber, 10.0 ) ;
		TempNumber -= ( double )NumberStrTemp[ FloatNumberNum ] ;

		// 0 �ł͂Ȃ�������A�u0�ł͖������̈ʒu�v��ۑ�
		if( NumberStrTemp[ FloatNumberNum ] != 0 )
		{
			LastNonZeroFigure = FloatNumberNum ;
		}

		FloatNumberNum++ ;
	}

	// ���̌��̒l�� 0.5 �𒴂���ꍇ�͍Ō�̌��̒l�ɂP�𑫂��ĕ����񉻏������ŏ������蒼��
	if( LoopFlag == 0 )
	{
		TempNumber *= 10.0 ;
		if( fmod( TempNumber, 10.0 ) > 5.0 )
		{
			Number += TargetFigureOne ;
			LoopFlag = 1 ;
			goto LOOPLABEL ;
		}
	}

	// �w�����̕\�����s�v���ǂ����𔻒�
	if( TopFigure >= 0 )
	{
		// �ŏ�ʌ��������̏ꍇ�͐��x�����̏ꍇ�͎w�����̕\�����s��Ȃ�
		if( TopFigure < Precision )
		{
			// �w�����̕\�����s��Ȃ��ꍇ�� f �p�̊֐����g�p����

			// �����_�ȉ������܂ŏo�͂��邩������AFlag_Sharp �̎w�肪�����ꍇ�͐��x�͈͓��ł��s�v�� 0 �͏o�͂��Ȃ�
			if( Flag_Sharp == 0 )
			{
				PrecisionF = -( TopFigure - ( LastNonZeroFigure + 1 ) ) ;
			}
			else
			{
				PrecisionF = -( TopFigure - ( Precision - 1 ) ) ;
			}
			if( PrecisionF < 0 )
			{
				PrecisionF = 0 ;
			}
			return CL_vsprintf_help_ftoa_f( Number, Flag_Sharp, PrecisionF, NumberStrBuffer ) ;
		}
	}
	else
	{
		// �ŏ�ʌ��������̏ꍇ�͍ŏ�ʌ��� 0.0001 ���傫���ꍇ�͎w�����̕\�����s��Ȃ�
		if( TopFigure >= -4 )
		{
			// �w�����̕\�����s��Ȃ��ꍇ�� f �p�̊֐����g�p����

			// �����_�ȉ������܂ŏo�͂��邩������
			PrecisionF = -TopFigure + Precision - 1 ;

			// Flag_Sharp �̎w�肪�����ꍇ�́A���x�͈͓��ł��s�v�� 0 �͏o�͂��Ȃ�
			if( Flag_Sharp == 0 )
			{
				if( PrecisionF > -TopFigure + LastNonZeroFigure + 1 )
				{
					PrecisionF = -TopFigure + LastNonZeroFigure + 1 ;
				}
			}

			return CL_vsprintf_help_ftoa_f( Number, Flag_Sharp, PrecisionF, NumberStrBuffer ) ;
		}
	}

	// �ŏ�ʌ��̏o��
	NumberCharTemp = ( u8 )TopFigureNumber ;
	NumberStrBuffer[ DestSize ] = '0' + NumberCharTemp ;
	DestSize ++ ;

	// Flag_Sharp �̎w�肪�����ꍇ�� Precision �ȓ��ł������_�ȉ��Ŗ��[���瑱�� 0 �͏o�͂��Ȃ�
	if( Flag_Sharp == 0 )
	{
		Precision = LastNonZeroFigure + 2 ;
	}

	// ���x�� 1 �ł� Flag_Sharp ���L���ȏꍇ�� . ���Z�b�g����
	if( Precision > 1 || Flag_Sharp )
	{
		NumberStrBuffer[ DestSize ] = '.' ;
		DestSize ++ ;
	}

	// �ŏ�ʌ��ȉ����o�͂���
	for( i = 0 ; i < Precision - 1 ; i ++ )
	{
		NumberStrBuffer[ DestSize ] = '0' + NumberStrTemp[ i ] ;
		DestSize ++ ;
	}

	// �w������ e ���� E ���Z�b�g����
	NumberStrBuffer[ DestSize ] = Big ? 'E' : 'e' ;
	DestSize ++ ;

	// �w������ + ���Z�b�g����
	if( TopFigure < 0 )
	{
		TopFigure = -TopFigure ;
		NumberStrBuffer[ DestSize ] = '-' ;
	}
	else
	{
		NumberStrBuffer[ DestSize ] = '+' ;
	}
	DestSize ++ ;

	// �w�����̂R�����o��
	NumberStrBuffer[ DestSize + 0 ] = '0' + ( TopFigure / 100 ) % 10 ;
	NumberStrBuffer[ DestSize + 1 ] = '0' + ( TopFigure /  10 ) % 10 ;
	NumberStrBuffer[ DestSize + 2 ] = '0' +   TopFigure         % 10 ;
	DestSize += 3 ;

	// �I��
	return DestSize ;
}

// ���������_�l�𕶎��񉻂���֐��A�߂�l�͏������񂾃o�C�g��
static int CL_vsprintf_help_ftoa(
	double Number,
	int Type,
	int Flag_Sharp,
	int Flag_Zero,
	int Flag_Minus,
	int Flag_Plus,
	int Flag_Space,
	int Width,
	int Precision,
	char *Dest,
	int DestCharCodeFormat
)
{
	int DestSize ;
	u32 NumberNum ;
	u32 SignedCharNum ;
	int NumberMinus ;
	int FPClass ;
	int NaN ;
	int Big ;
	int i ;
	int Flag_Precision ;
	u8 NumberStrTemp[ 1024 ] ;

	DestSize = 0 ;

	// NaN�`�F�b�N
	FPClass = _fpclass( Number ) ;
	NaN = 0 ;
	if( FPClass == _FPCLASS_SNAN ||
		FPClass == _FPCLASS_QNAN ||
		FPClass == _FPCLASS_NINF ||
		FPClass == _FPCLASS_PINF )
	{
		NaN = 1 ;

		// NaN�̒l�̓v���X����
		if( FPClass == _FPCLASS_SNAN ||
			FPClass == _FPCLASS_QNAN )
		{
			NumberMinus = 0 ;
		}
		else
		{
			// ����ȊO�͕��̖�����̏ꍇ�̂݃}�C�i�X����
			NumberMinus = FPClass == _FPCLASS_NINF ? 1 : 0 ;
		}
	}
	else
	{
		// �ʏ�̒l�̏ꍇ�̏���

		// �}�C�i�X�̒l�̓v���X�ɂ���
		NumberMinus = 0 ;
		if( Number < 0.0 )
		{
			Number = -Number ;
			NumberMinus = 1 ;
		}
	}

	// ���̎w�肪�����ꍇ�͉��� 0 ���Z�b�g
	if( Width < 0 )
	{
		Width = 0 ;
	}

	// �啶�����ǂ������Z�b�g
	Big = 0 ;
	if( Type == PRINTF_TYPE_E ||
		Type == PRINTF_TYPE_G ||
		Type == PRINTF_TYPE_A )
	{
		Big = 1 ;
	}

	// ���x�̎w�肪���邩�ǂ������Z�b�g
	Flag_Precision = Precision >= 0 ? 1 : 0 ;

	// ���l�ߎw�肳��Ă���ꍇ�� Flag_Zero �͖���
	if( Flag_Minus )
	{
		Flag_Zero = 0 ;
	}

	// Flag_Plus ������ꍇ�AFlag_Space �͈Ӗ��𐬂��Ȃ�
	if( Flag_Plus )
	{
		Flag_Space = 0 ;
	}

	// �l���}�C�i�X�l�̏ꍇ�AFlag_Plus, Flag_Space �͈Ӗ��𐬂��Ȃ�
	if( NumberMinus )
	{
		Flag_Space = 0 ;
		Flag_Plus  = 0 ;
	}

	// ���l�𕶎��񉻂ƕ������̎Z�o
	if( NaN )
	{
		// NaN �̏ꍇ

		switch( Type )
		{
		case PRINTF_TYPE_f :
			NumberNum = CL_vsprintf_help_ftoa_NaN_f( FPClass, Flag_Sharp, 1, Precision, NumberStrTemp ) ;
			break ;

		case PRINTF_TYPE_e :
		case PRINTF_TYPE_E :
			NumberNum = CL_vsprintf_help_ftoa_NaN_e( FPClass, Flag_Sharp, Precision, Type == PRINTF_TYPE_E ? 1 : 0, NumberStrTemp ) ;
			break ;

		case PRINTF_TYPE_g :
		case PRINTF_TYPE_G :
			NumberNum = CL_vsprintf_help_ftoa_NaN_g( FPClass, Flag_Sharp, Precision, NumberStrTemp ) ;
			break ;

		case PRINTF_TYPE_a :
		case PRINTF_TYPE_A :
			NumberNum = CL_vsprintf_help_ftoa_NaN_a( FPClass, Flag_Sharp, Precision, Type == PRINTF_TYPE_A ? 1 : 0, NumberStrTemp ) ;
			break ;
		}
	}
	else
	{
		// �ʏ퐔�l�̏ꍇ

		switch( Type )
		{
		case PRINTF_TYPE_f :
			NumberNum = CL_vsprintf_help_ftoa_f( Number, Flag_Sharp, Precision, NumberStrTemp ) ;
			break ;

		case PRINTF_TYPE_e :
		case PRINTF_TYPE_E :
			NumberNum = CL_vsprintf_help_ftoa_e( Number, Flag_Sharp, Precision, Type == PRINTF_TYPE_E ? 1 : 0, NumberStrTemp ) ;
			break ;

		case PRINTF_TYPE_g :
		case PRINTF_TYPE_G :
			NumberNum = CL_vsprintf_help_ftoa_g( Number, Flag_Sharp, Precision, Type == PRINTF_TYPE_G ? 1 : 0, NumberStrTemp ) ;
			break ;

		case PRINTF_TYPE_a :
		case PRINTF_TYPE_A :
			NumberNum = CL_vsprintf_help_ftoa_a( Number, Flag_Sharp, Precision, Type == PRINTF_TYPE_A ? 1 : 0, NumberStrTemp ) ;
			break ;
		}
	}

	// ������X�y�[�X�̕��������Z�o
	if( Flag_Space || Flag_Plus || NumberMinus )
	{
		SignedCharNum = 1 ;
	}
	else
	{
		SignedCharNum = 0 ;
	}

	// �����琔���̕������A���������A�i���t������������
	Width -= NumberNum + SignedCharNum ;

	// ���l�ߎw�肪���邩�ǂ����ŏ����𕪊�
	if( Flag_Minus )
	{
		// ���l�ߎw�肪����ꍇ

		// �����n����������ꍇ�̓Z�b�g����
		if( NumberMinus || Flag_Plus || Flag_Space )
		{
			DestSize += PutCharCode_inline( NumberMinus ? '-' : ( Flag_Plus ? '+' : ' ' ), DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
		}

		// ���l���Z�b�g����
		for( i = 0 ; i < ( int )NumberNum ; i++ )
		{
			DestSize += PutCharCode_inline( NumberStrTemp[ i ], DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
		}

		// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
		if( Width > 0 )
		{
			DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
		}
	}
	else
	{
		// 0�l�ߎw�肪���邩�ǂ����ŏ����𕪊�
		if( Flag_Zero )
		{
			// �����n����������ꍇ�̓Z�b�g����
			if( NumberMinus || Flag_Plus || Flag_Space )
			{
				DestSize += PutCharCode_inline( NumberMinus ? '-' : ( Flag_Plus ? '+' : ' ' ), DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
			}

			// �]�蕝������ꍇ�́A0���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, '0', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}
		}
		else
		{
			// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}

			// �����n����������ꍇ�̓Z�b�g����
			if( NumberMinus || Flag_Plus || Flag_Space )
			{
				DestSize += PutCharCode_inline( NumberMinus ? '-' : ( Flag_Plus ? '+' : ' ' ), DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
			}
		}

		// ���l���Z�b�g����
		for( i = 0 ; i < ( int )NumberNum ; i++ )
		{
			DestSize += PutCharCode_inline( NumberStrTemp[ i ], DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
		}
	}

	return DestSize ;
}

// �����l�𕶎��񉻂���֐��A�߂�l�͏������񂾃o�C�g��
static int CL_vsprintf_help_itoa(
	u64 Number,
	int NumberMinus,
	int Signed,
	u32 Decimal,
	int Flag_Sharp,
	int Flag_Zero,
	int Flag_Minus,
	int Flag_Plus,
	int Flag_Space,
	int Width,
	int Precision,
	int Big,
	char *Dest,
	int DestCharCodeFormat
)
{
	int DestSize ;
	u32 NumberNum ;
	u32 SignedCharNum ;
	u32 SharpCharNum ;
	u32 PrecisionCharNum ;
	int i ;
	int Flag_Precision ;
	u8 NumberStrTemp[ 1024 ] ;

	DestSize = 0 ;

	// ���̎w�肪�����ꍇ�͉��� 0 ���Z�b�g
	if( Width < 0 )
	{
		Width = 0 ;
	}

	// ���x�̎w�肪�����ꍇ�͊���� 1 ���Z�b�g
	if( Precision < 0 )
	{
		Precision = 1 ;
		Flag_Precision = 0 ;
	}
	else
	{
		Flag_Precision = 1 ;
	}

	// 10�i���ȊO�͕�������
	if( Decimal != 10 )
	{
		Signed = 0 ;
	}

	// ���l�ߎw�肳��Ă��邩�A���x�w�肪����ꍇ�� Flag_Zero �͖���
	if( Flag_Minus || Flag_Precision )
	{
		Flag_Zero = 0 ;
	}

	// �����������ꍇ�� �}�C�i�X�l�AFlag_Plus, Flag_Space �͈Ӗ��𐬂��Ȃ�
	if( Signed == 0 )
	{
		NumberMinus = 0 ;
		Flag_Plus   = 0 ;
		Flag_Space  = 0 ;
	}
	else
	{
		// ����������ꍇ�� # �͈Ӗ��𐬂��Ȃ�
		Flag_Sharp = 0 ;

		// Flag_Plus ������ꍇ�AFlag_Space �͈Ӗ��𐬂��Ȃ�
		if( Flag_Plus )
		{
			Flag_Space = 0 ;
		}

		// �l���}�C�i�X�l�̏ꍇ�AFlag_Plus, Flag_Space �͈Ӗ��𐬂��Ȃ�
		if( NumberMinus )
		{
			Flag_Space = 0 ;
			Flag_Plus  = 0 ;
		}
	}

	// ���l�𕶎��񉻂ƕ������̎Z�o
	NumberNum = 0 ;
	if( Number == 0 )
	{
		// ���l�� 0 �ł����x�� 0 �ł͂Ȃ���� 0 �̂P�������o�͂���
		if( Precision == 0 )
		{
			NumberStrTemp[ NumberNum ] = '0' ;
			NumberNum ++ ;
		}
	}
	else
	{
		while( Number > 0 )
		{
			NumberStrTemp[ NumberNum ] = NumberToCharTable[ Big ][ Number % Decimal ] ;
			NumberNum ++ ;
			Number /= Decimal ;
		}
	}

	// ���x�ɂ�镶�������Z�o
	if( NumberNum < ( u32 )Precision )
	{
		PrecisionCharNum = Precision - NumberNum ;
	}
	else
	{
		PrecisionCharNum = 0 ;
	}

	// # ������ꍇ�̕��������Z�o
	SharpCharNum = 0 ;
	if( Flag_Sharp )
	{
		switch( Decimal )
		{
		case 8 :
			SharpCharNum = 1 ;
			break ;

		case 16 :
			SharpCharNum = 2 ;
			break ;
		}
	}

	// ������X�y�[�X�̕��������Z�o
	if( Flag_Space || Flag_Plus || NumberMinus )
	{
		SignedCharNum = 1 ;
	}
	else
	{
		SignedCharNum = 0 ;
	}

	// �����琔���̕������A���������A�i���t������������
	Width -= NumberNum + PrecisionCharNum + SignedCharNum + SharpCharNum ;

	// ���l�ߎw�肪���邩�ǂ����ŏ����𕪊�
	if( Flag_Minus )
	{
		// ���l�ߎw�肪����ꍇ

		// �����n����������ꍇ�̓Z�b�g����
		if( NumberMinus || Flag_Plus || Flag_Space )
		{
			DestSize += PutCharCode_inline( NumberMinus ? '-' : ( Flag_Plus ? '+' : ' ' ), DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
		}

		// �i���t������������ꍇ�͋L�����Z�b�g����
		if( Flag_Sharp )
		{
			if( Decimal == 8 )
			{
				DestSize += PutCharCode_inline( '0', DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
			}
			else
			{
				DestSize += PutCharCode_inline( '0',             DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
				DestSize += PutCharCode_inline( Big ? 'X' : 'x', DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
			}
		}

		// ���x�ɑ���Ȃ����� 0 ���Z�b�g����
		if( PrecisionCharNum > 0 )
		{
			DestSize += CL_vsprintf_help_set_char( PrecisionCharNum, '0', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
		}

		// ���l���Z�b�g����
		for( i = NumberNum - 1 ; i >= 0 ; i-- )
		{
			DestSize += PutCharCode_inline( NumberStrTemp[ i ], DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
		}

		// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
		if( Width > 0 )
		{
			DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
		}
	}
	else
	{
		// 0�l�ߎw�肪���邩�ǂ����ŏ����𕪊�
		if( Flag_Zero )
		{
			// �����n����������ꍇ�̓Z�b�g����
			if( NumberMinus || Flag_Plus || Flag_Space )
			{
				DestSize += PutCharCode_inline( NumberMinus ? '-' : ( Flag_Plus ? '+' : ' ' ), DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
			}

			// �i���t������������ꍇ�͋L�����Z�b�g����
			if( Flag_Sharp )
			{
				if( Decimal == 8 )
				{
					DestSize += PutCharCode_inline( '0', DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
				}
				else
				{
					DestSize += PutCharCode_inline( '0',             DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
					DestSize += PutCharCode_inline( Big ? 'X' : 'x', DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
				}
			}

			// �]�蕝������ꍇ�́A0���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, '0', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}
		}
		else
		{
			// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}

			// �����n����������ꍇ�̓Z�b�g����
			if( NumberMinus || Flag_Plus || Flag_Space )
			{
				DestSize += PutCharCode_inline( NumberMinus ? '-' : ( Flag_Plus ? '+' : ' ' ), DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
			}

			// �i���t������������ꍇ�͋L�����Z�b�g����
			if( Flag_Sharp )
			{
				if( Decimal == 8 )
				{
					DestSize += PutCharCode_inline( '0', DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
				}
				else
				{
					DestSize += PutCharCode_inline( '0',             DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
					DestSize += PutCharCode_inline( Big ? 'X' : 'x', DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
				}
			}
		}

		// ���x�ɑ���Ȃ����� 0 ���Z�b�g����
		if( PrecisionCharNum > 0 )
		{
			DestSize += CL_vsprintf_help_set_char( PrecisionCharNum, '0', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
		}

		// ���l���Z�b�g����
		for( i = NumberNum - 1 ; i >= 0 ; i-- )
		{
			DestSize += PutCharCode_inline( NumberStrTemp[ i ], DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
		}
	}

	return DestSize ;
}

// s �� S ����������֐��A�߂�l�͏������񂾃o�C�g��
static int CL_vsprintf_help_s(
	char *String,
	int Flag_Zero,
	int Flag_Minus,
	int Width,
	int Precision,
	char *Dest,
	int DestCharCodeFormat
)
{
	int DestSize ;
	u8 NullBuffer[ 128 ] ;
	int StrLength ;

	DestSize = 0 ;

	// NULL �̏ꍇ��(null)���Z�b�g
	if( String == NULL )
	{
		int TempAddr = 0 ;
		TempAddr += PutCharCode_inline( '(', DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;
		TempAddr += PutCharCode_inline( 'n', DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;
		TempAddr += PutCharCode_inline( 'u', DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;
		TempAddr += PutCharCode_inline( 'l', DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;
		TempAddr += PutCharCode_inline( 'l', DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;
		TempAddr += PutCharCode_inline( ')', DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;
		TempAddr += PutCharCode_inline(   0, DestCharCodeFormat, ( char * )&NullBuffer[ TempAddr ] ) ;

		String = ( char * )NullBuffer ;
	}

	// ������̒������擾
	StrLength = CL_strlen( DestCharCodeFormat, String ) ;

	// ���̎w�肪�����ꍇ�͉��� 0 ���Z�b�g
	if( Width < 0 )
	{
		Width = 0 ;
	}

	// ���x�̎w�肪�����ꍇ�͕�����̒������Z�b�g
	if( Precision < 0 )
	{
		Precision = StrLength ;
	}
	else
	{
		// ���x�������񒷈ȉ��̏ꍇ�͐��x�𕶎��񒷂ɂ���
		if( Precision > StrLength )
		{
			Precision = StrLength ;
		}
	}

	// ���l�ߎw�肳��Ă���ꍇ�� Flag_Zero �͖���
	if( Flag_Minus )
	{
		Flag_Zero = 0 ;
	}

	// �����琸�x������
	Width -= Precision ;

	// ���l�ߎw�肪���邩�ǂ����ŏ����𕪊�
	if( Flag_Minus )
	{
		// ���l�ߎw�肪����ꍇ

		// ��������R�s�[����
		CL_strncpy( DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ], String, Precision ) ;
		DestSize += GetCharCodeFormatUnitSize_inline( DestCharCodeFormat ) * Precision ;

		// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
		if( Width > 0 )
		{
			DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
		}
	}
	else
	{
		// 0�l�ߎw�肪���邩�ǂ����ŏ����𕪊�
		if( Flag_Zero )
		{
			// �]�蕝������ꍇ�́A0���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, '0', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}
		}
		else
		{
			// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}
		}

		// ��������R�s�[����
		CL_strncpy( DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ], String, Precision ) ;
		DestSize += GetCharCodeFormatUnitSize_inline( DestCharCodeFormat ) * Precision ;
	}

	return DestSize ;
}

// c �� C ����������֐��A�߂�l�͏������񂾃o�C�g��
static int CL_vsprintf_help_c(
	u32 CharCode,
	int Flag_Zero,
	int Flag_Minus,
	int Width,
	char *Dest,
	int DestCharCodeFormat
)
{
	int DestSize ;

	DestSize = 0 ;

	// ���̎w�肪�����ꍇ�͉��� 1 ���Z�b�g
	if( Width < 0 )
	{
		Width = 1 ;
	}

	// ���l�ߎw�肳��Ă���ꍇ�� Flag_Zero �͖���
	if( Flag_Minus )
	{
		Flag_Zero = 0 ;
	}

	// ������P����������
	Width -= 1 ;

	// ���l�ߎw�肪���邩�ǂ����ŏ����𕪊�
	if( Flag_Minus )
	{
		// ���l�ߎw�肪����ꍇ

		// �������Z�b�g����
		DestSize += PutCharCode_inline( CharCode, DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;

		// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
		if( Width > 0 )
		{
			DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
		}
	}
	else
	{
		// 0�l�ߎw�肪���邩�ǂ����ŏ����𕪊�
		if( Flag_Zero )
		{
			// �]�蕝������ꍇ�́A0���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, '0', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}
		}
		else
		{
			// �]�蕝������ꍇ�́A�X�y�[�X���Z�b�g����
			if( Width > 0 )
			{
				DestSize += CL_vsprintf_help_set_char( Width, ' ', ( char * )&( ( u8 * )Dest )[ DestSize ], DestCharCodeFormat ) ;
			}
		}

		// �������Z�b�g����
		DestSize += PutCharCode_inline( CharCode, DestCharCodeFormat, ( char * )&( ( u8 * )Dest )[ DestSize ] ) ;
	}

	return DestSize ;
}

extern int CL_vsprintf( int CharCodeFormat, char *Buffer, const char *FormatString, va_list Arg )
{
	int i ;
	int DestSize ;
	int WriteBytes ;
	u32 BaseBuffer[ 1024 ] ;
	u32 *TempBuffer = NULL ;
	u32 *FCode ;
	int FormatStringSize ;

	// �P�����S�o�C�g�`���ɕϊ�����
	FormatStringSize = StringToCharCodeString_inline( FormatString, CharCodeFormat, NULL ) ;
	if( FormatStringSize + sizeof( u32 ) * 16 > sizeof( BaseBuffer ) )
	{
		TempBuffer = ( u32 * )malloc( FormatStringSize + sizeof( u32 ) * 16 ) ;
		if( TempBuffer == NULL )
		{
			return -1 ;
		}

		FCode = TempBuffer ;
	}
	else
	{
		FCode = BaseBuffer ;
	}
	memset( FCode, 0, FormatStringSize + sizeof( u32 ) * 16 ) ;
	StringToCharCodeString_inline( FormatString, CharCodeFormat, FCode ) ;

	DestSize = 0 ;
	for( i = 0 ; FCode[ i ] != 0 ; )
	{
		if( FCode[ i ] == 0x25 && FCode[ i + 1 ] == 0x25 )
		{
			WriteBytes = PutCharCode_inline( 0x25, CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;
			DestSize += WriteBytes ;
			i += 2 ;
		}
		else
		if( FCode[ i ] != 0x25 || FCode[ i + 1 ] == 0x25 )
		{
			WriteBytes = PutCharCode_inline( FCode[ i ], CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;
			DestSize += WriteBytes ;
			i ++ ;
		}
		else
		{
			int Flag_Sharp   = 0 ;
			int Flag_Zero    = 0 ;
			int Flag_Space   = 0 ;
			int Flag_Plus    = 0 ;
			int Flag_Minus   = 0 ;
			int FlagEnd      = 0 ;
			int Width        = -1 ;
			int Dot          = 0 ;
			int Precision    = -1 ;
			int PrecisionEnd = 0 ;
			int Prefix       = -1 ;
			int Type         = -1 ;
			int LoopEnd      = 0 ;
			int UseCharNum ;

			i ++ ;
			do
			{
				switch( FCode[ i ] )
				{
				default :
					LoopEnd = 1 ;
					break ;

				case '#':
				case '-':
				case '+':
				case ' ':
					// �t���O�f�B���N�e�B�u�́A�t���O�f�B���N�e�B�u���w��ł���͈͂���O��Ă�����G���[�ƂȂ�
					if( FlagEnd )
					{
						LoopEnd = 1 ;
					}
					else
					{
						switch( FCode[ i ] )
						{
						case '#':	Flag_Sharp = 1 ; break ;
						case '-':	Flag_Minus = 1 ; break ;
						case '+':	Flag_Plus  = 1 ; break ;
						case ' ':	Flag_Space = 1 ; break ;
						}
						i ++ ;
					}
					break ;

				case '.':
					// ���� . ���������ꍇ�A��ȏ�� . �̓G���[�ƂȂ�
					if( Dot )
					{
						LoopEnd = 1 ;
					}
					else
					{
						// . �͐��x�w��̊J�n�Ƌ��ɁA�t���O�f�B���N�e�B�u�ƕ������w��ł���͈͂̏I���ł�����
						FlagEnd   = 1 ;
						Dot       = 1 ;
						Precision = 0 ;
						i ++ ;
					}
					break ;

					// * �� . �̑O���ォ�ŏ������������
				case '*':
					// ���x�w��͈̔͊O�Ő��l����������G���[�ƂȂ�
					if( PrecisionEnd )
					{
						LoopEnd = 1 ;
					}
					else
					{
						// . �̌�̏ꍇ�͐��x�A�O�̏ꍇ�͕������w��
						if( Dot )
						{
							Precision = va_arg( Arg, int ) ;
							i ++ ;
						}
						else
						{
							Width = va_arg( Arg, int ) ;
							i ++ ;
						}
					}
					break ;

					// ������ . �̑O���ォ�ŏ������������
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					// ���x�w��͈̔͊O�Ő��l����������G���[�ƂȂ�
					if( PrecisionEnd )
					{
						LoopEnd = 1 ;
					}
					else
					{
						// . �̌�̏ꍇ�͐��x�A�O�̏ꍇ�͕������w��
						if( Dot )
						{
							Precision = ( int )CL_vsprintf_help_getnumber( &FCode[ i ], &UseCharNum ) ;
							i += UseCharNum ;
						}
						else
						{
							// . �̑O�Ńt���O�f�B���N�e�B�u�w��ł���͈͂� '0' �ł������ꍇ�̓t���O�f�B���N�e�B�u�Ƃ��Ă� '0' �Ɣ��f����
							if( FlagEnd == 0 && FCode[ i ] == '0' )
							{
								Flag_Zero = 1 ;
								i ++ ;
							}
							else
							{
								// ����ȊO�̏ꍇ�͕������w��A�������w��̓t���O�f�B���N�e�B�u�͈͂̏I���ł�����
								FlagEnd = 1 ;
								Width = ( int )CL_vsprintf_help_getnumber( &FCode[ i ], &UseCharNum ) ;
								i += UseCharNum ;
							}
						}
					}
					break ;

				case 'h':
				case 'w':
				case 'l':
				case 'I':
					// �v���t�B�b�N�X�w��̓t���O�f�B���N�e�B�u�A�������w��A�h�b�g�A���x�w��͈͂̏I���ł�����
					FlagEnd      = 1 ;
					Dot          = 1 ;
					PrecisionEnd = 1 ;
					switch( FCode[ i ] )
					{
					case 'h':	Prefix = PRINTF_SIZE_PREFIX_h ;	i ++ ;	break ;
					case 'w':	Prefix = PRINTF_SIZE_PREFIX_w ;	i ++ ;	break ;
					case 'l':
						if( FCode[ i + 1 ] == 'l' )
						{
							Prefix = PRINTF_SIZE_PREFIX_ll ;
							i += 2 ;
						}
						else
						{
							Prefix = PRINTF_SIZE_PREFIX_l ;
							i ++ ;
						}
						break ;

					case 'I':
						if( FCode[ i + 1 ] == '3' && FCode[ i + 2 ] == '2' )
						{
							Prefix = PRINTF_SIZE_PREFIX_I32 ;
							i += 3 ;
						}
						else
						if( FCode[ i + 1 ] == '6' && FCode[ i + 2 ] == '4' )
						{
							Prefix = PRINTF_SIZE_PREFIX_I64 ;
							i += 3 ;
						}
						else
						{
							Prefix = PRINTF_SIZE_PREFIX_I ;
							i ++ ;
						}
						break ;
					}
					break ;

					// �^�w��́A�^�w��ł���Ƌ��ɏ����w��\���̏I���ł�����
				case 'c':	Type = PRINTF_TYPE_c ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'C':	Type = PRINTF_TYPE_C ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'd':	Type = PRINTF_TYPE_d ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'i':	Type = PRINTF_TYPE_i ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'o':	Type = PRINTF_TYPE_o ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'u':	Type = PRINTF_TYPE_u ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'x':	Type = PRINTF_TYPE_x ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'X':	Type = PRINTF_TYPE_X ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'e':	Type = PRINTF_TYPE_e ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'E':	Type = PRINTF_TYPE_E ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'f':	Type = PRINTF_TYPE_f ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'g':	Type = PRINTF_TYPE_g ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'G':	Type = PRINTF_TYPE_G ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'a':	Type = PRINTF_TYPE_a ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'A':	Type = PRINTF_TYPE_A ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'n':	Type = PRINTF_TYPE_n ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'p':	Type = PRINTF_TYPE_p ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 's':	Type = PRINTF_TYPE_s ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'S':	Type = PRINTF_TYPE_S ;	i ++ ;	LoopEnd = 1 ;	break ;
				case 'Z':	Type = PRINTF_TYPE_Z ;	i ++ ;	LoopEnd = 1 ;	break ;
				}
			}while( LoopEnd == 0 ) ;

			// �^�w�肪�����ꍇ�̓G���[�Ȃ̂łȂɂ��������Ȃ�
			if( Type != -1 )
			{
				switch( Type )
				{
					// wchar �ɂ͔�Ή��Ȃ̂� c �� C �͓�������
				case PRINTF_TYPE_c :
				case PRINTF_TYPE_C :
					{
						u32 ParamC ;

						switch( GetCharCodeFormatUnitSize_inline( CharCodeFormat ) )
						{
						case 1 :
							ParamC = va_arg( Arg, u8 ) ;
							break ;

						case 2 :
							ParamC = va_arg( Arg, u16 ) ;
							break ;

						case 4 :
							ParamC = va_arg( Arg, u32 ) ;
							break ;
						}

						WriteBytes = CL_vsprintf_help_c(
							ParamC,
							Flag_Zero, Flag_Minus,
							Width,
							( char * )&( ( u8 * )Buffer )[ DestSize ],
							CharCodeFormat
						) ;
						DestSize += WriteBytes ;
					}
					break ;

				case PRINTF_TYPE_d :
				case PRINTF_TYPE_i :
					{
						s64 ParamI ;
						int NumberMinus ;

						switch( Prefix )
						{
						default :
						case PRINTF_SIZE_PREFIX_l	:
						case PRINTF_SIZE_PREFIX_I	:
						case PRINTF_SIZE_PREFIX_I32	:
							ParamI = va_arg( Arg, int ) ;
							break ;

						case PRINTF_SIZE_PREFIX_h	:
							ParamI = ( short )va_arg( Arg, int ) ;
							break ;

						case PRINTF_SIZE_PREFIX_ll	:
						case PRINTF_SIZE_PREFIX_I64	:
							ParamI = va_arg( Arg, s64 ) ;
							break ;
						}

						NumberMinus = 0 ;
						if( ParamI < 0 )
						{
							NumberMinus = 1 ;
							ParamI = -ParamI ;
						}

						WriteBytes = CL_vsprintf_help_itoa(
							( u64 )ParamI,
							NumberMinus,
							1,		// Signed
							10,		// Decimal
							Flag_Sharp, Flag_Zero, Flag_Minus, Flag_Plus, Flag_Space,
							Width, Precision,
							0,		// Big
							( char * )&( ( u8 * )Buffer )[ DestSize ],
							CharCodeFormat
						) ;
						DestSize += WriteBytes ;
					}
					break ;

				case PRINTF_TYPE_u :
				case PRINTF_TYPE_o :
				case PRINTF_TYPE_x :
				case PRINTF_TYPE_X :
					{
						u64 ParamU ;
						u32 Decimal ;

						switch( Type )
						{
						case PRINTF_TYPE_u :
							Decimal = 10 ;
							break ;

						case PRINTF_TYPE_o :
							Decimal = 8 ;
							break ;

						case PRINTF_TYPE_x :
						case PRINTF_TYPE_X :
							Decimal = 16 ;
							break ;
						}

						switch( Prefix )
						{
						default :
						case PRINTF_SIZE_PREFIX_l	:
						case PRINTF_SIZE_PREFIX_I32	:
							ParamU = va_arg( Arg, u32 ) ;
							break ;

						case PRINTF_SIZE_PREFIX_I	:
							if( sizeof( void * ) > 4 )
							{
								ParamU = va_arg( Arg, u64 ) ;
							}
							else
							{
								ParamU = va_arg( Arg, u32 ) ;
							}
							break ;

						case PRINTF_SIZE_PREFIX_h	:
							ParamU = ( u16 )va_arg( Arg, u32 ) ;
							break ;

						case PRINTF_SIZE_PREFIX_ll	:
						case PRINTF_SIZE_PREFIX_I64	:
							ParamU = va_arg( Arg, u64 ) ;
							break ;
						}

						WriteBytes = CL_vsprintf_help_itoa(
							ParamU,
							0,		// NumberMinus
							0,		// Signed
							Decimal,
							Flag_Sharp, Flag_Zero, Flag_Minus, Flag_Plus, Flag_Space,
							Width, Precision,
							Type == PRINTF_TYPE_X,	// Big
							( char * )&( ( u8 * )Buffer )[ DestSize ],
							CharCodeFormat
						) ;
						DestSize += WriteBytes ;
					}
					break ;

				case PRINTF_TYPE_e :
				case PRINTF_TYPE_E :
				case PRINTF_TYPE_f :
				case PRINTF_TYPE_g :
				case PRINTF_TYPE_G :
				case PRINTF_TYPE_a :
				case PRINTF_TYPE_A :
					{
						double ParamF ;

						ParamF = va_arg( Arg, double ) ;

						WriteBytes = CL_vsprintf_help_ftoa(
							ParamF,
							Type,
							Flag_Sharp, Flag_Zero, Flag_Minus, Flag_Plus, Flag_Space,
							Width, Precision,
							( char * )&( ( u8 * )Buffer )[ DestSize ],
							CharCodeFormat
						) ;
						DestSize += WriteBytes ;
					}
					break ;

				case PRINTF_TYPE_n :
					// ��Ή�
					break ;

				case PRINTF_TYPE_p :
					{
						void *ParamP ;

						// ���x�͋����ύX
						ParamP = va_arg( Arg, void * ) ;
						if( sizeof( void * ) > 4 )
						{
							Precision = 16 ;
						}
						else
						{
							Precision = 8 ;
						}

						WriteBytes = CL_vsprintf_help_itoa(
							( u64 )ParamP,
							0,		// NumberMinus
							0,		// Signed
							16,
							Flag_Sharp, Flag_Zero, Flag_Minus, Flag_Plus, Flag_Space,
							Width, Precision,
							1,		// Big
							( char * )&( ( u8 * )Buffer )[ DestSize ],
							CharCodeFormat
						) ;
						DestSize += WriteBytes ;
					}
					break ;

					// wchar �ɂ͔�Ή��Ȃ̂� s �� S �͓�������
				case PRINTF_TYPE_s :
				case PRINTF_TYPE_S :
					{
						char *ParamP ;

						ParamP = va_arg( Arg, char * ) ;

						WriteBytes = CL_vsprintf_help_s(
							ParamP,
							Flag_Zero, Flag_Minus,
							Width, Precision,
							( char * )&( ( u8 * )Buffer )[ DestSize ],
							CharCodeFormat
						) ;
						DestSize += WriteBytes ;
					}
					break ;

				case PRINTF_TYPE_Z :
					// ��Ή�
					break ;
				}
			}
		}
	}

	// �I�[�������Z�b�g
	DestSize += PutCharCode_inline( 0, CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;

	if( TempBuffer != NULL )
	{
		free( TempBuffer ) ;
		TempBuffer = NULL ;
	}

	return 0 ;
}

extern int CL_sprintf( int CharCodeFormat, char *Buffer, const char *FormatString, ... )
{
	int Result ;
	va_list VaList ;

	va_start( VaList, FormatString ) ;

	Result = CL_vsprintf( CharCodeFormat, Buffer, FormatString, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

extern char *CL_itoa( int CharCodeFormat, int Value, char *Buffer, int Radix )
{
	int i ;
	u8 Number[ 512 ] ;
	int NumberNum ;
	int DestSize ;

	DestSize = 0 ;

	// ���l�� 0 �̏ꍇ�� 0 �̂݃Z�b�g
	if( Value == 0 )
	{
		DestSize += PutCharCode_inline( '0', CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;
	}
	else
	{
		// ���l���}�C�i�X�̏ꍇ�� - �L����ǉ�������Œl���v���X�ɂ���
		if( Value < 0 )
		{
			DestSize += PutCharCode_inline( '-', CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;
			Value = -Value ;
		}

		// �e���̐��l�ɕϊ�
		for( NumberNum = 0 ; Value != 0 ; NumberNum ++, Value /= Radix )
		{
			Number[ NumberNum ] = Value % Radix ;
		}

		// ���l�𕶎���
		for( i = NumberNum - 1 ; i >= 0 ; i -- )
		{
			DestSize += PutCharCode_inline( Number[ i ] <= 9 ? '0' + Number[ i ] : 'a' + Number[ i ] - 10, CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;
		}
	}

	// �I�[�������Z�b�g
	DestSize += PutCharCode_inline( 0, CharCodeFormat, ( char * )&( ( u8 * )Buffer )[ DestSize ] ) ;
	
	return Buffer ;
}

extern int CL_atoi( int CharCodeFormat, const char *Str )
{
	int i ;
	int AddNum ;
	int NumCharNum ;
	int Number[ 256 ] ;
	int Total = 0 ;
	int Minus ;
	u32 BaseBuffer[ 1024 ] ;
	u32 *TempBuffer = NULL ;
	u32 *FCode ;
	int StringSize ;

	// NULL �̃A�h���X���n���ꂽ�� 0 ��Ԃ�
	if( Str == NULL )
	{
		return 0 ;
	}

	// �P�����S�o�C�g�`���ɕϊ�����
	StringSize = StringToCharCodeString_inline( Str, CharCodeFormat, NULL ) ;
	if( StringSize + sizeof( u32 ) * 16 > sizeof( BaseBuffer ) )
	{
		TempBuffer = ( u32 * )malloc( StringSize + sizeof( u32 ) * 16 ) ;
		if( TempBuffer == NULL )
		{
			return -1 ;
		}

		FCode = TempBuffer ;
	}
	else
	{
		FCode = BaseBuffer ;
	}
	memset( FCode, 0, StringSize + sizeof( u32 ) * 16 ) ;
	StringToCharCodeString_inline( Str, CharCodeFormat, FCode ) ;

	// ������T��
	while( *FCode != '\0' )
	{
		if( ( *FCode >= '0' && *FCode <= '9' ) || *FCode == '-' )
		{
			break ;
		}
		FCode ++ ;
	}

	// �����Ȃ�I�[�����̏ꍇ�̓G���[�Ȃ̂� 0 ��Ԃ�
	if( *FCode == '\0' )
	{
		goto END ;
	}

	// �ŏ��̕������}�C�i�X�L���̏ꍇ�̓}�C�i�X�t���O�𗧂Ă�
	if( *FCode == '-' )
	{
		Minus = 1 ;
		FCode ++ ;
	}
	else
	{
		Minus = 0 ;
	}
	
	// �������擾
	for( NumCharNum = 0 ; *FCode != '\0' && ( *FCode >= '0' && *FCode <= '9' ) ; NumCharNum ++, FCode ++ )
	{
		Number[ NumCharNum ] = *FCode - '0' ;
	}
	
	// ���l�ɕϊ�
	AddNum = 1 ;
	Total  = 0 ;
	for( i = NumCharNum - 1 ; i >= 0 ; i --, AddNum *= 10 )
	{
		Total += Number[ i ] * AddNum ;
	}

	// �}�C�i�X�t���O�������Ă�����}�C�i�X�l�ɂ���
	if( Minus == 1 )
	{
		Total = -Total ;
	}

END :

	if( TempBuffer != NULL )
	{
		free( TempBuffer ) ;
		TempBuffer = NULL ;
	}

	// ���l��Ԃ�
	return Total ;
}

extern double CL_atof( int CharCodeFormat, const char *Str )
{
	int MinusFlag ;
	int DotFlag ;
	int IndexFlag ;
	int IndexMinusFlag ;
	int i ;
	u32 IntNumStr[ 256 ] ;
	u32 FloatNumStr[ 256 ] ;
	u32 IndexNumStr[ 256 ] ;
	int IntNumberNum ;
	int FloatNumberNum ;
	int IndexNumberNum ;
	double IntNum ;
	double FloatNum ;
	double IndexNum ;
	double AddNum ;
	s64 int64Num, int64Count ;
	u32 BaseBuffer[ 1024 ] ;
	u32 *TempBuffer = NULL ;
	u32 *FCode ;
	int StringSize ;
	double Result = 0.0 ;

	MinusFlag      = 0 ;
	DotFlag        = 0 ;
	IndexFlag      = 0 ;
	IndexMinusFlag = 0 ;

	// NULL �̃A�h���X���n���ꂽ�� 0.0 ��Ԃ�
	if( Str == NULL )
	{
		return 0.0 ;
	}

	// �P�����S�o�C�g�`���ɕϊ�����
	StringSize = StringToCharCodeString_inline( Str, CharCodeFormat, NULL ) ;
	if( StringSize + sizeof( u32 ) * 16 > sizeof( BaseBuffer ) )
	{
		TempBuffer = ( u32 * )malloc( StringSize + sizeof( u32 ) * 16 ) ;
		if( TempBuffer == NULL )
		{
			return -1.0 ;
		}

		FCode = TempBuffer ;
	}
	else
	{
		FCode = BaseBuffer ;
	}
	memset( FCode, 0, StringSize + sizeof( u32 ) * 16 ) ;
	StringToCharCodeString_inline( Str, CharCodeFormat, FCode ) ;

	// �����Ȃ�I�[�����̏ꍇ�� 0.0 ��Ԃ�
	if( *FCode == '\0' )
	{
		goto END ;
	}

	// �}�C�i�X�L�����ŏ��ɂ�������}�C�i�X�t���O�𗧂Ă�
	if( *FCode == '-' )
	{
		FCode ++ ;

		// �}�C�i�X�L���̒���ɏI�[�����̏ꍇ�� 0.0 ��Ԃ�
		if( *FCode == '\0' )
		{
			goto END ;
		}
		MinusFlag = 1 ;
	}

	IntNumberNum   = 0 ;
	FloatNumberNum = 0 ;
	IndexNumberNum = 0 ;
	for( ; *FCode != '\0' ; FCode ++ )
	{
		// �����_�̏���
		if( *FCode == '.' )
		{
			// ���ɏ����_����������G���[�Ȃ̂� 0.0 ��Ԃ�
			if( DotFlag )
			{
				goto END ;
			}

			// �_�t���O�𗧂Ă�
			DotFlag = 1 ;
		}
		else
		// �w���l�w�蕶������
		if( *FCode == 'e' || *FCode == 'E' )
		{
			// ���Ɏw���l�w�肪��������G���[�Ȃ̂� 0.0 ��Ԃ�
			if( IndexFlag )
			{
				goto END ;
			}

			// �w���l�w��t���O�𗧂Ă�
			IndexFlag = 1 ;
		}
		else
		// �w���l�w��p�̃}�C�i�X�L�����̓v���X�L���̏���
		if( *FCode == '-' || *FCode == '+' )
		{
			// �w���l�w�肪���������Ȃ� - �� + ���������ꍇ��A
			// e �� E �̕����̒���� - �� + �����������ꍇ�̓G���[�Ȃ̂� 0.0 ��Ԃ�
			if( IndexFlag == 0 || IndexNumberNum != 0 )
			{
				goto END ;
			}

			// �}�C�i�X�L���̏ꍇ�͎w���l���}�C�i�X�̃t���O�𗧂Ă�
			if( *FCode == '-' )
			{
				IndexMinusFlag = 1 ;
			}
		}
		else
		// �����̏���
		if( *FCode >= '0' && *FCode <= '9' )
		{
			// �w���l�w��̌�̏ꍇ�͎w���l
			if( IndexFlag )
			{
				// 256 �����ȏ�w���l�̕����񂪑�������G���[�Ȃ̂� 0 ��Ԃ�
				if( IndexNumberNum >= 255 )
				{
					goto END ;
				}
				IndexNumStr[ IndexNumberNum ] = *FCode - '0';
				IndexNumberNum ++ ;
			}
			else
			if( DotFlag )
			{
				// 256 �����ȏ㏬���_�ȉ��̒l����������G���[�Ȃ̂� 0 ��Ԃ�
				if( FloatNumberNum >= 255 )
				{
					goto END ;
				}
				FloatNumStr[ FloatNumberNum ] = *FCode - '0';
				FloatNumberNum ++ ;
			}
			else
			{
				// 256 �����ȏ㐮���l�̕����񂪑�������G���[�Ȃ̂� 0 ��Ԃ�
				if( IntNumberNum >= 255 )
				{
					goto END ;
				}
				IntNumStr[ IntNumberNum ] = *FCode - '0';
				IntNumberNum ++ ;
			}
		}
		else
		// ��L�ȊO�̕����������ꍇ�̓G���[�Ȃ̂� 0.0 ��Ԃ�
		{
			goto END ;
		}
	}

	// ����������������������̒����O�������ꍇ�̓G���[�Ȃ̂� 0.0 ��Ԃ�
	if( IntNumberNum == 0 && FloatNumberNum == 0 )
	{
		goto END ;
	}

	// �����l���Z�o
	AddNum = 1.0 ;
	IntNum = 0 ;
	for( i = IntNumberNum - 1 ; i >= 0; i --, AddNum *= 10.0 )
	{
		if( IntNumStr[ i ] != 0 )
		{
			IntNum += IntNumStr[ i ] * AddNum ;
		}
	}
	if( MinusFlag )
	{
		IntNum = -IntNum ;
	}

	// �����l���Z�o
	AddNum   = 0.1 ;
	FloatNum = 0 ;
	for( i = 0 ; i < FloatNumberNum ; i ++, AddNum /= 10.0 )
	{
		if( FloatNumStr[ i ] != 0 )
		{
			FloatNum += FloatNumStr[i] * AddNum ;
		}
	}
	if( MinusFlag )
	{
		FloatNum = -FloatNum ;
	}

	// �w���l������𐔒l��
	int64Count = 1 ;
	int64Num   = 0 ;
	for( i = IndexNumberNum - 1; i >= 0; i --, int64Count *= 10 )
	{
		int64Num += IndexNumStr[ i ] * int64Count ;
	}
	if( IndexMinusFlag )
	{
		int64Num = -int64Num ;
	}

	// �w���l�ɏ]���ď�Z�l������
	IndexNum = 1.0 ;
	if( int64Num != 0 )
	{
		if( int64Num < 0 )
		{
			int64Num = -int64Num ;
			for( i = 0 ; i < int64Num ; i ++ )
			{
				IndexNum /= 10.0 ;
			}
		}
		else
		{
			for( i = 0 ; i < int64Num ; i++ )
			{
				IndexNum *= 10.0 ;
			}
		}
	}

	// �߂�l���Z�o
	Result = ( IntNum + FloatNum ) * IndexNum ;

END :

	if( TempBuffer != NULL )
	{
		free( TempBuffer ) ;
		TempBuffer = NULL ;
	}

	return Result ;
}

static int CL_vsscanf_skipspace( u32 *String )
{
	int AddSize = 0 ;

	for( AddSize = 0 ;
		String[ AddSize ] == ' '  || 
		String[ AddSize ] == '\n' ||
		String[ AddSize ] == '\r' ||
		String[ AddSize ] == '\t' ;
		AddSize ++ ){}

	return AddSize ;
}

extern int CL_vsscanf( int CharCodeFormat, const char *String, const char *FormatString, va_list Arg )
{
	u32 c ;
	u32 Number[ 256 ] ;
	u32 VStr[ 1024 ] ;
	int ReadNum = 0 ;
	int Width ;
	int i, j, k ;
	int num, num2 ;
	int SkipFlag ;
	int VStrRFlag ;
	int I64Flag ;
	int lFlag ;
	int hFlag ;
	int Eof ;
	int MinusFlag ;
	int UIntFlag ;
	s64 int64Num ;
	s64 int64Count ;
	int UseCharNum ;
	u8 *pStr ;
	int UnitSize ;
	int CharBytes ;
	u8 TempBuffer[ 64 ] ;

	u32 FormatStringBaseBuffer[ 1024 ] ;
	u32 *FormatStringTempBuffer = NULL ;
	int FormatStringSize ;
	u32 *FCode ;

	u32 StringBaseBuffer[ 1024 ] ;
	u32 *StringTempBuffer = NULL ;
	int StringSize ;
	u32 *SCode ;

	UnitSize = GetCharCodeFormatUnitSize_inline( CharCodeFormat ) ;

	// �P�����S�o�C�g�`���ɕϊ�����
	{
		StringSize = StringToCharCodeString_inline( String, CharCodeFormat, NULL ) ;
		if( StringSize + sizeof( u32 ) * 16 > sizeof( StringBaseBuffer ) )
		{
			StringTempBuffer = ( u32 * )malloc( StringSize + sizeof( u32 ) * 16 ) ;
			if( StringTempBuffer == NULL )
			{
				return 0 ;
			}

			SCode = StringTempBuffer ;
		}
		else
		{
			SCode = StringBaseBuffer ;
		}
		memset( SCode, 0, StringSize + sizeof( u32 ) * 16 ) ;
		StringToCharCodeString_inline( String, CharCodeFormat, SCode ) ;


		FormatStringSize = StringToCharCodeString_inline( FormatString, CharCodeFormat, NULL ) ;
		if( FormatStringSize + sizeof( u32 ) * 16 > sizeof( FormatStringBaseBuffer ) )
		{
			FormatStringTempBuffer = ( u32 * )malloc( FormatStringSize + sizeof( u32 ) * 16 ) ;
			if( FormatStringTempBuffer == NULL )
			{
				if( StringTempBuffer != NULL )
				{
					free( StringTempBuffer ) ;
					StringTempBuffer = NULL ;
				}
				return 0 ;
			}

			FCode = FormatStringTempBuffer ;
		}
		else
		{
			FCode = FormatStringBaseBuffer ;
		}
		memset( FCode, 0, FormatStringSize + sizeof( u32 ) * 16 ) ;
		StringToCharCodeString_inline( FormatString, CharCodeFormat, FCode ) ;
	}

	// �����Ȃ�I�[�����̏ꍇ�� 0 ��Ԃ�
	if( *FCode == '\0' )
	{
		goto END ;
	}

	ReadNum = 0;
	Eof = FALSE ;
	while( *FCode != '\0' )
	{
		// % ���ǂ����ŏ����𕪊�
		if( FCode[ 0 ] == '%' && FCode[ 1 ] != '%' )
		{
			Width    = -1;
			I64Flag  = FALSE;
			lFlag    = FALSE;
			hFlag    = FALSE;
			UIntFlag = FALSE;
			SkipFlag = FALSE;

			FCode ++ ;
			if( *FCode == '\0' )
			{
				break;
			}

			if( *FCode == '*' )
			{
				SkipFlag = TRUE;
				FCode ++ ;
				if( *FCode == '\0' )
				{
					break;
				}
			}

			if( *FCode >= '0' && *FCode <= '9' )
			{
				Width = ( int )CL_vsprintf_help_getnumber( FCode, &UseCharNum ) ;
				FCode += UseCharNum ;
				if( *FCode == '\0' )
				{
					break;
				}
				if( Width == 0 )
				{
					break;
				}
			}

			switch( *FCode )
			{
			case 'l':
			case 'L':
				lFlag = TRUE ;
				FCode++ ;
				break ;

			case 'h':
			case 'H':
				hFlag = TRUE ;
				FCode++ ;
				break ;

			case 'I':
				if( FCode[ 1 ] == '6' && FCode[ 2 ] == '4' )
				{
					I64Flag = TRUE;
					FCode += 3;
				}
				break;
			}
			if( *FCode == '\0' )
			{
				break;
			}

			if( *FCode == '[' )
			{
				if( lFlag || hFlag || I64Flag )
				{
					break;
				}

				FCode ++ ;
				VStrRFlag = FALSE ;
				if( *FCode == '^' )
				{
					VStrRFlag = TRUE;
					FCode++;
				}
				j = 0;
				memset( VStr, 0, sizeof( VStr ) ) ;

				c = '[';
				while( *FCode != '\0' && *FCode != ']' )
				{
					if( *FCode == '-' && c != '[' && FCode[ 1 ] != '\0' && FCode[ 1 ] != ']' )
					{
						num  = ( int )( u8 )c ;
						num2 = ( int )( u8 )FCode[ 1 ] ;
						if( num2 < num )
						{
							k    = num2 ;
							num2 = num ;
							num  = k ;
						}
						for( k = num; k <= num2; k++ )
						{
							if( c != k )
							{
								*( ( u8 *)&VStr[ j ] ) = ( u8 )k ;
								j++ ;
							}
						}
						FCode += 2 ;
						c = '[' ;
					}
					else
					{
						VStr[ j ] = *FCode ;
						c = *FCode ;
						j ++ ;
						FCode ++ ;
					}
				}
				if( *FCode == '\0' )
				{
					break;
				}

				FCode ++ ;
				pStr = NULL;
				if( SkipFlag == FALSE )
				{
					pStr = va_arg( Arg, u8 * ) ;
				}

				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				i = 0;
				for(;;)
				{
					if( *SCode == 0 )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
					j = 0 ;
					while( VStr[ j ] != '\0' )
					{
						if( VStr[ j ] == c )
						{
							break ;
						}
						j ++ ;
					}

					if( ( VStrRFlag == TRUE  && VStr[ j ] != '\0' ) ||
						( VStrRFlag == FALSE && VStr[ j ] == '\0' ) )
					{
						break ;
					}

					if( pStr )
					{
						pStr += PutCharCode_inline( c, CharCodeFormat, ( char * )pStr ) ;
					}
					i ++ ;
					if( Width != 0 && Width == i )
					{
						break ;
					}
				}

				if( pStr )
				{
					pStr += PutCharCode_inline( '\0', CharCodeFormat, ( char * )pStr ) ;
				}

				if( Eof == FALSE && Width != i )
				{
					SCode -- ;
				}
			}
			else
			if( *FCode == 'd' || *FCode == 'D' || *FCode == 'u' || *FCode == 'U' )
			{
				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				FCode ++ ;
				UIntFlag  = *FCode == 'u' || *FCode == 'U' ;
				MinusFlag = FALSE ;
				c = *SCode ;
				SCode ++ ;
				if( c == '-' )
				{
					MinusFlag = TRUE;
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break;
						}
					}
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
STR_10INT :
				for( i = 0; i < 255 && ( Width == -1 || Width != i ) && c >= '0' && c <= '9'; i ++ )
				{
					Number[ i ] = c - '0';
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
				if( Eof == FALSE )
				{
					SCode -- ;
				}
				num = i ;
				if( num == 0 )
				{
					break ;
				}
				int64Count = 1 ;
				int64Num   = 0 ;
				for( i = num - 1 ; i >= 0 ; i --, int64Count *= 10 )
				{
					if( UIntFlag )
					{
						int64Num = ( s64 )( ( u64 )int64Num + ( u64 )Number[ i ] * ( u64 )int64Count ) ;
					}
					else
					{
						int64Num += ( s64 )Number[i] * int64Count ;
					}
				}
				if( MinusFlag )
				{
					int64Num = -int64Num ;
				}
				if( SkipFlag == FALSE )
				{
					if( I64Flag )
					{
						if( UIntFlag )
						{
							*va_arg( Arg, u64 * ) = ( u64 )int64Num ;
						}
						else
						{
							*va_arg( Arg, s64 * ) = ( s64 )int64Num ;
						}
					}
					else if( hFlag )
					{
						if( UIntFlag )
						{
							*va_arg( Arg, u16 * ) = ( u16 )int64Num ;
						}
						else
						{
							*va_arg( Arg, short * ) = ( short )int64Num ;
						}
					}
					else
					{
						if( UIntFlag )
						{
							*va_arg( Arg, u32 * ) = ( u32 )int64Num ;
						}
						else
						{
							*va_arg( Arg, int * ) = ( int )int64Num ;
						}
					}
				}
			}
			else
			if( *FCode == 'x' || *FCode == 'X' )
			{
				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				FCode ++ ;
				MinusFlag = FALSE ;
				c = *SCode ;
				SCode ++ ;
				if( c == '-' )
				{
					MinusFlag = TRUE ;
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break ;
						}
					}
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
STR_16INT :
				i = 0;
				for(;;)
				{
					if( i >= 255 )
					{
						break ;
					}

					if( Width != -1 && Width == i )
					{
						break ;
					}
					else
					if( c >= '0' && c <= '9' )
					{
						Number[ i ] = c - '0' ;
					}
					else
					if( c >= 'a' && c <= 'f' )
					{
						Number[ i ] = c - 'a' + 10 ;
					}
					else
					if( c >= 'A' && c <= 'F' )
					{
						Number[ i ] = c - 'A' + 10 ;
					}
					else
					{
						break;
					}

					i ++ ;
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
				if( Eof == FALSE )
				{
					SCode -- ;
				}
				num = i ;
				if( num == 0 )
				{
					break ;
				}
				int64Count = 1 ;
				int64Num   = 0 ;
				for( i = num - 1; i >= 0; i --, int64Count *= 16 )
				{
					int64Num = ( s64 )( ( u64 )int64Num + ( u64 )Number[ i ] * ( u64 )int64Count ) ;
				}
				if( MinusFlag )
				{
					int64Num = -int64Num ;
				}
				if( SkipFlag == FALSE )
				{
					if( I64Flag )
					{
						*va_arg( Arg, u64 * ) = ( u64 )int64Num ;
					}
					else
					if( hFlag )
					{
						*va_arg( Arg, u16 * ) = ( u16 )int64Num ;
					}
					else
					{
						*va_arg( Arg, u32 * ) = ( u32 )int64Num ;
					}
				}
			}
			else
			if( *FCode == 'o' || *FCode == 'O' )
			{
				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				FCode ++ ;
				MinusFlag = FALSE;
				c = *SCode ;
				SCode ++ ;
				if( c == '-' )
				{
					MinusFlag = TRUE ;
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break ;
						}
					}
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
STR_8INT:
				for( i = 0; i < 255 && ( Width == -1 || Width != i ) && c >= '0' && c <= '7'; i ++ )
				{
					Number[ i ] = c - '0';
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
				if( Eof == FALSE )
				{
					SCode -- ;
				}
				num = i ;
				if( num == 0 )
				{
					break ;
				}
				int64Count = 1 ;
				int64Num   = 0 ;
				for( i = num - 1; i >= 0; i --, int64Count *= 8 )
				{
					int64Num = ( s64 )( ( u64 )int64Num + ( u64 )Number[ i ] * ( u64 )int64Count ) ;
				}
				if( MinusFlag )
				{
					int64Num = -int64Num ;
				}
				if( SkipFlag == FALSE )
				{
					if( I64Flag )
					{
						*va_arg( Arg, u64 * ) = ( u64 )int64Num ;
					}
					else
					if( hFlag )
					{
						*va_arg( Arg, u16 * ) = ( u16 )int64Num ;
					}
					else
					{
						*va_arg( Arg, u32 * ) = ( u32 )int64Num ;
					}
				}
			}
			else
			if( *FCode == 'i' || *FCode == 'I' )
			{
				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				FCode ++ ;
				MinusFlag = FALSE;
				c = *SCode ;
				SCode ++ ;
				if( c == '-' )
				{
					MinusFlag = TRUE;
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break ;
						}
					}
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}
				if( c == '0' )
				{
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break ;
						}
					}
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
					if( c == 'x' )
					{
						if( Width != -1 )
						{
							Width -- ;
							if( Width == 0 )
							{
								break ;
							}
						}
						if( *SCode == '\0' )
						{
							Eof = TRUE ;
							break ;
						}
						c = *SCode ;
						SCode ++ ;
						goto STR_16INT ;
					}
					else
					{
						goto STR_8INT ;
					}
				}
				else
				{
					goto STR_10INT ;
				}
			}
			else
			if( *FCode == 'c' || *FCode == 'C' )
			{
				FCode ++ ;
				if( Width == -1 )
				{
					Width = 1 ;
				}
				pStr = NULL ;
				if( SkipFlag == FALSE )
				{
					pStr = va_arg( Arg, u8 * ) ;
				}
				for( i = 0; i < Width; i += CharBytes / UnitSize )
				{
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;

					if( pStr )
					{
						CharBytes = PutCharCode_inline( c, CharCodeFormat, ( char * )pStr ) ;
						pStr += CharBytes ;
					}
					else
					{
						CharBytes = PutCharCode_inline( c, CharCodeFormat, ( char * )TempBuffer ) ;
					}
				}
			}
			else
			if( *FCode == 's' || *FCode == 'S' )
			{
				FCode ++ ;
				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				pStr = NULL ;
				if( SkipFlag == FALSE )
				{
					pStr = va_arg( Arg, u8 * ) ;
				}

				i = 0 ;
				for(;;)
				{
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;

					if( c == ' ' || c == '\n' || c == '\r' || c == '\t' )
					{
						SCode -- ;
						break;
					}

					if( pStr )
					{
						CharBytes = PutCharCode_inline( c, CharCodeFormat, ( char * )pStr ) ;
						pStr += CharBytes ;
					}
					else
					{
						CharBytes = PutCharCode_inline( c, CharCodeFormat, ( char * )TempBuffer ) ;
					}

					i += CharBytes / UnitSize ;
					if( Width != -1 && i >= Width )
					{
						break ;
					}
				}

				if( pStr )
				{
					pStr += PutCharCode_inline( '\0', CharCodeFormat, ( char * )pStr ) ;
				}
			}
			else
			if( *FCode == 'f' || *FCode == 'F' || *FCode == 'g' || *FCode == 'G' || *FCode == 'e' || *FCode == 'E' )
			{
				int num3 ;
				u32 Number2[ 256 ] ;
				u32 Number3[ 256 ] ;
				double doubleNum ;
				double doubleNum2 ;
				double doubleNum3 ;
				double doubleCount ;
				int DotFlag ;
				int IndexFlag ;
				int MinusFlag2 ;

				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}

				FCode ++ ;
				MinusFlag  = FALSE ;
				DotFlag    = FALSE ;
				IndexFlag  = FALSE ;
				MinusFlag2 = FALSE ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break ;
				}
				c = *SCode ;
				SCode ++ ;
				if( c == '-' )
				{
					MinusFlag = TRUE ;
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break ;
						}
					}
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
				}

				i = 0 ;	// ���R��
				j = 0 ;	// ����
				k = 0 ;	// �w��
				for(;;)
				{
					if( c == '.' )
					{
						if( DotFlag )
						{
							break ;
						}
						DotFlag = TRUE ;
					}
					else
					if( c == 'e' || c == 'E' )
					{
						if( IndexFlag )
						{
							break ;
						}
						IndexFlag = TRUE ;
					}
					else
					if( c == '-' || c == '+' )
					{
						if( IndexFlag == FALSE || k != 0 )
						{
							break ;
						}
						if( c == '-' )
						{
							MinusFlag2 = TRUE ;
						}
					}
					else
					if( c >= '0' && c <= '9' )
					{
						if( IndexFlag )
						{
							if( k >= 255 )
							{
								break ;
							}
							Number3[ k ] = c - '0' ;
							k ++ ;
						}
						else
						if( DotFlag )
						{
							if( j >= 255 )
							{
								break ;
							}
							Number2[ j ] = c - '0' ;
							j ++ ;
						}
						else
						{
							if( i >= 255 )
							{
								break ;
							}
							Number[ i ] = c - '0';
							i ++ ;
						}
					}
					else
					{
						break;
					}

					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
					if( Width != -1 )
					{
						Width -- ;
						if( Width == 0 )
						{
							break ;
						}
					}
				}
				if( Eof == FALSE )
				{
					SCode -- ;
				}
				if( i == 0 && j == 0 )
				{
					break ;
				}
				num  = i ;
				num2 = j ;
				num3 = k ;
				if( num == 0 && num2 == 0 )
				{
					break ;
				}

				doubleCount = 1.0 ;
				doubleNum   = 0 ;
				for( i = num - 1; i >= 0; i --, doubleCount *= 10.0 )
				{
					if( Number[ i ] != 0 )
					{
						doubleNum += Number[ i ] * doubleCount ;
					}
				}
				if( MinusFlag )
				{
					doubleNum = -doubleNum ;
				}

				doubleCount = 0.1 ;
				doubleNum2  = 0 ;
				for( i = 0; i < num2; i ++, doubleCount /= 10.0 )
				{
					if( Number2[ i ] != 0 )
					{
						doubleNum2 += Number2[ i ] * doubleCount ;
					}
				}
				if( MinusFlag )
				{
					doubleNum2 = -doubleNum2 ;
				}

				int64Count = 1 ;
				int64Num   = 0 ;
				for( i = num3 - 1; i >= 0; i --, int64Count *= 10 )
				{
					int64Num += Number3[ i ] * int64Count ;
				}
				if( MinusFlag2 )
				{
					int64Num = -int64Num ;
				}

				doubleNum3 = 1.0 ;
				if( int64Num != 0 )
				{
					if( int64Num < 0 )
					{
						int64Num = -int64Num ;
						for( i = 0; i < int64Num; i++ )
						{
							doubleNum3 /= 10.0 ;
						}
					}
					else
					{
						for( i = 0; i < int64Num; i++ )
						{
							doubleNum3 *= 10.0 ;
						}
					}
				}

				doubleNum = ( doubleNum + doubleNum2 ) * doubleNum3 ;

				if( SkipFlag == FALSE )
				{
					if( lFlag )
					{
						*va_arg( Arg, double * ) =          doubleNum ;
					}
					else
					{
						*va_arg( Arg, float *  ) = ( float )doubleNum ;
					}
				}
			}
			if( SkipFlag == FALSE )
			{
				ReadNum ++ ;
			}
		}
		else
		{
			if( *FCode == ' ' || *FCode == '\n' || *FCode == '\r' || *FCode == '\t' )
			{
				while( *FCode != '\0' && ( *FCode == ' ' || *FCode == '\n' || *FCode == '\r' || *FCode == '\t' ) )
				{
					FCode ++ ;
				}
				SCode += CL_vsscanf_skipspace( SCode ) ;
				if( *SCode == '\0' )
				{
					Eof = TRUE ;
					break;
				}
			}
			else
			{
				u32 str[ 256 ] ;

				for( num = 0; *FCode != '\0' && *FCode != ' ' && *FCode != '\n' && *FCode != '\r' && *FCode != '\t' && *FCode != '%'; num++, FCode ++ )
				{
					str[ num ] = *FCode ;
				}
				str[ num ] = '\0' ;

				for( i = 0; i < num; i ++ )
				{
					if( *SCode == '\0' )
					{
						Eof = TRUE ;
						break ;
					}
					c = *SCode ;
					SCode ++ ;
					if( str[ i ] != c )
					{
						break ;
					}
				}
			}
		}
	}

END :

	if( FormatStringTempBuffer != NULL )
	{
		free( FormatStringTempBuffer ) ;
		FormatStringTempBuffer = NULL ;
	}

	if( StringTempBuffer != NULL )
	{
		free( StringTempBuffer ) ;
		StringTempBuffer = NULL ;
	}

	return ReadNum;
}

extern int CL_sscanf( int CharCodeFormat, const char *String, const char *FormatString, ... )
{
	int Result ;
	va_list VaList ;

	va_start( VaList, FormatString ) ;

	Result = CL_vsscanf( CharCodeFormat, String, FormatString, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}






































