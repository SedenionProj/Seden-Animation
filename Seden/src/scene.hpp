#pragma once
#include "src/renderer/renderer.hpp"
#include "src/renderer/shaderRenderer.hpp"
#include "src/animation/curve.hpp"
#include "src/animation/animation.hpp"
#include "src/util/clock.hpp"
#include "src/util/sync.h"

namespace Seden {
	class Camera;
	class window;

	class Scene {
	public:
		Scene() = default;

		virtual void animation() = 0;

		void wait(float seconds);
		void wait();

		void block();
		void unBlock();	

		void anim(Animator* anim, float time = 1, float shift = 0, Curve* curve = new EaseInOut(5));
		void animAttach(Animator* anim, float shift = 0);

		float m_animationSpeed = 1;

	protected:
		
		virtual void startAnimationLoop() = 0;
		virtual void init(Window* window) = 0;

	protected:
		entt::registry m_registry;
		Window* m_window;
		
		bool isRunning = true;

		Sync m_waitSync;
		Sync m_loopSync;		 
		bool m_waiting = false;
		float m_waitTime = 0;

		std::vector<std::unique_ptr<Animation>> m_animations;

		friend class Object;
		friend class Application;
	};

	class ObjectScene : public Scene {
	public:
		void setCamera(std::shared_ptr<Camera> camera);
		void draw();
		void startAnimationLoop() override ;
		void init(Window* window) override {
			m_window = window;
			m_renderer.init(window);
		}
		Renderer m_renderer;
	};



	class ShaderScene : public Scene {
	public:
		ShaderScene(const char* vs, const char* fs, uint32_t renderPassCount = 1) {
			m_renderer.m_shader.createShaderPath(vs, fs);
			m_renderer.m_renderPassCount = renderPassCount;
		}

		void startAnimationLoop() override;
		void init(Window* window) override;

		void addUniform(ShaderDataType type, const std::string& name, void* value, float min=0, float max=1, int precision=3) {
			m_renderer.m_uniforms.push_back({ value, name, type, {min, max, precision} });
		}

		ShaderRenderer m_renderer;
	};

	extern Scene* s_scene;
}