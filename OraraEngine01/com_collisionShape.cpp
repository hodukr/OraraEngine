#include "main.h"
#include "renderer.h"
#include "com_collisionShape.h"
#include "collisionManager.h"

CollisionShape::CollisionShape()
{
    CollisionManager::SetShape(this);
}
