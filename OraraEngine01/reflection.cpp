#include "main.h"
#include "reflection.h"
#include "com_common.h"
#include "com_cloth.h"
#include "com_waterSurface.h"


std::vector<ReflectionList::IReflection> ReflectionList::m_InstansList{};

SetReflectionComponent(Transform)
SetReflectionComponent(Mesh)
SetReflectionComponent(Camera)
SetReflectionComponent(PraticleSystem)
SetReflectionComponent(Plane)
SetReflectionComponent(Text)
SetReflectionComponent(InputSystem)
SetReflectionComponent(Cloth)
SetReflectionComponent(WaterSurface)

//#include <cereal/cereal.hpp>
//#include <cereal/types/memory.hpp>
//#include <cereal/types/polymorphic.hpp>



//SetReflectionComponent(Material)

//CEREAL_REGISTER_TYPE_WITH_NAME(Mesh, "Mesh")
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Transform)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Mesh)
