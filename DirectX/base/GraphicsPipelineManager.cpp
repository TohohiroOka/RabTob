#include "GraphicsPipelineManager.h"
#include "SafeDelete.h"

#include <string>

using namespace Microsoft::WRL;

ID3D12Device* GraphicsPipelineManager::device = nullptr;
std::unique_ptr<ShaderManager> GraphicsPipelineManager::shaderManager = nullptr;
D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPipelineManager::defaultGpipeline{};
D3D12_RENDER_TARGET_BLEND_DESC GraphicsPipelineManager::blenddesc{};

//���O����������
LPCWSTR GetName(std::string className, std::string setName)
{
	//���O�̌���
	std::string name = className + setName;
	//�傫���擾
	size_t size = name.size();
	//���O�̃T�C�Y+1�̔z��ɍ�蒼��
	LPWSTR returnName = new wchar_t[size + 1];
	std::copy(name.begin(), name.end(), returnName);
	//�z��̖���0����
	returnName[size] = 0;

	return returnName;
}

GraphicsPipelineManager::~GraphicsPipelineManager()
{
	rootSignature.Reset();
	pipelineState.Reset();
}

void GraphicsPipelineManager::StaticInitialize(ID3D12Device* device)
{
	// �ď������`�F�b�N
	assert(!GraphicsPipelineManager::device);

	// nullptr�`�F�b�N
	assert(device);

	//�f�o�C�X�̓o�^
	GraphicsPipelineManager::device = device;

	//�V�F�[�_�[�̃R���p�C��
	shaderManager = ShaderManager::Create();

	//�p�C�v���C���̊�Ր���
	CreateDefaultGpipeline();
}

void GraphicsPipelineManager::CreateDefaultGpipeline()
{
	// �T���v���}�X�N
	defaultGpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	defaultGpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g
	defaultGpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	defaultGpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	defaultGpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// �}�`�̌`��ݒ�i�O�p�`�j
	defaultGpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	defaultGpipeline.NumRenderTargets = 1;    // �`��Ώۂ�1��
	defaultGpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	defaultGpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
}

std::unique_ptr<GraphicsPipelineManager> GraphicsPipelineManager::Create(const std::string name, const unsigned char objectKind,
	const D3D12_INPUT_ELEMENT_DESC* inputLayout, const UINT inputLayoutSize)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	GraphicsPipelineManager* instance = new GraphicsPipelineManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->CreatePipeline(name, objectKind, inputLayout, inputLayoutSize);

	return std::unique_ptr<GraphicsPipelineManager>(instance);
}

void GraphicsPipelineManager::CreatePipeline(const std::string name, const unsigned char objectKind,
	const D3D12_INPUT_ELEMENT_DESC* inputLayout, const UINT inputLayoutSize)
{
	HRESULT result = S_FALSE;

	//�I�u�W�F�N�g�̎�ނ�o�^
	this->objectKind = objectKind;

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = CreatepelineDesc(inputLayout, inputLayoutSize);

	//���[�g�V�O�l�`���̐���
	CreateRootSignature();

	//�p�C�v���C���f�X�N�Ƀ��[�g�V�O�l�`����o�^
	gpipeline.pRootSignature = rootSignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	if (FAILED(result)) {
		assert(0);
	}

	pipelineState->SetName(GetName(name, "PipelineState"));
	rootSignature->SetName(GetName(name, "RootSignature"));
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPipelineManager::CreatepelineDesc(
	const D3D12_INPUT_ELEMENT_DESC* inputLayout, const UINT inputLayoutSize)
{
	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = defaultGpipeline;

	//Obj
	if (objectKind == OBJECT_KINDS::OBJ)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["OBJ"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["OBJ"].Get());

		// �u�����h�X�e�[�g�̐ݒ�i�|�X�g�G�t�F�N�g����ǉ��Ő����j
		gpipeline.BlendState.RenderTarget[1] = blenddesc;
		gpipeline.BlendState.RenderTarget[2] = blenddesc;

		gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�1��
		gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
		gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	}
	//Fbx
	else if (objectKind == OBJECT_KINDS::FBX)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["FBX"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["FBX"].Get());

		// �u�����h�X�e�[�g�̐ݒ�i�|�X�g�G�t�F�N�g����ǉ��Ő����j
		gpipeline.BlendState.RenderTarget[1] = blenddesc;
		gpipeline.BlendState.RenderTarget[2] = blenddesc;

		gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�1��
		gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
		gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	}
	//DrawLine3d
	else if (objectKind == OBJECT_KINDS::DRAW_LINE_3D)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["DRAW_LINE_3D"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["DRAW_LINE_3D"].Get());
	}
	//NormalMap
	else if (objectKind == OBJECT_KINDS::NORMAL_MAP)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["NORMAL_MAP"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["NORMAL_MAP"].Get());
	}
	//Sprite
	else if (objectKind == OBJECT_KINDS::SPRITE)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["SPRITE"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["SPRITE"].Get());

		//��ɎO�p�`�`��
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		//��ɏ㏑�����[��
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gpipeline.DepthStencilState.DepthEnable = false;

		// �u�����h�X�e�[�g�̍Đݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;
	}
	//DrawLine2d
	else if (objectKind == OBJECT_KINDS::DRAW_LINE_2D)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["DRAW_LINE_3D"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["DRAW_LINE_3D"].Get());

		//��ɎO�p�`�`��
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		//��ɏ㏑�����[��
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gpipeline.DepthStencilState.DepthEnable = false;

		// �u�����h�X�e�[�g�̍Đݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;
	}
	//PostEffect
	else if (objectKind == OBJECT_KINDS::POST_EFFECT)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["POST_EFFECT"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["POST_EFFECT"].Get());

		//��ɎO�p�`�`��
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		//��ɏ㏑�����[��
		gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		gpipeline.DepthStencilState.DepthEnable = false;

		// �u�����h�X�e�[�g�̍Đݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;
	}
	//Particle
	else if (objectKind == OBJECT_KINDS::PARTICLE)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["PARTICLE"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["PARTICLE"].Get());
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectGS["PARTICLE"].Get());

		gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		// ���Z�u�����f�B���O
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// �}�`�̌`��ݒ�i�O�p�`�j
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

		// �u�����h�X�e�[�g�̍Đݒ�
		gpipeline.BlendState.RenderTarget[0] = blenddesc;
	}
	//CubeBox
	else if (objectKind == OBJECT_KINDS::CUBE_BOX)
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS["CUBE_BOX"].Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS["CUBE_BOX"].Get());
	}

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = inputLayoutSize;

	return gpipeline;
}

void GraphicsPipelineManager::CreateRootSignature()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^
	// ���[�g�p�����[�^
	std::vector<CD3DX12_ROOT_PARAMETER> rootparams;
	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	//Obj
	if (objectKind == OBJECT_KINDS::OBJ)
	{
		// ���[�g�p�����[�^
		rootparams.resize(5);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// CBV�i�}�e���A���f�[�^�p�j
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`���j
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		// CBV (���C�g)
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	}
	//Fbx
	else if (objectKind == OBJECT_KINDS::FBX)
	{
		// �f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV_t1;
		descRangeSRV_t1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 ���W�X�^
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV_t2;
		descRangeSRV_t2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // t2 ���W�X�^

		// ���[�g�p�����[�^
		rootparams.resize(7);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// CBV�i�}�e���A���f�[�^�p�j
		rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`���j
		rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		// CBV (���C�g)
		rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
		// CBV (�X�L�j���O�p)
		rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�L���[�u�}�b�v�e�N�X�`���j
		rootparams[5].InitAsDescriptorTable(1, &descRangeSRV_t1, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�L���[�u�}�b�v�e�N�X�`���j
		rootparams[6].InitAsDescriptorTable(1, &descRangeSRV_t2, D3D12_SHADER_VISIBILITY_ALL);
	}
	//DrawLine3d
	else if (objectKind == OBJECT_KINDS::DRAW_LINE_3D)
	{
		// ���[�g�p�����[�^
		rootparams.resize(2);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`���j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	}
	//NormalMap
	else if (objectKind == OBJECT_KINDS::NORMAL_MAP)
	{
		//�ǉ��̃f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE addDescRangeSRV[2] = {};
		addDescRangeSRV[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);//t0���W�X�^
		addDescRangeSRV[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);//t0���W�X�^

		// ���[�g�p�����[�^
		rootparams.resize(4);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��1�j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��2�j
		rootparams[2].InitAsDescriptorTable(1, &addDescRangeSRV[0], D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��3�j
		rootparams[3].InitAsDescriptorTable(1, &addDescRangeSRV[1], D3D12_SHADER_VISIBILITY_ALL);
	}
	//Sprite
	else if (objectKind == OBJECT_KINDS::SPRITE)
	{
		// ���[�g�p�����[�^
		rootparams.resize(2);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��1�j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		
		// �X�^�e�B�b�N�T���v���[��s0���W�X�^
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	}
	//DrawLine2d
	else if (objectKind == OBJECT_KINDS::DRAW_LINE_2D)
	{
		// ���[�g�p�����[�^
		rootparams.resize(2);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��1�j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[��s0���W�X�^
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	}
	//PostEffect
	else if (objectKind == OBJECT_KINDS::POST_EFFECT)
	{
		//�ǉ��̃f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE addDescRangeSRV1;
		addDescRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);//t1���W�X�^
		CD3DX12_DESCRIPTOR_RANGE addDescRangeSRV2;
		addDescRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);//t2���W�X�^

		// ���[�g�p�����[�^
		rootparams.resize(4);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��1�j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��2�j
		rootparams[2].InitAsDescriptorTable(1, &addDescRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��3�j
		rootparams[3].InitAsDescriptorTable(1, &addDescRangeSRV2, D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[��s0���W�X�^
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	}
	//Particle
	else if (objectKind == OBJECT_KINDS::PARTICLE)
	{
		// ���[�g�p�����[�^
		rootparams.resize(2);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��1�j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	}
	//CubeBox
	else if (objectKind == OBJECT_KINDS::CUBE_BOX)
	{
		// ���[�g�p�����[�^
		rootparams.resize(2);
		// CBV�i���W�ϊ��s��p�j
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
		// SRV�i�e�N�X�`��1�j
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	}

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0((UINT)rootparams.size(), rootparams.data(), 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
	if (FAILED(result)) {
		assert(0);
	}

	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(result)) {
		assert(0);
	}
}

void GraphicsPipelineManager::Finalize()
{
	shaderManager.reset();
}