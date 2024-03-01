#include "main.h"
#include "manager.h"
#include "scene.h"
#include "gameObject.h"
#include "com_floating.h"

void Floating::Init()
{
    GameObject* gameObject = Manager::GetScene()->GetGameObject("Water");
    if (gameObject)
    {
        m_WaterSurface = gameObject->GetComponent<WaterSurface>();
    }
}
void Floating::Uninit()
{

}
void Floating::EditorUpdate()
{

}
void Floating::Update()
{
    m_Velocity -= 0.1f;
    m_GameObject->m_Transform->Translate(0.0f, m_Velocity, 0.0f);

    if (m_WaterSurface)
    {
        float groundHeight = m_WaterSurface->GetHeigt(m_GameObject->m_Transform->GetPosition());
        float difference = m_GameObject->m_Transform->GetPosition().y - groundHeight;
        if (difference < 0)
        {
            m_GameObject->m_Transform->SetPositionY(groundHeight);
            m_Velocity = 0.0f;
        }
    }

}
void Floating::Draw()
{

}
