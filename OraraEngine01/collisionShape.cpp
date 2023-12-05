#include "main.h"
#include "renderer.h"
#include "collisionShape.h"
#include "collisionManager.h"

CollisionShape::CollisionShape()
{
    CollisionManager::SetShape(this);
}
