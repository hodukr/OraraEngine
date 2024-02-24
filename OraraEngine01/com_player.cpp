#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "input.h"
#include "com_player.h"

void Player::Init()
{
	m_Collision = m_GameObject->GetComponent<BoxCollision>();
    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {
                if (state == COLLISION_ENTER)
                {
                    if (m_Collision->GetHitDirection() == BOXHITDIRECTION_DOWN)
                    {
                        m_IsGround = true;
                        m_Velocity.y = -0.1f;
                    }
                    if (m_Collision->GetHitDirection() == BOXHITDIRECTION_UP)
                    {
                        m_Velocity.y = -0.1f;
                    }
                }
                else if (state == COLLISION_STAY)
                {
                    if(m_Collision->GetHitDirection() == BOXHITDIRECTION_DOWN)
                        m_Velocity.y = 0.0f;
                }
                else if (state == COLLISION_EXIT)
                {
                    m_IsGround = false;
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
	if (Input::Instance().GetKeyTrigger(VK_SPACE) && m_IsGround)
	{
		m_JumpPower = 1.0f;
        m_IsGround = false;
        m_Velocity.y = m_JumpPower;
        Manager::SetLoadScene("newScene");
	}
    m_Velocity.y -= 0.1f;
	m_GameObject->m_Transform->Translate(m_Velocity);
}
void Player::Draw()
{

}

