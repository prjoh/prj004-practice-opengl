#include <Geometry/Line.h>


core::Line::Line(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
    : m_material(core::LineMaterial(color))
{
    m_vertices.insert(m_vertices.end(), {
        from.x, from.y, from.z,
        to.x, to.y, to.z,
    });

    glGenVertexArrays(1, &m_vao);

    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// TODO: It is really not great that we have to pass these unneeded references: cameraPosition, sceneLights
void core::Line::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition, const core::SceneLights& sceneLights) const
{
    m_material.use();
    m_material.setMVP(glm::mat4(1.0f), viewMatrix, projectionMatrix);  // TODO: pass world matrix, and camera view + projections
    m_material.setUniforms(cameraPosition, sceneLights);  // TODO: It is really not great that we have to pass these unneeded references: cameraPosition, sceneLights

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 2);
}
