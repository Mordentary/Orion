#pragma once

#include<unordered_map>
#include"Orion/Renderer/Cameras/DummyCamera.h"
#include"Orion/Core/TimeHelper.h"
#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/MouseEvent.h"
namespace Orion {


	class CamerasController
	{
	public:
		static void Initiliaze();
		static void OnUpdate(Timestep ts);
		static void OnEvent(Event& e);
	

		static void SetActiveCamera(std::string name);
		static void AddCamera(std::string name, const Shared<DummyCamera>& camera);


		static const std::string& GetActiveCameraName() { return s_CamerasController.s_ActiveCamera; }
		static const Shared<DummyCamera> GetActiveCamera();

		static const Shared<DummyCamera>& GetCamera(std::string name);
		static bool IsCameraExist(std::string name);

		//CamerasController() = default;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void OnEventImpl(Event& e);
	private:
		static CamerasController s_CamerasController;
		std::string s_ActiveCamera;
		std::unordered_map<std::string, Shared<DummyCamera>> s_CamerasStorage;




	};
}
