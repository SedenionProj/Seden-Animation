#include "src/scene.h"
#include "src/applicationInfo.h"

Scene::Scene(AppInfo& appInfo)
	: m_appInfo(appInfo) {
	createFramebuffer();
}

void Scene::addShape(std::shared_ptr<Shape> shape) {
	shapes.push_back(shape);
	m_appInfo.setCurrentShape(shape);
}

void Scene::draw() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
		auto shape = *it;
		shape->shader.Bind();
		shape->shader.setMat4("iProjection", viewportInfo.projection);
		shape->shader.setInt("iFrame", m_appInfo.currentFrame);
		if (m_appInfo.currentFrame >= shape->mFrameStart && m_appInfo.currentFrame <= shape->mFrameEnd) {
			for (auto& eff : shape->getEffects()) {
				eff->update(shape->getShader());
				if (eff->reconstructShader) {
					eff->reconstructShader = false;
					shape->shader.reconstructShader(shape->getEffects());
					goto exit;
				}
			}
			shape->draw();
		}
	}
	exit:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::createFramebuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportInfo.width, viewportInfo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportInfo.width, viewportInfo.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		DEBUG_ERROR("framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
