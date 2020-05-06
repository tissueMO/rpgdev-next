// ============================================================================
//
//		�t�@�C���֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

#ifndef FILELIB_H
#define FILELIB_H
#pragma warning(disable:4996)

// include --------------------------------------
#include <stdio.h>
#include <tchar.h>
#include "DataType.h"

// define ---------------------------------------

// data type ------------------------------------

// �t�@�C���̓��t�f�[�^
typedef struct tagFILE_DATE
{
	u64 Create ;			// �쐬����
	u64 LastAccess ;		// �ŏI�A�N�Z�X����
	u64 LastWrite ;			// �ŏI�X�V����
} FILE_DATE ;

// �t�@�C�����
typedef struct tagFILE_INFO
{
	TCHAR *FileName ;			// �t�@�C����
	u32 FileNameHash;			// �t�@�C������ CRC32 �̃n�b�V���l
	TCHAR *RelDirectoryPath ;	// �񋓏������̃J�����g�f�B���N�g������̑��΃f�B���N�g���p�X( ����� '\' �������Ă��� )
	TCHAR *AbsDirectoryPath ;	// ��΃f�B���N�g���p�X( ����� '\' �������Ă��� )
	u64 Size ;					// �t�@�C���̃T�C�Y(�f�B���N�g���̏ꍇ�͉��������Ă��Ȃ�)
	u32 IsDirectory ;			// �f�B���N�g�����ۂ�
	FILE_DATE Date ;			// ���t�f�[�^
	u32 Attributes ;			// �t�@�C������
} FILE_INFO ;

// �t�@�C����񃊃X�g
typedef struct tagFILE_INFOLIST
{
	int Num ;					// �f�B���N�g�����̃t�@�C���̐�
	FILE_INFO  *List ;			// �f�B���N�g�����̃t�@�C���̏����i�[�����z��ւ̃|�C���^
} FILE_INFOLIST ;

// data -----------------------------------------

// function proto type --------------------------

extern void EasyEncode( void *Data , unsigned int Size ) ;					// �f�[�^���ȈՈÍ�������֐�
extern void EasyEncodeFileWrite( void *Data, int Size, FILE *fp ) ;			// �f�[�^���ȈՈÍ������ăt�@�C���ɏ����o���֐�
extern void EasyEncodeFileRead( void *Data, int Size, FILE *fp ) ;			// �f�[�^���ȈՈÍ������ăt�@�C������ǂݍ��ފ֐�

extern unsigned int BinToChar128( void *Src, unsigned int SrcSize, void *Dest ) ;	// �o�C�i���f�[�^�𔼊p������ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern unsigned int Char128ToBin( void *Src, void *Dest ) ;							// ���p��������o�C�i���f�[�^�ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )

extern unsigned int BinToBase64( void *Src, unsigned int SrcSize, void *Dest ) ;	// �o�C�i���f�[�^��Base64������ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern unsigned int Base64ToBin( void *Src, void *Dest ) ;							// Base64��������o�C�i���f�[�^�ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )

extern int LoadFileMem( const TCHAR *Path, void **DataBuf, size_t *Size ) ;		// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int LoadFileMem( const TCHAR *Path, void *DataBuf,  size_t *Size ) ;		// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int SaveFileMem( const TCHAR *Path, void *Data,     size_t  Size ) ;		// �������̓��e���t�@�C���ɏ����o�� 

// �w��̃f�B���N�g�����쐬����A���Ԃ̃f�B���N�g�������݂��Ȃ��ꍇ�͍쐬����
// �Ō���� '\' �������Ă���������
// �h���C�u���̌�� '\' ���Ȃ��ꍇ�͐���ɓ��삵�Ȃ�
extern int __CreateDirectory( const TCHAR *Path ) ;

// �w��̃p�X�������Ă�����̂��f�B���N�g�����ǂ����𓾂�( 1:�f�B���N�g��  0:�t�@�C�� )
extern int IsDirectory( const TCHAR *Path ) ;

extern int CreateFileInfo( const TCHAR *Path, FILE_INFO *FileInfoBuffer ) ;	// �w��̃p�X�̏��𓾂�(-1:�G���[���� 0:����I��)
extern int ReleaseFileInfo( FILE_INFO *FileInfo ) ;							// �p�X���̌�n�����s��
extern int SetFileTimeStamp( const TCHAR *Path, FILE_INFO *FileInfo ) ;		// �w��̃p�X�̃t�@�C���̃^�C���X�^���v�� FileInfo �Ɋi�[����Ă���^�C���X�^���v�ɂ���
extern int CmpFileTimeStamp( FILE_INFO *FileInfo1, FILE_INFO *FileInfo2, bool Create = true, bool LastAccess = true, bool LastWrite = true );	// ��̃t�@�C���̃^�C���X�^���v����v���Ă��邩�ǂ����𓾂�(0:��v���Ă��� -1:��v���Ă��Ȃ�)

// �w��̃f�B���N�g���̃t�@�C�����X�g���擾����
// �p�X������̍Ō�Ɂw\�x�������Ă���ɍŌ�̕�������f�B���N�g���Ɣ��f����
// �f�B���N�g���ł͂Ȃ������ꍇ���������Ԃ�
// �t���p�X�ł͂Ȃ��ꍇ�͌��݂̃J�����g�f�B���N�g������̑��΃p�X�ƂȂ�
// FileListInfo �� 0 �ł͂Ȃ��ꍇ�g�p�� ReleaseFileObjectList ���ĂԕK�v������
// SubDirectory �� 1 �ɂ���ƃT�u�f�B���N�g�����̃t�@�C�����S�ė񋓂���
// NULL �A�������͕���������Ȃ��������n�����ꍇ�J�����g�f�B���N�g���̃t�@�C�����񋓂����
// �߂�l : -1 = �G���[  0�ȏ� = �񋓂����t�@�C���̐�
// OmitDirectory �� 1 �ɂ���ƃf�B���N�g���͗񋓂��Ȃ�
// OmitName �ɏ��O�������t�@�C������n���Ƃ��̕���������O���Ă����( ';'�ŕ����̕������A���\ )
// OmitExName �ɏ��O�������g���q��n���Ƃ��̊g���q�����t�@�C�������O���Ă����( ';'�ŕ����̕������A���\ )
// ValidExName �ɗL���ɂ������g���q��n���Ƃ��̊g���q�����t�@�C���̂ݗ񋓂��Ă����( Omit�n�����D��x������, ';'�ŕ����̕������A���\ )
// EnumFileCallback �� 1�t�@�C�����ƂɌĂ΂��R�[���o�b�N�֐��APhase �� 0=�t�@�C�����񋓒� 1=�t�@�C�����擾��
extern int CreateFileList( const TCHAR *DirectoryPath, FILE_INFOLIST *FileListInfo = 0,
							int OmitDirectory = 0, int SubDirectory = 0,
							const TCHAR *OmitName = 0, const TCHAR *OmitExName = 0,
							const TCHAR *ValidExName = 0, void ( *EnumFileCallback )( int Phase, int NowFileNum, int TotalFileNum, const TCHAR *FileName, const TCHAR *RelDirPath, const TCHAR *AbsDirPath ) = 0 ) ;
extern int ReleaseFileList( FILE_INFOLIST *DirectoryInfo ) ;

// ����̃p�X�����񂩂�F�X�ȏ����擾����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int AnalyseFilePath(
	const TCHAR *Src, // DirPath �̏I�[�ɂ� \ �͕t���Ȃ�
	TCHAR *FullPath, TCHAR *DirPath, TCHAR *FileName, TCHAR *Name, TCHAR *ExeName, const TCHAR *CurrentDir = 0 );

// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int ConvertFullPath__( const TCHAR *Src, TCHAR *Dest, const TCHAR *CurrentDir = 0 ) ;

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
// DirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
extern int AnalysisFileNameAndDirPath( const TCHAR *Src, TCHAR *FileName = 0, TCHAR *DirPath = 0 ) ;

// �t�@�C���p�X����t�@�C�����Ɗg���q���擾����
extern int AnalysisFileNameAndExeName( const TCHAR *Src, TCHAR *Name = 0, TCHAR *ExeName = 0 ) ;

// �t�@�C���p�X�̊g���q��ς���������𓾂�
extern int GetChangeExeNamePath( const TCHAR *Src, TCHAR *Dest, const TCHAR *ExeName ) ;

extern void SetEnMark( TCHAR *PathBuf ) ;			// ����Ɂw\�x�����Ă��Ȃ��ꍇ�͕t����
extern void SetChr( TCHAR *PathBuf, TCHAR chr ) ;		// ����Ɏw��̕������Ȃ��ꍇ�͂���
extern void DelChr( TCHAR *PathBuf, TCHAR chr ) ;		// ����Ɏw��̕���������ꍇ�͍폜����

extern int GetExName( const TCHAR *Path, TCHAR *ExNameBuf ) ;						// �g���q�𓾂�
extern int SetExName( const TCHAR *Path, const TCHAR *ExName, TCHAR *DestBuf ) ;	// �g���q��ύX����

extern int CheckTextData( void *buffer, int size ) ;		// �e�L�X�g�f�[�^���ǂ����𔻒肷��( �V�t�gJIS�t�@�C���^�C�v�̂ݑΉ� )( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )
extern int CheckTextFile( const TCHAR *Path ) ;				// �e�L�X�g�t�@�C�����ǂ����𔻒肷��( �V�t�gJIS�t�@�C���^�C�v�̂ݑΉ� )( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )

// �Q�o�C�g���������ׂ�( TRUE:�Q�o�C�g���� FALSE:�P�o�C�g���� ) 
extern int CheckMultiByteChar( char *Buf ) ;

extern u32 FileLib_HashCRC32( const void *SrcData, size_t SrcDataSize ) ;		// CRC32 �̃n�b�V���l���擾����

#endif
