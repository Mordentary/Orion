#pragma once


#include "Orion/Core/Core.h"
#include "Orion/Events/Event.h"
#include "Orion/Core/TimeHelper.h"
namespace Orion {
	class DummyCamera;
	class Layer
	{

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Init()		{}
		virtual void Update(Timestep deltaTime) {}
		virtual void Render() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender(Timestep deltaTime) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}

