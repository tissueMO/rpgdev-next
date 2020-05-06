// ============================================================================
//
//		�t�@�C���֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

// include --------------------------------------
#include <string.h>
#include <stdio.h>
#include <mbstring.h>
#include <windows.h>
#include "FileLib.h"

// define ---------------------------------------

#define PATH_LENGTH		(2048)			// �p�X�o�b�t�@�̒���
#define TEXTCHECKSIZE	(0x2000)		// �e�L�X�g�f�[�^�����ׂ�T�C�Y

// data type ------------------------------------

// data -----------------------------------------

unsigned char BinToChar128Table[ 128 ] =
{
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0xA1, 0xA2,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
	0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2,
	0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2,
} ;

unsigned char Char128ToBinTable[ 256 ] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
	0x5f, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
	0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
	0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
	0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x4e, 0x4f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
	0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
	0x7d, 0x7e, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

unsigned char BinToBase64Table[ 64 ] =
{
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,	0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,	0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F
} ;

unsigned char Base64ToBinTable[ 256 ] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

// function proto type --------------------------

// �t�@�C���I�u�W�F�N�g�񋓗p�֐�( -1:�G���[  0�ȏ�:�t�@�C���̐� )
// flist �� NULL �ł��ǂ�
static int __EnumObject( TCHAR *Path, TCHAR *CurrentPath, FILE_INFOLIST *FileList,
							int OmitDirectory, int SubDirectory,
							TCHAR **OmitName, TCHAR **OmitExName, TCHAR **ValidExName, int *TotalFileNumCounter, int TotalFileNum,
						void ( *EnumFileCallback )( int Phase, int NowFileNum, int TotalFileNum, const TCHAR *FileName, const TCHAR *RelDirPath, const TCHAR *AbsDirPath ) ) ;

// function code --------------------------------

// �t�@�C���I�u�W�F�N�g�񋓗p�֐�( -1:�G���[  0�ȏ�:�t�@�C���̐� )
// flist �� NULL �ł��ǂ�
static int __EnumObject( TCHAR *Path, TCHAR *CurrentPath, FILE_INFOLIST *FileList,
							int OmitDirectory, int SubDirectory,
							TCHAR **OmitName, TCHAR **OmitExName, TCHAR **ValidExName, int *TotalFileNumCounter, int TotalFileNum,
						void ( *EnumFileCallback )( int Phase, int NowFileNum, int TotalFileNum, const TCHAR *FileName, const TCHAR *RelDirPath, const TCHAR *AbsDirPath ) )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle = INVALID_HANDLE_VALUE ;
	int FileNum, IsDirectory ;
	TCHAR RelDir[PATH_LENGTH] ;
	TCHAR *AbsDir ;
	int RelDirLen, AbsDirLen, StartNum ;

	// �f�B���N�g���p�X�̍쐬
	AbsDir = Path ;
	AbsDirLen = ( int )_tcslen( AbsDir ) ;
	_tcscpy( RelDir, AbsDir + _tcslen( CurrentPath ) ) ;
	RelDirLen = ( int )_tcslen( RelDir ) ;

	// �t�@�C���̗񋓊J�n
	{
		TCHAR temp[PATH_LENGTH] ;

		_tcscpy( temp, AbsDir ) ;
		_tcscat( temp, TEXT( "*" ) ) ;
		FindHandle = FindFirstFile( temp, &FindData ) ;
		if( FindHandle == INVALID_HANDLE_VALUE )
			return -1 ;
	}

	// �t�@�C���̏���񋓂���
	FileNum = 0 ;
	if( FileList != NULL ) StartNum = FileList->Num ;
	do
	{
		// ��̃t�H���_�ɖ߂����肷�邽�߂̃p�X�͖�������
		if( _tcscmp( FindData.cFileName, TEXT( "." ) ) == 0 || _tcscmp( FindData.cFileName, TEXT( ".." ) ) == 0 ) continue ;

		// �f�B���N�g�����ǂ����𓾂�
		IsDirectory = ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0 ;

		// �L���g���q�w�肪����A�L���g���q�ł͂Ȃ��ꍇ�͖�������
		if( ValidExName != NULL && IsDirectory == 0 )
		{
			int i ;
			const TCHAR *name ;

			name = _tcschr( FindData.cFileName, TEXT( '.' ) ) ;
			if( name == NULL ) name = TEXT( "" ) ;
			else name ++ ;

			for( i = 0 ; ValidExName[i] != NULL && _tcsicmp( name, ValidExName[i] ) != 0 ; i ++ ){}
			if( ValidExName[i] == NULL ) continue ;
		}

		// ���O�w�肪���镶����͖�������
		if( OmitName != NULL )
		{
			int i ;

			for( i = 0 ; OmitName[i] != NULL && _tcscmp( FindData.cFileName, OmitName[i] ) != 0 ; i ++ ){}
			if( OmitName[i] != NULL ) continue ;
		}

		// ���O�w�肪����g���q�����t�@�C���͖�������
		if( OmitExName != NULL && IsDirectory == 0 )
		{
			int i ;
			const TCHAR *name ;

			name = _tcschr( FindData.cFileName, TEXT( '.' ) ) ;
			if( name == NULL ) name = TEXT( "" ) ;
			else name ++ ;

			for( i = 0 ; OmitExName[i] != NULL && _tcscmp( name, OmitExName[i] ) != 0 ; i ++ ){}
			if( OmitExName[i] != NULL ) continue ;
		}

		// �f�B���N�g���̏ꍇ�ŃT�u�f�B���N�g�������ׂ�w�肪����ꍇ�̓T�u�f�B���N�g�������񋓂���
		if( IsDirectory == 1 && SubDirectory == 1 )
		{
			int res ;
			TCHAR tempAbs[PATH_LENGTH], tempRel[PATH_LENGTH] ;

			// ��΃p�X�̍쐬
			_tcscpy( tempAbs, AbsDir ) ;
			_tcscat( tempAbs, FindData.cFileName ) ;
			_tcscat( tempAbs, TEXT( "\\" ) ) ;

			// ���΃p�X�̍쐬
			_tcscpy( tempRel, RelDir ) ;
			_tcscat( tempRel, FindData.cFileName ) ;
			_tcscat( tempRel, TEXT( "\\" ) ) ;
			
			// ��
			res = __EnumObject( tempAbs, CurrentPath, FileList, OmitDirectory, SubDirectory, OmitName, OmitExName, ValidExName, TotalFileNumCounter, TotalFileNum, EnumFileCallback ) ;
			if( res < 0 )
				goto ERR ;
			FileNum += res ;
		}

		if( EnumFileCallback != 0 )
		{
			EnumFileCallback( FileList == NULL ? 0 : 1, *TotalFileNumCounter, TotalFileNum, FindData.cFileName, RelDir, AbsDir ) ;
		}

		// �f�[�^���i�[���邱�Ƃ��o����ꍇ�̓f�[�^���i�[����
		if( FileList != NULL &&
			( IsDirectory == 0 || ( IsDirectory == 1 && OmitDirectory == 0 ) ) )
		{
			FILE_INFO *info ;
			int FileNameLen ;

			info = &FileList->List[FileList->Num] ;
			memset( info, 0, sizeof( FILE_INFO ) ) ;

			// ������ۑ�
			info->Date.Create		= ( ( ( LONGLONG )FindData.ftCreationTime	.dwHighDateTime ) << 32 ) + FindData.ftCreationTime		.dwLowDateTime ;
			info->Date.LastAccess	= ( ( ( LONGLONG )FindData.ftLastAccessTime	.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime	.dwLowDateTime ;
			info->Date.LastWrite	= ( ( ( LONGLONG )FindData.ftLastWriteTime	.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime	.dwLowDateTime ;

			info->Size			= ( u64 )FindData.nFileSizeLow | ( ( ( u64 )FindData.nFileSizeHigh ) << 32 ) ;		// �T�C�Y��ۑ�
			info->Attributes	= FindData.dwFileAttributes ;	// ������ۑ�
			info->IsDirectory	= (u8)IsDirectory ;

			// �p�X�n�̕ۑ�
			{
				// �p�X�n��ۑ����郁�����̈�̊m��
				FileNameLen = ( int )_tcslen( FindData.cFileName ) ;
				info->FileName = ( TCHAR * )malloc( sizeof( TCHAR ) * ( ( FileNameLen + 1 ) + ( AbsDirLen + 1 ) + ( RelDirLen + 1 ) ) ) ;
				if( info->FileName == NULL )
					goto ERR ;
				info->RelDirectoryPath = info->FileName + FileNameLen + 1 ;
				info->AbsDirectoryPath = info->RelDirectoryPath + RelDirLen + 1 ;

				// �R�s�[
				_tcscpy( info->FileName, FindData.cFileName ) ;
				_tcscpy( info->RelDirectoryPath, RelDir ) ;
				_tcscpy( info->AbsDirectoryPath, AbsDir ) ;

				// �n�b�V���l���v�Z
				info->FileNameHash = FileLib_HashCRC32( info->FileName, FileNameLen * sizeof( TCHAR ) ) ;
			}

			// �t�@�C���̐��𑝂₷
			FileList->Num ++ ;
		}

		// �t�@�C���̐��𑝂₷
		FileNum ++ ;
		*TotalFileNumCounter += 1 ;
	}
	while( FindNextFile( FindHandle, &FindData ) != 0 ) ;

	// �񋓏I��
	FindClose( FindHandle ) ;
	FindHandle = INVALID_HANDLE_VALUE ;

	// �I��
	return FileNum ;

	// �G���[����
ERR :
	if( FindHandle != INVALID_HANDLE_VALUE ) FindClose( FindHandle ) ;

	// ���Ɋm�ۂ��Ă��܂����������̉������
	if( FileList != NULL )
	{
		int i ;

		for( i = StartNum ; i < FileList->Num ; i ++ )
			if( FileList->List[i].FileName != NULL ) free( FileList->List[i].FileName ) ;
	}

	// �G���[�I��
	return -1 ;
}

// �f�[�^���ȈՈÍ�������֐�
extern void EasyEncode( void *Data, unsigned int Size )
{
	if( Size == 0 ) return ;

#ifdef _WIN64
	unsigned int i ;
	BYTE *p = ( BYTE * )Data ;
	for( i = 0 ; i < Size ; i ++ )
	{
		p[ i ] = ( ~p[ i ] << 4 ) | ( ~p[ i ] >> 4 ) ;
	}
#else
	unsigned int tempSize, tempData ;

	tempData = ( unsigned int )Data ;
	tempSize = ( unsigned int )Size ;
	__asm
	{
		// ����
		mov edi, tempData
		mov ecx, tempSize

		// �l�𔽓]�A��]4�r�b�g���ď����߂�
LOOP1:
		mov al, [edi]
		not al
		rol al, 4
		mov [edi], al
		inc edi

		// ���[�v����
		dec ecx
		jnz LOOP1
	} ;
#endif
}

// �f�[�^���ȈՈÍ������ăt�@�C���ɏ����o���֐�
extern void EasyEncodeFileWrite( void *Data, int Size, FILE *fp )
{
	EasyEncode( Data, Size ) ;
	fwrite( Data, Size, 1, fp ) ;
	EasyEncode( Data, Size ) ;
}

// �f�[�^���ȈՈÍ������ăt�@�C������ǂݍ��ފ֐�
extern void EasyEncodeFileRead( void *Data, int Size, FILE *fp )
{
	fread( Data, Size, 1, fp ) ;
	EasyEncode( Data, Size ) ;
}

// �o�C�i���f�[�^�𔼊p������ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern unsigned int BinToChar128( void *Src, unsigned int SrcSize, void *Dest )
{
	unsigned int DestSize ;

	DestSize  = SrcSize + ( SrcSize + 6 ) / 7 + 5 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned char *DestP ;
		unsigned char *SrcP ;
		unsigned int i ;

		DestP = ( unsigned char * )Dest ;
		SrcP  = ( unsigned char * )&SrcSize ;

		DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
		DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
		DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
		DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
		DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 )                      ] ;

		DestP += 5 ;

		PackNum = SrcSize / 7 ;
		ModNum  = SrcSize - PackNum * 7 ;
		SrcP  = ( unsigned char * )Src ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
			DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
			DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
			DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
			DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) | ( SrcP[ 4 ] >> 5 ) ] ;
			DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) | ( SrcP[ 5 ] >> 6 ) ] ;
			DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) | ( SrcP[ 6 ] >> 7 ) ] ;
			DestP[ 7 ] = BinToChar128Table[ ( ( SrcP[ 6 ] & 0x7f )      )                      ] ;

			DestP += 8 ;
			SrcP  += 7 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
			if( ModNum == 1 )
			{
				DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) ] ;
			}
			else
			{
				DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
				if( ModNum == 2 )
				{
					DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) ] ;
				}
				else
				{
					DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
					if( ModNum == 3 )
					{
						DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) ] ;
					}
					else
					{
						DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
						if( ModNum == 4 )
						{
							DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) ] ;
						}
						else
						{
							DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) | ( SrcP[ 4 ] >> 5 ) ] ;
							if( ModNum == 5 )
							{
								DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) ] ;
							}
							else
							{
								DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) | ( SrcP[ 5 ] >> 6 ) ] ;
								if( ModNum == 6 )
								{
									DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) ] ;
								}
								else
								{
									DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) | ( SrcP[ 6 ] >> 7 ) ] ;
									DestP[ 7 ] = BinToChar128Table[ ( ( SrcP[ 6 ] & 0x7f )      )                      ] ;
								}
							}
						}
					}
				}
			}
		}
	}

	return DestSize ;
}

// ���p��������o�C�i���f�[�^�ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern unsigned int Char128ToBin( void *Src, void *Dest )
{
	unsigned int DestSize ;
	unsigned char *SrcP ;
	unsigned char *DestP ;

	SrcP    = ( unsigned char * )Src ;
	DestP   = ( unsigned char * )&DestSize ;

	DestP[ 0 ] = ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ;
	DestP[ 1 ] = ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ;
	DestP[ 2 ] = ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ;
	DestP[ 3 ] = ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ;

	SrcP += 5 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned int i ;

		PackNum = DestSize / 7 ;
		ModNum  = DestSize - PackNum * 7 ;
		DestP = ( unsigned char * )Dest ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ;
			DestP[ 1 ] = ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ;
			DestP[ 2 ] = ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ;
			DestP[ 3 ] = ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ;
			DestP[ 4 ] = ( Char128ToBinTable[ SrcP[ 4 ] ] << 5 ) | ( Char128ToBinTable[ SrcP[ 5 ] ] >> 2 ) ;
			DestP[ 5 ] = ( Char128ToBinTable[ SrcP[ 5 ] ] << 6 ) | ( Char128ToBinTable[ SrcP[ 6 ] ] >> 1 ) ;
			DestP[ 6 ] = ( Char128ToBinTable[ SrcP[ 6 ] ] << 7 ) | ( Char128ToBinTable[ SrcP[ 7 ] ]      ) ;

			DestP += 7 ;
			SrcP  += 8 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ;
			if( ModNum > 1 )
			{
				DestP[ 1 ] = ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ;
				if( ModNum > 2 )
				{
					DestP[ 2 ] = ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ;
					if( ModNum > 3 )
					{
							DestP[ 3 ] = ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ;
						if( ModNum > 4 )
						{
							DestP[ 4 ] = ( Char128ToBinTable[ SrcP[ 4 ] ] << 5 ) | ( Char128ToBinTable[ SrcP[ 5 ] ] >> 2 ) ;
							if( ModNum > 5 )
							{
								DestP[ 5 ] = ( Char128ToBinTable[ SrcP[ 5 ] ] << 6 ) | ( Char128ToBinTable[ SrcP[ 6 ] ] >> 1 ) ;
								if( ModNum > 6 )
								{
									DestP[ 6 ] = ( Char128ToBinTable[ SrcP[ 6 ] ] << 7 ) | ( Char128ToBinTable[ SrcP[ 7 ] ]      ) ;
								}
							}
						}
					}
				}
			}
		}
	}

	return DestSize ;
}

// �o�C�i���f�[�^��Base64������ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern unsigned int BinToBase64( void *Src, unsigned int SrcSize, void *Dest )
{
	unsigned int DestSize ;
	unsigned int ModNum ;
	unsigned int PackNum ;

	PackNum = SrcSize / 3 ;
	ModNum  = SrcSize - PackNum * 3 ;
	DestSize  = PackNum * 4 + ( ModNum > 0 ? ModNum + 1 : 0 ) + 6 ;

	if( Dest != NULL )
	{
		unsigned char *DestP ;
		unsigned char *SrcP ;
		unsigned int i ;

		DestP = ( unsigned char * )Dest ;
		SrcP  = ( unsigned char * )&SrcSize ;

		DestP[ 0 ] = BinToBase64Table[                                 ( SrcP[ 0 ] >> 2 ) ] ;
		DestP[ 1 ] = BinToBase64Table[ ( ( SrcP[ 0 ] & 0x03 ) << 4 ) | ( SrcP[ 1 ] >> 4 ) ] ;
		DestP[ 2 ] = BinToBase64Table[ ( ( SrcP[ 1 ] & 0x0f ) << 2 ) | ( SrcP[ 2 ] >> 6 ) ] ;
		DestP[ 3 ] = BinToBase64Table[ ( ( SrcP[ 2 ] & 0x3f )      )                      ] ;
		DestP[ 4 ] = BinToBase64Table[                                 ( SrcP[ 3 ] >> 2 ) ] ;
		DestP[ 5 ] = BinToBase64Table[ ( ( SrcP[ 3 ] & 0x03 ) << 4 )                      ] ;

		DestP += 6 ;

		SrcP  = ( unsigned char * )Src ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = BinToBase64Table[                                 ( SrcP[ 0 ] >> 2 ) ] ;
			DestP[ 1 ] = BinToBase64Table[ ( ( SrcP[ 0 ] & 0x03 ) << 4 ) | ( SrcP[ 1 ] >> 4 ) ] ;
			DestP[ 2 ] = BinToBase64Table[ ( ( SrcP[ 1 ] & 0x0f ) << 2 ) | ( SrcP[ 2 ] >> 6 ) ] ;
			DestP[ 3 ] = BinToBase64Table[ ( ( SrcP[ 2 ] & 0x3f )      )                      ] ;

			DestP += 4 ;
			SrcP  += 3 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = BinToBase64Table[ ( SrcP[ 0 ] >> 2 ) ] ;
			if( ModNum == 1 )
			{
				DestP[ 1 ] = BinToBase64Table[ ( ( SrcP[ 0 ] & 0x03 ) << 4 )                      ] ;
				DestP += 2 ;
			}
			else
			{
				DestP[ 1 ] = BinToBase64Table[ ( ( SrcP[ 0 ] & 0x03 ) << 4 ) | ( SrcP[ 1 ] >> 4 ) ] ;
				DestP[ 2 ] = BinToBase64Table[ ( ( SrcP[ 1 ] & 0x0f ) << 2 )                      ] ;
				DestP += 3 ;
			}
		}
	}

	return DestSize ;
}

// Base64��������o�C�i���f�[�^�ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern unsigned int Base64ToBin( void *Src, void *Dest )
{
	unsigned int DestSize ;
	unsigned char *SrcP ;
	unsigned char *DestP ;

	SrcP    = ( unsigned char * )Src ;
	DestP   = ( unsigned char * )&DestSize ;

	DestP[ 0 ] = ( Base64ToBinTable[ SrcP[ 0 ] ] << 2 ) | ( Base64ToBinTable[ SrcP[ 1 ] ] >> 4 ) ;
	DestP[ 1 ] = ( Base64ToBinTable[ SrcP[ 1 ] ] << 4 ) | ( Base64ToBinTable[ SrcP[ 2 ] ] >> 2 ) ;
	DestP[ 2 ] = ( Base64ToBinTable[ SrcP[ 2 ] ] << 6 ) | ( Base64ToBinTable[ SrcP[ 3 ] ]      ) ;
	DestP[ 3 ] = ( Base64ToBinTable[ SrcP[ 4 ] ] << 2 ) | ( Base64ToBinTable[ SrcP[ 5 ] ] >> 4 ) ;

	SrcP += 6 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned int i ;

		PackNum = DestSize / 3 ;
		ModNum  = DestSize - PackNum * 3 ;
		DestP = ( unsigned char * )Dest ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = ( Base64ToBinTable[ SrcP[ 0 ] ] << 2 ) | ( Base64ToBinTable[ SrcP[ 1 ] ] >> 4 ) ;
			DestP[ 1 ] = ( Base64ToBinTable[ SrcP[ 1 ] ] << 4 ) | ( Base64ToBinTable[ SrcP[ 2 ] ] >> 2 ) ;
			DestP[ 2 ] = ( Base64ToBinTable[ SrcP[ 2 ] ] << 6 ) | ( Base64ToBinTable[ SrcP[ 3 ] ]      ) ;

			DestP += 3 ;
			SrcP  += 4 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = ( Base64ToBinTable[ SrcP[ 0 ] ] << 2 ) | ( Base64ToBinTable[ SrcP[ 1 ] ] >> 4 ) ;
			if( ModNum > 1 )
			{
				DestP[ 1 ] = ( Base64ToBinTable[ SrcP[ 1 ] ] << 4 ) | ( Base64ToBinTable[ SrcP[ 2 ] ] >> 2 ) ;
				if( ModNum > 2 )
				{
					DestP[ 2 ] = ( Base64ToBinTable[ SrcP[ 2 ] ] << 6 ) | ( Base64ToBinTable[ SrcP[ 3 ] ]      ) ;
				}
			}
		}
	}

	return DestSize ;
}

// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int LoadFileMem( const TCHAR *Path, void **DataBuf, size_t *Size )
{
	FILE *fp = NULL ;
	void *buf = NULL ;
	__int64 size ;

	fp = _tfopen( Path, TEXT( "rb" ) ) ;
	if( fp == NULL ) goto ERR ;

	// �T�C�Y�𓾂�
	fseek( fp, 0L, SEEK_END ) ;
	size = _ftelli64( fp ) ;
	if( size == 0 ) goto ERR ;
	fseek( fp, 0L, SEEK_SET ) ;

	// �������̊m��
	buf = malloc( ( size_t )size ) ;
	if( buf == NULL ) goto ERR ;

	// �ǂݍ���
	fread( buf, ( size_t )size, 1, fp ) ;
	fclose( fp ) ;

	// �Z�b�g
	if( DataBuf != NULL ) *DataBuf = buf ;
	if( Size != NULL ) *Size = ( size_t )size ;

	// �I��
	return 0 ;

ERR :
	if( buf != NULL ) free( buf ) ;
	if( fp != NULL ) fclose( fp ) ;

	return -1 ;
}

// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int LoadFileMem( const TCHAR *Path, void *DataBuf, size_t *Size )
{
	FILE *fp = NULL ;
	__int64 size ;

	fp = _tfopen( Path, TEXT( "rb" ) ) ;
	if( fp == NULL ) return -1;

	// �T�C�Y�𓾂�
	fseek( fp, 0L, SEEK_END ) ;
	size = _ftelli64( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;

	// �ǂݍ���
	if( DataBuf )
	{
		fread( DataBuf, ( size_t )size, 1, fp ) ;
	}
	fclose( fp ) ;

	// �Z�b�g
	if( Size != NULL ) *Size = ( size_t )size ;

	// �I��
	return 0 ;
}

// �������̓��e���t�@�C���ɏ����o�� 
extern int SaveFileMem( const TCHAR *Path, void *Data, size_t Size )
{
	FILE *fp ;

	fp = _tfopen( Path, TEXT( "wb" ) ) ;
	if( fp == NULL ) return -1 ;
	fwrite( Data, Size, 1, fp ) ;
	fclose( fp ) ;

	return 0 ;
}


// �w��̃f�B���N�g�����쐬����A���Ԃ̃f�B���N�g�������݂��Ȃ��ꍇ�͍쐬����
// �Ō���� '\' �������Ă���������
// �h���C�u���̌�� '\' ���Ȃ��ꍇ�͐���ɓ��삵�Ȃ�
extern int __CreateDirectory( const TCHAR *Path )
{
	TCHAR dir[MAX_PATH] ;

	// �t���p�X�𓾂�
	ConvertFullPath__( Path, dir, NULL ) ;
	SetEnMark( dir ) ;

	// �w��̃f�B���N�g�������݂����牽�������I��
	{
		WIN32_FIND_DATA FindData ;
		HANDLE FindHandle ;

		FindHandle = FindFirstFile( dir, &FindData ) ;
		if( FindHandle != INVALID_HANDLE_VALUE )
		{
			FindClose( FindHandle ) ;
			return 0 ;
		}
	}

	// �f�B���N�g�����쐬���郋�[�v
	{
		TCHAR *p ;
		p = _tcschr( dir, TEXT( '\\' ) ) ;
		while( p != NULL )
		{
			*p = '\0' ;
			CreateDirectory( dir, NULL ) ;
			*p = '\\' ;

			p = _tcschr( p + 1, TEXT( '\\' ) ) ;
		}
	}

	// �I��
	return 0 ;
}


// �w��̃p�X�������Ă�����̂��f�B���N�g�����ǂ����𓾂�
extern int IsDirectory( const TCHAR *Path )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle ;
	
	// �t�@�C���̏��𓾂�
	FindHandle = FindFirstFile( Path, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1 ;
	FindClose( FindHandle ) ;
	
	// �f�B���N�g�����ǂ�����Ԃ�
	return ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0 ;
}

// �w��̃p�X�̏��𓾂�
extern int CreateFileInfo( const TCHAR *Path, FILE_INFO *FileInfoBuffer )
{
	FILE_INFO *info ;
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle = INVALID_HANDLE_VALUE ;
	int AbsDirLen, RelDirLen ;
	TCHAR RelDir[1] ;
	TCHAR AbsDir[PATH_LENGTH] ;
	
	info = FileInfoBuffer ;
	
	// �f�B���N�g���p�X�̍쐬
	{
		ConvertFullPath__( Path, AbsDir ) ;
		DelChr( AbsDir, TEXT( '\\' ) ) ;
		_tcsrchr( AbsDir, TEXT( '\\' ) )[1] = TEXT( '\0' ) ;
		AbsDirLen = ( int )_tcslen( AbsDir ) ;
		RelDir[0] = '\0' ;
		RelDirLen = 0 ;
	}

	// �t�@�C���̏��𓾂�
	FindHandle = FindFirstFile( Path, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1 ;
	FindClose( FindHandle ) ;
	
	// �Ƃ肠�����돉����
	memset( info, 0, sizeof( FILE_INFO ) ) ;

	// ������ۑ�
	info->Date.Create		= ( ( ( LONGLONG )FindData.ftCreationTime	.dwHighDateTime ) << 32 ) + FindData.ftCreationTime		.dwLowDateTime ;
	info->Date.LastAccess	= ( ( ( LONGLONG )FindData.ftLastAccessTime	.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime	.dwLowDateTime ;
	info->Date.LastWrite	= ( ( ( LONGLONG )FindData.ftLastWriteTime	.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime	.dwLowDateTime ;

	info->Size			= FindData.nFileSizeLow ;		// �T�C�Y��ۑ�
	info->Attributes	= FindData.dwFileAttributes ;	// ������ۑ�
	info->IsDirectory	= (u8)(( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0) ;	// �f�B���N�g�����ǂ�����ۑ�

	// �p�X�n�̕ۑ�
	{
		int FileNameLen ;
	
		// �p�X�n��ۑ����郁�����̈�̊m��
		FileNameLen = ( int )_tcslen( FindData.cFileName ) ;
		info->FileName = ( TCHAR * )malloc( sizeof( TCHAR ) * ( ( FileNameLen + 1 ) + ( AbsDirLen + 1 ) + ( RelDirLen + 1 ) ) ) ;
		if( info->FileName == NULL ) return -1 ;
		info->RelDirectoryPath = info->FileName + FileNameLen + 1 ;
		info->AbsDirectoryPath = info->RelDirectoryPath + RelDirLen + 1 ;

		// �R�s�[
		_tcscpy( info->FileName, FindData.cFileName ) ;
		_tcscpy( info->RelDirectoryPath, RelDir ) ;
		_tcscpy( info->AbsDirectoryPath, AbsDir ) ;
	}
	
	// �I��
	return 0 ;
}

// �p�X���̌�n�����s��
extern int ReleaseFileInfo( FILE_INFO *FileInfo )
{
	if( FileInfo->FileName != NULL ) free( FileInfo->FileName ) ;
	FileInfo->FileName = NULL ;
	
	// �I��
	return 0 ;
}

// �w��̃p�X�̃t�@�C���̃^�C���X�^���v�� FileInfo �Ɋi�[����Ă���^�C���X�^���v�ɂ���
extern int SetFileTimeStamp( const TCHAR *Path, FILE_INFO *FileInfo )
{
	HANDLE HFile ;
	FILETIME CreateTime, LastAccessTime, LastWriteTime ;

	HFile = CreateFile( Path,
						GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( HFile == INVALID_HANDLE_VALUE )
	{
		HFile = HFile ;
	}

	CreateTime.dwHighDateTime     = ( u32 )( FileInfo->Date.Create     >> 32        ) ;
	CreateTime.dwLowDateTime      = ( u32 )( FileInfo->Date.Create     & 0xffffffff ) ;
	LastAccessTime.dwHighDateTime = ( u32 )( FileInfo->Date.LastAccess >> 32        ) ;
	LastAccessTime.dwLowDateTime  = ( u32 )( FileInfo->Date.LastAccess & 0xffffffff ) ;
	LastWriteTime.dwHighDateTime  = ( u32 )( FileInfo->Date.LastWrite  >> 32        ) ;
	LastWriteTime.dwLowDateTime   = ( u32 )( FileInfo->Date.LastWrite  & 0xffffffff ) ;
	SetFileTime( HFile, &CreateTime, &LastAccessTime, &LastWriteTime ) ;
	CloseHandle( HFile ) ;

	// �I��
	return 0;
}

// ��̃t�@�C���̃^�C���X�^���v����v���Ă��邩�ǂ����𓾂�(0:��v���Ă��� -1:��v���Ă��Ȃ�)
extern int CmpFileTimeStamp( FILE_INFO *FileInfo1, FILE_INFO *FileInfo2, bool Create, bool LastAccess, bool LastWrite )
{
	if( Create     && FileInfo1->Date.Create     != FileInfo2->Date.Create     ) return -1;
	if( LastAccess && FileInfo1->Date.LastAccess != FileInfo2->Date.LastAccess ) return -1;
	if( LastWrite  && FileInfo1->Date.LastWrite  != FileInfo2->Date.LastWrite  ) return -1;
	return 0;
}

// �w��̃f�B���N�g���̃t�@�C�����X�g���擾����
// �p�X������̍Ō�Ɂw\�x�������Ă���ɍŌ�̕�������f�B���N�g���Ɣ��f����
// �t���p�X�ł͂Ȃ��ꍇ�͌��݂̃J�����g�f�B���N�g������̑��΃p�X�ƂȂ�
// FileListInfo �� 0 �ł͂Ȃ��ꍇ�g�p�� ReleaseFileObjectList ���ĂԕK�v������
// SubDirectory �� 1 �ɂ���ƃT�u�f�B���N�g�����̃t�@�C�����S�ė񋓂���
// NULL �A�������͕���������Ȃ��������n�����ꍇ�J�����g�f�B���N�g���̃t�@�C�����񋓂����
// OmitDirectory �� 1 �ɂ���ƃf�B���N�g���͗񋓂��Ȃ�
// OmitName �ɏ��O�������t�@�C������n���Ƃ��̕���������O���Ă����( ';'�ŕ����̕������A���\ )
// OmitExName �ɏ��O�������g���q��n���Ƃ��̊g���q�����t�@�C�������O���Ă����( ';'�ŕ����̕������A���\ )
// ValidExName �ɗL���ɂ������g���q��n���Ƃ��̊g���q�����t�@�C���̂ݗ񋓂��Ă����( Omit�n�����D��x������, ';'�ŕ����̕������A���\ )
// �߂�l : -1 = �G���[  0�ȏ� = �񋓂����t�@�C���̐�
extern int CreateFileList( const TCHAR *DirectoryPath, FILE_INFOLIST *FileListInfo,
							int OmitDirectory, int SubDirectory,
							const TCHAR *OmitName, const TCHAR *OmitExName, const TCHAR *ValidExName,
						void ( *EnumFileCallback )( int Phase, int NowFileNum, int TotalFileNum, const TCHAR *FileName, const TCHAR *RelDirPath, const TCHAR *AbsDirPath ) )
{
	TCHAR DirPath[PATH_LENGTH] ;
	FILE_INFOLIST FileList ;
	TCHAR *OmitStrBuf = NULL, *OmitExStrBuf = NULL, *ValidExStrBuf = NULL ;
	TCHAR *OmitStr[50], *OmitExStr[50], *ValidExStr[100] ;
	int TotalFileNum ;
	int TotalFileNumCounter ;
	
	// �t���p�X�𓾂�
	ConvertFullPath__( DirectoryPath, DirPath, NULL ) ;
	
	// �f�B���N�g���ł͂Ȃ��t�@�C���������獟���ŏ������Ă��܂�
	{
		int res ;
		
		DelChr( DirPath, '\\' ) ;
		
		res = IsDirectory( DirPath ) ;
		if( res == -1 ) return -1 ;
		
		if( IsDirectory( DirPath ) == 0 )
		{
			FileListInfo->Num = 1 ;
			
			FileListInfo->List = ( FILE_INFO * )malloc( sizeof( FILE_INFO ) ) ;
			CreateFileInfo( DirectoryPath, FileListInfo->List ) ;
			
			return 1 ;
		}

		SetEnMark( DirPath ) ;
	}

	// ������
	FileList.Num = 0 ;
	FileList.List = NULL ;
	OmitStrBuf = ( TCHAR * )malloc( sizeof( TCHAR ) * 2048 ) ;
	if( OmitStrBuf == NULL ) goto ERR ;
	OmitExStrBuf = ( TCHAR * )malloc( sizeof( TCHAR ) * 2048 ) ;
	if( OmitExStrBuf == NULL ) goto ERR ;
	ValidExStrBuf = ( TCHAR * )malloc( sizeof( TCHAR ) * 2048 ) ;
	if( ValidExStrBuf == NULL ) goto ERR ;

	// ���O�����񃊃X�g�̍쐬
	if( OmitName != NULL )
	{
		int i ;
		TCHAR *p ;

		_tcscpy( OmitStrBuf, OmitName ) ;
		i = 0 ;
		p = OmitStrBuf ;
		OmitStr[i] = p ;
		i ++ ;
		while( ( p = _tcschr( p, TEXT( ';' ) ) ) != NULL )
		{
			*p = TEXT( '\0' ) ;
			p ++ ;
			OmitStr[i] = p ;
			i ++ ;
		}
		OmitStr[i] = NULL ;
	}
	else OmitStr[0] = NULL ;

	// ���O�g���q���X�g�̍쐬
	if( OmitExName != NULL )
	{
		int i ;
		TCHAR *p ;

		_tcscpy( OmitExStrBuf, OmitExName ) ;
		i = 0 ;
		p = OmitExStrBuf ;
		OmitExStr[i] = p ;
		i ++ ;
		while( ( p = _tcschr( p, TEXT( ';' ) ) ) != NULL )
		{
			*p = TEXT( '\0' ) ;
			p ++ ;
			OmitExStr[i] = p ;
			i ++ ;
		}
		OmitExStr[i] = NULL ;
	}
	else OmitExStr[0] = NULL ;

	// �L���g���q���X�g�̍쐬
	if( ValidExName != NULL )
	{
		int i ;
		TCHAR *p ;

		_tcscpy( ValidExStrBuf, ValidExName ) ;
		i = 0 ;
		p = ValidExStrBuf ;
		ValidExStr[i] = p ;
		i ++ ;
		while( ( p = _tcschr( p, TEXT( ';' ) ) ) != NULL )
		{
			*p = TEXT( '\0' ) ;
			p ++ ;
			ValidExStr[i] = p ;
			i ++ ;
		}
		ValidExStr[i] = NULL ;
	}
	else ValidExStr[0] = NULL ;

	// �񋓗p�֐��ɓ����ăt�@�C���̐��𓾂�
	TotalFileNumCounter = 0 ;
	FileList.Num = __EnumObject( DirPath, DirPath, NULL, OmitDirectory, SubDirectory, OmitStr, OmitExStr, ValidExStr[0] != NULL ? ValidExStr : NULL, &TotalFileNumCounter, 0, EnumFileCallback ) ;
	if( FileList.Num < 0 )
		goto ERR ;
	TotalFileNum = TotalFileNumCounter ;

	// �t�@�C�����X�g�̃f�[�^���i�[�ł��郁�����̈���m��
	FileList.List = ( FILE_INFO * )malloc( FileList.Num * sizeof( FILE_INFO ) ) ;
	if( FileList.List == NULL )
		goto ERR ;
	memset( FileList.List, 0, FileList.Num * sizeof( FILE_INFO ) ) ;

	// �t�@�C�����X�g�Ƀf�[�^���i�[����
	TotalFileNumCounter = 0 ;
	FileList.Num = 0 ;
	if( __EnumObject( DirPath, DirPath, &FileList, OmitDirectory, SubDirectory, OmitStr, OmitExStr, ValidExStr[0] != NULL ? ValidExStr : NULL, &TotalFileNumCounter, TotalFileNum, EnumFileCallback ) < 0 )
		goto ERR ;

	// �f�[�^���i�[
	if( FileListInfo != NULL ) *FileListInfo = FileList ;

	// ���O������o�b�t�@���
	free( OmitStrBuf ) ;

	// ���O�g���q�o�b�t�@���
	free( OmitExStrBuf ) ;

	// �L���g���q�o�b�t�@���
	free( ValidExStrBuf ) ;

	// �I��
	return FileList.Num ;

	// �G���[����
ERR :
	ReleaseFileList( &FileList ) ;
	if( OmitStrBuf != NULL ) free( OmitStrBuf ) ;
	if( OmitExStrBuf != NULL ) free( OmitExStrBuf ) ;
	if( ValidExStrBuf != NULL ) free( ValidExStrBuf ) ;
	
	return -1 ;
}

extern int ReleaseFileList( FILE_INFOLIST *DirectoryInfo )
{
	// �������̉��
	if( DirectoryInfo->List != NULL )
	{
		int i ;
		FILE_INFO *finfo ;

		finfo = DirectoryInfo->List ;
		for( i = 0 ; i < DirectoryInfo->Num ; i ++, finfo ++ )
			if( finfo->FileName != NULL ) free( finfo->FileName ) ;

		free( DirectoryInfo->List ) ;
		DirectoryInfo->List = NULL ;
	}

	// �[��������
	memset( DirectoryInfo, 0, sizeof( FILE_INFOLIST ) ) ;

	// �I��
	return 0 ;
}

// ����̃p�X�����񂩂�F�X�ȏ����擾����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int AnalyseFilePath(
	const TCHAR *Src,
	TCHAR *FullPath, TCHAR *DirPath, TCHAR *FileName, TCHAR *Name, TCHAR *ExeName, const TCHAR *CurrentDir )
{
	TCHAR full[1124], dir[1124], fname[1124], name[1124], exename[1124];

	ConvertFullPath__( Src, full, CurrentDir );
	AnalysisFileNameAndDirPath( full, fname, dir );
	AnalysisFileNameAndExeName( fname, name, exename );
	if( FullPath ) _tcscpy( FullPath, full    );
	if( DirPath  ) _tcscpy( DirPath,  dir     );
	if( FileName ) _tcscpy( FileName, fname   );
	if( Name     ) _tcscpy( Name,     name    );
	if( ExeName  ) _tcscpy( ExeName,  exename );

	return 0;
}

// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� NULL �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int ConvertFullPath__( const TCHAR *Src, TCHAR *Dest, const TCHAR *CurrentDir )
{
	int i, j, k ;
	TCHAR iden[512], cur[1024] ;

	if( CurrentDir == NULL )
	{
		GetCurrentDirectory( 1024, cur ) ;
		CurrentDir = cur ;
	}

	if( Src == NULL )
	{
		_tcscpy( Dest, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	j = 0 ;
	k = 0 ;
	
	// �ŏ��Ɂw\�x���́w/�x���Q��A���ő����Ă���ꍇ�̓l�b�g���[�N����Ă���Ɣ��f
	if( ( Src[0] == TEXT( '\\' ) && Src[1] == TEXT( '\\' ) ) ||
		( Src[0] == TEXT( '/'  ) && Src[1] == TEXT( '/'  ) ) )
	{
		Dest[0] = TEXT( '\\' );
		Dest[1] = TEXT( '\0' );

		i += 2;
		j ++ ;
	}
	else
	// �ŏ����w\�x���́w/�x�̏ꍇ�̓J�����g�h���C�u�̃��[�g�f�B���N�g���܂ŗ�����
	if( Src[0] == TEXT( '\\' ) )
	{
		Dest[0] = CurrentDir[0] ;
		Dest[1] = CurrentDir[1] ;
		Dest[2] = TEXT( '\0' ) ;

		i ++ ;
		j = 2 ;
	}
	else
	// �h���C�u����������Ă����炻�̃h���C�u��
	if( Src[1] == TEXT( ':' ) )
	{
		Dest[0] = Src[0] ;
		Dest[1] = Src[1] ;
		Dest[2] = TEXT( '\0' ) ;

		i = 2 ;
		j = 2 ;

		if( Src[i] == TEXT( '\\' ) ) i ++ ;
	}
	else
	// ����ȊO�̏ꍇ�̓J�����g�f�B���N�g��
	{
		_tcscpy( Dest, CurrentDir ) ;
		j = ( int )_tcslen( Dest ) ;
		if( Dest[j-1] == TEXT( '\\' ) || Dest[j-1] == TEXT( '/' ) )
		{
			Dest[j-1] = TEXT( '\0' ) ;
			j -- ;
		}
	}

	for(;;)
	{
		switch( Src[i] )
		{
		case TEXT( '\0' ) :
			if( k != 0 )
			{
				Dest[j] = TEXT( '\\' ) ; j ++ ;
				_tcscpy( &Dest[j], iden ) ;
				j += k ;
				k = 0 ;
			}
			goto END ;

		case TEXT( '\\' ) :
		case TEXT( '/'  ) :
			// �����񂪖���������X�L�b�v
			if( k == 0 )
			{
				i ++ ;
				break;
			}
			if( _tcscmp( iden, TEXT( "." ) ) == 0 )
			{
				// �Ȃɂ����Ȃ�
			}
			else
			if( _tcscmp( iden, TEXT( ".." ) ) == 0 )
			{
				// ����̃f�B���N�g����
				j -- ;
				while( Dest[j] != TEXT( '\\' ) && Dest[j] != TEXT( '/' ) && Dest[j] != TEXT( ':' ) ) j -- ;
				if( Dest[j] != TEXT( ':' ) ) Dest[j] = TEXT( '\0' ) ;
				else j ++ ;
			}
			else
			{
				Dest[j] = TEXT( '\\' ) ; j ++ ;
				_tcscpy( &Dest[j], iden ) ;
				j += k ;
			}

			k = 0 ;
			i ++ ;
			break ;
		
		default :
#ifdef _UNICODE
			iden[k] = Src[i] ;
			iden[k+1] = 0 ; 
			k ++ ;
			i ++ ;
#else // _UNICODE
			if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
			{
				iden[k] = Src[i] ;
				iden[k+1] = 0 ; 
				k ++ ;
				i ++ ;
			}
			else
			{
				*(( unsigned short * )&iden[k] ) = *(( unsigned short * )&Src[i] ) ;
				iden[k+2] = '\0' ;
				k += 2 ;
				i += 2 ;
			}
#endif // _UNICODE
			break ;
		}
	}
	
END :
	// ����I��
	return 0 ;
}

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
extern int AnalysisFileNameAndDirPath( const TCHAR *Src, TCHAR *FileName, TCHAR *DirPath )
{
	int i, Last ;
	
	// �t�@�C�����𔲂��o��
	i = 0 ;
	Last = -1 ;
	while( Src[i] != TEXT( '\0' ) )
	{
#ifdef _UNICODE
		if( Src[i] == TEXT( '\\' ) || Src[i] == TEXT( '/' ) || Src[i] == TEXT( '\0' ) || Src[i] == TEXT( ':' ) ) Last = i ;
		i ++ ;
#else // _UNICODE
		if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
#endif // _UNICODE
	}
	if( FileName != NULL )
	{
		if( Last != -1 ) _tcscpy( FileName, &Src[Last+1] ) ;
		else _tcscpy( FileName, Src ) ;
	}
	
	// �f�B���N�g���p�X�𔲂��o��
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			_tcsncpy( DirPath, Src, Last ) ;
			DirPath[Last] = TEXT( '\0' ) ;
		}
		else
		{
			DirPath[0] = TEXT( '\0' ) ;
		}
	}
	
	// �I��
	return 0 ;
}

// �t�@�C���p�X����t�@�C�����Ɗg���q���擾����
extern int AnalysisFileNameAndExeName( const TCHAR *Src, TCHAR *Name, TCHAR *ExeName )
{
	TCHAR FileName[256], *p, ename[256], name[256] ;

	// �t�@�C�����݂̂��擾
	AnalysisFileNameAndDirPath( Src, FileName, 0 ) ;

	// �w.�x�����邩�ǂ����ŏ����𕪊�
	if( ( p = _tcsrchr( FileName, TEXT( '.' ) ) ) == NULL )
	{
		_tcscpy( name, FileName ) ;
		ename[0] = TEXT( '\0' ) ;
	}
	else
	{
		_tcsncpy( name, FileName, p - FileName ) ;
		name[p - FileName] = TEXT( '\0' ) ;
		_tcscpy( ename, p + 1 ) ;
	}

	if( Name != NULL ) _tcscpy( Name, name ) ;
	if( ExeName != NULL ) _tcscpy( ExeName, ename ) ;

	// �I��
	return 0 ;
}

// �t�@�C���p�X�̊g���q��ς���������𓾂�
extern int GetChangeExeNamePath( const TCHAR *Src, TCHAR *Dest, const TCHAR *ExeName )
{
	TCHAR DirPath[1024], FileName[1024] ;

	AnalysisFileNameAndDirPath( Src, NULL, DirPath ) ;
	AnalysisFileNameAndExeName( Src, FileName, 0 ) ;
	SetEnMark( DirPath ) ;
	_stprintf( Dest, TEXT( "%s%s.%s" ), DirPath, FileName, ExeName ) ;

	// �I��
	return 0 ;
}


// ����Ɂw\�x�����Ă��Ȃ��ꍇ�͕t����
extern void SetEnMark( TCHAR *PathBuf )
{
	int Len = ( int )_tcslen( PathBuf ) ;
	
	if( PathBuf[Len-1] != TEXT( '\\' ) )
	{
		PathBuf[Len] = TEXT( '\\' ) ;
		PathBuf[Len+1] = TEXT( '\0' ) ;
	}
}

// ����Ɏw��̕������Ȃ��ꍇ�͂���
extern void SetChr( TCHAR *PathBuf, TCHAR chr )
{
	int Len = ( int )_tcslen( PathBuf ) ;
	
	if( PathBuf[Len-1] != chr )
	{
		PathBuf[Len] = chr ;
		PathBuf[Len+1] = TEXT( '\0' ) ;
	}
}

// ����Ɏw��̕���������ꍇ�͍폜����
extern void DelChr( TCHAR *PathBuf, TCHAR chr )
{
	int Len = ( int )_tcslen( PathBuf ) ;

	if( PathBuf[Len-1] == chr )
	{
		PathBuf[Len-1] = TEXT( '\0' ) ;
	}
}


// �g���q�𓾂�
extern int GetExName( const TCHAR *Path, TCHAR *ExNameBuf )
{
	TCHAR *p ;
	
	// ��ԍŌ�� '.' ���o�镔�����擾����
	p = _tcsrchr( ( TCHAR * )Path, TEXT( '.' ) ) ;
	if( p == NULL ) ExNameBuf[0] = TEXT( '\0' ) ;
	else _tcscpy( ExNameBuf, p + 1 ) ;

	// �I��
	return 0 ;
}

// �g���q��ύX����
extern int SetExName( const TCHAR *Path, const TCHAR *ExName, TCHAR *DestBuf )
{
	TCHAR *p ;
	TCHAR tempstr[1024] ;
	
	_tcscpy( tempstr, Path ) ;

	// ��ԍŌ�� '.' ���o�镔�����擾����
	p = _tcsrchr( tempstr, TEXT( '.' ) ) ;
	if( p == NULL )
	{
		_stprintf( DestBuf, TEXT( "%s.%s" ), tempstr, ExName ) ;
	}
	else
	{
		_tcsncpy( DestBuf, tempstr, p - tempstr + 1 ) ;
		_tcscpy( DestBuf + ( p - tempstr + 1 ), ExName ) ;
	}

	// �I��
	return 0 ;
}


// �e�L�X�g�f�[�^���ǂ����𔻒肷��( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )
extern int CheckTextData( void *buffer, int size )
{
	unsigned char *p ;
	int len, saerch ;
	
	// �擪�W�j�a�̂݌���
	saerch = size > TEXTCHECKSIZE ? TEXTCHECKSIZE : size ;

	// �I�[�������Q��ȏ�o�Ă�����o�C�i���f�[�^
	{
		int con ;
	
		p = ( unsigned char * )buffer ;
		con = 0 ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( *p == '\0' )
			{
				con ++ ;
				if( con >= 2 ) return 0 ;
			}
		}
	}
	
	// ���s�A�^�u�ȊO�̃R���g���[���������o�Ă�����o�C�i���f�[�^
	{
		int con ;
		
		p = ( unsigned char * )buffer ;
		con = 0 ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( *p < 0x20 && *p != '\r' && *p != '\n' && *p != '\t' )
			{
				con ++ ;
				if( con >= 1 ) return 0 ;
			}
		}
	}
	
	// �����ł͂Ȃ��R�[�h���o�Ă�����o�C�i���f�[�^
	{
		p = ( unsigned char * )buffer ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( ( *p >= 0x81 && *p <= 0x9f ) || ( *p >= 0xe0 && *p <= 0xfc ) )
			{
				len -- ;
				p ++ ;
				if( len == 0 && size <= saerch ) return 0 ;
				
				if( !( ( *p >= 0x40 && *p <= 0x7e ) || ( *p >= 0x80 && *p <= 0xfc ) ) )
					return 0 ;
				
				if( len == 0 ) return 1 ;
			}
		}
	}
	
	// �����ɗ�����e�L�X�g�f�[�^
	return 1 ;
}

// �e�L�X�g�t�@�C�����ǂ����𔻒肷��( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )
extern int CheckTextFile( const TCHAR *Path )
{
	void *buf ;
	int size, res ;
	FILE *fp ;

	fp = _tfopen( Path, TEXT( "rb" ) ) ;
	if( fp == NULL ) return -1 ;
	
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;
	
	size = size > TEXTCHECKSIZE ? TEXTCHECKSIZE : size ;
	
	buf = malloc( size ) ;
	if( buf == NULL )
	{
		fclose( fp ) ;
		return -1 ;
	}
	
	fread( buf, size, 1, fp ) ;
	fclose( fp ) ;
	
	res = CheckTextData( buf, size ) ;
	
	free( buf ) ;

	return res ;
}

// �Q�o�C�g���������ׂ�( TRUE:�Q�o�C�g���� FALSE:�P�o�C�g���� )
extern int CheckMultiByteChar( char *Buf )
{
	return  ( (unsigned char)*Buf >= 0x81 && (unsigned char)*Buf <= 0x9F ) || ( (unsigned char)*Buf >= 0xE0 && (unsigned char)*Buf <= 0xFC ) ;
}

// �o�C�i���f�[�^������ CRC32 �̃n�b�V���l���v�Z����
extern u32 FileLib_HashCRC32( const void *SrcData, size_t SrcDataSize )
{
	static u32 CRC32Table[ 256 ] ;
	static int CRC32TableInit = 0 ;
	u32 CRC = 0xffffffff ;
	u8 *SrcByte = ( u8 * )SrcData ;
	u32 i ;

	// �e�[�u��������������Ă��Ȃ������珉��������
	if( CRC32TableInit == 0 )
	{
		u32 Magic = 0xedb88320 ;	// 0x4c11db7 ���r�b�g���x���ŏ��Ԃ��t�ɂ������̂� 0xedb88320
		u32 j ;

		for( i = 0; i < 256; i++ )
		{
			u32 Data = i ;
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
		CRC = CRC32Table[ ( u8 )( CRC ^ SrcByte[ i ] ) ] ^ ( CRC >> 8 ) ;
	}

	return CRC ^ 0xffffffff ;
}






