#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "input.h"
#include "com_player.h"
#include "com_waterSurface.h"

void Player::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {
                BoxCollision* boxother = (BoxCollision*)other;
                    if (state == COLLISION_ENTER)
                    {
                        if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_UP)
                        {
                            m_IsGround++;
                            m_Velocity.y = -0.1f;
                        }
                        if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_DOWN)
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
                        if (boxother->GetHitDirection(m_Collision) == BOXHITDIRECTION_UP)
                        {
                            m_IsGround--;
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


}
void Player::Draw()
{

}

