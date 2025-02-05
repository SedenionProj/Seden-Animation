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

    struct ApplicationInfo {
	//	./testApp -vsync -rec -res=1920x1080 -fps=30 -o="path/to/video"
        enum Options {
            RECORD = 1<<0,
			VSYNC  = 1<<1
        };

		int optionsFlags = 0;

		glm::vec2 resolution = { 1280,720 };

		std::string path = "animation";

		uint32_t frameRate = 60;
		float speed = 1;
		
    };
}