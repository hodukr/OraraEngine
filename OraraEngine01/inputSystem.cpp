#include "main.h"
#include "inputSystem.h"
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
    if (Input::GetKeyPress('W'))
    {
        vel.z = 1.0f;
    }

    if (Input::GetKeyPress('S'))
    {
        vel.z = -1.0f;
    }
    if (Input::GetKeyPress('D'))
    {
        vel.x = 1.0f;
    }
    if (Input::GetKeyPress('A'))
    {
        vel.x = -1.0f;
    }
    vel.NormalizThis();
    m_GameObject->m_Transform->Translate(vel);
}


