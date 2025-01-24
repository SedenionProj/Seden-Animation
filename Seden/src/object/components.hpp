#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_truetype.h>

namespace Seden {

	class Object;

	namespace Comp {
		class Transform {
		public:
			Transform();
			explicit Transform(const glm::mat4& transform);

			operator glm::mat4& ();
			operator const glm::mat4& () const;

			glm::vec3 getPosition() const;
			glm::quat getRotation() const;
			glm::vec3 getScale() const;

			glm::mat4& getTransform();
			const glm::mat4& getTransform() const;

			void setPosition(const glm::vec3& position);
			void setRotation(const glm::quat& rotation);
			void setScale(const glm::vec3& scale);

			void translate(const glm::vec3& offset);
			void rotate(const glm::quat& rotation);
			void scale(const glm::vec3& factor);

		private:
			glm::mat4 m_transform;
		};

		class PolygonMesh {
		public:
			struct Vertex {
				glm::vec3 position = { 0, 0, 0 };
				glm::vec3 color = { 1, 1, 1 };
			};

			explicit PolygonMesh(const std::initializer_list<Vertex>& mesh);

			~PolygonMesh();

			void setColor(const glm::vec3& color);
			void setColorGradient(const std::initializer_list<glm::vec3>& grad);

			Vertex& getVertex(uint32_t index);
			size_t getsize() const;
			const Vertex* getdata() const;
			size_t getVertexCount() const;
			std::vector<Vertex>& getVertices();
			const std::vector<Vertex>& getVertices() const;

			static bool hasVertexCountChanged;
			static size_t totalVertexCount;

		private:
			std::vector<Vertex> m_vertices;
		};

		class GroupObjects {
		public:
			GroupObjects();
			explicit GroupObjects(std::vector<std::shared_ptr<Object>> objects);
			GroupObjects(std::initializer_list<std::shared_ptr<Object>> objects);
			GroupObjects(GroupObjects&& other) noexcept;
			GroupObjects(const GroupObjects&) = delete;

			void addObject(const std::shared_ptr<Object>& object);
			void removeObject(size_t index);
			std::shared_ptr<Object> getObject(size_t index) const;
			size_t size() const;
			void clear();

			auto begin() { return m_objects.begin(); }
			auto end() { return m_objects.end(); }
			auto begin() const { return m_objects.begin(); }
			auto end() const { return m_objects.end(); }

		private:
			std::vector<std::shared_ptr<Object>> m_objects;
		};

		class Perspective {
		public:
			explicit Perspective(float aspectRatio);

			glm::mat4& get();

		private:
			glm::mat4 m_projection;
		};

		class Text {
		public:
			struct Vertex {
				glm::vec2 pos;
				glm::vec2 texCoords;
			};

			Text();
			explicit Text(std::string text);

			void setText(const std::string& text);

			static bool reconstructVB;

		private:
			void reloadText();
			uint32_t ftex = 0;
			stbtt_bakedchar cdata[96]; // ASCII 32..126
			int texResolution;
			std::filesystem::path fontPath;
			std::string m_text;
		};
	}
}
