#pragma once


#include <vector>
#include <Transform/Constants.h>

#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>


namespace core
{
	class Transform
	{
		friend class Transform;
	public:
		Transform(Transform* parent = nullptr);

		glm::vec3 position() const;
		void position(const glm::vec3& position);
		glm::quat orientation() const;
		void orientation(const glm::quat& orientation);
		glm::vec3 scale() const;
		void scale(const glm::vec3& scale);

		glm::vec3 localPosition() const { return m_position; };
		void localPosition(const glm::vec3& position) { m_position = position; updateWorldMatrix(); };
		glm::quat localOrientation() const { return m_orientation; };
		void localOrientation(const glm::quat& orientation) { m_orientation = orientation; updateWorldMatrix(); };
		glm::vec3 localScale() const { return m_scale; };
		void localScale(const glm::vec3& scale) { m_scale = scale; updateWorldMatrix(); };

		glm::vec3 front() const { return glm::normalize(-g_WorldForward * orientation()); };
		glm::vec3 right() const { return glm::normalize(glm::cross(front(), g_WorldUp)); };
		glm::vec3 up() const { return glm::normalize(glm::cross(right(), front())); };

		void setParent(Transform* parent);
		Transform* parent() const { return m_parent; };

		glm::mat4 modelMatrix() const;
		glm::mat4 worldMatrix() const { return m_worldMatrix; };

		// TODO: Implement 
		/*
		 * vec3 eulerAngles	      The rotation as Euler angles in degrees.
		 * vec3 localEulerAngles  The rotation as Euler angles in degrees relative to the parent transform's rotation.
		 * 
		 * Translate(Vector3 translation, Space relativeTo = Space.Self);                    Moves the transform in the direction and distance of translation.
		 * Translate(float x, float y, float z, Space relativeTo = Space.Self);
		 * Translate(Vector3 translation, Transform relativeTo);
		 * Translate(float x, float y, float z, Transform relativeTo);
		 * Rotate(Vector3 eulers, Space relativeTo = Space.Self);                            Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as an Euler angle and not a Quaternion.
		 * Rotate(float xAngle, float yAngle, float zAngle, Space relativeTo = Space.Self);
		 * Rotate(Vector3 axis, float angle, Space relativeTo = Space.Self);
		 * RotateAround(Vector3 point, Vector3 axis, float angle);	                         Rotates the transform about axis passing through point in world coordinates by angle degrees.
		 * LookAt(Transform target, Vector3 worldUp = Vector3.up);                           Rotates the transform so the forward vector points at /target/'s current position.
		 * LookAt(Vector3 worldPosition, Vector3 worldUp = Vector3.up);
		 * 
		 * InverseTransformDirection(Vector3 direction);          Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
		 * InverseTransformDirection(float x, float y, float z);
		 * InverseTransformPoint(Vector3 position);               Transforms position from world space to local space.
		 * InverseTransformPoint(float x, float y, float z);
		 * InverseTransformVector(Vector3 vector);                Transforms a vector from world space to local space. The opposite of Transform.TransformVector.
		 * InverseTransformVector(float x, float y, float z);
		 * 
		 * TransformDirection  Transforms direction from local space to world space.
		 * TransformPoint      Transforms position from local space to world space.
		 * TransformVector     Transforms vector from local space to world space.
		 * 
		 */

	private:
		void updateChildReferences();
		void addChildReference(Transform* child);

		void updateWorldMatrix();

		glm::vec3 m_position;     // position in local space
		glm::quat m_orientation;  // orientation in local space
		glm::vec3 m_scale;		  // scale in local space

		glm::mat4 m_worldMatrix;  // contains translation, orienatation and scaling transforms in world space

		Transform* m_parent;
		std::vector<Transform*> m_children;
	};
}
