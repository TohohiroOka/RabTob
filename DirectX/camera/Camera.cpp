#include "Camera.h"
#include "WindowApp.h"

using namespace DirectX;

Camera::Camera()
{
	aspectRatio = (float)WindowApp::GetWindowWidth() / WindowApp::GetWindowHeight();

	// �ˉe�s��̌v�Z
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1200.0f//���s/��O,�ŉ�
	);
}

std::unique_ptr<Camera> Camera::Create()
{
	// Sprite�̃C���X�^���X�𐶐�
	Camera* instance = new Camera();
	if (instance == nullptr) {
		return nullptr;
	}

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<Camera>(instance);
}

void Camera::Update()
{
	XMFLOAT3 inoutEye = { ShakeDifference.x + eye.x,ShakeDifference.y + eye.y,ShakeDifference.z + eye.z };

	matView = XMMatrixLookAtLH(XMLoadFloat3(&inoutEye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::StartCameraShake(int strength)
{
	int x = (rand() % strength * 2) - strength;
	int y = (rand() % strength * 2) - strength;

	ShakeDifference = { (float)x ,(float)y,0.0f };
}

void Camera::EndCameraShake()
{
	ShakeDifference = { 0.0f,0.0f,0.0f };
}