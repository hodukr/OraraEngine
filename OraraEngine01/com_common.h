#pragma once
#include "reflection.h"
#include "com_transform.h"
#include "com_mesh.h"
#include "com_camera.h"
#include "com_material.h"
#include "com_praticle.h"
#include "com_plane.h"
#include "com_uiText.h"
#include "com_inputSystem.h"
#include "col_boxCollision.h"

#define SET_COMPONENT_CLASS(Classname)\
CEREAL_REGISTER_TYPE(Classname)\
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Classname)

SET_COMPONENT_CLASS(Transform)
SET_COMPONENT_CLASS(Mesh)
SET_COMPONENT_CLASS(Camera)
SET_COMPONENT_CLASS(PraticleSystem)
SET_COMPONENT_CLASS(Plane)
SET_COMPONENT_CLASS(Text)
SET_COMPONENT_CLASS(InputSystem)
SET_COMPONENT_CLASS(CollisionShape)
SET_COMPONENT_CLASS(BoxCollision)

