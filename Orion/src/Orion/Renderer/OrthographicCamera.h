#pragma once

#include"glm/glm.hpp"

namespace Orion {
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		const inline glm::vec3& GetPosition() const { return m_Position; }
		const inline float GetRotation() const { return m_Rotation; }

		void inline SetPosition(const glm::vec3& position)
		{
			m_Position = position; RecalculateViewMatrix();
		}
		void inline SetRotation(float rotation)
		{
			m_Rotation = rotation; RecalculateViewMatrix();
		}

		const glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0;
	};
}
