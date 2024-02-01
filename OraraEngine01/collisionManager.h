#pragma once

#include <list>
#include "com_collisionShape.h"

class CollisionManager
{
private:
    static std::list<CollisionShape*> m_Shape;
    static std::list<CollisionShape*> m_NextShape;

public: 
    void Init();
    void Uninit();
    void Update();

    static void SetShape(CollisionShape* shape)
    {
        m_NextShape.push_back(shape);
    }
    
    static void DeletShape(CollisionShape* deletShape)
    {
        m_NextShape.remove_if([deletShape](CollisionShape* shape) {return shape == deletShape; });
    }
};
