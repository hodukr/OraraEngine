#include "main.h"
#include "reflection.h"
#include "transform.h"
#include "mesh.h"
#include "cameracm.h"
#include "material.h"
//#include <cereal/cereal.hpp>
//#include <cereal/types/memory.hpp>
//#include <cereal/types/polymorphic.hpp>

std::vector<ReflectionList::IReflection> ReflectionList::m_InstansList;
SetReflectionComponent(Transform)
SetReflectionComponent(Mesh)
SetReflectionComponent(Cameracm)
SetReflectionComponent(Material)

//CEREAL_REGISTER_TYPE_WITH_NAME(Mesh, "Mesh")
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Transform)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Mesh)
