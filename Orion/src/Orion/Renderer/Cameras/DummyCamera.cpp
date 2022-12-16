#include "oripch.h"
#include "DummyCamera.h"
#include"Orion/Core/Application.h"
#include"Orion/Renderer/GraphicsRendering/Renderer2D.h"
#include"Orion/Core/Input.h"
#include"Orion/Core/KeyCodes.h"
#include"Orion/Core/MouseButtonCodes.h"
#include "Orion/Renderer/GraphicsObjects/Model.h"
#include "Orion/Core/AdvanceCamerasFamily/CamerasController.h"

#include <glm/gtx/string_cast.hpp>


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

	const Ray& DummyCamera::Raycast(float xCoord, float yCoord)
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
		//std::cout << glm::to_string(ray_wor) << "\n";

		m_Ray.Update(glm::normalize(ray_wor),m_Position,10.f);

		return m_Ray;
	}

	void DummyCamera::DragObjectAlongCameraPlane(Shared<Model>&model)
	{
		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1) && !Orion::CamerasController::GetBlockUpdate())
		{	


			float t = Orion::CollisionHandler::PlaneVsRay(Plane{ -m_CameraForward, model->GetPosition() }, m_Ray);

			static glm::vec3 pointIntersection{};
			if (t != -1) 
			{
				pointIntersection = m_Ray.GetOrigin() + (m_Ray.GetDirection() * t);
			}

			model->SetPosition(pointIntersection);

		}
	}
}