#include "DirectInput.h"
#include "WindowApp.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

DirectInput* DirectInput::GetInstance()
{
	static DirectInput instance;

	return &instance;
}

void DirectInput::Initialize()
{
	HRESULT result;

	//DirectInput�̃C���X�^���X����
	result = DirectInput8Create(
		WindowApp::GetWinInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
	}

	///-----------------�L�[���͏�����-----------------///

	//�L�[�{�[�h�f�o�C�X�̐ݒ�
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
	}

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	if (FAILED(result)) {
		assert(0);
	}

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		WindowApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
	}


	///----------------�}�E�X���͏�����----------------///

	//�}�E�X�f�o�C�X�̐ݒ�
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result)) {
		assert(0);
	}

	//���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse);	//�W���`��
	if (FAILED(result)) {
		assert(0);
	}

	//�r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(
		WindowApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
	}

}

void DirectInput::Update()
{
	HRESULT result;

	///-----------------�L�[���͍X�V-----------------///
	
	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));
	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	///----------------�}�E�X���͍X�V----------------///

	//�}�E�X���̎擾�J�n
	result = devMouse->Acquire();
	//�O��̃}�E�X���͂�ۑ�
	mousePre = mouse;
	//�}�E�X�̓��͏�Ԃ��擾����
	result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
	//�}�E�X���W���擾����
	GetCursorPos(&mousePoint);
	//�X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(WindowApp::GetHwnd(), &mousePoint);
}

bool DirectInput::PushKey(BYTE keyNumber)
{
	//�w��̃L�[�������Ă����true��Ԃ�
	if (key[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool DirectInput::TriggerKey(BYTE keyNumber)
{
	//�w��̃L�[���������u�ԂȂ�true��Ԃ�
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool DirectInput::ReleaseKey(BYTE keyNumber)
{
	//�w��̃L�[�𗣂����u�ԂȂ�true��Ԃ�
	if (!key[keyNumber] && keyPre[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool DirectInput::PushMouseButton(const int mouseButton)
{
	//�w��̃{�^���������Ă����true��Ԃ�
	if (mouse.rgbButtons[mouseButton]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool DirectInput::TriggerMouseButton(const int mouseButton)
{
	//�w��̃{�^�����������u�ԂȂ�true��Ԃ�
	if (mouse.rgbButtons[mouseButton] &&
		!mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool DirectInput::ReleaseMouseButton(const int mouseButton)
{
	//�w��̃{�^���𗣂����u�ԂȂ�true��Ԃ�
	if (!mouse.rgbButtons[mouseButton] &&
		mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

DirectX::XMFLOAT2 DirectInput::GetMousePoint()
{
	return DirectX::XMFLOAT2((float)mousePoint.x, (float)mousePoint.y);
}

DirectX::XMFLOAT2 DirectInput::GetMouseVelocity()
{
	return DirectX::XMFLOAT2((float)mouse.lX, (float)mouse.lY);
}

float DirectInput::GetMouseWheelVelocity()
{
	float wheel = mouse.lZ / 120.0f;
	return wheel;
}