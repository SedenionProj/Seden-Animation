#pragma once
#include <iostream>
#include "src/window.hpp"
#include "src/scene.hpp"

namespace Seden {
	class Application {
	public:
		void start();

		virtual void animation() = 0;

		~Application();
	
	private:
		uint32_t m_width = 1280, m_height = 720;
		Window m_window{ m_width, m_height, "application" };

	public:
		Scene scene{ m_window };
	};
}