#pragma once

#ifdef ORI_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main(int argc, char** argv) 
{

	Orion::Log::Init();
	//ORI_CORE_TRACE("INIT_TRACE");
	//ORI_CORE_INFO("INIT_INFO");
	//ORI_CORE_WARN("INIT_WARN");
	//ORI_CORE_ERROR("INIT_ERROR");
	//ORI_ERROR("NICE ENGINE!");
	//ORI_CORE_FATAL("INIT_FATAL");
	auto app = Orion::CreateApplication();
	app->Run();
	delete app;
}
#endif 