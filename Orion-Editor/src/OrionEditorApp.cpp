#include<Orion.h>
#include<Orion/Core/EntryPoint.h>

#include"EditLayer2D.h"
#include"EditLayer3D.h"







namespace Orion {

	class OrionEditor : public Orion::Application
	{
	public:
		OrionEditor() : Application("Orion-Editor")
		{
			//PushLayer(new EditLayer2D());
			PushLayer(new EditLayer3D());

		}
		~OrionEditor()
		{

		}
	};

	Application* Orion::CreateApplication()
	{
		return new OrionEditor();
	}
}