#include "oripch.h"
#include "OrthographicCamera.h"
#include"Orion/Core/MathHelper.h"
#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"
namespace Orion
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, bool rotation)
		: DummyCamera(glm::vec3(0.0f), rotation, glm::mat4(1.0f), (glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), (m_ProjectionMatrix* m_ViewMatrix))
	{
	}

	OrthographicCamera::OrthographicCamera(glm::vec3& position, bool rotation, bool pixelByPixel)
		: DummyCamera(position, rotation), m_ProjMatchesPixelByPixel(pixelByPixel)
	{
		RecalculateProjection();

	}
	

	void OrthographicCamera::Update(Orion::Timestep ts)
	{

		glm::vec3 velocity(0.0f);
		if (Input::IsKeyPressed(ORI_KEY_A))
			velocity.x -= GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_D))
			velocity.x += GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_W))
			velocity.y += GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_S))
			velocity.y -= GetTranslationSpeed() * ts;


		if (IsRotation())
		{
			if (Input::IsKeyPressed(ORI_KEY_Q))
				m_Rotation += GetSensitivity() * ts;
			if (Input::IsKeyPressed(ORI_KEY_E))
				m_Rotation -= GetSensitivity() * ts;
		}
		if (velocity != glm::vec3(0.0f)) {

			m_Position += velocity * (m_ZoomLevel * 5);
			RecalculateView();
		}

	}
	
	void OrthographicCamera::RecalculateViewProjection()
	{
		RecalculateView();
		RecalculateProjection();
	}

	inline void OrthographicCamera::RecalculateView()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_CameraRotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	inline void OrthographicCamera::RecalculateProjection()
	{
		if (m_ProjMatchesPixelByPixel)
		{
			
			m_ProjectionMatrix = glm::ortho(0.f, m_ScreenSize.x, 0.f, m_ScreenSize.y, -1.0f, 1.0f);
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
			return;
		}

		auto& [width, height] = static_cast<std::pair<float, float>>(MathHelper::ReduceFraction(m_ScreenSize.x, m_ScreenSize.y));

		m_ProjectionMatrix = glm::ortho(-width * m_ZoomLevel , width * m_ZoomLevel, -height * m_ZoomLevel, height * m_ZoomLevel, -1.0f, 1.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}

