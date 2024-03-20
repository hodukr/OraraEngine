#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "com_goal.h"
#include "post.h"
#include "shaderManager.h" 
#include "randomNumberGenerator.h"
#include "timeProvider.h"
void Goal::DrawInspector()
{
    Scene* scene = Manager::GetScene();
    if (SET_DATE_STATE(m_HitObjTag, CASTOMDRAWSTATE_STRING_TAG))  
        m_HitObj = scene->GetGameObjectToTag(m_HitObjTag.c_str());


    if (SET_DATE_STATE(m_ClothName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        m_Cloth = scene->GetGameObject(m_ClothName.c_str())->GetComponent<Cloth>();

    SET_DATE(m_ThresholdSpeed);
}

void Goal::Init()
{
    Scene* scene = Manager::GetScene();
    if (m_HitObjTag != "")
        m_HitObj = scene->GetGameObjectToTag(m_HitObjTag.c_str());
    if (m_ClothName != "")
        m_Cloth = scene->GetGameObject(m_ClothName.c_str())->GetComponent<Cloth>();

    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    m_ChangeScene = m_GameObject->GetComponent<ChangeScene>();
    m_Audio = m_GameObject->GetComponent<Audio>();

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
        if (m_HitObj)
        {
            m_HitObj->GetComponent<InputSystem>()->SetEnable(false);
            m_HitObj->GetComponent<Player>()->SetEnable(false);
            m_HitObj->GetComponent<BoxCollision>()->SetEnable(false);
        }
        m_Cloth->GetParameter()->dissolveThreshold = 0.0f;
        m_StartPosition = m_Cloth->GetGameObejct()->m_Transform->GetPosition();
        m_IsDissolve = true;
        m_Audio->Play();
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
        static ElapsedTimeTracker timer;
        if(timer.GetElapsedTimeInSeconds() >= 0.0f){
            timer.Reset();
            static RandomNumberGenerator random;
            Vector3 offset;
            offset.x = random.GenerateFloat(-20.0f, 20.0f);
            offset.y = random.GenerateFloat(1.0f, 10.0f);
            offset.z = random.GenerateFloat(2.0f, 5.0f);
            Vector3 size{};
            size.x = random.GenerateFloat(2.0f, 4.0f);
            size.y = size.x;
            Vector3 createpos = m_GameObject->m_Transform->GetPosition() + offset;
            GameObject* obj = Manager::CreatePrefab("fireworks", createpos);
            D3DXCOLOR color{random.GenerateFloat(0.0f,1.0f),random.GenerateFloat(0.0f,1.0f) ,random.GenerateFloat(0.0f,1.0f) ,1.0f};
            obj->GetMaterial()->SetColor(color);
            obj->m_Transform->SetScale(size);
            obj->SetDestroy(1.0f);
        }

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

