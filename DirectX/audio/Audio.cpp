#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

std::vector<Audio::SoundData*> Audio::SData;
Microsoft::WRL::ComPtr<IXAudio2>Audio::xAudio2;
IXAudio2MasteringVoice* Audio::masterVoice;

Audio::Audio()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);//XAudio�G���W���̃C���X�^���X����
	xAudio2->CreateMasteringVoice(&masterVoice);//�}�X�^�[�{�C�X����
	masterVoice->SetVolume(0.1f);//�S�̂̉���}
}

Audio::~Audio()
{
	//xAudio2���
	xAudio2.Reset();

	const int size = (UINT)SData.size();

	//�o�b�t�@�̃����������
	for (int i = 0; i < size; i++)
	{
		delete[] SData[i]->pBuffer;

		SData[i]->pBuffer = 0;
		SData[i]->bufferSize = 0;
		SData[i]->wfex = {};
	}

	SData.clear();
}

//�����f�[�^�ǂݍ���
int Audio::SoundLoadWave(const char* fileName)
{
	//���݂̔z��T�C�Y
	int size = (UINT)SData.size();

	SoundData* soundData = nullptr;
	soundData = new SoundData();

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fileName, std::ios_base::binary);

	//�t�@�C���I�[�v�����s�����m
	assert(file.is_open());

	///wav�f�[�^�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};

	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0 ||
		strncmp(data.id, "LIST", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);

		//�ēx�ǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	///�t�@�C���N���[�Y
	//Wave�t�@�C�������
	file.close();

	///�ǂݍ��񂾉����f�[�^��Ԃ�
	//return���邽�߂̉����f�[�^
	soundData->wfex = format.fmt;
	soundData->pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData->bufferSize = data.size;

	CreateSoundData(*soundData);
	float SourceVoiceChannelVolumes[1] = { 0.0 };
	soundData->pSourceVoice->SetChannelVolumes(1, SourceVoiceChannelVolumes);

	SData.push_back(soundData);

	return size;
}

//�f�[�^�̐���
void Audio::CreateSoundData(SoundData& soundData) {
	HRESULT result;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));
}

//�����Đ�
bool Audio::SoundPlayWava(int number, bool roop) {
	HRESULT result;

	XAUDIO2_VOICE_STATE xa3state;
	SData[number]->pSourceVoice->GetState(&xa3state);
	//0�łȂ��Ȃ特�y���Đ����ɂȂ邽�߃X���[����
	if (xa3state.BuffersQueued != 0)
	{
		return false;
	}

	//�f�[�^����
	CreateSoundData(*SData[number]);

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = SData[number]->pBuffer;
	buf.AudioBytes = SData[number]->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (roop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//�g�`�f�[�^�̍Đ�
	result = SData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	result = SData[number]->pSourceVoice->Start();

	buf = { NULL };

	return true;
}

//���y�̒�~
void Audio::StopSound(int number)
{
	SData[number]->pSourceVoice->Stop(0);
	SData[number]->pSourceVoice->FlushSourceBuffers();
	SData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
}