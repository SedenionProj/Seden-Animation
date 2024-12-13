#pragma once
#include <vector>
#include <string>

#include "src/plugin_api/effect.h"
#include "src/ressource_manager/ShaderSystem.h"

namespace Seden {

	class Shape {
	public:
		Shape(std::shared_ptr<Scene> scene) : m_scene(scene) {}

		void setName(const std::string& name) { m_name = name; }

		std::string& getName() { return m_name; }
		std::vector<std::shared_ptr<Effect>>& getEffects() { return effects; }
		ShaderSystem& getShader() { return shader; }

		virtual void draw() = 0; // todo batch renderer
		virtual void drawGui() = 0;

		void addEffect(std::shared_ptr<Effect> effect) {
			effects.push_back(effect);
			shader.reconstructShader(effects);
		}

		ShaderSystem shader;

		int mType;
		int mFrameStart = 0, mFrameEnd = 10;
		bool mExpanded;
		int instanceCount = 1;
	protected:
		std::shared_ptr<Scene> m_scene;
		std::string m_name = "shape";
		std::vector<std::shared_ptr<Effect>> effects;
	};
}