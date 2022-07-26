#pragma once

#ifdef ORI_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main(int argc, char** argv) 
{
	Orion::Log::Init();
	
	ORI_PROFILE_BEGIN_SESSION("Startup", "OrionProfile-Startup.json");
	auto app = Orion::CreateApplication();
	ORI_PROFILE_END_SESSION();

	ORI_PROFILE_BEGIN_SESSION("Runtime", "OrionProfile-Runtime.json");
	app->Run();
	ORI_PROFILE_END_SESSION();

	ORI_PROFILE_BEGIN_SESSION("Shutdown", "OrionProfile-Shutdown.json");
	delete app;
	ORI_PROFILE_END_SESSION();
}
#endif 