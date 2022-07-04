workspace "Orion"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "Orion/vendor/GLFW/include"
IncludeDir["Glad"] = "Orion/vendor/Glad/include"
IncludeDir["ImGui"] = "Orion/vendor/imgui"

include "Orion/vendor/GLFW"
include "Orion/vendor/Glad"
include "Orion/vendor/imgui"

project "Orion"
    location "Orion"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
	


	targetdir("bin/" .. outputdir ..  "/%{prj.name}")
	objdir("bin-int/" .. outputdir ..  "/%{prj.name}")

	pchheader "oripch.h"
	pchsource "Orion/src/oripch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"

	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"


		defines 
		{
			"ORI_PLATFORM_WINDOWS",
			"ORI_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "ORI_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ORI_REALESE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ORI_DIST"
		runtime "Release"
		optimize "On"




project "Sandbox"
	location"Sandbox"
	kind"ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir ..  "/%{prj.name}")
	objdir("bin-int/" .. outputdir ..  "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Orion/vendor/spdlog/include",
		"Orion/src"
	}

	links
	{
		"Orion"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"


		defines 
		{
			"ORI_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "ORI_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ORI_REALESE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ORI_DIST"
		runtime "Release"
		optimize "On"
