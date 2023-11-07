#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"

Model* Bullet::m_Model{};

void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");
}
void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Bullet::Init()
{
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.1f);

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
	Scene* scene = Manager::GetScene();
	auto enemies = scene->GetGameObjects<Enemy>();
	Player* player = scene->GetGameObject<Player>();
	
	m_Position += m_Velocity;

	D3DXVECTOR3 dDir = m_Position - player->GetPosition();

	if (D3DXVec3Length(&dDir) > 15.0f)
	{
		//消す予約をする
		SetDestroy();
	}

	//敵との衝突判定
	for (Enemy* enemy : enemies)
	{
		D3DXVECTOR3 enemyPos = enemy->GetPosition();

		D3DXVECTOR3 cDir = enemyPos - m_Position;

		if (D3DXVec3Length(&cDir) < 2.0f)
		{
			scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);

			SetDestroy();
			enemy->SetDestroy();

			Score* score = scene->GetGameObject<Score>();
			score->AddCount(70);

			return;
		}
	}
}

void Bullet::Draw()
{

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}