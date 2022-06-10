#include "ShaderManager.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

const std::wstring pass = L"Resources/Shaders/";

//�G���[�̏o��
void OutputErrstr(ID3DBlob* errorBlob)
{
	// errorBlob����G���[���e��string�^�ɃR�s�[
	std::string errstr;
	errstr.resize(errorBlob->GetBufferSize());

	std::copy_n((char*)errorBlob->GetBufferPointer(),
		errorBlob->GetBufferSize(),
		errstr.begin());
	errstr += "\n";
	// �G���[���e���o�̓E�B���h�E�ɕ\��
	OutputDebugStringA(errstr.c_str());
	exit(1);
}

//�p�X�̎擾
std::wstring GetFullPath(LPCWSTR assetName)
{
	return pass + assetName;
}

//�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
ID3DBlob* CompileShader(const LPCWSTR fileName, LPCSTR target)
{
	HRESULT result = S_FALSE;
	//�o�͗p�I�u�W�F�N�g
	ID3DBlob* blob = nullptr;
	//�C���N���[�h�ݒ�
	ID3DInclude* includeFrag = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	// �f�o�b�O�p�ݒ�
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	//�G���[�I�u�W�F�N�g
	ID3DBlob* error;

	result = D3DCompileFromFile(GetFullPath(fileName).c_str(), nullptr, includeFrag, "main", target,
		compileFlags, 0, &blob, &error);
	if (FAILED(result))
	{
		OutputErrstr(error);
	}

	return blob;
}

std::unique_ptr<ShaderManager> ShaderManager::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ShaderManager* instance = new ShaderManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->LoadShader();

	return std::unique_ptr<ShaderManager>(instance);
}

void ShaderManager::LoadShader()
{
	HRESULT result = S_FALSE;

	//���_�V�F�[�_�[���f��
	LPCSTR vsModel = "vs_5_0";
	//�s�N�Z���V�F�[�_�[���f��
	LPCSTR psModel = "ps_5_0";
	//�W�I���g���V�F�[�_�[���f��
	LPCSTR gsModel = "gs_5_0";
	//�R���s���[�g�V�F�[�_�[���f��
	LPCSTR csModel = "cs_5_0";

	//Obj�V�F�[�_�[
	shaderObjectVS["OBJ"] = CompileShader(L"ObjVS.hlsl", vsModel);
	shaderObjectPS["OBJ"] = CompileShader(L"ObjPS.hlsl", psModel);
	//Fbx�V�F�[�_�[
	shaderObjectVS["FBX"] = CompileShader(L"FbxVS.hlsl", vsModel);
	shaderObjectPS["FBX"] = CompileShader(L"FbxPS.hlsl", psModel);
	//DrawLine3d�V�F�[�_�[
	shaderObjectVS["DRAW_LINE_3D"] = CompileShader(L"DrawLine3DVS.hlsl", vsModel);
	shaderObjectPS["DRAW_LINE_3D"] = CompileShader(L"DrawLine3DPS.hlsl", psModel);
	//NormalMap�V�F�[�_�[
	shaderObjectVS["NORMAL_MAP"] = CompileShader(L"NormalMapVS.hlsl", vsModel);
	shaderObjectPS["NORMAL_MAP"] = CompileShader(L"NormalMapPS.hlsl", psModel);
	//Sprite�V�F�[�_�[
	shaderObjectVS["SPRITE"] = CompileShader(L"SpriteVS.hlsl", vsModel);
	shaderObjectPS["SPRITE"] = CompileShader(L"SpritePS.hlsl", psModel);
	//DrawLine2d�V�F�[�_�[
	shaderObjectVS["DRAW_LINE_2D"] = CompileShader(L"DrawLine2DVS.hlsl", vsModel);
	shaderObjectPS["DRAW_LINE_2D"] = CompileShader(L"DrawLine2DPS.hlsl", psModel);
	//PostEffect�V�F�[�_�[
	shaderObjectVS["POST_EFFECT"] = CompileShader(L"PostEffectVS.hlsl", vsModel);
	shaderObjectPS["POST_EFFECT"] = CompileShader(L"PostEffectPS.hlsl", psModel);
	//Particle�V�F�[�_�[
	shaderObjectVS["PARTICLE"] = CompileShader(L"ParticleVS.hlsl", vsModel);
	shaderObjectPS["PARTICLE"] = CompileShader(L"ParticlePS.hlsl", psModel);
	shaderObjectGS["PARTICLE"] = CompileShader(L"ParticleGS.hlsl", gsModel);
	//CubeBox�V�F�[�_�[
	shaderObjectVS["CUBE_BOX"] = CompileShader(L"CubeBoxVS.hlsl", vsModel);
	shaderObjectPS["CUBE_BOX"] = CompileShader(L"CubeBoxPS.hlsl", psModel);

}

void ShaderManager::Finalize()
{
	for (auto shader = shaderObjectVS.begin(); shader != shaderObjectVS.end(); shader++)
	{
		shader->second.Reset();
	}
	shaderObjectVS.clear();

	for (auto shader = shaderObjectPS.begin(); shader != shaderObjectPS.end(); shader++)
	{
		shader->second.Reset();
	}
	shaderObjectPS.clear();

	for (auto shader = shaderObjectGS.begin(); shader != shaderObjectGS.end(); shader++)
	{
		shader->second.Reset();
	}
	shaderObjectGS.clear();

	for (auto shader = shaderObjectCS.begin(); shader != shaderObjectCS.end(); shader++)
	{
		shader->second.Reset();
	}
	shaderObjectCS.clear();

}