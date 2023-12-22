#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "bullet.h"
#include "cylinder.h"
#include "box.h"
#include "explosion.h"
#include "audio.h"
#include "shadow.h"
#include "animationModel.h"
#include "meshField.h"


void Player::Init()
{
	m_Model = new AnimationModel();
	m_Model->Load("asset\\animation\\akai_e_espiritu.fbx");
	m_Model->LoadAnimation("asset\\animation\\Unarmed Idle 01.fbx", "Idle");
	m_Model->LoadAnimation("asset\\animation\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\animation\\Left Strafe.fbx", "RunLeft");
	m_Model->LoadAnimation("asset\\animation\\Right Strafe.fbx", "RunRight");
	m_Model->LoadAnimation("asset\\animation\\Bot_RunBack.fbx", "RunBack");
	m_Model->LoadAnimation("asset\\animation\\Standing Jump.fbx", "Jump");

	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	//m_Position = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
	m_Transform->SetPosition(0.0f, 0.0f, 0.0f);
    m_Transform->SetScale(0.02f, 0.02f, 0.02f);


	m_ShotSE = AddComponent<Audio>();
	m_ShotSE->Load("asset\\audio\\shot000.wav");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();
}

void Player::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}
void Player::Update()
{
	Vector3 oldPosition = m_Transform->GetPosition();

	//ステートマシーン
	switch (m_PlayerState)
	{
	case PLAYER_STATE_GROUND:
		UpdateGround();
		break;
	case PLAYER_STATE_JUMP:
		UpdateJump();
		break;
	default:
		break;
	}

	Scene* scene = Manager::GetScene();

	//重いテクスチャを読み込んでおく
	if (m_Count == 0)
	{
        Vector3 pos = m_Transform->GetPosition() - m_Transform->GetForward() * 5;
		scene->AddGameObject<Explosion>(1)->m_Transform->SetPosition(pos);
		m_Count++;
	}

	//弾の発射
	if (Input::GetKeyTrigger('F'))
	{
		m_ShotSE->Play();
        Vector3 setPos = m_Transform->GetPosition() + m_Transform->GetUp() + m_Transform->GetForward();
		scene->AddGameObject<Bullet>(1)->SetBullet(setPos, m_Transform->GetForward() * 0.3f);
	}


	//重力 
	m_Velocity.y -= 0.015f;

	m_Transform->Translate(m_Velocity);


	//メッシュフィールドとの衝突判定
	float groundHeight = 0.0f;

	MeshField* meshField = scene->GetGameObject<MeshField>();

	groundHeight = meshField->GetHeight(m_Transform->GetPosition().dx());


	//障害物との衝突判定
	//円柱
	std::vector<Cylinder*> cylinders = scene->GetGameObjects<Cylinder>();

	for (Cylinder* cylinder : cylinders)
	{
		Vector3 position = cylinder->m_Transform->GetPosition();
		Vector3 scale = cylinder->m_Transform->GetScale();
		
        Vector3 direction = m_Transform->GetPosition() - position;
		direction.y = 0.0f;
		float length = direction.Length();

		if (length < m_Transform->GetScale().x + scale.x)
		{
			if (m_Transform->GetPosition().y < position.y + scale.y - 0.5f)
			{
                Vector3 repos = m_Transform->GetPosition();
                repos.x = oldPosition.x;
                repos.z = oldPosition.z;
                m_Transform->SetPosition(repos);
			}
			else 
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}

	}

	//直方体
	std::vector<Box*> boxes = scene->GetGameObjects<Box>();

	for (Box* box : boxes)
	{
		Vector3 position = box->m_Transform->GetPosition();
        Vector3 scale = box->m_Transform->GetScale();

		if (position.x - scale.x - 0.5f < m_Transform->GetPosition().x &&
            m_Transform->GetPosition().x < position.x + scale.x + 0.5 &&
			position.z - scale.z - 0.5f < m_Transform->GetPosition().z &&
            m_Transform->GetPosition().z < position.z + scale.z + 0.5f)
		{
			if (m_Transform->GetPosition().y < position.y + scale.y * 2.0f - 0.5f)
			{
                Vector3 repos = m_Transform->GetPosition();
                repos.x = oldPosition.x;
                repos.z = oldPosition.z;
                m_Transform->SetPosition(repos);

			}
			else
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
			break;
		}

	}

	//接地
	if (m_Transform->GetPosition().y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_isGround = false;
        m_Transform->SetPositionY(groundHeight);
		m_Velocity.y = 0.0f;
	}
	else
	{
		m_isGround = true;
	}

	m_Shadow->SetPosition(D3DXVECTOR3(m_Transform->GetPosition().x, groundHeight + 0.01f , m_Transform->GetPosition().z));


	GameObject::Update();
}

void Player::Draw()
{
	GameObject::Draw();

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_Transform->GetScale().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetPosition().dx();

	D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
	D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
	world = scale * rot * trans;

	m_Matrix = world;

	Renderer::SetWorldMatrix(&world);
	

	m_Model->Update(m_AnimationName.c_str(), m_Time, 
		                    m_NextAnimationName.c_str(), m_Time, m_BlendRate);
	m_Time++;
	
	m_BlendRate += 0.05f;

	if (m_BlendRate > 1.0f)
		m_BlendRate = 1.0f;

	m_Model->Draw();
}

void Player::UpdateGround()
{
	//移動
	Vector3 vec = Vector3(0.0f, 0.0f, 0.0f);

	bool move = false;

	if (Input::GetKeyPress('A'))
	{
		SetAnimation("RunLeft");

		vec -= m_Transform->GetRight();
		move = true;
	}
	if (Input::GetKeyPress('D'))
	{
		SetAnimation("RunRight");

		vec += m_Transform->GetRight();
		move = true;
	}
	if (Input::GetKeyPress('W'))
	{
		SetAnimation("Run");

		vec += m_Transform->GetForward();
		move = true;
	}
	if (Input::GetKeyPress('S'))
	{
		SetAnimation("RunBack");

		vec -= m_Transform->GetForward();
		move = true;
	}

	if (!move)
		SetAnimation("Idle");

    vec.NormalizThis();

    m_Transform->Translate(vec * 0.2f);

	//プレイヤーの回転
	if (Input::GetKeyPress('Q'))
	{
        m_Transform->Rotate(Vector3::Up() *  - 0.1f);
	}
	if (Input::GetKeyPress('E'))
	{
        m_Transform->Rotate(Vector3::Up() * 0.1f);

	}

	//ジャンプ
	if (Input::GetKeyTrigger(VK_SPACE) && m_Velocity.y == 0.0f)
	{
		m_Time = 0;
		SetAnimation("Jump");
		m_Velocity.y = 0.35f;
		m_PlayerState = PLAYER_STATE_JUMP;
	}
}

void Player::UpdateJump()
{
	if (m_isGround)
	{
	   m_PlayerState = PLAYER_STATE_GROUND;
	}
}

void Player::SetAnimation(std::string animName)
{
	if (m_NextAnimationName != animName)
	{
		m_AnimationName = m_NextAnimationName;
		m_NextAnimationName = animName;
		m_BlendRate = 0.0f;
	}
}
