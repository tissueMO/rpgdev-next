//###################################################################
//		各種素材
//###################################################################

//map: 素材の基底クラス
SQ_ABSTRACT class CMaterialBase : public CNullCheck {
public:
	//SQ_BEGINMEMBER
	//純粋仮想関数
	SQ_NOBIND virtual int Add(int dbType, int fixedID) = 0;			//戻り値はDXライブラリが生成したハンドル
	SQ_NOBIND virtual int Restore(int dbType, int fixedID) = 0;		//戻り値はDXライブラリが生成したハンドル
	SQ_NOBIND virtual void Delete(int hdl) = 0;
	SQ_NOBIND virtual void Clear() = 0;
};


class CMaterial {
public:

	//####################  グラフィック  ####################
	class CMaterialGraphics : public CMaterialBase {
	public:
		//SQ_BEGINMEMBER
		//メンバーメソッド
		SQ_NOBIND int Restore(int dbType, int dbID) override;
		SQ_NOBIND int Add(int dbType, int dbID) override;
		SQ_INSTANCEFUNC int Add(string fileName);
		SQ_INSTANCEFUNC void Delete(int hdl) override;
		SQ_INSTANCEFUNC void Clear() override;
		SQ_NOBIND void LoadIconGraphic();

		//メンバー変数
		SQ_NOBIND int IconSize;
		SQ_PROPGET int getIconSize() { return this->IconSize; }

		SQ_PROPGET int getIconGraphicHandle() { return this->iconGraphicHandle; }

		SQ_NOBIND Cstdmap<CIDValueData> Data;

	private:
		int iconGraphicHandle;
	};


	//####################  サウンド  ####################
	//SQ_NOBIND_BEGIN
	class CSoundData : public CIDNameData {
	public:
		//メンバー変数
		bool IsMIDI;				//MIDIファイルであるか
		int StartLoopPos;			//Ａループのミリ秒位置（どこ "に" 戻って再生するのか）
		int EndLoopPos;			//Ｂループのミリ秒位置（どこ "まで" 再生して戻るのか）
		int CurrentPos;				//再生位置の保存用
		int Volume;					//全体の最大音量を超えない
	};
	//SQ_NOBIND_END

	class CMaterialSound : public CMaterialBase {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND Cstdmap<CSoundData> Data;
		SQ_NOBIND int StandardVolume = 255;			//標準音量
		SQ_PROPGET int getStandardVolume() { return this->StandardVolume; } SQ_PROPSET void setStandardVolume(int value);

		//メソッド
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


	//####################  フォント  ####################
	//SQ_NOBIND_BEGIN
	class CFontData : public CIDNameData {
	public:
		//メンバーメソッド
		static CFontData GetDefaultFont();

		//メンバー変数
		bool IsOriginalFont;		//外部フォントかどうか
		int DBID;					//データベース上でのID
		int Size;					//サイズ
		int Thick;					//太さ
		int Type;					//アンチエイリアス・エッジ付きなどの設定
		int Space;					//文字間スペース
		int LineSpace;				//行間スペース
		int EdgeColor;				//縁の色
		string FontName;			//フォント正式名称
		HANDLE OriginalFontHandle;	//外部フォントのハンドル (WinAPI用)
	};
	//SQ_NOBIND_END

	class CMaterialFont : public CMaterialBase {
	public:
		//SQ_BEGINMEMBER
		//メンバー変数
		SQ_NOBIND int DefaultFontHandle;
		SQ_NOBIND Cstdmap<CFontData> Data;

		//メソッド
		SQ_NOBIND void AddDefaultFont();
		SQ_NOBIND int Restore(int dbType, int dbID) override;
		SQ_NOBIND int Add(int dbType, int dbID) override;
		SQ_INSTANCEFUNC int Add(int dbID, string fontName, string fileName, int size, int thick, int lineSpace, int space, int type, int edgeColor);
		SQ_INSTANCEFUNC void Delete(int hdl) override;
		SQ_INSTANCEFUNC void Clear() override;
	};


	//SQ_BEGINMEMBER
	//メンバーメソッド
	//####################  各種素材を統合  ####################
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

	//####################  各種素材へアクセス  ####################
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
