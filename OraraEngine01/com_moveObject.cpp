#include "main.h"
#include "gameObject.h"
#include "com_moveObject.h"

void MoveObject::Init()
{
	m_Vel = m_Target - m_GameObject->m_Transform->GetPosition();
	m_Vel.NormalizThis();
}
void MoveObject::Uninit()
{

}
void MoveObject::EditorUpdate()
{
}
void MoveObject::Update()
{

	m_GameObject->m_Transform->Translate(m_Vel);
}
void MoveObject::Draw()
{

}
