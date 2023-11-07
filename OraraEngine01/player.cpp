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
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale    = D3DXVECTOR3(0.02f, 0.02f, 0.02f);


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
	D3DXVECTOR3 oldPosition = m_Position;

	//�X�e�[�g�}�V�[��
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

	//�d���e�N�X�`����ǂݍ���ł���
	if (m_Count == 0)
	{
		scene->AddGameObject<Explosion>(1)->SetPosition(m_Position - GetForward() * 5);
		m_Count++;
	}

	//�e�̔���
	if (Input::GetKeyTrigger('F'))
	{
		m_ShotSE->Play();
		scene->AddGameObject<Bullet>(1)->SetBullet(m_Position + GetUp() * 1.5f + GetForward(), GetForward() * 0.3f);
	}


	//�d��
	m_Velocity.y -= 0.015f;

	m_Position += m_Velocity;


	//���b�V���t�B�[���h�Ƃ̏Փ˔���
	float groundHeight = 0.0f;

	MeshField* meshField = scene->GetGameObject<MeshField>();

	groundHeight = meshField->GetHeight(m_Position);


	//��Q���Ƃ̏Փ˔���
	//�~��
	std::vector<Cylinder*> cylinders = scene->GetGameObjects<Cylinder>();

	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		
	    D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < m_Scale.x + scale.x)
		{
			if (m_Position.y < position.y + scale.y - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;

			}
			else 
			{
				groundHeight = position.y + scale.y;
			}
			break;
		}

	}

	//������
	std::vector<Box*> boxes = scene->GetGameObjects<Box>();

	for (Box* box : boxes)
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();

		if (position.x - scale.x - 0.5f < m_Position.x &&
			m_Position.x < position.x + scale.x + 0.5 &&
			position.z - scale.z - 0.5f < m_Position.z &&
			m_Position.z < position.z + scale.z + 0.5f)
		{
			if (m_Position.y < position.y + scale.y * 2.0f - 0.5f)
			{
				m_Position.x = oldPosition.x;
				m_Position.z = oldPosition.z;

			}
			else
			{
				groundHeight = position.y + scale.y * 2.0f;
			}
			break;
		}

	}

	//�ڒn
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_isGround = false;
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}
	else
	{
		m_isGround = true;
	}

	m_Shadow->SetPosition(D3DXVECTOR3(m_Position.x, groundHeight + 0.01f , m_Position.z));


	GameObject::Update();
}

void Player::Draw()
{
	GameObject::Draw();

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
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
	//�ړ�
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	bool move = false;

	if (Input::GetKeyPress('A'))
	{
		SetAnimation("RunLeft");

		vec -= GetRight();
		move = true;
	}
	if (Input::GetKeyPress('D'))
	{
		SetAnimation("RunRight");

		vec += GetRight();
		move = true;
	}
	if (Input::GetKeyPress('W'))
	{
		SetAnimation("Run");

		vec += GetForward();
		move = true;
	}
	if (Input::GetKeyPress('S'))
	{
		SetAnimation("RunBack");

		vec -= GetForward();
		move = true;
	}

	if (!move)
		SetAnimation("Idle");


	D3DXVec3Normalize(&vec, &vec);

	m_Position += vec * 0.2f;

	//�v���C���[�̉�]
	if (Input::GetKeyPress('Q'))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Rotation.y += 0.1f;
	}

	//�W�����v
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