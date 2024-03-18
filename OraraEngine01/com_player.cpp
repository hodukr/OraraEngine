#include "main.h"
#include "manager.h"
#include "shaderManager.h"
#include "gameObject.h"
#include "input.h"
#include "com_player.h"
#include "com_waterSurface.h"
#include "com_changeScene.h"
#include "post.h"
void Player::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    GameObject* goal = Manager::GetScene()->GetGameObjectToTag("Goal");
    if (goal)
    {
        m_ChangeScene = goal->GetComponent<ChangeScene>();
    }

    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {

                BoxCollision* boxother = (BoxCollision*)other;
                if (state == COLLISION_ENTER)
                {
                    if (other->GetGameObejct()->GetTag() == "Enemy")
                    {
                        DeadPlayer();
                    }
                    if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_UP)
                    {
                        m_IsGround++;
                        m_Velocity.y = -0.1f;
                        m_IsOldUpHit[boxother] = true;
                    }
                    else if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_DOWN)
                    {
                        m_Velocity.y = -0.1f;
                    }
                }
                else if (state == COLLISION_STAY)
                {
                    if (m_Collision->GetHitDirection(boxother) == BOXHITDIRECTION_DOWN)
                        m_Velocity.y = 0.0f;
                }
                else if (state == COLLISION_EXIT)
                {
                    if (m_IsOldUpHit[boxother])
                    {
                        m_IsGround--;
                        m_IsOldUpHit[boxother] = false;
                    }
                }
            });
    }
}
void Player::Uninit()
{

}
void Player::EditorUpdate()
{

}
void Player::Update()
{
	if (Input::Instance().GetKeyTrigger(VK_SPACE) && m_IsGround > 0)
	{
		m_JumpPower = 1.0f;
        //m_IsGround--;
        m_Velocity.y = m_JumpPower;
	}
    m_Velocity.y -= 0.1f;
	m_GameObject->m_Transform->Translate(m_Velocity);
    D3DXCOLOR color = m_GameObject->GetMaterial()->GetColor();
    static float alphaSpeed = 0.01f;
    if (color.a >= 1.0f)alphaSpeed = -0.01f;
    if (color.a <= 0.0f)alphaSpeed = 0.01f;
    color.a += alphaSpeed;
    m_GameObject->GetMaterial()->SetColor(color);

}
void Player::Draw()
{
}

void Player::DeadPlayer()
{
    InputSystem* is = m_GameObject->GetComponent<InputSystem>();
    if (is)is->SetEnable(false);
    
    string newScene = Manager::GetScene()->GetName();
    Post* post = ShaderManager::Instance().GetPost();
    post->SetIsWipe(true);
    post->SetThreshold(0.0f);
    post->SetWipeSpeed(0.01f);
    if (!m_ChangeScene)return;
    m_ChangeScene->SetScene(newScene);
    m_ChangeScene->SetIsFadeOut(true);
}

