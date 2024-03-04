#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "input.h"
#include "com_enemy.h"

void Enemy::DrawInspector()
{
    Scene* scene = Manager::GetScene();
    SET_DATE(m_Speed);
    SET_DATE(m_Range);
    if (SET_DATE_STATE(m_TargetName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        m_Target = scene->GetGameObject(m_TargetName.c_str());
}

void Enemy::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    Scene* scene = Manager::GetScene();
    m_Target = scene->GetGameObject(m_TargetName.c_str());

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
    Vector3 vec = m_Target->m_Transform->GetPosition() - m_GameObject->m_Transform->GetPosition();
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


