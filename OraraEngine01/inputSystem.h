#pragma once
#include "component.h"

class InputSystem :public Component
{
private:

public:
    void Init()override;
    void Uninit()override;
    void Update()override;
};

