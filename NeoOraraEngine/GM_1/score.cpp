#include "main.h"
#include "renderer.h"
#include "score.h"
#include "sprite.h"
#include "input.h"

void Score::Init()
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	{
		D3DXVECTOR2 pos(90.0f, 30.0f);
		D3DXVECTOR2 size(60.0f, 60.0f);
		Sprite* sprite = AddComponent<Sprite>("hundred");
		sprite->Init(pos, size, "asset\\texture\\nonber.png");
		sprite->SetUV(D3DXVECTOR2(0.0f, 0.0f));
		sprite->SetUVlen(D3DXVECTOR2(0.2f, 0.2f));
	}
	{
		D3DXVECTOR2 pos(30.0f, 30.0f);
		D3DXVECTOR2 size(60.0f, 60.0f);
		Sprite* sprite = AddComponent<Sprite>("thousand");
		sprite->Init(pos, size, "asset\\texture\\nonber.png");
		sprite->SetUV(D3DXVECTOR2(0.0f, 0.0f));
		sprite->SetUVlen(D3DXVECTOR2(0.2f, 0.2f));
	}
	{
		D3DXVECTOR2 pos(150.0f, 30.0f);
		D3DXVECTOR2 size(60.0f, 60.0f);
		Sprite* sprite = AddComponent<Sprite>();
		sprite->Init(pos, size, "asset\\texture\\nonber.png");
		sprite->SetUV(D3DXVECTOR2(0.0f, 0.0f));
		sprite->SetUVlen(D3DXVECTOR2(0.2f, 0.2f));
	}
	{
		D3DXVECTOR2 pos(210.0f, 30.0f);
		D3DXVECTOR2 size(60.0f, 60.0f);
		Sprite* sprite = AddComponent<Sprite>();
		sprite->Init(pos, size, "asset\\texture\\nonber.png");
		sprite->SetUV(D3DXVECTOR2(0.0f, 0.0f));
		sprite->SetUVlen(D3DXVECTOR2(0.2f, 0.2f));
	}

	m_count = 0;
}

void Score::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}

void Score::Update()
{


	float x = (m_count % 10) % 5 * (1.0f / 5);
	float y = (m_count % 10) / 5 * (1.0f / 5);
	GetComponent<Sprite>("hundred")->SetUV(D3DXVECTOR2(x, y));
	x = (m_count/10) % 5 * (1.0f / 5);
	y = (m_count / 10) / 5 * (1.0f / 5);
	GetComponent<Sprite>("thousand")->SetUV(D3DXVECTOR2(x,y));
	GameObject::Update();
	m_Position = (D3DXVECTOR3)Input::GetMousesPos();
}

void Score::Draw()
{

	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	//マトリックス設定
	Renderer::SetWorldViewProjection2D();

	GameObject::Draw();

}

