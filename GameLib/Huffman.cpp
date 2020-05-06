// ============================================================================
//
//	Huffman���k���C�u����
//
//	Huffman.cpp
//
//	Creator			: �R�c�@�I
//	Creation Date	: 2018/12/16
//
// ============================================================================

// include --------------------------------------
#include "Huffman.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

// data type ------------------------------------

// ���l���Ƃ̏o������Z�o���ꂽ�G���R�[�h��̃r�b�g���A���������̏�񓙂̍\����
struct HUFFMAN_NODE
{
    u64 Weight ;                  // �o����( �����f�[�^�ł͏o�����𑫂������m )
    int BitNum ;                  // ���k��̃r�b�g��̃r�b�g��( �����f�[�^�ł͎g��Ȃ� )
    unsigned char BitArray[32] ;  // ���k��̃r�b�g��( �����f�[�^�ł͎g��Ȃ� )
    int Index ;                   // �����f�[�^�Ɋ��蓖�Ă�ꂽ�Q�ƃC���f�b�N�X( 0 or 1 )

    int ParentNode ;              // ���̃f�[�^���]���Ă��錋���f�[�^�̗v�f�z��̃C���f�b�N�X
    int ChildNode[2] ;            // ���̃f�[�^�������������Q�v�f�̗v�f�z��C���f�b�N�X( �����f�[�^�ł͂Ȃ��ꍇ�͂ǂ���� -1 )
} ;

// �r�b�g�P�ʓ��o�͗p�f�[�^�\����
struct BIT_STREAM
{
	u8 *Buffer ;
	u64 Bytes ;
	u8 Bits ;
} ;

// ���k�f�[�^�̏��
//   6bit      ���k�O�̃f�[�^�̃T�C�Y�̃r�b�g��(A) - 1( 0=1�r�b�g 63=64�r�b�g )
//   (A)bit    ���k�O�̃f�[�^�̃T�C�Y
//   6bit      ���k��̃f�[�^�̃T�C�Y�̃r�b�g��(B) - 1( �w�b�_�����܂܂Ȃ� )( 0=1�r�b�g 63=64�r�b�g )
//   (B)bit    ���k��̃f�[�^�̃T�C�Y
//
//   3bit      ���l���Ƃ̏o���p�x�̍����l�̃r�b�g��(C) / 2 - 1( 0=2�r�b�g 7=16�r�b�g )
//   1bit      �����r�b�g( 0=�v���X  1=�}�C�i�X )
//   (C)bit    ���l���Ƃ̏o���p�x�̍����l
//   �����ꂪ256����

// proto type -----------------------------------

static void BitStream_Init(  BIT_STREAM *BitStream, void *Buffer, bool IsRead ) ;		// �r�b�g�P�ʓ��o�͂̏�����
static void BitStream_Write( BIT_STREAM *BitStream, u8 BitNum, u64 OutputData ) ;		// �r�b�g�P�ʂ̐��l�̏������݂��s��
static u64  BitStream_Read(  BIT_STREAM *BitStream, u8 BitNum ) ;						// �r�b�g�P�ʂ̐��l�̓ǂݍ��݂��s��
static u8   BitStream_GetBitNum( u64 Data ) ;											// �w��̐��l�̃r�b�g�����擾����
static u64  BitStream_GetBytes( BIT_STREAM *BitStream ) ;								// �r�b�g�P�ʂ̓��o�̓f�[�^�̃T�C�Y( �o�C�g�� )���擾����

// code -----------------------------------------

// �r�b�g�P�ʓ��o�͂̏�����
void BitStream_Init( BIT_STREAM *BitStream, void *Buffer, bool IsRead )
{
	BitStream->Buffer = ( u8 * )Buffer ;
	BitStream->Bytes = 0 ;
	BitStream->Bits = 0 ;
	if( IsRead == false )
	{
		BitStream->Buffer[ 0 ] = 0 ;
	}
}

// �r�b�g�P�ʂ̐��l�̏������݂��s��
void BitStream_Write( BIT_STREAM *BitStream, u8 BitNum, u64 OutputData )
{
	u32 i ;
	for( i = 0 ; i < BitNum ; i ++ )
	{
		BitStream->Buffer[ BitStream->Bytes ] |= ( ( OutputData >> ( BitNum - 1 - i ) ) & 1 ) << ( 7 - BitStream->Bits ) ;
		BitStream->Bits ++ ;
		if( BitStream->Bits == 8 )
		{
			BitStream->Bytes ++ ;
			BitStream->Bits = 0 ;
			BitStream->Buffer[ BitStream->Bytes ] = 0 ;
		}
	}
}

// �r�b�g�P�ʂ̐��l�̓ǂݍ��݂��s��
u64 BitStream_Read( BIT_STREAM *BitStream, u8 BitNum )
{
	u64 Result = 0 ;
	u32 i ;
	for( i = 0 ; i < BitNum ; i ++ )
	{
		Result |= ( ( u64 )( ( BitStream->Buffer[ BitStream->Bytes ] >> ( 7 - BitStream->Bits ) ) & 1 ) ) << ( BitNum - 1 - i ) ;
		BitStream->Bits ++ ;
		if( BitStream->Bits == 8 )
		{
			BitStream->Bytes ++ ;
			BitStream->Bits = 0 ;
		}
	}

	return Result ;
}

// �w��̐��l�̃r�b�g�����擾����
u8 BitStream_GetBitNum( u64 Data )
{
	u32 i ;
	for( i = 1 ; i < 64 ; i ++ )
	{
		if( Data < ( 1ULL << i ) )
		{
			return i ;
		}
	}

	return i ;
}

// �r�b�g�P�ʂ̓��o�̓f�[�^�̃T�C�Y( �o�C�g�� )���擾����
u64 BitStream_GetBytes( BIT_STREAM *BitStream )
{
	return BitStream->Bytes + ( BitStream->Bits != 0 ? 1 : 0 ) ;
}

// �f�[�^�����k
//
// �߂�l:���k��̃T�C�Y  0 �̓G���[  Dest �� NULL ������ƈ��k�f�[�^�i�[�ɕK�v�ȃT�C�Y���Ԃ�
u64 Huffman_Encode( void *Src, u64 SrcSize, void *Dest )
{
    // �����f�[�^�Ɛ��l�f�[�^�A�O�`�Q�T�T�܂ł����l�f�[�^
    // (�����f�[�^�̐��ƈ��k����f�[�^�̎�ނ̐��𑫂��ƕK���w��ނ̐��{(��ނ̐��|�P)�x�ɂȂ�B
    // �w�z���g���H�x�Ǝv������̓n�t�}�����k�̐����ŏo�Ă����`,�a,�b,�c,�d�̌��������̐���
    // �����Ă݂ĉ������A��ނ��T�ɑ΂��Č��������͈���Ȃ��S�ɂȂ��Ă���͂��ł��B
    // ��ނ��U�̎��͌��������͂T�ɁA�����Ď�ނ��Q�T�U���̎��͌��������͂Q�T�T�ɂȂ�܂�)
    HUFFMAN_NODE Node[256 + 255] ;

    unsigned char *SrcPoint ;
    u64 PressBitCounter, PressSizeCounter, SrcSizeCounter ;
    u64 i ;

    // void �^�̃|�C���^�ł̓A�h���X�̑��삪�o���Ȃ��̂� unsigned char �^�̃|�C���^�ɂ���
    SrcPoint = ( unsigned char * )Src ;

    // �e���l�̈��k��̃r�b�g����Z�o����
    {
        int NodeIndex, MinNode1, MinNode2 ;
        int NodeNum, DataNum ;

        // ���l�f�[�^������������
        for( i = 0 ; i < 256 ; i ++ )
        {
            Node[i].Weight = 0 ;           // �o�����͂��ꂩ��Z�o����̂łO�ɏ�����
            Node[i].ChildNode[0] = -1 ;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
            Node[i].ChildNode[1] = -1 ;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
            Node[i].ParentNode = -1 ;      // �܂��ǂ̗v�f�Ƃ���������Ă��Ȃ��̂� -1 ���Z�b�g����
        }

        // �e���l�̏o�������J�E���g
        for( i = 0 ; i < SrcSize ; i ++ )
        {
            Node[ SrcPoint[i] ].Weight ++ ;
        }

		// �o������ 0�`65535 �̔䗦�ɕϊ�����
		for( i = 0 ; i < 256 ; i ++ )
		{
			Node[ i ].Weight = Node[ i ].Weight * 0xffff / SrcSize ;
		}

        // �o�����̏��Ȃ����l�f�[�^ or �����f�[�^���q����
        // �V���������f�[�^���쐬�A�S�Ă̗v�f���q���Ŏc��P�ɂȂ�܂ŌJ��Ԃ�
        DataNum = 256 ; // �c��v�f��
        NodeNum = 256 ; // ���ɐV������錋���f�[�^�̗v�f�z��̃C���f�b�N�X
        while( DataNum > 1 )
        {
            // �o�����l�̒Ⴂ�v�f���T��
            {
                MinNode1 = -1 ;
                MinNode2 = -1 ;
                
                // �c���Ă���v�f�S�Ă𒲂ׂ�܂Ń��[�v
                NodeIndex = 0 ;
                for( i = 0 ; i < DataNum ; NodeIndex ++ )
                {
                    // �������ɉ������̗v�f�ƌ�������Ă���ꍇ�͑ΏۊO
                    if( Node[NodeIndex].ParentNode != -1 ) continue ;
                    
                    i ++ ;
                    
                    // �܂��L���ȗv�f���Z�b�g���Ă��Ȃ����A���o�����l��
                    // ���Ȃ��v�f������������X�V
                    if( MinNode1 == -1 || Node[MinNode1].Weight > Node[NodeIndex].Weight )
                    {
                        // ���܂ň�ԏo�����l�����Ȃ������Ǝv��ꂽ
                        // �v�f�͓�Ԗڂɍ~�i
                        MinNode2 = MinNode1 ;

                        // �V������Ԃ̗v�f�̗v�f�z��̃C���f�b�N�X��ۑ�
                        MinNode1 = NodeIndex ;
                    }
                    else
                    {
                        // ��Ԃ��͏o�����l�������Ă��A��Ԗڂ��͏o�����l��
                        // ���Ȃ���������Ȃ��̂ňꉞ�`�F�b�N(���͓�Ԗڂɏo�����l��
                        // ���Ȃ��v�f���Z�b�g����Ă��Ȃ������ꍇ���Z�b�g)
                        if( MinNode2 == -1 || Node[MinNode2].Weight > Node[NodeIndex].Weight )
                        {
                            MinNode2 = NodeIndex ;
                        }
                    }
                }
            }
            
            // ��̗v�f���q���ŐV�����v�f(�����f�[�^)�����
            Node[NodeNum].ParentNode = -1 ;  // �V�����f�[�^�͓��R�܂������Ƃ��q�����Ă��Ȃ��̂� -1 
            Node[NodeNum].Weight = Node[MinNode1].Weight + Node[MinNode2].Weight ;    // �o�����l�͓�̐��l�𑫂������̂��Z�b�g����
            Node[NodeNum].ChildNode[0] = MinNode1 ;    // ���̌������� 0 ��I�񂾂�o�����l����ԏ��Ȃ��v�f�Ɍq����
            Node[NodeNum].ChildNode[1] = MinNode2 ;    // ���̌������� 1 ��I�񂾂�o�����l����Ԗڂɏ��Ȃ��v�f�Ɍq����

            // �������ꂽ�v�f��ɁA�����B�ɉ��̒l�����蓖�Ă�ꂽ�����Z�b�g����
            Node[MinNode1].Index = 0 ;    // ��ԏo�����l�����Ȃ��v�f�� 0 ��
            Node[MinNode2].Index = 1 ;    // ��Ԗڂɏo�����l�����Ȃ��v�f�� 1 ��

            // �������ꂽ�v�f��ɁA�����B���������������f�[�^�̗v�f�z��C���f�b�N�X���Z�b�g����
            Node[MinNode1].ParentNode = NodeNum ;
            Node[MinNode2].ParentNode = NodeNum ;

            // �v�f�̐�������₷
            NodeNum ++ ;

            // �c��v�f�̐��́A��v�f���V�����ǉ����ꂽ�����
            // ��̗v�f����������Č����̑Ώۂ���O�ꂽ�̂�
            // ���� 1 - 2 �� -1 
            DataNum -- ;
        }
        
        // �e���l�̈��k��̃r�b�g�������o��
        {
            unsigned char TempBitArray[32] ;
            int TempBitIndex, TempBitCount, BitIndex, BitCount ;
        
            // ���l�f�[�^�̎�ނ̐������J��Ԃ�
            for( i = 0 ; i < 256 ; i ++ )
            {
                // ���l�f�[�^���猋���f�[�^����֏�ւƒH���ăr�b�g���𐔂���
                {
                    // �r�b�g�������������Ă���
                    Node[i].BitNum = 0 ;
                    
                    // �ꎞ�I�ɐ��l�f�[�^����k���Ă������Ƃ��̃r�b�g���ۑ����鏈���̏���
                    TempBitIndex = 0 ;
                    TempBitCount = 0 ;
                    TempBitArray[TempBitIndex] = 0 ;
                    
                    // �������ƌ�������Ă������J�E���g��������(�V�ӂ͉����Ƃ���������Ă��Ȃ��̂ŏI�_���ƕ�����)
                    for( NodeIndex = ( int )i ; Node[NodeIndex].ParentNode != -1 ; NodeIndex = Node[NodeIndex].ParentNode )
                    {
                        // �z��v�f��ɓ���r�b�g�f�[�^�͂W�Ȃ̂ŁA�����z��v�f��
                        // ���ɂW�ۑ����Ă����玟�̔z��v�f�ɕۑ����ύX����
                        if( TempBitCount == 8 )
                        {
                            TempBitCount = 0 ;
                            TempBitIndex ++ ;
                            TempBitArray[TempBitIndex] = 0 ;
                        }
                        
                        // �V�����������ޏ��ō��܂ł̃f�[�^���㏑�����Ă��܂�Ȃ��悤�ɂP�r�b�g���ɃV�t�g����
                        TempBitArray[TempBitIndex] <<= 1 ;

                        // �����f�[�^�Ɋ���U��ꂽ�C���f�b�N�X���ŉ��ʃr�b�g(��ԉE���̃r�b�g)�ɏ�������
                        TempBitArray[TempBitIndex] |= (unsigned char)Node[NodeIndex].Index ;

                        // �ۑ������r�b�g���𑝂₷
                        TempBitCount ++ ;

                        // �r�b�g���𑝂₷
                        Node[i].BitNum ++ ;
                    }
                }
				
                // TempBitArray �ɗ��܂����f�[�^�͐��l�f�[�^���猋���f�[�^��V�ӂɌ�������
                // ��֏�ւƑk���Ă��������̃r�b�g��Ȃ̂ŁA�t���܂ɂ��Ȃ��ƈ��k��̃r�b�g
                // �z��Ƃ��Ďg���Ȃ�(�W�J���ɓV�ӂ̌����f�[�^���琔�l�f�[�^�܂ŒH�邱�Ƃ�
                // �o���Ȃ�)�̂ŁA�������t���܂ɂ������̂𐔒l�f�[�^���̃r�b�g��o�b�t�@�ɕۑ�����
                {
                    BitCount = 0 ;
                    BitIndex = 0 ;
                    
                    // �ŏ��̃o�b�t�@�����������Ă���
                    // (�S�� �_���a(or)���Z �ŏ������ނ̂ŁA�ŏ�����P�ɂȂ��Ă���
                    // �r�b�g�ɂO����������ł��P�̂܂܂ɂȂ��Ă��܂�����)
                    Node[i].BitArray[BitIndex] = 0 ;
                    
                    // �ꎞ�I�ɕۑ����Ă������r�b�g��̍ŏ��܂ők��
                    while( TempBitIndex >= 0 )
                    {
                        // �������񂾃r�b�g������̔z��v�f�ɓ���W�r�b�g��
                        // �B���Ă��܂����玟�̔z��v�f�Ɉڂ�
                        if( BitCount == 8 )
                        {
                            BitCount = 0 ;
                            BitIndex ++ ;
                            Node[i].BitArray[BitIndex] = 0 ;
                        }

                        // �܂������������܂�Ă��Ȃ��r�b�g�A�h���X�ɂP�r�b�g��������
                        Node[i].BitArray[BitIndex] |= (unsigned char)( ( TempBitArray[TempBitIndex] & 1 ) << BitCount ) ;
						
                        // �������ݏI������r�b�g�͂�������Ȃ��̂Ŏ��̃r�b�g��
                        // �������߂�悤�ɂP�r�b�g�E�ɃV�t�g����
                        TempBitArray[TempBitIndex] >>= 1 ;
                        
                        // �P�r�b�g�������񂾂̂Ŏc��r�b�g�����P���炷
                        TempBitCount -- ;
                        
                        // �������ݏ������݌��ƂȂ��Ă���z��v�f�ɏ�������ł��Ȃ�
                        // �r�b�g��񂪖����Ȃ����玟�̔z��v�f�Ɉڂ�
                        if( TempBitCount == 0 )
                        {
                            TempBitIndex -- ;
                            TempBitCount = 8 ;
                        }
                        
                        // �������񂾃r�b�g���𑝂₷
                        BitCount ++ ;
                    }
                }
            }
        }
    }

    // �ϊ�����
    {
        unsigned char *PressData ;
        int BitData, BitCounter, BitIndex, BitNum, NodeIndex ;
        
        // ���k�f�[�^���i�[����A�h���X���Z�b�g
        // (���k�f�[�^�{�̂͌��̃T�C�Y�A���k��̃T�C�Y�A�e���l�̏o��������
        // �i�[����f�[�^�̈�̌�Ɋi�[����)
        PressData = ( unsigned char * )Dest ;
        
        // ���k����f�[�^�̎Q�ƃA�h���X��������
        SrcSizeCounter = 0 ;
        
        // ���k�����f�[�^�̎Q�ƃA�h���X��������
        PressSizeCounter = 0 ;
        
        // ���k�����r�b�g�f�[�^�̃J�E���^��������
        PressBitCounter = 0 ;
        
        // ���k�f�[�^�̍ŏ��̃o�C�g�����������Ă���
        if( Dest != NULL ) PressData[PressSizeCounter] = 0 ;

        // ���k�ΏƂ̃f�[�^��S�Ĉ��k��̃r�b�g��ɕϊ�����܂Ń��[�v
        for( SrcSizeCounter = 0 ; SrcSizeCounter < SrcSize ; SrcSizeCounter ++ )
        {
            // �ۑ����鐔�l�f�[�^�̃C���f�b�N�X���擾
            NodeIndex = SrcPoint[SrcSizeCounter] ;
            
            // �w��̐��l�f�[�^�̈��k��̃r�b�g����o��
            {
                // �Q�Ƃ���z��̃C���f�b�N�X��������
                BitIndex = 0 ;
                
                // �z��v�f���̏o�͂����r�b�g���̏�����
                BitNum = 0 ;
                
                // �ŏ��ɏ������ރr�b�g��̔z��v�f���Z�b�g
                BitData = Node[NodeIndex].BitArray[0] ;

                // �S�Ẵr�b�g���o�͂���܂Ń��[�v
                for( BitCounter = 0 ; BitCounter < Node[NodeIndex].BitNum ; BitCounter ++ )
                {
                    // �����������񂾃r�b�g�����W�ɂȂ��Ă����玟�̔z��v�f�Ɉڂ�
                    if( PressBitCounter == 8 )
                    {
                        PressSizeCounter ++ ;
                        if( Dest != NULL ) PressData[PressSizeCounter] = 0 ;
                        PressBitCounter = 0 ;
                    }
                    
                    // ���������o�����r�b�g�����W�ɂȂ��Ă����玟�̔z��v�f�Ɉڂ�
                    if( BitNum == 8 )
                    {
                        BitIndex ++ ;
                        BitData = Node[NodeIndex].BitArray[BitIndex] ;
                        BitNum = 0 ;
                    }
                    
                    // �܂������������܂�Ă��Ȃ��r�b�g�A�h���X�ɂP�r�b�g��������
                    if( Dest != NULL ) PressData[PressSizeCounter] |= (unsigned char)( ( BitData & 1 ) << PressBitCounter ) ;

                    // �������񂾃r�b�g���𑝂₷
                    PressBitCounter ++ ;

                    // ���ɏ����o���r�b�g���ŉ��ʃr�b�g(��ԉE�̃r�b�g)�ɂ���ׂ�
                    // �P�r�b�g�E�V�t�g����
                    BitData >>= 1 ;
                    
                    // �����o�����r�b�g���𑝂₷
                    BitNum ++ ;
                }
            }
        }
        
        // �Ō�̂P�o�C�g���̃T�C�Y�𑫂�
        PressSizeCounter ++ ;
    }
    
    // ���k�f�[�^�̏���ۑ�����
    {
		BIT_STREAM BitStream ;
		u8 HeadBuffer[ 256 * 2 + 32 ] ;
		u8 BitNum ;
		u64 HeadSize ;
		s32 WeightSaveData[ 256 ] ;

		BitStream_Init( &BitStream, HeadBuffer, false ) ;

        // ���̃f�[�^�̃T�C�Y���Z�b�g
		BitNum = BitStream_GetBitNum( SrcSize ) ;
		if( BitNum > 0 )
		{
			BitNum -- ;
		}
        BitStream_Write( &BitStream, 6, BitNum ) ;
		BitStream_Write( &BitStream, BitNum + 1, SrcSize ) ;
        
        // ���k��̃f�[�^�̃T�C�Y���Z�b�g
		BitNum = BitStream_GetBitNum( PressSizeCounter ) ;
        BitStream_Write( &BitStream, 6, BitNum ) ;
		BitStream_Write( &BitStream, BitNum + 1, PressSizeCounter ) ;
        
        // �e���l�̏o�����̍����l��ۑ�����
		WeightSaveData[ 0 ] = ( s32 )Node[ 0 ].Weight ;
        for( i = 1 ; i < 256 ; i ++ )
        {
			WeightSaveData[ i ] = ( s32 )Node[ i ].Weight - ( s32 )Node[ i - 1 ].Weight ;
        }
        for( i = 0 ; i < 256 ; i ++ )
        {
			u64 OutputNum ;
			bool Minus ;

			if( WeightSaveData[ i ] < 0 )
			{
				OutputNum = ( u64 )( -WeightSaveData[ i ] ) ;
				Minus = true ;
			}
			else
			{
				OutputNum = ( u64 )WeightSaveData[ i ] ;
				Minus = false ;
			}

			BitNum = ( BitStream_GetBitNum( OutputNum ) + 1 ) / 2 ;
			if( BitNum > 0 )
			{
				BitNum -- ;
			}
	        BitStream_Write( &BitStream, 3, BitNum ) ;
			BitStream_Write( &BitStream, 1, Minus ? 1 : 0 ) ;
			BitStream_Write( &BitStream, ( BitNum + 1 ) * 2, OutputNum ) ;
        }
		
		// �w�b�_�T�C�Y���擾
		HeadSize = BitStream_GetBytes( &BitStream ) ;

		// ���k�f�[�^�̏������k�f�[�^�ɃR�s�[����
		if( Dest != NULL )
		{
			u64 j ;

			// �w�b�_�̕������ړ�
			for( j = PressSizeCounter - 1 ; j >= 0 ; j -- )
			{
				( ( u8 * )Dest )[ HeadSize + j ] = ( ( u8 * )Dest )[ j ] ;
				if( j == 0 )
				{
					break ;
				}
			}

			// �w�b�_����������
			memcpy( Dest, HeadBuffer, ( size_t )HeadSize ) ;
		}

		// ���k��̃T�C�Y��Ԃ�
		return PressSizeCounter + HeadSize ;
    }
}

// ���k�f�[�^����
//
// �߂�l:�𓀌�̃T�C�Y  0 �̓G���[  Dest �� NULL ������Ɖ𓀃f�[�^�i�[�ɕK�v�ȃT�C�Y���Ԃ�
u64 Huffman_Decode( void *Press, void *Dest )
{
    // �����f�[�^�Ɛ��l�f�[�^�A�O�`�Q�T�T�܂ł����l�f�[�^
    HUFFMAN_NODE Node[256 + 255] ;

    u64 PressSizeCounter, DestSizeCounter, DestSize ;
    unsigned char *PressPoint, *DestPoint ;
	u64 OriginalSize ;
	u64 PressSize ;
	u64 HeadSize ;
	u16 Weight[ 256 ] ;
    int i ;

    // void �^�̃|�C���^�ł̓A�h���X�̑��삪�o���Ȃ��̂� unsigned char �^�̃|�C���^�ɂ���
    PressPoint = ( unsigned char * )Press ;
    DestPoint = ( unsigned char * )Dest ;

    // ���k�f�[�^�̏����擾����
	{
		BIT_STREAM BitStream ;
		u8 BitNum ;
		u8 Minus ;
		u16 SaveData ;

		BitStream_Init( &BitStream, PressPoint, true ) ;

		OriginalSize = BitStream_Read( &BitStream, ( u8 )( BitStream_Read( &BitStream, 6 ) + 1 ) ) ;
		PressSize    = BitStream_Read( &BitStream, ( u8 )( BitStream_Read( &BitStream, 6 ) + 1 ) ) ;

		// �o���p�x�̃e�[�u���𕜌�����
		BitNum      = ( u8 )( BitStream_Read( &BitStream, 3 ) + 1 ) * 2 ;
		Minus       = ( u8 )BitStream_Read( &BitStream, 1 ) ;
		SaveData    = ( u16 )BitStream_Read( &BitStream, BitNum ) ;
		Weight[ 0 ] = SaveData ;
        for( i = 1 ; i < 256 ; i ++ )
        {
			BitNum      = ( u8 )( BitStream_Read( &BitStream, 3 ) + 1 ) * 2 ;
			Minus       = ( u8 )BitStream_Read( &BitStream, 1 ) ;
			SaveData    = ( u16 )BitStream_Read( &BitStream, BitNum ) ;
			Weight[ i ] = Minus == 1 ? Weight[ i - 1 ] - SaveData : Weight[ i - 1 ] + SaveData ;
        }

		HeadSize = BitStream_GetBytes( &BitStream ) ;
	}
    
    // Dest �� NULL �̏ꍇ�� �𓀌�̃f�[�^�̃T�C�Y��Ԃ�
    if( Dest == NULL )
        return OriginalSize ;

    // �𓀌�̃f�[�^�̃T�C�Y���擾����
    DestSize = OriginalSize ;

    // �e���l�̌����f�[�^���\�z����
    {
        int NodeIndex, MinNode1, MinNode2 ;
        int NodeNum, DataNum ;

        // ���l�f�[�^������������
        for( i = 0 ; i < 256 + 255 ; i ++ )
        {
            Node[i].Weight = Weight[i] ;    // �o�����͕ۑ����Ă������f�[�^����R�s�[
            Node[i].ChildNode[0] = -1 ;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
            Node[i].ChildNode[1] = -1 ;    // ���l�f�[�^���I�_�Ȃ̂� -1 ���Z�b�g����
            Node[i].ParentNode = -1 ;      // �܂��ǂ̗v�f�Ƃ���������Ă��Ȃ��̂� -1 ���Z�b�g����
        }

        // �o�����̏��Ȃ����l�f�[�^ or �����f�[�^���q����
        // �V���������f�[�^���쐬�A�S�Ă̗v�f���q���Ŏc��P�ɂȂ�܂ŌJ��Ԃ�
        // (���k���Ɠ����R�[�h�ł�)
        DataNum = 256 ; // �c��v�f��
        NodeNum = 256 ; // ���ɐV������錋���f�[�^�̗v�f�z��̃C���f�b�N�X
        while( DataNum > 1 )
        {
            // �o�����l�̒Ⴂ�v�f���T��
            {
                MinNode1 = -1 ;
                MinNode2 = -1 ;
                
                // �c���Ă���v�f�S�Ă𒲂ׂ�܂Ń��[�v
                NodeIndex = 0 ;
                for( i = 0 ; i < DataNum ; NodeIndex ++ )
                {
                    // �������ɉ������̗v�f�ƌ�������Ă���ꍇ�͑ΏۊO
                    if( Node[NodeIndex].ParentNode != -1 ) continue ;
                    
                    i ++ ;
                    
                    // �܂��L���ȗv�f���Z�b�g���Ă��Ȃ����A���o�����l��
                    // ���Ȃ��v�f������������X�V
                    if( MinNode1 == -1 || Node[MinNode1].Weight > Node[NodeIndex].Weight )
                    {
                        // ���܂ň�ԏo�����l�����Ȃ������Ǝv��ꂽ
                        // �v�f�͓�Ԗڂɍ~�i
                        MinNode2 = MinNode1 ;

                        // �V������Ԃ̗v�f�̗v�f�z��̃C���f�b�N�X��ۑ�
                        MinNode1 = NodeIndex ;
                    }
                    else
                    {
                        // ��Ԃ��͏o�����l�������Ă��A��Ԗڂ��͏o�����l��
                        // ���Ȃ���������Ȃ��̂ňꉞ�`�F�b�N(���͓�Ԗڂɏo�����l��
                        // ���Ȃ��v�f���Z�b�g����Ă��Ȃ������ꍇ���Z�b�g)
                        if( MinNode2 == -1 || Node[MinNode2].Weight > Node[NodeIndex].Weight )
                        {
                            MinNode2 = NodeIndex ;
                        }
                    }
                }
            }
            
            // ��̗v�f���q���ŐV�����v�f(�����f�[�^)�����
            Node[NodeNum].ParentNode = -1 ;  // �V�����f�[�^�͓��R�܂������Ƃ��q�����Ă��Ȃ��̂� -1 
            Node[NodeNum].Weight = Node[MinNode1].Weight + Node[MinNode2].Weight ;    // �o�����l�͓�̐��l�𑫂������̂��Z�b�g����
            Node[NodeNum].ChildNode[0] = MinNode1 ;    // ���̌������� 0 ��I�񂾂�o�����l����ԏ��Ȃ��v�f�Ɍq����
            Node[NodeNum].ChildNode[1] = MinNode2 ;    // ���̌������� 1 ��I�񂾂�o�����l����Ԗڂɏ��Ȃ��v�f�Ɍq����

            // �������ꂽ�v�f��ɁA�����B�ɉ��̒l�����蓖�Ă�ꂽ�����Z�b�g����
            Node[MinNode1].Index = 0 ;    // ��ԏo�����l�����Ȃ��v�f�� 0 ��
            Node[MinNode2].Index = 1 ;    // ��Ԗڂɏo�����l�����Ȃ��v�f�� 1 ��

            // �������ꂽ�v�f��ɁA�����B���������������f�[�^�̗v�f�z��C���f�b�N�X���Z�b�g����
            Node[MinNode1].ParentNode = NodeNum ;
            Node[MinNode2].ParentNode = NodeNum ;

            // �v�f�̐�������₷
            NodeNum ++ ;

            // �c��v�f�̐��́A��v�f���V�����ǉ����ꂽ�����
            // ��̗v�f����������Č����̑Ώۂ���O�ꂽ�̂�
            // ���� 1 - 2 �� -1 
            DataNum -- ;
        }

        // �e���l�̈��k���̃r�b�g�������o��
        {
            unsigned char TempBitArray[32] ;
            int TempBitIndex, TempBitCount, BitIndex, BitCount ;
        
            // ���l�f�[�^�ƌ����f�[�^�̐������J��Ԃ�
            for( i = 0 ; i < 256 + 254 ; i ++ )
            {
                // ���l�f�[�^���猋���f�[�^����֏�ւƒH���ăr�b�g���𐔂���
                {
                    // �r�b�g�������������Ă���
                    Node[i].BitNum = 0 ;
                    
                    // �ꎞ�I�ɐ��l�f�[�^����k���Ă������Ƃ��̃r�b�g���ۑ����鏈���̏���
                    TempBitIndex = 0 ;
                    TempBitCount = 0 ;
                    TempBitArray[TempBitIndex] = 0 ;
                    
                    // �������ƌ�������Ă������J�E���g��������(�V�ӂ͉����Ƃ���������Ă��Ȃ��̂ŏI�_���ƕ�����)
                    for( NodeIndex = ( int )i ; Node[NodeIndex].ParentNode != -1 ; NodeIndex = Node[NodeIndex].ParentNode )
                    {
                        // �z��v�f��ɓ���r�b�g�f�[�^�͂W�Ȃ̂ŁA�����z��v�f��
                        // ���ɂW�ۑ����Ă����玟�̔z��v�f�ɕۑ����ύX����
                        if( TempBitCount == 8 )
                        {
                            TempBitCount = 0 ;
                            TempBitIndex ++ ;
                            TempBitArray[TempBitIndex] = 0 ;
                        }
                        
                        // �V�����������ޏ��ō��܂ł̃f�[�^���㏑�����Ă��܂�Ȃ��悤�ɂP�r�b�g���ɃV�t�g����
                        TempBitArray[TempBitIndex] <<= 1 ;

                        // �����f�[�^�Ɋ���U��ꂽ�C���f�b�N�X���ŉ��ʃr�b�g(��ԉE���̃r�b�g)�ɏ�������
                        TempBitArray[TempBitIndex] |= (unsigned char)Node[NodeIndex].Index ;

                        // �ۑ������r�b�g���𑝂₷
                        TempBitCount ++ ;

                        // �r�b�g���𑝂₷
                        Node[i].BitNum ++ ;
                    }
                }
				
                // TempBitArray �ɗ��܂����f�[�^�͐��l�f�[�^���猋���f�[�^��V�ӂɌ�������
                // ��֏�ւƑk���Ă��������̃r�b�g��Ȃ̂ŁA�t���܂ɂ��Ȃ��ƈ��k��̃r�b�g
                // �z��Ƃ��Ďg���Ȃ�(�W�J���ɓV�ӂ̌����f�[�^���琔�l�f�[�^�܂ŒH�邱�Ƃ�
                // �o���Ȃ�)�̂ŁA�������t���܂ɂ������̂𐔒l�f�[�^���̃r�b�g��o�b�t�@�ɕۑ�����
                {
                    BitCount = 0 ;
                    BitIndex = 0 ;
                    
                    // �ŏ��̃o�b�t�@�����������Ă���
                    // (�S�� �_���a(or)���Z �ŏ������ނ̂ŁA�ŏ�����P�ɂȂ��Ă���
                    // �r�b�g�ɂO����������ł��P�̂܂܂ɂȂ��Ă��܂�����)
                    Node[i].BitArray[BitIndex] = 0 ;
                    
                    // �ꎞ�I�ɕۑ����Ă������r�b�g��̍ŏ��܂ők��
                    while( TempBitIndex >= 0 )
                    {
                        // �������񂾃r�b�g������̔z��v�f�ɓ���W�r�b�g��
                        // �B���Ă��܂����玟�̔z��v�f�Ɉڂ�
                        if( BitCount == 8 )
                        {
                            BitCount = 0 ;
                            BitIndex ++ ;
                            Node[i].BitArray[BitIndex] = 0 ;
                        }

                        // �܂������������܂�Ă��Ȃ��r�b�g�A�h���X�ɂP�r�b�g��������
                        Node[i].BitArray[BitIndex] |= (unsigned char)( ( TempBitArray[TempBitIndex] & 1 ) << BitCount ) ;
						
                        // �������ݏI������r�b�g�͂�������Ȃ��̂Ŏ��̃r�b�g��
                        // �������߂�悤�ɂP�r�b�g�E�ɃV�t�g����
                        TempBitArray[TempBitIndex] >>= 1 ;
                        
                        // �P�r�b�g�������񂾂̂Ŏc��r�b�g�����P���炷
                        TempBitCount -- ;
                        
                        // �������ݏ������݌��ƂȂ��Ă���z��v�f�ɏ�������ł��Ȃ�
                        // �r�b�g��񂪖����Ȃ����玟�̔z��v�f�Ɉڂ�
                        if( TempBitCount == 0 )
                        {
                            TempBitIndex -- ;
                            TempBitCount = 8 ;
                        }
                        
                        // �������񂾃r�b�g���𑝂₷
                        BitCount ++ ;
                    }
                }
            }
		}
    }

    // �𓀏���
    {
        unsigned char *PressData ;
        int PressBitCounter, PressBitData, Index, NodeIndex ;
		int NodeIndexTable[ 512 ] ;
		int j ;

		// �e�r�b�g�z�񂪂ǂ̃m�[�h�Ɍq���邩�̃e�[�u�����쐬����
		{
			u16 BitMask[ 9 ] ;

			for( i = 0 ; i < 9 ; i ++ )
			{
				BitMask[ i ] = ( u16 )( ( 1 << ( i + 1 ) ) - 1 ) ;
			}

			for( i = 0 ; i < 512 ; i ++ )
			{
				NodeIndexTable[ i ] = -1 ;

				// �r�b�g��ɓK�������m�[�h��T��
				for( j = 0 ; j < 256 + 254 ; j ++ )
				{
					u16 BitArray01 ;

					if( Node[ j ].BitNum > 9 )
					{
						continue ;
					}

					BitArray01 = ( u16 )Node[ j ].BitArray[ 0 ] | ( Node[ j ].BitArray[ 1 ] << 8 ) ;
					if( ( i & BitMask[ Node[ j ].BitNum - 1 ] ) == ( BitArray01 & BitMask[ Node[ j ].BitNum - 1 ] ) )
					{
						NodeIndexTable[ i ] = j ;
						break ;
					}
				}
			}
		}

        // ���k�f�[�^�{�̂̐擪�A�h���X���Z�b�g
        // (���k�f�[�^�{�̂͌��̃T�C�Y�A���k��̃T�C�Y�A�e���l�̏o��������
        // �i�[����f�[�^�̈�̌�ɂ���)
        PressData = PressPoint + HeadSize ;

        // �𓀂����f�[�^�̊i�[�A�h���X��������
        DestSizeCounter = 0 ;
        
        // ���k�f�[�^�̎Q�ƃA�h���X��������
        PressSizeCounter = 0 ;
        
        // ���k�r�b�g�f�[�^�̃J�E���^��������
        PressBitCounter = 0 ;
        
        // ���k�f�[�^�̂P�o�C�g�ڂ��Z�b�g
        PressBitData = PressData[PressSizeCounter] ;

        // ���k�O�̃f�[�^�T�C�Y�ɂȂ�܂ŉ𓀏������J��Ԃ�
        for( DestSizeCounter = 0 ; DestSizeCounter < DestSize ; DestSizeCounter ++ )
        {
            // �r�b�g�񂩂琔�l�f�[�^����������
            {
				// �Ō��17byte���̃f�[�^�͓V�ӂ���T��( �Ō�̎��̃o�C�g��ǂݏo�����Ƃ��ă������̕s���ȃA�N�Z�X�ɂȂ�\�������邽�� )
				if( DestSizeCounter >= DestSize - 17 )
				{
					// �����f�[�^�̓V�ӂ͈�ԍŌ�̌����f�[�^���i�[�����T�P�O�Ԗ�(�O�Ԃ��琔����)
					// �V�ӂ��珇�ɉ��ɍ~��Ă���
					NodeIndex = 510 ;
				}
				else
				{
					// ����ȊO�̏ꍇ�̓e�[�u�����g�p����

                    // ���� PressBitData �Ɋi�[����Ă���S�Ă�
                    // �r�b�g�f�[�^���g���؂��Ă��܂����ꍇ�͎���
                    // �r�b�g�f�[�^���Z�b�g����
                    if( PressBitCounter == 8 )
                    {
                        PressSizeCounter ++ ;
                        PressBitData = PressData[PressSizeCounter] ;
                        PressBitCounter = 0 ;
                    }

					// ���k�f�[�^��9bit���p�ӂ���
					PressBitData = ( PressBitData | ( PressData[ PressSizeCounter + 1 ] << ( 8 - PressBitCounter ) ) ) & 0x1ff ;

					// �e�[�u������ŏ��̌����f�[�^��T��
					NodeIndex = NodeIndexTable[ PressBitData ] ;

					// �g���������k�f�[�^�̃A�h���X��i�߂�
					PressBitCounter += Node[ NodeIndex ].BitNum ;
					if( PressBitCounter >= 16 )
					{
						PressSizeCounter += 2 ;
						PressBitCounter -= 16 ;
						PressBitData = PressData[PressSizeCounter] >> PressBitCounter ;
					}
					else
					if( PressBitCounter >= 8 )
					{
						PressSizeCounter ++ ;
						PressBitCounter -= 8 ;
						PressBitData = PressData[PressSizeCounter] >> PressBitCounter ;
					}
					else
					{
						PressBitData >>= Node[ NodeIndex ].BitNum ;
					}
				}
                
                // ���l�f�[�^�ɒH�蒅���܂Ō����f�[�^������Ă���
                while( NodeIndex > 255 )
                {
                    // ���� PressBitData �Ɋi�[����Ă���S�Ă�
                    // �r�b�g�f�[�^���g���؂��Ă��܂����ꍇ�͎���
                    // �r�b�g�f�[�^���Z�b�g����
                    if( PressBitCounter == 8 )
                    {
                        PressSizeCounter ++ ;
                        PressBitData = PressData[PressSizeCounter] ;
                        PressBitCounter = 0 ;
                    }
                    
                    // �P�r�b�g�擾����
                    Index = PressBitData & 1 ;
                    
                    // �g�p�����P�r�b�g�������E�ɃV�t�g����
                    PressBitData >>= 1 ;
                    
                    // �g�p�����r�b�g��������₷
                    PressBitCounter ++ ;
                    
                    // ���̗v�f(�����f�[�^�����l�f�[�^���͂܂�������Ȃ�)�Ɉڂ�
                    NodeIndex = Node[NodeIndex].ChildNode[Index] ;
                }
            }

            // �H�蒅�������l�f�[�^���o��
            DestPoint[DestSizeCounter] = (unsigned char)NodeIndex ;
        }
    }

    // �𓀌�̃T�C�Y��Ԃ�
    return OriginalSize ;
}

