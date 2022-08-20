#include<Orion.h>
#include"Orion/Core/EntryPoint.h"
#include"ExampleLayer2D.h"
#include"ExampleLayer3D.h"









class Sandbox : public Orion::Application 
{
public: 
	Sandbox() 
	{
		//PushLayer(new ExampleLayer2D());
		PushLayer(new ExampleLayer3D());

	}
	~Sandbox() 
	{
	
	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Sandbox();
}