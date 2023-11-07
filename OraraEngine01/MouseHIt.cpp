#include "main.h"
#include "manager.h"
#include "scene.h"
#include "MouseHIt.h"
#include "renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "camera.h"
#include "model.h"
void MouseHit::Init()
{
    GameObject::Init();
    m_Model = new Model();
    m_Model->Load("asset\\model\\torus.obj");
    m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Scale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\vertexLightingVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\vertexLightingPS.cso");
}

void MouseHit::UnInit()
{
    GameObject::Uninit();
    m_Model->Unload();
    delete m_Model;

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

}

void MouseHit::Update()
{
    Scene* scene = Manager::GetScene();
    Camera* camera = scene->GetGameObject<Camera>();
    GameObject::Update();

	GetCursorPos(&m_MousePos);
	ScreenToClient(GetWindow(), &m_MousePos);



    //�X�N���[�����W����r���[�|�[�g���W�ւ̕ϊ�
    D3DXVECTOR3 screenSpace(m_MousePos.x, m_MousePos.y, 0.0f); // �}�E�X�̃X�N���[�����W
    D3DXVECTOR3 viewportSpace;
    viewportSpace.x = (2.0f * screenSpace.x / SCREEN_WIDTH) - 1.0f;
    viewportSpace.y = 1.0f - (2.0f * screenSpace.y / SCREEN_HEIGHT);
    viewportSpace.z = 0.0f; // �X�N���[�����W����̐[�x�l�i�ʏ��0.0f���g�p�j

    //�r���[�|�[�g���W���烏�[���h���W�ւ̕ϊ�
    D3DXMATRIX projectionMatrix, viewMatrix;

    // �v���W�F�N�V�����s��A�r���[�s��
    viewMatrix = camera->GetViewMatrix();

    // �X�N���[���̃A�X�y�N�g��i�� / �����j
    float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    // ��p�iFOVY�j�A�A�X�y�N�g��A�߃N���b�v�ʁA���N���b�v�ʂ��w�肵�ăv���W�F�N�V�����s����쐬
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    D3DXMATRIX Matrix = viewMatrix * projectionMatrix;

    D3DXMATRIX inverseViewProjection; // �r���[�s��ƃv���W�F�N�V�����s��̋t�s����|�����s��
    D3DXMatrixInverse(&inverseViewProjection, NULL, &Matrix);



    D3DXVECTOR3 worldSpace;
    D3DXVec3TransformCoord(&worldSpace, &viewportSpace, &inverseViewProjection);
    m_Position = worldSpace;
}

void MouseHit::Draw()
{
    ////���̓��C�A�E�g�ݒ�
    //Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    ////�V�F�[�_�[�ݒ�
    //Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    //Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    ////�}�g���N�X�ݒ�
    //D3DXMATRIX world, scale, rot, trans;

    //D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
    //D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
    //D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
    //world = scale * rot * trans;

    //Renderer::SetWorldMatrix(&world);

    //m_Model->Draw();
}

