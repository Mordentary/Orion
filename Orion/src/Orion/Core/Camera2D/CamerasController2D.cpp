#include "oripch.h"
#include "CamerasController2D.h"

#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"

namespace Orion 
{
	Scoped<CamerasController2D> CamerasController2D::s_CamerasController = std::make_unique<CamerasController2D>();

	void CamerasController2D::Initiliaze()
	{
		
	}
	void CamerasController2D::OnUpdate(Timestep ts)
	{
		auto& cam = s_CamerasController->s_CamerasStorage[s_CamerasController->s_ActiveCamera];

		glm::vec3 velocity(0.0f);
		if (Input::IsKeyPressed(ORI_KEY_A))
			velocity.x -= cam->GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_D))
			velocity.x += cam->GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_W))
			velocity.y += cam->GetTranslationSpeed() * ts;
		 if (Input::IsKeyPressed(ORI_KEY_S))
			velocity.y -= cam->GetTranslationSpeed() * ts;
			

		if (cam->IsRotation())
		{
			if (Input::IsKeyPressed(ORI_KEY_Q))
				cam->SetRotation(cam->GetRotation() + cam->GetRotationSpeed() * ts);
			if (Input::IsKeyPressed(ORI_KEY_E))
				cam->SetRotation(cam->GetRotation() - cam->GetRotationSpeed() * ts);
		}
		if (velocity != glm::vec3(0.0f)) {

			cam->SetTraslationSpeed(cam->GetZoomLevel());
			cam->SetPosition(cam->GetPosition() + velocity);
		}
	}

	void CamerasController2D::OnEvent(Event& e)
	{
		s_CamerasController->OnEventImpl(e);
	}
	void CamerasController2D::OnEventImpl(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ORI_BIND_EVENT_FN(CamerasController2D::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ORI_BIND_EVENT_FN(CamerasController2D::OnWindowResized));
	}

	void CamerasController2D::SetActiveCamera(std::string name)
	{
		if (IsCameraExist(name)) {
			s_CamerasController->s_ActiveCamera = name;
		}
	}

	void CamerasController2D::AddCamera(std::string name, const Shared<OrthographicCamera>& camera)
	{
		s_CamerasController->s_ActiveCamera = name;
		s_CamerasController->m_AspectRatio = camera->GetAspectRatio();
		s_CamerasController->s_CamerasStorage[name] = camera; 
	}

	void CamerasController2D::AddCamera(std::string name, float aspect)
	{
		s_CamerasController->s_ActiveCamera = name;
		s_CamerasController->m_AspectRatio = aspect;
		s_CamerasController->s_CamerasStorage[name] = std::make_shared<OrthographicCamera>(aspect);
	
	}

	const Shared<OrthographicCamera>& CamerasController2D::GetActiveCamera()
	{
		return s_CamerasController->s_CamerasStorage[s_CamerasController->s_ActiveCamera];
	}

	const Shared<OrthographicCamera>& CamerasController2D::GetCamera(std::string name)
	{
		if (IsCameraExist(name))
		{
			return s_CamerasController->s_CamerasStorage[name];
		}
		return nullptr;
	}

	bool CamerasController2D::IsCameraExist(std::string name)
	{
		bool isExist = s_CamerasController->s_CamerasStorage.find(name) != s_CamerasController->s_CamerasStorage.end();
		ORI_CORE_ASSERT(isExist, "No camera with this name!")
		return isExist;
	}

	bool CamerasController2D::OnMouseScrolled(MouseScrolledEvent& e)
	{
		auto& cam = s_CamerasStorage[s_ActiveCamera];
		cam->SetZoomLevel(std::max(cam->GetZoomLevel() - e.GetYOffset() * 0.25f, 0.25f));
		float zoom = cam->GetZoomLevel();
		cam->SetProjection(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom);
		return false;
	}

	bool CamerasController2D::OnWindowResized(WindowResizeEvent& e)
	{
		auto& cam = s_CamerasController->s_CamerasStorage[s_CamerasController->s_ActiveCamera];
		m_AspectRatio = ((float)e.GetWidth() / (float)e.GetHeight());
		float zoom = cam->GetZoomLevel();
		cam->SetProjection(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom);
		return false;
	}

}