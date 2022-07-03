#pragma once



#include"Orion/Layers/Layer.h"

#include"Orion/Events/MouseEvent.h"
#include"Orion/Events/ApplicationEvent.h"
#include"Orion/Events/KeyEvent.h"



namespace Orion {

	class ORION_API ImGuiLayer : public Layer	
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);

		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		bool OnWindowResize(WindowResizeEvent& e);  
	
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);

		

		
	private:
		float m_Time = 0.0f;
	};
}

