//###################################################################
//		�e��f��
//###################################################################

//map: �f�ނ̊��N���X
SQ_ABSTRACT class CMaterialBase : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//�������z�֐�
	SQ_NOBIND virtual int Add(int dbType, int fixedID) = 0;			//�߂�l��DX���C�u���������������n���h��
	SQ_NOBIND virtual int Restore(int dbType, int fixedID) = 0;		//�߂�l��DX���C�u���������������n���h��
	SQ_NOBIND virtual void Delete(int hdl) = 0;
	SQ_NOBIND virtual void Clear() = 0;
};


class CMaterial {
public:

	//####################  �O���t�B�b�N  ####################
	class CMaterialGraphics : public CMaterialBase {
	public:
		//SQ_BEGINMEMBER
		//�����o�[���\�b�h
		SQ_NOBIND int Restore(int dbType, int dbID) override;
		SQ_NOBIND int Add(int dbType, int dbID) override;
		SQ_INSTANCEFUNC int Add(string fileName);
		SQ_INSTANCEFUNC void Delete(int hdl) override;
		SQ_INSTANCEFUNC void Clear() override;
		SQ_NOBIND void LoadIconGraphic();

		//�����o�[�ϐ�
		SQ_NOBIND int IconSize;
		SQ_PROPGET int getIconSize() { return this->IconSize; }

		SQ_PROPGET int getIconGraphicHandle() { return this->iconGraphicHandle; }

		SQ_NOBIND Cstdmap<CIDValueData> Data;

	private:
		int iconGraphicHandle;
	};


	//####################  �T�E���h  ####################
	//SQ_NOBIND_BEGIN
	class CSoundData : public CIDNameData {
	public:
		//�����o�[�ϐ�
		bool IsMIDI;				//MIDI�t�@�C���ł��邩
		int StartLoopPos;			//�`���[�v�̃~���b�ʒu�i�ǂ� "��" �߂��čĐ�����̂��j
		int EndLoopPos;			//�a���[�v�̃~���b�ʒu�i�ǂ� "�܂�" �Đ����Ė߂�̂��j
		int CurrentPos;				//�Đ��ʒu�̕ۑ��p
		int Volume;					//�S�̂̍ő剹�ʂ𒴂��Ȃ�
	};
	//SQ_NOBIND_END

	class CMaterialSound : public CMaterialBase {
	public:
		//SQ_BEGINMEMBER
		//�����o�[�ϐ�
		SQ_NOBIND Cstdmap<CSoundData> Data;
		SQ_NOBIND int StandardVolume = 255;			//�W������
		SQ_PROPGET int getStandardVolume() { return this->StandardVolume; } SQ_PROPSET void setStandardVolume(int value);

		//���\�b�h
		SQ_NOBIND int Restore(int dbType, int dbID) override;
		SQ_NOBIND int Add(int dbType, int dbID) override;
		SQ_INSTANCEFUNC int Add(string fileName);
		SQ_INSTANCEFUNC void Delete(int hdl) override;
		SQ_INSTANCEFUNC void Clear() override;

		SQ_INSTANCEFUNC void Play(int hdl, int playType, int volume = 255, int pitchRate = 0, int panPot = 0, double fadeTime = 0.0F);
		SQ_INSTANCEFUNC void PlayOnce(int hdl, int playType, int volume = 255, int pitchRate = 0, int panPot = 0);
		SQ_INSTANCEFUNC void Stop(int hdl, double fadeTime = 0.0F, bool afterDelete = false);
		SQ_INSTANCEFUNC void StopAll(double fadeTime = 0.0F, bool afterDelete = false);
		SQ_INSTANCEFUNC void Pause(int hdl, double fadeTime = 0.0F);
		SQ_INSTANCEFUNC void Resume(int hdl, bool loopMode = false, double fadeTime = 0.0F);
		SQ_INSTANCEFUNC int CheckPlayStatus(int hdl);
	};


	//####################  �t�H���g  ####################
	//SQ_NOBIND_BEGIN
	class CFontData : public CIDNameData {
	public:
		//�����o�[���\�b�h
		static CFontData GetDefaultFont();

		//�����o�[�ϐ�
		bool IsOriginalFont;		//�O���t�H���g���ǂ���
		int DBID;					//�f�[�^�x�[�X��ł�ID
		int Size;					//�T�C�Y
		int Thick;					//����
		int Type;					//�A���`�G�C���A�X�E�G�b�W�t���Ȃǂ̐ݒ�
		int Space;					//�����ԃX�y�[�X
		int LineSpace;				//�s�ԃX�y�[�X
		int EdgeColor;				//���̐F
		string FontName;			//�t�H���g��������
		HANDLE OriginalFontHandle;	//�O���t�H���g�̃n���h�� (WinAPI�p)
	};
	//SQ_NOBIND_END

	class CMaterialFont : public CMaterialBase {
	public:
		//SQ_BEGINMEMBER
		//�����o�[�ϐ�
		SQ_NOBIND int DefaultFontHandle;
		SQ_NOBIND Cstdmap<CFontData> Data;

		//���\�b�h
		SQ_NOBIND void AddDefaultFont();
		SQ_NOBIND int Restore(int dbType, int dbID) override;
		SQ_NOBIND int Add(int dbType, int dbID) override;
		SQ_INSTANCEFUNC int Add(int dbID, string fontName, string fileName, int size, int thick, int lineSpace, int space, int type, int edgeColor);
		SQ_INSTANCEFUNC void Delete(int hdl) override;
		SQ_INSTANCEFUNC void Clear() override;
	};


	//SQ_BEGINMEMBER
	//�����o�[���\�b�h
	//####################  �e��f�ނ𓝍�  ####################
	SQ_NOBIND std::array<CMaterialBase*, static_cast<int>(TS::Material::MaterialType::Count)> Types;
	SQ_NOBIND void Dispose() {
		for(auto& mt : this->Types) {
			mt->Clear();
			delete mt;
			mt = nullptr;
		}
	}
	SQ_INSTANCEFUNC void Clear() {
		for(auto& mt : this->Types) {
			mt->Clear();
		}
	}

	//####################  �e��f�ނփA�N�Z�X  ####################
	SQ_PROPGET CMaterialFont* getFonts() {
		return dynamic_cast<CMaterialFont*>(this->Types[static_cast<int>(TS::Material::MaterialType::Font)]);
	}
	SQ_PROPGET CMaterialGraphics* getGraphics() {
		return dynamic_cast<CMaterialGraphics*>(this->Types[static_cast<int>(TS::Material::MaterialType::Graphics)]);
	}
	SQ_PROPGET CMaterialSound* getSounds() {
		return dynamic_cast<CMaterialSound*>(this->Types[static_cast<int>(TS::Material::MaterialType::Sound)]);
	}
};
