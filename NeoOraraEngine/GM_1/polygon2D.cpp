#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "sprite.h"
#include "input.h"

void Polygon2D::Init(char* texture)
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	D3DXVECTOR2 pos(400.0f, 500.0f);
	D3DXVECTOR2 size(300.0f, 300.0f);
	
	AddComponent<Sprite>()->Init(pos, size, texture);
}

void Polygon2D::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}

void Polygon2D::Update()
{
	GameObject::Update();
	m_Position = (D3DXVECTOR3)Input::GetMousesPos();
}

void Polygon2D::Draw()
{

	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader,NULL,0);


	//マトリックス設定
	Renderer::SetWorldViewProjection2D();

	GameObject::Draw();

}
