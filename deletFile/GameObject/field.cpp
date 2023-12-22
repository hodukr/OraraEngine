#include "main.h"
#include "renderer.h"
#include "field.h"
#include "plane.h"

void Field::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	AddComponent<Plane>()->Init(-50, -50, 100, 100, "asset\\texture\\field004.jpg");
	/*AddComponent<Plane>()->Init(-20, -20 20, 20, "asset\\texture\\field004.jpg");
	AddComponent<Plane>()->Init(-20, 0, 20, 20, "asset\\texture\\field004.jpg");
	AddComponent<Plane>()->Init(0, -20, 20, 20, "asset\\texture\\field004.jpg");*/
}

void Field::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}
void Field::Update()
{
	GameObject::Update();
}

void Field::Draw()
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

	

	GameObject::Draw();
}
