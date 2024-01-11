#pragma once
#include "reflection.h"
#include "transform.h"
#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "praticle.h"
#include "plane.h"
#include "uiText.h"
#include "inputSystem.h"

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

