#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "com_moveCamera.h"

void MoveCamera::Init()
{
	m_Target = Manager::GetScene()->GetGameObject("MainCamera");
}
void MoveCamera::Uninit()
{

}
void MoveCamera::EditorUpdate()
{

}
void MoveCamera::Update()
{

}
void MoveCamera::Draw()
{

}
