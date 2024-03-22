#include "main.h"
#include "reflection.h"
#include "com_common.h"



vector<ReflectionList::IReflection> ReflectionList::m_InstansList{};

SetReflectionComponent(Transform)
SetReflectionComponent(Mesh)
SetReflectionComponent(Camera)
SetReflectionComponent(PraticleSystem)
SetReflectionComponent(Plane)
SetReflectionComponent(Texture)
SetReflectionComponent(InputSystem)
SetReflectionComponent(Cloth)
SetReflectionComponent(WaterSurface)
SetReflectionComponent(BoxCollision)
SetReflectionComponent(SphereCollision)
SetReflectionComponent(Rigidbody)
SetReflectionComponent(Player)
SetReflectionComponent(ComponentPost)
SetReflectionComponent(Enemy)
SetReflectionComponent(Audio)
SetReflectionComponent(MoveCamera)
SetReflectionComponent(SpriteAnimation)
SetReflectionComponent(ChangeScene)
SetReflectionComponent(Floating)
SetReflectionComponent(RotObject)
SetReflectionComponent(Goal)
SetReflectionComponent(ShakeCamera)

static ReflectionList::Reflection<Component> reflection(true);
SetReflectionComponent(MoveObject)
