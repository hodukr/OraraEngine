#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"

Model* Bullet::m_Model{};

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");
}

void Bullet::Unroad()
{
	m_Model->Unload();
	delete m_Model;
}

void Bullet::Init()
{
	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.1f);


	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
}

void Bullet::Uninit()
{


	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Bullet::Update()
{
	m_Position += m_Velocity;

	//範囲外で消去
	if (m_Position.z > 10 )
	{
		SetDestroy();
	}


	//敵との衝突判定
	Scene* scene = Manager::GetScene();
	auto enemyies = scene->GetGameObjects<Enemy>();

	for (Enemy* enemy: enemyies)//範囲for
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();
		D3DXVECTOR3 disVector = m_Position - enemyPos;
		float disLength = disVector.x * disVector.x + disVector.y * disVector.y + disVector.z * disVector.z;

		if (disLength < 1.0f)
		{
			SetDestroy();
			enemy->SetDestroy();
			scene->AddGameObject<Explosion>(1)->SetPosition(enemyPos);
			scene->GetGameObject<Score>()->SetCount(true);
		}
	}
}

void Bullet::Draw()
{
	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	//ポリゴン描画
	m_Model->Draw();
}
