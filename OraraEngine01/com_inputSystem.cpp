#include "main.h"
#include "com_inputSystem.h"
#include "input.h"
#include "gameObject.h"

void InputSystem::Init()
{
}

void InputSystem::Uninit()
{
}

void InputSystem::Update()
{
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
        vel.x = 1.0f;
    }
    if (Input::Instance().GetKeyPress('A'))
    {
        vel.x = -1.0f;
    }
    vel.NormalizThis();
    vel = vel * 0.3f;
    m_GameObject->m_Transform->Translate(vel);
}


