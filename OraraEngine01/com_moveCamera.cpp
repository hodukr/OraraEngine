#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "com_moveCamera.h"

void MoveCamera::Init()
{
	SetTarget();
}
void MoveCamera::Uninit()
{

}
void MoveCamera::EditorUpdate()
{

}
void MoveCamera::Update()
{
	Vector3 pos = m_Target->m_Transform->GetPosition() + m_Offset;
	m_GameObject->m_Transform->SetPosition(pos);
}
void MoveCamera::Draw()
{

}

void MoveCamera::SetTarget()
{
	m_Target = Manager::GetScene()->GetGameObject(m_TargetName.c_str());
}
