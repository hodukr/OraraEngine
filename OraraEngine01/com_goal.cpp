#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "com_goal.h"
#include "post.h"
#include "shaderManager.h" 

void Goal::DrawInspector()
{
    Scene* scene = Manager::GetScene();
    if (SET_DATE_STATE(m_HitObjName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        m_HitObj = scene->GetGameObject(m_HitObjName.c_str())->GetComponent<InputSystem>();

    if (SET_DATE_STATE(m_ClothName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        m_Cloth = scene->GetGameObject(m_ClothName.c_str())->GetComponent<Cloth>();
}

void Goal::Init()
{
    Scene* scene = Manager::GetScene();
    if(m_HitObjName != "")
        m_HitObj = scene->GetGameObject(m_HitObjName.c_str())->GetComponent<InputSystem>();
    if (m_ClothName != "")
        m_Cloth = scene->GetGameObject(m_ClothName.c_str())->GetComponent<Cloth>();

    m_Collision = m_GameObject->GetComponent<BoxCollision>();
    m_ChangeScene = m_GameObject->GetComponent<ChangeScene>();
   
    // �R�[���o�b�N�֐���o�^   
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

    ZeroMemory(&m_Param, sizeof(PARAMETER));
    m_Param.dissolveThreshold = 0.0f;
    m_Param.dissolveRange = 0.1f;
}

void Goal::Update()
{
    if (m_IsGoal)
    {
        m_HitObj->SetEnable(false);
        Post* post = ShaderManager::Instance().GetPost();
        post->SetIsWipe(true);
        post->SetThreshold(0.0f);
        post->SetWipeSpeed(0.01f);
        m_ChangeScene->SetIsFadeOut(true);
        m_IsGoal = false;
    }
    static float delta;
    m_Param.dissolveThreshold += delta;

    if (m_Cloth->GetThreshold() >= 1.1f || m_Cloth->GetThreshold() <= -0.1f)
    {
        delta *= -1;
    }
    m_Cloth->SetParam(m_Param);
}
