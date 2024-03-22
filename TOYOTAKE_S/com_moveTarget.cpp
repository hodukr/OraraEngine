#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "input.h"
#include "com_moveTarget.h"

void MoveTarget::DrawInspector()
{
    Scene* scene = Manager::GetScene();
    SET_DATE(m_Speed);
    if (SET_DATE_STATE(m_TargetName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        m_Target = scene->GetGameObject(m_TargetName.c_str());
}

void MoveTarget::Init()
{
    Scene* scene = Manager::GetScene();
    m_Target = scene->GetGameObject(m_TargetName.c_str());
}
void MoveTarget::Uninit()
{
}
void MoveTarget::EditorUpdate()
{
}
void MoveTarget::Update()
{
    Vector3 vec = m_Target->m_Transform->GetPosition() - m_GameObject->m_Transform->GetPosition();
   
    if (vec.LengthSpr() >= m_Speed * m_Speed)
    {
        vec.NormalizThis();
        m_GameObject->m_Transform->Translate(vec * m_Speed);
    }
}
void MoveTarget::Draw()
{
}


