#include <Rendering/Mesh.h>

#include <glm/matrix.hpp>


#include <iostream>
#include <glm/gtx/string_cast.hpp>


void core::Mesh::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& cameraPosition, const core::SceneLights& sceneLights) const
{
    m_material->use();
    m_material->setMVP(m_transform.worldMatrix(), viewMatrix, projectionMatrix);  // TODO: pass world matrix, and camera view + projections
    m_material->setUniforms(cameraPosition, sceneLights);
    
    //std::cout << glm::to_string(camera.ViewMatrix()) << std::endl;

    //glUseProgram(m_material->shader);

    // TODO: We should only update when flag is set!
    //auto& trans = transform_component.transforms[transform_ndx];
    //if (trans->needs_update)
    //    trans->update_world_matrix();  // WARNING: Removing this will break rendering. We should find a good place to always initialize all world matrices at least once!!

    //set_material_uniforms(*material, camera, lights, trans->world_mat, camera.view_mat, camera.projection_mat);

    //if (material->wireframe) {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //}
    //else {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}

    //// Set blend mode
    //if (material->transparent) {
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //}
    //else {
    //    glDisable(GL_BLEND);
    //}

    // draw mesh
    m_geometry->bind();
    glDrawElements(GL_TRIANGLES, m_geometry->size(), GL_UNSIGNED_INT, 0);

    // always good practice to set everything back to defaults once configured.
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
