#include <Geometry/Geometry.h>

#include <glm/glm.hpp>


#define _USE_MATH_DEFINES
#include <math.h>


void core::Geometry::initialize()
{
	// Create vertex array object
	glGenVertexArrays(1, &m_vao);

	// Create buffers
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);  // TODO: STATIC_DRAW option should be parameterized??

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);  // TODO: STATIC_DRAW option should be parameterized??

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}


// As described by Inigo Quilez: https://iquilezles.org/articles/normals/
void core::Geometry::computeNormals()
{
	for (auto& v : m_vertices) v.normal = { 0.0f, 0.0f, 0.0f };

	for (unsigned int i = 0; i < m_indices.size();) {
		unsigned int ia = m_indices[i];
		unsigned int ib = m_indices[i + 1];
		unsigned int ic = m_indices[i + 2];

		glm::vec3 e1 = m_vertices[ia].position - m_vertices[ib].position;
		glm::vec3 e2 = m_vertices[ic].position - m_vertices[ib].position;
		glm::vec3 no = glm::cross(e1, e2);

		m_vertices[ia].normal += no;
		m_vertices[ib].normal += no;
		m_vertices[ic].normal += no;

		i = i + 3;
	}

	for (auto& v : m_vertices) v.normal = glm::normalize(v.normal);
}

void core::Geometry::bind()
{
	glBindVertexArray(m_vao);
}


core::Triangle::Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	Vertex v1{}, v2{}, v3{};
	v1.position = p1;
	v2.position = p2;
	v3.position = p3;

	m_vertices.insert(m_vertices.end(), { v1, v2, v3 });
	m_indices.insert(m_indices.end(), { 0, 1, 2 });

	computeNormals();
	initialize();
}


core::Plane::Plane(unsigned int width, unsigned int height, unsigned int width_segments, unsigned int height_segments)
{
	float width_half = width * 0.5f;
	float height_half = height * 0.5f;

	unsigned int grid_x = width_segments;
	unsigned int grid_y = height_segments;

	unsigned int  grid_x1 = grid_x + 1;
	unsigned int  grid_y1 = grid_y + 1;

	float segment_width = (float)width / (float)grid_x;
	float segment_height = (float)height / (float)grid_y;

	for (unsigned int iy = 0; iy < grid_y1; iy++) {

		float y = iy * segment_height - height_half;

		for (unsigned int ix = 0; ix < grid_x1; ix++) {

			float x = ix * segment_width - width_half;

			float u = (float)ix / (float)grid_x;
			float v = 1.0f - ((float)iy / (float)grid_y);

			m_vertices.push_back({
				glm::vec3{x, -y, 0.0f},
				glm::vec3{0, 0, 1},
				glm::vec2{u, v}
			});
		}
	}

	for (unsigned int iy = 0; iy < grid_y; iy++) {

		for (unsigned int ix = 0; ix < grid_x; ix++) {

			unsigned int a = ix + grid_x1 * iy;
			unsigned int b = ix + grid_x1 * (iy + 1);
			unsigned int c = (ix + 1) + grid_x1 * (iy + 1);
			unsigned int d = (ix + 1) + grid_x1 * iy;

			m_indices.insert(m_indices.end(), { a, b, d });
			m_indices.insert(m_indices.end(), { b, c, d });
		}
	}

	computeNormals();
	initialize();
}


core::Cube::Cube(int width, int height, int depth, int width_segments, int height_segments, int depth_segments)
{
	unsigned int numVertices = 0;

	buildPlane(2, 1, 0, -1, -1, depth, height, width, depth_segments, height_segments, &numVertices); // px
	buildPlane(0, 2, 1, 1, 1, width, depth, height, width_segments, depth_segments, &numVertices); // py
	buildPlane(0, 1, 2, 1, -1, width, height, depth, width_segments, height_segments, &numVertices); // pz

	buildPlane(2, 1, 0, 1, -1, depth, height, -width, depth_segments, height_segments, &numVertices); // nx
	buildPlane(0, 2, 1, 1, -1, width, depth, -height, width_segments, depth_segments, &numVertices); // ny
	buildPlane(0, 1, 2, -1, -1, width, height, -depth, width_segments, height_segments, &numVertices); // nz

	computeNormals();
	initialize();
}

void core::Cube::buildPlane(int u, int v, int w, int u_dir, int v_dir, int width, int height, int depth, int grid_x, int grid_y, unsigned int* num_vertices)
{
	float segment_width = (float)width / (float)grid_x;
	float segment_height = (float)height / (float)grid_y;

	float width_half = (float)width * 0.5f;
	float height_half = (float)height * 0.5f;
	float depth_half = (float)depth * 0.5f;

	unsigned int grid_x1 = grid_x + 1;
	unsigned int grid_y1 = grid_y + 1;

	int vertex_counter = 0;

	float position[3];
	float normal[3];

	// generate vertices, normals and uvs

	for (unsigned int iy = 0; iy < grid_y1; iy++) {

		float y = (float)iy * segment_height - height_half;

		for (unsigned int ix = 0; ix < grid_x1; ix++) {

			float x = (float)ix * segment_width - width_half;

			// set values to correct vector component

			position[u] = x * (float)u_dir;
			position[v] = y * (float)v_dir;
			position[w] = depth_half;

			// set values to correct vector component

			normal[u] = 0.0f;
			normal[v] = 0.0f;
			normal[w] = depth > 0 ? 1.0f : -1.0f;

			m_vertices.push_back({
			  {position[0], position[1], position[2]},
			  {normal[0], normal[1], normal[2]},
			  {ix / grid_x, 1 - (iy / grid_y)},
				});

			vertex_counter += 1;

		}

	}

	// indices

	// 1. you need three indices to draw a single face
	// 2. a single segment consists of two faces
	// 3. so we need to generate six (2*3) indices per segment

	for (int iy = 0; iy < grid_y; iy++) {

		for (int ix = 0; ix < grid_x; ix++) {

			unsigned int a = *num_vertices + ix + grid_x1 * iy;
			unsigned int b = *num_vertices + ix + grid_x1 * (iy + 1);
			unsigned int c = *num_vertices + (ix + 1) + grid_x1 * (iy + 1);
			unsigned int d = *num_vertices + (ix + 1) + grid_x1 * iy;

			// faces
			m_indices.insert(m_indices.end(), { a, b, d });
			m_indices.insert(m_indices.end(), { b, c, d });
		}

	}

	*num_vertices += vertex_counter;
}


core::Sphere::Sphere(float radius, int width_segments, int height_segments, float phi_start, float phi_length, float theta_start, float theta_length)
{
	width_segments = std::max(3, width_segments);
	height_segments = std::max(2, height_segments);

	float theta_end = std::min(theta_start + theta_length, (float)M_PI);

	int index = 0;
	std::vector<std::vector<unsigned int>> grid;

	// generate vertices, normals and uvs
	for (int iy = 0; iy <= height_segments; ++iy) {

		std::vector<unsigned int> vertices_row;

		float v = (float)iy / (float)height_segments;

		// special case for the poles
		float uOffset = 0.0f;

		if (iy == 0 && theta_start == 0) {

			uOffset = 0.5f / (float)width_segments;

		}
		else if (iy == height_segments && fabs(theta_end - (float)M_PI) < FLT_EPSILON) {

			uOffset = -0.5f / (float)width_segments;

		}

		for (int ix = 0; ix <= width_segments; ix++) {

			float u = (float)ix / (float)width_segments;

			// vertex
			Vertex vertex{};

			vertex.position.x = -radius * std::cos(phi_start + u * phi_length) * std::sin(theta_start + v * theta_length);
			vertex.position.y = radius * std::cos(theta_start + v * theta_length);
			vertex.position.z = radius * std::sin(phi_start + u * phi_length) * std::sin(theta_start + v * theta_length);

			// normal
			vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);

			// uv
			vertex.uv.x = u + uOffset;
			vertex.uv.y = 1 - v;


			m_vertices.push_back(vertex);
			vertices_row.push_back(index++);
		}

		grid.push_back(vertices_row);

	}

	// indices
	for (int iy = 0; iy < height_segments; iy++) {

		for (int ix = 0; ix < width_segments; ix++) {

			unsigned int a = grid[iy][ix + 1];
			unsigned int b = grid[iy][ix];
			unsigned int c = grid[iy + 1][ix];
			unsigned int d = grid[iy + 1][ix + 1];

			if (iy != 0 || theta_start > 0) m_indices.insert(m_indices.end(), { a, b, d });
			if (iy != height_segments - 1 || theta_end < M_PI) m_indices.insert(m_indices.end(), { b, c, d });

		}

	}

	computeNormals();
	initialize();
}
