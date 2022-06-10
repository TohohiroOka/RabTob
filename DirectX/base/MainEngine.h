#pragma once
#include "DirectXCommon.h"
#include "SceneManager.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include "PostEffect.h"
#include "FrameRateKeep.h"
#include "CubeMap.h"

#include <sstream>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class WindowApp;

class MainEngine
{
public:

	MainEngine() = default;
	~MainEngine();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <returns>�Q�[���𑱂��邩</returns>
	bool Update();

	/// <summary>
	/// �f�o�b�O�p����
	/// </summary>
	/// <param name="x">�l1</param>
	/// <param name="y">�l2</param>
	/// <param name="z">�l3</param>
	void debugNum(float x, float y, float z);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �E�B���h�E�N���X�̃C���X�^���X
	/// </summary>
	void FrameControl(WindowApp* winApp);

private:

	//DirectXCommon�̃C���X�^���X
	std::unique_ptr<DirectXCommon> dXCommon = nullptr;
	//Input�̃C���X�^���X
	DirectInput* input = nullptr;
	//XInputManager
	XInputManager* Xinput = nullptr;
	//GameScene�̃C���X�^���X
	std::unique_ptr<SceneManager> scene = nullptr;
	//�|�X�g�G�t�F�N�g�̃C���X�^���X
	std::unique_ptr<PostEffect> postEffect = nullptr;
	//Fps�Œ�p�N���X�̃C���X�^���X
	std::unique_ptr<FrameRateKeep> fps = nullptr;
	//�L���[�u�}�b�v
	std::unique_ptr<CubeMap> cubemap;
	//�����\���f�o�b�O�p
	wchar_t str[256] = {};
};