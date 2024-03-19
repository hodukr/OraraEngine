#include "main.h"
#include "com_inputSystem.h"
#include "input.h"
#include "gameObject.h"
#include<math.h>
void InputSystem::Init()
{
}

void InputSystem::Uninit()
{
}

void InputSystem::Update()
{
    Vector3 vel{};
    D3DXQUATERNION oquat{};
    D3DXQUATERNION quaternion = m_GameObject->m_Transform->GetQuaternion();
    D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


    if (Input::Instance().GetKeyPress('W'))
    {
        vel.z += 1.0f;
    }

    if (Input::Instance().GetKeyPress('S'))
    {
        vel.z -= 1.0f;
    }
    if (Input::Instance().GetKeyPress('D'))
    {
        vel.x += 1.0f;
    }
    if (Input::Instance().GetKeyPress('A'))
    {
        vel.x -= 1.0f;
    }
    vel.NormalizThis();
    m_Vel += vel * m_Acceleration;
    m_Vel = m_Vel * 0.6f;
    m_GameObject->m_Transform->Translate(m_Vel);
    

    if (m_IsDirection && vel.LengthSpr())
    {
        float angle = atan2f(vel.x, vel.z);
        angle += D3DXToRadian(m_OffsetY);
        D3DXQuaternionRotationAxis(&oquat, &axis, angle);
        //‹…–ÊüŒ`•âŠÔ
        D3DXQuaternionSlerp(&quaternion, &quaternion, &oquat, 0.3f);
        m_GameObject->m_Transform->SetQuaternion(quaternion);

    }
}


