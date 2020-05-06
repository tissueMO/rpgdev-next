// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u�����A�[�J�C�o Ver5
// 
//	Creator			: �R�c �I
//	Creation Date	: 2003/09/11
//	Version			: 1.02
//
// -------------------------------------------------------------------------------

// ���d�C���N���[�h�h�~�p��`
#ifndef __DXARCHIVE_VER5
#define __DXARCHIVE_VER5
#pragma warning(disable:4996)

// include --------------------------------------
#include <stdio.h>

// define ---------------------------------------

// �f�[�^�^��`
#ifndef u64
#define u64		unsigned __int64
#endif

#ifndef u32
#define u32		unsigned int
#endif

#ifndef u16
#define u16		unsigned short
#endif

#ifndef u8
#define u8		unsigned char
#endif

#ifndef s64
#define s64		signed __int64
#endif

#ifndef s32
#define s32		signed int
#endif

#ifndef s16
#define s16		signed short
#endif

#ifndef s8
#define s8		signed char
#endif

#ifndef f64
#define f64		double
#endif

#ifndef TRUE
#define TRUE	(1)
#endif

#ifndef FALSE
#define FALSE	(0)
#endif

#ifndef NULL
#define NULL	(0)
#endif

#define DXA_HEAD_VER5			*((u16 *)"DX")	// �w�b�_
#define DXA_VER_VER5			(0x0005)		// �o�[�W����
#define DXA_BUFFERSIZE_VER5		(0x1000000)		// �A�[�J�C�u�쐬���Ɏg�p����o�b�t�@�̃T�C�Y
#define DXA_KEYSTR_LENGTH_VER5	(12)			// ��������̒���

/*
	�o�[�W�������Ƃ̈Ⴂ

	0x0002 DARC_FILEHEAD_VER5 �� PressDataSize ��ǉ�
	0x0004 DARC_HEAD_VER5 �� CodePage ��ǉ�
	0x0005 �Í����������ꕔ�ύX
*/

/*
	�f�[�^�}�b�v
		
	DARC_HEAD_VER5
	�t�@�C�����f�[�^
	�t�@�C�����e�[�u��
	DARC_FILEHEAD_VER5 �e�[�u��
	DARC_DIRECTORY_VER5 �e�[�u��
*/

/*
	�t�@�C�����̃f�[�^�`��
	2byte:������̒���(�o�C�g�T�C�Y���S)
	2byte:������̃p���e�B�f�[�^(�S�Ă̕����̒l�𑫂�������)
	�p���͑啶���ɕϊ����ꂽ�t�@�C�����̃f�[�^(�S�̔{���̃T�C�Y)
	�p�����啶���ɕϊ�����Ă��Ȃ��t�@�C�����̃f�[�^
*/

// struct ---------------------------------------

#pragma pack(push)
#pragma pack(1)

// �A�[�J�C�u�f�[�^�̍ŏ��̃w�b�_
typedef struct tagDARC_HEAD_VER5
{
	u16 Head ;								// �w�b�_
	u16 Version ;							// �o�[�W����
	u32 HeadSize ;							// �w�b�_���� DARC_HEAD_VER5 �𔲂����S�T�C�Y
	u32 DataStartAddress ;					// �ŏ��̃t�@�C���̃f�[�^���i�[����Ă���f�[�^�A�h���X(�t�@�C���̐擪�A�h���X���A�h���X�O�Ƃ���)
	u32 FileNameTableStartAddress ;			// �t�@�C�����e�[�u���̐擪�A�h���X(�t�@�C���̐擪�A�h���X���A�h���X�O�Ƃ���)
	u32 FileTableStartAddress ;				// �t�@�C���e�[�u���̐擪�A�h���X(�����o�ϐ� FileNameTableStartAddress �̃A�h���X���O�Ƃ���)
	u32 DirectoryTableStartAddress ;		// �f�B���N�g���e�[�u���̐擪�A�h���X(�����o�ϐ� FileNameTableStartAddress �̃A�h���X���O�Ƃ���)
											// �A�h���X�O����z�u����Ă��� DARC_DIRECTORY_VER5 �\���̂����[�g�f�B���N�g��
	u32 CodePage ;							// �t�@�C�����Ɏg�p���Ă���R�[�h�y�[�W�ԍ�
} DARC_HEAD_VER5 ;

// �A�[�J�C�u�f�[�^�̍ŏ��̃w�b�_(Ver 0x0003�܂�)
typedef struct tagDARC_HEAD_VER3
{
	u16 Head ;								// �w�b�_
	u16 Version ;							// �o�[�W����
	u32 HeadSize ;							// �w�b�_���� DARC_HEAD_VER5 �𔲂����S�T�C�Y
	u32 DataStartAddress ;					// �ŏ��̃t�@�C���̃f�[�^���i�[����Ă���f�[�^�A�h���X(�t�@�C���̐擪�A�h���X���A�h���X�O�Ƃ���)
	u32 FileNameTableStartAddress ;			// �t�@�C�����e�[�u���̐擪�A�h���X(�t�@�C���̐擪�A�h���X���A�h���X�O�Ƃ���)
	u32 FileTableStartAddress ;				// �t�@�C���e�[�u���̐擪�A�h���X(�����o�ϐ� FileNameTableStartAddress �̃A�h���X���O�Ƃ���)
	u32 DirectoryTableStartAddress ;		// �f�B���N�g���e�[�u���̐擪�A�h���X(�����o�ϐ� FileNameTableStartAddress �̃A�h���X���O�Ƃ���)
											// �A�h���X�O����z�u����Ă��� DARC_DIRECTORY_VER5 �\���̂����[�g�f�B���N�g��
} DARC_HEAD_VER3 ;

// �t�@�C���̎��ԏ��
typedef struct tagDARC_FILETIME_VER5
{
	u64 Create ;			// �쐬����
	u64 LastAccess ;		// �ŏI�A�N�Z�X����
	u64 LastWrite ;			// �ŏI�X�V����
} DARC_FILETIME_VER5 ;

// �t�@�C���i�[���(Ver 0x0001)
typedef struct tagDARC_FILEHEAD_VER1
{
	u32 NameAddress ;			// �t�@�C�������i�[����Ă���A�h���X( ARCHIVE_HEAD�\���� �̃����o�ϐ� FileNameTableStartAddress �̃A�h���X���A�h���X�O�Ƃ���) 

	u32 Attributes ;			// �t�@�C������
	DARC_FILETIME_VER5 Time ;	// ���ԏ��
	u32 DataAddress ;			// �t�@�C�����i�[����Ă���A�h���X
								//			�t�@�C���̏ꍇ�FDARC_HEAD_VER5�\���� �̃����o�ϐ� DataStartAddress �������A�h���X���A�h���X�O�Ƃ���
								//			�f�B���N�g���̏ꍇ�FDARC_HEAD_VER5�\���� �̃����o�ϐ� DirectoryTableStartAddress �̂������A�h���X���A�h���X�O�Ƃ���
	u32 DataSize ;				// �t�@�C���̃f�[�^�T�C�Y
} DARC_FILEHEAD_VER1 ;

// �t�@�C���i�[���
typedef struct tagDARC_FILEHEAD_VER5
{
	u32 NameAddress ;			// �t�@�C�������i�[����Ă���A�h���X( ARCHIVE_HEAD�\���� �̃����o�ϐ� FileNameTableStartAddress �̃A�h���X���A�h���X�O�Ƃ���) 

	u32 Attributes ;			// �t�@�C������
	DARC_FILETIME_VER5 Time ;	// ���ԏ��
	u32 DataAddress ;			// �t�@�C�����i�[����Ă���A�h���X
								//			�t�@�C���̏ꍇ�FDARC_HEAD_VER5�\���� �̃����o�ϐ� DataStartAddress �������A�h���X���A�h���X�O�Ƃ���
								//			�f�B���N�g���̏ꍇ�FDARC_HEAD_VER5�\���� �̃����o�ϐ� DirectoryTableStartAddress �̂������A�h���X���A�h���X�O�Ƃ���
	u32 DataSize ;				// �t�@�C���̃f�[�^�T�C�Y
	u32 PressDataSize ;			// ���k��̃f�[�^�̃T�C�Y( 0xffffffff:���k����Ă��Ȃ� ) ( Ver0x0002 �Œǉ����ꂽ )
} DARC_FILEHEAD_VER5 ;

// �f�B���N�g���i�[���
typedef struct tagDARC_DIRECTORY_VER5
{
	u32 DirectoryAddress ;			// ������ DARC_FILEHEAD_VER5 ���i�[����Ă���A�h���X( DARC_HEAD_VER5 �\���� �̃����o�ϐ� FileTableStartAddress �������A�h���X���A�h���X�O�Ƃ���)
	u32 ParentDirectoryAddress ;	// �e�f�B���N�g���� DARC_DIRECTORY_VER5 ���i�[����Ă���A�h���X( DARC_HEAD_VER5�\���� �̃����o�ϐ� DirectoryTableStartAddress �������A�h���X���A�h���X�O�Ƃ���)
	u32 FileHeadNum ;				// �f�B���N�g�����̃t�@�C���̐�
	u32 FileHeadAddress ;			// �f�B���N�g�����̃t�@�C���̃w�b�_�񂪊i�[����Ă���A�h���X( DARC_HEAD_VER5�\���� �̃����o�ϐ� FileTableStartAddress �������A�h���X���A�h���X�O�Ƃ���) 
} DARC_DIRECTORY_VER5 ;

#pragma pack(pop)

// class ----------------------------------------

// �A�[�J�C�u�N���X
class DXArchive_VER5
{
public :
	// ���t�̔�r����
	enum DATE_RESULT
	{
		DATE_RESULT_LEFTNEW = 0,		// ���������V����
		DATE_RESULT_RIGHTNEW,			// ���������V����
		DATE_RESULT_DRAW,				// ���t�͓���
	} ;

	DXArchive_VER5( char *ArchivePath = NULL ) ;
	~DXArchive_VER5() ;

	static int			EncodeArchive( char *OutputFileName, char **FileOrDirectoryPath, int FileNum, bool Press = false, const char *KeyString = NULL ) ;	// �A�[�J�C�u�t�@�C�����쐬����
	static int			EncodeArchiveOneDirectory( char *OutputFileName, char *FolderPath, bool Press = false, const char *KeyString = NULL ) ;		// �A�[�J�C�u�t�@�C�����쐬����(�f�B���N�g�������)
	static int			DecodeArchive( char *ArchiveName, char *OutputPath, const char *KeyString = NULL ) ;								// �A�[�J�C�u�t�@�C����W�J����

	int					OpenArchiveFile( const char *ArchivePath, const char *KeyString = NULL ) ;				// �A�[�J�C�u�t�@�C�����J��( 0:����  -1:���s )
	int					OpenArchiveFileMem( const char *ArchivePath, const char *KeyString = NULL ) ;			// �A�[�J�C�u�t�@�C�����J���ŏ��ɂ��ׂă�������ɓǂݍ���ł��珈������( 0:����  -1:���s )
	int					OpenArchiveMem( void *ArchiveImage, int ArchiveSize, const char *KeyString = NULL ) ;	// ��������ɂ���A�[�J�C�u�t�@�C���C���[�W���J��( 0:����  -1:���s )
	int					CloseArchiveFile( void ) ;																// �A�[�J�C�u�t�@�C�������

	int					LoadFileToMem( const char *FilePath, void *Buffer, unsigned int BufferLength ) ;	// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����������ɓǂݍ���( -1:�G���[ 0�ȏ�:�t�@�C���T�C�Y )
	int					GetFileSize( const char *FilePath ) ;										// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����T�C�Y���擾����( -1:�G���[ )
	int					GetFileInfo( const char *FilePath, int *PositionP, int *SizeP ) ;			// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C���̃t�@�C�����̈ʒu�ƃt�@�C���̑傫���𓾂�( -1:�G���[ )
	void				*GetFileImage( void ) ;														// �A�[�J�C�u�t�@�C�����������ɓǂݍ��񂾏ꍇ�̃t�@�C���C���[�W���i�[����Ă���擪�A�h���X���擾����( ����������J���Ă���ꍇ�̂ݗL���A���k���Ă���ꍇ�́A���k���ꂽ��Ԃ̃f�[�^���i�[����Ă���̂Œ��� )
	class DXArchiveFile_VER5 *OpenFile( const char *FilePath ) ;											// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����J���A�t�@�C���A�N�Z�X�p�I�u�W�F�N�g���쐬����( �t�@�C������J���Ă���ꍇ�̂ݗL�� )

	void *				LoadFileToCash( const char *FilePath ) ;									// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����A�N���X���̃L���b�V���o�b�t�@�ɓǂݍ���
	void *				GetCash( void ) ;															// �L���b�V���o�b�t�@�̃A�h���X���擾����
	int					ClearCash( void ) ;															// �L���b�V���o�b�t�@���J������

	int					ChangeCurrentDir( const char *DirPath ) ;									// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
	int					GetCurrentDir( char *DirPathBuffer, int BufferLength ) ;					// �A�[�J�C�u���̃J�����g�f�B���N�g���p�X���擾����



	// �ȉ��͊��Ɠ����Ŏg�p
	static void NotConv( void *Data , int Size ) ;									// �f�[�^�𔽓]������֐�
	static void NotConvFileWrite( void *Data, int Size, FILE *fp ) ;				// �f�[�^�𔽓]�����ăt�@�C���ɏ����o���֐�
	static void NotConvFileRead( void *Data, int Size, FILE *fp ) ;					// �f�[�^�𔽓]�����ăt�@�C������ǂݍ��ފ֐�
	static void KeyCreate( const char *Source, unsigned char *Key ) ;				// ����������쐬
	static void KeyConv( void *Data, int Size, int Position, unsigned char *Key ) ;	// ����������g�p���� Xor ���Z( Key �͕K�� DXA_KEYSTR_LENGTH_VER5 �̒������Ȃ���΂Ȃ�Ȃ� )
	static void KeyConvFileWrite( void *Data, int Size, FILE *fp, unsigned char *Key, int Position = -1 ) ;	// �f�[�^������������g�p���� Xor ���Z������t�@�C���ɏ����o���֐�( Key �͕K�� DXA_KEYSTR_LENGTH_VER5 �̒������Ȃ���΂Ȃ�Ȃ� )
	static void KeyConvFileRead( void *Data, int Size, FILE *fp, unsigned char *Key, int Position = -1 ) ;	// �t�@�C������ǂݍ��񂾃f�[�^������������g�p���� Xor ���Z����֐�( Key �͕K�� DXA_KEYSTR_LENGTH_VER5 �̒������Ȃ���΂Ȃ�Ȃ� )
	static DATE_RESULT DateCmp( DARC_FILETIME_VER5 *date1, DARC_FILETIME_VER5 *date2 ) ;		// �ǂ��炪�V���������r����
	static int Encode( void *Src, unsigned int SrcSize, void *Dest ) ;				// �f�[�^�����k����( �߂�l:���k��̃f�[�^�T�C�Y )
	static int Decode( void *Src, void *Dest ) ;									// �f�[�^���𓀂���( �߂�l:�𓀌�̃f�[�^�T�C�Y )

	DARC_DIRECTORY_VER5 *GetCurrentDirectoryInfo( void ) ;									// �A�[�J�C�u���̃J�����g�f�B���N�g���̏����擾����
	DARC_FILEHEAD_VER5 *GetFileInfo( const char *FilePath ) ;							// �t�@�C���̏��𓾂�
	inline DARC_HEAD_VER5 *GetHeader( void ){ return &Head ; }
	inline u8 *GetKey( void ){ return Key ; }
	inline FILE *GetFilePointer( void ){ return fp ; }
	inline u8 *GetNameP( void ){ return NameP ; }

protected :
	FILE *fp ;							// �A�[�J�C�u�t�@�C���̃|�C���^	
	u8 *HeadBuffer ;					// �w�b�_�[�o�b�t�@�[
	u8 *FileP, *DirP, *NameP ;			// �e��e�[�u��(�t�@�C���w�b�_���e�[�u���A�f�B���N�g�����e�[�u���A���O���e�[�u��)�ւ̃|�C���^
	DARC_DIRECTORY_VER5 *CurrentDirectory ;	// �J�����g�f�B���N�g���f�[�^�ւ̃|�C���^

	void *CashBuffer ;					// �ǂݍ��񂾃t�@�C���f�[�^���ꎞ�I�ɕۑ����Ă����o�b�t�@
	u32 CashBufferSize ;				// �L���b�V���o�b�t�@�Ɋm�ۂ��Ă��郁�����e��
	bool MemoryOpenFlag ;				// ��������̃t�@�C�����J���Ă��邩�A�t���O
	bool UserMemoryImageFlag ;			// ���[�U�[���W�J�����������C���[�W���g�p���Ă��邩�A�t���O
	int MemoryImageSize ;				// ��������̃t�@�C������J���Ă����ꍇ�̃C���[�W�̃T�C�Y
	u8 Key[DXA_KEYSTR_LENGTH_VER5] ;			// ��������

	DARC_HEAD_VER5 Head ;				// �A�[�J�C�u�̃w�b�_

	// �T�C�Y�ۑ��p�\����
	typedef struct tagSIZESAVE
	{
		u32 DataSize ;			// ���f�[�^�̑���
		u32 NameSize ;			// �t�@�C�����f�[�^�̑���
		u32 DirectorySize ;		// �f�B���N�g���f�[�^�̑���
		u32 FileSize ;			// �t�@�C���v���p�e�B�f�[�^�̑���
	} SIZESAVE ;

	// �t�@�C���������p�f�[�^�\����
	typedef struct tagSEARCHDATA
	{
		u8 FileName[1024] ;
		u16 Parity ;
		u16 PackNum ;
	} SEARCHDATA ;

	static int DirectoryEncode( char *DirectoryName, u8 *NameP, u8 *DirP, u8 *FileP, DARC_DIRECTORY_VER5 *ParentDir, SIZESAVE *Size, int DataNumber, FILE *DestP, void *TempBuffer, bool Press, unsigned char *Key ) ;	// �w��̃f�B���N�g���ɂ���t�@�C�����A�[�J�C�u�f�[�^�ɓf���o��
	static int DirectoryDecode( u8 *NameP, u8 *DirP, u8 *FileP, DARC_HEAD_VER5 *Head, DARC_DIRECTORY_VER5 *Dir, FILE *ArcP, unsigned char *Key ) ;											// �w��̃f�B���N�g���f�[�^�ɂ���t�@�C����W�J����
	static int StrICmp( const char *Str1, const char *Str2 ) ;							// ��r�ΏƂ̕����񒆂̑啶�����������Ƃ��Ĉ�����r����( 0:������  1:�Ⴄ )
	static int ConvSearchData( SEARCHDATA *Dest, const char *Src, int *Length ) ;		// ������������p�̃f�[�^�ɕϊ�( �k�������� \ ����������I�� )
	static int AddFileNameData( const char *FileName, u8 *FileNameTable ) ;				// �t�@�C�����f�[�^��ǉ�����( �߂�l�͎g�p�����f�[�^�o�C�g�� )
	static const char *GetOriginalFileName( u8 *FileNameTable ) ;						// �t�@�C�����f�[�^���猳�̃t�@�C�����̕�������擾����
	static int GetDirectoryFilePath( const char *DirectoryPath, char *FilePathBuffer = NULL ) ;	// �f�B���N�g�����̃t�@�C���̃p�X���擾����( FilePathBuffer �͈�t�@�C���ɕt��256�o�C�g�̗e�ʂ��K�v )
	int	ChangeCurrentDirectoryFast( SEARCHDATA *SearchData ) ;							// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
	int	ChangeCurrentDirectoryBase( const char *DirectoryPath, bool ErrorIsDirectoryReset, SEARCHDATA *LastSearchData = NULL ) ;		// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
	int DirectoryKeyConv( DARC_DIRECTORY_VER5 *Dir ) ;										// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )

	// �Q�o�C�g���������ׂ�( TRUE:�Q�o�C�g���� FALSE:�P�o�C�g���� )
	inline static int CheckMultiByteChar( const char *Buf )
	{
		return  ( (unsigned char)*Buf >= 0x81 && (unsigned char)*Buf <= 0x9F ) || ( (unsigned char)*Buf >= 0xE0 && (unsigned char)*Buf <= 0xFC ) ;
	}

	// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C���p�X�ƃf�B���N�g���p�X�𕪊�����
	// �t���p�X�ł���K�v�͖���
	static int GetFilePathAndDirPath( char *Src, char *FilePath, char *DirPath ) ;
} ;


// �A�[�J�C�u���ꂽ�t�@�C���̃A�N�Z�X�p�̃N���X
class DXArchiveFile_VER5
{
protected :
	DARC_FILEHEAD_VER5 *FileData ;		// �t�@�C���f�[�^�ւ̃|�C���^
	DXArchive_VER5 *Archive ;			// �A�[�J�C�u�N���X�ւ̃|�C���^
	void *DataBuffer ;				// �������Ƀf�[�^��W�J�����ۂ̃o�b�t�@�̃|�C���^

	int EOFFlag ;					// EOF�t���O
	u32 FilePoint ;					// �t�@�C���|�C���^

public :
	DXArchiveFile_VER5( DARC_FILEHEAD_VER5 *FileHead, DXArchive_VER5 *Archive ) ;
	~DXArchiveFile_VER5() ;

	int Read( void *Buffer, int ReadLength ) ;					// �t�@�C���̓��e��ǂݍ���
	int Seek( int SeekPoint, int SeekMode ) ;					// �t�@�C���|�C���^��ύX����
	int Tell( void ) ;											// ���݂̃t�@�C���|�C���^�𓾂�
	int Eof( void ) ;											// �t�@�C���̏I�[�ɗ��Ă��邩�A�̃t���O�𓾂�
	int Size( void ) ;											// �t�@�C���̃T�C�Y���擾����

	inline DARC_FILEHEAD_VER5 *GetFileData( void ){ return FileData ; }
} ;

#endif

