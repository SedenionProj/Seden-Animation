#pragma once

#define PLUGIN_API __declspec(dllexport)

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Seden {

	class IPlugin {
	public:
		virtual ~IPlugin() = default;
	};
}