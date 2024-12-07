#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>

#include "src/plugin_api/effect.h"

namespace Seden {

	inline std::string colEff = R"(

uniform vec3 solidColorEff_color;

vec4 _main(vec4 frag){
	return vec4(solidColorEff_color,1);
})";

	class SolidColorEff : public Effect {
	public:
		SolidColorEff() {
			setName("SolidColor");
		}
		void drawGUI() override {
			ImGui::SliderFloat3("rgb", rgb, 0, 1);
		};
		void update(ShaderSystem& shader) override {
			shader.setVec3("solidColorEff_color", glm::vec3(rgb[0], rgb[1], rgb[2]));
		};

		std::string getShaderCode() override {
			return colEff;
		}
	private:
		float rgb[3] = { 0.5,0.2,0.1 };
	};

	class Transform : public Effect {
	public:
		Transform() {
			setName("Transform");
		}

		void drawGUI() override {
			bool updated = false;
			if (ImGui::Button("reset")) {
				position = { 0.0f, 0.0f, 0.0f };
				rotation = { 0.0f, 0.0f, 0.0f };
				scale = { 1.0f, 1.0f, 1.0f };
				updated = true;
			}
			if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f)) { updated = true; }
			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.01f)) { updated = true; }
			if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f)) { updated = true; }

			if (updated) { updateModelMatrix(); }
		}

		void update(ShaderSystem& shader) override {
			shader.setMat4("iModel", model);
		}

		std::string getShaderCode() override { return ""; }

	private:
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 model = glm::mat4(1.0f);

		void updateModelMatrix() {
			model = glm::mat4(1.0f);

			model = glm::translate(model, position);
			model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, scale);

		}
	};

	class Instance : public Effect {
	public:
		Instance() {
			setName("Instance");
			type = Effect::Type::VERTEX;
		}
		void drawGUI() override {
			ImVec2 wsize = ImGui::GetWindowSize();
			wsize.x -= 10;
			ImGui::InputTextMultiline("##", &code, wsize);
			if (ImGui::BeginMenu("Input variables")) {
				ImGui::Text("gl_InstanceID\niFrame");
				ImGui::EndMenu();
			}
			if (ImGui::Button("compile")) {
				reconstructShader = true;
			}
		};
		void update(ShaderSystem& shader) override {

		};

		std::string getShaderCode() override {
			return code;
		}

		bool recreateShader;
	private:
		std::string code = "gl_Position = iProjection * iModel * vec4(aPos, 0, 1);";
	};

	//float moveL = (gl_InstanceID % 8) * 0.3;
	//float moveB = (gl_InstanceID / 8) * 0.3;
	//gl_Position = iProjection * iModel * vec4(sin((iFrame+gl_InstanceID)/10.)*0.1 * aPos - 1.f + vec2(moveL, moveB), 0, 1);
}