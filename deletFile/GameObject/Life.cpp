#include "main.h"
#include "renderer.h"
#include "Life.h"
#include "sprite.h"
#include "textureManager.h"


void Life::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\gaugeVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\gaugePS.cso");

	//Sprite* sprite = AddComponent<Sprite>();
	//sprite->Init(100.0f, 100.0f, 500.0f, 500.0f, "asset\\texture\\kizuna.jpg");

	AddComponent<Sprite>()->Init(30.0f, 30.0f, 200.0f, 80.0f, "asset\\texture\\kizuna.jpg");
	m_hp = 600;
	m_hpMax = 1000;
	m_hpOld = 800;

}


void Life::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}


void Life::Update()
{
	m_Count++;
	if (m_Count > 60)
	{
		m_hpOld = m_hp;
		m_Count = 0;
	}
	m_hp--;
	if (m_hp < 0)
	{
		m_hp = 1000;
	}
}


void Life::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	PARAMETER param;
	param.Hitpoint.x = m_hp;
	param.Hitpoint.y = m_hpMax;
	param.Hitpoint.z = m_hpOld;

	param.BaseColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);//緑
	if (m_hp < 600)
		param.BaseColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);//黄色
	if (m_hp < 200)
		param.BaseColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//赤
	param.LostColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);//灰色
	param.DiffColor = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);//薄い赤
	Renderer::SetParameter(param);

	//基底クラスのメソッド呼び出し
	GameObject::Draw();
}