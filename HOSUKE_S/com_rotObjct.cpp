#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "input.h"
#include "com_rotObject.h"


void RotObject::Update()
{
    
    D3DXQUATERNION oquat{};
    static float rot;
    rot += m_Speed;
    D3DXQuaternionRotationAxis(&oquat, m_Axis, rot);
    m_GameObject->m_Transform->SetQuaternion(oquat);
}


