#pragma once
#include <vector>
#include <string>

#include "src/plugin_api/effect.h"
#include "src/ressource_manager/ShaderSystem.h"

class Shape {
public:
	void setName(const std::string& name) { m_name = name; }

	std::string& getName() { return m_name; }
	std::vector<std::shared_ptr<Effect>>& getEffects() { return effects; }
	ShaderSystem& getShader() { return shader; }

	virtual void draw() = 0; // todo batch renderer

	void addEffect(std::shared_ptr<Effect> effect) {
		effects.push_back(effect); 
		shader.reconstructShader(effects);
	}

	ShaderSystem shader;

	int mType;
	int mFrameStart = 0, mFrameEnd = 10;
	bool mExpanded;
	int instanceCount = 1;
private:
	std::string m_name = "shape";
	std::vector<std::shared_ptr<Effect>> effects;
};