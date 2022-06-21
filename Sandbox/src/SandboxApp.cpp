#include<Orion.h>

class Sandbox : public Orion::Application 
{
public: 
	Sandbox() 
	{

	}
	~Sandbox() 
	{

	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Sandbox();
}