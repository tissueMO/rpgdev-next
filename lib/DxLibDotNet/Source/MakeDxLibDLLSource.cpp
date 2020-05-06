#include <stdio.h>
#include "TextAnalyse.h"
#include <windows.h>
#include <string.h>
#include <malloc.h>
#include <varargs.h>

// ������ǂݍ��ݗp�N���X�I�u�W�F�N�g
TextAnalyse dhtxt, dfhtxt, dxtxt, ttxt;

// �֐������
struct FUNCTIONNAME
{
	int				Counter;				// �����֐��̐�
	char			Name[ 64 ];				// �֐���
} ;

// ���C���f�[�^
struct MAIN
{
	char			*Struct;				// DxLib.h �̍\���̂���`����Ă��镔���̕�����
	char			*Function;				// DxLib.h �̊֐�����`����Ă��镔���̕�����

	int				FuncNameNum ;			// �֐����̐�
	FUNCTIONNAME	FuncName[ 65536 ] ;		// �֐������
} dat;

// �֐��̈����Ɋւ�����
struct PARAMETER
{
	bool			ValidDefault;			// �f�t�H���g�������L�����ǂ���
	bool			Out;					// out �������ǂ���
	bool			Array;					// �z�� In/Out �������ǂ���
	char			Default[64];			// �f�t�H���g�����̕�����
	char			CPPType[64];			// C++ �ł̌^
	char			CSType[64];				// C# �ł̌^
	char			CSTypeName[64];			// C# �ł̌^��
	char			Name[64];				// ������
};

// �֐��Ɋւ�����
struct FUNCTION
{
	bool			Void;					// �߂�l�̌^�� void �^���ǂ���
	char			CPPType[64];			// C++ �ł̖߂�l�̌^
	char			CSType[64];				// C# �ł̖߂�l�̌^
	char			CSTypeName[64];			// C# �ł̖߂�l�̌^��
	char			Name[64];				// �֐���
	
	PARAMETER		Parameter[64];			// �����̏��
	int				ParameterNum;			// �����̐�
	int				DefaultNum;				// �f�t�H���g�����̐�
};

// �^�ϊ��Ŏg�p������
struct TYPEINFO
{
	const char		*CPPName ;				// C++ �ł̌^��
	const char		*CSName ;				// C# �ł̌^��
	int				Size ;					// �^�̃T�C�Y( byte�� )
} ;


// �^�̕ϊ����
TYPEINFO TypeInfoDim[] =
{
	"void",						NULL,					0,
	"HWND",						"System.IntPtr",		4,
	"HANDLE",					"System.IntPtr",		4,
	"HICON",					"System.IntPtr",		4,
	"D_IDirect3DDevice9",		"System.IntPtr",		4,
	"D_IDirect3DSurface9",		"System.IntPtr",		4,
	"DX_CHAR",					"byte",					1,
	"WORD",						"ushort",				2,
	"BYTE",						"byte",					1,
	"LONGLONG",					"long",					8,
	"ULONGLONG",				"ulong",				8,
	"size_t",					"ulong",				8,
	"int",						NULL,					4,
	"short",					NULL,					2,
	"char",						NULL,					1,
	"TCHAR",					"char",					1,
	"float",					NULL,					4,
	"double",					NULL,					8,
	"VECTOR",					NULL,					12,
	"VECTOR_D",					NULL,					24,
	"MATRIX",					NULL,					64,
	"MATRIX_D",					NULL,					128,
	"RECT",						NULL,					16,
	"DATEDATA",					NULL,					24,
	"IPDATA",					NULL,					4,
	"IPDATA_IPv6",				NULL,					16,
	"COLOR_F",					NULL,					16,
	"FLOAT4",					NULL,					16,
	"DOUBLE4",					NULL,					32,
	"COLOR_U8",					NULL,					4,
	"FLOAT2",					NULL,					8,
	"BOOL",						"int",					4,
	"COLORDATA",				NULL,					1064,
	"IMEINPUTCLAUSEDATA",		NULL,					8,
	"HITRESULT_LINE",			NULL,					16,
	"MV1_COLL_RESULT_POLY",		NULL,					100,
	"MV1_COLL_RESULT_POLY_DIM",	NULL,					8,
	"MV1_REF_VERTEX",			NULL,					52,
	"MV1_REF_POLYGON",			NULL,					44,
	"MV1_REF_POLYGONLIST",		NULL,					48,
	"DISPLAYMODEDATA",			NULL,					16,
	NULL, NULL, -1,
} ;

// DLL �ɂ̂ݏo�͂��� C# �p�ɂ͏o�͂��Ȃ��֐��̃��X�g
char *DLLOnlyFunction[] =
{
	"ClearDrawScreen", "ClearDrawScreenZBuffer",
	"GetTexPixelFormat", "GetTexColorData", "LoadGraphToResource", "GetWindowSizeChangeEnableFlag",
	"DrawChipMap", "BltBaseImage", "CreateGraphFromGraphImage",
	"ReCreateGraphFromGraphImage", "CreateDivGraphFromGraphImage", "ReCreateDivGraphFromGraphImage",
	"MemStreamOpen", "MemStreamClose", "vsprintfDx", "vsnprintfDx", "vsscanfDx",
	"GetDrawTargetSurface", "GetPrimarySurface", "GetBackSurface",
	"GetWorkSurface", "GetUseDDrawObj", "GetPixelFormat",
	"GetOverlayPixelFormat", "GetDirectDrawCaps", "GetDrawScreenDC",
	"GetDrawStringCharInfo", "GetDrawExtendStringCharInfo",
	"GetDrawStringCharInfoToHandle", "GetDrawExtendStringCharInfoToHandle",
	"GetDrawNStringCharInfo", "GetDrawExtendNStringCharInfo",
	"GetDrawNStringCharInfoToHandle", "GetDrawExtendNStringCharInfoToHandle",
	"GetDirectDrawDeviceGUID", "GetUseD3DDevObj", "GetVertexBuffer",
	"GetTexPixelFormat", "GetTexColorData", "GetTexPixelFormat",
	"GetTexColorData", "GetTexPixelFormat", "GetTexColorData",
	"GetZBufferPixelFormat", "GraphColorMatchBltVer2", "GetFullColorImage",
	"GetResourceIDString", "CreateDIBGraphVer2", "SetHookWinProc",
	"FileRead_getInfo", "FileRead_findFirst", "FileRead_findFirst_WithStrLen", "FileRead_findNext", "FileRead_findClose",
	NULL,
};

// ��؏o�͂��Ȃ��֐��̃��X�g
char *SkipFunction[] =
{
	"GraphFilter", "GraphFilterBlt", "GraphFilterRectBlt",
	"GraphBlend", "GraphBlendBlt", "GraphBlendRectBlt",
	"MV1SetMaterialTypeParamAll", "MV1SetMaterialTypeParam",
	"sprintfDx", "snprintfDx", "sscanfDx",
	"SetBlendGraphParam", "MailApp_Send", "MailApp_SendWithStrLen",
	"SetBeepFrequency", "PlayBeep", "StopBeep",
	"LogFileFmtAdd", "ErrorLogFmtAdd", "AppLogAdd", "printfDx", "FileRead_scanf", 
	"DrawFormatString", "DrawFormatVString", "DrawFormatStringToHandle", "DrawFormatVStringToHandle",
	"DrawExtendFormatString", "DrawExtendFormatVString", "DrawExtendFormatStringToHandle", "DrawExtendFormatVStringToHandle",
	"DrawRotaFormatString", "DrawRotaFormatStringF", "DrawRotaFormatStringToHandle", "DrawRotaFormatStringFToHandle",
	"DrawModiFormatString", "DrawModiFormatStringF", "DrawModiFormatStringToHandle", "DrawModiFormatStringFToHandle",
	"DrawFormatStringF", "DrawFormatVStringF", "DrawFormatStringFToHandle", "DrawFormatVStringFToHandle",
	"DrawExtendFormatStringF", "DrawExtendFormatVStringF", "DrawExtendFormatStringFToHandle", "DrawExtendFormatVStringFToHandle",
	"DrawFormatStringMask", "DrawFormatStringMaskToHandle",
	"GetDrawFormatStringWidth", "GetDrawFormatStringWidthToHandle", 
	"GetDrawExtendFormatStringWidth", "GetDrawExtendFormatStringWidthToHandle",
	"GetDrawFormatStringSize", "GetDrawFormatStringSizeToHandle", 
	"GetDrawExtendFormatStringSize", "GetDrawExtendFormatStringSizeToHandle",
	"GetDrawFormatStringCharInfo", "GetDrawFormatStringCharInfoToHandle", 
	"GetDrawExtendFormatStringCharInfo", "GetDrawExtendFormatStringCharInfoToHandle",
	"DrawFormatStringToZBuffer", "DrawFormatVStringToZBuffer", "DrawFormatStringToHandleToZBuffer",
	"DrawFormatVStringToHandleToZBuffer", "DrawExtendFormatStringToZBuffer", "DrawExtendFormatVStringToZBuffer",
	"DrawExtendFormatStringToHandleToZBuffer", "DrawExtendFormatVStringToHandleToZBuffer",
	"DrawRotaFormatStringToZBuffer", "DrawRotaFormatStringToHandleToZBuffer",
	"DrawModiFormatStringToZBuffer", "DrawModiFormatStringToHandleToZBuffer",
	"SetKeyInputStringColor", "Paint",
	NULL,
} ;

// ��������}�N���̃��X�g
char *SkipDefine[] =
{
	"STTELL(", "STSEEK(", "STREAD(", "STWRITE(", "STEOF(", "STCLOSE(",
	"STREAM_SEEKTYPE_SET", "STREAM_SEEKTYPE_END", "STREAM_SEEKTYPE_CUR",
	"DEFAULT_FOV", "DEFAULT_TAN_FOV_HALF", "DEFAULT_NEAR", "DEFAULT_FAR",
	"DEFAULT_FONT_SIZE", "DEFAULT_FONT_THINCK", "DEFAULT_FONT_TYPE", "DEFAULT_FONT_EDGESIZE",
	NULL,
};


int		AddFunctionName( const char *Name ) ;								// �֐�����ǉ�����( �߂�l�F�������O�̊֐��̐� )

int		CheckSkipFunc( char *str );											// �w��̊֐����X�L�b�v���邩�ǂ����𒲂ׂ�( 0:�X�L�b�v���Ȃ�  1:DLL�ɂ̂ݏo�͂���  2:�X�L�b�v���� )
bool	CheckSkipDefine( char *str );										// �w��̃}�N���𖳎����邩�ǂ����𒲂ׂ�( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )
bool	CheckSkipType( TextAnalyse *txt, bool function, bool *unsafe );		// �^���炻�̊֐���C#�̃v���O�����ɏo�͂��ׂ����𒲂ׂ�( true:�o�͂��Ȃ�  false:�o�͂��� )
bool	CheckSkipStruct( char *source );									// �w��̍\���̂̒��g���疳������\���̂��ǂ����𔻒f����( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )

bool	GetDirectXDefineStr( char *search, char *buffer );					// �w���DirectX�}�N���̐��l��������擾����( true:����  false:���s )
void	GetNumberOrIdentifier( char *src, char *dest );						// �w��̕����񂩂琔�����͎��ʎq�𔲂��o��

int		TypeConv( TextAnalyse *txt, char *CSType, char *CSTypeName, bool UseSize, char *CPPType, bool *pOutFlag, bool Struct, bool Unsafe, bool Function, bool *TCHARretFlag );		// C++ �p�̌^������� C# �p�̌^������ɕϊ�����( �߂�l�F�^�̃T�C�Y( byte�� ) )
void	Conv16To10( char *src, char *dest );								// �P�U�i���̕�������P�O�i���̕�����ɕϊ�����
int		ConvTokenStr( int bufferPos, char *buffer, char *src );				// �w��̕����񒆂̍\���ԂɃX�y�[�X�����镶����ɕϊ�����
void	CreateTokenStr( void );												// �\���ԂɃX�y�[�X�����镶������쐬����
void	fprintf2( FILE *fp1, FILE *fp2, const char *FormatStr, ... );		// ��̃t�@�C���|�C���^�ɓ������e���v�����g�o�͂���
void	fprintf4( FILE *fp1, FILE *fp2, FILE *fp3, FILE *fp4, const char *FormatStr, ... );		// �l�̃t�@�C���|�C���^�ɓ������e���v�����g�o�͂���

void	OutputDefineStr( FILE *cs, FILE *csW ) ;										// �萔��`���o�͂���
void	OutputStructStr( FILE *cs, FILE *csW ) ;										// �\���̒�`���o�͂���
void	OutputBetaFunctionStr( FILE *cs, FILE *csW, FILE *sc, FILE *scW, FILE *def, FILE *defW, FILE *def64, FILE *def64W, FILE *hd, FILE *hdW ) ;	// �x�^�o�͂̊֐���`���o�͂���
void	OutputFunctionStr( FILE *cs, FILE *csW, FILE *sc, FILE *scW, FILE *def, FILE *defW, FILE *def64, FILE *def64W, FILE *hd, FILE *hdW ) ;		// �֐���`���o�͂���
void	OutputCSFunctionDefine( FILE *cs, FILE *csW, char *FuncName, char *CSRet, char *CSParamDefine, bool Unsafe = false );	// �蓮�֐���`�̏o�͂��y�ɂ���֐�
void	OutputCSFunctionCode( FILE *cs, FILE *csW, char *FuncCode1, char *FuncCode2 );	// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�
void	OutputCSFunctionCode2( FILE *cs, FILE *csW, char *FuncCode1, char *FuncCode2 );	// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�


// �֐�����ǉ�����( �߂�l�F�������O�̊֐��̐� )
int AddFunctionName( const char *Name )
{
	int i ;

	// ���ɓ����֐��������邩���ׂ�
	for( i = 0 ; i < dat.FuncNameNum ; i ++ )
	{
		if( strcmp( dat.FuncName[ i ].Name, Name ) == 0 )
			break ;
	}

	// ����������V�K�ɒǉ�
	if( i == dat.FuncNameNum )
	{
		dat.FuncName[ dat.FuncNameNum ].Counter = 1 ;
		strcpy( dat.FuncName[ dat.FuncNameNum ].Name, Name ) ;
		dat.FuncNameNum ++ ;
		return 1 ;
	}
	else
	{
		// �������瓯���֐��̐����C���N�������g
		dat.FuncName[ i ].Counter ++ ;
		return dat.FuncName[ i ].Counter ;
	}
}

// �w��̊֐����X�L�b�v���邩�ǂ����𒲂ׂ�( 0:�X�L�b�v���Ȃ�  1:DLL�ɂ̂ݏo�͂���  2:�X�L�b�v���� )
int CheckSkipFunc( char *str )
{
	int i;
	
	for( i = 0; SkipFunction[i]; i++ )
	{
		if( strcmp( SkipFunction[i], str ) == 0 )
			return 2;
	}

	for( i = 0; DLLOnlyFunction[i]; i ++ )
	{
		if( strcmp( DLLOnlyFunction[i], str ) == 0 )
			return 1;
	}

	return 0;
}

// �w��̃}�N���𖳎����邩�ǂ����𒲂ׂ�( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )
bool CheckSkipDefine( char *str )
{
	int i;
	char Str[256];

	strcpy( Str, str );
	
	for( i = 0; SkipDefine[i]; i ++ )
	{
		if( strcmp( SkipDefine[i], Str ) == 0 )
			return true;
	}

	return false;
}

// �^���炻�̊֐���C#�̃v���O�����ɏo�͂��ׂ����𒲂ׂ�( true:�o�͂��Ȃ�  false:�o�͂��� )
bool CheckSkipType( TextAnalyse *txt, bool function, bool *unsafe )
{
	char *base;
	char Str[256], Str2[256], Str3[256];

	base = txt->getpos();
	txt->getstr( Str );

	// const �̕�����̓X�L�b�v����
	if( strcmp( Str, "const" ) == 0 )
	{
		txt->getstr( Str );
	}

	txt->getstr( Str2 );
	txt->getstr( Str3 );
	txt->move( base );

	// �|�C���^�̃|�C���^�̓X�L�b�v�Ώ�
	if( strcmp( Str2, "*" ) == 0 && strcmp( Str3, "*" ) == 0 ) return true;

	// �ϒ��������X�L�b�v�Ώ�
	if( strcmp( Str, "..." ) == 0 ) return true;

	// �ȉ��̌^���X�L�b�v�Ώ�
	if( strcmp( Str, "HMODULE" ) == 0 ) return true;
//	if( strcmp( Str, "HWND" ) == 0 ) return true;
	if( strcmp( Str, "HDC" ) == 0 ) return true;
	if( strcmp( Str, "STREAMDATASHREDTYPE2" ) == 0 ) return true;
	if( strcmp( Str, "STREAMDATASHREDTYPE2W" ) == 0 ) return true;
	if( strcmp( Str, "BASEIMAGE" ) == 0 ) return true;
	if( strcmp( Str, "GUID" ) == 0 ) return true;
//	if( strcmp( Str, "MATRIX" ) == 0 ) return true;
//	if( strcmp( Str, "COLORDATA" ) == 0 ) return true;
	if( strcmp( Str, "HBITMAP" ) == 0 ) return true;
	if( strcmp( Str, "BITMAPINFO" ) == 0 ) return true;
	if( strcmp( Str, "WAVEFORMATEX" ) == 0 ) return true;
	if( strcmp( Str, "STREAMDATA" ) == 0 ) return true;
	if( function == false && strncmp( Str, "D_", 2 ) == 0 ) return true;

	// �֐��̖߂�l�̌^�`�F�b�N���ǂ����ŏ����𕪊�
	if( function )
	{
		// const TCHAR * ��������n�j
		if( strcmp( Str, "TCHAR" ) == 0 &&
			strcmp( Str2, "*" ) == 0 )
		{
			return false ;
		}

		// �|�C���^��Ԃ��֐��͌^����ł� unsafe �̂ݗL��
		if( strcmp( Str2, "*" ) == 0 &&
			strcmp( Str, "D_IDirect3DDevice9" ) != 0 &&
			strcmp( Str, "D_IDirect3DSurface9" ) != 0 &&
			strcmp( Str, "TCHAR" ) != 0 &&
			strcmp( Str, "void" ) != 0 )
		{
			*unsafe = true ;
		}

		// �֐��̖߂�l���ȉ��̌^�ȊO�̏ꍇ�̓X�L�b�v
		if( strcmp( Str, "DWORD_PTR" ) != 0 &&
			strcmp( Str, "DWORD" ) != 0 &&
			strcmp( Str, "HWND" ) != 0 &&
			strcmp( Str, "int" ) != 0 &&
			strcmp( Str, "void" ) != 0 &&
			strcmp( Str, "float" ) != 0 &&
			strcmp( Str, "FLOAT4" ) != 0 &&
			strcmp( Str, "DOUBLE4" ) != 0 &&
			strcmp( Str, "BOOL" ) != 0 &&
			strcmp( Str, "HANDLE" ) != 0 &&
			strcmp( Str, "LONGLONG" ) != 0 &&
			strcmp( Str, "ULONGLONG" ) != 0 &&
			strcmp( Str, "VECTOR" ) != 0 &&
			strcmp( Str, "VECTOR_D" ) != 0 &&
			strcmp( Str, "HITRESULT_LINE" ) != 0 &&
			strcmp( Str, "MATRIX" ) != 0 &&
			strcmp( Str, "MATRIX_D" ) != 0 &&
			strcmp( Str, "size_t" ) != 0 &&
			strcmp( Str, "COLOR_F" ) != 0 &&
			strcmp( Str, "COLOR_U8" ) != 0 &&
			strcmp( Str, "MV1_COLL_RESULT_POLY_DIM" ) != 0 &&
			strcmp( Str, "MV1_COLL_RESULT_POLY" ) != 0 &&
			strcmp( Str, "DISPLAYMODEDATA" ) != 0 &&
			strcmp( Str, "D_IDirect3DDevice9" ) != 0 &&
			strcmp( Str, "D_IDirect3DSurface9" ) != 0 &&
			( strcmp( Str, "unsigned" ) != 0 || strcmp( Str2, "int" ) != 0 ) ) return true;
	}
	else
	{
		// IPDATA �̃|�C���^�� unsafe �̂ݗL��
		if( ( strcmp( Str, "IPDATA_IPv6" ) == 0 && strcmp( Str2, "*" ) == 0 ) || 
			( strcmp( Str, "IPDATA" ) == 0 && strcmp( Str2, "*" ) == 0 ) /* || 
			( strcmp( Str, "void" ) == 0   && strcmp( Str2, "*" ) == 0 ) */ )
		{
			*unsafe = true ;
		}
	}

	return false;
}

// �w��̍\���̂̒��g���疳������\���̂��ǂ����𔻒f����( true:�X�L�b�v����  false:�X�L�b�v���Ȃ� )
bool CheckSkipStruct( char *source )
{
	// �\���̂̒��Ɉȉ��̕����񂪊܂܂�Ă�����o�͂��Ȃ�
	if( strstr( source, "union" ) ) return true;
	if( strstr( source, "struct" ) ) return true;
	if( strstr( source, "[" ) ) return true;
//	if( strstr( source, "*" ) ) return true;
//	if( strstr( source, "COLORDATA" ) ) return true;
	if( strstr( source, "STREAMDATASHRED" ) ) return true;
	if( strstr( source, "MV1_REF_POLYGONLIST" ) ) return true;

	return false;
}

// �w���DirectX�}�N���̐��l��������擾����( true:����  false:���s )
bool GetDirectXDefineStr( char *search, char *buffer )
{
	char str[256], str2[256], searchstr[256];

	// �ŏ��Ɍ������镶������Z�b�g
	strcpy( searchstr, search );

	for(;;)
	{
		// DxDirectX.h �� DX_NOUSE_DIRECTX_SDK_FILE ���L�q����Ă���Ƃ���܂ňړ�����
		dxtxt.reset();
		dxtxt.search( "DX_NOUSE_DIRECTX_SDK_FILE" );

		// �������������������
		if( dxtxt.search( searchstr ) )
		{
			// ��������擾����
			dxtxt.getstr( str );

			// enum �̏ꍇ�� = �̌�̕�������擾����
			if( strcmp( str, "=" ) == 0 )
			{
				dxtxt.getstr( str );
			}

			// enum �̏ꍇ�͖��[�� , ���������肷��̂ŁA���ʎq�␔�l�Ɏg�p�ł��镶�������̕�������擾����
			GetNumberOrIdentifier( str, str2 );

			// �擪�����������ł͂Ȃ������ꍇ�͕ʂ̃}�N���Ɣ��f����
			if( str2[0] < '0' || str2[0] > '9' )
			{
				// �������镶�����ύX���Ă���Ɍ������s��
				strcpy( searchstr, str2 );
				continue;
			}

			// 16�i���̉\�����l����10�i���ɕϊ����鏈�����s��
			Conv16To10( str2, str );

			// ���l�������ۑ�����
			strcpy( buffer, str );

			// ����
			return true;
		}
		else
		{
			// ����������G���[
			return false;
		}
	}

	// �����ɗ��邱�Ƃ͂Ȃ����ǈꉞ�G���[
	return false;
}

// �w��̕����񂩂琔�����͎��ʎq�𔲂��o��
void GetNumberOrIdentifier( char *src, char *dest )
{
	// �w��̕����񂩂环�ʎq�Ɏg�p���邱�Ƃ��ł��镶���݂̂𔲂��o��
	for(;;)
	{
		if( *src == '\0' ) break;
		if( ( *src >= 'a' && *src <= 'z' ) ||
			( *src >= 'A' && *src <= 'Z' ) ||
			( *src >= '0' && *src <= '9' ) ||
			*src == '_' )
		{
			*dest = *src;
			dest ++;
		}
		src ++ ;
	}
	*dest = '\0';
}

// C++ �p�̌^������� C# �p�̌^������ɕϊ�����( �߂�l�F�^�̃T�C�Y( byte�� ) )
int TypeConv( TextAnalyse *txt, char *CSType, char *CSTypeName, bool UseSize, char *CPPType, bool *pOutFlag, bool Struct, bool Unsafe, bool Function, bool *TCHARretFlag )
{
	char Str1[256], Str2[256], Str3[256], Str4[256];
	char cstype[64];
	int i, k, cppobj_usenum = 1, typebytes;
	char *start;
	bool is_const;

	// ������p�z�񏉊���
	memset( Str1, 0, sizeof( Str1 ) );
	memset( Str2, 0, sizeof( Str2 ) );
	memset( Str3, 0, sizeof( Str3 ) );
	memset( Str4, 0, sizeof( Str4 ) );
	start = txt->getpos();
	txt->getstr( Str1 );
	txt->getstr( Str2 );
	txt->getstr( Str3 );
	txt->getstr( Str4 );
	txt->move( start );

	// �ŏ��� const TCHAR * ���߂�l�ł͂Ȃ��A�ɂ��Ă���
	if( TCHARretFlag ) *TCHARretFlag = false ;

	// �ŏ��� out �^�C�v�ł͂Ȃ��A�ɂ��Ă���
	if( pOutFlag ) *pOutFlag = false;

	// C# �ł̌^���擾���鏈��
	for(;;)
	{
		// const ���t���ꍇ�͕ʏ���
		is_const = false;
		if( strcmp( Str1, "const" ) == 0 )
		{
			is_const = true;

			txt->getstr( Str1 );

			start = txt->getpos();
			txt->getstr( Str1 );
			txt->getstr( Str2 );
			txt->getstr( Str3 );
			txt->getstr( Str4 );
			txt->move( start );

			// const void * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "void" ) == 0 && strcmp( Str2, "*" ) == 0 )
			{
				txt->getstr( Str4 );
				txt->getstr( Str4 );
				sprintf( CPPType, "const %s %s", Str1, Str2 );
//				strcpy( CSType, "void * " );
				strcpy( CSType, "System.IntPtr " );
				strcpy( CSTypeName, "System.IntPtr" );
				return 4;
			}

			// const IMEINPUTCLAUSEDATA * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "IMEINPUTCLAUSEDATA" ) == 0 && strcmp( Str2, "*" ) == 0 && Struct )
			{
				txt->getstr( Str4 );
				txt->getstr( Str4 );
				sprintf( CPPType, "const %s %s", Str1, Str2 );
				strcpy( CSType, "uint " );
				strcpy( CSTypeName, "uint" );
				return 4;
			}

			// const COLORDATA * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "COLORDATA" ) == 0 && strcmp( Str2, "*" ) == 0 && Struct )
			{
				txt->getstr( Str4 );
				txt->getstr( Str4 );
				sprintf( CPPType, "const %s %s", Str1, Str2 );
				strcpy( CSType, "uint " );
				strcpy( CSTypeName, "uint" );
				return 4;
			}

			// const char ** �� const char * �̏ꍇ�͓���Ȃ̂ł����ŏ������Ă��܂�
			if( strcmp( Str1, "char" ) == 0 || strcmp( Str1, "TCHAR" ) == 0 )
			{
				if( strcmp( Str2, "*" ) == 0 && strcmp( Str3, "*" ) == 0 )
				{
					txt->getstr( Str4 );
					txt->getstr( Str4 );
					txt->getstr( Str4 );
					sprintf( CPPType, "const %s **", Str1 );
					strcpy( CSType, "uint " );
					strcpy( CSTypeName, "uint" );
					return 4;
				}
				if( strcmp( Str2, "*" ) == 0 )
				{
					txt->getstr( Str4 );
					txt->getstr( Str4 );
					sprintf( CPPType, "const %s *", Str1 );

					// �\���̂̏ꍇ�� uint
					if( Struct )
					{
						strcpy( CSType, "uint " );
						strcpy( CSTypeName, "uint" );
					}
					else
					// �֐��̏ꍇ�� System.IntPtr
					if( Function )
					{
						*TCHARretFlag = true ;
						strcpy( CSType, "System.IntPtr " );
						strcpy( CSTypeName, "System.IntPtr" );
					}
					else
					{
						strcpy( CSType, "string " );
						strcpy( CSTypeName, "string" );
					}
					return 4;
				}
			}
		}

		// unsigned ���t���ꍇ�̏���
		if( strcmp( Str1, "unsigned" ) == 0 )
		{
			cppobj_usenum = 2;
			if( strcmp( Str2, "int" ) == 0 )
			{
				strcpy( cstype, "uint" );
				typebytes = 4;
				break;
			}

			if( strcmp( Str2, "short" ) == 0 )
			{
				strcpy( cstype, "ushort" );
				typebytes = 2;
				break;
			}

			if( strcmp( Str2, "char" ) == 0 || strcmp( Str2, "TCHAR" ) == 0 )
			{
				strcpy( cstype, "byte" );
				typebytes = 1;
				break;
			}
		}

		if( strcmp( Str1, "DWORD_PTR" ) == 0 )
		{
			strcpy( cstype, "uint" );
			typebytes = 4;
			break;
		}

		// DWORD �� GetColor �̊֐��̖߂�l���ǂ����ŏ����𕪊�
		if( strcmp( Str1, "DWORD" ) == 0 )
		{
//			if( strcmp( Str2, "GetColor" ) == 0 )
//			{
//				strcpy( cstype, "int" );
//			}
//			else
			{
				strcpy( cstype, "uint" );
			}
			typebytes = 4;
			break;
		}

		// ���̑��̈�ʓI�ȕϊ��̓e�[�u�����g���čs��
		for( k = 0 ; TypeInfoDim[ k ].CPPName ; k ++ )
		{
			if( strcmp( TypeInfoDim[ k ].CPPName, Str1 ) == 0 ) break ;
		}
		if( TypeInfoDim[ k ].CPPName != NULL && k == 3 )
		{
			k = k ;
		}

		// �e�[�u���̒����猩�������ꍇ�͂��̏����Z�b�g
		if( TypeInfoDim[ k ].CPPName != NULL )
		{
			strcpy( cstype, TypeInfoDim[ k ].CSName == NULL ? TypeInfoDim[ k ].CPPName : TypeInfoDim[ k ].CSName ) ;
			typebytes = TypeInfoDim[ k ].Size ;
		}
		else
		{
			// ������Ȃ������ꍇ�̏���
			if( UseSize == true )
			{
				printf( "%s error\n", Str1 );
				for(;;){}
				break;
			}
			strcpy( cstype, Str1 );
			typebytes = 4;
		}
		break;
	}

	// C++ �̌^��������쐬
	strcpy( CPPType, "" );
	if( is_const )
	{
		strcat( CPPType, "const " );
	}
	for( i = 0; i < cppobj_usenum; i ++ )
	{
		txt->getstr( Str1 );
		strcat( CPPType, Str1 );
		strcat( CPPType, " " );
	}

	// �����|�C���^�̏ꍇ�͏����𕪊�
	if( txt->nextchar() == '*' )
	{
		txt->getstr( Str1 );

		// �\���̂̃����o�ϐ��̏ꍇ�� unsigned int �^�̕ϐ��Ƃ���
		if( Struct )
		{
			strcpy( CSType, "uint " ) ;
			strcpy( CSTypeName, "uint" );
		}
		else
		{
			// ������̏ꍇ�� System.Text.StringBuilder �^�Ƃ���
			if( strcmp( cstype, "char" ) == 0 )
			{
				sprintf( CSType, "System.Text.StringBuilder " );
				strcpy( CSTypeName, "System.Text.StringBuilder" );
			}
			else
			// void �^�̏ꍇ�� System.IntPtr ��
			if( strcmp( cstype, "void" ) == 0 )
			{
//				sprintf( CSType, "void * " );
				sprintf( CSType, "System.IntPtr " );
				strcpy( CSTypeName, "System.IntPtr" );
			}
			else
			// �o�͂� System.IntPtr �̏ꍇ�͂��̂܂܏o��
			if( strcmp( cstype, "System.IntPtr" ) == 0 )
			{
				strcpy( CSType, cstype ) ;
			}
			else
			// ����ȊO�̏ꍇ�� unsafe �̏ꍇ�͂��̂܂܃|�C���^�Ƃ���
			if( Unsafe )
			{
				sprintf( CSType, "%s * ", cstype );
				strcpy( CSTypeName, cstype );
			}
			else
			// unsafe �ł͂Ȃ��ꍇ�� out �t���̌^�Ƃ���
			{
				sprintf( CSType, "out %s ", cstype );
				strcpy( CSTypeName, cstype );
			}
			if( pOutFlag ) *pOutFlag = true;
		}

		// �|�C���^�̏ꍇ�̓T�C�Y�� 4byte
		typebytes = 4;

		// C++ �̌^�ɂ͕��ʂ� * ��ǉ�����
		strcat( CPPType, "* " );
	}
	else
	{
		// �|�C���^�ł͂Ȃ��ꍇ�͓���ꂽ�^�����̂܂ܕԂ�
		sprintf( CSType, "%s ", cstype );
		strcpy( CSTypeName, cstype );
	}

	// �^�̃T�C�Y��Ԃ�
	return typebytes;
}

// �P�U�i���̕�������P�O�i���̕�����ɕϊ�����
void Conv16To10( char *src, char *dest )
{
	// 16�i���̕�����ł͂Ȃ������炽���R�s�[���邾��
	if( src[0] == '0' && src[1] == 'x' )
	{
		int i, j, num[16], digit, total;

		// �e���̐��l�𓾂�
		src += 2;
		i = 0;
		for( i = 0; *src != '\0'; src++, i++)
		{
			if( *src >= 'a' && *src <= 'f' )
			{
				num[i] = *src - 'a' + 10;
			}
			else
			if( *src >= 'A' && *src <= 'F' )
			{
				num[i] = *src - 'A' + 10;
			}
			else
			{
				num[i] = *src - '0';
			}
		}
		digit = i;

		// ���l�𓾂�
		j = 0;
		total = 0;
		for( i = 0; i < digit; i ++, j += 4 )
		{
			total |= num[digit-1-i] << j;
		}

		// 10�i���̕�����𓾂�
		itoa( total, dest, 10 );
	}
	else
	{
		strcpy( dest, src );
	}
}

// �w��̕����񒆂̍\���ԂɃX�y�[�X�����镶����ɕϊ�����
int ConvTokenStr( int bufferPos, char *buffer, char *src )
{
	for(;;)
	{
		switch( src[0] )
		{
		case '\0':
			buffer[bufferPos] = ' ';
			bufferPos ++ ;
			return bufferPos;

		case '(': case ')': case '[': case ']':
		case '*': case ',': case ';':
			buffer[bufferPos]   = ' ';
			buffer[bufferPos+1] = src[0];
			buffer[bufferPos+2] = ' ';
			bufferPos += 3;
			break;

		default:
			buffer[bufferPos] = src[0];
			bufferPos ++ ;
			break;
		}
		src ++ ;
	}
}

// �\���ԂɃX�y�[�X�����镶������쐬����
void CreateTokenStr( void )
{
	char str1[256] ;
	int pos;

	dat.Struct   = (char *)malloc( 1024 * 1024 * 16 );
	dat.Function = (char *)malloc( 1024 * 1024 * 16 );

	// �\���̂���`����Ă���Ƃ���܂ňړ�����
	dhtxt.reset() ;
	dhtxt.search( "DX_STRUCT_START" );
	pos = 0;
	for(;;)
	{
		if( dhtxt.getstr( str1 ) == false ) break;
		if( strcmp( str1, "DX_STRUCT_END" ) == 0 ) break;
		pos = ConvTokenStr( pos, dat.Struct, str1 );
	}
	dat.Struct[pos] = '\0';

	dhtxt.search( "DX_FUNCTION_START" );
	pos = 0;
	for(;;)
	{
		if( dhtxt.getstr( str1 ) == false ) break;
		if( strcmp( str1, "DX_FUNCTION_END" ) == 0 ) break;
		pos = ConvTokenStr( pos, dat.Function, str1 );
	}
	dfhtxt.search( "DX_FUNCTION_START" );
	for(;;)
	{
		if( dfhtxt.getstr( str1 ) == false ) break;
		if( strcmp( str1, "DX_FUNCTION_END" ) == 0 ) break;
		pos = ConvTokenStr( pos, dat.Function, str1 );
	}
	dat.Function[pos] = '\0';
}

// ��̃t�@�C���|�C���^�ɓ������e���v�����g�o�͂���
void fprintf2( FILE *fp1, FILE *fp2, const char *FormatStr, ... )
{
	va_list VaList;
	TCHAR String[ 2048 ];

	va_start( VaList, FormatStr );
	vsprintf( String, FormatStr, VaList );
	va_end( VaList );

	fprintf( fp1, String );
	fprintf( fp2, String );
}

// �l�̃t�@�C���|�C���^�ɓ������e���v�����g�o�͂���
void fprintf4( FILE *fp1, FILE *fp2, FILE *fp3, FILE *fp4, const char *FormatStr, ... )
{
	va_list VaList;
	TCHAR String[ 2048 ];

	va_start( VaList, FormatStr );
	vsprintf( String, FormatStr, VaList );
	va_end( VaList );

	fprintf( fp1, String );
	fprintf( fp2, String );
	fprintf( fp3, String );
	fprintf( fp4, String );
}

// �c�w���C�u�����̒萔��`���o�͂���
void OutputDefineStr( FILE *cs, FILE *csW )
{
	char Str[256], Str2[256] ;

	// TRUE �� FALSE �̒�`���o��
	fprintf2( cs, csW, "\t\tpublic const int TRUE  = 1;\n" );
	fprintf2( cs, csW, "\t\tpublic const int FALSE = 0;\n" );

	// define �����܂Ŕ��
	dhtxt.reset();
	dhtxt.search( "DX_DEFINE_START" );
	for(;;)
	{
		// #define ��T��
		dhtxt.getstr( Str );
		if( strcmp( Str, "#define" ) == 0 )
		{
			// DX_DEFINE_END ��������I��
			dhtxt.getstr( Str );
			if( strcmp( Str, "DX_DEFINE_END" ) == 0 ) break;

			// ��������}�N���������ꍇ�� continue
			if( CheckSkipDefine( Str ) ) continue;

			// �萔���̏o��
			fprintf2( cs, csW, "\t\tpublic const int %s = ", Str );

			// �萔�����̎擾
			dhtxt.getstr( Str );

			// ( �������ꍇ�� ) �����镔���܂ŏo��
			if( strcmp( Str, "(" ) == 0 )
			{
				fprintf2( cs, csW, "( " );
				for(;;)
				{
					dhtxt.getstr( Str );
					if( strcmp( Str, ")" ) == 0 ) break;
					fprintf2( cs, csW, "%s ", Str );
				}
				fprintf2( cs, csW, " );\n" );
			}
			else
			// ( �����l�������ꍇ�͐��l�����݂̂𔲂��o���A�P�U�i���̐��l���P�O�i���ɕϊ����ďo��
			if( Str[0] == '(' || ( Str[0] >= '0' && Str[0] <= '9' ) )
			{
				GetNumberOrIdentifier( Str, Str2 );
				Conv16To10( Str2, Str );
				fprintf2( cs, csW, "%s;\n", Str );
			}
			else
			// ����ȊO�̏ꍇ�� DirectX �̒�`�����̂܂܎g�p���Ă���̂� DxDirectX.h ����萔���擾���ďo��
			{
				if( GetDirectXDefineStr( Str, Str2 ) == false )
				{
					printf( "%s error\n", Str );
					for(;;){}
				}
				fprintf2( cs, csW, "%s;\n", Str2 );
			}
		}
	}
	fprintf2( cs, csW, "\n" );
}

// �\���̕������o�͂���
void OutputStructStr( FILE *cs, FILE *csW )
{
	char Str[256], Str2[2024 * 10], Str3[256], Str4[256] ;
	char *start, *end ;
	int count, size ;

	// ���������ł͏o�͂ł��Ȃ��\���̂��x�^�o��
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct IPDATA_IPv6\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public byte Byte00;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(1)] public byte Byte01;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(2)] public byte Byte02;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(3)] public byte Byte03;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public byte Byte04;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(5)] public byte Byte05;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(6)] public byte Byte06;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(7)] public byte Byte07;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public byte Byte08;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(9)] public byte Byte09;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(10)] public byte Byte10;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(11)] public byte Byte11;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public byte Byte12;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(13)] public byte Byte13;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(14)] public byte Byte14;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(15)] public byte Byte15;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
/*	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct IPDATA\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public byte d1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(1)] public byte d2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(2)] public byte d3;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(3)] public byte d4;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );*/
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct RECT\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public int left;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public int top;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public int right;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public int bottom;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct MATRIX\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public float m00;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public float m01;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public float m02;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public float m03;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public float m10;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(20)] public float m11;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public float m12;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(28)] public float m13;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(32)] public float m20;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(36)] public float m21;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public float m22;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(44)] public float m23;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(48)] public float m30;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(52)] public float m31;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(56)] public float m32;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(60)] public float m33;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct MATRIX_D\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public double m00;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public double m01;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public double m02;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public double m03;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(32)] public double m10;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public double m11;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(48)] public double m12;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(56)] public double m13;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(64)] public double m20;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(72)] public double m21;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(80)] public double m22;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(88)] public double m23;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(96)] public double m30;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(104)] public double m31;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(112)] public double m32;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(120)] public double m33;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct COLORDATA\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public byte Format ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(1)] public byte ChannelNum ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(2)] public byte ChannelBitDepth ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(3)] public byte FloatTypeFlag ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public byte PixelByte;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(5)] public byte ColorBitDepth;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(6)] public byte NoneLoc ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(7)] public byte NoneWidth ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public byte RedWidth ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(9)] public byte GreenWidth ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(10)] public byte BlueWidth ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(11)] public byte AlphaWidth ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public byte RedLoc ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(13)] public byte GreenLoc ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(14)] public byte BlueLoc ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(15)] public byte AlphaLoc ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public uint RedMask ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(20)] public uint GreenMask ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public uint BlueMask ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(28)] public uint AlphaMask ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(32)] public uint NoneMask ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(36)] public int MaxPaletteNo ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public COLORPALETTEDATA Palette0 ;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(1060)] public COLORPALETTEDATA Palette255 ;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct MV1_COLL_RESULT_POLY\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public int HitFlag;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public VECTOR HitPosition;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public int FrameIndex;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(20)] public int PolyIndex;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public int MaterialIndex;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(28)] public VECTOR Position0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public VECTOR Position1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(52)] public VECTOR Position2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(64)] public VECTOR Normal;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(76)] public float PositionWeight0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(80)] public float PositionWeight1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(84)] public float PositionWeight2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(88)] public int PosMaxWeightFrameIndex0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(92)] public int PosMaxWeightFrameIndex1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(96)] public int PosMaxWeightFrameIndex2;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct MV1_REF_POLYGON\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public ushort FrameIndex;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(2)] public ushort MeshIndex;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public ushort MaterialIndex;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(6)] public ushort VIndexTarget;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public int VIndex0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public int VIndex1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public int VIndex2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(20)] public VECTOR MinPosition;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(32)] public VECTOR MaxPosition;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct MV1_REF_VERTEX\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public VECTOR Position;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public VECTOR Normal;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public FLOAT2 TexCoord0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(32)] public FLOAT2 TexCoord1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public COLOR_U8 DiffuseColor;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(44)] public COLOR_U8 SpecularColor;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(48)] public int MaxWeightFrameIndex;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Sequential)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct MV1_REF_POLYGONLIST_NT\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\tpublic int PolygonNum;\n" );
	fprintf2( cs, csW, "\t\t\tpublic int VertexNum;\n" );
	fprintf2( cs, csW, "\t\t\tpublic VECTOR MinPosition;\n" );
	fprintf2( cs, csW, "\t\t\tpublic VECTOR MaxPosition;\n" );
	fprintf2( cs, csW, "\t\t\tpublic System.IntPtr Polygons;\n" );
	fprintf2( cs, csW, "\t\t\tpublic System.IntPtr Vertexs;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\tpublic struct MV1_REF_POLYGONLIST\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\tpublic int PolygonNum;\n" );
	fprintf2( cs, csW, "\t\t\tpublic int VertexNum;\n" );
	fprintf2( cs, csW, "\t\t\tpublic VECTOR MinPosition;\n" );
	fprintf2( cs, csW, "\t\t\tpublic VECTOR MaxPosition;\n" );
	fprintf2( cs, csW, "\t\t\tpublic MV1_REF_POLYGON[] Polygons;\n" );
	fprintf2( cs, csW, "\t\t\tpublic MV1_REF_VERTEX[] Vertexs;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct DINPUT_JOYSTATE\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public int X;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public int Y;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public int Z;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public int Rx;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public int Ry;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(20)] public int Rz;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public int Slider0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(28)] public int Slider1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(32)] public uint POV0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(36)] public uint POV1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public uint POV2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(44)] public uint POV3;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(48)] public byte Buttons0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(49)] public byte Buttons1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(50)] public byte Buttons2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(51)] public byte Buttons3;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(52)] public byte Buttons4;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(53)] public byte Buttons5;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(54)] public byte Buttons6;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(55)] public byte Buttons7;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(56)] public byte Buttons8;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(57)] public byte Buttons9;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(58)] public byte Buttons10;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(59)] public byte Buttons11;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(60)] public byte Buttons12;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(61)] public byte Buttons13;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(62)] public byte Buttons14;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(63)] public byte Buttons15;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(64)] public byte Buttons16;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(65)] public byte Buttons17;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(66)] public byte Buttons18;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(67)] public byte Buttons19;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(68)] public byte Buttons20;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(69)] public byte Buttons21;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(70)] public byte Buttons22;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(71)] public byte Buttons23;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(72)] public byte Buttons24;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(73)] public byte Buttons25;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(74)] public byte Buttons26;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(75)] public byte Buttons27;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(76)] public byte Buttons28;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(77)] public byte Buttons29;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(78)] public byte Buttons30;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(79)] public byte Buttons31;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );
	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct XINPUT_STATE\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public byte Buttons0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(1)] public byte Buttons1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(2)] public byte Buttons2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(3)] public byte Buttons3;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public byte Buttons4;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(5)] public byte Buttons5;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(6)] public byte Buttons6;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(7)] public byte Buttons7;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public byte Buttons8;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(9)] public byte Buttons9;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(10)] public byte Buttons10;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(11)] public byte Buttons11;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(12)] public byte Buttons12;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(13)] public byte Buttons13;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(14)] public byte Buttons14;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(15)] public byte Buttons15;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(16)] public byte LeftTrigger;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(17)] public byte RightTrigger;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(18)] public short ThumbLX;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(20)] public short ThumbLY;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(22)] public short ThumbRX;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public short ThumbRY;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );

	fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
	fprintf2( cs, csW, "\t\tpublic struct TOUCHINPUTDATA\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(0)] public int Time;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(4)] public int PointNum;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(8)] public TOUCHINPUTPOINT Point0;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(24)] public TOUCHINPUTPOINT Point1;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(40)] public TOUCHINPUTPOINT Point2;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(56)] public TOUCHINPUTPOINT Point3;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(72)] public TOUCHINPUTPOINT Point4;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(88)] public TOUCHINPUTPOINT Point5;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(104)] public TOUCHINPUTPOINT Point6;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(120)] public TOUCHINPUTPOINT Point7;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(136)] public TOUCHINPUTPOINT Point8;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(152)] public TOUCHINPUTPOINT Point9;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(168)] public TOUCHINPUTPOINT Point10;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(184)] public TOUCHINPUTPOINT Point11;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(200)] public TOUCHINPUTPOINT Point12;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(216)] public TOUCHINPUTPOINT Point13;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(232)] public TOUCHINPUTPOINT Point14;\n" );
	fprintf2( cs, csW, "\t\t\t[FieldOffset(248)] public TOUCHINPUTPOINT Point15;\n" );
	fprintf2( cs, csW, "\t\t};\n\n" );

	// ��͏���
	dhtxt.reset();
	ttxt.setmem( dat.Struct );
	for(;;)
	{
		// struct ��T��
		if( ttxt.getstr( Str ) == false ) break;
		if( strcmp( Str, "struct" ) == 0 )
		{
			// �\���̂̒��g�̕�������ۂ��� Str2 �ɃR�s�[����
			start = ttxt.getpos();
			ttxt.skipgroup( "{", "}" );
			end = ttxt.getpos();
			strncpy( Str2, start, end - start );
			Str2[end-start] = '\0';

			// �\���̂̒��g���疳������\���̂��ǂ������`�F�b�N����
			if( CheckSkipStruct( Str2 ) ) continue;

			// �\���̒�`�̏o��
			ttxt.getstr( Str );
			fprintf2( cs, csW, "\t\t[StructLayout(LayoutKind.Explicit)]\n" );
			fprintf2( cs, csW, "\t\tpublic struct %s\n", Str );
			fprintf2( cs, csW, "\t\t{\n" );
			ttxt.move( start );
			ttxt.search( "{" );

			count = 0;
			for(;;)
			{

				// } �ɗ�����I��
				start = ttxt.getpos();
				ttxt.getstr( Str );
				if( strcmp( Str, "}" ) == 0 ) break;
				ttxt.move( start );

				// �^�� C# �p�ɕϊ�����
				size = TypeConv( &ttxt, Str2, Str4, true, Str3, NULL, true, false, false, NULL );
				for(;;)
				{
					// �ϐ������擾
					ttxt.getstr( Str );

					// �ϐ��̒�`���o��
					fprintf2( cs, csW, "\t\t\t[FieldOffset(%d)] public %s %s;\n", count, Str2, Str );
					count += size;

					// ; �ɗ����烋�[�v�𔲂���
					ttxt.getstr( Str );
					if( strcmp( Str, ";" ) == 0 ) break;
				}
			}
			fprintf2( cs, csW, "\t\t};\n\n" );
		}
	}
	fprintf2( cs, csW, "\n" );
}

// �x�^�o�͂̊֐���`���o�͂���
void OutputBetaFunctionStr( FILE *cs, FILE *csW, FILE *sc, FILE *scW, FILE *def, FILE *defW, FILE *def64, FILE *def64W, FILE *hd, FILE *hdW )
{
	fprintf2( cs, csW, "\t\tpublic static VECTOR MGetTranslateElem( MATRIX InM )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = InM.m30 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = InM.m31 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = InM.m32 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D MGetTranslateElemD( MATRIX_D InM )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = InM.m30 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = InM.m31 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = InM.m32 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VConvFtoD( VECTOR In )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VConvDtoF( VECTOR_D In )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = ( float )In.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = ( float )In.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = ( float )In.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VGet( float x, float y, float z )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VGetD( double x, double y, double z )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT2 F2Get( float u, float v )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT2 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.u = u ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.v = v ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT4 F4Get( float x, float y, float z, float w )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static DOUBLE4 D4Get( double x, double y, double z, double w )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tDOUBLE4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VAdd( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VAddD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VSub( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VSubD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT2 F2Add( FLOAT2 In1, FLOAT2 In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT2 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.u = In1.u + In2.u ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.v = In1.v + In2.v ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT4 F4Add( FLOAT4 In1, FLOAT4 In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = In1.w + In2.w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static DOUBLE4	D4Add( DOUBLE4 In1, DOUBLE4 In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tDOUBLE4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = In1.w + In2.w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT2 F2Sub( FLOAT2 In1, FLOAT2 In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT2 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.u = In1.u - In2.u ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.v = In1.v - In2.v ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT4 F4Sub( FLOAT4 In1, FLOAT4 In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = In1.w - In2.w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static DOUBLE4	D4Sub( DOUBLE4 In1, DOUBLE4 In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tDOUBLE4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = In1.w - In2.w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static float VDot( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static double VDotD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VCross( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.y * In2.z - In1.z * In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.z * In2.x - In1.x * In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.x * In2.y - In1.y * In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VCrossD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In1.y * In2.z - In1.z * In2.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In1.z * In2.x - In1.x * In2.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In1.x * In2.y - In1.y * In2.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VScale( VECTOR In, float Scale )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VScaleD( VECTOR_D In, double Scale )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT2 F2Scale( FLOAT2 In, float Scale )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT2 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.u = In.u * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.v = In.v * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT4 F4Scale( FLOAT4 In, float Scale )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = In.w * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static DOUBLE4	D4Scale( DOUBLE4 In, double Scale )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tDOUBLE4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = In.w * Scale ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static float VSquareSize( VECTOR In )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn In.x * In.x + In.y * In.y + In.z * In.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static double VSquareSizeD( VECTOR_D In )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn In.x * In.x + In.y * In.y + In.z * In.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VTransform( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = InV.x * InM.m00 + InV.y * InM.m10 + InV.z * InM.m20 + InM.m30 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = InV.x * InM.m01 + InV.y * InM.m11 + InV.z * InM.m21 + InM.m31 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = InV.x * InM.m02 + InV.y * InM.m12 + InV.z * InM.m22 + InM.m32 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VTransformD( VECTOR_D InV, MATRIX_D InM )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = InV.x * InM.m00 + InV.y * InM.m10 + InV.z * InM.m20 + InM.m30 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = InV.x * InM.m01 + InV.y * InM.m11 + InV.z * InM.m21 + InM.m31 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = InV.x * InM.m02 + InV.y * InM.m12 + InV.z * InM.m22 + InM.m32 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR VTransformSR( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = InV.x * InM.m00 + InV.y * InM.m10 + InV.z * InM.m20 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = InV.x * InM.m01 + InV.y * InM.m11 + InV.z * InM.m21 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = InV.x * InM.m02 + InV.y * InM.m12 + InV.z * InM.m22 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static VECTOR_D VTransformSRD( VECTOR_D InV, MATRIX_D InM )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tVECTOR_D Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = InV.x * InM.m00 + InV.y * InM.m10 + InV.z * InM.m20 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = InV.x * InM.m01 + InV.y * InM.m11 + InV.z * InM.m21 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = InV.x * InM.m02 + InV.y * InM.m12 + InV.z * InM.m22 ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT4 QTCross( FLOAT4 A, FLOAT4 B )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = A.w * B.w - ( A.x * B.x + A.y * B.y + A.z * B.z ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = B.x * A.w + A.x * B.w + ( A.y * B.z - A.z * B.y ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = B.y * A.w + A.y * B.w + ( A.z * B.x - A.x * B.z ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = B.z * A.w + A.z * B.w + ( A.x * B.y - A.y * B.x ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static DOUBLE4 QTCrossD( DOUBLE4 A, DOUBLE4 B )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tDOUBLE4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w = A.w * B.w - ( A.x * B.x + A.y * B.y + A.z * B.z ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = B.x * A.w + A.x * B.w + ( A.y * B.z - A.z * B.y ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = B.y * A.w + A.y * B.w + ( A.z * B.x - A.x * B.z ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = B.z * A.w + A.z * B.w + ( A.x * B.y - A.y * B.x ) ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static FLOAT4 QTConj( FLOAT4 A )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tFLOAT4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w =  A.w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = -A.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = -A.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = -A.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static DOUBLE4 QTConjD( DOUBLE4 A )\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tDOUBLE4 Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.w =  A.w ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.x = -A.x ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.y = -A.y ;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.z = -A.z ;\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result ;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;

	fprintf2( cs, csW, "\n\n" );

	fprintf2( cs, csW, "\t\t[DllImport(\"DxLib.dll\", EntryPoint = \"dx_MV1GetReferenceMesh\")]\n" ) ;
	fprintf2( cs, csW, "\t\textern static MV1_REF_POLYGONLIST_NT dx_MV1GetReferenceMesh_x86(int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly);\n" ) ;
	fprintf2( cs, csW, "\t\t[DllImport(\"DxLib_x64.dll\", EntryPoint = \"dx_MV1GetReferenceMesh\")]\n" ) ;
	fprintf2( cs, csW, "\t\textern static MV1_REF_POLYGONLIST_NT dx_MV1GetReferenceMesh_x64(int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly);\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static MV1_REF_POLYGONLIST MV1GetReferenceMeshBase(int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly)\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\tMV1_REF_POLYGONLIST_NT Native;\n" ) ;
	fprintf2( cs, csW, "\t\t\tMV1_REF_POLYGONLIST Result;\n" ) ;
	fprintf2( cs, csW, "\t\t\tif( System.IntPtr.Size == 4 )\n" ) ;
	fprintf2( cs, csW, "\t\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tNative = dx_MV1GetReferenceMesh_x86( MHandle, FrameIndex, IsTransform, IsPositionOnly );\n" ) ;
	fprintf2( cs, csW, "\t\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\t\telse\n" ) ;
	fprintf2( cs, csW, "\t\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tNative = dx_MV1GetReferenceMesh_x64( MHandle, FrameIndex, IsTransform, IsPositionOnly );\n" ) ;
	fprintf2( cs, csW, "\t\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.PolygonNum  = Native.PolygonNum;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.VertexNum   = Native.VertexNum;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.MinPosition = Native.MinPosition;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.MaxPosition = Native.MaxPosition;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.Polygons    = null;\n" ) ;
	fprintf2( cs, csW, "\t\t\tResult.Vertexs     = null;\n" ) ;
	fprintf2( cs, csW, "\t\t\tif( Native.PolygonNum > 0 )\n" ) ;
	fprintf2( cs, csW, "\t\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tResult.Polygons = new MV1_REF_POLYGON[ Native.PolygonNum ];\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tint MV1_REF_POLYGON_Size = Marshal.SizeOf( Result.Polygons[ 0 ] );\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tlong Addr = Native.Polygons.ToInt64();\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tfor( int i = 0; i < Native.PolygonNum; i++, Addr += MV1_REF_POLYGON_Size )\n" ) ;
	fprintf2( cs, csW, "\t\t\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\t\t\tResult.Polygons[ i ] = ( MV1_REF_POLYGON )Marshal.PtrToStructure( ( System.IntPtr )Addr, typeof( MV1_REF_POLYGON ) );\n" ) ;
	fprintf2( cs, csW, "\t\t\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\t\tif( Native.VertexNum > 0 )\n" ) ;
	fprintf2( cs, csW, "\t\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tResult.Vertexs = new MV1_REF_VERTEX[ Native.VertexNum ];\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tint MV1_REF_VERTEX_Size = Marshal.SizeOf( Result.Vertexs[ 0 ] );\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tlong Addr = Native.Vertexs.ToInt64();\n" ) ;
	fprintf2( cs, csW, "\t\t\t\tfor( int i = 0; i < Native.VertexNum; i++, Addr += MV1_REF_VERTEX_Size )\n" ) ;
	fprintf2( cs, csW, "\t\t\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\t\t\tResult.Vertexs[ i ] = ( MV1_REF_VERTEX )Marshal.PtrToStructure( ( System.IntPtr )Addr, typeof( MV1_REF_VERTEX ) );\n" ) ;
	fprintf2( cs, csW, "\t\t\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn Result;\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static MV1_REF_POLYGONLIST MV1GetReferenceMesh(int MHandle, int FrameIndex, int IsTransform)\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn MV1GetReferenceMeshBase( MHandle, FrameIndex, IsTransform, FALSE );\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;
	fprintf2( cs, csW, "\t\tpublic static MV1_REF_POLYGONLIST MV1GetReferenceMesh(int MHandle, int FrameIndex, int IsTransform, int IsPositionOnly)\n" ) ;
	fprintf2( cs, csW, "\t\t{\n" ) ;
	fprintf2( cs, csW, "\t\t\treturn MV1GetReferenceMeshBase( MHandle, FrameIndex, IsTransform, IsPositionOnly );\n" ) ;
	fprintf2( cs, csW, "\t\t}\n" ) ;

	fprintf2( cs, csW, "\n\n" );

	fprintf2( cs, csW, "#if DX_USE_UNSAFE\n" );
	fprintf2( cs, csW, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf2( cs, csW, "\t\tpublic unsafe delegate int SetActiveStateChangeCallBackFunctionCallback(int ActiveState, void* UserData);\n" );
	OutputCSFunctionDefine( cs, csW, "dx_SetActiveStateChangeCallBackFunction", "int", "(SetActiveStateChangeCallBackFunctionCallback Callback, void* UserData)", true );
	fprintf2( cs, csW, "\t\tpublic unsafe static int SetActiveStateChangeCallBackFunction(SetActiveStateChangeCallBackFunctionCallback Callback, void* UserData)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetActiveStateChangeCallBackFunction", "(Callback, UserData)" );
	fprintf2( cs, csW, "\n" ) ;
	fprintf2( cs, csW, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf2( cs, csW, "\t\tpublic unsafe delegate void SetUseASyncChangeWindowModeFunctionCallback( void *Data );\n" );
	OutputCSFunctionDefine( cs, csW, "dx_SetUseASyncChangeWindowModeFunction", "int", "( int Flag , SetUseASyncChangeWindowModeFunctionCallback CallBackFunction, void *Data )", true );
	fprintf2( cs, csW, "\t\tpublic unsafe static int SetUseASyncChangeWindowModeFunction( int Flag , SetUseASyncChangeWindowModeFunctionCallback CallBackFunction, void *Data )\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetUseASyncChangeWindowModeFunction", "( Flag , CallBackFunction, Data )" );
	fprintf2( cs, csW, "#endif\n" );
	fprintf2( cs, csW, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf2( cs, csW, "\t\tpublic delegate void SetMenuItemSelectCallBackFunctionCallback( string ItemName, int ItemID );\n" );
	OutputCSFunctionDefine( cs, csW, "dx_SetMenuItemSelectCallBackFunction", "int", "( SetMenuItemSelectCallBackFunctionCallback CallBackFunction )" );
	fprintf2( cs, csW, "\t\tpublic static int SetMenuItemSelectCallBackFunction( SetMenuItemSelectCallBackFunctionCallback CallBackFunction )\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetMenuItemSelectCallBackFunction", "( CallBackFunction )" );
	fprintf2( cs, csW, "\n" ) ;
	fprintf2( cs, csW, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf2( cs, csW, "\t\tpublic delegate void SetWindowMenuCallback( ushort ID );\n" );
	OutputCSFunctionDefine( cs, csW, "dx_SetWindowMenu", "int", "( int MenuID, SetWindowMenuCallback MenuProc )" );
	fprintf2( cs, csW, "\t\tpublic static int SetWindowMenu( int MenuID, SetWindowMenuCallback MenuProc )\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetWindowMenu", "( MenuID, MenuProc )" );
	fprintf2( cs, csW, "\n" ) ;
	fprintf2( cs, csW, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf2( cs, csW, "\t\tpublic delegate void SetRestoreShredPointCallback();\n" );
	OutputCSFunctionDefine( cs, csW, "dx_SetRestoreShredPoint", "int", "( SetRestoreShredPointCallback ShredPoint )" );
	fprintf2( cs, csW, "\t\tpublic static int SetRestoreShredPoint( SetRestoreShredPointCallback ShredPoint )\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetRestoreShredPoint", "( ShredPoint )" );
	fprintf2( cs, csW, "\n" ) ;
	fprintf2( cs, csW, "\t\t[UnmanagedFunctionPointer(CallingConvention.Cdecl)]\n" ) ;
	fprintf2( cs, csW, "\t\tpublic delegate void SetRestoreGraphCallbackCallback();\n" );
	OutputCSFunctionDefine( cs, csW, "dx_SetRestoreGraphCallback", "int", "( SetRestoreGraphCallbackCallback Callback )" );
	fprintf2( cs, csW, "\t\tpublic static int SetRestoreGraphCallback( SetRestoreGraphCallbackCallback Callback )\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetRestoreGraphCallback", "( Callback )" );
	fprintf2( cs, csW, "\n" ) ;
	OutputCSFunctionDefine( cs, csW, "dx_ClearDrawScreen", "int", "( out RECT  ClearRect)" );
	fprintf2( cs, csW, "\t\tpublic static int  ClearDrawScreen( out RECT  ClearRect)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_ClearDrawScreen", "( out ClearRect )" );
	fprintf2( cs, csW, "\t\tpublic static int  ClearDrawScreen()\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\tRECT temp;\n" );
	fprintf2( cs, csW, "\t\t\n" );
	fprintf2( cs, csW, "\t\t\ttemp.left = -1;\n" );
	fprintf2( cs, csW, "\t\t\ttemp.top = -1;\n" );
	fprintf2( cs, csW, "\t\t\ttemp.right = -1;\n" );
	fprintf2( cs, csW, "\t\t\ttemp.bottom = -1;\n" );
	OutputCSFunctionCode2( cs, csW, "return dx_ClearDrawScreen", "( out temp )" );
	fprintf2( cs, csW, "\t\t}\n" );
	fprintf2( cs, csW, "\t\t\n" );
	OutputCSFunctionDefine( cs, csW, "dx_ClearDrawScreenZBuffer", "int", "( out RECT  ClearRect)" );
	fprintf2( cs, csW, "\t\tpublic static int  ClearDrawScreenZBuffer( out RECT  ClearRect)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_ClearDrawScreenZBuffer", "( out ClearRect )" );
	fprintf2( cs, csW, "\t\tpublic static int ClearDrawScreenZBuffer()\n" );
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\tRECT temp;\n" );
	fprintf2( cs, csW, "\t\t\n" );
	fprintf2( cs, csW, "\t\t\ttemp.left = -1;\n" );
	fprintf2( cs, csW, "\t\t\ttemp.top = -1;\n" );
	fprintf2( cs, csW, "\t\t\ttemp.right = -1;\n" );
	fprintf2( cs, csW, "\t\t\ttemp.bottom = -1;\n" );
	OutputCSFunctionCode2( cs, csW, "return dx_ClearDrawScreenZBuffer", "( out temp )" );
	fprintf2( cs, csW, "\t\t}\n" );
	fprintf2( cs, csW, "\t\t\n" );

	OutputCSFunctionDefine( cs, csW, "dx_SetKeyInputStringColor", "int", "( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor, ulong  SelectStrColor, ulong  SelectStrEdgeColor)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , 0 , 0 , 0 , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , 0 , 0 , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , 0 , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , 0xffffffffffffffff , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , SelectStrBackColor , 0xffffffffffffffff , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor, ulong  SelectStrColor)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , SelectStrBackColor , SelectStrColor , 0xffffffffffffffff )" );
	fprintf2( cs, csW, "\t\tpublic static int  SetKeyInputStringColor( ulong  NmlStr, ulong  NmlCur, ulong  IMEStr, ulong  IMECur, ulong  IMELine, ulong  IMESelectStr, ulong  IMEModeStr, ulong  NmlStrE, ulong  IMESelectStrE, ulong  IMEModeStrE, ulong  IMESelectWinE, ulong  IMESelectWinF, ulong  SelectStrBackColor, ulong  SelectStrColor, ulong  SelectStrEdgeColor)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetKeyInputStringColor", "( NmlStr , NmlCur , IMEStr , IMECur , IMELine , IMESelectStr , IMEModeStr , NmlStrE , IMESelectStrE , IMEModeStrE , IMESelectWinE , IMESelectWinF , SelectStrBackColor , SelectStrColor , SelectStrEdgeColor )" );
	fprintf2( hd, hdW, "int  __stdcall dx_SetKeyInputStringColor( ULONGLONG  NmlStr, ULONGLONG  NmlCur, ULONGLONG  IMEStr, ULONGLONG  IMECur, ULONGLONG  IMELine, ULONGLONG  IMESelectStr, ULONGLONG  IMEModeStr, ULONGLONG  NmlStrE = 0, ULONGLONG  IMESelectStrE = 0, ULONGLONG  IMEModeStrE = 0, ULONGLONG  IMESelectWinE = 0xffffffffffffffff, ULONGLONG  IMESelectWinF = 0xffffffffffffffff, ULONGLONG  SelectStrBackColor = 0xffffffffffffffff, ULONGLONG  SelectStrColor = 0xffffffffffffffff, ULONGLONG  SelectStrEdgeColor = 0xffffffffffffffff);\n" );
	fprintf2( sc, scW, "int  __stdcall dx_SetKeyInputStringColor( ULONGLONG  NmlStr, ULONGLONG  NmlCur, ULONGLONG  IMEStr, ULONGLONG  IMECur, ULONGLONG  IMELine, ULONGLONG  IMESelectStr, ULONGLONG  IMEModeStr, ULONGLONG  NmlStrE, ULONGLONG  IMESelectStrE, ULONGLONG  IMEModeStrE, ULONGLONG  IMESelectWinE, ULONGLONG  IMESelectWinF, ULONGLONG  SelectStrBackColor, ULONGLONG  SelectStrColor, ULONGLONG  SelectStrEdgeColor)\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return SetKeyInputStringColor( NmlStr, NmlCur, IMEStr, IMECur, IMELine, IMESelectStr, IMEModeStr, NmlStrE, IMESelectStrE, IMEModeStrE, IMESelectWinE, IMESelectWinF, SelectStrBackColor, SelectStrColor, SelectStrEdgeColor);\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_SetKeyInputStringColor\n" ) ;

	fprintf2( cs, csW, "\n\n" );

	OutputCSFunctionDefine( cs, csW, "dx_Paint", "int", "( int  x, int  y, uint  FillColor, ulong  BoundaryColor)" );
	fprintf2( cs, csW, "\t\tpublic static int  Paint( int  x, int  y, uint  FillColor)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_Paint", "( x , y , FillColor , 0xffffffffffffffffUL )" );
	fprintf2( cs, csW, "\t\tpublic static int  Paint( int  x, int  y, uint  FillColor, ulong  BoundaryColor)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_Paint", "( x , y , FillColor , BoundaryColor )" );
	fprintf2( hd, hdW, "int  __stdcall dx_Paint( int  x, int  y, unsigned int  FillColor, ULONGLONG  BoundaryColor = 0xffffffffffffffffUL);\n" );
	fprintf2( sc, scW, "int  __stdcall dx_Paint( int  x, int  y, unsigned int  FillColor, ULONGLONG  BoundaryColor)\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return Paint( x, y, FillColor, BoundaryColor);\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_Paint\n" ) ;

	fprintf2( cs, csW, "\n\n" );

	OutputCSFunctionDefine( cs, csW, "dx_MV1SetMaterialTypeParamAllS", "int", "( int MHandle, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, float Param0, float Param1, float Param2, float Param3, float Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, Param0, Param1, Param2, Param3, Param4, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, float Param0, float Param1, float Param2, float Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, Param0, Param1, Param2, Param3, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, float Param0, float Param1, float Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, Param0, Param1, Param2, 0.0f, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, float Param0, float Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, Param0, Param1, 0.0f, 0.0f, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, float Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, Param0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParamAll( int MHandle, int FilterType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamAllS", "( MHandle, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f )" );
	fprintf2( cs, csW, "\n" );
	OutputCSFunctionDefine( cs, csW, "dx_MV1SetMaterialTypeParamS", "int", "( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2, float Param3, float Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, Param0, Param1, Param2, Param3, Param4, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2, float Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, Param0, Param1, Param2, Param3, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, Param0, Param1, Param2, 0.0f, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, float Param0, float Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, Param0, Param1, 0.0f, 0.0f, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, float Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, Param0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)" );
	fprintf2( cs, csW, "\t\tpublic static int  MV1SetMaterialTypeParam( int MHandle, int MaterialIndex, int FilterType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_MV1SetMaterialTypeParamS", "( MHandle, MaterialIndex, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f )" );

	fprintf2( cs, csW, "\n\n" );

	OutputCSFunctionDefine( cs, csW, "dx_GraphFilterS", "int", "( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilter( int GrHandle, int FilterType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterS", "( GrHandle, FilterType, 0, 0, 0, 0, 0, 0 )" );
	fprintf2( cs, csW, "\n" );
	OutputCSFunctionDefine( cs, csW, "dx_GraphFilterBltS", "int", "( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterBlt( int SrcGrHandle, int DestGrHandle, int FilterType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterBltS", "( SrcGrHandle, DestGrHandle, FilterType, 0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\n" );
	OutputCSFunctionDefine( cs, csW, "dx_GraphFilterRectBltS", "int", "( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphFilterRectBlt( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphFilterRectBltS", "( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, 0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\n" );
	OutputCSFunctionDefine( cs, csW, "dx_GraphBlendS", "int", "( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlend( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendS", "( GrHandle, BlendGrHandle, BlendRatio, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf2( cs, csW, "\n" );
	OutputCSFunctionDefine( cs, csW, "dx_GraphBlendBltS", "int", "( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf2( cs, csW, "\n" );
	OutputCSFunctionDefine( cs, csW, "dx_GraphBlendRectBltS", "int", "( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  GraphBlendRectBlt( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_GraphBlendRectBltS", "( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf2( hd, hdW, "int  __stdcall dx_MV1SetMaterialTypeParamAllS( int MHandle, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_MV1SetMaterialTypeParamS( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_GraphFilterS( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_GraphFilterBltS( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_GraphFilterRectBltS( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_GraphBlendS( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_GraphBlendBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_GraphBlendRectBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( sc, scW, "int  __stdcall dx_MV1SetMaterialTypeParamAllS( int MHandle, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return MV1SetMaterialTypeParamAll( MHandle, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_MV1SetMaterialTypeParamS( int MHandle, int MaterialIndex, float Param0, float Param1, float Param2, float Param3, float Param4, float Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return MV1SetMaterialTypeParam( MHandle, MaterialIndex, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_GraphFilterS( int GrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return GraphFilter( GrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_GraphFilterBltS( int SrcGrHandle, int DestGrHandle, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return GraphFilterBlt( SrcGrHandle, DestGrHandle, FilterType, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_GraphFilterRectBltS( int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return GraphFilterRectBlt( SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_GraphBlendS( int GrHandle, int BlendGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return GraphBlend( GrHandle, BlendGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_GraphBlendBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return GraphBlendBlt( SrcGrHandle, BlendGrHandle, DestGrHandle, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf2( sc, scW, "int  __stdcall dx_GraphBlendRectBltS( int SrcGrHandle, int BlendGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int BlendX, int BlendY, int DestX, int DestY, int BlendRatio, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return GraphBlendRectBlt( SrcGrHandle, BlendGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, BlendX, BlendY, DestX, DestY, BlendRatio, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 ) ;\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_MV1SetMaterialTypeParamAllS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_MV1SetMaterialTypeParamS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_GraphFilterS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_GraphFilterBltS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_GraphFilterRectBltS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_GraphBlendS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_GraphBlendBltS\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_GraphBlendRectBltS\n" );
	fprintf2( cs, csW, "\n\n" );

	OutputCSFunctionDefine( cs, csW, "dx_SetBlendGraphParamS", "int", "( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, Param3, Param4, Param5)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, Param3, Param4, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, Param3, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1, int Param2)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, Param2, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0, int Param1)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, Param1, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType, int Param0)\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, Param0, 0, 0, 0, 0, 0)" );
	fprintf2( cs, csW, "\t\tpublic static int  SetBlendGraphParam( int BlendGraph, int BlendType )\n" );
	OutputCSFunctionCode( cs, csW, "return dx_SetBlendGraphParamS", "( BlendGraph, BlendType, 0, 0, 0, 0, 0, 0 )" );
	fprintf2( cs, csW, "\n" );
	fprintf2( hd, hdW, "int  __stdcall dx_SetBlendGraphParamS( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 ) ;\n" );
	fprintf2( sc, scW, "int  __stdcall dx_SetBlendGraphParamS( int BlendGraph, int BlendType, int Param0, int Param1, int Param2, int Param3, int Param4, int Param5 )\n" );
	fprintf2( sc, scW, "{\n" );
	fprintf2( sc, scW, "	return SetBlendGraphParam( BlendGraph, BlendType, Param0, Param1, Param2, Param3, Param4, Param5 );\n" );
	fprintf2( sc, scW, "}\n" );
	fprintf4( def, defW, def64, def64W, "\tdx_SetBlendGraphParamS\n" );
	fprintf2( cs, csW, "\n\n" );

	fprintf2( sc, scW, "VECTOR __stdcall dx_MGetTranslateElem( MATRIX InM )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = InM.m[ 3 ][ 0 ] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = InM.m[ 3 ][ 1 ] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = InM.m[ 3 ][ 2 ] ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_MGetTranslateElemD( MATRIX_D InM )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = InM.m[ 3 ][ 0 ] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = InM.m[ 3 ][ 1 ] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = InM.m[ 3 ][ 2 ] ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VConvFtoD( VECTOR In )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VConvDtoF( VECTOR_D In )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = ( float )In.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = ( float )In.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = ( float )In.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VGet( float x, float y, float z )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VGetD( double x, double y, double z )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "FLOAT2 __stdcall dx_F2Get( float u, float v )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT2 Result;\n" ) ;
	fprintf2( sc, scW, "\tResult.u = u ;\n" ) ;
	fprintf2( sc, scW, "\tResult.v = v ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "FLOAT4 __stdcall dx_F4Get( float x, float y, float z, float w )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT4 Result;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = w ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "DOUBLE4 __stdcall dx_D4Get( double x, double y, double z, double w )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tDOUBLE4 Result;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = w ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VAdd( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VAddD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VSub( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VSubD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "FLOAT2 __stdcall dx_F2Add( FLOAT2 In1, FLOAT2 In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT2 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.u = In1.u + In2.u ;\n" ) ;
	fprintf2( sc, scW, "\tResult.v = In1.v + In2.v ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "FLOAT4 __stdcall dx_F4Add( FLOAT4 In1, FLOAT4 In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = In1.w + In2.w ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "DOUBLE4	__stdcall dx_D4Add( DOUBLE4 In1, DOUBLE4 In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tDOUBLE4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x + In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y + In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z + In2.z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = In1.w + In2.w ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "FLOAT2 __stdcall dx_F2Sub( FLOAT2 In1, FLOAT2 In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT2 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.u = In1.u - In2.u ;\n" ) ;
	fprintf2( sc, scW, "\tResult.v = In1.v - In2.v ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "FLOAT4 __stdcall dx_F4Sub( FLOAT4 In1, FLOAT4 In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = In1.w - In2.w ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "DOUBLE4	__stdcall dx_D4Sub( DOUBLE4 In1, DOUBLE4 In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tDOUBLE4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.x - In2.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.y - In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.z - In2.z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = In1.w - In2.w ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "float __stdcall dx_VDot( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\treturn In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "double __stdcall dx_VDotD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\treturn In1.x * In2.x + In1.y * In2.y + In1.z * In2.z ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VCross( VECTOR In1, VECTOR In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.y * In2.z - In1.z * In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.z * In2.x - In1.x * In2.z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.x * In2.y - In1.y * In2.x ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VCrossD( VECTOR_D In1, VECTOR_D In2 )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In1.y * In2.z - In1.z * In2.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In1.z * In2.x - In1.x * In2.z ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In1.x * In2.y - In1.y * In2.x ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VScale( VECTOR In, float Scale )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VScaleD( VECTOR_D In, double Scale )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "FLOAT2 __stdcall dx_F2Scale( FLOAT2 In, float Scale )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT2 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.u = In.u * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.v = In.v * Scale ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "FLOAT4 __stdcall dx_F4Scale( FLOAT4 In, float Scale )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = In.w * Scale ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "DOUBLE4 __stdcall dx_D4Scale( DOUBLE4 In, double Scale )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tDOUBLE4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = In.x * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = In.y * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = In.z * Scale ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w = In.w * Scale ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "float __stdcall dx_VSquareSize( VECTOR In )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\treturn In.x * In.x + In.y * In.y + In.z * In.z ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "double __stdcall dx_VSquareSizeD( VECTOR_D In )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\treturn In.x * In.x + In.y * In.y + In.z * In.z ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VTransform( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2] ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VTransformD( VECTOR_D InV, MATRIX_D InM )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2] ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR __stdcall dx_VTransformSR( VECTOR InV, MATRIX InM )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "VECTOR_D __stdcall dx_VTransformSRD( VECTOR_D InV, MATRIX_D InM )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tVECTOR_D Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;
	fprintf2( sc, scW, "FLOAT4 __stdcall dx_QTCross( FLOAT4 A, FLOAT4 B )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
    fprintf2( sc, scW, "\tFLOAT4 Result ;\n" ) ;
    fprintf2( sc, scW, "\tResult.w = A.w * B.w - ( A.x * B.x + A.y * B.y + A.z * B.z ) ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = B.x * A.w + A.x * B.w + ( A.y * B.z - A.z * B.y ) ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = B.y * A.w + A.y * B.w + ( A.z * B.x - A.x * B.z ) ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = B.z * A.w + A.z * B.w + ( A.x * B.y - A.y * B.x ) ;\n" ) ;
    fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "DOUBLE4 __stdcall dx_QTCrossD( DOUBLE4 A, DOUBLE4 B )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
    fprintf2( sc, scW, "\tDOUBLE4 Result ;\n" ) ;
    fprintf2( sc, scW, "\tResult.w = A.w * B.w - ( A.x * B.x + A.y * B.y + A.z * B.z ) ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = B.x * A.w + A.x * B.w + ( A.y * B.z - A.z * B.y ) ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = B.y * A.w + A.y * B.w + ( A.z * B.x - A.x * B.z ) ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = B.z * A.w + A.z * B.w + ( A.x * B.y - A.y * B.x ) ;\n" ) ;
    fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "FLOAT4 __stdcall dx_QTConj( FLOAT4 A )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tFLOAT4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w =  A.w ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = -A.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = -A.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = -A.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n" ) ;
	fprintf2( sc, scW, "DOUBLE4 __stdcall dx_QTConjD( DOUBLE4 A )\n" ) ;
	fprintf2( sc, scW, "{\n" ) ;
	fprintf2( sc, scW, "\tDOUBLE4 Result ;\n" ) ;
	fprintf2( sc, scW, "\tResult.w =  A.w ;\n" ) ;
	fprintf2( sc, scW, "\tResult.x = -A.x ;\n" ) ;
	fprintf2( sc, scW, "\tResult.y = -A.y ;\n" ) ;
	fprintf2( sc, scW, "\tResult.z = -A.z ;\n" ) ;
	fprintf2( sc, scW, "\treturn Result ;\n" ) ;
	fprintf2( sc, scW, "}\n\n" ) ;



	fprintf2( hd, hdW, "VECTOR   __stdcall dx_MGetTranslateElem( MATRIX InM ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_MGetTranslateElem\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_MGetTranslateElemD( MATRIX_D InM ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_MGetTranslateElemD\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VConvFtoD( VECTOR In ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VConvFtoD\n" ) ;
	fprintf2( hd, hdW, "VECTOR __stdcall dx_VConvDtoF( VECTOR_D In ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VConvDtoF\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VGet(  float x, float y, float z ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VGet\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VGetD( double x, double y, double z ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VGetD\n" ) ;
	fprintf2( hd, hdW, "FLOAT2 __stdcall dx_F2Get( float u, float v ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F2Get\n" ) ;
	fprintf2( hd, hdW, "FLOAT4 __stdcall dx_F4Get( float x, float y, float z, float w ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F4Get\n" ) ;
	fprintf2( hd, hdW, "DOUBLE4 __stdcall dx_D4Get( double x, double y, double z, double w ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_D4Get\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VAdd(  VECTOR   In1, VECTOR   In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VAdd\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VAddD( VECTOR_D In1, VECTOR_D In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VAddD\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VSub(  VECTOR   In1, VECTOR   In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VSub\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VSubD( VECTOR_D In1, VECTOR_D In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VSubD\n" ) ;
	fprintf2( hd, hdW, "FLOAT2 __stdcall dx_F2Add( FLOAT2 In1, FLOAT2 In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F2Add\n" ) ;
	fprintf2( hd, hdW, "FLOAT4 __stdcall dx_F4Add( FLOAT4 In1, FLOAT4 In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F4Add\n" ) ;
	fprintf2( hd, hdW, "DOUBLE4 __stdcall dx_D4Add( DOUBLE4 In1, DOUBLE4 In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_D4Add\n" ) ;
	fprintf2( hd, hdW, "FLOAT2 __stdcall dx_F2Sub( FLOAT2 In1, FLOAT2 In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F2Sub\n" ) ;
	fprintf2( hd, hdW, "FLOAT4 __stdcall dx_F4Sub( FLOAT4 In1, FLOAT4 In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F4Sub\n" ) ;
	fprintf2( hd, hdW, "DOUBLE4 __stdcall dx_D4Sub( DOUBLE4 In1, DOUBLE4 In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_D4Sub\n" ) ;
	fprintf2( hd, hdW, "float  __stdcall dx_VDot(  VECTOR   In1, VECTOR   In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VDot\n" ) ;
	fprintf2( hd, hdW, "double __stdcall dx_VDotD( VECTOR_D In1, VECTOR_D In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VDotD\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VCross(  VECTOR   In1, VECTOR   In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VCross\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VCrossD( VECTOR_D In1, VECTOR_D In2 ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VCrossD\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VScale(  VECTOR   In, float Scale ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VScale\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VScaleD( VECTOR_D In, double Scale ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VScaleD\n" ) ;
	fprintf2( hd, hdW, "FLOAT2 __stdcall dx_F2Scale( FLOAT2 In, float Scale ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F2Scale\n" ) ;
	fprintf2( hd, hdW, "FLOAT4 __stdcall dx_F4Scale( FLOAT4 In, float Scale ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_F4Scale\n" ) ;
	fprintf2( hd, hdW, "DOUBLE4 __stdcall dx_D4Scale( DOUBLE4 In, double Scale ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_D4Scale\n" ) ;
	fprintf2( hd, hdW, "float  __stdcall dx_VSquareSize(  VECTOR   In ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VSquareSize\n" ) ;
	fprintf2( hd, hdW, "double __stdcall dx_VSquareSizeD( VECTOR_D In ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VSquareSizeD\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VTransform(  VECTOR   InV, MATRIX   InM ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VTransform\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VTransformD( VECTOR_D InV, MATRIX_D InM ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VTransformD\n" ) ;
	fprintf2( hd, hdW, "VECTOR   __stdcall dx_VTransformSR(  VECTOR   InV, MATRIX   InM ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VTransformSR\n" ) ;
	fprintf2( hd, hdW, "VECTOR_D __stdcall dx_VTransformSRD( VECTOR_D InV, MATRIX_D InM ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_VTransformSRD\n" ) ;
	fprintf2( hd, hdW, "FLOAT4 __stdcall dx_QTCross( FLOAT4 A, FLOAT4 B ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_QTCross\n" ) ;
	fprintf2( hd, hdW, "DOUBLE4 __stdcall dx_QTCrossD( DOUBLE4 A, DOUBLE4 B ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_QTCrossD\n" ) ;
	fprintf2( hd, hdW, "FLOAT4 __stdcall dx_QTConj( FLOAT4 A ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_QTConj\n" ) ;
	fprintf2( hd, hdW, "DOUBLE4 __stdcall dx_QTConjD( DOUBLE4 A ) ;\n" ) ;
	fprintf4( def, defW, def64, def64W, "\tdx_QTConjD\n" ) ;
}

// �֐���`���o�͂���
void OutputFunctionStr( FILE *cs, FILE *csW, FILE *sc, FILE *scW, FILE *def, FILE *defW, FILE *def64, FILE *def64W, FILE *hd, FILE *hdW )
{
	char Str[256], Str2[2024 * 10], Str3[256], DllFuncName[ 256 ] ;
	char *start;
	bool unsafe;
	bool TCHARret;
	int i, j, k, l, size, funccount;
	FUNCTION Func;

	// �x�^�����̏o��
	OutputBetaFunctionStr( cs, csW, sc, scW, def, defW, def64, def64W, hd, hdW ) ;

	ttxt.setmem( dat.Function );
	for(;;)
	{
		PARAMETER *par;
		char *FuncStart ;

		// ������擾
		if( ttxt.getstr( Str ) == false ) break;

		// �C�����C���֐��̓X�L�b�v
		if( strcmp( Str, "__inline" ) == 0 )
		{
			ttxt.search( "{" ) ;
			ttxt.search( "}" ) ;
			continue ;
		}
		else
		// extern �ȊO�̓X�L�b�v
		if( strcmp( Str, "extern" ) != 0 )
		{
			continue ;
		}

		// �����֐��̃J�E���^��������
		funccount = 0x0ccccccc ;

		// ���݂̕�����ʒu��ۑ�
		FuncStart = ttxt.getpos() ;

		// unsafe ���ǂ����̃t���O��|���Ă���
		unsafe = false ;

		// C# �p�ɏo�͂ł��Ȃ��߂�l�̌^�̏ꍇ�� DLL �ɂ̂ݏo�͂���
		if( CheckSkipType( &ttxt, true, &unsafe ) == true ) goto CPPOUTONLY;

		// �߂�l�̌^���擾����
		size = TypeConv( &ttxt, Func.CSType, Func.CSTypeName, false, Func.CPPType, NULL, false, unsafe, true, &TCHARret );

		// void �^���ǂ�����ۑ�����
		Func.Void = size == 0;

		// �֐������擾
		ttxt.getstr( Func.Name );
/*
		if( strcmp( Func.Name, "BltBmpToGraph" ) == 0 )
		{
			size = size;
		}
*/
		// �֐����̓o�^
		funccount = AddFunctionName( Func.Name ) ;

		// �����Ώۂ̊֐����ǂ����𒲂ׂ�
		i = CheckSkipFunc( Func.Name ) ;

		// ���S�����̏ꍇ�͉��������Ɏ���
		if( i == 2 ) continue ;

		// �߂�l�� 1 �̏ꍇ�� DLL �ɂ̂ݏo�͂���
		if( i == 1 ) goto CPPOUTONLY;

		// �������
		{
			// �����̊J�n�ʒu��ۑ�
			ttxt.getstr( Str );
			start = ttxt.getpos();

			// �ŏ��̂Q�����񂩂���������������ׂ�
			ttxt.getstr( Str );
			ttxt.getstr( Str2 );
			ttxt.move( start );
			if( strcmp( Str, ")" ) == 0 || ( strcmp( Str, "void" ) == 0 && strcmp( Str2, ")" ) == 0 ) )
			{
				// �����������ꍇ
				Func.DefaultNum = 0;
				Func.ParameterNum = 0;
				if( strcmp( Str, "void" ) == 0 )
					ttxt.getstr( Str );
			}
			else
			{
				// ����������ꍇ
				i = 0;
				par = Func.Parameter;
				Func.DefaultNum = 0;
				for(;;)
				{
					// �z�񂩂ǂ����̃t���O��|���Ă���
					par->Array = false;

					// �����̌^�� C# �ɏo�͂ł��邩�ǂ������`�F�b�N
					if( CheckSkipType( &ttxt, false, &unsafe ) == true )
					{
						i = -1;
						break;
					}

					// C#�p�̈����̌^�𓾂�
					TypeConv( &ttxt, par->CSType, par->CSTypeName, false, par->CPPType, &par->Out, false, unsafe, false, NULL );

					// ���������擾
					ttxt.getstr( par->Name );

					// out �����̏ꍇ�́A�������̖����� Array �������ꍇ�́A In/Out �����ɂ���
					if( par->Out && strncmp( par->CSType, "out ", 4 ) == 0 )
					{
						int Length = strlen( par->Name );
						if( Length >= 5/*strlen( "Array" )*/ &&
							strcmp( &par->Name[ Length - 5 ], "Array" ) == 0 )
						{
							char TempStr[ 64 ] ;

							strcpy( TempStr, par->CSType );
							sprintf( par->CSType, "[In, Out] %s[] ", par->CSTypeName );

							par->Out = false;
							par->Array = true;
						}
					}

					// �^�̎��� ( ������ꍇ�͊֐��|�C���^�Ȃ̂ŁA�����ϊ��ł� C# �ɏo�͂ł��Ȃ�
					if( strcmp( par->Name, "(" ) == 0 )
					{
						i = -1;
						break;
					}

					// �f�t�H���g���������邩�ǂ����̃`�F�b�N
					par->ValidDefault = false;
					if( ttxt.nextchar() == '=' )
					{
						// �f�t�H���g�������擾����
						ttxt.getstr( Str );
						par->ValidDefault = true;
						ttxt.getstr( par->Default );
						Func.DefaultNum ++ ;
					}

					par ++ ;
					i ++ ;
					ttxt.getstr( Str );
					if( strcmp( Str, ")" ) == 0 ) break;
				}

				// �G���[������������ DLL �ɂ̂ݏo�͂���
				if( i == -1 )
				{
					goto CPPOUTONLY;
				}

				// �����̐���ۑ�
				Func.ParameterNum = i;
			}
			ttxt.getstr( Str );
		}

		// �����̊֐����������Ă��Ȃ��ꍇ�� dx_�֐��� �� DLL �̊֐����ɂ���
		if( funccount == 1 )
		{
			sprintf( DllFuncName, "dx_%s", Func.Name );
		}
		else
		// �Q�ڈȏ�̏ꍇ�� dx_�֐���_���ڂ� �� DLL �̊֐����ɂ���
		{
			sprintf( DllFuncName, "dx_%s_%d", Func.Name, funccount );
		}

		// unsafe �̏ꍇ�̓}�N�����o�͂���
		if( unsafe )
		{
			fprintf2( cs, csW, "#if DX_USE_UNSAFE\n" );
		}

		// DLL �ւ̏o�͊֐����X�g�ɏo��
		fprintf4( def, defW, def64, def64W, "\t%s\n", DllFuncName ) ;

		// C# �p�̊֐��錾�o��
		for( l = 0; l < 2; l++ )
		{
			fprintf(  cs,      "\t\t[DllImport(\"DxLib%s.dll\", EntryPoint=\"%s\")]\n", l == 0 ? "" : "_x64", DllFuncName );
			fprintf(  csW,     "\t\t[DllImport(\"DxLibW%s.dll\", EntryPoint=\"%s\", CharSet=CharSet.Unicode)]\n", l == 0 ? "" : "_x64", DllFuncName );
			fprintf2( cs, csW, "\t\textern %sstatic %s %s_%s( ", unsafe ? "unsafe " : "", Func.CSType, DllFuncName, l == 0 ? "x86" : "x64" );

			// �����������o��
			par = Func.Parameter;
			for( i = 0; i < Func.ParameterNum; i ++, par ++ )
			{
				fprintf2( cs, csW, "%s %s", par->CSType, par->Name );
				if( i != Func.ParameterNum - 1 )
				{
					fprintf2( cs, csW, ", " );
				}
			}
			fprintf2( cs, csW, ");\n" );
		}

		// C#�p�֐��o��
		{
			// unsafe �֐��ł��Ȃ� void * �ȊO�� out �����̈���������ꍇ�̓f�t�H���g�������֐��� C# �ŃG�~�����[�g�ł��Ȃ��̂�
			// ���̂悤�Ȉ��������邩�ǂ������ׂ�
			par = Func.Parameter ;
			for( l = 0 ; l < Func.ParameterNum ; l ++, par ++ )
			{
				if( par->ValidDefault && ( par->Out || par->Array ) )
				{
					if( unsafe == false || strcmp( par->CSType, "void * " ) != 0 )
					{
						// ��������f�t�H���g�����𖳌��ɂ���
						Func.DefaultNum = 0;
						par = Func.Parameter ;
						for( k = 0 ; k < Func.ParameterNum ; k ++, par ++ )
						{
							par->ValidDefault = false ;
						}
						break ;
					}
				}
			}

			// �f�t�H���g���������̏o�͂ƁA�f�t�H���g�������������̐������֐����쐬����K�v������̂ł��̃��[�v
			for( j = 0 ; j <= Func.DefaultNum ; j ++ )
			{
				// �߂�l�̌^�Ɗ֐����̏o��
				fprintf2( cs, csW, "\t\tpublic %sstatic %s %s( ", unsafe ? "unsafe " : "", TCHARret ? "string " : Func.CSType, Func.Name );

				// �p�����[�^��`�̏o��
				par = Func.Parameter;
				k = 0;
				for( i = 0; i < Func.ParameterNum; i ++, par ++ )
				{
					// �f�t�H���g�������g�p����ꍇ�͂���ȍ~�̈����̏o�͍͂s��Ȃ�
					if( par->ValidDefault )
					{
						k ++ ;
						if( k > j ) break;
					}

					// �����ƈ����̊Ԃ̏o��
					if( i != 0 )
					{
						fprintf2( cs, csW, ", " );
					}

					// �����̌^�ƈ������̏o��
					fprintf2( cs, csW, "%s %s", par->CSType, par->Name );
				}
				fprintf2( cs, csW, ")\n" );

				// �֐��̒��g���o��
				fprintf2( cs, csW, "\t\t{\n" );

				// out �^�C�v�̃f�t�H���g�����́A���[�J���ϐ��ŏo�͂��󂯎��悤�ɂ���
				par = Func.Parameter;
				k = 0;
				for( i = 0; i < Func.ParameterNum; i ++, par ++ )
				{
					if( par->ValidDefault == 0 ) continue ;
					if( k >= j && par->Out && strcmp( par->CSType, "string " ) != 0 && strcmp( par->CSType, "void * " ) != 0 && strcmp( par->CSType, "System.IntPtr " ) != 0 )
					{
						fprintf2( cs, csW, "\t\t\t%s%s;\n", par->CSType[0] == 'o' && par->CSType[1] == 'u' && par->CSType[2] == 't' && par->CSType[3] == ' ' ? &par->CSType[ 4 ] : par->CSType, par->Name ) ;
					}
					k ++ ;
				}

				for( l = 0; l < 2; l++ )
				{
					if( l == 0 )
					{
						// 64bit��32bit���̕���
						fprintf2( cs, csW, "\t\t\tif( System.IntPtr.Size == 4 )\n" );
						fprintf2( cs, csW, "\t\t\t{\n" );
					}

					// TCHAR * ���߂�l�̏ꍇ�� System.IntPtr �ɖ߂�l��������
					if( TCHARret )
					{
						fprintf2( cs, csW, "\t\t\t\tSystem.IntPtr resultIntPtr = " );
					}
					else
					// void �^�̏ꍇ�� return ���o�͂��Ȃ�
					if( Func.Void == false )
					{
						fprintf2( cs, csW, "\t\t\t\treturn " );
					}
					else
					{
						fprintf2( cs, csW, "\t\t\t\t" );
					}

					// DLL �̊֐������o��
					fprintf2( cs, csW, "%s%s( ", DllFuncName, l == 0 ? "_x86" : "_x64" ) ;

					// �������o��
					par = Func.Parameter;
					k = 0;
					for( i = 0; i < Func.ParameterNum; i ++, par ++ )
					{
						// ��؂�� , ���o��
						if( i != 0 )
						{
							fprintf2( cs, csW, ", " );
						}

						// �f�t�H���g���������邩�ǂ����ŕ���
						if( par->ValidDefault && k >= j )
						{
							// �f�t�H���g������ NULL ���ǂ����ŏ����𕪊�
							if( strcmp( par->Default, "NULL" ) == 0 )
							{
								// string �� void * �� System.IntPtr �̏ꍇ�� null ���o��
								if( strcmp( par->CSType, "string " ) == 0 || strcmp( par->CSType, "void * " ) == 0 || strcmp( par->CSType, "System.IntPtr " ) == 0 )
								{
									// System.IntPtr �̏ꍇ�̓L���X�g����
									if( strcmp( par->CSType, "System.IntPtr " ) == 0 )
									{
										fprintf2( cs, csW, "( System.IntPtr )null " );
									}
									else
									{
										fprintf2( cs, csW, "null " );
									}
								}
								else
								// ����ȊO�̏ꍇ�͈������Ɠ����̃��[�J���ϐ�����`����Ă���̂ł�����g��
								if( unsafe )
								{
									fprintf2( cs, csW, "%s ", par->Name ) ;
								}
								else
								{
									fprintf2( cs, csW, "out %s ", par->Name ) ;
								}
							}
							else
							{
								// NULL �ȊO�̏ꍇ�̓|�C���^�n�ł͂Ȃ��̂ŁA�f�t�H���g���l�����̂܂܏o�͂���
								fprintf2( cs, csW, "%s ", par->Default );
							}
						}
						else
						{
							// �f�t�H���g�����������ꍇ

							// out ���������邩�ǂ����ŏ����𕪊�
							if( par->Out )
							{
								// out �����ł� System.Text.StringBuilder �� System.IntPtr �� unsafe �̏ꍇ�͂��̂܂ܕϐ������o��
								if( strncmp( par->CSType, "System.Text.StringBuilder", strlen( "System.Text.StringBuilder" ) ) == 0 ||
									strncmp( par->CSType, "System.IntPtr", strlen( "System.IntPtr" ) ) == 0 || unsafe )
								{
									fprintf2( cs, csW, "%s ", par->Name );
								}
								else
								// ����ȊO�̏ꍇ�� out ��t����
								{
									fprintf2( cs, csW, "out %s ", par->Name );
								}
							}
							else
							{
								// out �����������ꍇ�͕��ʂɏo��
								fprintf2( cs, csW, "%s ", par->Name );
							}
						}

						// �f�t�H���g�������������ꍇ�̓J�E���^���C���N�������g
						if( par->ValidDefault )
							k ++ ;
					}
					fprintf2( cs, csW, ");\n" );

					// TCHAR * ���߂�l�̏ꍇ�� System.IntPtr ���� String ���o�͂��ĕԂ��������s��
					if( TCHARret )
					{
						fprintf2( cs, csW, "\t\t\t\tif( resultIntPtr == System.IntPtr.Zero )\n" );
						fprintf2( cs, csW, "\t\t\t\t{\n" );
						fprintf2( cs, csW, "\t\t\t\t\treturn \"\";\n" );
						fprintf2( cs, csW, "\t\t\t\t}\n" );
						fprintf2( cs, csW, "\t\t\t\telse\n" );
						fprintf2( cs, csW, "\t\t\t\t{\n" );
						fprintf(  cs,      "\t\t\t\t\treturn System.Runtime.InteropServices.Marshal.PtrToStringAnsi( resultIntPtr );\n" );
						fprintf(  csW,     "\t\t\t\t\treturn System.Runtime.InteropServices.Marshal.PtrToStringUni( resultIntPtr );\n" );
						fprintf2( cs, csW, "\t\t\t\t}\n" );
					}

					if( l == 0 )
					{
						fprintf2( cs, csW, "\t\t\t}\n" );
						fprintf2( cs, csW, "\t\t\telse\n" );
						fprintf2( cs, csW, "\t\t\t{\n" );
					}
					else
					{
						fprintf2( cs, csW, "\t\t\t}\n" );
					}
				}

				fprintf2( cs, csW, "\t\t}\n" );
			}
		}

		// DLL�p�֐��o��
		{
			// GetColor �����ʏ���
//			if( strcmp( Func.CPPType, "DWORD " ) == 0 && strcmp( Func.Name, "GetColor" ) == 0 )
//			{
//				fprintf( hd, "int __stdcall dx_GetColor( " );
//				fprintf( sc, "int __stdcall dx_GetColor( " );
//			}
//			else
			{
				// �����̊֐�������ꍇ�͊֐����̌��ɉ��ڂ��̔ԍ����o�͂���
				if( funccount == 1 )
				{
					fprintf2( hd, hdW, "%s __stdcall dx_%s( ", Func.CPPType, Func.Name );
					fprintf2( sc, scW, "%s __stdcall dx_%s( ", Func.CPPType, Func.Name );
				}
				else
				{
					fprintf2( hd, hdW, "%s __stdcall dx_%s_%d( ", Func.CPPType, Func.Name, funccount );
					fprintf2( sc, scW, "%s __stdcall dx_%s_%d( ", Func.CPPType, Func.Name, funccount );
				}
			}

			// �����̏o��
			{
				// �����������ꍇ�� void ���o�͂��ďI��
				if( Func.ParameterNum == 0 )
				{
					fprintf2( hd, hdW, "void" );
					fprintf2( sc, scW, "void" );
				}
				else
				{
					// �������o��
					par = Func.Parameter;
					for( i = 0; i < Func.ParameterNum; i ++, par ++ )
					{
						// ��؂蕶���̏o��
						if( i != 0 )
						{
							fprintf2( hd, hdW, ", " );
							fprintf2( sc, scW, ", " );
						}

						// �����̌^�ƈ��������o��
						fprintf2( hd, hdW, "%s %s", par->CPPType, par->Name );
						fprintf2( sc, scW, "%s %s", par->CPPType, par->Name );

						// �f�t�H���g����������ꍇ�͂�����o��
						if( par->ValidDefault )
						{
							fprintf2( hd, hdW, " = %s", par->Default );
						}
					}
				}
				fprintf2( hd, hdW, ");\n" );
				fprintf2( sc, scW, ")\n" );
			}

			// �֐��̒��g���o��
			{
				fprintf2( sc, scW, "{\n" );

				// �߂�l�� void �^�ȊO�̏ꍇ�� return ���o��
				if( Func.Void )
				{
					fprintf2( sc, scW, "\t" );
				}
				else
				{
					fprintf2( sc, scW, "\treturn " );
				}

				// �֐����̏o��
				fprintf2( sc, scW, "%s( ", Func.Name );

				// �����̏o��
				par = Func.Parameter;
				for( i = 0; i < Func.ParameterNum; i ++, par ++ )
				{
					if( i != 0 )
					{
						fprintf2( sc, scW, ", " );
					}
					fprintf2( sc, scW, "%s", par->Name );
				}
				fprintf2( sc, scW, ");\n" );
				fprintf2( sc, scW, "}\n" );
			}
		}

		// unsafe �̏ꍇ�̓}�N���̏I�[���o�͂���
		if( unsafe )
		{
			fprintf2( cs, csW, "#endif\n" );
		}
		fprintf2( cs, csW, "\n" );

		continue ;

		// DLL �ɂ̂ݏo�͂���ꍇ
CPPOUTONLY:
		ttxt.move( FuncStart ) ;

		// �֐��̖߂�l�� void �^���ǂ������Z�b�g
		ttxt.getstr( Str ) ;
		Func.Void = strcmp( Str, "void" ) == 0 ? 1 : 0 ;

		// �֐��̖߂�l�̌^�Ɗ֐������o��
		{
			fprintf2( hd, hdW, "%s ", Str ) ;
			fprintf2( sc, scW, "%s ", Str ) ;

			// �֐���������Ƃ���܂ŕ�������o��
			for(;;)
			{
				ttxt.getstr( Str ) ;
				if( Str[ 0 ] == '*' ) Func.Void = 0 ;
				if( ttxt.nextchar() == '(' ) break ;
				fprintf2( hd, hdW, "%s ", Str ) ;
				fprintf2( sc, scW, "%s ", Str ) ;
			}
		}

		// �֐�����ۑ�
		strcpy( Func.Name, Str ) ;

		// �֐����o�^���܂���������֐�����o�^
		if( funccount == 0x0ccccccc )
		{
			funccount = AddFunctionName( Func.Name ) ;
		}

		// DLL �ł̊֐����̍쐬
		if( funccount == 1 )
		{
			sprintf( Str2, "dx_%s", Func.Name ) ;
		}
		else
		{
			sprintf( Str2, "dx_%s_%d", Func.Name, funccount ) ;
		}

		// �֐������o��
		fprintf4( def, defW, def64, def64W, "\t%s\n", Str2 ) ;
		fprintf2( hd, hdW, "__stdcall %s( ", Str2 ) ;
		fprintf2( sc, scW, "__stdcall %s( ", Str2 ) ;

		// �����̏o��
		{
			Func.ParameterNum = 0 ;
			ttxt.getstr( Str ) ;
			ttxt.getstr( Str ) ;

			// �����������ꍇ�� void �Ƃ����o�͂���
			if( strcmp( Str, "void" ) == 0 && ttxt.nextchar() != '*' && ttxt.nextchar() != '(' )
			{
				ttxt.getstr( Str ) ;
				ttxt.getstr( Str ) ;
				fprintf2( hd, hdW, "void )" ) ;
				fprintf2( sc, scW, "void )" ) ;
			}
			else
			{
				for(;;)
				{
					ttxt.getstr( Str2 ) ;

					// �R�߂� * �̏ꍇ�͏����𕪊�
					if( ttxt.nextchar() == '*' )
					{
						ttxt.getstr( Str3 ) ;

						// �Q�ڂ� ( �̏ꍇ�͈����͊֐��|�C���^
						if( Str2[ 0 ] == '(' )
						{
							// �֐��|�C���^�̏ꍇ�͂S�ڂ�������
							ttxt.getstr( Func.Parameter[ Func.ParameterNum ].Name ) ;

							// ) �̎擾
							ttxt.getstr( Str2 ) ;

							// �֐��|�C���^�̈����������̏o��
							fprintf2( hd, hdW, "%s ( *%s )", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;
							fprintf2( sc, scW, "%s ( *%s )", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;

							// �֐��|�C���^�̊֐��̈������o�͂���
							for(;;)
							{
								ttxt.getstr( Str ) ;
								if( Str[0] == ')' ) break ;
								fprintf2( hd, hdW, "%s ", Str ) ;
								fprintf2( sc, scW, "%s ", Str ) ;
							}
							fprintf2( hd, hdW, "%s ", Str ) ;
							fprintf2( sc, scW, "%s ", Str ) ;
						}
						else
						{
							// �֐��|�C���^�ȊO�̏ꍇ�͕��ʂ̃|�C���^�����Ƃ��ďo��
							ttxt.getstr( Func.Parameter[ Func.ParameterNum ].Name ) ;
							fprintf2( hd, hdW, "%s %s * %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
							fprintf2( sc, scW, "%s %s * %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
						}
					}
					else
					// �|�C���^�� unsigned int �̏ꍇ�͕��ʂɏo��
					if( Str2[ 0 ] == '*' || strcmp( Str2, "int" ) == 0 )
					{
						ttxt.getstr( Func.Parameter[ Func.ParameterNum ].Name ) ;
						fprintf2( hd, hdW, "%s %s %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
						fprintf2( sc, scW, "%s %s %s ", Str, Str2, Func.Parameter[ Func.ParameterNum ].Name ) ;
					}
					else
					// ����ȊO�̏ꍇ�����ʂɏo��
					{
						strcpy( Func.Parameter[ Func.ParameterNum ].Name, Str2 ) ;
						fprintf2( hd, hdW, "%s %s ", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;
						fprintf2( sc, scW, "%s %s ", Str, Func.Parameter[ Func.ParameterNum ].Name ) ;
					}
					Func.ParameterNum ++ ;

					ttxt.getstr( Str ) ;
					if( Str[ 0 ] == '=' )
					{
						// �f�t�H���g����������ꍇ�͋�؂蕶��������܂ŏo��
						for(;;)
						{
							fprintf2( hd, hdW, "%s ", Str ) ;
							ttxt.getstr( Str ) ;
							if( Str[ 0 ] == ',' || Str[ 0 ] == ')' )
								break ;
						}
						fprintf2( hd, hdW, "%s ", Str ) ;
						fprintf2( sc, scW, "%s ", Str ) ;
					}
					else
					{
						fprintf2( hd, hdW, "%s ", Str ) ;
						fprintf2( sc, scW, "%s ", Str ) ;
					}

					// ) ��������I��
					if( Str[ 0 ] == ')' ) break ;
					ttxt.getstr( Str ) ;
				}
				ttxt.getstr( Str ) ;
			}
			fprintf2( hd, hdW, ";\n" ) ;
			fprintf2( sc, scW, "\n" ) ;
		}

		// �֐��̒��g�̏o��
		{
			fprintf2( sc, scW, "{\n" ) ;

			// �߂�l�̌^�� void �ȊO�̏ꍇ�� return ��t����
			if( Func.Void )
			{
				fprintf2( sc, scW, "\t %s( ", Func.Name ) ;
			}
			else
			{
				fprintf2( sc, scW, "\treturn %s( ", Func.Name ) ;
			}

			// �����̏o��
			for( i = 0 ; i < Func.ParameterNum ; i ++ )
			{
				fprintf2( sc, scW, "%s ", Func.Parameter[ i ].Name ) ;
				if( i != Func.ParameterNum - 1 )
				{
					fprintf2( sc, scW, ", " ) ;
				}
			}
			fprintf2( sc, scW, " ) ;\n" ) ;
			fprintf2( sc, scW, "}\n" ) ;
		}
	}
	fprintf2( cs, csW, "\n\n" );
}

// �蓮�֐���`�̏o�͂��y�ɂ���֐�
void OutputCSFunctionDefine( FILE *cs, FILE *csW, char *FuncName, char *CSRet, char *CSParamDefine, bool Unsafe )
{
	fprintf(  cs,      "\t\t[DllImport(\"DxLib.dll\",     EntryPoint=\"%s\")]\n",                           FuncName );
	fprintf(  csW,     "\t\t[DllImport(\"DxLibW.dll\",     EntryPoint=\"%s\", CharSet=CharSet.Unicode)]\n", FuncName );
	fprintf2( cs, csW, "\t\textern %sstatic %s %s_x86%s;\n", Unsafe ? "unsafe " : "", CSRet, FuncName, CSParamDefine );
	fprintf(  cs,      "\t\t[DllImport(\"DxLib_x64.dll\", EntryPoint=\"%s\")]\n",                           FuncName );
	fprintf(  csW,     "\t\t[DllImport(\"DxLibW_x64.dll\", EntryPoint=\"%s\", CharSet=CharSet.Unicode)]\n", FuncName );
	fprintf2( cs, csW, "\t\textern %sstatic %s %s_x64%s;\n", Unsafe ? "unsafe " : "", CSRet, FuncName, CSParamDefine );
}

// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�
void OutputCSFunctionCode( FILE *cs, FILE *csW, char *FuncCode1, char *FuncCode2 )
{
	fprintf2( cs, csW, "\t\t{\n" );
	fprintf2( cs, csW, "\t\t\tif( System.IntPtr.Size == 4 )\n" );
	fprintf2( cs, csW, "\t\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t\t%s_x86%s;\n", FuncCode1, FuncCode2 );
	fprintf2( cs, csW, "\t\t\t}\n" );
	fprintf2( cs, csW, "\t\t\telse\n" );
	fprintf2( cs, csW, "\t\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t\t%s_x64%s;\n", FuncCode1, FuncCode2 );
	fprintf2( cs, csW, "\t\t\t}\n" );
	fprintf2( cs, csW, "\t\t}\n" );
}

// �蓮�֐��R�[�h�̏o�͂��y�ɂ���֐�
void OutputCSFunctionCode2( FILE *cs, FILE *csW, char *FuncCode1, char *FuncCode2 )
{
	fprintf2( cs, csW, "\t\t\tif( System.IntPtr.Size == 4 )\n" );
	fprintf2( cs, csW, "\t\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t\t%s_x86%s;\n", FuncCode1, FuncCode2 );
	fprintf2( cs, csW, "\t\t\t}\n" );
	fprintf2( cs, csW, "\t\t\telse\n" );
	fprintf2( cs, csW, "\t\t\t{\n" );
	fprintf2( cs, csW, "\t\t\t\t%s_x64%s;\n", FuncCode1, FuncCode2 );
	fprintf2( cs, csW, "\t\t\t}\n" );
}

// main�֐�
void main( int argc, char **argv )
{
	FILE *hd,  *sc,  *cs,  *def,  *def64;
	FILE *hdW, *scW, *csW, *defW, *def64W;
	
	// �p�����[�^�� 3 ����������w���v���o�͂��ďI��
	if( argc != 4 )
	{
		printf( "MakeDxLibDLLSource.exe DxLibHeaderPath DxFunctionWinHeaderPath DxDirectXHeaderPath\n" );
		return;
	}

	// DxLib.h �̓ǂݍ���
	if( dhtxt.load( argv[1] ) == false )
	{
		printf( "DxLib.h �̃I�[�v���Ɏ��s���܂���\n" ) ;
		return;
	}

	// DxFunctionWin.h �̓ǂݍ���
	if( dfhtxt.load( argv[2] ) == false )
	{
		printf( "DxLib.h �̃I�[�v���Ɏ��s���܂���\n" ) ;
		return;
	}

	// DxDirectX.h �̓ǂݍ���
	if( dxtxt.load( argv[3] ) == false )
	{
		printf( "DxDirectXb.h �̃I�[�v���Ɏ��s���܂���\n" ) ;
		return;
	}

	// �Ӗ��̂����؂�ɃX�y�[�X��������������̍쐬
	CreateTokenStr();

	// �o�͐�̃t�@�C�����J��
	hd     = fopen( "DxDLL.h", "wt" );
	hdW    = fopen( "DxDLLW.h", "wt" );
	sc     = fopen( "DxDLL.cpp", "wt" );
	scW    = fopen( "DxDLLW.cpp", "wt" );
	cs     = fopen( "DxDLL.cs", "wt" );
	csW    = fopen( "DxDLLW.cs", "wt" );
	def    = fopen( "DxDLL.def", "wt" ) ;
	defW   = fopen( "DxDLLW.def", "wt" ) ;
	def64  = fopen( "DxDLLx64.def", "wt" ) ;
	def64W = fopen( "DxDLLWx64.def", "wt" ) ;


	// �擪�����̏o��
	fprintf( def,    "LIBRARY DxLib\n\nEXPORTS\n" ) ;
	fprintf( defW,   "LIBRARY DxLibW\n\nEXPORTS\n" ) ;
	fprintf( def64,  "LIBRARY DxLib_x64\n\nEXPORTS\n" ) ;
	fprintf( def64W, "LIBRARY DxLibW_x64\n\nEXPORTS\n" ) ;

	fprintf2( hd, hdW, "#include \"DxLib.h\"\n" );
	fprintf2( hd, hdW, "extern \"C\"\n{\n " );
	fprintf(  sc,      "#include \"DxDLL.h\"\n" );
	fprintf(  scW,     "#include \"DxDLLW.h\"\n" );
	fprintf2( sc, scW, "#define DX_CHAR char\n" );

	fprintf2( cs, csW, "using System.Runtime.InteropServices;\n" );
	fprintf2( cs, csW, "namespace DxLibDLL\n" );
	fprintf2( cs, csW, "{\n" );
	fprintf2( cs, csW, "\tpublic static class DX\n" );
	fprintf2( cs, csW, "\t{\n" );

	// �萔��`�� C# �̃t�@�C���ɏo�͂���
	OutputDefineStr( cs, csW ) ;

	// �\���̕������o�͂���
	OutputStructStr( cs, csW ) ;

	// �֐��������o�͂���
	OutputFunctionStr( cs, csW, sc, scW, def, defW, def64, def64W, hd, hdW ) ;

	// �I�[�������o��
	fprintf2( hd, hdW, "}\n" );
	fprintf2( cs, csW, "\t}\n" );
	fprintf2( cs, csW, "}\n" );

	// �t�@�C�������
	fclose( hd );
	fclose( hdW );
	fclose( sc );
	fclose( scW );
	fclose( cs );
	fclose( csW );
	fclose( def ) ;
	fclose( defW ) ;
	fclose( def64 ) ;
	fclose( def64W ) ;

	// DxLib.h �̉��
	dhtxt.release() ;

	// DxDirectX.h �̉��
	dxtxt.release() ;
}




