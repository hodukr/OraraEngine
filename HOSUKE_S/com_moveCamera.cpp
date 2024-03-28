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
	if (m_TargetName != "")
	{
		Vector3 pos = m_Target->m_Transform->GetPosition() + m_Offset;
		if (!m_IsMoveDirX)pos.x = m_GameObject->m_Transform->GetPosition().x;
		if (!m_IsMoveDirY)pos.y = m_GameObject->m_Transform->GetPosition().y;
		if (!m_IsMoveDirZ)pos.z = m_GameObject->m_Transform->GetPosition().z;
		m_GameObject->m_Transform->SetPosition(pos);
	}
}
void MoveCamera::Draw()
{

}

void MoveCamera::SetTarget()
{
	m_Target = Manager::GetScene()->GetGameObject(m_TargetName.c_str());
}
