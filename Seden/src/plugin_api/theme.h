#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "src/plugin_api/IPlugin.h"

class Theme : public IPlugin {
public:
	virtual void select() = 0;
};