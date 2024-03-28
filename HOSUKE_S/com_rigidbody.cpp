#include "main.h"
#include "gameObject.h"
#include "com_rigidbody.h"

void Rigidbody::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {
                if (state == COLLISION_ENTER)
                { 
                   m_Velocity.y = m_Gravity;   
                }
                else if (state == COLLISION_STAY)
                {
                   m_Velocity.y = 0.0f;
                }
                else if (state == COLLISION_EXIT)
                {
                    m_Velocity.y = 0.0f;
                }
            });
    }
}
void Rigidbody::Uninit()
{

}
void Rigidbody::EditorUpdate()
{

}
void Rigidbody::Update()
{
	m_Velocity.y += m_Gravity;
	m_GameObject->m_Transform->Translate(m_Velocity);
}
void Rigidbody::Draw()
{

}
