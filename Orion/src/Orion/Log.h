#pragma once


#include"spdlog/spdlog.h"
#include"Core.h"
#include"spdlog/fmt/ostr.h"
namespace Orion
{
	class ORION_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


//Core Log Macros
#define ORI_CORE_TRACE(...)		::Orion::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ORI_CORE_INFO(...)		::Orion::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ORI_CORE_WARN(...)		::Orion::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ORI_CORE_ERROR(...)		::Orion::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define ORI_CORE_FATAL(...)		::Orion::Log::GetCoreLogger()->fatal(__VA_ARGS__)
//Client Log Macros
#define ORI_TRACE(...)			::Orion::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ORI_INFO(...)			::Orion::Log::GetClientLogger()->info(__VA_ARGS__)
#define ORI_WARN(...)			::Orion::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ORI_ERROR(...)			::Orion::Log::GetClientLogger()->error(__VA_ARGS__)
//#define ORI_FATAL(...)			::Orion::Log::GetCoreLogger()->fatal(__VA_ARGS__)

