#include <entt/entt.hpp>
#include "src/scene.hpp"
#include "src/components.hpp"

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
		template<typename T>
		T& get() {
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

	class Quad : public Object {
	public: 
		Quad(Scene& scene) : Object(scene) {
			add<Transform>();
			//add<PolygonMesh>();
		}
	};
}