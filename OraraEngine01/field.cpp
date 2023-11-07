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
	D3DXMATRIX world,scale,rot,trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	

	GameObject::Draw();
}