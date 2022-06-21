#pragma once

#ifdef ORI_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main(int argc, char** argv) 
{
	auto app = Orion::CreateApplication();
	app->Run();
	delete app;
}
#endif 