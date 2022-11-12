#include "oripch.h"
#include "DummyCamera.h"
#include"Orion/Core/Application.h"
#include"Orion/Renderer/GraphicsRendering/Renderer2D.h"
#include"Orion/Core/Input.h"
#include"Orion/Core/KeyCodes.h"
#include"Orion/Core/MouseButtonCodes.h"

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

	const CameraRay& DummyCamera::Raycast(float xCoord, float yCoord)
	{
		float mouse_x = xCoord;
		float mouse_y = yCoord;

		//ORI_INFO("X: {0}, Y: {1}", mouse_x, mouse_y);
		float x = (2.0f * mouse_x) / m_ScreenSize.x - 1.0f;
		float y = 1.0f - (2.0f * mouse_y) / m_ScreenSize.y;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(m_ProjectionMatrix) * ray_clip;
		glm::vec3 ray_wor = glm::vec3((inverse(m_ViewMatrix) * glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0f)));


		m_Ray.Update(glm::normalize(ray_wor),m_Position,50.f);

		return m_Ray;
	}





	CameraRay::CameraRay(glm::vec3 direction, glm::vec3 origin, float rayLength)
		: m_Direction(direction), m_Origin(origin), m_Length(rayLength)
	{
		
	}
	void CameraRay::Update(glm::vec3 direction, glm::vec3 origin, float rayLength)
	{
		m_Origin = origin;
		m_Direction = direction;
		m_Length = rayLength;
	}
	void CameraRay::DebugDraw() const
	{
		static glm::vec3 p1{ 0.0f };
		static glm::vec3 p2{ 0.0f };


		if(Orion::Input::IsKeyPressed(ORI_KEY_R))
		{
			p1 = GetOrigin();
			p2 = GetEndPoint();

		}
		Orion::Renderer2D::DrawLine(p1, p2, glm::vec4(0.7f,0.2f,0.2f,1.0f));
	}

	glm::vec3 CameraRay::GetEndPoint() const
	{
		return m_Origin + (m_Direction * m_Length);
	}


}