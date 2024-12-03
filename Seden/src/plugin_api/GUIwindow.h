#pragma once
#include "src/plugin_api/IPlugin.h"

class GuiWindow : public IPlugin {
public:
	virtual void draw() = 0;
	bool show_window = true;
};