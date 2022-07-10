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

		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;


		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

