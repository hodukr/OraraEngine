#include "main.h"
#include "gameObject.h"
#include "com_collisionShape.h"


void CollisionShape::EditorUpdate()
{
	m_Position = m_GameObject->m_Transform->GetPosition() + m_Offset;
}
