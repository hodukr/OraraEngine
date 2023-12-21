#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "skyDomeGO.h"
#include "camera.h"


void SkyDomeGO::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sky.obj");

	m_Transform->SetScale(100.0f, 100.0f, 100.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");
}

void SkyDomeGO::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}
void SkyDomeGO::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

    Vector3 postion = camera->m_Transform->GetPosition() - m_Transform->GetUp() * 10.0f;
    m_Transform->SetPosition(postion);

    m_Transform->Rotate(Vector3::Up() * 0.0001f);
}

void SkyDomeGO::Draw()
{


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_Transform->GetScale().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetPosition().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

}
