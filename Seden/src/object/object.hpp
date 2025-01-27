#pragma once
#include <entt/entt.hpp>
#include <glm/matrix.hpp>
#include <string>
#include "src/scene.hpp"
#include "src/object/components.hpp"

namespace Seden {
	class Object {
	public:
		Object(Scene& scene) : m_scene(scene) {
			m_entity = m_scene.m_registry.create();
		}

		static std::shared_ptr<Object> create(Scene& scene) {
			return std::make_shared<Object>(scene);
		}

		template<typename T, typename... Args>
		T& add(Args&&... args) {
			return m_scene.m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T, typename I>
		T& add(const std::initializer_list<I>& il) {
			return m_scene.m_registry.emplace<T>(m_entity, il);
		}

		template<typename T>
		T& get() {
			// todo: maybe add a safety check, or add "getOrCreate" function
			return m_scene.m_registry.get<T>(m_entity);
		}

		template<typename T>
		T& tryGet() {
			return m_scene.m_registry.try_get<T>(m_entity);
		}

		/*
		template<typename T>
		auto getRecurse(auto(*op)(T&, T&)) {
			T* comp = tryGet<T>();
			Comp::Parent* parent = tryGet<Comp::Parent>();
			auto result = op(comp, par->;
			while ( par->m_parent)
			{
				if()
			}

			return result;
		}
		*/

		template<typename... Args>
		bool has() {
			return m_scene.m_registry.all_of<Args...>(m_entity);
		}

		template<typename T>
		void remove() {
			m_scene.m_registry.remove<T>(m_entity);
		}

	private:
		Scene& m_scene;
		entt::entity m_entity;
	};
	class ConvexPolygon : public Object {
	public:
		static std::shared_ptr<ConvexPolygon> create(Scene& scene, const std::initializer_list<Comp::PolygonMesh::Vertex> vertices) {
			return std::make_shared<ConvexPolygon>(scene, vertices);
		}

		ConvexPolygon(Scene& scene, const std::initializer_list<Comp::PolygonMesh::Vertex> vertices) : Object(scene) {
			add<Comp::Transform>(glm::mat4(1));
			add<Comp::PolygonMesh, Comp::PolygonMesh::Vertex>(vertices);
		}
	};

	class Quad : public Object {
	public: 
		static std::shared_ptr<Quad> create(Scene& scene) {
			return std::make_shared<Quad>(scene);
		}

		Quad(Scene& scene) : Object(scene) {
			add<Comp::Transform>(glm::mat4(1));
			add<Comp::PolygonMesh, Comp::PolygonMesh::Vertex>({
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
		    auto& t = get<Comp::Transform>();
		    glm::quat rotation = t.getRotation();
		    glm::vec3 pos = t.getPosition();
		
		    glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, 1.0f);
		    glm::vec3 up = rotation * glm::vec3(0.0f, -1.0f, 0.0f);
		
		    return glm::lookAt(pos, pos + front, up);
		}

		glm::mat4& getProjection() {
			return get<Comp::Perspective>().get();
		}

		PerspectiveCamera(Scene& scene) : Object(scene) {
			auto& t = add<Comp::Transform>(glm::mat4(1));
			t.setPosition({ 0, 0, -2 });
			add<Comp::Perspective>(16.f / 9.f);
		}
	};

	class SimpleText : public Object {
	public:
		static std::shared_ptr<SimpleText> create(Scene& scene, const std::string& text) {
			return std::make_shared<SimpleText>(scene, text);
		}

		SimpleText(Scene& scene, const std::string& text) : Object(scene) {
			add<Comp::Transform>(glm::mat4(1));
			add<Comp::Text>(text);
		}
	};
}