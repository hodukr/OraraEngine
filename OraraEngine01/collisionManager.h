#pragma once

#include <list>
#include "com_collisionShape.h"

class CollisionManager
{
private:
    static std::list<CollisionShape*> m_Shape;

public: 
    void Uninit();
    void Update();

    static void SetShape(CollisionShape* shape)
    {
        m_Shape.push_back(shape);
    }
    
};
