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
    float inputvec = 1.0f;
    Vector3 vel;
    if (Input::Instance().GetKeyPress('W'))
    {
        vel.z = 1.0f;
    }

    if (Input::Instance().GetKeyPress('S'))
    {
        vel.z = -1.0f;
    }
    if (Input::Instance().GetKeyPress('D'))
    {
        inputvec *= -1.0f;
        vel.x = 1.0f;
    }
    if (Input::Instance().GetKeyPress('A'))
    {
        vel.x = -1.0f;
    }
    vel.NormalizThis();
    vel = vel * m_Speed;
    m_GameObject->m_Transform->Translate(vel);

    if (m_IsDirection && vel.LengthSpr())
    {
        D3DXVECTOR3 velx;
        vel.NormalizThis();
        VEC3D3DX(vel, velx);
        Vector3 z(0.0f, 0.0f, -1.0f);
        D3DXVECTOR3 zx = z.dx();
        float dot = D3DXVec3Dot(&velx, &zx);
        float mag1 = vel.LengthSpr();
        float mag2 = z.LengthSpr();
        // 内積の値を大きさの積で割り、acosを用いて角度を求める
        float cosTheta = dot / (mag1 * mag2);
        float angle =  acos(cosTheta) * inputvec;


        D3DXQUATERNION oquat{};
        D3DXVECTOR3 arex = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        D3DXQuaternionRotationAxis(&oquat, &arex, angle);
        m_GameObject->m_Transform->SetQuaternion(oquat);
    }
}


