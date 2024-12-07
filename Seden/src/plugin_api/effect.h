#pragma once
#include <string>

namespace Seden {
	class ShaderSystem;

	class Effect {
	public:
		enum class Type {
			VERTEX,
			FRAGMENT
		};

		Effect() {}
		virtual ~Effect() = default;
		virtual void drawGUI() = 0;
		virtual void update(ShaderSystem& shader) = 0;
		virtual std::string getShaderCode() = 0;
		void setName(const std::string& name) { m_name = name; }
		std::string& getName() { return m_name; }

		Type type = Type::FRAGMENT;
		bool reconstructShader = false;
	protected:
		std::string m_name = "Effect";
	};
}