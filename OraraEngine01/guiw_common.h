#pragma once
#include "guiw_gameManagerGui.h"
#include "guiw_hierarchy.h"
#include "guiw_inspector.h"
#include "guiw_menu.h"
#include "guiw_nodeEditor.h"
#include "accessFolder.h"

#define SET_IMGUIWINDOW_CLASS(Classname)\
CEREAL_REGISTER_TYPE(Classname)\
CEREAL_REGISTER_POLYMORPHIC_RELATION(ImGuiWindow, Classname);

SET_IMGUIWINDOW_CLASS(GameManagerGui)
SET_IMGUIWINDOW_CLASS(Hierarchy)
SET_IMGUIWINDOW_CLASS(Inspector)
SET_IMGUIWINDOW_CLASS(Menu)
SET_IMGUIWINDOW_CLASS(NodeEditorManager)
SET_IMGUIWINDOW_CLASS(AccessFolder)



