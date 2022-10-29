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
		~ImGuiLayer() = default;

		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		virtual void OnImGuiRender(Timestep deltaTime) override;

		void Begin();
		void End();

		void SetBlockEvent(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}

