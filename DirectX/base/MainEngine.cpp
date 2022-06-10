#include "MainEngine.h"
#include "DrawLine.h"
#include "DrawLine3D.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Emitter.h"
#include "Fbx.h"
#include "SafeDelete.h"
#include "ComputeShaderManager.h"
#include "GraphicsPipelineManager.h"
#include "Texture.h"

using namespace DirectX;
using namespace Microsoft::WRL;

MainEngine::~MainEngine()
{
	DebugText::Finalize();
	scene.reset();
	Object3d::Finalize();
	DrawLine3D::Finalize();
	DrawLine::Finalize();
	Sprite::Finalize();
	Fbx::Finalize();
	CubeMap::Finalize();
	ParticleManager::Finalize();
	postEffect->Finalize();
	ComputeShaderManager::Finalize();
	DescriptorHeapManager::Finalize();
}

void MainEngine::Initialize()
{
	//directX������
	dXCommon = DirectXCommon::Create();

	//key
	input = DirectInput::GetInstance();
	input->Initialize();

	//�p�b�h
	Xinput = XInputManager::GetInstance();
	Xinput->Initialize();

	//Object�n�̏�����
	Texture::StaticInitialize(dXCommon->GetDevice());
	GraphicsPipelineManager::StaticInitialize(dXCommon->GetDevice());
	Object3d::StaticInitialize(dXCommon->GetDevice());
	Sprite::StaticInitialize(dXCommon->GetDevice());
	DrawLine::StaticInitialize(dXCommon->GetDevice());
	DrawLine3D::StaticInitialize(dXCommon->GetDevice());
	ParticleManager::StaticInitialize(dXCommon->GetDevice());
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	Fbx::StaticInitialize(dXCommon->GetDevice());
	PostEffect::StaticInitialize();
	ComputeShaderManager::StaticInitialize(dXCommon->GetDevice());
	DebugText::GetInstance()->Initialize();
	CubeMap::StaticInitialize(dXCommon->GetDevice());

	scene = SceneManager::Create();

	postEffect = PostEffect::Create();

	fps = FrameRateKeep::Create();

	cubemap = CubeMap::Create(dXCommon->GetCmdList());
}

bool MainEngine::Update()
{
	input->Update();
	Xinput->Update();

	//�G�X�P�[�v���~�������ꂽ�Ƃ��Q�[���I��
	if (input->PushKey(DIK_ESCAPE)) { return true; }

	//�X�V
	Fbx::SetCubeTex(cubemap->SetTexture());
	scene->Update();
	cubemap->Update();

	return false;
}

void MainEngine::Draw()
{
	//�`��
	postEffect->PreDrawScene(dXCommon->GetCmdList());
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());
	CubeMap::PreDraw(dXCommon->GetCmdList());
	cubemap->Draw();
	CubeMap::PostDraw();

	scene->Draw(dXCommon->GetCmdList());
	postEffect->PostDrawScene(dXCommon->GetCmdList());

	//�`��O�ݒ�
	dXCommon->PreDraw();

	//imgui�\��
	scene->ImguiDraw();

	//�|�X�g�G�t�F�N�g�`��
	postEffect->Draw(dXCommon->GetCmdList());

	//�R�}���h���s
	dXCommon->PostDraw();
}

void MainEngine::debugNum(float x, float y, float z)
{
	//�����̃f�o�b�O
	swprintf_s(str, L"%f,%f,%f\n", x, y, z);
	OutputDebugString(str);
}

void MainEngine::FrameControl(WindowApp* winApp)
{
	fps->FixedFps(winApp);
}