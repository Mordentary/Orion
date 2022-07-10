#include<Orion.h>

#include"imgui/imgui.h"

class ExampleLayer : public Orion::Layer
{
public:
	ExampleLayer() : Layer("Example"){}

	void OnUpdate() override
	{
		ORI_INFO("ExampleLayer:Update");
	}
	void OnEvent(Orion::Event& event) override
	{
		ORI_INFO("{0}", event);
	}
	virtual void OnImGuiRender() override 
	{
		ImGui::Begin("Test");
		ImGui::Text("TestText");
		ImGui::End();
	}
};

class Sandbox : public Orion::Application 
{
public: 
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Orion::ImGuiLayer());
	}
	~Sandbox() 
	{
	
	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Sandbox();
}