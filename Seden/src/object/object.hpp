#pragma once
#include <entt/entt.hpp>
#include <glm/matrix.hpp>
#include "src/scene.hpp"
#include "src/object/components.hpp"

namespace Seden {
	class Object {
	public:
		Object(Scene& scene) : m_scene(scene) {
			m_entity = m_scene.getRegistry().create();
		}

		static std::shared_ptr<Object> create(Scene& scene) {
			return std::make_shared<Object>(scene);
		}

		template<typename T, typename... Args>
		T& add(Args&&... args) {
			return m_scene.getRegistry().emplace<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T, typename I>
		T& add(const std::initializer_list<I>& il) {
			return m_scene.getRegistry().emplace<T>(m_entity, il);
		}

		template<typename T>
		T& get() {
			// todo: maybe add a safety check, or add "getOrCreate" function
			return m_scene.getRegistry().get<T>(m_entity);
		}
		template<typename... Args>
		bool has() {
			return m_scene.getRegistry().all_of<Args...>(m_entity);
		}

		template<typename T>
		void remove() {
			m_scene.getRegistry().remove<T>(m_entity);
		}

	private:
		entt::entity m_entity;
		Scene& m_scene;
	};

	class Polygon : public Object {
	public:
		static std::shared_ptr<Polygon> create(Scene& scene, const std::initializer_list<PolygonMesh::Vertex> vertices) {
			return std::make_shared<Polygon>(scene, vertices);
		}

		Polygon(Scene& scene, const std::initializer_list<PolygonMesh::Vertex> vertices) : Object(scene) {
			add<Transform>(glm::mat4(1));
			add<PolygonMesh, PolygonMesh::Vertex>(vertices);
		}
	};

	class Quad : public Object {
	public: 
		static std::shared_ptr<Quad> create(Scene& scene) {
			return std::make_shared<Quad>(scene);
		}

		Quad(Scene& scene) : Object(scene) {
			add<Transform>(glm::mat4(1));
			add<PolygonMesh, PolygonMesh::Vertex>({ 
				{ glm::vec3(-0.5f, -0.5f,0) }, 
				{ glm::vec3(0.5f,  -0.5f,0) },
				{ glm::vec3(0.5f,   0.5f,0) },
				{ glm::vec3(-0.5f,  0.5f,0) }
				});
		}
	};

	class PerspectiveCamera : public Object {
	public:
		static std::shared_ptr<PerspectiveCamera> create(Scene& scene) {
			return std::make_shared<PerspectiveCamera>(scene);
		}

		glm::mat4 getView() {
			auto& t = get<Transform>();
			glm::vec3 front = glm::normalize(glm::rotate(t.getRotation(), glm::vec3(0, 0, 1)));
			glm::vec3 pos = t.getPosition();
			return glm::lookAt(pos, pos + front, glm::vec3(0, 1, 0));
		}

		glm::mat4& getProjection() {
			return get<Perspective>().get();
		}

		PerspectiveCamera(Scene& scene) : Object(scene) {
			auto& t = add<Transform>(glm::mat4(1));
			t.setPosition({ 0, 0, -2 });
			add<Perspective>(16.f / 9.f);
		}
	};
}