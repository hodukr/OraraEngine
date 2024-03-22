#include "main.h"
#include "gameObject.h"
#include "com_moveObject.h"

void MoveObject::Init()
{
	m_NewTarget = m_Target;
	m_StatePos = m_GameObject->m_Transform->GetPosition();
	m_Vel = m_NewTarget - m_GameObject->m_Transform->GetPosition();
	m_Vel.NormalizThis();
	m_Vel = m_Vel * m_Speed;
}
void MoveObject::Uninit()
{

}
void MoveObject::EditorUpdate()
{
}
void MoveObject::Update()
{
	float vel = (m_GameObject->m_Transform->GetPosition() - m_NewTarget).LengthSpr();
	
	if (vel > m_Speed)
	{
		
		m_GameObject->m_Transform->Translate(m_Vel);
	}
	else 
	{
		m_GameObject->m_Transform->SetPosition(m_NewTarget);
		if (m_IsLoop)
		{
			m_Vel = m_Vel * -1.0f;
			if (m_NewTarget == m_StatePos)m_NewTarget = m_Target;
			else m_NewTarget = m_StatePos;
		}
	}
}
void MoveObject::Draw()
{

}
