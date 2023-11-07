#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "bullet.h"
#include "cylinder.h"
#include "box.h"
#include "audio.h"
#include "shadow.h"
#include "animationModel.h"
#include "meshField.h"
void Player::Init()
{
	//m_Model = new Model();
	//m_Model->Load("asset\\model\\player.obj");

	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\Bot.fbx");
	m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx","Idle");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");
	m_ShotSE = AddComponent<Audio>();
	m_ShotSE->Load("asset\\audio\\shot.wav");

	m_Shadow = AddComponent<Shadow>();
	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";
	m_BlendRate = 0.0f;
}

void Player::Uninit()
{
	
	GameObject::Uninit();


	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Player::Update()
{
	D3DXVECTOR3 oldPosition = m_Position;
	Scene* scene = Manager::GetScene();


	bool move = false;
	//サードパーソンビュー
	if (Input::GetKeyPress('W'))
	{
		m_Position += GetForward() * 0.1f;
		if (m_NextAnimationName != "Run")
		{
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Run";
			m_BlendRate = 0.0f;
		}
		move = true;
	}


	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0;
	}


	if (Input::GetKeyPress('A'))
	{
		m_Position -= GetRight() * 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position -= GetForward() * 0.1f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position += GetRight() * 0.1f;
	}
	if (Input::GetKeyPress('Q'))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Rotation.y += 0.1f;
	}
	

	if (move == false)
	{
		if (m_NextAnimationName != "Idle")
		{
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;
		}
	}
	
	//玉発射
	if (Input::GetKeyTrigger('O'))
	{
		Bullet* bullet = scene->AddGameObject<Bullet>(1);
		bullet->SetPosition(m_Position + GetForward() * 0.5f);
		bullet->SetVelocity(GetForward() * 0.1f);
		m_ShotSE->Play();
	}

	//ジャンプ
	if (Input::GetKeyTrigger('J') && m_Velocity.y ==0.0f)
	{
		m_Velocity.y += 0.35f;
	}

	m_Velocity.y -= 0.015f;

	m_Position += m_Velocity;

	auto cylinders = scene->GetGameObjects<Cylinder>();

	float groundHeight = 0.0f;

	//メッシュフィールドとの当たり判定
	MeshField* mf = scene->GetGameObject<MeshField>();
	groundHeight = mf->GetHeigth(m_Position);


	//Cylinderの当たり判定
	for (Cylinder* cylinder : cylinders)//範囲for
	{
		D3DXVECTOR3 positon = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();

		D3DXVECTOR3 dilection = m_Position - positon;
		float lenght = D3DXVec3Length(&dilection);

		if (lenght < scale.x)
		{
			if (m_Position.y < positon.y + scale.y - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = positon.y + scale.y;
			}

		}
		break;
	}

	auto boxes = scene->GetGameObjects<Box>();

	//Boxの当たり判定
	for (Box* box : boxes)//範囲for
	{
		D3DXVECTOR3 positon = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();


		if (positon.x - scale.x - 0.5f < m_Position.x && 
			positon.x + scale.x + 0.5f > m_Position.x &&
			positon.z - scale.z - 0.5f < m_Position.z &&
			positon.z + scale.z + 0.5f > m_Position.z )
		{
			if (m_Position.y < positon.y + scale.y * 2.0f - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;
			}
			else
			{
				groundHeight = positon.y + scale.y * 2.0f;
			}

		}
		break;
	}



	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;

	}

	D3DXVECTOR3 shadowpos = m_Position;
	shadowpos.y = groundHeight;
	m_Shadow->SetPos(shadowpos);

}

void Player::Draw()
{
	GameObject::Draw();

	//入力レイアウト
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリックス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y , m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	m_Matrix = world;

	Renderer::SetWorldMatrix(&world);


	//ポリゴン描画
	m_BlendRate += 0.2f;
	if (m_BlendRate > 1.0f)
	{
		m_BlendRate = 1.0f;
	}
	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);
	m_Time++;
	m_Model->Draw();
}
