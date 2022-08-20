#include "oripch.h"
#include "CamerasController.h"

#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"
#include"Orion/Renderer/Cameras/OrthographicCamera.h"
#include"Orion/Renderer/Cameras/PerspectiveCamera.h"


namespace Orion 
{
	Scoped<CamerasController> CamerasController::s_CamerasController = Orion::CreateScoped<CamerasController>();

	void CamerasController::Initiliaze()
	{
		
	}
	void CamerasController::OnUpdate(Timestep ts)
	{
		auto& cam = s_CamerasController->s_CamerasStorage[s_CamerasController->s_ActiveCamera];

		cam->Update(ts);
	}

	void CamerasController::OnEvent(Event& e)
	{
		s_CamerasController->OnEventImpl(e);
	}
	void CamerasController::OnEventImpl(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ORI_BIND_EVENT_FN(CamerasController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(ORI_BIND_EVENT_FN(CamerasController::OnWindowResized));
	}

	void CamerasController::SetActiveCamera(std::string name)
	{
		if (IsCameraExist(name)) {
			s_CamerasController->s_ActiveCamera = name;
		}
	}

	void CamerasController::AddCamera(std::string name, const Shared<DummyCamera>& camera)
	{
		s_CamerasController->s_ActiveCamera = name;
		s_CamerasController->s_CamerasStorage[name] = camera; 
	}


	const Shared<DummyCamera>& CamerasController::GetActiveCamera()
	{
		auto& activeCam = s_CamerasController->s_CamerasStorage[s_CamerasController->s_ActiveCamera];
		auto& result =	std::dynamic_pointer_cast<Orion::PerspectiveCamera>(activeCam);

		if (result)
		{
			return result;
		}
		else 
		{
			return std::dynamic_pointer_cast<Orion::OrthographicCamera>(activeCam);
		}
	
	}

	const Shared<DummyCamera>& CamerasController::GetCamera(std::string name)
	{
		if (IsCameraExist(name))
		{
			return s_CamerasController->s_CamerasStorage[name];
		}
		return nullptr;
	}

	bool CamerasController::IsCameraExist(std::string name)
	{
		bool isExist = s_CamerasController->s_CamerasStorage.find(name) != s_CamerasController->s_CamerasStorage.end();
		ORI_CORE_ASSERT(isExist, "No camera with this name!")
		return isExist;
	}

	bool CamerasController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		auto& cam = s_CamerasStorage[s_ActiveCamera];

		cam->SetZoomLevel(std::max(cam->GetZoomLevel() - e.GetYOffset() * 0.25f, 0.25f));
		cam->RecalculateProjection();
		return false;
	}

	bool CamerasController::OnWindowResized(WindowResizeEvent& e)
	{
		auto& cam = s_CamerasController->s_CamerasStorage[s_CamerasController->s_ActiveCamera];
		float aspect = ((float)e.GetWidth() / (float)e.GetHeight());

		for (auto& cam : s_CamerasController->s_CamerasStorage)
		{
			cam.second->SetAspectRatio(aspect);
			cam.second->SetScreenSize(glm::vec2(e.GetWidth() ,e.GetHeight()));
		}
		cam->RecalculateProjection();

		return false;
	}

}