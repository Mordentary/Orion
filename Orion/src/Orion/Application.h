#pragma once

#include"Core.h"

namespace Orion {

	class ORION_API Application
	{
		public: 
		Application();
		virtual ~Application();
		void Run();

	};
	//Defined on a client side
	Application* CreateApplication();
}

