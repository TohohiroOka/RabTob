#include "SceneManager.h"
#include "TestField.h"

std::unique_ptr<InterfaceScene> SceneManager::scene = nullptr;
InterfaceScene* SceneManager::nextScene = nullptr;

std::unique_ptr<SceneManager> SceneManager::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	SceneManager* instance = new SceneManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();

	return std::unique_ptr<SceneManager>(instance);
}

SceneManager::~SceneManager()
{
	scene.reset();
}

void SceneManager::Initialize()
{
	//�J�����̏�����
	camera = Camera::Create();
	//�T�E���h�p
	audio = std::make_unique<Audio>();
	//���C�g
	light = LightGroup::Create();
	//light->SetDirLightActive(0, true);
	//light->SetDirLightColor(0, { 1,1,1 });
	//light->SetDirLightDir(0, { 0.0f, 0.0f, 1.0f, 0 });
	light->DefaultLightSetting();
	//light->SetPointLightActive(0, true);
	//light->SetPointLightPos(0, { -100,0,-100 });
	//light->SetPointLightColor(0, { 1,1,1 });
	//light->SetPointLightAtten(0, { 0.001f,0.00023f,0.000001f });

	//�ŏ��̃V�[���ݒ�
	TestField* firstScene = new TestField();
	firstScene->Initialize();
	scene = std::unique_ptr<TestField>(firstScene);
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�
	if (nextScene)
	{
		if (scene)
		{
			scene.reset();
			Sprite::SceneFinalize();
			ParticleManager::SceneFinalize();
		}

		//�V�[���؂�ւ�
		scene = std::unique_ptr<InterfaceScene>(nextScene);
		nextScene = nullptr;

		//������
		scene->Initialize();
	}

	//�J�����̃Z�b�g
	Object3d::SetCamera(camera.get());
	Fbx::SetCamera(camera.get());
	DrawLine3D::SetCamera(camera.get());
	ParticleManager::SetCamera(camera.get());
	scene->SetCamera(camera.get());
	CubeMap::SetCamera(camera.get());
	//���C�g
	light->Update();
	scene->SetLight(light.get());

	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(light.get());
	Fbx::SetLightGroup(light.get());

	camera->Update();
	scene->Update();
}

void SceneManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	scene->SetCmdList(cmdList);
	scene->Draw();
}

void SceneManager::ImguiDraw()
{
	scene->ImguiDraw();
}

void SceneManager::GetConstbufferNum()
{
	scene->GetConstbufferNum();
}