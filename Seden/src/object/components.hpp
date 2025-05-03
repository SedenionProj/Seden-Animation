#pragma once

namespace Seden {

	class Object;
	class Shader;

	namespace Comp {
		class Transform {
		public:
			Transform();
			explicit Transform(const glm::mat4& transform);
			explicit Transform(const glm::vec3& position);

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

			std::shared_ptr<Object> addObject(std::shared_ptr<Object> object);
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

		class Orthographic {
		public:
			explicit Orthographic(float aspectRatio);
			explicit Orthographic(float left, float right, float bottom, float top);

			glm::mat4& get();

		private:
			glm::mat4 m_projection;
		};

		class Parent {
		public:
			std::shared_ptr<Object> m_parent;
		};

		class Color {
		public:
			Color(const glm::vec4& color) 
				: m_color(color){}

			glm::vec4 m_color;
		};

		class Text {
		public:
			explicit Text(std::string text, Comp::GroupObjects* letters, float scale, const glm::vec4& color);

			void setText(const std::string& text) { m_text = text; reloadText(); };

			const std::string& getText() const { return m_text; };
			uint32_t getLength() const { return m_letters->size(); }

			void reloadText();

			float& getScale() {
				return m_scale;
			}

		public:
			float m_scale;
			std::string m_text;
			glm::vec4 m_defaultColor;
			Comp::GroupObjects* m_letters;
		};

		class Path {
		public:
			struct Vertex {
				glm::vec4 color;
				glm::vec3 position;
				float thickness;
			};
		
			Path(const std::vector<Vertex>& vertices);
			
			Path(const std::initializer_list<Comp::Path::Vertex> vertices);
				

		static bool hasVertexCountChanged;
		static size_t totalVertexCount;
		static size_t totalLineCount;

		std::vector<Vertex> m_vertices;
		};

		class Point {
		public:
			explicit Point(float thickness);
			~Point();
			float m_thickness;
			static bool hasVertexCountChanged;
			static size_t totalVertexCount;
		};

		class Shader {
		public:
			Shader(uint32_t instanceCount, std::shared_ptr<Seden::Shader> shader);

			std::shared_ptr<Seden::Shader> getShader() { return m_shader; }
		private:
			uint32_t m_instanceCount;
			std::shared_ptr<Seden::Shader> m_shader;
		};
	}
}
