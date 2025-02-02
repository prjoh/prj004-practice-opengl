#pragma once

#include <glad/gl.h>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


namespace core
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };


	class Geometry
	{
	public:
        Geometry() = default;
        virtual ~Geometry() = default;

        void computeNormals();
        void bind();
        GLsizei size() { return (GLsizei)m_indices.size(); }
	protected:
        void initialize();

        std::vector<Vertex>       m_vertices;
        std::vector<unsigned int> m_indices;
        GLuint m_vao{};
        GLuint m_vbo{};
        GLuint m_ebo{};
	};


    class Triangle : public Geometry
    {
    public:
        Triangle() = delete;
        Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    };


    class Plane : public Geometry {
    public:
        Plane() = delete;
        Plane(unsigned int width = 1, unsigned int height = 1, unsigned int width_segments = 1, unsigned int height_segments = 1);
    };

    class Cube : public Geometry {
    public:
        Cube() = delete;
        Cube(int width = 1, int height = 1, int depth = 1, int width_segments = 1, int height_segments = 1, int depth_segments = 1);
    private:
        void buildPlane(int u, int v, int w, int u_dir, int v_dir, int width, int height, int depth, int grid_x, int grid_y, unsigned int* num_vertices);
    };

    class Sphere : public Geometry {
    public:
        Sphere() = delete;
        Sphere(float radius, int width_segments, int height_segments, float phi_start, float phi_length, float theta_start, float theta_length);
    };
}
