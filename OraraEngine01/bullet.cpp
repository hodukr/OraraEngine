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
	m_Transform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    m_Transform->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	m_Velocity = Vector3(0.0f, 0.0f, 0.1f);

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
	
    m_Transform->Translate(m_Velocity);

	Vector3 dDir = m_Transform->GetPosition() - player->m_Transform->GetPosition();

	if (dDir.Length() > 15.0f)
	{
		//消す予約をする
		SetDestroy();
	}

	//敵との衝突判定
	for (Enemy* enemy : enemies)
	{
        Vector3 enemyPos = enemy->m_Transform->GetPosition();

        Vector3 cDir = enemyPos - m_Transform->GetPosition();

		if (cDir.Length() < 2.0f)
		{
			scene->AddGameObject<Explosion>(1)->m_Transform->SetPosition(m_Transform->GetPosition());

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

    D3DXVECTOR3 Scale = m_Transform->GetPosition().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetRotation().dx();



	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y + D3DX_PI, Rotation.x, Rotation.z);
	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}
