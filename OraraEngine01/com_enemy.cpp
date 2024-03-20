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
    m_Mesh = m_GameObject->GetComponent<Mesh>();

    m_Mesh->SetModel("run01_kirby.obj");
    m_Mesh->SetModel("run02_kirby.obj");
    m_Mesh->SetModel("run03_kirby.obj");

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
    bool stop = true;
    if (vec.LengthSpr() <= m_Range * m_Range)
    {
        if (vec.LengthSpr() >= m_Speed * m_Speed)
        {
            stop = false;
            vec.NormalizThis();
            m_GameObject->m_Transform->Translate(vec * m_Speed);

            D3DXQUATERNION oquat{};
            D3DXQUATERNION quaternion = m_GameObject->m_Transform->GetQuaternion();
            D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

            float angle = atan2f(vec.x, vec.z);
            D3DXQuaternionRotationAxis(&oquat, &axis, angle);
            //球面線形補間
            D3DXQuaternionSlerp(&quaternion, &quaternion, &oquat, 0.3f);
            m_GameObject->m_Transform->SetQuaternion(quaternion);
            m_time++;
            if (m_time > 5)
            {
                m_time = 0;
                switch (m_ModelState)
                {
                case 0:
                    m_Mesh->SetModel("run01_kirby.obj");
                    m_ModelState = 1;
                    break;
                case 1:
                    m_Mesh->SetModel("run02_kirby.obj");
                    m_ModelState = 2;
                    break;
                case 2:
                    m_Mesh->SetModel("run03_kirby.obj");
                    m_ModelState = 3;
                    break;
                case 3:
                    m_Mesh->SetModel("run02_kirby.obj");
                    m_ModelState = 0;
                    break;
                default:
                    break;
                }

            }
        }
    }
    if (stop)
    {
        m_Mesh->SetModel("kirby.obj");
    }
}
void Enemy::Draw()
{

}


