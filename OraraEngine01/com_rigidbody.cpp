#include "main.h"
#include "gameObject.h"
#include "com_rigidbody.h"

void Rigidbody::Init()
{

}
void Rigidbody::Uninit()
{

}
void Rigidbody::EditorUpdate()
{

}
void Rigidbody::Update()
{
	m_GameObject->m_Transform->Translate(0.0f, m_Gravity,0.0f);
}
void Rigidbody::Draw()
{

}
