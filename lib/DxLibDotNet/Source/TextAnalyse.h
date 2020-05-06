#ifndef TEXTANALYSE_H
#define TEXTANALYSE_H

//#define USE_DXLIB
#include "stdlib.h"

//�������͗p�N���X
class TextAnalyse
{
public:
	TextAnalyse();
	~TextAnalyse();

	bool load( const char *path );						//�e�L�X�g�t�@�C���̓ǂݍ���
	bool setmem( const char *string, int size = -1 );	//��������̃e�L�X�g�̉�͂�ݒ�

	bool release( void );							//�ǂݍ��񂾃e�L�X�g�t�@�C�����J��
	void reset( void );								//��͈ʒu��擪�ɖ߂�
	bool move( const char *pos );					//�w��̍��W�Ɉړ�����
	char *getpos( void );							//���݂̍��W���擾����
	bool search( const char *str );					//�w��̕����񂪂���ʒu�̂P�o�C�g��܂ňړ�����(�����ꍇ�͈ړ����Ȃ�)
	char *check( const char *str );					//�w��̕����񂪂���ʒu�̂P�o�C�g��̍��W���擾����(�����ꍇ��0)
	bool getint( int *buf );						//���̐��l���擾
	bool getint16( int *buf );						//���̐��l���P�U�i���Ƃ��Ď擾
	bool getfloat( float *buf );					//���̐��l�𕂓������_���Ƃ��Ď擾
	bool getstr( char *buf, bool nomove = false );	//���̕�������擾

	bool next( void );								//�Ӗ��̂���P��܂Ŕ��( true:�Ӗ��̂���P�ꂪ������  false:�Ӗ��̂���P�ꂪ�������� )
	char *nextpos( void );							//���ɈӖ��̂���P��̃|�C���^���擾����(���W�͈Ӗ��̂���P��̍ŏ��̕����Ɉړ�����)
	char nextchar( void );							//���ɈӖ��̂���P��̍ŏ��̕������擾����(���W�͈Ӗ��̂���P��̍ŏ��̕����Ɉړ�����)( -1:�t�@�C���̏I�[ )
	const char *nextstr();							//���ɈӖ��̂���P��(�t�@�C���̏I�[�ɗ��Ă���ꍇ�̓k�������݂̂̕����񂪕Ԃ�)
	bool nextline( void );							//���̍s�Ɉړ�����
	bool skipgroup( const char *groupPre, const char *groupPost );	//�O���[�v���X�L�b�v����
	void comment_skip_enable( bool flag );			//�R�����g���X�L�b�v���邩�ǂ�����ݒ肷��(true:�X�L�b�v���� false:�X�L�b�v���Ȃ�)
	void setaddskip( const char *str );				//�ǉ��Ӗ��Ȃ�������ݒ肷��
	void setnotskip_space_tab( bool flag );			//�X�y�[�X�ƃ^�u���X�L�b�v���Ȃ����ǂ���(true:�X�L�b�v���Ȃ� false:�X�L�b�v����)

	static int checksize( const char *str );		//�����̃o�C�g���𓾂�

protected:
	bool enable;			//�L���ȃf�[�^�������Ԃ�
	bool mem;				//��������̃e�L�X�g����͂��Ă��邩
	bool commentNoSkip;		//�R�����g���X�L�b�v���Ȃ����ǂ���( true:���Ȃ� false:���� )
	char *buffer;			//������o�b�t�@�̐擪
	char *last;				//������o�b�t�@�̏I�[�̂P�o�C�g��
	int strsize;			//������̃T�C�Y
	char *pos;				//���݂̈ʒu
	char addskip[16];		//�ǉ��Ӗ��Ȃ�����
	bool notskip_space_tab;	//�X�y�[�X�ƃ^�u���X�L�b�v���Ȃ����ǂ���(true:�X�L�b�v���Ȃ� false:�X�L�b�v����)

	bool checkC( char str );		//�Ӗ��̂��镶�������ׂ�(true:�Ӗ������镶��)
};

//Unicode�������͗p�N���X
class TextAnalyseW
{
public:
	TextAnalyseW();
	~TextAnalyseW();

	static bool checkUnicode( const char *path );			//�w��̃e�L�X�g�t�@�C�������j�R�[�h���ǂ����𔻒肷��(true:���j�R�[�h  false:�}���`�o�C�g)
	static bool checkUnicodeMem( void *string );			//�w��̃�������̃e�L�X�g�����j�R�[�h���ǂ����𔻒肷��(true:���j�R�[�h  false:�}���`�o�C�g)

	bool load( const char *path );							//�e�L�X�g�t�@�C���̓ǂݍ���
	bool setmem( const wchar_t *string, int size = -1 );	//��������̃e�L�X�g�̉�͂�ݒ�(size �̓o�C�g��)

	bool release( void );									//�ǂݍ��񂾃e�L�X�g�t�@�C�����J��
	void reset( void );										//��͈ʒu��擪�ɖ߂�
	bool move( const wchar_t *pos );						//�w��̍��W�Ɉړ�����
	wchar_t *getpos( void );								//���݂̍��W���擾����
	bool search( const wchar_t *str );						//�w��̕����񂪂���ʒu�̂P�o�C�g��܂ňړ�����(�����ꍇ�͈ړ����Ȃ�)
	wchar_t *check( const wchar_t *str );					//�w��̕����񂪂���ʒu�̂P�o�C�g��̍��W���擾����(�����ꍇ��0)
	bool getint( int *buf );								//���̐��l���擾
	bool getint16( int *buf );								//���̐��l���P�U�i���Ƃ��Ď擾
	bool getstr( wchar_t *buf, bool nomove = false );		//���̕�������擾

	bool next( void );										//�Ӗ��̂���P��܂Ŕ��
	wchar_t *nextpos( void );								//���ɈӖ��̂���P��̃|�C���^���擾����(���W�͈Ӗ��̂���P��̍ŏ��̕����Ɉړ�����)
	wchar_t nextchar( void );								//���ɈӖ��̂���P��̍ŏ��̕������擾����(���W�͈Ӗ��̂���P��̍ŏ��̕����Ɉړ�����)
	const wchar_t *nextstr();								//���ɈӖ��̂���P��(�t�@�C���̏I�[�ɗ��Ă���ꍇ�̓k�������݂̂̕����񂪕Ԃ�)
	bool nextline( void );									//���̍s�Ɉړ�����
	bool skipgroup( const wchar_t *groupPre, const wchar_t *groupPost );	//�O���[�v���X�L�b�v����
	void comment_skip_enable( bool flag );					//�R�����g���X�L�b�v���邩�ǂ�����ݒ肷��
	void setaddskip( const wchar_t *str );					//�ǉ��Ӗ��Ȃ�������ݒ肷��
	void setnotskip_space_tab( bool flag );					//�X�y�[�X�ƃ^�u���X�L�b�v���Ȃ����ǂ���(true:�X�L�b�v���Ȃ� false:�X�L�b�v����)

protected:
	bool enable;		//�L���ȃf�[�^�������Ԃ�
	bool mem;			//��������̃e�L�X�g����͂��Ă��邩
	bool commentNoSkip;	//�R�����g���X�L�b�v���Ȃ����ǂ���( true:���Ȃ� false:���� )
	wchar_t *buffer;	//������o�b�t�@�̐擪
	wchar_t *last;		//������o�b�t�@�̏I�[�̂P�o�C�g��
	int strsize;		//������̃T�C�Y
	wchar_t *pos;		//���݂̈ʒu
	wchar_t addskip[16];		//�ǉ��Ӗ��Ȃ�����
	bool notskip_space_tab;	//�X�y�[�X�ƃ^�u���X�L�b�v���Ȃ����ǂ���(true:�X�L�b�v���Ȃ� false:�X�L�b�v����)

	bool checkC( wchar_t str );		//�Ӗ��̂��镶�������ׂ�(true:�Ӗ������镶��)
};

#endif
