#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "com_goal.h"
#include "post.h"
#include "shaderManager.h" 

void Goal::Init()
{
    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    m_ChangeScene = m_GameObject->GetComponent<ChangeScene>();

    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {
                if (state == COLLISION_ENTER)
                {
                    if (other->GetGameObejct()->GetName() == m_HitObjName)
                        m_IsGoal = true;
                }
            });
    }
}

void Goal::Update()
{
    if (m_IsGoal)
    {
        Post* post = ShaderManager::Instance().GetPost();
        post->SetIsWipe(true);
        post->SetThreshold(0.0f);
        post->SetWipeSpeed(0.01f);
        m_ChangeScene->SetIsFadeOut(true);
        m_IsGoal = false;
    }
}
