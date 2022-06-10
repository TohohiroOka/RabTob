#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3dx12.h>

class Audio
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];//�`�����N����ID
		int	size;  // �`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;//"RIFF"
		char type[4];//"WAVE"
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;//"fmt"
		WAVEFORMATEX fmt;//�g�`�t�H�[�}�b�g
	};

	//�����f�[�^
	struct SoundData
	{
		WAVEFORMATEX wfex;//�g�`�t�H�[�}�b�g
		BYTE* pBuffer;//�o�b�t�@�̐擪�A�h���X
		unsigned int bufferSize;//�o�b�t�@�̃T�C�Y
		IXAudio2SourceVoice* pSourceVoice;
	};


public:

	Audio();
	~Audio();

	//�����f�[�^�ǂݍ���
	int SoundLoadWave(const char* fileName);
	
	//�f�[�^�̐���
	void CreateSoundData(SoundData& soundData);

	//�����Đ�
	bool SoundPlayWava(int number, bool roop);

	//�T�E���h�̒�~
	void StopSound(int number);

private:
	//�T�E���h�f�[�^�z��
	static std::vector<SoundData*> SData;

	//�T�E���h�̃o�b�t�@
	XAUDIO2_BUFFER buf{};

	static ComPtr<IXAudio2>xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
};