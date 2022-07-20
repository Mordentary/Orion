#pragma once

#include<unordered_map>
#include"Orion/Renderer/OrthographicCamera.h"
#include"Orion/Core/TimeHelper.h"

namespace Orion {


	class CamerasController2D
	{
	public:
		static void Initiliaze();
		static void OnUpdate(Timestep ts);
		static void OnEvent(Event& e);
	

		static void SetActiveCamera(std::string name);
		static void AddCamera(std::string name, const Shared<OrthographicCamera>& camera);
		static void AddCamera(std::string name, float aspect);


		static const std::string& GetActiveCameraName() { return s_CamerasController->s_ActiveCamera; }
		static const Shared<OrthographicCamera>& GetActiveCamera();
		static const Shared<OrthographicCamera>& GetCamera(std::string name);
		static bool IsCameraExist(std::string name);

		//CamerasController2D() = default;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void OnEventImpl(Event& e);
	private:
		static Scoped<CamerasController2D> s_CamerasController;
		float m_AspectRatio;
		std::string s_ActiveCamera;
		std::unordered_map<std::string, Shared<OrthographicCamera>> s_CamerasStorage;




	};
}
