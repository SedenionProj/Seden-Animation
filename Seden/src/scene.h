#pragma once
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/plugin_api/shape.h"
#include "src/logger.h"

class AppInfo;

struct ViewportInfo {
	enum Projection {
		NO_PROJECTION,
		PERSPECTIVE,
		ORTHOGRAPHIC
	};
	ViewportInfo() {
		projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
	}

	void setProjection(Projection proj) {
		switch (proj) {
		case ViewportInfo::Projection::NO_PROJECTION:
			projection = glm::mat4(1);
			break;
		case ViewportInfo::Projection::PERSPECTIVE:
			projection = glm::perspective(glm::radians(90.f), aspectRatio, 0.1f, 100.f);
			break;
		case ViewportInfo::Projection::ORTHOGRAPHIC:
			projection = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
			break;
		default:
			DEBUG_ERROR("invalid projection mode");
			break;
		}
	}

	glm::mat4 projection;
	uint32_t width = 1280;
	uint32_t height = 720;
	uint32_t fps = 60;
	uint32_t mFrameMin = 0;
	uint32_t mFrameMax = 100;
	float aspectRatio = (float) width/ height;
};

class Scene {
public:
	Scene(AppInfo& appInfo);

	std::vector<std::shared_ptr<Shape>>& getShapes() {
		return shapes;
	}

	void addShape(std::shared_ptr<Shape> shape);

	void draw();

	void createFramebuffer();

	ViewportInfo viewportInfo;
	GLuint textureColorbuffer;
	GLuint framebuffer;
private:
	
	std::vector<std::shared_ptr<Shape>> shapes;
	
	AppInfo& m_appInfo;
};