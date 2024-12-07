#pragma once
#include "src/scene.h"
#include "src/plugin_api/GUIwindow.h"
#include "src/ressource_manager/pluginSystem.h"

namespace Seden {

	class AppInfo {
	public:
		void addScene(const std::shared_ptr<Scene> scene) {
			scenes.push_back(scene);
			currentScene = scene;
		}

		void addWindows(GuiWindow* windows) {
			guiWindows.push_back(windows);
		}

		std::vector<GuiWindow*>& getWindows() {
			return guiWindows;
		}

		std::vector<std::shared_ptr<Scene>>& getScenes() {
			return scenes;
		}

	public:
		std::shared_ptr<Scene> getCurrentScene() const { return currentScene; }
		std::shared_ptr<Shape> getCurrentShape() const { return currentShape; }
		std::shared_ptr<Shape> getShapeByIndex(int idx) const { return getCurrentScene()->getShapes()[idx]; }

		void setCurrentScene(std::shared_ptr<Scene> s) { currentScene = s; }
		void setCurrentShape(std::shared_ptr<Shape> s) { currentShape = s; }

		int currentFrame = 0;
		bool isContinuousRefresh = false;

	private:
		std::vector<GuiWindow*> guiWindows;
		std::vector<std::shared_ptr<Scene>> scenes;

	private:
		std::shared_ptr<Scene> currentScene;
		std::shared_ptr<Shape> currentShape;
	};
}