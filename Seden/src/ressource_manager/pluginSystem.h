#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>

#include "src/plugin_api/IPlugin.h"

namespace fs = std::filesystem;
typedef IPlugin* (*CreatePlugin)(ImGuiContext*);
typedef void (*DestroyPlugin)(IPlugin*);

struct PluginHandle {
	HMODULE handle;
	IPlugin* plugin;
	DestroyPlugin destroy;
};

class PluginSystem {
public:
	void loadPlugins(const std::string& pluginDir, std::vector<PluginHandle>& plugins) {
		for (const auto& entry : fs::directory_iterator(pluginDir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".dll") {
				std::wstring path = entry.path().wstring();
				HMODULE handle = LoadLibraryW(path.c_str());

				if (!handle) {
					std::cerr << "Cannot load plugin: " << GetLastError() << std::endl;
					continue;
				}

				// Load the create and destroy functions
				CreatePlugin createPlugin = (CreatePlugin)GetProcAddress(handle, "create");
				DestroyPlugin destroyPlugin = (DestroyPlugin)GetProcAddress(handle, "destroy");

				if (!createPlugin || !destroyPlugin) {
					std::cerr << "Cannot load functions 'create' or 'destroy': " << GetLastError() << std::endl;
					FreeLibrary(handle);
					continue;
				}

				// Create and store the plugin
				IPlugin* plugin = createPlugin(ImGui::GetCurrentContext());
				
				if (plugin) {
					plugins.push_back({ handle, plugin, destroyPlugin });
				}
				else {
					FreeLibrary(handle);
				}
			}
		}
	}

	void unloadPlugins(std::vector<PluginHandle>& plugins) {
		for (auto& pluginHandle : plugins) {
			pluginHandle.destroy(pluginHandle.plugin);
			FreeLibrary(pluginHandle.handle);
		}
		plugins.clear();
	}

	void setRelativeCurrentPath() {
		std::filesystem::path sourceFilePath(__FILE__);
		std::filesystem::path sourceDir = sourceFilePath.parent_path();
		std::filesystem::current_path(sourceDir);
	}
};
