#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "input.h"
#include "com_enemy.h"

void Enemy::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    Scene* scene = Manager::GetScene();
    m_Player = scene->GetGameObject("Player");

    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {
                if (state == COLLISION_ENTER)
                {
                }
                else if (state == COLLISION_STAY)
                {
                }
                else if (state == COLLISION_EXIT)
                {
                }
            });
    }
}
void Enemy::Uninit()
{

}
void Enemy::EditorUpdate()
{

}
void Enemy::Update()
{
    Vector3 vec = m_Player->m_Transform->GetPosition() - m_GameObject->m_Transform->GetPosition();
    if (vec.LengthSpr() <= m_Range * m_Range)
    {
        if (vec.LengthSpr() >= m_Speed * m_Speed)
        {
            vec.NormalizThis();
            m_GameObject->m_Transform->Translate(vec * m_Speed);
        }
    }
}
void Enemy::Draw()
{

}

