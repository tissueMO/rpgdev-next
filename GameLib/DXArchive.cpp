// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u�����A�[�J�C�o
// 
//	Creator			: �R�c �I
//	Creation Date	: 2003/09/11
//	Version			: 1.02
// 
// -------------------------------------------------------------------------------

#define INLINE_ASM

// include ----------------------------
#include "DXArchive.h"
#include "CharCode.h"
#include "Huffman.h"
#include "FileLib.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>

// define -----------------------------

#define MIN_COMPRESS		(4)						// �Œሳ�k�o�C�g��
#define MAX_SEARCHLISTNUM	(64)					// �ő��v����T���ׂ̃��X�g��H��ő吔
#define MAX_SUBLISTNUM		(65536)					// ���k���ԒZ�k�̂��߂̃T�u���X�g�̍ő吔
#define MAX_COPYSIZE 		(0x1fff + MIN_COMPRESS)	// �Q�ƃA�h���X����R�s�[�o�؂�ő�T�C�Y( ���k�R�[�h���\���ł���R�s�[�T�C�Y�̍ő�l + �Œሳ�k�o�C�g�� )
#define MAX_ADDRESSLISTNUM	(1024 * 1024 * 1)		// �X���C�h�����̍ő�T�C�Y
#define MAX_POSITION		(1 << 24)				// �Q�Ɖ\�ȍő告�΃A�h���X( 16MB )

// struct -----------------------------

// ���k���ԒZ�k�p���X�g
typedef struct LZ_LIST
{
	LZ_LIST *next, *prev ;
	u32 address ;
} LZ_LIST ;

// data -------------------------------

// �f�t�H���g��������
static char DefaultKeyString[ 9 ] = { 0x44, 0x58, 0x42, 0x44, 0x58, 0x41, 0x52, 0x43, 0x00 }; // "DXLIBARC"

// ���O������̒���
int LogStringLength = 0 ;

// class code -------------------------

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C���p�X�ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖���
int DXArchive::GetFilePathAndDirPath( char *Src, char *FilePath, char *DirPath )
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
DARC_FILEHEAD *DXArchive::GetFileInfo( const char *FilePath, DARC_DIRECTORY **DirectoryP )
{
	DARC_DIRECTORY *OldDir ;
	DARC_FILEHEAD *FileH ;
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
	FileHeadSize = sizeof( DARC_FILEHEAD ) ;
	FileH = ( DARC_FILEHEAD * )( this->FileP + this->CurrentDirectory->FileHeadAddress ) ;
	Num = ( int )this->CurrentDirectory->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DARC_FILEHEAD *)( (u8 *)FileH + FileHeadSize ) )
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

	// �f�B���N�g���̃A�h���X��ۑ�����w�肪�������ꍇ�͕ۑ�
	if( DirectoryP != NULL )
	{
		*DirectoryP = this->CurrentDirectory ;
	}
	
	// �f�B���N�g�������ɖ߂�
	this->CurrentDirectory = OldDir ;
	
	// �ړI�̃t�@�C���̃A�h���X��Ԃ�
	return FileH ;
	
ERR :
	// �f�B���N�g�������ɖ߂�
	this->CurrentDirectory = OldDir ;
	
	// �G���[�I��
	return NULL ;
}

// �A�[�J�C�u���̃J�����g�f�B���N�g���̏����擾����
DARC_DIRECTORY *DXArchive::GetCurrentDirectoryInfo( void )
{
	return CurrentDirectory ;
}

// �ǂ��炪�V���������r����
DXArchive::DATE_RESULT DXArchive::DateCmp( DARC_FILETIME *date1, DARC_FILETIME *date2 )
{
	if( date1->LastWrite == date2->LastWrite ) return DATE_RESULT_DRAW ;
	else if( date1->LastWrite > date2->LastWrite ) return DATE_RESULT_LEFTNEW ;
	else return DATE_RESULT_RIGHTNEW ;
}

// ��r�ΏƂ̕����񒆂̑啶�����������Ƃ��Ĉ�����r����( 0:������  1:�Ⴄ )
int DXArchive::StrICmp( const char *Str1, const char *Str2 )
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
int DXArchive::ConvSearchData( SEARCHDATA *SearchData, const char *Src, int *Length )
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
int DXArchive::AddFileNameData( int CharCodeFormat, const char *FileName, u8 *FileNameTable )
{
	int PackNum, Length, i, j ;
	u32 Parity ;

	// �T�C�Y���Z�b�g
	Length = ( int )CL_strlen( CharCodeFormat, FileName ) * GetCharCodeFormatUnitSize( CharCodeFormat ) ;

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
	CL_strcpy( CharCodeFormat, (char *)&FileNameTable[4 + PackNum * 4], FileName ) ;

	// �p���̏�������S�đ啶���ɕϊ������t�@�C������ۑ�
	Parity = 0 ;
	for( i = 0 ; FileName[i] != '\0' ; )
	{
		int Bytes = GetCharBytes( &FileName[i], CharCodeFormat ) ;

		// 1�o�C�g�������ǂ����ŏ����𕪊�
		if( Bytes == 1 )
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
		else
		{
			// �}���`�o�C�g����
			for( j = 0 ; j < Bytes ; j ++ )
			{
				FileNameTable[4 + i + j] = (u8)FileName[i + j] ;
				Parity += (u8)FileName[i + j] ;
			}
			i += Bytes ;
		}
	}

	// �p���e�B����ۑ�
	*((u16 *)&FileNameTable[2]) = (u16)Parity ;

	// �g�p�����T�C�Y��Ԃ�
	return PackNum * 4 * 2 + 4 ;
}

// �t�@�C�����f�[�^���猳�̃t�@�C�����̕�������擾����
const char *DXArchive::GetOriginalFileName( u8 *FileNameTable )
{
	return (char *)FileNameTable + *((u16 *)&FileNameTable[0]) * 4 + 4 ;
}

// �W���X�g���[���Ƀf�[�^����������( 64bit�� )
void DXArchive::fwrite64( void *Data, s64 Size, FILE *fp )
{
	int WriteSize ;
	s64 TotalWriteSize ;

	TotalWriteSize = 0 ;
	while( TotalWriteSize < Size )
	{
		if( Size > 0x7fffffff )
		{
			WriteSize = 0x7fffffff ;
		}
		else
		{
			WriteSize = ( int )Size ;
		}

		fwrite( ( u8 * )Data + TotalWriteSize, 1, WriteSize, fp ) ;

		TotalWriteSize += WriteSize ;
	}
}

// �W���X�g���[������f�[�^��ǂݍ���( 64bit�� )
void DXArchive::fread64( void *Buffer, s64 Size, FILE *fp )
{
	int ReadSize ;
	s64 TotalReadSize ;

	TotalReadSize = 0 ;
	while( TotalReadSize < Size )
	{
		if( Size > 0x7fffffff )
		{
			ReadSize = 0x7fffffff ;
		}
		else
		{
			ReadSize = ( int )Size ;
		}

		fread( ( u8 * )Buffer + TotalReadSize, 1, ReadSize, fp ) ;

		TotalReadSize += ReadSize ;
	}
}

// �f�[�^�𔽓]������֐�
void DXArchive::NotConv( void *Data , s64 Size )
{
	s64 DwordNumQ ;
	s64 ByteNum ;
	u32 *dd ;

	dd = ( u32 * )Data ;

	DwordNumQ = Size / 4 ;
	ByteNum = Size - DwordNumQ * 4 ;

	if( DwordNumQ != 0 )
	{
		if( DwordNumQ < 0x100000000 )
		{
			u32 DwordNum ;

			DwordNum = ( u32 )DwordNumQ ;
			do
			{
				*dd++ = ~*dd ;
			}while( --DwordNum ) ;
		}
		else
		{
			do
			{
				*dd++ = ~*dd ;
			}while( --DwordNumQ ) ;
		}
	}
	if( ByteNum != 0 )
	{
		do
		{
			*((BYTE *)dd) = ~*((u8 *)dd) ;
			dd = (u32 *)(((u8 *)dd) + 1) ;
		}while( --ByteNum ) ;
	}
}


// �f�[�^�𔽓]�����ăt�@�C���ɏ����o���֐�
void DXArchive::NotConvFileWrite( void *Data, s64 Size, FILE *fp )
{
	// �f�[�^�𔽓]����
	NotConv( Data, Size ) ;

	// �����o��
	fwrite64( Data, Size, fp ) ;

	// �Ăє��]
	NotConv( Data, Size ) ;
}

// �f�[�^�𔽓]�����ăt�@�C������ǂݍ��ފ֐�
void DXArchive::NotConvFileRead( void *Data, s64 Size, FILE *fp )
{
	// �ǂݍ���
	fread64( Data, Size, fp ) ;

	// �f�[�^�𔽓]
	NotConv( Data, Size ) ;
}

// �J�����g�f�B���N�g���ɂ���w��̃t�@�C���̌��p�̕�������쐬����A�߂�l�͕�����̒���( �P�ʁFByte )( FileString �� DXA_KEY_STRING_MAXLENGTH �̒������K�v )
size_t DXArchive::CreateKeyFileString( int CharCodeFormat, const char *KeyString, size_t KeyStringBytes, DARC_DIRECTORY *Directory, DARC_FILEHEAD *FileHead, u8 *FileTable, u8 *DirectoryTable, u8 *NameTable, u8 *FileString )
{
	size_t StartAddr ;

	// �ŏ��Ƀp�X���[�h�̕�������Z�b�g
	if( KeyString != NULL && KeyStringBytes != 0 )
	{
		memcpy( FileString, KeyString, KeyStringBytes ) ;
		FileString[ KeyStringBytes ] = '\0' ;
		StartAddr = KeyStringBytes ;
	}
	else
	{
		FileString[ 0 ] = '\0' ;
		StartAddr = 0 ;
	}
	memset( &FileString[ DXA_KEY_STRING_MAXLENGTH - 8 ], 0, 8 ) ;

	// ���Ƀt�@�C�����̕�������Z�b�g
	CL_strcat_s( CharCodeFormat, ( char * )&FileString[ StartAddr ], ( DXA_KEY_STRING_MAXLENGTH - 8 ) - StartAddr, ( char * )( NameTable + FileHead->NameAddress + 4 ) ) ;

	// ���̌�Ƀf�B���N�g���̕�������Z�b�g
	if( Directory->ParentDirectoryAddress != 0xffffffffffffffff )
	{
		do
		{
			CL_strcat_s( CharCodeFormat, ( char * )&FileString[ StartAddr ], ( DXA_KEY_STRING_MAXLENGTH - 8 ) - StartAddr, ( char * )( NameTable + ( ( DARC_FILEHEAD * )( FileTable + Directory->DirectoryAddress ) )->NameAddress + 4 ) ) ;
			Directory = ( DARC_DIRECTORY * )( DirectoryTable + Directory->ParentDirectoryAddress ) ;
		}while( Directory->ParentDirectoryAddress != 0xffffffffffffffff ) ;
	}

	return StartAddr + CL_strlen( CharCodeFormat, ( char * )&FileString[ StartAddr ] ) * GetCharCodeFormatUnitSize( CharCodeFormat ) ;
}

// ����������쐬
void DXArchive::KeyCreate( const char *Source, size_t SourceBytes, u8 *Key )
{
	char SourceTempBuffer[ 1024 ] ;
	char WorkBuffer[ 1024 ] ;
	char *UseWorkBuffer ;
	u32 i, j ;
	u32 CRC32_0 ;
	u32 CRC32_1 ;

	if( SourceBytes == 0 )
	{
		SourceBytes = CL_strlen( CHARCODEFORMAT_ASCII, Source ) ;
	}

	if( SourceBytes < 4 )
	{
		CL_strcpy( CHARCODEFORMAT_ASCII, SourceTempBuffer, Source ) ;
		CL_strcpy( CHARCODEFORMAT_ASCII, &SourceTempBuffer[ SourceBytes ], DefaultKeyString ) ;
		Source = SourceTempBuffer ;
		SourceBytes = CL_strlen( CHARCODEFORMAT_ASCII, Source ) ;
	}

	if( SourceBytes > sizeof( WorkBuffer ) )
	{
		UseWorkBuffer = ( char * )malloc( SourceBytes ) ;
	}
	else
	{
		UseWorkBuffer = WorkBuffer ;
	}

	j = 0 ;
	for( i = 0 ; i < SourceBytes ; i += 2, j++ )
	{
		UseWorkBuffer[ j ] = Source[ i ] ;
	}
	CRC32_0 = HashCRC32( UseWorkBuffer, j ) ;

	j = 0 ;
	for( i = 1 ; i < SourceBytes ; i += 2, j++ )
	{
		UseWorkBuffer[ j ] = Source[ i ] ;
	}
	CRC32_1 = HashCRC32( UseWorkBuffer, j ) ;

	Key[ 0 ] = ( u8 )( CRC32_0 >>  0 ) ;
	Key[ 1 ] = ( u8 )( CRC32_0 >>  8 ) ;
	Key[ 2 ] = ( u8 )( CRC32_0 >> 16 ) ;
	Key[ 3 ] = ( u8 )( CRC32_0 >> 24 ) ;
	Key[ 4 ] = ( u8 )( CRC32_1 >>  0 ) ;
	Key[ 5 ] = ( u8 )( CRC32_1 >>  8 ) ;
	Key[ 6 ] = ( u8 )( CRC32_1 >> 16 ) ;

	if( SourceBytes > sizeof( WorkBuffer ) )
	{
		free( UseWorkBuffer ) ;
	}
}

// ����������g�p���� Xor ���Z( Key �͕K�� DXA_KEY_BYTES �̒������Ȃ���΂Ȃ�Ȃ� )
void DXArchive::KeyConv( void *Data, s64 Size, s64 Position, unsigned char *Key )
{
	if( Key == NULL )
	{
		return ;
	}

	Position %= DXA_KEY_BYTES ;

	if( Size < 0x100000000 )
	{
		u32 i, j ;

		j = ( u32 )Position ;
		for( i = 0 ; i < Size ; i ++ )
		{
			((u8 *)Data)[i] ^= Key[j] ;

			j ++ ;
			if( j == DXA_KEY_BYTES ) j = 0 ;
		}
	}
	else
	{
		s64 i, j ;

		j = Position ;
		for( i = 0 ; i < Size ; i ++ )
		{
			((u8 *)Data)[i] ^= Key[j] ;

			j ++ ;
			if( j == DXA_KEY_BYTES ) j = 0 ;
		}
	}
}

// �f�[�^������������g�p���� Xor ���Z������t�@�C���ɏ����o���֐�( Key �͕K�� DXA_KEY_BYTES �̒������Ȃ���΂Ȃ�Ȃ� )
void DXArchive::KeyConvFileWrite( void *Data, s64 Size, FILE *fp, unsigned char *Key, s64 Position )
{
	s64 pos = 0 ;

	if( Key != NULL )
	{
		// �t�@�C���̈ʒu���擾���Ă���
		pos = Position == -1 ? _ftelli64( fp ) : Position ;

		// �f�[�^������������g���� Xor ���Z����
		KeyConv( Data, Size, pos, Key ) ;
	}

	// �����o��
	fwrite64( Data, Size, fp ) ;

	if( Key != NULL )
	{
		// �Ă� Xor ���Z
		KeyConv( Data, Size, pos, Key ) ;
	}
}

// �t�@�C������ǂݍ��񂾃f�[�^������������g�p���� Xor ���Z����֐�( Key �͕K�� DXA_KEY_BYTES �̒������Ȃ���΂Ȃ�Ȃ� )
void DXArchive::KeyConvFileRead( void *Data, s64 Size, FILE *fp, unsigned char *Key, s64 Position )
{
	s64 pos = 0 ;

	if( Key != NULL )
	{
		// �t�@�C���̈ʒu���擾���Ă���
		pos = Position == -1 ? _ftelli64( fp ) : Position ;
	}

	// �ǂݍ���
	fread64( Data, Size, fp ) ;

	if( Key != NULL )
	{
		// �f�[�^������������g���� Xor ���Z
		KeyConv( Data, Size, pos, Key ) ;
	}
}

// �w��̃f�B���N�g���ɂ���t�@�C�����A�[�J�C�u�f�[�^�ɓf���o��
int DXArchive::DirectoryEncode( int CharCodeFormat, char *DirectoryName, u8 *NameP, u8 *DirP, u8 *FileP, DARC_DIRECTORY *ParentDir, SIZESAVE *Size, int DataNumber, FILE *DestFp, void *TempBuffer, bool Press, bool AlwaysHuffman, u8 HuffmanEncodeKB, const char *KeyString, size_t KeyStringBytes, bool NoKey, char *KeyStringBuffer, DARC_ENCODEINFO *EncodeInfo )
{
	char DirPath[MAX_PATH] ;
	WIN32_FIND_DATAA FindData ;
	HANDLE FindHandle ;
	DARC_DIRECTORY Dir ;
	DARC_DIRECTORY *DirectoryP ;
	DARC_FILEHEAD File ;
	u8 lKey[DXA_KEY_BYTES] ;
	size_t KeyStringBufferBytes ;

	// �f�B���N�g���̏��𓾂�
	FindHandle = FindFirstFileA( DirectoryName, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return 0 ;
	
	// �f�B���N�g�������i�[����t�@�C���w�b�_���Z�b�g����
	{
		File.NameAddress       = Size->NameSize ;
		File.Time.Create       = ( ( ( LONGLONG )FindData.ftCreationTime.dwHighDateTime ) << 32 ) + FindData.ftCreationTime.dwLowDateTime ;
		File.Time.LastAccess   = ( ( ( LONGLONG )FindData.ftLastAccessTime.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime.dwLowDateTime ;
		File.Time.LastWrite    = ( ( ( LONGLONG )FindData.ftLastWriteTime.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime.dwLowDateTime ;
		File.Attributes        = FindData.dwFileAttributes ;
		File.DataAddress       = Size->DirectorySize ;
		File.DataSize          = 0 ;
		File.PressDataSize	   = 0xffffffffffffffff ;
		File.HuffPressDataSize = 0xffffffffffffffff ;
	}

	// �f�B���N�g�����������o��
	Size->NameSize += AddFileNameData( CharCodeFormat, FindData.cFileName, NameP + Size->NameSize ) ;

	// �f�B���N�g����񂪓������t�@�C���w�b�_�������o��
	memcpy( FileP + ParentDir->FileHeadAddress + DataNumber * sizeof( DARC_FILEHEAD ),
			&File, sizeof( DARC_FILEHEAD ) ) ;

	// Find �n���h�������
	FindClose( FindHandle ) ;

	// �w��̃f�B���N�g���ɃJ�����g�f�B���N�g�����ڂ�
	GetCurrentDirectoryA( MAX_PATH, DirPath ) ;
	SetCurrentDirectoryA( DirectoryName ) ;

	// �f�B���N�g�����̃Z�b�g
	{
		Dir.DirectoryAddress = ParentDir->FileHeadAddress + DataNumber * sizeof( DARC_FILEHEAD ) ;
		Dir.FileHeadAddress  = Size->FileSize ;

		// �e�f�B���N�g���̏��ʒu���Z�b�g
		if( ParentDir->DirectoryAddress != 0xffffffffffffffff && ParentDir->DirectoryAddress != 0 )
		{
			Dir.ParentDirectoryAddress = ((DARC_FILEHEAD *)( FileP + ParentDir->DirectoryAddress ))->DataAddress ;
		}
		else
		{
			Dir.ParentDirectoryAddress = 0 ;
		}

		// �f�B���N�g�����̃t�@�C���̐����擾����
		Dir.FileHeadNum = GetDirectoryFilePath( "", NULL ) ;
	}

	// �f�B���N�g���̏����o�͂���
	memcpy( DirP + Size->DirectorySize, &Dir, sizeof( DARC_DIRECTORY ) ) ;	
	DirectoryP = ( DARC_DIRECTORY * )( DirP + Size->DirectorySize ) ;

	// �A�h���X�𐄈ڂ�����
	Size->DirectorySize += sizeof( DARC_DIRECTORY ) ;
	Size->FileSize      += sizeof( DARC_FILEHEAD ) * Dir.FileHeadNum ;
	
	// �t�@�C�������������ꍇ�͂����ŏI��
	if( Dir.FileHeadNum == 0 )
	{
		// ���Ƃ̃f�B���N�g�����J�����g�f�B���N�g���ɃZ�b�g
		SetCurrentDirectoryA( DirPath ) ;
		return 0 ;
	}

	// �t�@�C�������o�͂���
	{
		int i ;
		
		i = 0 ;
		
		// �񋓊J�n
		FindHandle = FindFirstFileA( "*", &FindData ) ;
		do
		{
			// ��̃f�B���N�g���ɖ߂����肷�邽�߂̃p�X�͖�������
			if( strcmp( FindData.cFileName, "." ) == 0 || strcmp( FindData.cFileName, ".." ) == 0 ) continue ;

			// �t�@�C���ł͂Ȃ��A�f�B���N�g���������ꍇ�͍ċA����
			if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���������ꍇ�̏���
				if( DirectoryEncode( CharCodeFormat, FindData.cFileName, NameP, DirP, FileP, &Dir, Size, i, DestFp, TempBuffer, Press, AlwaysHuffman, HuffmanEncodeKB, KeyString, KeyStringBytes, NoKey, KeyStringBuffer, EncodeInfo ) < 0 ) return -1 ;
			}
			else
			{
				// �t�@�C���������ꍇ�̏���

				// �t�@�C���̃f�[�^���Z�b�g
				File.NameAddress       = Size->NameSize ;
				File.Time.Create       = ( ( ( LONGLONG )FindData.ftCreationTime.dwHighDateTime   ) << 32 ) + FindData.ftCreationTime.dwLowDateTime   ;
				File.Time.LastAccess   = ( ( ( LONGLONG )FindData.ftLastAccessTime.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime.dwLowDateTime ;
				File.Time.LastWrite    = ( ( ( LONGLONG )FindData.ftLastWriteTime.dwHighDateTime  ) << 32 ) + FindData.ftLastWriteTime.dwLowDateTime  ;
				File.Attributes        = FindData.dwFileAttributes ;
				File.DataAddress       = Size->DataSize ;
				File.DataSize          = ( ( ( LONGLONG )FindData.nFileSizeHigh ) << 32 ) + FindData.nFileSizeLow ;
				File.PressDataSize     = 0xffffffffffffffff ;
				File.HuffPressDataSize = 0xffffffffffffffff ;

				// �i�s�󋵏o��
				if( EncodeInfo->OutputStatus )
				{
					// �����t�@�C�������Z�b�g
					strcpy( EncodeInfo->ProcessFileName, FindData.cFileName ) ;

					// �t�@�C�����𑝂₷
					EncodeInfo->CompFileNum ++ ;

					// �\��
					EncodeStatusOutput( EncodeInfo ) ;
				}

				// �t�@�C�����������o��
				Size->NameSize += AddFileNameData( CharCodeFormat, FindData.cFileName, NameP + Size->NameSize ) ;

				// �t�@�C���ʂ̌����쐬
				if( NoKey == false )
				{
					KeyStringBufferBytes = CreateKeyFileString( CharCodeFormat, KeyString, KeyStringBytes, DirectoryP, &File, FileP, DirP, NameP, ( BYTE * )KeyStringBuffer ) ;
					KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
				}
				
				// �t�@�C���f�[�^�������o��
				if( File.DataSize != 0 )
				{
					FILE *SrcP ;
					u64 FileSize, WriteSize, MoveSize ;
					bool Huffman = false ;
					bool AlwaysPress = false ;

					// �t�@�C�����J��
					SrcP = fopen( FindData.cFileName, "rb" ) ;
					
					// �T�C�Y�𓾂�
					_fseeki64( SrcP, 0, SEEK_END ) ;
					FileSize = _ftelli64( SrcP ) ;
					_fseeki64( SrcP, 0, SEEK_SET ) ;

					// ���k�̑ΏۂƂȂ�t�@�C���t�H�[�}�b�g�����ׂ�
					{
						u32 Len ;
						Len = ( u32 )strlen( FindData.cFileName ) ;
						if( Len > 4 )
						{
							char *sp ;
						
							sp = &FindData.cFileName[Len-3] ;
							if( StrICmp( sp, "wav" ) == 0 ||
								StrICmp( sp, "jpg" ) == 0 ||
								StrICmp( sp, "png" ) == 0 ||
								StrICmp( sp, "mpg" ) == 0 ||
								StrICmp( sp, "mp3" ) == 0 ||
								StrICmp( sp, "mp4" ) == 0 ||
								StrICmp( sp, "m4a" ) == 0 ||
								StrICmp( sp, "ogg" ) == 0 ||
								StrICmp( sp, "ogv" ) == 0 ||
								StrICmp( sp, "ops" ) == 0 ||
								StrICmp( sp, "wmv" ) == 0 ||
								StrICmp( sp, "tif" ) == 0 ||
								StrICmp( sp, "tga" ) == 0 ||
								StrICmp( sp, "bmp" ) == 0 ||
								StrICmp( sp - 1, "jpeg" ) == 0 )
							{
								Huffman = true ;
							}

							// wav �� bmp �̏ꍇ�͕K�����k����
							if( StrICmp( sp, "wav" ) == 0 ||
								StrICmp( sp, "tga" ) == 0 ||
								StrICmp( sp, "bmp" ) == 0 )
							{
								AlwaysPress = true ;
							}
						}
					}

					// AlwaysHuffman �� true �̏ꍇ�͕K���n�t�}�����k����
					if( AlwaysHuffman )
					{
						Huffman = true ;
					}

					// �n�t�}�����k����T�C�Y�� 0 �̏ꍇ�̓n�t�}�����k���s��Ȃ�
					if( HuffmanEncodeKB == 0 )
					{
						Huffman = false ;
					}
					
					// ���k�̎w�肪����ꍇ�ŁA
					// �K�����k����t�@�C���t�H�[�}�b�g���A�t�@�C���T�C�Y�� 10MB �ȉ��̏ꍇ�͈��k�����݂�
					if( Press == true && ( AlwaysPress || File.DataSize < 10 * 1024 * 1024 ) )
					{
						void *SrcBuf, *DestBuf ;
						u32 DestSize, Len ;
						
						// �ꕔ�̃t�@�C���`���̏ꍇ�͗\�ߒe��
						if( AlwaysPress == false && ( Len = ( int )strlen( FindData.cFileName ) ) > 4 )
						{
							char *sp ;
							
							sp = &FindData.cFileName[Len-3] ;
							if( StrICmp( sp, "wav" ) == 0 ||
								StrICmp( sp, "jpg" ) == 0 ||
								StrICmp( sp, "png" ) == 0 ||
								StrICmp( sp, "mpg" ) == 0 ||
								StrICmp( sp, "mp3" ) == 0 ||
								StrICmp( sp, "mp4" ) == 0 ||
								StrICmp( sp, "ogg" ) == 0 ||
								StrICmp( sp, "ogv" ) == 0 ||
								StrICmp( sp, "ops" ) == 0 ||
								StrICmp( sp, "wmv" ) == 0 ||
								StrICmp( sp - 1, "jpeg" ) == 0 ) goto NOPRESS ;
						}
						
						// �f�[�^���ۂ��Ɠ��郁�����̈�̊m��
						SrcBuf  = malloc( ( size_t )( FileSize + FileSize * 2 + 64 ) ) ;
						DestBuf = (u8 *)SrcBuf + FileSize ;
						
						// �t�@�C�����ۂ��Ɠǂݍ���
						fread64( SrcBuf, FileSize, SrcP ) ;
						
						// ���k����ꍇ�͋����I�ɐi�s�󋵏o�͂��X�V
						if( EncodeInfo->OutputStatus )
						{
							EncodeStatusOutput( EncodeInfo, true ) ;
						}

						// ���k
						DestSize = Encode( SrcBuf, ( u32 )FileSize, DestBuf, EncodeInfo->OutputStatus ) ;
						
						// �w�ǈ��k�o���Ȃ������ꍇ�͈��k�����ŃA�[�J�C�u����
						if( AlwaysPress == false && ( (f64)DestSize / (f64)FileSize > 0.90 ) )
						{
							_fseeki64( SrcP, 0L, SEEK_SET ) ;
							free( SrcBuf ) ;
							goto NOPRESS ;
						}

						// ���k�f�[�^�̃T�C�Y��ۑ�����
						File.PressDataSize = DestSize ;
						
						// �n�t�}�����k���s�����ǂ����ŏ����𕪊�
						if( Huffman )
						{
							u8 *HuffData ;

							// �n�t�}�����k����T�C�Y�ɂ���ď����𕪊�
							if( HuffmanEncodeKB == 0xff || DestSize <= ( u64 )( HuffmanEncodeKB * 1024 * 2 ) )
							{
								// �n�t�}�����k�p�̃������̈���m��
								HuffData = ( u8 * )calloc( 1, DestSize * 2 + 256 * 2 + 32 ) ;

								// �t�@�C���S�̂��n�t�}�����k
								File.HuffPressDataSize = Huffman_Encode( DestBuf, DestSize, HuffData ) ;

								// ���k�f�[�^�Ɍ���K�p���ď����o��
								WriteSize = ( File.HuffPressDataSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
								KeyConvFileWrite( HuffData, WriteSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
							}
							else
							{
								// �n�t�}�����k�p�̃������̈���m��
								HuffData = ( u8 * )calloc( 1, HuffmanEncodeKB * 1024 * 2 * 4 + 256 * 2 + 32 ) ;

								// �t�@�C���̑O����n�t�}�����k
								memcpy( HuffData,                                  DestBuf,                                     HuffmanEncodeKB * 1024 ) ;
								memcpy( HuffData + HuffmanEncodeKB * 1024, ( u8 * )DestBuf + DestSize - HuffmanEncodeKB * 1024, HuffmanEncodeKB * 1024 ) ;
								File.HuffPressDataSize = Huffman_Encode( HuffData, HuffmanEncodeKB * 1024 * 2, HuffData + HuffmanEncodeKB * 1024 * 2 ) ;

								// �n�t�}�����k���������������o��
								KeyConvFileWrite( HuffData + HuffmanEncodeKB * 1024 * 2, File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;

								// �n�t�}�����k���Ă��Ȃ��ӏ��������o��
								WriteSize = File.HuffPressDataSize + DestSize - HuffmanEncodeKB * 1024 * 2 ;
								WriteSize = ( WriteSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
								KeyConvFileWrite( ( u8 * )DestBuf + HuffmanEncodeKB * 1024, WriteSize - File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize + File.HuffPressDataSize ) ;
							}

							// �������̉��
							free( HuffData ) ;
						}
						else
						{
							// ���k�f�[�^�𔽓]���ď����o��
							WriteSize = ( DestSize + 3 ) / 4 * 4 ;
							KeyConvFileWrite( DestBuf, WriteSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
						}
						
						// �������̉��
						free( SrcBuf ) ;
					}
					else
					{
NOPRESS:					
						// �n�t�}�����k���s�����ǂ����ŏ����𕪊�
						if( Press && Huffman )
						{
							u8 *SrcBuf, *HuffData ;

							// �f�[�^���ۂ��Ɠ��郁�����̈�̊m��
							SrcBuf = ( u8 * )calloc( 1, ( size_t )( FileSize + 32 ) ) ;

							// �t�@�C�����ۂ��Ɠǂݍ���
							fread64( SrcBuf, FileSize, SrcP ) ;
						
							// �n�t�}�����k����T�C�Y�ɂ���ď����𕪊�
							if( HuffmanEncodeKB == 0xff || FileSize <= HuffmanEncodeKB * 1024 * 2 )
							{
								// �n�t�}�����k�p�̃������̈���m��
								HuffData = ( u8 * )calloc( 1, ( size_t )( FileSize * 2 + 256 * 2 + 32 ) ) ;

								// �t�@�C���S�̂��n�t�}�����k
								File.HuffPressDataSize = Huffman_Encode( SrcBuf, FileSize, HuffData ) ;

								// ���k�f�[�^�Ɍ���K�p���ď����o��
								WriteSize = ( File.HuffPressDataSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
								KeyConvFileWrite( HuffData, WriteSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
							}
							else
							{
								// �n�t�}�����k�p�̃������̈���m��
								HuffData = ( u8 * )calloc( 1, HuffmanEncodeKB * 1024 * 2 * 4 + 256 * 2 + 32 ) ;

								// �t�@�C���̑O����n�t�}�����k
								memcpy( HuffData,                          SrcBuf,                                     HuffmanEncodeKB * 1024 ) ;
								memcpy( HuffData + HuffmanEncodeKB * 1024, SrcBuf + FileSize - HuffmanEncodeKB * 1024, HuffmanEncodeKB * 1024 ) ;
								File.HuffPressDataSize = Huffman_Encode( HuffData, HuffmanEncodeKB * 1024 * 2, HuffData + HuffmanEncodeKB * 1024 * 2 ) ;

								// �n�t�}�����k���������������o��
								KeyConvFileWrite( HuffData + HuffmanEncodeKB * 1024 * 2, File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
								
								// �n�t�}�����k���Ă��Ȃ��ӏ��������o��
								WriteSize = File.HuffPressDataSize + FileSize - HuffmanEncodeKB * 1024 * 2 ;
								WriteSize = ( WriteSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
								KeyConvFileWrite( SrcBuf + HuffmanEncodeKB * 1024, WriteSize - File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize + File.HuffPressDataSize ) ;
							}

							// �������̉��
							free( SrcBuf ) ;
							free( HuffData ) ;
						}
						else
						{
							// �]���J�n
							WriteSize = 0 ;
							while( WriteSize < FileSize )
							{
								// �]���T�C�Y����
								MoveSize = DXA_BUFFERSIZE < FileSize - WriteSize ? DXA_BUFFERSIZE : FileSize - WriteSize ;
								MoveSize = ( MoveSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
							
								// �t�@�C���̌��K�p�ǂݍ���
								memset( TempBuffer, 0, ( size_t )MoveSize ) ;
								KeyConvFileRead( TempBuffer, MoveSize, SrcP, NoKey ? NULL : lKey, File.DataSize + WriteSize ) ;

								// �����o��
								fwrite64( TempBuffer, MoveSize, DestFp ) ;
							
								// �����o���T�C�Y�̉��Z
								WriteSize += MoveSize ;
							}
						}
					}
					
					// �����o�����t�@�C�������
					fclose( SrcP ) ;
				
					// �f�[�^�T�C�Y�̉��Z
					Size->DataSize += WriteSize ;
				}
				
				// �t�@�C���w�b�_�������o��
				memcpy( FileP + Dir.FileHeadAddress + sizeof( DARC_FILEHEAD ) * i, &File, sizeof( DARC_FILEHEAD ) ) ;
			}
			
			i ++ ;
		}
		while( FindNextFileA( FindHandle, &FindData ) != 0 ) ;
		
		// Find �n���h�������
		FindClose( FindHandle ) ;
	}
						
	// ���Ƃ̃f�B���N�g�����J�����g�f�B���N�g���ɃZ�b�g
	SetCurrentDirectoryA( DirPath ) ;

	// �I��
	return 0 ;
}

// �w��̃f�B���N�g���f�[�^�ɂ���t�@�C����W�J����
int DXArchive::DirectoryDecode( u8 *NameP, u8 *DirP, u8 *FileP, DARC_HEAD *Head, DARC_DIRECTORY *Dir, FILE *ArcP, unsigned char *Key, const char *KeyString, size_t KeyStringBytes, bool NoKey, char *KeyStringBuffer )
{
	char DirPath[MAX_PATH] ;
	
	// ���݂̃J�����g�f�B���N�g����ۑ�
	GetCurrentDirectoryA( MAX_PATH, DirPath ) ;

	// �f�B���N�g����񂪂���ꍇ�́A�܂��W�J�p�̃f�B���N�g�����쐬����
	if( Dir->DirectoryAddress != 0xffffffffffffffff && Dir->ParentDirectoryAddress != 0xffffffffffffffff )
	{
		DARC_FILEHEAD *DirFile ;
		
		// DARC_FILEHEAD �̃A�h���X���擾
		DirFile = ( DARC_FILEHEAD * )( FileP + Dir->DirectoryAddress ) ;
		
		// �f�B���N�g���̍쐬
		CreateDirectoryA( GetOriginalFileName( NameP + DirFile->NameAddress ), NULL ) ;
		
		// ���̃f�B���N�g���ɃJ�����g�f�B���N�g�����ڂ�
		SetCurrentDirectoryA( GetOriginalFileName( NameP + DirFile->NameAddress ) ) ;
	}

	// �W�J�����J�n
	{
		u32 i, FileHeadSize ;
		DARC_FILEHEAD *File ;
		size_t KeyStringBufferBytes ;
		unsigned char lKey[ DXA_KEY_BYTES ] ;

		// �i�[����Ă���t�@�C���̐������J��Ԃ�
		FileHeadSize = sizeof( DARC_FILEHEAD ) ;
		File = ( DARC_FILEHEAD * )( FileP + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = (DARC_FILEHEAD *)( (u8 *)File + FileHeadSize ) )
		{
			// �f�B���N�g�����ǂ����ŏ����𕪊�
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���̏ꍇ�͍ċA��������
				DirectoryDecode( NameP, DirP, FileP, Head, ( DARC_DIRECTORY * )( DirP + File->DataAddress ), ArcP, Key, KeyString, KeyStringBytes, NoKey, KeyStringBuffer ) ;
			}
			else
			{
				FILE *DestP ;
				void *Buffer ;
			
				// �t�@�C���̏ꍇ�͓W�J����
				
				// �o�b�t�@���m�ۂ���
				Buffer = malloc( DXA_BUFFERSIZE ) ;
				if( Buffer == NULL ) return -1 ;

				// �t�@�C�����J��
				DestP = fopen( GetOriginalFileName( NameP + File->NameAddress ), "wb" ) ;

				// �t�@�C���ʂ̌����쐬
				if( NoKey == false )
				{
					KeyStringBufferBytes = CreateKeyFileString( ( int )Head->CharCodeFormat, KeyString, KeyStringBytes, Dir, File, FileP, DirP, NameP, ( BYTE * )KeyStringBuffer ) ;
					KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
				}

				// �f�[�^������ꍇ�̂ݓ]��
				if( File->DataSize != 0 )
				{
					void *temp ;

					// �����ʒu���Z�b�g����
					if( _ftelli64( ArcP ) != ( s32 )( Head->DataStartAddress + File->DataAddress ) )
						_fseeki64( ArcP, Head->DataStartAddress + File->DataAddress, SEEK_SET ) ;
						
					// �f�[�^�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( File->PressDataSize != 0xffffffffffffffff )
					{
						// ���k����Ă���ꍇ

						// �n�t�}�����k������Ă��邩�ǂ����ŏ����𕪊�
						if( File->HuffPressDataSize != 0xffffffffffffffff )
						{
							// ���k�f�[�^�����܂郁�����̈�̊m��
							temp = malloc( ( size_t )( File->PressDataSize + File->HuffPressDataSize + File->DataSize ) ) ;

							// ���k�f�[�^�̓ǂݍ���
							KeyConvFileRead( temp, File->HuffPressDataSize, ArcP, NoKey ? NULL : lKey, File->DataSize ) ;

							// �n�t�}�����k����
							Huffman_Decode( temp, (u8 *)temp + File->HuffPressDataSize ) ;

							// �t�@�C���̑O����n�t�}�����k���Ă���ꍇ�͏����𕪊�
							if( Head->HuffmanEncodeKB != 0xff && File->PressDataSize > Head->HuffmanEncodeKB * 1024 * 2 )
							{
								// �𓀂����f�[�^�̓��A��딼�����ړ�����
								memmove( 
									( u8 * )temp + File->HuffPressDataSize + File->PressDataSize - Head->HuffmanEncodeKB * 1024,
									( u8 * )temp + File->HuffPressDataSize + Head->HuffmanEncodeKB * 1024,
									Head->HuffmanEncodeKB * 1024 ) ;

								// �c���LZ���k�f�[�^��ǂݍ���
								KeyConvFileRead(
									( u8 * )temp + File->HuffPressDataSize + Head->HuffmanEncodeKB * 1024,
									File->PressDataSize - Head->HuffmanEncodeKB * 1024 * 2,
									ArcP, NoKey ? NULL : lKey, File->DataSize + File->HuffPressDataSize ) ;
							}
						
							// ��
							Decode( (u8 *)temp + File->HuffPressDataSize, (u8 *)temp + File->HuffPressDataSize + File->PressDataSize ) ;
						
							// �����o��
							fwrite64( (u8 *)temp + File->HuffPressDataSize + File->PressDataSize, File->DataSize, DestP ) ;
						
							// �������̉��
							free( temp ) ;
						}
						else
						{
							// ���k�f�[�^�����܂郁�����̈�̊m��
							temp = malloc( ( size_t )( File->PressDataSize + File->DataSize ) ) ;

							// ���k�f�[�^�̓ǂݍ���
							KeyConvFileRead( temp, File->PressDataSize, ArcP, NoKey ? NULL : lKey, File->DataSize ) ;
						
							// ��
							Decode( temp, (u8 *)temp + File->PressDataSize ) ;
						
							// �����o��
							fwrite64( (u8 *)temp + File->PressDataSize, File->DataSize, DestP ) ;
						
							// �������̉��
							free( temp ) ;
						}
					}
					else
					{
						// ���k����Ă��Ȃ��ꍇ
					
						// �n�t�}�����k�͂���Ă��邩�ǂ����ŏ����𕪊�
						if( File->HuffPressDataSize != 0xffffffffffffffff )
						{
							// ���k�f�[�^�����܂郁�����̈�̊m��
							temp = malloc( ( size_t )( File->HuffPressDataSize + File->DataSize ) ) ;

							// ���k�f�[�^�̓ǂݍ���
							KeyConvFileRead( temp, File->HuffPressDataSize, ArcP, NoKey ? NULL : lKey, File->DataSize ) ;

							// �n�t�}�����k����
							Huffman_Decode( temp, (u8 *)temp + File->HuffPressDataSize ) ;

							// �t�@�C���̑O��̂݃n�t�}�����k���Ă���ꍇ�͏����𕪊�
							if( Head->HuffmanEncodeKB != 0xff && File->DataSize > Head->HuffmanEncodeKB * 1024 * 2 )
							{
								// �𓀂����f�[�^�̓��A��딼�����ړ�����
								memmove( 
									( u8 * )temp + File->HuffPressDataSize + File->DataSize - Head->HuffmanEncodeKB * 1024,
									( u8 * )temp + File->HuffPressDataSize + Head->HuffmanEncodeKB * 1024,
									Head->HuffmanEncodeKB * 1024 ) ;

								// �c��̃f�[�^��ǂݍ���
								KeyConvFileRead(
									( u8 * )temp + File->HuffPressDataSize + Head->HuffmanEncodeKB * 1024,
									File->DataSize - Head->HuffmanEncodeKB * 1024 * 2,
									ArcP, NoKey ? NULL : lKey, File->DataSize + File->HuffPressDataSize ) ;
							}
						
							// �����o��
							fwrite64( (u8 *)temp + File->HuffPressDataSize, File->DataSize, DestP ) ;
						
							// �������̉��
							free( temp ) ;
						}
						else
						{
							u64 MoveSize, WriteSize ;
							
							// �]�������J�n
							WriteSize = 0 ;
							while( WriteSize < File->DataSize )
							{
								MoveSize = File->DataSize - WriteSize > DXA_BUFFERSIZE ? DXA_BUFFERSIZE : File->DataSize - WriteSize ;

								// �t�@�C���̔��]�ǂݍ���
								KeyConvFileRead( Buffer, MoveSize, ArcP, NoKey ? NULL : lKey, File->DataSize + WriteSize ) ;

								// �����o��
								fwrite64( Buffer, MoveSize, DestP ) ;
								
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

					HFile = CreateFileA( GetOriginalFileName( NameP + File->NameAddress ),
										GENERIC_WRITE, 0, NULL,
										OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
					if( HFile == INVALID_HANDLE_VALUE )
					{
						HFile = HFile ;
					}

					CreateTime.dwHighDateTime     = ( u32 )( File->Time.Create     >> 32        ) ;
					CreateTime.dwLowDateTime      = ( u32 )( File->Time.Create     & 0xffffffffffffffff ) ;
					LastAccessTime.dwHighDateTime = ( u32 )( File->Time.LastAccess >> 32        ) ;
					LastAccessTime.dwLowDateTime  = ( u32 )( File->Time.LastAccess & 0xffffffffffffffff ) ;
					LastWriteTime.dwHighDateTime  = ( u32 )( File->Time.LastWrite  >> 32        ) ;
					LastWriteTime.dwLowDateTime   = ( u32 )( File->Time.LastWrite  & 0xffffffffffffffff ) ;
					SetFileTime( HFile, &CreateTime, &LastAccessTime, &LastWriteTime ) ;
					CloseHandle( HFile ) ;
				}

				// �t�@�C��������t����
				SetFileAttributesA( GetOriginalFileName( NameP + File->NameAddress ), ( u32 )File->Attributes ) ;
			}
		}
	}
	
	// �J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectoryA( DirPath ) ;

	// �I��
	return 0 ;
}

// �f�B���N�g�����̃t�@�C���p�X���擾����
int DXArchive::GetDirectoryFilePath( const char *DirectoryPath, char *FileNameBuffer )
{
	WIN32_FIND_DATAA FindData ;
	HANDLE FindHandle ;
	int FileNum ;
	char DirPath[256], String[256] ;

	// �f�B���N�g�����ǂ������`�F�b�N����
	if( DirectoryPath[0] != '\0' )
	{
		FindHandle = FindFirstFileA( DirectoryPath, &FindData ) ;
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
	FindHandle = FindFirstFileA( String, &FindData ) ;
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
		while( FindNextFileA( FindHandle, &FindData ) != 0 ) ;
		FindClose( FindHandle ) ;
	}

	// ����Ԃ�
	return FileNum ;
}

// �G���R�[�h�̐i�s�󋵂�\������������
void DXArchive::EncodeStatusErase( void )
{
	static char StringBuffer[ 8192 ] ;

	if( LogStringLength > 0 )
	{
		int i ;
		int p = 0 ;
		for( i = 0 ; i < LogStringLength ; i ++ )
		{
			StringBuffer[ p ] = '\b' ;
			p ++ ;
		}
		for( i = 0 ; i < LogStringLength ; i ++ )
		{
			StringBuffer[ p ] = ' ' ;
			p ++ ;
		}
		for( i = 0 ; i < LogStringLength ; i ++ )
		{
			StringBuffer[ p ] = '\b' ;
			p ++ ;
		}
		StringBuffer[ p ] = '\0' ;
		printf( StringBuffer ) ;
	}
}

// �G���R�[�h�̐i�s�󋵂�\������
void DXArchive::EncodeStatusOutput( DARC_ENCODEINFO *EncodeInfo, bool Always )
{
	static char StringBuffer[ 8192 ] ;
	static char FileNameBuffer[ 2048 ] ;
	int FileNameLength ;
	unsigned int NowTime = GetTickCount() ;

	// �O�񂩂�16ms�ȏ㎞�Ԃ��o�߂��Ă�����\������
	if( Always == false && NowTime - EncodeInfo->PrevDispTime < 16 )
	{
		return ;
	}

	// �O��̕\�����e������
	EncodeStatusErase() ;

	EncodeInfo->PrevDispTime = NowTime ;
	strcpy( FileNameBuffer, EncodeInfo->ProcessFileName ) ;
	FileNameLength = strlen( EncodeInfo->ProcessFileName ) ;
	if( FileNameLength > 50 )
	{
		strncpy( FileNameBuffer, EncodeInfo->ProcessFileName, 50 ) ;
		strcpy( &FileNameBuffer[ 50 ], "..." ) ;
	}
	sprintf( StringBuffer, " [%d/%d] %d%%%%  %s", EncodeInfo->CompFileNum, EncodeInfo->TotalFileNum, EncodeInfo->CompFileNum * 100 / EncodeInfo->TotalFileNum, FileNameBuffer ) ;
	LogStringLength = strlen( StringBuffer ) ;
	printf( StringBuffer ) ;
}

// �n�t�}�����k������O��̃T�C�Y���擾����
void DXArchive::AnalyseHuffmanEncode( u64 DataSize, u8 HuffmanEncodeKB, u64 *HeadDataSize, u64 *FootDataSize )
{
	if( HuffmanEncodeKB == 0xff || DataSize < HuffmanEncodeKB * 1024 * 2 )
	{
		*HeadDataSize = DataSize ;
		*FootDataSize = 0 ;
	}
	else
	{
		*HeadDataSize = HuffmanEncodeKB * 1024 ;
		*FootDataSize = HuffmanEncodeKB * 1024 ;
	}
}

// �G���R�[�h( �߂�l:���k��̃T�C�Y  -1 �̓G���[  Dest �� NULL �����邱�Ƃ��\ )
int DXArchive::Encode( void *Src, u32 SrcSize, void *Dest, bool OutStatus )
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
	LZ_LIST *listbuf, *listtemp, *list, *newlist ;
	u8 *listfirsttable, *usesublistflagtable, *sublistbuf ;
	
	// �T�u���X�g�̃T�C�Y�����߂�
	{
			 if( SrcSize < 100 * 1024 )			sublistmaxnum = 1 ;
		else if( SrcSize < 3 * 1024 * 1024 )	sublistmaxnum = MAX_SUBLISTNUM / 3 ;
		else									sublistmaxnum = MAX_SUBLISTNUM ;
	}

	// ���X�g�̃T�C�Y�����߂�
	{
		maxlistnum = MAX_ADDRESSLISTNUM ;
		if( maxlistnum > SrcSize )
		{
			while( ( maxlistnum >> 1 ) > 0x100 && ( maxlistnum >> 1 ) > SrcSize )
				maxlistnum >>= 1 ;
		}
		maxlistnummask = maxlistnum - 1 ;
	}

	// �������̊m��
	usesublistflagtable   = (u8 *)malloc(
		sizeof( void * )  * 65536 +					// ���C�����X�g�̐擪�I�u�W�F�N�g�p�̈�
		sizeof( LZ_LIST ) * maxlistnum +			// ���C�����X�g�p�̈�
		sizeof( u8 )      * 65536 +					// �T�u���X�g���g�p���Ă��邩�t���O�p�̈�
		sizeof( void * )  * 256 * sublistmaxnum ) ;	// �T�u���X�g�p�̈�
		
	// �A�h���X�̃Z�b�g
	listfirsttable =     usesublistflagtable + sizeof(     u8 ) * 65536 ;
	sublistbuf     =          listfirsttable + sizeof( void * ) * 65536 ;
	listbuf        = (LZ_LIST *)( sublistbuf + sizeof( void * ) * 256 * sublistmaxnum ) ;
	
	// ������
	memset( usesublistflagtable, 0, sizeof(     u8 ) * 65536               ) ;
	memset(          sublistbuf, 0, sizeof( void * ) * 256 * sublistmaxnum ) ;
	memset(      listfirsttable, 0, sizeof( void * ) * 65536               ) ;
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
	if( OutStatus )
	{
		printf( " ���k     " ) ;
		LogStringLength += 10 ;
	}
	while( srcaddress < SrcSize )
	{
		// �c��T�C�Y���Œሳ�k�T�C�Y�ȉ��̏ꍇ�͈��k���������Ȃ�
		if( srcaddress + MIN_COMPRESS >= SrcSize ) goto NOENCODE ;

		// ���X�g���擾
		code = *((u16 *)sp) ;
		list = (LZ_LIST *)( listfirsttable + code * sizeof( void * ) ) ;
		if( usesublistflagtable[code] == 1 )
		{
			list = (LZ_LIST *)( (void **)list->next + sp[2] ) ;
		}
		else
		{
			if( sublistnum < sublistmaxnum )
			{
				list->next = (LZ_LIST *)( sublistbuf + sizeof( void * ) * 256 * sublistnum ) ;
				list       = (LZ_LIST *)( (void **)list->next + sp[2] ) ;
			
				usesublistflagtable[code] = 1 ;
				sublistnum ++ ;
			}
		}

		// ��Ԉ�v���̒����R�[�h��T��
		maxconbo   = -1 ;
		maxaddress = -1 ;
		maxbonus   = -1 ;
		for( m = 0, listtemp = list->next ; m < MAX_SEARCHLISTNUM && listtemp != NULL ; listtemp = listtemp->next, m ++ )
		{
			address = srcaddress - listtemp->address ;
			if( address >= MAX_POSITION )
			{
				if( listtemp->prev ) listtemp->prev->next = listtemp->next ;
				if( listtemp->next ) listtemp->next->prev = listtemp->prev ;
				listtemp->address = 0xffffffff ;
				continue ;
			}
			
			sp2 = &sp[-address] ;
			sp1 = sp ;
			if( srcaddress + MAX_COPYSIZE < SrcSize )
			{
				conbo = MAX_COPYSIZE / 4 ;
				while( conbo && *((u32 *)sp2) == *((u32 *)sp1) )
				{
					sp2 += 4 ;
					sp1 += 4 ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE - ( MAX_COPYSIZE / 4 - conbo ) * 4 ;

				while( conbo && *sp2 == *sp1 )
				{
					sp2 ++ ;
					sp1 ++ ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE - conbo ;
			}
			else
			{
				for( conbo = 0 ;
						conbo < MAX_COPYSIZE &&
						conbo + srcaddress < SrcSize &&
						sp[conbo - address] == sp[conbo] ;
							conbo ++ ){}
			}

			if( conbo >= 4 )
			{
				conbosize   = ( conbo - MIN_COMPRESS ) < 0x20 ? 0 : 1 ;
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

				// �o�͂���A�����͍Œ� MIN_COMPRESS ���邱�Ƃ��O��Ȃ̂� - MIN_COMPRESS �������̂��o�͂���
				maxconbo -= MIN_COMPRESS ;

				// �A�����O�`�S�r�b�g�ƘA�����A���΃A�h���X�̃r�b�g�����o��
				*dp = (u8)( ( ( maxconbo & 0x1f ) << 3 ) | ( maxconbosize << 2 ) | maxaddresssize ) ;

				// �L�[�R�[�h�̘A���̓L�[�R�[�h�ƒl�̓������񈳏k�R�[�h��
				// ���f���邽�߁A�L�[�R�[�h�̒l�ȏ�̏ꍇ�͒l���{�P����
				if( *dp >= keycode ) dp[0] += 1 ;
				dp ++ ;

				// �A�����T�`�P�Q�r�b�g���o��
				if( maxconbosize == 1 )
					*dp++ = (u8)( ( maxconbo >> 5 ) & 0xff ) ;

				// maxconbo �͂܂��g������ - MIN_COMPRESS ��������߂�
				maxconbo += MIN_COMPRESS ;

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
				for( j = 1 ; j < maxconbo && (u64)&sp2[2] - (u64)srcp < SrcSize ; j ++, sp2 ++ )
				{
					code = *((u16 *)sp2) ;
					list = (LZ_LIST *)( listfirsttable + code * sizeof( void * ) ) ;
					if( usesublistflagtable[code] == 1 )
					{
						list = (LZ_LIST *)( (void **)list->next + sp2[2] ) ;
					}
					else
					{
						if( sublistnum < sublistmaxnum )
						{
							list->next = (LZ_LIST *)( sublistbuf + sizeof( void * ) * 256 * sublistnum ) ;
							list       = (LZ_LIST *)( (void **)list->next + sp2[2] ) ;
						
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
			if( OutStatus ) printf( "\b\b\b\b%3d%%", (s32)( (f32)srcaddress * 100 / SrcSize ) ) ;
		}
	}

	if( OutStatus )
	{
		printf( "\b\b\b\b100%%" ) ;
	}

	// ���k��̃f�[�^�T�C�Y��ۑ�����
	*((u32 *)&destp[4]) = dstsize + 9 ;

	// �m�ۂ����������̉��
	free( usesublistflagtable ) ;

	// �f�[�^�̃T�C�Y��Ԃ�
	return dstsize + 9 ;
}

// �f�R�[�h( �߂�l:�𓀌�̃T�C�Y  -1 �̓G���[  Dest �� NULL �����邱�Ƃ��\ )
int DXArchive::Decode( void *Src, void *Dest )
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

// �o�C�i���f�[�^������ CRC32 �̃n�b�V���l���v�Z����
u32 DXArchive::HashCRC32( const void *SrcData, size_t SrcDataSize )
{
	static DWORD CRC32Table[ 256 ] ;
	static int CRC32TableInit = 0 ;
	DWORD CRC = 0xffffffff ;
	BYTE *SrcByte = ( BYTE * )SrcData ;
	DWORD i ;

	// �e�[�u��������������Ă��Ȃ������珉��������
	if( CRC32TableInit == 0 )
	{
		DWORD Magic = 0xedb88320 ;	// 0x4c11db7 ���r�b�g���x���ŏ��Ԃ��t�ɂ������̂� 0xedb88320
		DWORD j ;

		for( i = 0; i < 256; i++ )
		{
			DWORD Data = i ;
			for( j = 0; j < 8; j++ )
			{
				int b = ( Data & 1 ) ;
				Data >>= 1 ;
				if( b != 0 )
				{
					Data ^= Magic ;
				}
			}
			CRC32Table[ i ] = Data ;
		}

		// �e�[�u���������������t���O�𗧂Ă�
		CRC32TableInit = 1 ;
	}

	for( i = 0 ; i < SrcDataSize ; i ++ )
	{
		CRC = CRC32Table[ ( BYTE )( CRC ^ SrcByte[ i ] ) ] ^ ( CRC >> 8 ) ;
	}

	return CRC ^ 0xffffffff ;
}


// �A�[�J�C�u�t�@�C�����쐬����(�f�B���N�g�������)
int DXArchive::EncodeArchiveOneDirectory( char *OutputFileName, char *DirectoryPath, bool Press, bool AlwaysHuffman, u8 HuffmanEncodeKB, const char *KeyString_, bool NoKey, bool OutputStatus )
{
	int i, FileNum, Result ;
	char **FilePathList, *NameBuffer ;

	// �t�@�C���̐����擾����
	FileNum = GetDirectoryFilePath( DirectoryPath, NULL ) ;
	if( FileNum < 0 ) return -1 ;

	// �t�@�C���̐��̕������t�@�C�����ƃt�@�C���|�C���^�̊i�[�p�̃��������m�ۂ���
	NameBuffer = (char *)malloc( FileNum * ( 256 + sizeof( char * ) ) ) ;

	// �t�@�C���̃p�X���擾����
	GetDirectoryFilePath( DirectoryPath, NameBuffer ) ;

	// �t�@�C���p�X�̃��X�g���쐬����
	FilePathList = (char **)( NameBuffer + FileNum * 256 ) ;
	for( i = 0 ; i < FileNum ; i ++ )
		FilePathList[i] = NameBuffer + i * 256 ;
	
	// �G���R�[�h
	Result = EncodeArchive( OutputFileName, FilePathList, FileNum, Press, AlwaysHuffman, HuffmanEncodeKB, KeyString_, NoKey, OutputStatus ) ;

	// �m�ۂ����������̉��
	free( NameBuffer ) ;

	// ���ʂ�Ԃ�
	return Result ;
}

// �A�[�J�C�u�t�@�C�����쐬����
int DXArchive::EncodeArchive( char *OutputFileName, char **FileOrDirectoryPath, int FileNum, bool Press, bool AlwaysHuffman, u8 HuffmanEncodeKB, const char *KeyString_, bool NoKey, bool OutputStatus )
{
	DARC_HEAD Head ;
	DARC_DIRECTORY Directory, *DirectoryP ;
	u64 HeaderHuffDataSize ;
	SIZESAVE SizeSave ;
	FILE *DestFp ;
	u8 *NameP, *FileP, *DirP ;
	int i ;
	u32 Type ;
	void *TempBuffer ;
	u8 Key[ DXA_KEY_BYTES ] ;
	char KeyString[ DXA_KEY_STRING_LENGTH + 1 ] ;
	size_t KeyStringBytes ;
	char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
	DARC_ENCODEINFO EncodeInfo ;

	// �󋵏o�͂��s���ꍇ�̓t�@�C���̑����𐔂���
	EncodeInfo.CompFileNum = 0 ;
	EncodeInfo.TotalFileNum = 0 ;
	EncodeInfo.OutputStatus = OutputStatus ;
	if( EncodeInfo.OutputStatus )
	{
		for( i = 0 ; i < FileNum ; i++ )
		{
			// �w�肳�ꂽ�t�@�C�������邩�ǂ�������
			Type = GetFileAttributesA( FileOrDirectoryPath[i] ) ;
			if( ( signed int )Type == -1 ) continue ;

			// �t�@�C���̃^�C�v�ɂ���ď����𕪊�
			if( ( Type & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				FILE_INFOLIST FileList ;

				// �t�H���_�ȉ��̃t�@�C�����擾����
				CreateFileList( FileOrDirectoryPath[i], &FileList, TRUE, TRUE, NULL, NULL, NULL ) ;

				// �t�H���_�ȉ��̃t�@�C���������Z����
				EncodeInfo.TotalFileNum += FileList.Num ;

				// �t�@�C�����X�g���̌�n��
				ReleaseFileList( &FileList ) ;
			}
			else
			{
				// �t�@�C���������ꍇ�̓t�@�C���̐��𑝂₷
				EncodeInfo.TotalFileNum ++ ;
			}
		}
	}

	// ��������̕ۑ��ƌ��̍쐬
	if( NoKey == false )
	{
		// �w�肪�����ꍇ�̓f�t�H���g�̌���������g�p����
		if( KeyString_ == NULL )
		{
			KeyString_ = DefaultKeyString ;
		}

		KeyStringBytes = CL_strlen( CHARCODEFORMAT_ASCII, KeyString_ ) ;
		if( KeyStringBytes > DXA_KEY_STRING_LENGTH )
		{
			KeyStringBytes = DXA_KEY_STRING_LENGTH ;
		}
		memcpy( KeyString, KeyString_, KeyStringBytes ) ;
		KeyString[ KeyStringBytes ] = '\0' ;

		// ���̍쐬
		KeyCreate( KeyString, KeyStringBytes, Key ) ;
	}

	// �t�@�C���ǂݍ��݂Ɏg�p����o�b�t�@�̊m��
	TempBuffer = malloc( DXA_BUFFERSIZE ) ;

	// �o�̓t�@�C�����J��
	DestFp = fopen( OutputFileName, "wb" ) ;

	// �A�[�J�C�u�̃w�b�_���o�͂���
	{
		memset( &Head, 0, sizeof( Head ) ) ;
		Head.Head						= DXA_HEAD ;
		Head.Version					= DXA_VER ;
		Head.HeadSize					= 0xffffffff ;
		Head.DataStartAddress			= sizeof( DARC_HEAD ) ;
		Head.FileNameTableStartAddress	= 0xffffffffffffffff ;
		Head.DirectoryTableStartAddress	= 0xffffffffffffffff ;
		Head.FileTableStartAddress		= 0xffffffffffffffff ;
		Head.CharCodeFormat				= GetACP() ;
		Head.Flags						= 0 ;
		Head.HuffmanEncodeKB			= HuffmanEncodeKB ;
		if( NoKey )						Head.Flags |= DXA_FLAG_NO_KEY ;
		if( Press == false )			Head.Flags |= DXA_FLAG_NO_HEAD_PRESS ;
		SetFileApisToANSI() ;

		KeyConvFileWrite( &Head, sizeof( DARC_HEAD ), DestFp, NoKey ? NULL : Key, 0 ) ;
	}
	
	// �e�o�b�t�@���m�ۂ���
	NameP = ( u8 * )malloc( DXA_BUFFERSIZE ) ;
	if( NameP == NULL ) return -1 ;
	memset( NameP, 0, DXA_BUFFERSIZE ) ;

	FileP = ( u8 * )malloc( DXA_BUFFERSIZE ) ;
	if( FileP == NULL ) return -1 ;
	memset( FileP, 0, DXA_BUFFERSIZE ) ;

	DirP = ( u8 * )malloc( DXA_BUFFERSIZE ) ;
	if( DirP == NULL ) return -1 ;
	memset( DirP, 0, DXA_BUFFERSIZE ) ;

	// �T�C�Y�ۑ��\���̂Ƀf�[�^���Z�b�g
	SizeSave.DataSize		= 0 ;
	SizeSave.NameSize		= 0 ;
	SizeSave.DirectorySize	= 0 ;
	SizeSave.FileSize		= 0 ;
	
	// �ŏ��̃f�B���N�g���̃t�@�C�����������o��
	{
		DARC_FILEHEAD File ;
		
		memset( &File, 0, sizeof( DARC_FILEHEAD ) ) ;
		File.NameAddress	= SizeSave.NameSize ;
		File.Attributes		= FILE_ATTRIBUTE_DIRECTORY ;
		File.DataAddress	= SizeSave.DirectorySize ;
		File.DataSize		= 0 ;
		File.PressDataSize  = 0xffffffffffffffff ;

		// �f�B���N�g�����̏����o��
		SizeSave.NameSize += AddFileNameData( ( int )Head.CharCodeFormat, "", NameP + SizeSave.NameSize ) ;

		// �t�@�C�����̏����o��
		memcpy( FileP + SizeSave.FileSize, &File, sizeof( DARC_FILEHEAD ) ) ;
		SizeSave.FileSize += sizeof( DARC_FILEHEAD ) ;
	}

	// �ŏ��̃f�B���N�g�����������o��
	Directory.DirectoryAddress 			= 0 ;
	Directory.ParentDirectoryAddress 	= 0xffffffffffffffff ;
	Directory.FileHeadNum 				= FileNum ;
	Directory.FileHeadAddress 			= SizeSave.FileSize ;
	memcpy( DirP + SizeSave.DirectorySize, &Directory, sizeof( DARC_DIRECTORY ) ) ;
	DirectoryP = ( DARC_DIRECTORY * )( DirP + SizeSave.DirectorySize ) ;

	// �T�C�Y�����Z����
	SizeSave.DirectorySize 	+= sizeof( DARC_DIRECTORY ) ;
	SizeSave.FileSize 		+= sizeof( DARC_FILEHEAD ) * FileNum ;

	// �n���ꂽ�t�@�C���̐������������J��Ԃ�
	for( i = 0 ; i < FileNum ; i ++ )
	{
		// �w�肳�ꂽ�t�@�C�������邩�ǂ�������
		Type = GetFileAttributesA( FileOrDirectoryPath[i] ) ;
		if( ( signed int )Type == -1 ) continue ;

		// �t�@�C���̃^�C�v�ɂ���ď����𕪊�
		if( ( Type & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
		{
			// �f�B���N�g���̏ꍇ�̓f�B���N�g���̃A�[�J�C�u�ɉ�
			DirectoryEncode( ( int )Head.CharCodeFormat, FileOrDirectoryPath[i], NameP, DirP, FileP, &Directory, &SizeSave, i, DestFp, TempBuffer, Press, AlwaysHuffman, HuffmanEncodeKB, KeyString, KeyStringBytes, NoKey, KeyStringBuffer, &EncodeInfo ) ;
		}
		else
		{
			WIN32_FIND_DATAA FindData ;
			HANDLE FindHandle ;
			DARC_FILEHEAD File ;
			u8 lKey[DXA_KEY_BYTES] ;
			size_t KeyStringBufferBytes ;
	
			// �t�@�C���̏��𓾂�
			FindHandle = FindFirstFileA( FileOrDirectoryPath[i], &FindData ) ;
			if( FindHandle == INVALID_HANDLE_VALUE ) continue ;

			// �i�s�󋵏o��
			if( EncodeInfo.OutputStatus )
			{
				// �����t�@�C�������Z�b�g
				strcpy( EncodeInfo.ProcessFileName, FindData.cFileName ) ;

				// �t�@�C�����𑝂₷
				EncodeInfo.CompFileNum ++ ;

				// �\��
				EncodeStatusOutput( &EncodeInfo ) ;
			}

			// �t�@�C���w�b�_���Z�b�g����
			{
				File.NameAddress       = SizeSave.NameSize ;
				File.Time.Create       = ( ( ( LONGLONG )FindData.ftCreationTime.dwHighDateTime   ) << 32 ) + FindData.ftCreationTime.dwLowDateTime   ;
				File.Time.LastAccess   = ( ( ( LONGLONG )FindData.ftLastAccessTime.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime.dwLowDateTime ;
				File.Time.LastWrite    = ( ( ( LONGLONG )FindData.ftLastWriteTime.dwHighDateTime  ) << 32 ) + FindData.ftLastWriteTime.dwLowDateTime  ;
				File.Attributes        = FindData.dwFileAttributes ;
				File.DataAddress       = SizeSave.DataSize ;
				File.DataSize          = ( ( ( LONGLONG )FindData.nFileSizeHigh ) << 32 ) + FindData.nFileSizeLow ;
				File.PressDataSize	   = 0xffffffffffffffff ;
				File.HuffPressDataSize = 0xffffffffffffffff ;
			}

			// �t�@�C�����������o��
			SizeSave.NameSize += AddFileNameData( ( int )Head.CharCodeFormat, FindData.cFileName, NameP + SizeSave.NameSize ) ;

			// �t�@�C���ʂ̌����쐬
			if( NoKey == false )
			{
				KeyStringBufferBytes = CreateKeyFileString( ( int )Head.CharCodeFormat, KeyString, KeyStringBytes, DirectoryP, &File, FileP, DirP, NameP, ( BYTE * )KeyStringBuffer ) ;
				KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey  ) ;
			}

			// �t�@�C���f�[�^�������o��
			if( File.DataSize != 0 )
			{
				FILE *SrcP ;
				u64 FileSize, WriteSize, MoveSize ;
				bool Huffman = false ;
				bool AlwaysPress = false ;

				// �t�@�C�����J��
				SrcP = fopen( FileOrDirectoryPath[i], "rb" ) ;
				
				// �T�C�Y�𓾂�
				_fseeki64( SrcP, 0, SEEK_END ) ;
				FileSize = _ftelli64( SrcP ) ;
				_fseeki64( SrcP, 0, SEEK_SET ) ;

				// ���k�̑ΏۂƂȂ�t�@�C���t�H�[�}�b�g�����ׂ�
				{
					u32 Len ;
					Len = ( u32 )strlen( FindData.cFileName ) ;
					if( Len > 4 )
					{
						char *sp ;
						
						sp = &FindData.cFileName[Len-3] ;
						if( StrICmp( sp, "wav" ) == 0 ||
							StrICmp( sp, "jpg" ) == 0 ||
							StrICmp( sp, "png" ) == 0 ||
							StrICmp( sp, "mpg" ) == 0 ||
							StrICmp( sp, "mp3" ) == 0 ||
							StrICmp( sp, "mp4" ) == 0 ||
							StrICmp( sp, "m4a" ) == 0 ||
							StrICmp( sp, "ogg" ) == 0 ||
							StrICmp( sp, "ogv" ) == 0 ||
							StrICmp( sp, "ops" ) == 0 ||
							StrICmp( sp, "wmv" ) == 0 ||
							StrICmp( sp, "tif" ) == 0 ||
							StrICmp( sp, "tga" ) == 0 ||
							StrICmp( sp, "bmp" ) == 0 ||
							StrICmp( sp - 1, "jpeg" ) == 0 )
						{
							Huffman = true ;
						}

						// wav �� bmp �̏ꍇ�͕K�����k����
						if( StrICmp( sp, "wav" ) == 0 ||
							StrICmp( sp, "tga" ) == 0 ||
							StrICmp( sp, "bmp" ) == 0 )
						{
							AlwaysPress = true ;
						}
					}
				}

				// AlwaysHuffman �� true �̏ꍇ�͕K���n�t�}�����k����
				if( AlwaysHuffman )
				{
					Huffman = true ;
				}

				// �n�t�}�����k����T�C�Y�� 0 �̏ꍇ�̓n�t�}�����k���s��Ȃ�
				if( HuffmanEncodeKB == 0 )
				{
					Huffman = false ;
				}

				// ���k�̎w�肪����ꍇ�ŁA
				// �K�����k����t�@�C���t�H�[�}�b�g���A�t�@�C���T�C�Y�� 10MB �ȉ��̏ꍇ�͈��k�����݂�
				if( Press == true && ( AlwaysPress || File.DataSize < 10 * 1024 * 1024 ) )
				{
					void *SrcBuf, *DestBuf ;
					u32 DestSize, Len ;
					
					// �ꕔ�̃t�@�C���`���̏ꍇ�͗\�ߒe��
					if( AlwaysPress == false && ( Len = ( int )strlen( FindData.cFileName ) ) > 4 )
					{
						char *sp ;
						
						sp = &FindData.cFileName[Len-3] ;
						if( StrICmp( sp, "wav" ) == 0 ||
							StrICmp( sp, "jpg" ) == 0 ||
							StrICmp( sp, "png" ) == 0 ||
							StrICmp( sp, "mpg" ) == 0 ||
							StrICmp( sp, "mp3" ) == 0 ||
							StrICmp( sp, "mp4" ) == 0 ||
							StrICmp( sp, "ogg" ) == 0 ||
							StrICmp( sp, "ogv" ) == 0 ||
							StrICmp( sp, "ops" ) == 0 ||
							StrICmp( sp, "wmv" ) == 0 ||
							StrICmp( sp - 1, "jpeg" ) == 0 ) goto NOPRESS ;
					}

					// �f�[�^���ۂ��Ɠ��郁�����̈�̊m��
					SrcBuf  = calloc( 1, ( size_t )( FileSize + FileSize * 2 + 64 ) ) ;
					DestBuf = (u8 *)SrcBuf + FileSize ;
					
					// �t�@�C�����ۂ��Ɠǂݍ���
					fread64( SrcBuf, FileSize, SrcP ) ;

					// ���k����ꍇ�͋����I�ɐi�s�󋵏o�͂��X�V
					if( EncodeInfo.OutputStatus )
					{
						EncodeStatusOutput( &EncodeInfo, true ) ;
					}
					
					// ���k
					DestSize = Encode( SrcBuf, ( u32 )FileSize, DestBuf, EncodeInfo.OutputStatus ) ;
					
					// �w�ǈ��k�o���Ȃ������ꍇ�͈��k�����ŃA�[�J�C�u����
					if( AlwaysPress == false && ( (f64)DestSize / (f64)FileSize > 0.90 ) )
					{
						_fseeki64( SrcP, 0L, SEEK_SET ) ;
						free( SrcBuf ) ;
						goto NOPRESS ;
					}

					// ���k�f�[�^�̃T�C�Y��ۑ�����
					File.PressDataSize = DestSize ;

					// �n�t�}�����k���s�����ǂ����ŏ����𕪊�
					if( Huffman )
					{
						u8 *HuffData ;

						// �n�t�}�����k����T�C�Y�ɂ���ď����𕪊�
						if( HuffmanEncodeKB == 0xff || DestSize <= ( u64 )( HuffmanEncodeKB * 1024 * 2 ) )
						{
							// �n�t�}�����k�p�̃������̈���m��
							HuffData = ( u8 * )calloc( 1, DestSize * 2 + 256 * 2 + 32 ) ;

							// �t�@�C���S�̂��n�t�}�����k
							File.HuffPressDataSize = Huffman_Encode( DestBuf, DestSize, HuffData ) ;

							// ���k�f�[�^�Ɍ���K�p���ď����o��
							WriteSize = ( File.HuffPressDataSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
							KeyConvFileWrite( HuffData, WriteSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
						}
						else
						{
							// �n�t�}�����k�p�̃������̈���m��
							HuffData = ( u8 * )calloc( 1, HuffmanEncodeKB * 1024 * 2 * 4 + 256 * 2 + 32 ) ;

							// �t�@�C���̑O����n�t�}�����k
							memcpy( HuffData,                                  DestBuf,                                     HuffmanEncodeKB * 1024 ) ;
							memcpy( HuffData + HuffmanEncodeKB * 1024, ( u8 * )DestBuf + DestSize - HuffmanEncodeKB * 1024, HuffmanEncodeKB * 1024 ) ;
							File.HuffPressDataSize = Huffman_Encode( HuffData, HuffmanEncodeKB * 1024 * 2, HuffData + HuffmanEncodeKB * 1024 * 2 ) ;

							// �n�t�}�����k���������������o��
							KeyConvFileWrite( HuffData + HuffmanEncodeKB * 1024 * 2, File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;

							// �n�t�}�����k���Ă��Ȃ��ӏ��������o��
							WriteSize = File.HuffPressDataSize + DestSize - HuffmanEncodeKB * 1024 * 2 ;
							WriteSize = ( WriteSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
							KeyConvFileWrite( ( u8 * )DestBuf + HuffmanEncodeKB * 1024, WriteSize - File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize + File.HuffPressDataSize ) ;
						}

						// �������̉��
						free( HuffData ) ;
					}
					else
					{
						// ���k�f�[�^�𔽓]���ď����o��
						WriteSize = ( DestSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
						KeyConvFileWrite( DestBuf, WriteSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
					}

					// �������̉��
					free( SrcBuf ) ;
				}
				else
				{
NOPRESS:
					// �n�t�}�����k���s�����ǂ����ŏ����𕪊�
					if( Press && Huffman )
					{
						u8 *SrcBuf, *HuffData ;

						// �f�[�^���ۂ��Ɠ��郁�����̈�̊m��
						SrcBuf = ( u8 * )calloc( 1, ( size_t )( FileSize + 32 ) ) ;

						// �t�@�C�����ۂ��Ɠǂݍ���
						fread64( SrcBuf, FileSize, SrcP ) ;
						
						// �n�t�}�����k����T�C�Y�ɂ���ď����𕪊�
						if( HuffmanEncodeKB == 0xff || FileSize <= HuffmanEncodeKB * 1024 * 2 )
						{
							// �n�t�}�����k�p�̃������̈���m��
							HuffData = ( u8 * )calloc( 1, ( size_t )( FileSize * 2 + 256 * 2 + 32 ) ) ;

							// �t�@�C���S�̂��n�t�}�����k
							File.HuffPressDataSize = Huffman_Encode( SrcBuf, FileSize, HuffData ) ;

							// ���k�f�[�^�Ɍ���K�p���ď����o��
							WriteSize = ( File.HuffPressDataSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
							KeyConvFileWrite( HuffData, WriteSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;
						}
						else
						{
							// �n�t�}�����k�p�̃������̈���m��
							HuffData = ( u8 * )calloc( 1, HuffmanEncodeKB * 1024 * 2 * 4 + 256 * 2 + 32 ) ;

							// �t�@�C���̑O����n�t�}�����k
							memcpy( HuffData,                          SrcBuf,                                     HuffmanEncodeKB * 1024 ) ;
							memcpy( HuffData + HuffmanEncodeKB * 1024, SrcBuf + FileSize - HuffmanEncodeKB * 1024, HuffmanEncodeKB * 1024 ) ;
							File.HuffPressDataSize = Huffman_Encode( HuffData, HuffmanEncodeKB * 1024 * 2, HuffData + HuffmanEncodeKB * 1024 * 2 ) ;

							// �n�t�}�����k���������������o��
							KeyConvFileWrite( HuffData + HuffmanEncodeKB * 1024 * 2, File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize ) ;

							// �n�t�}�����k���Ă��Ȃ��ӏ��������o��
							WriteSize = File.HuffPressDataSize + FileSize - HuffmanEncodeKB * 1024 * 2 ;
							WriteSize = ( WriteSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
							KeyConvFileWrite( SrcBuf + HuffmanEncodeKB * 1024, WriteSize - File.HuffPressDataSize, DestFp, NoKey ? NULL : lKey, File.DataSize + File.HuffPressDataSize ) ;
						}

						// �������̉��
						free( SrcBuf ) ;
						free( HuffData ) ;
					}
					else
					{
						// �]���J�n
						WriteSize = 0 ;
						while( WriteSize < FileSize )
						{
							// �]���T�C�Y����
							MoveSize = DXA_BUFFERSIZE < FileSize - WriteSize ? DXA_BUFFERSIZE : FileSize - WriteSize ;
							MoveSize = ( MoveSize + 3 ) / 4 * 4 ;	// �T�C�Y�͂S�̔{���ɍ��킹��
						
							// �t�@�C���̌��K�p�ǂݍ���
							memset( TempBuffer, 0, ( size_t )MoveSize ) ;
							KeyConvFileRead( TempBuffer, MoveSize, SrcP, NoKey ? NULL : lKey, File.DataSize + WriteSize ) ;

							// �����o��
							fwrite64( TempBuffer, MoveSize, DestFp ) ;
						
							// �����o���T�C�Y�̉��Z
							WriteSize += MoveSize ;
						}
					}
				}
				
				// �����o�����t�@�C�������
				fclose( SrcP ) ;
			
				// �f�[�^�T�C�Y�̉��Z
				SizeSave.DataSize += WriteSize ;
			}
			
			// �t�@�C���w�b�_�������o��
			memcpy( FileP + Directory.FileHeadAddress + sizeof( DARC_FILEHEAD ) * i, &File, sizeof( DARC_FILEHEAD ) ) ;

			// Find �n���h�������
			FindClose( FindHandle ) ;
		}
	}
	
	// �o�b�t�@�ɗ��ߍ��񂾊e��w�b�_�f�[�^���o�͂���
	{
		u8 *PressSource ;
		u64 TotalSize = SizeSave.NameSize + SizeSave.FileSize + SizeSave.DirectorySize ;

		// �S���̃f�[�^��Z�߂�
		PressSource = ( u8 * )malloc( ( size_t )TotalSize ) ;
		if( PressSource == NULL ) return -1 ;
		memcpy( PressSource,                                         NameP, ( size_t )SizeSave.NameSize ) ;
		memcpy( PressSource + SizeSave.NameSize,                     FileP, ( size_t )SizeSave.FileSize ) ;
		memcpy( PressSource + SizeSave.NameSize + SizeSave.FileSize, DirP,  ( size_t )SizeSave.DirectorySize ) ;

		// ���k���邩�ǂ����ŏ����𕪊�
		if( Press )
		{
			u8 *PressData ;
			int LZDataSize ;

			// ���k����ꍇ

			// ���k�f�[�^���i�[���郁�����̈�̊m��
			PressData = ( u8 * )malloc( ( size_t )( TotalSize * 4 + 256 * 2 + 32 * 2 ) ) ;
			if( PressData == NULL ) return -1 ;

			// LZ���k
			LZDataSize = Encode( PressSource, ( u32 )TotalSize, PressData, false ) ;

			// �n�t�}�����k
			HeaderHuffDataSize = Huffman_Encode( PressData, ( u64 )LZDataSize, PressData + TotalSize * 2 + 32 ) ;

			// �Z�߂����̂Ɍ���K�p���ďo��
			KeyConvFileWrite( PressData + TotalSize * 2 + 32, HeaderHuffDataSize, DestFp, NoKey ? NULL : Key, 0 ) ;

			// �������̉��
			free( PressData ) ;
		}
		else
		{
			// �Z�߂����̂Ɍ���K�p���ďo��
			KeyConvFileWrite( PressSource, TotalSize, DestFp, NoKey ? NULL : Key, 0 ) ;
		}

		// �������̉��
		free( PressSource ) ;
	}
		
	// �ĂуA�[�J�C�u�̃w�b�_���o�͂���
	{
		Head.Head                       = DXA_HEAD ;
		Head.Version                    = DXA_VER ;
		Head.HeadSize                   = ( u32 )( SizeSave.NameSize + SizeSave.FileSize + SizeSave.DirectorySize ) ;
		Head.DataStartAddress           = sizeof( DARC_HEAD ) ;
		Head.FileNameTableStartAddress  = SizeSave.DataSize + Head.DataStartAddress ;
		Head.FileTableStartAddress      = SizeSave.NameSize ;
		Head.DirectoryTableStartAddress = Head.FileTableStartAddress + SizeSave.FileSize ;

		_fseeki64( DestFp, 0, SEEK_SET ) ;
		fwrite64( &Head, sizeof( DARC_HEAD ), DestFp ) ;
	}
	
	// �����o�����t�@�C�������
	fclose( DestFp ) ;
	
	// �m�ۂ����o�b�t�@���J������
	free( NameP ) ;
	free( FileP ) ;
	free( DirP ) ;
	free( TempBuffer ) ;

	// ���k�󋵕\�����N���A
	EncodeStatusErase() ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C����W�J����
int DXArchive::DecodeArchive( char *ArchiveName, char *OutputPath, const char *KeyString_ )
{
	u8 *HeadBuffer = NULL ;
	DARC_HEAD Head ;
	u8 *FileP, *NameP, *DirP ;
	FILE *ArcP = NULL ;
	char OldDir[MAX_PATH] ;
	u8 Key[DXA_KEY_BYTES] ;
	char KeyString[ DXA_KEY_STRING_LENGTH + 1 ] ;
	size_t KeyStringBytes ;
	char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
	bool NoKey ;

	// ��������̕ۑ��ƌ��̍쐬
	{
		// �w�肪�����ꍇ�̓f�t�H���g�̌���������g�p����
		if( KeyString_ == NULL )
		{
			KeyString_ = DefaultKeyString ;
		}

		KeyStringBytes = CL_strlen( CHARCODEFORMAT_ASCII, KeyString_ ) ;
		if( KeyStringBytes > DXA_KEY_STRING_LENGTH )
		{
			KeyStringBytes = DXA_KEY_STRING_LENGTH ;
		}
		memcpy( KeyString, KeyString_, KeyStringBytes ) ;
		KeyString[ KeyStringBytes ] = '\0' ;

		// ���̍쐬
		KeyCreate( KeyString, KeyStringBytes, Key ) ;
	}

	// �A�[�J�C�u�t�@�C�����J��
	ArcP = fopen( ArchiveName, "rb" ) ;
	if( ArcP == NULL ) return -1 ;

	// �o�͐�̃f�B���N�g���ɃJ�����g�f�B���N�g����ύX����
	GetCurrentDirectoryA( MAX_PATH, OldDir ) ;
	SetCurrentDirectoryA( OutputPath ) ;

	// �w�b�_����͂���
	{
		s64 FileSize ;

		// �w�b�_�̓ǂݍ���
		fread64( &Head, sizeof( DARC_HEAD ), ArcP ) ;
		
		// �h�c�̌���
		if( Head.Head != DXA_HEAD )
		{
			goto ERR ;
		}
		
		// �o�[�W��������
		if( Head.Version > DXA_VER || Head.Version < DXA_VER_MIN ) goto ERR ;

		// ���������s���Ă��Ȃ������擾����
		NoKey = ( Head.Flags & DXA_FLAG_NO_KEY ) != 0 ;
		
		// �w�b�_�̃T�C�Y���̃��������m�ۂ���
		HeadBuffer = ( u8 * )malloc( ( size_t )Head.HeadSize ) ;
		if( HeadBuffer == NULL ) goto ERR ;

		// �w�b�_�����k����Ă���ꍇ�͉𓀂���
		if( ( Head.Flags & DXA_FLAG_NO_HEAD_PRESS ) != 0 )
		{
			// ���k����Ă��Ȃ��ꍇ�͕��ʂɓǂݍ���
			KeyConvFileRead( HeadBuffer, Head.HeadSize, ArcP, NoKey ? NULL : Key, 0 ) ;
		}
		else
		{
			void *HuffHeadBuffer ;
			u64 HuffHeadSize ;
			void *LzHeadBuffer ;
			u64 LzHeadSize ;

			// �n�t�}�����k���ꂽ�w�b�_�̃T�C�Y���擾����
			_fseeki64( ArcP, 0, SEEK_END ) ;
			FileSize = _ftelli64( ArcP ) ;
			_fseeki64( ArcP, Head.FileNameTableStartAddress, SEEK_SET ) ;
			HuffHeadSize = ( u32 )( FileSize - _ftelli64( ArcP ) ) ;

			// �n�t�}�����k���ꂽ�w�b�_��ǂݍ��ރ��������m�ۂ���
			HuffHeadBuffer = malloc( ( size_t )HuffHeadSize ) ;
			if( HuffHeadBuffer == NULL ) goto ERR ;

			// �n�t�}�����k���ꂽ�w�b�_���R�s�[�ƈÍ�������
			KeyConvFileRead( HuffHeadBuffer, HuffHeadSize, ArcP, NoKey ? NULL : Key, 0 ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̗e�ʂ��擾����
			LzHeadSize = Huffman_Decode( HuffHeadBuffer, NULL ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̃f�[�^���i�[���郁�����p��̊m��
			LzHeadBuffer = malloc( ( size_t )LzHeadSize ) ;
			if( LzHeadBuffer == NULL )
			{
				free( HuffHeadBuffer ) ;
				goto ERR ;
			}

			// �n�t�}�����k���ꂽ�w�b�_���𓀂���
			Huffman_Decode( HuffHeadBuffer, LzHeadBuffer ) ;

			// LZ���k���ꂽ�w�b�_���𓀂���
			Decode( LzHeadBuffer, HeadBuffer ) ;

			// �������̉��
			free( HuffHeadBuffer ) ;
			free( LzHeadBuffer ) ;
		}

		// �e�A�h���X���Z�b�g����
		NameP = HeadBuffer ;
		FileP = NameP + Head.FileTableStartAddress ;
		DirP  = NameP + Head.DirectoryTableStartAddress ;
	}

	// �A�[�J�C�u�̓W�J���J�n����
	DirectoryDecode( NameP, DirP, FileP, &Head, ( DARC_DIRECTORY * )DirP, ArcP, Key, KeyString, KeyStringBytes, NoKey, KeyStringBuffer ) ;
	
	// �t�@�C�������
	fclose( ArcP ) ;
	
	// �w�b�_��ǂݍ���ł������������������
	free( HeadBuffer ) ;

	// �J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectoryA( OldDir ) ;

	// �I��
	return 0 ;

ERR :
	if( HeadBuffer != NULL ) free( HeadBuffer ) ;
	if( ArcP != NULL ) fclose( ArcP ) ;

	// �J�����g�f�B���N�g�������ɖ߂�
	SetCurrentDirectoryA( OldDir ) ;

	// �I��
	return -1 ;
}



// �R���X�g���N�^
DXArchive::DXArchive( char *ArchivePath )
{
	this->fp = NULL ;
	this->HeadBuffer = NULL ;
	this->NameP = this->DirP = this->FileP = NULL ;
	this->CurrentDirectory = NULL ;
	this->CacheBuffer = NULL ;

	if( ArchivePath != NULL )
	{
		this->OpenArchiveFile( ArchivePath ) ;
	}
}

// �f�X�g���N�^
DXArchive::~DXArchive()
{
	if( this->fp != NULL ) this->CloseArchiveFile() ;

	if( this->fp != NULL ) fclose( this->fp ) ;
	if( this->HeadBuffer != NULL ) free( this->HeadBuffer ) ;
	if( this->CacheBuffer != NULL ) free( this->CacheBuffer ) ;

	this->fp = NULL ;
	this->HeadBuffer = NULL ;
	this->NameP = this->DirP = this->FileP = NULL ;
	this->CurrentDirectory = NULL ;
}

// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )
int DXArchive::DirectoryKeyConv( DARC_DIRECTORY *Dir, char *KeyStringBuffer )
{
	// �������C���[�W�ł͂Ȃ��ꍇ�̓G���[
	if( this->MemoryOpenFlag == false )
		return -1 ;

	// �����g��Ȃ��ꍇ�͉��������ɏI��
	if( this->NoKey )
		return -1 ;
	
	// �Í������������J�n
	{
		u32 i, FileHeadSize ;
		DARC_FILEHEAD *File ;
		unsigned char lKey[ DXA_KEY_BYTES ] ;
		size_t KeyStringBufferBytes ;

		// �i�[����Ă���t�@�C���̐������J��Ԃ�
		FileHeadSize = sizeof( DARC_FILEHEAD ) ;
		File = ( DARC_FILEHEAD * )( this->FileP + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = (DARC_FILEHEAD *)( (u8 *)File + FileHeadSize ) )
		{
			// �f�B���N�g�����ǂ����ŏ����𕪊�
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���̏ꍇ�͍ċA��������
				DirectoryKeyConv( ( DARC_DIRECTORY * )( this->DirP + File->DataAddress ), KeyStringBuffer ) ;
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

					// �t�@�C���ʂ̌����쐬
					if( NoKey == false )
					{
						KeyStringBufferBytes = CreateKeyFileString( ( int )this->Head.CharCodeFormat, this->KeyString, this->KeyStringBytes, Dir, File, this->FileP, this->DirP, this->NameP, ( BYTE * )KeyStringBuffer ) ;
						KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
					}

					// �n�t�}�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( File->HuffPressDataSize != 0xffffffffffffffff )
					{
						// �n�t�}�����k����Ă���ꍇ
						KeyConv( DataP, File->HuffPressDataSize, File->DataSize, lKey ) ;
					}
					else
					// �f�[�^�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( File->PressDataSize != 0xffffffffffffffff )
					{
						// ���k����Ă���ꍇ
						KeyConv( DataP, File->PressDataSize, File->DataSize, lKey ) ;
					}
					else
					{
						// ���k����Ă��Ȃ��ꍇ
						KeyConv( DataP, File->DataSize, File->DataSize, lKey ) ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C�����J��
int DXArchive::OpenArchiveFile( const char *ArchivePath, const char *KeyString_ )
{
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( this->fp != NULL ) return -1 ;

	// �A�[�J�C�u�t�@�C�����J�����Ǝ��݂�
	this->fp = fopen( ArchivePath, "rb" ) ;
	if( this->fp == NULL ) return -1 ;

	// ��������̕ۑ��ƌ��̍쐬
	{
		// �w�肪�����ꍇ�̓f�t�H���g�̌���������g�p����
		if( KeyString_ == NULL )
		{
			KeyString_ = DefaultKeyString ;
		}

		KeyStringBytes = CL_strlen( CHARCODEFORMAT_ASCII, KeyString_ ) ;
		if( KeyStringBytes > DXA_KEY_STRING_LENGTH )
		{
			KeyStringBytes = DXA_KEY_STRING_LENGTH ;
		}
		memcpy( KeyString, KeyString_, KeyStringBytes ) ;
		KeyString[ KeyStringBytes ] = '\0' ;

		// ���̍쐬
		KeyCreate( KeyString, KeyStringBytes, Key ) ;
	}

	// �w�b�_����͂���
	{
		// �w�b�_�̓ǂݍ���
		fread64( &this->Head, sizeof( DARC_HEAD ), this->fp ) ;
		
		// �h�c�̌���
		if( this->Head.Head != DXA_HEAD )
		{
			goto ERR ;
		}
		
		// �o�[�W��������
		if( this->Head.Version > DXA_VER || this->Head.Version < DXA_VER_MIN ) goto ERR ;

		// ���������s���Ă��Ȃ������擾����
		this->NoKey = ( Head.Flags & DXA_FLAG_NO_KEY ) != 0 ;
		
		// �w�b�_�̃T�C�Y���̃��������m�ۂ���
		this->HeadBuffer = ( u8 * )malloc( ( size_t )this->Head.HeadSize ) ;
		if( this->HeadBuffer == NULL ) goto ERR ;

		// �w�b�_�����k����Ă���ꍇ�͉𓀂���
		if( ( Head.Flags & DXA_FLAG_NO_HEAD_PRESS ) != 0 )
		{
			// ���k����Ă��Ȃ��ꍇ�͕��ʂɓǂݍ���
			_fseeki64( this->fp, this->Head.FileNameTableStartAddress, SEEK_SET ) ;
			KeyConvFileRead( HeadBuffer, this->Head.HeadSize, this->fp, this->NoKey ? NULL : this->Key, 0 ) ;
		}
		else
		{
			void *HuffHeadBuffer ;
			u64 HuffHeadSize ;
			void *LzHeadBuffer ;
			u64 LzHeadSize ;
			s64 FileSize ;

			// ���k���ꂽ�w�b�_�̗e�ʂ��擾����
			_fseeki64( this->fp, 0, SEEK_END ) ;
			FileSize = _ftelli64( this->fp ) ;
			_fseeki64( this->fp, this->Head.FileNameTableStartAddress, SEEK_SET ) ;
			HuffHeadSize = ( u32 )( FileSize - _ftelli64( this->fp ) ) ;

			// �n�t�}�����k���ꂽ�w�b�_��ǂݍ��ރ��������m�ۂ���
			HuffHeadBuffer = malloc( ( size_t )HuffHeadSize ) ;
			if( HuffHeadBuffer == NULL ) goto ERR ;

			// �n�t�}�����k���ꂽ�w�b�_���������ɓǂݍ���
			KeyConvFileRead( HuffHeadBuffer, HuffHeadSize, this->fp, NoKey ? NULL : Key, 0 ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̗e�ʂ��擾����
			LzHeadSize = Huffman_Decode( HuffHeadBuffer, NULL ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̃f�[�^���i�[���郁�����p��̊m��
			LzHeadBuffer = malloc( ( size_t )LzHeadSize ) ;
			if( LzHeadBuffer == NULL )
			{
				free( HuffHeadBuffer ) ;
				goto ERR ;
			}

			// �n�t�}�����k���ꂽ�w�b�_���𓀂���
			Huffman_Decode( HuffHeadBuffer, LzHeadBuffer ) ;

			// LZ���k���ꂽ�w�b�_���𓀂���
			Decode( LzHeadBuffer, this->HeadBuffer ) ;

			// �������̉��
			free( HuffHeadBuffer ) ;
			free( LzHeadBuffer ) ;
		}

		// �e�A�h���X���Z�b�g����
		this->NameP = this->HeadBuffer ;
		this->FileP = this->NameP + this->Head.FileTableStartAddress ;
		this->DirP = this->NameP + this->Head.DirectoryTableStartAddress ;
	}

	// �J�����g�f�B���N�g���̃Z�b�g
	this->CurrentDirectory = ( DARC_DIRECTORY * )this->DirP ;

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

// �A�[�J�C�u�t�@�C�����J���ŏ��ɂ��ׂă�������ɓǂݍ���ł��珈������( 0:����  -1:���s )
int DXArchive::OpenArchiveFileMem( const char *ArchivePath, const char *KeyString_ )
{
	FILE *fp ;
	u8 *datp ;
	void *ArchiveImage ;
	s64 ArchiveSize ;
	void *HuffHeadBuffer = NULL ;

	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( this->fp != NULL ) return -1 ;

	// ��������̕ۑ��ƌ��̍쐬
	{
		// �w�肪�����ꍇ�̓f�t�H���g�̌���������g�p����
		if( KeyString_ == NULL )
		{
			KeyString_ = DefaultKeyString ;
		}

		KeyStringBytes = CL_strlen( CHARCODEFORMAT_ASCII, KeyString_ ) ;
		if( KeyStringBytes > DXA_KEY_STRING_LENGTH )
		{
			KeyStringBytes = DXA_KEY_STRING_LENGTH ;
		}
		memcpy( KeyString, KeyString_, KeyStringBytes ) ;
		KeyString[ KeyStringBytes ] = '\0' ;

		// ���̍쐬
		KeyCreate( KeyString, KeyStringBytes, Key ) ;
	}

	// �������ɓǂݍ���
	{
		fp = fopen( ArchivePath, "rb" ) ;
		if( fp == NULL ) return -1 ;
		_fseeki64( fp, 0L, SEEK_END ) ;
		ArchiveSize = _ftelli64( fp ) ;
		_fseeki64( fp, 0L, SEEK_SET ) ;
		ArchiveImage = malloc( ( size_t )ArchiveSize ) ;
		if( ArchiveImage == NULL )
		{
			fclose( fp ) ;
			return -1 ;
		}
		fread64( ArchiveImage, ArchiveSize, fp ) ;
		fclose( fp ) ;
	}

	// �w�b�_�̕������R�s�[
	memcpy( &this->Head, ArchiveImage, sizeof( DARC_HEAD ) ) ;

	// �h�c���Ⴄ�ꍇ�̓G���[
	if( Head.Head != DXA_HEAD )
	{
		return -1 ;
	}

	// �|�C���^��ۑ�
	this->fp = (FILE *)ArchiveImage ;
	datp = (u8 *)ArchiveImage ;

	// �w�b�_����͂���
	{
		datp += sizeof( DARC_HEAD ) ;
		
		// �o�[�W��������
		if( this->Head.Version > DXA_VER || this->Head.Version < DXA_VER_MIN ) goto ERR ;

		// ���������s���Ă��Ȃ������擾����
		this->NoKey = ( Head.Flags & DXA_FLAG_NO_KEY ) != 0 ;

		// �w�b�_�̃T�C�Y���̃��������m�ۂ���
		this->HeadBuffer = ( u8 * )malloc( ( size_t )this->Head.HeadSize ) ;
		if( this->HeadBuffer == NULL ) goto ERR ;

		// �w�b�_�����k����Ă���ꍇ�͉𓀂���
		if( ( Head.Flags & DXA_FLAG_NO_HEAD_PRESS ) != 0 )
		{
			// ���k����Ă��Ȃ��ꍇ�͕��ʂɓǂݍ���
			memcpy( HeadBuffer, ( u8 * )this->fp + this->Head.FileNameTableStartAddress, this->Head.HeadSize ) ;
			if( this->NoKey == false ) KeyConv( HeadBuffer, this->Head.HeadSize, 0, this->Key ) ;
		}
		else
		{
			void *HuffHeadBuffer ;
			u64 HuffHeadSize ;
			void *LzHeadBuffer ;
			u64 LzHeadSize ;

			// �n�t�}�����k���ꂽ�w�b�_�̗e�ʂ��擾����
			HuffHeadSize = ( u32 )( ( u64 )ArchiveSize - this->Head.FileNameTableStartAddress ) ;

			// �n�t�}�����k���ꂽ�w�b�_��ǂݍ��ރ��������m�ۂ���
			HuffHeadBuffer = malloc( ( size_t )HuffHeadSize ) ;
			if( HuffHeadBuffer == NULL ) goto ERR ;

			// ���k���ꂽ�w�b�_���R�s�[�ƈÍ�������
			memcpy( HuffHeadBuffer, ( u8 * )this->fp + this->Head.FileNameTableStartAddress, ( size_t )HuffHeadSize ) ;
			if( this->NoKey == false ) KeyConv( HuffHeadBuffer, HuffHeadSize, 0, this->Key ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̗e�ʂ��擾����
			LzHeadSize = Huffman_Decode( HuffHeadBuffer, NULL ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̃f�[�^���i�[���郁�����p��̊m��
			LzHeadBuffer = malloc( ( size_t )LzHeadSize ) ;
			if( LzHeadBuffer == NULL )
			{
				free( HuffHeadBuffer ) ;
				goto ERR ;
			}

			// �n�t�}�����k���ꂽ�w�b�_���𓀂���
			Huffman_Decode( HuffHeadBuffer, LzHeadBuffer ) ;

			// LZ���k���ꂽ�w�b�_���𓀂���
			Decode( LzHeadBuffer, this->HeadBuffer ) ;

			// �������̉��
			free( HuffHeadBuffer ) ;
			free( LzHeadBuffer ) ;
		}

		// �e�A�h���X���Z�b�g����
		this->NameP = this->HeadBuffer ;
		this->FileP = this->NameP + this->Head.FileTableStartAddress ;
		this->DirP = this->NameP + this->Head.DirectoryTableStartAddress ;
	}

	// �J�����g�f�B���N�g���̃Z�b�g
	this->CurrentDirectory = ( DARC_DIRECTORY * )this->DirP ;

	// �������C���[�W����J���Ă���t���O�𗧂Ă�
	MemoryOpenFlag = true ;

	// �S�Ẵt�@�C���̈Í�������������
	if( this->NoKey == false )
	{
		char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
		DirectoryKeyConv( ( DARC_DIRECTORY * )this->DirP, KeyStringBuffer ) ;
	}
	
	// ���[�U�[�̃C���[�W����J�����킯�ł͂Ȃ��̂Ńt���O��|��
	UserMemoryImageFlag = false ;

	// �T�C�Y���ۑ����Ă���
	MemoryImageSize = ArchiveSize ;

	// �I��
	return 0 ;

ERR :
	
	// �I��
	return -1 ;
}

// ��������ɂ���A�[�J�C�u�C���[�W���J��( 0:����  -1:���s )
int	DXArchive::OpenArchiveMem( void *ArchiveImage, s64 ArchiveSize, const char *KeyString_ )
{
	u8 *datp ;

	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( this->fp != NULL ) return -1 ;

	// ��������̕ۑ��ƌ��̍쐬
	{
		// �w�肪�����ꍇ�̓f�t�H���g�̌���������g�p����
		if( KeyString_ == NULL )
		{
			KeyString_ = DefaultKeyString ;
		}

		KeyStringBytes = CL_strlen( CHARCODEFORMAT_ASCII, KeyString_ ) ;
		if( KeyStringBytes > DXA_KEY_STRING_LENGTH )
		{
			KeyStringBytes = DXA_KEY_STRING_LENGTH ;
		}
		memcpy( KeyString, KeyString_, KeyStringBytes ) ;
		KeyString[ KeyStringBytes ] = '\0' ;

		// ���̍쐬
		KeyCreate( KeyString, KeyStringBytes, Key ) ;
	}

	// �w�b�_�̕������R�s�[
	memcpy( &this->Head, ArchiveImage, sizeof( DARC_HEAD ) ) ;

	// �h�c���Ⴄ�ꍇ�̓G���[
	if( Head.Head != DXA_HEAD )
	{
		goto ERR ;
	}

	// �|�C���^��ۑ�
	this->fp = (FILE *)ArchiveImage ;
	datp = (u8 *)ArchiveImage ;

	// �w�b�_����͂���
	{
		datp += sizeof( DARC_HEAD ) ;

		// �o�[�W��������
		if( this->Head.Version > DXA_VER || this->Head.Version < DXA_VER_MIN ) goto ERR ;

		// ���������s���Ă��Ȃ������擾����
		this->NoKey = ( Head.Flags & DXA_FLAG_NO_KEY ) != 0 ;

		// �w�b�_�̃T�C�Y���̃��������m�ۂ���
		this->HeadBuffer = ( u8 * )malloc( ( size_t )this->Head.HeadSize ) ;
		if( this->HeadBuffer == NULL ) goto ERR ;

		// �w�b�_�����k����Ă���ꍇ�͉𓀂���
		if( ( Head.Flags & DXA_FLAG_NO_HEAD_PRESS ) != 0 )
		{
			// ���k����Ă��Ȃ��ꍇ�͕��ʂɓǂݍ���
			memcpy( HeadBuffer, ( u8 * )this->fp + this->Head.FileNameTableStartAddress, this->Head.HeadSize ) ;
			if( this->NoKey == false ) KeyConv( HeadBuffer, this->Head.HeadSize, 0, this->Key ) ;
		}
		else
		{
			void *HuffHeadBuffer ;
			u64 HuffHeadSize ;
			void *LzHeadBuffer ;
			u64 LzHeadSize ;

			// �n�t�}�����k���ꂽ�w�b�_�̗e�ʂ��擾����
			HuffHeadSize = ( u32 )( ( u64 )ArchiveSize - this->Head.FileNameTableStartAddress ) ;

			// �n�t�}�����k���ꂽ�w�b�_��ǂݍ��ރ��������m�ۂ���
			HuffHeadBuffer = malloc( ( size_t )HuffHeadSize ) ;
			if( HuffHeadBuffer == NULL ) goto ERR ;

			// �n�t�}�����k���ꂽ�w�b�_���R�s�[�ƈÍ�������
			memcpy( HuffHeadBuffer, ( u8 * )this->fp + this->Head.FileNameTableStartAddress, ( size_t )HuffHeadSize ) ;
			if( this->NoKey == false ) KeyConv( HuffHeadBuffer, HuffHeadSize, 0, this->Key ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̗e�ʂ��擾����
			LzHeadSize = Huffman_Decode( HuffHeadBuffer, NULL ) ;

			// �n�t�}�����k���ꂽ�w�b�_�̉𓀌�̃f�[�^���i�[���郁�����p��̊m��
			LzHeadBuffer = malloc( ( size_t )LzHeadSize ) ;
			if( LzHeadBuffer == NULL )
			{
				free( HuffHeadBuffer ) ;
				goto ERR ;
			}

			// �n�t�}�����k���ꂽ�w�b�_���𓀂���
			Huffman_Decode( HuffHeadBuffer, LzHeadBuffer ) ;

			// LZ���k���ꂽ�w�b�_���𓀂���
			Decode( LzHeadBuffer, this->HeadBuffer ) ;

			// �������̉��
			free( HuffHeadBuffer ) ;
			free( LzHeadBuffer ) ;
		}

		// �e�A�h���X���Z�b�g����
		this->NameP = this->HeadBuffer ;
		this->FileP = this->NameP + this->Head.FileTableStartAddress ;
		this->DirP = this->NameP + this->Head.DirectoryTableStartAddress ;
	}

	// �J�����g�f�B���N�g���̃Z�b�g
	this->CurrentDirectory = ( DARC_DIRECTORY * )this->DirP ;

	// �������C���[�W����J���Ă���t���O�𗧂Ă�
	MemoryOpenFlag = true ;

	// �S�Ẵt�@�C���̈Í�������������
	if( this->NoKey == false )
	{
		char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
		DirectoryKeyConv( ( DARC_DIRECTORY * )this->DirP, KeyStringBuffer ) ;
	}

	// ���[�U�[�̃C���[�W����J�����t���O�𗧂Ă�
	UserMemoryImageFlag = true ;

	// �T�C�Y���ۑ����Ă���
	MemoryImageSize = ArchiveSize ;

	// �I��
	return 0 ;

ERR :
	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�������
int DXArchive::CloseArchiveFile( void )
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
			if( this->NoKey == false )
			{
				char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
				DirectoryKeyConv( ( DARC_DIRECTORY * )this->DirP, KeyStringBuffer ) ;
				KeyConv( this->HeadBuffer, this->Head.HeadSize, 0, this->Key ) ;
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
	}

	// �w�b�_�o�b�t�@�����
	free( this->HeadBuffer ) ;

	// �|�C���^������
	this->fp = NULL ;
	this->HeadBuffer = NULL ;
	this->NameP = this->DirP = this->FileP = NULL ;
	this->CurrentDirectory = NULL ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
int	DXArchive::ChangeCurrentDirectoryFast( SEARCHDATA *SearchData )
{
	DARC_FILEHEAD *FileH ;
	int i, j, k, Num ;
	u8 *NameData, *PathData ;
	u16 PackNum, Parity ;

	PackNum  = SearchData->PackNum ;
	Parity   = SearchData->Parity ;
	PathData = SearchData->FileName ;

	// �J�����g�f�B���N�g�����瓯���̃f�B���N�g����T��
	FileH = ( DARC_FILEHEAD * )( this->FileP + this->CurrentDirectory->FileHeadAddress ) ;
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
	this->CurrentDirectory = ( DARC_DIRECTORY * )( this->DirP + FileH->DataAddress ) ;

	// ����I��
	return 0 ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
int DXArchive::ChangeCurrentDir( const char *DirPath )
{
	return ChangeCurrentDirectoryBase( DirPath, true ) ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
int	DXArchive::ChangeCurrentDirectoryBase( const char *DirectoryPath, bool ErrorIsDirectoryReset, SEARCHDATA *LastSearchData )
{
	DARC_DIRECTORY *OldDir ;
	SEARCHDATA SearchData ;

	// �����ɗ��܂�p�X�������疳��
	if( strcmp( DirectoryPath, "." ) == 0 ) return 0 ;

	// �w\�x�����̏ꍇ�̓��[�g�f�B���N�g���ɖ߂�
	if( strcmp( DirectoryPath, "\\" ) == 0 )
	{
		this->CurrentDirectory = ( DARC_DIRECTORY * )this->DirP ;
		return 0 ;
	}

	// ���Ɉ������p�X�������珈���𕪊�
	if( strcmp( DirectoryPath, ".." ) == 0 )
	{
		// ���[�g�f�B���N�g���ɋ�����G���[
		if( this->CurrentDirectory->ParentDirectoryAddress == 0xffffffffffffffff ) return -1 ;
		
		// �e�f�B���N�g�����������炻����Ɉڂ�
		this->CurrentDirectory = ( DARC_DIRECTORY * )( this->DirP + this->CurrentDirectory->ParentDirectoryAddress ) ;
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
		FileH = ( DARC_FILEHEAD * )( this->FileP + this->CurrentDirectory->FileHeadAddress ) ;
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
int DXArchive::GetCurrentDir( char *DirPathBuffer, int BufferLength )
{
	char DirPath[MAX_PATH] ;
	DARC_DIRECTORY *Dir[200], *DirTempP ;
	int Depth, i ;

	// ���[�g�f�B���N�g���ɒ����܂Ō�������
	Depth = 0 ;
	DirTempP = this->CurrentDirectory ;
	while( DirTempP->DirectoryAddress != 0xffffffffffffffff && DirTempP->DirectoryAddress != 0 )
	{
		Dir[Depth] = DirTempP ;
		DirTempP = ( DARC_DIRECTORY * )( this->DirP + DirTempP->ParentDirectoryAddress ) ;
		Depth ++ ;
	}
	
	// �p�X����A������
	DirPath[0] = '\0' ;
	for( i = Depth - 1 ; i >= 0 ; i -- )
	{
		strcat( DirPath, "\\" ) ;
		strcat( DirPath, ( char * )( this->NameP + ( ( DARC_FILEHEAD * )( this->FileP + Dir[i]->DirectoryAddress ) )->NameAddress ) ) ;
	}

	// �o�b�t�@�̒������O���A����������Ȃ��Ƃ��̓f�B���N�g�����̒�����Ԃ�
	if( BufferLength == 0 || BufferLength < (s32)strlen( DirPath ) )
	{
		return ( int )( strlen( DirPath ) + 1 ) ;
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
s64 DXArchive::LoadFileToMem( const char *FilePath, void *Buffer, u64 BufferLength )
{
	DARC_FILEHEAD *FileH ;
	char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
	size_t KeyStringBufferBytes ;
	unsigned char lKey[ DXA_KEY_BYTES ] ;
	DARC_DIRECTORY *Directory ;
	void *HuffDataBuffer = NULL ;

	// �w��̃t�@�C���̏��𓾂�
	FileH = this->GetFileInfo( FilePath, &Directory ) ;
	if( FileH == NULL ) return -1 ;

	// �t�@�C���T�C�Y������Ă��邩���ׂ�A����Ă��Ȃ����A�o�b�t�@�A���̓T�C�Y���O��������T�C�Y��Ԃ�
	if( BufferLength < FileH->DataSize || BufferLength == 0 || Buffer == NULL )
	{
		return ( s64 )FileH->DataSize ;
	}

	// ����Ă���ꍇ�̓o�b�t�@�[�ɓǂݍ���

	// �t�@�C�������k����Ă��邩�ǂ����ŏ����𕪊�
	if( FileH->PressDataSize != 0xffffffffffffffff )
	{
		// ���k����Ă���ꍇ

		// �n�t�}�����k����Ă��邩�ǂ����ŏ����𕪊�
		if( FileH->HuffPressDataSize != 0xffffffffffffffff )
		{
			// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
			if( MemoryOpenFlag == true )
			{
				// �n�t�}�����k���𓀂����f�[�^���i�[���郁�����̈�̊m��
				HuffDataBuffer = malloc( ( size_t )FileH->PressDataSize ) ;
				if( HuffDataBuffer == NULL ) return -1 ;

				// �n�t�}�����k�f�[�^����
				Huffman_Decode( (u8 *)this->fp + this->Head.DataStartAddress + FileH->DataAddress, HuffDataBuffer ) ;

				// ��������̈��k�f�[�^���𓀂���
				Decode( HuffDataBuffer, Buffer ) ;

				// �����������
				free( HuffDataBuffer ) ;
			}
			else
			{
				void *temp ;

				// ���k�f�[�^���������ɓǂݍ���ł���𓀂���

				// ���k�f�[�^�����܂郁�����̈�̊m��
				temp = malloc( ( size_t )( FileH->PressDataSize + FileH->HuffPressDataSize ) ) ;

				// ���k�f�[�^�̓ǂݍ���
				_fseeki64( this->fp, this->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;

				// �t�@�C���ʂ̌����쐬
				if( this->NoKey == false )
				{
					KeyStringBufferBytes = CreateKeyFileString( ( int )this->Head.CharCodeFormat, this->KeyString, this->KeyStringBytes, Directory, FileH, this->FileP, this->DirP, this->NameP, ( BYTE * )KeyStringBuffer ) ;
					KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
				}

				// �Í��������ǂݍ���
				KeyConvFileRead( temp, FileH->HuffPressDataSize, this->fp, this->NoKey ? NULL : lKey, FileH->DataSize ) ;

				// �n�t�}�����k�f�[�^����
				Huffman_Decode( temp, ( u8 * )temp + FileH->HuffPressDataSize ) ;
			
				// ��
				Decode( ( u8 * )temp + FileH->HuffPressDataSize, Buffer ) ;
			
				// �������̉��
				free( temp ) ;
			}
		}
		else
		{
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
				temp = malloc( ( size_t )FileH->PressDataSize ) ;

				// ���k�f�[�^�̓ǂݍ���
				_fseeki64( this->fp, this->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;

				// �t�@�C���ʂ̌����쐬
				if( this->NoKey == false )
				{
					KeyStringBufferBytes = CreateKeyFileString( ( int )this->Head.CharCodeFormat, this->KeyString, this->KeyStringBytes, Directory, FileH, this->FileP, this->DirP, this->NameP, ( BYTE * )KeyStringBuffer ) ;
					KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
				}

				KeyConvFileRead( temp, FileH->PressDataSize, this->fp, this->NoKey ? NULL : lKey, FileH->DataSize ) ;
			
				// ��
				Decode( temp, Buffer ) ;
			
				// �������̉��
				free( temp ) ;
			}
		}
	}
	else
	{
		// ���k����Ă��Ȃ��ꍇ

		// �n�t�}�����k����Ă��邩�ǂ����ŏ����𕪊�
		if( FileH->HuffPressDataSize != 0xffffffffffffffff )
		{
			// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
			if( MemoryOpenFlag == true )
			{
				// �n�t�}�����k���𓀂����f�[�^���i�[���郁�����̈�̊m��
				HuffDataBuffer = malloc( ( size_t )FileH->PressDataSize ) ;
				if( HuffDataBuffer == NULL ) return -1 ;

				// �n�t�}�����k�f�[�^����
				Huffman_Decode( (u8 *)this->fp + this->Head.DataStartAddress + FileH->DataAddress, HuffDataBuffer ) ;

				// �R�s�[
				memcpy( Buffer, HuffDataBuffer, ( size_t )FileH->DataSize ) ;

				// �����������
				free( HuffDataBuffer ) ;
			}
			else
			{
				void *temp ;

				// ���k�f�[�^���������ɓǂݍ���ł���𓀂���

				// �n�t�}�����k�f�[�^�����܂郁�����̈�̊m��
				temp = malloc( ( size_t )FileH->HuffPressDataSize ) ;

				// ���k�f�[�^�̓ǂݍ���
				_fseeki64( this->fp, this->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;

				// �t�@�C���ʂ̌����쐬
				if( this->NoKey == false )
				{
					KeyStringBufferBytes = CreateKeyFileString( ( int )this->Head.CharCodeFormat, this->KeyString, this->KeyStringBytes, Directory, FileH, this->FileP, this->DirP, this->NameP, ( BYTE * )KeyStringBuffer ) ;
					KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
				}

				// �Í��������ǂݍ���
				KeyConvFileRead( temp, FileH->HuffPressDataSize, this->fp, this->NoKey ? NULL : lKey, FileH->DataSize ) ;

				// �n�t�}�����k�f�[�^����
				Huffman_Decode( temp, Buffer ) ;
			
				// �������̉��
				free( temp ) ;
			}
		}
		else
		{
			// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
			if( MemoryOpenFlag == true )
			{
				// �R�s�[
				memcpy( Buffer, (u8 *)this->fp + this->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
			}
			else
			{
				// �t�@�C���|�C���^���ړ�
				_fseeki64( this->fp, this->Head.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;

				// �ǂݍ���

				// �t�@�C���ʂ̌����쐬
				if( this->NoKey == false )
				{
					KeyStringBufferBytes = CreateKeyFileString( ( int )this->Head.CharCodeFormat, this->KeyString, this->KeyStringBytes, Directory, FileH, this->FileP, this->DirP, this->NameP, ( BYTE * )KeyStringBuffer ) ;
					KeyCreate( KeyStringBuffer, KeyStringBufferBytes, lKey ) ;
				}

				KeyConvFileRead( Buffer, FileH->DataSize, this->fp, this->NoKey ? NULL : lKey, FileH->DataSize ) ;
			}
		}
	}
	
	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����T�C�Y���擾����
s64 DXArchive::GetFileSize( const char *FilePath )
{
	// �t�@�C���T�C�Y��Ԃ�
	return this->LoadFileToMem( FilePath, NULL, 0 ) ;
}

// �A�[�J�C�u�t�@�C�����������ɓǂݍ��񂾏ꍇ�̃t�@�C���C���[�W���i�[����Ă���擪�A�h���X���擾����( ����������J���Ă���ꍇ�̂ݗL�� )
void *DXArchive::GetFileImage( void )
{
	// �������C���[�W����J���Ă��Ȃ�������G���[
	if( MemoryOpenFlag == false ) return NULL ;

	// �擪�A�h���X��Ԃ�
	return this->fp ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C���̃t�@�C�����̈ʒu�ƃt�@�C���̑傫���𓾂�( -1:�G���[ )
int DXArchive::GetFileInfo( const char *FilePath, u64 *PositionP, u64 *SizeP )
{
	DARC_FILEHEAD *FileH ;

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
void *DXArchive::LoadFileToCache( const char *FilePath )
{
	s64 FileSize ;

	// �t�@�C���T�C�Y���擾����
	FileSize = this->GetFileSize( FilePath ) ;

	// �t�@�C��������������G���[
	if( FileSize < 0 ) return NULL ;

	// �m�ۂ��Ă���L���b�V���o�b�t�@�̃T�C�Y�����傫���ꍇ�̓o�b�t�@���Ċm�ۂ���
	if( FileSize > ( s64 )this->CacheBufferSize )
	{
		// �L���b�V���o�b�t�@�̃N���A
		this->ClearCache() ;

		// �L���b�V���o�b�t�@�̍Ċm��
		this->CacheBuffer = malloc( ( size_t )FileSize ) ;

		// �m�ۂɎ��s�����ꍇ�� NULL ��Ԃ�
		if( this->CacheBuffer == NULL ) return NULL ;

		// �L���b�V���o�b�t�@�̃T�C�Y��ۑ�
		this->CacheBufferSize = FileSize ;
	}

	// �t�@�C�����������ɓǂݍ���
	this->LoadFileToMem( FilePath, this->CacheBuffer, FileSize ) ;

	// �L���b�V���o�b�t�@�̃A�h���X��Ԃ�
	return this->CacheBuffer ;
}

// �L���b�V���o�b�t�@���J������
int DXArchive::ClearCache( void )
{
	// ���������m�ۂ���Ă�����������
	if( this->CacheBuffer != NULL )
	{
		free( this->CacheBuffer ) ;
		this->CacheBuffer = NULL ;
		this->CacheBufferSize = 0 ;
	}

	// �I��
	return 0 ;
}

	
// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����J���A�t�@�C���A�N�Z�X�p�I�u�W�F�N�g���쐬����
DXArchiveFile *DXArchive::OpenFile( const char *FilePath )
{
	DARC_FILEHEAD *FileH ;
	DARC_DIRECTORY *Directory ;
	DXArchiveFile *CDArc = NULL ;

	// ����������J���Ă���ꍇ�͖���
	if( MemoryOpenFlag == true ) return NULL ;

	// �w��̃t�@�C���̏��𓾂�
	FileH = this->GetFileInfo( FilePath, &Directory ) ;
	if( FileH == NULL ) return NULL ;

	// �V���� DXArchiveFile �N���X���쐬����
	CDArc = new DXArchiveFile( FileH, Directory, this ) ;
	
	// DXArchiveFile �N���X�̃|�C���^��Ԃ�
	return CDArc ;
}













// �R���X�g���N�^
DXArchiveFile::DXArchiveFile( DARC_FILEHEAD *FileHead, DARC_DIRECTORY *Directory, DXArchive *Archive )
{
	void *temp ;

	this->FileData  = FileHead ;
	this->Archive   = Archive ;
	this->EOFFlag   = FALSE ;
	this->FilePoint = 0 ;
	this->DataBuffer = NULL ;

	// �����쐬����
	if( this->Archive->GetNoKey() == false )
	{
		char KeyStringBuffer[ DXA_KEY_STRING_MAXLENGTH ] ;
		size_t KeyStringBufferBytes ;

		KeyStringBufferBytes = DXArchive::CreateKeyFileString(
			( int )this->Archive->GetHeader()->CharCodeFormat,
			this->Archive->GetKeyString(),
			this->Archive->GetKeyStringBytes(),
			Directory,
			FileHead,
			this->Archive->GetFileHeadTable(),
			this->Archive->GetDirectoryTable(),
			this->Archive->GetNameTable(),
			( BYTE * )KeyStringBuffer
		) ;
		DXArchive::KeyCreate( KeyStringBuffer, KeyStringBufferBytes, Key ) ;
	}

	// �t�@�C�������k����Ă���ꍇ�͉𓀃f�[�^�����܂郁�����̈�̊m��
	if( FileHead->PressDataSize     != 0xffffffffffffffff ||
		FileHead->HuffPressDataSize != 0xffffffffffffffff )
	{
		// �𓀃f�[�^�����܂郁�����̈�̊m��
		this->DataBuffer = malloc( ( size_t )FileHead->DataSize ) ;
	}
	
	// �t�@�C�������k����Ă���ꍇ�͂����œǂݍ���ŉ𓀂��Ă��܂�
	if( FileHead->PressDataSize != 0xffffffffffffffff )
	{
		// �n�t�}�����k������Ă��邩�ǂ����ŏ����𕪊�
		if( FileHead->HuffPressDataSize != 0xffffffffffffffff )
		{
			// ���k�f�[�^�����܂郁�����̈�̊m��
			temp = malloc( ( size_t )( FileHead->PressDataSize + FileHead->HuffPressDataSize ) ) ;

			// ���k�f�[�^�̓ǂݍ���
			_fseeki64( this->Archive->GetFilePointer(), this->Archive->GetHeader()->DataStartAddress + FileHead->DataAddress, SEEK_SET ) ;

			// �������ǂݍ���
			DXArchive::KeyConvFileRead( temp, FileHead->HuffPressDataSize, this->Archive->GetFilePointer(), this->Archive->GetNoKey() ? NULL : Key, FileHead->DataSize ) ;

			// �n�t�}�����k�f�[�^����
			Huffman_Decode( temp, ( u8 * )temp + FileHead->HuffPressDataSize ) ;
		
			// ��
			DXArchive::Decode( ( u8 * )temp + FileHead->HuffPressDataSize, this->DataBuffer ) ;

			// �������̉��
			free( temp ) ;
		}
		else
		{
			// ���k�f�[�^�����܂郁�����̈�̊m��
			temp = malloc( ( size_t )FileHead->PressDataSize ) ;

			// ���k�f�[�^�̓ǂݍ���
			_fseeki64( this->Archive->GetFilePointer(), this->Archive->GetHeader()->DataStartAddress + FileHead->DataAddress, SEEK_SET ) ;
			DXArchive::KeyConvFileRead( temp, FileHead->PressDataSize, this->Archive->GetFilePointer(), this->Archive->GetNoKey() ? NULL : Key, FileHead->DataSize ) ;
		
			// ��
			DXArchive::Decode( temp, this->DataBuffer ) ;

			// �������̉��
			free( temp ) ;
		}
	}
	else
	// �n�t�}�����k��������Ă��邩�ǂ����ŏ����𕪊�
	if( FileHead->HuffPressDataSize != 0xffffffffffffffff )
	{
		// ���k�f�[�^�����܂郁�����̈�̊m��
		temp = malloc( ( size_t )FileHead->HuffPressDataSize ) ;

		// ���k�f�[�^�̓ǂݍ���
		_fseeki64( this->Archive->GetFilePointer(), this->Archive->GetHeader()->DataStartAddress + FileHead->DataAddress, SEEK_SET ) ;

		// �Í��������ǂݍ���
		DXArchive::KeyConvFileRead( temp, FileHead->HuffPressDataSize, this->Archive->GetFilePointer(), this->Archive->GetNoKey() ? NULL : Key, FileHead->DataSize ) ;

		// �n�t�}�����k�f�[�^����
		Huffman_Decode( temp, this->DataBuffer ) ;

		// �������̉��
		free( temp ) ;
	}
}

// �f�X�g���N�^
DXArchiveFile::~DXArchiveFile()
{
	// �������̉��
	if( this->DataBuffer != NULL )
	{
		free( this->DataBuffer ) ;
		this->DataBuffer = NULL ;
	}
}

// �t�@�C���̓��e��ǂݍ���
s64 DXArchiveFile::Read( void *Buffer, s64 ReadLength )
{
	s64 ReadSize ;

	// EOF �t���O�������Ă�����O��Ԃ�
	if( this->EOFFlag == TRUE ) return 0 ;
	
	// �A�[�J�C�u�t�@�C���|�C���^�ƁA���z�t�@�C���|�C���^����v���Ă��邩���ׂ�
	// ��v���Ă��Ȃ�������A�[�J�C�u�t�@�C���|�C���^���ړ�����
	if( this->DataBuffer == NULL && _ftelli64( this->Archive->GetFilePointer() ) != (s32)( this->FileData->DataAddress + this->Archive->GetHeader()->DataStartAddress + this->FilePoint ) )
	{
		_fseeki64( this->Archive->GetFilePointer(), this->FileData->DataAddress + this->Archive->GetHeader()->DataStartAddress + this->FilePoint, SEEK_SET ) ;
	}
	
	// EOF ���o
	if( this->FileData->DataSize == this->FilePoint )
	{
		this->EOFFlag = TRUE ;
		return 0 ;
	}
	
	// �f�[�^��ǂݍ��ޗʂ�ݒ肷��
	ReadSize = ReadLength < (s64)( this->FileData->DataSize - this->FilePoint ) ? ReadLength : this->FileData->DataSize - this->FilePoint ;
	
	// �f�[�^��ǂݍ���
	if( this->DataBuffer == NULL )
	{
		DXArchive::KeyConvFileRead( Buffer, ReadSize, this->Archive->GetFilePointer(), this->Archive->GetNoKey() ? NULL : Key, this->FileData->DataSize + this->FilePoint ) ;
	}
	else
	{
		memcpy( Buffer, (u8 *)this->DataBuffer + this->FilePoint, ( size_t )ReadSize ) ;
	}
	
	// EOF �t���O��|��
	this->EOFFlag = FALSE ;

	// �ǂݍ��񂾕������t�@�C���|�C���^���ړ�����
	this->FilePoint += ReadSize ;
	
	// �ǂݍ��񂾗e�ʂ�Ԃ�
	return ReadSize ;
}
	
// �t�@�C���|�C���^��ύX����
s64 DXArchiveFile::Seek( s64 SeekPoint, s64 SeekMode )
{
	// �V�[�N�^�C�v�ɂ���ď����𕪊�
	switch( SeekMode )
	{
	case SEEK_SET : break ;		
	case SEEK_CUR : SeekPoint += this->FilePoint ; break ;
	case SEEK_END :	SeekPoint = this->FileData->DataSize + SeekPoint ; break ;
	}
	
	// �␳
	if( SeekPoint > (s64)this->FileData->DataSize ) SeekPoint = this->FileData->DataSize ;
	if( SeekPoint < 0 ) SeekPoint = 0 ;
	
	// �Z�b�g
	this->FilePoint = SeekPoint ;
	
	// EOF�t���O��|��
	this->EOFFlag = FALSE ;
	
	// �I��
	return 0 ;
}

// ���݂̃t�@�C���|�C���^�𓾂�
s64 DXArchiveFile::Tell( void )
{
	return this->FilePoint ;
}

// �t�@�C���̏I�[�ɗ��Ă��邩�A�̃t���O�𓾂�
s64 DXArchiveFile::Eof( void )
{
	return this->EOFFlag ;
}

// �t�@�C���̃T�C�Y���擾����
s64 DXArchiveFile::Size( void )
{
	return this->FileData->DataSize ;
}



