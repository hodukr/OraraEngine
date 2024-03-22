#pragma once

#include "com_collisionShape.h"

class CollisionManager
{
private:
    static list<CollisionShape*> m_Shape;
    static list<CollisionShape*> m_NextShape;

public: 
    void Init();
    void Uninit();
    void Update();

    static void SetShape(CollisionShape* shape)
    {
        m_Shape.push_back(shape);
    }
    
    static void DeletShape(CollisionShape* deletShape)
    {
        m_Shape.remove_if([deletShape](CollisionShape* shape) {return shape == deletShape; });
    }
};
