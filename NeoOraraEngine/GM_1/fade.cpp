#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "game.h"

void Fade::Init(char* texture)
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	D3DXVECTOR2 pos(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT / 2.0f);
	D3DXVECTOR2 size(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(pos, size, texture);

	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	m_Sprite->SetColor(m_Color);

	InOutflg = true;
}

void Fade::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}

void Fade::Update()
{
	GameObject::Update();
	if (InOutflg)
	{
		if (m_Color.a > 0)
		{
			m_Color.a -= 0.01f;
		}
		else
		{
			m_Color.a = 0.0f;
		}
	}
	else
	{
		if (m_Color.a < 1.0)
		{
			m_Color.a += 0.01f;
		}
		else
		{
			m_Color.a = 1.0f;
			Manager::SetScene<Game>();

			
		}
	}
	m_Sprite->SetColor(m_Color);


	if (Input::GetKeyPress('K'))
	{
		InOutflg = false;
	}
}

void Fade::Draw()
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
