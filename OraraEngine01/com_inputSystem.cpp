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
        D3DXVECTOR3 velx;
        vel.NormalizThis();       
        VEC3D3DX(vel, velx);
        D3DXVECTOR3 fowerd;
        fowerd.x = sinf(m_angle);
        fowerd.z = cosf(m_angle);
        fowerd.y = 0.0f;
        D3DXVECTOR3 cross;
        D3DXVec3Cross(&cross, &fowerd, &velx);
        float dot = D3DXVec3Dot(&fowerd, &velx);
        float cos = dot / (D3DXVec3Length(&fowerd) * D3DXVec3Length(&velx));
        float rot = acos(cos);
        if (rot >= D3DX_PI)rot -= D3DX_PI;
        if (cross.y > 0)
        {
            if (rot < 0.2f)m_angle -= rot;
            else m_angle -= 0.2f;
        }
        else
        {
            if (rot < 0.2f)m_angle += rot;
            else m_angle += 0.2f;

        }
        D3DXQUATERNION oquat{};
        D3DXVECTOR3 arex = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        D3DXQuaternionRotationAxis(&oquat, &arex, m_angle);
        m_GameObject->m_Transform->SetQuaternion(oquat);
    }
}


