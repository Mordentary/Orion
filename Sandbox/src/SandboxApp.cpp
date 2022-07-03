#include<Orion.h>


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
};

class Sandbox : public Orion::Application 
{
public: 
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Orion::ImGuiLayer());
	}
	~Sandbox() 
	{
	
	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Sandbox();
}