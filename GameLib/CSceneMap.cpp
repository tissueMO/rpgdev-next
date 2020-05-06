//###################################################################
//			�}�b�v�V�[����`
//###################################################################
#include "Header.h"
#define Z_INFLATE_WINDOWBITS (15/*zlib�t�H�[�}�b�g�l*/ + 16/*gzip�t�H�[�}�b�g�l*/ + 16/*zlib/gzip�݊��p�t�H�[�}�b�g�l*/)

const uInt buflimit = 1024 * 1024 * 1;
const uInt tempsize = 16384;
extern char buf[TS::System::BufferLengthMax];
bool EVPage(TS_MT);
bool EVTrigger(TS_MT);
bool EVMove(TS_MT);
bool Anim(TS_MT);
void RestoreTilePallet();


//####################  �����ݒ�  ####################
CSceneMap::CSceneMap() {
	//�p�ɂɎg���ݒ荀�ڂ��R�s�[
	this->MapData.TileSize = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::TileSize);
	this->MapData.ShadowTransparent = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::ShadowTransparent);
	this->MapData.FogTransparent = Game.DB.getFDBSystem()->GetInteger(TS::DB::System::FogTransparent);

	//�v���C���[�ݒ�
	this->MapData.Player.MapObject.ID = -1;
	this->MapData.Player.MapObject.Hide = false;
	this->MapData.Player.CharData.EnabledStopAnim = true;
	this->MapData.Player.CharData.EnabledWalkAnim = true;
	this->MapData.Player.CharData.NoHit = false;
	this->MapData.Player.CharData.FixedDirection = false;
	this->MapData.Player.CharData.Direction = static_cast<int>(TS::Map::Direction4::South);

	//�W����MT�֐���o�^
	Game.MT.AddThread("EVPage", ::EVPage, TS::System::Scene::Map, true);
	Game.MT.AddThread("EVTrigger", ::EVTrigger, TS::System::Scene::Map, true);
	Game.MT.AddThread("EVMove", ::EVMove, TS::System::Scene::Map, true);
	Game.MT.AddThread("Anim", ::Anim, TS::System::Scene::Map, true)
		.AddInteger("counter", 0);
}


//####################  �V�[���j����  ####################
void CSceneMap::Dispose() {
	CSceneBase::Dispose();
	Game.DB.getFDBTileset()->DisposeTilePallet();
	this->MapData.Dispose();
}


//####################  ���݂̃}�b�v�ɂ���C�x���g�̌ʕϐ���������~�σf�[�^�ɃR�~�b�g����  ####################
void CSceneMap::commitMapSelfValue() {
	//�S�C�x���g�𑖍�
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		//�S�C�x���g�ʕϐ��𑖍�
		for(auto& selfValue : ev.SelfValues.data.Items) {
			auto exists = false;

			//���ɃR�~�b�g�ς݂̌ʕϐ��e�[�u���𑖍�
			for(auto& item : SelfValueData) {
				if(item.MapGUID == Game.getSceneMap()->MapData.GUID 
				&& item.EVID == ev.ID
				&& selfValue.first == item.Value.ID) {
					//���Ƀf�[�^���o�^����Ă���ꍇ�͒l���X�V����
					item.Value = selfValue.second;
					exists = true;
					break;
				}
			}

			if(!exists) {
				//�V�K�o�^
				SelfValueData.push_back(CEVSelfValueOneData(
					Game.getSceneMap()->MapData.GUID,
					ev.ID,
					&selfValue.second
				));
			}
		}
	}
}


//####################  ���݂̃}�b�v�̃C�x���g�ɑ΂��A�R�~�b�g����Ă���ʕϐ����𔽉f����  ####################
//���ӁF�}�b�v�C�x���g��z�u���I������ɌĂяo�����ƁB
void CSceneMap::applyMapSelfValue() {
	//�S�f�[�^�𑖍����A���̃}�b�v�ł��邪���݂��Ȃ��C�x���g�̏�񂪂���ꍇ�͍폜����
	SelfValueData.erase(std::remove_if(TS_CONTAINERALL(SelfValueData), [&](CEVSelfValueOneData& item) {
		//���̃}�b�v�ł͂Ȃ��ꍇ�̓X�L�b�v
		if(item.MapGUID != Game.getSceneMap()->MapData.GUID) {
			return false;
		}

		//�S�C�x���g�𑖍����A�Y���C�x���g�����݂��Ȃ����ǂ����𒲂ׂ�
		return std::find_if(TS_CONTAINERALL(Game.getSceneMap()->MapData.EVs.Items), [&](CSceneMap::CMapData::CMapEventData& data) {
			return item.EVID == data.ID;
		}) == Game.getSceneMap()->MapData.EVs.Items.end();
	}), SelfValueData.end());

	//�S�C�x���g�𑖍�
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		//��ɏ����l��^����
		Game.DB.getVDBValue()->GetSelfValueList(static_cast<int>(TS::DB::DBValueIndices::SelfEvent), &ev.SelfValues);

		//���̃}�b�v�̂��̃C�x���g�̌ʕϐ���񂪊��ɓo�^����Ă��邩�ǂ����𒲂ׂ�
		for(auto& sv : SelfValueData) {
			if(sv.MapGUID == Game.getSceneMap()->MapData.GUID && sv.EVID == ev.ID) {
				//���Ƀf�[�^���o�^����Ă���ꍇ�͍ŐV�l���Z�b�g����
				ev.SelfValues.Get(sv.Value.ID)->Value = sv.Value.Value;
			}
		}
	}
}


//####################  �Z�[�u�f�[�^��������~�σf�[�^�x�[�X�ɔ��f����  ####################
void CSceneMap::loadSelfValueData(vector<string>& vectorbuf) {
	string temp;
	vector<string> spl;
	vector<string> subspl;
	auto& selfEV = *Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfEvent));

	//��s���o�Ă���܂ŘA���I�ɓǂݍ���
	SelfValueData.clear();
	while(!(temp = Game.PopHead(vectorbuf)).empty()) {
		Game.SplitString(spl, temp, "\t");
		if(spl.size() < 4) {
			continue;		//�s���ȃf�[�^�͖���
		}

		//�L���ȕϐ���ID�ł��邩���؂��A�����ȃf�[�^�͗��Ƃ�
		const int valueID = atoi(spl[2].c_str());
		auto row = selfEV.GetRowFromID(valueID);
		if(row == nullptr) {
			continue;
		}

		//���ۂɓo�^����
		auto data = CIDValueData(valueID, *row->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR(), atoi(spl[3].c_str()));
		SelfValueData.push_back(CEVSelfValueOneData(spl[0].c_str(), atoi(spl[1].c_str()), &data));
	}
}


//####################  �����~�σf�[�^�x�[�X���Z�[�u�f�[�^�ɏo�͂���  ####################
void CSceneMap::saveSelfValueData(string& strbuf) {
	for(auto& data : SelfValueData) {
		strbuf += Game.Format("%s\t%d\t%d\t%d\n", data.MapGUID.c_str(), data.EVID, data.Value.ID, data.Value.Value);
	}
	strbuf += "\n";		//��s�����
}


//####################  �Z�[�u�f�[�^���猻�݂̃}�b�v�����[�h  ####################
void CSceneMap::Load(vector<string>& vectorbuf) {
	string temp;
	auto& map = Game.getSceneMap()->MapData;

	//�}�b�v�̊�{���
	temp = Game.PopHead(vectorbuf);
	Game.getSceneMap()->BeforeBGMFileName = "";
	Game.getSceneMap()->BeforeBGSFileName = "";
	map.LoadMap(temp, true);
	map.Player.MapObject.Hide = atoi(Game.PopHead(vectorbuf).c_str());
	map.Player.MapObject.Position.Set(Game.PopHead(vectorbuf));
	map.Player.CharData.Direction = atoi(Game.PopHead(vectorbuf).c_str());
	map.Player.CharData.LoadGraphics(Game.PopHead(vectorbuf));

	//�C�x���g�̔z�u���
	temp = Game.PopHead(vectorbuf);
	const int EVCount = atoi(temp.c_str());
	TS_COUNTLOOP(i, EVCount) {
		//�f�[�^�����[�h���ăC���X�^���X�𐶐�����
		map.EVs.Add(CMapData::CMapEventData(vectorbuf));
	}
	TS_COUNTLOOP(i, EVCount) {
		//����ړ��̓X�N���v�g���畜������
		auto ev = map.EVs.GetFromIndex(i);
		ev->TargetCharPTR = ev->getCurrentPage();
		Game.getSceneMap()->MapData.RunEV(ev->ID, ev->GetPageIndex(), static_cast<int>(TS::Map::EventScriptType::Move), false);
	}

	//�C�x���g�ʕϐ��̓����~�σf�[�^�x�[�X��ǂݍ��݁A���ݒn�}�b�v�ɊY������ʕϐ�����K�p����
	CSceneMap::loadSelfValueData(vectorbuf);
	CSceneMap::applyMapSelfValue();

	//�X�N���v�g��GC���s
	Game.SQ.CollectGarbage();
}


//####################  ���݂̃}�b�v���Z�[�u�f�[�^�ɏo��  ####################
void CSceneMap::Save(string& strbuf) {
	//�Z�[�u�O�Ɍ��݂̌ʕϐ������R�~�b�g���Ă���
	CSceneMap::commitMapSelfValue();

	//�}�b�v�̊�{���
	auto& map = Game.getSceneMap()->MapData;
	strbuf += map.CurrentMapFileName + "\n";
	strbuf += to_string(map.Player.MapObject.Hide) + "\n";
	strbuf += map.Player.MapObject.Position.ToString() + "\n";
	strbuf += to_string(map.Player.CharData.getDirection()) + "\n";
	strbuf += map.Player.CharData.Graphics + "\n";

	//�C�x���g�̔z�u���
	strbuf += to_string(map.EVs.GetCount()) + "\n";
	for(auto& ev : map.EVs.Items) {
		ev.toString(strbuf);		//�C�x���g��{���A�y�[�W�����o��
	}

	//�C�x���g�ʕϐ��̓����~�σf�[�^�x�[�X
	CSceneMap::saveSelfValueData(strbuf);
}


//####################  �}�b�v�������O�o�͂���  ####################
void CSceneMap::PrintMap() {
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return;
	}

	TS_LOG("\n�}�b�v���:");
	TS_LOG(Game.Format("\t�}�b�v��: %s", Game.getSceneMap()->MapData.CurrentMapName.c_str()).c_str());
	TS_LOG(Game.Format("\t�}�b�vGUID: %s", Game.getSceneMap()->MapData.GUID.c_str()).c_str());
	TS_LOG(Game.Format("\t�v���C���[���:").c_str());
	TS_LOG(Game.Format("\t\t���W: (x, y) = (%d, %d)",
		Game.getSceneMap()->MapData.Player.MapObject.Position.X,
		Game.getSceneMap()->MapData.Player.MapObject.Position.Y).c_str());
	TS_LOG(Game.Format("\t\t����: %d", Game.getSceneMap()->MapData.Player.CharData.Direction).c_str());
}


//####################  �C�x���g�������O�o�͂���  ####################
void CSceneMap::PrintEVs() {
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return;
	}

	TS_LOG("\n�}�b�v�C�x���g�ꗗ:");
	for(auto& data : Game.getSceneMap()->MapData.EVs.Items) {
		TS_LOG(data.toString());
	}
}


//####################  �����~�σf�[�^�x�[�X�����O�o�͂���  ####################
void CSceneMap::PrintSelfValueData() {
	if(Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return;
	}

	TS_LOG("\n�}�b�v�C�x���g�ʕϐ��̓����~�σf�[�^�x�[�X:");
	for(auto& data : SelfValueData) {
		TS_LOG("\tMap [%s] -> EVID: %d -> �ʕϐ�ID: %d (%s) = %d",
			   data.MapGUID.c_str(),
			   data.EVID,
			   data.Value.ID,
			   Game.DB.getVDBValue()->GetSubDB(static_cast<int>(TS::DB::DBValueIndices::SelfEvent))->GetRowFromID(data.Value.ID)->at(static_cast<int>(TS::DB::DBDefaultColumnIndices::Name)).GetStringPTR()->c_str(),
			   data.Value.Value
			   );
	}
}


//####################  gzip�𓀏���  ####################
bool CSceneMap::deflate(string fileName, string& text) {
	text = "";
	int ret;

#ifdef _DEBUG
	//�f�o�b�O��: �t�@�C�����璼�ډ𓀂��s��
	gzFile inFile = ::gzopen(fileName.c_str(), GZ_MODE_READ);

	//�𓀏���
	while((ret = ::gzread(inFile, ::buf, sizeof(::buf) - 1)) > 0) {
		::buf[sizeof(::buf) - 1] = '\0';		//�r���œr�؂ꂽ�Ƃ��Agzread�֐��͖����ɏI�[�L����t���Ă���Ȃ����Ƃ�����炵��
		text += ::buf;
	}

	//gzerror�ŃG���[���b�Z�[�W���擾����
	if(ret == -1) {
		const char *msg = ::gzerror(inFile, &ret);
		if(ret == Z_ERRNO) {
			msg = ::strerror(ret);
		}
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�𓀂Ɏ��s���܂����B\n>%s\n\n\n>%s", fileName.c_str(), msg);
		return false;
	}

	//gzclose�Ɏ��s�����ꍇ�A�t�@�C���͕�����̂�gzerror�ł��Ȃ�
	if((ret = ::gzclose(inFile)) != Z_OK) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�𓀌�̃t�@�C���N���[�Y�Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}

#else
	//�����[�X��: DX�A�[�J�C�u����o�C�i����ǂݏo���ăX�g���[���𓀂��s��
	z_stream z;

	//�𓀃X�g���[���̏����ݒ�
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	//�𓀏���
	ret = ::inflateInit2(&z, Z_INFLATE_WINDOWBITS);
	if(ret != Z_OK) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�𓀂Ɏ��s���܂����B\n>%s\n\n\n>%s", fileName.c_str(), z.msg);
		return false;
	}

	//�Ώۃt�@�C����DX�A�[�J�C�u������o��
	int hdl = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str(), false);
	if (hdl == 0) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�}�b�v�t�@�C���̎��o���Ɏ��s���܂����B\n>%s\n\n\n>%s", fileName.c_str(), z.msg);
		return false;
	}
	int size = DxLib::FileRead_size((TS::DirectoryNames::PrePath + fileName).c_str());
	void* bin = malloc(size);
	DxLib::FileRead_read(bin, size, hdl);

	//���͌��A�o�͐��p�ӂ���
	z.next_in = (Bytef*)bin;
	z.avail_in = size;					//���́i���k�f�[�^�j����������邱�Ƃ͂Ȃ�
	z.next_out = (Bytef*)::buf;
	const auto availOutSize = sizeof(::buf) - 2;
	z.avail_out = availOutSize;			//�o�́i�𓀃f�[�^�j�͕������Ȃ��珑���o��

	//�����𓀂��Ă���
	while(ret != Z_STREAM_END) {
		ret = ::inflate(&z, Z_NO_FLUSH);
		if(ret != Z_OK && ret != Z_STREAM_END) {
			::inflateEnd(&z);			//��������Ȃ��Ƒ�ʂ̃��[�N�����������炷
			::free(bin);
			Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�𓀂Ɏ��s���܂����B\n>%s\n\n\n>%s", fileName.c_str(), z.msg);
			return false;
		}
		text += (char*)::buf;			//�𓀂���Ɖ��s�L�����܂ޕ����񂪓�����

		//���̓W�J����
		z.next_out = (Bytef*)::buf;
		z.avail_out = availOutSize;		//�o�́i�𓀃f�[�^�j�͕������Ȃ��珑���o��
	}

	DxLib::FileRead_close(hdl);
	::inflateEnd(&z);		//��������Ȃ��Ƒ�ʂ̃��[�N�����������炷
	::free(bin);
#endif

	return true;
}


//####################  �񈳏k�}�b�v�f�[�^�ǂݍ��ݏ���  ####################
bool CSceneMap::loadTextMap(string fileName, string& text) {
	text = "";
	const int size = sizeof(::buf) - 1;
	
#ifdef _DEBUG		//�f�o�b�O���[�h�̂Ƃ��͓ǂݍ��ݐ�p�Ń��[�h�������̂ŕW��IO���g��

	FILE* fp;
	fp = ::fopen(fileName.c_str(), "r");
	if (fp == nullptr) {
		Game.RaiseWarning(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�e�L�X�g�t�@�C���̃I�[�v���Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}

	while (::feof(fp) == 0) {
		while (::feof(fp) == 0) {
			::fgets(::buf, size, fp);

			//fgets�ł͉��s�R�[�h���܂܂�Ă��܂��̂ł����ŏ�������
			if (::buf[::strlen(::buf) - 1] == '\n') {
				::buf[::strlen(::buf) - 1] = '\0';
				break;
			}
		}

		text += ::buf;
		text += "\r\n";
	}

	if (fp != nullptr) {
		::fclose(fp);
		fp = nullptr;
	}

#else				//�����[�X���[�h�̂Ƃ���DX���C�u�����A�[�J�C�u���J�������̂�DX���C�u�����̃t�@�C�������֐����g��

	int fp;
	fp = DxLib::FileRead_open((TS::DirectoryNames::PrePath + fileName).c_str());
	if (fp == 0) {
		Game.RaiseError(__FILE__, __FUNCTION__, __LINE__, TS::System::ErrorID::Failed, "�e�L�X�g�t�@�C���̃I�[�v���Ɏ��s���܂����B\n>%s", fileName.c_str());
		return false;
	}

	while (DxLib::FileRead_eof(fp) == 0) {
		while (DxLib::FileRead_eof(fp) == 0) {
			DxLib::FileRead_gets(::buf, size, fp);

			//��s�����S�ɓǂ߂����ǂ����́A������̒����� (size-1) �����ł��邩�ǂ����Ŕ��f����
			if (::strlen(::buf) < size - 1) {
				break;
			}
		}

		text += ::buf;
		text += "\r\n";
	}

	DxLib::FileRead_close(fp);

#endif

	return true;
}


//####################  �W���R�}���h�F���b�Z�[�W�\��  ####################
//��UC++���Ŏ󂯂邪�A�g�������������邽�߂ɃC�x���g�n���h���[���Ăяo��
void CSceneMap::ShowMessage(int faceID, string speakerName, string msg) {
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_ShowMessage, faceID, speakerName, msg);
}


//####################  �W���R�}���h�F�ꏊ�ړ�  ####################
//��UC++���Ŏ󂯂邪�A�g�������������邽�߂ɃC�x���g�n���h���[���Ăяo��
void CSceneMap::ChangeMap(string fileName, int x, int y, int direction4) {
	this->MapData.ClearEVReserve();			//�C�x���g�\������ׂĔj��
	//Game.MT.Clear(false);
	CSceneMap::commitMapSelfValue();		//�ʕϐ������R�~�b�g
	Game.SQ.DoFunc<int>(TS::SQ::SQEvent_ChangeMap, fileName, x, y, direction4);
}


//####################  �w�肵���������獶���������S������Ԃ�  ####################
int CSceneMap::TurnLeft4(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:		return static_cast<int>(TS::Map::Direction4::North);
		case TS::Map::Direction4::West:		return static_cast<int>(TS::Map::Direction4::South);
		case TS::Map::Direction4::South:	return static_cast<int>(TS::Map::Direction4::East);
		case TS::Map::Direction4::North:	return static_cast<int>(TS::Map::Direction4::West);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  �w�肵���������獶���������W������Ԃ�  ####################
int CSceneMap::TurnLeft8(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:	return static_cast<int>(TS::Map::Direction8::NorthEast);
		case TS::Map::Direction8::NorthEast:	return static_cast<int>(TS::Map::Direction8::North);
		case TS::Map::Direction8::North:	return static_cast<int>(TS::Map::Direction8::NorthWest);
		case TS::Map::Direction8::NorthWest:	return static_cast<int>(TS::Map::Direction8::West);
		case TS::Map::Direction8::West:	return static_cast<int>(TS::Map::Direction8::SouthWest);
		case TS::Map::Direction8::SouthWest:	return static_cast<int>(TS::Map::Direction8::South);
		case TS::Map::Direction8::South:	return static_cast<int>(TS::Map::Direction8::SouthEast);
		case TS::Map::Direction8::SouthEast:	return static_cast<int>(TS::Map::Direction8::East);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  �w�肵����������E���������S������Ԃ�  ####################
int CSceneMap::TurnRight4(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:	return static_cast<int>(TS::Map::Direction4::South);
		case TS::Map::Direction4::West:	return static_cast<int>(TS::Map::Direction4::North);
		case TS::Map::Direction4::South:	return static_cast<int>(TS::Map::Direction4::West);
		case TS::Map::Direction4::North:	return static_cast<int>(TS::Map::Direction4::East);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  �w�肵����������E���������W������Ԃ�  ####################
int CSceneMap::TurnRight8(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:	return static_cast<int>(TS::Map::Direction8::SouthEast);
		case TS::Map::Direction8::SouthEast:	return static_cast<int>(TS::Map::Direction8::South);
		case TS::Map::Direction8::South:	return static_cast<int>(TS::Map::Direction8::SouthWest);
		case TS::Map::Direction8::SouthWest:	return static_cast<int>(TS::Map::Direction8::West);
		case TS::Map::Direction8::West:	return static_cast<int>(TS::Map::Direction8::NorthWest);
		case TS::Map::Direction8::NorthWest:	return static_cast<int>(TS::Map::Direction8::North);
		case TS::Map::Direction8::North:	return static_cast<int>(TS::Map::Direction8::NorthEast);
		case TS::Map::Direction8::NorthEast:	return static_cast<int>(TS::Map::Direction8::East);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  �w�肵���������甽�΂��������S������Ԃ�  ####################
int CSceneMap::TurnHalf4(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:	return static_cast<int>(TS::Map::Direction4::West);
		case TS::Map::Direction4::West:	return static_cast<int>(TS::Map::Direction4::East);
		case TS::Map::Direction4::South:	return static_cast<int>(TS::Map::Direction4::North);
		case TS::Map::Direction4::North:	return static_cast<int>(TS::Map::Direction4::South);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  �w�肵���������甽�΂��������W������Ԃ�  ####################
int CSceneMap::TurnHalf8(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:	return static_cast<int>(TS::Map::Direction8::West);
		case TS::Map::Direction8::SouthEast:	return static_cast<int>(TS::Map::Direction8::NorthWest);
		case TS::Map::Direction8::West:	return static_cast<int>(TS::Map::Direction8::East);
		case TS::Map::Direction8::SouthWest:	return static_cast<int>(TS::Map::Direction8::NorthEast);
		case TS::Map::Direction8::South:	return static_cast<int>(TS::Map::Direction8::North);
		case TS::Map::Direction8::NorthEast:	return static_cast<int>(TS::Map::Direction8::SouthWest);
		case TS::Map::Direction8::North:	return static_cast<int>(TS::Map::Direction8::South);
		case TS::Map::Direction8::NorthWest:	return static_cast<int>(TS::Map::Direction8::SouthEast);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  ���΍��W����S������Ԃ�  ####################
int CSceneMap::PointToDirection4(const CPoint& pos) {
	if(pos.X < 0 && (pos.Y == 0 || abs(pos.X) >= abs(pos.Y))) {
		return static_cast<int>(TS::Map::Direction4::West);
	} else if(pos.X > 0 && (pos.Y == 0 || abs(pos.X) > abs(pos.Y))) {
		return static_cast<int>(TS::Map::Direction4::East);
	} else if((pos.X == 0 || abs(pos.X) <= abs(pos.Y)) && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction4::North);
	} else if((pos.X == 0 || abs(pos.X) < abs(pos.Y)) && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction4::South);
	}
	return static_cast<int>(TS::Map::Direction4::None);
}


//####################  ���΍��W����W������Ԃ�  ####################
int CSceneMap::PointToDirection8(const CPoint& pos) {
	if(pos.X < 0 && pos.Y == 0) {
		return static_cast<int>(TS::Map::Direction8::West);
	} else if(pos.X > 0 && pos.Y == 0) {
		return static_cast<int>(TS::Map::Direction8::East);
	} else if(pos.X == 0 && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction8::North);
	} else if(pos.X == 0 && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction8::South);
	} else if(pos.X < 0 && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction8::NorthWest);
	} else if(pos.X > 0 && pos.Y < 0) {
		return static_cast<int>(TS::Map::Direction8::NorthEast);
	} else if(pos.X < 0 && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction8::SouthWest);
	} else if(pos.X > 0 && pos.Y > 0) {
		return static_cast<int>(TS::Map::Direction8::SouthEast);
	}
	return static_cast<int>(TS::Map::Direction8::None);
}


//####################  �w�肵�������ւ̂P�^�C�����̑��΍��W��Ԃ�  ####################
CPoint CSceneMap::Direction4ToPoint(int direction4) {
	switch(static_cast<TS::Map::Direction4>(direction4)) {
		case TS::Map::Direction4::East:		return CPoint(+1, 0);
		case TS::Map::Direction4::West:		return CPoint(-1, 0);
		case TS::Map::Direction4::South:	return CPoint(0, +1);
		case TS::Map::Direction4::North:	return CPoint(0, -1);
	}
	return CPoint();
}


//####################  �w�肵�������ւ̂P�^�C�����̑��΍��W��Ԃ�  ####################
CPoint CSceneMap::Direction8ToPoint(int direction8) {
	switch(static_cast<TS::Map::Direction8>(direction8)) {
		case TS::Map::Direction8::East:			return CPoint(+1, 0);
		case TS::Map::Direction8::SouthEast:	return CPoint(+1, +1);
		case TS::Map::Direction8::SouthWest:	return CPoint(-1, +1);
		case TS::Map::Direction8::West:			return CPoint(-1, 0);
		case TS::Map::Direction8::South:		return CPoint(0, +1);
		case TS::Map::Direction8::North:		return CPoint(0, -1);
		case TS::Map::Direction8::NorthEast:	return CPoint(+1, -1);
		case TS::Map::Direction8::NorthWest:	return CPoint(-1, -1);
	}
	return CPoint();
}


//####################  �l�s���s�F�C�x���g�y�[�W����  ####################
bool EVPage(TS_MT) {
	//�}�b�v�V�[���łȂ��Ƃ��͏������Ȃ�
	if(Game.IsSubScene() || Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return false;
	}

	//NOTE: �C�x���g���Ƀt���O���ς�����Ƃ��Ƀy�[�W���ς�����悤�ɁA�ȉ��̔���͔p�~�����B
	//�C�x���g���s���͔��肵�Ȃ�
	//if(Game.getSceneMap()->MapData.getIsEVRunning()) {
	//	return false;
	//}

	//�S�C�x���g�𑖍�
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		if(ev.IsDummy) {
			continue;		//�_�~�[�C�x���g�͑ΏۊO
		}

		//�����y�[�W���画��
		bool found = false;
		for(int pageIndex = ev.GetPageCount() - 1; pageIndex >= 0; pageIndex--) {
			//������s
			if(Game.getSceneMap()->MapData.RunEV(ev.ID, pageIndex, static_cast<int>(TS::Map::EventScriptType::Page), false)) {
				//true���Ԃ��ꂽ��L���ȃy�[�W�ŁA�y�[�W�C���f�b�N�X���ύX�ɂȂ�ꍇ�͂����œ���ړ��̓��e���X�V����
				found = true;
				if(pageIndex != ev.GetPageIndex()) {
					ev.SetPageIndex(pageIndex);
				}
				break;
			}
		}
		if(!found) {
			//��o����Ԃɂ���Ɨl�X�ȕ��Q���N���邽�߁A�ŏ��̃y�[�W�ŃZ�b�g����
			ev.SetPageIndex(0);
			
			//��������������Ȃ���Δ�o����Ԃɂ���
			//ev.SetPageIndex(-1);
		}
	}
	return false;
}


//####################  �l�s���s�F�C�x���g�N������  ####################
bool EVTrigger(TS_MT) {
	//�}�b�v�V�[���łȂ��Ƃ��͏������Ȃ�
	if(Game.IsSubScene() || Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return false;
	}
	//�C�x���g���s���͔��肵�Ȃ�
	if(Game.getSceneMap()->MapData.getIsEVRunning()) {
		return false;
	}

	//�\�񂳂�Ă���C�x���g������΂�����Ɏ��s����
	int EVID;
	while((EVID = Game.getSceneMap()->MapData.PopReserveEVID()) != -1) {
		Game.getSceneMap()->MapData.RunEV(EVID, static_cast<int>(TS::Map::EventScriptType::Run), true);
	}

	//�S�C�x���g�𑖍��F�����E������s�݂̂𔻒�
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		auto pg = ev.getCurrentPage();
		if(pg == nullptr) {
			//���o���C�x���g�͏��O
			continue;
		} else if(pg->TriggerType != static_cast<int>(TS::Map::EventTriggerType::Auto) && pg->TriggerType != static_cast<int>(TS::Map::EventTriggerType::Parallel)) {
			//�ΏۊO�̋N�������͏��O
			continue;
		}

		//������s
		if(Game.getSceneMap()->MapData.RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Trigger), false)) {
			//true���Ԃ��ꂽ��N������
			if(ev.getCurrentPage()->TriggerType == static_cast<int>(TS::Map::EventTriggerType::Parallel)) {
				//������s�C�x���g�͎��s���̃C�x���gID�X�^�b�N�ɓo�^���Ȃ��i�������~���Ȃ��j
				Game.getSceneMap()->MapData.RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Run), false);
			} else {
				//�������s�C�x���g�͎��s���̃C�x���gID�X�^�b�N�ɓo�^����i�������~����j
				Game.getSceneMap()->MapData.RunEV(ev.ID, static_cast<int>(TS::Map::EventScriptType::Run), true);
			}
		}
	}
	return false;
}


//####################  �l�s���s�F�C�x���g����ړ�  ####################
bool EVMove(TS_MT) {
	int temp;

	//�}�b�v�V�[���łȂ��Ƃ��͏������Ȃ�
	if(Game.IsSubScene() || Game.GetCurrentMainSceneID() != static_cast<int>(TS::System::Scene::Map)) {
		return false;
	}

	//�v���C���[���ړ����̂Ƃ��͂��̈ړ����p������
	auto& player = Game.getSceneMap()->MapData.Player;
	if(player.MapObject.getIsMoving()) {
		//�ړ����̏ꍇ�͂��̈ړ����p������E��l���̈ړ��ɍ��킹���X�N���[���̃J�E���^�[�������œ�����
		int delta;

		delta = 0;
		if(player.MapObject.MoveCounter.X < 0) {
			delta = static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.X = min(0, player.MapObject.MoveCounter.X + delta);
		} else if(player.MapObject.MoveCounter.X > 0) {
			delta = -static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.X = max(0, player.MapObject.MoveCounter.X + delta);
		}
		Game.getSceneMap()->MapData.PictureScrollCounter.X += delta;

		delta = 0;
		if(player.MapObject.MoveCounter.Y < 0) {
			delta = static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.Y = min(0, player.MapObject.MoveCounter.Y + delta);
		} else if(player.MapObject.MoveCounter.Y > 0) {
			delta = -static_cast<int>(pow(2.0, player.CharData.MoveSpeed));
			player.MapObject.MoveCounter.Y = max(0, player.MapObject.MoveCounter.Y + delta);
		}
		Game.getSceneMap()->MapData.PictureScrollCounter.Y += delta;

		//�ړ����I���������v�N���C�x���g�̔�����s��
		if(!player.MapObject.getIsMoving() && !Game.getSceneMap()->MapData.getIsEVRunning()) {
			Game.getSceneMap()->MapData.CheckOverlapTriggerEV(-1, player.MapObject.Position.X, player.MapObject.Position.Y);
		}
	}

	//�S�C�x���g�𑖍�
	for(auto& ev : Game.getSceneMap()->MapData.EVs.Items) {
		auto pg = ev.getCurrentPage();
		if(pg == nullptr) {
			//���o���C�x���g�͏��O
			continue;
		}

		if(ev.getIsMoving() && !ev.ForbiddenAutoMove) {
			//�ړ��J�E���^�[�̎����������L���ŁA���ړ����̂Ƃ��͂��̈ړ����p������
			if(ev.MoveCounter.X < 0) {
				ev.MoveCounter.X = min(0, ev.MoveCounter.X + (int)pow(2.0, pg->MoveSpeed));
			} else if(ev.MoveCounter.X > 0) {
				ev.MoveCounter.X = max(0, ev.MoveCounter.X - (int)pow(2.0, pg->MoveSpeed));
			}
			if(ev.MoveCounter.Y < 0) {
				ev.MoveCounter.Y = min(0, ev.MoveCounter.Y + (int)pow(2.0, pg->MoveSpeed));
			} else if(ev.MoveCounter.Y > 0) {
				ev.MoveCounter.Y = max(0, ev.MoveCounter.Y - (int)pow(2.0, pg->MoveSpeed));
			}

			//�ړ����I���������v�N���C�x���g�̔�����s��
			if(!ev.getIsMoving() && !Game.getSceneMap()->MapData.getIsEVRunning()) {
				Game.getSceneMap()->MapData.CheckOverlapTriggerEV(ev.ID, ev.Position.X, ev.Position.Y);
			}
		}

		//�C�x���g���s���łȂ��Ƃ��̂݁A�V�K�ړ����s��
		if(!ev.getIsMoving() && !Game.getSceneMap()->MapData.getIsEVRunning()) {
			if(ev.getCurrentPage()->MoveType == static_cast<int>(TS::Map::EventMoveType::Script)) {
				//�X�N���v�g�w��̈ړ����s��
				auto task = ev.getNextMoveTask();
				if(task != nullptr) {
					ev.MoveTaskNowIndex = ev.moveScript(*task, ev.MoveTaskNowIndex, true);
				}
			} else {
				//�K��E�F�C�g���Ԃ܂ŒB������T�^�ړ����J�n����
				ev.MoveWaitTimer++;

				//�ړ��p�x�ɉ���������: �҂����ԃt���[�������Z�o
				temp = TS::Map::EVMoveFrequencyFrameLatest - (ev.getCurrentPage()->MoveFrequency * TS::Map::EVMoveFrequencyFrameLatest / (static_cast<int>(TS::Map::Speed::Moment) + 1));

				if(temp <= ev.MoveWaitTimer) {
					switch(static_cast<TS::Map::EventMoveType>(ev.getCurrentPage()->MoveType)) {
						case TS::Map::EventMoveType::Random:
							ev.moveRandom();
							break;
						case TS::Map::EventMoveType::Near:
							ev.moveNear();
							break;
						case TS::Map::EventMoveType::Away:
							ev.moveAway();
							break;
					}
					ev.MoveWaitTimer = 0;

					if(ev.getIsMoving()) {
						//�}�b�v�͈̔͊O�ɏo����}�b�v�̃��[�v�ݒ�ɏ]���ĕ␳����
						ev.ApplyMapLoopPosition();
					}
				}
			}
		}
	}
	return false;
}


//####################  �l�s���s�F�A�j���[�V����  ####################
//Vint["counter"]
bool Anim(TS_MT) {
	int animSpeed;
	const int FrequencyFrameLatest = TS::Map::EVMoveFrequencyFrameLatest / 2;

	//�}�b�v�^�C���̃A�j���[�V�����p�^�[�����̓^�C�����ƂɈقȂ邽�߁A�`�掞�Ɍv�Z����
	*Data.GetInteger("counter") = Game.UpAround(*Data.GetInteger("counter"), 0, INT_MAX);
	Game.getSceneMap()->MapData.setAnimCounter(*Data.GetInteger("counter"));

	//��l���E�C�x���g�̃A�j���[�V�������s��
	for(int i = -1; i < Game.getSceneMap()->MapData.EVs.GetCount(); i++) {
		CSceneMap::CMapData::CMapObjectData* ev;
		CSceneMap::CMapData::CMapCharObjectData* pg;
		int frequency;
		if(i == -1) {
			//��l���̏����Z�b�g
			ev = &Game.getSceneMap()->MapData.Player.MapObject;
			pg = &Game.getSceneMap()->MapData.Player.CharData;
			frequency = static_cast<int>(TS::Map::Speed::Normal);
		} else {
			//�C�x���g�̏����Z�b�g
			ev = Game.getSceneMap()->MapData.EVs.GetFromIndex(i);
			pg = Game.getSceneMap()->MapData.EVs.GetFromIndex(i)->getCurrentPage();
			if(pg == nullptr || dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(pg) == nullptr) {
				continue;
			}
			frequency = dynamic_cast<CSceneMap::CMapData::CMapEventData::CMapEventPageData*>(pg)->MoveFrequency;
		}

		if(!ev->getIsMoving()) {
			//�Î~�A�j���[�V����
			if(!pg->EnabledStopAnim) {
				//�A�j���[�V�������s��Ȃ��ꍇ�͏�����Ԃ�ۂ��A����ȊO�͐Î~��Ԃɂ���
				if(pg->EnabledWalkAnim) {
					pg->AnimID = static_cast<int>(TS::Map::EventAnimation::Stop);
				}
				continue;
			}
			animSpeed = (FrequencyFrameLatest - (FrequencyFrameLatest * frequency / (static_cast<int>(TS::Map::Speed::Moment) + 1)));
			pg->AnimID = ((*Data.GetInteger("counter") % animSpeed) / (animSpeed / 2) == 0) ? static_cast<int>(TS::Map::EventAnimation::Move1) : static_cast<int>(TS::Map::EventAnimation::Move2);
		} else {
			//���s�A�j���[�V����
			if(!pg->EnabledWalkAnim) {
				//�A�j���[�V�������s��Ȃ��ꍇ�͏�����Ԃ�ۂ��A����ȊO�͐Î~��Ԃɂ���
				if(pg->EnabledStopAnim) {
					pg->AnimID = static_cast<int>(TS::Map::EventAnimation::Stop);
				}
				continue;
			}
			animSpeed = (FrequencyFrameLatest - (FrequencyFrameLatest * static_cast<int>(TS::Map::Speed::Moment) / (static_cast<int>(TS::Map::Speed::Moment) + 1)));
			pg->AnimID = ((*Data.GetInteger("counter") % animSpeed) / (animSpeed / 2) == 0) ? static_cast<int>(TS::Map::EventAnimation::Move1) : static_cast<int>(TS::Map::EventAnimation::Move2);
		}
	}

	return false;
}


//####################  �R�[���o�b�N�֐��F�t���X�N���[�����[�h�ؑւ�MakeScreen�����O���t�B�b�N��������̂ŕ�������  ####################
void RestoreTilePallet() {
	DxLib::ReloadFileGraphAll();			//�������摜�f�[�^�����߂�
	if(Game.getSceneMap() != nullptr) {
		Game.getSceneMap()->MapData.TilePallet = Game.DB.getFDBTileset()->LoadTileset(Game.getSceneMap()->MapData.TilesetID);
	}
}
