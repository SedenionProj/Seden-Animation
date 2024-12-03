// cl @compile.rsp

#define PLUGIN_EXPORTS
#include "../plugin_api/GUIwindow.h"
#include <iostream>



class Plugin1 : public GuiWindow {
public:
	void draw() override {
		std::cout << "Gui Drawn "<< std::endl;
		
		ImGui::ShowAboutWindow();
	}
};

extern "C" PLUGIN_API IPlugin* create(ImGuiContext* context) {
	std::cout << "Plugin1 created" << std::endl;
	ImGui::SetCurrentContext(context);
	IPlugin* ptr = new Plugin1();
	return ptr;
}

extern "C" PLUGIN_API void destroy(IPlugin* plugin) {
	std::cout << "Plugin1 deleted" << std::endl;
	delete plugin;
}
