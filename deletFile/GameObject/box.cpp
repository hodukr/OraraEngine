#include "main.h"
#include "renderer.h"
#include "box.h"
#include "material.h"
//#include "mesh.h"
void Box::Init()
{
    AddComponent<Material>()->Init("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
    //AddComponent<Mesh>();
    m_Transform->SetScale(1.0f, 1.0f, 1.0f);
//	m_Model = new Model();
//	m_Model->Load("asset\\model\\box.obj");
//
//	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
//		"shader\\vertexLightingVS.cso");
//
//	Renderer::CreatePixelShader(&m_PixelShader,
//		"shader\\vertexLightingPS.cso");
}

void Box::Uninit()
{
    GameObject::Uninit();
	//m_Model->Unload();
	//delete m_Model;

	//m_VertexLayout->Release();
	//m_VertexShader->Release();
	//m_PixelShader->Release();

}
void Box::Update()
{
    GameObject::Update();

}

void Box::Draw()
{
    GameObject::Draw();

	////入力レイアウト設定
	//Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	////シェーダー設定
	//Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	////マトリクス設定
 //   D3DXMATRIX world, scale, rot, trans;
 //   D3DXVECTOR3 Scale = m_Transform->GetPosition().dx();
 //   D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
 //   D3DXVECTOR3 Position = m_Transform->GetRotation().dx();

 //   D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
 //   D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
 //   D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
 //   world = scale * rot * trans;


	//Renderer::SetWorldMatrix(&world);

	//m_Model->Draw();

}
