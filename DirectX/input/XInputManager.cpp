#include "XInputManager.h"

//�X�e�B�b�N����
#define INPUT_DEADZONE  ( 0.41f * FLOAT(0x7FFF) )
//�X�e�B�b�N�̏��
const int STICK_MAX = 32767;

XInputManager::~XInputManager()
{
#pragma warning(disable:4995)
	XInputEnable(FALSE);
}

XInputManager* XInputManager::GetInstance()
{
	static XInputManager instance;

	return &instance;
}

void XInputManager::Initialize()
{
#pragma warning(disable:4995)
	XInputEnable(TRUE);
}

void XInputManager::Update()
{
	g_Controllers.lastState = g_Controllers.state;
	g_Controllers.dwResult = XInputGetState(0, &g_Controllers.state);
	if (g_Controllers.state.Gamepad.bLeftTrigger < -STICK_MAX) { g_Controllers.state.Gamepad.bLeftTrigger = -STICK_MAX; }
}

bool XInputManager::PushButton(XInputManager::PUD_BUTTON button)
{
	bool check = false;
	if (button == PUD_BUTTON::PAD_A &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_B &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_Y &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_X &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LT &&
		g_Controllers.state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RT &&
		g_Controllers.state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_START &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_BUCK &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_UP &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_DOWN &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LEFT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RIGHT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LEFT_STICK_PUSH &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RIGHT_STICK_PUSH &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		check = true;
	}
	return check;
}

bool XInputManager::TriggerButton(PUD_BUTTON button)
{
	bool check = false;
	if (button == PUD_BUTTON::PAD_A &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_B &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_Y &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_X &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RB &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LT &&
		g_Controllers.state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		g_Controllers.lastState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RT &&
		g_Controllers.state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		g_Controllers.lastState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_START &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_START &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_START) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_BUCK &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_UP &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_DOWN &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_LEFT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) == false)
	{
		check = true;
	} else if (button == PUD_BUTTON::PAD_RIGHT &&
		g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT &&
		(g_Controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) == false)
	{
		check = true;
	}
	return check;
}

bool XInputManager::LeftStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbLX < -INPUT_DEADZONE)
	{
		return true;
	} else if (LeftRight == false && g_Controllers.state.Gamepad.sThumbLX > INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::LeftStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbLY > INPUT_DEADZONE)
	{
		return true;
	} else if (UpDown == false && g_Controllers.state.Gamepad.sThumbLY < -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::RightStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbRX < -INPUT_DEADZONE)
	{
		return true;
	} else if (LeftRight == false && g_Controllers.state.Gamepad.sThumbRX > INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::RightStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbRY > INPUT_DEADZONE)
	{
		return true;
	} else if (UpDown == false && g_Controllers.state.Gamepad.sThumbRY < -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerLeftStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbLX < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLX >= -INPUT_DEADZONE)
	{
		return true;
	} if (LeftRight == false && g_Controllers.state.Gamepad.sThumbLX > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLX <= INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerLeftStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbLY > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLY <= INPUT_DEADZONE)
	{
		return true;
	} else if (UpDown == false && g_Controllers.state.Gamepad.sThumbLY < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbLY >= -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerRightStickX(bool LeftRight)
{
	if (LeftRight == true && g_Controllers.state.Gamepad.sThumbRX < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRX >= -INPUT_DEADZONE)
	{
		return true;
	}if (LeftRight == false && g_Controllers.state.Gamepad.sThumbRX > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRX <= INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerRightStickY(bool UpDown)
{
	if (UpDown == true && g_Controllers.state.Gamepad.sThumbRY > INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRY <= INPUT_DEADZONE)
	{
		return true;
	} else if (UpDown == false && g_Controllers.state.Gamepad.sThumbRY < -INPUT_DEADZONE &&
		g_Controllers.lastState.Gamepad.sThumbRY >= -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

DirectX::XMFLOAT2 XInputManager::GetPadLStickIncline()
{
	return DirectX::XMFLOAT2((float)g_Controllers.state.Gamepad.sThumbLX / STICK_MAX, (float)g_Controllers.state.Gamepad.sThumbLY / STICK_MAX);
}

DirectX::XMFLOAT2 XInputManager::GetPadRStickIncline()
{
	return DirectX::XMFLOAT2((float)g_Controllers.state.Gamepad.sThumbRX / STICK_MAX, (float)g_Controllers.state.Gamepad.sThumbRY / STICK_MAX);
}

float XInputManager::GetPadLStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadLStickIncline();
	float h = pad.x;
	float v = -pad.y;

	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	if (radian < 0)
	{
		radian += 360;
	}

	//�E�����Ȃ̂ŏ�����ɒ���
	radian += 90;

	return radian;
}

float XInputManager::GetPadRStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadRStickIncline();
	float h = pad.x;
	float v = -pad.y;

	//������� 0 (360)�ɂȂ�悤�ɂ���
	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	if (radian < 0)
	{
		radian += 360;
	}

	//�E�����Ȃ̂ŏ�����ɒ���
	radian += 90;

	return radian;
}

void XInputManager::StartVibration(STRENGTH strength)
{
	g_Controllers.vibration.wLeftMotorSpeed = strength;
	g_Controllers.vibration.wRightMotorSpeed = strength;

	XInputSetState(0, &g_Controllers.vibration);
}

void XInputManager::EndVibration()
{
	g_Controllers.vibration.wLeftMotorSpeed = 0;
	g_Controllers.vibration.wRightMotorSpeed = 0;

	XInputSetState(0, &g_Controllers.vibration);
}
