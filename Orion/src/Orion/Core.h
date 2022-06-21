#pragma once
#ifdef ORI_PLATFORM_WINDOWS
	#ifdef ORI_BUILD_DLL
		#define ORION_API __declspec(dllexport)
	#else 
		#define ORION_API __declspec(dllimport)
	#endif
#else 
#error Orion only supports windows! 
#endif