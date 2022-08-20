#include "oripch.h"
#include "DummyCamera.h"
#include"Orion/Core/Application.h"
namespace Orion 
{
	DummyCamera::DummyCamera(
		const glm::vec3& position,
		bool rotation,
		const glm::mat4& viewMatrix,
		const glm::mat4& projectionMatrix,
		const glm::mat4& projectionViewMatrix 
	)
		:
		m_Position(position),
		m_ViewMatrix(viewMatrix),
		m_ProjectionMatrix(projectionMatrix),
		m_ProjectionViewMatrix(projectionViewMatrix),
		m_Rotation(rotation)
	{
		auto& app = Orion::Application::Get();
		m_ScreenSize.x = app.GetWindow().GetWidth();
		m_ScreenSize.y = app.GetWindow().GetHeight();
		m_AspectRatio = m_ScreenSize.x / m_ScreenSize.y;

	}


}