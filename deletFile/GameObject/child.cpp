#include "main.h"
#include "renderer.h"
#include "model.h"
#include "child.h"
#include "player.h"



void Child::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Transform->SetScale(8.0f, 8.0f, 8.0f);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
}

void Child::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}
void Child::Update()
{
	GameObject::Update();

    m_Transform->Rotate(Vector3::Up() * 0.3f);
    m_Transform->SetPositionY(-1.0f);
    m_Transform->SetPositionX(2.0f);

}

void Child::Draw()
{


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_Transform->GetPosition().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetRotation().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;
	world = scale * rot * trans * m_Parent->GetMatrix();

	Renderer::SetWorldMatrix(&world);


	GameObject::Draw();

	m_Model->Draw();
}
