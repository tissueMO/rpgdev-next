// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u�����A�[�J�C�o Ver5
// 
//	Creator			: �R�c �I
//	Creation Date	: 2003/09/11
//	Version			: 1.02
// 
// -------------------------------------------------------------------------------

#define INLINE_ASM

// include ----------------------------
#include "DXArchiveVer5.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>

// define -----------------------------

#define MIN_COMPRESS_VER5		(4)						// �Œሳ�k�o�C�g��
#define MAX_SEARCHLISTNUM_VER5	(64)					// �ő��v����T���ׂ̃��X�g��H��ő吔
#define MAX_SUBLISTNUM_VER5		(65536)					// ���k���ԒZ�k�̂��߂̃T�u���X�g�̍ő吔
#define MAX_COPYSIZE_VER5 		(0x1fff + MIN_COMPRESS_VER5)	// �Q�ƃA�h���X����R�s�[�o�؂�ő�T�C�Y( ���k�R�[�h���\���ł���R�s�[�T�C�Y�̍ő�l + �Œሳ�k�o�C�g�� )
#define MAX_ADDRESSLISTNUM_VER5	(1024 * 1024 * 1)		// �X���C�h�����̍ő�T�C�Y
#define MAX_POSITION_VER5		(1 << 24)				// �Q�Ɖ\�ȍő告�΃A�h���X( 16MB )

// struct -----------------------------

// ���k���ԒZ�k�p���X�g
typedef struct LZ_LIST_VER5
{
	LZ_LIST_VER5 *next, *prev ;
	u32 address ;
} LZ_LIST_VER5 ;

// class code -------------------------

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C���p�X�ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖���
int DXArchive_VER5::GetFilePathAndDirPath( char *Src, char *FilePath, char *DirPath )
{
	int i, Last ;
	
	// �t�@�C�����𔲂��o��
	i = 0 ;
	Last = -1 ;
	while( Src[i] != '\0' )
	{
		if( CheckMultiByteChar( &Src[i] ) == FALSE )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
	}
	if( FilePath != NULL )
	{
		if( Last != -1 ) strcpy( FilePath, &Src[Last+1] ) ;
		else strcpy( FilePath, Src ) ;
	}
	
	// �f�B���N�g���p�X�𔲂��o��
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			strncpy( DirPath, Src, Last ) ;
			DirPath[Last] = '\0' ;
		}
		else
		{
			DirPath[0] = '\0' ;
		}
	}
	
	// �I��
	return 0 ;
}

// �t�@�C���̏��𓾂�
DARC_FILEHEAD_VER5 *DXArchive_VER5::GetFileInfo( const char *FilePath )
{
	DARC_DIRECTORY_VER5 *OldDir ;
	DARC_FILEHEAD_VER5 *FileH ;
	u8 *NameData ;
	int i, j, k, Num, FileHeadSize ;
	SEARCHDATA SearchData ;

	// ���̃f�B���N�g����ۑ����Ă���
	OldDir = this->CurrentDirectory ;

	// �t�@�C���p�X�� \ ���܂܂�Ă���ꍇ�A�f�B���N�g���ύX���s��
	if( strchr( FilePath, '\\' ) != NULL )
	{
		// �J�����g�f�B���N�g����ړI�̃t�@�C��������f�B���N�g���ɕύX����
		if( this->ChangeCurrentDirectoryBase( FilePath, false, &SearchData ) >= 0 )
		{
			// �G���[���N���Ȃ������ꍇ�̓t�@�C�������f�B���N�g�����������ƂɂȂ�̂ŃG���[
			goto ERR ;
		}
	}
	else
	{
		// �t�@�C�����������p�f�[�^�ɕϊ�����
		ConvSearchData( &SearchData, FilePath, NULL ) ;
	}

	// �����̃t�@�C����T��
	FileHeadSize = this->Head.Version >= 0x0002 ? sizeof( DARC_FILEHEAD_VER5 ) : sizeof( DARC_FILEHEAD_VER1 ) ;
	FileH = ( DARC_FILEHEAD_VER5 * )( this->FileP + this->CurrentDirectory->FileHeadAddress ) ;
	Num = (s32)this->CurrentDirectory->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DARC_FILEHEAD_VER5 *)( (u8 *)FileH + FileHeadSize ) )
	{
		// �f�B���N�g���`�F�b�N
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ) continue ;

		// �����񐔂ƃp���e�B�`�F�b�N
		NameData = this->NameP + FileH->NameAddress ;
		if( SearchData.PackNum != ((u16 *)NameData)[0] || SearchData.Parity != ((u16 *)NameData)[1] ) continue ;

		// ������`�F�b�N
		NameData += 4 ;
		for( j = 0, k = 0 ; j < SearchData.PackNum ; j ++, k += 4 )
			if( *((u32 *)&SearchData.FileName[k]) != *((u32 *)&NameData[k]) ) break ;

		// �K�������t�@�C�����������炱���ŏI��
		if( SearchData.PackNum == j ) break ;
	}

	// ����������G���[
	if( i == Num ) goto ERR ;
	
	// �f�B���N�g�������ɖ߂�
	this->CurrentDirectory = OldDir ;
	
	// �ړI�̃t�@�C���̃A�h���X��Ԃ�
	return FileH ;
	
ERR :
	// �f�B���N�g�������ɖ߂�
	this->CurrentDirectory = OldDir ;
	
	// �G���[�I��
	return NULL ;

/*
	DARC_DIRECTORY_VER5 *OldCurrent ;
	DARC_FILEHEAD_VER5 *File ;
	char TPath[MAX_PATH], FPath[MAX_PATH], DPath[MAX_PATH] ;
	int i, len ;
		
	// �ꕶ�����Ȃ��ꍇ�͗�O����
	if( FilePath[0] == '\0' ) return ( DARC_FILEHEAD_VER5 * )FileP ;

	// ���̃J�����g�f�B���N�g����ۑ����Ă���
	OldCurrent = CurrentDirectory ;
	
	// �t�@�C���p�X�ƃf�B���N�g���p�X�𕪉�����
	strcpy( TPath, FilePath ) ;
	if( TPath[(len = strlen( TPath ))-1] == '\\' ) TPath[len-1] = '\0' ;
	GetFilePathAndDirPath( TPath, FPath, DPath ) ;
	
	// �J�����g�f�B���N�g���̕ύX
	if( ChangeCurrentDirectory( DPath ) == -1 ) goto ERR ;
	
	// �t�@�C���p�X�̕ύX
	File = ( DARC_FILEHEAD_VER5 * )( FileP + CurrentDirectory->FileHeadAddress ) ;
	for( 	i = 0 ;
			i < (int)CurrentDirectory->FileHeadNum && strcmp( ( const char * )( NameP + File->NameAddress ), FPath ) != 0 ;
			i ++, File ++ ){}
	if( i == (int)CurrentDirectory->FileHeadNum ) goto ERR ;

	// �J�����g�f�B���N�g�������ɖ߂�
	CurrentDirectory = OldCurrent ;

	// �������t�@�C���̏���Ԃ�
	return File ;	

ERR :
	// �J�����g�f�B���N�g�������ɖ߂�
	CurrentDirectory = OldCurrent ;
	
	return NULL ;
*/
}

// �A�[�J�C�u���̃J�����g�f�B���N�g���̏����擾����
DARC_DIRECTORY_VER5 *DXArchive_VER5::GetCurrentDirectoryInfo( void )
{
	return CurrentDirectory ;
}

// �ǂ��炪�V���������r����
DXArchive_VER5::DATE_RESULT DXArchive_VER5::DateCmp( DARC_FILETIME_VER5 *date1, DARC_FILETIME_VER5 *date2 )
{
	if( date1->LastWrite == date2->LastWrite ) return DATE_RESULT_DRAW ;
	else if( date1->LastWrite > date2->LastWrite ) return DATE_RESULT_LEFTNEW ;
	else return DATE_RESULT_RIGHTNEW ;
}

// ��r�ΏƂ̕����񒆂̑啶�����������Ƃ��Ĉ�����r����( 0:������  1:�Ⴄ )
int DXArchive_VER5::StrICmp( const char *Str1, const char *Str2 )
{
	int c1, c2 ;
	
	while( *Str1 != '\0' && *Str2 != '\0' )
	{
		if( CheckMultiByteChar( Str1 ) == FALSE )
		{
			c1 = ( *Str1 >= 'A' && *Str1 <= 'Z' ) ? *Str1 - 'A' + 'a' : *Str1 ;
			c2 = ( *Str2 >= 'A' && *Str2 <= 'Z' ) ? *Str2 - 'A' + 'a' : *Str2 ;
			if( c1 != c2 ) return 1 ;
			Str1 ++ ;
			Str2 ++ ;
		}
		else
		{
			if( *( (unsigned short *)Str1 ) != *( (unsigned short *)Str2 ) ) return 1 ;
			Str1 += 2 ;
			Str2 += 2 ;
		}
	}
	if( *Str1 != '\0' || *Str2 != '\0' ) return 1 ;

	// �����܂ŗ��ď��߂ē�����
	return 0 ;
}

// �����񒆂̉p���̏�������啶���ɕϊ�
int DXArchive_VER5::ConvSearchData( SEARCHDATA *SearchData, const char *Src, int *Length )
{
	int i, StringLength ;
	u16 ParityData ;

	ParityData = 0 ;
	for( i = 0 ; Src[i] != '\0' && Src[i] != '\\' ; )
	{
		if( CheckMultiByteChar( &Src[i] ) == TRUE )
		{
			// �Q�o�C�g�����̏ꍇ�͂��̂܂܃R�s�[
			*((u16 *)&SearchData->FileName[i]) = *((u16 *)&Src[i]) ;
			ParityData += (u8)Src[i] + (u8)Src[i+1] ;
			i += 2 ;
		}
		else
		{
			// �������̏ꍇ�͑啶���ɕϊ�
			if( Src[i] >= 'a' && Src[i] <= 'z' )	SearchData->FileName[i] = (u8)Src[i] - 'a' + 'A' ;
			else									SearchData->FileName[i] = Src[i] ;
			ParityData += (u8)SearchData->FileName[i] ;
			i ++ ;
		}
	}

	// ������̒�����ۑ�
	if( Length != NULL ) *Length = i ;

	// �S�̔{���̈ʒu�܂łO����
	StringLength = ( ( i + 1 ) + 3 ) / 4 * 4 ;
	memset( &SearchData->FileName[i], 0, StringLength - i ) ;

	// �p���e�B�f�[�^�̕ۑ�
	SearchData->Parity = ParityData ;

	// �p�b�N�f�[�^���̕ۑ�
	SearchData->PackNum = StringLength / 4 ;

	// ����I��
	return 0 ;
}

// �t�@�C�����f�[�^��ǉ�����( �߂�l�͎g�p�����f�[�^�o�C�g�� )
int DXArchive_VER5::AddFileNameData( const char *FileName, u8 *FileNameTable )
{
	int PackNum, Length, i ;
	u32 Parity ;

	// �T�C�Y���Z�b�g
	Length = strlen( FileName ) ;

	// �ꕶ�������������ꍇ�̏���
	if( Length == 0 )
	{
		// �p�b�N���ƃp���e�B���̂ݕۑ�
		*((u32 *)&FileNameTable[0]) = 0 ;

		// �g�p�T�C�Y��Ԃ�
		return 4 ;
	}
	Length ++ ;

	PackNum = ( Length + 3 ) / 4 ;

	// �p�b�N����ۑ�
	*((u16 *)&FileNameTable[0]) = PackNum ;

	// �o�b�t�@�̏�����
	memset( &FileNameTable[4], 0, PackNum * 4 * 2 ) ;

	// ��������R�s�[
	strcpy( (char *)&FileNameTable[4 + PackNum * 4], FileName ) ;

	// �p���̏�������S�đ啶���ɕϊ������t�@�C������ۑ�
	Parity = 0 ;
	for( i = 0 ; FileName[i] != '\0' ; )
	{
		// �Q�o�C�g�������ǂ����ŏ����𕪊�
		if( CheckMultiByteChar( &FileName[i] ) == TRUE )
		{
			// �Q�o�C�g����
			*((u16 *)&FileNameTable[4 + i]) = *((u16 *)&FileName[i]) ;
			Parity += (u8)FileName[i] + (u8)FileName[i+1] ;
			i += 2 ;
		}
		else
		{
			// �P�o�C�g����
			if( FileName[i] >= 'a' && FileName[i] <= 'z' )
			{
				// �������̏ꍇ�͑啶���ɕϊ�
				FileNameTable[4 + i] = (u8)FileName[i] - 'a' + 'A' ;
			}
			else
			{
				// �����ł͂Ȃ��ꍇ�͕��ʂɃR�s�[
				FileNameTable[4 + i] = (u8)FileName[i] ;
			}
			Parity += FileNameTable[4 + i] ;
			i ++ ;
		}
	}

	// �p���e�B����ۑ�
	*((u16 *)&FileNameTable[2]) = (u16)Parity ;

	// �g�p�����T�C�Y��Ԃ�
	return PackNum * 4 * 2 + 4 ;
}

// �t�@�C�����f�[�^���猳�̃t�@�C�����̕�������擾����
const char *DXArchive_VER5::GetOriginalFileName( u8 *FileNameTable )
{
	return (char *)FileNameTable + *((u16 *)&FileNameTable[0]) * 4 + 4 ;
}

// �f�[�^�𔽓]������֐�
void DXArchive_VER5::NotConv( void *Data , int Size )
{
	int DwordNum ;
	int ByteNum ;

	DwordNum = Size / 4 ;
	ByteNum = Size - DwordNum * 4 ;
#ifndef INLINE_ASM
	{
//		int i ;
		DWORD *dd ;
//		for( int i = 0 ; i < Size ; i ++ ) ( ( BYTE * )Data )[i] = ~( ( BYTE * )Data )[i] ;

		dd = ( DWORD * )Data ;
		if( DwordNum != 0 )
		{
			do
			{
				*dd++ = ~*dd ;
			}while( --DwordNum ) ;
		}
		if( ByteNum != 0 )
		{
			do
			{
				*((BYTE *)dd) = ~*((BYTE *)dd) ;
				dd = (DWORD *)(((BYTE *)dd) + 1) ;
			}while( --ByteNum ) ;
		}
	}
#else
	__asm
	{
		MOV ESI, Data
		MOV ECX, DwordNum
		CMP ECX, 0
		JE LABEL1
LOOP1:
		MOV EAX, [ESI]
		NOT EAX
		MOV [ESI], EAX
		ADD ESI, 4
		DEC ECX
		JNZ LOOP1

LABEL1:
		MOV ECX, ByteNum
		CMP ECX, 0
		JE END
LOOP2:
		MOV AL, [ESI]
		NOT AL
		MOV [ESI], AL
		INC ESI
		DEC ECX
		JNZ LOOP2
END:
	} ;
#endif
}


// �f�[�^�𔽓]�����ăt�@�C���ɏ����o���֐�
void DXArchive_VER5::NotConvFileWrite( void *Data, int Size, FILE *fp )
{
	// �f�[�^�𔽓]����
	NotConv( Data, Size ) ;

	// �����o��
	fwrite( Data, Size, 1, fp ) ;

	// �Ăє��]
	NotConv( Data, Size ) ;
}

// �f�[�^�𔽓]�����ăt�@�C������ǂݍ��ފ֐�
void DXArchive_VER5::NotConvFileRead( void *Data, int Size, FILE *fp )
{
	// �ǂݍ���
	fread( Data, 1, Size, fp ) ;

	// �f�[�^�𔽓]
	NotConv( Data, Size ) ;
}

// ����������쐬
void DXArchive_VER5::KeyCreate( const char *Source, unsigned char *Key )
{
	int Len ;

	if( Source == NULL )
	{
		memset( Key, 0xaaaaaaaa, DXA_KEYSTR_LENGTH_VER5 ) ;
	}
	else
	{
		Len = strlen( Source ) ;
		if( Len > DXA_KEYSTR_LENGTH_VER5 )
		{
			memcpy( Key, Source, DXA_KEYSTR_LENGTH_VER5 ) ;
		}
		else
		{
			// �������� DXA_KEYSTR_LENGTH_VER5 ���Z�������烋�[�v����
			int i ;

			for( i = 0 ; i + Len <= DXA_KEYSTR_LENGTH_VER5 ; i += Len )
				memcpy( Key + i, Source, Len ) ;
			if( i < DXA_KEYSTR_LENGTH_VER5 )
				memcpy( Key + i, Source, DXA_KEYSTR_LENGTH_VER5 - i ) ;
		}
	}

	Key[0] = ~Key[0] ;
	Key[1] = ( Key[1] >> 4 ) | ( Key[1] << 4 ) ;
	Key[2] = Key[2] ^ 0x8a ;
	Key[3] = ~( ( Key[3] >> 4 ) | ( Key[3] << 4 ) ) ;
	Key[4] = ~Key[4] ;
	Key[5] = Key[5] ^ 0xac ;
	Key[6] = ~Key[6] ;
	Key[7] = ~( ( Key[7] >> 3 ) | ( Key[7] << 5 ) ) ;
	Key[8] = ( Key[8] >> 5 ) | ( Key[8] << 3 ) ;
	Key[9] = Key[9] ^ 0x7f ;
	Key[10] = ( ( Key[10] >> 4 ) | ( Key[10] << 4 ) ) ^ 0xd6 ;
	Key[11] = Key[11] ^ 0xcc ;
}

// ����������g�p���� Xor ���Z( Key �͕K�� DXA_KEYSTR_LENGTH_VER5 �̒������Ȃ���΂Ȃ�Ȃ� )
void DXArchive_VER5::KeyConv( void *Data, int Size, int Position, unsigned char *Key )
{
	Position %= DXA_KEYSTR_LENGTH_VER5 ;

#ifndef INLINE_ASM
	int i, j ;

	j = Position ;
	for( i = 0 ; i < Size ; i ++ )
	{
		((u8 *)Data)[i] ^= Key[j] ;

		j ++ ;
		if( j == DXA_KEYSTR_LENGTH_VER5 ) j = 0 ;
	}
#else
	u32 DataT, SizeT ;
	SizeT = (u32)Size ;
	DataT = (u32)Data ;
	Position %= DXA_KEYSTR_LENGTH_VER5 ;
	__asm
	{
		MOV EDI, DataT
		MOV ESI, Key

		MOV EAX, SizeT
		CMP EAX, DXA_KEYSTR_LENGTH_VER5
		JB LABEL2


		MOV EAX, Position
		CMP EAX, 0
		JE LABEL1


		MOV EDX, SizeT
LOOP1:
		MOV BL, [ESI+EAX]
		XOR [EDI], BL
		INC EAX
		INC EDI
		DEC EDX
		CMP EAX, DXA_KEYSTR_LENGTH_VER5
		JB LOOP1
		XOR ECX, ECX
		MOV Position, ECX

		MOV SizeT, EDX
		CMP EDX, DXA_KEYSTR_LENGTH_VER5
		JB LABEL2


LABEL1:
		MOV EAX, SizeT
		XOR EDX, EDX
		MOV ECX, DXA_KEYSTR_LENGTH_VER5
		DIV ECX
		MOV SizeT, EDX
		MOV ECX, EAX

		MOV EAX, [ESI]
		MOV EBX, [ESI+4]
		MOV EDX, [ESI+8]
LOOP2:
		XOR [EDI],    EAX
		XOR [EDI+4],  EBX
		XOR [EDI+8],  EDX
		ADD EDI, DXA_KEYSTR_LENGTH_VER5
		DEC ECX
		JNZ LOOP2


LABEL2:
		MOV EDX, SizeT
		CMP EDX, 0
		JE LABEL3


		MOV EAX, Position
LOOP3:
		MOV BL, [ESI+EAX]
		XOR [EDI], BL
		INC EAX
		CMP EAX, DXA_KEYSTR_LENGTH_VER5
		JNE LABEL4
		XOR EAX, EAX
LABEL4:
		INC EDI
		DEC EDX
		JNZ LOOP3
LABEL3:
	} ;
#endif
}

// �f�[�^������������g�p���� Xor ���Z������t�@�C���ɏ����o���֐�( Key �͕K�� DXA_KEYSTR_LENGTH_VER5 �̒������Ȃ���΂Ȃ�Ȃ� )
void DXArchive_VER5::KeyConvFileWrite( void *Data, int Size, FILE *fp, unsigned char *Key, int Position )
{
	int pos ;

	// �t�@�C���̈ʒu���擾���Ă���
	if( Position == -1 ) pos = ftell( fp ) ;
	else                 pos = Position ;

	// �f�[�^������������g���� Xor ���Z����
	KeyConv( Data, Size, pos, Key ) ;

	// �����o��
	fwrite( Data, Size, 1, fp ) ;

	// �Ă� Xor ���Z
	KeyConv( Data, Size, pos, Key ) ;
}

// �t�@�C������ǂݍ��񂾃f�[�^������������g�p���� Xor ���Z����֐�( Key �͕K�� DXA_KEYSTR_LENGTH_VER5 �̒������Ȃ���΂Ȃ�Ȃ� )
void DXArchive_VER5::KeyConvFileRead( void *Data, int Size, FILE *fp, unsigned char *Key, int Position )
{
	int pos ;

	// �t�@�C���̈ʒu���擾���Ă���
	if( Position == -1 ) pos = ftell( fp ) ;
	else                 pos = Position ;

	// �ǂݍ���
	fread( Data, 1, Size, fp ) ;

	// �f�[�^������������g���� Xor ���Z
	KeyConv( Data, Size, pos, Key ) ;
}

/*
// �Q�o�C�g���������ׂ�( TRUE:�Q�o�C�g���� FALSE:�P�o�C�g���� )
int DXArchive_VER5::CheckMultiByteChar( const char *Buf )
{
	return  ( (unsigned char)*Buf >= 0x81 && (unsigned char)*Buf <= 0x9F ) || ( (unsigned char)*Buf >= 0xE0 && (unsigned char)*Buf <= 0xFC ) ;
}
*/

// �w��̃f�B���N�g���ɂ���t�@�C�����A�[�J�C�u�f�[�^�ɓf���o��
int DXArchive_VER5::DirectoryEncode( char *DirectoryName, u8 *NameP, u8 *DirP, u8 *FileP, DARC_DIRECTORY_VER5 *ParentDir, SIZESAVE *Size, int DataNumber, FILE *DestP, void *TempBuffer, bool Press, unsigned char *Key )
{
	char DirPath[MAX_PATH] ;
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle ;
	DARC_DIRECTORY_VER5 Dir ;
	DARC_FILEHEAD_VER5 File ;

	// �f�B���N�g���̏��𓾂�
	FindHandle = FindFirstFile( DirectoryName, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return 0 ;
	
	// �f�B���N�g�������i�[����t�@�C���w�b�_���Z�b�g����
	{
		File.NameAddress     = Size->NameSize ;
		File.Time.Create     = ( ( ( LONGLONG )FindData.ftCreationTime.dwHighDateTime ) << 32 ) + FindData.ftCreationTime.dwLowDateTime ;
		File.Time.LastAccess = ( ( ( LONGLONG )FindData.ftLastAccessTime.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime.dwLowDateTime ;
		File.Time.LastWrite  = ( ( ( LONGLONG )FindData.ftLastWriteTime.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime.dwLowDateTime ;
		File.Attributes      = FindData.dwFileAttributes ;
		File.DataAddress     = Size->DirectorySize ;
		File.DataSize        = 0 ;
		File.PressDataSize	 = 0xffffffff ;
	}

	// �f�B���N�g�����������o��
	Size->NameSize += AddFileNameData( FindData.cFileName, NameP + Size->NameSize ) ;

	// �f�B���N�g����񂪓������t�@�C���w�b�_�������o��
	memcpy( FileP + ParentDir->FileHeadAddress + DataNumber * sizeof( DARC_FILEHEAD_VER5 ),
			&File, sizeof( DARC_FILEHEAD_VER5 ) ) ;

	// Find �n���h�������
	FindClose( FindHandle ) ;

	// �w��̃f�B���N�g���ɃJ�����g�f�B���N�g�����ڂ�
	GetCurrentDirectory( MAX_PATH, DirPath ) ;
	SetCurrentDirectory( DirectoryName ) ;

	// �f�B���N�g�����̃Z�b�g
	{
		Dir.DirectoryAddress = ParentDir->FileHeadAddress + DataNumber * sizeof( DARC_FILEHEAD_VER5 ) ;
		Dir.FileHeadAddress  = Size->FileSize ;

		// �e�f�B���N�g���̏��ʒu���Z�b�g
		if( ParentDir->DirectoryAddress != 0xffffffff && ParentDir->DirectoryAddress != 0 )
		{
			Dir.ParentDirectoryAddress = ((DARC_FILEHEAD_VER5 *)( FileP + ParentDir->DirectoryAddress ))->DataAddress ;
		}
		else
		{
			Dir.ParentDirectoryAddress = 0 ;
		}

		// �f�B���N�g�����̃t�@�C���̐����擾����
		Dir.FileHeadNum = GetDirectoryFilePath( "", NULL ) ;
	}

	// �f�B���N�g���̏����o�͂���
	memcpy( DirP + Size->DirectorySize, &Dir, sizeof( DARC_DIRECTORY_VER5 ) ) ;	

	// �A�h���X�𐄈ڂ�����
	Size->DirectorySize += sizeof( DARC_DIRECTORY_VER5 ) ;
	Size->FileSize      += sizeof( DARC_FILEHEAD_VER5 ) * Dir.FileHeadNum ;
	
	// �t�@�C�������������ꍇ�͂����ŏI��
	if( Dir.FileHeadNum == 0 )
	{
		// ���Ƃ̃f�B���N�g�����J�����g�f�B���N�g���ɃZ�b�g
		SetCurrentDirectory( DirPath ) ;
		return 0 ;
	}

	// �t�@�C�������o�͂���
	{
		int i ;
		
		i = 0 ;
		
		// �񋓊J�n
		FindHandle = FindFirstFile( "*", &FindData ) ;
		do
		{
			// ��̃f�B���N�g���ɖ߂����肷�邽�߂̃p�X�͖�������
			if( strcmp( FindData.cFileName, "." ) == 0 || strcmp( FindData.cFileName, ".." ) == 0 ) continue ;

			// �t�@�C���ł͂Ȃ��A�f�B���N�g���������ꍇ�͍ċA����
			if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���������ꍇ�̏���
				if( DirectoryEncode( FindData.cFileName, NameP, DirP, FileP, &Dir, Size, i, DestP, TempBuffer, Press, Key ) < 0 ) return -1 ;
			}
			else
			{
				// �t�@�C���������ꍇ�̏���

				// �t�@�C���̃f�[�^���Z�b�g
				File.NameAddress     = Size->NameSize ;
				File.Time.Create     = ( ( ( LONGLONG )FindData.ftCreationTime.dwHighDateTime   ) << 32 ) + FindData.ftCreationTime.dwLowDateTime   ;
				File.Time.LastAccess = ( ( ( LONGLONG )FindData.ftLastAccessTime.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime.dwLowDateTime ;
				File.Time.LastWrite  = ( ( ( LONGLONG )FindData.ftLastWriteTime.dwHighDateTime  ) << 32 ) + FindData.ftLastWriteTime.dwLowDateTime  ;
				File.Attributes      = FindData.dwFileAttributes ;
				File.DataAddress     = Size->DataSize ;
				File.DataSize        = FindData.nFileSizeLow ;
				File.PressDataSize   = 0xffffffff ;

				// �t�@�C�����������o��
				Size->NameSize += AddFileNameData( FindData.cFileName, NameP + Size->NameSize ) ;
				
				// �t�@�C���f�[�^�������o��
				if( (u64)FindData.nFileSizeLow + ( (u64)FindData.nFileSizeHigh << 32 ) != 0 )
				{
					FILE *SrcP ;
					u32 FileSize, WriteSize, MoveSize ;

					// �t�@�C�����J��
					SrcP = fopen( FindData.cFileName, "rb" ) ;
					
					// �T�C�Y�𓾂�
					fseek( SrcP, 0, SEEK_END ) ;
					FileSize = ftell( SrcP ) ;
					fseek( SrcP, 0, SEEK_SET ) ;
					
					// �t�@�C���T�C�Y�� 10MB �ȉ��̏ꍇ�ŁA���k�̎w�肪����ꍇ�͈��k�����݂�
					if( Press == true && (u64)FindData.nFileSizeLow + ( (u64)FindData.nFileSizeHigh << 32 ) < 10 * 1024 * 1024 )
					{
						void *SrcBuf, *DestBuf ;
						u32 DestSize, Len ;
						
						// �ꕔ�̃t�@�C���`���̏ꍇ�͗\�ߒe��
						if( ( Len = strlen( FindData.cFileName ) ) > 4 )
						{
							char *sp ;
							
							sp = &FindData.cFileName[Len-3] ;
							if( StrICmp( sp, "wav" ) == 0 ||
								StrICmp( sp, "jpg" ) == 0 ||
								StrICmp( sp, "png" ) == 0 ||
								StrICmp( sp, "mpg" ) == 0 ||
								StrICmp( sp, "mp3" ) == 0 ||
								StrICmp( sp, "ogg" ) == 0 ||
								StrICmp( sp, "wmv" ) == 0 ||
								StrICmp( sp - 1, "jpeg" ) == 0 ) goto NOPRESS ;
						}
						
						// �f�[�^���ۂ��Ɠ��郁�����̈�̊m��
						SrcBuf  = malloc( FileSize + FileSize * 2 + 64 ) ;
						DestBuf = (u8 *)SrcBuf + FileSize ;
						
						// �t�@�C�����ۂ��Ɠǂݍ���
						fread( SrcBuf, FileSize, 1, SrcP ) ;
						
						// ���k
						DestSize = Encode( SrcBuf, FileSize, DestBuf ) ;
						
						// �w�ǈ��k�o���Ȃ������ꍇ�͈��k�����ŃA�[�J�C�u����
						if( (f64)DestSize / (f64)FileSize > 0.90 )
						{
							fseek( SrcP, 0L, SEEK_SET ) ;
							free( SrcBuf ) ;
							goto NOPRESS ;
						}
						
						// ���k�f�[�^�𔽓]���ď����o��
						WriteSize = ( DestSize + 3 ) / 4 * 4 ;
						KeyConvFileWrite( DestBuf, WriteSize, DestP, Key, File.DataSize ) ;
						
						// �������̉��
						free( SrcBuf ) ;
						
						// ���k�f�[�^�̃T�C�Y��ۑ�����
						File.PressDataSize = DestSize ;
					}
					else
					{
NOPRESS:					
						// �]���J�n
						WriteSize = 0 ;
						while( WriteSize < FileSize )
						{
							// �]���T�C�Y����
							MoveSize = DXA_BUFFERSIZE_VER5 < FileSize - WriteSize ? DXA_BUFFERSIZE_VER5 : FileSize - WriteSize ;
							MoveSize = ( MoveSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
							
							// �t�@�C���̔��]�ǂݍ���
							KeyConvFileRead( TempBuffer, MoveSize, SrcP, Key, File.DataSize + WriteSize ) ;

							// �����o��
							fwrite( TempBuffer, MoveSize, 1, DestP ) ;
							
							// �����o���T�C�Y�̉��Z
							WriteSize += MoveSize ;
						}
					}
					
					// �����o�����t�@�C�������
					fclose( SrcP ) ;
				
					// �f�[�^�T�C�Y�̉��Z
					Size->DataSize += WriteSize ;
				}
				
				// �t�@�C���w�b�_�������o��
				memcpy( FileP + Dir.FileHeadAddress + sizeof( DARC_FILEHEAD_VER5 ) * i, &File, sizeof( DARC_FILEHEAD_VER5 ) ) ;
			}
			
			i ++ ;
		}
		while( FindNextFile( FindHandle, &FindData ) != 0 ) ;
		
		// Find �n���h�������
		FindClose( FindHandle ) ;
	}
						
	// ���Ƃ̃f�B���N�g�����J�����g�f�B���N�g���ɃZ�b�g
	SetCurrentDirectory( DirPath ) ;

	// �I��
	return 0 ;
}

// �w��̃f�B���N�g���f�[�^�ɂ���t�@�C����W�J����
int DXArchive_VER5::DirectoryDecode( u8 *NameP, u8 *DirP, u8 *FileP, DARC_HEAD_VER5 *Head, DARC_DIRECTORY_VER5 *Dir, FILE *ArcP, unsigned char *Key )
{
	char DirPath[MAX_PATH] ;
	
	// ���݂̃J�����g�f�B���N�g����ۑ�
	GetCurrentDirectory( MAX_PATH, DirPath ) ;

	// �f�B���N�g����񂪂���ꍇ�́A�܂��W�J�p�̃f�B���N�g�����쐬����
	if( Dir->DirectoryAddress != 0xffffffff && Dir->ParentDirectoryAddress != 0xffffffff )
	{
		DARC_FILEHEAD_VER5 *DirFile ;
		
		// DARC_FILEHEAD_VER5 �̃A�h���X���擾
		DirFile = ( DARC_FILEHEAD_VER5 * )( FileP + Dir->DirectoryAddress ) ;
		
		// �f�B���N�g���̍쐬
		CreateDirectory( GetOriginalFileName( NameP + DirFile->NameAddress ), NULL ) ;
		
		// ���̃f�B���N�g���ɃJ�����g�f�B���N�g�����ڂ�
		SetCurrentDirectory( GetOriginalFileName( NameP + DirFile->NameAddress ) ) ;
	}

	// �W�J�����J�n
	{
		u32 i, FileHeadSize ;
		DARC_FILEHEAD_VER5 *File ;

		// �i�[����Ă���t�@�C���̐������J��Ԃ�
		FileHeadSize = Head->Version >= 0x0002 ? sizeof( DARC_FILEHEAD_VER5 ) : sizeof( DARC_FILEHEAD_VER1 ) ;
		File = ( DARC_FILEHEAD_VER5 * )( FileP + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = (DARC_FILEHEAD_VER5 *)( (u8 *)File + FileHeadSize ) )
		{
			// �f�B���N�g�����ǂ����ŏ����𕪊�
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���̏ꍇ�͍ċA��������
				DirectoryDecode( NameP, DirP, FileP, Head, ( DARC_DIRECTORY_VER5 * )( DirP + File->DataAddress ), ArcP, Key ) ;
			}
			else
			{
				FILE *DestP ;
				void *Buffer ;
			
				// �t�@�C���̏ꍇ�͓W�J����
				
				// �o�b�t�@���m�ۂ���
				Buffer = malloc( DXA_BUFFERSIZE_VER5 ) ;
				if( Buffer == NULL ) return -1 ;

				// �t�@�C�����J��
				DestP = fopen( GetOriginalFileName( NameP + File->NameAddress ), "wb" ) ;
				
				// �f�[�^������ꍇ�̂ݓ]��
				if( File->DataSize != 0 )
				{
					// �����ʒu���Z�b�g����
					if( ftell( ArcP ) != ( s32 )( Head->DataStartAddress + File->DataAddress ) )
						fseek( ArcP, Head->DataStartAddress + File->DataAddress, SEEK_SET ) ;
						
					// �f�[�^�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( Head->Version >= 0x0002 && File->PressDataSize != 0xffffffff )
					{
						void *temp ;
						
						// ���k����Ă���ꍇ

						// ���k�f�[�^�����܂郁�����̈�̊m��
						temp = malloc( File->PressDataSize + File->DataSize ) ;

						// ���k�f�[�^�̓ǂݍ���
						if( Head->Version >= 0x0005 )
						{
							KeyConvFileRead( temp, File->PressDataSize, ArcP, Key, File->DataSize ) ;
						}
						else
						{
							KeyConvFileRead( temp, File->PressDataSize, ArcP, Key ) ;
						}
						
						// ��
						Decode( temp, (u8 *)temp + File->PressDataSize ) ;
						
						// �����o��
						fwrite( (u8 *)temp + File->PressDataSize, File->DataSize, 1, DestP ) ;
						
						// �������̉��
						free( temp ) ;
					}
					else
					{
						// ���k����Ă��Ȃ��ꍇ
					
						// �]�������J�n
						{
							u32 MoveSize, WriteSize ;
							
							WriteSize = 0 ;
							while( WriteSize < File->DataSize )
							{
								MoveSize = File->DataSize - WriteSize > DXA_BUFFERSIZE_VER5 ? DXA_BUFFERSIZE_VER5 : File->DataSize - WriteSize ;

								// �t�@�C���̔��]�ǂݍ���
								if( Head->Version >= 0x0005 )
								{
									KeyConvFileRead( Buffer, MoveSize, ArcP, Key, File->DataSize + WriteSize ) ;
								}
								else
								{
									KeyConvFileRead( Buffer, MoveSize, ArcP, Key ) ;
								}

								// �����o��
								fwrite( Buffer, MoveSize, 1, DestP ) ;
								
								WriteSize += MoveSize ;
							}
						}
					}
				}
				
				// �t�@�C�������
				fclose( DestP ) ;

				// �o�b�t�@���J������
				free( Buffer ) ;

				// �t�@�C���̃^�C���X�^���v��ݒ肷��
				{
					HANDLE HFile ;
					FILETIME CreateTime, LastAccessTime, LastWriteTime ;

					HFile = CreateFile( GetOriginalFileName( NameP + File->NameAddress ),
										GENERIC_WRITE, 0, NULL,
										OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
					if( HFile == INVALID_HANDLE_VALUE )
					{
						HFile = HFile ;
					}

					CreateTime.dwHighDateTime     = ( u32 )( File->Time.Create     >> 32        ) ;
					CreateTime.dwLowDateTime      = ( u32 )( File->Time.Create     & 0xffffffff ) ;
					LastAccessTime.dwHighDateTime = ( u32 )( File->Time.LastAccess >> 32        ) ;
					LastAccessTime.dwLowDateTime  = ( u32 )( File->Time.LastAccess & 0xffffffff ) ;
					LastWriteTime.dwHighDateTime  = ( u32 )( File->Time.LastWrite  >> 32        ) ;
					LastWriteTime.dwLowDateTime   = ( u32 )( File->Time.LastWrite  & 0xffffffff ) ;
					SetFileTime( HFile, &CreateTime, &LastAccessTime, &LastWriteTime ) ;
					CloseHandle( HFile ) ;
				}

				// �t�@�C��������t����
				SetFileAttributes( GetOriginalFileName( NameP + File->NameAddress ), File->Attributes ) ;
			}
		}
	}
	
	// �J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectory( DirPath ) ;

	// �I��
	return 0 ;
}

// �f�B���N�g�����̃t�@�C���p�X���擾����
int DXArchive_VER5::GetDirectoryFilePath( const char *DirectoryPath, char *FileNameBuffer )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle ;
	int FileNum ;
	char DirPath[256], String[256] ;

	// �f�B���N�g�����ǂ������`�F�b�N����
	if( DirectoryPath[0] != '\0' )
	{
		FindHandle = FindFirstFile( DirectoryPath, &FindData ) ;
		if( FindHandle == INVALID_HANDLE_VALUE || ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 ) return -1 ;
		FindClose( FindHandle ) ;
	}

	// �w��̃t�H���_�̃t�@�C���̖��O���擾����
	FileNum = 0 ;
	if( DirectoryPath[0] != '\0' )
	{
		strcpy( DirPath, DirectoryPath ) ;
		if( DirPath[ strlen( DirPath ) - 1 ] != '\\' ) strcat( DirPath, "\\" ) ;
		strcpy( String, DirPath ) ;
		strcat( String, "*" ) ;
	}
	else
	{
		strcpy( DirPath, "" ) ;
		strcpy( String, "*" ) ;
	}
	FindHandle = FindFirstFile( String, &FindData ) ;
	if( FindHandle != INVALID_HANDLE_VALUE )
	{
		do
		{
			// ��̃f�B���N�g���ɖ߂����肷�邽�߂̃p�X�͖�������
			if( strcmp( FindData.cFileName, "." ) == 0 || strcmp( FindData.cFileName, ".." ) == 0 ) continue ;

			// �t�@�C���p�X��ۑ�����
			if( FileNameBuffer != NULL )
			{
				strcpy( FileNameBuffer, DirPath ) ;
				strcat( FileNameBuffer, FindData.cFileName ) ;
				FileNameBuffer += 256 ;
			}

			// �t�@�C���̐��𑝂₷
			FileNum ++ ;
		}
		while( FindNextFile( FindHandle, &FindData ) != 0 ) ;
		FindClose( FindHandle ) ;
	}

	// ����Ԃ�
	return FileNum ;
}

// �G���R�[�h( �߂�l:���k��̃T�C�Y  -1 �̓G���[  Dest �� NULL �����邱�Ƃ��\ )
int DXArchive_VER5::Encode( void *Src, unsigned int SrcSize, void *Dest )
{
	s32 dstsize ;
	s32    bonus,    conbo,    conbosize,    address,    addresssize ;
	s32 maxbonus, maxconbo, maxconbosize, maxaddress, maxaddresssize ;
	u8 keycode, *srcp, *destp, *dp, *sp, *sp2, *sp1 ;
	u32 srcaddress, nextprintaddress, code ;
	s32 j ;
	u32 i, m ;
	u32 maxlistnum, maxlistnummask, listaddp ;
	u32 sublistnum, sublistmaxnum ;
	LZ_LIST_VER5 *listbuf, *listtemp, *list, *newlist ;
	u8 *listfirsttable, *usesublistflagtable, *sublistbuf ;
	
	// �T�u���X�g�̃T�C�Y�����߂�
	{
			 if( SrcSize < 100 * 1024 )			sublistmaxnum = 1 ;
		else if( SrcSize < 3 * 1024 * 1024 )	sublistmaxnum = MAX_SUBLISTNUM_VER5 / 3 ;
		else									sublistmaxnum = MAX_SUBLISTNUM_VER5 ;
	}

	// ���X�g�̃T�C�Y�����߂�
	{
		maxlistnum = MAX_ADDRESSLISTNUM_VER5 ;
		if( maxlistnum > SrcSize )
		{
			while( ( maxlistnum >> 1 ) > 0x100 && ( maxlistnum >> 1 ) > SrcSize )
				maxlistnum >>= 1 ;
		}
		maxlistnummask = maxlistnum - 1 ;
	}

	// �������̊m��
	usesublistflagtable   = (u8 *)malloc(
		sizeof( u32 )     * 65536 +					// ���C�����X�g�̐擪�I�u�W�F�N�g�p�̈�
		sizeof( LZ_LIST_VER5 ) * maxlistnum +			// ���C�����X�g�p�̈�
		sizeof( u8 )      * 65536 +					// �T�u���X�g���g�p���Ă��邩�t���O�p�̈�
		sizeof( u32 )     * 256 * sublistmaxnum ) ;	// �T�u���X�g�p�̈�
		
	// �A�h���X�̃Z�b�g
	listfirsttable =     usesublistflagtable + sizeof(  u8 ) * 65536 ;
	sublistbuf     =          listfirsttable + sizeof( u32 ) * 65536 ;
	listbuf        = (LZ_LIST_VER5 *)( sublistbuf + sizeof( u32 ) * 256 * sublistmaxnum ) ;
	
	// ������
	memset( usesublistflagtable, 0, sizeof(  u8 ) * 65536               ) ;
	memset(          sublistbuf, 0, sizeof( u32 ) * 256 * sublistmaxnum ) ;
	memset(      listfirsttable, 0, sizeof( u32 ) * 65536               ) ;
	list = listbuf ;
	for( i = maxlistnum / 8 ; i ; i --, list += 8 )
	{
		list[0].address =
		list[1].address =
		list[2].address =
		list[3].address =
		list[4].address =
		list[5].address =
		list[6].address =
		list[7].address = 0xffffffff ;
	}

	srcp  = (u8 *)Src ;
	destp = (u8 *)Dest ;

	// ���k���f�[�^�̒��ň�ԏo���p�x���Ⴂ�o�C�g�R�[�h����������
	{
		u32 qnum, table[256], mincode ;

		for( i = 0 ; i < 256 ; i ++ )
			table[i] = 0 ;
		
		sp   = srcp ;
		qnum = SrcSize / 8 ;
		i    = qnum * 8 ;
		for( ; qnum ; qnum --, sp += 8 )
		{
			table[sp[0]] ++ ;
			table[sp[1]] ++ ;
			table[sp[2]] ++ ;
			table[sp[3]] ++ ;
			table[sp[4]] ++ ;
			table[sp[5]] ++ ;
			table[sp[6]] ++ ;
			table[sp[7]] ++ ;
		}
		for( ; i < SrcSize ; i ++, sp ++ )
			table[*sp] ++ ;
			
		keycode = 0 ;
		mincode = table[0] ;
		for( i = 1 ; i < 256 ; i ++ )
		{
			if( mincode < table[i] ) continue ;
			mincode = table[i] ;
			keycode = (u8)i ;
		}
	}

	// ���k���̃T�C�Y���Z�b�g
	((u32 *)destp)[0] = SrcSize ;

	// �L�[�R�[�h���Z�b�g
	destp[8] = keycode ;

	// ���k����
	dp               = destp + 9 ;
	sp               = srcp ;
	srcaddress       = 0 ;
	dstsize          = 0 ;
	listaddp         = 0 ;
	sublistnum       = 0 ;
	nextprintaddress = 1024 * 100 ;
	while( srcaddress < SrcSize )
	{
		// �c��T�C�Y���Œሳ�k�T�C�Y�ȉ��̏ꍇ�͈��k���������Ȃ�
		if( srcaddress + MIN_COMPRESS_VER5 >= SrcSize ) goto NOENCODE ;

		// ���X�g���擾
		code = *((u16 *)sp) ;
		list = (LZ_LIST_VER5 *)( listfirsttable + code * sizeof( u32 ) ) ;
		if( usesublistflagtable[code] == 1 )
		{
			list = (LZ_LIST_VER5 *)( (u32 *)list->next + sp[2] ) ;
		}
		else
		{
			if( sublistnum < sublistmaxnum )
			{
				list->next = (LZ_LIST_VER5 *)( sublistbuf + sizeof( u32 ) * 256 * sublistnum ) ;
				list       = (LZ_LIST_VER5 *)( (u32 *)list->next + sp[2] ) ;
			
				usesublistflagtable[code] = 1 ;
				sublistnum ++ ;
			}
		}

		// ��Ԉ�v���̒����R�[�h��T��
		maxconbo   = -1 ;
		maxaddress = -1 ;
		maxbonus   = -1 ;
		for( m = 0, listtemp = list->next ; m < MAX_SEARCHLISTNUM_VER5 && listtemp != NULL ; listtemp = listtemp->next, m ++ )
		{
			address = srcaddress - listtemp->address ;
			if( address >= MAX_POSITION_VER5 )
			{
				if( listtemp->prev ) listtemp->prev->next = listtemp->next ;
				if( listtemp->next ) listtemp->next->prev = listtemp->prev ;
				listtemp->address = 0xffffffff ;
				continue ;
			}
			
			sp2 = &sp[-address] ;
			sp1 = sp ;
			if( srcaddress + MAX_COPYSIZE_VER5 < SrcSize )
			{
				conbo = MAX_COPYSIZE_VER5 / 4 ;
				while( conbo && *((u32 *)sp2) == *((u32 *)sp1) )
				{
					sp2 += 4 ;
					sp1 += 4 ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE_VER5 - ( MAX_COPYSIZE_VER5 / 4 - conbo ) * 4 ;

				while( conbo && *sp2 == *sp1 )
				{
					sp2 ++ ;
					sp1 ++ ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE_VER5 - conbo ;
			}
			else
			{
				for( conbo = 0 ;
						conbo < MAX_COPYSIZE_VER5 &&
						conbo + srcaddress < SrcSize &&
						sp[conbo - address] == sp[conbo] ;
							conbo ++ ){}
			}

			if( conbo >= 4 )
			{
				conbosize   = ( conbo - MIN_COMPRESS_VER5 ) < 0x20 ? 0 : 1 ;
				addresssize = address < 0x100 ? 0 : ( address < 0x10000 ? 1 : 2 ) ;
				bonus       = conbo - ( 3 + conbosize + addresssize ) ;

				if( bonus > maxbonus )
				{
					maxconbo       = conbo ;
					maxaddress     = address ;
					maxaddresssize = addresssize ;
					maxconbosize   = conbosize ;
					maxbonus       = bonus ;
				}
			}
		}

		// ���X�g�ɓo�^
		newlist = &listbuf[listaddp] ;
		if( newlist->address != 0xffffffff )
		{
			if( newlist->prev ) newlist->prev->next = newlist->next ;
			if( newlist->next ) newlist->next->prev = newlist->prev ;
			newlist->address = 0xffffffff ;
		}
		newlist->address = srcaddress ;
		newlist->prev    = list ;
		newlist->next    = list->next ;
		if( list->next != NULL ) list->next->prev = newlist ;
		list->next       = newlist ;
		listaddp         = ( listaddp + 1 ) & maxlistnummask ;

		// ��v�R�[�h��������Ȃ�������񈳏k�R�[�h�Ƃ��ďo��
		if( maxconbo == -1 )
		{
NOENCODE:
			// �L�[�R�[�h�������ꍇ�͂Q��A���ŏo�͂���
			if( *sp == keycode )
			{
				if( destp != NULL )
				{
					dp[0]  =
					dp[1]  = keycode ;
					dp += 2 ;
				}
				dstsize += 2 ;
			}
			else
			{
				if( destp != NULL )
				{
					*dp = *sp ;
					dp ++ ;
				}
				dstsize ++ ;
			}
			sp ++ ;
			srcaddress ++ ;
		}
		else
		{
			// ���������ꍇ�͌������ʒu�ƒ������o�͂���
			
			// �L�[�R�[�h�ƌ������ʒu�ƒ������o��
			if( destp != NULL )
			{
				// �L�[�R�[�h�̏o��
				*dp++ = keycode ;

				// �o�͂���A�����͍Œ� MIN_COMPRESS_VER5 ���邱�Ƃ��O��Ȃ̂� - MIN_COMPRESS_VER5 �������̂��o�͂���
				maxconbo -= MIN_COMPRESS_VER5 ;

				// �A�����O�`�S�r�b�g�ƘA�����A���΃A�h���X�̃r�b�g�����o��
				*dp = (u8)( ( ( maxconbo & 0x1f ) << 3 ) | ( maxconbosize << 2 ) | maxaddresssize ) ;

				// �L�[�R�[�h�̘A���̓L�[�R�[�h�ƒl�̓������񈳏k�R�[�h��
				// ���f���邽�߁A�L�[�R�[�h�̒l�ȏ�̏ꍇ�͒l���{�P����
				if( *dp >= keycode ) dp[0] += 1 ;
				dp ++ ;

				// �A�����T�`�P�Q�r�b�g���o��
				if( maxconbosize == 1 )
					*dp++ = (u8)( ( maxconbo >> 5 ) & 0xff ) ;

				// maxconbo �͂܂��g������ - MIN_COMPRESS_VER5 ��������߂�
				maxconbo += MIN_COMPRESS_VER5 ;

				// �o�͂��鑊�΃A�h���X�͂O��( ���݂̃A�h���X�|�P )��}���̂ŁA�|�P�������̂��o�͂���
				maxaddress -- ;

				// ���΃A�h���X���o��
				*dp++ = (u8)( maxaddress ) ;
				if( maxaddresssize > 0 )
				{
					*dp++ = (u8)( maxaddress >> 8 ) ;
					if( maxaddresssize == 2 )
						*dp++ = (u8)( maxaddress >> 16 ) ;
				}
			}
			
			// �o�̓T�C�Y�����Z
			dstsize += 3 + maxaddresssize + maxconbosize ;
			
			// ���X�g�ɏ���ǉ�
			if( srcaddress + maxconbo < SrcSize )
			{
				sp2 = &sp[1] ;
				for( j = 1 ; j < maxconbo && (u32)&sp2[2] - (u32)srcp < SrcSize ; j ++, sp2 ++ )
				{
					code = *((u16 *)sp2) ;
					list = (LZ_LIST_VER5 *)( listfirsttable + code * sizeof( u32 ) ) ;
					if( usesublistflagtable[code] == 1 )
					{
						list = (LZ_LIST_VER5 *)( (u32 *)list->next + sp2[2] ) ;
					}
					else
					{
						if( sublistnum < sublistmaxnum )
						{
							list->next = (LZ_LIST_VER5 *)( sublistbuf + sizeof( u32 ) * 256 * sublistnum ) ;
							list       = (LZ_LIST_VER5 *)( (u32 *)list->next + sp2[2] ) ;
						
							usesublistflagtable[code] = 1 ;
							sublistnum ++ ;
						}
					}

					newlist = &listbuf[listaddp] ;
					if( newlist->address != 0xffffffff )
					{
						if( newlist->prev ) newlist->prev->next = newlist->next ;
						if( newlist->next ) newlist->next->prev = newlist->prev ;
						newlist->address = 0xffffffff ;
					}
					newlist->address = srcaddress + j ;
					newlist->prev = list ;
					newlist->next = list->next ;
					if( list->next != NULL ) list->next->prev = newlist ;
					list->next = newlist ;
					listaddp = ( listaddp + 1 ) & maxlistnummask ;
				}
			}
			
			sp         += maxconbo ;
			srcaddress += maxconbo ;
		}

		// ���k���̕\��
		if( nextprintaddress < srcaddress )
		{
			nextprintaddress = srcaddress + 100 * 1024 ;
		}
	}

	// ���k��̃f�[�^�T�C�Y��ۑ�����
	*((u32 *)&destp[4]) = dstsize + 9 ;

	// �m�ۂ����������̉��
	free( usesublistflagtable ) ;

	// �f�[�^�̃T�C�Y��Ԃ�
	return dstsize + 9 ;
}

// �f�R�[�h( �߂�l:�𓀌�̃T�C�Y  -1 �̓G���[  Dest �� NULL �����邱�Ƃ��\ )
int DXArchive_VER5::Decode( void *Src, void *Dest )
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
		conbo += MIN_COMPRESS_VER5 ;	// �ۑ����Ɍ��Z�����ŏ����k�o�C�g���𑫂�

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


// �A�[�J�C�u�t�@�C�����쐬����(�f�B���N�g�������)
int DXArchive_VER5::EncodeArchiveOneDirectory( char *OutputFileName, char *DirectoryPath, bool Press, const char *KeyString )
{
	int i, FileNum, Result ;
	char **FilePathList, *NameBuffer ;

	// �t�@�C���̐����擾����
	FileNum = GetDirectoryFilePath( DirectoryPath, NULL ) ;
	if( FileNum < 0 ) return -1 ;

	// �t�@�C���̐��̕������t�@�C�����ƃt�@�C���|�C���^�̊i�[�p�̃��������m�ۂ���
	NameBuffer = (char *)malloc( FileNum * ( 256 + 4 ) ) ;

	// �t�@�C���̃p�X���擾����
	GetDirectoryFilePath( DirectoryPath, NameBuffer ) ;

	// �t�@�C���p�X�̃��X�g���쐬����
	FilePathList = (char **)( NameBuffer + FileNum * 256 ) ;
	for( i = 0 ; i < FileNum ; i ++ )
		FilePathList[i] = NameBuffer + i * 256 ;

	// �G���R�[�h
	Result = EncodeArchive( OutputFileName, FilePathList, FileNum, Press, KeyString ) ;

	// �m�ۂ����������̉��
	free( NameBuffer ) ;

	// ���ʂ�Ԃ�
	return Result ;
}

// �A�[�J�C�u�t�@�C�����쐬����
int DXArchive_VER5::EncodeArchive( char *OutputFileName, char **FileOrDirectoryPath, int FileNum, bool Press, const char *KeyString )
{
	DARC_HEAD_VER5 Head ;
	DARC_DIRECTORY_VER5 Directory ;
	SIZESAVE SizeSave ;
	FILE *DestFp ;
	u8 *NameP, *FileP, *DirP ;
	int i ;
	u32 Type ;
	void *TempBuffer ;
	u8 Key[DXA_KEYSTR_LENGTH_VER5] ;

	// ��������̍쐬
	KeyCreate( KeyString, Key ) ;

	// �t�@�C���ǂݍ��݂Ɏg�p����o�b�t�@�̊m��
	TempBuffer = malloc( DXA_BUFFERSIZE_VER5 ) ;
	
	// �o�̓t�@�C�����J��
	DestFp = fopen( OutputFileName, "wb" ) ;

	// �A�[�J�C�u�̃w�b�_���o�͂���
	{
		Head.Head						= DXA_HEAD_VER5 ;
		Head.Version					= DXA_VER_VER5 ;
		Head.HeadSize					= 0xffffffff ;
		Head.DataStartAddress			= sizeof( DARC_HEAD_VER5 ) ;
		Head.FileNameTableStartAddress	= 0xffffffff ;
		Head.DirectoryTableStartAddress	= 0xffffffff ;
		Head.FileTableStartAddress		= 0xffffffff ;
		Head.CodePage					= GetACP() ;
		SetFileApisToANSI() ;

		KeyConvFileWrite( &Head, sizeof( DARC_HEAD_VER5 ), DestFp, Key, 0 ) ;
	}
	
	// �e�o�b�t�@���m�ۂ���
	if( ( NameP = ( u8 * )malloc( DXA_BUFFERSIZE_VER5 ) ) == NULL ) return -1 ;
	memset( NameP, 0, DXA_BUFFERSIZE_VER5 ) ;

	if( ( FileP = ( u8 * )malloc( DXA_BUFFERSIZE_VER5 ) ) == NULL ) return -1 ;
	memset( FileP, 0, DXA_BUFFERSIZE_VER5 ) ;

	if( ( DirP = ( u8 * )malloc( DXA_BUFFERSIZE_VER5 ) ) == NULL ) return -1 ;
	memset( DirP, 0, DXA_BUFFERSIZE_VER5 ) ;

	// �T�C�Y�ۑ��\���̂Ƀf�[�^���Z�b�g
	SizeSave.DataSize		= 0 ;
	SizeSave.NameSize		= 0 ;
	SizeSave.DirectorySize	= 0 ;
	SizeSave.FileSize		= 0 ;
	
	// �ŏ��̃f�B���N�g���̃t�@�C�����������o��
	{
		DARC_FILEHEAD_VER5 File ;
		
		memset( &File, 0, sizeof( DARC_FILEHEAD_VER5 ) ) ;
		File.NameAddress	= SizeSave.NameSize ;
		File.Attributes		= FILE_ATTRIBUTE_DIRECTORY ;
		File.DataAddress	= SizeSave.DirectorySize ;
		File.DataSize		= 0 ;
		File.PressDataSize  = 0xffffffff ;

		// �f�B���N�g�����̏����o��
		SizeSave.NameSize += AddFileNameData( "", NameP + SizeSave.NameSize ) ;

		// �t�@�C�����̏����o��
		memcpy( FileP + SizeSave.FileSize, &File, sizeof( DARC_FILEHEAD_VER5 ) ) ;
		SizeSave.FileSize += sizeof( DARC_FILEHEAD_VER5 ) ;
	}

	// �ŏ��̃f�B���N�g�����������o��
	Directory.DirectoryAddress 			= 0 ;
	Directory.ParentDirectoryAddress 	= 0xffffffff ;
	Directory.FileHeadNum 				= FileNum ;
	Directory.FileHeadAddress 			= SizeSave.FileSize ;
	memcpy( DirP + SizeSave.DirectorySize, &Directory, sizeof( DARC_DIRECTORY_VER5 ) ) ;

	// �T�C�Y�����Z����
	SizeSave.DirectorySize 	+= sizeof( DARC_DIRECTORY_VER5 ) ;
	SizeSave.FileSize 		+= sizeof( DARC_FILEHEAD_VER5 ) * FileNum ;

	// �n���ꂽ�t�@�C���̐������������J��Ԃ�
	for( i = 0 ; i < FileNum ; i ++ )
	{
		// �w�肳�ꂽ�t�@�C�������邩�ǂ�������
		Type = GetFileAttributes( FileOrDirectoryPath[i] ) ;
		if( ( signed int )Type == -1 ) continue ;

		// �t�@�C���̃^�C�v�ɂ���ď����𕪊�
		if( ( Type & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
		{
			// �f�B���N�g���̏ꍇ�̓f�B���N�g���̃A�[�J�C�u�ɉ�
			DirectoryEncode( FileOrDirectoryPath[i], NameP, DirP, FileP, &Directory, &SizeSave, i, DestFp, TempBuffer, Press, Key ) ;
		}
		else
		{
			WIN32_FIND_DATA FindData ;
			HANDLE FindHandle ;
			DARC_FILEHEAD_VER5 File ;
	
			// �t�@�C���̏��𓾂�
			FindHandle = FindFirstFile( FileOrDirectoryPath[i], &FindData ) ;
			if( FindHandle == INVALID_HANDLE_VALUE ) continue ;
			
			// �t�@�C���w�b�_���Z�b�g����
			{
				File.NameAddress     = SizeSave.NameSize ;
				File.Time.Create     = ( ( ( LONGLONG )FindData.ftCreationTime.dwHighDateTime   ) << 32 ) + FindData.ftCreationTime.dwLowDateTime   ;
				File.Time.LastAccess = ( ( ( LONGLONG )FindData.ftLastAccessTime.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime.dwLowDateTime ;
				File.Time.LastWrite  = ( ( ( LONGLONG )FindData.ftLastWriteTime.dwHighDateTime  ) << 32 ) + FindData.ftLastWriteTime.dwLowDateTime  ;
				File.Attributes      = FindData.dwFileAttributes ;
				File.DataAddress     = SizeSave.DataSize ;
				File.DataSize        = FindData.nFileSizeLow ;
				File.PressDataSize	 = 0xffffffff ;
			}

			// �t�@�C�����������o��
			SizeSave.NameSize += AddFileNameData( FindData.cFileName, NameP + SizeSave.NameSize ) ;

			// �t�@�C���f�[�^�������o��
			if( (u64)FindData.nFileSizeLow + ( (u64)FindData.nFileSizeHigh << 32 ) != 0 )
			{
				FILE *SrcP ;
				u32 FileSize, WriteSize, MoveSize ;

				// �t�@�C�����J��
				SrcP = fopen( FileOrDirectoryPath[i], "rb" ) ;
				
				// �T�C�Y�𓾂�
				fseek( SrcP, 0, SEEK_END ) ;
				FileSize = ftell( SrcP ) ;
				fseek( SrcP, 0, SEEK_SET ) ;
				
				// �t�@�C���T�C�Y�� 10MB �ȉ��̏ꍇ�ŁA���k�̎w�肪����ꍇ�͈��k�����݂�
				if( Press == true && (u64)FindData.nFileSizeLow + ( (u64)FindData.nFileSizeHigh << 32 ) < 10 * 1024 * 1024 )
				{
					void *SrcBuf, *DestBuf ;
					u32 DestSize, Len ;
					
					// �ꕔ�̃t�@�C���`���̏ꍇ�͗\�ߒe��
					if( ( Len = strlen( FindData.cFileName ) ) > 4 )
					{
						char *sp ;
						
						sp = &FindData.cFileName[Len-3] ;
						if( StrICmp( sp, "wav" ) == 0 ||
							StrICmp( sp, "jpg" ) == 0 ||
							StrICmp( sp, "png" ) == 0 ||
							StrICmp( sp, "mpg" ) == 0 ||
							StrICmp( sp, "mp3" ) == 0 ||
							StrICmp( sp, "ogg" ) == 0 ||
							StrICmp( sp, "wmv" ) == 0 ||
							StrICmp( sp - 1, "jpeg" ) == 0 ) goto NOPRESS ;
					}
					
					// �f�[�^���ۂ��Ɠ��郁�����̈�̊m��
					SrcBuf  = malloc( FileSize + FileSize * 2 + 64 ) ;
					DestBuf = (u8 *)SrcBuf + FileSize ;
					
					// �t�@�C�����ۂ��Ɠǂݍ���
					fread( SrcBuf, FileSize, 1, SrcP ) ;
					
					// ���k
					DestSize = Encode( SrcBuf, FileSize, DestBuf ) ;
					
					// �w�ǈ��k�o���Ȃ������ꍇ�͈��k�����ŃA�[�J�C�u����
					if( (f64)DestSize / (f64)FileSize > 0.90 )
					{
						fseek( SrcP, 0L, SEEK_SET ) ;
						free( SrcBuf ) ;
						goto NOPRESS ;
					}
					
					// ���k�f�[�^�𔽓]���ď����o��
					WriteSize = ( DestSize + 3 ) / 4 * 4 ;
					KeyConvFileWrite( DestBuf, WriteSize, DestFp, Key, File.DataSize ) ;
					
					// �������̉��
					free( SrcBuf ) ;
					
					// ���k�f�[�^�̃T�C�Y��ۑ�����
					File.PressDataSize = DestSize ;
				}
				else
				{
NOPRESS:					
					// �]���J�n
					WriteSize = 0 ;
					while( WriteSize < FileSize )
					{
						// �]���T�C�Y����
						MoveSize = DXA_BUFFERSIZE_VER5 < FileSize - WriteSize ? DXA_BUFFERSIZE_VER5 : FileSize - WriteSize ;
						MoveSize = ( MoveSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
						
						// �t�@�C���̔��]�ǂݍ���
						KeyConvFileRead( TempBuffer, MoveSize, SrcP, Key, File.DataSize + WriteSize ) ;

						// �����o��
						fwrite( TempBuffer, MoveSize, 1, DestFp ) ;
						
						// �����o���T�C�Y�̉��Z
						WriteSize += MoveSize ;
					}
				}
				
				// �����o�����t�@�C�������
				fclose( SrcP ) ;
			
				// �f�[�^�T�C�Y�̉��Z
				SizeSave.DataSize += WriteSize ;
			}
			
			// �t�@�C���w�b�_�������o��
			memcpy( FileP + Directory.FileHeadAddress + sizeof( DARC_FILEHEAD_VER5 ) * i, &File, sizeof( DARC_FILEHEAD_VER5 ) ) ;

			// Find �n���h�������
			FindClose( FindHandle ) ;
		}
	}
	
	// �o�b�t�@�ɗ��ߍ��񂾊e��w�b�_�f�[�^���o�͂���
	{
		// �o�͂��鏇�Ԃ� �t�@�C���l�[���e�[�u���A DARC_FILEHEAD_VER5 �e�[�u���A DARC_DIRECTORY_VER5 �e�[�u�� �̏�
		KeyConvFileWrite( NameP, SizeSave.NameSize,      DestFp, Key, 0 ) ;
		KeyConvFileWrite( FileP, SizeSave.FileSize,      DestFp, Key, SizeSave.NameSize ) ;
		KeyConvFileWrite( DirP,  SizeSave.DirectorySize, DestFp, Key, SizeSave.NameSize + SizeSave.FileSize ) ;
	}
		
	// �ĂуA�[�J�C�u�̃w�b�_���o�͂���
	{
		Head.Head                       = DXA_HEAD_VER5 ;
		Head.Version                    = DXA_VER_VER5 ;
		Head.HeadSize                   = SizeSave.NameSize + SizeSave.DirectorySize + SizeSave.FileSize  ;
		Head.DataStartAddress           = sizeof( DARC_HEAD_VER5 ) ;
		Head.FileNameTableStartAddress  = SizeSave.DataSize + Head.DataStartAddress ;
		Head.FileTableStartAddress      = SizeSave.NameSize ;
		Head.DirectoryTableStartAddress = Head.FileTableStartAddress + SizeSave.FileSize ;

		fseek( DestFp, 0, SEEK_SET ) ;
		KeyConvFileWrite( &Head, sizeof( DARC_HEAD_VER5 ), DestFp, Key, 0 ) ;
	}
	
	// �����o�����t�@�C�������
	fclose( DestFp ) ;
	
	// �m�ۂ����o�b�t�@���J������
	free( NameP ) ;
	free( FileP ) ;
	free( DirP ) ;
	free( TempBuffer ) ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C����W�J����
int DXArchive_VER5::DecodeArchive( char *ArchiveName, char *OutputPath, const char *KeyString )
{
	u8 *HeadBuffer = NULL ;
	DARC_HEAD_VER5 Head ;
	u8 *FileP, *NameP, *DirP ;
	FILE *ArcP = NULL ;
	char OldDir[MAX_PATH] ;
	u8 Key[DXA_KEYSTR_LENGTH_VER5] ;

	// ��������̍쐬
	KeyCreate( KeyString, Key ) ;

	// �A�[�J�C�u�t�@�C�����J��
	ArcP = fopen( ArchiveName, "rb" ) ;
	if( ArcP == NULL ) return -1 ;

	// �o�͐�̃f�B���N�g���ɃJ�����g�f�B���N�g����ύX����
	GetCurrentDirectory( MAX_PATH, OldDir ) ;
	SetCurrentDirectory( OutputPath ) ;

	// �w�b�_����͂���
	{
		KeyConvFileRead( &Head, sizeof( DARC_HEAD_VER5 ), ArcP, Key, 0 ) ;

		// �h�c�̌���
		if( Head.Head != DXA_HEAD_VER5 )
		{
			// �o�[�W�����Q�ȑO�����ׂ�
			memset( Key, 0xffffffff, DXA_KEYSTR_LENGTH_VER5 ) ;

			fseek( ArcP, 0L, SEEK_SET ) ;
			KeyConvFileRead( &Head, sizeof( DARC_HEAD_VER5 ), ArcP, Key, 0 ) ;

			// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
			if( Head.Head != DXA_HEAD_VER5 )
				goto ERR ;
		}
		
		// �o�[�W��������
		if( Head.Version > DXA_VER_VER5 ) goto ERR ;
		
		// �w�b�_�̃T�C�Y���̃��������m�ۂ���
		HeadBuffer = ( u8 * )malloc( Head.HeadSize ) ;
		if( HeadBuffer == NULL ) goto ERR ;
		
		// �w�b�_�p�b�N���������ɓǂݍ���
		fseek( ArcP, Head.FileNameTableStartAddress, SEEK_SET ) ;
		if( Head.Version >= 0x0005 )
		{
			KeyConvFileRead( HeadBuffer, Head.HeadSize, ArcP, Key, 0 ) ;
		}
		else
		{
			KeyConvFileRead( HeadBuffer, Head.HeadSize, ArcP, Key ) ;
		}
		
		// �e�A�h���X���Z�b�g����
		NameP = HeadBuffer ;
		FileP = NameP + Head.FileTableStartAddress ;
		DirP  = NameP + Head.DirectoryTableStartAddress ;
	}

	// �A�[�J�C�u�̓W�J���J�n����
	DirectoryDecode( NameP, DirP, FileP, &Head, ( DARC_DIRECTORY_VER5 * )DirP, ArcP, Key ) ;
	
	// �t�@�C�������
	fclose( ArcP ) ;
	
	// �w�b�_��ǂݍ���ł������������������
	free( HeadBuffer ) ;

	// �J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectory( OldDir ) ;

	// �I��
	return 0 ;

ERR :
	if( HeadBuffer != NULL ) free( HeadBuffer ) ;
	if( ArcP != NULL ) fclose( ArcP ) ;

	// �J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectory( OldDir ) ;

	// �I��
	return -1 ;
}



// �R���X�g���N�^
DXArchive_VER5::DXArchive_VER5( char *ArchivePath )
{
	this->fp = NULL ;
	this->HeadBuffer = NULL ;
	this->NameP = this->DirP = this->FileP = NULL ;
	this->CurrentDirectory = NULL ;
	this->CashBuffer = NULL ;

	if( ArchivePath != NULL )
	{
		this->OpenArchiveFile( ArchivePath ) ;
	}
}

// �f�X�g���N�^
DXArchive_VER5::~DXArchive_VER5()
{
	if( this->fp != NULL ) this->CloseArchiveFile() ;

	if( this->fp != NULL ) fclose( this->fp ) ;
	if( this->HeadBuffer != NULL ) free( this->HeadBuffer ) ;
	if( this->CashBuffer != NULL ) free( this->CashBuffer ) ;

	this->fp = NULL ;
	this->HeadBuffer = NULL ;
	this->NameP = this->DirP = this->FileP = NULL ;
	this->CurrentDirectory = NULL ;
}

// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )
int DXArchive_VER5::DirectoryKeyConv( DARC_DIRECTORY_VER5 *Dir )
{
	// �������C���[�W�ł͂Ȃ��ꍇ�̓G���[
	if( this->MemoryOpenFlag == false )
		return -1 ;

	// �o�[�W���� 0x0005 ���O�ł͉������Ȃ�
	if( this->Head.Version < 0x0005 )
		return 0 ;
	
	// �Í������������J�n
	{
		u32 i, FileHeadSize ;
		DARC_FILEHEAD_VER5 *File ;

		// �i�[����Ă���t�@�C���̐������J��Ԃ�
		FileHeadSize = this->Head.Version >= 0x0002 ? sizeof( DARC_FILEHEAD_VER5 ) : sizeof( DARC_FILEHEAD_VER1 ) ;
		File = ( DARC_FILEHEAD_VER5 * )( this->FileP + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = (DARC_FILEHEAD_VER5 *)( (u8 *)File + FileHeadSize ) )
		{
			// �f�B���N�g�����ǂ����ŏ����𕪊�
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���̏ꍇ�͍ċA��������
				DirectoryKeyConv( ( DARC_DIRECTORY_VER5 * )( this->DirP + File->DataAddress ) ) ;
			}
			else
			{
				u8 *DataP ;

				// �t�@�C���̏ꍇ�͈Í�������������
				
				// �f�[�^������ꍇ�̂ݏ���
				if( File->DataSize != 0 )
				{
					// �f�[�^�ʒu���Z�b�g����
					DataP = ( u8 * )this->fp + this->Head.DataStartAddress + File->DataAddress ;

					// �f�[�^�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( this->Head.Version >= 0x0002 && File->PressDataSize != 0xffffffff )
					{
						// ���k����Ă���ꍇ
						KeyConv( DataP, File->PressDataSize, File->DataSize, this->Key ) ;
					}
					else
					{
						// ���k����Ă��Ȃ��ꍇ
						KeyConv( DataP, File->DataSize, File->DataSize, this->Key ) ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// ��������ɂ���A�[�J�C�u�C���[�W���J��( 0:����  -1:���s )
int	DXArchive_VER5::OpenArchiveMem( void *ArchiveImage, int ArchiveSize, const char *KeyString )
{
	u8 *datp ;

	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( this->fp != NULL ) return -1 ;

	// ���̍쐬
	KeyCreate( KeyString, this->Key ) ;

	// �ŏ��Ƀw�b�_�̕����𔽓]����
	memcpy( &this->Head, ArchiveImage, sizeof( DARC_HEAD_VER5 ) ) ;
	KeyConv( &this->Head, sizeof( DARC_HEAD_VER5 ), 0, this->Key ) ;

	// �h�c���Ⴄ�ꍇ�̓o�[�W�����Q�ȑO�����ׂ�
	if( Head.Head != DXA_HEAD_VER5 )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		memset( this->Key, 0xffffffff, DXA_KEYSTR_LENGTH_VER5 ) ;

		memcpy( &this->Head, ArchiveImage, sizeof( DARC_HEAD_VER5 ) ) ;
		KeyConv( &this->Head, sizeof( DARC_HEAD_VER5 ), 0, this->Key ) ;

		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( Head.Head != DXA_HEAD_VER5 )
			return -1 ;
	}

	if( Head.Version < 0x0005 )
	{
		// ���ׂẴf�[�^�𔽓]����
		KeyConv( ArchiveImage, ArchiveSize, 0, this->Key ) ;
	}

	// �|�C���^��ۑ�
	this->fp = (FILE *)ArchiveImage ;
	datp = (u8 *)ArchiveImage ;

	// �w�b�_����͂���
	{
		if( Head.Version >= 0x0005 )
		{
			memcpy( &this->Head, datp, sizeof( DARC_HEAD_VER5 ) ) ;
			KeyConv( &this->Head, sizeof( DARC_HEAD_VER5 ), 0, this->Key ) ;
		}
		else
		{
			memcpy( &this->Head, datp, sizeof( DARC_HEAD_VER5 ) ) ;
		}

		datp += sizeof( DARC_HEAD_VER5 ) ;

		// �h�c�̌���
		if( this->Head.Head != DXA_HEAD_VER5 ) goto ERR ;
		
		// �o�[�W��������
		if( this->Head.Version > DXA_VER_VER5 ) goto ERR ;

		// �w�b�_�p�b�N�̃A�h���X���Z�b�g����
		this->HeadBuffer = (u8 *)this->fp + this->Head.FileNameTableStartAddress ;

		// �e�A�h���X���Z�b�g����
		if( Head.Version >= 0x0005 )
		{
			KeyConv( this->HeadBuffer, this->Head.HeadSize, 0, this->Key ) ;
		}
		this->NameP = this->HeadBuffer ;
		this->FileP = this->NameP + this->Head.FileTableStartAddress ;
		this->DirP = this->NameP + this->Head.DirectoryTableStartAddress ;
	}

	// �J�����g�f�B���N�g���̃Z�b�g
	this->CurrentDirectory = ( DARC_DIRECTORY_VER5 * )this->DirP ;

	// �������C���[�W����J���Ă���t���O�𗧂Ă�
	MemoryOpenFlag = true ;

	// �S�Ẵt�@�C���̈Í�������������
	if( this->Head.Version >= 0x0005 )
	{
		DirectoryKeyConv( ( DARC_DIRECTORY_VER5 * )this->DirP ) ;
	}

	// ���[�U�[�̃C���[�W����J�����t���O�𗧂Ă�
	UserMemoryImageFlag = true ;

	// �T�C�Y���ۑ����Ă���
	MemoryImageSize = ArchiveSize ;

	// �I��
	return 0 ;

ERR :
	if( Head.Version < 0x0005 )
	{
		// ���]�����f�[�^�����ɖ߂�
		KeyConv( ArchiveImage, ArchiveSize, 0, this->Key ) ;
	}

	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�����J���ŏ��ɂ��ׂă�������ɓǂݍ���ł��珈������( 0:����  -1:���s )
int DXArchive_VER5::OpenArchiveFileMem( const char *ArchivePath, const char *KeyString )
{
	FILE *fp ;
	u8 *datp ;
	void *ArchiveImage ;
	int ArchiveSize ;

	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( this->fp != NULL ) return -1 ;

	// ���̍쐬
	KeyCreate( KeyString, this->Key ) ;

	// �������ɓǂݍ���
	{
		fp = fopen( ArchivePath, "rb" ) ;
		if( fp == NULL ) return -1 ;
		fseek( fp, 0L, SEEK_END ) ;
		ArchiveSize = ftell( fp ) ;
		fseek( fp, 0L, SEEK_SET ) ;
		ArchiveImage = malloc( ArchiveSize ) ;
		if( ArchiveImage == NULL )
		{
			fclose( fp ) ;
			return -1 ;
		}
		fread( ArchiveImage, ArchiveSize, 1, fp ) ;
		fclose( fp ) ;
	}

	// �ŏ��Ƀw�b�_�̕����𔽓]����
	memcpy( &this->Head, ArchiveImage, sizeof( DARC_HEAD_VER5 ) ) ;
	KeyConv( &this->Head, sizeof( DARC_HEAD_VER5 ), 0, this->Key ) ;

	// �h�c���Ⴄ�ꍇ�̓o�[�W�����Q�ȑO�����ׂ�
	if( Head.Head != DXA_HEAD_VER5 )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		memset( this->Key, 0xffffffff, DXA_KEYSTR_LENGTH_VER5 ) ;

		memcpy( &this->Head, ArchiveImage, sizeof( DARC_HEAD_VER5 ) ) ;
		KeyConv( &this->Head, sizeof( DARC_HEAD_VER5 ), 0, this->Key ) ;

		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( Head.Head != DXA_HEAD_VER5 )
			return -1 ;
	}

	// ���ׂẴf�[�^�𔽓]����
	if( Head.Version < 0x0005 )
	{
		KeyConv( ArchiveImage, ArchiveSize, 0, this->Key ) ;
	}

	// �|�C���^��ۑ�
	this->fp = (FILE *)ArchiveImage ;
	datp = (u8 *)ArchiveImage ;

	// �w�b�_����͂���
	{
		if( Head.Version >= 0x0005 )
		{
			memcpy( &this->Head, datp, sizeof( DARC_HEAD_VER5 ) ) ;
			KeyConv( &this->Head, sizeof( DARC_HEAD_VER5 ), 0, this->Key ) ;
		}
		else
		{
			memcpy( &this->Head, datp, sizeof( DARC_HEAD_VER5 ) ) ;
		}

		datp += sizeof( DARC_HEAD_VER5 ) ;
		
		// �h�c�̌���
		if( this->Head.Head != DXA_HEAD_VER5 ) goto ERR ;
		
		// �o�[�W��������
		if( this->Head.Version > DXA_VER_VER5 ) goto ERR ;

		// �w�b�_�p�b�N�̃A�h���X���Z�b�g����
		this->HeadBuffer = (u8 *)this->fp + this->Head.FileNameTableStartAddress ;

		// �e�A�h���X���Z�b�g����
		if( Head.Version >= 0x0005 )
		{
			KeyConv( this->HeadBuffer, this->Head.HeadSize, 0, this->Key ) ;
		}
		this->NameP = this->HeadBuffer ;
		this->FileP = this->NameP + this->Head.FileTableStartAddress ;
		this->DirP = this->NameP + this->Head.DirectoryTableStartAddress ;
	}

	// �J�����g�f�B���N�g���̃Z�b�g
	this->CurrentDirectory = ( DARC_DIRECTORY_VER5 * )this->DirP ;

	// �������C���[�W����J���Ă���t���O�𗧂Ă�
	MemoryOpenFlag = true ;

	// �S�Ẵt�@�C���̈Í�������������
	if( this->Head.Version >= 0x0005 )
	{
		DirectoryKeyConv( ( DARC_DIRECTORY_VER5 * )this->DirP ) ;
	}
	
	// ���[�U�[�̃C���[�W����J�����킯�ł͂Ȃ��̂Ńt���O��|��
	UserMemoryImageFlag = false ;

	// �T�C�Y���ۑ����Ă���
	MemoryImageSize = ArchiveSize ;

	// �I��
	return 0 ;

ERR :
	if( Head.Version < 0x0005 )
	{
		// ���]�����f�[�^�����ɖ߂�
		KeyConv( ArchiveImage, ArchiveSize, 0, this->Key ) ;
	}
	
	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�����J��
int DXArchive_VER5::OpenArchiveFile( const char *ArchivePath, const char *KeyString )
{
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( this->fp != NULL ) return -1 ;

	// �A�[�J�C�u�t�@�C�����J�����Ǝ��݂�
	this->fp = fopen( ArchivePath, "rb" ) ;
	if( this->fp == NULL ) return -1 ;

	// ��������̍쐬
	KeyCreate( KeyString, this->Key ) ;

	// �w�b�_����͂���
	{
		KeyConvFileRead( &this->Head, sizeof( DARC_HEAD_VER5 ), this->fp, this->Key, 0 ) ;
		
		// �h�c�̌���
		if( this->Head.Head != DXA_HEAD_VER5 )
		{
			// �o�[�W�����Q�ȑO�����ׂ�
			memset( this->Key, 0xffffffff, DXA_KEYSTR_LENGTH_VER5 ) ;

			fseek( this->fp, 0L, SEEK_SET ) ;
			KeyConvFileRead( &Head, sizeof( DARC_HEAD_VER5 ), this->fp, this->Key, 0 ) ;

			// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
			if( Head.Head != DXA_HEAD_VER5 )
				goto ERR ;
		}
		
		// �o�[�W��������
		if( this->Head.Version > DXA_VER_VER5 ) goto ERR ;
		
		// �w�b�_�̃T�C�Y���̃��������m�ۂ���
		this->HeadBuffer = ( u8 * )malloc( this->Head.HeadSize ) ;
		if( this->HeadBuffer == NULL ) goto ERR ;
		
		// �w�b�_�p�b�N���������ɓǂݍ���
		fseek( this->fp, this->Head.FileNameTableStartAddress, SEEK_SET ) ;
		if( this->Head.Version >= 0x0005 )
		{
			KeyConvFileRead( this->HeadBuffer, this->Head.HeadSize, this->fp, this->Key, 0 ) ;
		}
		else
		{
			KeyConvFileRead( this->HeadBuffer, this->Head.HeadSize, this->fp, this->Key ) ;
		}
		
		// �e�A�h���X���Z�b�g����
		this->NameP = this->HeadBuffer ;
		this->FileP = this->NameP + this->Head.FileTableStartAddress ;
		this->DirP = this->NameP + this->Head.DirectoryTableStartAddress ;
	}

	// �J�����g�f�B���N�g���̃Z�b�g
	this->CurrentDirectory = ( DARC_DIRECTORY_VER5 * )this->DirP ;

	// �������C���[�W����J���Ă���A�t���O��|��
	MemoryOpenFlag = false ;

	// �I��
	return 0 ;

ERR :
	if( this->fp != NULL ){ fclose( this->fp ) ; this->fp = NULL ; }
	if( this->HeadBuffer != NULL ){ free( this->HeadBuffer ) ; this->HeadBuffer = NULL ; }
	
	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�������
int DXArchive_VER5::CloseArchiveFile( void )
{
	// ���ɕ��Ă����牽�������I��
	if( this->fp == NULL ) return 0 ;

	// ����������J���Ă��邩�ǂ����ŏ����𕪊�
	if( MemoryOpenFlag == true )
	{
		// �A�[�J�C�u�N���X���ǂݍ��񂾏ꍇ�Ƃ����łȂ��ꍇ�ŏ����𕪊�
		if( UserMemoryImageFlag == true )
		{
			// ���]�����f�[�^�����ɖ߂�
			if( this->Head.Version >= 0x0005 )
			{
				DirectoryKeyConv( ( DARC_DIRECTORY_VER5 * )this->DirP ) ;
				KeyConv( this->HeadBuffer, this->Head.HeadSize, 0, this->Key ) ;
			}
			else
			{
				KeyConv( this->fp, this->MemoryImageSize, 0, this->Key ) ;
			}
		}
		else
		{
			// �m�ۂ��Ă������������J������
			free( this->fp ) ;
		}
	}
	else
	{
		// �A�[�J�C�u�t�@�C�������
		fclose( this->fp ) ;
		
		// �w�b�_�o�b�t�@�����
		free( this->HeadBuffer ) ;
	}

	// �|�C���^������
	this->fp = NULL ;
	this->HeadBuffer = NULL ;
	this->NameP = this->DirP = this->FileP = NULL ;
	this->CurrentDirectory = NULL ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
int	DXArchive_VER5::ChangeCurrentDirectoryFast( SEARCHDATA *SearchData )
{
	DARC_FILEHEAD_VER5 *FileH ;
	int i, j, k, Num ;
	u8 *NameData, *PathData ;
	u16 PackNum, Parity ;

	PackNum  = SearchData->PackNum ;
	Parity   = SearchData->Parity ;
	PathData = SearchData->FileName ;

	// �J�����g�f�B���N�g�����瓯���̃f�B���N�g����T��
	FileH = ( DARC_FILEHEAD_VER5 * )( this->FileP + this->CurrentDirectory->FileHeadAddress ) ;
	Num = (s32)this->CurrentDirectory->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH ++ )
	{
		// �f�B���N�g���`�F�b�N
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 ) continue ;

		// �����񐔂ƃp���e�B�`�F�b�N
		NameData = this->NameP + FileH->NameAddress ;
		if( PackNum != ((u16 *)NameData)[0] || Parity != ((u16 *)NameData)[1] ) continue ;

		// ������`�F�b�N
		NameData += 4 ;
		for( j = 0, k = 0 ; j < PackNum ; j ++, k += 4 )
			if( *((u32 *)&PathData[k]) != *((u32 *)&NameData[k]) ) break ;

		// �K�������f�B���N�g�����������炱���ŏI��
		if( PackNum == j ) break ;
	}

	// ����������G���[
	if( i == Num ) return -1 ;

	// �݂�����J�����g�f�B���N�g����ύX
	this->CurrentDirectory = ( DARC_DIRECTORY_VER5 * )( this->DirP + FileH->DataAddress ) ;

	// ����I��
	return 0 ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
int DXArchive_VER5::ChangeCurrentDir( const char *DirPath )
{
	return ChangeCurrentDirectoryBase( DirPath, true ) ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
int	DXArchive_VER5::ChangeCurrentDirectoryBase( const char *DirectoryPath, bool ErrorIsDirectoryReset, SEARCHDATA *LastSearchData )
{
	DARC_DIRECTORY_VER5 *OldDir ;
	SEARCHDATA SearchData ;

	// �����ɗ��܂�p�X�������疳��
	if( strcmp( DirectoryPath, "." ) == 0 ) return 0 ;

	// �w\�x�����̏ꍇ�̓��[�g�f�B���N�g���ɖ߂�
	if( strcmp( DirectoryPath, "\\" ) == 0 )
	{
		this->CurrentDirectory = ( DARC_DIRECTORY_VER5 * )this->DirP ;
		return 0 ;
	}

	// ���Ɉ������p�X�������珈���𕪊�
	if( strcmp( DirectoryPath, ".." ) == 0 )
	{
		// ���[�g�f�B���N�g���ɋ�����G���[
		if( this->CurrentDirectory->ParentDirectoryAddress == 0xffffffff ) return -1 ;
		
		// �e�f�B���N�g�����������炻����Ɉڂ�
		this->CurrentDirectory = ( DARC_DIRECTORY_VER5 * )( this->DirP + this->CurrentDirectory->ParentDirectoryAddress ) ;
		return 0 ;
	}

	// ����ȊO�̏ꍇ�͎w��̖��O�̃f�B���N�g����T��
	
	// �ύX�ȑO�̃f�B���N�g����ۑ����Ă���
	OldDir = this->CurrentDirectory ;

	// �p�X���Ɂw\�x�����邩�ǂ����ŏ����𕪊�
	if( strchr( DirectoryPath, '\\' ) == NULL )
	{
		// �t�@�C������������p�̌`���ɕϊ�����
		ConvSearchData( &SearchData, DirectoryPath, NULL ) ;

		// �f�B���N�g����ύX
		if( ChangeCurrentDirectoryFast( &SearchData ) < 0 ) goto ERR ;

/*		// \ �������ꍇ�́A�����̃f�B���N�g����T��
		FileH = ( DARC_FILEHEAD_VER5 * )( this->FileP + this->CurrentDirectory->FileHeadAddress ) ;
		for( i = 0 ;
			 i < (s32)this->CurrentDirectory->FileHeadNum &&
			 StrICmp( ( char * )( this->NameP + FileH->NameAddress ), DirectoryPath ) != 0 ;
			 i ++, FileH ++ ){}
*/
	}
	else
	{
		// \ ������ꍇ�͌q�������f�B���N�g������ÂύX���Ă䂭
	
		int Point, StrLength ;

		Point = 0 ;
		// ���[�v
		for(;;)
		{
			// ��������擾����
			ConvSearchData( &SearchData, &DirectoryPath[Point], &StrLength ) ;
			Point += StrLength ;
/*			StrPoint = 0 ;
			while( DirectoryPath[Point] != '\0' && DirectoryPath[Point] != '\\' )
			{
				if( CheckMultiByteChar( &DirectoryPath[Point] ) == TRUE )
				{
					*((u16 *)&String[StrPoint]) = *((u16 *)&DirectoryPath[Point]) ;
					StrPoint += 2 ;
					Point    += 2 ;
				}
				else
				{
					String[StrPoint] = DirectoryPath[Point] ;
					StrPoint ++ ;
					Point    ++ ;
				}
			}
			String[StrPoint] = '\0' ;
*/
			// ���������[�� \ �������ꍇ�̓��[�g�f�B���N�g���ɗ��Ƃ�
			if( StrLength == 0 && DirectoryPath[Point] == '\\' )
			{
				this->ChangeCurrentDirectoryBase( "\\", false ) ;
			}
			else
			{
				// ����ȊO�̏ꍇ�͕��ʂɃf�B���N�g���ύX
				if( this->ChangeCurrentDirectoryFast( &SearchData ) < 0 )
				{
					// �G���[���N���āA�X�ɃG���[���N�������Ɍ��̃f�B���N�g���ɖ߂���
					// �t���O�������Ă���ꍇ�͌��̃f�B���N�g���ɖ߂�
					if( ErrorIsDirectoryReset == true ) this->CurrentDirectory = OldDir ;

					// �G���[�I��
					goto ERR ;
				}
			}

			// �����I�[�����ŏI�������ꍇ�̓��[�v���甲����
			// ���͂��� \ �����Ȃ��ꍇ�����[�v���甲����
			if( DirectoryPath[Point] == '\0' ||
				( DirectoryPath[Point] == '\\' && DirectoryPath[Point+1] == '\0' ) ) break ;
			Point ++ ;
		}
	}

	if( LastSearchData != NULL )
	{
		memcpy( LastSearchData->FileName, SearchData.FileName, SearchData.PackNum * 4 ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}

	// ����I��
	return 0 ;

ERR:
	if( LastSearchData != NULL )
	{
		memcpy( LastSearchData->FileName, SearchData.FileName, SearchData.PackNum * 4 ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}

	// �G���[�I��
	return -1 ;
}
		
// �A�[�J�C�u���̃J�����g�f�B���N�g���p�X���擾����
int DXArchive_VER5::GetCurrentDir( char *DirPathBuffer, int BufferLength )
{
	char DirPath[MAX_PATH] ;
	DARC_DIRECTORY_VER5 *Dir[200], *DirTempP ;
	int Depth, i ;

	// ���[�g�f�B���N�g���ɒ����܂Ō�������
	Depth = 0 ;
	DirTempP = this->CurrentDirectory ;
	while( DirTempP->DirectoryAddress != 0xffffffff && DirTempP->DirectoryAddress != 0 )
	{
		Dir[Depth] = DirTempP ;
		DirTempP = ( DARC_DIRECTORY_VER5 * )( this->DirP + DirTempP->ParentDirectoryAddress ) ;
		Depth ++ ;
	}
	
	// �p�X����A������
	DirPath[0] = '\0' ;
	for( i = Depth - 1 ; i >= 0 ; i -- )
	{
		strcat( DirPath, "\\" ) ;
		strcat( DirPath, ( char * )( this->NameP + ( ( DARC_FILEHEAD_VER5 * )( this->FileP + Dir[i]->DirectoryAddress ) )->NameAddress ) ) ;
	}

	// �o�b�t�@�̒������O���A����������Ȃ��Ƃ��̓f�B���N�g�����̒�����Ԃ�
	if( BufferLength == 0 || BufferLength < (s32)strlen( DirPath ) )
	{
		return strlen( DirPath ) + 1 ;
	}
	else
	{
		// �f�B���N�g�������o�b�t�@�ɓ]������
		strcpy( DirPathBuffer, DirPath ) ;
	}

	// �I��
	return 0 ;
}



// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����������ɓǂݍ���
int DXArchive_VER5::LoadFileToMem( const char *FilePath, void *Buffer, unsigned int BufferLength )
{
	DARC_FILEHEAD_VER5 *FileH ;

	// �w��̃t�@�C���̏��𓾂�
	FileH = this->GetFileInfo( FilePath ) ;
	if( FileH == NULL ) return -1 ;

	// �t�@�C���T�C�Y������Ă��邩���ׂ�A����Ă��Ȃ����A�o�b�t�@�A���̓T�C�Y���O��������T�C�Y��Ԃ�
	if( BufferLength < FileH->DataSize || BufferLength == 0 || Buffer == NULL )
	{
		return ( int )FileH->DataSize ;
	}

	// ����Ă���ꍇ�̓o�b�t�@�[�ɓǂݍ���

	// �t�@�C�������k����Ă��邩�ǂ����ŏ����𕪊�
	if( this->Head.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
	{
		// ���k����Ă���ꍇ

		// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
		if( MemoryOpenFlag == true )
		{
			// ��������̈��k�f�[�^���𓀂���
			Decode( (u8 *)this->fp + this->Head.DataStartAddress + FileH->DataAddress, Buffer ) ;
		}
		else
		{
			void *temp ;

			// ���k�f�[�^���������ɓǂݍ���ł���𓀂���

			// ���k�f�[�^�����܂郁�����̈�̊m��
			temp = malloc( FileH->PressDataSize ) ;

			// ���k�f�[�^�̓ǂݍ���
			fseek( this->fp, this->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
			if( this->Head.Version >= 0x0005 )
			{
				KeyConvFileRead( temp, FileH->PressDataSize, this->fp, this->Key, FileH->DataSize ) ;
			}
			else
			{
				KeyConvFileRead( temp, FileH->PressDataSize, this->fp, this->Key ) ;
			}
			
			// ��
			Decode( temp, Buffer ) ;
			
			// �������̉��
			free( temp ) ;
		}
	}
	else
	{
		// ���k����Ă��Ȃ��ꍇ

		// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
		if( MemoryOpenFlag == true )
		{
			// �R�s�[
			memcpy( Buffer, (u8 *)this->fp + this->Head.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
		}
		else
		{
			// �t�@�C���|�C���^���ړ�
			fseek( this->fp, this->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;

			// �ǂݍ���
			if( this->Head.Version >= 0x0005 )
			{
				KeyConvFileRead( Buffer, FileH->DataSize, this->fp, this->Key, FileH->DataSize ) ;
			}
			else
			{
				KeyConvFileRead( Buffer, FileH->DataSize, this->fp, this->Key ) ;
			}
		}
	}
	
	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����T�C�Y���擾����
int DXArchive_VER5::GetFileSize( const char *FilePath )
{
	// �t�@�C���T�C�Y��Ԃ�
	return this->LoadFileToMem( FilePath, NULL, 0 ) ;
}

// �A�[�J�C�u�t�@�C�����������ɓǂݍ��񂾏ꍇ�̃t�@�C���C���[�W���i�[����Ă���擪�A�h���X���擾����( ����������J���Ă���ꍇ�̂ݗL�� )
void *DXArchive_VER5::GetFileImage( void )
{
	// �������C���[�W����J���Ă��Ȃ�������G���[
	if( MemoryOpenFlag == false ) return NULL ;

	// �擪�A�h���X��Ԃ�
	return this->fp ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C���̃t�@�C�����̈ʒu�ƃt�@�C���̑傫���𓾂�( -1:�G���[ )
int DXArchive_VER5::GetFileInfo( const char *FilePath, int *PositionP, int *SizeP )
{
	DARC_FILEHEAD_VER5 *FileH ;

	// �w��̃t�@�C���̏��𓾂�
	FileH = this->GetFileInfo( FilePath ) ;
	if( FileH == NULL ) return -1 ;

	// �t�@�C���̃f�[�^������ʒu�ƃt�@�C���T�C�Y��ۑ�����
	if( PositionP != NULL ) *PositionP = this->Head.DataStartAddress + FileH->DataAddress ;
	if( SizeP     != NULL ) *SizeP     = FileH->DataSize ;

	// �����I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����A�N���X���̃o�b�t�@�ɓǂݍ���
void *DXArchive_VER5::LoadFileToCash( const char *FilePath )
{
	int FileSize ;

	// �t�@�C���T�C�Y���擾����
	FileSize = this->GetFileSize( FilePath ) ;

	// �t�@�C��������������G���[
	if( FileSize < 0 ) return NULL ;

	// �m�ۂ��Ă���L���b�V���o�b�t�@�̃T�C�Y�����傫���ꍇ�̓o�b�t�@���Ċm�ۂ���
	if( FileSize > ( int )this->CashBufferSize )
	{
		// �L���b�V���o�b�t�@�̃N���A
		this->ClearCash() ;

		// �L���b�V���o�b�t�@�̍Ċm��
		this->CashBuffer = malloc( FileSize ) ;

		// �m�ۂɎ��s�����ꍇ�� NULL ��Ԃ�
		if( this->CashBuffer == NULL ) return NULL ;

		// �L���b�V���o�b�t�@�̃T�C�Y��ۑ�
		this->CashBufferSize = FileSize ;
	}

	// �t�@�C�����������ɓǂݍ���
	this->LoadFileToMem( FilePath, this->CashBuffer, FileSize ) ;

	// �L���b�V���o�b�t�@�̃A�h���X��Ԃ�
	return this->CashBuffer ;
}

// �L���b�V���o�b�t�@���J������
int DXArchive_VER5::ClearCash( void )
{
	// ���������m�ۂ���Ă�����������
	if( this->CashBuffer != NULL )
	{
		free( this->CashBuffer ) ;
		this->CashBuffer = NULL ;
		this->CashBufferSize = 0 ;
	}

	// �I��
	return 0 ;
}

	
// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����J���A�t�@�C���A�N�Z�X�p�I�u�W�F�N�g���쐬����
DXArchiveFile_VER5 *DXArchive_VER5::OpenFile( const char *FilePath )
{
	DARC_FILEHEAD_VER5 *FileH ;
	DXArchiveFile_VER5 *CDArc = NULL ;

	// ����������J���Ă���ꍇ�͖���
	if( MemoryOpenFlag == true ) return NULL ;

	// �w��̃t�@�C���̏��𓾂�
	FileH = this->GetFileInfo( FilePath ) ;
	if( FileH == NULL ) return NULL ;

	// �V���� DXArchiveFile_VER5 �N���X���쐬����
	CDArc = new DXArchiveFile_VER5( FileH, this ) ;
	
	// DXArchiveFile_VER5 �N���X�̃|�C���^��Ԃ�
	return CDArc ;
}













// �R���X�g���N�^
DXArchiveFile_VER5::DXArchiveFile_VER5( DARC_FILEHEAD_VER5 *FileHead, DXArchive_VER5 *Archive )
{
	this->FileData  = FileHead ;
	this->Archive   = Archive ;
	this->EOFFlag   = FALSE ;
	this->FilePoint = 0 ;
	this->DataBuffer = NULL ;
	
	// �t�@�C�������k����Ă���ꍇ�͂����œǂݍ���ŉ𓀂��Ă��܂�
	if( this->Archive->GetHeader()->Version >= 0x0002 && FileHead->PressDataSize != 0xffffffff )
	{
		void *temp ;

		// ���k�f�[�^�����܂郁�����̈�̊m��
		temp = malloc( FileHead->PressDataSize ) ;

		// �𓀃f�[�^�����܂郁�����̈�̊m��
		this->DataBuffer = malloc( FileHead->DataSize ) ;

		// ���k�f�[�^�̓ǂݍ���
		fseek( this->Archive->GetFilePointer(), this->Archive->GetHeader()->DataStartAddress + FileHead->DataAddress, SEEK_SET ) ;
		if( this->Archive->GetHeader()->Version >= 0x0005 )
		{
			DXArchive_VER5::KeyConvFileRead( temp, FileHead->PressDataSize, this->Archive->GetFilePointer(), this->Archive->GetKey(), FileHead->DataSize ) ;
		}
		else
		{
			DXArchive_VER5::KeyConvFileRead( temp, FileHead->PressDataSize, this->Archive->GetFilePointer(), this->Archive->GetKey() ) ;
		}
		
		// ��
		DXArchive_VER5::Decode( temp, this->DataBuffer ) ;
		
		// �������̉��
		free( temp ) ;
	}
}

// �f�X�g���N�^
DXArchiveFile_VER5::~DXArchiveFile_VER5()
{
	// �������̉��
	if( this->DataBuffer != NULL )
	{
		free( this->DataBuffer ) ;
		this->DataBuffer = NULL ;
	}
}

// �t�@�C���̓��e��ǂݍ���
int DXArchiveFile_VER5::Read( void *Buffer, int ReadLength )
{
	int ReadSize ;

	// EOF �t���O�������Ă�����O��Ԃ�
	if( this->EOFFlag == TRUE ) return 0 ;
	
	// �A�[�J�C�u�t�@�C���|�C���^�ƁA���z�t�@�C���|�C���^����v���Ă��邩���ׂ�
	// ��v���Ă��Ȃ�������A�[�J�C�u�t�@�C���|�C���^���ړ�����
	if( this->DataBuffer == NULL && ftell( this->Archive->GetFilePointer() ) != (s32)( this->FileData->DataAddress + this->Archive->GetHeader()->DataStartAddress + this->FilePoint ) )
	{
		fseek( this->Archive->GetFilePointer(), this->FileData->DataAddress + this->Archive->GetHeader()->DataStartAddress + this->FilePoint, SEEK_SET ) ;
	}
	
	// EOF ���o
	if( this->FileData->DataSize == this->FilePoint )
	{
		this->EOFFlag = TRUE ;
		return 0 ;
	}
	
	// �f�[�^��ǂݍ��ޗʂ�ݒ肷��
	ReadSize = ReadLength < (s32)( this->FileData->DataSize - this->FilePoint ) ? ReadLength : this->FileData->DataSize - this->FilePoint ;
	
	// �f�[�^��ǂݍ���
	if( this->DataBuffer == NULL )
	{
		if( this->Archive->GetHeader()->Version >= 0x0005 )
		{
			DXArchive_VER5::KeyConvFileRead( Buffer, ReadSize, this->Archive->GetFilePointer(), this->Archive->GetKey(), this->FileData->DataSize + this->FilePoint ) ;
		}
		else
		{
			DXArchive_VER5::KeyConvFileRead( Buffer, ReadSize, this->Archive->GetFilePointer(), this->Archive->GetKey() ) ;
		}
	}
	else
	{
		memcpy( Buffer, (u8 *)this->DataBuffer + this->FilePoint, ReadSize ) ;
	}
	
	// EOF �t���O��|��
	this->EOFFlag = FALSE ;

	// �ǂݍ��񂾕������t�@�C���|�C���^���ړ�����
	this->FilePoint += ReadSize ;
	
	// �ǂݍ��񂾗e�ʂ�Ԃ�
	return ReadSize ;
}
	
// �t�@�C���|�C���^��ύX����
int DXArchiveFile_VER5::Seek( int SeekPoint, int SeekMode )
{
	// �V�[�N�^�C�v�ɂ���ď����𕪊�
	switch( SeekMode )
	{
	case SEEK_SET : break ;		
	case SEEK_CUR : SeekPoint += this->FilePoint ; break ;
	case SEEK_END :	SeekPoint = this->FileData->DataSize + SeekPoint ; break ;
	}
	
	// �␳
	if( SeekPoint > (s32)this->FileData->DataSize ) SeekPoint = this->FileData->DataSize ;
	if( SeekPoint < 0 ) SeekPoint = 0 ;
	
	// �Z�b�g
	this->FilePoint = SeekPoint ;
	
	// EOF�t���O��|��
	this->EOFFlag = FALSE ;
	
	// �I��
	return 0 ;
}

// ���݂̃t�@�C���|�C���^�𓾂�
int DXArchiveFile_VER5::Tell( void )
{
	return this->FilePoint ;
}

// �t�@�C���̏I�[�ɗ��Ă��邩�A�̃t���O�𓾂�
int DXArchiveFile_VER5::Eof( void )
{
	return this->EOFFlag ;
}

// �t�@�C���̃T�C�Y���擾����
int DXArchiveFile_VER5::Size( void )
{
	return this->FileData->DataSize ;
}



