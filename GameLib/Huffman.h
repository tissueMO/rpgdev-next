// ============================================================================
//
//	Huffman���k���C�u����
//
//	Huffman.h
//
//	Creator			: �R�c�@�I
//	Creation Date	: 2018/12/16
//
// ============================================================================

#ifndef HUFFMAN_H
#define HUFFMAN_H
#pragma warning(disable:4996)

// define ---------------------------------------

#ifndef u64
#define u64		unsigned long long
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

#ifndef s32
#define s32		signed int
#endif

#ifndef f32
#define f32		float
#endif

#ifndef NULL
#define NULL	(0)
#endif

// proto type -----------------------------------

// �f�[�^�����k
// �߂�l:���k��̃T�C�Y  0 �̓G���[  Dest �� NULL ������ƈ��k�f�[�^�i�[�ɕK�v�ȃT�C�Y���Ԃ�
extern u64 Huffman_Encode( void *Src, u64 SrcSize, void *Dest ) ;

// ���k�f�[�^����
// �߂�l:�𓀌�̃T�C�Y  0 �̓G���[  Dest �� NULL ������Ɖ𓀃f�[�^�i�[�ɕK�v�ȃT�C�Y���Ԃ�
extern u64 Huffman_Decode( void *Press, void *Dest ) ;

#endif // HUFFMAN_H