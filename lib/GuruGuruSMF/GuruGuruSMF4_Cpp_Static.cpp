
//#include "stdafy.h"
#include "GuruGuruSMF4_Cpp.h"
#pragma warning(disable:4717)	//167�s�ڂ̊֐�����A�����Ƃ��ɔ�������X�^�b�N�I�[�o�[�t���[�̊댯���ɂ���

using namespace GuruGuruSmf;

extern "C" {
	GGSERROR WINAPI GGS4OpenDevice(int deviceId, HWND handle);
	void WINAPI GGS4CloseDevice();
	GGSERROR WINAPI GGS4AddDlsW(LPCWSTR fileName);
	GGSERROR WINAPI GGS4AddDlsA(LPCSTR fileName);
	GGSERROR WINAPI GGS4Play(int option, int musicId, int seek, int fadeInTime, int keyShift);
	void WINAPI GGS4Stop(int fadeOutTime);
	void WINAPI GGS4Pause();
	void WINAPI GGS4Restart();
	void WINAPI GGS4SetMasterTempo(float tempo);
	void WINAPI GGS4SetMasterVolume(int volume);
	void WINAPI GGS4SetMasterPitch(int pitch);
	void WINAPI GGS4GetPlayerStatus(PlayerStatus* status, int size);
	GGSERROR WINAPI GGS4AddListFromFileW(LPCWSTR fileName, int option, int musicId);
	GGSERROR WINAPI GGS4AddListFromFileA(LPCSTR fileName, int option, int musicId);
	GGSERROR WINAPI GGS4AddListFromMemory(LPCBYTE smfBinary, int length, int option, int musicId);
	void WINAPI GGS4DeleteListItem(int musicId);
	void WINAPI GGS4ClearList();
	const SmfListKeys* WINAPI GGS4EnumList();
	GGSERROR WINAPI GGS4GetSmfInformation(SmfInformation* info, int size, int musicId);
}

namespace GuruGuruSmf {

	Ggs4* Ggs4::instance = 0;	// �C���X�^���X�u����

	// �C���X�^���X�擾(Singleton)
	Ggs4* Ggs4::GetInstance() {
		if(instance == 0) {
			instance = new Ggs4();
		}
		return instance;
	}

	// �C���X�^���X�j��
	void Ggs4::FreeInstance() {
		delete instance;
		instance = 0;
	}

	// ������(static�łł͉������Ȃ�)
	GGSERROR Ggs4::Initialize() {
		return GgsError::NoError;
	}

	// �f�X�g���N�^
	Ggs4::~Ggs4() {
		CloseDevice();
	}

	// �f�o�C�X���J��
	GGSERROR Ggs4::OpenDevice(int deviceId, HWND handle) {
		return GGS4OpenDevice(deviceId, handle);
	}

	// �f�o�C�X�����
	void Ggs4::CloseDevice() {
		GGS4CloseDevice();
	}

	// DLS��ǉ�(UNICODE)
	GGSERROR Ggs4::AddDlsW(LPCWSTR fileName) {
		return GGS4AddDlsW(fileName);
	}

	// DLS��ǉ�(Ansi)
	GGSERROR Ggs4::AddDlsA(LPCSTR fileName) {
		return GGS4AddDlsA(fileName);
	}

	// ���t�I
	GGSERROR Ggs4::Play(int option, int musicId, int seek, int fadeInTime, int keyShift) {
		return GGS4Play(option, musicId, seek, fadeInTime, keyShift);
	}

	// ��~
	void Ggs4::Stop(int fadeOutTime) {
		GGS4Stop(fadeOutTime);
	}

	// �ꎞ��~
	void Ggs4::Pause() {
		GGS4Pause();
	}

	// �ꎞ��~����
	void Ggs4::Restart() {
		GGS4Restart();
	}

	// �}�X�^�[�e���|
	void Ggs4::SetMasterTempo(float tempo) {
		GGS4SetMasterTempo(tempo);
	}

	// �}�X�^�[�{�����[��
	void Ggs4::SetMasterVolume(int volume) {
		GGS4SetMasterVolume(volume);
	}

	// �}�X�^�[�s�b�`
	void Ggs4::SetMasterPitch(int pitch) {
		GGS4SetMasterPitch(pitch);
	}

	// ���t���擾
	void Ggs4::GetPlayerStatus(PlayerStatus* status) {
		GGS4GetPlayerStatus(status, sizeof(PlayerStatus));
	}

	// �t�@�C�����烊�X�g�֒ǉ�(UNICODE)
	GGSERROR Ggs4::AddListFromFileW(LPCWSTR fileName, int option, int musicId) {
		return GGS4AddListFromFileW(fileName, option, musicId);
	}

	// �t�@�C�����烊�X�g�֒ǉ�(Ansi)
	GGSERROR Ggs4::AddListFromFileA(LPCSTR fileName, int option, int musicId) {
		return GGS4AddListFromFileA(fileName, option, musicId);
	}

	// ���������烊�X�g�֒ǉ�
	GGSERROR Ggs4::AddListFromMemory(LPCBYTE smfBinary, int length, int option, int musicId) {
		return GGS4AddListFromMemory(smfBinary, length, option, musicId);
	}

	// ���X�g����1�폜
	void Ggs4::DeleteListItem(int musicId) {
		GGS4DeleteListItem(musicId);
	}

	// ���X�g���N���A
	void Ggs4::ClearList() {
		GGS4ClearList();
	}

	// ���X�g���
	const SmfListKeys* Ggs4::EnumList() {
		return EnumList();
	}

	// SMF�̏����擾
	GGSERROR Ggs4::GetSmfInformation(SmfInformation* info, int musicId) {
		return GGS4GetSmfInformation(info, sizeof(SmfInformation), musicId);
	}

}



