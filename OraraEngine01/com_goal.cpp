#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "com_goal.h"
#include "post.h"
#include "shaderManager.h" 

void Goal::DrawInspector()
{
    Scene* scene = Manager::GetScene();
    if (SET_DATE_STATE(m_HitObjTag, CASTOMDRAWSTATE_STRING_TAG))
    {
        if (scene->GetGameObjectToTag(m_HitObjTag.c_str()))
            m_HitObj = scene->GetGameObjectToTag(m_HitObjTag.c_str())->GetComponent<InputSystem>();
    }

    if (SET_DATE_STATE(m_ClothName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        m_Cloth = scene->GetGameObject(m_ClothName.c_str())->GetComponent<Cloth>();

    SET_DATE(m_ThresholdSpeed);
}

void Goal::Init()
{
    Scene* scene = Manager::GetScene();
    if (m_HitObjTag != "")
        m_HitObj = scene->GetGameObjectToTag(m_HitObjTag.c_str())->GetComponent<InputSystem>();
    if (m_ClothName != "")
        m_Cloth = scene->GetGameObject(m_ClothName.c_str())->GetComponent<Cloth>();

    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    m_ChangeScene = m_GameObject->GetComponent<ChangeScene>();

    // コールバック関数を登録   
    if (m_Collision)
    {
        m_Collision->SetCollisionCallback([&](CollisionState state, CollisionShape* other)
            {
                if (state == COLLISION_ENTER)
                {
                    if (other->GetGameObejct()->GetTag() == m_HitObjTag)
                        m_IsGoal = true;
                }
            });
    }

}

void Goal::Update()
{
    if (m_IsGoal)
    {
        m_HitObj->SetEnable(false);
        m_Cloth->GetParameter()->dissolveThreshold = 0.0f;
        m_StartPosition = m_Cloth->GetGameObejct()->m_Transform->GetPosition();
        m_IsDissolve = true;
        m_IsGoal = false;
    }

    if (m_IsDissolve)
    {
        m_Cloth->GetParameter()->dissolveThreshold += m_ThresholdSpeed;

        if (m_Cloth->GetParameter()->dissolveThreshold >= 1.1f)
        {
            m_Cloth->GetParameter()->dissolveThreshold = 1.0f;
            m_ThresholdSpeed *= -1;
            m_Cloth->GetGameObejct()->GetMaterial()->SetTexture("Mushroom",".png");
            m_Cloth->GetGameObejct()->m_Transform->Translate(0.0f, -14.0f, 0.0f);
        }

        if (m_Cloth->GetParameter()->dissolveThreshold <= -0.1f)
        {
            m_IsUp = true;
            m_IsDissolve = false;
        }
    }
    if (m_IsUp)
    {
        Vector3 vec = m_StartPosition - m_Cloth->GetGameObejct()->m_Transform->GetPosition();

        float len = vec.LengthSpr();

        if (len <= 0.02f * 0.02f)
        {
            Post* post = ShaderManager::Instance().GetPost();
            post->SetIsWipe(true);
            post->SetThreshold(0.0f);
            post->SetWipeSpeed(0.01f);
            m_ChangeScene->SetIsFadeOut(true);
            m_IsUp = false;
        }
        else
        {
            vec.NormalizThis();
            m_Cloth->GetGameObejct()->m_Transform->Translate(vec * 0.08f);
        }
    }
}

