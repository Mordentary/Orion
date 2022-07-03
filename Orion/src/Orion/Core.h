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


#ifdef ORI_ENABLE_ASSERTS
#define ORI_ASSERT(x, ...) {if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define ORI_CORE_ASSERT(x, ...) {if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define ORI_ASSERT(x,...);
#define ORI_CORE_ASSERT(x,...);
#endif 


#define BIT(x) (1 << x)


#define ORI_BIND_EVENT_FN(fn) [this](auto& e) { return fn(e); }
