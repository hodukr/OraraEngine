#include"main.h"
#include "manager.h"
#include "renderer.h"
#include <thread>
#include "shaderManager.h"
#include "collisionManager.h" 
#include "scene.h"
#include "loading.h"

void Loading::AsyncLoad()
{
	// ������ m_NextScene �ւ̃A�N�Z�X�����b�N����
	{
		std::lock_guard<std::mutex> lock(m_LoadedMutex);
		m_NextScene->Init(); // m_NextScene �� nullptr �̏ꍇ�ɃA�N�Z�X���Ȃ��悤�ɏC��
		Manager::GetCollisionManager()->Init();
		GuiManager::Instance().Init();
		ShaderManager::Instance().Init();
	}

	m_Loaded = true;
}

void Loading::Init()
{
	Scene::Init();
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 150, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(PATTERN_X, 0.0f);

	vertex[2].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, PATTERN_Y);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(PATTERN_X, PATTERN_Y);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_TextureNum = TextureManager::LoadTexture((char*)"asset\\texture\\loading.png");
	m_ShaderNum = ShaderManager::Instance().LoadShader("unlitTexture");

	// �o�b�N�O���E���h�Ń��\�[�X�̔񓯊��ǂݍ��݂��J�n
	 // std::async �͕Ԃ�l��Ԃ��Ȃ��ꍇ�A�����I�ȃX�e�[�^�X���擾������@���Ȃ����ߎg��Ȃ������ǂ��ł��B
	 // ����� std::thread ���g�p���܂��B
	std::thread loadingThread(&Loading::AsyncLoad, this);
	loadingThread.detach(); // ���C���X���b�h�ƕ���
	//loadingThread.join(); // ���[�h�����܂őҋ@

	// �����������̓o�b�N�O���E���h�Ŏ��s���ꂽ�̂ŁA���[�h�t���O�� false �̂܂܂ł�
}

void Loading::Uninit()
{

	// ���̃V�[���̃��\�[�X��������A�N���[���A�b�v
	//if (m_NextScene != nullptr)
	//{
	//    delete m_NextScene;
	//    m_NextScene = nullptr;
	//}
	//Scene::Uninit();

	m_VertexBuffer->Release();

	// �ǂݍ��ݍς݃t���O�����Z�b�g
	m_Loaded = false;
}

void Loading::Update()
{
	//Scene::Update();

	if (m_Loaded)
	{
		//���[�h����
		//m_NextScene->Draw();
		Manager::SetLoaded(m_NextScene);
	}
	else
	{
		static int count = 0;
		count++;

		if (count >= 10)
		{
			if (m_Count >= 12)
				m_Count = 0;

			m_Count++;
			count = 0;
		}
	}
}

void Loading::Draw()
{
	//Scene::Draw();

	//�e�N�X�`�����W��������
	float u = m_Count % 3 * PATTERN_X;
	float v = m_Count / 3 * PATTERN_Y;

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 150, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(u, v);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 150, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(u + PATTERN_X, v);

	vertex[2].Position = D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 50, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(u, v + PATTERN_Y);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(u + PATTERN_X, v + PATTERN_Y);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	ShaderManager::Instance().GetShader(m_ShaderNum)->SetShader();

	Renderer::SetWorldViewProjection2D();

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_TextureNum));

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}


