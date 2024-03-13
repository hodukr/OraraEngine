#pragma once
#include "guiw_hierarchy.h"
#include "guiw_inspector.h"
#include "guiw_menu.h"
#include "guiw_nodeEditor.h"
#include "guiw_accessFolder.h"
#include "guiw_sceneWindow.h"
#include "guiw_debug.h"

#define SET_IMGUIWINDOW_CLASS(Classname)\
CEREAL_REGISTER_TYPE(Classname)\
CEREAL_REGISTER_POLYMORPHIC_RELATION(GuiWindowBase, Classname);

SET_IMGUIWINDOW_CLASS(Hierarchy)
SET_IMGUIWINDOW_CLASS(Inspector)
SET_IMGUIWINDOW_CLASS(Menu)
SET_IMGUIWINDOW_CLASS(NodeEditorManager)
SET_IMGUIWINDOW_CLASS(AccessFolder)
SET_IMGUIWINDOW_CLASS(SceneWindow)
SET_IMGUIWINDOW_CLASS(Debug);



