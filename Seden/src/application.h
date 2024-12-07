#pragma once
#include <iostream>
#include "src/renderer.h"
#include "src/window.h"
#include "src/scene.h"
#include "src/applicationInfo.h"
#include "src/ressource_manager/pluginSystem.h"
#include "src/default_Items/defaultWindows.h"



namespace Seden {
	PluginSystem pluginSystem;

	class Application {
	public:
		void start() {
			appInfo.addScene(std::make_shared<Scene>(appInfo));
			pluginSystem.setRelativeCurrentPath();

			//std::vector<PluginHandle> windowPlugins;
			//pluginSystem.loadPlugins("./plugin_api", windowPlugins);
			//
			//for(auto& win : windowPlugins)
			//	appInfo.addWindows(static_cast<GuiWindow*>(win.plugin));
			appInfo.addWindows(new CompositionWin(appInfo));
			appInfo.addWindows(new PrimitivesWin(appInfo));
			appInfo.addWindows(new EffectsWin(appInfo));
			appInfo.addWindows(new Timeline(appInfo));
			appInfo.addWindows(new TopBar(appInfo));

			while (window.isRunning()) {
				renderer.beginFrame();
				renderer.drawFrame();
				renderer.endFrame();
			}

			//pluginSystem.unloadPlugins(windowPlugins);
			glfwTerminate();
		}



		~Application() {

		}

	private:
		uint32_t width = 1280, height = 720;
		Window window{ width, height };
		AppInfo appInfo{};
		Renderer renderer{ window,appInfo };
	};
}