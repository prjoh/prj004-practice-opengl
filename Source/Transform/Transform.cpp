#include <Transform/Transform.h>


core::Transform::Transform(Transform* parent)
    : m_position({0.0f, 0.0f, 0.0f}), 
      m_orientation({1.0f, 0.0f, 0.0f, 0.0f}), 
      m_scale({1.0f, 1.0f, 1.0f}),
      m_parent(parent)
{
    updateWorldMatrix();
}

// https://stackoverflow.com/questions/27655885/get-position-rotation-and-scale-from-matrix-in-opengl
// https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
glm::vec3 core::Transform::position() const
{
    return m_worldMatrix[3];
}

void core::Transform::position(const glm::vec3& position)
{
    glm::vec3 localPosition = position;

    if (m_parent != nullptr)
        localPosition -= m_parent->position();

    this->localPosition(localPosition);
}

glm::quat core::Transform::orientation() const
{
    glm::vec3 s = scale();
    const glm::mat3 r(
        glm::vec3(m_worldMatrix[0]) / s[0],
        glm::vec3(m_worldMatrix[1]) / s[1],
        glm::vec3(m_worldMatrix[2]) / s[2]);
    return glm::quat_cast(r);
}

void core::Transform::orientation(const glm::quat& orientation)
{
    glm::quat localOrientation = orientation;

    if (m_parent != nullptr)
        localOrientation *= glm::inverse(m_parent->orientation());

    this->localOrientation(localOrientation);
}

glm::vec3 core::Transform::scale() const
{
    return glm::vec3 {
        glm::length(glm::vec3(m_worldMatrix[0])),
        glm::length(glm::vec3(m_worldMatrix[1])),
        glm::length(glm::vec3(m_worldMatrix[2]))
    };
}

void core::Transform::scale(const glm::vec3& scale)
{
    glm::vec3 localScale = scale;

    if (m_parent != nullptr)
        localScale /= m_parent->scale();

    this->localScale(localScale);
}

glm::mat4 core::Transform::modelMatrix() const
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 rotation = glm::toMat4(m_orientation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
    return translation * rotation * scale;
}

void core::Transform::setParent(Transform* parent)
{
    auto previousParent = m_parent;

    m_parent = parent;

    // TODO: We need to call updateWorldMatrix??? I think not
    if (previousParent != nullptr) {
        previousParent->updateChildReferences();
    }

    // TODO: We need to call updateWorldMatrix??? I think not
    if (m_parent) {
        m_parent->addChildReference(this);
    }

    updateWorldMatrix();
}

// TODO: This only updates top-down!!!
void core::Transform::updateWorldMatrix()
{
    if (m_parent == nullptr) {
        m_worldMatrix = modelMatrix();
    }
    else {
        m_worldMatrix = m_parent->worldMatrix() * modelMatrix();
    }

    for (const auto& child : m_children) {
        child->updateWorldMatrix();
    }
}

void core::Transform::updateChildReferences()
{
    m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
        [&](Transform* child) { return child->parent() != this; }), m_children.end());
}

void core::Transform::addChildReference(Transform* child)
{
    m_children.push_back(child);
}
