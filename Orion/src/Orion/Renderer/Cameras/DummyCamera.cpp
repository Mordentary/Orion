#include "oripch.h"
#include "DummyCamera.h"
#include"Orion/Core/Application.h"
namespace Orion 
{
	DummyCamera::DummyCamera(
		const glm::vec3& position,
		const glm::mat4& viewMatrix,
		const glm::mat4& projectionMatrix,
		bool rotation
		)
		:
		m_Position(position),
		m_ViewMatrix(viewMatrix),
		m_ProjectionMatrix(projectionMatrix),
		m_ProjectionViewMatrix(m_ViewMatrix * m_ProjectionMatrix),
		m_Rotation(rotation)
	{
		auto& app = Orion::Application::Get();
		m_ScreenSize.x = app.GetWindow().GetWidth();
		m_ScreenSize.y = app.GetWindow().GetHeight();
		m_AspectRatio = m_ScreenSize.x / m_ScreenSize.y;

	}


}