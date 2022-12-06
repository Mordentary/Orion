#include "oripch.h"
#include "CamerasController.h"

#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"
#include"Orion/Renderer/Cameras/OrthographicCamera.h"
#include"Orion/Renderer/Cameras/PerspectiveCamera.h"


namespace Orion 
{
	CamerasController CamerasController::s_CamerasController;

	void CamerasController::Initiliaze()
	{
		
	}
	void CamerasController::Update(Timestep ts)
	{
		if (!s_CamerasController.m_BlockUpdate)
		{
			auto& cam = s_CamerasController.m_CamerasStorage[s_CamerasController.m_ActiveCamera];

			cam->Update(ts);

			for (auto& [name, cam] : s_CamerasController.m_CamerasStorage)
			{
				if (name == GetActiveCameraName()) continue;
				cam->UpdateFrustum();
			}
		}
	}

	void CamerasController::OnEvent(Event& e)
	{
		s_CamerasController.OnEventImpl(e);
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
			s_CamerasController.m_ActiveCamera = name;
		}else
		{
			ORI_CORE_ASSERT(false, "Camera doesn't exist!");
		}
	}

	void CamerasController::RenderCamerasFrustum()
	{
		for (auto&[name, cam] : s_CamerasController.m_CamerasStorage)
		{
			if (name == GetActiveCameraName()) continue;

			cam->RenderFrustum();

		}

	}


	void CamerasController::AddCamera(std::string name, const Shared<DummyCamera>& camera)
	{
		s_CamerasController.m_ActiveCamera = name;
		s_CamerasController.m_CamerasStorage[name] = camera; 
	}


	void CamerasController::OnViewportResize(const glm::vec2& size)
	{
		for (auto& cam : s_CamerasController.m_CamerasStorage)
		{
			cam.second->OnViewportResize(size);
		}
	}

	const Shared<DummyCamera> CamerasController::GetActiveCamera()
	{
		auto camType = s_CamerasController.m_CamerasStorage[s_CamerasController.m_ActiveCamera]->Get();
	
		if (camType == Cameras::Perspective)
		{
			return std::dynamic_pointer_cast<PerspectiveCamera>(s_CamerasController.m_CamerasStorage[s_CamerasController.m_ActiveCamera]);
		}
		if (camType == Cameras::Orthographic)
		{
			return std::dynamic_pointer_cast<OrthographicCamera>(s_CamerasController.m_CamerasStorage[s_CamerasController.m_ActiveCamera]);
		}
		ORI_ASSERT(false, "Uknown camera type!");
	}

	const Shared<DummyCamera> CamerasController::GetCamera(std::string name)
	{
		if (IsCameraExist(name))
		{
			return s_CamerasController.m_CamerasStorage[name];
		}
		return nullptr;
	}

	bool CamerasController::IsCameraExist(std::string name)
	{
		bool isExist = s_CamerasController.m_CamerasStorage.find(name) != s_CamerasController.m_CamerasStorage.end();
		ORI_CORE_ASSERT(isExist, "No camera with this name!")
		return isExist;
	}

	bool CamerasController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		auto& cam = *m_CamerasStorage[m_ActiveCamera];


		cam.SetZoomLevel(std::max(cam.GetZoomLevel() - e.GetYOffset() * 0.25f, 0.25f));
		cam.RecalculateProjection();


		return false;
	}

	bool CamerasController::OnWindowResized(WindowResizeEvent& e)
	{
		//auto& cam = *s_CamerasController.s_CamerasStorage[s_CamerasController.s_ActiveCamera];
		if (e.GetWidth() && e.GetHeight() != 0) {
			float aspect = ((float)e.GetWidth() / (float)e.GetHeight());

			for (auto& cam : s_CamerasController.m_CamerasStorage)
			{
				cam.second->SetAspectRatio(aspect);
				cam.second->SetScreenSize(glm::vec2(e.GetWidth(), e.GetHeight()));
				cam.second->RecalculateProjection();
			}
		}
		return false;
	}

}