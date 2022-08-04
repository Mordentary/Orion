#include "oripch.h"
#include "OrthographicCamera.h"


namespace Orion
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, bool rotation) 
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f), m_Position(0.0f), m_Rotation(rotation)
	{
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		//	m_AspectRatio = (right - left) / (top - bottom);
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	OrthographicCamera::OrthographicCamera(float aspect, bool rotation )
		: m_ProjectionMatrix(glm::ortho(-aspect , aspect, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f)), m_ViewMatrix(1.0f), m_Position(0.0f), m_Rotation(rotation), m_AspectRatio(aspect)
	{
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), m_CameraRotation, glm::vec3(0, 0, 1));


		m_ViewMatrix = glm::inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
		

	}

}

